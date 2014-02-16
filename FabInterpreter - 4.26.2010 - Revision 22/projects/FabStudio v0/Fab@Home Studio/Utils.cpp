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

// Utils.cpp: implementation of the CUtils class.
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

//--------------------------------------------------------------------
CUtils::CUtils()
//--------------------------------------------------------------------
{

}

//--------------------------------------------------------------------
CUtils::~CUtils()
//--------------------------------------------------------------------
{

}



// Copyright 2001, softSurfer (www.softsurfer.com)
// This code may be freely used and modified for any purpose
// providing that this copyright notice is included with it.
// SoftSurfer makes no warranty for this code, and cannot be held
// liable for any real or imagined damage resulting from its use.
// Users of this code must verify correctness for their application.


// Assume that a class is already given for the object:
//    Point with coordinates {float x, y;}
//===================================================================


// isLeft(): tests if a point is Left|On|Right of an infinite line.
//    Input:  three points P0, P1, and P2
//    Return: >0 for P2 left of the line through P0 and P1
//            =0 for P2 on the line
//            <0 for P2 right of the line
//    See: the January 2001 Algorithm on Area of Triangles
inline double
isLeft( CVec& P0, CVec& P1, CVec& P2 )
{
    return (P1.x - P0.x)*(P2.y - P0.y) - (P2.x - P0.x)*(P1.y - P0.y);
}
//===================================================================
 

// chainHull_2D(): Andrew's monotone chain 2D convex hull algorithm
//     Input:  P[] = an array of 2D points 
//                   presorted by increasing x- and y-coordinates
//             n = the number of points in P[]
//     Output: H[] = an array of the convex hull vertices (max is n)
//			idx[] = an array of indices into P[] of the vertices in H[]
//     Return: the number of points in H[] (and idx[])
int CUtils::ChainHull_2D( CVec* P, int n, CVec* H, int* idx )
{
    // the output array H[] will be used as the stack
    int    bot=0, top=(-1);  // indices for bottom and top of the stack
    int    i;                // array scan index

    // Get the indices of points with min x-coord and min|max y-coord
    int minmin = 0, minmax;
    double xmin = P[0].x;
    for (i=1; i<n; i++)
        if (P[i].x != xmin) break;
    minmax = i-1;
    if (minmax == n-1) {       // degenerate case: all x-coords == xmin
        H[++top] = P[minmin];
		idx[top] = minmin;
		if (P[minmax].y != P[minmin].y){ // a nontrivial segment
            H[++top] = P[minmax];
			idx[top] = minmax;
		}
        H[++top] = P[minmin];           // add polygon endpoint
		idx[top] = minmin;
        return top+1;
    }

    // Get the indices of points with max x-coord and min|max y-coord
    int maxmin, maxmax = n-1;
    double xmax = P[n-1].x;
    for (i=n-2; i>=0; i--)
        if (P[i].x != xmax) break;
    maxmin = i+1;

    // Compute the lower hull on the stack H
    H[++top] = P[minmin];      // push minmin point onto stack
	idx[top] = minmin;
    i = minmax;
    while (++i <= maxmin)
    {
        // the lower line joins P[minmin] with P[maxmin]
        if (isLeft( P[minmin], P[maxmin], P[i]) >= 0 && i < maxmin)
            continue;          // ignore P[i] above or on the lower line

        while (top > 0)        // there are at least 2 points on the stack
        {
            // test if P[i] is left of the line at the stack top
            if (isLeft( H[top-1], H[top], P[i]) > 0)
                break;         // P[i] is a new hull vertex
            else
                top--;         // pop top point off stack
        }
        H[++top] = P[i];       // push P[i] onto stack
		idx[top] = i;
    }

    // Next, compute the upper hull on the stack H above the bottom hull
	if (maxmax != maxmin){      // if distinct xmax points
        H[++top] = P[maxmax];  // push maxmax point onto stack
		idx[top] = maxmax;
	}
    bot = top;                 // the bottom point of the upper hull stack
    i = maxmin;
    while (--i >= minmax)
    {
        // the upper line joins P[maxmax] with P[minmax]
        if (isLeft( P[maxmax], P[minmax], P[i]) >= 0 && i > minmax)
            continue;          // ignore P[i] below or on the upper line

        while (top > bot)    // at least 2 points on the upper stack
        {
            // test if P[i] is left of the line at the stack top
            if (isLeft( H[top-1], H[top], P[i]) > 0)
                break;         // P[i] is a new hull vertex
            else
                top--;         // pop top point off stack
        }
        H[++top] = P[i];       // push P[i] onto stack
		idx[top] = i;
    }
	if (minmax != minmin){
        H[++top] = P[minmin];  // push joining endpoint onto stack
		idx[top] = minmin;
	}
    return top+1;
}




