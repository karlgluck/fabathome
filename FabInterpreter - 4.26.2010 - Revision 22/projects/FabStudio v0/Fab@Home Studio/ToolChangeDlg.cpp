// ToolChangeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FabAtHome.h"
#include "ToolChangeDlg.h"
#include ".\toolchangedlg.h"


// CToolChangeDlg dialog

IMPLEMENT_DYNAMIC(CToolChangeDlg, CPropertyPage)
CToolChangeDlg::CToolChangeDlg(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CToolChangeDlg::IDD)
	, m_sMaxTools(_T(""))
{
}

CToolChangeDlg::~CToolChangeDlg()
{
}

void CToolChangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_AVAIL, m_lbAvailable);
	DDX_Control(pDX, IDC_LIST_MOUNTED, m_lbMounted);
	DDX_Text(pDX, IDC_MAXTOOLS, m_sMaxTools);
}


BEGIN_MESSAGE_MAP(CToolChangeDlg, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_MOUNT, OnBnClickedButtonMount)
	ON_BN_CLICKED(IDC_BUTTON_UNMOUNT, OnBnClickedButtonUnmount)
	ON_LBN_SELCHANGE(IDC_LIST_AVAIL, OnLbnSelchangeListAvail)
	ON_LBN_DBLCLK(IDC_LIST_AVAIL, OnLbnDblclkListAvail)
	ON_LBN_DBLCLK(IDC_LIST_MOUNTED, OnLbnDblclkListMounted)
END_MESSAGE_MAP()


// CToolChangeDlg message handlers

// --------------------------------------------------------------------------
BOOL CToolChangeDlg::OnInitDialog()
// --------------------------------------------------------------------------
{
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	int nTool = pApp->printer.GetMaxTools();

	if(nTool > 1)
		m_sMaxTools.Format("Configuration supports %d tools", nTool);
	else
		m_sMaxTools.Format("Configuration supports %d tool", nTool);
	CPropertyPage::OnInitDialog();
	UpdateListBoxes();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// --------------------------------------------------------------------------
bool CToolChangeDlg::UpdateListBoxes()
// --------------------------------------------------------------------------
{
	//update the contents of the list boxes
	
	//load available listbox with the currently available tools
	//and mounted listbox with currently mounted tools
	//track mounting state locally - only change
	//if user confirms with OK button
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	m_lbMounted.ResetContent();
	m_lbAvailable.ResetContent();
	for (int i=0; i<pApp->tool.GetSize(); i++) 
	{
		if(pApp->tool[i].IsMounted())
			m_lbMounted.AddString(pApp->tool[i].name);
		else
			m_lbAvailable.AddString(pApp->tool[i].name);
	}
	return true;
}

// --------------------------------------------------------------------------
void CToolChangeDlg::OnBnClickedButtonMount()
// --------------------------------------------------------------------------
{
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	int nCount = m_lbAvailable.GetSelCount();
	CArray<int,int> aryListBoxSel;

	aryListBoxSel.SetSize(nCount);
	m_lbAvailable.GetSelItems(nCount, aryListBoxSel.GetData());
	for(int i = 0; i < nCount; i++)
	{
		CString selection;
		m_lbAvailable.GetText(aryListBoxSel[i],selection);

		// Check if the name of the tool is valid
		if(CTool::SearchByName(selection) != -1)
			if(!pApp->printer.MountTool(selection))
				((CFabAtHomeApp *)AfxGetApp())->Log("Unable to mount the tool.");
	}
	UpdateListBoxes();
	UpdateDlgParameters();
	pApp->UpdateAllViews();
}

// --------------------------------------------------------------------------
void CToolChangeDlg::OnBnClickedButtonUnmount()
// --------------------------------------------------------------------------
{
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();

	if (m_lbMounted.GetCount() == 1 && pApp->printer.bHardwareInitialized)
	{
		if (MessageBox("When the hardware is connected it is needed to have at least one mounted tool.\n"
			"Are you sure you want to unmount this tool and disconnect the hardware?", 0, MB_OKCANCEL) == IDOK)
		{
			pApp->printer.ShutdownHardware();
			if (pApp->pJogCarriageDlg != NULL)
				pApp->pJogCarriageDlg->SendMessage(WM_CLOSE);
			if (pApp->pJogToolDlg != NULL)
				pApp->pJogToolDlg->SendMessage(WM_CLOSE);
		}
		else
		{
			return;
		}
	}

	int nCount = m_lbMounted.GetSelCount();
	CArray<int,int> aryListBoxSel;

	aryListBoxSel.SetSize(nCount);
	m_lbMounted.GetSelItems(nCount, aryListBoxSel.GetData());
	 
	for(int i = 0; i < nCount; i++)
	{
		CString selection;
		m_lbMounted.GetText(aryListBoxSel[i],selection);
		// Check if the name of the tool is valid
		if(CTool::SearchByName(selection) != -1)
			if(!pApp->printer.UnmountTool(selection))
				((CFabAtHomeApp *)AfxGetApp())->Log("Unable to unmount the tool.");
	}
	UpdateListBoxes();
	UpdateDlgParameters();
	pApp->UpdateAllViews();
}

// --------------------------------------------------------------------------
void CToolChangeDlg::UpdateDlgParameters()
// --------------------------------------------------------------------------
{
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();

	for(int i = 0; i < pApp->printer.GetMaxTools(); i++)
	{
		if(!pApp->pToolDlg->m_ToolDlgPropSheet->ClearParameters(i))
			return;
	}
	for (int i = 0, j = 0; i < pApp->tool.GetSize(); i++) 
	{
		if (pApp->tool[i].IsMounted())
		{
			pApp->pToolDlg->m_ToolDlgPropSheet->SetParameters(&pApp->tool[i], j);
			j++;
			//In case of there more mounted tools than the number of tabs
			if (j >= pApp->printer.GetMaxTools())
				return;
		}
	}
}

// --------------------------------------------------------------------------
void CToolChangeDlg::OnLbnSelchangeListAvail()
// --------------------------------------------------------------------------
{
	//if user tries to select more tools for mounting
	//than the configuration maximum - number of currently mounted
	//then deselect all
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	int nMtd = m_lbMounted.GetCount();
	int nAvSel = m_lbAvailable.GetSelCount();
	if(nAvSel > (pApp->printer.GetMaxTools()-nMtd))
	{
		m_lbAvailable.ResetContent();
		UpdateListBoxes();
	}
}
void CToolChangeDlg::OnLbnDblclkListAvail()
{
	OnBnClickedButtonMount();
}

void CToolChangeDlg::OnLbnDblclkListMounted()
{
	OnBnClickedButtonUnmount();
}
