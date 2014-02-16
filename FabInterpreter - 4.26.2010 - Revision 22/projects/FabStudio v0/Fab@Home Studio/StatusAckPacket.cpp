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

#include "StdAfx.h"
#include ".\statusackpacket.h"

IMPLEMENT_DYNAMIC(CStatusAckPacket,CPacket)

CStatusAckPacket::CStatusAckPacket(void)
{
}

CStatusAckPacket::~CStatusAckPacket(void)
{
}

bool CStatusAckPacket::InitPacket(int fwVers)
{
	//call the base class Init fn FIRST!!!!
	//it fills in the packet code, so we need to overwrite it afterward
	if(!CPacket::InitPacket()) return false;

	//check the firmware version
	m_nFWVersion = fwVers;

	//always remember to overwrite the packet type
	m_enumPktCode = STATUS_ACK;

	//initialize the data payload; will be overwritten when packet is rec'd
	m_status.qSpace.W = 0;	//memory space remaining in the path queue on the machine, 2bytes
	for(int i = 0; i < AXES_CNT; i++ )
	{
		m_status.gPos[i].L = 0;	//distance of axes from home position in steps, 24 bytes if 6 axes
		//m_status.gEnc[i].L = 0; //encoder counts
	}
	m_status.limits.data.W = 0;	//no limits active; bitfield/word union for limit switch flags, 2 bytes
	m_status.state.data.W = 0;	//bitfield/word union for general operational status, 2 bytes
	m_status.curIndex.DW = 0;	//the index of the command currently executing, 4 bytes
	m_status.execIndex.DW = 0;	//the index of the last command executed, 4 bytes
	m_status.msElapsed.DW = 0;	//the elapsed time in ms since the Fab@Home reset/powered on, 4 bytes; 49.7 days before overflow

	return true;
}

bool CStatusAckPacket::PacketToStream(BYTE *buf, unsigned int *size)
{
	//convert the packet into a stream of bytes
	//return a stream of bytes in *buf, and true if successful
	//*size should pass in the maximum space available in the buffer
	//and will be used to return the number of bytes written
	if( (buf == NULL) || (size == NULL) || (*size < m_bytePktSize) )
		return false;

	//call the base class PacketToStream to handle the common members
	unsigned int cnt = *size;
	if(!CPacket::PacketToStream(buf,&cnt))
		return false;

	int i;

	//now fill in the derived class members

	//queue space
	buf[cnt++] = m_status.qSpace.B.B0;
	buf[cnt++] = m_status.qSpace.B.B1;

	//global axis positions
	for(i=0; i<AXES_CNT; i++)
	{
		buf[cnt++] = m_status.gPos[i].B.B00;
		buf[cnt++] = m_status.gPos[i].B.B01;
		buf[cnt++] = m_status.gPos[i].B.B10;
		buf[cnt++] = m_status.gPos[i].B.B11;
	}
	//limit switches
	buf[cnt++] = m_status.limits.data.B.B0;
	buf[cnt++] = m_status.limits.data.B.B1;
	//general state
	buf[cnt++] = m_status.state.data.B.B0;
	buf[cnt++] = m_status.state.data.B.B1;
	//index of the command currently executing
	buf[cnt++] = m_status.curIndex.B.B00;
	buf[cnt++] = m_status.curIndex.B.B01;
	buf[cnt++] = m_status.curIndex.B.B10;
	buf[cnt++] = m_status.curIndex.B.B11;
	//index of last executed command
	buf[cnt++] = m_status.execIndex.B.B00;
	buf[cnt++] = m_status.execIndex.B.B01;
	buf[cnt++] = m_status.execIndex.B.B10;
	buf[cnt++] = m_status.execIndex.B.B11;
	//elapsed time since hw reset/power on
	buf[cnt++] = m_status.msElapsed.B.B00;
	buf[cnt++] = m_status.msElapsed.B.B01;
	buf[cnt++] = m_status.msElapsed.B.B10;
	buf[cnt++] = m_status.msElapsed.B.B11;
	/*if(m_nFWVersion > 3)
	{
		//encoder reading for each axis
		for(i=0; i<AXES_CNT; i++)
		{
			buf[cnt++] = m_status.gEnc[i].B.B00;
			buf[cnt++] = m_status.gEnc[i].B.B01;
			buf[cnt++] = m_status.gEnc[i].B.B10;
			buf[cnt++] = m_status.gEnc[i].B.B11;
		}
	}*/

	if(cnt > *size) return false;
	*size = cnt;

	return true;
}

bool CStatusAckPacket::StreamToPacket(BYTE *buf, unsigned int *size)
{
	//convert a stream of bytes into a packet
	//read data from *buf as a stream of bytes
	//*size should pass in the length of the data in the buffer in bytes
	//and will be used to return the number of bytes read
	//return true if successful
	if( (buf == NULL) || (size == NULL) || (*size < m_bytePktSize) )
		return false;

	unsigned int cnt = *size;
	//call the base class fn to handle the common members
	if(!CPacket::StreamToPacket(buf,&cnt))
		return false;

	//now read in the data payload

	//queue space
	m_status.qSpace.B.B0 = buf[cnt++];
	m_status.qSpace.B.B1 = buf[cnt++];

	//global axis positions
	for(int i=0; i<AXES_CNT; i++)
	{
		m_status.gPos[i].B.B00 = buf[cnt++];
		m_status.gPos[i].B.B01 = buf[cnt++];
		m_status.gPos[i].B.B10 = buf[cnt++];
		m_status.gPos[i].B.B11 = buf[cnt++];
	}
	//limit switches
	m_status.limits.data.B.B0 = buf[cnt++];
	m_status.limits.data.B.B1 = buf[cnt++];
	//general state
	m_status.state.data.B.B0 = buf[cnt++];
	m_status.state.data.B.B1 = buf[cnt++];
	//index of the command currently executing
	m_status.curIndex.B.B00 = buf[cnt++];
	m_status.curIndex.B.B01 = buf[cnt++];
	m_status.curIndex.B.B10 = buf[cnt++];
	m_status.curIndex.B.B11 = buf[cnt++];
	//index of last executed command
	m_status.execIndex.B.B00 = buf[cnt++];
	m_status.execIndex.B.B01 = buf[cnt++];
	m_status.execIndex.B.B10 = buf[cnt++];
	m_status.execIndex.B.B11 = buf[cnt++];
	//elapsed time since hw reset/power on
	m_status.msElapsed.B.B00 = buf[cnt++];
	m_status.msElapsed.B.B01 = buf[cnt++];
	m_status.msElapsed.B.B10 = buf[cnt++];
	m_status.msElapsed.B.B11 = buf[cnt++];
	/*if(m_nFWVersion > 3)
	{
		//encoder counts for each axis
		for(int i=0; i<AXES_CNT; i++)
		{
			m_status.gEnc[i].B.B00 = buf[cnt++];
			m_status.gEnc[i].B.B01 = buf[cnt++];
			m_status.gEnc[i].B.B10 = buf[cnt++];
			m_status.gEnc[i].B.B11 = buf[cnt++];
		}
	}*/

	if(cnt > *size)
		return false;
	*size = cnt;
	return true;
}