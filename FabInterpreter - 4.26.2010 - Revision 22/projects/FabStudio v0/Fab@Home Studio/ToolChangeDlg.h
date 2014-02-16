#pragma once
#include "afxwin.h"


// CToolChangeDlg dialog

class CToolChangeDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CToolChangeDlg)

public:
	CToolChangeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CToolChangeDlg();

	bool UpdateListBoxes();

// Dialog Data
	enum { IDD = IDD_TOOLCHANGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedButtonMount();
	afx_msg void OnBnClickedButtonUnmount();
	afx_msg void OnLbnSelchangeListAvail();
	void UpdateDlgParameters(void);

protected:
	CListBox m_lbAvailable;
	CListBox m_lbMounted;
	CString m_sMaxTools;

public:
	afx_msg void OnLbnDblclkListAvail();
	afx_msg void OnLbnDblclkListMounted();
};
