/*License Notification
Fab@Home operates under the BSD Open Source License

Copyright (c) 2006, Hod Lipson and Evan Malone (evan.malone@cornell.edu) All rights reserved. 

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met: 

Redistributions of source code must retain the above copyright notice, 
this list of conditions and the following disclaimer. 
Redistributions in binary form must reproduce the above copyright notice, 
this list of conditions and the following disclaimer in the documentation and/or 
other materials provided with the distribution. 
Neither the name of the Fab@Home Project nor the names of its contributors may be 
used to endorse or promote products derived from this software without specific 
prior written permission. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// FabAtHomeDoc.cpp : implementation of the CFabAtHomeDoc class
//

#include "stdafx.h"
#include ".\fabathomedoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <direct.h>



extern CFabAtHomeApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CFabAtHomeDoc

IMPLEMENT_DYNCREATE(CFabAtHomeDoc, CDocument)

BEGIN_MESSAGE_MAP(CFabAtHomeDoc, CDocument)
	//{{AFX_MSG_MAP(CFabAtHomeDoc)
	ON_COMMAND(ID_CHUNK_IMPORTGEOMETRY, OnChunkImportgeometry)
	ON_UPDATE_COMMAND_UI(ID_MODEL_MATERIAL, OnUpdateModelMaterial)
	ON_COMMAND(ID_FABRICATE_FABRICATE, OnFabricateFabricate)
	ON_UPDATE_COMMAND_UI(ID_FABRICATE_FABRICATE, OnUpdateFabricateFabricate)
	ON_COMMAND(ID_MODEL_DELETE, OnModelDelete)
	ON_UPDATE_COMMAND_UI(ID_MODEL_DELETE, OnUpdateModelDelete)
	ON_COMMAND(ID_EDIT_SELECTALL, OnEditSelectall)
	ON_COMMAND(ID_MODEL_MOVETOORIGIN, OnModelMovetoorigin)
	ON_UPDATE_COMMAND_UI(ID_MODEL_MOVETOORIGIN, OnUpdateModelMovetoorigin)
	ON_COMMAND(ID_MODEL_ROTATE, OnModelRotate)
	ON_UPDATE_COMMAND_UI(ID_MODEL_ROTATE, OnUpdateModelRotate)
	ON_COMMAND(ID_MODEL_TRANSLATE, OnModelTranslate)
	ON_UPDATE_COMMAND_UI(ID_MODEL_TRANSLATE, OnUpdateModelTranslate)
	ON_COMMAND(ID_MODEL_SCALE, OnModelScale)
	ON_UPDATE_COMMAND_UI(ID_MODEL_SCALE, OnUpdateModelScale)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_MODEL_PROPERTIES, OnModelProperties)
	ON_UPDATE_COMMAND_UI(ID_MODEL_PROPERTIES, OnUpdateModelProperties)
	ON_COMMAND(ID_FABRICATION_SLICE, OnFabricationSlice)
	ON_UPDATE_COMMAND_UI(ID_FABRICATION_SLICE, OnUpdateFabricationSlice)
	ON_COMMAND(ID_FABRICATION_PAUSEPRINTING, OnFabricationPauseprinting)
	ON_UPDATE_COMMAND_UI(ID_FABRICATION_PAUSEPRINTING, OnUpdateFabricationPauseprinting)
	ON_COMMAND(ID_FABRICATION_PAUSEATTOOLCHANGE, OnFabricationPauseattoolchange)
	ON_UPDATE_COMMAND_UI(ID_FABRICATION_PAUSEATTOOLCHANGE, OnUpdateFabricationPauseattoolchange)
	ON_COMMAND(ID_FABRICATION_AUTORESUME, OnFabricationAutoresume)
	ON_UPDATE_COMMAND_UI(ID_FABRICATION_AUTORESUME, OnUpdateFabricationAutoresume)
	ON_UPDATE_COMMAND_UI(ID_CHUNK_IMPORTGEOMETRY, &CFabAtHomeDoc::OnUpdateChunkImportgeometry)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFabAtHomeDoc construction/destruction
// --------------------------------------------------------------------------
CFabAtHomeDoc::CFabAtHomeDoc()
// --------------------------------------------------------------------------
{

}

// --------------------------------------------------------------------------
CFabAtHomeDoc::~CFabAtHomeDoc()
// --------------------------------------------------------------------------
{
	return;
}

// --------------------------------------------------------------------------
BOOL CFabAtHomeDoc::OnNewDocument()
// --------------------------------------------------------------------------
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CFabAtHomeDoc serialization

// --------------------------------------------------------------------------
void CFabAtHomeDoc::Serialize(CArchive& ar)
// --------------------------------------------------------------------------
{
	model.Serialize(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CFabAtHomeDoc diagnostics

#ifdef _DEBUG
// --------------------------------------------------------------------------
void CFabAtHomeDoc::AssertValid() const
// --------------------------------------------------------------------------
{
	CDocument::AssertValid();
}

// --------------------------------------------------------------------------
void CFabAtHomeDoc::Dump(CDumpContext& dc) const
// --------------------------------------------------------------------------
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFabAtHomeDoc commands

// --------------------------------------------------------------------------
void CFabAtHomeDoc::OnChunkImportgeometry() 
// --------------------------------------------------------------------------
{// import an stl file or vector data file
	CFabAtHomeApp *pApp = (CFabAtHomeApp*)AfxGetApp();

	if (!pApp->printer.EnsureCurrentTool(true))
	{
		pApp->Log("Please ensure that a valid tool configuration file is loaded and mounted before connect the hardware.");
		return;
	}

	CString startDir;
	startDir.Format("%s\\*.stl",pApp->modelsDir);
	CFileDialog dlg(TRUE, "stl", (LPCTSTR)startDir, OFN_ALLOWMULTISELECT,
					"Geometry File (*.stl)|*.stl|Vector File (*.vct, *.txt)|*.vct;*.txt|All Files (*.*)|*.*||", 0);
	
	if (dlg.DoModal() == IDOK)
	{
		//get the dialog path
		POSITION pos( dlg.GetStartPosition() );
		while( pos )
		{
			CString dlgPathName( dlg.GetNextPathName(pos) );

			pApp->modelsDir.Format("%s",dlgPathName);

			BeginWaitCursor();
			CChunk newchunk;

			//peel the name out of the path
			int idx;
			CString chunkName = pApp->modelsDir;
			while((idx=chunkName.Find('\\')) != -1)
			{
				chunkName = chunkName.Right(chunkName.GetLength()-idx-1);
			}
			//make sure it is not a duplicate name
			CSingleLock modelLock(&model.m_mutAccess);
			modelLock.Lock();
			for(int i = 0; i < model.chunk.GetSize(); i++)
			{
				int nameCnt = 1;
				//if a duplicate is found, append a number
				while(chunkName.Compare(model.chunk[i].GetName())==0)
				{
					CString tmp = chunkName;
					chunkName.Format("%s-%d",tmp,nameCnt);
					nameCnt++;
				}
			}
			//check the type of file being imported
			bool ok = false;
			if((chunkName.Find(".vct") >= 0) || (chunkName.Find(".VCT") >=0) ||
				(chunkName.Find(".txt") >= 0) || (chunkName.Find(".TXT") >=0))
			{
				//import a vector file
				ok = newchunk.geometry.LoadVCT(pApp->modelsDir);
				newchunk.SetType(CHUNKTYPE_VCT);
			}
			else if ((chunkName.Find(".stl") >= 0) || (chunkName.Find(".STL") >= 0))
			{
				//import an stl file
				ok = newchunk.geometry.LoadSTL(pApp->modelsDir);
				newchunk.SetType(CHUNKTYPE_STL);
			}
			else
			{
				((CFabAtHomeApp *)AfxGetApp())->Log("Cannot import files of this type");
				return;
			}
			EndWaitCursor();
			if (!ok) {
				((CFabAtHomeApp *)AfxGetApp())->Log("Cannot open file for reading "+pApp->modelsDir);
				return;
			}
			
			//now set the name of the chunk
			newchunk.SetName(chunkName);
			newchunk.materialname = "Undefined";
			newchunk.toolname = "Undefined";
			newchunk.color = CVec(0.5,0.5,0.5);
			newchunk.alpha = 1;
			newchunk.id = model.GetNewId();
			model.chunk.Add(newchunk);
		}
		model.FocusCentroid();
		model.Flush();
		model.InvalidateBuild();
		model.InvalidateProcess();
		SetModifiedFlag();
		UpdateAllViews(0);
		pApp->printer.SetModelPtr(&model);
		model.SelectAll();
		CVec pmax,pmin;
		model.ComputeBoundingBox(pmax,pmin);
		if(pmin.z < 0)
		{
			CString msg;
			msg.Format("Warning - model extends below table surface!\nPlease move to origin or translate %0.3fmm in z direction.", -(min(pmin.z,pmax.z)));
			AfxMessageBox(msg,MB_OK);
		}
	}
		
}

// --------------------------------------------------------------------------
void CFabAtHomeDoc::OnUpdateChunkImportgeometry(CCmdUI *pCmdUI)
// --------------------------------------------------------------------------
{
	pCmdUI->Enable(!model.IsSlicing());
}


// --------------------------------------------------------------------------
void CFabAtHomeDoc::OnUpdateModelMaterial(CCmdUI* pCmdUI) 
// --------------------------------------------------------------------------
{
	int n = model.CountChunksSelected();
	pCmdUI->Enable(n == 1 && !model.IsSlicing());
}


// --------------------------------------------------------------------------
void CFabAtHomeDoc::OnFabricateFabricate() 
// --------------------------------------------------------------------------
{
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	if (!pApp->printer.m_bOriginSet) 
	{
			((CFabAtHomeApp *)AfxGetApp())->Log("Print origin must be defined using Jog Carriage dialog.");
		return;
	}

	if (model.fab.IsFabricating()) 
	{ 
		//tell the fab thread of the abort
		//pApp->printer.AbortBuild();
		model.fab.RequestAbort();
		//clear the E-stop
		//pApp->printer.ClearEStop();
	} 
	else
	{
		model.FlushTraces();
		AfxBeginThread(StartFabrication, &model);
	}
}

// --------------------------------------------------------------------------
UINT CFabAtHomeDoc::StartFabrication(LPVOID pParam)
// --------------------------------------------------------------------------
{ // this is a controlling function called by a separate thread

	CModel* pModel = (CModel*) pParam;
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();

	//call the fabrication function
	TRACE("In Fabrication Thread\n");
	pModel->fab.Fabricate(pModel);
	//no longer fabricating
	//clear any aborts
	pModel->fab.ClearAbort();
	//tell the printer to clean up any job related variables or details
	pApp->printer.CleanupBuild(); 
	TRACE("Exiting Fabrication Thread\n");
	return 0; //ok
}

// --------------------------------------------------------------------------
void CFabAtHomeDoc::OnUpdateFabricateFabricate(CCmdUI* pCmdUI) 
// --------------------------------------------------------------------------
{
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	pCmdUI->Enable( (model.IsBuildReady() && pApp->printer.IsDefined())
		&& (pApp->printer.bEmulateHardware || pApp->printer.bHardwareInitialized)
		&& (pApp->printer.IsOriginSet() && pApp->printer.IsHomed() && pApp->printer.IsSafeSet()));
    
	if (model.fab.IsFabricating()) 
	{ // stop
		if (model.fab.IsAbortRequested()) 
		{
			pCmdUI->SetText("Stopping...");
		} 
		else 
		{
			pCmdUI->SetText("Stop Printing\tCtrl+P");
		}
	} 
	else 
	{
		pCmdUI->SetText("Start Printing\tCtrl+P");
	}
	
}


// --------------------------------------------------------------------------
BOOL CFabAtHomeDoc::CanCloseFrame(CFrameWnd* pFrame) 
// --------------------------------------------------------------------------
{
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	if (model.fab.IsFabricating()) {
		if(IDNO == AfxMessageBox("Fabrication process is running.\nTerminate application?", MB_YESNO))
		{
			return false;
		}
		else
		{
			model.fab.RequestAbort();

		}
	}
	//TODO: make saveable jobs
	return true; //CDocument::CanCloseFrame(pFrame);
}

// --------------------------------------------------------------------------
void CFabAtHomeDoc::OnModelDelete() 
// --------------------------------------------------------------------------
{
	int cnt = 0;

	for (int i=0; i<model.chunk.GetSize(); i++) {
		if (model.chunk[i].IsSelected()) {
			model.chunk.RemoveAt(i);
			i--;
			cnt++;
		}
	}
	
	if (cnt > 0) {
		model.FocusCentroid();
		model.fab.layer.RemoveAll();
		model.fab.SetCurrentLayer(0);
		SetModifiedFlag();
	}
	model.Flush();

	UpdateAllViews(0);

}

// --------------------------------------------------------------------------
void CFabAtHomeDoc::OnUpdateModelDelete(CCmdUI* pCmdUI) 
// --------------------------------------------------------------------------
{
	int n = model.CountChunksSelected();
	pCmdUI->Enable(n > 0 && !model.IsSlicing());
	
}

// --------------------------------------------------------------------------
void CFabAtHomeDoc::OnEditSelectall() 
// --------------------------------------------------------------------------
{
	model.SelectAll(true);	
	UpdateAllViews(0);
}

// --------------------------------------------------------------------------
void CFabAtHomeDoc::OnModelMovetoorigin() 
// --------------------------------------------------------------------------
{

	model.MoveToOrigin();
	model.FocusCentroid();
	model.fab.Flush();
	model.InvalidateBuild();
	SetModifiedFlag();
	UpdateAllViews(0);

}

// --------------------------------------------------------------------------
void CFabAtHomeDoc::OnUpdateModelMovetoorigin(CCmdUI* pCmdUI) 
// --------------------------------------------------------------------------
{
	int n = model.CountChunksSelected();
	pCmdUI->Enable(n > 0 && !model.IsSlicing());
	
}

// --------------------------------------------------------------------------
void CFabAtHomeDoc::ShowAllLayers()
// --------------------------------------------------------------------------
{

	POSITION pos = GetFirstViewPosition();
	while (pos != NULL) {
		CFabAtHomeView* pView = (CFabAtHomeView*) GetNextView(pos);
		pView->firstlayer = 0;
		pView->lastlayer = model.fab.layer.GetSize()-1;
	}

}

// --------------------------------------------------------------------------
void CFabAtHomeDoc::OnModelRotate() 
// --------------------------------------------------------------------------
{
	CString action;
	action.Format("Rotate Model");
	CAxesDlg dlg(NULL, action);

	dlg.m_xax = "0.0";
	dlg.m_yax = "0.0";
	dlg.m_zax = "0.0";

	if (dlg.DoModal() == IDOK)
	{
		double pi = atan(1.0)*4;
		for (int i=0; i<model.chunk.GetSize(); i++)
		{
			if (model.chunk[i].IsSelected())
			{
				if (atof(dlg.m_xax) != 0)
					model.chunk[i].geometry.RotX(atof(dlg.m_xax)*pi/180);
				if (atof(dlg.m_yax) != 0)
					model.chunk[i].geometry.RotY(atof(dlg.m_yax)*pi/180);
				if (atof(dlg.m_zax) != 0)
					model.chunk[i].geometry.RotZ(atof(dlg.m_zax)*pi/180);
			}
		}
		
		model.Flush();
		model.MoveToOrigin();
		SetModifiedFlag();
		UpdateAllViews(0);
	}
	
}

// --------------------------------------------------------------------------
void CFabAtHomeDoc::OnUpdateModelRotate(CCmdUI* pCmdUI) 
// --------------------------------------------------------------------------
{
	int n = model.CountChunksSelected();
	pCmdUI->Enable(n > 0 && !model.IsSlicing());
	
}

// --------------------------------------------------------------------------
void CFabAtHomeDoc::OnModelTranslate() 
// --------------------------------------------------------------------------
{
	CString action;
	action.Format("Translate Model");
	CAxesDlg dlg(NULL, action);

	dlg.m_xax = "0.0";
	dlg.m_yax = "0.0";
	dlg.m_zax = "0.0";

	if (dlg.DoModal() == IDOK)
	{
		for (int i=0; i<model.chunk.GetSize(); i++)
		{
			if (model.chunk[i].IsSelected())
			{
				model.chunk[i].geometry.Translate(CVec(atof(dlg.m_xax),atof(dlg.m_yax),atof(dlg.m_zax)));
			}
		}
		model.Flush();
		SetModifiedFlag();
		UpdateAllViews(0);
	}
	
}

// --------------------------------------------------------------------------
void CFabAtHomeDoc::OnUpdateModelTranslate(CCmdUI* pCmdUI) 
// --------------------------------------------------------------------------
{
	int n = model.CountChunksSelected();
	pCmdUI->Enable(n > 0 && !model.IsSlicing());
	
}

// --------------------------------------------------------------------------
void CFabAtHomeDoc::OnModelScale() 
// --------------------------------------------------------------------------
{
	CString action;
	action.Format("Rescale Model");
	CAxesDlg dlg(NULL, action);

	dlg.m_xax = "1.0";
	dlg.m_yax = "1.0";
	dlg.m_zax = "1.0";

	if (dlg.DoModal() == IDOK) 
	{
		//check for zero scale factor
		CVec s(atof(dlg.m_xax),atof(dlg.m_yax),atof(dlg.m_zax));
		if(s.x==0 || s.y==0 || s.z==0)
		{
			((CFabAtHomeApp *)AfxGetApp())->Log("Scale factor of zero is not allowed.");
			return;
		}
		double pi = atan(1.0)*4;
		for (int i=0; i<model.chunk.GetSize(); i++)
		{
			if (model.chunk[i].IsSelected())
			{
				model.chunk[i].geometry.Scale(s);
			}
		}
		
		model.Flush();
		model.MoveToOrigin();
		SetModifiedFlag();
		UpdateAllViews(0);
	}
	
}

// --------------------------------------------------------------------------
void CFabAtHomeDoc::OnUpdateModelScale(CCmdUI* pCmdUI) 
// --------------------------------------------------------------------------
{
	int n = model.CountChunksSelected();
	pCmdUI->Enable(n > 0 && !model.IsSlicing());
	
}

//---------------------------------------------------------------------------
void CFabAtHomeDoc::RedrawViewsNow()
//---------------------------------------------------------------------------
{ // update all views now
	
	POSITION pos = GetFirstViewPosition();

	while (pos != NULL) {
		CFabAtHomeView* pView = (CFabAtHomeView*) GetNextView(pos);
		pView->RedrawWindow();
	}
	
}

//---------------------------------------------------------------------------
void CFabAtHomeDoc::OnModelProperties()
//---------------------------------------------------------------------------
{// edit material/tool properties

	EditChunkProperties();

	UpdateAllViews(0);
}

//---------------------------------------------------------------------------
void CFabAtHomeDoc::OnUpdateModelProperties(CCmdUI *pCmdUI)
//---------------------------------------------------------------------------
{
	pCmdUI->Enable(model.chunk.GetSize()>0 && !model.IsSlicing());
}

//---------------------------------------------------------------------------
void CFabAtHomeDoc::EditChunkProperties()
//---------------------------------------------------------------------------
{
	CChunkDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		model.Flush();
	}
}

//---------------------------------------------------------------------------
UINT CFabAtHomeDoc::StartSlicing( LPVOID pParam )
//---------------------------------------------------------------------------
{
	//worker function for slicing
	CPrinter *pPrinter = (CPrinter *)pParam;
	pPrinter->GetModel().Slice(pPrinter->GetMaxSubsegLen());
	return 0;
}


//---------------------------------------------------------------------------
void CFabAtHomeDoc::OnFabricationSlice()
//---------------------------------------------------------------------------
{

	if(model.IsSlicing())
	{
		if(IDYES==AfxMessageBox("Cancel Job Planning?",MB_YESNO))
			model.CancelSlicing();
	}
	else
	{
		CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
		
		CSingleLock locker(&model.m_mutAccess);
		locker.Lock();
		model.Flush();
		locker.Unlock();
		AfxBeginThread(StartSlicing, &(pApp->printer));
	}
	UpdateAllViews(0);
}

//---------------------------------------------------------------------------
void CFabAtHomeDoc::OnUpdateFabricationSlice(CCmdUI *pCmdUI)
//---------------------------------------------------------------------------
{
	pCmdUI->Enable(model.IsProcessReady() && !model.fab.IsFabricating());
	if(model.IsSlicing())
		pCmdUI->SetText("Cancel Planning");
	else
		pCmdUI->SetText("Plan Process");
}

//---------------------------------------------------------------------------
void CFabAtHomeDoc::OnFabricationPauseprinting()
//---------------------------------------------------------------------------
{
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	if(model.fab.IsPaused() ||
		(pApp->printer.bHardwareInitialized && pApp->printer.IsPaused()) )
	{
		//pApp->printer.ResumeBuild();
		model.fab.RequestResume();
	}
	else
	{
		//pApp->printer.PauseBuild();
		model.fab.RequestPause();
	}
}

//---------------------------------------------------------------------------
void CFabAtHomeDoc::OnUpdateFabricationPauseprinting(CCmdUI *pCmdUI)
//---------------------------------------------------------------------------
{
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	pCmdUI->Enable(model.fab.IsFabricating());
	if(model.fab.PauseRequested())
		pCmdUI->SetText("Resume Printing\tShift+B");
	else
		pCmdUI->SetText("Pause Printing\tShift+B");
}

//---------------------------------------------------------------------------
void CFabAtHomeDoc::OnFabricationPauseattoolchange()
//---------------------------------------------------------------------------
{
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	pApp->printer.SetPauseAtToolChange(!pApp->printer.GetPauseAtToolChange());
}

//---------------------------------------------------------------------------
void CFabAtHomeDoc::OnUpdateFabricationPauseattoolchange(CCmdUI *pCmdUI)
//---------------------------------------------------------------------------
{
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	pCmdUI->Enable( (pApp->printer.GetMaxTools()>1) && model.IsBuildReady() );
	if(pApp->printer.GetPauseAtToolChange())
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

//---------------------------------------------------------------------------
void CFabAtHomeDoc::OnFabricationAutoresume()
//---------------------------------------------------------------------------
{
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	pApp->printer.SetAutoresume(!pApp->printer.GetAutoresume());
}

//---------------------------------------------------------------------------
void CFabAtHomeDoc::OnUpdateFabricationAutoresume(CCmdUI *pCmdUI)
//---------------------------------------------------------------------------
{
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	pCmdUI->Enable(model.IsBuildReady());
	if(pApp->printer.GetAutoresume())
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}