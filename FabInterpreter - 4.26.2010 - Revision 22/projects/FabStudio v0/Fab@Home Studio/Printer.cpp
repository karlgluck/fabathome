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

//Printer.cpp
//implementation of the CPrinter class

#include "stdafx.h"
#include ".\printer.h"
#include "FabAtHomeParameters.h"
#include "ToolChangeDlg.h"

//--------------------------------------------------------------------
CPrinter::CPrinter(void)
//--------------------------------------------------------------------
{
	Clear();
	pApp = (CFabAtHomeApp *) AfxGetApp();
	pModel = NULL; //will be filled in once valid
	bEmulateHardware = true;
	bHardwareInitialized = false;
	m_bSafeSet = false;
	m_bLastSet = false;
	m_bOriginSet = false;
	m_bHomed = false;
	m_bIsSafe = false;
	bNewStatus = true;
	m_nQSpace = 0;	//memory space remaining in the path queue on the machine
	for(int i = 0; i<AXES_CNT; i++)
	{
		m_arlGlobalPos[i] = 0;	//distance of axes from home position in steps
		m_arBLimitSwitch[2*i] = FALSE;	//which limit switches are connected
		m_arBLimitSwitch[(2*i)+1] = FALSE; //twice as many limits as axes
		m_arbLimitState[2*i] = false;	//states of limit switches
		m_arbLimitState[(2*i)+1] = false; //twice as many limits as axes
		//m_arlEncoder[i] = 0; //raw encoder readings
	}
	m_BFirmwareVersion = -1;		//the firmware version number
	m_stateFlags.STARTED = false;		//has the machine been told to start processing the queue
	m_stateFlags.PAUSED = false;		//is the machine paused
	m_stateFlags.ABORTED = false;		//is the machine aborted (stopped, and queue flushed)
	m_stateFlags.HOMED = false;		//is the HW machine homed
	m_stateFlags.MALF = false;		//is there an error of some sort
	m_stateFlags.MOVING = false;	//is the hardware currently moving
	m_stateFlags.CONFIGURED = false;	//has the microcontroller been successfully configured?
	m_stateFlags.QEMPTY = TRUE;		//the queue should be empty when starting
	m_bCollectStatus = false; //to track whether status is being collected by worker or not
	m_pStatusThread = NULL; //to hold the status thread pointer
	m_dwMachineElapsedTime_ms = 0; //track the elapsed time on the micrcontroller since micro rebo
	m_dwMachineStartTime_ms = 0;	//tick count from microcontroller in first status packet since reset
	m_bFirstPacket = true;	//to identify the first status packet received
	m_dwUSRStatusPeriod = STATUS_UPDATE_PERIOD; //the user's requested status update period
	m_dwCMDStatusPeriod = STATUS_UPDATE_PERIOD; //the commanded period at which the status is collected from the machine
	m_dwTGTStatusPeriod = STATUS_UPDATE_PERIOD; //the target period at which the status should be collected from the machine
	m_nPeriodCalcStep = 0; //counter for steps in status period average calculation
	m_dwACTStatusPeriod = 0; //the achieved status update period
	m_nStatusSamples = 10;	//number of samples to average for status update rate calc
	for(int i = 0; i < m_nStatusSamples; i++)
		m_arStatusPeriod.Add(0); //buffer for averaging status update rate
	m_dwLastCmdExecuted = 0;
	m_dwQueuePathIndex = 0;
	m_sCurTool = "Undefined";
	m_sCurMaterial = "Undefined";
	//for the worker thread sending segments to the microcontroller queue
	m_pathArray.RemoveAll();
	m_nPathIndex = 0;
	m_bQPathDone = true;
	//initialize the range of motion of the positioning axes
	for(int i = 0; i < POSITION_AXES; i++)
		m_ardMaxTravel[i] = 0.0;
	m_nMaxTools = 1; //set the maximum number of tools that can be mounted simultaneously
	m_bPauseAtToolChange = true; //have system pause building during a tool change, unless multitool and user specifies
	m_vPOD = CVec(99,-318,-60); //init to standard 1-syringe tool in tool graphics coords- used to calculate offsets
	m_vPODOffset = CVec(0,0,0); //offset for adjusting point of deposition after tool change
	m_bDefined = false;
	m_dJogSpeed = 5; //speed of all non-build motions, mm/s
	m_ctsTCAuto = AUTO_TC_TIME; //estimated time in seconds for auto (multi-tool system) tool change
	m_ctsTCMan = MAN_TC_TIME; //estimated time in seconds for manual tool change
	m_ctsIdleTime = 0; //idle time with amps on but no motion - for auto disabling of amps
	m_ctIdleStart = 0; //start of idle period
	m_dCommsErrorRate = 0.0; //error rate for status comms with micro
	m_bEnableRequested = false;
	m_bEStop = false;
	m_nPushoutState = 0; //initially should be no net pressure in syringe
	m_bLogging = false; //initially not logging information
	m_dEmulatorSlowdown = EMULATOR_SLOWDOWN; //init speed for emulator

	m_dSubsegLen = -1.0;

	m_bNetworkConnect = false;
	m_bToolMoving = false;
}

//--------------------------------------------------------------------
CPrinter::~CPrinter(void)
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
bool CPrinter::Shutdown()
//--------------------------------------------------------------------
{
	//if using the hardware, shut it down
	if(bHardwareInitialized) return( ShutdownHardware() );
	//otherwise, make sure all threads are dead
	return( CollectStatus(false) );
}

//--------------------------------------------------------------------
void CPrinter::Log(CString& callingFn, CString& comment)
//--------------------------------------------------------------------
{
	//if not logging, the user open new file, write strings to log
	if(m_bLogging)
	{
		CString msg;
		CVec cmdPos = GetCurrentPos(true);
		CVec rtPos = GetCurrentPos(false);
		CVec podPos = rtPos + m_vPOD;
		double toolCMDPos = GetCurrentToolPos(true);
		double toolRTPos = GetCurrentToolPos(false);

		msg.Format("%d\t%0.2f\t%0.2f\t%0.2f\t%0.2f\t%0.2f\t%0.2f\t%0.2f\t%0.2f\t%0.2f\t%0.2f\t%0.2f\t%0.2f\t%0.2f\t%0.2f\t%s\t%s\n",
			GetPCElapsedTime(), m_vOrigin.x, m_vOrigin.y, m_vOrigin.z, cmdPos.x, cmdPos.y, cmdPos.z, rtPos.x, rtPos.y, rtPos.z, 
			podPos.x, podPos.y, podPos.z, toolCMDPos, toolRTPos, callingFn, comment);
		m_fLog.Write((LPCTSTR)msg, msg.GetLength());
	}
}

//--------------------------------------------------------------------
void CPrinter::StartLog(CString& startFile)
//--------------------------------------------------------------------
{
	//if already logging, do nothing
	//else open a new logfile, and update logging flag
	if(!m_bLogging)
	{
		CFileDialog dlg(TRUE, "log", (LPCTSTR)startFile, NULL,
					"Log File (*.dat)|*.dat|All Files (*.*)|*.*||", 0);
		if(IDOK != dlg.DoModal()) return;
		startFile = dlg.GetPathName();
		CFileException e;
		if( m_fLog.Open((LPCTSTR)startFile, CFile::modeCreate | CFile::modeWrite, &e))
		{
			m_bLogging = true;
			CString msg;
			msg.Format("Time\tOrigin.x\tOrigin.y\tOrigin.z\tCMDPos.x\tCMDPos.y\tCMDPos.z\tRTPos.x\tRTPos.y\tRTPos.z\tPOD.x\tPOD.y\tPOD.z\tToolCMD\tToolRT\tFunction\tComment\n");
			m_fLog.Write((LPCTSTR)msg,msg.GetLength());
		}
		else
			((CFabAtHomeApp *)AfxGetApp())->Log("Cannot create log file");
	}
}

//--------------------------------------------------------------------
void CPrinter::StopLog()
//--------------------------------------------------------------------
{
	//stop logging and close the log file
	m_bLogging = false;
	m_fLog.Close();
}

//--------------------------------------------------------------------
bool CPrinter::SetPauseAtToolChange(bool bPause)
//--------------------------------------------------------------------
{
	//if this is a multi-tool configuration,
	//then set system to pause build at tool changes according to bPause
	//return true if success
	if(GetMaxTools()>1)
	{
		m_bPauseAtToolChange = bPause;
		return true;
	}
	return false;
}

//--------------------------------------------------------------------
bool CPrinter::SetAutoresume(bool bResume)
//--------------------------------------------------------------------
{
	//if system is autopausing because of tool PAUSEPATHS parameter
	//then automatically resume (without waiting for user action) if bResume
	//return true if success
	m_bAutoresume = bResume;
	return true;
}

//--------------------------------------------------------------------
CTimeSpan &CPrinter::GetManTCTime()
//--------------------------------------------------------------------
{
	//return the estimated time for a manual tool change
	return m_ctsTCMan;
}

//--------------------------------------------------------------------
CTimeSpan &CPrinter::GetAutoTCTime()
//--------------------------------------------------------------------
{
	//return the estimated time for an automatic tool change
	return m_ctsTCAuto;
}

//--------------------------------------------------------------------
bool CPrinter::EnsureLoadedTool(CString &toolname)
//--------------------------------------------------------------------
{
	//ensure that the named tool is loaded by asking user to
	//reload tools if necessary, 
	int idx = -1;

	if(CTool::LoadedToolCount() < 1)
		return false;
	if((idx = CTool::SearchByName(toolname)) < 0)
		return false;

	return true;
}

//--------------------------------------------------------------------
bool CPrinter::EnsureMountedTool(CString& toolname)
//--------------------------------------------------------------------
{
	//ensure that the named tool is loaded and mounted
	//ask user to reload and mount, exit if fail

	if(!EnsureLoadedTool(toolname))
		return false;
	
	if(!CTool::GetByName(toolname)->IsMounted())
		return false;

	return true;
}

//--------------------------------------------------------------------
bool CPrinter::EnsureCurrentTool(bool bForceDlg)
//--------------------------------------------------------------------
{
	// if bForceDlg == true, then show the tool configuration dialog
	//make sure that a current tool is defined, loaded and mounted
	if( (CTool::SearchByName(GetCurToolName()) < 0) )
	{
		if(bForceDlg)
			pApp->UserToolChange();
		return false;
	}
	if(!EnsureMountedTool(GetCurToolName()))
	{
		if(bForceDlg)
			pApp->UserToolChange();
		return false;
	}

	return true;
}

//--------------------------------------------------------------------
CTool* CPrinter::GetCurTool()
//--------------------------------------------------------------------
{
	//return pointer to the current tool
	//prompt user if unable to find
	return(CTool::GetByName(m_sCurTool));
}

//--------------------------------------------------------------------
CMaterial* CPrinter::GetCurMaterial()
//--------------------------------------------------------------------
{
	//return reference to the current material
	return(&(pApp->material[CMaterial::SearchByName(m_sCurMaterial)]));
}


//--------------------------------------------------------------------
bool CPrinter::LoadConfiguration(CString configfilename, CString& errorstr)
//--------------------------------------------------------------------
{// read configuration

	Clear();

	CStdioFile file;
	if (file.Open(configfilename, CFile::modeRead) == 0)
		return false;

	CString line;
	BOOL readok;
	CString error;
	int idx = -1;

	do {
		readok = file.ReadString(line);
		line.Trim();
		if (readok && !line.IsEmpty() && !(line[0]=='#')  && !(line[0]=='/') && !(line[0]=='%') && !(line[0]==';')) {
			CString str1 = line.SpanExcluding(" \t,:");
			CString str1a = line.Mid(str1.GetLength());
			CString str1b = str1a.SpanIncluding(" \t,:");
			CString str2 = str1a.Mid(str1b.GetLength());
			str1.Trim();
			str2.Trim();
			if (!str1.IsEmpty()) {
				if (str1.CompareNoCase("NAME")==0) {
					name = str2;
				} else if (str1.CompareNoCase("DESCRIPTION")==0) {
					description = str2;
				} else if (str1.CompareNoCase("COMPONENT")==0) {
					if (component.GetSize() >= 4) {
						error = "5th component encountered. Printer cannot have more than 4 components";
						errorstr += error;
						return false;
					}
					CString geomfile = CUtils::RemoveComment(str2);
					CString geompath = CUtils::GetPath(configfilename)+"\\"+geomfile;
					CPrinterComponent comp;
					if (_access(geompath,4) != 0 || !comp.geom.LoadSTL(geompath, 10000)) {
						error.Format("%s: Cannot load printer geometry '%s'\n", configfilename, geompath);
						errorstr += error;
						return false;}
					comp.geom.ComputeBoundingBox(comp.pmin, comp.pmax);
					component.Add(comp);
					idx = component.GetSize()-1;
					component[idx].component_geom = geomfile;
				} else if (str1.CompareNoCase("APPENDGEOM")==0) {
					CString geomfile = CUtils::RemoveComment(str2);
					component[idx].component_appendgeom = geomfile;
					CString geompath = CUtils::GetPath(configfilename)+"\\"+geomfile;
					CGeometry tmpgeom;
					bool ok = tmpgeom.LoadSTL(geompath, 10000);
					if (ok) {
						component[idx].geom.AppendGeometry(tmpgeom);
						component[idx].geom.ComputeBoundingBox(component[idx].pmin, component[idx].pmax);
					} else {
						error.Format("%s: Cannot access geometry file '%s'\n", configfilename, geomfile);
						errorstr += error;
					}
				} else if (str1.CompareNoCase("COLOR")==0) {
					str2.Replace(","," ");
					int j=sscanf_s(str2,"%lf %lf %lf %lf", 
						&component[idx].color.x, 
						&component[idx].color.y, 
						&component[idx].color.z, 
						&component[idx].alpha);
					if (j != 4) {
						error.Format("%s: Cannot read color '%s'\n", configfilename, line);
						errorstr += error;
						component[idx].color = CVec(200,200,200);
						component[idx].alpha = 255;
						return false;
					}
				} else if (str1.CompareNoCase("RANGE")==0) {
					str2.Replace(","," ");
					int j=sscanf_s(str2,"%lf %lf %lf", 
						&component[idx].rangemin,
						&component[idx].rangemax, 
						&component[idx].home);
					if (j != 3) {
						error.Format("%s: Cannot read range parameters '%s'\n", configfilename, line);
						errorstr += error;
						component[idx].rangemin = 0;
						component[idx].rangemax = 240;
						component[idx].home = 0;
						return false;
					}
				} else if (str1.CompareNoCase("DIRECTION")==0) {
					str2.Replace(","," ");
					int j=sscanf_s(str2,"%ld %lf %lf %lf", 
						&component[idx].refidx,
						&component[idx].dir.x, 
						&component[idx].dir.y, 
						&component[idx].dir.z);
					component[idx].dir.Normalize();
					if (j != 4) {
						error.Format("%s: Cannot read motion parameters '%s'\n", configfilename, line);
						errorstr += error;
						return false;
					}
					if (component[idx].refidx >= idx) {
						error.Format("%s: Component %d cannot reference future component %d in '%s'\n", configfilename, idx, component[idx].refidx, line);
						errorstr += error;
						component[idx].refidx = -1;
						return false;
					}
				} else if (str1.CompareNoCase("MOTOR")==0) {
					str2.Replace(","," ");
					int j=sscanf_s(str2,"%ld %ld %lf", 
						&component[idx].axis, &component[idx].axisDir, 
						&component[idx].mmps);
					if (j != 3) {
						error.Format("%s: Cannot read motor parameters '%s'\n", configfilename, line);
						errorstr += error;
						return false;
					}
				} else if (str1.CompareNoCase("LIMITSWITCH")==0) {
					str2.Replace(","," ");
					int j=sscanf_s(str2,"%d %d", &component[idx].posLimitSwitch, &component[idx].negLimitSwitch);
					if (j != 2) {
						error.Format("%s: Cannot read limit switch config '%s'\n", configfilename, line);
						errorstr += error;
						component[idx].posLimitSwitch = FALSE;
						component[idx].negLimitSwitch = FALSE;
					}
				} else if (str1.CompareNoCase("INCREMENT")==0) {
					str2.Replace(","," ");
					int j=sscanf_s(str2,"%lf", &component[idx].jogIncrement);
					if (j != 1) {
						error.Format("%s: Cannot read jog increment'%s'\n", configfilename, line);
						errorstr += error;
						component[idx].jogIncrement = JOG_STEP_AXIS;
					}
				} else if (str1.CompareNoCase("OFFSET")==0) {
					str2.Replace(","," ");
					int j=sscanf_s(str2,"%lf %lf %lf", &component[idx].offset.x, &component[idx].offset.y, &component[idx].offset.z);
					if (j != 3) {
						error.Format("%s: Cannot read offset coordinates '%s'\n", configfilename, line);
						errorstr += error;
						component[idx].offset = CVec(0,0,0);
						return false;
					}
				} else if (str1.CompareNoCase("STATUS_UPDATE_PERIOD")==0) {
					str2.Replace(","," ");
					double period;
					int j=sscanf_s(str2,"%lf", &period);
					SetUserStatusPeriod((DWORD)period);
					if (j != 1) {
						error.Format("%s: Cannot read status update rate '%s'\nPlease add entry STATUS_UPDATE_PERIOD and value to .printer file",
							configfilename, line);
						errorstr += error;
						return false;
					}
				} else if (str1.CompareNoCase("MAXTOOLS")==0) {
					str2.Replace(","," ");
					int n;
					int j=sscanf_s(str2,"%d", &n);
					SetMaxTools(n);
					if (j != 1) {
						error.Format("%s: Cannot read maximum number of tools: '%s'",
							configfilename, line);
						errorstr += error;
						return false;
					}
				} else if (str1.CompareNoCase("JOGSPEED")==0) {
					str2.Replace(","," ");
					double spd;
					int j=sscanf_s(str2,"%lf", &spd);
					SetJogSpeed(spd);
					if (j != 1) {
						error.Format("%s: Cannot read jog speed: '%s'",
							configfilename, line);
						errorstr += error;
						return false;
					}
				} else if (str1.CompareNoCase("TOOLLIMITSWITCH")==0) {
					str2.Replace(","," ");
					int j=sscanf_s(str2,"%d %d", 
						&m_arBLimitSwitch[2*AXIS_U+1], 
						&m_arBLimitSwitch[2*AXIS_V+1]);
					if (j != 2) {
						error.Format("%s: Cannot read limit switch config '%s'\n", configfilename, line);
						errorstr += error;
						m_arBLimitSwitch[2*AXIS_U+1] = FALSE;
						m_arBLimitSwitch[2*AXIS_V+1] = FALSE;
					}
				} else if (str1.CompareNoCase("DEFAULT_POD")==0) {
					str2.Replace(","," ");
					int j=sscanf_s(str2,"%lf %lf %lf", &m_vPOD.x, &m_vPOD.y, &m_vPOD.z);
					if (j != 3) {
						error.Format("%s: Cannot read default point of deposition: '%s'",
							configfilename, line);
						errorstr += error;
						return false;
					}
				} else {
					error.Format("%s: Unrecognized token '%s'\n", configfilename, str1);
					errorstr += error;
				}
			} else {
				error.Format("%s: Cannot parse '%s'\n", configfilename, line);
				errorstr += error;
			}
		}
	} while (readok);

	file.Close();

	//gather all of the configuration information into a compact format
	for (int i=1; i<=POSITION_AXES; i++)
	{
		//two limit switches per axis, configuration stored in printer component
		m_arBLimitSwitch[2*component[i].axis]=component[i].posLimitSwitch;
		m_arBLimitSwitch[(2*component[i].axis)+1]=component[i].negLimitSwitch;
		//store the travel for the axes
		m_ardMaxTravel[i-1] = abs((component[i].rangemax)-(component[i].rangemin));
	}
	//TODO: handle limit switch configuration for tool axes

	return true;
}

