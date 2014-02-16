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

// GLUtils.h: interface for the CGLUtils class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLUTILS_H__A8A3CA61_06C6_4F63_AD71_055ECE67DD99__INCLUDED_)
#define AFX_GLUTILS_H__A8A3CA61_06C6_4F63_AD71_055ECE67DD99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Vec.h"
#include "gl\gl.h"
#include "gl\glu.h"

class CGLUtils : public CObject  
{
public:
	CGLUtils();
	virtual ~CGLUtils();

	static void DrawCylinder(CVec v0, CVec v1, double rad, double rad2 = 0);
	static void DrawWave(CVec v0, CVec v1, double rad, double amp, double freq);
	static void DrawCylinderBox(CVec v0, CVec v1, double rad);
	static void CGLUtils::DrawArbBox(CVec bminf, CVec bmaxf, CVec xaxf, CVec yaxf, CVec zaxf);
	static void DrawCylinder(double rad, double length, double rad2 = 0);
	static void DrawCone(double rad, double length);
	static void DrawCone(CVec v0, CVec v1, double rad);
	static void DrawSphere(double rad);;
	static void DrawSphere(CVec p, double rad);
	static void DrawCube();
	static void glVertex(CVec v);
	static void glRasterPos(CVec v);
	static void glColor(CVec v);
	static void glNormal(CVec v);
	static void glTranslate(CVec v);
	static HANDLE MakeDibFromBitmap(HBITMAP hbitmap, UINT bits );
	static void Rectangle(double x1, double y1, double x2, double y2);

};

#endif // !defined(AFX_GLUTILS_H__A8A3CA61_06C6_4F63_AD71_055ECE67DD99__INCLUDED_)
