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
// CToolDlgParameters dialog
// --------------------------------------------------------------------------
class CToolDlgParameters : public CPropertyPage
{
	DECLARE_DYNAMIC(CToolDlgParameters)

public:
	CToolDlgParameters();
	virtual ~CToolDlgParameters();

// Dialog Data
	enum { IDD = IDD_TOOLDLGPARAMETERS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int tool_index; // index of the tool that this property pages shows
	CToolTipCtrl* m_pToolTip;
	CEdit m_BACKFILL;
	CEdit m_BARRELCOLOR;
	CEdit m_BARRELDIAM;
	CEdit m_BARRELGEOM;
	CEdit m_BARRELOFFSET;
	CEdit m_CLEARANCE;
	CEdit m_DEPOSITIONRATE;
	CEdit m_DESCRIPTION;
	CEdit m_DIRECTION;
	CEdit m_INCREMENT;
	CEdit m_MINPATHLENGTH;
	CEdit m_MOTOR;
	CEdit m_PATHHEIGHT;
	CEdit m_PATHSPEED;
	CEdit m_PATHWIDTH;
	CEdit m_PAUSEPATHS;
	CEdit m_PLUNGERCOLOR;
	CEdit m_PLUNGERGEOM;
	CEdit m_PLUNGEROFFSET;
	CEdit m_PUSHOUT;
	CEdit m_RANGE;
	CEdit m_SUCKBACK;
	CEdit m_SUCKBACKDELAY;
	CEdit m_TIPCOLOR;
	CEdit m_TIPDIAM;
	CEdit m_TIPGEOM;
	CEdit m_TIPOFFSET;
	CEdit m_TIPPOINT;
	CEdit m_TOOLCOLOR;
	CEdit m_TOOLGEOM;
	CEdit m_TOOLOFFSET;
	CEdit m_VOLUME;
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

class CToolDlgPropSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CToolDlgPropSheet)

public:
	CToolDlgPropSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CToolDlgPropSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CToolDlgPropSheet();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
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

	CToolDlgParameters* syringes;
	CToolChangeDlg* ToolChangeDlg;

	bool SetParameters(CTool *tool, int idx); //Set the paremeters of the specified tool in the indexed (idx) tab of the dialog
	bool ClearParameters(int idx); //Clear the paremeters of the specified tool in the indexed (idx) tab of the dialog

protected:
	bool RefreshParameters(CTool *tool, int idx); //Refresh the paremeters of the specified tool in the indexed (idx) tab of the dialog
	bool SaveParameters(CTool *tool, int idx, CString filename = ""); //Refresh the paremeters of the specified tool in the indexed (idx) tab of the dialog
	CTool *GetActiveTool(void); // It returns the CTool class of the active tab
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
// --------------------------------------------------------------------------


// --------------------------------------------------------------------------
// CToolDlg frame
// --------------------------------------------------------------------------
class CToolDlg : public CMiniFrameWnd
{
	DECLARE_DYNCREATE(CToolDlg)
	CToolDlg();
	virtual ~CToolDlg();	

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	CToolDlgPropSheet *m_ToolDlgPropSheet;
	CFabAtHomeApp *pApp;
};
// --------------------------------------------------------------------------