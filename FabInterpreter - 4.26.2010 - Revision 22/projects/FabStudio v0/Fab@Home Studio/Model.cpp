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

// Model.cpp: implementation of the CModel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FABOutput.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------
CModel::CModel()
//--------------------------------------------------------------------
{

	nextid = 100;
	m_bBuilt = false;
	m_bBuildReady = false; //is model ready to be built (paths have been planned)
	m_bProcessReady = false; //is model ready to be processed (all parameters have been assigned
	m_bSlicing = false;
	m_bCancelSlicing = false;
	m_nToolChanges = 0;
	m_nToolChangesEst = 0;
	m_ctsTimeEst = 0;
	m_ctsTCTimeEst = 0;
	m_nToolCount = 0;
}

//--------------------------------------------------------------------
CModel::~CModel()
//--------------------------------------------------------------------
{

}

//--------------------------------------------------------------------
bool CModel::Flush()
//--------------------------------------------------------------------
{
	//flush out the model details because a change has occurred

	CSingleLock accessLock(&fab.m_mutAccess);
	accessLock.Lock();
	FocusCentroid();
	fab.Flush(); //reset the fabrication
	InvalidateBuild();
	m_nToolChanges = 0;
	m_nToolChangesEst = 0;
	m_ctsTimeEst = 0;
	m_ctsTCTimeEst = 0;
	m_nToolCount = 0;
	//clear all of the needed volume for tools
	for(int i = 0; i < CTool::LoadedToolCount(); i++)
	{
		CTool::GetByIndex(i)->SetVolumeNeeded(0);
	}
	m_sMaterialVol.Empty();
	m_sStatusMsg.Format("Model flushed");
	m_bSlicing = false;
	m_bCancelSlicing = false;
	return true;
}

//---------------------------------------------------------------------------
void CModel::TracePoint(CVec& v, bool bNewTrace, double tol2)
//---------------------------------------------------------------------------
{
	//if v is 
	//capture v as a point in the path actually traced by hardware

	CSingleLock modelLock(&m_mutAccess);
	modelLock.Lock();
	CSingleLock fabLock(&fab.m_mutAccess);
	fabLock.Lock();
	if(!IsBuildReady() || !fab.IsFabricating()) return;
	int lyr = fab.GetCurrentLayer();
	CSingleLock lyrLock(&fab.layer[lyr].m_mutAccess);
	lyrLock.Lock();
	int rgn = fab.layer[lyr].GetCurRegion();
	CSingleLock rgnLock(&fab.layer[lyr].region[rgn].m_mutAccess);
	rgnLock.Lock();
	if((fab.layer[lyr].region[rgn].trace.GetSize() < 1) || bNewTrace)
	{
		CPath tmp;
		tmp.type = PATHTYPE_TRACEDEP;
		tmp.v.Add(v);
		fab.layer[lyr].region[rgn].trace.Add(tmp);
	}
	else
	{
		//don't add unless we've moved since last
		CVec last = fab.layer[lyr].region[rgn].trace[0].v[fab.layer[lyr].region[rgn].trace[0].v.GetSize()-1];
		if(v.Dist2(last) > tol2 )
		{
			fab.layer[lyr].region[rgn].trace[0].v.Add(v);
		}
	}
}

//---------------------------------------------------------------------------
void CModel::FlushTraces()
//---------------------------------------------------------------------------
{
	//flush out any stored trace data
	for(int i = 0; i < fab.layer.GetSize(); i++)
	{
		for(int j = 0; j < fab.layer[i].region.GetSize(); j++)
			fab.layer[i].region[j].FlushTrace();
	}
}


