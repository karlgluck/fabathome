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

// JogDlg.cpp : implementation file
//

#include "stdafx.h"
#include ".\jogcarriagedlg.h"

// CJogCarriageDlg dialog

IMPLEMENT_DYNAMIC(CJogCarriageDlg, CDialog)
// --------------------------------------------------------------------------
CJogCarriageDlg::CJogCarriageDlg(CWnd* pParent /*=NULL*/)
// --------------------------------------------------------------------------
	: CDialog(CJogCarriageDlg::IDD, pParent)
{
}

// --------------------------------------------------------------------------
CJogCarriageDlg::~CJogCarriageDlg()
// --------------------------------------------------------------------------
{
}

// --------------------------------------------------------------------------
void CJogCarriageDlg::DoDataExchange(CDataExchange* pDX)
// --------------------------------------------------------------------------
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPINX, spinX);
	DDX_Control(pDX, IDC_SPINY, spinY);
	DDX_Control(pDX, IDC_SPINZ, spinZ);
	DDX_Control(pDX, IDC_XEDIT, xeditctrl);
	DDX_Control(pDX, IDC_YEDIT, yeditctrl);
	DDX_Control(pDX, IDC_ZEDIT, zeditctrl);
	DDX_Control(pDX, IDC_EDIT_XRT, xrtpos);
	DDX_Control(pDX, IDC_EDIT_YRT, yrtpos);
	DDX_Control(pDX, IDC_EDIT_ZRT, zrtpos);
	DDX_Control(pDX, IDC_X_INC, xinc);
	DDX_Control(pDX, IDC_Y_INC, yinc);
	DDX_Control(pDX, IDC_Z_INC, zinc);
}


BEGIN_MESSAGE_MAP(CJogCarriageDlg, CDialog)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINX, OnDeltaposSpinx)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINY, OnDeltaposSpiny)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINZ, OnDeltaposSpinz)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTONHOME, OnBnClickedButtonhome)
	ON_BN_CLICKED(IDC_BUTTONSETORIGIN, OnBnClickedButtonsetorigin)
	ON_BN_CLICKED(IDC_BUTTONGOTOORIGIN, OnBnClickedButtongotoorigin)
	ON_BN_CLICKED(IDC_BUTTONSETHOME, OnBnClickedButtonsethome)
	ON_WM_TIMER()
	ON_COMMAND(ID_JOG_X_BKD, OnJogXBkd)
	ON_COMMAND(ID_JOG_X_FWD, OnJogXFwd)
	ON_COMMAND(ID_JOG_Y_LEFT, OnJogYLeft)
	ON_COMMAND(ID_JOG_Y_RIGHT, OnJogYRight)
	ON_COMMAND(ID_JOG_Z_DOWN, OnJogZDown)
	ON_COMMAND(ID_JOG_Z_UP, OnJogZUp)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTONSETSAFE, OnBnClickedButtonsetsafe)
	ON_BN_CLICKED(IDC_BUTTONGOTOSAFE, OnBnClickedButtongotosafe)
END_MESSAGE_MAP()


// CJogCarriageDlg message handlers

