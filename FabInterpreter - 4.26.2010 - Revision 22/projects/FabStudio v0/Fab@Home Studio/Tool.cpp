f/*License Notification
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

// Tool.cpp: implementation of the CTool class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Tool.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
CTool::CTool()
//---------------------------------------------------------------------------
{

	SetDefaults();
}

//---------------------------------------------------------------------------
CTool::~CTool()
//---------------------------------------------------------------------------
{

}

//---------------------------------------------------------------------------
CTool::CTool(CTool& s)
//---------------------------------------------------------------------------
{
	*this = s;
}

//---------------------------------------------------------------------------
CTool& CTool::operator=(const CTool& s)
//---------------------------------------------------------------------------
{// copy tool object

	m_bIsMounted = s.m_bIsMounted;
	name = s.name;
	description = s.description;
	maxvolume = s.maxvolume;
	currentvolume = s.currentvolume;
	pathwidth = s.pathwidth;
	pathheight = s.pathheight;
	backfill = s.backfill;
	tip = s.tip;
	tooloffset = s.tooloffset;
	barreloffset = s.barreloffset;
	tipoffset = s.tipoffset;
	plungeroffset = s.plungeroffset;
	toolgeom = s.toolgeom;
	plungergeom = s.plungergeom;
	barrelgeom = s.barrelgeom;
	tipgeom = s.tipgeom;
	toolcolor = s.toolcolor;
	plungercolor = s.plungercolor;
	barrelcolor = s.barrelcolor;
	tipcolor = s.tipcolor;
	toolalpha = s.toolalpha;
	plungeralpha = s.plungeralpha;
	tipalpha = s.tipalpha;
	barrelalpha = s.barrelalpha;
	disprate = s.disprate;
	dir = s.dir;
	mmps = s.mmps;
	axis = s.axis;
	axisDir = s.axisDir;
	curstep=s.curstep;
	cmdstep=s.cmdstep;
	rangemin=s.rangemin;
	rangemax=s.rangemax;
	stepstomove=s.stepstomove;
	bottompos=s.bottompos;
	unloadpos=s.unloadpos;
	pushout=s.pushout;
	suckback = s.suckback;
	suckbackDelay = s.suckbackDelay;
	//posLimitSwitch = s.posLimitSwitch;
	//negLimitSwitch = s.negLimitSwitch;
	jogIncrement = s.jogIncrement;
	clearance = s.clearance;
	pathspeed = s.pathspeed;
	jogspeed = s.jogspeed;
	m_dTipID = s.m_dTipID;
	m_dBarrelID = s.m_dBarrelID;
	m_dVolNeeded = s.m_dVolNeeded;
	m_nPausePathCount = s.m_nPausePathCount;
	m_dMinPathLength = s.m_dMinPathLength;
	return *this;
}


//---------------------------------------------------------------------------
CString CTool::GetDescription()
//---------------------------------------------------------------------------
{ // Get textual description of tool (of parameters user might care about)

	CString desc, line;

	line.Format("Description:\t%s\n", description);
	desc += line;

	line.Format("Path width:\t%.3fmm\n", pathwidth);
	desc += line;

	line.Format("Layer Thickness:\t%.3fmm\n", pathheight);
	desc += line;

	line.Format("Path speed:\t%.3fmm/s\n", pathspeed);
	return desc;
}

//---------------------------------------------------------------------------
bool CTool::RequestPause(int pathCnt)
//---------------------------------------------------------------------------
{
	//return true if auto pause is desirable
	if(m_nPausePathCount <= 0) return false;
	return (pathCnt > m_nPausePathCount);
}

//---------------------------------------------------------------------------
void CTool::SetPausePathCount(int count)
//---------------------------------------------------------------------------
{
	//number of paths after which an automatic pause will be triggered
	if(count > 1)
		m_nPausePathCount = count;
	else
		m_nPausePathCount = 0; //zero means never auto pause
}

//---------------------------------------------------------------------------
double CTool::GetPathspeed()
//---------------------------------------------------------------------------
{
	//get the current commanded build path tangential speed for this tool
	return pathspeed;
}

//---------------------------------------------------------------------------
bool CTool::SetPathspeed(double spd)
//---------------------------------------------------------------------------
{
	//set the current commanded build path tangential speed for this tool
	//return true if value is acceptable
	if( (spd<0) || (spd>MAX_STEP_FREQ*mmps)) return false;
	pathspeed = spd;
	return true;
}

//---------------------------------------------------------------------------
double CTool::GetJogspeed()
//---------------------------------------------------------------------------
{
	//get the current commanded jog speed (in Hz) for this tool
	return jogspeed;
}

//---------------------------------------------------------------------------
bool CTool::SetJogspeed(double spd)
//---------------------------------------------------------------------------
{
	//set the current commanded jog speed for this tool (in Hz)
	//return true if value is acceptable
	if( (spd<0) || (spd>MAX_STEP_FREQ)) return false;
	jogspeed = spd;
	return true;
}

//---------------------------------------------------------------------------
double CTool::GetVolumeFlowrate()
//---------------------------------------------------------------------------
{
	//return the average volumetric flowrate (estimated)
	//per unit length of path traversed, mm^3/mm
	return (disprate*GetBarrelXSection());
}

//---------------------------------------------------------------------------
double CTool::GetLinearFlowrate()
//---------------------------------------------------------------------------
{
	//return the average linear extrusion flowrate (estimated)
	//per unit length of path traversed, mm/mm
	return (GetVolumeFlowrate()/GetTipXSection());
}

//---------------------------------------------------------------------------
double CTool::GetPathXSection()
//---------------------------------------------------------------------------
{
	//estimate path crosssection as ellipse from deposition parameters
	//return value in mm^2
	return (3.1415926*pathwidth*pathheight/4.0);
}

//---------------------------------------------------------------------------
double CTool::GetTipXSection()
//---------------------------------------------------------------------------
{
	//return the (interior) cross section area of the tip, mm^2
	return(3.1415926*m_dTipID*m_dTipID/4.0);
}

//---------------------------------------------------------------------------
double CTool::GetBarrelXSection()
//---------------------------------------------------------------------------
{
	//return the (interior) cross section area of the syringe barrel, mm^2
	return(3.1415926*m_dBarrelID*m_dBarrelID/4.0);
}

//---------------------------------------------------------------------------
bool CTool::LoadFile(CString toolfilename, CString& errorstr, bool loadgeom)
//---------------------------------------------------------------------------
{ // read tool parameters from asci file

	SetDefaults();

	CStdioFile file;
	if (file.Open(toolfilename, CFile::modeRead) == 0)
		return false;

	CString line, filename = toolfilename;
	BOOL readok;
	CString error;
	int idx = 0;

	//get the name from the file name
	//strip off the directory info
	while((idx=filename.Find('\\')) != -1)
	{
		filename = filename.Right(filename.GetLength()-idx-1);
	}
	name = filename;

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
				if (str1.CompareNoCase("DESCRIPTION")==0) {
					description = str2;
				} else if (str1.CompareNoCase("PATHWIDTH")==0) {
					pathwidth = atof(str2);
					if (pathwidth < 0.01 && pathwidth > 20) {
						error.Format("Path width %.2f [mm] is unreasonable in file\n%s\n", pathwidth, toolfilename);
						errorstr += error;
						return false;
					}
				}
				// 04-18-2008: fix for name of the parameter
				else if (str1.CompareNoCase("BACKFILL")==0) {
					backfill = atof(str2);
					if (backfill < 0.01 && backfill > 20) {
						error.Format("Backfill parameter %.2f [mm] is unreasonable in file\n%s\n", pathwidth, toolfilename);
						errorstr += error;
						return false;
					}
				} else if (str1.CompareNoCase("PUSHOUT")==0) {
					str2.Replace(","," ");
					int j=sscanf_s(str2,"%lf", &pushout);
					if (j != 1) {
						error.Format("%s: Cannot read predispensing parameters '%s'\n", toolfilename, line);
						errorstr += error;
						pushout = 0;
						return false;
					}
				} else if (str1.CompareNoCase("SUCKBACK")==0) {
					str2.Replace(","," ");
					int j=sscanf_s(str2,"%lf", &suckback);
					if (j != 1) {
						error.Format("%s: Cannot read suck-back parameters '%s'\n", toolfilename, line);
						errorstr += error;
						suckback = 0;
						return false;
					}
				} else if (str1.CompareNoCase("SUCKBACKDELAY")==0) {
					str2.Replace(","," ");
					int j=sscanf_s(str2,"%lf", &suckbackDelay);
					if ((j != 1)||(suckbackDelay < 0)||(suckbackDelay>suckback)) {
						error.Format("%s: SUCKBACKDELAY '%s' should be positive and smaller than SUCKBACK\n", toolfilename, line);
						errorstr += error;
						suckbackDelay = 0;
						return false;
					}
				} else if (str1.CompareNoCase("DIRECTION")==0) {
					str2.Replace(","," ");
					int j=sscanf_s(str2,"%lf %lf %lf", 
						&dir.x, &dir.y, &dir.z);
					if (j != 3) {
						error.Format("%s: Cannot read graphics direction parameters '%s'\n", toolfilename, line);
						errorstr += error;
						dir = CVec(0,0,-1);
						return false;
					}
				} else if (str1.CompareNoCase("MOTOR")==0) {
					str2.Replace(","," ");
					int j=sscanf_s(str2,"%ld %ld %lf", 
						&axis, &axisDir, &mmps);
					if (j != 3) {
						error.Format("%s: Cannot read tool motor parameters '%s'\n", toolfilename, line);
						errorstr += error;
						return false;
					}
				} else if (str1.CompareNoCase("VOLUME")==0) {
					maxvolume = atof(str2);
					if (maxvolume < 1 && maxvolume > 100) {
						error.Format("Volume parameter %.2f [cc] is unreasonable in file\n%s\n", maxvolume, toolfilename);
						errorstr += error;
						maxvolume = 10;
						return false;
					}
				} else if (str1.CompareNoCase("DEPOSITIONRATE")==0) {
					disprate = atof(str2);
					if (disprate < 0.0000001 && disprate > 1000) {
						error.Format("Dispensing rate parameter %.2f [mm^2] is unreasonable in file\n%s\n", disprate, toolfilename);
						errorstr += error;
						disprate = 2;
						return false;
					}
				} else if (str1.CompareNoCase("PATHHEIGHT")==0) {
					pathheight = atof(str2);
					if (pathheight < 0.001 && pathheight > 20) {
						error.Format("Path Height %.2f [mm] is unreasonable in file\n%s\n", pathwidth, toolfilename);
						errorstr += error;
						return false;
					}
				} else if (str1.CompareNoCase("RANGE")==0) {
					str2.Replace(","," ");
					int j=sscanf_s(str2,"%lf %lf %lf", &rangemin, &rangemax, &unloadpos);
					if (j != 3) {
						error.Format("%s: Cannot read range parameters '%s'\n", toolfilename, line);
						errorstr += error;
						rangemin = 0;
						rangemax = 100;
						curstep = 0;
					}
				} else if (str1.CompareNoCase("TIPPOINT")==0) {
					str2.Replace(","," ");
					int j=sscanf_s(str2,"%lf %lf %lf", &tip.x, &tip.y, &tip.z);
					if (j != 3) {
						error.Format("%s: Cannot read tip coordinates '%s'\n", toolfilename, line);
						errorstr += error;
						tip = CVec(0,0,0);
						return false;
					}
				} else if (str1.CompareNoCase("TOOLOFFSET")==0) {
					str2.Replace(","," ");
					int j=sscanf_s(str2,"%lf %lf %lf", &tooloffset.x, &tooloffset.y, &tooloffset.z);
					if (j != 3) {
						error.Format("%s: Cannot read tool offset coordinates '%s'\n", toolfilename, line);
						errorstr += error;
						tooloffset = CVec(0,0,0);
					}
				} else if (str1.CompareNoCase("BARRELOFFSET")==0) {
					str2.Replace(","," ");
					int j=sscanf_s(str2,"%lf %lf %lf", &barreloffset.x, &barreloffset.y, &barreloffset.z);
					if (j != 3) {
						error.Format("%s: Cannot read barrel offset coordinates '%s'\n", toolfilename, line);
						errorstr += error;
						barreloffset = CVec(0,0,0);
					}
				} else if (str1.CompareNoCase("PLUNGEROFFSET")==0) {
					str2.Replace(","," ");
					int j=sscanf_s(str2,"%lf %lf %lf", &plungeroffset.x, &plungeroffset.y, &plungeroffset.z);
					if (j != 3) {
						error.Format("%s: Cannot read plunger offset coordinates '%s'\n", toolfilename, line);
						errorstr += error;
						plungeroffset = CVec(0,0,0);
					}
				} else if (str1.CompareNoCase("TIPOFFSET")==0) {
					str2.Replace(","," ");
					int j=sscanf_s(str2,"%lf %lf %lf", &tipoffset.x, &tipoffset.y, &tipoffset.z);
					if (j != 3) {
						error.Format("%s: Cannot read tip offset coordinates '%s'\n", toolfilename, line);
						errorstr += error;
						tipoffset = CVec(0,0,0);
					}
				} else if (str1.CompareNoCase("TOOLGEOM")==0) {
					if (loadgeom) { // skip geometry if not neccesary
						CString geomfile = CUtils::RemoveComment(str2);
						CString geompath = CUtils::GetPath(toolfilename)+"\\"+geomfile;
						if (_access(geompath,4) != 0 || !toolgeom.LoadSTL(geompath, 10000)) {
							error.Format("%s: Cannot load tool geometry '%s'\n", toolfilename, geompath);
							errorstr += error;
							return false;
						}
						toolgeom.name = geomfile;
					}
				} else if (str1.CompareNoCase("BARRELGEOM")==0) {
					if (loadgeom) { // skip geometry if not neccesary
						CString geomfile = CUtils::RemoveComment(str2);
						CString geompath = CUtils::GetPath(toolfilename)+"\\"+geomfile;
						if (_access(geompath,4) != 0 || !barrelgeom.LoadSTL(geompath, 10000)) {
							error.Format("%s: Cannot load barrel geometry '%s'\n", toolfilename, geompath);
							errorstr += error;
							return false;
						}
						barrelgeom.name = geomfile;
					}
				} else if (str1.CompareNoCase("TIPGEOM")==0) {
					if (loadgeom) { // skip geometry if not neccesary
						CString geomfile = CUtils::RemoveComment(str2);
						CString geompath = CUtils::GetPath(toolfilename)+"\\"+geomfile;
						if (_access(geompath,4) != 0 || !tipgeom.LoadSTL(geompath, 10000)) {
							error.Format("%s: Cannot load tip geometry '%s'\n", toolfilename, geompath);
							errorstr += error;
							return false;
						}
						tipgeom.name = geomfile;
					}
				} else if (str1.CompareNoCase("PLUNGERGEOM")==0) {
					if (loadgeom) { // skip geometry if not neccesary
						CString geomfile = CUtils::RemoveComment(str2);
						CString geompath = CUtils::GetPath(toolfilename)+"\\"+geomfile;
						if (_access(geompath,4) != 0 || !plungergeom.LoadSTL(geompath, 10000)) {
							error.Format("%s: Cannot load plunger geometry '%s'\n", toolfilename, geompath);
							errorstr += error;
							return false;
						}
						plungergeom.name = geomfile;
					}
				} else if (str1.CompareNoCase("TOOLCOLOR")==0) {
					str2.Replace(","," ");
					int j=sscanf_s(str2,"%lf %lf %lf %lf", &toolcolor.x, &toolcolor.y, &toolcolor.z, &toolalpha);
					if (j != 4) {
						error.Format("%s: Cannot read tool color '%s'\n", toolfilename, line);
						errorstr += error;
						barrelcolor = CVec(0.5,0.5,0.5);
						barrelalpha = 1;
					}
				} else if (str1.CompareNoCase("BARRELCOLOR")==0) {
					str2.Replace(","," ");
					int j=sscanf_s(str2,"%lf %lf %lf %lf", &barrelcolor.x, &barrelcolor.y, &barrelcolor.z, &barrelalpha);
					if (j != 4) {
						error.Format("%s: Cannot read barrel color '%s'\n", toolfilename, line);
						errorstr += error;
						barrelcolor = CVec(0.5,0.5,0.5);
						barrelalpha = 1;
					}
				} else if (str1.CompareNoCase("TIPCOLOR")==0) {
					str2.Replace(","," ");
					int j=sscanf_s(str2,"%lf %lf %lf %lf", &tipcolor.x, &tipcolor.y, &tipcolor.z, &tipalpha);
					if (j != 4) {
						error.Format("%s: Cannot read tip color '%s'\n", toolfilename, line);
						errorstr += error;
						tipcolor = CVec(0.5,0.5,0.5);
						tipalpha = 1;
					}
				} else if (str1.CompareNoCase("PLUNGERCOLOR")==0) {
					str2.Replace(","," ");
					int j=sscanf_s(str2,"%lf %lf %lf %lf", &plungercolor.x, &plungercolor.y, &plungercolor.z, &plungeralpha);
					if (j != 4) {
						error.Format("%s: Cannot read plunger color '%s'\n", toolfilename, line);
						errorstr += error;
						plungercolor = CVec(0.5,0.5,0.5);
						plungeralpha = 1;
					}
				} else if (str1.CompareNoCase("INCREMENT")==0) {
					str2.Replace(","," ");
					int j=sscanf_s(str2,"%lf", &jogIncrement);
					if (j != 1) {
						error.Format("%s: Cannot read jog increment '%s'\n", toolfilename, line);
						errorstr += error;
						jogIncrement = JOG_STEP_TOOL;
					}
				} else if (str1.CompareNoCase("CLEARANCE")==0) {
					str2.Replace(","," ");
					int j=sscanf_s(str2,"%lf", &clearance);
					if (j != 1) {
						error.Format("%s: Cannot read travel clearance '%s'\n", toolfilename, line);
						errorstr += error;
						clearance = DEFAULT_TRAVEL_CLEARANCE;
					}

				} else if (str1.CompareNoCase("PATHSPEED")==0) {
					str2.Replace(","," ");
					int j=sscanf_s(str2,"%lf", &pathspeed);
					if (j != 1) {
						error.Format("%s: Cannot read speed'%s'\n", toolfilename, line);
						errorstr += error;
						pathspeed = 5.0;
					}
				} else if (str1.CompareNoCase("JOGSPEED")==0) {
					str2.Replace(","," ");
					int j=sscanf_s(str2,"%lf", &jogspeed);
					if (j != 1) {
						error.Format("%s: Cannot read jog speed'%s'\n", toolfilename, line);
						errorstr += error;
						jogspeed = MAX_STEP_FREQ;
					}
				} else if (str1.CompareNoCase("TIPDIAM")==0) {
					str2.Replace(","," ");
					int j=sscanf_s(str2,"%lf", &m_dTipID);
					if (j != 1) {
						error.Format("%s: Cannot read tip ID '%s'\n", toolfilename, line);
						errorstr += error;
						m_dTipID = 0.25; //red EFD tip id
					}
				} else if (str1.CompareNoCase("BARRELDIAM")==0) {
					str2.Replace(","," ");
					int j=sscanf_s(str2,"%lf", &m_dBarrelID);
					if (j != 1) {
						error.Format("%s: Cannot read barrel ID '%s'\n", toolfilename, line);
						errorstr += error;
						m_dBarrelID = 15.8; //standard for 10cc EFC
					}
				} else if (str1.CompareNoCase("PAUSEPATHS")==0) {
					str2.Replace(","," ");
					int j=sscanf_s(str2,"%d", &m_nPausePathCount);
					if (j != 1) {
						error.Format("%s: Cannot read pause path count '%s'\n", toolfilename, line);
						errorstr += error;
						m_nPausePathCount = 0;
					}
				} else if (str1.CompareNoCase("MINPATHLENGTH")==0) {
					str2.Replace(","," ");
					int j=sscanf_s(str2,"%lf", &m_dMinPathLength);
					if (j != 1) {
						error.Format("%s: Cannot read minimum path length '%s'\n", toolfilename, line);
						errorstr += error;
						m_dMinPathLength = 0;
					} else {
	//				error.Format("%s: Unrecognized token '%s'\n", toolfilename, str1);
	//				errorstr += error;
					}
				}
			} else {
				error.Format("%s: Cannot parse '%s'\n", toolfilename, line);
				errorstr += error;
			}
		}
	} while (readok);

	// home current position

	curstep = (long) (unloadpos/mmps);
	cmdstep = curstep;

	file.Close();
	return true;
}


//---------------------------------------------------------------------------
void CTool::SetDefaults(void)
//---------------------------------------------------------------------------
{
	m_bIsMounted = false;
	name = "";
	description = "";
	pathheight = 1;
	pathwidth = 1;
	maxvolume = 100;
	currentvolume = 100;
	backfill = 2;
	tip = CVec(0,0,0);
	tooloffset = barreloffset = tipoffset = plungeroffset = CVec(0,0,0);
	toolcolor = barrelcolor = tipcolor = plungercolor = CVec(200,200,200);
	toolalpha = barrelalpha = tipalpha = plungeralpha = 15;
	toolgeom.Clear();
	barrelgeom.Clear();
	plungergeom.Clear();
	tipgeom.Clear();
	dir = CVec(0,0,-1);
	mmps = 0.005;
	axis = -1; //which amp axis tool is connected to
	axisDir = 1; // +/-1 for normal / reverse motor motion relative to commands
	disprate = 2; // mm of plunger motion per mm travel of tool along path
	curstep = 0; //"real-time" position (in steps) - typically inaccurate if hardware is connected
	cmdstep = 0; //commanded position, in steps
	rangemin = 0; // minimum allowed steps
	rangemax = 100; // [mm] maximum allowed step
	stepstomove = 0; // number of steps to move in current segmnent
	bottompos = 0; // [mm] position at bottom of displenser
	unloadpos = 100; // [mm] position suitable for loading/unloading plunger
	pushout = 0;
	suckback = 0;
	suckbackDelay = 0;
	pathspeed = 5.0; //default maximum path traverse speed for this tool
	jogspeed = MAX_STEP_FREQ;
	m_dTipID = 0.25; //standard EFD red tip ID
	m_dBarrelID = 15.8; //standard EFD 10cc barrel ID
	m_dVolNeeded = 0; //volume of material in mL needed to build current model
	m_nPausePathCount = 0; //at zero, auto pause will never occur
	m_dMinPathLength = 0.0; //mm length of shortest build path (not path segment)
}

//---------------------------------------------------------------------------
CVec CTool::GetPos(bool bCommanded, bool bGraphics)
//---------------------------------------------------------------------------
{// get the component (stage) position; use commanded if bCommanded, else "real-time"
//if bGraphics, then adjust for graphics offset relative to hardware home position
	CFabAtHomeApp* pApp = (CFabAtHomeApp *) AfxGetApp();

	// calculate aggregated position of current stage

	int ref = 2;
	CVec pos(0,0,0);
	long grOffs = 0; //offset in mm
	
	while (ref != -1) {
		//home is stored for component when the hardware/emulator is homed
		if(bGraphics)
			grOffs = (long)pApp->printer.component[ref].home;
		if(bCommanded)
			pos += pApp->printer.component[ref].dir * 
				   (pApp->printer.component[ref].GetCMDPos() + grOffs);
		else
			pos += pApp->printer.component[ref].dir * 
				   (pApp->printer.component[ref].GetRTPos() + grOffs);

		ref = pApp->printer.component[ref].refidx;
	}

	return pos;
}

//---------------------------------------------------------------------------
bool CTool::Draw(bool bCommanded)
//---------------------------------------------------------------------------
{// draw the tool component, show commanded if bCommanded, else show "real-time"

	CFabAtHomeApp* pApp = (CFabAtHomeApp *) AfxGetApp();
	CVec pmotion, toolpos;
	double pdist;

	// calculate aggregated position of printer head stage
	toolpos = GetPos(bCommanded, true); //get the graphics offset position
	// calculate the location of the tool body
	toolpos -= tooloffset;

	// calculate plunger motion
	if(bCommanded)
		pdist = cmdstep*mmps;
	else
		pdist = curstep*mmps;
	if(pdist > rangemax) pdist = rangemax;
	if(pdist < rangemin) pdist = rangemin;

	pmotion = axisDir*dir*pdist;

	// prep OpenGL for drawing transparency of components
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// draw tool
	glPushMatrix();
	glColor4d(toolcolor.x,toolcolor.y,toolcolor.z,toolalpha);
	CGLUtils::glTranslate(toolpos);
	toolgeom.Draw(false, true);
	glPopMatrix();
	// draw plunger
	glPushMatrix();
	glColor4d(plungercolor.x,plungercolor.y,plungercolor.z,plungeralpha);
	CGLUtils::glTranslate(toolpos+plungeroffset+pmotion);
	plungergeom.Draw(false, true);
	glPopMatrix();
	// draw barrel
	glPushMatrix();
	glColor4d(barrelcolor.x,barrelcolor.y,barrelcolor.z,barrelalpha);
	CGLUtils::glTranslate(toolpos+barreloffset);
	barrelgeom.Draw(false, true);
	glPopMatrix();
	// draw tip
	glPushMatrix();
	glColor4d(tipcolor.x,tipcolor.y,tipcolor.z,tipalpha);
	CGLUtils::glTranslate(toolpos+tipoffset);
	tipgeom.Draw(false, true);
	glPopMatrix();
	// draw tip point
	glColor4d(plungercolor.x,plungercolor.y,plungercolor.z,1);
	CGLUtils::DrawSphere(toolpos+tip, 1.5);

	//glPopMatrix();
	glDisable(GL_BLEND);

	return false;
}

//---------------------------------------------------------------------------
bool CTool::LoadTools(CString directory, bool bUpdateParams)
//---------------------------------------------------------------------------
{ // open and read all tool files in given directory
	//if bUpdateParams, just update the parameters from the files to the
	//tool array

   CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();

   CTool newtool;
   CString errorstr;
   int toolIdx;
   int oldCnt = 0;
   int newCnt = 0;

   WIN32_FIND_DATA FindFileData;
   HANDLE hFind = INVALID_HANDLE_VALUE;

   hFind = FindFirstFile(directory+"\\*.tool", &FindFileData);
   if (hFind == INVALID_HANDLE_VALUE)
   {// Invalid file handle
	   errorstr.Format("Unable to load tools from directory %s",directory);
	   ((CFabAtHomeApp *)AfxGetApp())->Log(errorstr);
	   return false;
   }

	CArray<CString,CString> mountedNames;
	int activeIdx;
	if(!bUpdateParams)
	{
	   //if we are not updating parameters, but instead reading in everything
	   //cache the name(s) of the mounted tool(s)
	   for(int i = 0; i < pApp->tool.GetSize(); i++)
	   {
		   if(pApp->tool[i].IsMounted())
		   {
			   mountedNames.Add(pApp->tool[i].GetName());
			   if(pApp->printer.GetCurToolName().CompareNoCase(pApp->tool[i].GetName())==0)
				   activeIdx = mountedNames.GetSize()-1;
		   }
	   }
	   //then flush the array
	   pApp->tool.RemoveAll();
	}

   do 
   {
	   bool loaded = newtool.LoadFile(directory+"\\"+FindFileData.cFileName, errorstr, true);
	   if (loaded)
	   {
		   //if we are just updating changed parameters, match tools from files with
		   //tools from tool array by name, and copy in the parameters
		   if(bUpdateParams)
		   {
			   toolIdx = SearchByName(newtool.name);
			   //if we can find the tool in the current array
			   if(toolIdx >= 0 && toolIdx < pApp->tool.GetSize() )
			   {
				   //just copy its parameters
				   pApp->tool[toolIdx].CopyParamsFrom(newtool);
				   oldCnt++;
			   }
			   else
			   {
				   //couldn't find it, so add it to the array as a new tool
				   pApp->tool.Add(newtool);
				   newCnt++;
			   }
		   }
		   else //not updating parameters
		   {
			   //if the previously mounted tools are present in the new files
			   //mount them again
			   for(int i = 0; i < mountedNames.GetSize(); i++)
			   {
				   if(newtool.GetName().CompareNoCase(mountedNames[i])==0)
				   {
					   newtool.Mount();
					   mountedNames.RemoveAt(i);
				   }
			   }
			   //then add new tool to the array
			   pApp->tool.Add(newtool);
			   newCnt++;
		   }
		   TRACE("Loaded tool %s\n", newtool.name);
	   }
   } while (FindNextFile(hFind, &FindFileData) != 0);

   if (!errorstr.IsEmpty())
	   ((CFabAtHomeApp *)AfxGetApp())->Log(CString("Error found while reading tool files: ")+errorstr+" Erroneous files skipped.");

   DWORD dwError = GetLastError();
   FindClose(hFind);
   if (dwError != ERROR_NO_MORE_FILES) // FindNextFile error; dwError
	   return false;

   //success, so report
   if(bUpdateParams)
   {
	   CString str1, str2;
	   //str1.Format("Updated parameters for ");
	   str1.Format("Reloaded parameters for ");
	   if(newCnt == 0)
	   {
		   str2.Format("%d existing tools", oldCnt);
	   }
	   else
	   {
		   str2.Format("%d existing tools, and loaded %d new tools", oldCnt, newCnt);

	   }
	   errorstr.Format("%s%s from directory: %s", str1, str2,directory);
   }
   else
   {
	   CString tmp;
	   if(mountedNames.GetSize() > 0)
	   {
		   for(int i = 0; i < mountedNames.GetSize(); i++)
		   {
			   tmp+=mountedNames[i];
			   tmp+=" ";
		   }
		   errorstr.Format("Previously mounted tools not found: %s",tmp);
	   }
	   else
	   {
		   errorstr.Format("Loaded %d tools from directory: %s", newCnt, directory);
	   }
   }
   ((CFabAtHomeApp *)AfxGetApp())->Log(errorstr);

   return true;
}

//---------------------------------------------------------------------------
bool CTool::CopyParamsFrom( const CTool& s )
//---------------------------------------------------------------------------
{
	//copy only the parameters, not all varables from the fromTool to this tool
	//COMMENTED OUT VARIABLES ARE STILL HERE TO SHOW WHAT IS NOT BEING COPIED

	description = s.description;
	maxvolume = s.maxvolume;
	currentvolume = s.currentvolume;
	pathwidth = s.pathwidth;
	pathheight = s.pathheight;
	backfill = s.backfill;
	tip = s.tip;
	tooloffset = s.tooloffset;
	barreloffset = s.barreloffset;
	tipoffset = s.tipoffset;
	plungeroffset = s.plungeroffset;
	toolgeom = s.toolgeom;
	plungergeom = s.plungergeom;
	barrelgeom = s.barrelgeom;
	tipgeom = s.tipgeom;
	toolcolor = s.toolcolor;
	plungercolor = s.plungercolor;
	barrelcolor = s.barrelcolor;
	tipcolor = s.tipcolor;
	toolalpha = s.toolalpha;
	plungeralpha = s.plungeralpha;
	tipalpha = s.tipalpha;
	barrelalpha = s.barrelalpha;
	disprate = s.disprate;
	dir = s.dir;
	mmps = s.mmps;
	axis = s.axis;
	axisDir = s.axisDir;
	//curstep=s.curstep;
	//cmdstep=s.cmdstep;
	rangemin=s.rangemin;
	rangemax=s.rangemax;
	//stepstomove=s.stepstomove;
	bottompos=s.bottompos;
	unloadpos=s.unloadpos;
	pushout=s.pushout;
	suckback = s.suckback;
	suckbackDelay = s.suckbackDelay;
	//posLimitSwitch = s.posLimitSwitch;
	//negLimitSwitch = s.negLimitSwitch;
	jogIncrement = s.jogIncrement;
	clearance = s.clearance;
	pathspeed = s.pathspeed;
	jogspeed = s.jogspeed;
	m_nPausePathCount = s.m_nPausePathCount;
	m_dMinPathLength = s.m_dMinPathLength;
	return true;
}


//---------------------------------------------------------------------------
int CTool::SearchByName(CString& name)
//---------------------------------------------------------------------------
{ // search all tools and find match by name

   CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
   name.Trim();
   if (name.IsEmpty())
	   return -1;

   // try exact match
   for (int i=0; i<pApp->tool.GetSize(); i++) {
	   if (name.CompareNoCase(pApp->tool[i].name) == 0) 
		   return i;
   }

   // try weak match

   for (int i=0; i<pApp->tool.GetSize(); i++) {
	   if (name.Find(pApp->tool[i].name) != -1) 
		   return i;
   }

   return -1;
}

//---------------------------------------------------------------------------
CTool* CTool::GetByName(CString& name)
//--------------------------------------------------------------------------
{
	//return pointer to the named tool or NULL if fails
	int idx;
	CFabAtHomeApp* pApp = (CFabAtHomeApp*)AfxGetApp();
	idx = SearchByName(name);
	return GetByIndex(idx);
}

//---------------------------------------------------------------------------
CTool* CTool::GetByIndex(int idx)
//---------------------------------------------------------------------------
{
	//return a pointer to the indexed tool or NULL if fails
	CFabAtHomeApp* pApp = (CFabAtHomeApp*)AfxGetApp();
	if((idx >= 0) && (idx < pApp->tool.GetSize()))
		return(	&(pApp->tool[idx]) );
	else
		return NULL;
}

//---------------------------------------------------------------------------
CString CTool::GetName(int idx)
//---------------------------------------------------------------------------
{
	//return the name of the indexed tool
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	if((idx >= pApp->tool.GetSize()) || (idx < 0)) return CString("Undefined");
	return pApp->tool[idx].GetName();
}

//---------------------------------------------------------------------------
bool CTool::Mount(CString name)
//---------------------------------------------------------------------------
{
	//find the tool by name and mount it
	int idx = SearchByName(name);
	if(idx < 0) return false;
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	pApp->tool[idx].Mount();
	return true;
}

//---------------------------------------------------------------------------
bool CTool::Unmount(CString name)
//---------------------------------------------------------------------------
{
	//find the tool by name and unmount it
	int idx = SearchByName(name);
	if(idx < 0) return false;
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	pApp->tool[idx].Unmount();
	return true;
}

//---------------------------------------------------------------------------
CString CTool::FindNextMounted(CString &curTool)
//---------------------------------------------------------------------------
{ 
	//find the next mounted tool after curTool
	//wrap to start of array if necessary
	//return "Undefined" if none

	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	int cnt = pApp->tool.GetSize()-1; //look at all except curTool
	int i = SearchByName(curTool)+1;
	if(i <= 0) //searchbyname failed
		return CString("Undefined");
	while(cnt > 0)
	{	
		if(i >= pApp->tool.GetSize()-1) 
			i=0; //wrap around
		if(pApp->tool[i].IsMounted()) return pApp->tool[i].GetName();
		    i++;
		cnt--;
	}
	return CString("Undefined");
}

//--------------------------------------------------------------------
int CTool::MountedToolCount()
//--------------------------------------------------------------------
{
	//return the number of currently mounted tools
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	int cnt = 0;
	for(int i = 0; i < pApp->tool.GetSize(); i++)
	{
		if(pApp->tool[i].IsMounted())
			cnt++;
	}
	return cnt;
}

//--------------------------------------------------------------------
int CTool::LoadedToolCount()
//--------------------------------------------------------------------
{
	//return the number currently loaded tools
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	return pApp->tool.GetSize();
}