//---------------------------------------------------------------------------
void CModel::Serialize(CArchive& ar)
//---------------------------------------------------------------------------
{// Save / Load

	int version = 10;

	if (ar.IsStoring()) { //add storing code here

		ar << version;

		// save data

		chunk.Serialize(ar);
		focus.Serialize(ar);
		fab.Serialize(ar);
		ar << m_bBuildReady;
		ar << m_bProcessReady;
		ar << nextid;

		ar << version;

	}	else	{  // add loading code here

		int version1, version2;
		ar >> version1; if (version1 < 10 || version1 > version) throw 6;

		// load data

		chunk.Serialize(ar);
		focus.Serialize(ar);
		fab.Serialize(ar);
		ar >> m_bBuildReady;
		ar >> m_bProcessReady;
		ar >> nextid;

		ar >> version2; if (version2 != version1) throw 6;

	}
}

//---------------------------------------------------------------------------
CModel::CModel(CModel& s)
//---------------------------------------------------------------------------
{
	
	chunk.Copy(s.chunk);
	fab = s.fab;
	focus = s.focus;
	nextid = s.nextid;
	m_bBuilt = s.m_bBuilt;
	m_bBuildReady = s.m_bBuildReady;
	m_bProcessReady = s.m_bProcessReady;
	m_nToolChanges = s.m_nToolChanges;
	m_nToolChangesEst = s.m_nToolChangesEst;
	m_ctsTimeEst = s.m_ctsTimeEst;
	m_ctsTCTimeEst = s.m_ctsTCTimeEst;
	m_nToolCount = s.m_nToolCount;
	m_sStatusMsg = s.m_sStatusMsg;
	m_bActive = s.m_bActive;
}

//---------------------------------------------------------------------------
CModel& CModel::operator=(const CModel& s)
//---------------------------------------------------------------------------
{

	chunk.Copy(s.chunk);
	fab = s.fab;
	focus = s.focus;
	nextid = s.nextid;
	m_bBuilt = s.m_bBuilt;
	m_bBuildReady = s.m_bBuildReady;
	m_bProcessReady = s.m_bProcessReady;
	m_nToolChanges = s.m_nToolChanges;
	m_nToolChangesEst = s.m_nToolChangesEst;
	m_ctsTimeEst = s.m_ctsTimeEst;
	m_ctsTCTimeEst = s.m_ctsTCTimeEst;
	m_nToolCount = s.m_nToolCount;
	m_sStatusMsg = s.m_sStatusMsg;
	m_bActive = s.m_bActive;

	return *this;
}

//---------------------------------------------------------------------------
void CModel::FocusCentroid()
//---------------------------------------------------------------------------
{

	CVec s;
	int cnt = 0;

	for (int i = 0; i<chunk.GetSize(); i++) {
		for (int j=0; j<chunk[i].geometry.mesh.GetSize(); j++) {
			for (int k=0; k<3; k++) {
				s += chunk[i].geometry.mesh[j].v[k];
				cnt++;
			}
		}
	}

	if (cnt > 0)
		focus = s/cnt;

}

//---------------------------------------------------------------------------
bool CModel::IsBuildReady()
//---------------------------------------------------------------------------
{
	//is model ready to be built (paths have been planned)
	m_bBuildReady = m_bBuildReady && IsProcessReady() && fab.IsReady();

	if(m_bBuildReady)
	{
		CSingleLock accessLock(&m_mutAccess);
		accessLock.Lock();
		m_sStatusMsg.Format("Model ready for fabrication");
	}
	return m_bBuildReady;
}

//---------------------------------------------------------------------------
bool CModel::IsProcessReady()
//---------------------------------------------------------------------------
{
	//is model ready to be processed (all parameters have been assigned)
	//check that all chunks have properties assigned
	if(chunk.GetSize() < 1) return false;

	m_bProcessReady = true;
	for(int i = 0; i < chunk.GetSize(); i++)
	{
		if(!chunk[i].IsProcessReady()) m_bProcessReady = false;
	}
	//if not ready for process, then not ready for build
    if(!m_bProcessReady) InvalidateBuild();  

	if(m_bProcessReady)
	{
		CSingleLock accessLock(&m_mutAccess);
		accessLock.Lock();
		m_sStatusMsg.Format("Model ready for processing");
	}
	return m_bProcessReady;
}

