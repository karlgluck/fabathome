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

// Path.cpp: implementation of the CPath class.
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
CPath::CPath()
// --------------------------------------------------------------------------
{
	type = PATHTYPE_ERROR;
}

// --------------------------------------------------------------------------
CPath::~CPath()
// --------------------------------------------------------------------------
{
	v.RemoveAll();
}

//---------------------------------------------------------------------------
void CPath::Serialize(CArchive& ar)
//---------------------------------------------------------------------------
{// Save / Load

	int version = 10;

	if (ar.IsStoring()) { //add storing code here

		ar << version;

		// save data

		ar << (int)type;
		v.Serialize(ar);

		ar << version;

	}	else	{  // add loading code here

		int version1, version2;
		ar >> version1; if (version1 < 10 || version1 > version) throw 6;

		// load data

		ar >> (int &)type;
		v.Serialize(ar);

		ar >> version2; if (version2 != version1) throw 6;

	}
}

//---------------------------------------------------------------------------
CPath::CPath(CPath& s)
//---------------------------------------------------------------------------
{
	type = s.type;
	v.Copy(s.v);
}

//---------------------------------------------------------------------------
CPath& CPath::operator=(const CPath& s)
//---------------------------------------------------------------------------
{
	type = s.type;
	v.Copy(s.v);
	return *this;
}

//---------------------------------------------------------------------------
bool CPath::operator==(const CPath& s)
//---------------------------------------------------------------------------
{
	if(v.GetSize() != s.v.GetSize()) return false;
	if( type != s.type ) return false;
	for(int i = 0; i < v.GetSize(); i++)
	{
		if(v[i] != s.v[i]) return false;
	}
	return true;
}

//---------------------------------------------------------------------------
void CPath::Draw(bool bPathNormals, CVec c)
//---------------------------------------------------------------------------
{
	CVec col;

	switch (type) {
	case PATHTYPE_BOUNDARY: col=c; break; // boundary
	case PATHTYPE_CONTOUR: col=CVec(c.x,c.y,1.0); break; // contour
	case PATHTYPE_RASTER: col=CVec(c.x,1.0,c.y); break; // scanline
	case PATHTYPE_VECTOR: col=CVec(c.x,0.5,0.5); break;	//vector data
	case PATHTYPE_TRACEDEP: col=CVec(1,0,0); break; //trace of actual deposition paths followed
	case PATHTYPE_TRACEJOG: col=CVec(0,1,0); break; //trace of jog (non-deposition) paths followed
	case PATHTYPE_ERROR: col=CVec(1,0,0); break; // error line
	}
	glColor3d(col.x,col.y,col.z);
	glBegin(GL_LINE_STRIP);
	for (int i=0; i<v.GetSize(); i++) 
	{
			glVertex3d(v[i].x, v[i].y, v[i].z);
	}
	if(IsClosed())
		glVertex3d(v[0].x, v[0].y, v[0].z);
	glEnd();
	CFabAtHomeView::ReportGLError(CString("CPath::Draw:GLEnd"));
	//if deposition paths, mark start and end vertices
	if(!((type == PATHTYPE_BOUNDARY) || (type == PATHTYPE_ERROR)))
	{
		glColor3d(0,1,0);
		v[0].DrawMarker();
		glColor3d(1,0,0);
		v[v.GetSize()-1].DrawMarker();
		glColor3d(col.x,col.y,col.z);
	}

	if (bPathNormals && (type == PATHTYPE_CONTOUR) && (v.GetSize() >= 2)) {
		// draw line pointing outwards
		CVec z(0,0,1);
		CVec ax = v[1]-v[0];
		CVec ay = (z.Cross(ax)).Normalized();
		glColor3d(1,1,0);
		glBegin(GL_LINES);
		glVertex3d(v[0].x, v[0].y, v[0].z);
		glVertex3d(v[0].x+ay.x, v[0].y+ay.y, v[0].z+ay.z);
		glEnd();
	}

}

//---------------------------------------------------------------------------
void CPath::ConvexHullXY(CArray<int,int>* pIdx)
//---------------------------------------------------------------------------
{
	//calculate the convex hull of *this in the XY plane, return array of indices
	if(pIdx == NULL) return;

	CVec* P = new CVec[v.GetSize()];
	CVec* H = new CVec[v.GetSize()];
	int* idx = new int[v.GetSize()];
	int i;
	for(i = 0; i < v.GetSize(); i++)
	{
		P[i] = v[i];
	}
	int cnt = CUtils::ChainHull_2D(P,v.GetSize(),H,idx);
	for(int i = 0; i < cnt; i++)
		pIdx->Add(idx[i]);
}

//---------------------------------------------------------------------------
void CPath::CalcBBox(CVec xax, CVec yax, CVec zax, CVec &pmin, CVec &pmax)
//---------------------------------------------------------------------------
{

	if (v.GetSize() == 0)
		return;

	pmax = CVec(-1E10, -1E10, -1E10);
	pmin = CVec(1E10, 1E10, 1E10);

	for (int j=0; j<v.GetSize(); j++)
	{
		double x = v[j].Dot(xax);
		double y = v[j].Dot(yax);
		double z = v[j].Dot(zax);
		if (x<-100)
		{
			CVec vv = v[j];
			int n = v.GetSize();
		}
		pmin.x = min(pmin.x, x);
		pmin.y = min(pmin.y, y);
		pmin.z = min(pmin.z, z);
		pmax.x = max(pmax.x, x);
		pmax.y = max(pmax.y, y);
		pmax.z = max(pmax.z, z);
	}
}

