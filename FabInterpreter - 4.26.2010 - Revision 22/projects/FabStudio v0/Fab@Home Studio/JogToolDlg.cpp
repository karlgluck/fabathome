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

// JogToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include ".\jogtooldlg.h"

// CJogToolDlg dialog

IMPLEMENT_DYNAMIC(CJogToolDlg, CDialog)
// --------------------------------------------------------------------------
CJogToolDlg::CJogToolDlg(CWnd* pParent /*=NULL*/)
// --------------------------------------------------------------------------
	: CDialog(CJogToolDlg::IDD, pParent)
{
}

// --------------------------------------------------------------------------
CJogToolDlg::~CJogToolDlg()
// --------------------------------------------------------------------------
{
}

// --------------------------------------------------------------------------
void CJogToolDlg::DoDataExchange(CDataExchange* pDX)
// --------------------------------------------------------------------------
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBOTOOL, combotool);
	DDX_Control(pDX, IDC_EDITPOS, editpos);
	DDX_Control(pDX, IDC_EDIT_RTPOS, editRTPos);
	DDX_Control(pDX, IDC_TOOL_INC, editInc);
	DDX_Control(pDX, IDC_SPINPOS, spinpos);
	DDX_Control(pDX, IDC_CHECK_CURRENT, m_cbActive);
}


BEGIN_MESSAGE_MAP(CJogToolDlg, CDialog)
//	ON_WM_DESTROY()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINPOS, OnDeltaposSpinpos)
	ON_CBN_SELCHANGE(IDC_COMBOTOOL, OnCbnSelchangeCombotool)
//	ON_WM_CLOSE()
ON_WM_DESTROY()
ON_WM_TIMER()
ON_COMMAND(ID_JOG_TOOL_BKD, OnJogToolBkd)
ON_COMMAND(ID_JOG_TOOL_FWD, OnJogToolFwd)
ON_BN_CLICKED(IDC_CHECK_CURRENT, OnBnClickedCheckCurrent)
ON_WM_ACTIVATE()
ON_WM_CLOSE()
END_MESSAGE_MAP()


// CJogToolDlg message handlers


// --------------------------------------------------------------------------
BOOL CJogToolDlg::OnInitDialog()
// --------------------------------------------------------------------------
{
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();

	CDialog::OnInitDialog();

	//tell the spin control that it has a buddy
	spinpos.SetBuddy((CWnd*)&editpos);

	//load combobox with the currently mounted tools
	for (int i=0; i<pApp->tool.GetSize(); i++) 
	{
		if(pApp->tool[i].IsMounted())
			combotool.AddString(pApp->tool[i].name);  // CComboBox
	}
	int curtool = combotool.FindString(-1, pApp->printer.GetCurToolName());
	if(curtool == CB_ERR) //cannot find the name in the list
	{
		CString msg;
		msg.Format("Cannot find the tool %s. Please reload tool files",
			pApp->printer.GetCurToolName());
		return FALSE;
	}

	combotool.SetCurSel(curtool); //select the current tool
	OnCbnSelchangeCombotool(); //let the rest of the system know about the selection

	m_uiTimer = SetTimer(JOG_TOOL_DLG_TIMER, VIEW_REFRESH_MULT*STATUS_UPDATE_PERIOD, NULL); 
	
	//try to enable motion on the hardware
	if(pApp->printer.bHardwareInitialized)
		if(!pApp->printer.IsStarted())
			return(pApp->printer.Start());

	UpdatePositionReadout();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// --------------------------------------------------------------------------
void CJogToolDlg::OnOK()
// --------------------------------------------------------------------------
{// move to absolute keyed-in position; update the jog increment

	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();

	CString strpos, strinc;
	editpos.GetWindowText(strpos);
	double newpos = atof(strpos);
	if (newpos < -300)
		newpos = -300;
	if (newpos > 300)
		newpos = 300;

	editInc.GetWindowText(strinc);
	double newInc = atof(strinc);
	//check for reasonable jog increments
	if((newInc < 0) || (newInc > MAX_JOG_STEP_TOOL)) newInc = JOG_STEP_TOOL;
	pApp->printer.GetCurTool()->jogIncrement = newInc;
	
	//pApp->printer.JogToolBy(pApp->printer.GetCurTool()->jogIncrement, false); //immediate, not queued
	//pApp->printer.Move(false);
	UpdatePositionReadout();
}


// --------------------------------------------------------------------------
void CJogToolDlg::UpdatePositionReadout(void)
// --------------------------------------------------------------------------
{// fill edit controls with current position and jog increment

	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();

	CString text;
	text.Format("%.2f", pApp->printer.GetCurTool()->cmdstep*pApp->printer.GetCurTool()->mmps);
	editpos.SetWindowText(text);
	text.Format("%.2f", pApp->printer.GetCurTool()->curstep*pApp->printer.GetCurTool()->mmps);
	editRTPos.SetWindowText(text);
	text.Format("%.2f", pApp->printer.GetCurTool()->jogIncrement);
	editInc.SetWindowText(text);
}

//handlers for keyboard shortcuts
// --------------------------------------------------------------------------
void CJogToolDlg::OnJogToolBkd()
// --------------------------------------------------------------------------
{
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	pApp->printer.JogToolBy(-pApp->printer.GetCurTool()->jogIncrement, false); //immediate, not queued
	pApp->printer.Move(MAX_STEP_FREQ/pApp->printer.GetCurTool()->GetJogspeed(),false);
}

// --------------------------------------------------------------------------
void CJogToolDlg::OnJogToolFwd()
// --------------------------------------------------------------------------
{
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	pApp->printer.JogToolBy(pApp->printer.GetCurTool()->jogIncrement, false); //immediate, not queued
	pApp->printer.Move(MAX_STEP_FREQ/pApp->printer.GetCurTool()->GetJogspeed(),false);
}

// --------------------------------------------------------------------------
void CJogToolDlg::OnDeltaposSpinpos(NMHDR *pNMHDR, LRESULT *pResult)
// --------------------------------------------------------------------------
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();

	if (pNMUpDown->iDelta > 0) {
		OnJogToolFwd();
	} else {
		OnJogToolBkd();
	}
	UpdatePositionReadout();

	*pResult = 0;
}

