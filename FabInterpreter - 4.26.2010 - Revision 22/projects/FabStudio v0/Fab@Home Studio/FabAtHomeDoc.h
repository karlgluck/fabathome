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

// FabAtHomeDoc.h : interface of the CFabAtHomeDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FabAtHomeDOC_H__78EFA953_4FA2_4E2E_BC5A_0ABA08689AF7__INCLUDED_)
#define AFX_FabAtHomeDOC_H__78EFA953_4FA2_4E2E_BC5A_0ABA08689AF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CFabAtHomeDoc : public CDocument
{
protected: // create from serialization only
	CFabAtHomeDoc();
	DECLARE_DYNCREATE(CFabAtHomeDoc)

// Attributes
public:

	CModel model; 
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFabAtHomeDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
	//}}AFX_VIRTUAL

// Implementation
public:
	void ShowAllLayers();
	static UINT StartFabrication(LPVOID pParam);
	static UINT StartCalibration(LPVOID pParam);
	static UINT StartSlicing(LPVOID pParam);
	void EditMaterial();
	void RedrawViewsNow();
	virtual ~CFabAtHomeDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CFabAtHomeDoc)
	afx_msg void OnChunkImportgeometry();
	afx_msg void OnUpdateModelMaterial(CCmdUI* pCmdUI);
	afx_msg void OnFabricateFabricate();
	afx_msg void OnUpdateFabricateFabricate(CCmdUI* pCmdUI);
	afx_msg void OnFabricateSimulateonly();
	afx_msg void OnUpdateFabricateSimulateonly(CCmdUI* pCmdUI);
	afx_msg void OnFabricateCalibrate();
	afx_msg void OnUpdateFabricateCalibrate(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFabricatePause(CCmdUI* pCmdUI);
	afx_msg void OnModelDelete();
	afx_msg void OnUpdateModelDelete(CCmdUI* pCmdUI);
	afx_msg void OnEditSelectall();
	afx_msg void OnModelMovetoorigin();
	afx_msg void OnUpdateModelMovetoorigin(CCmdUI* pCmdUI);
	afx_msg void OnModelRotate();
	afx_msg void OnUpdateModelRotate(CCmdUI* pCmdUI);
	afx_msg void OnModelTranslate();
	afx_msg void OnUpdateModelTranslate(CCmdUI* pCmdUI);
	afx_msg void OnModelScale();
	afx_msg void OnUpdateModelScale(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnModelProperties();
	afx_msg void OnUpdateModelProperties(CCmdUI *pCmdUI);
	void EditChunkProperties();
	afx_msg void OnFabricationSlice();
	afx_msg void OnUpdateFabricationSlice(CCmdUI *pCmdUI);
	afx_msg void OnFabricationPauseprinting();
	afx_msg void OnUpdateFabricationPauseprinting(CCmdUI *pCmdUI);
	afx_msg void OnFabricationPauseattoolchange();
	afx_msg void OnUpdateFabricationPauseattoolchange(CCmdUI *pCmdUI);
	afx_msg void OnFabricationAutoresume();
	afx_msg void OnUpdateFabricationAutoresume(CCmdUI *pCmdUI);
	afx_msg void OnUpdateChunkImportgeometry(CCmdUI *pCmdUI);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FabAtHomeDOC_H__78EFA953_4FA2_4E2E_BC5A_0ABA08689AF7__INCLUDED_)