//---------------------------------------------------------------------------
bool CModel::Slice(double maxSegLen)
//---------------------------------------------------------------------------
{// generate slices for each chunk

	int i;

	CProgressDlg progdlg;

	CSingleLock modelLock(&m_mutAccess);
	modelLock.Lock();
	//if model was already built, then flush it
	if(IsBuilt())
		Flush();
	IsBuilt(false);
	
	m_bSlicing = true;
	
	int slices = 0;
	for (i=0; i<chunk.GetSize(); i++) {
		int ns = chunk[i].CountSlices();
		if (ns == -1) // cant get tool parameters to compute slices 
		{
			Flush();
			m_bSlicing = false;
			return m_bSlicing;
		}
		slices += ns;
	}

	m_sStatusMsg.Format("Model being processed");

	modelLock.Unlock();
	for (i=0; i<chunk.GetSize(); i++) 
	{
		CSingleLock chunkLock(&chunk[i].m_mutAccess);
		chunkLock.Lock();
		bool ok = chunk[i].Slice(this, &progdlg);
		if (!ok)
		{
			Flush();
			m_bSlicing = false;
			return m_bSlicing;
		}
	}
	CSingleLock fabLock(&fab.m_mutAccess);
	fabLock.Lock();
	CSingleLock lyrLock(&fab.layer[i].m_mutAccess);
	lyrLock.Lock();
	//if necessary for kinematics, reduce maximum path segment length
	if(maxSegLen > 0.0)
	{
		for(i = 0; i < fab.layer.GetSize(); i++)
		{
			for(int j = 0; j < fab.layer[i].region.GetSize(); j++)
			{
				CSingleLock rgnLock(&fab.layer[i].region[j].m_mutAccess);
				rgnLock.Lock();
				for(int k = 0; k < fab.layer[i].region[j].path.GetSize(); k++)
				{
					CSingleLock pthLock(&fab.layer[i].region[j].path[k].m_mutAccess);
					fab.layer[i].region[j].path[k].Interpolate(maxSegLen);
				}
			}
		}
	}
	// sort layers by z and by priority

	//first sort by z
	bool changed = true;
	while (changed) { // buble sort is bad! TODO: quicksort
		changed = false;
		for (i=0; i<fab.layer.GetSize()-1; i++) {
			//TRACE("Layer %d org.z:%f, Layer %d org.z %f\n",i,fab.layer[i].m_coordSys.GetOrg().z,i+1,fab.layer[i+1].m_coordSys.GetOrg().z);
			if (fab.layer[i].m_coordSys.GetOrg().z > fab.layer[i+1].m_coordSys.GetOrg().z) {
				CLayer tmp;
				tmp = fab.layer[i];
				fab.layer[i] = fab.layer[i+1];
				fab.layer[i+1] = tmp;
				changed = true;
			}
		}
	}

	//now sort by priority
	changed = true;
	int curPriority, nextPriority;
	int curChnkIdx, nextChnkIdx;
	while (changed) { // bubble sort is bad! TODO: quicksort
		changed = false;
		for (i=0; i<fab.layer.GetSize()-1; i++)
		{
			curChnkIdx = FindChunk(fab.layer[i].chunkid);
			nextChnkIdx = FindChunk(fab.layer[i+1].chunkid);
			curPriority = chunk[curChnkIdx].GetPriority();
			nextPriority = chunk[nextChnkIdx].GetPriority();
			if (curPriority < nextPriority) {
				CLayer tmp;
				tmp = fab.layer[i];
				fab.layer[i] = fab.layer[i+1];
				fab.layer[i+1] = tmp;
				changed = true;
			}
		}
	}

     
	FABOutput *output = new FABOutput();
	output->addConstants();
	for(int c=0; c<fab.layer.GetSize(); c++) {
		//layers passed one at a time with their respective chunks
		output->addLayer(fab.layer[c],chunk[FindChunk(fab.layer[c].chunkid)],c); 
	}
	output->~FABOutput();

	//now highest priority layers come first, but within priority, layers are sorted by z.

	fab.SetCurrentLayer(0);
	fab.SetReady(true); //set flag in fabrication to indicate ready for execution
	//now make volume and time estimates for this model
	CalcModelStatistics();
	
	m_bBuildReady = true;
	m_bSlicing = false;
	m_sStatusMsg.Format("Slicing Complete.");
	return true;
}