//--------------------------------------------------------------------
CString CUtils::NoExtention(CString str)
//--------------------------------------------------------------------
{ // remove extention part from a file name
	
	int p = str.ReverseFind('.');
	
	if (p == -1) 
		return str;
	else
		return str.Left(p);
}


//--------------------------------------------------------------------
CString CUtils::RemovePath(CString str)
//--------------------------------------------------------------------
{ // remove path part from a file name
	
	int p = str.ReverseFind('\\');
	
	if (p == -1) 
		return str;
	else
		return str.Mid(p+1);
}


/*----------------------------------------------------------------------*/
int CUtils::Solve(long N,  double a[SOLVESIZE][SOLVESIZE], double b[SOLVESIZE])
/*--------------------------------------------------------------------- */
{ // Solve matrix Eqs. aX=b. Input: N=Order of matrices a & b
	// Output: Solved X in b. Diagnostics: returns 0=Error, 1=OK
	
    long            i, j, k;
    double          c, f;
	
#define SOLVEPS 1E-12
	
    /* perform pivoting */
	
    for (i = 0; i < N; i++) {
		if (fabs(a[i][i]) <= SOLVEPS) {
			for (j = 0; j < N; j++) {
				if (fabs(a[j][i]) > SOLVEPS && fabs(a[i][j]) > SOLVEPS) {
					for (k = 0; k < N; k++) {	/* swap rows i,j */
						c = a[j][k];
						a[j][k] = a[i][k];
						a[i][k] = c;
					}
					c = b[j];	/* swap const vector rows i,j */
					b[j] = b[i];
					b[i] = c;
					break;
				}
			}
		}
		if (fabs(a[i][i]) <= SOLVEPS)
			return 0;		/* Cannot pivot */
    }
	
    /* solve [a]*[x]=[b] */
	
    for (i = 0; i < N - 1; i++) {
		for (j = i + 1; j < N; j++) {
			if (fabs(a[i][i]) <= SOLVEPS)
				return 0;	/* Zero Pivot */
			f = a[j][i] / a[i][i];
			for (k = i; k < N; k++)
				a[j][k] = a[j][k] - a[i][k] * f;
			b[j] = b[j] - b[i] * f;
		}
    }
	
    /* back substitute and find solution [x] in [b] */
	
    for (j = N - 1; j >= 0; j--) {
		if (fabs(a[i][i]) <= SOLVEPS)
			return 0;		/* Zero Pivot */
		b[j] = b[j] / a[j][j];
		a[j][j] = 1;
		for (i = 0; i < j; i++) {
			b[i] = b[i] - a[i][j] * b[j];
			a[i][j] = 0;
		}
    }
	
    return 1;
	
}

//--------------------------------------------------------------------
double CUtils::DistPointLine(CVec p, CVec p1, CVec p2, CVec &c, double& d)
//--------------------------------------------------------------------
{// distance of point from line.

	CVec ax = (p2-p1);
	double l = ax.Normalize();
	if (l <= 0) {
		c = p1;
		d = 0;
		return (p-p1).Length();
	}

	d = (p-p1).Dot(ax);
	c = p1 + d*ax;
	double len = (p-c).Length();
	ASSERT(fabs((p-c).Dot(p2-p1))<1E-3);
	return len;

}

