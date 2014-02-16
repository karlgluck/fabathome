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

// ComPortDialog.cpp : implementation file
//

#include "stdafx.h"


// CComPortDialog dialog

IMPLEMENT_DYNAMIC(CComPortDialog, CDialog)
CComPortDialog::CComPortDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CComPortDialog::IDD, pParent)
{
	m_bPortChanged = false;
}

CComPortDialog::~CComPortDialog()
{
}

void CComPortDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PORT, m_cbPortList);
}


BEGIN_MESSAGE_MAP(CComPortDialog, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_PORT, OnCbnSelchangeComboPort)
END_MESSAGE_MAP()


// CComPortDialog message handlers

void CComPortDialog::OnCbnSelchangeComboPort()
{
	//update the port string
	int curSel = m_cbPortList.GetCurSel();
	
	if( curSel >= 0 )
	{
		m_cbPortList.GetLBText(curSel,m_sPortString);
		return;
	}
	//otherwise we failed
	m_sPortString.Empty();
	return;
}


int CComPortDialog::SearchPort( CString &srch )
{
	//search for a portname in the combobox list which contains
	//the string srch
	CString tmp;
	int bufLen;
	for(int i = 0; i < m_cbPortList.GetCount(); i++)
	{
		bufLen = m_cbPortList.GetLBTextLen(i);
		m_cbPortList.GetLBText(i, tmp.GetBuffer(bufLen));
		tmp.ReleaseBuffer();
		if(tmp.Find(srch)>=0)
			return i;
	}
	return -1;
}

BOOL CComPortDialog::OnInitDialog(void)
{
	CDialog::OnInitDialog();

	int curSel = -1;
	CString desc;
	//flush out the combo box
	m_cbPortList.ResetContent();
	//enumerate the available com ports
	CUIntArray arPorts; //array of all available ports
	CStringArray arPortNames; //array of port names
	if( CEnumerateSerial::UsingSetupAPI(arPorts,arPortNames) )  //get info on all free ports
	{
		for( int i = 0; i < arPorts.GetSize(); i++ )
		{
			//put descriptive strings for each port into the combobox list
			desc.Format("%s%d%s %s", COM_PREFIX, arPorts[i], COM_SUFFIX, arPortNames[i]);
			m_cbPortList.AddString(desc);
		}
	}
	//if the port is open, it may not be in the list
	if( !m_sPortString.IsEmpty() )
	{
		if( m_cbPortList.FindString(-1, m_sPortString) == CB_ERR )	//not found
		{
			//so force it in
			m_cbPortList.AddString(m_sPortString);
		}
		//in any case, make it the current selection
		curSel = m_cbPortList.SelectString(-1, m_sPortString);
	}
	else
	{
		//suggest a likely port to the user

		int i = 0;
		while( (i < SEARCH_STR_CNT) && (curSel < 0) )
		{
			desc = SEARCH_STR[i];
			curSel = SearchPort(desc);
			i++;
		}
	}
	//set the current selection
	if(curSel < 0) curSel = 0;
	m_cbPortList.SetCurSel( curSel );
	OnCbnSelchangeComboPort();

	return TRUE;  // return TRUE unless you set the focus to a control
			         // EXCEPTION: OCX Property Pages should return FALSE
}

void CComPortDialog::OnOK() 
{
	CDialog::OnOK();
	m_bPortChanged = true;
}

void CComPortDialog::OnCancel()
{
	CDialog::OnCancel();
	m_bPortChanged = false;
}
