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
#include "FabAtHomeTypes.h"
#include "FabAtHomeParameters.h"
#include "Channel.h"
#include "AbortPacket.h"
#include "ConfigurePacket.h"
#include "ConfigAckPacket.h"
#include "HomePacket.h"
#include "PausePacket.h"
#include "ResetPacket.h"
#include "ResumePacket.h"
#include "SegmentPacket.h"
#include "StartPacket.h"
#include "StatusPacket.h"
#include "StatusAckPacket.h"
#include "Segment.h"
#include "IMMSegmentPacket.h"
#include "Tool.h"
#include "Material.h"
#include "conn.h"

class CFabAtHomeApp;


class CPrinter :
	public CObject
{
public:
	CPrinter(void);
	~CPrinter(void);
	CModel& GetModel(){return *pModel;};
	double GetMaxSubsegLen(){return m_dSubsegLen;};
	void UpdateStatusBarMessage(); //look at system state and post a message accordingly
	bool StatusOK(); //return false if any abnormal condition (e.g. EStop, abort, etc)
	CString& GetStatusBarMessage(){return m_sStatusBarMessage;}; //return the most recently posted status message
	bool EmulateFaster();
	bool EmulateSlower();
	bool IsLogging(){return m_bLogging;};
	void StopLog(); //stop logging and close the log file
	void StartLog(CString& startFile); //open a new log file and start logging
	void Log(CString& callingFn, CString& comment); //if not logging, the user open new file, write strings to log
	bool TrackError(CVec &pos); //track position error by comparing pos to calibrated safe position
	bool Pushout(bool bQueue = false);	//command a pushout for the current tool
	bool Suckback(bool bQueue = false);//command a suckback for the current tool
	bool IsEStop(){return m_bEStop;};
	void SetEStop();
	void ClearEStop();
	void ManageAmpDisable(); //handle auto-disable of amps during idle time
	bool IsEnableRequested(){return m_bEnableRequested;};
	double GetCommsErrorRate(){return m_dCommsErrorRate;};
	CTimeSpan &GetManTCTime(); //return the estimated time for a manual tool change
	CTimeSpan &GetAutoTCTime(); //return the estimated time for an automatic tool change
	CModel *GetModelPtr(){return pModel;}; //return pointer to the current model
	bool SetJogSpeed(double spd); //set the jog speed for all non-build motions
	double GetJogSpeed(){return m_dJogSpeed;};
	double GetActualPathSpeed(); //return the current path tangential speed
	double GetMaxPathSpeed(); //return the maximum path tangential speed
	void WaitForStatusUpdate(int nPeriods = 2); //wait for status update thread to update the status info
	bool IsFabricating(); //return true is a fabrication process is underway
	bool IsMoving();
	bool IsToolMoving();
	void SetModelPtr(CModel *pM){pModel = pM;};
	bool GetPauseAtToolChange(){return m_bPauseAtToolChange;};
	bool SetPauseAtToolChange(bool bPause=true);
	bool GetAutoresume(){return m_bAutoresume;};
	bool SetAutoresume(bool bResume=true);
	bool MountTool(CString &toolName); //set the named tool as mounted, true if successful
	bool UnmountTool(CString &toolName); //set the named tool as unmounted, true if successful
	bool FindToolToRemove(CString& newToolName, CString &removeTool); //return name of tool which should be removed to minimize swaps
	bool EnsureMountedTool(CString &toolname);
	bool EnsureLoadedTool(CString &toolname);
	bool EnsureCurrentTool(bool bForceDlg); //make sure that a current tool is defined, loaded and mounted
	int GetMaxTools(){return m_nMaxTools;}; //return the number of tools that can be mounted simultaneously
	void SetMaxTools(int n){m_nMaxTools = n;};
	DWORD GetPCElapsedTime(); //return the time in ms of the PC clock since the printer was reset
	DWORD GetCommandedStatusPeriod(); //read the currently commanded status update period
	DWORD GetTargetStatusPeriod(); //read the current target status update period
	void SetTargetStatusPeriod(DWORD msPeriod); //set the target status update period
	bool CalcStatusPeriod(DWORD deltaT); //update a rolling average for the update period being achieved
	DWORD GetActualStatusPeriod(); //return the rolling average value of the status update period
	void SetUserStatusPeriod(DWORD msPeriod); //set the user's requested update period
	DWORD GetUserStatusPeriod(){return m_dwUSRStatusPeriod;};
	bool Shutdown(void); //clean shutdown for termination of program
	bool PauseBuild(bool bFindSafe = true, bool bNoSuckback = false); //pause the fabrication process
	bool ResumeBuild(bool bToolChange = false, bool bNoPushout = false); //resume the fabrication process
	bool AbortBuild(); //emergency stop and abort the build underway; 
	bool CleanupBuild(); //clean up variables after successful build
	CString GetStatusText(void);
	bool LoadConfiguration(CString configfilename, CString& errorstr);
	void Clear(void);
	bool Draw(bool bCommanded = false); //typically show the "real-time" position
	void JogCarriageBy(CVec delta, bool bQueue = true); //if bQueue, then commands will be queued, else immediate
	void JogCarriageTo(CVec abspos, bool bQueue = true, CVec *pCurPos = NULL);//if bQueue, then commands will be queued, else immediate
	void SetCurrentCmdPos(CVec abspos); //set the current commanded position for the carriage
	CVec GetCurrentPos(bool bCommanded = true); //don't generate commands based on "real-time" position - it has high latency
	double GetCarriageSpeed(); //return the current path-tangential speed of the carriage
	CVec GetLastPos(void); //return the position stored as the "last" position, e.g. before a pause
	bool SetLastPos(CVec lpos = CVec(0,0,0)); //set the specified position as the prior or last position; for resuming from a pause
	bool InPosition(double tol = 0.5); //true if the real time position of the printer is within tol (each axis) of the commanded pos
	void JogToolBy(double delta, bool bQueue = true);//if bQueue, then commands will be queued, else immediate
	void JogToolTo(double abspos, bool bQueue = true);//if bQueue, then commands will be queued, else immediate
	double GetCurrentToolPos(bool bCommanded = true); //typically return the commanded rather than "real-time" position
	void SetCurrentToolCmdPos(double abspos); //set the current commanded position for the specified tool
	void Move(bool bQueue = true, DWORD *pIndex = NULL);//if bQueue, then commands will be queued, else immediate
	void Move(double slowfactor, bool bQueue = true, DWORD *pIndex = NULL);//if bQueue, then commands will be queued, else immediate
	bool DoHardwareStep(CSegment &curSeg, bool bQueue = true, DWORD *pIndex = NULL); //if bQueue, then commands will be queued, else immediate
	bool DoEmulatorStep(CSegment &curSeg, bool bQueue = true, DWORD* pIndex = NULL); //emulate hardware motion for a segment
	bool FindOrigin(); //automatically find origin (X,Y) position
	CVec GetOrigin();
	void SetOrigin(CVec org = CVec(0,0,0));
	CVec GetHomePos();
	bool FindHomePos(); //attempt to automatically find the home position
	bool SetHomePos(CVec& pos = CVec(0,0,0)); //set the current position as home position; zero out real-time, commanded, and firmware positions
	void GotoHome(); //go to the home position
	void ExecuteHomeSequence();
	CVec GetSafePos(void); //return the safe position set by the user
	bool FindSafePos(CVec firstSafe = CVec(0,0,0)); //attempt to automatically find the safe position; set position regs to firstSafe, unless zero
	bool SetSafePos(CVec sfpos = CVec(0,0,0)); //set the specified position as the safe position
	void GotoSafe(); //go to the safe position
	void ExecuteSafeSequence();
	void GotoLast(); //go to the last position
	void ExecuteLastSequence();
	void GotoOrigin(); //go to the build origin position
	void ExecuteOriginSequence();
	void GotoNext(); //go to the start of the next path
	bool IsDefined(void);
	bool InitializeHardware(void); //initialize the data members of the class
	bool ShutdownHardware(void);
	bool ChangePOD(); //change the current point of deposition - e.g. when active tool has changed
	bool SetCurTool(CString newToolName, CString newMatName = CString("")); //change the currently active tool
	CTool* GetCurTool(); //return pointer to the current tool
	CMaterial* GetCurMaterial(); //return pointer to the current material
	CString GetCurToolName(){return m_sCurTool;};
	CString GetCurMatName(){return m_sCurMaterial;};
	bool ChangeTool(CString newToolName, CString newMatName = CString(""));
	double GetClearance( bool inChunk = true );	//get the vertical clearance required to avoid collision of tool with part
	CVec ModelToRobotTfm(CVec& v); //transform to robot coords from model coords
	CVec RobotToModelTfm(CVec& v); //transform from robot to model coords

	bool WaitForMoveDone(long index=-1, int timeout_ms=1000); //wait for completion of command with index "index" or for MOVING flag to be cleared
	bool WaitForMoveStart(long index=-1, int timeout_ms=1000); //wait for commencement of command with index "index" or for MOVING flag to be set

	//the various commands that can be sent to the Fab@Home
	bool Reset(void);	//(re)initialize the machine
	bool Start(void);	//enable motion
	bool Stop(void);	//stop motion (terminate current command)
	bool PauseQueue(void);	//pause at current position in segment queue
	bool ResumeQueue(void);	//resume at next position in seqment queue
	bool Abort(void);	//stop all motion, flush the segment queue
	bool ClearError(void);	//try to clear error condition
	bool HomeHW();	//tell HW to declare current position as home, reset global positions 
	bool SetOrigin(void);	//set the current location as the build origin
	bool QueuePath(void);	//runs in worker thread to transfer segments from gPathArray to the uC queue
	bool IMMSegment( CSegment &curSeg, DWORD* index = NULL ); //send a single segment for immediate execution
	bool GetStatus(void);	//fetch the status information from the machine; will probably happen with worker thread
	bool CollectStatus(bool start); //start/stop the worker thread to collect status information
	bool Configure(void);	//forward compatibility
	bool OpenChannel( bool isManual = false ); //open a com channel, use a dialog if isManual
	bool CloseChannel();	//close the com channel
	//Some inline definitions
	CString GetChannelName(){m_channel.GetPortName();}; //get string name of the com port
	int GetChannelNum(){m_channel.GetPortNum();}; //get int number of the com port
	CString GetChannelString(){return m_channel.GetPortString();}; //get friendly description of the port
	bool IsChannelOpen(){return m_channel.IsOpen();};	//is there a port open to the Fab@Home?
	HANDLE GetPortHandle(){return m_channel.GetPortHandle();};
	//various accessors for the status information members
	BYTE GetFirmwareVersion(){return m_BFirmwareVersion;};
	long GetGlobalPos( BYTE axis );
	bool LimitsActive();	//true if any limits are active
	bool GetLimit( BYTE axis, bool fwd ); //fwd or bkwd limit
	bool CanAutoRange(); //return true if limit switches are installed to allow for automatic range of motion discovery
	WORD GetQSpace();
	bool IsStarted();
	bool IsPaused();
	bool IsAborted();
	bool IsHomed();
	bool IsOriginSet();
	bool IsSafeSet();
	bool IsPathDone(); //true when path array is empty
	bool IsError();
	bool IsQEmpty();
	bool IsConfigured();  //microcontroller only returns true if configuration was successful
	bool IsCollecting();  //return true if the worker thread is collecting status
	DWORD GetLastCmdSent();	//get the index of the last command sent over the channel
	DWORD GetLastCmdQueued();	//get the index of the last command queued to the microcontroller
	DWORD GetLastCmdRecd(); //get the index of the last command recd over the channel
	DWORD GetLastCmdExecuted(); //get the index of the last command executed by the Fab@Home
	DWORD GetCurCommand(); //get the index of the command currently being executed by the Fab@Home
	DWORD GetMachineElapsedTime_ms();
	void StartNetworkThread();
	bool IsNetworked(){return m_bNetworkConnect;};
	void NetworkConnect(bool bConnect);
	
protected:
	bool ManageComms(bool bError = false); //manage the comms workload and check for error conditions - set bError if known error condition
	static UINT StatusThreadProc( LPVOID pParam );
	static UINT HomeSeqProc( LPVOID pParam );
	static UINT OriginSeqProc( LPVOID pParam );
	static UINT SafeSeqProc( LPVOID pParam );
	static UINT LastSeqProc( LPVOID pParam );
	static UINT NetworkThreadProc( LPVOID pParam );

	//DATA MEMBERS
public:
	CString name, description;
	bool bEmulateHardware;
	CArray <CPrinterComponent, CPrinterComponent> component;
	bool bHardwareInitialized;
	bool m_bSafeSet; //true if safe position has been set
	bool m_bLastSet; //true is last position has been set
	bool m_bOriginSet; //true if build origin has been set
	bool m_bHomed;		//true if hardware and software (or software is emulating) is homed
	bool bNewStatus;	//flag to notify the views that status info has changed
	CVec m_vLastPos; //store the last position e.g. prior to a pause
	CVec m_vSafePos; //store a safe position for e.g. changing tools, pausing
	CVec m_vOrigin;	//store the build origin
	CVec m_vPOD; //the current point of deposition in tool graphics coordinates
	CVec m_vPODOffset;	//the point of deposition offset - to calculate offsets for different tools
	double m_ardMaxTravel[POSITION_AXES]; //store the maximum travel of the X,Y,Z axes
	bool m_bIsSafe;	//store whether the printer is in the safe position or not
	//for the segment queueing worker thread
	CArray <CSegment,CSegment> m_pathArray;  //global path storage for transfer to uC queue
	int m_nPathIndex;	//index into the path array; used by worker thread for queue transfer
	bool m_bQPathDone; //thread will set to true when entire path is sent; user set to false to start transfer
	double m_dModelClearance; //the max height attained during fabrication so far - for collision avoidance
	BOOL m_arBLimitSwitch[2*AXES_CNT]; //define which limit switches are connected/used (from printer/tool files)

protected:
	//parameters for Festo kinematics

	double m_dSubsegLen; //maximum path segment length required by kinematics
	double m_dJogSpeed; //speed for all jog motions
	int m_nMaxTools;	//maximum number of simultaneously mounted tools
	CFile m_fLog; //file pointer for logging
	bool m_bLogging; //logging state
	CFabAtHomeApp *pApp; //pointer to the application for easy access to tools and materials
	CModel *pModel; //pointer to the current model
	CChannel m_channel;	//channel handles formatting, RX, and TX of commands and data
	WORD m_nQSpace;	//memory space remaining in the path queue on the machine
	long m_arlGlobalPos[AXES_CNT];	//distance of axes from home position in steps
	//long m_arlEncoder[AXES_CNT];;	//raw encoder readings
	bool m_arbLimitState[2*AXES_CNT];	//states of limit switches; 2 for each axis, true => limit active
	GEN_STATE_FLAGS m_stateFlags;	//general state of the system
	BYTE m_BFirmwareVersion;	//hold the firmware version
	bool m_bCollectStatus; //to track whether status is being collected by worker or not
	CWinThread *m_pStatusThread; //to hold the status thread pointer
	DWORD m_dwTGTStatusPeriod; //the commanded period at which the status is collected from the machine
	DWORD m_dwCMDStatusPeriod; //the commanded status update period - differs from the target because of system timing
	DWORD m_dwACTStatusPeriod; //the achieved status update period
	DWORD m_dwUSRStatusPeriod; //the users requested status period
	int m_nPeriodCalcStep; //counter for steps in status period average calculation
	CArray<double,double> m_arStatusPeriod; //buffer for averaging status update rate
	int m_nStatusSamples;	//number of samples to average for status update rate calc
	DWORD m_dwCurCommand;   //the index of the command currently being executed
	DWORD m_dwLastCmdExecuted;  //the index of the last command completed
	DWORD m_dwQueuePathIndex;	//the index of the last path segment queued to the uC
	CStatusPacket m_statusPkt; //to reduce memory alloc/dealloc by the status fetching thread
	CStatusAckPacket m_statusAckPkt; //to reduce memory alloc/dealloc by the status fetching thread 
	DWORD m_dwMachineElapsedTime_ms; //elapsed time on the machine since reset/reboot 
	DWORD m_dwMachineStartTime_ms;	//tick count from microcontroller in first status packet since reset
	bool m_bFirstPacket;	//to identify the first status packet received
	CString m_sCurTool;	//store the name of the current tool- use name not index to allow online param changes 
	CString m_sCurMaterial; //store the name of the current material- use name not index to allow online param changes 
	bool m_bPauseAtToolChange; //if a multi-tool config, allows user to decide whether to pause build at tool changes
	bool m_bAutoresume;	//if the system is auto-pausing because of tool pausepaths setting, automatically resume if true
	bool m_bDefined; //true if all component defs are loaded, and ready for drawing
	CTimeSpan m_ctsTCAuto; //estimated time for an auto tool change
	CTimeSpan m_ctsTCMan; //estimated time for a manual tool change
	CTimeSpan m_ctsIdleTime; //time spend idle with amps on - for auto amp disables
	CTime m_ctIdleStart; //start time for idle period
	double m_dCommsErrorRate; //rate of errors in communications
	bool m_bEnableRequested; //true if amplifier enable has been requested
	bool m_bEStop;			//true if emergency stop has been hit
	int m_nPushoutState;	//track the number of pushouts/suckbacks - net pushout if >0, net suckback if <0
	CMutex m_mutUpdate;	//mutex to ensure that state updates are atomic
	double m_dEmulatorSlowdown; //speedup factor for emulator
	CString m_sStatusBarMessage; //show brief message
	CConn *p_connListener; //sockets connection listener remote procedure interface
	CConn *p_connEctor; //sockets connection for remote procedure interface
	bool m_bNetworkConnect;
	bool m_bToolMoving;
};
