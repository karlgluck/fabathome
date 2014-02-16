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

// FabAtHomeView.h : interface of the CFabAtHomeView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FabAtHomeVIEW_H__DBB5840A_C0B1_11D4_A896_002078E0CCA0__INCLUDED_)
#define AFX_FabAtHomeVIEW_H__DBB5840A_C0B1_11D4_A896_002078E0CCA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "gl\gl.h"
#include "gl\glu.h"

class CFabAtHomeView : public CView
{
protected: // create from serialization only
	CFabAtHomeView();
	DECLARE_DYNCREATE(CFabAtHomeView)
	void InitGL();
    BOOL bSetupPixelFormat(void);
	void ResetView(void); //reset the view to its initial condition
    void DrawScene(void);
	void DrawStatusInfo( CDC* pDC ); //display status of physical printer
	void SetViewTransformation();
	void DrawMainAxes(double size);
	void UpdatePerspective();
	void UpdateCurrentLayer();
	void Pick(UINT nFlags, CPoint point);

// Attributes
public:
	static bool ReportGLError(CString& note, CStdioFile *pFile = NULL);
	CFabAtHomeDoc* GetDocument();

	double		m_fov; // field of view (degree)
	HGLRC		hrc;
	CRect		m_oldRect;
	float		m_fRadius;
	CClientDC	*m_pDC;
	double		scale;

	bool		bRolling;
	bool		bPanning;
	CPoint		startPoint;
	CVec		eye;
	CVec		view;
	CVec		up;
	GLdouble	rotmat[16];
	GLdouble	rotmat0[16];

	bool bShowPrinter;
	bool bShowModel;
	bool bShowPaths;
	bool bShowTrace;
	bool bShowStatus;
	bool bShowTool;
	bool bShaded;
	bool bShowCommanded; // show commanded positions vs show "real-time"
	int firstlayer, lastlayer;
	bool bModelNormals;
	bool bPathNormals;
	bool bFollowCurrentLayer;
protected:
	UINT m_uiTimer;
	DWORD m_dwTimerPeriod; //period for view update timer
	CMutex m_mutAccess; //mutex for thread safety
	bool m_bDrawing; //flag to indicate drawing in progress

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFabAtHomeView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetLighting();
	virtual ~CFabAtHomeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CFabAtHomeView)
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnViewShaded();
	afx_msg void OnUpdateViewShaded(CCmdUI* pCmdUI);
	afx_msg void OnViewShowpaths();
	afx_msg void OnUpdateViewShowpaths(CCmdUI* pCmdUI);
	afx_msg void OnViewShowmodel();
	afx_msg void OnUpdateViewShowmodel(CCmdUI* pCmdUI);
	afx_msg void OnViewNextpath();
	afx_msg void OnViewPreviouspath();
	afx_msg void OnViewAllpaths();
	afx_msg void OnUpdateViewNextpath(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewAllpaths(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewPreviouspath(CCmdUI* pCmdUI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnViewShowtool();
	afx_msg void OnUpdateViewShowtool(CCmdUI* pCmdUI);
	afx_msg void OnViewNormals();
	afx_msg void OnUpdateViewNormals(CCmdUI* pCmdUI);
	afx_msg void OnViewModelnormals();
	afx_msg void OnUpdateViewModelnormals(CCmdUI* pCmdUI);
	afx_msg void OnViewPathnormals();
	afx_msg void OnUpdateViewPathnormals(CCmdUI* pCmdUI);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnViewShowstatus();
	afx_msg void OnUpdateViewShowstatus(CCmdUI *pCmdUI);
	afx_msg void OnViewShowcommanded();
	afx_msg void OnUpdateViewShowcommanded(CCmdUI *pCmdUI);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnViewZoomin();
	afx_msg void OnViewZoomout();
	afx_msg void OnViewCurrentpath();
	afx_msg void OnUpdateViewCurrentpath(CCmdUI *pCmdUI);
	afx_msg void OnViewResetview();
	afx_msg void OnViewShowprinter();
	afx_msg void OnUpdateViewShowprinter(CCmdUI *pCmdUI);
	afx_msg void OnViewShowtrace();
	afx_msg void OnUpdateViewShowtrace(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // debug version in FabAtHomeView.cpp
inline CFabAtHomeDoc* CFabAtHomeView::GetDocument()
   { return (CFabAtHomeDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FabAtHomeVIEW_H__DBB5840A_C0B1_11D4_A896_002078E0CCA0__INCLUDED_)
