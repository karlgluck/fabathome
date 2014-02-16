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

// FabAtHome.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include ".\fabathome.h"
#include "afxwin.h"
#include "ToolChangeDlg.h"
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFabAtHomeApp

BEGIN_MESSAGE_MAP(CFabAtHomeApp, CWinApp)
	//{{AFX_MSG_MAP(CFabAtHomeApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_PRINTER_JOG, OnPrinterJog)
	ON_UPDATE_COMMAND_UI(ID_PRINTER_JOG, OnUpdatePrinterJog)
	ON_COMMAND(ID_PRINTER_JOGTOOL, OnPrinterJogtool)
	ON_UPDATE_COMMAND_UI(ID_PRINTER_JOGTOOL, OnUpdatePrinterJogtool)
	ON_COMMAND(ID_PRINTER_INITIALIZEHARDWARE, OnPrinterInitializehardware)
	ON_COMMAND(ID_PRINTER_LOADCONFIGURATION, OnPrinterLoadconfiguration)
	ON_UPDATE_COMMAND_UI(ID_PRINTER_INITIALIZEHARDWARE, OnUpdatePrinterInitializehardware)
	ON_COMMAND(ID_PRINTER_ENABLEMOTION, OnPrinterEnablemotion)
	ON_UPDATE_COMMAND_UI(ID_PRINTER_ENABLEMOTION, OnUpdatePrinterEnablemotion)
	ON_COMMAND(ID_PRINTER_PAUSEQUEUE, OnPrinterPausequeue)
	ON_UPDATE_COMMAND_UI(ID_PRINTER_PAUSEQUEUE, OnUpdatePrinterPausequeue)
	ON_COMMAND(ID_PRINTER_ABORTALLMOTION, OnPrinterAbortallmotion)
	ON_UPDATE_COMMAND_UI(ID_PRINTER_ABORTALLMOTION, OnUpdatePrinterAbortallmotion)
	ON_COMMAND(ID_PRINTER_RESETHARDWARE, OnPrinterResethardware)
	ON_UPDATE_COMMAND_UI(ID_PRINTER_RESETHARDWARE, OnUpdatePrinterResethardware)
	ON_COMMAND(ID_PRINTER_SHOWSTATUS, OnPrinterShowstatus)
	ON_UPDATE_COMMAND_UI(ID_PRINTER_SHOWSTATUS, OnUpdatePrinterShowstatus)
	ON_COMMAND(ID_TOOL_LOADFILES, OnToolLoadfiles)
	ON_UPDATE_COMMAND_UI(ID_TOOL_LOADFILES, OnUpdateToolLoadfiles)
	ON_COMMAND(ID_TOOL_REFRESHPARAMETERS, OnToolRefreshparameters)
	ON_COMMAND(ID_VIEW_SHOWBUILDINFO, OnViewShowbuildinfo)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWBUILDINFO, OnUpdateViewShowbuildinfo)
	ON_COMMAND(ID_TOOL_CHANGETOOLS, OnToolChangetools)
	ON_UPDATE_COMMAND_UI(ID_TOOL_CHANGETOOLS, OnUpdateToolChangetools)
	ON_COMMAND(ID_PRINTER_EMERGENCYSTOP, OnPrinterEmergencystop)
	ON_UPDATE_COMMAND_UI(ID_PRINTER_EMERGENCYSTOP, OnUpdatePrinterEmergencystop)
	ON_UPDATE_COMMAND_UI(ID_PRINTER_LOG, &CFabAtHomeApp::OnUpdatePrinterLog)
	ON_COMMAND(ID_PRINTER_LOG, &CFabAtHomeApp::OnPrinterLog)
	ON_COMMAND(ID_TOOL_TOOLCONFIGURATION, &CFabAtHomeApp::OnToolToolconfiguration)
	ON_UPDATE_COMMAND_UI(ID_TOOL_TOOLCONFIGURATION, &CFabAtHomeApp::OnUpdateToolToolconfiguration)
	ON_UPDATE_COMMAND_UI(ID_PRINTER_LOADCONFIGURATION, &CFabAtHomeApp::OnUpdatePrinterLoadconfiguration)
	ON_COMMAND(ID_PRINTER_EMULATEFASTER, &CFabAtHomeApp::OnPrinterEmulatefaster)
	ON_UPDATE_COMMAND_UI(ID_PRINTER_EMULATEFASTER, &CFabAtHomeApp::OnUpdatePrinterEmulatefaster)
	ON_COMMAND(ID_PRINTER_EMULATESLOWER, &CFabAtHomeApp::OnPrinterEmulateslower)
	ON_UPDATE_COMMAND_UI(ID_PRINTER_EMULATESLOWER, &CFabAtHomeApp::OnUpdatePrinterEmulateslower)
	ON_COMMAND(ID_EMULATION_EMULATEHARDWARE, &CFabAtHomeApp::OnEmulationEmulatehardware)
	ON_UPDATE_COMMAND_UI(ID_EMULATION_EMULATEHARDWARE, &CFabAtHomeApp::OnUpdateEmulationEmulatehardware)
	ON_COMMAND(ID_PRINTER_NETWORKCONNECT, &CFabAtHomeApp::OnPrinterNetworkconnect)
	ON_UPDATE_COMMAND_UI(ID_PRINTER_NETWORKCONNECT, &CFabAtHomeApp::OnUpdatePrinterNetworkconnect)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFabAtHomeApp construction

