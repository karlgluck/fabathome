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

struct CONFIG_RX_DATA
{
	BYTE fwVersion;			//version number for the firmware, 1 byte
	LIMIT_STATE m_hwLimits;	//flags should be set for axis directions where HW limits are connected, 2 bytes
};

class CConfigAckPacket :
	public CPacket
{
	DECLARE_DYNAMIC(CConfigAckPacket);
public:
	CConfigAckPacket(void);
	~CConfigAckPacket(void);

public:
	bool InitPacket(void);
	BYTE GetFWVersion(void){return m_config.fwVersion;};
	LIMIT_STATE GetLimitConfig(void){return m_config.m_hwLimits;};
protected:
	bool PacketToStream(BYTE *buf, unsigned int *size); //convert the packet into a stream of bytes
	bool StreamToPacket(BYTE *buf, unsigned int *size);	//convert a stream of bytes into a packet

protected:
	CONFIG_RX_DATA m_config;
};