//--------------------------------------------------------------------
void CPrinter::Clear(void)
//--------------------------------------------------------------------
{
	name = "";
	description= "";	

	component.RemoveAll();
}

//--------------------------------------------------------------------
bool CPrinter::IsFabricating()
//--------------------------------------------------------------------
{
	//return true if the model is valid and fabrication has been started
	if(pModel != NULL)
	{
		if(pModel->fab.IsFabricating()) return true;
	}
	return false;
}

//--------------------------------------------------------------------
bool CPrinter::Draw(bool bCommanded)
//--------------------------------------------------------------------
{// if bCommanded, draw the desired destination, else
	//show the "real-time" (delayed) position

	if (!IsDefined())
		return false;

	//draw the current tool
	//ensure thread safety
	CSingleLock updateLock(&m_mutUpdate);
	updateLock.Lock(2*m_dwTGTStatusPeriod);
	if(GetCurTool()!=NULL)
		GetCurTool()->Draw(bCommanded);

	for (int i=0; i<component.GetSize(); i++) 
	{
		component[i].Draw(bCommanded);
	}

	return true;
}

//--------------------------------------------------------------------
void CPrinter::JogCarriageBy(CVec delta, bool bQueue)
//--------------------------------------------------------------------
{// move carriage by specified amount in mm

	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();

	//ensure thread safety
	CSingleLock updateLock(&m_mutUpdate);
	updateLock.Lock();
	component[1].stepstomove = (delta.x/component[1].mmps);
	component[2].stepstomove = (delta.y/component[2].mmps);
	component[3].stepstomove = (delta.z/component[3].mmps);

	//update the current commanded position
	SetCurrentCmdPos(delta + GetCurrentPos(bQueue));
	//TRACE("%.2f %.2f %.2f\n", delta.x, delta.y, delta.z);
}

//--------------------------------------------------------------------
void CPrinter::JogCarriageTo(CVec abspos, bool bQueue, CVec *pCurPos)
//--------------------------------------------------------------------
{// move carriage to specified position

	//if queuing this command then use the commanded position to eliminate latency errors
	//in "real-time" position
	//TRACE("%.2f %.2f %.2f\n", abspos.x, abspos.y, abspos.z);
	//ensure thread safety
	CSingleLock updateLock(&m_mutUpdate);
	updateLock.Lock();

	CVec delta = abspos - GetCurrentPos(bQueue);
	if(pCurPos!=NULL)
		delta = abspos - *pCurPos;
	JogCarriageBy(delta, bQueue);

}

//--------------------------------------------------------------------
CVec CPrinter::GetCurrentPos(bool bCommanded)
//--------------------------------------------------------------------
{
	double x,y,z;

	//ensure thread safety
	CSingleLock updateLock(&m_mutUpdate);
	updateLock.Lock();
	
	if(bCommanded)
	{
		//just report the commanded value
		x = component[1].GetCMDPos();
		y = component[2].GetCMDPos();
		z = component[3].GetCMDPos();
		TRACE("GetCurPos:CMD(%.2f,%.2f,%.2f)\n", x, y, z);
	}
	else
	{
		//the "real-time" (typically inaccurate during motion) value is desired
		x = component[1].GetRTPos();
		y = component[2].GetRTPos();
		z = component[3].GetRTPos();
		TRACE("GetCurPos:RT(%.2f,%.2f,%.2f)\n", x, y, z);
	}

	return CVec(x,y,z);
}

//--------------------------------------------------------------------
void CPrinter::SetCurrentCmdPos(CVec abspos)
//--------------------------------------------------------------------
{// update the current commanded position for the carriage

	//ensure thread safety
	CSingleLock updateLock(&m_mutUpdate);
	updateLock.Lock();
	component[1].SetCMDPos(abspos.x);
	component[2].SetCMDPos(abspos.y);
	component[3].SetCMDPos(abspos.z);
	//TRACE("%0.2f %0.2f %0.2f\n", component[1].cmdstep, component[2].cmdstep, component[3].cmdstep);
}

//--------------------------------------------------------------------
bool CPrinter::InPosition(double tol)
//--------------------------------------------------------------------
{
	//true if the real time position of the printer is 
	//within tol (each axis) of the commanded pos
	//positions are not valid prior to homing, so always return when not homed
	if(!IsHomed()) return true;

	CVec err = (GetCurrentPos(false)-GetCurrentPos(true));
	TRACE("InPosition Distance: (%0.3f,%0.3f,%0.3f)\n",err.x, err.y, err.z);
		
	return((tol*tol) > err.Length2());
}

//--------------------------------------------------------------------
void CPrinter::JogToolBy(double delta, bool bQueue)
//--------------------------------------------------------------------
{// move tool actuator by specified amount in mm

	//ensure thread safety
	CSingleLock updateLock(&m_mutUpdate);
	updateLock.Lock();
	GetCurTool()->stepstomove = (delta/GetCurTool()->mmps);

	//update the current commanded position
	//SetCurrentToolCmdPos(delta + GetCurrentToolPos(bQueue));
	SetCurrentToolCmdPos(0);
}

//--------------------------------------------------------------------
void CPrinter::JogToolTo(double abspos, bool bQueue)
//--------------------------------------------------------------------
{// move tool axis to specified position

	//if this command is to be queued, then calculate using
	//the commanded position because of latency in "real-time" position
	//for immediate commands, use the "real-time"
	//ensure thread safety
	CSingleLock updateLock(&m_mutUpdate);
	updateLock.Lock();
	double delta = abspos - GetCurrentToolPos(bQueue);

	JogToolBy(delta, bQueue);

}

//--------------------------------------------------------------------
bool CPrinter::Pushout(bool bQueue)
//--------------------------------------------------------------------
{
	//command a standard pushout for the active tool
	//only pushout if pushout state is <=0, meaning net negative pressure
	CString msg1,msg2;
	msg1.Format("Pushout(%d)",bQueue);
	msg2.Format("m_nPushoutState:%d",m_nPushoutState);
	Log(msg1,msg2);
	if(m_nPushoutState<=0)
	{
		double mmpushout = GetCurTool()->pushout *
						OLD_MSPS *
						GetCurTool()->mmps;
		DWORD index = 0;
		JogToolBy(mmpushout, bQueue);
		Move(bQueue, &index);
		WaitForMoveStart(index);
		WaitForMoveDone(index);
		m_nPushoutState++;
		return true;
	}
	return false;
}

//--------------------------------------------------------------------
bool CPrinter::Suckback(bool bQueue)
//--------------------------------------------------------------------
{
	//command a standard suckback for the active tool
	//only suckback if pushout state is >= 0, meaning net positive pressure
	CString msg1, msg2;
	msg1.Format("Suckback(%d)",bQueue);
	msg2.Format("m_nPushoutState:%d",m_nPushoutState);
	Log(msg1,msg2);
	if(m_nPushoutState>=0)
	{
		double mmsuckback = GetCurTool()->pushout *
						OLD_MSPS *
						GetCurTool()->mmps;
		DWORD index = 0;
		JogToolBy(-mmsuckback, bQueue); //immediately
		Move(bQueue, &index);
		WaitForMoveStart(index);
		WaitForMoveDone(index);
		m_nPushoutState--;
		return true;
	}
	return false;
}


//--------------------------------------------------------------------
double CPrinter::GetCurrentToolPos(bool bCommanded)
//--------------------------------------------------------------------
{
	//if bCommanded, return the commanded destination for the tool
	//else return the "real-time" (delayed)
	//ensure thread safety
	CSingleLock updateLock(&m_mutUpdate);
	updateLock.Lock();
	double x;
	if(bCommanded)
		x = GetCurTool()->cmdstep*GetCurTool()->mmps;
	else
		x = GetCurTool()->curstep*GetCurTool()->mmps;

	return x;
}

//--------------------------------------------------------------------
void CPrinter::SetCurrentToolCmdPos(double abspos)
//--------------------------------------------------------------------
{
	//set the currently commanded destination for the actuator of the specified tool
	//ensure thread safety
	CSingleLock updateLock(&m_mutUpdate);
	updateLock.Lock();
	GetCurTool()->cmdstep = (abspos/GetCurTool()->mmps);
}

//--------------------------------------------------------------------
double CPrinter::GetClearance( bool inChunk )
//--------------------------------------------------------------------
{
	//TODO: Move all path planning to a planner module
	//if inChunk, then assume we are not going to collide with 
	//other, possibly taller chunks, so just return the path to path clearance
	//height specified for this tool
	//otherwise,return the vertical clearance required to prevent tool tip
	//from colliding with the tallest part of the model built thus far

	//start by getting the recommended path to path clearance for this tool
	double z = GetCurTool()->GetClearance();
	//if we are leaving the bounding box of the current chunk, we might
	//collide with a different chunk, so increase clearance by max current height of model
	if(!inChunk)
		z += m_dModelClearance-(GetOrigin().z-GetCurrentPos().z);

	if( (z > 0.0) && (z < 300) ) return z;
	else return DEFAULT_TRAVEL_CLEARANCE;
}

//--------------------------------------------------------------------
CVec CPrinter::GetSafePos(void)
//--------------------------------------------------------------------
{ 
	//return the safe position set by the user
	return m_vSafePos;
}

//--------------------------------------------------------------------
bool CPrinter::SetSafePos(CVec sfpos)
//--------------------------------------------------------------------
{
	//set the specified position as the safe position
	if( sfpos != CVec(0,0,0) )
	{
		m_vSafePos = sfpos;
	}
	else //set the current real-time position as the safe position
	{
		m_vSafePos = GetCurrentPos(false);
	}
	m_bSafeSet = true;
	return m_bSafeSet;
}

//--------------------------------------------------------------------
CVec CPrinter::GetLastPos(void)
//--------------------------------------------------------------------
{
	//return the position stored as the "last" position, e.g. before a pause
	return m_vLastPos;
}

//--------------------------------------------------------------------
bool CPrinter::SetLastPos(CVec lpos)
//--------------------------------------------------------------------
{
	//set the specified position as the prior or last position; for resuming from a pause
	if( lpos != CVec(0,0,0) )
	{
		m_vLastPos = lpos;
	}
	else //set the current real-time position as the last position
	{
		m_vLastPos = GetCurrentPos(false);
	}
	m_bLastSet = true;
	CString msg1;
	msg1.Format("SetLastPos(%0.2f,%0.2f,%0.2f)",m_vLastPos.x,m_vLastPos.y,m_vLastPos.z);
	Log(msg1,msg1);
	return m_bLastSet;
}

//--------------------------------------------------------------------
CVec CPrinter::RobotToModelTfm( CVec &v )
//--------------------------------------------------------------------
{
	//transform the given vector into model coords
	return( CVec(v.x,v.y,-v.z) );
}

//--------------------------------------------------------------------
CVec CPrinter::ModelToRobotTfm( CVec &v )
//--------------------------------------------------------------------
{
	//transform the given vector into robot coords
	return( CVec(v.x,v.y,-v.z) );
}

