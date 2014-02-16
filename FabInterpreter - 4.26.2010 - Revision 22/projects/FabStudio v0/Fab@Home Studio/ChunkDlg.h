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

class CChunk;

// CChunkDlg dialog

class CChunkDlg : public CDialog
{
	DECLARE_DYNAMIC(CChunkDlg)

public:
	CChunkDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CChunkDlg();

// Dialog Data
	enum { IDD = IDD_CHUNKDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox combomat;
	CComboBox combotool;
	CComboBox combopriority;
	CStatic staticmat;
	CStatic statictool;
	CButton buttoncolor;
	CEdit editalpha;
	CButton m_checkBoundFirst;
	virtual BOOL OnInitDialog();
	CVec m_color;
	CChunk *m_pChunk;
	void UpdateContent();
	
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedButtoncolor();
	afx_msg void OnCbnSelchangeCombomat();
	afx_msg void OnCbnSelchangeCombotool();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCbnSelchangeCombopriority();
	int m_nPriority;
	afx_msg void OnCbnSelchangeComboChunkname();
	CComboBox m_cbChunkName;
	CString m_sSelChunkName;
	afx_msg void OnCbnKillfocusCombopriority();
	afx_msg void OnBnClickedApplyNow();
	afx_msg void OnBnClickedCheckBoundfirst();
};