// --------------------------------------------------------------------------
BOOL CJogCarriageDlg::OnInitDialog()
// --------------------------------------------------------------------------
{
	CDialog::OnInitDialog();
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	
	m_pToolTip = new CToolTipCtrl;
	if(!m_pToolTip->Create(this))
	{
		TRACE("Unable To create ToolTip\n");
		return true;
	}

	//m_pToolTip->SetMaxTipWidth(1024*8);
	m_pToolTip->AddTool(GetDlgItem(IDC_BUTTONSETHOME),"Set current position as Home");
	m_pToolTip->AddTool(GetDlgItem(IDC_BUTTONSETSAFE),"Set current position as Safe");
	m_pToolTip->AddTool(GetDlgItem(IDC_BUTTONSETORIGIN),"Set current position as Origin");
	m_pToolTip->AddTool(GetDlgItem(IDC_BUTTONHOME),"Find home position or goto if already set a home position");
	m_pToolTip->AddTool(GetDlgItem(IDC_BUTTONGOTOSAFE),"Find safe position or goto if already set a safe position");
	m_pToolTip->AddTool(GetDlgItem(IDC_BUTTONGOTOORIGIN),"Goto Origin position");

	m_pToolTip->Activate(TRUE);

	//update every VIEW_REFRESH ms
	m_uiTimer = SetTimer(JOG_CARRIAGE_DLG_TIMER, VIEW_REFRESH_MULT*STATUS_UPDATE_PERIOD, NULL); 
	UpdatePositionReadout(); //force all info to be loaded
	
	//tell the spin controls they have buddies (edit controls)
	spinX.SetBuddy((CWnd*)&xeditctrl);
	spinY.SetBuddy((CWnd*)&yeditctrl);
	spinZ.SetBuddy((CWnd*)&zeditctrl);

	//try to enable motion on the hardware
	if(pApp->printer.bHardwareInitialized)
		if(!pApp->printer.IsStarted())
			return(pApp->printer.Start());

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//mesage handlers for keyboard shortcuts
// --------------------------------------------------------------------------
void CJogCarriageDlg::OnJogXBkd()
// --------------------------------------------------------------------------
{
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	double xJogInc = pApp->printer.component[1].jogIncrement;
	pApp->printer.JogCarriageBy(CVec(-xJogInc,0,0), false); //immediate, not queued
	pApp->printer.Move(false);
	UpdatePositionReadout();
}

// --------------------------------------------------------------------------
void CJogCarriageDlg::OnJogXFwd()
// --------------------------------------------------------------------------
{	
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	double xJogInc = pApp->printer.component[1].jogIncrement;
	pApp->printer.JogCarriageBy(CVec(xJogInc,0,0), false); //immediate, not queued
	pApp->printer.Move(false);
	UpdatePositionReadout();
}

// --------------------------------------------------------------------------
void CJogCarriageDlg::OnDeltaposSpinx(NMHDR *pNMHDR, LRESULT *pResult)
// --------------------------------------------------------------------------
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	if (pNMUpDown->iDelta > 0) {
		OnJogXBkd();
	} else {
		OnJogXFwd();
	}
	UpdatePositionReadout();

	*pResult = 0;
}


//mesage handlers for keyboard shortcuts
// --------------------------------------------------------------------------
void CJogCarriageDlg::OnJogYLeft()
// --------------------------------------------------------------------------
{
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	double yJogInc = pApp->printer.component[2].jogIncrement;
	pApp->printer.JogCarriageBy(CVec(0,-yJogInc,0), false); //immediate, not queued
	pApp->printer.Move(false);
	UpdatePositionReadout();
}

// --------------------------------------------------------------------------
void CJogCarriageDlg::OnJogYRight()
// --------------------------------------------------------------------------
{
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	double yJogInc = pApp->printer.component[2].jogIncrement;
	pApp->printer.JogCarriageBy(CVec(0,yJogInc,0), false); //immediate, not queued
	pApp->printer.Move(false);
	UpdatePositionReadout();
}

// --------------------------------------------------------------------------
void CJogCarriageDlg::OnDeltaposSpiny(NMHDR *pNMHDR, LRESULT *pResult)
// --------------------------------------------------------------------------
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	if (pNMUpDown->iDelta > 0) {
		OnJogYLeft();
	} else {
		OnJogYRight();
	}
	UpdatePositionReadout();

	*pResult = 0;
}

//mesage handlers for keyboard shortcuts
// --------------------------------------------------------------------------
void CJogCarriageDlg::OnJogZDown()
// --------------------------------------------------------------------------
{
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	double zJogInc = pApp->printer.component[3].jogIncrement;
	pApp->printer.JogCarriageBy(CVec(0,0,-zJogInc), false); //immediate, not queued
	pApp->printer.Move(false);
	UpdatePositionReadout();
}

// --------------------------------------------------------------------------
void CJogCarriageDlg::OnJogZUp()
// --------------------------------------------------------------------------
{
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	double zJogInc = pApp->printer.component[3].jogIncrement;
	pApp->printer.JogCarriageBy(CVec(0,0,zJogInc), false); //immediate, not queued
	pApp->printer.Move(false);
	UpdatePositionReadout();
}

