// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

// --------------------------------------------------------------------------
CChildFrame::CChildFrame()
// --------------------------------------------------------------------------
{
	// TODO: add member initialization code here
	
}

// --------------------------------------------------------------------------
CChildFrame::~CChildFrame()
// --------------------------------------------------------------------------
{
}

// --------------------------------------------------------------------------
BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
// --------------------------------------------------------------------------
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	if( !CChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
// --------------------------------------------------------------------------
void CChildFrame::AssertValid() const
// --------------------------------------------------------------------------
{
	CChildWnd::AssertValid();
}

// --------------------------------------------------------------------------
void CChildFrame::Dump(CDumpContext& dc) const
// --------------------------------------------------------------------------
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers
