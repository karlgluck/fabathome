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

// Vec.cpp: implementation of the CVec class.
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

/* ---------------------------------------------------------------- */
CVec::CVec()
/* ---------------------------------------------------------------- */
{

	x = 0;
	y = 0;
	z = 0;
}

/* ---------------------------------------------------------------- */
CVec::~CVec()
/* ---------------------------------------------------------------- */
{

}

/* ---------------------------------------------------------------- */
CVec::CVec(CVec& s)
/* ---------------------------------------------------------------- */
{
	x = s.x;
	y = s.y;
	z = s.z;

}

/* ---------------------------------------------------------------- */
CVec operator*(double f, CVec v)
/* ---------------------------------------------------------------- */
{ // scalar multiplication

	return CVec(v.x*f, v.y*f, v.z*f);
}

/* ---------------------------------------------------------------- */
CVec::CVec(double dx, double dy, double dz)
/* ---------------------------------------------------------------- */
{ // Initialize by value

	x = dx;
	y = dy;
	z = dz;

}

/* ---------------------------------------------------------------- */
CVec::CVec(CString str)
/* ---------------------------------------------------------------- */
{ // Initialize by string with three values

	x=y=z=0;
	str.Replace(","," ");
	sscanf_s(str,"%lf %lf %lf", &x, &y, &z);
    
}

/* ---------------------------------------------------------------- */
double CVec::Dist(const CVec& v)
/* ---------------------------------------------------------------- */
{ // compute distance from another vector

	double d = sqrt(Dist2(v));
	return d;
}

/* ---------------------------------------------------------------- */
double CVec::Dist2(const CVec& v)
/* ---------------------------------------------------------------- */
{ // compute square distance from another vector

	double d = ((v.x-x)*(v.x-x) +
				(v.y-y)*(v.y-y) +
				(v.z-z)*(v.z-z));

	return d;
}

/* ---------------------------------------------------------------- */
bool CVec::operator ==(const CVec &s)
/* ---------------------------------------------------------------- */
{
	//cvec are equal if less than CVEC_TOL away from each other
	return (Dist2(s) < (CVEC_TOL*CVEC_TOL));
}


/* ---------------------------------------------------------------- */
void CVec::Serialize(CArchive& ar)
/* ---------------------------------------------------------------- */
{// Save / Load

	int version = 10;

	if (ar.IsStoring())
	{
		// TODO: add storing code here

		ar << version;

		// save vectors
		ar << x;
		ar << y;
		ar << z;

	}
	else
	{
		// TODO: add loading code here

		ar >> version;

		// load vectors
		ar >> x;
		ar >> y;
		ar >> z;

	}
}

/* ---------------------------------------------------------------- */
void CVec::DrawMarker()
/* ---------------------------------------------------------------- */
{ // plot a round marker

	int bitmap[]={0x18, 0x7E, 0x7E, 0xFF, 0xFF, 0x7E, 0x7E, 0x18};

	// draw marker
	glRasterPos3d(x,y,z);
	glBitmap(8,8,4,4,0,0, (GLubyte *) bitmap); 
}


/* ---------------------------------------------------------------- */
void CVec::DrawHollowMarker()
/* ---------------------------------------------------------------- */
{ // plot a round marker

	int bitmap[]={0x18, 0x42, 0x42, 0x81, 0x81, 0x42, 0x42, 0x18};

	// draw marker
	glRasterPos3d(x,y,z);
	glBitmap(8,8,4,4,0,0, (GLubyte *) bitmap); 
}

/* ---------------------------------------------------------------- */
void CVec::RotX(double a)
/* ---------------------------------------------------------------- */
{ // rotate point about X axis by "a" radians

	double yt =  y*cos(a) + z*sin(a);
	double zt = -y*sin(a) + z*cos(a);
	y = yt;
	z = zt;

}

/* ---------------------------------------------------------------- */
void CVec::RotY(double a)
/* ---------------------------------------------------------------- */
{ // rotate point about Y axis by "a" radians

	double xt =  x*cos(a) + z*sin(a);
	double zt = -x*sin(a) + z*cos(a);
	x = xt;
	z = zt;

}

/* ---------------------------------------------------------------- */
void CVec::RotZ(double a)
/* ---------------------------------------------------------------- */
{ // rotate point about Z axis by "a" radians

	double xt =  x*cos(a) + y*sin(a);
	double yt = -x*sin(a) + y*cos(a);
	x = xt;
	y = yt;

}

