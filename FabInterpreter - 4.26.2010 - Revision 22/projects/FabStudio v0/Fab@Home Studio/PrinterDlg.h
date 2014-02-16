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

#pragma once
#include "afxwin.h"

// --------------------------------------------------------------------------
// CPrinterDlgGeneral
// --------------------------------------------------------------------------
class CPrinterDlgGeneral : public CPropertyPage
{
	DECLARE_DYNAMIC(CPrinterDlgGeneral)

public:
	CPrinterDlgGeneral();
	virtual ~CPrinterDlgGeneral();

// Dialog Data
	enum { IDD = IDD_PRINTERDLGGENERAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CToolTipCtrl* m_pToolTip;
	CEdit m_DEFAULT_POD;
	CEdit m_DESCRIPTION;
	CEdit m_JOGSPEED;
	CEdit m_MAXTOOLS;
	CEdit m_Name;
	CEdit m_STATUS_UPDATE_PERIOD;
	CEdit m_TOOLLIMITSWITCH;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
// --------------------------------------------------------------------------


// --------------------------------------------------------------------------
// CPrinterDlgComponent
// --------------------------------------------------------------------------
class CPrinterDlgComponent : public CPropertyPage
{
	DECLARE_DYNAMIC(CPrinterDlgComponent)

public:
	CPrinterDlgComponent();
	virtual ~CPrinterDlgComponent();

// Dialog Data
	enum { IDD = IDD_PRINTERDLGCOMPONENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CToolTipCtrl* m_pToolTip;
	CEdit m_APPENDGEOM;
	CEdit m_COLOR;
	CEdit m_COMPONENT;
	CEdit m_DIRECTION;
	CEdit m_INCREMENT;
	CEdit m_LIMIT_SWITCH;
	CEdit m_MOTOR;
	CEdit m_OFFSET;
	CEdit m_RANGE;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
// --------------------------------------------------------------------------


// --------------------------------------------------------------------------
// CToolDlgPropSheet
// --------------------------------------------------------------------------
#define IDD_SAVE_BUTTON 1
#define IDD_CANCEL_BUTTON 2
#define IDD_CLOSE_BUTTON 3
#define IDD_REFRESH_BUTTON 4
#define IDD_IMPORT_BUTTON 5
#define IDD_EXPORT_BUTTON 6

#define BASE 1
#define X_CARRIAGE 2
#define Y_CARRIAGE 3
#define Z_CARRIAGE 4
#define MAX_COMPONENTS 4

class CPrinterDlgPropSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CPrinterDlgPropSheet)

public:
	CPrinterDlgPropSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPrinterDlgPropSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CPrinterDlgPropSheet();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedButtonRefresh();
	afx_msg void OnBnClickedButtonImport();
	afx_msg void OnBnClickedButtonExport();

	CToolTipCtrl* m_pToolTip;
	CButton Save;
	CButton Cancel;
	CButton Close;
	CButton Refresh;
	CButton Import;
	CButton Export;
	CFabAtHomeApp *pApp;
	CPrinterDlgGeneral m_PrinterDlgGeneral;
	CPrinterDlgComponent m_PrinterDlgComponent[MAX_COMPONENTS];
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	bool SetParameters(void); //Set the paremeters of the current loaded printer to the dialog
	bool ClearParameters(void); //Clear the paremeters of the current loaded printer to the dialog

protected:
	bool RefreshParameters(void); //Refresh the paremeters of the current loaded printer to the dialog
	bool SaveParameters(CString filename = ""); //Refresh the paremeters of the current loaded printer to the dialog
};
// --------------------------------------------------------------------------


// --------------------------------------------------------------------------
// CPrinterDlg frame
// --------------------------------------------------------------------------
class CPrinterDlg : public CMiniFrameWnd
{
	DECLARE_DYNCREATE(CPrinterDlg)
public:
	CPrinterDlg();           // protected constructor used by dynamic creation
	virtual ~CPrinterDlg();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	CFabAtHomeApp *pApp;
	CPrinterDlgPropSheet *m_PrinterDlgPropSheet;
};
// --------------------------------------------------------------------------