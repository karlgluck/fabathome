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

// FabAtHome.h : main header file for the FabAtHome application
//

#include <io.h>

#if !defined(AFX_FabAtHome_H__99975A8F_4B67_4A31_A9C2_8AED7E51A999__INCLUDED_)
#define AFX_FabAtHome_H__99975A8F_4B67_4A31_A9C2_8AED7E51A999__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

#define NAMESTACK_CHUNK 1

/////////////////////////////////////////////////////////////////////////////
// CFabAtHomeApp:
// See FabAtHome.cpp for the implementation of this class
//

#include "Printer.h"
#include "BuildInfoDlg.h"
class CJogCarriageDlg;
class CJogToolDlg;
class CStatusDlg;
class CTool;
class CMaterial;
class CToolDlg;
class CPrinterDlg;

class CFabAtHomeApp : public CWinApp
{
public:
	void UserToolChange(); //handle tool change dialog with user 
	CTool& GetTool(int index); //return the indexed tool from the tool array
	bool ResetDirectories(CString &subDir);
	bool LoadPrinter(bool bUserSelect = false); //load printer definition file
	bool LoadTools(bool bParamsOnly = false, bool bForceDialog = false); //load tools from files, ensuring correct formats and directories
	bool LoadMaterials(); //load materials from files, ensuring correct formats and directories

     
     CFabAtHomeApp();

     
	HCURSOR hHandCursor;

	CPrinter printer;
	CString workDir; //the working directory for the Fab@Home application
	CString printerFile; //the directory for the printer definition files
	CString toolsDir; //the directory for tool files
	CString matsDir; //the directory for material files
	CString modelsDir; //the directory for model files
	CArray<CMaterial, CMaterial> material;
	CArray<CTool, CTool> tool;

	CJogCarriageDlg* pJogCarriageDlg;
	CJogToolDlg* pJogToolDlg;
	CStatusDlg* pStatusDlg;
	CBuildInfoDlg* pBuildInfoDlg;
	CToolDlg* pToolDlg;
	CPrinterDlg* pPrinterDlg;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFabAtHomeApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CFabAtHomeApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPrinterJog();

public:
	void RedrawAllViews(void);
	void UpdateAllViews(void);
	void Log(CString appendtext);
	void HideToolDlgPropSheet(void);
	void HidePrinterDlgPropSheet(void);
	virtual int ExitInstance();
	afx_msg void OnUpdatePrinterJog(CCmdUI *pCmdUI);
	afx_msg void OnPrinterJogtool();
	afx_msg void OnUpdatePrinterJogtool(CCmdUI *pCmdUI);
	afx_msg void OnPrinterInitializehardware();
	afx_msg void OnPrinterLoadconfiguration();
	afx_msg void OnUpdatePrinterInitializehardware(CCmdUI *pCmdUI);
	afx_msg void OnPrinterEnablemotion();
	afx_msg void OnUpdatePrinterEnablemotion(CCmdUI *pCmdUI);
	afx_msg void OnPrinterPausequeue();
	afx_msg void OnUpdatePrinterPausequeue(CCmdUI *pCmdUI);
	afx_msg void OnPrinterAbortallmotion();
	afx_msg void OnUpdatePrinterAbortallmotion(CCmdUI *pCmdUI);
	afx_msg void OnPrinterResethardware();
	afx_msg void OnUpdatePrinterResethardware(CCmdUI *pCmdUI);
	afx_msg void OnPrinterShowstatus();
	afx_msg void OnUpdatePrinterShowstatus(CCmdUI *pCmdUI);
	afx_msg void OnToolLoadfiles();
	afx_msg void OnUpdateToolLoadfiles(CCmdUI *pCmdUI);
	afx_msg void OnToolRefreshparameters();
	afx_msg void OnViewShowbuildinfo();
	afx_msg void OnUpdateViewShowbuildinfo(CCmdUI *pCmdUI);
	afx_msg void OnToolChangetools();
	afx_msg void OnUpdateToolChangetools(CCmdUI *pCmdUI);
	afx_msg void OnPrinterEmergencystop();
	afx_msg void OnUpdatePrinterEmergencystop(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePrinterLog(CCmdUI *pCmdUI);
	afx_msg void OnPrinterLog();
	afx_msg void OnToolToolconfiguration();
	afx_msg void OnUpdateToolToolconfiguration(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePrinterLoadconfiguration(CCmdUI *pCmdUI);
	afx_msg void OnPrinterEmulatefaster();
	afx_msg void OnUpdatePrinterEmulatefaster(CCmdUI *pCmdUI);
	afx_msg void OnPrinterEmulateslower();
	afx_msg void OnUpdatePrinterEmulateslower(CCmdUI *pCmdUI);
	afx_msg void OnEmulationEmulatehardware();
	afx_msg void OnUpdateEmulationEmulatehardware(CCmdUI *pCmdUI);
	afx_msg void OnPrinterNetworkconnect();
	afx_msg void OnUpdatePrinterNetworkconnect(CCmdUI *pCmdUI);
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FabAtHome_H__99975A8F_4B67_4A31_A9C2_8AED7E51A999__INCLUDED_)
