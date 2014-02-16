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

// Triangle.cpp: implementation of the CTriangle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
CTriangle::CTriangle()
//---------------------------------------------------------------------------
{

}

//---------------------------------------------------------------------------
CTriangle::~CTriangle()
//---------------------------------------------------------------------------
{

}

//---------------------------------------------------------------------------
void CTriangle::Serialize(CArchive& ar)
//---------------------------------------------------------------------------
{// Save / Load

	int version = 10;

	if (ar.IsStoring()) { //add storing code here

		ar << version;

		// save data

		v[0].Serialize(ar);
		v[1].Serialize(ar);
		v[2].Serialize(ar);
		n.Serialize(ar);

	}	else	{  // add loading code here

		int versionmax = version;
		ar >> version; if (version < 10 || version > versionmax) throw 6;

		// load data

		v[0].Serialize(ar);
		v[1].Serialize(ar);
		v[2].Serialize(ar);
		n.Serialize(ar);

	}
}

//---------------------------------------------------------------------------
CTriangle::CTriangle(CTriangle& s)
//---------------------------------------------------------------------------
{
	*this = s;
}

//---------------------------------------------------------------------------
CTriangle& CTriangle::operator=(const CTriangle& s)
//---------------------------------------------------------------------------
{

	v[0] = s.v[0];
	v[1] = s.v[1];
	v[2] = s.v[2];
	n = s.n;

	return *this;
}

//---------------------------------------------------------------------------
void CTriangle::Draw(bool bShaded)
//---------------------------------------------------------------------------
{


}

//---------------------------------------------------------------------------
bool CTriangle::Intersect(double z, CVec &p1, CVec &p2)
//---------------------------------------------------------------------------
{// intersect triangle with plane parallel to x-y and at z.
 // Return p1 and p2 in an order such that material side is on
 // left when going from p1 to p2.

	if (v[0].z >= z && v[1].z >= z && v[2].z >= z)
		return false;
	if (v[0].z <= z && v[1].z <= z && v[2].z <= z)
		return false;
	
	CVec p;
	int cnt=0;

	for (int i0=0; i0<3; i0++) {
		int i1 = (i0+1)%3;
		if ((v[i0].z <= z && v[i1].z > z) || (v[i1].z <= z && v[i0].z > z)) {
			p = v[i0]+(v[i1]-v[i0])*(z-v[i0].z)/(v[i1].z-v[i0].z);
			if (cnt == 0) {
				p1 = p;
				cnt++;
			} else {
				p2 = p;
				// set right order
				if ((n.Cross(p2-p1)).Dot(CVec(0,0,1)) < 0) { // swap
					p = p1;
					p1 = p2;
					p2 = p;
				}
				return true;
			}
		}
	}
	
	return false; // less than 2 points found
}