//---------------------------------------------------------------------------
void CPath::CalcBBox(CVec &pmin, CVec &pmax)
//---------------------------------------------------------------------------
{
	//Find the bounding box for this path in the default coord system
	//return corners of box in pmin, pmax
  
	pmax = CVec(-1E10, -1E10, -1E10);
	pmin = CVec(1E10, 1E10, 1E10);

	for (int i=0; i<v.GetSize(); i++)
	{
			double x = v[i].x;
			double y = v[i].y;
			double z = v[i].z;
			pmin.x = min(pmin.x, x);
			pmin.y = min(pmin.y, y);
			pmin.z = min(pmin.z, z);
			pmax.x = max(pmax.x, x);
			pmax.y = max(pmax.y, y);
			pmax.z = max(pmax.z, z);
	}
	//TRACE("BBox:pmin(%f,%f,%f), pmax(%f,%f,%f)\n", pmin.x,pmin.y,pmin.z,pmax.x,pmax.y,pmax.z);
}


//---------------------------------------------------------------------------
void CPath::SetPathTypes(CArray<CPath,CPath>& paths, PATHTYPE t)
//---------------------------------------------------------------------------
{ 

	for (int i=0; i<paths.GetSize(); i++) {
		paths[i].type = t;
	}

}

//---------------------------------------------------------------------------
void CPath::Reverse()
//---------------------------------------------------------------------------
{

	int n = v.GetSize();
	CVec tmp;

	for (int i=0; i<n/2; i++) {
		tmp = v[i];
		v[i] = v[n-1-i];
		v[n-1-i] = tmp;
	}

}

//---------------------------------------------------------------------------
bool CPath::Offset(double d, CVec z, CArray<CPath,CPath>& output)
//---------------------------------------------------------------------------
{// Create offset paths by moving to left by d

	CPath tmp = *this;
	tmp.Clean(d/10.0);
	int n = tmp.v.GetSize();

	//handle degenerate contour paths (<3 vertices)
	if(n < 3)
	{
		output.Add(*this);
		return false;
	}
	bool closed = IsClosed();
	
	CVec v0, vp, vn;
	CPath tmp2 = tmp;
	CArray<INTERSECTION,INTERSECTION> intxnA, intxnB;

	for (int i=0; i<n; i++) {
		// this vertex
		v0 = tmp.v[i];
		// previous vertex
		if (i > 0) {
			vp = tmp.v[i-1];
		} else {
			if (closed) {
				vp = tmp.v[n-1]; // close contour
			} else {
				vp = tmp.v[i] - (tmp.v[1]-tmp.v[0]); // symmetry
			}
		}
		// next vertex
		if (i < n-1) {
			vn = tmp.v[i+1];
		} else {
			if (closed) {
				vn = tmp.v[0]; // close contour
			} else {
				vn = tmp.v[i] + (tmp.v[i]-tmp.v[i-1]); // symmetry
			}
		}
		CVec ax1 = (v0-vp).Normalized();
		ASSERT(fabs(ax1.Length()-1)<0.001);
		CVec ax2 = (vn-v0).Normalized();
		ASSERT(fabs(ax2.Length()-1)<0.001);
		CVec n = (ax1.Cross(ax2)).Normalized();
		if (n.Dot(z) < 0) {
			n = -n;
		}
		n = z;
		CVec off1 = (n.Cross(ax1)).Normalized();
		CVec off2 = (n.Cross(ax2)).Normalized();
		CVec s1, s2;
		if (CUtils::SkewIntersection(vp+off1*d, v0+off1*d, v0+off2*d, vn+off2*d, s1, s2) && s1.IsValid() && s2.IsValid()) {
			ASSERT((s1-s2).Length()<0.1);
			ASSERT(fabs(s1.z-v[i].z)<0.01);
			ASSERT(fabs(s2.z-v[i].z)<0.01);
			tmp2.v[i] = (s1+s2)/2.0;
		} else {
			tmp2.v[i] += (off1+off2)/2*d;
		}
	}
	/*CString fname;
	long cnt = GetTickCount();
	fname.Format("D:\\My Documents\\Fab@Home\\Fab@Home Application\\SourceForge\\%d_A.dat",cnt);
	tmp2.Log(fname);*/
	tmp2.type = PATHTYPE_CONTOUR;
	tmp2.RemoveSharpCorners(3.0*PI/4.0,0.1,0.1);
	if(tmp2.v.GetSize() > 0)
		output.Add(tmp2);
	/*fname.Format("D:\\My Documents\\Fab@Home\\Fab@Home Application\\SourceForge\\%d_B.dat",cnt);
	tmp2.Log(fname);*/
	//int crossings = tmp2.FindIntersections(tmp2,intxnA,intxnB);
	//TRACE("Found %d self-crossings after offset\n", crossings);
	//tmp2.PruneSelfIntersections(intxnA, intxnB, output);
	//tmp2.PruneOtherIntersections(tmp2,intxnA, intxnB, output);
	//for(int i = 0; i < output.GetSize(); i++)
	//{
	//	fname.Format("D:\\My Documents\\Fab@Home\\Fab@Home Application\\SourceForge\\%d_C%d.dat",cnt,i);
	//	output[i].Log(fname);
	//}
		
	return true;
}