//---------------------------------------------------------------------------
bool CPrinter::CanAutoRange()
//---------------------------------------------------------------------------
{
	//return true if the printer configuration allows for automatic homing
	//and automatic identification of range of motion
	
	if(!bEmulateHardware) //if only emulating hardware, then always true
	{
		//check that x, y, z limit switches are configured
		//for both forward and backward motion
		int i = 0;
		while(i<POSITION_AXES && 
			m_arBLimitSwitch[2*component[i+1].axis] &&
			m_arBLimitSwitch[(2*component[i+1].axis)+1])
		{
			i++;
		}
		return (i==POSITION_AXES); //xfwd,xbkd,yfwd,ybkd,zfwd,zbkd all configured
	}
	return true;
}

//---------------------------------------------------------------------------
UINT CPrinter::HomeSeqProc( LPVOID pParam )
//---------------------------------------------------------------------------
{
	//worker function for homing sequence
	CPrinter *pMachine = (CPrinter *)pParam;
	if(pMachine->IsHomed())
		pMachine->FindHomePos();
	else if(AfxMessageBox("Automatically find home position?", MB_OKCANCEL)==IDOK)
			pMachine->FindHomePos(); //try to automatically find the home position
	return 0;
}

//---------------------------------------------------------------------------
void CPrinter::ExecuteHomeSequence()
//---------------------------------------------------------------------------
{
	//start a worker thread to execute a short sequence of immediate motions
	//these execute ahead of any queued motion.

	AfxBeginThread(HomeSeqProc,this);

}

//--------------------------------------------------------------------
void CPrinter::GotoHome()
//--------------------------------------------------------------------
{// move printer head to home position (0,0,0)

	CVec home = CVec(0.0,0.0,0.0);
	if(bEmulateHardware || IsHomed())
	{
		DWORD index = 0; //var to store index for completion check
		JogCarriageTo(home, false); //immediate motion
		Move(false, &index); //immediate
		WaitForMoveStart(index);
		WaitForMoveDone(index);
	}
	else
	{
		CString msg;
		msg.Format("Hardware not homed");
		((CFabAtHomeApp *)AfxGetApp())->Log(msg);
	}
	// TODO: If limit switches installed, drive a little extra into the switches
}

//--------------------------------------------------------------------
bool CPrinter::IsHomed()
//--------------------------------------------------------------------
{
	//true if software is homed and emulating or both software and hardware
	//are homed and using hardware
	if(bEmulateHardware)
		return m_bHomed;
	if(!bEmulateHardware && bHardwareInitialized)
		return (m_bHomed && m_stateFlags.HOMED);
	return false;
}

//--------------------------------------------------------------------
void CPrinter::GotoOrigin()
//--------------------------------------------------------------------
{// move printer head to build origin position

	if(bEmulateHardware || !bHardwareInitialized || m_bOriginSet)
	{
		DWORD index = 0; //var to store index for completion check

		//determine where to go
		CVec org = GetOrigin();
		double trav = GetClearance(false); //false to indicate possible collision
		//move down for safe traverse
		JogCarriageBy(CVec(0,0,-trav), false); //immediate, not queued
		Move(false, &index);
		WaitForMoveStart(index);
		WaitForMoveDone(index);
		//move to the last position, xy first
		CVec cur = GetCurrentPos(false);
		JogCarriageTo(CVec(org.x, org.y, cur.z),false); //immediate, not queued
		Move(false, &index);
		WaitForMoveStart(index);
		WaitForMoveDone(index);
		//then move the z
		JogCarriageTo(org, false); //immediate, not queued
		Move(false, &index);
		WaitForMoveStart(index);
		WaitForMoveDone(index);
	}
	else
	{
		CString msg;
		msg.Format("Last return position not set");
		((CFabAtHomeApp *)AfxGetApp())->Log(msg);
	}
}

//---------------------------------------------------------------------------
UINT CPrinter::OriginSeqProc( LPVOID pParam )
//---------------------------------------------------------------------------
{
	//worker function for 
	CPrinter *pMachine = (CPrinter *)pParam;
	if(pMachine->IsOriginSet())
		pMachine->GotoOrigin();
	else if(AfxMessageBox("Automatically find origin X,Y position?", MB_OKCANCEL)==IDOK)
		pMachine->FindOrigin(); //move to origin automatically
	return 0;
}

//---------------------------------------------------------------------------
void CPrinter::ExecuteOriginSequence()
//---------------------------------------------------------------------------
{
	//start a worker thread to execute a short sequence of immediate motions
	//these execute ahead of any queued motion.

	AfxBeginThread(OriginSeqProc,this);

}

//--------------------------------------------------------------------
void CPrinter::GotoSafe()
//--------------------------------------------------------------------
{// move printer head to safe position

	if(m_bSafeSet)
	{
		DWORD index = 0; //var to store index for completion check
		//determine where to go
		CVec safe = GetSafePos();
		double trav = GetClearance(false); //false to indicate possible collision
		//move down for safe traverse
		JogCarriageBy(CVec(0,0,-trav), false); //immediate, not queued
		Move(false, &index);
		WaitForMoveStart(index);
		WaitForMoveDone(index);
		//move to the safe position, xy first if safe z is higher
		CVec cur = GetCurrentPos(false);
		CString msg1, msg2;
		msg1.Format("GotoSafe(%0.2f,%0.2f,%0.2f)",safe.x,safe.y,safe.z);
		msg2.Format("CurRTPos:%0.2f,%0.2f,%0.2f", cur.x,cur.y,cur.z);
		Log(msg1,msg2);
		if(safe.z > cur.z)
		{
			//if safe z is higher than current, move xy first
			JogCarriageTo(CVec(safe.x, safe.y, cur.z), false); //immediate, not queued
			Move(false, &index);
			WaitForMoveStart(index);
			WaitForMoveDone(index);
			//now move the z
			JogCarriageTo(safe, false); //immediate, not queued
			Move(false, &index);
			WaitForMoveStart(index);
			WaitForMoveDone(index);
		}
		else
		{
			//move down first
			JogCarriageTo(CVec(cur.x, cur.y, safe.z), false); //immediate, not queued
			Move(false, &index);
			WaitForMoveStart(index);
			WaitForMoveDone(index);
			//now move the xy
			JogCarriageTo(safe, false); //immediate, not queued
			Move(false, &index);
			WaitForMoveStart(index);
			WaitForMoveDone(index);
		}
	}
	else
	{
		CString msg;
		msg.Format("Safe position not set");
		((CFabAtHomeApp *)AfxGetApp())->Log(msg);
	}
}

//---------------------------------------------------------------------------
UINT CPrinter::SafeSeqProc( LPVOID pParam )
//---------------------------------------------------------------------------
{
	//worker function for safe position search sequence
	CPrinter *pMachine = (CPrinter *)pParam;
	//if safe has already been set, then drive to limits
	//and reset that first position (for error correction)
	if(pMachine->IsSafeSet()) pMachine->FindSafePos(pMachine->GetSafePos());
	else if(AfxMessageBox("Automatically find safe position?", MB_OKCANCEL)==IDOK)
			pMachine->FindSafePos(); //try to automatically find the safe position
	return 0;
}

//---------------------------------------------------------------------------
void CPrinter::ExecuteSafeSequence()
//---------------------------------------------------------------------------
{
	//start a worker thread to execute a short sequence of immediate motions
	//these execute ahead of any queued motion.

	AfxBeginThread(SafeSeqProc,this);
}

//--------------------------------------------------------------------
void CPrinter::GotoLast()
//--------------------------------------------------------------------
{// move printer head to "last" position - e.g. last position prior to a pause

	if(m_bLastSet)
	{
		DWORD index = 0;
		//determine where to go
		CVec last = GetLastPos();
		double trav = GetClearance(false); //false to indicate possible collision
		////move down for safe traverse
		//JogCarriageBy(CVec(0,0,-trav), false); //immediate, not queued
		//Move(false, &index);
		//WaitForMoveStart(index);
		//WaitForMoveDone(index);
		//move to the last position, xy first if last z is higher than current z
		CVec cur = GetCurrentPos(false);
		CString msg1, msg2;
		msg1.Format("GotoLast(%0.2f,%0.2f,%0.2f)",last.x,last.y,last.z);
		msg2.Format("CurRTPos:%0.2f,%0.2f,%0.2f", cur.x,cur.y,cur.z);
		Log(msg1,msg2);
		if(last.z > cur.z)
		{
			//if last z is higher than current, move xy first
			JogCarriageTo(CVec(last.x, last.y, cur.z), false); //immediate, not queued
			Move(false, &index);
			WaitForMoveStart(index);
			WaitForMoveDone(index);
			//now move the z
			JogCarriageTo(last, false); //immediate, not queued
			Move(false, &index);
			WaitForMoveStart(index);
			WaitForMoveDone(index);
		}
		else
		{
			//move down first
			JogCarriageTo(CVec(cur.x, cur.y, last.z), false); //immediate, not queued
			Move(false, &index);
			WaitForMoveStart(index);
			WaitForMoveDone(index);
			//now move the xy
			JogCarriageTo(last, false); //immediate, not queued
			Move(false, &index);
			WaitForMoveStart(index);
			WaitForMoveDone(index);
		}
	}
	else
	{
		CString msg;
		msg.Format("Last return position not set");
		((CFabAtHomeApp *)AfxGetApp())->Log(msg);
	}
}

//--------------------------------------------------------------------
void CPrinter::GotoNext()
//--------------------------------------------------------------------
{// move printer head to the start of the next path

	if(pModel != NULL)
	{
		DWORD index = 0;
		double trav = GetClearance(false); //false to indicate possible collision
		//fetch the first vertex of the first path of the current layer
		CVec next = GetOrigin()+ModelToRobotTfm(pModel->fab.layer[pModel->fab.GetCurrentLayer()].region[0].path[0].v[0]);
		//adjust the z for the travel height
		next.z -= trav;
		//move down for safe traverse
		JogCarriageBy(CVec(0,0,-trav), false); //immediate, not queued
		Move(false, &index);
		WaitForMoveStart(index);
		WaitForMoveDone(index);
		
		//move to the last position, xy first if last z is higher than current z
		CVec cur = GetCurrentPos(false);
		if(next.z > cur.z)
		{
			//if next z is higher than current, move xy first
			JogCarriageTo(CVec(next.x, next.y, cur.z), false); //immediate, not queued
			Move(false, &index);
			WaitForMoveStart(index);
			WaitForMoveDone(index);
			//now move the z
			JogCarriageTo(next, false); //immediate, not queued
			Move(false, &index);
			WaitForMoveStart(index);
			WaitForMoveDone(index);
		}
		else
		{
			//move down first
			JogCarriageTo(CVec(cur.x, cur.y, next.z), false); //immediate, not queued
			Move(false, &index);
			WaitForMoveStart(index);
			WaitForMoveDone(index);
			//now move the xy
			JogCarriageTo(next, false); //immediate, not queued
			Move(false, &index);
			WaitForMoveStart(index);
			WaitForMoveDone(index);
		}
	}
	else
	{
		CString msg;
		msg.Format("Unable to return to next path");
		((CFabAtHomeApp *)AfxGetApp())->Log(msg);
	}
}

//---------------------------------------------------------------------------
UINT CPrinter::LastSeqProc( LPVOID pParam )
//---------------------------------------------------------------------------
{
	//worker function for 
	CPrinter *pMachine = (CPrinter *)pParam;
	pMachine->GotoLast();
	return 0;
}

//---------------------------------------------------------------------------
void CPrinter::ExecuteLastSequence()
//---------------------------------------------------------------------------
{
	//start a worker thread to execute a short sequence of immediate motions
	//these execute ahead of any queued motion.

	AfxBeginThread(LastSeqProc,this);

}

//--------------------------------------------------------------------
CVec CPrinter::GetOrigin()
//--------------------------------------------------------------------
{// return the build origin

	return m_vOrigin;
}

//--------------------------------------------------------------------
void CPrinter::SetOrigin(CVec org)
//--------------------------------------------------------------------
{
	// set the specified location as the origin, unless not specified
	//in which case, use the current location

	if( org != CVec(0,0,0) )
	{
		m_vOrigin = org;
	}
	else //set the current commanded position as the origin
	{
		m_vOrigin = GetCurrentPos();
	}
	//now set the point of deposition based on the current tool
	m_vPODOffset = GetCurTool()->tip - m_vPOD;
	m_vPOD = GetCurTool()->tip;
	m_bOriginSet = true;
	//init the last position to the origin
	SetLastPos(m_vOrigin);
}

//--------------------------------------------------------------------
CVec CPrinter::GetHomePos()
//--------------------------------------------------------------------
{// get the printer home position FOR GRAPHICS ONLY

	ASSERT(component.GetSize() == 4);
	if (component.GetSize() != 4)
		return CVec(0,0,0);

	CVec home;
	home.x = component[1].home;
	home.y = component[2].home;
	home.z = component[3].home;

	return home;
}

//--------------------------------------------------------------------
bool CPrinter::SetHomePos(CVec& pos)
//--------------------------------------------------------------------
{// set current position as the home
	//store it for use in keeping graphics displayed correctly
	//zero the hardware and software position registers
	//if hardware is initialized, don't set home until motion has ceased

	ASSERT(component.GetSize() == 4);
	if (component.GetSize() != 4)
		return false;
	
	//now clear the realtime and commanded position registers
	component[1].SetCMDPos(pos.x);
	component[1].SetRTPos(pos.x);
	component[1].ClearErrorOffset();
	component[2].SetCMDPos(pos.y);
	component[2].SetRTPos(pos.y);
	component[2].ClearErrorOffset();
	component[3].SetCMDPos(pos.z);
	component[3].SetRTPos(pos.z);
	component[3].ClearErrorOffset();

	m_bHomed = true;
	if(!bEmulateHardware)
		return (m_bHomed = HomeHW()); //tell printer it is home, clear position registers

	return true;
}

//--------------------------------------------------------------------
bool CPrinter::FindOrigin()
//--------------------------------------------------------------------
{
	//move to the X,Y coords of the origin,
	//ask the user to set the z coord manually
	if(!IsSafeSet() || !IsHomed())
	{
		((CFabAtHomeApp *)AfxGetApp())->Log("Please set the home and safe positions before setting the origin.");
		return false;
	}
	DWORD index = 0;
	m_vOrigin = CVec(100,100,50);
	JogCarriageTo(m_vOrigin,false);
	Move(false, &index);
	WaitForMoveStart(index);
	WaitForMoveDone(index);
	if(!InPosition()) return false;
	//now ask user to move build surface up to tool
	CString msg;
	msg.Format("Please ensure that %s is installed, then jog the build surface up until it contacts the tool tip, and hit Set Build Origin",
		GetCurToolName());
	((CFabAtHomeApp *)AfxGetApp())->Log(msg);
	return true;
}


