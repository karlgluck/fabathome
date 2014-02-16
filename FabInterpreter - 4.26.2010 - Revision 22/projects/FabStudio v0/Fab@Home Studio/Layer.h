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

// Layer.h: interface for the CLayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LAYER_H__992394D6_A81F_4DD3_BF41_956EB09516D4__INCLUDED_)
#define AFX_LAYER_H__992394D6_A81F_4DD3_BF41_956EB09516D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCoordSys;
class CModel;
class CTool;
class CBoundaryTree;
enum CHUNKTYPE;

class CLayer : public CObject  
{
public:
	void MakeRegions();
	int MakeRegionsAux(int treeIdx, int rgnIdx, int depth, int nodeIdx);
	void CalcBBox(CVec xax, CVec yax, CVec zax, CVec& pmin, CVec& pmax);
	int RandStartCntr(); //randomize the starting point for each contour path
	double MinimizeTravel(int startIdx = 0); //sort paths in layer to minimize travel
	void Draw(CModel *pModel, bool bPaths, bool bTrace);
	bool CalcLayerStats(CHUNKTYPE type); //calculate some statistics for this layer
	void CalcLayerArea(CHUNKTYPE type);
	int GetNumPaths();
	int GetCurRegion(){return m_nCurRegion;};
	void Flush(); //delete everything and reinit members
	double GetPathTravel(){return m_dPathTravel;};
	double GetJogTravel(){return m_dJogTravel;};
	double GetArea(){return m_dArea;};
	double GetMaterialVolume(){return m_dVmat;};
	double GetTheoreticalVolume(){return m_dVtheor;};
	double GetRoadVolume(){return m_dVroads;};
	CTimeSpan& GetTimeEst(){return m_ctsTimeEst;};
	void SetTimeEst(double tEst){m_ctsTimeEst = (__time64_t)tEst;}; //set the estimated time for the layer, in seconds
	CLayer();
	virtual ~CLayer();
	void Serialize(CArchive& ar);
	CLayer(CLayer& s);
	CLayer& operator=(const CLayer& s);
	int chunkid;
	CString toolname;
	CString matname;
	//CArray<CPath,CPath> path;
	//CArray<CPath,CPath> contour;
	//CArray<CPath,CPath> boundary;
	CArray<CRegion,CRegion> region; //to hold the deposition paths and indices to their boundaries
	CArray<CBoundaryTree, CBoundaryTree> boundaryForest; //to hold the layer boundary trees
	CCoordSys m_coordSys; //coordinate system for the layer
	int m_nCurRegion; //the region currently being fabricated
	CMutex m_mutAccess; //thread safety

protected:
	double m_dPathTravel; //the total travel along paths to complete this layer
	double m_dJogTravel; //the total travel resulting from jumping from end of one path to start of next, summed for all paths
	double m_dArea;	//the theoretical cross-section area of this layer
	double m_dVmat;	//the estimated volume of material required to make this layer
	double m_dVtheor;	//the theoretical volume of this layer
	double m_dVroads; //the theoretical volume of the deposited roads if perfect ellipse pathWidth X pathHeight
	CTimeSpan m_ctsTimeEst;	//the estimated time to build this layer
	
};

#endif // !defined(AFX_LAYER_H__992394D6_A81F_4DD3_BF41_956EB09516D4__INCLUDED_)
