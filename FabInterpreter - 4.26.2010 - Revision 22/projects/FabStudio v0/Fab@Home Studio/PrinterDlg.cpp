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

// PrinterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FabAtHome.h"
#include "PrinterDlg.h"



// --------------------------------------------------------------------------
// CPrinterDlgGeneral
// --------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CPrinterDlgGeneral, CPropertyPage)

// --------------------------------------------------------------------------
CPrinterDlgGeneral::CPrinterDlgGeneral()
	: CPropertyPage(CPrinterDlgGeneral::IDD)
// --------------------------------------------------------------------------
{
	m_pToolTip = NULL;
}

// --------------------------------------------------------------------------
CPrinterDlgGeneral::~CPrinterDlgGeneral()
// --------------------------------------------------------------------------
{
	delete m_pToolTip;
}

// --------------------------------------------------------------------------
void CPrinterDlgGeneral::DoDataExchange(CDataExchange* pDX)
// --------------------------------------------------------------------------
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DEFAULT_POD, m_DEFAULT_POD);
	DDX_Control(pDX, IDC_DESCRIPTION, m_DESCRIPTION);
	DDX_Control(pDX, IDC_JOGSPEED, m_JOGSPEED);
	DDX_Control(pDX, IDC_MAXTOOLS, m_MAXTOOLS);
	DDX_Control(pDX, IDC_Name, m_Name);
	DDX_Control(pDX, IDC_STATUS_UPDATE_PERIOD, m_STATUS_UPDATE_PERIOD);
	DDX_Control(pDX, IDC_TOOLLIMITSWITCH, m_TOOLLIMITSWITCH);
}

// --------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CPrinterDlgGeneral, CPropertyPage)
END_MESSAGE_MAP()

// --------------------------------------------------------------------------
BOOL CPrinterDlgGeneral::OnInitDialog()
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
	m_pToolTip->AddTool(&m_DEFAULT_POD,"X,Y,Z coords of point of deposition in graphics coords");
	m_pToolTip->AddTool(&m_DESCRIPTION,"Description of the printer configuration file");
	m_pToolTip->AddTool(&m_JOGSPEED,"speed of motion for non-build motions, mm/s");
	m_pToolTip->AddTool(&m_MAXTOOLS,"maximum number of tools that can be mounted simultaneously (e.g. 2 for 2-syringe tool)");
	m_pToolTip->AddTool(&m_Name,"Name of the printer configuration file");
	m_pToolTip->AddTool(&m_STATUS_UPDATE_PERIOD,"period of status update comms with microcontroller, in ms");
	m_pToolTip->AddTool(&m_TOOLLIMITSWITCH,"-U direction and -V direction (0 -> limit switch not connected; 1 if it is)");
	
	m_pToolTip->Activate(TRUE);
	
	return bResult;
}

// --------------------------------------------------------------------------
BOOL CPrinterDlgGeneral::PreTranslateMessage(MSG* pMsg)
// --------------------------------------------------------------------------
{
	if (m_pToolTip != NULL)
		m_pToolTip->RelayEvent(pMsg);

	return CPropertyPage::PreTranslateMessage(pMsg);
}

// --------------------------------------------------------------------------





// --------------------------------------------------------------------------
// CPrinterDlgComponent
// --------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CPrinterDlgComponent, CPropertyPage)

// --------------------------------------------------------------------------
CPrinterDlgComponent::CPrinterDlgComponent()
	: CPropertyPage(CPrinterDlgComponent::IDD)
// --------------------------------------------------------------------------
{
	m_pToolTip = NULL;
}

// --------------------------------------------------------------------------
CPrinterDlgComponent::~CPrinterDlgComponent()
// --------------------------------------------------------------------------
{
	delete m_pToolTip;
}

// --------------------------------------------------------------------------
void CPrinterDlgComponent::DoDataExchange(CDataExchange* pDX)
// --------------------------------------------------------------------------
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_APPENDGEOM, m_APPENDGEOM);
	DDX_Control(pDX, IDC_COLOR, m_COLOR);
	DDX_Control(pDX, IDC_COMPONENT, m_COMPONENT);
	DDX_Control(pDX, IDC_DIRECTION, m_DIRECTION);
	DDX_Control(pDX, IDC_INCREMENT, m_INCREMENT);
	DDX_Control(pDX, IDC_LIMIT_SWITCH, m_LIMIT_SWITCH);
	DDX_Control(pDX, IDC_MOTOR, m_MOTOR);
	DDX_Control(pDX, IDC_OFFSET, m_OFFSET);
	DDX_Control(pDX, IDC_RANGE, m_RANGE);
}

// --------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CPrinterDlgComponent, CPropertyPage)
END_MESSAGE_MAP()