//--------------------------------------------------------------------
bool CPrinter::FindHomePos()
//--------------------------------------------------------------------
{
	//if limit switches are configured (CanAutoRange() == true)
	//then seek the home limit switches
	//else tell user to jog manually to home
	if(bEmulateHardware)
	{
		JogCarriageTo(CVec(0,0,0),false);
		Move();
		if(SetHomePos())
		{
			((CFabAtHomeApp *)AfxGetApp())->Log("Hardware successfully homed.");
			return true;
		}
	}
	else
	{
		if(CanAutoRange())
		{
			DWORD index = 0;
			//TODO: unscramble the limit and motion directions
			//need the xfwd (-x_model), yfwd (-y_model), zbkd (+z_model) limits to be active at home
			//and remember active == 0

			if(IsHomed())
			{
				GotoHome();
				((CFabAtHomeApp *)AfxGetApp())->Log("Home position successfully reached.");
				return true;
			}

			JogCarriageBy(CVec(-1.1*m_ardMaxTravel[0],-1.1*m_ardMaxTravel[1],-1.1*m_ardMaxTravel[2]), false); 
			Move(false, &index); //immediate motion. full speed because we'll use limits to stop
			WaitForMoveStart(index);
			WaitForMoveDone(index); //wait for completion
			
			if(!GetLimit(AXIS_X,true) && !GetLimit(AXIS_Y,true) && !GetLimit(AXIS_Z,false))
			{
				//set the home position here to clear the position registers
				SetHomePos();
		
				CVec avePos;
				int i = 0;
				while( i < AVERAGE )
				{
					if( IsEStop() ) return false;

					if(!GetLimit(AXIS_X,true) && !GetLimit(AXIS_Y,true) && !GetLimit(AXIS_Z,false))
					{
						//we are now at the home position, so store it
						avePos += GetCurrentPos(false);
						i++;
					}
					//move off limits a small distance
					JogCarriageBy(CVec(1,1,1), false); 
					Move(false, &index); //immediate motion. full speed because we'll use limits to stop
					WaitForMoveStart(index);
					WaitForMoveDone(index); //wait for completion
					//move back into limits by slightly more distance to be sure to trigger
					JogCarriageBy(CVec(-2,-2,-2), false); 
					Move(false, &index); //immediate motion. full speed because we'll use limits to stop
					WaitForMoveStart(index);
					WaitForMoveDone(index); //wait for completion
				}
							
				//now calculate the average position of the limit trigger
				avePos /= (double)i;			

				//and set the home position to be the offset of our current real-time position from the average trigger point
				if(SetHomePos(GetCurrentPos(false)-avePos))
				{
					((CFabAtHomeApp *)AfxGetApp())->Log("Hardware successfully homed.");
					return true;
				}
			}
		}
		((CFabAtHomeApp *)AfxGetApp())->Log("Unable to home automatically. Please move to home position via jog controls, then <Set Home>.");
	}
	return false;
}

//--------------------------------------------------------------------
bool CPrinter::TrackError(CVec &pos)
//--------------------------------------------------------------------
{
	//track position error by comparing pos to calibrated safe position
	/*CString msg;
	msg.Format("TrackError(pos(%0.2f,%0.2f,%0.2f)-m_vSafePos(%0.2f,%0.2f,%0.2f))",
		pos.x,pos.y,pos.z,m_vSafePos.x,m_vSafePos.y,m_vSafePos.z);
	((CFabAtHomeApp *)AfxGetApp())->Log(msg);*/
	component[1].AccumulateErrorOffset(pos.x-m_vSafePos.x);
	component[2].AccumulateErrorOffset(pos.y-m_vSafePos.y);
	component[3].AccumulateErrorOffset(pos.z-m_vSafePos.z);
	return true;
}


//--------------------------------------------------------------------
bool CPrinter::FindSafePos(CVec firstSafe)
//--------------------------------------------------------------------
{
	//if limit switches are configured (CanAutoRange() == true, and system IsHomed
	//then seek the safe limit switches (front, right, bottom when facing front of machine)
	//else tell user to jog manually to a safe position
	//if firstSafe is non-zero, then set the safe position to the provided value
	CString msg;
	if(bEmulateHardware)
	{
		JogCarriageTo(CVec(m_ardMaxTravel[0],m_ardMaxTravel[1],0), false); 
		Move();
		if(!InPosition()) return false;
		if(SetSafePos(firstSafe))
		{

			if(firstSafe==CVec(0,0,0)) 
				((CFabAtHomeApp *)AfxGetApp())->Log("Safe position successfully found.");
			return true;
		}
	}
	else
	{
		if(!IsHomed())
		{
			msg.Format("Please home the system before trying to find the safe position");
		}
		else if(!CanAutoRange())
		{
			msg.Format("Unable to find safe position automatically. Please move to a safe position via jog controls, then <Set Safe Pos>.");
		}
		else 
		{
			DWORD index = 0;
			//TODO: unscramble the limit and motion directions
			//need the xbkd (+x_model), ybkd (+y_model), zbkd (+z_model) limits to be active at safe
			//and remember active == 0

			// Enable the ability to manually set the safe position
			if(IsSafeSet())
			{
				//Use GotoSafe because it moves down before moving over to protect parts from collision
				GotoSafe();
				((CFabAtHomeApp *)AfxGetApp())->Log("Safe position successfully reached.");
				return true;
				// move toward front, right, bottom a bit more (10% of travel) in case of lost steps
				// JogCarriageBy(CVec(0.25*m_ardMaxTravel[0],0.25*m_ardMaxTravel[1],-0.25*m_ardMaxTravel[2]), false); 
			}
			else
			{
				//move to front, right, bottom when facing machine front
				//ensure limit contact by moving a bit extra (x= 1.1*max travel, y= 1.1*max travel, z = -0.1*max travel)
				//JogCarriageTo(CVec(250,250,-200), false); 
						
				JogCarriageBy(CVec(1.1*m_ardMaxTravel[0],1.1*m_ardMaxTravel[1],-1.1*m_ardMaxTravel[2]), false); 
				Move(false, &index); //immediate motion. full speed because we'll use limits to stop
				WaitForMoveStart(index);
				WaitForMoveDone(index); //wait for completion
				
				if(!GetLimit(AXIS_X,false) && !GetLimit(AXIS_Y,false) && !GetLimit(AXIS_Z,false))
				{
					CVec avePos;
					int i = 0;
					while( i < AVERAGE )
					{
						if( IsEStop() ) return false;

						if(!GetLimit(AXIS_X,false) && !GetLimit(AXIS_Y,false) && !GetLimit(AXIS_Z,false))
						{
							//we are now at the safe position, so store it
							avePos += GetCurrentPos(false);
							i++;
						}
						//move off limits a small distance
						JogCarriageBy(CVec(-1,-1,1), false); 
						Move(false, &index); //immediate motion. full speed because we'll use limits to stop
						WaitForMoveStart(index);
						WaitForMoveDone(index); //wait for completion
						//move back into limits by slightly more distance to be sure to trigger
						JogCarriageBy(CVec(5,5,-5), false); 
						Move(false, &index); //immediate motion. full speed because we'll use limits to stop
						WaitForMoveStart(index);
						WaitForMoveDone(index); //wait for completion
					}
								
					//now calculate the average position of the limit trigger
					avePos /= (double)i;
					//if the safe position has been set already via calibration, then only track error
					//otherwise set the safe position
					if(IsSafeSet())
					{
						TrackError(avePos);
						return true;
					}
					else
					{
						if(firstSafe==CVec(0,0,0))
							SetSafePos(avePos);
						else
							SetSafePos(firstSafe);
						((CFabAtHomeApp *)AfxGetApp())->Log("Safe Position Found.");
						return true;
					}
				}

			}

			msg.Format("Unable to find safe position automatically.");
		}
		((CFabAtHomeApp *)AfxGetApp())->Log(msg);
	}
	return false;
}

//--------------------------------------------------------------------
void CPrinter::Move(bool bQueue, DWORD *pIndex)
//--------------------------------------------------------------------
{
	//this overload is to allow calling with a default slowfactor
	//but still specifying bQueue and pIndex
	Move(-1, bQueue, pIndex);
}

//--------------------------------------------------------------------
void CPrinter::Move(double slowfactor, bool bQueue, DWORD *pIndex)
//--------------------------------------------------------------------
{ // move motors simultaneously by specified amounts

	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	if(slowfactor < 1) //default to m_dJogSpeed
		slowfactor = GetMaxPathSpeed()/GetJogSpeed();
	ASSERT(slowfactor >= 1);

	CSegment curSeg;

	for (int i=0; i<AXES_CNT; i++) {
		curSeg.d[i]=0;
	}

	 // first 3 counters are for x,y,z - read control axis from component
	for(int i = 0; i<POSITION_AXES; i++)
	{
		curSeg.d[pApp->printer.component[i+1].axis] = 
			CUtils::Round(pApp->printer.component[i+1].stepstomove); //fir
		///////////////8-4-2006 Fixing Simultaneity////////////////////////////////////////
		pApp->printer.component[i+1].stepstomove = 0;
		///////////////8-4-2006 Fixing Simultaneity////////////////////////////////////////
	}

	// rest of counters are for tools
	//again, read the control axis (which amplifier) from the tool
	curSeg.d[GetCurTool()->axis] = (long)GetCurTool()->stepstomove;
	///////////////8-4-2006 Fixing Simultaneity////////////////////////////////////////
	GetCurTool()->stepstomove = 0;
	///////////////8-4-2006 Fixing Simultaneity////////////////////////////////////////

	//s2 needs to be even larger than 32bits, else overflow
	long long s2=0;
	for (int i=0; i<AXES_CNT; i++) {
		s2 += (long long)curSeg.d[i]*(long long)curSeg.d[i];
	}

	curSeg.s = CUtils::Round(sqrt((long double)s2)*slowfactor); // compute hypotenuse
	curSeg.L = curSeg.s;  //L == s
	curSeg.pIndex = pIndex;

	//if logging is on, log the command
	CString fun, bq;
	if(bQueue) bq = "true";
	else bq = "false";
	fun.Format("Move(%0.2f, %s)",slowfactor,bq);
	Log(fun,fun);
	//send the segment to the hardware or simulate it
	//record the index of the command
	if(bEmulateHardware)
		DoEmulatorStep(curSeg, bQueue, pIndex);
	else
		DoHardwareStep(curSeg, bQueue, pIndex);
}

//--------------------------------------------------------------------
bool CPrinter::DoHardwareStep(CSegment &curSeg, bool bQueue, DWORD* pIndex)
//--------------------------------------------------------------------
{ // move hardware (or emulate hardware motion)
	//if bQueue, then the command will be queued, and should
	//be based on commanded positions rather than "real-time"
	//to eliminate latency error

  // curSeg.d: array of motion signed deltas in stepping units
  // curSeg.s: pulse threshold (inversely proportional to speed)
  // curSeg.L: Number of times to loop
  // return false if error occured (abort requested), otherwise true.

	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	CSingleLock aryLock(&m_mutUpdate);
	
	if(!IsStarted() || IsEStop())
	{
		if(IsEStop())
		{
			if(IDNO == AfxMessageBox("Clear Emergency Stop, and resume motion?", MB_YESNO))
			{
				//if queue is not empty, make sure it is paused so that user does not
				//keep getting asked to enable
				//AbortBuild();
				pModel->fab.RequestAbort();
				return true; //user wants this condition, so good result;
			}
			else
				ClearEStop();
		}
	}
	if(!bEmulateHardware)
	{
		if(!IsStarted())
			Start(); //enable motion
		
		// move real hardware here
		// component.curstep and tool.curstep updated
		// in GetStatus, running in a worker thread with some latency
		// when status is successfully returned, bNewStatus is set
		// the views can then be updated only when necessary
		
		////////////////////////////////////////////////////////////////////////
		//TODO: have firmware handle axes directions via configuration packet
		//need to accommodate the different orientations of the hardware axes
		//relative to the graphics here
		//for now - just change the signs of the d values appropriately
		curSeg.d[component[1].axis] *= component[1].axisDir;
		curSeg.d[component[2].axis] *= component[2].axisDir;
		curSeg.d[component[3].axis] *= component[3].axisDir;
			
		curSeg.d[GetCurTool()->axis] *= GetCurTool()->axisDir;
		//don't forget to correct the status packets as well!
		///////////////////////////////////////////////////////////////////////

		
		aryLock.Lock();
		if(bQueue) //load segment into the m_pathArray buffer for transmission to uC
		{
			//make the m_pathArray thread safe; accessed by status thread and fab thread
			m_pathArray.Add(curSeg);
			//tell the worker thread to start moving path data to the uC
			m_bQPathDone = false;	
		}
		else if(!IMMSegment(curSeg, pIndex)) //send an immediate segment
			return false;
		aryLock.Unlock();
	} 
	return true;
}

//--------------------------------------------------------------------
bool CPrinter::DoEmulatorStep(CSegment &curSeg, bool bQueue, DWORD* pIndex)
//--------------------------------------------------------------------
{ // emulate hardware motion)
	//if bQueue, then the command will be queued, and should
	//be based on commanded positions rather than "real-time"
	//to eliminate latency error

  // curSeg.d: array of motion signed deltas in stepping units
  // curSeg.s: pulse threshold (inversely proportional to speed)
  // curSeg.L: Number of times to loop
  // return false if error occured (abort requested), otherwise true.

	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	int i;
	
	if(!IsStarted() || IsEStop())
	{
		if(IsEStop())
		{
			if(IDNO == AfxMessageBox("Clear Emergency Stop, and resume motion?", MB_YESNO))
			{
				//if queue is not empty, make sure it is paused so that user does not
				//keep getting asked to enable
				pModel->fab.RequestAbort();

				return true; //user wants this condition, so good result;
			}
			else
				ClearEStop();
		}
	}
	// Emulate hardware
	unsigned int c[AXES_CNT]; // counters
	double h[AXES_CNT];
	for (i=0; i<AXES_CNT; i++)
	{
		c[i]=0;
		if (curSeg.d[i]>0)
		{
			h[i] = 1.0;
		}
		else
		{
			h[i] = -1.0;
		}
		curSeg.d[i] = abs(curSeg.d[i]);
	}
	for (unsigned int j=0; j<=curSeg.L; j++)
	{
		for (i=0; i<AXES_CNT; i++)
		{
			c[i] += curSeg.d[i];
			if (c[i] > curSeg.s)
			{
				c[i] -= curSeg.s;
				if (i<3)
				{ 
					component[i+1].SetRTPos(component[i+1].GetRTPos()+h[i]*component[i+1].mmps);
					// emulate carriage hardware limit switches (if exist)
					if (component[i+1].GetRTPos()<component[i+1].rangemin) 
					{
						component[i+1].SetRTPos(component[i+1].rangemin);
						//MessageBeep(MB_ICONHAND);
					}
					if (component[i+1].GetRTPos()>component[i+1].rangemax) 
					{
						component[i+1].SetRTPos(component[i+1].rangemax);
						//MessageBeep(MB_ICONHAND);
					}
				} 
				else
				{
					GetCurTool()->curstep+=h[i]*m_dEmulatorSlowdown;
					// emulate tool hardware limit switches (if exist)
					if (GetCurTool()->curstep*GetCurTool()->mmps<GetCurTool()->rangemin)
					{
						GetCurTool()->curstep=GetCurTool()->rangemin/GetCurTool()->mmps;
						MessageBeep(MB_ICONHAND);
					}
					if (GetCurTool()->curstep*GetCurTool()->mmps>GetCurTool()->rangemax)
					{
						GetCurTool()->curstep=GetCurTool()->rangemax/GetCurTool()->mmps;
						MessageBeep(MB_ICONHAND);
					}
					//collect trace points while depositing
					if(pModel != NULL)
					{
						CVec X;
						X.x = component[1].GetRTPos();
						X.y = component[2].GetRTPos();
						X.z = component[3].GetRTPos();
						pModel->TracePoint(ModelToRobotTfm(X-m_vOrigin));
					}
				}
			}
			if(IsEStop()) return false;
		}
		//tell everyone that the graphics need updating
		if(j%(int)(5000.0/m_dEmulatorSlowdown) == 0)
			AfxGetMainWnd()->Invalidate();
	}
	return true;
}

//--------------------------------------------------------------------
bool CPrinter::EmulateSlower()
//--------------------------------------------------------------------
{
	//speed up the emulation rate
	if(m_dEmulatorSlowdown < 1000)
	{
		m_dEmulatorSlowdown*=1.1;
		return true;
	}
	return false;
}