// --------------------------------------------------------------------------
CFabAtHomeApp::CFabAtHomeApp()
// --------------------------------------------------------------------------
{
	pJogCarriageDlg = NULL;
	pStatusDlg = NULL;
	pJogToolDlg = NULL;

}

/////////////////////////////////////////////////////////////////////////////
// The one and only CFabAtHomeApp object

CFabAtHomeApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CFabAtHomeApp initialization

// --------------------------------------------------------------------------
BOOL CFabAtHomeApp::InitInstance()
// --------------------------------------------------------------------------
{
	CString msg;

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.


	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Cornell University Fab@Home"));

	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CFabAtHomeDoc),
		RUNTIME_CLASS(CMainFrame),
		RUNTIME_CLASS(CFabAtHomeView));
	AddDocTemplate(pDocTemplate);
	
	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Special cursors
	hHandCursor = LoadCursor(IDC_HANDCURSOR);

	//try to read the various directories from the registry
	//start by setting up the defaults
	
	//get the current working directory 
	char buf[_MAX_PATH];
	if( _getcwd( buf, _MAX_PATH ) == NULL )
	{
		Log("Cannot read current directory while initializing");
	}
	//the working directory is set every time you run the application
	//to be the directory from which you run the fabathome.exe
	workDir.Format("%s", buf);
	printerFile.Format("%s\\%s", workDir, "printer\\*.printer");
	toolsDir.Format("%s\\%s", workDir, "tools");
	matsDir.Format("%s\\%s", workDir, "materials");
	modelsDir = workDir;

	//now try to read from the registry
	//defaults will be applied if necessary

	/*DISABLED THE REGISTRY USAGE EXCEPT FOR PRINTER AND MODELS BECAUSE USERS FIND IT CONFUSING (12/3/2006, EDM)
	toolsDir = GetProfileString("Application", "Tools Directory", (LPCTSTR)toolsDir);
	matsDir = GetProfileString("Application", "Materials Directory", (LPCTSTR)matsDir);
	*/
	printerFile = GetProfileString("Application", "Printer Configuration File", (LPCTSTR)printerFile);
	modelsDir = GetProfileString("Application", "Models Directory", (LPCTSTR)modelsDir);

	// The main window has been initialized and data loaded, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	// load printer specs
    if(LoadPrinter() == true)
	{
		msg.Format("Loaded printer configuration from %s", printerFile);
		Log(msg);
	}

	// Load Tool definitions (*.tool) in tools directory
	//LoadTools(); // It isn't necessary now to mount the tool right away
	
	// Load Material definitions (*.material) in materials directory
	LoadMaterials();

	UpdateAllViews();

	return TRUE;
}

//--------------------------------------------------------------------
bool CFabAtHomeApp::LoadPrinter(bool bUserSelect)
//--------------------------------------------------------------------
{
	CString startFile, error;

	startFile = printerFile;
	CFileDialog dlg(TRUE, "printer", (LPCTSTR)startFile, NULL,
					"Printer Configuration File (*.printer)|*.printer|All Files (*.*)|*.*||", 0);

	if(bUserSelect)
	{
		if(IDOK != dlg.DoModal()) return false;
		startFile = dlg.GetPathName();
	}

	//make sure that hardware is not running while printer config is changing
	if(printer.bHardwareInitialized) 
	{
		if(IDOK==AfxMessageBox("Shutdown hardware to update configuration?", MB_OKCANCEL))
		{
			if(!printer.ShutdownHardware())
			{
				Log("Unable to shutdown hardware - load failed.");
				return false;
			}
		}
		else return false;
	}

	while(!printer.LoadConfiguration(startFile, error))
	{
		Log("Invalid printer configuration file! Please select a valid printer file.");
		if(dlg.DoModal()==IDOK)
		{
			//update the printer file
			startFile = dlg.GetPathName();
		}
		else
		{
			if(AfxMessageBox("Would you like to quit?", MB_YESNO) == IDYES)
				ExitProcess(0);
		}
	}
	printerFile = startFile;
	return true;
}

// --------------------------------------------------------------------------
bool CFabAtHomeApp::LoadTools(bool bParamsOnly, bool bForceDialog)
// --------------------------------------------------------------------------
{
	//load tools from files, ensuring correct formats and directories
	//if bParamsOnly, then only update the deposition related parameters (see CTool::LoadTools())
	//if bForceDialog, open a file dialog and have the user set the directory

	CString startDir;
	startDir.Format("%s\\*.tool",toolsDir);
	CFileDialog dlg(TRUE, "tool", (LPCTSTR)startDir, NULL,
					"Tool File (*.tool)|*.tool|All Files (*.*)|*.*||", 0);
	if(bForceDialog)
	{
		if(dlg.DoModal()==IDOK)
		{
			//update the tools directory
			startDir = dlg.GetPathName();
			//cut off the file name and the back slash
			int index = startDir.Find(dlg.GetFileName());
			if(index < 0) return false;
			//update the tools directory
			toolsDir = startDir.Left(index-1); //remove name and backslash
		}
		else //user changed mind about selecting files
			return false;
	}
	while(!CTool::LoadTools(toolsDir, bParamsOnly))
	{
		Log("Invalid tool files or directory! Please select a valid tool file.");
		if(dlg.DoModal()==IDOK)
		{
			//update the tools directory
			startDir = dlg.GetPathName();
			//cut off the file name and the back slash
			int index = startDir.Find(dlg.GetFileName());
			if(index < 0) return false;
			//update the tools directory
			toolsDir = startDir.Left(index-1); //remove name and backslash
		}
		else
		{
			//if(IDCANCEL == AfxMessageBox("Tool definitions not successfully loaded.", MB_RETRYCANCEL))
			//{
			Log("Please ensure that valid tool definitions are loaded before building.");
				//return false;
			//}
		}
	}
	//now make sure that there is a valid current (mounted and selected) tool
	//printer.EnsureCurrentTool();
	//if we were successful loading the files or parameters, we need to recalculate model statistics
	if(printer.GetModelPtr() != NULL)
	{
		CSingleLock mdlLock(&printer.GetModelPtr()->m_mutAccess);
		mdlLock.Lock();
		printer.GetModelPtr()->CalcModelStatistics();
	}
	return true;
}

// --------------------------------------------------------------------------
bool CFabAtHomeApp::LoadMaterials()
// --------------------------------------------------------------------------
{
	// It isn't necessary now to load material file
	return true;

	//load materials from files, ensuring correct formats and directories
	while(!CMaterial::LoadMaterials(matsDir))
	{
		Log("Invalid material files or directory! Please select a valid material file.");
		CString startDir;
		startDir.Format("%s\\*.material",matsDir);
		CFileDialog dlg(TRUE, "material", (LPCTSTR)startDir, NULL,
					"Material File (*.material)|*.material|All Files (*.*)|*.*||", 0);
		if(dlg.DoModal()==IDOK)
		{
			//update the tools directory
			startDir = dlg.GetPathName();
			//cut off the file name and the back slash
			int index = startDir.Find(dlg.GetFileName());
			if(index < 0) return false;
			//update the materials directory
			matsDir = startDir.Left(index-1); //remove name and backslash
		}
		else if(IDYES == AfxMessageBox("Would you like to quit?", MB_YESNO))
		{
			PostQuitMessage(0);
			return false;
		}
	}
	return true;
}

// --------------------------------------------------------------------------
bool CFabAtHomeApp::ResetDirectories(CString &prtrFile)
// --------------------------------------------------------------------------
{
	//reset the working directory to the nearest root of printer file
	//reset the tools materials, and model directories to the 
	//tools, materials sudirectories of the working dir, and to the working dir
	//respectively

	//trim off the printer and the last backslash
	int index = prtrFile.Find("\\printer");
	if(index < 0) //not found
	{
		CString msg;
		msg.Format("Error reseting directories from printer file %s",prtrFile);
		Log(msg);
		return false;
	}
	//update the directories and reload the files
	workDir = prtrFile.Left(index);
	toolsDir.Format("%s\\%s", workDir, "tools");
	if(!LoadTools()) return false;
	matsDir.Format("%s\\%s", workDir, "materials");
	if(!LoadMaterials()) return false;
	modelsDir = workDir;
	return true;
}



/////////////////////////////////////////////////////////////////////////////
// CFabAtHomeApp message handlers

