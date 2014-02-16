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

// AxesDlg.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAxesDlg dialog


//--------------------------------------------------------------------
CAxesDlg::CAxesDlg(CWnd* pParent /*=NULL*/)
//--------------------------------------------------------------------
	: CDialog(CAxesDlg::IDD, pParent)
	, m_cstrAxesActionName(_T(""))
{
	//{{AFX_DATA_INIT(CAxesDlg)
	m_xax = _T("");
	m_yax = _T("");
	m_zax = _T("");
	//}}AFX_DATA_INIT
}

//--------------------------------------------------------------------
CAxesDlg::CAxesDlg(CWnd* pParent /*=NULL*/, CString& actionName)
//--------------------------------------------------------------------
	: CDialog(CAxesDlg::IDD, pParent)
	, m_cstrAxesActionName(actionName)
{
	//{{AFX_DATA_INIT(CAxesDlg)
	m_xax = _T("");
	m_yax = _T("");
	m_zax = _T("");
	//}}AFX_DATA_INIT
}

//--------------------------------------------------------------------
void CAxesDlg::DoDataExchange(CDataExchange* pDX)
//--------------------------------------------------------------------
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAxesDlg)
	DDX_Text(pDX, IDC_XAX, m_xax);
	DDX_Text(pDX, IDC_YAX, m_yax);
	DDX_Text(pDX, IDC_ZAX, m_zax);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_AXES_STATIC, m_cstrAxesActionName);
}


BEGIN_MESSAGE_MAP(CAxesDlg, CDialog)
	//{{AFX_MSG_MAP(CAxesDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxesDlg message handlers