//--------------------------------------------------------------------
bool CPrinter::EmulateFaster()
//--------------------------------------------------------------------
{
	//slow down the emulation rate
	if(m_dEmulatorSlowdown > 0)
	{
		m_dEmulatorSlowdown/=1.1;
		return true;
	}
	return false;
}

//--------------------------------------------------------------------
double CPrinter::GetCarriageSpeed()
//--------------------------------------------------------------------
{
	//return the current path-tangential speed of the carriage
	CSingleLock updateLock(&m_mutUpdate);
	if(!updateLock.Lock((DWORD)(2*GetTargetStatusPeriod())))
	{
		//have lock timeout
		return false;
	}
	double vi, speed=0.0;
	for(int i = 1; i<=POSITION_AXES; i++)
	{
		vi=component[i].GetVel();
		speed+=vi*vi;
	}


	//TRACE("Speed^2: %f\n", speed);
	return sqrt(speed);
}

//--------------------------------------------------------------------
bool CPrinter::IsMoving()
//--------------------------------------------------------------------
{
	//check whether hardware "MOVING" flag is set
	//or return immediately if simulating
	if(bEmulateHardware) return (GetCarriageSpeed()>0.0);
	return (m_stateFlags.MOVING && (GetCarriageSpeed()>0.0));
}

//--------------------------------------------------------------------
bool CPrinter::IsToolMoving()
//--------------------------------------------------------------------
{
	//check whether hardware "MOVING" flag is set
	//or return immediately if simulating
	if(bEmulateHardware) return (false);
	return (m_bToolMoving);
}

//--------------------------------------------------------------------
bool CPrinter::WaitForMoveDone(long index, int timeout_ms)
//--------------------------------------------------------------------
{
	//sleep until the hardware reports completion of the indexed command
	//or wait for hardware MOVING flag to be false if index not valid
	//or return immediately if simulating
	//returns false if the commanded move is reported as done or motion has stopped, but not InPosition
	//TODO: make the firmware handle the MOVING flag more carefully to prevent false reporting
	
	if (m_dwTGTStatusPeriod < STATUS_UPDATE_PERIOD)
	{
		((CFabAtHomeApp *)AfxGetApp())->Log("Could not control the printer movement, please increase Status Update Period.");
		return false;
	}
	

	if(bEmulateHardware) return true;

	if(index < 0)
	{
		while( (timeout_ms > 0) && (IsMoving() || IsToolMoving()))
		{
			Sleep(SLEEP_PERIOD);
			if(!IsMoving() && !IsToolMoving())
			{
				timeout_ms -= SLEEP_PERIOD;
			}
		}
	}
	else
	{
		//wait for confirmation from the micro that the indexed command has been completed
		//but also include a timeout and a check for motion having stopped
		while( ((GetLastCmdExecuted() < (DWORD)index) || IsMoving() || IsToolMoving()) && (timeout_ms > 0))
		{
			Sleep(SLEEP_PERIOD);
			if(!IsMoving() && !IsToolMoving())
			{
				timeout_ms -= SLEEP_PERIOD;
			}
		}
	}
	return InPosition();
}

//--------------------------------------------------------------------
bool CPrinter::WaitForMoveStart(long index, int timeout_ms)
//--------------------------------------------------------------------
{
	//sleep until the hardware reports that the indexed command is being executed
	//or wait for hardware MOVING flag to be true if index not valid
	//or return immediately if simulating

	
	if(bEmulateHardware) return true;

	if(index < 0)
	{
			while((!IsMoving() || !IsToolMoving()) && (timeout_ms > 0))
		{
			Sleep(SLEEP_PERIOD);
			timeout_ms -= SLEEP_PERIOD;
		}
		if(timeout_ms <= 0)
			return false;
	}
	else
	{
		while( (GetCurCommand() < (DWORD)index) && (timeout_ms > 0) )
		{
			Sleep(SLEEP_PERIOD);
			timeout_ms -= SLEEP_PERIOD;
		}
		if(timeout_ms <= 0)
			return false;
	}

	return true;
}

//--------------------------------------------------------------------
void CPrinter::WaitForStatusUpdate(int nPeriods)
//--------------------------------------------------------------------
{
	//wait for status update thread to update the status info
	DWORD timeout = nPeriods * m_dwTGTStatusPeriod;
	while(timeout > 0)
	{
		Sleep(m_dwTGTStatusPeriod);
		timeout-=m_dwTGTStatusPeriod;
	}
}

//--------------------------------------------------------------------
bool CPrinter::CleanupBuild(void)
//--------------------------------------------------------------------
{
	//clean up variables and system state after successful build
	if(pModel!=NULL)
	{
		if(pModel->fab.IsFabricating())
			pModel->fab.RequestAbort();
	}
	//now stop sending commands 
	if(!m_bQPathDone)
		m_bQPathDone = true; //this will stop the queueing thread from sending any more segments

	//and empty the path array
	m_pathArray.RemoveAll();
	//zero out the path index counter
	m_nPathIndex = 0;

	//make sure the queue is empty
	if(!IsQEmpty()) Abort();
	WaitForStatusUpdate();
	//and clear any aborts
	if(IsAborted()) Abort();
	
	//success
	return true;
}



//--------------------------------------------------------------------
bool CPrinter::PauseBuild(bool bFindSafe, bool bNoSuckback)
//--------------------------------------------------------------------
{
	//pause the fabrication process - pause queue
	//unless bNoSuckback, also move to safe position and suckback

	CString msg1;
	msg1.Format("PauseBuild(%d,%d)",bFindSafe, bNoSuckback);
	Log(msg1,msg1);

	pModel->fab.RequestPause();

	//now queue & motion should be stopped
	//so figure out where to return to
	WaitForStatusUpdate();
	SetLastPos(); //sets current RT position as the last position

	if(!bNoSuckback)
	{
		Suckback();
	}
	if(bFindSafe)
	{
		FindSafePos(GetSafePos());
	}
	return true;
}

//--------------------------------------------------------------------
bool CPrinter::ResumeBuild(bool bToolChange, bool bNoPushout)
//--------------------------------------------------------------------
{
	//resume the fabrication process - move back to last position, pushout, resume queue

	CString msg1;
	msg1.Format("ResumeBuild(%d,%d)",bToolChange, bNoPushout);
	Log(msg1,msg1);

	if(bToolChange)
		GotoNext(); //move to the start of the next path point
	else
		GotoLast(); //move back to the last position
	WaitForMoveDone();

	if(!bNoPushout)
	{
		Pushout();
	}

	//resume the fabrication thread
	pModel->fab.RequestResume();
	return true;
}

//--------------------------------------------------------------------
bool CPrinter::AbortBuild()
//--------------------------------------------------------------------
{
	//emergency stop and abort the build underway
	//this includes flushing the uC queue, and throwing out the
	//paths generated by the process planner

	//tell the fabrication process to abort
	int timeout = 100;
	if(pModel!=NULL)
	{
		if(pModel->fab.IsFabricating())
			pModel->fab.RequestAbort();
	}
	//now stop sending commands 
	m_bQPathDone = true; //this will stop the queueing thread from sending any more segments

	//and empty the path array
	m_pathArray.RemoveAll();
	//zero out the path index counter
	m_nPathIndex = 0;
	//aryLock.Unlock();

	//emergency stop all motion
	//and empty the queue by aborting
	if(bHardwareInitialized && !IsAborted())
	{
		Stop();
		if(!Abort())return false;
		//now clear the abort
		if(IsAborted()) return Abort();
	}
	//success
	return true;
}


//--------------------------------------------------------------------
bool CPrinter::IsDefined(void)
//--------------------------------------------------------------------
{ // return true if printer components are defined

	m_bDefined = (component.GetSize()==4);
	return (m_bDefined);
}

//--------------------------------------------------------------------
bool CPrinter::InitializeHardware(void)
//--------------------------------------------------------------------
{// Initialize hardware comm and start comm thread
	
	//start by opening/refreshing the com port
	if(IsChannelOpen())
	{
		CloseChannel();
	}
	if(!OpenChannel(false)) //try to autofind the port
		if(!OpenChannel(true))	//ask user to select it
		{
			bHardwareInitialized = false; //failed
			return bHardwareInitialized;
		}

	//now try to initialize the microcontroller motion control subsystem
	if(!Reset()) //send a reset packet to the machine
	{
		bHardwareInitialized = false; 
		return bHardwareInitialized;
	}

	//attempt to send configuration data to the microcontroller
	if(!Configure())
	{
		//if configuration fails - probably communication error
		bHardwareInitialized = false; 
		return bHardwareInitialized;
	}

	//now start the status collection thread to collect status info
	if(!CollectStatus(true))
	{	
		bHardwareInitialized = false; 
		return bHardwareInitialized;
	}

	//reset the comms status
	m_dCommsErrorRate = 0.0;
	m_dwCMDStatusPeriod = GetUserStatusPeriod(); 
	m_dwTGTStatusPeriod = GetUserStatusPeriod();
	m_dwACTStatusPeriod = 0;
	m_nPeriodCalcStep = 0;
	m_dwMachineElapsedTime_ms = 0;
	m_ctsIdleTime = 0; 
	m_ctIdleStart = 0;
	m_bEStop = false;
	//we should be ready to go now
	bHardwareInitialized = true;
	
	return bHardwareInitialized;
}

//--------------------------------------------------------------------
bool CPrinter::ShutdownHardware(void)
//--------------------------------------------------------------------
{// terminate status thread, reset micro, close USB channel

	//try to stop the status collection thread
	if(CollectStatus(false))
	{
		//try to reinitialize the microcontroller motion control subsystem
		Reset();
	}
	//no matter what, try to shut down the channel
	if(CloseChannel())
	{
		bHardwareInitialized = false;
		return true;
	}
	return false;
}


//---------------------------------------------------------------------------
bool CPrinter::MountTool(CString &toolName)
//---------------------------------------------------------------------------
{
	//set the named tool as mounted, true if successful
	int idx = CTool::SearchByName(toolName);
	if(idx < 0) return false;
	
	bool mounted = CTool::Mount(toolName);

	//set the last tool mounted as active
	return (mounted && SetCurTool(toolName));
}

//---------------------------------------------------------------------------
bool CPrinter::UnmountTool(CString &toolName)
//---------------------------------------------------------------------------
{
	//set the named tool as unmounted, true if successful
	int idx = CTool::SearchByName(toolName);
	if(idx < 0) return false;
	//if this was the active tool
	if(m_sCurTool.CompareNoCase(toolName) == 0)
	{
		//if there were one or fewer mounted tools
		if(CTool::MountedToolCount() <= 1)
		{
			SetCurTool("Undefined");
			return CTool::Unmount(toolName);
		}
		else
		{
			//set the current tool to the next mounted tool
			CString next = CTool::FindNextMounted(toolName);
			SetCurTool(next);
		}
	}
	//finally unmount the tool
	return CTool::Unmount(toolName);
}

//---------------------------------------------------------------------------
bool CPrinter::ChangePOD()
//---------------------------------------------------------------------------
{
	//change the current point of deposition to compensate for offsets of
	//multiple tools
	//subtract the old POD from the new to get offset
	//store the new POD
	m_vPODOffset = GetCurTool()->tip - m_vPOD;
	m_vPOD = GetCurTool()->tip;
	//now offset the "last" and "origin" positions with the new offset
	m_vLastPos = m_vLastPos-m_vPODOffset;
	m_vOrigin = m_vOrigin-m_vPODOffset;
	CString msg1, msg2;
	msg1.Format("ChangePOD");
	msg2.Format("PODOffs(%0.2f,%0.2f,%0.2f), pod(%0.2f,%0.2f,%0.2f), m_vLastPos(%0.2f,%0.2f,%0.2f), m_vOrigin(%0.2f,%0.2f,%0.2f)",
		m_vPODOffset.x,m_vPODOffset.y,m_vPODOffset.z,
		m_vPOD.x,m_vPOD.y,m_vPOD.z,
		m_vLastPos.x,m_vLastPos.y,m_vLastPos.z,
		m_vOrigin.x,m_vOrigin.y,m_vOrigin.z);
	Log(msg1,msg2);
	return true;
}

//---------------------------------------------------------------------------
bool CPrinter::FindToolToRemove(CString& newToolName, CString& removeTool)
//---------------------------------------------------------------------------
{
	//find name of tool which should be removed to minimize swaps
	//pass back through removeTool; return true if a tool needs to be removed
	if(GetMaxTools() < 2)
	{
		removeTool = m_sCurTool;
		return true;
	}
	//for now, just make sure we don't remove the next tool needed
	//TODO: do proper optimization for arbitrary number of tools
	else
	{
		int newAxis = CTool::GetByName(newToolName)->axis;
		for(int i = 0; i < pApp->tool.GetSize(); i++)
		{
			//if a mounted tool is using the same axis as the new tool, then remove it
			if(pApp->tool[i].IsMounted() && (pApp->tool[i].axis == newAxis))
			{
				removeTool = pApp->tool[i].GetName();
				return true;
			}
		}
	}
	//if no mounted tool is using the same axis, then no tool needs to be removed.
	removeTool = "";
	return false;
}

//---------------------------------------------------------------------------
bool CPrinter::SetCurTool(CString newToolName, CString newMatName)
//---------------------------------------------------------------------------
{
	//change the currently active tool
	//if the tool is already mounted (e.g. multiple syringe tool)
	//then just update variables; else ask user to mount

	//if newMatName not specified, just keep the old one
	if(newMatName.CompareNoCase("")==0)
	{
		newMatName = GetCurMatName();
	}
	//if the tool name is undefined, just change the name and return
	//this should only be temporary; don't change POD yet
	if(newToolName.CompareNoCase("Undefined")==0)
	{
		m_sCurTool = newToolName;
		m_sCurMaterial = newMatName;
		return true;
	}

	EnsureMountedTool(newToolName);

	if((newToolName.CompareNoCase(m_sCurTool) == 0) && 
		(newMatName.CompareNoCase(m_sCurMaterial) == 0) &&
		GetCurTool()->IsMounted()) return true;

	
	//tool is loaded and mounted and not already current
	m_sCurTool = newToolName;
	m_sCurMaterial = newMatName;
	//note that the view contents have changed
	bNewStatus = true;
	//change point of deposition
	return ChangePOD();
}

