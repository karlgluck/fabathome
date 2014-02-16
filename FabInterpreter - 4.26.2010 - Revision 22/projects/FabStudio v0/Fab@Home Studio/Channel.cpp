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

#pragma once
#include "StdAfx.h"

//for logging/tracing
#define STATUS_PREFIX "CChannel: "
#define STATUS_SUFFIX "\n"

IMPLEMENT_DYNAMIC(CChannel,CSerialPort);

//construct and init some variables
CChannel::CChannel(void)
{
	m_bWriting = false;
	m_bReading = false;
	m_bLogging = false;
	m_dwTxPktIdx = 0;
	m_dwRxPktIdx = 0;
	ASSERT( (rxBuf != NULL) && (txBuf != NULL) );
	for(int i = 0; i<MAX_PACKET; i++)
	{
		rxBuf[i] = 0;
		txBuf[i] = 0;
	}
}

CChannel::~CChannel(void)
{
	Disconnect();
}

bool CChannel::Connect()
{
	//check that the port info is reasonable
	if( (m_nPortNum < MIN_PORT_NUM) || (m_nPortNum > MAX_PORT_NUM) )
	{
		m_sStatusMsg.Format("Bad Port Number");
		StatusInfo();
		return false;
	}

	//if already open, then close and reopen
	if( IsOpen() )
	{
		Close();
	}
	CSerialPort::Open(m_nPortNum,BITSPERSEC,PARITY,DATABITS,STOPBITS,FLOWCTRL,OVERLAP);
	if( IsOpen() )
	{
		//setup the timeouts
		COMMTIMEOUTS commTO;
		commTO.ReadIntervalTimeout = RX_INTVL;
		commTO.ReadTotalTimeoutMultiplier = RX_MULT;
		commTO.ReadTotalTimeoutConstant = RX_CONST;
		commTO.WriteTotalTimeoutMultiplier = TX_MULT;
		commTO.WriteTotalTimeoutConstant = TX_CONST;
		SetTimeouts(commTO);
		GetTimeouts(commTO);
		ClearReadBuffer();
		ClearWriteBuffer();
	}
	return StatusGood();
}

bool CChannel::Disconnect()
{
	if( IsOpen() )
	{
		TerminateOutstandingReads();
		TerminateOutstandingWrites();
		Close();
		m_sStatusMsg.Format("Closed");
		StatusInfo();
		return true;
	}
	return false;
}

bool CChannel::AutoFindPort()
{
	//search for a port with a likely name, and connect to it automatically
	//return true is successful
	CUIntArray arPorts; //array of all available ports
	CStringArray arPortNames; //array of port names
	m_sStatusMsg.Format("Searching for port...");
	StatusInfo();
	if( CEnumerateSerial::UsingSetupAPI(arPorts,arPortNames) )  //get info on all ports
	{
		int i = 0;
		int j;
		CString desc, tmp;
		while( i < SEARCH_STR_CNT )
		{
			desc = SEARCH_STR[i];	
			for(j = 0; j < arPorts.GetSize(); j++)
			{
				tmp = arPortNames[j];
				if(tmp.Find(desc)>=0)
				{
					m_nPortNum = arPorts[j];
					m_sPortName = arPortNames[j];
					return Connect();
				}
			}
			i++;
		}
	}
	m_sStatusMsg.Format("Port Not Found");
	StatusInfo();
	((CFabAtHomeApp *)AfxGetApp())->Log(m_sStatusMsg);
	return false;
}

bool CChannel::DoDialog(void)
{
	//if the channel is open, then init the dialog with the description of the port
	CString portStr;
	portStr.Empty();
	if( IsOpen() )
	{
		portStr.Format("%s%d%s %s", COM_PREFIX, m_nPortNum, COM_SUFFIX, m_sPortName);
	}
	m_comPortDlg.SetPortString(portStr);
	m_comPortDlg.DoModal();
	//if the user OK'd the dialog (rather than canceled)
	if( m_comPortDlg.IsPortChanged() )
	{
		//update the port information from the user selection
		ExtractDlgPortInfo();
		//connect/refresh connection to the port
		return(Connect());
	}
	//user didn't change anything
	return StatusGood();
}

