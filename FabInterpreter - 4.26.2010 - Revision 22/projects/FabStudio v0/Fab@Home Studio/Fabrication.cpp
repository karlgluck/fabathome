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

// Fabrication.cpp: implementation of the CFabrication class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// --------------------------------------------------------------------------
CFabrication::CFabrication()
// --------------------------------------------------------------------------
{

	bFabricating = false;
	bPause = false;
	bFabPaused = false;
	bAbortRequested = false;
	m_bReady = false;
	m_nCurrentLayer = 0;
	m_nCurrentPath = 0;
	m_ctsPauseTotal = 0;
	m_ctsFabTotal = 0;
}

// --------------------------------------------------------------------------
CFabrication::~CFabrication()
// --------------------------------------------------------------------------
{

}

//---------------------------------------------------------------------------
void CFabrication::Serialize(CArchive& ar)
//---------------------------------------------------------------------------
{// Save / Load

	int version = 10;

	if (ar.IsStoring()) { //add storing code here

		ar << version;

		// save data
		ar << m_nCurrentLayer;
		ar << m_nCurrentPath;
		ar << version;

	}	else	{  // add loading code here

		int version1, version2;
		ar >> version1; if (version1 < 10 || version1 > version) throw 6;

		// load data
		ar >> m_nCurrentLayer;
		ar >> m_nCurrentPath;
		ar >> version2; if (version2 != version1) throw 6;

	}
}

//---------------------------------------------------------------------------
CFabrication::CFabrication(CFabrication& s)
//---------------------------------------------------------------------------
{
	*this = s;
	m_nCurrentLayer = s.m_nCurrentLayer;
	m_nCurrentPath = s.m_nCurrentPath;
}

//---------------------------------------------------------------------------
CFabrication& CFabrication::operator=(const CFabrication& s)
//---------------------------------------------------------------------------
{

	layer.Copy(s.layer);
	m_nCurrentLayer = s.m_nCurrentLayer;
	m_nCurrentPath = s.m_nCurrentPath;
	return *this;
}

//---------------------------------------------------------------------------
void CFabrication::SetPauseEnd()
//---------------------------------------------------------------------------
{
	//add the duration of the pause to the pause total
	m_ctsPauseTotal += (CTime::GetCurrentTime()-m_ctPauseStart);
}

//---------------------------------------------------------------------------
CTimeSpan& CFabrication::GetFabTotal()
//---------------------------------------------------------------------------
{
	//return the current elapsed time for this build, and store it
	if(IsFabricating())
		m_ctsFabTotal = CTime::GetCurrentTime() - m_ctFabStart;
	return m_ctsFabTotal;
}

//---------------------------------------------------------------------------
void CFabrication::Abort()
//---------------------------------------------------------------------------
{
	//terminate the fabrication process
	m_sStatusMsg.Format("Fabrication aborted");
	TRACE("CFabrication:Abort()\n");
	bAbortRequested = false;
	bFabricating = false;
	bFabPaused = false;
	bPause = false;
}

//---------------------------------------------------------------------------
bool CFabrication::Flush()
//---------------------------------------------------------------------------
{
	//reset the fabrication, flush all data
	for(int i = 0; i < layer.GetSize(); i++)
	{
		layer[i].m_mutAccess.Lock();
	}
	layer.RemoveAll();
	SetCurrentLayer(0);
	SetCurrentPath(0);
	SetReady(false);
	bFabricating = false;
	bAbortRequested = false;
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	pApp->printer.m_dModelClearance = 0;
	return true;
}

