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

// Geometry.cpp: implementation of the CGeometry class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include ".\geometry.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------
CGeometry::CGeometry()
//--------------------------------------------------------------------
{

}

//--------------------------------------------------------------------
CGeometry::~CGeometry()
//--------------------------------------------------------------------
{

}

//--------------------------------------------------------------------
bool CGeometry::LoadSTL(CString filename, int maxsize)
//--------------------------------------------------------------------
{// import stl file (binarry or ascii)

typedef struct {
	char	description[80];
	long	nfacets;
} STLHEADER;

typedef struct {
	float	nx,ny,nz;
	float	x1,y1,z1;
	float	x2,y2,z2;
	float	x3,y3,z3;
	char	pad[2];
} STLFACETINFO;

	CTriangle tri;
	int cnt;

	// read ascii

	CString line;
	CStdioFile afile;
	if (afile.Open(filename, CFile::modeRead) == 0)
		return false;
	BOOL ok = afile.ReadString(line);
	line.TrimLeft();
	if (line.Left(5).CompareNoCase("solid")!=0) {
		afile.Close();
		goto BINARY;
	}

	// read facet loops

	do {
		//		facet normal x y z
		//		outer loop
		//		vertex x y z
		//		vertex x y z
		//		vertex x y z
		//		endloop
		//		endfacet
		//		endsolid name_of_object

		ok = afile.ReadString(line);
		line.Trim(_T("\t *"));
		if (line.Left(8).CompareNoCase("endsolid")!=0 &&
			line.Left(5).CompareNoCase("facet")!=0) {
				afile.Close();
				goto BINARY;
			}

		if (line.Left(8).CompareNoCase("endsolid")==0) {
			afile.Close();
			break;
		}
		cnt = sscanf_s(line, "facet normal %lf %lf %lf", &tri.n.x, &tri.n.y, &tri.n.z);
		if (!ok || cnt != 3) {
			((CFabAtHomeApp *)AfxGetApp())->Log("Error during read. Expected 'facet normal nx ny nz'\n"+line);
			break;
		}
		ok = afile.ReadString(line);
		line.Trim(_T("\t *"));
		if (!ok || line.Left(10).CompareNoCase("outer loop")!=0) {
			((CFabAtHomeApp *)AfxGetApp())->Log("Error during read. Expected 'outer loop'\n"+line);
			break;
		}
		ok = afile.ReadString(line);
		line.Trim(_T("\t *"));
		cnt = sscanf_s(line, "vertex %lf %lf %lf", &tri.v[0].x, &tri.v[0].y, &tri.v[0].z);
		if (!ok || cnt != 3) {
			((CFabAtHomeApp *)AfxGetApp())->Log("Error during read. Expected 'vertex nx ny nz'\n"+line);
			break;
		}
		ok = afile.ReadString(line);
		line.Trim(_T("\t *"));
		cnt = sscanf_s(line, "vertex %lf %lf %lf", &tri.v[1].x, &tri.v[1].y, &tri.v[1].z);
		if (!ok || cnt != 3) {
			((CFabAtHomeApp *)AfxGetApp())->Log("Error during read. Expected 'vertex nx ny nz'\n"+line);
			break;
		}
		ok = afile.ReadString(line);
		line.Trim(_T("\t *"));
		cnt = sscanf_s(line, "vertex %lf %lf %lf", &tri.v[2].x, &tri.v[2].y, &tri.v[2].z);
		if (!ok || cnt != 3) {
			((CFabAtHomeApp *)AfxGetApp())->Log("Error during read. Expected 'vertex nx ny nz'\n"+line);
			break;
		}
		ok = afile.ReadString(line);
		line.Trim(_T("\t *"));
		if (!ok || line.Left(7).CompareNoCase("endloop")!=0) {
			((CFabAtHomeApp *)AfxGetApp())->Log("Error during read. Expected 'endloop'\n"+line);
			break;
		}
		ok = afile.ReadString(line);
		line.Trim(_T("\t *"));
		if (!ok || line.Left(8).CompareNoCase("endfacet")!=0) {
			((CFabAtHomeApp *)AfxGetApp())->Log("Error during read. Expected 'endfacet'\n"+line);
			break;
		}
		mesh.Add(tri);
	} while (ok);


	return true;

	// read binary

BINARY:

	CFile file;
	if (file.Open(filename, CFile::modeRead) == 0)
		return false;

	STLHEADER header;
	STLFACETINFO facetinfo;
	UINT nBytesRead = file.Read(&header, sizeof(header));

	// check

	if (nBytesRead < sizeof(header) || header.nfacets <= 0) {
		((CFabAtHomeApp *)AfxGetApp())->Log("Corrupted file.");
		return false;
	}

	if (header.nfacets > maxsize) {
		CString text;
		text.Format("This file contains %d facets. This number of facets will result it slow graphic update.\n"
			"Please regenerate at lower resolution; meanwhile, truncating to %d\n", header.nfacets, maxsize);
		header.nfacets = maxsize;
	}

	// load

	int i;

	mesh.SetSize(header.nfacets);
	double maxx = -1E30, maxy = -1E30, maxz = -1E30, minx = 1E30, miny = 1E30, minz = 1E30;
	for (i=0; i<min(header.nfacets,maxsize); i++) {
		nBytesRead = file.Read(&facetinfo, 50);
		if (nBytesRead < 50) {
			((CFabAtHomeApp *)AfxGetApp())->Log("Corrupted file - premature end of file.");
			return 0;
		}
		mesh[i].v[0] = CVec(facetinfo.x1,facetinfo.y1,facetinfo.z1);
		mesh[i].v[1] = CVec(facetinfo.x2,facetinfo.y2,facetinfo.z2);
		mesh[i].v[2] = CVec(facetinfo.x3,facetinfo.y3,facetinfo.z3);
		mesh[i].n = CVec(facetinfo.nx,facetinfo.ny,facetinfo.nz);
		mesh[i].n.Normalize();
		maxx = max(max(maxx,facetinfo.x1),max(facetinfo.x2,facetinfo.x3));
		minx = min(min(minx,facetinfo.x1),min(facetinfo.x2,facetinfo.x3));
		maxy = max(max(maxy,facetinfo.y1),max(facetinfo.y2,facetinfo.y3));
		miny = min(min(miny,facetinfo.y1),min(facetinfo.y2,facetinfo.y3));
		maxz = max(max(maxz,facetinfo.z1),max(facetinfo.z2,facetinfo.z3));
		minz = min(min(minz,facetinfo.z1),min(facetinfo.z2,facetinfo.z3));
	}
	file.Close();

	double size = max(max(maxx-minx,maxy-miny),maxz-minz);

	return true;

}


//--------------------------------------------------------------------
bool CGeometry::LoadVCT(CString filename)
//--------------------------------------------------------------------
{
	//load a vector (VCT or TXT) file into geometry.vectors, a CPath array
	//VCT files should be ASCII formatted
	//each line representing a vertex as an ordered triplet separated by white space
	//the end of a path is indicated by a blank line

	CVec vtx;
	CPath pth;
	int cnt;

	// read ascii

	CString line;
	CStdioFile afile;
	if (afile.Open(filename, CFile::modeRead) == 0)
		return false;
	BOOL ok;

	// read vertices

	do {
		ok = afile.ReadString(line);
		//skip any comment lines
		if((line.Find("//") >= 0) || (line.Find("/*") >= 0))
		{
			continue;
		}
		//trim leading and trailing whitespace and comment characters
		line.Trim(_T("\t *"));
		if(line.GetLength() < 5) //min of 5 characters to represent 3 numbers with spaces between
		{
			//so assume this is the end of a path or the start of the file
			if(pth.v.GetSize() > 0)
			{
				vectors.Add(pth);
				pth.v.RemoveAll();
			}
			continue;
		}
		//read a vertex
		cnt = sscanf_s(line, "%lf %lf %lf", &vtx.x, &vtx.y, &vtx.z);
		if (!ok || cnt != 3) {
			((CFabAtHomeApp *)AfxGetApp())->Log("Error during read. Expected 'x1 y1 z1 x2 y2 z2 ...'\n");
			break;
		}
		pth.v.Add(vtx);
	} while (ok);
	afile.Close();
	return true;
}



//--------------------------------------------------------------------
bool CGeometry::AppendGeometry(CGeometry& geom)
//--------------------------------------------------------------------
{// add mesh from another object

	mesh.Append(geom.mesh);

	return true;
}


//---------------------------------------------------------------------------
void CGeometry::Serialize(CArchive& ar)
//---------------------------------------------------------------------------
{// Save / Load

	int version = 10;

	if (ar.IsStoring()) { //add storing code here

		ar << version;

		// save data

		int k = mesh.GetSize();
		ar << k;
		for (int i=0; i<k; i++) {
			mesh[i].Serialize(ar);
		}

		ar << version;

	}	else	{  // add loading code here

		int version1, version2;
		ar >> version1; if (version1 < 10 || version1 > version) throw 6;

		// load data

		int k;
		ar >> k;
		mesh.SetSize(k);
		for (int i=0; i<k; i++) {
			mesh[i].Serialize(ar);
		}

		ar >> version2; if (version2 != version1) throw 6;

	}
}

//---------------------------------------------------------------------------
CGeometry::CGeometry(CGeometry& s)
//---------------------------------------------------------------------------
{
	*this = s;
}

//---------------------------------------------------------------------------
CGeometry& CGeometry::operator=(const CGeometry& s)
//---------------------------------------------------------------------------
{

	mesh.Copy(s.mesh);
	vectors.Copy(s.vectors);
	name = s.name;

	return *this;
}


//---------------------------------------------------------------------------
void CGeometry::Draw(bool bModelhNormals, bool bShaded)
//---------------------------------------------------------------------------
{
	if(mesh.GetSize() > 0)
	{
		if (bShaded) {
			glBegin(GL_TRIANGLES);
			for (int i=0; i<mesh.GetSize(); i++) {
				glNormal3d(mesh[i].n.x, mesh[i].n.y, mesh[i].n.z);
				for (int j=0; j<3; j++) {
					glVertex3d(mesh[i].v[j].x,mesh[i].v[j].y,mesh[i].v[j].z);
				}
			}
			glEnd();
		} else { // wireframe
			for (int i=0; i<mesh.GetSize(); i++) {
				glBegin(GL_LINE_LOOP);
				glNormal3d(mesh[i].n.x, mesh[i].n.y, mesh[i].n.z);
				for (int j=0; j<3; j++) {
					glVertex3d(mesh[i].v[j].x,mesh[i].v[j].y,mesh[i].v[j].z);
				}
				glEnd();
			}
		}

		if (bModelhNormals) {
			glColor3d(1,1,0);
			glBegin(GL_LINES);
			for (int i=0; i<mesh.GetSize(); i++) {
				CVec c = (mesh[i].v[0] + mesh[i].v[1] + mesh[i].v[2])/3;
				CVec c2 = c - mesh[i].n*3;
				glVertex3d(c.x, c.y, c.z);
				glVertex3d(c2.x, c2.y, c2.z);
			}
			glEnd();
		}
	}
	if(vectors.GetSize() > 0)
	{
		glBegin(GL_LINE_STRIP);
		for(int i = 0; i < vectors.GetSize(); i++)
		{
			for(int j = 0; j < vectors[i].v.GetSize(); j++)
			{
				glVertex3d(vectors[i].v[j].x,vectors[i].v[j].y,vectors[i].v[j].z);
			}
		}
		glEnd();
	}
}

//---------------------------------------------------------------------------
bool CGeometry::ComputeZSlice(double z, CArray <CPath,CPath>& paths)
//---------------------------------------------------------------------------
{ // compute slice of geometry at height Z, store in paths

	CVec p1, p2;
	int count = 0;
	for (int i=0; i<mesh.GetSize(); i++) {
		if (mesh[i].Intersect(z, p1, p2)) {
			CPath newpath;
			newpath.v.Add(p1);
			newpath.v.Add(p2);
			paths.Add(newpath);
			count++;
		}
	}

	if(count > 0) return true;
	return false;
}

//---------------------------------------------------------------------------
void CGeometry::ComputeBoundingBox(CVec &pmin, CVec &pmax)
//---------------------------------------------------------------------------
{

	if (mesh.GetSize() > 0)
	{
		pmin = pmax = mesh[0].v[0];
	} 
	else if (vectors.GetSize() > 0)
	{
		pmin = pmax = vectors[0].v[0];
	}
	
	for (int i=0; i<mesh.GetSize(); i++) {
		for (int j=0; j<3; j++) {
			pmin.x = min(pmin.x, mesh[i].v[j].x);
			pmin.y = min(pmin.y, mesh[i].v[j].y);
			pmin.z = min(pmin.z, mesh[i].v[j].z);
			pmax.x = max(pmax.x, mesh[i].v[j].x);
			pmax.y = max(pmax.y, mesh[i].v[j].y);
			pmax.z = max(pmax.z, mesh[i].v[j].z);
		}
	}
	for(int i=0; i<vectors.GetSize(); i++)
	{
		for(int j=0; j<vectors[i].v.GetSize(); j++)
		{
			pmin.x = min(pmin.x, vectors[i].v[j].x);
			pmin.y = min(pmin.y, vectors[i].v[j].y);
			pmin.z = min(pmin.z, vectors[i].v[j].z);
			pmax.x = max(pmax.x, vectors[i].v[j].x);
			pmax.y = max(pmax.y, vectors[i].v[j].y);
			pmax.z = max(pmax.z, vectors[i].v[j].z);
		}
	}
}


//---------------------------------------------------------------------------
void CGeometry::Translate(CVec d)
//---------------------------------------------------------------------------
{// translate geometry

	for (int i=0; i<mesh.GetSize(); i++) {
		for (int j=0; j<3; j++) {
			mesh[i].v[j] += d;
		}
	}
	for(int i = 0; i<vectors.GetSize(); i++)
	{
		for(int j = 0; j<vectors[i].v.GetSize(); j++)
		{
			vectors[i].v[j] +=d;
		}
	}
}

//---------------------------------------------------------------------------
void CGeometry::Scale(CVec s)
//---------------------------------------------------------------------------
{// scale geometry

	//check for zero scale factor
	if(s.x<=0 || s.y<=0 || s.z<=0) return;
	for (int i=0; i<mesh.GetSize(); i++) {
		for (int j=0; j<3; j++) {
			mesh[i].v[j].x *= s.x;
			mesh[i].v[j].y *= s.y;
			mesh[i].v[j].z *= s.z;
		}
		mesh[i].n.x *= s.x;
		mesh[i].n.y *= s.y;
		mesh[i].n.z *= s.z;
		mesh[i].n.Normalize();
	}

	for(int i = 0; i<vectors.GetSize(); i++)
	{
		for(int j = 0; j<vectors[i].v.GetSize(); j++)
		{
			vectors[i].v[j].x *= s.x;
			vectors[i].v[j].y *= s.y;
			vectors[i].v[j].z *= s.z;
		}
	}

}

//---------------------------------------------------------------------------
void CGeometry::Rotate(CVec ax, double a)
//---------------------------------------------------------------------------
{

	for (int i=0; i<mesh.GetSize(); i++) {
		for (int j=0; j<3; j++) {
			mesh[i].v[j] = mesh[i].v[j].Rot(ax, a);
		}
		mesh[i].n = mesh[i].n.Rot(ax, a);
	}

	for(int i = 0; i<vectors.GetSize(); i++)
	{
		for(int j = 0; j<vectors[i].v.GetSize(); j++)
		{
			vectors[i].v[j] = vectors[i].v[j].Rot(ax,a);
		}
	}
}

//---------------------------------------------------------------------------
void CGeometry::RotX(double a)
//---------------------------------------------------------------------------
{

	for (int i=0; i<mesh.GetSize(); i++) {
		for (int j=0; j<3; j++) {
			mesh[i].v[j].RotX(a);
		}
		mesh[i].n.RotX(a);
	}
	for(int i = 0; i<vectors.GetSize(); i++)
	{
		for(int j = 0; j<vectors[i].v.GetSize(); j++)
		{
			vectors[i].v[j].RotX(a);
		}
	}
}


//---------------------------------------------------------------------------
void CGeometry::RotY(double a)
//---------------------------------------------------------------------------
{

	for (int i=0; i<mesh.GetSize(); i++) {
		for (int j=0; j<3; j++) {
			mesh[i].v[j].RotY(a);
		}
		mesh[i].n.RotY(a);
	}
	for(int i = 0; i<vectors.GetSize(); i++)
	{
		for(int j = 0; j<vectors[i].v.GetSize(); j++)
		{
			vectors[i].v[j].RotY(a);
		}
	}
}


//---------------------------------------------------------------------------
void CGeometry::RotZ(double a)
//---------------------------------------------------------------------------
{

	for (int i=0; i<mesh.GetSize(); i++) {
		for (int j=0; j<3; j++) {
			mesh[i].v[j].RotZ(a);
		}
		mesh[i].n.RotZ(a);
	}
	for(int i = 0; i<vectors.GetSize(); i++)
	{
		for(int j = 0; j<vectors[i].v.GetSize(); j++)
		{
			vectors[i].v[j].RotZ(a);
		}
	}
}


//---------------------------------------------------------------------------
void CGeometry::Clear(void)
//---------------------------------------------------------------------------
{
	mesh.RemoveAll();
	vectors.RemoveAll();
	name = "";
}
