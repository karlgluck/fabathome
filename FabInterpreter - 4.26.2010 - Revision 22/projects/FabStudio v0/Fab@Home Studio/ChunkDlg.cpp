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

// ChunkDlg.cpp : implementation file
//

#include "stdafx.h"
#include ".\chunkdlg.h"


// CChunkDlg dialog

IMPLEMENT_DYNAMIC(CChunkDlg, CDialog)
// --------------------------------------------------------------------------
CChunkDlg::CChunkDlg(CWnd* pParent /*=NULL*/)
// --------------------------------------------------------------------------
	: CDialog(CChunkDlg::IDD, pParent)
{
	m_nPriority = 0;
	m_pChunk = NULL;
}

// --------------------------------------------------------------------------
CChunkDlg::~CChunkDlg()
// --------------------------------------------------------------------------
{
}

// --------------------------------------------------------------------------
void CChunkDlg::DoDataExchange(CDataExchange* pDX)
// --------------------------------------------------------------------------
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBOMAT, combomat);
	DDX_Control(pDX, IDC_COMBOTOOL, combotool);
	DDX_Control(pDX, IDC_STATICMAT, staticmat);
	DDX_Control(pDX, IDC_STATICTOOL, statictool);
	DDX_Control(pDX, IDC_BUTTONCOLOR, buttoncolor);
	DDX_Control(pDX, IDC_EDITALPHA, editalpha);
	DDX_Control(pDX, IDC_COMBOPRIORITY, combopriority);
	DDX_CBIndex(pDX, IDC_COMBOPRIORITY, m_nPriority);
	DDX_Control(pDX, IDC_COMBO_CHUNKNAME, m_cbChunkName);
	DDX_Control(pDX, IDC_CHECK_BOUNDFIRST, m_checkBoundFirst);
}


BEGIN_MESSAGE_MAP(CChunkDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTONCOLOR, OnBnClickedButtoncolor)
	ON_CBN_SELCHANGE(IDC_COMBOMAT, OnCbnSelchangeCombomat)
	ON_CBN_SELCHANGE(IDC_COMBOTOOL, OnCbnSelchangeCombotool)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBOPRIORITY, OnCbnSelchangeCombopriority)
	ON_CBN_SELCHANGE(IDC_COMBO_CHUNKNAME, OnCbnSelchangeComboChunkname)
	ON_CBN_KILLFOCUS(IDC_COMBOPRIORITY, OnCbnKillfocusCombopriority)
	ON_BN_CLICKED(ID_APPLY_NOW, OnBnClickedApplyNow)
	ON_BN_CLICKED(IDC_CHECK_BOUNDFIRST, &CChunkDlg::OnBnClickedCheckBoundfirst)
END_MESSAGE_MAP()


// CChunkDlg message handlers