//NOTE: Fabricate does not run in real time
//the commands are queued onto the microcontroller
//and status information (e.g. current position) is not
//updated in a predictable fashion.  Thus Fabricate should
//base all commands on desired destination (commanded) rather
//than the actual position
//Also - bear in mind that the coordinate system for the part to be built
//is attached to the z build surface, while the robot coordinate system
//is attached to the robot base
//---------------------------------------------------------------------------
void CFabrication::Fabricate(CModel *pModel)
//---------------------------------------------------------------------------
{ // Fabricate all layers. 
  // Use "if (bAbortRequested) return" before each stage

	ASSERT_VALID(pModel);
	//set flags to indicate we have begun
	int suckStartIdx = 0;
	bAbortRequested = false;
	bPause = false;
	bFabPaused = false;
	bFabricating = true;

	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	CVec lastCommandedPos = pApp->printer.GetCurrentPos(true);
	//make sure the current tool/material is the correct one to start with
	long segmentsProcessed = 0;
	long lastQueued = 0;
	int pathsSincePause = 0; //keep track of number of paths since last pause
	CString curToolName = pApp->printer.GetCurToolName();
	CString curMatName = pApp->printer.GetCurMatName();
	int chunkid = layer[0].chunkid;
	int chunkidx = pModel->FindChunk(chunkid);
	int lastChunkID = layer[0].chunkid;
	CString needToolName = pModel->chunk[chunkidx].toolname;
	CString needMatName = pModel->chunk[chunkidx].materialname;
	DWORD tmpIdx;
	CVec prevPos;

	//do all chunks in the model have the same priority
	//if so then strict layerwise building
	//and don't have to worry about collision with taller parts
	bool samePriority = true;
	for(int i = 1; i < pModel->chunk.GetSize(); i++)
	{
		if(pModel->chunk[0].GetPriority() != pModel->chunk[i].GetPriority())
			samePriority = false;
	}

	//mark the current time
	SetFabStart();

	CVec org = pApp->printer.GetOrigin();
	prevPos = org;
	pApp->printer.GotoOrigin();
		
	for (int lyr = 0; lyr < layer.GetSize(); lyr++) {
		CString msg1,msg2;
		msg1.Format("Fabricate()");
		msg2.Format("Layer:%d",lyr);
		pApp->printer.Log(msg1,msg1);
		if(!pApp->printer.IsQEmpty() || pApp->printer.IsMoving() || pApp->printer.IsToolMoving())
		{
			m_sStatusMsg.Format("New layer waiting on queue to empty");
			pApp->printer.WaitForMoveDone();
		}

		if (bAbortRequested) 
		{
			return Abort();
		}
	
		//keep track of the current layer
		m_nCurrentLayer = lyr;

		//keep track (in CPrinter) of the current maximum height of the model
		if(pApp->printer.m_dModelClearance <= pModel->fab.layer[lyr].region[0].path[0].v[0].z)
		{
			pApp->printer.m_dModelClearance  = pModel->fab.layer[lyr].region[0].path[0].v[0].z;
		}

		//update some variables for this layer
		curToolName = pApp->printer.GetCurToolName();
		curMatName = pApp->printer.GetCurMatName();
		lastChunkID = chunkid;
		chunkid = layer[lyr].chunkid;
		chunkidx = pModel->FindChunk(chunkid);
		needToolName = pModel->chunk[chunkidx].toolname;
		needMatName = pModel->chunk[chunkidx].materialname;

		//if the current tool/material is not the needed tool/material, ask for tool/material change
		if((curToolName.CompareNoCase(needToolName) != 0) /*|| 
			(curMatName.CompareNoCase(needMatName) != 0)*/) //TODO: add material capability back in
		{
			m_sStatusMsg.Format("Fabrication paused for tool change");
			SetPauseStart();
			bFabPaused = true;
			if(!pApp->printer.ChangeTool(needToolName, needMatName))
			{
				bPause = bFabPaused = false;
				return Abort();
			}
			//note that a tool change has occurred
			pModel->IncrementToolChanges();
			bFabPaused = bPause;
			TRACE("bFabPaused = %d",(BOOL)bFabPaused);
			//unless this is a multitool config, and user requests no pausing
			//user will need to return from pause using the menu command
			while(bPause && !bAbortRequested)
				Sleep(25);
			m_sStatusMsg.Format("Fabrication resumed after tool change");
			if (bAbortRequested) return Abort();
			bFabPaused = false;
			SetPauseEnd();
		}

		//print all regions
		for(int rgn = 0; rgn < layer[lyr].region.GetSize(); rgn++)
		{
			layer[lyr].m_nCurRegion = rgn;
			// print all paths
			for (int pth = 0; pth < layer[lyr].region[rgn].path.GetSize(); pth++)
			{
				//pause the queue to prevent any premature execution
				pApp->printer.PauseQueue();

				//make sure we have stopped moving				
				if(!pApp->printer.IsQEmpty() || pApp->printer.IsMoving() || pApp->printer.IsToolMoving())
				{
					m_sStatusMsg.Format("New path waiting on queue to empty");
					pApp->printer.WaitForMoveDone();
				}
				
				CString msg1,msg2;
				msg1.Format("Fabricate()");
				msg2.Format("Path:%d",pth);
				pApp->printer.Log(msg1,msg2);

				pathsSincePause++;
				m_nCurrentPath = pth;
				if (bAbortRequested) return Abort();

				//distance to liftoff at every path for clearance
				//remember robot coords (negate z)
				org = pApp->printer.GetOrigin(); //update the origin (changes with tool change)
				CVec skip(0,0,pApp->printer.GetClearance(samePriority)); 
				skip = pApp->printer.ModelToRobotTfm(skip);
				
				// move tool from end of last path to start of new path
				CVec v1 = pApp->printer.ModelToRobotTfm(layer[lyr].region[rgn].path[pth].v[0]);
				if((org+v1).z > pApp->printer.GetCurrentPos().z)
				{
					//if next point is above the current move down then over
					TRACE("next.z: %f >= current.z: %f\n", (org+v1).z, pApp->printer.GetCurrentPos().z);
					pApp->printer.JogCarriageBy(skip); //lower surface for clearance
					pApp->printer.Move(true, &tmpIdx);	//execute the motion
					segmentsProcessed++;
					TRACE("JogBy(%f,%f,%f)\n",skip.x, skip.y, skip.z);
					pApp->printer.JogCarriageTo(org+v1+skip); //move over to start of next path
					pApp->printer.Move(true, &tmpIdx);	//execute the motion 
					segmentsProcessed++;
					TRACE("JogTo(%f,%f,%f)\n", (org+v1+skip).x,(org+v1+skip).y,(org+v1+skip).z);
				} else {
					//if next point is below current, move over then down
					TRACE("next.z: %f < current.z: %f\n", (org+v1).z, pApp->printer.GetCurrentPos().z);
					pApp->printer.JogCarriageBy(skip); //lower surface for extra clearance
					pApp->printer.Move(true, &tmpIdx);	//execute the motion
					segmentsProcessed++;
					TRACE("JogBy(%f,%f,%f)\n",skip.x, skip.y, skip.z);
					CVec nxt = (org+v1);
					nxt.z = pApp->printer.GetCurrentPos().z;
					pApp->printer.JogCarriageTo(nxt); //move over to start of next path
					pApp->printer.Move(true, &tmpIdx);	//execute the motion
					segmentsProcessed++;
					TRACE("JogTo(%f,%f,%f)\n",nxt.x,nxt.y,nxt.z);
				}
				//now move up to the height of the next path
				pApp->printer.JogCarriageTo(org+v1); //raise back up
				pApp->printer.Move(true, &tmpIdx);	//execute the motion
				segmentsProcessed++;
				TRACE("JogTo(%f,%f,%f)\n", (org+v1).x, (org+v1).y, (org+v1).z);

				CPath newpath;

				// create temporary copy of path, and edit to insert pushout and suckback

				CPath pathpos = layer[lyr].region[rgn].path[pth];
				//we convert all paths to hardware type here for consistent handling
				//closed paths will have end point duplicated rather than implicit wrapping
				if(pathpos.IsClosed()) pathpos.v.Add(pathpos.v[0]);
				pathpos.type = PATHTYPE_HARDWARE;

				CArray<double,double> tooldelta;
				//tool plunger motion calculated based on path segment length
				//but for consistent handling, we keep the arrays the same length
				//and register tooldelta[1] with the segment between the 0 and 1st vertices of pathpos
				tooldelta.SetSize(pathpos.v.GetSize());

				//cache some tool and printer properties - we may need to modify them for unusual paths
				//duration of pushout and suckback must not exceed duration of entire path
				double disprate = pApp->printer.GetCurTool()->disprate; //mm PLUNGER motion per mm of path
				double pushout = pApp->printer.GetCurTool()->pushout; //seconds
				double suckback = pApp->printer.GetCurTool()->suckback; //seconds
				double suckbackDelay = pApp->printer.GetCurTool()->suckbackDelay; //seconds delay in onset of suckback
				double pitch = pApp->printer.GetCurTool()->mmps; //mm per step
				double pathspeed = pApp->printer.GetCurTool()->GetPathspeed(); //mm/s along path
				double pathwidth = pApp->printer.GetCurTool()->pathwidth;
				
				//calculate the path properties
				double pathlen = pathpos.GetLength();
				//duration of the path in seconds from nominal start of material flow to stop
				double pathdur = pathlen / pathspeed + suckback + suckbackDelay;

				//check for problematic path too short too reach steady state
				double ssDur = suckback + suckbackDelay + 10.0*pathwidth/pathspeed; //try to estimate duration of a path which reaches steady state
				if(pathdur < ssDur)
				{
					//adjust parameters to allow steady state to be achieved
					double scale = pathdur/ssDur;
					//reduce pathspeed to allow flow to reach steady state
					pathspeed *= scale;
				}



				//adjust coordinate transform to robot coords,
				//and offset it to the build origin
				for (int seg = 0; seg < pathpos.v.GetSize()-1; seg++)
				{
					CVec v0 = pApp->printer.ModelToRobotTfm(pathpos.v[seg]);
					v1 = pApp->printer.ModelToRobotTfm(pathpos.v[seg+1]);
					double seglen = (v1-v0).Length();
					tooldelta[seg+1] = seglen*disprate; //remember tooldelta is associated with the end vtx of the segment
					pathpos.v[seg] = org+v0;
				}
				//remember to copy and offset the last vertex in the path
				pathpos.v[pathpos.v.GetSize()-1] = org+v1;












				//// insert pushout segment
				if (pApp->printer.GetCurTool()->pushout > 0)
				{
					// mm_pushout = predisp_sec*steps_per_sec*mm_per_step
					double mmpushout = pushout*OLD_MSPS*pitch;

					



					CVec firstpoint = pathpos.v[0];
					pathpos.v.InsertAt(0, firstpoint); // duplicate first point
					tooldelta.InsertAt(1, mmpushout); // add syringe plunger move, associated with end vts of segment
					TRACE("pushout: %0.2f\n", mmpushout);
				}






















				// insert suckback segment
				if (pApp->printer.GetCurTool()->suckback > 0) 
				{
					//calculate the distance to move the plunger (in mm)
					double mmplungersuckback = suckback*OLD_MSPS*pitch;
					//calculate the distance from the end of the nominal path at which to start suckback
					double suckpathdist = (suckback-suckbackDelay)*pathspeed;
					//and the total path length required for complete suckback
					double sucktotdist = suckback*pathspeed;				
					TRACE("mmplugsb: %0.2f, pathlen: %0.2f, suckdist: %0.2f\n",mmplungersuckback, pathlen, suckpathdist);
					//if the total path length is larger than the suckback duration
					if (pathlen>suckpathdist)
					{
						//find or insert a point in the path at which to begin suckback
						CVec newpt;
						if(pathpos.AddSplitPoint(pathlen-suckpathdist, 0.1, suckStartIdx, newpt))
						{
							//if a new point was inserted, update tooldelta
							TRACE("pathpos.GetSize:%d, suckStartIdx:%d\n", pathpos.v.GetSize(), suckStartIdx); 
							//update the plunger motion for the now shorter segment before the inserted point
							double seglen;
							seglen = (pathpos.v[suckStartIdx]-pathpos.v[suckStartIdx-1]).Length();
							tooldelta.InsertAt(suckStartIdx,seglen*disprate);
							TRACE("seglen*disprate for partial seg:%0.2f\n", seglen*pApp->printer.GetCurTool()->disprate);
						}
						//now distribute the suckback over the remaining segments of the path
						for (int j=suckStartIdx; j< pathpos.v.GetSize()-1; j++) 
						{
							double seglen = (pathpos.v[j+1]-pathpos.v[j]).Length();
							tooldelta[j+1] =  -mmplungersuckback*seglen/sucktotdist;
							TRACE("seglen[%d]= %0.2f, tooldelta[%d] = %0.2f\n",j, seglen, j, -mmplungersuckback*seglen/sucktotdist);
						}

						//finally, if there was a positive suckbackDelay, such that suckback extends beyond the end of the path
						//then insert moves to keep traversing or hold position until suckback has stopped
						if(suckbackDelay > 0)
						{
							double delayDist = suckbackDelay*pathspeed;
							if(layer[lyr].region[rgn].path[pth].type == PATHTYPE_CONTOUR)
							{
								//for contour paths, keep traversing the contour (wrap around)
								//keep adding path segments from the front of the path until
								//enough distance is added; add a partial segment if necessary
								CVec endPt;
								int insertIdx = -1;
								CPath tmpPath = pathpos;
								bool inserted = tmpPath.AddSplitPoint(delayDist, 0.1, insertIdx, endPt);

								int lastIdx = 1;
								for(int i = 2; i <= insertIdx; i++)
								{
									double seglen = (tmpPath.v[i]-tmpPath.v[lastIdx]).Length();
									pathpos.v.Add(tmpPath.v[i]);
									tooldelta.Add(-mmplungersuckback*seglen/sucktotdist);
									lastIdx = i;
								}
								//ensure full stop
								pathpos.v.Add(tmpPath.v[insertIdx]);
								tooldelta.Add(0.0);
							}
							else 
							{
								//for raster fill and vector paths
								//just hold the final position until suckback is complete
								pathpos.v.Add(pathpos.v[pathpos.v.GetSize()-1]);
								tooldelta.Add(-mmplungersuckback*delayDist/sucktotdist);
								//then move forward a couple pathwidths along last vector in the XY plane to break strand
								CVec scrape = pathpos.v[pathpos.v.GetSize()-1]-pathpos.v[pathpos.v.GetSize()-2];
								scrape = scrape.Normalized();
								pathpos.v.Add(pathpos.v[pathpos.v.GetSize()-1]+scrape*pApp->printer.GetCurTool()->pathwidth);
								tooldelta.Add(0.0);
							}
						}
					}
				}
				else
					AfxMessageBox("No Suckback Inserted",MB_OK);
				for(int vtx = 0; vtx < pathpos.v.GetSize(); vtx++)
				{
					msg1.Format("Fabricate()");
					msg2.Format("\t\t%d\t%d\t%0.3f\t%0.3f\t%0.3f\t%0.3f\n", pathpos.v.GetSize(), tooldelta.GetSize(),
						pathpos.v[vtx].x,pathpos.v[vtx].y,pathpos.v[vtx].z, tooldelta[vtx]);
					pApp->printer.Log(msg1,msg2);
				}





				CArray<DWORD,DWORD> indices;
				indices.RemoveAll();
				indices.SetSize(pathpos.v.GetSize());
				indices[0] = 0;
				// execute the path
				for (int vtx = 1; vtx < pathpos.v.GetSize(); vtx++)
				{
					//send the motion distances for carriage and tool
					pApp->printer.JogToolBy(tooldelta[vtx]);
					pApp->printer.JogCarriageTo(pathpos.v[vtx]);
					indices[vtx] = 0;
					//now execute them in parallel
					pApp->printer.Move(pApp->printer.GetMaxPathSpeed()/pathspeed, true, &(indices[vtx]));
					
					
					
					
					
					segmentsProcessed++;
					
					
					
					
					
					
					
					
					//if we are emulating, then pausing needs to happen inside this vertex loop
					//otherwise it will not be possible until the end of a  path
					if(bPause && pApp->printer.bEmulateHardware)
					{
						SetPauseStart();
						CVec tmp = pApp->printer.GetCurrentPos(false);
						CString msg1,msg2;
						msg1.Format("Fabricate()");
						msg2.Format("Pausing");
						pApp->printer.Log(msg1,msg1);
						bFabPaused = true;
						pApp->printer.PauseBuild(true,true);
						//find the commanded position associated with the last command executed
						int i = 0;
						while( (i < indices.GetSize()) && (indices[i] < pApp->printer.GetLastCmdExecuted()) )
						{
							lastCommandedPos = pathpos.v[i]+pApp->printer.GetOrigin();
							i++;
						}
						while(bPause && !bAbortRequested)
							Sleep(10);
						if (bAbortRequested) return Abort();
						bFabPaused = false;
						pApp->printer.ResumeBuild(false,true);
						tmp = pApp->printer.GetCurrentPos(false);
						TRACE("FabAfterResume:(%0.2f,%0.2f,%0.2f)\n",tmp.x,tmp.y,tmp.z);
						//restore the last known commanded position
						pApp->printer.SetCurrentCmdPos(lastCommandedPos);
						SetPauseEnd();
					}
					if (bAbortRequested) return Abort();
				}












				//if we are using the hardware
				if(pApp->printer.bHardwareInitialized && !pApp->printer.bEmulateHardware)
				{
					//wait for the last command in the path to finish
					int i = 0;
					do
					{	
						m_sStatusMsg.Format("Waiting for path %d of layer %d to finish", pth+1, lyr+1); 
						TRACE("Waiting for path to finish:%d < %d?\n", pApp->printer.GetLastCmdExecuted(), pApp->printer.GetLastCmdQueued());
						//handle aborts and pausing
						if (bAbortRequested) return Abort();
						//trigger an automatic pause if appropriate
						if(pApp->printer.GetCurTool()->RequestPause(pathsSincePause))
						{
							RequestPause();
						}
						if(bPause)
						{
							//first, pause the queue
							pApp->printer.PauseQueue();
							//wait for motion to stop
							pApp->printer.WaitForMoveDone();
							bool bAutoPause = pApp->printer.GetCurTool()->RequestPause(pathsSincePause);
							if(bAutoPause)
							{
								if(pApp->printer.GetAutoresume())
									m_sStatusMsg.Format("Automatic pause to calibrate - will autoresume");
								else
									m_sStatusMsg.Format("Automatic pause to calibrate - hit <Fabrication->Resume Printing> when ready");
							}
							else //if a manual pause, never autoresume
							{
								m_sStatusMsg.Format("Fabrication paused by user - hit <Fabrication->Resume Printing> when ready");
							}
							pathsSincePause = 0;
							SetPauseStart();
							CVec tmp = pApp->printer.GetCurrentPos(false);
							CString msg1,msg2;
							msg1.Format("Fabricate()");
							msg2.Format("Pausing");
							pApp->printer.Log(msg1,msg1);
							bFabPaused = true;
							//now pause the build
							bool bPathEnd = pApp->printer.IsQEmpty();
							pApp->printer.PauseBuild(true,bPathEnd);
							//if this was an autopause, and autoresume is on
							if(pApp->printer.GetAutoresume() && bAutoPause)
								RequestResume();

							while(bPause && !bAbortRequested)
									Sleep(10);

							m_sStatusMsg.Format("Fabrication resumed");
							if (bAbortRequested) return Abort();
							bFabPaused = false;
							//now resume the build; queue will be resumed below
							msg1.Format("Fabricate()");
							msg2.Format("Resuming");
							pApp->printer.Log(msg1,msg2);
							pApp->printer.ResumeBuild(false, bPathEnd);
							
							pApp->printer.ResumeQueue();
							SetPauseEnd();
						}
						if (!pApp->printer.IsQEmpty() && pApp->printer.IsPaused())
						{
							TRACE("Trying to restart queue...\n");
							pApp->printer.ResumeQueue();
							pApp->printer.WaitForStatusUpdate(2);
						}
						//if the queue is empty and motion has stopped, then increment a timout counter
						if(pApp->printer.IsQEmpty() && !pApp->printer.IsMoving() && !pApp->printer.IsToolMoving())
						{
							m_sStatusMsg.Format("Running Timeout Counter...");
							i++;
						}
						else i = 0;
						//drop out if abort is requested
						if (bAbortRequested) return Abort();
						//otherwise, wait for some info from the hardware
						pApp->printer.WaitForStatusUpdate();

						//find the commanded position associated with the last command executed
						int i = 0;
						bool bFound = false;
						TRACE("Fabrication: indices.GetSize()=%d\n", indices.GetSize());
						while( i < indices.GetSize() && !bFound)
						{
							if(indices[i] == pApp->printer.GetLastCmdExecuted())
							{
								lastCommandedPos = pathpos.v[i];
								TRACE("Fabrication: indices[%d]=%d LastCmdExecuted= %d, LastCommandedPos(%f,%f,%f)\n",
								i, indices[i], pApp->printer.GetLastCmdExecuted(), 
								lastCommandedPos.x, lastCommandedPos.y, lastCommandedPos.z);
								bFound = true;
							}
							i++;
						}
						
						if(bFound)
							pApp->printer.SetCurrentCmdPos(lastCommandedPos);	
						else
							pApp->printer.SetCurrentCmdPos(pApp->printer.GetCurrentPos(false));
						
					} while(((pApp->printer.GetLastCmdExecuted() < (DWORD)(lastQueued=pApp->printer.GetLastCmdQueued())) || 
						(!pApp->printer.IsQEmpty() || pApp->printer.IsMoving() || pApp->printer.IsToolMoving() || !pApp->printer.InPosition())) 
						&& (i < TIMEOUT_ITER));
					//if we timed out, request an abort?
					if(i>=TIMEOUT_ITER)
					{
						bAbortRequested = (IDYES == AfxMessageBox("Fabrication Loop Timeout: Abort Build?",MB_YESNO));
					}						

					m_sStatusMsg.Format("Path %d, Layer %d, %s", pth,lyr, pApp->printer.InPosition()?"In position":"Not in position");
					CString msg1, msg2;
					msg1.Format("Fabricate()");
					msg2.Format("Last Executed: %d, Last Queued: %d, Queue Empty: %d",
						pApp->printer.GetLastCmdExecuted(), pApp->printer.GetLastCmdQueued(), pApp->printer.IsQEmpty());
					pApp->printer.Log(msg1,msg2);
				} //using hardware
			} //for all paths
			if (bAbortRequested) return Abort();
		} //for all regions
	} //for all layers

	if (bAbortRequested) return Abort();
	// TODO: Model cleanup stage here
	m_sStatusMsg.Format("Fabrication complete");
	//model successfully completed, so mark as built
	pModel->IsBuilt(true);
	//finish at the home location
	pApp->printer.GotoHome();
	pApp->printer.FindHomePos();

	bAbortRequested = false;
	bFabricating = false;
	bFabPaused = false;
	bPause = false;
}