// --------------------------------------------------------------------------
void CFabAtHomeApp::RedrawAllViews(void)
// --------------------------------------------------------------------------
{ // cycle through all documents and their views and redraw their views directly
  // does not work across threads (use UpdateAllViews)

	POSITION tmplpos = GetFirstDocTemplatePosition();
	while (tmplpos != NULL) {
		CMultiDocTemplate* pDocTempl = (CMultiDocTemplate*) GetNextDocTemplate(tmplpos);
		POSITION docpos = pDocTempl->GetFirstDocPosition();
		while (docpos != NULL) {
			CFabAtHomeDoc* pDoc = (CFabAtHomeDoc*) pDocTempl->GetNextDoc(docpos);
			pDoc->RedrawViewsNow();
		}
	}

}

// --------------------------------------------------------------------------
void CFabAtHomeApp::UpdateAllViews(void)
// --------------------------------------------------------------------------
{ // cycle through all documents and send update message to all their views

	POSITION tmplpos = GetFirstDocTemplatePosition();
	while (tmplpos != NULL) {
		CMultiDocTemplate* pDocTempl = (CMultiDocTemplate*) GetNextDocTemplate(tmplpos);
		POSITION docpos = pDocTempl->GetFirstDocPosition();
		if (docpos != NULL) {
			CFabAtHomeDoc* pDoc = (CFabAtHomeDoc*) pDocTempl->GetNextDoc(docpos);
			pDoc->UpdateAllViews(0);
		}
	}

}

// --------------------------------------------------------------------------
int CFabAtHomeApp::ExitInstance()
// --------------------------------------------------------------------------
{
	//close the windows and dialogs

	if(printer.bHardwareInitialized)
	{
		if(!printer.Shutdown())
			Sleep(10);
	}
	if (pJogCarriageDlg != NULL) {
		if (pJogCarriageDlg->m_hWnd != NULL)
			pJogCarriageDlg->CloseWindow();
		delete pJogCarriageDlg;
		pJogCarriageDlg = NULL;
	}

	if (pJogToolDlg != NULL) {
		if (pJogToolDlg->m_hWnd != NULL)
			pJogToolDlg->CloseWindow();
		delete pJogToolDlg;
		pJogToolDlg = NULL;
	}

	if (pStatusDlg != NULL) {
		if (pStatusDlg->m_hWnd != NULL)
			pStatusDlg->CloseWindow();
		delete pStatusDlg;
		pStatusDlg = NULL;
	}

	if (pBuildInfoDlg != NULL) {
		if (pBuildInfoDlg->m_hWnd != NULL)
			pBuildInfoDlg->CloseWindow();
		delete pBuildInfoDlg;
		pStatusDlg = NULL;
	}

	
	POSITION tmplpos = GetFirstDocTemplatePosition();
	while (tmplpos != NULL) {
		CMultiDocTemplate* pDocTempl = (CMultiDocTemplate*) GetNextDocTemplate(tmplpos);
		POSITION docpos = pDocTempl->GetFirstDocPosition();
		if (docpos != NULL) {
			CFabAtHomeDoc* pDoc = (CFabAtHomeDoc*) pDocTempl->GetNextDoc(docpos);
			pDoc->OnCloseDocument();
		}
	}

	//write out some vars to the registry entry
	//so they are the same at the next execution
	WriteProfileString("Application", "Printer Configuration File", printerFile);
	WriteProfileString("Application", "Tools Directory", toolsDir);
	WriteProfileString("Application", "Materials Directory", matsDir);
	WriteProfileString("Application", "Models Directory", modelsDir);

	return CWinApp::ExitInstance();
}

// --------------------------------------------------------------------------
void CFabAtHomeApp::OnPrinterJog()
// --------------------------------------------------------------------------
{// jog printer
	if(!printer.EnsureCurrentTool(true))
		return;

	if (pJogCarriageDlg == NULL) 
	{
		pJogCarriageDlg = new CJogCarriageDlg;
		pJogCarriageDlg->Create(IDD_JOGCARRIAGE);
		pJogCarriageDlg->ShowWindow(SW_SHOW);
	} 
	else 
	{
		if(pJogCarriageDlg->IsWindowVisible())
		{
			pJogCarriageDlg->ShowWindow(SW_HIDE);
		}
		else
		{
			pJogCarriageDlg->ShowWindow(SW_SHOW);
		}
	}

}

// --------------------------------------------------------------------------
void CFabAtHomeApp::OnUpdatePrinterJog(CCmdUI *pCmdUI)
// --------------------------------------------------------------------------
{
	pCmdUI->Enable(printer.IsDefined() && (printer.bHardwareInitialized || printer.bEmulateHardware));
	if(pJogCarriageDlg == NULL || (!pJogCarriageDlg->IsWindowVisible()))
	{
		pCmdUI->SetCheck(0);
	}
	else
	{
		pCmdUI->SetCheck(1);
	}
}