//---------------------------------------------------------------------------
bool CPath::PruneSelfIntersections(CArray<INTERSECTION,INTERSECTION>& va, 
								   CArray<INTERSECTION,INTERSECTION>& vb,
								   CArray<CPath,CPath>& output)
//---------------------------------------------------------------------------
{
	//this is a rough starting effort at pruning self-intersections after offsets
	//it should calculate the area of the sequence of vertices between intersections
	//and if the area is negative, the vertices should be pruned back to the intersection
	//need to remember that v[0] might actually be in a negative area loop
	//return true if intersections were removed, or none present, false otherwise

	ASSERT(va.GetSize() == vb.GetSize());

	//no intersections found, so just return this
	if(va.GetSize() < 1)
	{
		output.Add(*this);
		return true;
	}
	
	CArray<CPath,CPath> chains;

	for(int curX = 0; curX < va.GetSize(); curX++)
	{
		int prvX, nxtX, V1, V2;
		CPath tmp;
		bool multiple = false;
		tmp.type = PATHTYPE_CONTOUR;
		//build a first chain between intersection points
		tmp.v.Add(va[curX].pos);
		//add the vertices from this path between the intersections
		if(curX < va.GetSize()-1)
		{
			nxtX = curX+1;
			V1 = (va[curX].index+1)%v.GetSize();
			V2 = va[nxtX].index;
		}
		else
		{
			nxtX = 0;
			V1 = (va[curX].index+1)%v.GetSize();
			V2 = va[nxtX].index;
		}

		prvX = curX-1;
		if(curX == 0) prvX = va.GetSize()-1;

		//if indices have wrapped around
		if(V2 < V1-1)
		{
			V2 += v.GetSize();
		}
		if(va[curX].index == va[prvX].index) //> 1st crossing of same segment
		{
			//TRACE("MULTn prvX: %d, curX:%d, nxtX:%d, V1:%d, V2:%d\n", prvX, curX, nxtX, V1, V2);
			//add previous crossing point
			tmp.v.Add(va[prvX].pos);
			//add segments forward from there to current crossing
			for(int j = vb[prvX].index+1; j <= vb[curX].index; j++)
				tmp.v.Add(v[j%v.GetSize()]);
			//add second crossing point
			tmp.v.Add(vb[curX].pos);
		}
		else if(va[curX].index == va[nxtX].index) // 1st of multiple crossings of same segment
		{
			//TRACE("MULT1 prvX: %d, curX:%d, nxtX:%d, V1:%d, V2:%d\n", prvX, curX, nxtX, V1, V2);
			//add segments from current crossing to next crossing
			for(int j = va[curX].index+1; j <= vb[curX].index; j++)
				tmp.v.Add(v[j%v.GetSize()]);
			//add second crossing point
			tmp.v.Add(vb[curX].pos);
		}
		else //normal single crossing
		{
			//TRACE("SINGL prvX:%d, curX:%d, nxtX:%d, V1:%d, V2:%d\n", prvX, curX, nxtX, V1, V2);
			for(int j = V1; j <= V2; j++)
				tmp.v.Add(v[j%v.GetSize()]);
			//add the second intersection point
			tmp.v.Add(va[nxtX].pos);
		}

		//now check whether this chain should be kept or pruned
		//if the chain looks to be closed (we are using endpoint proximity here!, not a flag)
		if(tmp.v[0] == tmp.v[tmp.v.GetSize()-1])
		{
			//remove the duplicate endpoint to conform to closed path standards
			tmp.v.RemoveAt(tmp.v.GetSize()-1);
			//if the area is positive
			if(tmp.PolyArea2() > 0.0)
			{
				//keep the chain as a new path
				chains.Add(tmp);
				//TRACE("Positive Area Chain\n");
			}
			else
			{
				//TRACE("Nonpositive Area Chain\n");
			}
		}
		else
		{
			chains.Add(tmp);
			//TRACE("Open Chain\n");
		}
		//otherwise we throw out the chain
	}
	//TRACE("%d chains created;", chains.GetSize());
	CPath::Consolidate(chains,CVEC_TOL);
	CPath::Clean(chains,5.0*CVEC_TOL);
	//TRACE("consolidated into %d chains\n", chains.GetSize());
	//TRACE("this->v.GetSize():%d\n",v.GetSize());
	/*for(int i = 0; i < chains.GetSize(); i++)
	{
		CString fname;
		fname.Format("D:\\My Documents\\Fab@Home\\Fab@Home Application\\SourceForge\\chains%d.dat",i);
		chains[i].Log(fname);
		TRACE("Chains[%d].v.GetSize():%d\n",i,chains[i].v.GetSize());
	}*/

	//output the finished chains
	output.Append(chains);
	return true;
}

