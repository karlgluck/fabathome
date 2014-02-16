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

// Utils.h: interface for the CUtils class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UTILS_H__040B19F7_0D12_4D4F_9C3A_C318C33CB516__INCLUDED_)
#define AFX_UTILS_H__040B19F7_0D12_4D4F_9C3A_C318C33CB516__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SOLVESIZE 5
#define PI 3.14159265

class CUtils : public CObject  
{
public:
	CUtils();
	virtual ~CUtils();
	static int ChainHull_2D(CVec* P, int n, CVec* H, int* idx); //calculate the convex hull of 2D P[], return in H[]
	static double DistPointLine(CVec p, CVec p1, CVec p2, CVec& c, double& d);
	static double ParameterAlongLine(CVec p, CVec p1, CVec p2);
	static bool SkewIntersection(CVec A, CVec B, CVec C, CVec D, CVec& AB, CVec& CD);
	static bool SkewIntersection2(CVec A, CVec B, CVec C, CVec D, CVec& AB, CVec& CD);
	static bool ParallelOverlap(CVec A, CVec B, CVec C, CVec D, CVec& AB, CVec& CD);
	static double LineDistance(CVec A, CVec B, CVec C, CVec D);
	static CString NoExtention(CString str);
	static CString RemovePath(CString str);
	static int Solve(long N,  double a[SOLVESIZE][SOLVESIZE], double b[SOLVESIZE]);
	static bool IsIntersectingLL(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);
	static CString GetPath(CString filename);
	static CString RemoveComment(CString str);
	inline static int sign(int i) {if (i>0) return 1; else if (i<0) return -1; else return 0; };
	static long Round(double x);
};

#endif // !defined(AFX_UTILS_H__040B19F7_0D12_4D4F_9C3A_C318C33CB516__INCLUDED_)