//---------------------------------------------------------------------------
bool CPrinter::ChangeTool(CString newToolName, CString newMatName)
//---------------------------------------------------------------------------
{
	//if multitool, and new tool is already mounted, 
	//switch automatically if bPauseAtChange;
	//if new tool needs to be manually mounted first,
	//move to safe position, ask user to change the tool/material
	//remain paused until user resumes build

	CString msg;
	//if newMatName not specified, just keep the old one
	if(newMatName.CompareNoCase("")==0)
	{
		newMatName = GetCurMatName();
	}

	if((newToolName.CompareNoCase(m_sCurTool) == 0) && 
		(newMatName.CompareNoCase(m_sCurMaterial) == 0) &&
		GetCurTool()->IsMounted()) return true;

	//check that the new tool and material are present in the arrays
	EnsureLoadedTool(newToolName);

	//TODO: add material capability back in
	/*if( (CMaterial::SearchByName(newMatName) < 0) )
	{
		msg.Format("Cannot find material %s", newMatName);
		((CFabAtHomeApp *)AfxGetApp())->Log(msg);
		return false;
	}*/

	//Grab the current position as the place to return to, and pause the queue
	if(IsFabricating())
	{
		if(!PauseBuild(false,true))
			((CFabAtHomeApp *)AfxGetApp())->Log("PauseBuild failed");

		
		//if new tool isn't already mounted (e.g. multi-syringe tool)
		//of if user wants pause, or configuration is single tool
		if(!CTool::GetByName(newToolName)->IsMounted() || GetPauseAtToolChange())
		{
			//move to safe position and ask user to swap out or remind to resume
			FindSafePos(m_vSafePos);

			if(!CTool::GetByName(newToolName)->IsMounted())
			{
				//if the configuration supports multiple tools, figure out which tool to remove
				//to minimize number of swaps
				CString removeTool;
				if(FindToolToRemove(newToolName, removeTool))
				{
					//need to remove a tool
					msg.Format("Please replace %s with\n%s,then resume build (Fabrication->Resume Printing).",
						removeTool, newToolName);
				}
				else
				{
					//only need to mount a tool
					msg.Format("Please mount %s, then resume build (Fabrication->Resume Printing).",
						newToolName);
				}
				if(AfxMessageBox(msg, MB_OKCANCEL) != IDOK)
				{
					msg.Format("Aborted tool change: aborting build.");
					((CFabAtHomeApp *)AfxGetApp())->Log(msg);
					return(AbortBuild());
				}
				//bring up the tool change dialog
				pApp->UserToolChange();
			}
			else //tool already mounted, but user wanted pause
			{
				((CFabAtHomeApp *)AfxGetApp())->Log("Please hit Fabrication->Resume Printing when ready to continue");
			}
			//now while a pause is requested
			while(pModel->fab.PauseRequested())
				Sleep(100);
		}
		SetCurTool(newToolName, newMatName);
		//now resume the build process
		if(!ResumeBuild(true, true))
		{
			((CFabAtHomeApp *)AfxGetApp())->Log("ResumeBuild failed");
			return false;
		}
		else
			return true;
	}
	//not fabricating, so just change the tool
	return SetCurTool(newToolName, newMatName);
}

//---------------------------------------------------------------------------
bool CPrinter::OpenChannel( bool isManual )
//---------------------------------------------------------------------------
{
	//open up a communications channel to the machine
	//ask user via dialog if isManual, else find automatically
	if( isManual )
	{
		return m_channel.DoDialog();
	}
	else
	{
		return m_channel.AutoFindPort();
	}
}

//---------------------------------------------------------------------------
bool CPrinter::CloseChannel()
//---------------------------------------------------------------------------
{
	//disconnect from the communications channel
	//kill the status thread
	if(!IsChannelOpen()) return bEmulateHardware;
	m_bCollectStatus = false;
	return m_channel.Disconnect();
}

//---------------------------------------------------------------------------
bool CPrinter::Start(void)
//---------------------------------------------------------------------------
{
	//tell the hardware to enable/disable motion
	//note that start toggles

	if(!bHardwareInitialized) return bEmulateHardware;
	CSingleLock aryLock(&m_mutUpdate);
	aryLock.Lock();
	CStartPacket pkt;
	if(!pkt.InitPacket()) return false;
	CString msg;
	int i = 0;
	//send out the Start command; try a few times if necessary
	while( !m_channel.Send(&pkt) && (i < RETRIES) )
	{
		i++;
		Sleep(RETRY_DLY);
	}
	if(i == RETRIES)
	{	
		msg.Format("CPrinter::Start: timed out sending");
		((CFabAtHomeApp *)AfxGetApp())->Log(msg);
		return false;
	}
	m_bEnableRequested = !m_bEnableRequested;
	
	return true;
}

//---------------------------------------------------------------------------
void CPrinter::SetEStop()
//---------------------------------------------------------------------------
{
	//set the EStop flag
	m_bEStop = true;
}

//---------------------------------------------------------------------------
void CPrinter::ClearEStop()
//---------------------------------------------------------------------------
{
	//clear the EStop flag, and any aborts
	if(!bEmulateHardware)
		if(IsAborted())
			Abort();
	m_bEStop = false;
}

//---------------------------------------------------------------------------
bool CPrinter::Stop(void)
//---------------------------------------------------------------------------
{
	//stop motion (terminate current command)
	if(bEmulateHardware || !IsStarted())
	{
		SetEStop();
		return true;
	}

	Abort();
	SetEStop(); //set E stop flag
	return true;
}

//---------------------------------------------------------------------------
bool CPrinter::PauseQueue(void)
//---------------------------------------------------------------------------
{
	//pause at current position in segment queue
	if(!bHardwareInitialized) return bEmulateHardware;

	CPausePacket pkt;
	if(!pkt.InitPacket()) return false;
	CString msg;
	int i = 0;
	//send out the Pause command; try a few times if necessary
	while( !m_channel.Send(&pkt) && (i < RETRIES) )
	{
		i++;
		Sleep(RETRY_DLY);
	}
	if(i == RETRIES)
	{	
		msg.Format("CPrinter::Start: timed out sending");
		((CFabAtHomeApp *)AfxGetApp())->Log(msg);
		return false;
	}
	return true;
}

//---------------------------------------------------------------------------
bool CPrinter::ResumeQueue(void)
//---------------------------------------------------------------------------
{
	//resume at next position in seqment queue
	if(!bHardwareInitialized) return bEmulateHardware;

	CResumePacket pkt;
	if(!pkt.InitPacket()) return false;
	CString msg;
	int i = 0;
	//send out the Resume command; try a few times if necessary
	while( !m_channel.Send(&pkt) && (i < RETRIES) )
	{
		i++;
		Sleep(RETRY_DLY);
	}
	if(i == RETRIES)
	{	
		msg.Format("CPrinter::Start: timed out sending");
		((CFabAtHomeApp *)AfxGetApp())->Log(msg);
		return false;
	}
	return true;
}

//---------------------------------------------------------------------------
bool CPrinter::Abort(void)
//---------------------------------------------------------------------------
{
	//stop all motion, flush the segment queue
	if(!bHardwareInitialized) return bEmulateHardware;

	CAbortPacket pkt;
	if(!pkt.InitPacket()) return false;
	CString msg;
	int i = 0;
	//send out the Abort command; try a few times if necessary
	while( !m_channel.Send(&pkt) && (i < RETRIES) )
	{
		i++;
		Sleep(RETRY_DLY);
	}
	if(i == RETRIES)
	{	
		msg.Format("CPrinter::Start: timed out sending");
		((CFabAtHomeApp *)AfxGetApp())->Log(msg);
		return false;
	}
	return true;
}

//---------------------------------------------------------------------------
bool CPrinter::ClearError(void)
//---------------------------------------------------------------------------
{
	//try to clear error condition
	return false;
}

//---------------------------------------------------------------------------
bool CPrinter::Reset(void)
//---------------------------------------------------------------------------
{
	//hard reset of microcontroller
	if(!IsChannelOpen()) return bEmulateHardware;

	CResetPacket pkt;
	if(!pkt.InitPacket()) return false;
	CString msg;
	int i = 0;
	//send out the Reset command; try a few times if necessary
	while( !m_channel.Send(&pkt) && (i < RETRIES) )
	{
		i++;
		Sleep(RETRY_DLY);
	}
	if(i == RETRIES)
	{	
		msg.Format("CPrinter::Start: timed out sending");
		((CFabAtHomeApp *)AfxGetApp())->Log(msg);
		return false;
	}
	//reset the PC elapsed time counter as well
	m_bFirstPacket = true;
	//variables that needs to be reseted when microcontroller is CenPRA
	m_bSafeSet = false;
	return true;
}

//---------------------------------------------------------------------------
bool CPrinter::HomeHW(void)
//---------------------------------------------------------------------------
{
	//declare current location as home; reset global positions
	if(!IsChannelOpen()) return bEmulateHardware;

	CHomePacket pkt;
	if(!pkt.InitPacket()) return false;
	CString msg;
	int i = 0;
	//send out the Home command; try a few times if necessary
	while( !m_channel.Send(&pkt) && (i < RETRIES) )
	{
		i++;
		Sleep(RETRY_DLY);
	}
	if(i == RETRIES)
	{	
		msg.Format("CPrinter::Start: timed out sending");
		((CFabAtHomeApp *)AfxGetApp())->Log(msg);
		return false;
	}
	return true;
}

//---------------------------------------------------------------------------
bool CPrinter::QueuePath( void )
//---------------------------------------------------------------------------
{
	//handle queuing a single path
	//return the array index of the next segment
	//to be sent - respects the queue capacity on the uC
	//the user should write new path data to m_pathArray
	//set m_nPathIndex to indicate the first segment to send
	//and then set m_bQPathDone to false to start the fn

	//VERY COMPLICATED AND COUPLED: BE CAREFUL WITH THIS

	//if the hardware is not running, then fail
	if(!bHardwareInitialized) return bEmulateHardware;

	//check validity of arguments

	//if the path is already done, then no new path to send
	if(m_bQPathDone) return true;

	//make the m_pathArray thread safe; accessed by status thread and fab thread
	CSingleLock aryLock(&m_mutUpdate);
	aryLock.Lock();
	if((m_nPathIndex < 0) || (m_nPathIndex > m_pathArray.GetSize())) 
	{
		//some index error
		return false; 
	}

	//check the queue space - remember that it may not
	//update while this function is running, so need to calculate
	int qAvail = GetQSpace();
	TRACE("QueuePath: m_nPathIndex %d, pathSize %d, qAvail %d\n", m_nPathIndex, m_pathArray.GetSize(), qAvail);
	qAvail += m_nPathIndex; //indicate relative queue remaining
	CSegmentPacket segPkt;
	int cnt = 0;
	for(int i = m_nPathIndex; (i<m_pathArray.GetSize())&&(i<qAvail); i++)
	{
		if(!segPkt.InitPacket(m_pathArray[i]))
			return false;
		CString msg;
		int timer = 0;
		//send out the request for status; try a few times if necessary
		while( !m_channel.Send(&segPkt) && (timer < RETRIES) )
		{
			timer++;
			Sleep(RETRY_DLY);
		}
		if(timer == RETRIES)
		{	
			msg.Format("CPrinter::QueuePath: timed out sending segment");
			((CFabAtHomeApp *)AfxGetApp())->Log(msg);
			return false;
		}
		//successfully sent a packet, so increment m_nPathIndex
		m_nPathIndex++;
		//get the segment index for completion tracking
		m_dwQueuePathIndex = segPkt.GetIndex();
		if(m_pathArray[i].pIndex != NULL)
			*(m_pathArray[i].pIndex) = m_dwQueuePathIndex;
		//debugging
		cnt++;
		TRACE("QSegment#%d:d(%d,%d,%d,%d),s:%d, L:%d\n",
			m_dwQueuePathIndex,m_pathArray[i].d[0],m_pathArray[i].d[1],m_pathArray[i].d[2],
			m_pathArray[i].d[GetCurTool()->axis],m_pathArray[i].s,m_pathArray[i].L);
	}
	//if we have sent the whole array, then we are done
	m_bQPathDone = (m_nPathIndex == m_pathArray.GetSize());
	TRACE("QueuePath: qAvail %d, sent %d\n",qAvail-m_nPathIndex, cnt);
	return true;
}

//---------------------------------------------------------------------------
bool CPrinter::IMMSegment( CSegment &curSeg, DWORD* pIndex )
//---------------------------------------------------------------------------
{
	//execute a single segment move immediately
	//return the packet index via *index
	if(!bHardwareInitialized) return bEmulateHardware;

	CIMMSegmentPacket segPkt;
	if(!segPkt.InitPacket(curSeg))
		return false;
	CString msg;
	int timer = 0;
	//send out the request for status; try a few times if necessary
	while( !m_channel.Send(&segPkt) && (timer < RETRIES) )
	{
		timer++;
		Sleep(RETRY_DLY);
	}
	if(timer == RETRIES)
	{	
		msg.Format("CPrinter::QueuePath: timed out sending segment");
		((CFabAtHomeApp *)AfxGetApp())->Log(msg);
		return false;
	}
	//if a valid index pointer has been provided, return the index of the command sent
	if(pIndex != NULL)
		*pIndex = segPkt.GetIndex();

	TRACE("IMMSegment#%d:d(%d,%d,%d,%d),s:%d, L:%d\n",
		segPkt.GetIndex(),curSeg.d[0],curSeg.d[1],curSeg.d[2],curSeg.d[GetCurTool()->axis],curSeg.s,curSeg.L);
	return true;
}

//---------------------------------------------------------------------------
double CPrinter::GetActualPathSpeed()
//---------------------------------------------------------------------------
{
	//return the current path tangential speed
	double spd = 0;
	for(int i = 0; i < POSITION_AXES; i++)
	{
		spd+=(component[i+1].GetVel()*component[i+1].GetVel());
	}
	return(sqrt(spd));
}

//---------------------------------------------------------------------------
double CPrinter::GetMaxPathSpeed()
//---------------------------------------------------------------------------
{
	//return the maximum path tangential speed (in XY plane)
	double mmps = min(component[1].mmps,component[2].mmps);
	ASSERT((mmps > 0.0)&&(mmps<1.0));
    return(mmps*MAX_STEP_FREQ);
}

//---------------------------------------------------------------------------
bool CPrinter::SetJogSpeed(double spd)
//---------------------------------------------------------------------------
{
	//set the non-build motion speed
	//return true if below maximum attainable
	if(spd > GetMaxPathSpeed())
	{
		m_dJogSpeed = GetMaxPathSpeed();
		return false;
	}
	m_dJogSpeed = spd;
	return true;
}

//---------------------------------------------------------------------------
DWORD CPrinter::GetPCElapsedTime()
//---------------------------------------------------------------------------
{
	//return the time in ms of the PC clock since the printer was reset
	return (GetTickCount() - m_dwMachineStartTime_ms);
}

//---------------------------------------------------------------------------
DWORD CPrinter::GetCommandedStatusPeriod()
//---------------------------------------------------------------------------
{
	//return the commanded status update rate in milliseconds

	return m_dwCMDStatusPeriod;
}

//---------------------------------------------------------------------------
DWORD CPrinter::GetTargetStatusPeriod()
//---------------------------------------------------------------------------
{
	//return the target status update period in milliseconds
	return m_dwTGTStatusPeriod;
}

//---------------------------------------------------------------------------
void CPrinter::SetTargetStatusPeriod(DWORD msPeriod)
//---------------------------------------------------------------------------
{
	//set the target status update period in milliseconds
	if(msPeriod > MAX_STATUS_UPDATE_PERIOD)
		msPeriod = MAX_STATUS_UPDATE_PERIOD;
	m_dwTGTStatusPeriod = (DWORD)msPeriod;
}

//---------------------------------------------------------------------------
void CPrinter::SetUserStatusPeriod(DWORD msPeriod)
//---------------------------------------------------------------------------
{
	//set the user's requested update period
	if(msPeriod > 0 && msPeriod < MAX_STATUS_UPDATE_PERIOD)
		m_dwUSRStatusPeriod = msPeriod;
	else
		m_dwUSRStatusPeriod = STATUS_UPDATE_PERIOD;
}

//---------------------------------------------------------------------------
bool CPrinter::CalcStatusPeriod(DWORD deltaT)
//---------------------------------------------------------------------------
{
	//calculate the actual status update period being achieved in milliseconds
	//use windowed average, with window size m_nStatusSamples

	//while calc_step is less than m_nStatusSamples, just accumulate
	//deltaT fractions; after m_nStatusSamples steps, remove the oldest value

	int i;

	if(deltaT <= 0) return false;

	//calculate the average weighted deltaT increment
	double dTinc = deltaT/m_nStatusSamples;
	
	if(m_nPeriodCalcStep < m_nStatusSamples)
	{
		//track the old deltaT increments
		m_arStatusPeriod.Add(dTinc);
		//track how many ticks have accumulated
		m_nPeriodCalcStep++;
	}
	else
	{
		//after accumulating m_nStatusSamples increments
		//start removing oldest increment and shift the array down
		for(i = 0; i < m_nStatusSamples-1; i++)
		{
			m_arStatusPeriod[i] = m_arStatusPeriod[i+1];
		}
		m_arStatusPeriod[m_nStatusSamples-1] = dTinc;
	}
	//finally calculate and store the average period
	double tmp = 0.0;
	for(i = 0; i < m_nPeriodCalcStep; i++)
	{
		tmp+=m_arStatusPeriod[i];
	}
	if(tmp > 0) m_dwACTStatusPeriod = (DWORD)tmp;
	return true;
}