// --------------------------------------------------------------------------
BOOL CChunkDlg::OnInitDialog()
// --------------------------------------------------------------------------
{
	CDialog::OnInitDialog();
	UpdateContent();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// --------------------------------------------------------------------------
void CChunkDlg::UpdateContent()
// --------------------------------------------------------------------------
{
	//update the controls to reflect the currently selected chunk
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();

	if(pApp->printer.GetModelPtr() == NULL) return;
	if(pApp->printer.GetModelPtr()->chunk.GetSize() <= 0) return;
	
	//load names of chunks
	int i;
	m_cbChunkName.ResetContent();
	for(i = 0; i < pApp->printer.GetModelPtr()->chunk.GetSize(); i++)
	{
		m_cbChunkName.AddString(pApp->printer.GetModelPtr()->chunk[i].GetName());
	}
	int chunkidx = pApp->printer.GetModelPtr()->GetChunkSelected();
	if(chunkidx < 0) chunkidx = 0;
	//set selected chunk
	m_pChunk = &(pApp->printer.GetModelPtr()->chunk[chunkidx]);
	chunkidx = m_cbChunkName.FindStringExact(-1,m_pChunk->GetName());
	m_cbChunkName.SetCurSel(chunkidx);

	
	// load material and tool combos	
	combomat.ResetContent();
	for (i=0; i<pApp->material.GetSize(); i++) {
		combomat.AddString(pApp->material[i].name);
	}
	int matidx = CMaterial::SearchByName(m_pChunk->materialname);
	if (matidx != -1) {
		combomat.SetCurSel(matidx);
		OnCbnSelchangeCombomat();
	} else {
		staticmat.SetWindowText("");
	}

	combotool.ResetContent();
	for (i=0; i<pApp->tool.GetSize(); i++) {
		combotool.AddString(pApp->tool[i].name);
	}
	int toolidx = combotool.FindStringExact(-1,m_pChunk->toolname);
	if(toolidx != CB_ERR)
	{
		combotool.SetCurSel(toolidx);
		OnCbnSelchangeCombotool();
	} else {
		statictool.SetWindowText("");
	}

	//set the selection in the priority combo to the priority of the current chunk
	combopriority.SetCurSel(m_pChunk->GetPriority());

	//set the state of the checkbox indicating whether boundary paths should be
	//deposited prior to fill paths
	m_checkBoundFirst.SetCheck((BOOL)m_pChunk->GetBoundFirst());
	
	CString text;
	text.Format("%.0f", m_pChunk->alpha*100);
	editalpha.SetWindowText(text);
	m_color = m_pChunk->color;
}

// --------------------------------------------------------------------------
void CChunkDlg::OnOK()
// --------------------------------------------------------------------------
{

	OnBnClickedApplyNow();
	CDialog::OnOK();
}

// --------------------------------------------------------------------------
void CChunkDlg::OnBnClickedButtoncolor()
// --------------------------------------------------------------------------
{// edit color

	CColorDialog dlg;

	dlg.m_cc.Flags |= CC_FULLOPEN | CC_RGBINIT;
	dlg.m_cc.rgbResult = RGB(m_color.x*255, m_color.y*255, m_color.z*255);

	if (dlg.DoModal() == IDOK) {
		COLORREF color = dlg.GetColor();
		m_color.x = GetRValue(color)/255.0;
		m_color.y = GetGValue(color)/255.0;
		m_color.z = GetBValue(color)/255.0;
	}

}

// --------------------------------------------------------------------------
void CChunkDlg::OnCbnSelchangeCombomat()
// --------------------------------------------------------------------------
{
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();

	int sel = combomat.GetCurSel();
	if (sel != LB_ERR) {
		CString desc = pApp->material[sel].description;
		staticmat.SetWindowText(desc);
	}
}

// --------------------------------------------------------------------------
void CChunkDlg::OnCbnSelchangeCombotool()
// --------------------------------------------------------------------------
{
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();

	int sel = combotool.GetCurSel();
	if (sel != LB_ERR) {
		CString desc = pApp->tool[sel].description;
		statictool.SetWindowText(desc);
	}
}

// --------------------------------------------------------------------------
HBRUSH CChunkDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
// --------------------------------------------------------------------------
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd == &buttoncolor) {
		if (nCtlColor == CTLCOLOR_BTN) 	{
//			CString str;								// all this doesnt change the button's color
//			pWnd->GetWindowText(str);
//			TRACE("Coloring button '%s'\n", str);
//			pDC->SetTextColor(RGB(255,0,0));
//			pDC->SetBkColor(RGB(255,0,0));
//			hbr = CreateSolidBrush(RGB(255,0,0));
//			pDC->SetBkMode(OPAQUE);
//			pDC->SetDCBrushColor(RGB(255,0,0));
//			pDC->SetDCPenColor(RGB(255,0,0));
			pDC->FillSolidRect(-5,-5, 83, 33, RGB(m_color.x*255, m_color.y*255, m_color.z*255));
		}
	}

	return hbr;
}

// --------------------------------------------------------------------------
void CChunkDlg::OnCbnSelchangeComboChunkname()
// --------------------------------------------------------------------------
{
	m_cbChunkName.GetLBText(m_cbChunkName.GetCurSel(),m_sSelChunkName);
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	if(pApp->printer.GetModelPtr() == NULL) return;
	pApp->printer.GetModelPtr()->SelectOneChunk(m_sSelChunkName);
	UpdateContent();
}

// --------------------------------------------------------------------------
void CChunkDlg::OnCbnSelchangeCombopriority()
// --------------------------------------------------------------------------
{
	// TODO: Add your control notification handler code here
}

// --------------------------------------------------------------------------
void CChunkDlg::OnCbnKillfocusCombopriority()
// --------------------------------------------------------------------------
{

}

// --------------------------------------------------------------------------
void CChunkDlg::OnBnClickedApplyNow()
// --------------------------------------------------------------------------
{
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();

	int sel = combomat.GetCurSel();
	if (sel == LB_ERR) {
		m_pChunk->materialname = "";
	} else {
		combomat.GetLBText(sel,m_pChunk->materialname);
	}

	sel = combotool.GetCurSel();
	if (sel == LB_ERR) {
		m_pChunk->toolname = "";
	} else {
		combotool.GetLBText(sel,m_pChunk->toolname);
	}

	//update the backfill deposition priority for the current chunk
	sel = combopriority.GetCurSel();
	m_pChunk->SetPriority(sel);

	//update the state of the chunk preferences for bound before fill
	m_pChunk->SetBoundFirst(m_checkBoundFirst.GetCheck()>0);

	CString text;
	editalpha.GetWindowText(text);
	double alpha = atof(text);
	if (alpha < 0)
		alpha = 0;
	if (alpha > 100)
		alpha = 100;
	m_pChunk->alpha = alpha/100;

	m_pChunk->color = m_color;
}


// --------------------------------------------------------------------------
void CChunkDlg::OnBnClickedCheckBoundfirst()
// --------------------------------------------------------------------------
{
	//respond to checkbox which determines user pref for
	//depositing boundary paths before or after raster fill paths
	
}