// --------------------------------------------------------------------------
void CJogCarriageDlg::OnDeltaposSpinz(NMHDR *pNMHDR, LRESULT *pResult)
// --------------------------------------------------------------------------
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	if (pNMUpDown->iDelta > 0) {
		OnJogZDown();
	} else {
		OnJogZUp();
	}
	UpdatePositionReadout();

	*pResult = 0;
}

// --------------------------------------------------------------------------
void CJogCarriageDlg::OnDestroy()
// --------------------------------------------------------------------------
{
	CDialog::OnDestroy();
}


// --------------------------------------------------------------------------
void CJogCarriageDlg::OnBnClickedButtonhome()
// --------------------------------------------------------------------------
{
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	pApp->printer.ExecuteHomeSequence();
	UpdatePositionReadout();
}

// --------------------------------------------------------------------------
void CJogCarriageDlg::OnBnClickedButtonsethome()
// --------------------------------------------------------------------------
{
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();

	pApp->printer.SetHomePos();
	UpdatePositionReadout();
	((CFabAtHomeApp *)AfxGetApp())->Log("New home position set.");
}

// --------------------------------------------------------------------------
void CJogCarriageDlg::UpdatePositionReadout(bool bAll)
// --------------------------------------------------------------------------
{// fill editable controls with current commanded position and increments

	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();

	CString text;
	//update the "real-time" position
	text.Format("%.2f", pApp->printer.component[1].GetRTPos());
	xrtpos.SetWindowText(text);
	text.Format("%.2f", pApp->printer.component[2].GetRTPos());
	yrtpos.SetWindowText(text);
	text.Format("%.2f", pApp->printer.component[3].GetRTPos());
	zrtpos.SetWindowText(text);
	//jog increments
	if(bAll)
	{
		text.Format("%.2f", pApp->printer.component[1].jogIncrement);
		xinc.SetWindowText(text);
		text.Format("%.2f", pApp->printer.component[2].jogIncrement);
		yinc.SetWindowText(text);
		text.Format("%.2f", pApp->printer.component[3].jogIncrement);
		zinc.SetWindowText(text);
	}
	//and the commanded position, if it has changed
	text.Format("%.2f", pApp->printer.component[1].GetCMDPos());
	if((oldXCmd.Compare(text)!=0) || bAll)
	{
		xeditctrl.SetWindowText(text);
		oldXCmd = text;
	}
	text.Format("%.2f", pApp->printer.component[2].GetCMDPos());
	if((oldYCmd.Compare(text)!=0) || bAll)
	{
		yeditctrl.SetWindowText(text);
		oldYCmd = text;
	}
	text.Format("%.2f", pApp->printer.component[3].GetCMDPos());
	if((oldZCmd.Compare(text)!=0) || bAll)
	{
		zeditctrl.SetWindowText(text);
		oldZCmd = text;
	}
}


// --------------------------------------------------------------------------
void CJogCarriageDlg::OnOK()
// --------------------------------------------------------------------------
{// Move to set position

	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();

	CString strx,stry,strz;
	xeditctrl.GetWindowText(strx);
	yeditctrl.GetWindowText(stry);
	zeditctrl.GetWindowText(strz);
	double x = atof(strx);
	double y = atof(stry);
	double z = atof(strz);
	CVec newpos = CVec(x,y,z);

	CString strIncX, strIncY, strIncZ;
	xinc.GetWindowText(strIncX);
	yinc.GetWindowText(strIncY);
	zinc.GetWindowText(strIncZ);
	double newXInc = atof(strIncX);
	double newYInc = atof(strIncY);
	double newZInc = atof(strIncZ);
	//check for reasonable jog increments
	if((newXInc < 0)||(newXInc > MAX_JOG_STEP_AXIS)) newXInc = JOG_STEP_AXIS;
	if((newYInc < 0)||(newYInc > MAX_JOG_STEP_AXIS)) newYInc = JOG_STEP_AXIS;
	if((newZInc < 0)||(newZInc > MAX_JOG_STEP_AXIS)) newZInc = JOG_STEP_AXIS;
	pApp->printer.component[1].jogIncrement = newXInc;
	pApp->printer.component[2].jogIncrement = newYInc;
	pApp->printer.component[3].jogIncrement = newZInc;

	pApp->printer.JogCarriageTo(newpos, false); //immediate, not queued
	pApp->printer.Move(false);
	UpdatePositionReadout();
}