//---------------------------------------------------------------------------
DWORD CPrinter::GetActualStatusPeriod()
//---------------------------------------------------------------------------
{
	//return the rolling average value of the status update period
	return m_dwACTStatusPeriod;
}

//---------------------------------------------------------------------------
bool CPrinter::GetStatus(void)
//---------------------------------------------------------------------------
{
	//fetch the status information from the machine
	//reinit the packets to increment their indices

	//lock the update mutex to ensure that state update is atomic
	if(!bHardwareInitialized) return bEmulateHardware;

	CSingleLock updateLock(&m_mutUpdate);
	if(!updateLock.Lock((DWORD)(2*GetTargetStatusPeriod())))
	{
		//have lock timeout
		return false;
	}

	if(!m_statusPkt.InitPacket()) return false;
	if(!m_statusAckPkt.InitPacket(m_BFirmwareVersion)) return false;

	CString msg;
	bool result = false;
	//send out the request for status, and receive the response
	//use a tight timeout here to quickly indicate bad connection
	result = m_channel.SendReceive(&m_statusPkt, &m_statusAckPkt,(DWORD)(2*GetTargetStatusPeriod()));
	if(!result)
	{
		msg.Format("GetStatus failed");
		((CFabAtHomeApp *)AfxGetApp())->Log(msg);
		return false;
	}

	//now extract the status info from the packet
	m_nQSpace = m_statusAckPkt.GetQSpace();
	for(int i = 0; i < AXES_CNT; i++)
	{
		m_arlGlobalPos[i] = m_statusAckPkt.GetGlobalPos(i);
		//m_arlEncoder[i] = m_statusAckPkt.GetEncoderRdg(i);
		m_arbLimitState[2*i] = (m_statusAckPkt.GetLimitSwitches() & (1 << (2*i)))>0;
		m_arbLimitState[(2*i)+1] = (m_statusAckPkt.GetLimitSwitches() & (1 << ((2*i)+1)))>0;
	}
	m_stateFlags = m_statusAckPkt.GetState(); 
	m_dwCurCommand = m_statusAckPkt.GetIdxCurCommand();
	m_dwLastCmdExecuted = m_statusAckPkt.GetIdxLastExecuted();

	//calculate the elapsed time for the last update cycle
	//and update the running average
	DWORD priorT = m_dwMachineElapsedTime_ms;
	m_dwMachineElapsedTime_ms = m_statusAckPkt.GetElapsedTime_ms();
	CalcStatusPeriod(m_dwMachineElapsedTime_ms-priorT); 
	//if this is the first packet received, store the PC tick count
	//to verify that micro timer interrupt is running correctly
	if(m_bFirstPacket)
	{
		//now this can be subtracted from the tick count to get PC elapsed time
		m_dwMachineStartTime_ms = GetTickCount()+m_dwMachineElapsedTime_ms;
		m_bFirstPacket = false;
	}
	//tell the graphics where to be
	//read axis positions from the component specifications
	//moveable components start at index = 1 (index 0 is the base)
	//TODO: have firmware handle axes directions via configuration packet
	//for now we are fixing the directions 

	CVec X;
	X.x = m_arlGlobalPos[component[1].axis]*component[1].mmps;
	X.y = m_arlGlobalPos[component[2].axis]*component[2].mmps;
	X.z = m_arlGlobalPos[component[3].axis]*component[3].mmps;
	component[1].SetRTPos(X.x*component[1].axisDir);
	component[2].SetRTPos(X.y*component[2].axisDir);
	component[3].SetRTPos(X.z*component[3].axisDir);

	//check for motion by comparing current positions with last

	// rest of counters are for tools
	int nt = pApp->tool.GetSize();
	bool moving = false;
	for (int i=0; i<nt; i++) 
	{
		double laststep = pApp->tool[i].curstep;
		pApp->tool[i].curstep = m_arlGlobalPos[pApp->tool[i].axis];
		if(laststep != pApp->tool[i].curstep)
		{
			X.x = component[1].GetRTPos();
			X.y = component[2].GetRTPos();
			X.z = component[3].GetRTPos();
			if(pModel != NULL) pModel->TracePoint(ModelToRobotTfm(X-m_vOrigin));
			moving = true;
		}
	}
	m_bToolMoving = moving;

	//set flag to indicate updated status info
	bNewStatus = true;
	
	//if logging is on, log the command
	CString fun;
	fun.Format("GetStatus");
	Log(fun,fun);
	return true;
}

//---------------------------------------------------------------------------
void CPrinter::ManageAmpDisable()
//---------------------------------------------------------------------------
{
	//manage the automatic disabling of amplifiers during idle time
	if(!bHardwareInitialized) return;

	if(!IsStarted() || IsMoving() || IsToolMoving())
	{
		m_ctIdleStart = CTime::GetCurrentTime();
		m_ctsIdleTime = 0;
		return;
	}
	m_ctsIdleTime = CTime::GetCurrentTime()-m_ctIdleStart;
	if(m_ctsIdleTime > CTimeSpan(AMP_IDLE_DISABLE_TIME))
		if(IsStarted()) Start(); //disable the amps
}

//---------------------------------------------------------------------------
bool CPrinter::ManageComms(bool bError)
//---------------------------------------------------------------------------
{
	//manage the comms workload and check for error conditions
	//set bError true if a malfunction has occurred
	//return false only if fatal condition
	if(!bHardwareInitialized) return bEmulateHardware;

	CSingleLock updateLock(&m_mutUpdate);
	updateLock.Lock(10000); //give up after 10s
	if(!updateLock.IsLocked()) return false; //fatal error

	DWORD targetPeriod = GetTargetStatusPeriod();
	double err = ((double)targetPeriod-(double)GetActualStatusPeriod());

	//if we experience very large error, consider it a comms fault
	if(abs(err)*2.0 > targetPeriod) bError = true;

	if(bError)
		m_dCommsErrorRate += 1.0/COMMS_AVE_WINDOW;
	m_dCommsErrorRate -= m_dCommsErrorRate/COMMS_AVE_WINDOW;
	if(m_dCommsErrorRate > 1.0) m_dCommsErrorRate = 1.0;
	if(m_dCommsErrorRate < 0.0) m_dCommsErrorRate = 0.0;

	if(m_dCommsErrorRate >= COMMS_FATAL_THRESH) return false; //declare fatal condition

	//now makes changes to commanded update period, slowly
	if((GetLastCmdSent()/10)%10 == 9) //every 100 packets
	{
		if(m_dCommsErrorRate >= COMMS_DERATE_THRESH)
		{
			//derate the target period by 10%
			SetTargetStatusPeriod((DWORD)min(1.1*GetTargetStatusPeriod(),MAX_STATUS_UPDATE_PERIOD));
		}
		if(m_dCommsErrorRate <= COMMS_RERATE_THRESH)
		{
			//rerate (decrease) the target period by 5%
			SetTargetStatusPeriod((DWORD)max(0.95*GetTargetStatusPeriod(),GetUserStatusPeriod()));
		}
		//update the rate with a low gain
		double cmd = m_dwCMDStatusPeriod;
		cmd += err/m_dwCMDStatusPeriod*COMMS_P_GAIN;
		if((cmd >= GetUserStatusPeriod()) && (cmd < MAX_STATUS_UPDATE_PERIOD))
			m_dwCMDStatusPeriod = (DWORD)cmd;
	}
	TRACE("CMDPeriod: %d, TGTPeriod: %d, ACTPeriod: %d, err %f\n", m_dwCMDStatusPeriod, GetTargetStatusPeriod(), GetActualStatusPeriod(), err);

	return true;
}

//---------------------------------------------------------------------------
void CPrinter::NetworkConnect(bool bConnect)
//---------------------------------------------------------------------------
{
	if(!bConnect) //force disconnection
	{
		m_sStatusBarMessage.Format("Disconnecting...");
		if(p_connEctor->isConnected())
			p_connEctor->disconnect();
		else 
			p_connListener->cancel();
		//m_connListener.disconnect();
	}
}

//---------------------------------------------------------------------------
void CPrinter::StartNetworkThread()
//---------------------------------------------------------------------------
{
	AfxBeginThread(NetworkThreadProc,this);
	m_bNetworkConnect = true;
}


//---------------------------------------------------------------------------
UINT CPrinter::NetworkThreadProc( LPVOID pParam )
//---------------------------------------------------------------------------
{
	//worker function for external process communications
	CPrinter *pMachine = (CPrinter *)pParam;
	AfxSocketInit();
	pMachine->p_connEctor = new CConn();
	pMachine->p_connListener = new CConn();
	if(pMachine->p_connEctor == NULL || pMachine->p_connListener == NULL)
	{
		pMachine->m_sStatusBarMessage.Format("Unable to make network sockets");
		return 1;
	}
	CString msg;
	bool commOK = true;
	if(pMachine->p_connListener->listen())
	{
		pMachine->m_sStatusBarMessage.Format("Listening on network...");
		pMachine->m_bNetworkConnect = true;
		if(pMachine->p_connListener->accept(*(pMachine->p_connEctor)))
		{
			pMachine->m_sStatusBarMessage.Format("Connected on Network");
			commOK = pMachine->p_connEctor->sendString("Hello World\n");
			commOK = pMachine->p_connEctor->flush();
			while(commOK && pMachine->m_bNetworkConnect)
			{
				commOK = pMachine->p_connEctor->readString(msg);
				pMachine->m_sStatusBarMessage=msg;
				Sleep(100);
			}
			pMachine->p_connEctor->disconnect();
			pMachine->p_connListener->disconnect();
		}
	}
	if(pMachine->p_connEctor != NULL) delete pMachine->p_connEctor;
	if(pMachine->p_connListener != NULL) delete pMachine->p_connListener;

	pMachine->m_sStatusBarMessage.Format("Network Connection Closed");
	pMachine->m_bNetworkConnect = false;
	return 0;
}

//---------------------------------------------------------------------------
UINT CPrinter::StatusThreadProc( LPVOID pParam )
//---------------------------------------------------------------------------
{
	//worker function for fetching status data from Fab@Home
	bool statusOK = false;
	bool queueOK = false;
	bool commsOK = true;
	int retries = 20;
	CString msg;
	CPrinter *pMachine = (CPrinter *)pParam;
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	
	int counter = 0;
	do
	{
		counter++; //track status fetches
		if(pMachine->bEmulateHardware) //if emulating, do nothing
		{
			//TODO: fetch status from simulated hardware
			statusOK = true;
			queueOK = true;
			commsOK = true;
		}
		else
		{
			//first fetch the status
			statusOK = pMachine->GetStatus();

			//now try to queue some path segments
			// 04-11-2008: Support for pause buiding if U- or V- limit switch is active
			// The printing process is only pause if the limit switch of the active tool is active
			if(statusOK)
			{
				queueOK = pMachine->QueuePath();
				if(pMachine->IsFabricating() && 
					((!pMachine->GetLimit(AXIS_U,false) && pMachine->GetCurTool()->axis == AXIS_U) || 
					(!pMachine->GetLimit(AXIS_V,false) && pMachine->GetCurTool()->axis == AXIS_V)))

					pMachine->pModel->fab.RequestPause();
			}
		}

		//now sleep for the status period
		Sleep(pMachine->GetCommandedStatusPeriod());

		//manage amplifier auto-disable
		pMachine->ManageAmpDisable();

		//manage the comms workload and check for fatal malfunctions
		if(pMachine->GetFirmwareVersion() > 2)
			commsOK = true; 
		else
			pMachine->ManageComms(!(statusOK && queueOK));

		pMachine->UpdateStatusBarMessage();
		if(counter % VIEW_REFRESH_MULT == 0)
			AfxGetMainWnd()->Invalidate();

	} while( (statusOK || queueOK) && commsOK && pMachine->m_bCollectStatus );
	//if both fail at same time, then probably something wrong
	//so kill the thread
	if(statusOK && queueOK && commsOK && !pMachine->m_bCollectStatus)
	{
		AfxEndThread(0); //success
		return 0;  //successful completion
	}
	else
	{
		//tell the base class we are no longer running
		pMachine->m_bCollectStatus = false;
		//tell user
		CString qMsg, sMsg, cMsg;
		if(statusOK)
			sMsg.Format("GetStatus: OK");
		else
			sMsg.Format("GetStatus: Fail");
		if(queueOK)
			qMsg.Format("QueuePath: OK");
		else
			qMsg.Format("QueuePath: Fail");
		if(commsOK)
			cMsg.Format("Comms: OK");
		else
			cMsg.Format("Comms: Fail");

		//probable cause is loss of connection, abort and try to disconnect
		pMachine->AbortBuild();
		pMachine->ShutdownHardware();
		pMachine->bNewStatus = true;
		msg.Format("Connection to hardware lost! Please disconnect and reconnect the USB cable, and try a lower STATUS_UPDATE_PERIOD (%s, %s, %s)",
			sMsg, qMsg, cMsg);
		pApp->Log(msg);
		AfxEndThread(1); //failed
		return 1;
	}
}

//---------------------------------------------------------------------------
bool CPrinter::CollectStatus( bool start )
//---------------------------------------------------------------------------
{
	//start/stop a worker thread to collect status info
	//also handles sending queued motion commands
	if(start && !m_bCollectStatus)
	{
		//this is an important thread - run at high priority
		m_pStatusThread = AfxBeginThread(StatusThreadProc, this, THREAD_PRIORITY_TIME_CRITICAL);
		m_bCollectStatus = (m_pStatusThread != NULL);
		return m_bCollectStatus;
	}
	else
	{
		//tell status thread to terminate
		m_bCollectStatus = false;
		//wait for the thread to die
		if(m_pStatusThread == NULL) //CWinThread already destroyed
		{
			return true;
		}
		else
		{	
			switch( WaitForSingleObject(m_pStatusThread->m_hThread, 10* m_dwTGTStatusPeriod) )
			{
			case WAIT_OBJECT_0:
				return true;
			case WAIT_TIMEOUT:
				return false;
			default:
				return false;
			}
		}
	}
	return true;
}

//---------------------------------------------------------------------------
bool CPrinter::Configure(void)
//---------------------------------------------------------------------------
{
	//send config info to the machine 
	//right now, just which limit switches are used
	if(!IsChannelOpen()) return bEmulateHardware;

	CConfigurePacket pkt;
	CConfigAckPacket ackPkt;
	//fill in the packet with the limit switch configuration
	if(!pkt.InitPacket(m_arBLimitSwitch)) return false;
	//prep the configuration acknowledgement packet to receive ack
	if(!ackPkt.InitPacket()) return false;

	//now do atomic send/receive to prevent goofy behavior
	if(!m_channel.SendReceive(&pkt, &ackPkt)) return false;

	//now unpack the packet
	m_BFirmwareVersion = ackPkt.GetFWVersion();

	//confirm that the limit switch configuration was successful
	int mismatch = 0;
	for(int i = 0; i < (2*AXES_CNT); i++)
	{
		if(m_arBLimitSwitch[i]!=((ackPkt.GetLimitConfig().data.W & (1 << i)) >> i))
			mismatch++;
	}

	CString msg;
	if( mismatch > 0 )
	{
		msg.Format("Firmware configuration of &d limit switches failed!", mismatch);
		((CFabAtHomeApp *)AfxGetApp())->Log(msg);
		return false;
	}

	//check that the firmware version is compatible
	if(m_BFirmwareVersion < NEEDS_FW_VERSION)
	{
		msg.Format("This Fab@Home requires firmware version %d! Behavior will be unpredictable.", NEEDS_FW_VERSION);
		((CFabAtHomeApp *)AfxGetApp())->Log(msg);
	}

	return true;
}

