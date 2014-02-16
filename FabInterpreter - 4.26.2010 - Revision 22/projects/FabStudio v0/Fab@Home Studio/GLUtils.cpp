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

// GLUtils.cpp: implementation of the CGLUtils class.
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

// --------------------------------------------------------------------------
CGLUtils::CGLUtils()
// --------------------------------------------------------------------------
{

}

// --------------------------------------------------------------------------
CGLUtils::~CGLUtils()
// --------------------------------------------------------------------------
{

}

// --------------------------------------------------------------------------
void CGLUtils::DrawWave(CVec v0, CVec v1, double rad, double amp, double freq)
// --------------------------------------------------------------------------
{ // draw solid cylinder pointing in +X direction
	
		static double pi = atan(1.0)*4.0;

		CVec ax = (v1-v0);
		double L = ax.Normalize();
		CVec ax2 = ax.ArbitraryNormal();

		int n = (int) (10*freq);

//		for (double a=0; a<1-d; a+= d) {
		for (int i=0; i<n; i++) {
			double a = (double) i/n;
			double a2 = (double) (i+1)/n;
			CVec p1 = v0 + ax*a*L + ax2*L*amp*sin(a*pi*2*freq);
			CVec p2 = v0 + ax*a2*L + ax2*L*amp*sin(a2*pi*2*freq);
			DrawCylinder(p1,p2,rad,rad);
			DrawSphere(p1,rad);
		}
		DrawSphere(v1,rad);

}


// --------------------------------------------------------------------------
void CGLUtils::DrawCylinder(CVec v0, CVec v1, double rad, double rad2)
// --------------------------------------------------------------------------
{ // draw solid cylinder pointing in +X direction
	
		static double pi = atan(1.0)*4.0;
		CVec ax = (v1-v0).Normalized();
		double len = (v1-v0).Length();
		CVec rotax;
		double rotang = ax.AlignWith(CVec(1,0,0), rotax);
		glPushMatrix();
		glTranslate(v0);
		glRotated(-rotang*180/pi, rotax.x, rotax.y, rotax.z);
		DrawCylinder(rad, len, rad2);
		glPopMatrix();
	
}


// --------------------------------------------------------------------------
void CGLUtils::DrawCone(CVec v0, CVec v1, double rad)
// --------------------------------------------------------------------------
{ // draw solid cylinder pointing in +X direction
	
		static double pi = atan(1.0)*4.0;
		CVec ax = (v1-v0).Normalized();
		double len = (v1-v0).Length();
		CVec rotax;
		double rotang = ax.AlignWith(CVec(1,0,0), rotax);
		glPushMatrix();
		glTranslate(v0);
		glRotated(-rotang*180/pi, rotax.x, rotax.y, rotax.z);
		DrawCone(rad, len);
		glPopMatrix();
	
}


// --------------------------------------------------------------------------
void CGLUtils::DrawCylinderBox(CVec v0, CVec v1, double rad)
// --------------------------------------------------------------------------
{ // draw solid cylinder pointing in +X direction
	
		static double pi = atan(1.0)*4.0;
		CVec ax = (v1-v0).Normalized();
		double len = (v1-v0).Length();
		CVec rotax;
		double rotang = ax.AlignWith(CVec(1,0,0), rotax);
		glPushMatrix();
		glTranslate(v0);
		glRotated(-rotang*180/pi, rotax.x, rotax.y, rotax.z);
		glTranslated(0,-rad,-rad);
		glScaled(len,rad*2,rad*2);
		DrawCube();
		glPopMatrix();
	
}

// --------------------------------------------------------------------------
void CGLUtils::DrawArbBox(CVec bminf, CVec bmaxf, CVec xaxf, CVec yaxf, CVec zaxf)
// --------------------------------------------------------------------------
{ // draw box in arbitrary axis system

	CVec o = xaxf*bminf.x+yaxf*bminf.y+zaxf*bminf.z;
	CVec dx = xaxf*(bmaxf.x-bminf.x);
	CVec dy = yaxf*(bmaxf.y-bminf.y);
	CVec dz = zaxf*(bmaxf.z-bminf.z);

	glBegin(GL_LINE_LOOP);
	glVertex(o);
	glVertex(o+dx);
	glVertex(o+dx+dy);
	glVertex(o+dy);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex(o+dz);
	glVertex(o+dx+dz);
	glVertex(o+dx+dy+dz);
	glVertex(o+dy+dz);
	glEnd();

	glBegin(GL_LINES);
	glVertex(o);
	glVertex(o+dz);
	glVertex(o+dx);
	glVertex(o+dx+dz);
	glVertex(o+dx+dy);
	glVertex(o+dx+dy+dz);
	glVertex(o+dy);
	glVertex(o+dy+dz);
	glEnd();

}

