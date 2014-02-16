#if !defined(__LOGBAR_H__)
#define __LOGBAR_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// LogBar.h : interface of the CLogBar class
//

/////////////////////////////////////////////////////////////////////////////
// CLogBar window

class CLogBar : public CSizingControlBarCF
{
// Construction
public:
	CLogBar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLogBar();
	CEdit	m_wndChild;

protected:
	CFont	m_font;

	// Generated message map functions
protected:
	//{{AFX_MSG(CLogBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(__LOGBAR_H__)