//---------------------------------------------------------------------------
long CPrinter::GetGlobalPos( BYTE axis )
//---------------------------------------------------------------------------
{
	//return the global position for the specified axis
	//in steps from the home position
	return m_arlGlobalPos[axis];
}

//---------------------------------------------------------------------------
bool CPrinter::LimitsActive()
//---------------------------------------------------------------------------
{
	//true if any limits are active
	for(BYTE i = 0; i < AXES_CNT; i++ )
	{
		if(GetLimit(i,true)) return true;
		if(GetLimit(i,false)) return true;
	}
	return false;
}

//---------------------------------------------------------------------------
bool CPrinter::GetLimit( BYTE axis, bool fwd )
//---------------------------------------------------------------------------
{
	//test limit switch active for specified axis and direction
	//limits stored as Xfwd, Xbkd, Yfwd,...
	//TODO: unscramble the limit and motion directions!
	if(fwd)
		return m_arbLimitState[2*axis]; 
	else
		return m_arbLimitState[(2*axis)+1];
}

//--------------------------------------------------------------------
WORD CPrinter::GetQSpace()
//--------------------------------------------------------------------
{
	return m_nQSpace;
}

//--------------------------------------------------------------------
bool CPrinter::IsStarted()
//--------------------------------------------------------------------
{
	return m_stateFlags.STARTED;
}

//--------------------------------------------------------------------
bool CPrinter::IsPaused()
//--------------------------------------------------------------------
{
	return m_stateFlags.PAUSED;
}

//--------------------------------------------------------------------
bool CPrinter::IsAborted()
//--------------------------------------------------------------------
{
	return m_stateFlags.ABORTED;};

//--------------------------------------------------------------------
bool CPrinter::IsOriginSet()
//--------------------------------------------------------------------
{
	return m_bOriginSet;
}

//--------------------------------------------------------------------
bool CPrinter::IsSafeSet()
//--------------------------------------------------------------------
{
	return m_bSafeSet;
}

//--------------------------------------------------------------------
bool CPrinter::IsPathDone()
//--------------------------------------------------------------------
{
	//true when path array is empty
	return m_bQPathDone;
} 

//--------------------------------------------------------------------
bool CPrinter::IsError()
//--------------------------------------------------------------------
{
	return m_stateFlags.MALF;
}

//--------------------------------------------------------------------
bool CPrinter::IsQEmpty()
//--------------------------------------------------------------------
{
	return m_stateFlags.QEMPTY;
}

//--------------------------------------------------------------------
bool CPrinter::IsConfigured()
//--------------------------------------------------------------------
{
	//microcontroller only returns true if configuration was successful
	return m_stateFlags.CONFIGURED;
}  

//--------------------------------------------------------------------
bool CPrinter::IsCollecting()
//--------------------------------------------------------------------
{
	//return true if the worker thread is collecting status

	return m_bCollectStatus;
}

//--------------------------------------------------------------------
DWORD CPrinter::GetLastCmdSent()
//--------------------------------------------------------------------
{
	//get the index of the last command sent over the channel
	CSingleLock updateLock(&m_mutUpdate);
	updateLock.Lock();
	return m_channel.GetLastSentIdx();
}	

//--------------------------------------------------------------------
DWORD CPrinter::GetLastCmdQueued()
//--------------------------------------------------------------------
{
	//get the index of the last command queued to the microcontroller
	return m_dwQueuePathIndex;
}	

//--------------------------------------------------------------------
DWORD CPrinter::GetLastCmdRecd()
//--------------------------------------------------------------------
{
	//get the index of the last command recd over the channel

	CSingleLock updateLock(&m_mutUpdate);
	updateLock.Lock();
	return m_channel.GetLastRecdIdx();

}
//--------------------------------------------------------------------
DWORD CPrinter::GetLastCmdExecuted()
//--------------------------------------------------------------------
{
	//get the index of the last command executed by the Fab@Home
	return m_dwLastCmdExecuted;
} 

//--------------------------------------------------------------------
DWORD CPrinter::GetCurCommand()
//--------------------------------------------------------------------
{
	//get the index of the command currently being executed by the Fab@Home
	return m_dwCurCommand;
} 

//--------------------------------------------------------------------
DWORD CPrinter::GetMachineElapsedTime_ms()
//--------------------------------------------------------------------
{
	return m_dwMachineElapsedTime_ms;
}

//--------------------------------------------------------------------
bool CPrinter::StatusOK()
//--------------------------------------------------------------------
{
	//return false if any abnormal condition (e.g. EStop, abort, etc)
	if(IsEStop()) return false;
	if(IsAborted()) return false;
	return true;
}

//--------------------------------------------------------------------
void CPrinter::UpdateStatusBarMessage()
//--------------------------------------------------------------------
{
	//look at system state and post a message accordingly
	if(IsEStop())
		m_sStatusBarMessage.Format("!!!!!EMERGENCY STOP!!!!!");
	else if(IsAborted())
		m_sStatusBarMessage.Format("!!!!!HARDWARE ABORT!!!!!");
	else if(bEmulateHardware)
		m_sStatusBarMessage.Format("Hardware Emulation Mode");
	else if(bHardwareInitialized)
		m_sStatusBarMessage.Format("Connected to Hardware");
	else
		m_sStatusBarMessage.Format("Unknown Status");
}

//--------------------------------------------------------------------
//CString CPrinter::GetStatusText(void)
////--------------------------------------------------------------------
//{	// Describe HW status info
//
//	if (!IsDefined()) 
//		return CString("Printer not defined. Use 'Load Configuration'.");
//
//
//	static CString msg;
//	msg = "";
//	if (this->bHardwareInitialized)
//	{
//		msg.Append("------FIRMWARE/COMMUNICATIONS--------\n");
//		msg.AppendFormat("Firmware Version: %d\n", GetFirmwareVersion());
//		msg.AppendFormat("Elapsed Time: %0.1fs (PC), %0.1fs (uC)\n", 
//			GetPCElapsedTime()/1000.0, GetMachineElapsedTime_ms()/1000.0);
//		msg.AppendFormat("COM Port Handle: %d\n", GetPortHandle());
//		msg.AppendFormat("Update: %0.1fHz (TGT), %0.1fHz (AVE(%d))\n",
//			1000.0/GetTargetStatusPeriod(), 1000.0/GetActualStatusPeriod(), m_nStatusSamples);
//		msg.AppendFormat("Comms error rate: %0.1f%% (AVE(%d))\n", GetCommsErrorRate()*100, COMMS_AVE_WINDOW); 
//		msg.Append("----------------COMMANDS---------------------\n");
//		msg.AppendFormat("Last Sent: %6d  Last Recd: %6d\n", GetLastCmdSent(), GetLastCmdSent());
//		msg.AppendFormat("Running: %6d  Last Executed: %6d\n",
//			GetCurCommand(), GetLastCmdExecuted());
//		msg.Append("-----------------MOTION---------------------\n");
//		msg.AppendFormat("Hardware Moving: %d\n", IsMoving());
//		msg.AppendFormat("Carriage Axes:\n");
//		msg.AppendFormat("X:%.2f%+.2f mm, %.2f mm/s\n", 
//			component[1].GetRTPos(),component[1].GetErrorOffset(),component[1].GetVel());
//		msg.AppendFormat("Y:%.2f%+.2f mm, %.2f mm/s\n",
//			component[2].GetRTPos(),component[2].GetErrorOffset(),component[2].GetVel());
//		msg.AppendFormat("Z:%.2f%+.2f mm, %.2f mm/s\n",
//			component[3].GetRTPos(),component[3].GetErrorOffset(),component[3].GetVel());
//		msg.AppendFormat("Tangential Speed: %5.2f mm/s\n", GetActualPathSpeed());
//		msg.AppendFormat("Tool Axes:\nU:%d step, V:%d step, W:%d step\n",
//				GetGlobalPos(AXIS_U),GetGlobalPos(AXIS_V),GetGlobalPos(AXIS_W));
//		msg.Append("--------------LIMIT SWITCHES-----------------\n0 means active, 1 means inactive\n");
//		msg.AppendFormat("X-=%d, X+=%d\nY-=%d, Y+=%d\nZ-=%d, Z+=%d\nU-=%d\nV-=%d\n", 
//			GetLimit(AXIS_X,true),GetLimit(AXIS_X,false),
//			GetLimit(AXIS_Y,true),GetLimit(AXIS_Y,false),
//			GetLimit(AXIS_Z,false),GetLimit(AXIS_Z,true),
//			GetLimit(AXIS_U,false),
//			GetLimit(AXIS_V,false));
//		msg.Append("-------------------STATUS-----------------------\n");
//		msg.AppendFormat("Aborted: %d  Homed: %d\n", IsAborted(), IsHomed());
//		msg.AppendFormat("Enabled: %d  Paused: %d\n", IsStarted(), IsPaused());
//		msg.AppendFormat("Configd: %d  Error: %d\n", IsConfigured(), IsError());
//		if(IsEStop()) msg.AppendFormat("!!!!!!!!!EMERGENCY STOP!!!!!!!!!\n");
//		if(IsStarted() && !IsMoving()) 
//			msg.AppendFormat("Amplifier idle auto-disable in: %s\n", 
//			(CTimeSpan(AMP_IDLE_DISABLE_TIME)-m_ctsIdleTime).Format("%H:%M.%S"));
//		msg.Append("--------------MOTION QUEUE-------------------\n");
//		msg.AppendFormat("Queue Empty: %d  Queue Space: %d\n", IsQEmpty(), GetQSpace());
//	}
//	else
//	{
//		msg.Format("Hardware Offline\n");
//	}
//
//	return msg;
//}

//--------------------------------------------------------------------
CString CPrinter::GetStatusText(void)
//--------------------------------------------------------------------
{	// Describe HW status info

	if (!IsDefined()) 
		return CString("Printer not defined. Use 'Load Configuration'.");


	static CString msg;
	msg = "";
	if (bEmulateHardware)
	{
		msg.Format("-----------EMULATION MODE------------\n");
		bNewStatus = true;
	}
	msg.Append("------FIRMWARE/COMMUNICATIONS--------\n");
	msg.AppendFormat("Firmware Version: %d\n", GetFirmwareVersion());
	msg.AppendFormat("Elapsed Time: %0.1fs (PC), %0.1fs (uC)\n", 
		GetPCElapsedTime()/1000.0, GetMachineElapsedTime_ms()/1000.0);
	msg.AppendFormat("COM Port Handle: %d\n", GetPortHandle());
	msg.AppendFormat("Update: %0.1fHz (TGT), %0.1fHz (AVE(%d))\n",
		1000.0/GetTargetStatusPeriod(), 1000.0/GetActualStatusPeriod(), m_nStatusSamples);
	msg.AppendFormat("Comms error rate: %0.1f%% (AVE(%d))\n", GetCommsErrorRate()*100, COMMS_AVE_WINDOW); 
	msg.Append("----------------COMMANDS---------------------\n");
	msg.AppendFormat("Last Sent: %6d  Last Recd: %6d\n", GetLastCmdSent(), GetLastCmdSent());
	msg.AppendFormat("Running: %6d  Last Executed: %6d\n",
		GetCurCommand(), GetLastCmdExecuted());
	msg.Append("-----------------MOTION---------------------\n");
	msg.AppendFormat("Hardware Moving: %d\n", IsMoving() || IsToolMoving());
	msg.AppendFormat("Hardware InPosition: %d\n", InPosition());
	msg.AppendFormat("Carriage Axes:\n");
	if(true)/*m_BFirmwareVersion > 3)
	{
		msg.AppendFormat("X:%.2f%+.2f mm, ENC:%d, %.2f mm/s\n", 
			component[1].GetRTPos(),component[1].GetErrorOffset(),
			m_arlEncoder[0], component[1].GetVel());
		msg.AppendFormat("Y:%.2f%+.2f mm, ENC:%d, %.2f mm/s\n",
			component[2].GetRTPos(),component[2].GetErrorOffset(),
			m_arlEncoder[1], component[2].GetVel());
		msg.AppendFormat("Z:%.2f%+.2f mm, ENC:%d, %.2f mm/s\n",
			component[3].GetRTPos(),component[3].GetErrorOffset(),
			m_arlEncoder[2], component[3].GetVel());
	} else*/
	{
		msg.AppendFormat("X:%.2f%+.2f mm, %.2f mm/s\n", 
			component[1].GetRTPos(),component[1].GetErrorOffset(),
			component[1].GetVel());
		msg.AppendFormat("Y:%.2f%+.2f mm, %.2f mm/s\n",
			component[2].GetRTPos(),component[2].GetErrorOffset(),
			component[2].GetVel());
		msg.AppendFormat("Z:%.2f%+.2f mm, %.2f mm/s\n",
			component[3].GetRTPos(),component[3].GetErrorOffset(),
			component[3].GetVel());
	}
	msg.AppendFormat("Tangential Speed: %5.2f mm/s\n", GetActualPathSpeed());
	msg.AppendFormat("Pos Axes:\nX:%d step, Y:%d step, Z:%d step\n",
			GetGlobalPos(AXIS_X),GetGlobalPos(AXIS_Y),GetGlobalPos(AXIS_Z));
	msg.AppendFormat("Tool Axes:\nU:%d step, V:%d step, W:%d step\n",
			GetGlobalPos(AXIS_U),GetGlobalPos(AXIS_V),GetGlobalPos(AXIS_W));
	msg.Append("--------------LIMIT SWITCHES-----------------\n0 means active, 1 means inactive\n");
	msg.AppendFormat("X-=%d, X+=%d\nY-=%d, Y+=%d\nZ-=%d, Z+=%d\nU-=%d\nV-=%d\n", 
		GetLimit(AXIS_X,true),GetLimit(AXIS_X,false),
		GetLimit(AXIS_Y,true),GetLimit(AXIS_Y,false),
		GetLimit(AXIS_Z,false),GetLimit(AXIS_Z,true),
		GetLimit(AXIS_U,false),
		GetLimit(AXIS_V,false));
	msg.Append("-------------------STATUS-----------------------\n");
	msg.AppendFormat("Aborted: %d  Homed: %d\n", IsAborted(), IsHomed());
	msg.AppendFormat("Enabled: %d  Paused: %d\n", IsStarted(), IsPaused());
	msg.AppendFormat("Configd: %d  Error: %d\n", IsConfigured(), IsError());
	if(IsEStop()) msg.AppendFormat("!!!!!!!!!EMERGENCY STOP!!!!!!!!!\n");
	if(IsStarted() && !IsMoving() && !IsToolMoving()) 
		msg.AppendFormat("Amplifier idle auto-disable in: %s\n", 
		(CTimeSpan(AMP_IDLE_DISABLE_TIME)-m_ctsIdleTime).Format("%H:%M.%S"));
	msg.Append("--------------MOTION QUEUE-------------------\n");
	msg.AppendFormat("Queue Empty: %d  Queue Space: %d\n", IsQEmpty(), GetQSpace());
	return msg;
}

//--------------------------------------------------------------------