// --------------------------------------------------------------------------
BOOL CPrinterDlgComponent::OnInitDialog()
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
	m_pToolTip->AddTool(&m_APPENDGEOM,"name of the stl file describing the appended geometry");
	m_pToolTip->AddTool(&m_COLOR,"R G B Alpha in range 0-1 (alpha=transparency)");
	m_pToolTip->AddTool(&m_COMPONENT,"name of the stl file describing the component geometry");
	m_pToolTip->AddTool(&m_DIRECTION,"ref# dx dy dz - orientation of graphical display of component");
	m_pToolTip->AddTool(&m_INCREMENT,"distance to move axis in manual jog");
	m_pToolTip->AddTool(&m_LIMIT_SWITCH,"+direction -direction (0 -> limit switch not connected; 1 if it is)");
	m_pToolTip->AddTool(&m_MOTOR,"amp axis#, axis dir(+/-1), mm per (1/8) step");
	m_pToolTip->AddTool(&m_OFFSET,"dx dy dz graphics offset of the component");
	m_pToolTip->AddTool(&m_RANGE,"[mm] start end home");
	
	m_pToolTip->Activate(TRUE);
	
	return bResult;
}

// --------------------------------------------------------------------------
BOOL CPrinterDlgComponent::PreTranslateMessage(MSG* pMsg)
// --------------------------------------------------------------------------
{
	if (m_pToolTip != NULL)
		m_pToolTip->RelayEvent(pMsg);

	return CPropertyPage::PreTranslateMessage(pMsg);
}

// --------------------------------------------------------------------------





// --------------------------------------------------------------------------
// CPrinterDlgPropSheet
// --------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CPrinterDlgPropSheet, CPropertySheet)

// --------------------------------------------------------------------------
CPrinterDlgPropSheet::CPrinterDlgPropSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
// --------------------------------------------------------------------------
{
	pApp = ((CFabAtHomeApp *)AfxGetApp());
	for(int i = 0; i < MAX_COMPONENTS; i++)
		AddPage(&m_PrinterDlgGeneral);
}

// --------------------------------------------------------------------------
CPrinterDlgPropSheet::CPrinterDlgPropSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
// --------------------------------------------------------------------------
{
	pApp = ((CFabAtHomeApp *)AfxGetApp());
	AddPage(&m_PrinterDlgGeneral);
	for(int i = 0; i < MAX_COMPONENTS; i++)
		AddPage(&m_PrinterDlgComponent[i]);
}

// --------------------------------------------------------------------------
CPrinterDlgPropSheet::~CPrinterDlgPropSheet()
// --------------------------------------------------------------------------
{
}

// --------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CPrinterDlgPropSheet, CPropertySheet)
	ON_BN_CLICKED(IDD_SAVE_BUTTON, OnBnClickedButtonSave)
	ON_BN_CLICKED(IDD_CANCEL_BUTTON, OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDD_CLOSE_BUTTON, OnBnClickedButtonClose)
	ON_BN_CLICKED(IDD_REFRESH_BUTTON, OnBnClickedButtonRefresh)
	ON_BN_CLICKED(IDD_IMPORT_BUTTON, OnBnClickedButtonImport)
	ON_BN_CLICKED(IDD_EXPORT_BUTTON, OnBnClickedButtonExport)
END_MESSAGE_MAP()

// --------------------------------------------------------------------------
BOOL CPrinterDlgPropSheet::OnInitDialog()
// --------------------------------------------------------------------------
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	TC_ITEM item;
	item.mask = TCIF_TEXT;
	item.pszText = "Base";
	GetTabControl()->SetItem(1, &item);
	item.pszText = "X-Carriage";
	GetTabControl()->SetItem(2, &item);
	item.pszText = "Y-Carriage";
	GetTabControl()->SetItem(3, &item);
	item.pszText = "Z-Carriage";
	GetTabControl()->SetItem(4, &item);

	CRect rectWnd;
	GetWindowRect(rectWnd);
	SetWindowPos(NULL, 0, 0, rectWnd.Width() + 90, rectWnd.Height(), SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

	Save.CreateEx(WS_EX_CLIENTEDGE, _T("BUTTON"), "Save", WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		rectWnd.Width(), 27, 80, 27, m_hWnd, (HMENU)IDD_SAVE_BUTTON, 0);
	Save.SetFont(GetFont());
	Cancel.CreateEx(WS_EX_CLIENTEDGE, _T("BUTTON"), "Cancel", WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		rectWnd.Width(), 57, 80, 27, m_hWnd, (HMENU)IDD_CANCEL_BUTTON, 0);
	Cancel.SetFont(GetFont());
	Close.CreateEx(WS_EX_CLIENTEDGE, _T("BUTTON"), "Close", WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		rectWnd.Width(), 87, 80, 27, m_hWnd, (HMENU)IDD_CLOSE_BUTTON, 0);
	Close.SetFont(GetFont());
	Refresh.CreateEx(WS_EX_CLIENTEDGE, _T("BUTTON"), "Refresh", WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		rectWnd.Width(), 117, 80, 27, m_hWnd, (HMENU)IDD_REFRESH_BUTTON, 0);
	Refresh.SetFont(GetFont());
	Import.CreateEx(WS_EX_CLIENTEDGE, _T("BUTTON"), "Import", WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		rectWnd.Width(), 147, 80, 27, m_hWnd, (HMENU)IDD_IMPORT_BUTTON, 0);
	Import.SetFont(GetFont());
	Export.CreateEx(WS_EX_CLIENTEDGE, _T("BUTTON"), "Export", WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		rectWnd.Width(), 177, 80, 27, m_hWnd, (HMENU)IDD_EXPORT_BUTTON, 0);
	Export.SetFont(GetFont());

	CenterWindow();

	//Set up the tooltip
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

	if(!SetParameters())
		pApp->Log("Unable to set parameters.");

	return bResult;
}