// --------------------------------------------------------------------------
void CJogCarriageDlg::OnCancel()
// --------------------------------------------------------------------------
{
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	if(pApp->printer.IsMoving() || pApp->printer.IsToolMoving())
	{
		pApp->printer.Stop();
	}
}

// --------------------------------------------------------------------------
void CJogCarriageDlg::OnBnClickedButtonsetorigin()
// --------------------------------------------------------------------------
{
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();

	CVec neworigin;

	//if you want to use the current hardware position then use the following code:
	//neworigin = pApp->printer.GetCurrentPos();

	//if you want to use the commanded position (without actually having to be there), use the following code:
	CString strx,stry,strz;
	xeditctrl.GetWindowText(strx);
	yeditctrl.GetWindowText(stry);
	zeditctrl.GetWindowText(strz);
	double x = atof(strx);
	double y = atof(stry);
	double z = atof(strz);
	neworigin = CVec(x,y,z);

	pApp->printer.SetOrigin(neworigin);
	UpdatePositionReadout();
	((CFabAtHomeApp *)AfxGetApp())->Log("New print origin set.");
}

// --------------------------------------------------------------------------
void CJogCarriageDlg::OnBnClickedButtongotoorigin()
// --------------------------------------------------------------------------
{
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();

	pApp->printer.ExecuteOriginSequence();
	UpdatePositionReadout();
}

// --------------------------------------------------------------------------
void CJogCarriageDlg::OnBnClickedButtonsetsafe()
// --------------------------------------------------------------------------
{
	//set the current position as the safe position e.g. for pausing, material changes
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();

	CVec safe;

	//if you want to use the current hardware position then use the following code:
	//safe = pApp->printer.GetCurrentPos();

	//if you want to use the commanded position (without actually having to be there), use the following code:
	CString strx,stry,strz;
	xeditctrl.GetWindowText(strx);
	yeditctrl.GetWindowText(stry);
	zeditctrl.GetWindowText(strz);
	double x = atof(strx);
	double y = atof(stry);
	double z = atof(strz);
	safe = CVec(x,y,z);

	pApp->printer.SetSafePos(safe);
	UpdatePositionReadout();
	((CFabAtHomeApp *)AfxGetApp())->Log("New safe position set.");
}

// --------------------------------------------------------------------------
void CJogCarriageDlg::OnBnClickedButtongotosafe()
// --------------------------------------------------------------------------
{
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();

	//grab the current position to know where to return to
	pApp->printer.SetLastPos(); 
	pApp->printer.ExecuteSafeSequence();
	UpdatePositionReadout();
}

// --------------------------------------------------------------------------
void CJogCarriageDlg::OnTimer(UINT nIDEvent)
// --------------------------------------------------------------------------
{
	//timer should update realtime and commanded position
	if ((nIDEvent == JOG_CARRIAGE_DLG_TIMER))
	{
		UpdatePositionReadout(false);
	}

	CDialog::OnTimer(nIDEvent);
}

// --------------------------------------------------------------------------
void CJogCarriageDlg::OnClose()
// --------------------------------------------------------------------------
{
	//close and destroy the dialog
	
	CDialog::OnClose();
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	ASSERT(pApp->pJogCarriageDlg != NULL);
	if (pApp->pJogCarriageDlg != NULL) {
		delete pApp->pJogCarriageDlg;
		pApp->pJogCarriageDlg = NULL;
	}
}

// --------------------------------------------------------------------------
BOOL CJogCarriageDlg::PreTranslateMessage(MSG* pMsg)
// --------------------------------------------------------------------------
{
	if (m_pToolTip != NULL)
		m_pToolTip->RelayEvent(pMsg);

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