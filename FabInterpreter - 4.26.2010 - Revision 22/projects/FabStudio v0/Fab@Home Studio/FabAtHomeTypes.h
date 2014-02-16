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

//FabAtHomeTypes.h  
//defining some data types used by the system

enum AXIS_ENUM
{
  AXIS_X,
  AXIS_Y,
  AXIS_Z,
  AXIS_U,
  AXIS_V,
  AXIS_W
};

enum PACKET_TYPE_ENUM
{
  UNDEFINED   = -1, //undefined, should not receive
  ABORT	      = 0,  //abort current motion, flush the queue, toggles
  ABORT_ACK   = 1,
  PAUSE	      = 2,  //pause motion; hold the queue index
  PAUSE_ACK   = 3,
  RESUME      = 4,  //resume motion at next queue index
  RESUME_ACK  = 5,
  START	      = 6,  //start motion at front of queue
  START_ACK   = 7,
  CONFIG      = 8,  //send configuration information to the device, i.e. clear error
  CONFIG_ACK  = 9,
  STATUS      = 10, //request that status information be returned
  STATUS_ACK  = 11, //the status information that was returned
  SEGMENT     = 12, //enqueue a path segment
  SEGMENT_ACK = 13,
  HOME	      = 14, //home the axes
  HOME_ACK    = 15, //signal when homed
  RESET	      = 16, //initialize the motion control system
  RESET_ACK   = 17, //signal when complete
  IMMSEG	  = 18,	//send segment for immediate (non-queued) execution
  IMMSEG_ACK  = 19	//acknowledgment of immediate segment
};

//allow byte access to DWORD type
struct DW_B
{
	BYTE B00;
	BYTE B01;
	BYTE B10;
	BYTE B11;
};

union BYTE_DWORD
{
	DW_B B;
	DWORD DW;
};

//allow byte access to long type
struct L_B
{
	BYTE B00;
	BYTE B01;
	BYTE B10;
	BYTE B11;
};

union BYTE_LONG
{
	L_B B;
	long L;
};

//allow byte access to WORD type
struct W_B
{
	BYTE B0;
	BYTE B1;
};

union BYTE_WORD
{
	W_B B;
	WORD W;
};

//allow byte access to short type
struct S_B
{
	BYTE B0;
	BYTE B1;
};

union BYTE_SHORT
{
	S_B B;
	short S;
};

struct LIMIT_FLAGS
{
	WORD X_FWD : 1;
	WORD X_BKD : 1;
	WORD Y_FWD : 1;
	WORD Y_BKD : 1;
	WORD Z_FWD : 1;
	WORD Z_BKD : 1;
	WORD U_FWD : 1;
	WORD U_BKD : 1;
	WORD V_FWD : 1;
	WORD V_BKD : 1;
	WORD W_FWD : 1;
	WORD W_BKD : 1;
};

union LIMIT_STATE
{
	LIMIT_FLAGS flags;
	BYTE_WORD data;
};

struct GEN_STATE_FLAGS
{
	WORD ABORTED : 1;
	WORD PAUSED : 1;
	WORD STARTED : 1;
	WORD CONFIGURED : 1;
	WORD HOMED : 1;
	WORD MOVING : 1;
	WORD MALF : 1;
	WORD QEMPTY : 1;
};

union GENERAL_STATE
{
	GEN_STATE_FLAGS flags;
	BYTE_WORD data;
};