// --------------------------------------------------------------------------
void CGLUtils::DrawCylinder(double rad, double length, double rad2)
// --------------------------------------------------------------------------
{ // draw solid cylinder pointing in +X direction
	
	double a,y,z;
	static double pi = atan(1.0)*4.0;
	ASSERT(rad > 0);
	if (rad2 == 0) rad2 = rad;
	
	// draw 12 segments
	
	glBegin(GL_QUAD_STRIP);
	for (a=0; a<pi*2.01; a+=pi/6) {
		y = rad*cos(a);
		z = rad2*sin(a);
		glNormal3d(0,cos(a),sin(a));
		glVertex3d(0,y,z);
		glVertex3d(length,y,z);
	}
	glEnd();
	
}

// --------------------------------------------------------------------------
void CGLUtils::DrawCone(double rad, double length)
// --------------------------------------------------------------------------
{ // draw solid cone with apex at 0,0,0 and axis pointing in +X direction
	
	double a,y,z;
	double pi = atan(1.0)*4.0;
	double t = atan(rad/length);
	double zx = -sin(t);
	double zr = cos(t);
	
	// draw 12 segments
	
	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(0,0,0);
	for (a=0; a<pi*2.01; a+=pi/6) {
		y = rad*cos(a);
		z = rad*sin(a);
		glNormal3d(zx,cos(a)*zr,sin(a)*zr);
		glVertex3d(length,y,z);
	}
	glEnd();
	
}

// --------------------------------------------------------------------------
void CGLUtils::DrawSphere(double rad)
// --------------------------------------------------------------------------
{ // draw solid sphere centered at 0,0,0
	
	double y,z,a1,a2,x1,x2,rad1,rad2;
	double pi = atan(1.0)*4.0;
	double da = pi/12;
	
	// draw 12 segments
	
	for (a2=0; a2<pi+da; a2+=da) {
		x1 = rad*cos(a2);
		rad1 = rad*sin(a2);
		x2 = rad*cos(a2+da);
		rad2 = rad*sin(a2+da);
		glBegin(GL_QUAD_STRIP);
		for (a1=0; a1<pi*2.01; a1+=da) {
			y = rad1*cos(a1);
			z = rad1*sin(a1);
			glNormal3d(-cos(a2),-sin(a2)*cos(a1),-sin(a2)*sin(a1));
			glVertex3d(x1,y,z);
			y = rad2*cos(a1);
			z = rad2*sin(a1);
			glNormal3d(-cos(a2+da),-sin(a2+da)*cos(a1),-sin(a2+da)*sin(a1));
			glVertex3d(x2,y,z);
		}
		glEnd();
	}
	
}

// --------------------------------------------------------------------------
void CGLUtils::DrawSphere(CVec p, double rad)
// --------------------------------------------------------------------------
{ // draw solid sphere at p
	
	glPushMatrix();
	glTranslated(p.x,p.y,p.z);
	DrawSphere(rad);
	glPopMatrix();

}

// --------------------------------------------------------------------------
void CGLUtils::DrawCube()
// --------------------------------------------------------------------------
{ // draw unit cube

	glBegin(GL_LINE_LOOP);
	glVertex3d(0,0,0);
	glVertex3d(1,0,0);
	glVertex3d(1,1,0);
	glVertex3d(0,1,0);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3d(0,0,1);
	glVertex3d(1,0,1);
	glVertex3d(1,1,1);
	glVertex3d(0,1,1);
	glEnd();

	glBegin(GL_LINES);
	glVertex3d(0,0,1);
	glVertex3d(0,0,0);
	glVertex3d(1,0,1);
	glVertex3d(1,0,0);
	glVertex3d(1,1,1);
	glVertex3d(1,1,0);
	glVertex3d(0,1,1);
	glVertex3d(0,1,0);
	glEnd();

}

