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
#include ".\packet.h"

IMPLEMENT_DYNAMIC(CPacket,CObject)

//static member of CPacket class
unsigned long CPacket::g_ulPacketIndex = 0;	//global tracking of the packets

/***********************************************/
//default constructor
/***********************************************/
CPacket::CPacket(void)
{
}

/***********************************************/
//initialize the packet, fill in fields
//each type should write its own init fn
/***********************************************/
bool CPacket::InitPacket()
{
	//fill in the packet type
	m_enumPktCode = UNDEFINED;
	//fill in the index
	m_ulPktIndex.DW = g_ulPacketIndex;
	//increment the global counter
	g_ulPacketIndex++;
	//calculate the packet size
	BYTE tmp[MAX_PACKET];
	m_bytePktSize = MAX_PACKET;
	return( PacketToStream(tmp,(unsigned int*)(&m_bytePktSize)) );
}

/***********************************************/
//destructor
/***********************************************/
CPacket::~CPacket(void)
{
}


/***********************************************/
//convert the packet into a stream of bytes
//return a stream of bytes in *buf, and true if successful
//*size should pass in the maximum space available in the buffer
//and will be used to return the number of bytes written
/***********************************************/
bool CPacket::PacketToStream(BYTE *buf, unsigned int *size)
{
	if( (buf == NULL) || (size == NULL) || (*size < m_bytePktSize) )
		return false;

	unsigned int cnt = 0;
	//force the packet size, code and index into a byte array buffer
	buf[cnt++] = m_bytePktSize;
	buf[cnt++] = (BYTE)m_enumPktCode;
	buf[cnt++] = m_ulPktIndex.B.B00;
	buf[cnt++] = m_ulPktIndex.B.B01;
	buf[cnt++] = m_ulPktIndex.B.B10;
	buf[cnt++] = m_ulPktIndex.B.B11;
	if(cnt>*size) return false;
	*size = cnt;
	return true;
}

/***********************************************/
//convert a stream of bytes into a packet
//read data from *buf as a stream of bytes
//*size should pass in the length of the data in the buffer in bytes
//and will be used to return the number of bytes read
//return true if successful
/***********************************************/
bool CPacket::StreamToPacket(BYTE *buf, unsigned int *size)
{
	if( (buf == NULL) || (size == NULL) || (*size < m_bytePktSize) )
		return false;

	unsigned int cnt = 0;
	//read the packet size, code and index into a packet
	m_bytePktSize = buf[cnt++];
	m_enumPktCode = (PACKET_TYPE_ENUM)buf[cnt++];
	m_ulPktIndex.B.B00 = buf[cnt++];
	m_ulPktIndex.B.B01 = buf[cnt++];
	m_ulPktIndex.B.B10 = buf[cnt++];
	m_ulPktIndex.B.B11 = buf[cnt++];
	if(cnt>*size) return false;
	*size = cnt;
	return true;
}