//---------------------------------------------------------------------------
bool CPath::PruneOtherIntersections(CArray<CPath,CPath>& raB, CArray<CPath,CPath>& output)
//---------------------------------------------------------------------------
{
	//for each path in raB, find intersections with this
	//keep loops with positive area, throw out rest
	//put results in output
	CArray<INTERSECTION,INTERSECTION> va, vb; 
	for(int i = 0; i < raB.GetSize(); i++)
	{
		if(FindIntersections(raB[i], va, vb) > 0)
			PruneOtherIntersections(raB[i], va, vb, output);
	}
	return true;
}

//---------------------------------------------------------------------------
bool CPath::PruneOtherIntersections(CPath& B, CArray<INTERSECTION,INTERSECTION>& va, 
								   CArray<INTERSECTION,INTERSECTION>& vb,
								   CArray<CPath,CPath>& output)
//---------------------------------------------------------------------------
{
	//prune intersections between this path (to be called A) and B
	//basically form closed loops from one crossing to the next and back
	//keep the loop if the area is positive
	//return all positive area loops in output

	ASSERT(va.GetSize() == vb.GetSize());
	ASSERT(B.type == PATHTYPE_CONTOUR);
	ASSERT(type == PATHTYPE_CONTOUR);

	//no intersections found, so just return this and other
	if(va.GetSize() < 1)
	{
		output.Add(*this);
		output.Add(B);
		return true;
	}
	
	//change some variable names for clarity
	CPath &A = *this;
	int NX = va.GetSize(); //# of intersections

	CPath tmp; //for building chains
	tmp.type = PATHTYPE_CONTOUR;
	CArray<CPath,CPath> chains; //for accumulating chains to be kept

	for(int curX = 0; curX < NX; curX++)
	{
		//TRACE("NX:%d, curX:%d\n", NX, curX);
		int prvX, nxtX, Va1, Va2, Vb1, Vb2;

		//handle wrapping around on closed (polygonal) paths
		if(curX < va.GetSize()-1)
		{
			nxtX = curX+1;

			Va1 = (va[curX].index+1)%A.v.GetSize();
			Va2 = va[nxtX].index;

			Vb1 = (vb[curX].index+1)%B.v.GetSize();
			Vb2 = vb[nxtX].index;
		}
		else
		{
			nxtX = 0;

			Va1 = (va[curX].index+1)%A.v.GetSize();
			Va2 = va[nxtX].index;

			Vb1 = (vb[curX].index+1)%B.v.GetSize();
			Vb2 = vb[nxtX].index;
		}

		prvX = curX-1;
		if(curX == 0) prvX = NX-1;

		//if indices have wrapped around
		if(Va2 < Va1-1)
		{
			Va2 += A.v.GetSize();
		}
		if(Vb2 < Vb1-1)
		{
			Vb2 += B.v.GetSize();
		}

		//start with the first intersection point on path A
		tmp.v.Add(va[curX].pos);
		//as long as the current and next intersection are not both on the same
		//segment of this path, add the segments of A between the intersections
		if(va[curX].index != va[nxtX].index)
		{
			//TRACE("STDA prvX: %d, curX:%d, nxtX:%d, Va1:%d, Va2:%d\n", prvX, curX, nxtX, Va1, Va2);
			//add segments from current crossing to next crossing
			for(int j = va[curX].index+1; j <= va[nxtX].index; j++)
				tmp.v.Add(A.v[j%A.v.GetSize()]);
		}
		//add the next intersection point on A
		tmp.v.Add(va[nxtX].pos);
		
		//now do the same thing on the return leg for B
		//add the closes intersection on path B to our current location (nxtX)
		tmp.v.Add(vb[nxtX].pos);
		//as long as the current and next intersection are not both on the same
		//segment of this path, add the segments of B between the intersections
		if(vb[nxtX].index != vb[curX].index)
		{
			//TRACE("STDB prvX: %d, curX:%d, nxtX:%d, Vb1:%d, Vb2:%d\n", prvX, curX, nxtX, Vb1, Vb2);
			//add segments from current crossing to next crossing
			for(int j = vb[nxtX].index+1; j <= vb[curX].index; j++)
				tmp.v.Add(B.v[j%B.v.GetSize()]);
		}
		//close the chain with the following intersection point on B
		tmp.v.Add(vb[curX].pos);

		//tmp.Log(CString("D:\\My Documents\\Fab@Home\\Fab@Home Application\\SourceForge\\PruneOtherTemp.dat"));
		//now check whether this chain should be kept or pruned
		//if the area is positive
		if(tmp.PolyArea2() > 0.0)
		{
			//keep the chain as a new path
			chains.Add(tmp);
			//TRACE("Positive Area Chain\n");
		}
		else
		{
			//TRACE("Nonpositive Area Chain\n");
		}

	}
	//TRACE("%d other intersection chains created\n", chains.GetSize());
	CPath::Consolidate(chains,CVEC_TOL);
	CPath::Clean(chains,5.0*CVEC_TOL);
	//TRACE("consolidated into %d chains\n", chains.GetSize());
	//TRACE("this->v.GetSize():%d\n",v.GetSize());
	/*for(int i = 0; i < chains.GetSize(); i++)
	{
		CString fname;
		fname.Format("D:\\My Documents\\Fab@Home\\Fab@Home Application\\SourceForge\\otherchains%d.dat",i);
		chains[i].Log(fname);
		TRACE("Chains[%d].v.GetSize():%d\n",i,chains[i].v.GetSize());
	}*/

	//output the finished chains
	output.Append(chains);
	return true;
}