// --------------------------------------------------------------------------
void CFabAtHomeApp::OnPrinterJogtool()
// --------------------------------------------------------------------------
{// open or close modeless jog tool dialog

	if(!printer.EnsureCurrentTool(true))
		return;

	if (pJogToolDlg == NULL) {
		pJogToolDlg = new CJogToolDlg;
		pJogToolDlg->Create(IDD_JOGTOOL);
		pJogToolDlg->ShowWindow(SW_SHOW);
	} 
	else 
	{
		if(pJogToolDlg->IsWindowVisible())
		{
			pJogToolDlg->ShowWindow(SW_HIDE);
		}
		else
		{
			pJogToolDlg->ShowWindow(SW_SHOW);
		}
	}
}

// --------------------------------------------------------------------------
void CFabAtHomeApp::OnUpdatePrinterJogtool(CCmdUI *pCmdUI)
// --------------------------------------------------------------------------
{
	pCmdUI->Enable(printer.IsDefined() && (printer.bHardwareInitialized || printer.bEmulateHardware));
	if(pJogToolDlg == NULL || (!pJogToolDlg->IsWindowVisible()))
	{
		pCmdUI->SetCheck(0);
	}
	else
	{
		pCmdUI->SetCheck(1);
	}
}

// --------------------------------------------------------------------------
void CFabAtHomeApp::OnPrinterShowstatus()
// --------------------------------------------------------------------------
{// open or close printer status dialog

	if (pStatusDlg == NULL) 
	{
		pStatusDlg = new CStatusDlg;
		pStatusDlg->Create(IDD_STATUSDLG);
		pStatusDlg->ShowWindow(SW_SHOW);
	} 
	else 
	{
		if(pStatusDlg->IsWindowVisible())
		{
			pStatusDlg->ShowWindow(SW_HIDE);
		}
		else
		{
			pStatusDlg->ShowWindow(SW_SHOW);
		}
	}
}

// --------------------------------------------------------------------------
void CFabAtHomeApp::OnUpdatePrinterShowstatus(CCmdUI *pCmdUI)
// --------------------------------------------------------------------------
{
	if(pStatusDlg == NULL || (!pStatusDlg->IsWindowVisible()))
	{
		pCmdUI->SetText("Show Hardware Status\tShift+H");
	}
	else
	{
		pCmdUI->SetText("Hide Hardware Status\tShift+H");
	}
}

// --------------------------------------------------------------------------
void CFabAtHomeApp::OnViewShowbuildinfo()
// --------------------------------------------------------------------------
{
	// open or close build information dialog

	if (pBuildInfoDlg == NULL)
	{
		pBuildInfoDlg = new CBuildInfoDlg;
		pBuildInfoDlg->Create(IDD_BUILDINFO);
		pBuildInfoDlg->ShowWindow(SW_SHOW);
	} 
	else
	{
		if(pBuildInfoDlg->IsWindowVisible())
		{
			pBuildInfoDlg->ShowWindow(SW_HIDE);
		}
		else
		{
			pBuildInfoDlg->ShowWindow(SW_SHOW);
		}
	}
}

// --------------------------------------------------------------------------
void CFabAtHomeApp::OnUpdateViewShowbuildinfo(CCmdUI *pCmdUI)
// --------------------------------------------------------------------------
{
	if(pBuildInfoDlg == NULL || (!pBuildInfoDlg->IsWindowVisible()))
	{
		pCmdUI->SetText("Show Build Info\tShift+I");
	}
	else
	{
		pCmdUI->SetText("Hide Build Info\tShift+I");
	}
}

// --------------------------------------------------------------------------
void CFabAtHomeApp::OnPrinterInitializehardware()
// --------------------------------------------------------------------------
{
	if (!printer.bHardwareInitialized) {
		if(!printer.IsDefined())
		{
			Log("Please ensure that a valid printer configuration file is loaded before connect the hardware.");
			return;
		}
		else if (!printer.EnsureCurrentTool(true))
		{
			Log("Please ensure that a valid tool configuration file is loaded and mounted before connect the hardware.");
			return;
		}
		printer.InitializeHardware();
	} else {
		//if hardware is shutdown, then we must emulate
		printer.bEmulateHardware = printer.ShutdownHardware();
	}
}

// --------------------------------------------------------------------------
void CFabAtHomeApp::OnUpdatePrinterInitializehardware(CCmdUI *pCmdUI)
// --------------------------------------------------------------------------
{
	pCmdUI->Enable(printer.IsDefined());

	if (printer.bHardwareInitialized){
		pCmdUI->SetText("Disconnect from hardware\tShift+C");
	} else {
		pCmdUI->SetText("Connect to hardware\tShift+C");
	}

}

