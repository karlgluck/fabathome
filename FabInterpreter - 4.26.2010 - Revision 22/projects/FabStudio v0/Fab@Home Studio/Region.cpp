// Region.cpp: implementation of the CRegion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Region.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CRegion, CObject, 1);

//---------------------------------------------------------------------------
CRegion::CRegion()
//---------------------------------------------------------------------------
{
	treeIdx = -1;
	depth = -1;
	m_nCurrentPath = -1;
}

//---------------------------------------------------------------------------
CRegion::CRegion( const CRegion& s )
//---------------------------------------------------------------------------
{
	treeIdx = s.treeIdx;
	boundaryIdxs.Copy(s.boundaryIdxs);
	depth = s.depth;
	path.Copy(s.path);
	trace.Copy(s.trace);
	m_arBoundaryPtr.Copy(s.m_arBoundaryPtr);
	//m_mutAccess = s.m_mutAccess;
}

//---------------------------------------------------------------------------
CRegion& CRegion::operator=(const CRegion& s)
//---------------------------------------------------------------------------
{
	treeIdx = s.treeIdx;
	boundaryIdxs.Copy(s.boundaryIdxs);
	depth = s.depth;
	path.Copy(s.path);
	trace.Copy(s.trace);
	m_arBoundaryPtr.Copy(s.m_arBoundaryPtr);
	//m_mutAccess = s.m_mutAccess;
	return *this;
}

//---------------------------------------------------------------------------
bool CRegion::operator==(const CRegion& s)
//---------------------------------------------------------------------------
{
	for(int i = 0; i < path.GetSize(); i++)
	{
		if(!(path[i] == s.path[i]) ) return false;
	}
	for(int i = 0; i < boundaryIdxs.GetSize(); i++)
	{
		if(!(boundaryIdxs[i] == s.boundaryIdxs[i]) ) return false;
	}
	return( (depth == s.depth) && (treeIdx == s.treeIdx) );
}

//---------------------------------------------------------------------------
CRegion::~CRegion()
//---------------------------------------------------------------------------
{
	path.RemoveAll();
	trace.RemoveAll();
}


//---------------------------------------------------------------------------
void CRegion::Draw(CVec color, bool bPaths, bool bTrace)
//---------------------------------------------------------------------------
{
	if(bPaths)
	{
		if( (m_nCurrentPath < 0) || (m_nCurrentPath >= path.GetSize()) )
		{
			for(int i = 0; i < path.GetSize(); i++)
			{
				CSingleLock pathLock(&path[i].m_mutAccess);
				pathLock.Lock();
				path[i].Draw(false, color);
				pathLock.Unlock();
			}
		}
		else
		{
			CSingleLock pathLock(&path[m_nCurrentPath].m_mutAccess);
			pathLock.Lock();
			path[m_nCurrentPath].Draw(false, color);
			pathLock.Unlock();
		}
	}
	if(bTrace)
	{
		if( (m_nCurrentPath < 0) || (m_nCurrentPath >= path.GetSize()) )
		{
			for(int i = 0; i < trace.GetSize(); i++)
			{
				CSingleLock pathLock(&trace[i].m_mutAccess);
				pathLock.Lock();
				trace[i].Draw(false, color);
				pathLock.Unlock();
			}
		}
		else
		{
			CSingleLock pathLock(&trace[m_nCurrentPath].m_mutAccess);
			pathLock.Lock();
			trace[m_nCurrentPath].Draw(false, color);
			pathLock.Unlock();
		}
	}
}

//---------------------------------------------------------------------------
void CRegion::CalcBBox(CVec xax, CVec yax, CVec zax, CVec &pmin, CVec &pmax)
//---------------------------------------------------------------------------
{
	//Bounding box for a given region is just the box which contains the outer contour
	//boundary for the region
	CPath& boundary = *(m_arBoundaryPtr[0]);
	boundary.CalcBBox(xax,yax,zax,pmin,pmax);
	//TRACE("RgnBBox:pmin(%f,%f,%f), pmax(%f,%f,%f)\n", pmin.x,pmin.y,pmin.z,pmax.x,pmax.y,pmax.z);
}