//---------------------------------------------------------------------------
bool CPath::Clean(double tol)
//---------------------------------------------------------------------------
{
	//recursively search through path for vertices closer than tol
	bool removed = false;
	//don't eliminate contours or boundaries

	for (int i=1; i<v.GetSize(); i++) {
		if(IsClosed() && v.GetSize() < 4) return false;
		if ((v[i]-v[i-1]).Length() < tol)  {
			v.RemoveAt(i);
			i--;
			removed = true;
		}
	}
	if(v.GetSize() > 0)
	{
		if(IsClosed() && (v.GetSize() > 3)){
			if((v[0]-v[v.GetSize()-1]).Length() < tol)
			{
				v.RemoveAt(v.GetSize()-1);
				removed = true;
			}
		}
	}
	if(removed) return Clean(tol);
	//Log(CString("D:\\My Documents\\Fab@Home\\Fab@Home Application\\SourceForge\\Should Be Clean.txt"));
	return removed;
}

//---------------------------------------------------------------------------
double CPath::GetLength()
//---------------------------------------------------------------------------
{

	double len = 0;
	for (int i=0; i<v.GetSize()-1; i++) {
		len += (v[i+1]-v[i]).Length();
	}
	if(IsClosed())
		len += (v[0]-v[v.GetSize()-1]).Length();
	return len;
}

//---------------------------------------------------------------------------
bool CPath::FindSplitPoint(double pos, double tol, int& insertIdx, CVec &newpt)
//---------------------------------------------------------------------------
{
	//sum the length of the path up to the vertex at which length ~ pos
	//if pos falls between two vertices, calculate an appropriate vertex
	//return true if pos falls in-between vertices, false if it is less than
	//tol away from an extant vertex
	//insertIdx will return the index at which an insertion should take place, if necessary
	//or the index of an extant vertex if one is within tol of pos
	//note that for closed paths, insertIdx might be 0

	ASSERT(pos>0 && pos <GetLength());

	int i;
	double len = 0;
	int nseg = v.GetSize();
	if(!IsClosed()) nseg--;
	double seglen;
	int nextIdx;

	for (i=0; i<nseg; i++)
	{
		nextIdx = i+1;
		if(IsClosed()&& (i>nseg-2))
			nextIdx %= nseg;

		//if pos is within tol of this vertex
		if (fabs(pos-len)<tol)
		{
			TRACE("A\n");
			newpt = v[i];
			insertIdx = i;
			return false;
		}
		seglen = (v[nextIdx]-v[i]).Length();
		//if pos is within tol of the next vertex
		if (fabs(pos-(len+seglen))<tol)
		{
			TRACE("B\n");
			newpt = v[nextIdx];
			insertIdx = nextIdx;
			return false;
		}
		//if pos is somewhere between this and the next
		if ((pos > len) && (pos < len+seglen)) {
			newpt = v[i]+(((pos-len)/seglen)*(v[nextIdx]-v[i]));
			insertIdx = nextIdx;
			return true;
		}
		len += seglen;
	}
	newpt = v[i];
	insertIdx = i;
	return false;
}

//---------------------------------------------------------------------------
bool CPath::AddSplitPoint(double pos, double tol, int& insertIdx, CVec &newpt)
//---------------------------------------------------------------------------
{
	//sum the length of the path up to the vertex at which length ~ pos
	//if pos falls between two vertices, insert an appropriate vertex
	//return true if a point was inserted
	ASSERT(pos>0 && pos <GetLength());
	bool added = FindSplitPoint(pos,tol,insertIdx,newpt);
	if(added)
	{
		//if closed path and the split was between last and first vertex of  closed path
		if(IsClosed() && (insertIdx == 0)) 
			v.InsertAt(v.GetSize(), newpt);
		else
			v.InsertAt(insertIdx, newpt);
	}
	return added;
}

//---------------------------------------------------------------------------
void CPath::Interpolate(double maxLen)
//---------------------------------------------------------------------------
{
	//insert vertices into path so that no segment is longer than maxLen

	ASSERT(maxLen > 0.0);
	CArray<CVec,CVec> tmpVtxs;
	for(int i = 0; i < v.GetSize(); i++)
	{
		//add the first vertex of this segment
		tmpVtxs.Add(v[i]);

		//interpolate vertices as necessary
		
		if((i < v.GetSize()-1) || IsClosed())
		{
			CVec diff;
			if(IsClosed() && (i == v.GetSize()-1))
				diff = v[0] - v[v.GetSize()-1];
			else
				diff = v[i+1] - v[i];
			double len = diff.Length();
			if(len > maxLen)
			{
				CVec dir = diff.Normalized();
				int n = (int)ceil(len/maxLen);
				double spacing = len/n;
				for(int j = 0; j<n-1; j++)
				{
					tmpVtxs.Add(v[i]+j*dir);
				}
			}
		}
	}
	//now overwrite v with the new set of vertices
	v.Copy(tmpVtxs);
}
			