bool CChannel::StatusGood()
{
	//verify that the com channel is working correctly
	//is it open
	if(!IsOpen())
	{
		m_sStatusMsg.Format("Status:Disconnected");
		StatusInfo();
		return false;
	}
	//are there any errors
	DWORD errors = 0;
	ClearError(errors);
	if(errors != 0)
	{
		m_sStatusMsg.Format("Status:Error");
		StatusInfo();
		ParseError(errors);
		return false;
	}
	//port looks good
	m_sStatusMsg.Format("Status:Good");
	StatusInfo();
	return true;
}

bool CChannel::ParseError(DWORD errors)
{
	//create strings representing the com port errors
	if(errors & CE_BREAK)
	{
		m_sStatusMsg.Format("Hardware Break");
		StatusInfo();
	}
	if(errors & CE_DNS)
	{
		m_sStatusMsg.Format("Parallel Device Not Selected");
		StatusInfo();
	}
	if(errors & CE_FRAME)
	{
		m_sStatusMsg.Format("Framing Error");
		StatusInfo();
	}
	if(errors & CE_IOE)
	{
		m_sStatusMsg.Format("Device IO Error");
		StatusInfo();
	}
	if(errors & CE_MODE)
	{
		m_sStatusMsg.Format("Mode Not Supported");
		StatusInfo();
	}
	if(errors & CE_OOP)
	{
		m_sStatusMsg.Format("Win ME/95:Out Of Paper");
		StatusInfo();
	}
	if(errors & CE_OVERRUN)
	{
		m_sStatusMsg.Format("Char Buffer Overrun");
		StatusInfo();
	}
	if(errors & CE_PTO)
	{
		m_sStatusMsg.Format("Win ME/95:Parallel Timeout");
		StatusInfo();
	}
	if(errors & CE_RXOVER)
	{
		m_sStatusMsg.Format("RX Buffer Overflow");
		StatusInfo();
	}
	if(errors & CE_RXPARITY)
	{
		m_sStatusMsg.Format("RX Parity Error");
		StatusInfo();
	}
	if(errors & CE_TXFULL)
	{
		m_sStatusMsg.Format("TX Buffer Full");
		StatusInfo();
	}
	return true;
}

bool CChannel::Repair()
{
	//try to automatically fix the channel
	//check that the port is good
	m_sStatusMsg.Format("Repairing...");
	StatusInfo();
	if(!StatusGood())
	{
		//if not, try to reconnect automatically
		if(AutoFindPort())
			return StatusGood();
		//if that fails, ask user to find the port
		if(DoDialog())
			return StatusGood();
	}
	return false;
}

bool CChannel::ExtractDlgPortInfo()
{
	//get the port number and name from the port string
	//filled in by the dialog

	CString portStr = m_comPortDlg.GetPortString();
	if( !portStr.IsEmpty() )
	{
		int indx = 0;
		CString tmp,tokens;
		tokens.Format("%s%s", COM_PREFIX, COM_SUFFIX);
		tmp = portStr.Tokenize(tokens,indx);
		m_nPortNum = atoi((LPCTSTR)tmp);
		tokens.Format("%s", COM_SUFFIX);
		tmp = portStr.Tokenize(tokens,indx);
		m_sPortName = tmp.TrimLeft(" ");
		return true;
	}
	//otherwise we failed
	m_nPortNum = -1;
	m_sPortName.Empty();
	return false;
}

CString CChannel::GetPortString()
{
	CString portStr;
	portStr.Format("%s%d%s %s", COM_PREFIX, m_nPortNum, COM_SUFFIX, m_sPortName );
	return portStr;
}

//send the packet, thread safe if locked
bool CChannel::Send(CPacket *pPkt, bool locked, DWORD timeout)
{
	//if the last packet is not done writing yet, then fail
	//calling fn should just keep trying to call Send again until
	//it works, or signal an error if too many tries fail

	CSingleLock sendLock(&m_mutTransfer);
	if(locked) //lock the mutex for thread safety
	{
		if(!sendLock.Lock(timeout)) return false; //lock timed out, so fail
	}

	//note that we are attempting to write now
	m_bWriting = true;
	m_sStatusMsg.Format("Sending...");
	StatusInfo();

	//now make sure the port is good; try to repair a few times if not
	int retry = 0;
	while(!StatusGood() && (retry < RETRIES))
	{
		Repair();
		Sleep(RETRY_DLY);
		retry++;
	}
	if(StatusGood())
	{
		unsigned int cnt = pPkt->GetSize(); //get the tx packet size
		//convert the packet into a byte stream in the channel's tx buffer
		if( pPkt->PacketToStream(txBuf, &cnt) )
		{		
			TRY
			{
				cnt = Write(txBuf, cnt);
			}
			CATCH( CSerialException, pEx )
			{
				pEx->ReportError();
				m_sStatusMsg.Format("Exception During Write");
				StatusInfo();
				m_bWriting = false;
				return false;
			}
			END_CATCH
			//if we were able to write the whole thing
			if(cnt == pPkt->GetSize())
			{
				//save the packet's index
				m_dwTxPktIdx = pPkt->GetIndex();
				m_bWriting = false;
				m_sStatusMsg.Format("...Successful");
				StatusInfo();
				return true;
			}
			m_sStatusMsg.Format("Wrong byte count");
			StatusInfo();
		}
		m_sStatusMsg.Format("Buffer too small");
		StatusInfo();
	}
	m_sStatusMsg.Format("...Failed");
	StatusInfo();
	m_bWriting = false;
	return false;
}