//---------------------------------------------------------------------------
bool CModel::CalcModelStatistics()
//---------------------------------------------------------------------------
{
	
	//calculate statistics for this model

	//CChunk::Slice also calculates layer and chunk stats, so just tap these
	//from the CChunk and CLayer classes

	//calculate the number of tool changes
	//TODO: handle multi-tool configurations
	CFabAtHomeApp* pApp = (CFabAtHomeApp*)AfxGetApp();
	int i;
	m_nToolChangesEst = 0;
	for(i = 0; i < fab.GetNumLayers()-1; i++)
	{
		if(fab.layer[i].toolname.CompareNoCase(fab.layer[i+1].toolname) != 0)
		{
			m_nToolChangesEst++;
		}
	}
	//clear all of the needed volume for tools
	for(i = 0; i < CTool::LoadedToolCount(); i++)
	{
		CTool::GetByIndex(i)->SetVolumeNeeded(0);
	}
	//calculate chunk stats and add up all of the needed volume
	m_ctsTimeEst = 0;
	for(i = 0; i < chunk.GetSize(); i++)
	{
		chunk[i].CalcChunkStats(this);
		m_ctsTimeEst += chunk[i].GetTimeEst();
		double prevol = CTool::GetByName(chunk[i].toolname)->GetVolumeNeeded();
		double chunkvol = chunk[i].GetMaterialVolume();
		CTool::GetByName(chunk[i].toolname)->SetVolumeNeeded(prevol+chunkvol);
	}
	
	//finally generate a string with the volume needed info
	m_sMaterialVol.Empty();
	m_nToolCount = 0;
	for(i = 0; i < CTool::LoadedToolCount(); i++)
	{
		double vol = CTool::GetByIndex(i)->GetVolumeNeeded();
		if(vol>0)
		{
			m_nToolCount++;
			m_sMaterialVol.AppendFormat("%0.2fmL of %s\n", vol/1000.0, CTool::GetByIndex(i)->GetName());
		}
	}
	//estimate average time required for a tool change
	//TODO: calculate actual number of tool changes by simulating
	int fauto = min(m_nToolCount, pApp->printer.GetMaxTools());
	m_ctsTCTimeEst = CTimeSpan(((pApp->printer.GetManTCTime().GetTimeSpan()*(pApp->printer.GetMaxTools()-fauto))
		+ (pApp->printer.GetAutoTCTime().GetTimeSpan()*fauto))/pApp->printer.GetMaxTools());
	return true;
}

//---------------------------------------------------------------------------
int CModel::GetNewId()
//---------------------------------------------------------------------------
{

	nextid++;
	return nextid;

}

//---------------------------------------------------------------------------
void CModel::SetStatusMsg(CString& msg)
//---------------------------------------------------------------------------
{
	CSingleLock locker(&m_mutAccess);
	locker.Lock();
	m_sStatusMsg = msg;
}

//---------------------------------------------------------------------------
void CModel::SelectAll(bool bSelect)
//---------------------------------------------------------------------------
{

	for (int i=0; i<chunk.GetSize(); i++) {
		if(bSelect)
			chunk[i].Select();
		else
			chunk[i].Deselect();
	}
	if(bSelect)
		m_sStatusMsg.Format("All parts selected");
	else
		m_sStatusMsg.Format("Nothing selected");

}

//---------------------------------------------------------------------------
int CModel::CountChunksSelected()
//---------------------------------------------------------------------------
{

	int cnt = 0;

	for (int i=0; i<chunk.GetSize(); i++) {
		if (chunk[i].IsSelected())
			cnt++;
	}

	return cnt;
}

