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
#include "afx.h"
#include "afxmt.h" //for multithreading support
#include "Packet.h"
#include "SerialPort.h"
#include "ComPortDialog.h"

//the port configuration specs
#define BITSPERSEC 921600
#define PARITY NoParity
#define DATABITS 8
#define STOPBITS OneStopBit
#define FLOWCTRL NoFlowControl
#define OVERLAP FALSE

//the port timeout specs
#define RX_INTVL 20		//milliseconds allowed per byte
#define RX_MULT	20		//transfer timeout is RX_MULT * # bytes ms
#define RX_CONST 500	//total timeout is transfer timeout + RX_CONST ms
#define TX_MULT	20		//transfer timeout is RX_MULT * # bytes ms 
#define TX_CONST 500	//total timeout is transfer timeout + TX_CONST ms

//retries for sending/receiving
#define RETRIES 10	//retry asynch process RETRIES times
#define RETRY_DLY	100	//RETRY_DLY ms sleep when an asynch process is awaited

//if tracing of status info desired
#define TRACE_STATUS FALSE

class CChannel :
	public CSerialPort
{
	DECLARE_DYNAMIC(CChannel);
public:
	CChannel(void);
	~CChannel(void);
	bool StartLog(CStdioFile* pLog); //start logging file
	bool StopLog(){m_bLogging = false;}; //stop logging
	bool DoDialog();	//do the com port dialog to let user select com port; return true if new port selected
	bool AutoFindPort();	//try to find the com port automatically; return true if port found successfully
	bool Connect();
	bool Disconnect();
	bool IsOpen(){return CSerialPort::IsOpen()>0;};
	bool StatusGood(); //make sure the connection link is good; update status message
	bool Repair(); //try to repair a bad connection
	CString GetPortName(){return m_sPortName;};
	int GetPortNum(){return m_nPortNum;};
	CString GetPortString();	//return a formatted string with number and name of port
	bool Send(CPacket *pPkt, bool locked = true, DWORD timeout = 1000);	//send a packet, thread safe if locked, give up if timeout exceeded (infinite if -1)
	bool Receive(CPacket *pPkt, bool locked = true, DWORD timeout = 1000);	//receive a packet, thread safe if locked, give up if timeout exceeded (infinite if -1)
	bool SendReceive(CPacket *sendPkt, CPacket *rcvPkt, DWORD timeout = 1000); //send packet and receive response - always thread safe, give up if timeout exceeded (infinite if -1)
	DWORD GetLastSentIdx(){return m_dwTxPktIdx;};	//get the index of the last packet sent
	DWORD GetLastRecdIdx(){return m_dwRxPktIdx;};	//get the index of the last packet received
	bool IsTxing(){return m_bWriting;}; //return true if a transmission is pending
	bool IsRxing(){return m_bReading;};	//return true if awaiting receipt of data
	bool StatusInfo(); //route status information
	HANDLE GetPortHandle(void){return m_hComm;};
protected:
	bool ExtractDlgPortInfo();	//pull the port name and number from the dialog selection
	bool ParseError( DWORD errors ); //fill in the status message with the current errors
protected:
	CComPortDialog m_comPortDlg;
	int m_nPortNum;
	CString m_sPortName;
	bool m_bWriting;	//true while an output transaction is underway
	bool m_bReading;	//true while an input transaction is underway
	DWORD m_dwTxPktIdx;	//the index of the packet being sent
	DWORD m_dwRxPktIdx;	//the index of the packet being received
	BYTE rxBuf[MAX_PACKET];
	BYTE txBuf[MAX_PACKET];
	CPacket *pRxPacket;	//use to keep memory while waiting for asynch rx to finish
	COMSTAT m_comstat;	//status of the com port
	CString m_sStatusMsg;	//holds the current status of the port - for GUI
	bool m_bLogging;	// true if logging requested
	CStdioFile *m_pLogFile;	//pointer to a log file

	CMutex m_mutTransfer;	//a mutex to lock the channel during transfers
};