// --------------------------------------------------------------------------
void CFabAtHomeApp::OnEmulationEmulatehardware()
// --------------------------------------------------------------------------
{
	printer.bEmulateHardware = !printer.bEmulateHardware;
}

// --------------------------------------------------------------------------
void CFabAtHomeApp::OnUpdateEmulationEmulatehardware(CCmdUI *pCmdUI)
// --------------------------------------------------------------------------
{
	pCmdUI->Enable(printer.IsDefined() && printer.bHardwareInitialized);
	pCmdUI->SetCheck(printer.bEmulateHardware);
}

// --------------------------------------------------------------------------
void CFabAtHomeApp::OnPrinterEmulatefaster()
// --------------------------------------------------------------------------
{
	printer.EmulateFaster();
}

// --------------------------------------------------------------------------
void CFabAtHomeApp::OnUpdatePrinterEmulatefaster(CCmdUI *pCmdUI)
// --------------------------------------------------------------------------
{
	// TODO: Add your command update UI handler code here
}

// --------------------------------------------------------------------------
void CFabAtHomeApp::OnPrinterEmulateslower()
// --------------------------------------------------------------------------
{
	printer.EmulateSlower();
}

// --------------------------------------------------------------------------
void CFabAtHomeApp::OnUpdatePrinterEmulateslower(CCmdUI *pCmdUI)
// --------------------------------------------------------------------------
{
	// TODO: Add your command update UI handler code here
}

// --------------------------------------------------------------------------
void CFabAtHomeApp::OnPrinterLoadconfiguration()
// --------------------------------------------------------------------------
{
	if (pPrinterDlg == NULL)
	{
		pPrinterDlg = new CPrinterDlg;
		CRect rect(0, 0, 0, 0);
		CString strTitle;
		strTitle.LoadString(ID_PRINTERDLGTITLE);
		if (!pPrinterDlg->Create(NULL, strTitle, WS_BORDER | WS_POPUP | WS_CAPTION | WS_SYSMENU, rect, m_pMainWnd))
		{
			pPrinterDlg = NULL;
			return;
		}
		pPrinterDlg->CenterWindow();
		pPrinterDlg->ShowWindow(SW_SHOW);
	}
	else 
	{
		if(pPrinterDlg->IsWindowVisible())
		{
			pPrinterDlg->ShowWindow(SW_HIDE);
		}
		else
		{
			pPrinterDlg->ShowWindow(SW_SHOW);
			if(!pPrinterDlg->m_PrinterDlgPropSheet->SetParameters())
				Log("Unable to set parameters.");
		}
	}

	UpdateAllViews();
}

// --------------------------------------------------------------------------
void CFabAtHomeApp::OnUpdatePrinterLoadconfiguration(CCmdUI *pCmdUI)
// --------------------------------------------------------------------------
{
	if (pPrinterDlg == NULL)
		pCmdUI->SetCheck(0);
	else
	{
		pCmdUI->Enable();
		pCmdUI->SetCheck(pPrinterDlg->IsWindowVisible());
	}
}

// --------------------------------------------------------------------------
void CFabAtHomeApp::OnPrinterEnablemotion()
// --------------------------------------------------------------------------
{
	if(!printer.IsStarted())
		printer.Start(); //start toggles
	else
		printer.Start();
}

// --------------------------------------------------------------------------
void CFabAtHomeApp::OnUpdatePrinterEnablemotion(CCmdUI *pCmdUI)
// --------------------------------------------------------------------------
{
	pCmdUI->Enable(printer.bHardwareInitialized);
	//use printer.IsStarted() here because we need the ground truth
	if(printer.IsStarted())
		pCmdUI->SetText("Disable Amplifiers");
	else
		pCmdUI->SetText("Enable Amplifiers");
}

// --------------------------------------------------------------------------
void CFabAtHomeApp::OnPrinterPausequeue()
// --------------------------------------------------------------------------
{
	if(printer.IsPaused())
		printer.ResumeQueue();
	else
		printer.PauseQueue();
}

// --------------------------------------------------------------------------
void CFabAtHomeApp::OnUpdatePrinterPausequeue(CCmdUI *pCmdUI)
// --------------------------------------------------------------------------
{
	pCmdUI->Enable(printer.bHardwareInitialized);
	if(printer.IsPaused())
		pCmdUI->SetText("Resume Queue");
	else
		pCmdUI->SetText("Pause Queue");
}

// --------------------------------------------------------------------------
void CFabAtHomeApp::OnPrinterAbortallmotion()
// --------------------------------------------------------------------------
{
	printer.Abort();
}