//---------------------------------------------------------------------------
int CModel::GetChunkSelected(int n)
//---------------------------------------------------------------------------
{// get nth selected (first is n==0)

	int cnt = 0;

	for (int i=0; i<chunk.GetSize(); i++)
	{
		if (chunk[i].IsSelected()) 
		{
			m_sStatusMsg.AppendFormat("%s", chunk[i].GetName());
			if (n==cnt)
			{
				if(cnt == 0)
					m_sStatusMsg.AppendFormat("  is selected");
				else
					m_sStatusMsg.AppendFormat(" are selected");
				return i;
			} 
			else 
			{
				cnt++;
			}
		}
	}

	return -1;
}

//---------------------------------------------------------------------------
bool CModel::SelectOneChunk(int n)
//---------------------------------------------------------------------------
{
	//set the nth chunk as selected, and deselect all others
	//return true if found; do nothing if not
	if(n >= chunk.GetSize()) return false;
	SelectAll(false);
    chunk[n].Select();
	m_sStatusMsg.Format("%s is selected", chunk[n].GetName());
	return true;
}

//---------------------------------------------------------------------------
bool CModel::SelectOneChunk(CString &name)
//---------------------------------------------------------------------------
{
	//set the named chunk as selected, and deselect all others
	//return true if found; do nothing if not
	for(int i = 0; i < chunk.GetSize(); i++)
	{
		if(name.Compare(chunk[i].GetName())==0)
			return(SelectOneChunk(i));
	}
	return false;
}

//---------------------------------------------------------------------------
int CModel::FindChunk(int id)
//---------------------------------------------------------------------------
{

	for (int i=0; i<chunk.GetSize(); i++) {
		if (chunk[i].id == id)
			return i;
	}

	return -1; 
}


//---------------------------------------------------------------------------
void CModel::ComputeBoundingBox(CVec &pmin, CVec &pmax)
//---------------------------------------------------------------------------
{

	pmin = CVec(1E10,1E10,1E10);
	pmax = CVec(-1E10,-1E10,-1E10);
	
	CVec pmin1, pmax1;

	for (int i=0; i<chunk.GetSize(); i++) {
		if (chunk[i].IsSelected()) {
			chunk[i].geometry.ComputeBoundingBox(pmin1, pmax1);
			pmin = pmin.Min(pmin1);
			pmax = pmax.Max(pmax1);
		}
	}

}

//---------------------------------------------------------------------------
void CModel::MoveToOrigin(bool all)
//---------------------------------------------------------------------------
{

	CVec tmin(1E10,1E10,1E10);
	CVec pmin, pmax;
	
	for (int i=0; i<chunk.GetSize(); i++) {
		if (chunk[i].IsSelected() || all) {
			chunk[i].geometry.ComputeBoundingBox(pmin, pmax);
			tmin = tmin.Min(pmin);
		}
	}

	for (int i=0; i<chunk.GetSize(); i++) {
		if (chunk[i].IsSelected() || all) {
			chunk[i].geometry.Translate(-tmin);
		}
	}

}

