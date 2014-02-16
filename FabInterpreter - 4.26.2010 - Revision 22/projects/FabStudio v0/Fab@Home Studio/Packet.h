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

class CPacket :
	public CObject
{
	DECLARE_DYNAMIC(CPacket)
	friend class CChannel;	//grant the channel class special access to the packet
public:
	CPacket(void);
	~CPacket(void);
	DWORD GetIndex(void){return m_ulPktIndex.DW;};
	PACKET_TYPE_ENUM GetType(){return m_enumPktCode;};
	void ResetIndex(){g_ulPacketIndex = 0;};	//reset the static index counter

//////VIRTUAL FUNCTIONS//////
	//derived classes must define their own versions of these
	//they are virtual so that derived types can be casted as CPacket,
	//but still have their proper functions called

	//the transmitted packet format should always be as follows:
	//m_bytePktSize 1byte
	//m_enumPktCode 1byte
	//m_ulPktIndex	4bytes
	//data payload	<= MAX_PACKET - 6 bytes
public:
	virtual bool InitPacket();
	virtual	BYTE GetSize(void){return m_bytePktSize;}; //return the size of the packet in bytes
protected:
	//these two functions should be exact mirror images of each other
	//PacketToStream is also used to calculate the size of the packet
	virtual bool PacketToStream(BYTE *buf, unsigned int *size); //convert the packet into a stream of bytes
	virtual bool StreamToPacket(BYTE *buf, unsigned int *size);	//convert a stream of bytes into the packet
	
	
protected:
	BYTE m_bytePktSize;					//size of the packet in bytes, including this field; currently max 255 
	BYTE_DWORD m_ulPktIndex;			//unique identifier for each packet created
	PACKET_TYPE_ENUM m_enumPktCode;		//enum describing the particular type of packet
public:
	static unsigned long g_ulPacketIndex;	//global tracking of the packets
};