// --------------------------------------------------------------------------
void CFabAtHomeApp::OnUpdatePrinterAbortallmotion(CCmdUI *pCmdUI)
// --------------------------------------------------------------------------
{
	pCmdUI->Enable(printer.bHardwareInitialized);
	if(printer.IsAborted())
		pCmdUI->SetText("Clear Hardware Abort");
	else
		pCmdUI->SetText("Hardware Abort");
}

// --------------------------------------------------------------------------
void CFabAtHomeApp::OnPrinterResethardware()
// --------------------------------------------------------------------------
{
	printer.Reset();
}

// --------------------------------------------------------------------------
void CFabAtHomeApp::OnUpdatePrinterResethardware(CCmdUI *pCmdUI)
// --------------------------------------------------------------------------
{
	pCmdUI->Enable(printer.bHardwareInitialized);
}

// --------------------------------------------------------------------------
void CFabAtHomeApp::OnPrinterEmergencystop()
// --------------------------------------------------------------------------
{
	if(printer.IsEStop()) 
		printer.ClearEStop();
	else 
		printer.Stop();
}

// --------------------------------------------------------------------------
void CFabAtHomeApp::OnUpdatePrinterEmergencystop(CCmdUI *pCmdUI)
// --------------------------------------------------------------------------
{
	pCmdUI->Enable(true);
	if(printer.IsEStop())
		pCmdUI->SetText("Clear Emergency Stop");
	else
		pCmdUI->SetText("Emergency Stop");
}


// --------------------------------------------------------------------------
CTool &CFabAtHomeApp::GetTool(int index)
// --------------------------------------------------------------------------
{
	//return the indexed tool from the tool array
	ASSERT(index > (tool.GetSize() - 1));
	return tool[index];
}

// --------------------------------------------------------------------------
void CFabAtHomeApp::OnToolLoadfiles()
// --------------------------------------------------------------------------
{
	//have the user select the location of the tool files
	//and reload all tool information from that directory
	LoadTools(false, true); //(false, true) -> (all data, force dialog box)
}

// --------------------------------------------------------------------------
void CFabAtHomeApp::OnToolRefreshparameters()
// --------------------------------------------------------------------------
{
	//reload the parameters of all of the tool files in the tools directory
	//into the application's tool array, and add any new tools
	//this can happen online, and should have immediate effect
	LoadTools(true); //(true, default) -> (params only, no dialog necessary)
}

// --------------------------------------------------------------------------
void CFabAtHomeApp::OnUpdateToolLoadfiles(CCmdUI *pCmdUI)
// --------------------------------------------------------------------------
{
	//can load files if paused or not building, or if no model yet
	if(printer.GetModelPtr() != NULL)
		pCmdUI->Enable(printer.GetModelPtr()->fab.IsPaused() || 
					!(printer.GetModelPtr()->fab.IsFabricating()));
	else
		pCmdUI->Enable();
}	

// --------------------------------------------------------------------------
void CFabAtHomeApp::OnToolChangetools()
// --------------------------------------------------------------------------
{
	//open the model Change Tools dialog
	UserToolChange();
}

// --------------------------------------------------------------------------
void CFabAtHomeApp::UserToolChange()
// --------------------------------------------------------------------------
{
	if (pToolDlg == NULL)
	{
		pToolDlg = new CToolDlg;
		CRect rect(0, 0, 0, 0);
		CString strTitle;
		strTitle.LoadString(ID_TOOLDLGTITLE);
		if (!pToolDlg->Create(NULL, strTitle, WS_BORDER | WS_POPUP | WS_CAPTION | WS_SYSMENU, rect, m_pMainWnd))
		{
			pToolDlg = NULL;
			return;
		}
		pToolDlg->CenterWindow();
		pToolDlg->ShowWindow(SW_SHOW);
	}
	// always show the dialog
	else if(!pToolDlg->IsWindowVisible())
		pToolDlg->ShowWindow(SW_SHOW);

	//UpdateAllViews();

	return;
}

// --------------------------------------------------------------------------
void CFabAtHomeApp::OnUpdateToolChangetools(CCmdUI *pCmdUI)
// --------------------------------------------------------------------------
{
	//can change tools if paused or not building, or if no model yet
	if(printer.GetModelPtr() != NULL)
		pCmdUI->Enable(printer.GetModelPtr()->fab.IsPaused() ||
						!(printer.GetModelPtr()->fab.IsFabricating()));
	else
		pCmdUI->Enable();
}

// --------------------------------------------------------------------------
void CFabAtHomeApp::OnUpdatePrinterLog(CCmdUI *pCmdUI)
// --------------------------------------------------------------------------
{
	if(printer.IsLogging())
		pCmdUI->SetText("Stop Logging");
	else
		pCmdUI->SetText("Generate Log File");
}

