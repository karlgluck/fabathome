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


// CJogToolDlg dialog

class CJogToolDlg : public CDialog
{
	DECLARE_DYNAMIC(CJogToolDlg)

public:
	CJogToolDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CJogToolDlg();
	void UpdatePositionReadout(void);

// Dialog Data
	enum { IDD = IDD_JOGTOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox combotool;
	CEdit editpos;
	CEdit editRTPos;
	CEdit editInc;
	CSpinButtonCtrl spinpos;
	CButton m_cbActive;
//	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
protected:
	UINT m_uiTimer;
protected:
	virtual void OnOK();
public:
	afx_msg void OnDeltaposSpinpos(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeCombotool();
//	afx_msg void OnClose();
protected:
//	virtual void PostNcDestroy();
public:
//	virtual BOOL DestroyWindow();
	afx_msg void OnDestroy();
protected:
	virtual void OnCancel();
public:
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnJogToolBkd();
	afx_msg void OnJogToolFwd();
	afx_msg void OnBnClickedCheckCurrent();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