//--------------------------------------------------------------------
double CUtils::ParameterAlongLine(CVec p, CVec p1, CVec p2)
//--------------------------------------------------------------------
{// parameter [0-1] along line segment

	double dx = p2.x-p1.x;
	double dy = p2.y-p1.y;
	double dz = p2.z-p1.z;
	double d = 0;

	if (fabs(dx)>fabs(dy) && fabs(dx)>fabs(dz)) {
		d = (p.x-p1.x)/dx;
	} else 	if (fabs(dy)>fabs(dx) && fabs(dy)>fabs(dz)) {
		d = (p.y-p1.y)/dy;
	} else {
		d = (p.z-p1.z)/dz;
	}

	return d;

}

//---------------------------------------------------------------------------
bool CUtils::SkewIntersection2(CVec A, CVec B, CVec C, CVec D, CVec& AB, CVec& CD)
//---------------------------------------------------------------------------
{// Find nearest crossing of two lines AB and CD in space. 
 // Return false if parallel, true otherwise

//	The near intersection of two skew lines (from http://www.physik.uni-regensburg.de/psi/idl-4.0.1/jhuapl.doc/vectors/v_skew.html)
//	Sometimes a problem arises where two lines in space should intersect but due to small measurement errors they are skew. The following figure shows the near intersection of two such skew lines. There is a point on each line that is closest to the other line. The midpoint of the segment connecting these points gives a good estimate of the desired intersection point. 
//	The desired intersection point is labeled M and shown in red. 

//	Line 1 is defined by points A and B, with unit vector U1 pointing along the line. Line 2 is defined by points C and D, with unit vector U2 pointing along the line. 
//	A vector, W, perpendicular to both lines is given by W = U1 cross U2. If the lines are parallel then the length of W is 0 and there is no one point of closest approach. 
//	Assuming non-parallel lines, a unit vector, U3, is given by 
//	U3 = W/|W| = (U1 cross U2)/|U1 cross U2|. This unit vector may point from Line 1 to Line 2, or in the opposite direction, depending on the directions of U1 and U2. 
//	A displacement vector from Line 2 to Line 1 may be found as follows. Let the vector from point C to point A be V = A-C. The length of V along U3 is (V dot U3) and is > 0 if U3 is parallel to V, and < 0 if U3 is anti-parallel to V. This length is also the distance between the lines. Since the sign of V reverses if U3 reverses, the product is a vector that always points the same way, from Line 2 to Line 1. So the displacement vector, E, needed to shift Line 2 to intersect Line 1 is given by 
//	E = (V dot U3)U3 = ((A-C) dot U3)U3. 
//	Line 2 is shifted as follows: 
//	C' = C + E 
//	D' = D + E 
//	Point I' is found using the algorithm for the intersection of two co-planar lines. 
//	Then I = I' - E/2. 
//	The distance by which the two lines miss each other is |E|, which may be used to check for abnormally large errors. 

//	Algorithm 

//	Unit vector U1 along Line 1: 
//	U1 = (B-A)/| B-A | 
	CVec u1 = (B-A).Normalized();
//	Unit vector U2 along Line 2: 
//	U2 = (D-C)/| D-C | 
	CVec u2 = (D-C).Normalized();
//	Unit vector U3 from Line 2 toward Line 1: 
//	U3 = (U1 cross U2)/| U1 cross U2 | 
	CVec u3 = (u1.Cross(u2)).Normalized();
	//  Check for parallel lines
	if (u3.x < 1E-4 && u3.x > -1E-4 && 
		u3.y < 1E-4 && u3.y > -1E-4 && 
		u3.z < 1E-4 && u3.z > -1E-4) {
		ParallelOverlap(A,B,C,D,AB,CD);
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

	double d0 = (AB-CD).Length();
	CVec cc;
	double dd;
	double d1 = DistPointLine(AB,C,D,cc,dd);
	double d2 = DistPointLine(CD,A,B,cc,dd);
	if (fabs(d0-d1)>1E-2) TRACE("Mismatch1!!! %.2f %.2f\n", d0, d1);
	if (fabs(d0-d2)>1E-2) TRACE("Mismatch2!!! %.2f %.2f\n", d0, d2);
	if (fabs(d2-d1)>1E-2) {
		TRACE("Mismatch3!!! %.2f %.2f\n", d2, d1); 
		TRACE("A=%.3f %.3f %.3f\n", A.x, A.y, A.z);
		TRACE("B=%.3f %.3f %.3f\n", B.x, B.y, B.z);
		TRACE("C=%.3f %.3f %.3f\n", C.x, C.y, C.z);
		TRACE("D=%.3f %.3f %.3f\n", D.x, D.y, D.z);
		TRACE("AB=%.3f %.3f %.3f\n", AB.x, AB.y, AB.z);
		d1 = DistPointLine(AB,C,D,cc,dd);
		double d1a = (AB-cc).Length(); // should be d1
		double d1b = (CD-cc).Length(); // should be zero
		TRACE("cc=%.3f %.3f %.3f\n", cc.x, cc.y, cc.z);
	}

	return TRUE;
}

//---------------------------------------------------------------------------
bool CUtils::SkewIntersection(CVec A, CVec B, CVec C, CVec D, CVec& AB, CVec& CD)
//---------------------------------------------------------------------------
{// Find nearest crossing of two lines AB and CD in space. 
 // Return false if parallel, true otherwise


	CVec ax1 = (B-A).Normalized();
	CVec ax2 = (D-C).Normalized();
	if (fabs(fabs(ax1.Dot(ax2))-1)<1E-4) return false; // parallel

	CVec n = ax1.Cross(ax2);
	n.Normalize();
	double d = (C-A).Dot(n);
	CVec A2 = A + d*n;
	CVec B2 = B + d*n;
	CVec zax = ax2.Cross(n).Normalized();
	double dA2 = (A2-C).Dot(zax);
	double dB2 = (B2-C).Dot(zax);
	double pA2 = dA2/(dA2-dB2);
	CD = A2 + pA2*(B2-A2);
	AB = CD - d*n;

	CVec cc;
	double dd;
	double d0 = (AB-CD).Length();
	double d1 = DistPointLine(AB,C,D,cc,dd);
	double d2 = DistPointLine(CD,A,B,cc,dd);
	if (fabs(d0-d1)>1E-2) TRACE("Mismatch1!!! %.2f %.2f\n", d0, d1);
	if (fabs(d0-d2)>1E-2) TRACE("Mismatch2!!! %.2f %.2f\n", d0, d2);
	if (fabs(d2-d1)>1E-2) {
		TRACE("Mismatch3!!! %.2f %.2f\n", d2, d1); 
		TRACE("A=%.3f %.3f %.3f\n", A.x, A.y, A.z);
		TRACE("B=%.3f %.3f %.3f\n", B.x, B.y, B.z);
		TRACE("C=%.3f %.3f %.3f\n", C.x, C.y, C.z);
		TRACE("D=%.3f %.3f %.3f\n", D.x, D.y, D.z);
		TRACE("AB=%.3f %.3f %.3f\n", AB.x, AB.y, AB.z);
		d1 = DistPointLine(AB,C,D,cc,dd);
		double d1a = (AB-cc).Length(); // should be d1
		double d1b = (CD-cc).Length(); // should be zero
		TRACE("cc=%.3f %.3f %.3f\n", cc.x, cc.y, cc.z);
	}

	return TRUE;
}


/*----------------------------------------------------------------------*/
bool CUtils::IsIntersectingLL(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
/*----------------------------------------------------------------------*/
{// determine if two finite lines cross

	// trivial rejection

	if (x1 >= x3 && x1 >= x4 && x2 >= x3 && x2 >= x4)
		return false;
	if (x1 <= x3 && x1 <= x4 && x2 <= x3 && x2 <= x4)
		return false;
	if (y1 >= y3 && y1 >= y4 && y2 >= y3 && y2 >= y4)
		return false;
	if (y1 <= y3 && y1 <= y4 && y2 <= y3 && y2 <= y4)
		return false;

	// first line

	double a1 = y2-y1;
	double b1 = x1-x2;
	double c1 = x2*y1 - x1*y2;

	// second line

	double a2 = y4-y3;
	double b2 = x3-x4;
	double c2 = x4*y3 - x3*y4;

	// check for alternating signes

	if ((a1*x3+b1*y3+c1)*(a1*x4+b1*y4+c1) < 0 &&
		(a2*x1+b2*y1+c2)*(a2*x2+b2*y2+c2) < 0) {
		return true; // interecting
	} else {
		return false; // not intersecting
	}

}

//---------------------------------------------------------------------------
bool CUtils::ParallelOverlap(CVec A, CVec B, CVec C, CVec D, CVec& AB, CVec& CD)
//---------------------------------------------------------------------------
{// Find center of overlap region between two parallel lines AB and CD in space. 
 // Return true

//	Unit vector U1 along Line 1: 
//	U1 = (B-A)/| B-A | 
	CVec u1 = (B-A).Normalized();
//	Displacement vector from Line 2 to Line 1: 
//	E = (A-C) - (A-C).dot(U1)*U1
	CVec E = (A-C) - (A-C).Dot(u1)*u1;
//	Parameters
	double pA = 0;
	double pB = (B-A).Dot(u1);
	double pC = (C+E-A).Dot(u1);
	double pD = (D+E-A).Dot(u1);
//	limits
	double minofmaxes = min(max(pA,pB),max(pC,pD));
	double maxofmins  = max(min(pA,pB),min(pC,pD));
//	center of limits
	AB = A + u1*(minofmaxes+maxofmins)/2;
	CD = AB - E;

	return TRUE;
}


//---------------------------------------------------------------------------
double CUtils::LineDistance(CVec A, CVec B, CVec C, CVec D)
//---------------------------------------------------------------------------
{// Find distance betweenn two skew lines AB and CD in space.

	double d;

//	Unit vector U1 along Line 1: 
//	U1 = (B-A)/| B-A | 
	CVec u1 = (B-A).Normalized();
//	Unit vector U2 along Line 2: 
//	U2 = (D-C)/| D-C | 
	CVec u2 = (D-C).Normalized();
//	Unit vector U3 from Line 2 toward Line 1: 
//	U3 = (U1 cross U2)/| U1 cross U2 | 
	CVec u3 = (u1.Cross(u2)).Normalized();
//  Check for parallel lines
	if (u3.x < 1E-4 && u3.x > -1E-4 && 
		u3.y < 1E-4 && u3.y > -1E-4 && 
		u3.z < 1E-4 && u3.z > -1E-4) {
		d = ((A-C) - (A-C).Dot(u2)*u2).Length(); // distance between parallel
	} else {
		//	Displacement vector from Line 2 to Line 1: 
		//	E = ((A-C) dot U3) U3 
		d = (A-C).Dot(u3);
	}

	return fabs(d);

}

//---------------------------------------------------------------------------
CString CUtils::GetPath(CString filename)
//---------------------------------------------------------------------------
{ // return string contaiing path portion of a complex filename

	int p = max(filename.ReverseFind('\\'), filename.ReverseFind('/'));
	if (p == -1)
		return "";

	// fix: if filename is already a path, then just return it
	int t = filename.ReverseFind('.');
	if(t < p)
		return filename;

	return filename.Left(p);

}

//---------------------------------------------------------------------------
CString CUtils::RemoveComment(CString str)
//---------------------------------------------------------------------------
{ // remove comment from the string
	CString delin = "#%/ \t";
	CString result = str.SpanExcluding(delin);
	return result;
}

//---------------------------------------------------------------------------
long CUtils::Round(double x)
//---------------------------------------------------------------------------
{
  ASSERT(x >= LONG_MIN-0.5);
  ASSERT(x <= LONG_MAX+0.5);
  if (x >= 0)
     return (long) (x+0.5);
  return (long) (x-0.5);
}
