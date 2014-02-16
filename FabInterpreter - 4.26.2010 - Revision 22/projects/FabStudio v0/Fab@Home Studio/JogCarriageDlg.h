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

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

// CJogCarriageDlg dialog

class CJogCarriageDlg : public CDialog
{
	DECLARE_DYNAMIC(CJogCarriageDlg)

public:
	CJogCarriageDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CJogCarriageDlg();

// Dialog Data
	enum { IDD = IDD_JOGCARRIAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposSpinx(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpiny(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinz(NMHDR *pNMHDR, LRESULT *pResult);
	CToolTipCtrl* m_pToolTip;
	CSpinButtonCtrl spinX;
	CSpinButtonCtrl spinY;
	CSpinButtonCtrl spinZ;
	CEdit xeditctrl;
	CEdit yeditctrl;
	CEdit zeditctrl;
	CEdit xrtpos;
	CEdit yrtpos;
	CEdit zrtpos;
	CEdit xinc;
	CEdit yinc;
	CEdit zinc;
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonhome();
	afx_msg void OnBnClickedButtonsetorigin();
	afx_msg void OnBnClickedButtongotoorigin();
	void UpdatePositionReadout(bool bAll = true);
protected:
	UINT m_uiTimer;
	CString oldXCmd,oldYCmd,oldZCmd;

protected:
	virtual void OnOK();
	virtual void OnCancel();
public:
	afx_msg void OnBnClickedButtonsethome();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnJogXBkd();
	afx_msg void OnJogXFwd();
	afx_msg void OnJogYLeft();
	afx_msg void OnJogYRight();
	afx_msg void OnJogZDown();
	afx_msg void OnJogZUp();
	afx_msg void OnBnClickedButtonsetsafe();
	afx_msg void OnBnClickedButtongotosafe();
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