//---------------------------------------------------------------------------
void CPath::CircShift(int n)
//---------------------------------------------------------------------------
{
	//circularly shift contour path start by n vertices
	if (type != PATHTYPE_CONTOUR) return;
	n %= v.GetSize();
	//TRACE("getsize = %d\n", v.GetSize());
	//TRACE("n = %d\n", n);
	if (n == 0) return;
	CArray<CVec, CVec> temp;
	temp.SetSize(v.GetSize());
	int i;
	for(i = 0; i < v.GetSize(); i++)
	{
		if(i<n) 
		{
			temp[v.GetSize()-n+i] = v[i];
			//TRACE("temp[%d] = v[%d]\n", v.GetSize()-n+i, i);
		}
		else 
		{
			temp[i-n] = v[i];
			//TRACE("temp[%d] = v[%d]\n", i-n, i);
		}
	}
	v.Copy(temp);
}

//---------------------------------------------------------------------------
bool CPath::IsClosed()
//---------------------------------------------------------------------------
{ //return true if the path is a boundary or contour

	return((type == PATHTYPE_BOUNDARY) || (type == PATHTYPE_CONTOUR));
}

//---------------------------------------------------------------------------
double CPath::PolyArea2()
//---------------------------------------------------------------------------
{
	//calculate 2x the area of a (closed) polygonal path
	//generally, if polygon vertices increase CCW, area is positive

	//check for a closed path
	if(!IsClosed()) 
		return 0.0f;
	
	double areaX2 = 0.0;		//holds twice the area
	double z_coord = v[0].z;	//make sure all vertices are coplanar

	for(int i = 0; i < v.GetSize(); i++)
	{
		//TRACE("PolyArea2: (%3.3f, %3.3f, %3.3f)\n", v[i].x,v[i].y, v[i].z);
		if(abs(v[i].z - z_coord) > CVEC_TOL)
		{
			AfxMessageBox("PolyArea: Path not coplanar with XY plane!", MB_OK);
			return 0.0;
		}
		if(i<v.GetSize()-1)
			areaX2 += (v[i].x + v[i+1].x)*(v[i+1].y - v[i].y);
		else
			areaX2 += (v[i].x + v[0].x)*(v[i].y - v[0].y);
	}
	return areaX2;
}

//---------------------------------------------------------------------------
bool CPath::PointInside( CVec& testPt )
//---------------------------------------------------------------------------
{
	//if this path is closed, test whether the testPt is inside
	//uses the ray projection, crossing counting method (Jordan Curve Theorem)
	//assumes that the path is planar and in the XY plane
	//ray is projected in the +X direction
	//Code adapted from "pnpoly" by William Randolph Franklin of RPI.edu
	//note that if the point is on an edge, it may be classified as either 
	//inside or outside

	//First make sure this path is closed
	ASSERT( IsClosed() );
	//Then check that the ray (+X direction) intersects the bounding box
	CVec pmin, pmax;
	CalcBBox(pmin, pmax);
	if( (testPt.x < pmin.x) ||
			(testPt.x > pmax.x ) ||
			(testPt.y > pmax.y) ||
			(testPt.y < pmin.y) )
		return false;

	//If so, then do full examination of crossings
  int i, j;
	int edges = v.GetSize()-1;
	bool inside = false;

	//note that the comparisons are crucial
  for (i = 0, j = edges; i < edges+1; j = i++)
	{
    if ((((v[i].y<=testPt.y) && (testPt.y<v[j].y)) ||
         ((v[j].y<=testPt.y) && (testPt.y<v[i].y))) &&
        (testPt.x < (v[j].x - v[i].x) * (testPt.y - v[i].y) / (v[j].y - v[i].y) + v[i].x))

      inside = !inside; //toggle at each crossing
  }
	return inside;
}


//---------------------------------------------------------------------------
bool CPath::PathInside( CPath& testPath, CArray <int,int>* pCrossIdx )
//---------------------------------------------------------------------------
{
	//if this path is closed, test whether the testPath is inside
	//uses the above PointInside code on each vertex of the path
	//if pCrossIdx != NULL, then check for crossing paths
	//if the paths cross, *pCrossIdx will contain the indices of the
	//vertices prior to the crossings (crossing occurs between *pCrossIdx[i] and *pCrossIdx[i+1])
	//if no crossing occurs, *pCrossIdx[0] will be -1.

	//First make sure this path is closed
	ASSERT( this->IsClosed() );
	//now check whether the bounding box of the path is inside
	//if so, then the path is (and cannot cross this path)
	CVec pmin, pmax;
	testPath.CalcBBox(pmin, pmax);
	if(this->PointInside(pmin) && this->PointInside(pmax))
		return true;

	//otherwise, we have to check each point until one fails
	//if some are in and some are out, then the paths are crossing
	int vertices = testPath.v.GetSize();
	int crossings = 0;
	bool pathInside = PointInside(testPath.v[0]);
	bool ptInside = pathInside;
	if(pCrossIdx != NULL) //check for crossings 
		(*pCrossIdx).Add(-1);  //initialize
	else if (!ptInside) //if the first point is not inside, and not checking for crossing, then done
		return false;

	for(int i = 1; i < vertices; i++)
	{
		if(ptInside != this->PointInside(testPath.v[i]))
		{
			if(pCrossIdx != NULL)
			{
				(*pCrossIdx).InsertAt(crossings,i-1); //point before the crossing
				crossings++;
				ptInside = !ptInside;
			}
			else //break out of the for loop since we are done
				i = vertices;
		}
	}
	return pathInside;
}

