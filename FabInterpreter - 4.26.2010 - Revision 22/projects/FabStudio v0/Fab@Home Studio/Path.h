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

// Path.h: interface for the CPath class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PATH_H__8C392222_1AC0_4DF2_8A6A_E99B97E2C456__INCLUDED_)
#define AFX_PATH_H__8C392222_1AC0_4DF2_8A6A_E99B97E2C456__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "afxmt.h" //for multithreading support

enum PATHTYPE {
	PATHTYPE_BOUNDARY,
	PATHTYPE_CONTOUR,
	PATHTYPE_RASTER,
	PATHTYPE_VECTOR,
	PATHTYPE_HARDWARE,
	PATHTYPE_TRACEDEP,
	PATHTYPE_TRACEJOG,
	PATHTYPE_ERROR
};

#define ANGLE_TOL 0.01 //angular tolerance in radians

struct INTERSECTION
{
	int index; //the index of the vertex prior to the intersection
	CVec pos; //the crossing point, or proximity point if slightly skew
};

class CPath : public CObject  
{
public:
	void Interpolate(double maxLen);
	void RemoveSharpCorners(double maxAngle, double minArea, double cleanTol);
	void Log(CString& fileName, CString& header = CString("")); //log the points of the path
	static void Clean(CArray<CPath,CPath>& paths, double tol);
	static void Consolidate(CArray<CPath,CPath>& paths, double tol);
	static void Offset(CArray<CPath,CPath>& paths, double d, CVec z, CArray<CPath,CPath>& output);
	static void SetPathTypes(CArray<CPath,CPath>& paths, PATHTYPE t);
	void ConvexHullXY(CArray<int,int>* pIdx); //calculate the convex hull of *this, return array of indices
	bool Clean(double tol);
	bool Offset(double d, CVec z, CArray<CPath,CPath>& output);
	void Reverse();
	void CircShift(int n); //circularly shift start/end of contour path by n vertices
	void Draw(bool bPathNormals, CVec basecolor);
	void CalcBBox(CVec xax, CVec yax, CVec zax, CVec &pmin, CVec &pmax); //compute the bounding box for this path
	void CalcBBox(CVec &pmin, CVec &pmax);
	CPath();
	virtual ~CPath();
	void CPath::Serialize(CArchive& ar);
	CPath::CPath(CPath& s);
	CPath& CPath::operator=(const CPath& s);
	bool CPath::operator==(const CPath& s);
	double GetLength();
	bool FindSplitPoint(double pos, double tol, int& insertIdx, CVec& newpt);
	bool AddSplitPoint(double pos, double tol, int& insertIdx, CVec &newpt);
	bool IsClosed();
	double PolyArea2(); //calc 2* area of closed path
	bool PathInside( CPath& testPath, CArray <int,int>* pCrossIdx = NULL );  //Check that the testPath is inside of the current path, search for crossings if pCrossIdx isn't NULL
	bool PointInside( CVec& testPt );  //test whether the test point is inside this path (assumes this path is closed)
	int FindIntersections(CPath &s, CArray<INTERSECTION,INTERSECTION>& va, 
		CArray<INTERSECTION,INTERSECTION>& vb, double tol = 1E-6); //find intersections between *this and s, return intersection data for each path
	bool PruneSelfIntersections(CArray<INTERSECTION,INTERSECTION>& va, CArray<INTERSECTION,INTERSECTION>& vb, 
								CArray<CPath,CPath>& output);
	bool PruneOtherIntersections(CPath &B, CArray<INTERSECTION,INTERSECTION>& va, CArray<INTERSECTION,INTERSECTION>& vb, 
								CArray<CPath,CPath>& output);
	bool PruneOtherIntersections(CArray<CPath,CPath>& arB, CArray<CPath,CPath>& output);
	PATHTYPE type;
	CArray<CVec,CVec> v;
	CMutex m_mutAccess;
};

#endif // !defined(AFX_PATH_H__8C392222_1AC0_4DF2_8A6A_E99B97E2C456__INCLUDED_)
