// Region.h: interface for the CRegion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGION_H__9C9F4FA3_12AC_4104_9C24_88B957A21464__INCLUDED_)
#define AFX_REGION_H__9C9F4FA3_12AC_4104_9C24_88B957A21464__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPath;

class CRegion : public CObject  
{
	DECLARE_SERIAL(CRegion);
public:
	CRegion();
	CRegion( const CRegion& s );
	virtual ~CRegion();
	void Serialize(CArchive& ar);
	CRegion& operator=(const CRegion& s);
	bool operator==(const CRegion& s);
	void CalcBBox(CVec xax, CVec yax, CVec zax, CVec &pmin, CVec &pmax);
	void MinimizeTravel(); //sort the paths to minimize travel from end of one to start of next
	double CalcRegionArea(); //return the theoretical cross section area of the region
	void SetPathTypes(PATHTYPE t);
	void Draw(CVec color, bool bPaths, bool bTrace);
	void RasterizeSimple(CArray<CPath,CPath>& cntr, double gap, double width, CVec dir, CArray<CPath,CPath>& output);
	void RasterizeOffset(double width, CVec dir);
	CArray<CPath*,CPath*> m_arBoundaryPtr; //array of pointers to boundary paths (stored in boundary tree)
	int treeIdx; //index of the boundary tree in which this region is contained
	CArray<int,int> boundaryIdxs;  //indices of the boundaries for this region; index zero is the outside boundary, others are internal
	int depth;  //the depth of nesting of the outside boundary of this region
	CArray<CPath,CPath> path; //array of toolpaths to make this region
	CArray<CPath,CPath> trace; //track the paths followed by the system
	int m_nCurrentPath; //index of the current path
	CMutex m_mutAccess; //prevent multi-thread clashes
	void FlushTrace(); //flush out any stored trace information
	void FlushPaths(); //flush out planned paths
};

template <> void AFXAPI SerializeElements <CRegion> ( CArchive& ar, CRegion* pNewObjects, int nCount );

#endif // !defined(AFX_REGION_H__9C9F4FA3_12AC_4104_9C24_88B957A21464__INCLUDED_)