//---------------------------------------------------------------------------
void CPath::Offset(CArray<CPath,CPath>& paths, double d, CVec z, CArray<CPath,CPath>& output)
//---------------------------------------------------------------------------
{

	//offset each path in paths[]
	CArray<CPath,CPath> selfchains, otherchains;
	for (int i=0; i<paths.GetSize(); i++) {
		//TRACE("Paths[%d].v.GetSize():%d\n",i,paths[i].v.GetSize());
		paths[i].Offset(d,z, selfchains);
		//TRACE("Created %d Self-chains at z:%f\n", selfchains.GetSize(),paths[i].v[0].z);
	}
	
	CArray<INTERSECTION,INTERSECTION> va, vb;
	CPath::SetPathTypes(selfchains,PATHTYPE_CONTOUR);
	//for the first new chain created, check for intersection with all of the others
	//put all valid loops in output

	if(true)//selfchains.GetSize() == 1)
	{
		output.Copy(selfchains);
		return;
	}

	for(int i = 0; i < 1; i++)
	{
		//TRACE("Selfchains.GetSize():%d, i:%d\n", selfchains.GetSize(), i);
		bool bPruned = false;
		for(int j = i+1; j < selfchains.GetSize(); j++)
		{
			if(selfchains[i].FindIntersections(selfchains[j],va,vb)>0)
			{
				selfchains[i].PruneOtherIntersections(selfchains[j],va,vb,otherchains);
				bPruned = true;
			}
		}
		if(!bPruned)
			otherchains.Add(selfchains[i]);
	}
	output.Copy(otherchains);
}

//---------------------------------------------------------------------------
void CPath::Clean(CArray<CPath,CPath>& paths, double tol)
//---------------------------------------------------------------------------
{
	for (int i=0; i<paths.GetSize(); i++)
	{
		paths[i].Clean(tol);

		// eliminate pathological paths

		if (paths[i].v.GetSize() < 2)
		{
			paths.RemoveAt(i);
			i--;
		}
	}
}

//---------------------------------------------------------------------------
void CPath::Consolidate(CArray<CPath,CPath>& paths, double tol)
//---------------------------------------------------------------------------
{ // join segments where endpoints are closer than tol

	bool inconsistent = false;

	double tol2 = tol*tol;

	for (int i=paths.GetSize()-1; i>=0; i--) {
		// find closest join
		double mindist = tol;
		int minidx = -1;
		int idxi, idxj;
		double len;
		for (int j=0; j<i; j++) {
			len = (paths[j].v[0]-paths[i].v[0]).Length();
			if (len < mindist) {
				mindist = len;
				minidx = j;
				idxj = 0;
				idxi = 0;
			}
			len = (paths[j].v[0]-paths[i].v[paths[i].v.GetSize()-1]).Length();
			if (len < mindist) {
				mindist = len;
				minidx = j;
				idxj = 0;
				idxi = 1;
			}
			len = (paths[j].v[paths[j].v.GetSize()-1]-paths[i].v[0]).Length();
			if (len < mindist) {
				mindist = len;
				minidx = j;
				idxj = 1;
				idxi = 0;
			}
			len = (paths[j].v[paths[j].v.GetSize()-1]-paths[i].v[paths[i].v.GetSize()-1]).Length();
			if (len < mindist) {
				mindist = len;
				minidx = j;
				idxj = 1;
				idxi = 1;
			}
		}
		if (minidx > -1) {
			ASSERT(mindist < tol);
			if (idxj == 0 && idxi == 0) {
				paths[i].Reverse();
				if (paths[i].type == PATHTYPE_CONTOUR) {
					//TRACE("Detected normal inconsistency.\n"); // should not happen often for contours if STL normals were set correctly
					inconsistent = true;
				}
				paths[i].v.Append(paths[minidx].v);
				paths[minidx].v.Copy(paths[i].v);
				paths.RemoveAt(i);
			}
			if (idxj == 0 && idxi == 1) {
				paths[i].v.Append(paths[minidx].v);
				paths[minidx].v.Copy(paths[i].v);
				paths.RemoveAt(i);
			}
			if (idxj == 1 && idxi == 0) {
				paths[minidx].v.Append(paths[i].v);
				paths.RemoveAt(i);
			}
			if (idxj == 1 && idxi == 1) {
				paths[i].Reverse(); 
				if (paths[i].type == PATHTYPE_CONTOUR) {
					//TRACE("Detected normal inconsistency.\n"); // should not happen often for contours if STL normals were set correctly
					inconsistent = true;
				}
				paths[minidx].v.Append(paths[i].v);
				paths.RemoveAt(i);
			}
		}
	}
	/*if (inconsistent) {
		AfxMessageBox("Model normals are inconsistent.");
	}*/
}

//---------------------------------------------------------------------------
int CPath::FindIntersections(CPath &s, CArray<INTERSECTION,INTERSECTION>& va, 
							 CArray<INTERSECTION,INTERSECTION>& vb, double tol)
