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

// Model.h: interface for the CModel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODEL_H__C122C004_939F_447F_8607_14253E7A0DD9__INCLUDED_)
#define AFX_MODEL_H__C122C004_939F_447F_8607_14253E7A0DD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CModel : public CObject  
{
public:
	void FlushTraces(); //flush out any stored trace data
	void TracePoint(CVec& v, bool bNewTrace = false, double tol2 = 0.); //capture v as a point in the path actually traced by hardware
	bool IsSlicingCancelled(){return m_bCancelSlicing;};
	void CancelSlicing(){m_bCancelSlicing = true;};
	bool IsSlicing(){return m_bSlicing;}; //is the model currently being sliced?
	bool IsBuildReady(); //is model ready to be built (paths have been planned)
	bool IsBuilt(){return m_bBuilt;}; //has model been built already, but still resides on the build surface
	void IsBuilt(bool built){m_bBuilt = built;};
	void InvalidateBuild(){m_bBuildReady = false;}; //declare the model unready to be built
	bool IsProcessReady(); //is model ready to be processed (all parameters have been assigned
	void InvalidateProcess(){m_bProcessReady = false;}; //declare the mode unready to be processed
	CString GetBuildInfo(void);
	bool CalcModelStatistics();
	int GetEstNumToolChanges(){return m_nToolChangesEst;};
	int GetNumToolChanges(){return m_nToolChanges;};
	void IncrementToolChanges(){m_nToolChanges++;};
	int GetToolCount(){return m_nToolCount;};
	void MoveToOrigin(bool all = false);
	void ComputeBoundingBox(CVec &pmin, CVec &pmax);
	int FindChunk(int id);
	int GetChunkSelected(int n=0);
	int CountChunksSelected();
	void SelectAll(bool bSelect = true);
	bool SelectOneChunk(int n);
	bool SelectOneChunk(CString &name);
	int GetNewId();
	bool Slice(double maxSegLen = -1.0);
	void FocusCentroid();
	bool Flush();
	CModel();
	virtual ~CModel();
	void Serialize(CArchive& ar);
	CModel(CModel& s);
	CModel& operator=(const CModel& s);
	CString& GetStatusMsg(){return m_sStatusMsg;};
	void SetStatusMsg(CString& msg);
	CArray<CChunk, CChunk> chunk;
	CFabrication fab;
	
	CVec focus;
	int nextid;
	CMutex m_mutAccess; //thread safety
protected:
	bool m_bActive; //mark the active model (the one currently selected or being fabricated)
	int m_nToolChangesEst; //the estimated number of tool changes required for the model
	int m_nToolChanges; //the number of tool changes so far
	bool m_bBuildReady; //is model ready to be built (paths have been planned)
	bool m_bProcessReady; //is model ready to be processed (all parameters have been assigned
	bool m_bSlicing;	//is model currently being sliced?
	bool m_bCancelSlicing;	//request cancellation of slicing
	int m_nToolCount; //the number of different tools needed to build this model
	bool m_bBuilt; //has this model been built already, but still resides on the build surface?
	CTimeSpan m_ctsTimeEst; //estimated time to fabricate the current model
	CTimeSpan m_ctsTCTimeEst; //estimated average time for a tool change
	CString m_sMaterialVol; //string description of all material volume needed for the model
	CString m_sStatusMsg; //message to inform user of state of model
};

#endif // !defined(AFX_MODEL_H__C122C004_939F_447F_8607_14253E7A0DD9__INCLUDED_)