/* ---------------------------------------------------------------- */
CVec CVec::Rot(CVec u, double a)
/* ---------------------------------------------------------------- */
{ // rotate point about axis u by angle a [radians]

	double ca = cos(a);
	double sa = sin(a);

	return CVec(

		(u.x*u.x + (1 - u.x*u.x)*ca) * x +
		(u.x*u.y*(1 - ca) + u.z*sa) * y +
		(u.z*u.x*(1 - ca) - u.y*sa) * z,

		(u.x*u.y*(1 - ca) - u.z*sa) * x +
		(u.y*u.y + (1 - u.y*u.y)*ca) * y +
		(u.y*u.z*(1 - ca) + u.x*sa) * z,

		(u.z*u.x*(1 - ca) + u.y*sa) * x +
		(u.y*u.z*(1 - ca) - u.x*sa) * y +
		(u.z*u.z + (1 - u.z*u.z)*ca) * z

		);
		
}

/* ---------------------------------------------------------------- */
CVec CVec::ArbitraryNormal()
/* ---------------------------------------------------------------- */
{// Generate arbitrary normal

	CVec n = Normalized();

	if (fabs(n.x) <= fabs(n.y) && fabs(n.x) <= fabs(n.z)) {
		n.x = 1;
	} else if (fabs(n.y) <= fabs(n.x) && fabs(n.y) <= fabs(n.z)) {
		n.y = 1;
	} else {
		n.z = 1;
	}

	return Cross(n).Normalized();

}

/* ---------------------------------------------------------------- */
double CVec::AlignWith(CVec target, CVec &rotax)
/* ---------------------------------------------------------------- */
{ // find rotation axis and angle to align this vector with target vector

	CVec thisvec = Normalized();
	CVec targvec = target.Normalized();
	CVec rotaxis = thisvec.Cross(targvec);
	rotax = rotaxis.Normalized();
//	double a = asin(rotaxis.Length());
	double a = acos(thisvec.Dot(targvec));

	return a;

}

/* ---------------------------------------------------------------- */
CVec CVec::Interpolate(const CVec& next, double dist)
/* ---------------------------------------------------------------- */
{
	//return a vector pointing to the point that is dist from this,
	//along the vector pointing from this to next
	CVec tmp;
	tmp = next;
	return( *this + ((tmp - (*this))/(tmp-(*this)).Length()) * dist );
}