//receive a packet
bool CChannel::Receive(CPacket *pPkt, bool locked, DWORD timeout)
{
	//if the last packet has not been received yet, then fail
	//or signal an error if too many tries fail

	CSingleLock rcvLock(&m_mutTransfer);
	if(locked) //lock the mutex for thread safety
	{
		if(!rcvLock.Lock(timeout)) return false; //lock timed out, so fail
	}

	//set the status that a read is underway
	m_bReading = true;
	m_sStatusMsg.Format("Receiving...");
	StatusInfo();

	//now make sure the port is good; try to repair a few times if not
	int retry = 0;
	while(!StatusGood() && (retry < RETRIES))
	{
		Repair();
		Sleep(RETRY_DLY);
		retry++;
	}
	if(StatusGood())
	{
		//calculate the size of the data packet desired
		unsigned int cnt = pPkt->GetSize();
		//make sure buffer is large enough
		if( cnt < sizeof(rxBuf) )
		{
			TRY
			{
				cnt = Read(rxBuf, cnt);
			}
			CATCH( CSerialException, pEx )
			{
				pEx->ReportError();
				m_sStatusMsg.Format("Exception During Read");
				StatusInfo();
				m_bWriting = false;
				return false;
			}
			END_CATCH
			//if we were able to read the whole thing
			if(cnt == pPkt->GetSize())
			{
				//convert the buffer data to a packet
				if(!pPkt->StreamToPacket(rxBuf, &cnt))
				{
					m_sStatusMsg.Format("Fill packet failed");
					StatusInfo();
					m_bReading = false;
					return false;
				}
				//save the index from the packet
				m_dwRxPktIdx = pPkt->GetIndex();
				m_sStatusMsg.Format("...Successful");
				m_bReading = false;
				StatusInfo();
				return true;
			}
			m_sStatusMsg.Format("Wrong byte count");
			StatusInfo();
		}
		m_sStatusMsg.Format("Buffer too small");
		StatusInfo();
	}
	m_sStatusMsg.Format("...Failed");
	StatusInfo();
	m_bReading = false;
	return false;
}

bool CChannel::SendReceive(CPacket *sendPkt, CPacket *rcvPkt, DWORD timeout)
{
	//send packet and receive response - always thread safe
	//assumes that if send fails, receive should not be attempted

	CSingleLock srLock(&m_mutTransfer);
	if(!srLock.Lock(timeout)) return false; //lock timed out so fail

	if(!Send(sendPkt, false))
		return false;
	return Receive(rcvPkt, false);
}

bool CChannel::StartLog(CStdioFile* pLog)
{
	//start logging file
	//if file pointer is valid
	if(pLog->m_hFile != INVALID_HANDLE_VALUE)
	{
		m_pLogFile = pLog;
		m_bLogging = true;
	}
	else
		m_bLogging = false;

	return m_bLogging;
}

bool CChannel::StatusInfo()
{
	//route the current status message
	if(m_bLogging)
	{
		if(m_pLogFile->m_hFile != INVALID_HANDLE_VALUE)
		{
			m_pLogFile->WriteString(STATUS_PREFIX);
			m_pLogFile->WriteString(m_sStatusMsg);
			m_pLogFile->WriteString(STATUS_SUFFIX);
		}
		else
			return false;
	}

#if TRACE_STATUS
	TRACE(STATUS_PREFIX);
	TRACE(m_sStatusMsg);
	TRACE(STATUS_SUFFIX);
#endif

	return true;
}