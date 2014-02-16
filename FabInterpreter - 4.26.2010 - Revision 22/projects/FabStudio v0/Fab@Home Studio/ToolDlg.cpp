/*License Notification
Fab@Home operates under the BSD Open Source License

Copyright (c) 2008, Paulo Inforçatti Neto (paulo.inforcatti@cenpra.gov.br) and Renan Rodrigues dos Santos
Renato Archer Research Center (CenPRA), Campinas, Brazil
All rights reserved. 

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

// ToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FabAtHome.h"
#include "ToolDlg.h"



// --------------------------------------------------------------------------
// CToolDlgParameters dialog
// --------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CToolDlgParameters, CPropertyPage)

// --------------------------------------------------------------------------
CToolDlgParameters::CToolDlgParameters()
	: CPropertyPage(CToolDlgParameters::IDD)
// --------------------------------------------------------------------------
{

	m_pToolTip = NULL;

}

// --------------------------------------------------------------------------
CToolDlgParameters::~CToolDlgParameters()
// --------------------------------------------------------------------------
{
	delete m_pToolTip;
}

// --------------------------------------------------------------------------
void CToolDlgParameters::DoDataExchange(CDataExchange* pDX)
// --------------------------------------------------------------------------
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BACKFILL, m_BACKFILL);
	DDX_Control(pDX, IDC_BARRELCOLOR, m_BARRELCOLOR);
	DDX_Control(pDX, IDC_BARRELDIAM, m_BARRELDIAM);
	DDX_Control(pDX, IDC_BARRELGEOM, m_BARRELGEOM);
	DDX_Control(pDX, IDC_BARRELOFFSET, m_BARRELOFFSET);
	DDX_Control(pDX, IDC_CLEARANCE, m_CLEARANCE);
	DDX_Control(pDX, IDC_DEPOSITIONRATE, m_DEPOSITIONRATE);
	DDX_Control(pDX, IDC_DESCRIPTION, m_DESCRIPTION);
	DDX_Control(pDX, IDC_DIRECTION, m_DIRECTION);
	DDX_Control(pDX, IDC_INCREMENT, m_INCREMENT);
	DDX_Control(pDX, IDC_MINPATHLENGTH, m_MINPATHLENGTH);
	DDX_Control(pDX, IDC_MOTOR, m_MOTOR);
	DDX_Control(pDX, IDC_PATHHEIGHT, m_PATHHEIGHT);
	DDX_Control(pDX, IDC_PATHSPEED, m_PATHSPEED);
	DDX_Control(pDX, IDC_PATHWIDTH, m_PATHWIDTH);
	DDX_Control(pDX, IDC_PAUSEPATHS, m_PAUSEPATHS);
	DDX_Control(pDX, IDC_PLUNGERCOLOR, m_PLUNGERCOLOR);
	DDX_Control(pDX, IDC_PLUNGERGEOM, m_PLUNGERGEOM);
	DDX_Control(pDX, IDC_PLUNGEROFFSET, m_PLUNGEROFFSET);
	DDX_Control(pDX, IDC_PUSHOUT, m_PUSHOUT);
	DDX_Control(pDX, IDC_RANGE, m_RANGE);
	DDX_Control(pDX, IDC_SUCKBACK, m_SUCKBACK);
	DDX_Control(pDX, IDC_SUCKBACKDELAY, m_SUCKBACKDELAY);
	DDX_Control(pDX, IDC_TIPCOLOR, m_TIPCOLOR);
	DDX_Control(pDX, IDC_TIPDIAM, m_TIPDIAM);
	DDX_Control(pDX, IDC_TIPGEOM, m_TIPGEOM);
	DDX_Control(pDX, IDC_TIPOFFSET, m_TIPOFFSET);
	DDX_Control(pDX, IDC_TIPPOINT, m_TIPPOINT);
	DDX_Control(pDX, IDC_TOOLCOLOR, m_TOOLCOLOR);
	DDX_Control(pDX, IDC_TOOLGEOM, m_TOOLGEOM);
	DDX_Control(pDX, IDC_TOOLOFFSET, m_TOOLOFFSET);
	DDX_Control(pDX, IDC_VOLUME, m_VOLUME);
}

// --------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CToolDlgParameters, CPropertyPage)
END_MESSAGE_MAP()

// --------------------------------------------------------------------------
BOOL CToolDlgParameters::OnInitDialog()
// --------------------------------------------------------------------------
{
	BOOL bResult = CPropertyPage::OnInitDialog();

	//Set up the tooltip
	m_pToolTip = new CToolTipCtrl;
	if(!m_pToolTip->Create(this))
	{
		TRACE("Unable To create ToolTip\n");
		return bResult;
	}

	//m_pToolTip->SetMaxTipWidth(1024*8);
	m_pToolTip->AddTool(&m_BACKFILL,"[mm] maximum depth allowed for backfill deposition (NOT YET IMPLEMENTED)");
	m_pToolTip->AddTool(&m_BARRELCOLOR,"R G B Alpha in range 0-1 (alpha=transparency)");
	m_pToolTip->AddTool(&m_BARRELDIAM,"[mm] internal diameter of syringe barrel");
	m_pToolTip->AddTool(&m_BARRELGEOM,"name of the stl file describing the syringe barrel geometry");
	m_pToolTip->AddTool(&m_BARRELOFFSET,"dx dy dz graphics offset of syringe barrel");
	m_pToolTip->AddTool(&m_CLEARANCE,"[mm] clearance between tip and last layer when traversing");
	m_pToolTip->AddTool(&m_DEPOSITIONRATE,"(mm plunger motion per mm of path length)");
	m_pToolTip->AddTool(&m_DESCRIPTION,"Description of the tool");
	m_pToolTip->AddTool(&m_DIRECTION,"orientation of graphical display of motion of plunger");
	m_pToolTip->AddTool(&m_INCREMENT,"[mm] default distance to move plunger in manual jog");
	//m_pToolTip->AddTool(&m_MINPATHLENGTH,"");
	m_pToolTip->AddTool(&m_MOTOR,"amplifier axis #, axis direction(+/-1), mm per (1/8) step");
	m_pToolTip->AddTool(&m_PATHHEIGHT,"[mm] height of layers of material deposited");
	m_pToolTip->AddTool(&m_PATHSPEED,"[mm/s] speed along paths when depositing material");
	m_pToolTip->AddTool(&m_PATHWIDTH,"[mm] width of stream of material deposited");
	m_pToolTip->AddTool(&m_PAUSEPATHS,"number of paths after which to trigger an automatic pause");
	m_pToolTip->AddTool(&m_PLUNGERCOLOR,"R G B Alpha in range 0-1 (alpha=transparency)");
	m_pToolTip->AddTool(&m_PLUNGERGEOM,"name of the stl file describing the syringe plunger geometry");
	m_pToolTip->AddTool(&m_PLUNGEROFFSET,"dx dy dz graphics offset of syringe plunger");
	m_pToolTip->AddTool(&m_PUSHOUT,"[seconds] early dispensing to start flow quickly");
	m_pToolTip->AddTool(&m_RANGE,"[mm] start end home of graphical range of motion for plunger");
	m_pToolTip->AddTool(&m_SUCKBACK,"[seconds] reverse plunger motion to stop flow quickly");
	m_pToolTip->AddTool(&m_SUCKBACKDELAY,"[sec] postpone onset of suckback-positive values shift it later in path");
	m_pToolTip->AddTool(&m_TIPCOLOR,"R G B Alpha in range 0-1 (alpha=transparency)");
	m_pToolTip->AddTool(&m_TIPDIAM,"[mm], internal diameter of syringe needle/tip");
	m_pToolTip->AddTool(&m_TIPGEOM,"name of the stl file describing the nozzle/needle geometry");
	m_pToolTip->AddTool(&m_TIPOFFSET,"dx dy dz graphics offset of syringe nozzle/needle");
	m_pToolTip->AddTool(&m_TIPPOINT,"x,y,z tip location in geometry file coordinates");
	m_pToolTip->AddTool(&m_TOOLCOLOR,"R G B Alpha in range 0-1 (alpha=transparency)");
	m_pToolTip->AddTool(&m_TOOLGEOM,"name of the stl file describing the tool body geometry");
	m_pToolTip->AddTool(&m_TOOLOFFSET,"dx dy dz graphics offset of tool body");
	m_pToolTip->AddTool(&m_VOLUME,"[mL] volume of syringe barrel");

	m_pToolTip->Activate(TRUE);

	return bResult;
}

// --------------------------------------------------------------------------
BOOL CToolDlgParameters::PreTranslateMessage(MSG* pMsg)
// --------------------------------------------------------------------------
{
	if (m_pToolTip != NULL)
		m_pToolTip->RelayEvent(pMsg);

	return CPropertyPage::PreTranslateMessage(pMsg);
}

// --------------------------------------------------------------------------




// --------------------------------------------------------------------------
// CToolDlgPropSheet
// --------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CToolDlgPropSheet, CPropertySheet)

// --------------------------------------------------------------------------
CToolDlgPropSheet::CToolDlgPropSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
// --------------------------------------------------------------------------
{

	pApp = ((CFabAtHomeApp *)AfxGetApp());
	ToolChangeDlg = new CToolChangeDlg;
	AddPage(ToolChangeDlg);
	syringes = new CToolDlgParameters[pApp->printer.GetMaxTools()];
	for(int i = 0; i < pApp->printer.GetMaxTools(); i++)
		AddPage(&syringes[i]);

}

// --------------------------------------------------------------------------
CToolDlgPropSheet::CToolDlgPropSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
// --------------------------------------------------------------------------
{

	pApp = ((CFabAtHomeApp *)AfxGetApp());
	ToolChangeDlg = new CToolChangeDlg;
	AddPage(ToolChangeDlg);
	syringes = new CToolDlgParameters[pApp->printer.GetMaxTools()];
	for(int i = 0; i < pApp->printer.GetMaxTools(); i++)
		AddPage(&syringes[i]);

}

// --------------------------------------------------------------------------
CToolDlgPropSheet::~CToolDlgPropSheet()
// --------------------------------------------------------------------------
{
}

// --------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CToolDlgPropSheet, CPropertySheet)
	ON_BN_CLICKED(IDD_SAVE_BUTTON, OnBnClickedButtonSave)
	ON_BN_CLICKED(IDD_CANCEL_BUTTON, OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDD_CLOSE_BUTTON, OnBnClickedButtonClose)
	ON_BN_CLICKED(IDD_REFRESH_BUTTON, OnBnClickedButtonRefresh)
	ON_BN_CLICKED(IDD_IMPORT_BUTTON, OnBnClickedButtonImport)
	ON_BN_CLICKED(IDD_EXPORT_BUTTON, OnBnClickedButtonExport)
END_MESSAGE_MAP()

// --------------------------------------------------------------------------
BOOL CToolDlgPropSheet::OnInitDialog()
// --------------------------------------------------------------------------
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	TC_ITEM item;
	item.mask = TCIF_TEXT;
	item.pszText = "Mount/Unmount or Load Tools";
	GetTabControl()->SetItem(0, &item);
	for(int i = 0; i < pApp->printer.GetMaxTools(); i++)
	{
		char tablabel[11];
		sprintf_s(tablabel, 10, "Syringe %d", i+1);
		item.pszText = tablabel;
		GetTabControl()->SetItem(i+1, &item);
	}

	CRect rectWnd;
	GetWindowRect(rectWnd);
	SetWindowPos(NULL, 0, 0, rectWnd.Width() + 90, rectWnd.Height(), SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

	Save.CreateEx(WS_EX_CLIENTEDGE, _T("BUTTON"), "Save", WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		rectWnd.Width(), 87, 90, 27, m_hWnd, (HMENU)IDD_SAVE_BUTTON, 0);
	Save.SetFont(GetFont());
	Cancel.CreateEx(WS_EX_CLIENTEDGE, _T("BUTTON"), "Cancel", WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		rectWnd.Width(), 147, 90, 27, m_hWnd, (HMENU)IDD_CANCEL_BUTTON, 0);
	Cancel.SetFont(GetFont());
	Close.CreateEx(WS_EX_CLIENTEDGE, _T("BUTTON"), "Close", WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		rectWnd.Width(), 177, 90, 27, m_hWnd, (HMENU)IDD_CLOSE_BUTTON, 0);
	Close.SetFont(GetFont());
	Refresh.CreateEx(WS_EX_CLIENTEDGE, _T("BUTTON"), "Refresh", WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		rectWnd.Width(), 57, 90, 27, m_hWnd, (HMENU)IDD_REFRESH_BUTTON, 0);
	Refresh.SetFont(GetFont());
	Import.CreateEx(WS_EX_CLIENTEDGE, _T("BUTTON"), "Change Tool Dir.", WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		rectWnd.Width(), 27, 90, 27, m_hWnd, (HMENU)IDD_IMPORT_BUTTON, 0);
	Import.SetFont(GetFont());
	Export.CreateEx(WS_EX_CLIENTEDGE, _T("BUTTON"), "Export", WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		rectWnd.Width(), 117, 90, 27, m_hWnd, (HMENU)IDD_EXPORT_BUTTON, 0);
	Export.SetFont(GetFont());

	CenterWindow();

	m_pToolTip = new CToolTipCtrl;
	if(!m_pToolTip->Create(this))
	{
		TRACE("Unable To create ToolTip\n");
		return bResult;
	}

	//m_pToolTip->SetMaxTipWidth(1024*8);
	m_pToolTip->AddTool(&Save,"Save the current parameters in the current loaded configuration file and refresh 'run-time' parameters.");
	m_pToolTip->AddTool(&Cancel,"Load the original file and replace the 'run-time' parameters");
	m_pToolTip->AddTool(&Close,"Close de dialog and you can also use ALT+F4 or ESC");
	m_pToolTip->AddTool(&Refresh,"The parameters showed in the dialog will replace the 'run-time' parameters and you can also use F5.");
	m_pToolTip->AddTool(&Import,"Load a different printer configuration file from your hard disk");
	m_pToolTip->AddTool(&Export,"Save the parameters showed in the dialog to a new configuration file");
	m_pToolTip->Activate(TRUE);// ToolTip

	return bResult;
}

// --------------------------------------------------------------------------
void CToolDlgPropSheet::OnBnClickedButtonSave()
// --------------------------------------------------------------------------
{
	CPropertyPage *page;

	page = GetActivePage();
	for(int i = 0; i < pApp->printer.GetMaxTools(); i++)
	{
		if(*page == syringes[i])
		{
			if(!SaveParameters(pApp->tool[i].GetByIndex(syringes[i].tool_index), i) || 
				!RefreshParameters(pApp->tool[i].GetByIndex(syringes[i].tool_index), i))
				pApp->Log("Unable to save parameters.");
			else
				pApp->Log("Parameters saved successfully.");
		}
	}
}

// --------------------------------------------------------------------------
void CToolDlgPropSheet::OnBnClickedButtonCancel()
// --------------------------------------------------------------------------
{
	//reload the parameters of all of the tool files in the tools directory
	//into the application's tool array, and add any new tools
	//this can happen online, and should have immediate effect
	pApp->LoadTools(true); //(true, default) -> (params only, no dialog necessary)

	// Instead of closing the modeless property sheet, just hide it.
	pApp->HideToolDlgPropSheet();
}

// --------------------------------------------------------------------------
void CToolDlgPropSheet::OnBnClickedButtonClose()
// --------------------------------------------------------------------------
{
	// Instead of closing the modeless property sheet, just hide it.
	pApp->HideToolDlgPropSheet();
}

// --------------------------------------------------------------------------
void CToolDlgPropSheet::OnBnClickedButtonRefresh()
// --------------------------------------------------------------------------
{
	CPropertyPage *page;

	page = GetActivePage();
	for(int i = 0; i < pApp->printer.GetMaxTools(); i++)
	{
		if(*page == syringes[i])
		{
			if(!RefreshParameters(pApp->tool[i].GetByIndex(syringes[i].tool_index), i))
				pApp->Log("Unable to refresh parameters.");
			else
				pApp->Log("Parameters refreshed successfully.");
		}
	}
}

// --------------------------------------------------------------------------
void CToolDlgPropSheet::OnBnClickedButtonImport()
// --------------------------------------------------------------------------
{
	//have the user select the location of the tool files
	//and reload all tool information from that directory
	if(pApp->LoadTools(false, true)) //(false, true) -> (all data, force dialog box)
	{
		for(int i = 0; i < pApp->printer.GetMaxTools(); i++)
		{
			if(!ClearParameters(i))
				return;
		}
		pApp->Log("Tool directory changed successfully.");
	}
	else
		pApp->Log("Unable to change tool directory.");

	ToolChangeDlg->UpdateListBoxes();
}

// --------------------------------------------------------------------------
void CToolDlgPropSheet::OnBnClickedButtonExport()
// --------------------------------------------------------------------------
{
	CPropertyPage *page;

	page = GetActivePage();
	if(*page == *ToolChangeDlg)
		return;

	CFileDialog dlg(FALSE, "tool", NULL, NULL, 
		"Export Tool Configuration File (*.tool)|*.tool|All Files (*.*)|*.*||", 0);
	dlg.GetOFN().lpstrTitle = "Export Tool Configuration File";
	dlg.GetOFN().Flags |= OFN_OVERWRITEPROMPT;

	if(dlg.DoModal()==IDOK)
	{
		CPropertyPage *page;

		page = GetActivePage();
		for(int i = 0; i < pApp->printer.GetMaxTools(); i++)
		{
			if(*page == syringes[i])
			{
				//Save the new tool file
				if(!SaveParameters(pApp->tool[i].GetByIndex(syringes[i].tool_index), i, dlg.GetPathName()))
					pApp->Log("Unable to export tool file.");
				else
					pApp->Log("Tool file exported successfully.");
			}
		}
	}
}

// --------------------------------------------------------------------------
CTool *CToolDlgPropSheet::GetActiveTool(void)
// --------------------------------------------------------------------------
{
	CPropertyPage *page;

	page = GetActivePage();
	for(int i = 0; i < pApp->printer.GetMaxTools(); i++)
	{
		if(*page == syringes[i])
			return pApp->tool[i].GetByIndex(syringes[i].tool_index);
	}

	return NULL;
}

// --------------------------------------------------------------------------
BOOL CToolDlgPropSheet::PreTranslateMessage(MSG* pMsg)
// --------------------------------------------------------------------------
{
	if (m_pToolTip != NULL)
		m_pToolTip->RelayEvent(pMsg);//Tooltip

	if(pMsg->message==WM_KEYDOWN)
    {
        if(pMsg->wParam == VK_F5)
            OnBnClickedButtonRefresh();
		else if(pMsg->wParam == VK_ESCAPE)
			OnBnClickedButtonClose();
    }	

	return CPropertySheet::PreTranslateMessage(pMsg);
}

// --------------------------------------------------------------------------
bool CToolDlgPropSheet::SetParameters(CTool *tool, int idx)
// --------------------------------------------------------------------------
{
	CString temp;
	CPropertyPage *temppage;
	syringes[idx].tool_index = tool->SearchByName(tool->name);

	temppage = GetActivePage();
	SetActivePage(&syringes[idx]);

	//BACKFILL parameter
	temp.Format("%lf", tool->backfill);
	syringes[idx].m_BACKFILL.SetWindowTextA(temp);

	//BARRELCOLOR parameter
	temp.Format("%lf %lf %lf %lf", tool->barrelcolor.x, 
		tool->barrelcolor.y, 
		tool->barrelcolor.z, 
		tool->barrelalpha);
	syringes[idx].m_BARRELCOLOR.SetWindowTextA(temp);

	//BARRELDIAM parameter
	temp.Format("%lf", tool->GetBarrelID());
	syringes[idx].m_BARRELDIAM.SetWindowTextA(temp);

	//BARRELGEOM parameter
	syringes[idx].m_BARRELGEOM.SetWindowTextA(tool->barrelgeom.name);

	//BARRELOFFSET parameter
	temp.Format("%lf %lf %lf", tool->barreloffset.x, tool->barreloffset.y, tool->barreloffset.z);
	syringes[idx].m_BARRELOFFSET.SetWindowTextA(temp);

	//CLEARANCE parameter
	temp.Format("%lf", tool->clearance);
	syringes[idx].m_CLEARANCE.SetWindowTextA(temp);

	//DEPOSITIONRATE parameter
	temp.Format("%lf", tool->disprate);
	syringes[idx].m_DEPOSITIONRATE.SetWindowTextA(temp);

	//DESCRIPTION parameter
	syringes[idx].m_DESCRIPTION.SetWindowTextA(tool->description);

	//DIRECTION parameter
	temp.Format("%lf %lf %lf", tool->dir.x, tool->dir.y, tool->dir.z);
	syringes[idx].m_DIRECTION.SetWindowTextA(temp);

	//INCREMENT parameter
	temp.Format("%lf", tool->jogIncrement);
	syringes[idx].m_INCREMENT.SetWindowTextA(temp);

	//MINPATHLENGTH parameter
	temp.Format("%lf", tool->m_dMinPathLength);
	syringes[idx].m_MINPATHLENGTH.SetWindowTextA(temp);

	//MOTOR parameter
	temp.Format("%ld %ld %lf", tool->axis, tool->axisDir, tool->mmps);
	syringes[idx].m_MOTOR.SetWindowTextA(temp);

	//PATHHEIGHT parameter
	temp.Format("%lf", tool->pathheight);
	syringes[idx].m_PATHHEIGHT.SetWindowTextA(temp);

	//PATHSPEED parameter
	temp.Format("%lf", tool->GetPathspeed());
	syringes[idx].m_PATHSPEED.SetWindowTextA(temp);

	//PATHWIDTH parameter
	temp.Format("%lf", tool->pathwidth);
	syringes[idx].m_PATHWIDTH.SetWindowTextA(temp);

	//PAUSEPATHS parameter
	temp.Format("%d", tool->GetPausePathCount());
	syringes[idx].m_PAUSEPATHS.SetWindowTextA(temp);

	//PLUNGERCOLOR parameter
	temp.Format("%lf %lf %lf %lf", tool->plungercolor.x,
		tool->plungercolor.y,
		tool->plungercolor.z,
		tool->plungeralpha);
	syringes[idx].m_PLUNGERCOLOR.SetWindowTextA(temp);

	//PLUNGERGEOM parameter
	syringes[idx].m_PLUNGERGEOM.SetWindowTextA(tool->plungergeom.name);

	//PLUNGEROFFSET parameter
	temp.Format("%lf %lf %lf", tool->plungeroffset.x,
		tool->plungeroffset.y,
		tool->plungeroffset.z);
	syringes[idx].m_PLUNGEROFFSET.SetWindowTextA(temp);

	//PUSHOUT parameter
	temp.Format("%lf", tool->pushout);
	syringes[idx].m_PUSHOUT.SetWindowTextA(temp);

	//RANGE parameter
	temp.Format("%lf %lf %lf", tool->rangemin, tool->rangemax, tool->unloadpos);
	syringes[idx].m_RANGE.SetWindowTextA(temp);

	//SUCKBACK parameter
	temp.Format("%lf", tool->suckback);
	syringes[idx].m_SUCKBACK.SetWindowTextA(temp);

	//SUCKBACKDELAY parameter
	temp.Format("%lf", tool->suckbackDelay);
	syringes[idx].m_SUCKBACKDELAY.SetWindowTextA(temp);

	//TIPCOLOR parameter
	temp.Format("%lf %lf %lf %lf", tool->tipcolor.x,
		tool->tipcolor.y,
		tool->tipcolor.z,
		tool->tipalpha);
	syringes[idx].m_TIPCOLOR.SetWindowTextA(temp);

	//TIPDIAM parameter
	temp.Format("%lf", tool->GetTipID());
	syringes[idx].m_TIPDIAM.SetWindowTextA(temp);

	//TIPGEOM parameter
	syringes[idx].m_TIPGEOM.SetWindowTextA(tool->tipgeom.name);

	//TIPOFFSET parameter
	temp.Format("%lf %lf %lf", tool->tipoffset.x,
		tool->tipoffset.y,
		tool->tipoffset.z);
	syringes[idx].m_TIPOFFSET.SetWindowTextA(temp);

	//TIPPOINT parameter
	temp.Format("%lf %lf %lf", tool->tip.x,
		tool->tip.y,
		tool->tip.z);
	syringes[idx].m_TIPPOINT.SetWindowTextA(temp);

	//TOOLCOLOR parameter
	temp.Format("%lf %lf %lf %lf", tool->toolcolor.x,
		tool->toolcolor.y,
		tool->toolcolor.z,
		tool->toolalpha);
	syringes[idx].m_TOOLCOLOR.SetWindowTextA(temp);

	//TOOLGEOM parameter
	syringes[idx].m_TOOLGEOM.SetWindowTextA(tool->toolgeom.name);

	//TOOLOFFSET parameter
	temp.Format("%lf %lf %lf", tool->tooloffset.x,
		tool->tooloffset.y,
		tool->tooloffset.z);
	syringes[idx].m_TOOLOFFSET.SetWindowTextA(temp);

	//VOLUME parameter
	temp.Format("%lf", tool->maxvolume);
	syringes[idx].m_VOLUME.SetWindowTextA(temp);

	SetActivePage(temppage);

	return true;
}

// --------------------------------------------------------------------------
bool CToolDlgPropSheet::ClearParameters(int idx)
// --------------------------------------------------------------------------
{
	CString temp = "";
	CPropertyPage *temppage;

	temppage = GetActivePage();
	SetActivePage(&syringes[idx]);

	//BACKFILL parameter
	syringes[idx].m_BACKFILL.SetWindowTextA(temp);

	//BARRELCOLOR parameter
	syringes[idx].m_BARRELCOLOR.SetWindowTextA(temp);

	//BARRELDIAM parameter
	syringes[idx].m_BARRELDIAM.SetWindowTextA(temp);

	//BARRELGEOM parameter
	syringes[idx].m_BARRELGEOM.SetWindowTextA(temp);

	//BARRELOFFSET parameter
	syringes[idx].m_BARRELOFFSET.SetWindowTextA(temp);

	//CLEARANCE parameter
	syringes[idx].m_CLEARANCE.SetWindowTextA(temp);

	//DEPOSITIONRATE parameter
	syringes[idx].m_DEPOSITIONRATE.SetWindowTextA(temp);

	//DESCRIPTION parameter
	syringes[idx].m_DESCRIPTION.SetWindowTextA(temp);

	//DIRECTION parameter
	syringes[idx].m_DIRECTION.SetWindowTextA(temp);

	//INCREMENT parameter
	syringes[idx].m_INCREMENT.SetWindowTextA(temp);

	//MINPATHLENGTH parameter
	syringes[idx].m_MINPATHLENGTH.SetWindowTextA(temp);

	//MOTOR parameter
	syringes[idx].m_MOTOR.SetWindowTextA(temp);

	//PATHHEIGHT parameter
	syringes[idx].m_PATHHEIGHT.SetWindowTextA(temp);

	//PATHSPEED parameter
	syringes[idx].m_PATHSPEED.SetWindowTextA(temp);

	//PATHWIDTH parameter
	syringes[idx].m_PATHWIDTH.SetWindowTextA(temp);

	//PAUSEPATHS parameter
	syringes[idx].m_PAUSEPATHS.SetWindowTextA(temp);

	//PLUNGERCOLOR parameter
	syringes[idx].m_PLUNGERCOLOR.SetWindowTextA(temp);

	//PLUNGERGEOM parameter
	syringes[idx].m_PLUNGERGEOM.SetWindowTextA(temp);

	//PLUNGEROFFSET parameter
	syringes[idx].m_PLUNGEROFFSET.SetWindowTextA(temp);

	//PUSHOUT parameter
	syringes[idx].m_PUSHOUT.SetWindowTextA(temp);

	//RANGE parameter
	syringes[idx].m_RANGE.SetWindowTextA(temp);

	//SUCKBACK parameter
	syringes[idx].m_SUCKBACK.SetWindowTextA(temp);

	//SUCKBACKDELAY parameter
	syringes[idx].m_SUCKBACKDELAY.SetWindowTextA(temp);

	//TIPCOLOR parameter
	syringes[idx].m_TIPCOLOR.SetWindowTextA(temp);

	//TIPDIAM parameter
	syringes[idx].m_TIPDIAM.SetWindowTextA(temp);

	//TIPGEOM parameter
	syringes[idx].m_TIPGEOM.SetWindowTextA(temp);

	//TIPOFFSET parameter
	syringes[idx].m_TIPOFFSET.SetWindowTextA(temp);

	//TIPPOINT parameter
	syringes[idx].m_TIPPOINT.SetWindowTextA(temp);

	//TOOLCOLOR parameter
	syringes[idx].m_TOOLCOLOR.SetWindowTextA(temp);

	//TOOLGEOM parameter
	syringes[idx].m_TOOLGEOM.SetWindowTextA(temp);

	//TOOLOFFSET parameter
	syringes[idx].m_TOOLOFFSET.SetWindowTextA(temp);

	//VOLUME parameter
	syringes[idx].m_VOLUME.SetWindowTextA(temp);

	SetActivePage(temppage);

	return true;
}

// --------------------------------------------------------------------------
bool CToolDlgPropSheet::RefreshParameters(CTool *tool, int idx)
// --------------------------------------------------------------------------
{
	CString temp, geomfile, geompath;
	CPropertyPage *temppage;
	int j;

	temppage = GetActivePage();
	SetActivePage(&syringes[idx]);

	//BACKFILL parameter
	syringes[idx].m_BACKFILL.GetWindowTextA(temp);
	tool->backfill = atof(temp);
	if (tool->backfill < 0.01 && tool->backfill > 20)
		return false;

	//BARRELCOLOR parameter
	syringes[idx].m_BARRELCOLOR.GetWindowTextA(temp);
	temp.Replace(","," ");
	j = sscanf_s(temp,"%lf %lf %lf %lf", &tool->barrelcolor.x,
		&tool->barrelcolor.y,
		&tool->barrelcolor.z,
		&tool->barrelalpha);
	if (j != 4) {
		tool->barrelcolor = CVec(0.5,0.5,0.5);
		tool->barrelalpha = 1;
	}

	//BARRELDIAM parameter
	syringes[idx].m_BARRELDIAM.GetWindowTextA(temp);
	temp.Replace(","," ");
	j = sscanf_s(temp,"%lf", &tool->m_dBarrelID);
	if (j != 1)
		tool->m_dBarrelID = 15.8; //standard for 10cc EFC

	//BARRELGEOM parameter
	syringes[idx].m_BARRELGEOM.GetWindowTextA(temp);
	geomfile = CUtils::RemoveComment(temp);
	geompath = CUtils::GetPath(pApp->toolsDir)+"\\"+geomfile;
	if (_access(geompath,4) != 0 || !tool->barrelgeom.LoadSTL(geompath, 10000))
		return false;
	tool->barrelgeom.name = geomfile;

	//BARRELOFFSET parameter
	syringes[idx].m_BARRELOFFSET.GetWindowTextA(temp);
	temp.Replace(","," ");
	j = sscanf_s(temp,"%lf %lf %lf", &tool->barreloffset.x, &tool->barreloffset.y, &tool->barreloffset.z);
	if (j != 3)
		tool->barreloffset = CVec(0,0,0);

	//CLEARANCE parameter
	syringes[idx].m_CLEARANCE.GetWindowTextA(temp);
	temp.Replace(","," ");
	j = sscanf_s(temp,"%lf", &tool->clearance);
	if (j != 1)
		tool->clearance = DEFAULT_TRAVEL_CLEARANCE;

	//DEPOSITIONRATE parameter
	syringes[idx].m_DEPOSITIONRATE.GetWindowTextA(temp);
	tool->disprate = atof(temp);
	if (tool->disprate < 0.0000001 && tool->disprate > 1000)
		tool->disprate = 2;

	//DESCRIPTION parameter
	syringes[idx].m_DESCRIPTION.GetWindowTextA(tool->description);

	//DIRECTION parameter
	syringes[idx].m_DIRECTION.GetWindowTextA(temp);
	temp.Replace(","," ");
	j = sscanf_s(temp,"%lf %lf %lf", 
		&tool->dir.x, &tool->dir.y, &tool->dir.z);
	if (j != 3)
		tool->dir = CVec(0,0,-1);

	//INCREMENT parameter
	syringes[idx].m_INCREMENT.GetWindowTextA(temp);
	temp.Replace(","," ");
	j = sscanf_s(temp,"%lf", &tool->jogIncrement);
	if (j != 1)
		tool->jogIncrement = JOG_STEP_TOOL;

	//MINPATHLENGTH parameter
	syringes[idx].m_MINPATHLENGTH.GetWindowTextA(temp);
	temp.Replace(","," ");
	j = sscanf_s(temp,"%lf", &tool->m_dMinPathLength);
	if (j != 1)
		tool->m_dMinPathLength = 0;

	//MOTOR parameter
	syringes[idx].m_MOTOR.GetWindowTextA(temp);
	temp.Replace(","," ");
	j = sscanf_s(temp,"%ld %ld %lf", 
		&tool->axis, &tool->axisDir, &tool->mmps);
	if (j != 3)
		return false;

	//PATHHEIGHT parameter
	syringes[idx].m_PATHHEIGHT.GetWindowTextA(temp);
	tool->pathheight = atof(temp);
	if (tool->pathheight < 0.001 && tool->pathheight > 20)
		return false;

	//PATHSPEED parameter
	syringes[idx].m_PATHSPEED.GetWindowTextA(temp);
	temp.Replace(","," ");
	j = sscanf_s(temp,"%lf", &tool->pathspeed);
	if (j != 1)
		tool->pathspeed = 5.0;

	//PATHWIDTH parameter
	syringes[idx].m_PATHWIDTH.GetWindowTextA(temp);
	tool->pathwidth = atof(temp);
	if (tool->pathwidth < 0.01 && tool->pathwidth > 20)
		return false;

	//PAUSEPATHS parameter
	syringes[idx].m_PAUSEPATHS.GetWindowTextA(temp);
	temp.Replace(","," ");
	j = sscanf_s(temp,"%d", &tool->m_nPausePathCount);
	if (j != 1)
		tool->m_nPausePathCount = 0;

	//PLUNGERCOLOR parameter
	syringes[idx].m_PLUNGERCOLOR.GetWindowTextA(temp);
	temp.Replace(","," ");
	j = sscanf_s(temp,"%lf %lf %lf %lf", &tool->plungercolor.x,
		&tool->plungercolor.y,
		&tool->plungercolor.z,
		&tool->plungeralpha);
	if (j != 4) {
		tool->plungercolor = CVec(0.5,0.5,0.5);
		tool->plungeralpha = 1;
	}

	//PLUNGERGEOM parameter
	syringes[idx].m_PLUNGERGEOM.GetWindowTextA(temp);
	geomfile = CUtils::RemoveComment(temp);
	geompath = CUtils::GetPath(pApp->toolsDir)+"\\"+geomfile;
	if (_access(geompath,4) != 0 || !tool->plungergeom.LoadSTL(geompath, 10000))
		return false;
	tool->plungergeom.name = geomfile;

	//PLUNGEROFFSET parameter
	syringes[idx].m_PLUNGEROFFSET.GetWindowTextA(temp);
	temp.Replace(","," ");
	j = sscanf_s(temp,"%lf %lf %lf", &tool->plungeroffset.x, &tool->plungeroffset.y, &tool->plungeroffset.z);
	if (j != 3)
		tool->plungeroffset = CVec(0,0,0);

	//PUSHOUT parameter
	syringes[idx].m_PUSHOUT.GetWindowTextA(temp);
	temp.Replace(","," ");
	j = sscanf_s(temp,"%lf", &tool->pushout);
	if (j != 1) {
		tool->pushout = 0;
		return false;
	}

	//RANGE parameter
	syringes[idx].m_RANGE.GetWindowTextA(temp);
	temp.Replace(","," ");
	j = sscanf_s(temp,"%lf %lf %lf", &tool->rangemin, &tool->rangemax, &tool->unloadpos);
	if (j != 3) {
		tool->rangemin = 0;
		tool->rangemax = 100;
		tool->curstep = 0;
	}

	//SUCKBACK parameter
	syringes[idx].m_SUCKBACK.GetWindowTextA(temp);
	temp.Replace(","," ");
	j = sscanf_s(temp,"%lf", &tool->suckback);
	if (j != 1) {
		tool->suckback = 0;
		return false;
	}

	//SUCKBACKDELAY parameter
	syringes[idx].m_SUCKBACKDELAY.GetWindowTextA(temp);
	temp.Replace(","," ");
	j = sscanf_s(temp,"%lf", &tool->suckbackDelay);
	if ((j != 1)||(tool->suckbackDelay < 0)||(tool->suckbackDelay>tool->suckback)) {
		tool->suckbackDelay = 0;
		return false;
	}

	//TIPCOLOR parameter
	syringes[idx].m_TIPCOLOR.GetWindowTextA(temp);
	temp.Replace(","," ");
	j = sscanf_s(temp,"%lf %lf %lf %lf", &tool->tipcolor.x,
		&tool->tipcolor.y,
		&tool->tipcolor.z,
		&tool->tipalpha);
	if (j != 4) {
		tool->tipcolor = CVec(0.5,0.5,0.5);
		tool->tipalpha = 1;
	}

	//TIPDIAM parameter
	syringes[idx].m_TIPDIAM.GetWindowTextA(temp);
	temp.Replace(","," ");
	j = sscanf_s(temp,"%lf", &tool->m_dTipID);
	if (j != 1)
		tool->m_dTipID = 0.25;

	//TIPGEOM parameter
	syringes[idx].m_TIPGEOM.GetWindowTextA(temp);
	geomfile = CUtils::RemoveComment(temp);
	geompath = CUtils::GetPath(pApp->toolsDir)+"\\"+geomfile;
	if (_access(geompath,4) != 0 || !tool->tipgeom.LoadSTL(geompath, 10000))
		return false;
	tool->tipgeom.name = geomfile;

	//TIPOFFSET parameter
	syringes[idx].m_TIPOFFSET.GetWindowTextA(temp);
	temp.Replace(","," ");
	j = sscanf_s(temp,"%lf %lf %lf", &tool->tipoffset.x, &tool->tipoffset.y, &tool->tipoffset.z);
	if (j != 3)
		tool->tipoffset = CVec(0,0,0);

	//TIPPOINT parameter
	syringes[idx].m_TIPPOINT.GetWindowTextA(temp);
	temp.Replace(","," ");
	j = sscanf_s(temp,"%lf %lf %lf", &tool->tip.x, &tool->tip.y, &tool->tip.z);
	if (j != 3) {
		tool->tipoffset = CVec(0,0,0);
		return false;
	}

	//TOOLCOLOR parameter
	syringes[idx].m_TOOLCOLOR.GetWindowTextA(temp);
	temp.Replace(","," ");
	j = sscanf_s(temp,"%lf %lf %lf %lf", &tool->toolcolor.x,
		&tool->toolcolor.y,
		&tool->toolcolor.z,
		&tool->toolalpha);
	if (j != 4) {
		tool->toolcolor = CVec(0.5,0.5,0.5);
		tool->toolalpha = 1;
	}

	//TOOLGEOM parameter
	syringes[idx].m_TOOLGEOM.GetWindowTextA(temp);
	geomfile = CUtils::RemoveComment(temp);
	geompath = CUtils::GetPath(pApp->toolsDir)+"\\"+geomfile;
	if (_access(geompath,4) != 0 || !tool->toolgeom.LoadSTL(geompath, 10000))
		return false;
	tool->toolgeom.name = geomfile;

	//TOOLOFFSET parameter
	syringes[idx].m_TOOLOFFSET.GetWindowTextA(temp);
	temp.Replace(","," ");
	j = sscanf_s(temp,"%lf %lf %lf", &tool->tooloffset.x, &tool->tooloffset.y, &tool->tooloffset.z);
	if (j != 3)
		tool->tooloffset = CVec(0,0,0);

	//VOLUME parameter
	syringes[idx].m_VOLUME.GetWindowTextA(temp);
	temp.Replace(","," ");
	tool->maxvolume = atof(temp);
	if (tool->maxvolume < 1 && tool->maxvolume > 100) {
		tool->maxvolume = 10;
		return false;
	}

	SetActivePage(temppage);

	return true;
}

// --------------------------------------------------------------------------
bool CToolDlgPropSheet::SaveParameters(CTool *tool, int idx, CString filename)
// --------------------------------------------------------------------------
{
	CString temp;
	CPropertyPage *temppage;

	if(filename == "")
		filename = pApp->toolsDir + "\\" + tool->name;

	CStdioFile file;
	if(file.Open(filename, CFile::modeCreate | CFile::modeReadWrite) == 0)
		return false;

	temppage = GetActivePage();
	SetActivePage(&syringes[idx]);

	file.WriteString("////////////////////////////FREQUENTLY CHANGED PARAMETERS////////////////////////////\n");

	//DESCRIPTION parameter
	syringes[idx].m_DESCRIPTION.GetWindowTextA(temp);
	if(!temp.IsEmpty()) {
		temp = "DESCRIPTION                   " + temp + "\n";
		file.WriteString(temp);
	}

	//TIPDIAM parameter
	syringes[idx].m_TIPDIAM.GetWindowTextA(temp);
	if(!temp.IsEmpty()) {
		temp = "TIPDIAM                       " + temp + "\n";
		file.WriteString(temp);
	}

	//PATHWIDTH parameter
	syringes[idx].m_PATHWIDTH.GetWindowTextA(temp);
	if(!temp.IsEmpty()) {
		temp = "PATHWIDTH                     " + temp + "\n";
		file.WriteString(temp);
	}

	//PATHHEIGHT parameter
	syringes[idx].m_PATHHEIGHT.GetWindowTextA(temp);
	if(!temp.IsEmpty()) {
		temp = "PATHHEIGHT                    " + temp + "\n";
		file.WriteString(temp);
	}

	//PUSHOUT parameter
	syringes[idx].m_PUSHOUT.GetWindowTextA(temp);
	if(!temp.IsEmpty()) {
		temp = "PUSHOUT                       " + temp + "\n";
		file.WriteString(temp);
	}

	//SUCKBACK parameter
	syringes[idx].m_SUCKBACK.GetWindowTextA(temp);
	if(!temp.IsEmpty()) {
		temp = "SUCKBACK                      " + temp + "\n";
		file.WriteString(temp);
	}

	//SUCKBACKDELAY parameter
	syringes[idx].m_SUCKBACKDELAY.GetWindowTextA(temp);
	if(!temp.IsEmpty()) {
		temp = "SUCKBACKDELAY                 " + temp + "\n";
		file.WriteString(temp);
	}

	//DEPOSITIONRATE parameter
	syringes[idx].m_DEPOSITIONRATE.GetWindowTextA(temp);
	if(!temp.IsEmpty()) {
		temp = "DEPOSITIONRATE                " + temp + "\n";
		file.WriteString(temp);
	}

	//CLEARANCE parameter
	syringes[idx].m_CLEARANCE.GetWindowTextA(temp);
	if(!temp.IsEmpty()) {
		temp = "CLEARANCE                     " + temp + "\n";
		file.WriteString(temp);
	}

	//PATHSPEED parameter
	syringes[idx].m_PATHSPEED.GetWindowTextA(temp);
	if(!temp.IsEmpty()) {
		temp = "PATHSPEED                     " + temp + "\n";
		file.WriteString(temp);
	}

	//PAUSEPATHS parameter
	syringes[idx].m_PAUSEPATHS.GetWindowTextA(temp);
	if(!temp.IsEmpty()) {
		temp = "PAUSEPATHS                    " + temp + "\n";
		file.WriteString(temp);
	}

	file.WriteString("\n\n\n////////////////////////////RARELY CHANGED PARAMETERS////////////////////////////\n");

	//BACKFILL parameter
	syringes[idx].m_BACKFILL.GetWindowTextA(temp);
	if(!temp.IsEmpty()) {
		temp = "BACKFILL                      " + temp + "\n";
		file.WriteString(temp);
	}

	//BARRELCOLOR parameter
	syringes[idx].m_BARRELCOLOR.GetWindowTextA(temp);
	if(!temp.IsEmpty()) {
		temp = "BARRELCOLOR                   " + temp + "\n";
		file.WriteString(temp);
	}

	//BARRELDIAM parameter
	syringes[idx].m_BARRELDIAM.GetWindowTextA(temp);
	if(!temp.IsEmpty()) {
		temp = "BARRELDIAM                    " + temp + "\n";
		file.WriteString(temp);
	}

	//BARRELGEOM parameter
	syringes[idx].m_BARRELGEOM.GetWindowTextA(temp);
	if(!temp.IsEmpty()) {
		temp = "BARRELGEOM                    " + temp + "\n";
		file.WriteString(temp);
	}

	//BARRELOFFSET parameter
	syringes[idx].m_BARRELOFFSET.GetWindowTextA(temp);
	if(!temp.IsEmpty()) {
		temp = "BARRELOFFSET                  " + temp + "\n";
		file.WriteString(temp);
	}

	//DIRECTION parameter
	syringes[idx].m_DIRECTION.GetWindowTextA(temp);
	if(!temp.IsEmpty()) {
		temp = "DIRECTION                     " + temp + "\n";
		file.WriteString(temp);
	}

	//INCREMENT parameter
	syringes[idx].m_INCREMENT.GetWindowTextA(temp);
	if(!temp.IsEmpty()) {
		temp = "INCREMENT                     " + temp + "\n";
		file.WriteString(temp);
	}

	//MOTOR parameter
	syringes[idx].m_MOTOR.GetWindowTextA(temp);
	if(!temp.IsEmpty()) {
		temp = "MOTOR                         " + temp + "\n";
		file.WriteString(temp);
	}

	//PLUNGERCOLOR parameter
	syringes[idx].m_PLUNGERCOLOR.GetWindowTextA(temp);
	if(!temp.IsEmpty()) {
		temp = "PLUNGERCOLOR                  " + temp + "\n";
		file.WriteString(temp);
	}

	//PLUNGERGEOM parameter
	syringes[idx].m_PLUNGERGEOM.GetWindowTextA(temp);
	if(!temp.IsEmpty()) {
		temp = "PLUNGERGEOM                   " + temp + "\n";
		file.WriteString(temp);
	}

	//PLUNGEROFFSET parameter
	syringes[idx].m_PLUNGEROFFSET.GetWindowTextA(temp);
	if(!temp.IsEmpty()) {
		temp = "PLUNGEROFFSET                 " + temp + "\n";
		file.WriteString(temp);
	}

	//RANGE parameter
	syringes[idx].m_RANGE.GetWindowTextA(temp);
	if(!temp.IsEmpty()) {
		temp = "RANGE                         " + temp + "\n";
		file.WriteString(temp);
	}

	//TIPCOLOR parameter
	syringes[idx].m_TIPCOLOR.GetWindowTextA(temp);
	if(!temp.IsEmpty()) {
		temp = "TIPCOLOR                      " + temp + "\n";
		file.WriteString(temp);
	}

	//TIPGEOM parameter
	syringes[idx].m_TIPGEOM.GetWindowTextA(temp);
	if(!temp.IsEmpty()) {
		temp = "TIPGEOM                       " + temp + "\n";
		file.WriteString(temp);
	}

	//TIPOFFSET parameter
	syringes[idx].m_TIPOFFSET.GetWindowTextA(temp);
	if(!temp.IsEmpty()) {
		temp = "TIPOFFSET                     " + temp + "\n";
		file.WriteString(temp);
	}

	//TIPPOINT parameter
	syringes[idx].m_TIPPOINT.GetWindowTextA(temp);
	if(!temp.IsEmpty()) {
		temp = "TIPPOINT                      " + temp + "\n";
		file.WriteString(temp);
	}
	//TOOLCOLOR parameter
	syringes[idx].m_TOOLCOLOR.GetWindowTextA(temp);
	if(!temp.IsEmpty()) {
		temp = "TOOLCOLOR                     " + temp + "\n";
		file.WriteString(temp);
	}

	//TOOLGEOM parameter
	syringes[idx].m_TOOLGEOM.GetWindowTextA(temp);
	if(!temp.IsEmpty()) {
		temp = "TOOLGEOM                      " + temp + "\n";
		file.WriteString(temp);
	}

	//TOOLOFFSET parameter
	syringes[idx].m_TOOLOFFSET.GetWindowTextA(temp);
	if(!temp.IsEmpty()) {
		temp = "TOOLOFFSET                    " + temp + "\n";
		file.WriteString(temp);
	}

	//VOLUME parameter
	syringes[idx].m_VOLUME.GetWindowTextA(temp);
	if(!temp.IsEmpty()) {
		temp = "VOLUME                        " + temp + "\n";
		file.WriteString(temp);
	}

	//MINPATHLENGTH parameter
	syringes[idx].m_MINPATHLENGTH.GetWindowTextA(temp);
	if(!temp.IsEmpty()) {
		temp = "MINPATHLENGTH                 " + temp + "\n";
		file.WriteString(temp);
	}
	
	SetActivePage(temppage);
	
	return true;
}
// --------------------------------------------------------------------------




// --------------------------------------------------------------------------
// CToolDlg
// --------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(CToolDlg, CMiniFrameWnd)

// --------------------------------------------------------------------------
CToolDlg::CToolDlg()
// --------------------------------------------------------------------------
{

	m_ToolDlgPropSheet = NULL;
	pApp = ((CFabAtHomeApp *)AfxGetApp());

}

// --------------------------------------------------------------------------
CToolDlg::~CToolDlg()
// --------------------------------------------------------------------------
{
}

// --------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CToolDlg, CMiniFrameWnd)
	ON_WM_ACTIVATE()
	ON_WM_CLOSE()
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

// --------------------------------------------------------------------------
void CToolDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
// --------------------------------------------------------------------------
{
	CMiniFrameWnd::OnActivate(nState, pWndOther, bMinimized);

	// Forward any WM_ACTIVATEs to the property sheet...
	// Like the dialog manager itself, it needs them to save/restore the focus.
	ASSERT_VALID(m_ToolDlgPropSheet);

	// Use GetCurrentMessage to get unmodified message data.
	const MSG* pMsg = GetCurrentMessage();
	ASSERT(pMsg->message == WM_ACTIVATE);
	m_ToolDlgPropSheet->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
}

// --------------------------------------------------------------------------
void CToolDlg::OnClose()
// --------------------------------------------------------------------------
{
	// Instead of closing the modeless property sheet, just hide it.
	pApp->HideToolDlgPropSheet();
}

// --------------------------------------------------------------------------
int CToolDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
// --------------------------------------------------------------------------
{
	if (CMiniFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_ToolDlgPropSheet = new CToolDlgPropSheet("Tool Configuration Properties", this);
	if (!m_ToolDlgPropSheet->Create(this, WS_CHILD | WS_VISIBLE, 0))
	{
		delete m_ToolDlgPropSheet;
		m_ToolDlgPropSheet = NULL;
		return -1;
	}

	// Resize the mini frame so that it fits around the child property
	// sheet.
	CRect rectClient, rectWindow;
	m_ToolDlgPropSheet->GetWindowRect(rectClient);
	rectWindow = rectClient;

	// CMiniFrameWnd::CalcWindowRect adds the extra width and height
	// needed from the mini frame.
	CalcWindowRect(rectWindow);
	SetWindowPos(NULL, rectWindow.left, rectWindow.top,	rectWindow.Width(), rectWindow.Height(),
		SWP_NOZORDER | SWP_NOACTIVATE);
	m_ToolDlgPropSheet->SetWindowPos(NULL, 0, 0, rectClient.Width(), rectClient.Height(),
		SWP_NOZORDER | SWP_NOACTIVATE);

	return 0;
}

// --------------------------------------------------------------------------
void CToolDlg::OnSetFocus(CWnd* pOldWnd)
// --------------------------------------------------------------------------
{
	// Forward focus to the embedded property sheet
	ASSERT_VALID(m_ToolDlgPropSheet);
	m_ToolDlgPropSheet->SetFocus();
}

// --------------------------------------------------------------------------
void CToolDlg::OnShowWindow(BOOL bShow, UINT nStatus)
// --------------------------------------------------------------------------
{
	CMiniFrameWnd::OnShowWindow(bShow, nStatus);
}

// --------------------------------------------------------------------------