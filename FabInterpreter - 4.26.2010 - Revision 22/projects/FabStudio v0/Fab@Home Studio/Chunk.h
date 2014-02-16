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

// Chunk.h: interface for the CChunk class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHUNK_H__69590D31_6CF6_4E3F_9EA1_3EF5CCCB3F15__INCLUDED_)
#define AFX_CHUNK_H__69590D31_6CF6_4E3F_9EA1_3EF5CCCB3F15__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CModel;

enum CHUNKTYPE {
	CHUNKTYPE_UNDEFINED,
	CHUNKTYPE_STL,
	CHUNKTYPE_VCT
};

class CChunk : public CObject  
{
public:
	CChunk();
	virtual ~CChunk();
	void Serialize(CArchive& ar);
	CChunk(CChunk& s);
	CChunk& operator=(const CChunk& s);
	bool GetBoundFirst(){return m_bBoundFirst;};
	void SetBoundFirst(bool bBoundFirst);
	bool IsProcessReady();	//is the chunk ready to be processed?
	bool IsBuildReady();	//is the chunk processed and ready to be built?
	void ComputeBoundingBox(CVec &pmin, CVec &pmax);
	bool CalcChunkStats(CModel *pModel);
	int GetPriority(){return m_nPriority;};
	void SetPriority(int priority);
	double GetTheoreticalVolume(){return m_dVtheor;};
	double GetMaterialVolume(){return m_dVmat;};
	double GetNumLayers(){return m_nNumLayers;};
	double GetPathTravel(){return m_dPathTravel;};
	double GetJogTravel(){return m_dJogTravel;};
	CTimeSpan& GetTimeEst(){return m_ctsTimeEst;};
	void Draw(CModel *pModel, bool bModelhNormals, bool bShaded);
	int CountSlices();
	bool Slice(CModel *pModel, CProgressDlg* pProgDlg);
	bool SliceVectors(CModel *pModel);
	bool GetPathProperties(double& thick, double& width, double& minLen);
	void ToggleSelect(){selected = !selected;};
	void Select(){selected = true;};
	void Deselect(){selected = false;};
	bool IsSelected(){return selected;};
	CString &GetName(){return m_sName;};
	void SetName(CString &name);
	void SetType(CHUNKTYPE type){m_ctType = type;};
	CHUNKTYPE GetType(){return m_ctType;};
	int id;
	CGeometry geometry;
	CString materialname;
	CString toolname;
	CVec color;
	double alpha;
	CMutex m_mutAccess; //thread safety
protected:
	double m_dVtheor;
	double m_dVmat;
	int m_nNumLayers;
	double m_dPathTravel;
	double m_dJogTravel;
	CTimeSpan m_ctsTimeEst;
	int m_nPriority; // For backfill deposition priority - lower numbers will be built first
	CString m_sName; //the name of the chunk (from filename)
	bool selected;
	bool m_bBoundFirst; //should boundary paths be deposited prior to raster fill
	bool m_bBuildReady; //is the chunk processed and ready to be built?
	CHUNKTYPE m_ctType; //the type of geometry in the chunk
};

#endif // !defined(AFX_CHUNK_H__69590D31_6CF6_4E3F_9EA1_3EF5CCCB3F15__INCLUDED_)
