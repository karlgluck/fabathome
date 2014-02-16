#pragma once


// CChunkPropertiesDlg dialog

class CChunkPropertiesDlg : public CDialog
{
	DECLARE_DYNAMIC(CChunkPropertiesDlg)

public:
	CChunkPropertiesDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CChunkPropertiesDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