//---------------------------------------------------------------------------
bool CVec::SkewIntersection(CVec B, CVec C, CVec D, CVec& AB, CVec& CD, double tol)
//---------------------------------------------------------------------------
{
	// Find nearest crossing of two lines A-B and C-D in space (A = *this). 
	// Return false if parallel, true otherwise, return proximity points on A-B via AB, and C-D via CD resp;
	//tol sets the tolerance for considering doubles identical

	//	The near intersection of two skew lines (from http://www.physik.uni-regensburg.de/psi/idl-4.0.1/jhuapl.doc/vectors/v_skew.html)
	//	Sometimes a problem arises where two lines in space should intersect but due to small measurement errors they are skew. 
	//	The following figure shows the near intersection of two such skew lines. There is a point on each line that is closest to the other line. 
	//	The midpoint of the segment connecting these points gives a good estimate of the desired intersection point. 
	//	The desired intersection point is labeled M and shown in red. 

	//	Line 1 is defined by points A and B, with unit vector U1 pointing along the line. 
	//	Line 2 is defined by points C and D, with unit vector U2 pointing along the line. 
	//	A vector, W, perpendicular to both lines is given by W = U1 cross U2. 
	//	If the lines are parallel then the length of W is 0 and there is no one point of closest approach. 
	//	Assuming non-parallel lines, a unit vector, U3, is given by	U3 = W/|W| = (U1 cross U2)/|U1 cross U2|. 
	//	This unit vector may point from Line 1 to Line 2, or in the opposite direction, depending on the directions of U1 and U2. 
	//	A displacement vector from Line 2 to Line 1 may be found as follows. Let the vector from point C to point A be V = A-C. 
	//	The length of V along U3 is (V dot U3) and is > 0 if U3 is parallel to V, and < 0 if U3 is anti-parallel to V. 
	//	This length is also the distance between the lines. 
	//	Since the sign of V reverses if U3 reverses, the product is a vector that always points the same way, from Line 2 to Line 1. 
	//	So the displacement vector, E, needed to shift Line 2 to intersect Line 1 is given by 
	//	E = (V dot U3)U3 = ((A-C) dot U3)U3. 
	//	Line 2 is shifted as follows: 
	//	C' = C + E 
	//	D' = D + E 
	//	Point I' is found using the algorithm for the intersection of two co-planar lines. 
	//	Then I = I' - E/2. 
	//	The distance by which the two lines miss each other is |E|, which may be used to check for abnormally large errors. 

	//	Algorithm 

	CVec &A = *this;
	double tol2 = tol*tol;
	//first check for common vertices (algorithm fails if A==C or A==D, or B==C or B==D
	if((C-A).Length2() < tol2)
	{
		AB = A;
		CD = C;
		return true;
	}
	if((D-A).Length2() < tol2)
	{
		AB = A;
		CD = D;
		return true;
	}
	if((C-B).Length2() < tol2)
	{
		AB = B;
		CD = C;
		return true;
	}
	if((D-B).Length2() < tol2)
	{
		AB = B;
		CD = D;
		return true;
	}

	//then check for zero length lines in input
	if(((B-A).Length2() < tol2) || ((C-D).Length2() < tol2))
	{
		//AfxMessageBox("SkewIntersection: Zero length line", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

//	Unit vector U1 along Line 1: 
//	U1 = (B-A)/| B-A | 
	CVec u1 = (B-A).Normalized();
//	Unit vector U2 along Line 2: 
//	U2 = (D-C)/| D-C | 
	CVec u2 = (D-C).Normalized();
//	Unit vector U3 from Line 2 toward Line 1: 
//	U3 = (U1 cross U2)/| U1 cross U2 | 
	CVec u3 = (u1.Cross(u2));
	//Check for (anti)parallel lines
	if(u3.Length2() < tol2) return false;
	u3 = u3.Normalized();
	//now check for colinearity
	//in this case there are infinitely many intersection points
	if (u3.x < tol && u3.x > -tol && 
		u3.y < tol && u3.y > -tol && 
		u3.z < tol && u3.z > -tol) 
	{
		return FALSE;
	}
//	Displacement vector from Line 2 to Line 1: 
//	E = ((A-C) dot U3) U3 
	CVec E = ((A-C).Dot(u3))*u3;
//	Line 2': 
//	C' = C + E 
	CVec Ct = C + E;
//	D' = D + E 
	CVec Dt = D + E;
//	Unit vector U4 perpendicular to Line 2': 
//	V = A - C' 
	CVec V = A - Ct;
//	V_perp = V - (V dot U2) U2 
	CVec V_perp = V - (V.Dot(u2))*u2;
//	U4 = V_perp/| V_perp | 
	CVec u4 = V_perp.Normalized();
//	Similar triangle side lengths P and p: 
//	P = (A - C') dot U4 
	double P = (A-Ct).Dot(u4);
//	p = (B - C') dot U4 
	double p = (B-Ct).Dot(u4);
//	Point I' fraction of distance from point A to point B: 
//	Frac = H/(H+h) = P/(P+p) !!!!ERROR Should be P/(P-p) -- Hod
	double f = P/(P-p);
//	Intersection point I': 
//	I' = A + U1 * Frac !!!!ERROR Should be (B-A)*Frac) -- Hod
	CVec It = A + (B-A)*f;
//	Intersection point I: 
//	I = I' - E/2 
//	Proximity point on AB is at I'
	AB = It;
//	Proximity point on CD is at I'-E
	CD = It - E;
	return TRUE;
}

//---------------------------------------------------------------------------
bool CVec::SegmentIntersection(CVec B, CVec C, CVec D, CVec& AB, CVec& CD, double tol)
//---------------------------------------------------------------------------
{
	//treat *this-B and C-D as lines and perform skew intersection
	//if lines intersect, then check that proximity points are within segments

	if(!SkewIntersection(B, C, D, AB, CD, tol)) return false;
	CVec &A = *this;
	//check for skewness (|E| > tol*tol)
	if((CD-AB).Length2() > tol*tol) return false;
	double testAB = (AB-A).Dot(B-A);
	double testCD = (CD-C).Dot(D-C);
	return((testAB >= 0) && !(testAB > (B-A).Length2()) &&
		(testCD >= 0) && !(testCD > (D-C).Length2()));
}

//---------------------------------------------------------------------------
void glVertex3d(const CVec& v)
//---------------------------------------------------------------------------
{
	glVertex3d(v.x,v.y,v.z);
}

//---------------------------------------------------------------------------
void glRasterPos3d(const CVec& v)
//---------------------------------------------------------------------------
{
	glRasterPos3d(v.x,v.y,v.z);
}