//---------------------------------------------------------------------------
void CRegion::Serialize(CArchive& ar)
//---------------------------------------------------------------------------
{
	// Save / Load

	int version = 1;

	if (ar.IsStoring()) { //add storing code here

		ar << version;

		// save data

		ar << treeIdx;
		boundaryIdxs.Serialize(ar);
		ar << depth;
		path.Serialize(ar);
		trace.Serialize(ar);
		ar << version;

	}	else	{  // add loading code here

		int version1, version2;
		ar >> version1; if (version1 < 1 || version1 > version) throw 6;

		// load data

		ar >> treeIdx;
		boundaryIdxs.Serialize(ar);
		ar >> depth;
		path.Serialize(ar);
		trace.Serialize(ar);
		ar >> version2; if (version2 != version1) throw 6;

	}
}

//---------------------------------------------------------------------------
void CRegion::MinimizeTravel()
//---------------------------------------------------------------------------
{
  //sort the paths, in order to minimize travel
  //look at first and last vertex in each path, and sort paths so that
  //end of one path is as close as possible to start of next.
  //paths are reversed if their final vertex is closer than their initial.
  CVec lastEnd, nextStart, nextEnd;
  for(int i = 0; i < path.GetSize()-1; i++)
  {
    lastEnd = path[i].v[path[i].v.GetUpperBound()];
    nextStart = path[i+1].v[0];
    nextEnd = path[i+1].v[path[i+1].v.GetUpperBound()];
    //if the distance squared from the end of the last region
    //to the end of the next is less than to the start of the next
    //then reverse the next region
    if( (nextEnd-lastEnd).Length2() < (nextStart-lastEnd).Length2() )
    {
      path[i+1].Reverse();
    }
  }
}


//---------------------------------------------------------------------------
void CRegion::RasterizeSimple(CArray<CPath,CPath>& cntr, double gap, double width, CVec dirx, CArray<CPath,CPath>& output)
//---------------------------------------------------------------------------
{ 
	// raster scan between contours using simple method and put scan lines in output

	if(cntr.GetSize() < 1) return;
	if(cntr[0].v.GetSize() < 1) return;
	int i;
	// find bounding box in direction
	CVec pmin, pmax;
	dirx.Normalize();
	CVec dirz(0,0,1);
	CVec diry = dirz.Cross(dirx);
	diry.Normalize();
	CalcBBox(dirx, diry, dirz, pmin, pmax);
	if ((pmax.y-pmin.y)/width > 10000) {
		cntr[0].CalcBBox(dirx, diry, dirz, pmin, pmax);
	}
	ASSERT((pmax.y-pmin.y)/width < 10000);

	// collect intersection points

	CArray<double,double> xs;
	CArray<int,int> cd;

	double z = cntr[0].v[0].z;

	for (double y=pmin.y; y<=pmax.y; y+= width) {
		xs.RemoveAll();
		cd.RemoveAll();
		for (i=0; i<cntr.GetSize(); i++) {
			for (int j=0; j<cntr[i].v.GetSize(); j++) {
				CVec* pv1;
				CVec* pv2;
				if(j<cntr[i].v.GetSize()-1)
				{
					pv1 = &(cntr[i].v[j]);
					pv2 = &(cntr[i].v[j+1]);
				}
				else
				{
					pv1 = &(cntr[i].v[j]);
					pv2 = &(cntr[i].v[0]);
				}
				double y1 = pv1->Dot(diry);
				double y2 = pv2->Dot(diry);
				if (y1 >= y && y2 >= y)
					continue;
				if (y1 <= y && y2 <= y)
					continue;
				double x1 = pv1->Dot(dirx);
				double x2 = pv2->Dot(dirx);
				double x = x1 + (x2-x1)*(y-y1)/(y2-y1);
				xs.Add(x);
				if (y1 > y2) {
					cd.Add(1);
				} else {
					cd.Add(-1);
				}
			}
		}

		// sort points
		bool changed = true; // buble sort is bad! TODO: quicksort
		do {
			changed = false;
			for (i=0; i<xs.GetSize()-1; i++) {
				if (xs[i] > xs[i+1]) {
					double c = xs[i];
					xs[i] = xs[i+1];
					xs[i+1] = c;
					int cc = cd[i];
					cd[i] = cd[i+1];
					cd[i+1] = cc;
					changed = true;
				}
			}
		} while (changed);

//			for (i=0; i<cd.GetSize(); i++) {
//				TRACE("%f:%d ", xs[i],cd[i]);
//			}
//				TRACE("\n");

		// generate paths between adjacent points
		CPath newpath;
		newpath.type = PATHTYPE_RASTER;
		for (i=0; i<xs.GetSize()-1; i+=2) {
			if (cd[i] == 1 && cd[i+1] == -1) {
				if (xs[i+1]-xs[i] > gap*2) {
					newpath.v.RemoveAll();
					CVec p = dirx*(xs[i]+gap) + diry*y +dirz*z;
					newpath.v.Add(p);
					p = dirx*(xs[i+1]-gap) + diry*y +dirz*z;
					newpath.v.Add(p);
					output.Add(newpath);
				}
			} else if (cd[i] == -1 && cd[i+1] == 1) {
				// Skip this pair: Offset contour flipped
			} else {
				// Skip just first point: Bad sequence
				i--;
			}
		}
		CPath::Consolidate(output,width*CONSOLIDATION_FACTOR);
	}
}

