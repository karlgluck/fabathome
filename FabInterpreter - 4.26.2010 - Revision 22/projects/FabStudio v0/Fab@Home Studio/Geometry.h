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

// Geometry.h: interface for the CGeometry class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GEOMETRY_H__1FF24DC0_A4A1_4821_94A4_5B74D45AF562__INCLUDED_)
#define AFX_GEOMETRY_H__1FF24DC0_A4A1_4821_94A4_5B74D45AF562__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGeometry  : public CObject
{
public:
	CGeometry();
	virtual ~CGeometry();
	void Serialize(CArchive& ar);
	CGeometry(CGeometry& s);
	CGeometry& operator=(const CGeometry& s);
	void Rotate(CVec ax, double a);
	void RotX(double a);
	void RotY(double a);
	void RotZ(double a);
	void Scale(CVec d);
	void Translate(CVec d);
	void ComputeBoundingBox(CVec& pmin, CVec& pmax);
	bool ComputeZSlice(double z, CArray<CPath,CPath>& paths);
	void Draw(bool bModelhNormals, bool bShaded);
	bool LoadSTL(CString file, int maxsize = 1000000);
	bool LoadVCT(CString file);
	bool CGeometry::AppendGeometry(CGeometry& geom);

	CArray<CTriangle,CTriangle> mesh;
	CArray<CPath,CPath> vectors;

	//18-04-2008 added the name variable to the class (name of the STL file)
	CString name;
	
	void Clear(void);
};

#endif // !defined(AFX_GEOMETRY_H__1FF24DC0_A4A1_4821_94A4_5B74D45AF562__INCLUDED_)
