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

struct STATUS_RX_DATA
{
	BYTE_WORD qSpace;			//memory space remaining in the path queue on the machine, 2bytes
	BYTE_LONG gPos[AXES_CNT];	//distance of axes from home position in steps, 24 bytes if 6 axes
	//BYTE_LONG gEnc[AXES_CNT];	//raw encoder values
	LIMIT_STATE limits;			//bitfield/word union for limit switch flags, 2 bytes
	GENERAL_STATE state;		//bitfield/word union for general operational status, 2 bytes
	BYTE_DWORD curIndex;		//the index of command currently running, 4 bytes
	BYTE_DWORD execIndex;		//the index of the last command executed, 4 bytes
	BYTE_DWORD msElapsed;		//the elapsed time in ms since the Fab@Home reset/powered on, 4 bytes; 49.7 days before overflow
};

class CStatusAckPacket :
	public CPacket
{
	DECLARE_DYNAMIC(CStatusAckPacket);
public:
	CStatusAckPacket(void);
	~CStatusAckPacket(void);
	WORD GetQSpace(){return m_status.qSpace.W;};
	long GetGlobalPos(BYTE axis){return m_status.gPos[axis].L;};
	//long GetEncoderRdg(BYTE axis){return m_status.gEnc[axis].L;};
	WORD GetLimitSwitches(){return m_status.limits.data.W;};
	GEN_STATE_FLAGS GetState(){return m_status.state.flags;};
	DWORD GetIdxCurCommand(){return m_status.curIndex.DW;};
	DWORD GetIdxLastExecuted(){return m_status.execIndex.DW;};
	DWORD GetElapsedTime_ms(){return m_status.msElapsed.DW;};
public:
	bool InitPacket(int fwVer = 3);
protected:
	bool PacketToStream(BYTE *buf, unsigned int *size); //convert the packet into a stream of bytes
	bool StreamToPacket(BYTE *buf, unsigned int *size);	//convert a stream of bytes into a packet

protected:
	STATUS_RX_DATA m_status;
	int m_nFWVersion;	//firmware version number
};