//---------------------------------------------------------------------------
{
	//find intersections between *this and s, return intersection data in va, vb resp.
	//for each segment in *this, check for intersection with all segments of s

	bool self = false;
	int n_i, n_j, i, imin, imax, j, jmin, jmax, cnt = 0;
	CVec AB, CD;
	INTERSECTION xA, xB;
	n_i = v.GetSize();
	n_j = s.v.GetSize();
	va.RemoveAll();
	vb.RemoveAll();
	//TRACE("Checking for intersections using ");
	if(s == *this) //self-intersection - different method
	{
		//TRACE("Self\n");
		self = true;
		imin = 0;
		imax = n_i-3;
	}
	else
	{
		//TRACE("Other\n");
		self = false;
		imin = 0;
		imax = n_i-1;
	}
	for(i = imin; i < imax; i++)
	{
		if(self)
		{
			jmin = i+2;
			if(i==0)
				jmax = n_i-2;
			else
				jmax = n_i;
		}
		else
		{
			jmin = 0;
			jmax = n_j;
		}
		for(j = jmin; j < jmax; j++)
		{
			if(v[i%n_i].SegmentIntersection(v[(i+1)%n_i], s.v[j%n_j], s.v[(j+1)%n_j], AB, CD, tol))
			{
				//if segments intersect, record the intersection data for each path
				//the actual point can differ if paths are slightly skew
				xA.index = i;
				xA.pos = AB;
				xB.index = j;
				xB.pos = CD;
				va.Add(xA);
				vb.Add(xB);
				cnt++; //count the number of intersections
				//TRACE("(v[%d],v[%d]) X (s.v[%d],s.v[%d]) Intersected\n",i,(i+1),j,(j+1));
			}
		}
	}
	//TRACE("Found %d intersections\n",cnt);
	return cnt;
}

//---------------------------------------------------------------------------
void CPath::RemoveSharpCorners(double maxAngle, double minArea, double cleanTol)
//---------------------------------------------------------------------------
{
	//traverse path looking for triplets of vertices in which
	//the external angle (deviation) v[i-1],v[i],v[i+1] is larger than maxAngle (in rads)
	//and the area of the triangle v[i-1],v[i],v[i+1] is less than minArea
	//remove these by connecting the vertex at the base of the shorter of the two legs
	//to the longer leg

	if(v.GetSize()<3) return;
	minArea *= 2; //we will compare to 2X area of triangles
	CVec Zhat = CVec(0,0,1);
	//Log(CString("D:\\My Documents\\Fab@Home\\Fab@Home Application\\SourceForge\\RSC_PreClean.txt"));
	CPath tmp = *this;
	bool removed;
	do{
		removed = false;
		for(int i = 0; i < v.GetSize(); i++)
		{
			CVec leg1, leg2;
			int prev,nxt;
			if(IsClosed())
			{
				//wrap around
				prev = (i-1+v.GetSize())%v.GetSize();
				nxt = (i+1)%v.GetSize();
			}
			else //open path
			{
				//don't wrap around
				if(i==0)
				{
					i++;
					prev = 0;
					nxt = i+1;
				}
				if(i == v.GetSize()-1) continue;
			}
			leg1 = v[i]-v[prev];
			leg2 = v[nxt]-v[i];

			CVec dir = leg1.Cross(leg2);
			bool posAng = dir.Dot(Zhat) > 0.0;
			double area2 = dir.Length();
			//TRACE("area: %f, ", area);
			if( area2 < minArea )
			{
				ASSERT(leg1.Length() > 0.0 && leg2.Length() > 0.0);
				double shift = leg1.Dot(leg2);
				if(!posAng)
					shift = leg2.Dot(leg1);
				double ratio = shift/(leg1.Length()*leg2.Length());
				if(ratio > 1.0) ratio = 1.0;
				if(ratio < -1.0) ratio = -1.0;
				double angle = acos(ratio);
				
				if(angle > maxAngle)
				{
					//TRACE("shift:%f, ratio:%f, angle:%f > %f\n", shift, ratio, angle, maxAngle);
					if(leg1.Length2() > leg2.Length2())
						dir = leg1.Normalized();
					else
						dir = -leg2.Normalized();

					//TRACE("moving v[%d] = (%f,%f,%f) ",i,tmp.v[i].x,tmp.v[i].y,tmp.v[i].z);
					tmp.v[i] += shift*dir;
					//TRACE("->(%f,%f,%f)\n",tmp.v[i].x,tmp.v[i].y,tmp.v[i].z);
					removed = true;
				}
			}
			//TRACE("\n");
		}
		tmp.Clean(cleanTol);
		(*this) = tmp;
	} while(removed);
	
	//Log(CString("D:\\My Documents\\Fab@Home\\Fab@Home Application\\SourceForge\\RSC_Done.txt"));
}

//---------------------------------------------------------------------------
void CPath::Log(CString& fileName, CString& header)
//---------------------------------------------------------------------------
{
	//log the points of the path

	//prep the data file
	CStdioFile file;
	if (file.Open(fileName, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate) == 0)
		return;
	//move to the end of the file
	file.SeekToEnd();
	//write a file header
	if(!header.IsEmpty())
		file.WriteString(header);
	CString dataString;
	for(int i = 0; i<v.GetSize(); i++)
	{
		dataString.Format("%d\t%0.3f\t%0.3f\t%0.3f\n",i,v[i].x,v[i].y,v[i].z);
		file.WriteString(dataString);
	}
	file.Close();
}