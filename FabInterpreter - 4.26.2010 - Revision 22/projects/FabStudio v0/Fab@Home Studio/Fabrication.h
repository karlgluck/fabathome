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

// Fabrication.h: interface for the CFabrication class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FABRICATION_H__20DB0430_6E7A_4076_8F35_3708A0E24A92__INCLUDED_)
#define AFX_FABRICATION_H__20DB0430_6E7A_4076_8F35_3708A0E24A92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "afxmt.h" //for multithreading support

class CModel;
class CLayer;

class CFabrication : public CObject  
{
public:
	bool NestBoundaries( CArray<CPath,CPath>& boundaries, CArray<CBoundaryTree,CBoundaryTree>& forest );
	bool NestTreeInsert( CBoundaryTree& tree, int treeParent, BOOL* relation, 
										CArray<CPath,CPath>& boundaries, int parentBoundaryIdx );
	void Fabricate(CModel *pModel);
	CFabrication();
	virtual ~CFabrication();
	void Serialize(CArchive& ar);
	CFabrication(CFabrication& s);
	CFabrication& operator=(const CFabrication& s);
	void Abort(); //terminate the fabrication process, stop all motion
	bool Flush(); //reset the fabrication, flush all data
	void DrawLayers(CModel *pModel, int firstlayer, int lastlayer, bool bShowPaths, bool bShowTrace);
	int GetNumLayers(){return layer.GetSize();};
	int GetCurrentLayer(){return m_nCurrentLayer;};
	void SetCurrentLayer(int lyr){m_nCurrentLayer = lyr;};
	int GetCurrentPath(){return m_nCurrentPath;};
	void SetCurrentPath(int pth){m_nCurrentPath = pth;};
	void RequestPause(){bPause = TRUE;};
	void RequestResume(){bPause = FALSE;};
	bool IsPaused(){return bFabPaused;};
	bool PauseRequested(){return bPause;};
	bool IsFabricating(){return (bool)bFabricating;};
	void SetFabricating(bool fab){bFabricating = (BOOL)fab;};
	double GetModelCurHeight();
	void SetModelCurHeight(double curHt);
	void RequestAbort(){bAbortRequested = true;};
	void ClearAbort(){bAbortRequested = false;};
	bool IsAbortRequested(){return bAbortRequested;};
	bool IsReady(){return m_bReady;};
	void SetReady(bool ready){m_bReady = ready;};
	void SetFabStart(){m_ctFabStart = CTime::GetCurrentTime();};
	void SetPauseStart(){m_ctPauseStart = CTime::GetCurrentTime();};
	void SetPauseEnd();
	CTime& GetFabStart(){return m_ctFabStart;};
	CTime& GetPauseStart(){return m_ctPauseStart;};
	CTimeSpan& GetPauseTotal(){return m_ctsPauseTotal;};
	CTimeSpan& GetFabTotal();
	CString& GetStatusMsg(){return m_sStatusMsg;};

	CArray<CLayer,CLayer> layer;
	CMutex m_mutAccess; //mutex object for thread safety
protected:
	CTime m_ctFabStart; //time at start of fabrication
	CTime m_ctPauseStart; //time at start of most recent pause
	CTimeSpan m_ctsPauseTotal; //total time span spent in all pauses
	CTimeSpan m_ctsFabTotal; //total elapsed time of fabrication
	bool m_bReady; //is fabrication ready to begin (e.g. planning is done)
	int m_nCurrentLayer;
	int m_nCurrentPath;
	bool bFabricating;
	bool bPause; //flag set outside of the fab thread to request pause/resume fabrication
	bool bFabPaused; //flag set inside of the fabrication function to indicate status
	bool bAbortRequested;
	CString m_sStatusMsg; //a status message to inform user of events during fabrication
};

#endif // !defined(AFX_FABRICATION_H__20DB0430_6E7A_4076_8F35_3708A0E24A92__INCLUDED_)
