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

// Tool.h: interface for the CTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOL_H__E6E66053_7A19_49D9_8A15_4BF2E0B90C2E__INCLUDED_)
#define AFX_TOOL_H__E6E66053_7A19_49D9_8A15_4BF2E0B90C2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTool : public CObject
{
public:
	CString GetDescription();
	CTool();
	virtual ~CTool();
	CTool(CTool& s);
	CTool& operator=(const CTool& s);
	bool RequestPause(int pathCnt); //return true if auto pause is desirable
	void SetPausePathCount(int count);
	int GetPausePathCount(){return m_nPausePathCount;};
	double GetPathspeed();
	bool SetPathspeed(double spd);
	double GetJogspeed();
	bool SetJogspeed(double spd);
	double GetTipID(){return m_dTipID;};
	double GetBarrelID(){return m_dBarrelID;};
	double GetVolumeNeeded(){return m_dVolNeeded;};
	void SetVolumeNeeded(double vol){m_dVolNeeded = vol;};
	double GetVolumeFlowrate(); //return the average volumetric flowrate (estimated), per unit length of path traversed, mm^3/mm
	double GetLinearFlowrate(); //return the average linear extrusion flowrate (estimated), per unit length of path traversed, mm/mm
	double GetPathXSection(); //return estimated cross section area of deposited path, based on deposition params, mm^2
	double GetTipXSection(); //return the (interior) cross section area of the tip, mm^2
	double GetBarrelXSection(); //return the (interior) cross section area of the syringe barrel, mm^2
	bool LoadFile(CString toolfilename, CString& errorstr, bool loadgeom);
	bool Draw(bool bCommanded = false); //typically want to show the "real-time" view
	static bool LoadTools(CString directory, bool bUpdateParams = false);
	static int SearchByName(CString& name);
	static CTool* GetByName(CString& name);
	static CTool* GetByIndex(int idx);
	static CString FindNextMounted(CString& curTool); //search for the next mounted tool after curTool return "Undefined" if none
	CString GetName(){return name;};
	static CString GetName(int idx); //return the name of the indexed tool
	CVec GetPos(bool bCommanded = true, bool bGraphics = false); //for most purposes, use the commanded position, not the "real-time" (delayed)
	double GetClearance(){return clearance;}; //return the travel clearance
	bool IsMounted(){return m_bIsMounted;}; //true if the tool is currently mounted
	static bool Mount(CString name); //find the tool by name and mount it
	static bool Unmount(CString name); //find the tool by name and unmount it
	static int MountedToolCount(); //return the number of currently mounted tools
	static int LoadedToolCount(); //return the number currently loaded tools
	bool CopyParamsFrom( const CTool& s ); //copy parameters, but not all variables from s
	double GetMinPathLength(){return m_dMinPathLength;};
	void SetMinPathLength(double len){m_dMinPathLength = len;};

	// IMPORTANT: Attributed added here must also be 
	// updated in function "operator=" and initialized in constructor.
	CString name; // identifier label
	CString description; // textual lomg description
	CGeometry toolgeom, barrelgeom, plungergeom, tipgeom; // triangulation mesh of tool components
	CVec tooloffset, barreloffset, plungeroffset, tipoffset; // drawing offset of 4 components
	CVec toolcolor, barrelcolor, plungercolor, tipcolor; // RGB color of tool parts (in range 0-1)
	double toolalpha, barrelalpha, plungeralpha, tipalpha; // transparency alpha (in range 0-1)
	double maxvolume; // maximum volume possible in this tool
	double currentvolume; // current colume remaining in tool
	double pathwidth; // [mm] path width produced by this tool
	double pathheight; // [mm] path produced by this tool
	double backfill; // [mm] maximum allwed backfill
	CVec tip; // position of tip in tool coordinates
	CVec dir; // direction of motion of plunger in graphics
	int axis; // which control axis this tool is attached to (0 to AXES_CNT-1)
	int axisDir; //+1 or -1; motion direction for axis 
	double mmps; // mm travel per step of plunger
	double disprate; // mm of plunger motion per mm tool travel along deposition path
	double curstep; // "real-time" position (in steps) - typically inaccurate if hardware is connected
	double cmdstep; // commanded position in steps
	double rangemin; // [mm] minimum allowed position
	double rangemax; // [mm] maximum allowed position
	double stepstomove; // number of steps to move in current segment
	double bottompos; // [mm] position at bottom of displenser
	double unloadpos; // [mm] position suitable for loading/unloading plunger
	double pushout; // [sec] early start of deposition before path
	double suckback; // [sec] start suck back and end of path
	double suckbackDelay; // [sec] postpone onset of suckback-positive values shift it later in path; ignore negative values
	double jogIncrement; //distance to move in manual jogging
	double clearance;	//clearance between tip and last layer when traversing
	double m_dMinPathLength; //minimum length of paths for this tool
	double m_dBarrelID; //interior diameter of the syringe barrel
	double pathspeed; //top path tangential speed in mm/s for this tool during building
	int m_nPausePathCount; //number of paths after which an automatic pause will be triggered
	double m_dTipID; //interior diameter of the tip


	// IMPORTANT: Attributed added here must also be updated 
	// in the functions "operator=", "Serialize", and "SetDefault"

	void SetDefaults(void);

protected:
	void Mount(){m_bIsMounted = true;};
	void Unmount(){m_bIsMounted = false;};

protected:
	bool m_bIsMounted; //is tool mounted on carriage
	double jogspeed; //jog speed for tool in Hz
	double m_dVolNeeded; //volume of material needed in mm^3 for the current model
};

#endif // !defined(AFX_TOOL_H__E6E66053_7A19_49D9_8A15_4BF2E0B90C2E__INCLUDED_)