//---------------------------------------------------------------------------
void CFabrication::DrawLayers(CModel *pModel, int firstlayer, int lastlayer, bool bShowPaths, bool bShowTrace)
//---------------------------------------------------------------------------
{
	if (layer.GetSize() == 0) 
		return;

	if (firstlayer == -1 && lastlayer == -1)
	{
		firstlayer = 0;
		lastlayer = layer.GetSize() -1;
	}

	if(!(firstlayer >= 0 && firstlayer < layer.GetSize())) return;
	if(!(lastlayer >= 0 && lastlayer < layer.GetSize())) return;
	if(!(lastlayer >= firstlayer)) return;

	for (int i=firstlayer; i<=lastlayer; i++) {
		CSingleLock layerLock(&layer[i].m_mutAccess);
		layerLock.Lock();
		layer[i].Draw(pModel, bShowPaths, bShowTrace );
	}
}

//---------------------------------------------------------------------------
double CFabrication::GetModelCurHeight()
//---------------------------------------------------------------------------
{
	//get the current max height of the model
	//stored in the CPrinter class as a hack for now
	//TODO: move all path planning into a planning module
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	return pApp->printer.m_dModelClearance;
}

//---------------------------------------------------------------------------
void CFabrication::SetModelCurHeight(double curHt)
//---------------------------------------------------------------------------
{
	//set the current max height of the model
	//stored in the CPrinter class as a hack for now
	//TODO: move all path planning into a planning module
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	if((curHt >= 0.0) && (curHt < 300.0))
		pApp->printer.m_dModelClearance = curHt;
}