//---------------------------------------------------------------------------
void CRegion::RasterizeOffset(double width, CVec dirx)
//---------------------------------------------------------------------------
{ // raster scan contour using offset method
	
	CArray<CPath,CPath> offsetcontour;
	CArray<CPath,CPath> output;
	for(int i = 0; i < m_arBoundaryPtr.GetSize(); i++)
	{
		offsetcontour.Add(*(m_arBoundaryPtr[i]));
	}
	//first offset the boundaries by 1/2 pathwidth
	//to create the contour paths
	CPath::Clean(offsetcontour,width/10);
	CPath::Offset(offsetcontour, width/2.0, CVec(0,0,1), offsetcontour);
	//then offset them by pathwidth for use
	//in generating raster paths
	CPath::Offset(offsetcontour, width, CVec(0,0,1), output);
	RasterizeSimple(output, 0, width, dirx, path);
	//path.Append(output);
	path.InsertAt(0,&offsetcontour);
}

//---------------------------------------------------------------------------
double CRegion::CalcRegionArea()
//---------------------------------------------------------------------------
{
	//return the theoretical cross section area of the region
	//the first boundary is outer, the rest are inner
	//so subtract the area of the inner from that of the outer
	double area = m_arBoundaryPtr[0]->PolyArea2();
	for(int i = 1; i < m_arBoundaryPtr.GetSize(); i++)
	{
		area -= m_arBoundaryPtr[i]->PolyArea2();
	}
	return area/2.0;
}

//---------------------------------------------------------------------------
void CRegion::SetPathTypes(PATHTYPE t)
//---------------------------------------------------------------------------
{ 

	for (int i=0; i<path.GetSize(); i++) {
		path[i].type = t;
	}
}



//---------------------------------------------------------------------------
template <> void AFXAPI SerializeElements <CRegion> ( CArchive& ar, CRegion* pNewObjects, int nCount )
//---------------------------------------------------------------------------
{// serialize an array
    
	for ( int i = 0; i < nCount; i++, pNewObjects++ )    {
        // Serialize each object        
		pNewObjects->Serialize(ar);
    }
}

//---------------------------------------------------------------------------
void CRegion::FlushTrace()
//---------------------------------------------------------------------------
{
	//flush out any stored trace information
	trace.RemoveAll();
}

//---------------------------------------------------------------------------
void CRegion::FlushPaths()
//---------------------------------------------------------------------------
{
	//flush out planned paths
	path.RemoveAll();
}