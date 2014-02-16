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

class CPrinterComponent :
	public CObject
{
public:
	CPrinterComponent(void);
	~CPrinterComponent(void);
	CPrinterComponent(CPrinterComponent& s);
	CPrinterComponent& operator=(const CPrinterComponent& s);
	double GetErrorOffset(){return m_dErrOffset;};
	void AccumulateErrorOffset(double offset);
	void ClearErrorOffset(){m_dErrOffset = 0;};
	void SetDefaults(void);
	CVec GetGraphicsRTPos(){return GetGraphicsPos(false);}; //return the real-time position of the graphics
	CVec GetGraphicsCMDPos(){return GetGraphicsPos(true);}; //return the commanded position of the graphics
	double GetCMDPos(){return GetPos(true);};	//get the current commanded position in mm
	double GetRTPos(){return GetPos(false);};	//get the current real-time position in mm
	bool SetCMDPos(double pos){return SetPos(pos, true);}; //set the current commanded position, in mm
	bool SetRTPos(double pos){return SetPos(pos, false);}; //set the current real-time position, in mm
	double GetVel();
	bool Draw(bool bCommanded = false); //typically want to show the "real-time" view

public:
	CGeometry geom;
	CVec color; // RGB
	double alpha; // transparency 0-1
	CVec dir; // motion direction for graphics
	double mmps; // mm travel per step
	CVec offset; // mm, offset of entire geometry
	int refidx; // index of component this stages moves relative to, or -1
	double stepstomove; // steps to move in next iteration, temporary
	CVec pmin, pmax; // bounding box of component (without offset)
	double rangemin, rangemax; // mm, extremal allowed travel position
	double home; //mm, offset of display from zero position
	double msps; // maximum speed (steps per second)
	BOOL posLimitSwitch; //TRUE if limit switch connected & used for positive direction
	BOOL negLimitSwitch; //TRUE if limit switch connected & used for negative direction
	int axis;	//which control axis this component is attached to (0 to AXES_CNT-1)
	int axisDir; //+1 or -1; motion direction for axis
	double jogIncrement; //distance to move in manual jogging
	CString component_geom;
	CString component_appendgeom;

protected:
	double curstep; // current position in steps - updates asynchronously when hardware is running
	double m_dErrOffset; //offset for errors detected by homing or seeking limit switches during a build
	double cmdstep;	// commanded destination in steps
	CArray <double,double> oldDeltaV; //array to keep track prior velocities for averaging
	double laststep; // last known position in steps
	DWORD last_t; // time of last position update
	DWORD cur_t; // time of the current position update
	int vwindow;	// number of time steps to average velocity over
	int v_calc_step;	//track ticks elapsed since velocity calc started
	double velocity; // current actual velocity -not for commanding

	bool CalcVel(); //calculate the velocity, update the velocity state variable
	bool SetPos(double pos, bool bCommanded); //set the current position
	double GetPos(bool bCommanded = true); //get the current position in mm
	CVec GetGraphicsPos(bool bCommanded = true); //get the current graphics position in global coords
};
