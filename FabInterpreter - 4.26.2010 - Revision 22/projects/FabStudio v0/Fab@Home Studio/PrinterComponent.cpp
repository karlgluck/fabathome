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

#include "stdafx.h"
#include ".\printercomponent.h"

//---------------------------------------------------------------------------
CPrinterComponent::CPrinterComponent(void)
//---------------------------------------------------------------------------
{
	SetDefaults();
}

//---------------------------------------------------------------------------
CPrinterComponent::~CPrinterComponent(void)
//---------------------------------------------------------------------------
{
}

//---------------------------------------------------------------------------
CPrinterComponent::CPrinterComponent(CPrinterComponent& s)
//---------------------------------------------------------------------------
{
	*this = s;
}

//---------------------------------------------------------------------------
CPrinterComponent& CPrinterComponent::operator=(const CPrinterComponent& s)
//---------------------------------------------------------------------------
{// copy component object

	geom = s.geom;
	color = s.color;
	dir = s.dir;
	mmps = s.mmps;
	offset = s.offset;
	refidx = s.refidx;
	curstep = s.curstep;
	cmdstep = s.cmdstep;
	stepstomove = s.stepstomove;
	pmin = s.pmin;
	pmax = s.pmax;
	rangemin = s.rangemin;
	rangemax = s.rangemax;
	home = s.home;
	posLimitSwitch = s.posLimitSwitch;
	negLimitSwitch = s.negLimitSwitch;
	axis = s.axis;
	axisDir = s.axisDir;
	jogIncrement = s.jogIncrement;
	oldDeltaV.Copy(s.oldDeltaV);
	m_dErrOffset = s.m_dErrOffset;
	component_geom = s.component_geom;
	component_appendgeom = s.component_appendgeom;
	return *this;
}


//---------------------------------------------------------------------------
void CPrinterComponent::SetDefaults(void)
//---------------------------------------------------------------------------
{
	geom.Clear();
	color = CVec(100,100,100);
	alpha = 1;
	dir = CVec(1,0,0);
	mmps = 0.01;
	offset = CVec(0,0,0);
	refidx = -1;
	curstep = 0;
	cmdstep = 0;
	laststep = 0;
	cur_t = 0;
	last_t = 0;
	stepstomove = 0;
	rangemin = 0;
	rangemax = 240;
	vwindow = 5;
	v_calc_step = 0;
	velocity = 0;
	home = 0;
	posLimitSwitch = FALSE;
	negLimitSwitch = FALSE;
	axis = -1;
	axisDir = 1;
	jogIncrement = JOG_STEP_AXIS;
	m_dErrOffset = 0;
	component_geom = "";
	component_appendgeom = "";
}

//---------------------------------------------------------------------------
bool CPrinterComponent::Draw(bool bCommanded)
//---------------------------------------------------------------------------
{// draw the printer component (stage); incorporate the graphics offset (home)

	CFabAtHomeApp* pApp = (CFabAtHomeApp *) AfxGetApp();

	CVec pos = GetGraphicsPos(bCommanded);  //getting position for graphics

	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix();
	CGLUtils::glTranslate(pos);
	glColor4d(color.x,color.y,color.z,alpha);
	geom.Draw(false, true);
	glPopMatrix();
	glDisable(GL_BLEND);

	return false;
}

//---------------------------------------------------------------------------
CVec CPrinterComponent::GetGraphicsPos(bool bCommanded)
//---------------------------------------------------------------------------
{
	// get the component's graphics position (in absolute coords)
	// adjust for graphics offset relative to hardware position
	// calculate aggregated position of current stage (e.g. Y is stacked on X)

	CFabAtHomeApp* pApp = (CFabAtHomeApp *) AfxGetApp();

	int ref = refidx;
	
	//home is stored for component when the hardware/emulator is homed
	long grOffs = (long) ((double)home / (double)mmps); //offset in steps
	CVec pos;
	if(bCommanded)
		pos = dir*(cmdstep+grOffs)*mmps-offset;
	else
		pos = dir*(curstep+grOffs)*mmps-offset;

	while (ref != -1)
	{
		if(bCommanded)
		{
			pos += pApp->printer.component[ref].dir * 
				pApp->printer.component[ref].GetCMDPos();
		}
		else
		{
			pos += pApp->printer.component[ref].dir * 
				pApp->printer.component[ref].GetRTPos();
		}
		ref = pApp->printer.component[ref].refidx;
	}

	return pos;
}

//---------------------------------------------------------------------------
double CPrinterComponent::GetPos(bool bCommanded)
//---------------------------------------------------------------------------
{
	// get the component (stage) position in mm 
	// return the commanded if bCommanded, else the "real-time" (delayed)

	double pos;
	if(bCommanded)
	{
		pos = cmdstep*mmps;
	}
	else
	{
		pos = curstep*mmps;
	}
	return pos;
}

//---------------------------------------------------------------------------
bool CPrinterComponent::SetPos(double pos, bool bCommanded)
//---------------------------------------------------------------------------
{
	//set the position of the component in mm, commanded pos if bCommanded
	//else real-time position - convert position (mm) to steps
	//if real-time - update the velocity

	last_t = cur_t;	//store the time of the last update
	cur_t = GetTickCount(); //grab the current time (ms)

	if(bCommanded)
		cmdstep = pos/mmps;
	else
	{
		laststep = curstep; //store the last position
		curstep = (pos-GetErrorOffset())/mmps;	//update the current position
	}

	//now update the calculated real-time velocity
	return CalcVel();
}

//---------------------------------------------------------------------------
void CPrinterComponent::AccumulateErrorOffset(double offset)
//---------------------------------------------------------------------------
{
	//error offset keeps accumulating
	//only homing can eliminate it (because homing clears microcontroller position registers)
	m_dErrOffset += offset;
}

//---------------------------------------------------------------------------
double CPrinterComponent::GetVel()
//---------------------------------------------------------------------------
{
	//return the velocity in mm/s
	//TRACE("Velocity:%3.3f\n", velocity*mmps);
	return velocity*mmps;
}

//---------------------------------------------------------------------------
bool CPrinterComponent::CalcVel()
//---------------------------------------------------------------------------
{
	//calculate the velocity in steps/s
	//update the velocity state variable
	//use windowed average, with window size vwindow

	//while v_calc_step is less than vwindow, just accumulate
	//deltaV fractions; after vwindow steps, remove the oldest value

	double deltaPos = curstep - laststep;
	double deltaT = (cur_t - last_t); 
	int i;
	if(deltaT <= 0) return false;

	//calculate the average weighted deltaV increment
	double dVinc = deltaPos*1000.0/(deltaT*vwindow); //convert to seconds
	
	if(v_calc_step < vwindow)
	{
		//track the old deltaV increments
		oldDeltaV.Add(dVinc);
		//track how many ticks have accumulated
		v_calc_step++;
	}
	else
	{
		//after accumulating vwindow increments
		//start removing oldest increment and shift the array down
		for(i = 0; i < vwindow-1; i++)
		{
			oldDeltaV[i] = oldDeltaV[i+1];
		}
		oldDeltaV[vwindow-1] = dVinc;
	}
	//finally calculate and store the average velocity
	velocity = 0.0;
	for(i = 0; i < v_calc_step; i++)
	{
		velocity+=oldDeltaV[i];
	}
	return true;
}