// --------------------------------------------------------------------------
void CPrinterDlgPropSheet::OnBnClickedButtonSave()
// --------------------------------------------------------------------------
{
	//make sure that hardware is not running while printer config is changing
	if(pApp->printer.bHardwareInitialized) 
	{
		if(IDOK==AfxMessageBox("Shutdown hardware to save configuration?", MB_OKCANCEL))
		{
			if(!pApp->printer.ShutdownHardware())
			{
				pApp->Log("Unable to shutdown hardware - load failed.");
				return;
			}
		}
		else
		{
			pApp->Log("Unable to save the printer configuration file.");
			return;
		}
	}

	if(!SaveParameters() || !RefreshParameters())
		pApp->Log("Unable to save parameters.");
	else
		pApp->Log("Parameters saved successfully.");
}

// --------------------------------------------------------------------------
void CPrinterDlgPropSheet::OnBnClickedButtonCancel()
// --------------------------------------------------------------------------
{
	//make sure that hardware is not running while printer config is changing
	if(pApp->printer.bHardwareInitialized) 
	{
		if(IDOK==AfxMessageBox("Shutdown hardware to restore configuration?", MB_OKCANCEL))
		{
			if(!pApp->printer.ShutdownHardware())
			{
				pApp->Log("Unable to shutdown hardware - load failed.");
				pApp->HidePrinterDlgPropSheet();
				return;
			}
		}
		else
		{
			pApp->Log("Unable to restore the original parameters.");
			pApp->HidePrinterDlgPropSheet();
			return;
		}
	}

	CString oldPrinterFile = pApp->printerFile;
	CString msg;
	if (!pApp->LoadPrinter(false)) //true forces a user file dialog
		pApp->Log("Unable to restore original parameters.");

	if(pApp->printerFile.Compare(oldPrinterFile)!=0)
	{
		pApp->Log("Your printer directory has changed. The tool directory was reset.");
		pApp->ResetDirectories(pApp->printerFile);
		if (pApp->pToolDlg != NULL)
		{
			pApp->pToolDlg->DestroyWindow();
			pApp->pToolDlg = NULL;
		}
	}

	// Instead of closing the modeless property sheet, just hide it.
	pApp->HidePrinterDlgPropSheet();
}

// --------------------------------------------------------------------------
void CPrinterDlgPropSheet::OnBnClickedButtonClose()
// --------------------------------------------------------------------------
{
	// Instead of closing the modeless property sheet, just hide it.
	pApp->HidePrinterDlgPropSheet();
}

// --------------------------------------------------------------------------
void CPrinterDlgPropSheet::OnBnClickedButtonRefresh()
// --------------------------------------------------------------------------
{
	//make sure that hardware is not running while printer config is changing
	if(pApp->printer.bHardwareInitialized) 
	{
		if(IDOK==AfxMessageBox("Shutdown hardware to update configuration?", MB_OKCANCEL))
		{
			if(!pApp->printer.ShutdownHardware())
			{
				pApp->Log("Unable to shutdown hardware - load failed.");
				return;
			}
		}
		else
		{
			pApp->Log("Unable to refresh the parameters.");
			return;
		}
	}

	if(!RefreshParameters())
		pApp->Log("Unable to refresh parameters.");
	else
		pApp->Log("Parameters refreshed successfully.");
}

// --------------------------------------------------------------------------
void CPrinterDlgPropSheet::OnBnClickedButtonImport()
// --------------------------------------------------------------------------
{
	//make sure that hardware is not running while printer config is changing
	if(pApp->printer.bHardwareInitialized) 
	{
		if(IDOK==AfxMessageBox("Shutdown hardware to update configuration?", MB_OKCANCEL))
		{
			if(!pApp->printer.ShutdownHardware())
			{
				pApp->Log("Unable to shutdown hardware - load failed.");
				return;
			}
		}
		else
		{
			pApp->Log("Unable to load new printer configuration file.");
			return;
		}
	}

	CString startFile, error;

	startFile = pApp->printerFile;
	CFileDialog dlg(TRUE, "printer", (LPCTSTR)startFile, NULL,
					"Import Printer Configuration File (*.printer)|*.printer|All Files (*.*)|*.*||", 0);
	dlg.GetOFN().lpstrTitle = "Import Printer Configuration File";

	if(IDOK != dlg.DoModal()) 
		return;
	startFile = dlg.GetPathName();

	if(!pApp->printer.LoadConfiguration(startFile, error))
	{
		pApp->Log("Invalid printer configuration file.");
			return;
	}
	
	if(pApp->printerFile.Compare(startFile)!=0)
	{
		pApp->Log("Your printer directory has changed. The tool directory was reset.");
		pApp->ResetDirectories(pApp->printerFile);
		if (pApp->pToolDlg != NULL)
		{
			pApp->pToolDlg->DestroyWindow();
			pApp->pToolDlg = NULL;
		}
	}

	pApp->printerFile = startFile;

	//Updates the parameters in the PrinterDlg dialog
	if(!SetParameters())
		pApp->Log("Unable to set the new parameters.");
	else
		pApp->Log("Printer configuration file imported successfully.");
}