//--------------------------------------------------------------------
CString CModel::GetBuildInfo(void)
//--------------------------------------------------------------------
{	// Describe fabrication status info

	CSingleLock modelLock(&m_mutAccess);
	modelLock.Lock();
	CSingleLock fabLock(&fab.m_mutAccess);
	fabLock.Lock();
	CString msg;

	CFabAtHomeApp *pApp = (CFabAtHomeApp *)AfxGetApp();

	// Modified condition (05/28/2008)
	if (pApp->printer.EnsureCurrentTool(false))
	{
		msg.AppendFormat("-------------------MODEL---------------------\n");
		msg.AppendFormat("Parts in Model: %d\n", chunk.GetSize());
		if(fab.IsReady())
		{
			msg.AppendFormat("Layers in Model: %d\n", fab.layer.GetSize());
			msg.AppendFormat("Layers to Go: %d\n", fab.layer.GetSize()-fab.GetCurrentLayer()-1);
			if(GetEstNumToolChanges()>0)
			{
				msg.AppendFormat("Completed %d of %d tool changes\n", GetNumToolChanges(), GetEstNumToolChanges());
				msg.AppendFormat("Est. ave. tool changing time: %s\n", m_ctsTCTimeEst.Format("%H:%M.%S"));
			}
			msg.AppendFormat("Est. time to build this model: %s\n", (m_ctsTimeEst+(m_ctsTCTimeEst.GetTimeSpan()*GetNumToolChanges())).Format("%H:%M.%S"));
		}
		msg.AppendFormat("---------------CURRENT LAYER-------------------\n");
		if(fab.IsReady())
		{ 
			msg.AppendFormat("Processing layer %d of %d\n", fab.GetCurrentLayer()+1, fab.layer.GetSize()); //index from 1 for user interpretation
			msg.AppendFormat("Processing path %d of %d\n", 
				fab.GetCurrentPath()+1, fab.layer[fab.GetCurrentLayer()].GetNumPaths());
			msg.AppendFormat("Total length of paths in this layer: %.1fmm\n",fab.layer[fab.GetCurrentLayer()].GetPathTravel());
			msg.AppendFormat("Theoretical volume of this layer: %.2fmL\n", fab.layer[fab.GetCurrentLayer()].GetTheoreticalVolume()/1000.0);
			msg.AppendFormat("Material volume of this layer: %.2fmL\n", fab.layer[fab.GetCurrentLayer()].GetMaterialVolume()/1000.0);
			msg.AppendFormat("Est. time to build this layer: %s\n", fab.layer[fab.GetCurrentLayer()].GetTimeEst().Format("%H:%M.%S"));
		}
		msg.AppendFormat("----------------DIMENSIONS---------------------\n");
		if(fab.IsReady())
		{
			msg.AppendFormat("Current height of model: %3.2f mm\n", fab.GetModelCurHeight());
		}
		msg.AppendFormat("--------------TOOLS/MATERIALS------------------\n");
		msg.AppendFormat("Current Tool: %s\n", pApp->printer.GetCurToolName());
		msg.AppendFormat("Tool Path Speed: %0.2f mm/s\n", pApp->printer.GetCurTool()->GetPathspeed());
		if(pApp->printer.GetCurTool()->GetPausePathCount() > 0)
			msg.AppendFormat("Tool will pause every %d paths\n", pApp->printer.GetCurTool()->GetPausePathCount());
		if(fab.IsReady())
		{
			msg.AppendFormat("Model will require:\n%s", m_sMaterialVol);
		}
		msg.AppendFormat("--------------------TIME-----------------------\n");
		if(fab.IsReady()&&fab.IsFabricating())
		{
			msg.AppendFormat("Fabrication started: %s\n",fab.GetFabStart().Format("%c"));
			if(fab.IsPaused())
			{
				CTimeSpan pauseLen = CTimeSpan(CTime::GetCurrentTime()-fab.GetPauseStart());
				msg.AppendFormat("Paused since: %s (%s)\n", 
					fab.GetPauseStart().Format("%c"), pauseLen.Format("%H:%M.%S"));
			}
			msg.AppendFormat("Total elapsed time: %s\n", fab.GetFabTotal().Format("%H:%M.%S"));
			msg.AppendFormat("Total time paused: %s\n", fab.GetPauseTotal().Format("%H:%M.%S"));
			msg.AppendFormat("Estimated time remaining: %s\n", 
				(m_ctsTimeEst+(m_ctsTCTimeEst.GetTimeSpan()*(GetNumToolChanges()-GetEstNumToolChanges()))
				-fab.GetFabTotal()+fab.GetPauseTotal()).Format("%H:%M.%S"));
		}
	}
	else
	{
		msg.AppendFormat("No Tool Mounted\n");
	}

	return msg;
}