// --------------------------------------------------------------------------
void CFabAtHomeApp::OnPrinterLog()
// --------------------------------------------------------------------------
{
	if(printer.IsLogging())
	{
		printer.StopLog();
	}
	else
	{
		CString filename;
		filename.Format("LogFile.dat");
		printer.StartLog(filename);
	}
		
}

// --------------------------------------------------------------------------
void CFabAtHomeApp::OnToolToolconfiguration()
// --------------------------------------------------------------------------
{
	if(!printer.IsDefined())
	{
		Log("Please ensure that a valid printer configuration file is loaded before open the Tool Configuration dialog.");
		return;
	}

	if (pToolDlg == NULL)
	{
		pToolDlg = new CToolDlg;
		CRect rect(0, 0, 0, 0);
		CString strTitle;
		strTitle.LoadString(ID_TOOLDLGTITLE);
		if (!pToolDlg->Create(NULL, strTitle, WS_BORDER | WS_POPUP | WS_CAPTION | WS_SYSMENU, rect, m_pMainWnd))
		{
			pToolDlg = NULL;
			return;
		}
		pToolDlg->CenterWindow();
		pToolDlg->ShowWindow(SW_SHOW);
	}
	else 
	{
		if(pToolDlg->IsWindowVisible())
		{
			pToolDlg->ShowWindow(SW_HIDE);
		}
		else
		{
			pToolDlg->ShowWindow(SW_SHOW);
		}
	}

	//UpdateAllViews();
}

// --------------------------------------------------------------------------
void CFabAtHomeApp::OnUpdateToolToolconfiguration(CCmdUI *pCmdUI)
// --------------------------------------------------------------------------
{
	if (pToolDlg == NULL)
		pCmdUI->SetCheck(0);
	else
	{
		pCmdUI->Enable();
		pCmdUI->SetCheck(pToolDlg->IsWindowVisible());
	}
}

// --------------------------------------------------------------------------
void CFabAtHomeApp::Log(CString appendtext)
// --------------------------------------------------------------------------
{
	CMainFrame *pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);

	CString text;
	pFrame->m_wndLogBar.m_wndChild.GetWindowTextA(text);
	text.AppendFormat("%s\r\n", appendtext);
	pFrame->m_wndLogBar.m_wndChild.SetWindowTextA(text);
	//pFrame->m_wndLogBar.m_wndChild.SetFocus();
	pFrame->m_wndLogBar.m_wndChild.SetSel(text.GetLength(), text.GetLength());
}

// --------------------------------------------------------------------------
void CFabAtHomeApp::HideToolDlgPropSheet()
// --------------------------------------------------------------------------
{
	if (pToolDlg != NULL)
		pToolDlg->ShowWindow(SW_HIDE);
}

// --------------------------------------------------------------------------
void CFabAtHomeApp::HidePrinterDlgPropSheet()
// --------------------------------------------------------------------------
{
	if (pPrinterDlg != NULL)
		pPrinterDlg->ShowWindow(SW_HIDE);
}
// --------------------------------------------------------------------------
void CFabAtHomeApp::OnPrinterNetworkconnect()
// --------------------------------------------------------------------------
{
	if(printer.IsNetworked()) printer.NetworkConnect(false);
	else
		printer.StartNetworkThread();
}

// --------------------------------------------------------------------------
void CFabAtHomeApp::OnUpdatePrinterNetworkconnect(CCmdUI *pCmdUI)
// --------------------------------------------------------------------------
{
	pCmdUI->Enable();
	if(printer.IsNetworked())
	{
		pCmdUI->SetText("Network Disconnect");
	}
	else
	{
		pCmdUI->SetText("Network Connect");
	}
}

// --------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

// --------------------------------------------------------------------------
class CAboutDlg : public CDialog
// --------------------------------------------------------------------------
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CStatic m_stFAHLogo;
	CStatic m_stCCSLLogo;
};

// --------------------------------------------------------------------------
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
// --------------------------------------------------------------------------
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

// --------------------------------------------------------------------------
BOOL CAboutDlg::OnInitDialog()
// --------------------------------------------------------------------------
{
	CDialog::OnInitDialog();
	return TRUE;
}

// --------------------------------------------------------------------------
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
// --------------------------------------------------------------------------
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_STATIC_FAHLOGO, m_stFAHLogo);
	DDX_Control(pDX, IDC_STATIC_CCSLLOGO, m_stCCSLLogo);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
// --------------------------------------------------------------------------
void CFabAtHomeApp::OnAppAbout()
// --------------------------------------------------------------------------
{
	CAboutDlg aboutDlg;
	//the following code is not working to produce static counters and timestamps
	//for the build
	//aboutDlg.buildstr.Format("Build %d on %s", __COUNTER__ , __TIMESTAMP__);
	aboutDlg.DoModal();
}