//////////////////////////////////////////////////////////////////////////////////////////
// --------------------------------------------------------------------------
void CPrinterDlgPropSheet::OnBnClickedButtonExport()
// --------------------------------------------------------------------------
{
   
	CFileDialog dlg(FALSE, "printer", (LPCTSTR)pApp->printerFile, NULL,
			"Export Printer Configuration File (*.printer)|*.printer|All Files (*.*)|*.*||", 0);
	dlg.GetOFN().lpstrTitle = "Export Printer Configuration File";
	dlg.GetOFN().Flags |= OFN_OVERWRITEPROMPT;
	if(dlg.DoModal()==IDOK)
	{
		if(!SaveParameters(dlg.GetPathName()))
			pApp->Log("Unable to save printer configuration file.");
		else
			pApp->Log("Printer configuration file saved successfully.");
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////

// --------------------------------------------------------------------------
BOOL CPrinterDlgPropSheet::PreTranslateMessage(MSG* pMsg)
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
bool CPrinterDlgPropSheet::SetParameters(void)
// --------------------------------------------------------------------------
{
	CString temp;
	CPropertyPage *temppage;
	temppage = GetActivePage();

	SetActivePage(&m_PrinterDlgGeneral);

	//DEFAULT_POD parameter
	temp.Format("%lf %lf %lf", pApp->printer.m_vPOD.x, pApp->printer.m_vPOD.y, pApp->printer.m_vPOD.z);
	m_PrinterDlgGeneral.m_DEFAULT_POD.SetWindowTextA(temp);

	//DESCRIPTION parameter
	m_PrinterDlgGeneral.m_DESCRIPTION.SetWindowTextA(pApp->printer.description);

	//JOGSPEED parameter
	temp.Format("%lf", pApp->printer.GetJogSpeed());
	m_PrinterDlgGeneral.m_JOGSPEED.SetWindowTextA(temp);

	//MAXTOOLS parameter
	temp.Format("%d", pApp->printer.GetMaxTools());
	m_PrinterDlgGeneral.m_MAXTOOLS.SetWindowTextA(temp);

	//Name parameter
	m_PrinterDlgGeneral.m_Name.SetWindowTextA(pApp->printer.name);

	//STATUS_UPDATE_PERIOD parameter
	temp.Format("%lu", pApp->printer.GetUserStatusPeriod());
	m_PrinterDlgGeneral.m_STATUS_UPDATE_PERIOD.SetWindowTextA(temp);

	//TOOLLIMITSWITCH parameter
	temp.Format("%d %d", pApp->printer.m_arBLimitSwitch[2*AXIS_U+1], pApp->printer.m_arBLimitSwitch[2*AXIS_V+1]);
	m_PrinterDlgGeneral.m_TOOLLIMITSWITCH.SetWindowTextA(temp);

	for(int i = 0; i < MAX_COMPONENTS; i++)
	{
		SetActivePage(&m_PrinterDlgComponent[i]);

		//APPENDGEOM parameter
		m_PrinterDlgComponent[i].m_APPENDGEOM.SetWindowTextA(pApp->printer.component[i].component_appendgeom);

		//COLOR parameter
		temp.Format("%lf %lf %lf %lf", pApp->printer.component[i].color.x, pApp->printer.component[i].color.y, 
			pApp->printer.component[i].color.z, pApp->printer.component[i].alpha);
		m_PrinterDlgComponent[i].m_COLOR.SetWindowTextA(temp);

		//COMPONENT parameter
		m_PrinterDlgComponent[i].m_COMPONENT.SetWindowTextA(pApp->printer.component[i].component_geom);

		//DIRECTION parameter
		temp.Format("%ld %lf %lf %lf", pApp->printer.component[i].refidx, pApp->printer.component[i].dir.x, 
			pApp->printer.component[i].dir.y, pApp->printer.component[i].dir.z);
		m_PrinterDlgComponent[i].m_DIRECTION.SetWindowTextA(temp);

		//INCREMENT parameter
		temp.Format("%lf", pApp->printer.component[i].jogIncrement);
		m_PrinterDlgComponent[i].m_INCREMENT.SetWindowTextA(temp);

		//LIMIT_SWITCH parameter
		temp.Format("%d %d", pApp->printer.component[i].posLimitSwitch, pApp->printer.component[i].negLimitSwitch); 
		m_PrinterDlgComponent[i].m_LIMIT_SWITCH.SetWindowTextA(temp);

		//MOTOR parameter
		temp.Format("%ld %ld %lf", pApp->printer.component[i].axis, pApp->printer.component[i].axisDir, pApp->printer.component[i].mmps);
		m_PrinterDlgComponent[i].m_MOTOR.SetWindowTextA(temp);

		//OFFSET parameter
		temp.Format("%lf %lf %lf", pApp->printer.component[i].offset.x, pApp->printer.component[i].offset.y, pApp->printer.component[i].offset.z);
		m_PrinterDlgComponent[i].m_OFFSET.SetWindowTextA(temp);

		//RANGE parameter
		temp.Format("%lf %lf %lf", pApp->printer.component[i].rangemin, pApp->printer.component[i].rangemax, pApp->printer.component[i].home);
		m_PrinterDlgComponent[i].m_RANGE.SetWindowTextA(temp);
	}

	SetActivePage(temppage);

	return true;
}

// --------------------------------------------------------------------------
bool CPrinterDlgPropSheet::ClearParameters(void)
// --------------------------------------------------------------------------
{
	CString temp = "";
	CPropertyPage *temppage;
	temppage = GetActivePage();

	SetActivePage(&m_PrinterDlgGeneral);

	//DEFAULT_POD parameter
	m_PrinterDlgGeneral.m_DEFAULT_POD.SetWindowTextA(temp);

	//DESCRIPTION parameter
	m_PrinterDlgGeneral.m_DESCRIPTION.SetWindowTextA(temp);

	//JOGSPEED parameter
	m_PrinterDlgGeneral.m_JOGSPEED.SetWindowTextA(temp);

	//MAXTOOLS parameter
	m_PrinterDlgGeneral.m_MAXTOOLS.SetWindowTextA(temp);

	//Name parameter
	m_PrinterDlgGeneral.m_Name.SetWindowTextA(temp);

	//STATUS_UPDATE_PERIOD parameter
	m_PrinterDlgGeneral.m_STATUS_UPDATE_PERIOD.SetWindowTextA(temp);

	//TOOLLIMITSWITCH parameter
	m_PrinterDlgGeneral.m_TOOLLIMITSWITCH.SetWindowTextA(temp);

	for(int i = 0; i < MAX_COMPONENTS; i++)
	{
		SetActivePage(&m_PrinterDlgComponent[i]);

		//APPENDGEOM parameter
		m_PrinterDlgComponent[i].m_APPENDGEOM.SetWindowTextA(temp);

		//COLOR parameter
		m_PrinterDlgComponent[i].m_COLOR.SetWindowTextA(temp);

		//COMPONENT parameter
		m_PrinterDlgComponent[i].m_COMPONENT.SetWindowTextA(temp);

		//DIRECTION parameter
		m_PrinterDlgComponent[i].m_DIRECTION.SetWindowTextA(temp);

		//INCREMENT parameter
		m_PrinterDlgComponent[i].m_INCREMENT.SetWindowTextA(temp);

		//LIMIT_SWITCH parameter
		m_PrinterDlgComponent[i].m_LIMIT_SWITCH.SetWindowTextA(temp);

		//MOTOR parameter
		m_PrinterDlgComponent[i].m_MOTOR.SetWindowTextA(temp);

		//OFFSET parameter
		m_PrinterDlgComponent[i].m_OFFSET.SetWindowTextA(temp);

		//RANGE parameter
		m_PrinterDlgComponent[i].m_RANGE.SetWindowTextA(temp);
	}

	SetActivePage(temppage);

	return true;
}

// --------------------------------------------------------------------------
bool CPrinterDlgPropSheet::RefreshParameters(void)
// --------------------------------------------------------------------------
{
	CString temp;
	CPropertyPage *temppage;
	temppage = GetActivePage();
	int j, i_temp;
	double d_temp;
	DWORD unsigi_temp;

	SetActivePage(&m_PrinterDlgGeneral);

	pApp->printer.Clear();

	for(int i = 0; i < MAX_COMPONENTS; i++)
	{
		SetActivePage(&m_PrinterDlgComponent[i]);

		//COMPONENT parameter
		m_PrinterDlgComponent[i].m_COMPONENT.GetWindowTextA(temp);
		if(!temp.IsEmpty())
		{
			CString appendgeomfile = CUtils::RemoveComment(temp);
			CString appendgeompath = CUtils::GetPath(pApp->printerFile)+"\\"+appendgeomfile;
			CPrinterComponent comp;
			if (_access(appendgeompath,4) != 0 || !comp.geom.LoadSTL(appendgeompath, 10000))
				return false;
			comp.geom.ComputeBoundingBox(comp.pmin, comp.pmax);
			pApp->printer.component.Add(comp);
			pApp->printer.component[i].component_geom = appendgeomfile;
		}

		//APPENDGEOM parameter
		m_PrinterDlgComponent[i].m_APPENDGEOM.GetWindowTextA(temp);
		if(!temp.IsEmpty())
		{
			CString geomfile = CUtils::RemoveComment(temp);
			pApp->printer.component[i].component_appendgeom = geomfile;
			CString geompath = CUtils::GetPath(pApp->printerFile)+"\\"+geomfile;
			CGeometry tmpgeom;
			bool ok = tmpgeom.LoadSTL(geompath, 10000);
			if (ok)
			{
				pApp->printer.component[i].geom.AppendGeometry(tmpgeom);
				pApp->printer.component[i].geom.ComputeBoundingBox(pApp->printer.component[i].pmin, pApp->printer.component[i].pmax);
			} else
				return false;
			pApp->printer.component[i].component_appendgeom = geomfile;
		}

		//COLOR parameter
		m_PrinterDlgComponent[i].m_COLOR.GetWindowTextA(temp);
		temp.Replace(","," ");
		j = sscanf_s(temp,"%lf %lf %lf %lf", &pApp->printer.component[i].color.x, &pApp->printer.component[i].color.y, 
			&pApp->printer.component[i].color.z, &pApp->printer.component[i].alpha);
		if (j != 4) {
			pApp->printer.component[i].color = CVec(200,200,200);
			pApp->printer.component[i].alpha = 255;
			return false;
		}

		//DIRECTION parameter
		m_PrinterDlgComponent[i].m_DIRECTION.GetWindowTextA(temp);
		temp.Replace(","," ");
		int j=sscanf_s(temp,"%ld %lf %lf %lf", 
			&pApp->printer.component[i].refidx,
			&pApp->printer.component[i].dir.x, 
			&pApp->printer.component[i].dir.y, 
			&pApp->printer.component[i].dir.z);
		pApp->printer.component[i].dir.Normalize();
		if (j != 4) {
			return false;
		}

		//INCREMENT parameter
		m_PrinterDlgComponent[i].m_INCREMENT.GetWindowTextA(temp);
		temp.Replace(","," ");
		j = sscanf_s(temp,"%lf", &pApp->printer.component[i].jogIncrement);
		if (j != 1)
			pApp->printer.component[i].jogIncrement = JOG_STEP_AXIS;

		//LIMIT_SWITCH parameter
		m_PrinterDlgComponent[i].m_LIMIT_SWITCH.GetWindowTextA(temp);
		temp.Replace(","," ");
		j = sscanf_s(temp,"%d %d", &pApp->printer.component[i].posLimitSwitch, &pApp->printer.component[i].negLimitSwitch);
		if (j != 2) {
			pApp->printer.component[i].posLimitSwitch = FALSE;
			pApp->printer.component[i].negLimitSwitch = FALSE;
		}

		//MOTOR parameter
		m_PrinterDlgComponent[i].m_MOTOR.GetWindowTextA(temp);
		temp.Replace(","," ");
		j = sscanf_s(temp,"%ld %ld %lf", &pApp->printer.component[i].axis, &pApp->printer.component[i].axisDir, &pApp->printer.component[i].mmps);
		if (j != 3)
			return false;

		//OFFSET parameter
		m_PrinterDlgComponent[i].m_OFFSET.GetWindowTextA(temp);
		temp.Replace(","," ");
		j = sscanf_s(temp,"%lf %lf %lf", &pApp->printer.component[i].offset.x, &pApp->printer.component[i].offset.y, &pApp->printer.component[i].offset.z);
		if (j != 3) {
			pApp->printer.component[i].offset = CVec(0,0,0);
			return false;
		}

		//RANGE parameter
		m_PrinterDlgComponent[i].m_RANGE.GetWindowTextA(temp);
		temp.Replace(","," ");
		j = sscanf_s(temp,"%lf %lf %lf", &pApp->printer.component[i].rangemin, &pApp->printer.component[i].rangemax, &pApp->printer.component[i].home);
		if (j != 3) {
			pApp->printer.component[i].rangemin = 0;
			pApp->printer.component[i].rangemax = 240;
			pApp->printer.component[i].home = 0;
			return false;
		}
	}

	//DEFAULT_POD parameter
	m_PrinterDlgGeneral.m_DEFAULT_POD.GetWindowTextA(temp);
	temp.Replace(","," ");
	j = sscanf_s(temp,"%lf %lf %lf", &pApp->printer.m_vPOD.x, &pApp->printer.m_vPOD.y, &pApp->printer.m_vPOD.z);
	if (j != 3)
		return false;

	//DESCRIPTION parameter
	m_PrinterDlgGeneral.m_DESCRIPTION.GetWindowTextA(pApp->printer.description);

	//JOGSPEED parameter
	m_PrinterDlgGeneral.m_JOGSPEED.GetWindowTextA(temp);
	temp.Replace(","," ");
	j = sscanf_s(temp,"%lf", &d_temp);
	if (j != 1)
		return false;
	pApp->printer.SetJogSpeed(d_temp);

	//MAXTOOLS parameter
	m_PrinterDlgGeneral.m_MAXTOOLS.GetWindowTextA(temp);
	j = sscanf_s(temp,"%d", &i_temp);
	if (j != 1)
		return false;
	pApp->printer.SetMaxTools(i_temp);

	//Name parameter
	m_PrinterDlgGeneral.m_Name.GetWindowTextA(pApp->printer.name);

	//STATUS_UPDATE_PERIOD parameter
	m_PrinterDlgGeneral.m_STATUS_UPDATE_PERIOD.GetWindowTextA(temp);
	temp.Replace(","," ");
	j = sscanf_s(temp,"%lu", &unsigi_temp);
	if (j != 1)
		return false;
	pApp->printer.SetUserStatusPeriod(unsigi_temp);

	//TOOLLIMITSWITCH parameter
	m_PrinterDlgGeneral.m_TOOLLIMITSWITCH.GetWindowTextA(temp);
	temp.Replace(","," ");
	j = sscanf_s(temp,"%d %d", &pApp->printer.m_arBLimitSwitch[2*AXIS_U+1], &pApp->printer.m_arBLimitSwitch[2*AXIS_V+1]);
	if (j != 2) {
		pApp->printer.m_arBLimitSwitch[2*AXIS_U+1] = FALSE;
		pApp->printer.m_arBLimitSwitch[2*AXIS_V+1] = FALSE;
	}

	SetActivePage(temppage);

	return true;
}

// --------------------------------------------------------------------------
bool CPrinterDlgPropSheet::SaveParameters(CString filename)
// --------------------------------------------------------------------------
{
	CString temp;
	CPropertyPage *temppage;

	if(filename == "")
		filename = pApp->printerFile;

	CStdioFile file;
	if(file.Open(filename, CFile::modeCreate | CFile::modeReadWrite) == 0)
		return false;

	temppage = GetActivePage();

	SetActivePage(&m_PrinterDlgGeneral);

	for(int i = 0; i < MAX_COMPONENTS; i++)
	{
		SetActivePage(&m_PrinterDlgComponent[i]);

		if(i == 0)
			file.WriteString("////////////////////////////////////////BASE/////////////////////////////////////////\n");
		if(i == 1)
			file.WriteString("/////////////////////////////////////X-CARRIAGE//////////////////////////////////////\n");
		if(i == 2)
			file.WriteString("/////////////////////////////////////Y-CARRIAGE//////////////////////////////////////\n");
		if(i == 3)
			file.WriteString("/////////////////////////////////////Z-CARRIAGE//////////////////////////////////////\n");

		//COMPONENT parameter
		m_PrinterDlgComponent[i].m_COMPONENT.GetWindowTextA(temp);
		if(!temp.IsEmpty()) {
			temp = "COMPONENT                     " + temp + "\n";
			file.WriteString(temp);
		}

		//APPENDGEOM parameter
		m_PrinterDlgComponent[i].m_APPENDGEOM.GetWindowTextA(temp);
		if(!temp.IsEmpty()) {
			temp = "APPENDGEOM                    " + temp + "\n";
			file.WriteString(temp);
		}

		//COLOR parameter
		m_PrinterDlgComponent[i].m_COLOR.GetWindowTextA(temp);
		if(!temp.IsEmpty()) {
			temp = "COLOR                         " + temp + "\n";
			file.WriteString(temp);
		}

		//DIRECTION parameter
		m_PrinterDlgComponent[i].m_DIRECTION.GetWindowTextA(temp);
		if(!temp.IsEmpty()) {
			temp = "DIRECTION                     " + temp + "\n";
			file.WriteString(temp);
		}

		//INCREMENT parameter
		m_PrinterDlgComponent[i].m_INCREMENT.GetWindowTextA(temp);
		if(!temp.IsEmpty()) {
			temp = "INCREMENT                     " + temp + "\n";
			file.WriteString(temp);
		}

		//LIMIT_SWITCH parameter
		m_PrinterDlgComponent[i].m_LIMIT_SWITCH.GetWindowTextA(temp);
		if(!temp.IsEmpty()) {
			temp = "LIMITSWITCH                   " + temp + "\n";
			file.WriteString(temp);
		}

		//MOTOR parameter
		m_PrinterDlgComponent[i].m_MOTOR.GetWindowTextA(temp);
		if(!temp.IsEmpty()) {
			temp = "MOTOR                         " + temp + "\n";
			file.WriteString(temp);
		}

		//OFFSET parameter
		m_PrinterDlgComponent[i].m_OFFSET.GetWindowTextA(temp);
		if(!temp.IsEmpty()) {
			temp = "OFFSET                        " + temp + "\n";
			file.WriteString(temp);
		}

		//RANGE parameter
		m_PrinterDlgComponent[i].m_RANGE.GetWindowTextA(temp);
		if(!temp.IsEmpty()) {
			temp = "RANGE                         " + temp + "\n";
			file.WriteString(temp);
		}

		file.WriteString("\n\n\n");
	}

		file.WriteString("/////////////////////////////////GENERAL PARAMETERS//////////////////////////////////\n");

	//DEFAULT_POD parameter
	m_PrinterDlgGeneral.m_DEFAULT_POD.GetWindowTextA(temp);
	if(!temp.IsEmpty()) {
		temp = "DEFAULT_POD                   " + temp + "\n";
		file.WriteString(temp);
	}

	//DESCRIPTION parameter
	m_PrinterDlgGeneral.m_DESCRIPTION.GetWindowTextA(temp);
	if(!temp.IsEmpty()) {
		temp = "DESCRIPTION                   " + temp + "\n";
		file.WriteString(temp);
	}

	//JOGSPEED parameter
	m_PrinterDlgGeneral.m_JOGSPEED.GetWindowTextA(temp);
	if(!temp.IsEmpty()) {
		temp = "JOGSPEED                      " + temp + "\n";
		file.WriteString(temp);
	}

	//MAXTOOLS parameter
	m_PrinterDlgGeneral.m_MAXTOOLS.GetWindowTextA(temp);
	if(!temp.IsEmpty()) {
		temp = "MAXTOOLS                      " + temp + "\n";
		file.WriteString(temp);
	}

	//Name parameter
	m_PrinterDlgGeneral.m_Name.GetWindowTextA(temp);
	if(!temp.IsEmpty()) {
		temp = "NAME                          " + temp + "\n";
		file.WriteString(temp);
	}

	//STATUS_UPDATE_PERIOD parameter
	m_PrinterDlgGeneral.m_STATUS_UPDATE_PERIOD.GetWindowTextA(temp);
	if(!temp.IsEmpty()) {
		temp = "STATUS_UPDATE_PERIOD          " + temp + "\n";
		file.WriteString(temp);
	}

	//TOOLLIMITSWITCH parameter
	m_PrinterDlgGeneral.m_TOOLLIMITSWITCH.GetWindowTextA(temp);
	if(!temp.IsEmpty()) {
		temp = "TOOLLIMITSWITCH               " + temp + "\n";
		file.WriteString(temp);
	}

	SetActivePage(temppage);

	return true;
}

// --------------------------------------------------------------------------





// --------------------------------------------------------------------------
// CPrinterDlg
// --------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(CPrinterDlg, CMiniFrameWnd)

// --------------------------------------------------------------------------
CPrinterDlg::CPrinterDlg()
// --------------------------------------------------------------------------
{
	m_PrinterDlgPropSheet = NULL;
	pApp = ((CFabAtHomeApp *)AfxGetApp());
}

// --------------------------------------------------------------------------
CPrinterDlg::~CPrinterDlg()
// --------------------------------------------------------------------------
{
}

// --------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CPrinterDlg, CMiniFrameWnd)
	ON_WM_ACTIVATE()
	ON_WM_CLOSE()
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

// --------------------------------------------------------------------------
void CPrinterDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
// --------------------------------------------------------------------------
{
	CMiniFrameWnd::OnActivate(nState, pWndOther, bMinimized);

	// Forward any WM_ACTIVATEs to the property sheet...
	// Like the dialog manager itself, it needs them to save/restore the focus.
	ASSERT_VALID(m_PrinterDlgPropSheet);

	// Use GetCurrentMessage to get unmodified message data.
	const MSG* pMsg = GetCurrentMessage();
	ASSERT(pMsg->message == WM_ACTIVATE);
	m_PrinterDlgPropSheet->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
}

// --------------------------------------------------------------------------
void CPrinterDlg::OnClose()
// --------------------------------------------------------------------------
{
	//CMiniFrameWnd::OnClose();

	// Instead of closing the modeless property sheet, just hide it.
	pApp->HidePrinterDlgPropSheet();
}

// --------------------------------------------------------------------------
int CPrinterDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
// --------------------------------------------------------------------------
{
	if (CMiniFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_PrinterDlgPropSheet = new CPrinterDlgPropSheet("Printer Configuration Properties", this);
	if (!m_PrinterDlgPropSheet->Create(this, WS_CHILD | WS_VISIBLE, 0))
	{
		delete m_PrinterDlgPropSheet;
		m_PrinterDlgPropSheet = NULL;
		return -1;
	}

	// Resize the mini frame so that it fits around the child property
	// sheet.
	CRect rectClient, rectWindow;
	m_PrinterDlgPropSheet->GetWindowRect(rectClient);
	rectWindow = rectClient;

	// CMiniFrameWnd::CalcWindowRect adds the extra width and height
	// needed from the mini frame.
	CalcWindowRect(rectWindow);
	SetWindowPos(NULL, rectWindow.left, rectWindow.top,	rectWindow.Width(), rectWindow.Height(),
		SWP_NOZORDER | SWP_NOACTIVATE);
	m_PrinterDlgPropSheet->SetWindowPos(NULL, 0, 0, rectClient.Width(), rectClient.Height(),
		SWP_NOZORDER | SWP_NOACTIVATE);

	return 0;
}

// --------------------------------------------------------------------------
void CPrinterDlg::OnSetFocus(CWnd* pOldWnd)
// --------------------------------------------------------------------------
{
	// Forward focus to the embedded property sheet
	ASSERT_VALID(m_PrinterDlgPropSheet);
	m_PrinterDlgPropSheet->SetFocus();
}

// --------------------------------------------------------------------------
void CPrinterDlg::OnShowWindow(BOOL bShow, UINT nStatus)
// --------------------------------------------------------------------------
{
	CMiniFrameWnd::OnShowWindow(bShow, nStatus);
}

// --------------------------------------------------------------------------