// --------------------------------------------------------------------------
void CGLUtils::glVertex(CVec v)
// --------------------------------------------------------------------------
{ // call vertex with Cvec

	glVertex3d(v.x, v.y, v.z);

}

// --------------------------------------------------------------------------
void CGLUtils::glRasterPos(CVec v)
// --------------------------------------------------------------------------
{ // call vertex with Cvec

	glRasterPos3d(v.x, v.y, v.z);

}


// --------------------------------------------------------------------------
void CGLUtils::glColor(CVec v)
// --------------------------------------------------------------------------
{ // call vertex with Cvec

	glColor3d(v.x, v.y, v.z);

}

// --------------------------------------------------------------------------
void CGLUtils::glNormal(CVec v)
// --------------------------------------------------------------------------
{ // call vertex with Cvec

	glNormal3d(v.x, v.y, v.z);

}

// --------------------------------------------------------------------------
void CGLUtils::glTranslate(CVec v)
// --------------------------------------------------------------------------
{ // call vertex with Cvec

	glTranslated(v.x, v.y, v.z);

}


//---------------------------------------------------------------------------
HANDLE CGLUtils::MakeDibFromBitmap(HBITMAP hbitmap, UINT bits )
//---------------------------------------------------------------------------
{ // make DIB from bitmap

	ASSERT(bits == 24 || bits == 8 || bits == 4 || bits == 2 || bits == 1);

	HANDLE              hdib ;
	HDC                 hdc ;
	BITMAP              bitmap ;
	UINT                wLineLen ;
	DWORD               dwSize ;
	DWORD               wColSize ;
	LPBITMAPINFOHEADER  lpbi ;
	LPBYTE              lpBits ;
	
	GetObject(hbitmap,sizeof(BITMAP),&bitmap) ;

	// DWORD align the width of the DIB
	// Figure out the size of the colour table
	// Calculate the size of the DIB
	//
	wLineLen = (bitmap.bmWidth*bits+31)/32 * 4;
	wColSize = sizeof(RGBQUAD)*((bits <= 8) ? 1<<bits : 0);
	dwSize = sizeof(BITMAPINFOHEADER) + wColSize +
		(DWORD)(UINT)wLineLen*(DWORD)(UINT)bitmap.bmHeight;

	//
	// Allocate room for a DIB and set the LPBI fields
	//
	hdib = GlobalAlloc(GHND,dwSize);
	if (!hdib)
		return hdib ;

	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hdib) ;

	lpbi->biSize = sizeof(BITMAPINFOHEADER) ;
	lpbi->biWidth = bitmap.bmWidth ;
	lpbi->biHeight = bitmap.bmHeight ;
	lpbi->biPlanes = 1 ;
	lpbi->biBitCount = (WORD) bits ;
	lpbi->biCompression = BI_RGB ;
	lpbi->biSizeImage = dwSize - sizeof(BITMAPINFOHEADER) - wColSize ;
	lpbi->biXPelsPerMeter = 0 ;
	lpbi->biYPelsPerMeter = 0 ;
	lpbi->biClrUsed = (bits <= 8) ? 1<<bits : 0;
	lpbi->biClrImportant = 0 ;

	//
	// Get the bits from the bitmap and stuff them after the LPBI
	//
	lpBits = (LPBYTE)(lpbi+1)+wColSize ;

	hdc = CreateCompatibleDC(NULL) ;

	GetDIBits(hdc,hbitmap,0,bitmap.bmHeight,lpBits,(LPBITMAPINFO)lpbi, DIB_RGB_COLORS);

	// Fix this if GetDIBits messed it up....
	lpbi->biClrUsed = (bits <= 8) ? 1<<bits : 0;

	DeleteDC(hdc) ;
	GlobalUnlock(hdib);

	return hdib ;
}

// --------------------------------------------------------------------------
void CGLUtils::Rectangle(double x1, double y1, double x2, double y2)
// --------------------------------------------------------------------------
{
		glBegin(GL_POLYGON);
		glVertex3d(x1, y1, 0);
		glVertex3d(x2, y1, 0);
		glVertex3d(x2, y2, 0);
		glVertex3d(x1, y2, 0);
		glEnd();
}