// --------------------------------------------------------------------------
void CJogToolDlg::OnCbnSelchangeCombotool()
// --------------------------------------------------------------------------
{
	CString curSel;
	combotool.GetLBText(combotool.GetCurSel(),curSel);

	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	//if this is the currently active tool
	//set the check box accordingly
	if(0==curSel.CompareNoCase(pApp->printer.GetCurToolName()))
		m_cbActive.SetCheck(BST_CHECKED);
	else
		m_cbActive.SetCheck(BST_UNCHECKED);
	
	UpdatePositionReadout();
}


// --------------------------------------------------------------------------
void CJogToolDlg::OnDestroy()
// --------------------------------------------------------------------------
{
	CDialog::OnDestroy();
}

// --------------------------------------------------------------------------
void CJogToolDlg::OnCancel()
// --------------------------------------------------------------------------
{
	// TODO: Add your specialized code here and/or call the base class
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	if(pApp->printer.IsToolMoving())
	{
		pApp->printer.Stop();
	}
}

// --------------------------------------------------------------------------
void CJogToolDlg::OnTimer(UINT nIDEvent)
// --------------------------------------------------------------------------
{
	//timer should update the "real-time" data in the dialog
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();

	if ((nIDEvent == JOG_TOOL_DLG_TIMER)) {
		CString text;
		text.Format("%.2f", pApp->printer.GetCurTool()->curstep*pApp->printer.GetCurTool()->mmps);
		editRTPos.SetWindowText(text);
	};

	CDialog::OnTimer(nIDEvent);
}

// --------------------------------------------------------------------------
void CJogToolDlg::OnBnClickedCheckCurrent()
// --------------------------------------------------------------------------
{
	//if not active, set the selected tool as the currently active tool
	//otherwise, do nothing (can only select a tool as active, not deselect)
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();

	CString curSel;
	combotool.GetLBText(combotool.GetCurSel(),curSel);

	//tool is found, so change current tool
	pApp->printer.SetCurTool(curSel);
	//and update the dialog 
	OnCbnSelchangeCombotool();
}

// --------------------------------------------------------------------------
void CJogToolDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
// --------------------------------------------------------------------------
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();

	//refresh combobox with the currently mounted tools
	combotool.ResetContent();
	for (int i=0; i<pApp->tool.GetSize(); i++) 
	{
		if(pApp->tool[i].IsMounted())
			combotool.AddString(pApp->tool[i].name);  // CComboBox
	}
	int curtool = combotool.FindString(-1, pApp->printer.GetCurToolName());
	if(curtool == CB_ERR) //cannot find the name in the list
	{
		CString msg;
		msg.Format("Cannot find the tool %s. Please reload tool files",
			pApp->printer.GetCurToolName());
		return;
	}

	combotool.SetCurSel(curtool); //select the current tool
	OnCbnSelchangeCombotool(); //let the rest of the system know about the selection

	UpdatePositionReadout();
}

// --------------------------------------------------------------------------
void CJogToolDlg::OnClose()
// --------------------------------------------------------------------------
{
	//close and destroy the dialog
	
	CDialog::OnClose();
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	ASSERT(pApp->pJogToolDlg != NULL);
	if (pApp->pJogToolDlg != NULL) {
		delete pApp->pJogToolDlg;
		pApp->pJogToolDlg = NULL;
	}
}

// --------------------------------------------------------------------------
BOOL CJogToolDlg::PreTranslateMessage(MSG* pMsg)
// --------------------------------------------------------------------------
{
	if(pMsg->message==WM_KEYDOWN)
    {
        if(pMsg->wParam == VK_ESCAPE)
		{
			SendMessage(WM_CLOSE);
			// In this case don't call the original PreTranslateMessage as the object is immediately destroyed
			return true;
		}
    }

	return CDialog::PreTranslateMessage(pMsg);
}

// --------------------------------------------------------------------------