//---------------------------------------------------------------------------------------------//
bool CFabrication::NestBoundaries( CArray<CPath,CPath>& boundaries, CArray<CBoundaryTree,CBoundaryTree>& forest )
//---------------------------------------------------------------------------------------------//
{
	//Generate tree of boundaries by nesting depth
	//e.g. b and c inside a, and d inside c
	//                          a
	//						   / \
	//                        b   c
	//							   \
	//								d
	const int n = boundaries.GetSize();
	BOOL* relation = new BOOL[n*n]; //matrix of bits to hold nesting relation info
	//could be more compact, since a in b implies b !in a, but this is simpler
	//relation[i][j] = 1 means boundary[i] is in boundary[j]

	//also allocate an array to hold the rowsums
	int* rsum = new int[n];
	//if unable to allocate memory, return false
	if((relation == NULL) || (rsum == NULL)) return false; 
	int i,j,k;

	for(i = 0; i < n; i++)
	{
		for(j = 0; j < n; j++)
		{
			if(j != i)
			{
				//determine nesting of paths
				if(boundaries[j].PathInside( boundaries[i] ))
					relation[i*n+j] = 1;
				else
					relation[i*n+j] = 0;
			}
			else
				//we'll define a boundary to not be inside itself
				relation[i*n+j] = 0;
		}
	}
	//outermost boundaries are the tree roots, and have a relation row sum of zero
	//leaf boundaries (innermost) have relation column sums of zero
	for(i = 0; i < n; i++)
	{
		//calculate the row sums
		rsum[i] = 0;
		for(j = 0; j < n; j++)
			rsum[i] += relation[i*n+j];
	}

	//go down each row
	for(i = 0; i < n; i++ )
	{
		//go down the column and find the children, and remove indirect relations
		for(j = 0; j < n; j++)
		{
			//if boundary[j] is a descendant of boundary[i]
			if( relation[j*n+i] == 1 )
			{
				if( rsum[j] > 1 )
				{
					//so boundary[j] has multiple ancestors (including boundary[i])
					//check whether the other ancestors are also descendants of boundary[i]
					//if so, remove the indirect relations and update the rowsum[j]
					for(k = 0; k < n; k++)
					{
						if( (k!= i) && (relation[j*n+k] == 1) )
						{
							//if j is in k, and k is in i, remove the j-i (indirect) relationship
							if( relation[k*n+i] == 1 )
								relation[j*n+i] = 0;
							else 
								//otherwise remove the j-k (indirect) relationship
								relation[j*n+k] = 0;
							//reduce the rowsum
							rsum[j]--;
						}		
					}
				}
			}
		}
	}
	//now all rowsums should be 0 or 1 (root or one parent)
	//so load up the trees
	CBoundaryTree tempTree;
	bool foundRoot = false;
	for(i = 0; i<n; i++)
	{
		if( rsum[i] == 0 )
		{
			int rootIdx = tempTree.InsertRootNode(boundaries[i]);
			//call recursive insertion function, -1 indicates first insertion is root
			if(!NestTreeInsert( tempTree, rootIdx, relation, boundaries, i )) return false;
			forest.Add(tempTree);
			foundRoot = true;
		}
	}
	ASSERT(foundRoot);
	//now release the allocated memory
	delete relation;
	delete rsum;
	return true;
}		

//---------------------------------------------------------------------------------------------//
bool CFabrication::NestTreeInsert( CBoundaryTree& tree, int treeParent, BOOL* relation, 
										CArray<CPath,CPath>& boundaries, int parentBoundaryIdx )
//---------------------------------------------------------------------------------------------//
{
	//recursively insert the child boundaries of parentIdx, as indicated by columns in relation, into tree
	int n = boundaries.GetSize();
	for( int i = 0; i < n; i++ )
	{
		//if boundaries[i] is child of boundaries[parentIdx], insert boundaries[i] and its children
		//then the for loop will take care of siblings, so this is depth-first insertion
		if( (i != parentBoundaryIdx) && (relation[i*n+parentBoundaryIdx] == 1) )
		{
			int treeChild = tree.InsertChildNode( treeParent, boundaries[i] );
			if(!NestTreeInsert( tree, treeChild, relation, boundaries, i ))
				return false;
		}
	}
	return true;
}
