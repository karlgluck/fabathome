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

// Vec.h: interface for the CVec class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VEC_H__C3B70EA1_253A_11D2_BD56_00104B6301C6__INCLUDED_)
#define AFX_VEC_H__C3B70EA1_253A_11D2_BD56_00104B6301C6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <math.h>
#include <float.h>
#define CVEC_TOL 1E-6

class CVec : public CObject  
{
public:
	double AlignWith(CVec target, CVec& rotax);
	CVec ArbitraryNormal();

	CVec();
	virtual ~CVec();
	CVec(CVec& s);
	CVec(double dx, double dy, double dz);
	CVec(CString str);

	inline CVec& operator=(const CVec& s) {	x = s.x; y = s.y; z = s.z; return *this; };
	inline CVec operator+(const CVec& s) {return CVec(x+s.x, y+s.y, z+s.z);};
	inline CVec operator-(const CVec& s) {return CVec(x-s.x, y-s.y, z-s.z);};
	inline CVec operator*(double f) {return CVec(x*f, y*f, z*f);};
	inline CVec operator/(double f) {return CVec(x/f, y/f, z/f);};
	inline CVec operator-() {return CVec(-x, -y, -z);};
	bool operator==(const CVec& s);
	inline bool operator!=(const CVec& s) {return !(*this == s);};
	inline CVec& operator+=(const CVec& s) {x += s.x; y += s.y; z += s.z; return *this; };
	inline CVec& operator-=(const CVec& s) {x -= s.x; y -= s.y; z -= s.z; return *this; };
	inline CVec& operator*=(double f) {x *= f; y *= f; z *= f; return *this;};
	inline CVec& operator/=(double f) {x /= f; y /= f; z /= f; return *this;};
	friend CVec operator*(double f, CVec v);
	inline CVec Cross(CVec v) {return CVec(y*v.z-z*v.y,z*v.x-x*v.z,x*v.y-y*v.x);};
	inline double Dot(CVec v) {return (x * v.x + y * v.y + z * v.z);};
	inline CVec Abs() {return CVec(fabs(x),fabs(y),fabs(z));};
	inline double Normalize() {double l = sqrt(x*x+y*y+z*z); if (l > 0) {x /= l;y /= l;z /= l;} return l;};
	inline CVec Normalized() {	double l = sqrt(x*x+y*y+z*z); return (l>0?(*this)/l:(*this));};
	inline CVec ProjXY() {	return CVec(x,y,0);};
	inline double Length() {return sqrt(x*x+y*y+z*z);};
	inline double Length2() {return (x*x+y*y+z*z);};
	inline CVec Min(CVec& s) {return CVec(__min(x,s.x),__min(y,s.y),__min(z,s.z));};
	inline CVec Max(CVec& s) {return CVec(__max(x,s.x),__max(y,s.y),__max(z,s.z));};
	bool IsValid() {return !_isnan(x) && _finite(x) && !_isnan(y) && _finite(y) && !_isnan(z) && _finite(z); }
	
	CVec Rot(CVec u, double a);
	void RotZ(double a);
	void RotY(double a);
	void RotX(double a);
	void DrawMarker();
	void DrawHollowMarker();
	double Dist(const CVec& v);
	double Dist2(const CVec& v);
	void Serialize(CArchive& ar);
	CVec Interpolate(const CVec& next, double dist);
	bool SkewIntersection(CVec B, CVec C, CVec D, CVec& AB, CVec& CD, double tol = 1E-6);
	bool SegmentIntersection(CVec B, CVec C, CVec D, CVec& AB, CVec& CD, double tol = 1E-6); //return true if segment AB intersects segment CD


	// attributes

	double x;
	double y;
	double z;

};
//global functions to simplify OpenGL
void glVertex3d(const CVec& v);
void glRasterPos3d(const CVec& v);

typedef CArray<CVec,CVec> VECLIST;

#endif // !defined(AFX_VEC_H__C3B70EA1_253A_11D2_BD56_00104B6301C6__INCLUDED_)
