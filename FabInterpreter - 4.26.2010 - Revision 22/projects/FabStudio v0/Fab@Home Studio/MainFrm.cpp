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

// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include ".\mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, CFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, CFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CFrameWnd::OnHelpFinder)
	ON_UPDATE_COMMAND_UI(ID_FABRICATION_STATUS, OnUpdateFabricationStatus)
	ON_COMMAND(ID_VIEW_LOGBAR, &CMainFrame::OnViewLogbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LOGBAR, &CMainFrame::OnUpdateViewLogbar)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_FABRICATION_STATUS
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

// --------------------------------------------------------------------------
CMainFrame::CMainFrame()
// --------------------------------------------------------------------------
{
	// TODO: add member initialization code here
	
}

// --------------------------------------------------------------------------
CMainFrame::~CMainFrame()
// --------------------------------------------------------------------------
{
}

// --------------------------------------------------------------------------
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
// --------------------------------------------------------------------------
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_wndStatusBar.SetPaneInfo(1,ID_FABRICATION_STATUS,SBPS_STRETCH,200); 

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	// The Log Bar Creation
	if (!m_wndLogBar.Create(_T("Log Window"), this, IDD_LOGBAR))
	{
		TRACE0("Failed to create mybar\n");
		return -1;
		// fail to create
	}
	m_wndLogBar.SetBarStyle(m_wndLogBar.GetBarStyle() |	CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	m_wndLogBar.EnableDocking(CBRS_ALIGN_ANY);

	EnableDocking(CBRS_ALIGN_ANY);

	#ifdef _SCB_REPLACE_MINIFRAME
		m_pFloatingFrameClass = RUNTIME_CLASS(CSCBMiniDockFrameWnd);
	#endif //_SCB_REPLACE_MINIFRAME

	DockControlBar(&m_wndLogBar, AFX_IDW_DOCKBAR_BOTTOM);

	// Load LogBarState
	CString sProfile = _T("LogBarState");
	if (VerifyBarState(sProfile))
	{
		CSizingControlBar::GlobalLoadState(this, sProfile);
		LoadBarState(sProfile);
	}

	return 0;
}

// --------------------------------------------------------------------------
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
// --------------------------------------------------------------------------
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
// --------------------------------------------------------------------------
void CMainFrame::AssertValid() const
// --------------------------------------------------------------------------
{
	CFrameWnd::AssertValid();
}

// --------------------------------------------------------------------------
void CMainFrame::Dump(CDumpContext& dc) const
// --------------------------------------------------------------------------
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

// --------------------------------------------------------------------------
void CMainFrame::OnClose()
// --------------------------------------------------------------------------
{
	//gracefully shutdown the printer object and hardware
	CFabAtHomeApp *pApp = (CFabAtHomeApp*)AfxGetApp();
	pApp->printer.Shutdown();

	CFrameWnd::OnClose();
}

// --------------------------------------------------------------------------
void CMainFrame::OnUpdateFabricationStatus(CCmdUI *pCmdUI)
// --------------------------------------------------------------------------
{
	//update the status bar with information on the system status
	CFabAtHomeApp *pApp = (CFabAtHomeApp*)AfxGetApp();
	pCmdUI->Enable(); 
	CString msg;

	if(!pApp->printer.StatusOK() || pApp->printer.GetModelPtr() == NULL)
		pCmdUI->SetText( pApp->printer.GetStatusBarMessage() );
	else
	{
		CSingleLock mdlLock(&(pApp->printer.GetModelPtr()->m_mutAccess));
		mdlLock.Lock(100); //wait only 100ms for lock - else give up
		if(mdlLock.IsLocked())
		{
			if( pApp->printer.GetModelPtr()->fab.IsFabricating())
			{
				pCmdUI->SetText( pApp->printer.GetModelPtr()->fab.GetStatusMsg() );
			}
			else
			{
				pCmdUI->SetText( pApp->printer.GetModelPtr()->GetStatusMsg() );
			}
		}
	}
}

// --------------------------------------------------------------------------
// This function is Copyright (c) 2000, Cristi Posea.
// See www.datamekanix.com for more control bars tips&tricks.
BOOL CMainFrame::VerifyBarState(LPCTSTR lpszProfileName)
// --------------------------------------------------------------------------
{
    CDockState state;
    state.LoadState(lpszProfileName);

    for (int i = 0; i < state.m_arrBarInfo.GetSize(); i++)
    {
        CControlBarInfo* pInfo = (CControlBarInfo*)state.m_arrBarInfo[i];
        ASSERT(pInfo != NULL);
        int nDockedCount = pInfo->m_arrBarID.GetSize();
        if (nDockedCount > 0)
        {
            // dockbar
            for (int j = 0; j < nDockedCount; j++)
            {
                UINT nID = (UINT) pInfo->m_arrBarID[j];
                if (nID == 0) continue; // row separator
                if (nID > 0xFFFF)
                    nID &= 0xFFFF; // placeholder - get the ID
                if (GetControlBar(nID) == NULL)
                    return FALSE;
            }
        }
        
        if (!pInfo->m_bFloating) // floating dockbars can be created later
            if (GetControlBar(pInfo->m_nBarID) == NULL)
                return FALSE; // invalid bar ID
    }

    return TRUE;
}

// --------------------------------------------------------------------------
void CMainFrame::OnViewLogbar()
// --------------------------------------------------------------------------
{
	BOOL bShow = m_wndLogBar.IsVisible();
	ShowControlBar(&m_wndLogBar, !bShow, FALSE);
}

// --------------------------------------------------------------------------
void CMainFrame::OnUpdateViewLogbar(CCmdUI *pCmdUI)
// --------------------------------------------------------------------------
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(m_wndLogBar.IsVisible());
}

// --------------------------------------------------------------------------
BOOL CMainFrame::DestroyWindow()
// --------------------------------------------------------------------------
{

	// Save LogBarState
	CString sProfile = _T("LogBarState");
	CSizingControlBar::GlobalSaveState(this, sProfile);
	SaveBarState(sProfile);

	return CFrameWnd::DestroyWindow();
}

// --------------------------------------------------------------------------