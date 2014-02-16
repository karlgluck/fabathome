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

// Layer.cpp: implementation of the CLayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <hash_map>
#include <vector>
#include <list>
#include <deque>
#include <iterator>
#include <string>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// --------------------------------------------------------------------------
CLayer::CLayer()
// --------------------------------------------------------------------------
{
	chunkid = 0;
	m_dPathTravel = 0;
	m_dJogTravel = 0;
	m_dArea = 0;
	m_ctsTimeEst = 0;
	m_dVmat = 0;
	m_dVtheor = 0;
	toolname = "Undefined";
	matname = "Undefined";
	m_nCurRegion = 0;
}

// --------------------------------------------------------------------------
CLayer::~CLayer()
// --------------------------------------------------------------------------
{
	Flush();
}

// --------------------------------------------------------------------------
void CLayer::Flush()
// --------------------------------------------------------------------------
{
	//delete everything and reinit members
	chunkid = 0;
	m_dPathTravel = 0;
	m_dJogTravel = 0;
	m_dArea = 0;
	m_ctsTimeEst = 0;
	m_dVmat = 0;
	m_dVtheor = 0;
	toolname = "Undefined";
	matname = "Undefined";
	m_nCurRegion = 0;
	region.RemoveAll();
}

//---------------------------------------------------------------------------
void CLayer::Serialize(CArchive& ar)
//---------------------------------------------------------------------------
{// Save / Load

	int version = 10;

	if (ar.IsStoring()) { //add storing code here

		ar << version;

		// save data

		ar << chunkid;
		region.Serialize(ar);
		boundaryForest.Serialize(ar);

		ar << version;

	}	else	{  // add loading code here

		int version1, version2;
		ar >> version1; if (version1 < 10 || version1 > version) throw 6;

		// load data

		ar >> chunkid;
		region.Serialize(ar);
		boundaryForest.Serialize(ar);
		ar >> version2; if (version2 != version1) throw 6;

	}
}

//---------------------------------------------------------------------------
CLayer::CLayer(CLayer& s)
//---------------------------------------------------------------------------
{
	chunkid = s.chunkid;
	m_dPathTravel = s.m_dPathTravel;
	m_dJogTravel = s.m_dJogTravel;
	m_dArea = s.m_dArea;
	m_dVmat = s.m_dVmat;
	m_dVtheor = s.m_dVtheor;
	m_ctsTimeEst = s.m_ctsTimeEst;
	toolname = s.toolname;
	matname = s.matname;
	region.Copy(s.region);
	boundaryForest.Copy(s.boundaryForest);
	m_coordSys = s.m_coordSys;
	m_nCurRegion = s.m_nCurRegion;
}

//---------------------------------------------------------------------------
CLayer& CLayer::operator=(const CLayer& s)
//---------------------------------------------------------------------------
{

	chunkid = s.chunkid;
	m_dPathTravel = s.m_dPathTravel;
	m_dJogTravel = s.m_dJogTravel;
	m_dArea = s.m_dArea;
	m_dVmat = s.m_dVmat;
	m_dVtheor = s.m_dVtheor;
	m_ctsTimeEst = s.m_ctsTimeEst;
	toolname = s.toolname;
	matname = s.matname;
	region.Copy(s.region);
	boundaryForest.Copy(s.boundaryForest);
	m_coordSys = s.m_coordSys;
	m_nCurRegion = s.m_nCurRegion;
	return *this;
}



/*//---------------------------------------------------------------------------
void CLayer::Draw(bool bPathNormals, CModel *pModel)
//---------------------------------------------------------------------------
{

	int chunkidx = pModel->FindChunk(chunkid);
	ASSERT(chunkidx != -1);

	glPushName(pModel->chunk[chunkidx].id);
	CFabAtHomeView::ReportGLError(CString("CLayer::Draw:PushName"));
	for(int i = 0; i < contour.GetSize(); i++)
	{
		contour[i].Draw(bPathNormals, pModel->chunk[chunkidx].color);
	}
	for (int i=0; i<path.GetSize(); i++)
	{
		path[i].Draw(bPathNormals, pModel->chunk[chunkidx].color);
	}
	for(int i =0; i< boundary.GetSize(); i++)
	{
		boundary[i].Draw(false, CVec(0,0,1));
	}
	glPopName();
	CFabAtHomeView::ReportGLError(CString("CLayer::Draw:PopName"));

}*/

//---------------------------------------------------------------------------
int CLayer::GetNumPaths()
//---------------------------------------------------------------------------
{
	int nPaths = 0;
	for(int i = 0; i < region.GetSize(); i++)
	{
		nPaths+=region[i].path.GetSize();
	}
	return nPaths;
}

//---------------------------------------------------------------------------
void CLayer::Draw(CModel *pModel, bool bPaths, bool bTrace)
//---------------------------------------------------------------------------
{
	int chunkidx = pModel->FindChunk(chunkid);
	ASSERT(chunkidx != -1);

	glPushName(pModel->chunk[chunkidx].id);
	CFabAtHomeView::ReportGLError(CString("CLayer::Draw:PushName"));
	for (int i=0; i<region.GetSize(); i++)
	{
		CSingleLock rgnLock(&region[i].m_mutAccess);
		rgnLock.Lock();
		region[i].Draw(pModel->chunk[chunkidx].color, bPaths, bTrace);
	}
	if(bPaths)
	{
		for(int i = 0; i<boundaryForest.GetSize(); i++)
		{
			CSingleLock forestLock(&boundaryForest[i].m_mutAccess);
			forestLock.Lock();
			boundaryForest[i].Draw(pModel->chunk[chunkidx].color);
		}
	}
	glPopName();
}

/*//---------------------------------------------------------------------------
double CLayer::MinimizeTravel(int startPath)
//---------------------------------------------------------------------------
{
	//sort paths in layer to minimize travel
	//start at last vertex of path[startPath]
	//this is a Traveling Salesman Problem.
	//Using "closest vertex" heuristic.
	//starting indices are even numbered, ending are odd

	//create an array to store the distance information between path endpoints
	using namespace std;
	using namespace stdext;

	int nPath = path.GetSize();
	//linked lists to hold visited and unvisited nodes
	list<pair<int,int>> closedList;
	list<pair<int,int>>::iterator closedPos;
	list<pair<int,int>>::const_iterator closedLenPos;
	list<pair<int,int>> openList;
	list<pair<int,int>>::iterator openPos;
	list<pair<int,int>>::iterator openBestPos;
	//define a hash map to hold the distance between pairs of vertices
	//but first, hash map needs binary predicate
	struct my_hasher : public hash_compare <string>
	{
		size_t operator()(const pair<int,int>& pr) const
		{
			string s;
			char tmp[255];
			_itoa_s(pr.first,tmp,10);
			s += tmp;
			_itoa_s(pr.second,tmp,10);
			s += tmp;
			size_t h = 0;
			std::string::const_iterator p, p_end;
			for(p = s.begin(), p_end = s.end(); p != p_end; ++p)
			{
			  h = 31 * h + (*p);
			}
			return h;
		}

		//From SGI docs on STL, I expected this to need to return true when
		//p1==p2, if that is the case distinct entries are only generated when 
		//the function returns true, but on further reading, it seems that it needs
		//to enforce a weak ordering
		bool operator()(const pair<int,int>& p1, const pair<int,int>& p2) const
		{
			return((p1.first < p2.first) || (p1.second < p2.second));
		}
	};
	
	typedef hash_map<pair<int,int>, double, my_hasher> MY_HMAP;
	MY_HMAP distances;
	MY_HMAP::iterator it;


	//calculate all of the distances (actually distance squared)
	//and load in the hash map; don't include (n,n+1) where n even
    for(int i = 0; i < nPath-1; i++)
	{
		for(int j = i+1; j < nPath; j++)
		{
			//convert pairs (and their mirror images) and enter distances into hash_map
			double dist = path[i].v[0].Dist2(path[j].v[0]);
			distances[pair<int,int>(2*i,2*j)] = dist;
			distances[pair<int,int>(2*j,2*i)] = dist;
			dist = path[i].v[0].Dist2(path[j].v[path[j].v.GetSize()-1]);
			distances[pair<int,int>(2*i,2*j+1)] = dist;
			distances[pair<int,int>(2*j+1,2*i)] = dist;
			dist = path[i].v[path[i].v.GetSize()-1].Dist2(path[j].v[0]);
			distances[pair<int,int>(2*i+1,2*j)] = dist;
			distances[pair<int,int>(2*j,2*i+1)] = dist;
			dist = path[i].v[path[i].v.GetSize()-1].Dist2(path[j].v[path[j].v.GetSize()-1]);
			distances[pair<int,int>(2*i+1,2*j+1)] = dist;
			distances[pair<int,int>(2*j+1,2*i+1)] = dist;
		}
	}

	//init the closed list with the starting pair
	closedList.push_back(pair<int,int>(2*startPath,2*startPath+1));

	//and the two other pairs which are maximally distant from the starting pair
	//using the sum distance of the pair from the endpoint of the starting path
	//we might use the convex hull except that we are constrained in our starting point
	//which might not be on the hull, and the fact that we have pairs joined by paths
	//which cannot be broken, but can be reversed
	double maxDist1 = 0.0;
	double minDist1;
	MY_HMAP::iterator max1,max1ptnr;
	int maxPth1;
	int n = 2*startPath+1;
	for(int i = 0; i < 2*nPath; i++)
	{
		if(i/2 != startPath)
		{
			double tmpDist;
			max1 = distances.find(pair<int,int>(2*startPath+1,i));
			max1ptnr = distances.find(pair<int,int>(2*startPath+1,(i%2==0 ? i+1 : i-1)));
			tmpDist = max1->second + max1ptnr->second;
			if(maxDist1 < tmpDist)
			{
				maxDist1 = tmpDist;
				maxPth1 = i/2; //path index rather than endpoint index
				TRACE("maxDist1:%f\tmax1pth:%d\n",maxDist1,maxPth1);
			}
		}
	}
	closedList.push_back(pair<int,int>(2*maxPth1,2*maxPth1+1));

	for(int i = 0; i < nPath; i++)
	{
		//init the open list of unvisited nodes
		//remember that the first and last vertices of the starting path
		//(2*startPath and 2*startPath+1) are already visited
		if(i != startPath)
		{
			openList.push_back(pair<int,int>(2*i, 2*i+1));
		}
	}
	//http://www-e.uni-magdeburg.de/mertens/TSP/node2.html
	//try greedy insertion
	
	//while(!openList.empty())
	//{
	//	double minDist = 0.0;
	//	//for each pair of vertices in the openList
	//	for(openPos = openList.begin(); openPos != openList.end(); ++openPos)
	//	{
	//		//for each pair of vertices in the closed list after the first (the mandated starting path)
	//		for(closedPos = ++closedList.begin(); closedPos != closedList.end(); ++closedPos)
	//		{
	//			double distFwd = 0.0;
	//			double distRev = 0.0;
	//			//try inserting the openList vertex pair in forward order
	//			closedList.insert(closedPos,*openPos);
	//			//and measure the total path length between proximate members adjacent pairs (2nd to 1st)
	//			for(closedLenPos = ++(closedList.begin()); closedLenPos != closedList.end(); ++closedLenPos)
	//			{
	//				list<pair<int,int>>::const_iterator tmpPos = --closedLenPos;
	//				distFwd += distances[pair<int,int>(tmpPos->second,(++tmpPos)->first)];
	//			}
	//			//now reverse the inserted vertex pair order (reverse)
	//			int tmp = closedPos->first;
	//			closedPos->first = closedPos->second;
	//			closedPos->second = tmp;
	//			//and measure the total path length over the closed list
	//			for(closedLenPos = ++(closedList.begin()); closedLenPos != closedList.end(); ++closedLenPos)
	//			{
	//				list<pair<int,int>>::const_iterator tmpPos = --closedLenPos;
	//				distRev += distances[pair<int,int>(tmpPos->second,(++tmpPos)->first)];
	//			}
	//			//now check if either of these closed lists improves on the 
	//		}
	//	}
	//}
				
    return 0;
}*/

/*//---------------------------------------------------------------------------
int CLayer::RandStartCntr()
//---------------------------------------------------------------------------
{
	//for each contour path, randomly rotate the starting point
	//return the number of paths altered
	int numchanged = 0;
	for(int i = 0; i < path.GetSize(); i++)
	{
		if(path[i].type == PATHTYPE_CONTOUR)
		{
			int n = rand()*(path[i].v.GetSize()-1)/RAND_MAX;
			path[i].CircShift(n);
			numchanged++;
		}
	}
	return numchanged;
}*/

//---------------------------------------------------------------------------
void CLayer::CalcBBox(CVec xax, CVec yax, CVec zax, CVec &pmin, CVec &pmax)
//---------------------------------------------------------------------------
{
	//Bounding box for a given layer is just the box which contains all of the root boundaries
	//in the boundary forest for the layer
	if (region.GetSize() == 0)
		return;
	pmax = CVec(-1E10, -1E10, -1E10);
	pmin = CVec(1E10, 1E10, 1E10);

	for (int i=0; i<boundaryForest.GetSize(); i++) {
		for (int j=0; j<boundaryForest[i].Root().GetBoundary().v.GetSize(); j++) {
			double x = boundaryForest[i].Root().GetBoundary().v[j].Dot(xax);
			double y = boundaryForest[i].Root().GetBoundary().v[j].Dot(yax);
			double z = boundaryForest[i].Root().GetBoundary().v[j].Dot(zax);
			pmin.x = min(pmin.x, x);
			pmin.y = min(pmin.y, y);
			pmin.z = min(pmin.z, z);
			pmax.x = max(pmax.x, x);
			pmax.y = max(pmax.y, y);
			pmax.z = max(pmax.z, z);
		}
	}
}

//---------------------------------------------------------------------------
bool CLayer::CalcLayerStats(CHUNKTYPE type)
//---------------------------------------------------------------------------
{
	//estimate several statistics for this layer including:
	//the total travel (in mm) along paths during deposition to complete this layer
	//the total jog distance (the lateral (non-z) distance from end of one path to start of next, for all paths)
	//the theoretical cross-section area of this layer
	//the theoretical volume of this layer
	//the estimated material volume of this layer

	if(0==toolname.CompareNoCase("Undefined")) return false;
	CTool *pLyrTool = CTool::GetByName(toolname);
	int i,j;
	m_dPathTravel = 0;
	m_dJogTravel = 0;
	CVec last, next;

	for(i = 0; i < region.GetSize(); i++)
	{
		for(j = 0; j < region[i].path.GetSize(); j++)
		{
			m_dPathTravel += region[i].path[j].GetLength();
			if(j < region[i].path.GetSize()-1)
			{
				last = region[i].path[j].v[region[i].path[j].v.GetSize()-1];
				next = region[i].path[j+1].v[0];
				m_dJogTravel += next.Dist(last);
			}
		}
		if(i < region.GetSize()-1)
		{
			last = region[i].path[region[i].path.GetSize()-1].v[region[i].path[j].v.GetSize()-1];
			next = region[i+1].path[0].v[0];
			m_dJogTravel += next.Dist(last);
		}
		m_dJogTravel += 2*region[i].path.GetSize(); //account for up/down table motion between paths
	}
	
	m_dJogTravel += 2*region.GetSize(); //account for up/down table motion between regions
	CFabAtHomeApp* pApp = (CFabAtHomeApp*)AfxGetApp();
	m_ctsTimeEst = (__time64_t)(m_dPathTravel/pLyrTool->GetPathspeed() + m_dJogTravel/pApp->printer.GetJogSpeed());

	CalcLayerArea(type);
	m_dVtheor = m_dArea*pLyrTool->pathheight;
	m_dVroads = m_dPathTravel*pLyrTool->GetPathXSection();
	m_dVmat = m_dPathTravel*pLyrTool->GetVolumeFlowrate();

	return true;
}

//---------------------------------------------------------------------------
void CLayer::CalcLayerArea(CHUNKTYPE type)
//---------------------------------------------------------------------------
{
	//calculate the theoretical cross-section area of this layer
	//using the area of the layer boundary
	//store result in m_dArea
	m_dArea = 0;
	switch(type)
	{
	case CHUNKTYPE_STL:
		for(int i = 0; i < region.GetSize(); i++)
		{
			m_dArea += region[i].CalcRegionArea(); 
		}
		break;
	}
}

//---------------------------------------------------------------------------
void CLayer::MakeRegions()
//---------------------------------------------------------------------------
{
  //Using the boundary forest, create new regions.
	//Assumes that there are no existing regions
  //Each region is bounded on the outside by a boundary curve at even depth
  //and on the inside by the child boundary curves of the outside boundary

  //for each tree in the forest...
  for(int i=0; i < boundaryForest.GetSize(); i++)
  {
		//call the recursive function MakeRegionsAux(treeIdx, rgnIdx, depth, nodeIdx)
		MakeRegionsAux(i, 0, 0, boundaryForest[i].RootIdx());
	}
}


//---------------------------------------------------------------------------
int CLayer::MakeRegionsAux(int treeIdx, int rgnIdx, int depth, int nodeIdx)
//---------------------------------------------------------------------------
{
	//if depth is even
	if( depth%2 == 0 )
	{
		//make a new region
		CRegion tmpRegion;
		//set what boundary tree it is related to
		tmpRegion.treeIdx = treeIdx;
		//set its outer boundary depth
		tmpRegion.depth = depth;
		//set its outside boundary index to the current node index
		tmpRegion.boundaryIdxs.SetAtGrow(0, nodeIdx);
		//add a pointer to this boundary to the boundary ptr array
		tmpRegion.m_arBoundaryPtr.SetAtGrow(0,&(boundaryForest[treeIdx].Ref(nodeIdx).GetBoundary()));
		//add the new region to the layer's region array
		rgnIdx = region.Add(tmpRegion);
	}
	else
	{
		//depth is odd, so add the node index to the array of inner boundary indices for this region
		region[rgnIdx].boundaryIdxs.Add(nodeIdx);
		//and add a pointer to the boundary path to the boundary pointer array
		region[rgnIdx].m_arBoundaryPtr.Add(&(boundaryForest[treeIdx].Ref(nodeIdx).GetBoundary()));
	}

	//the termination condition - return if no children and no siblings
	if( !(boundaryForest[treeIdx].Ref(nodeIdx).HasChild()) && !(boundaryForest[treeIdx].Ref(nodeIdx).HasSibling()) )
	{
		return nodeIdx;
	}
	//if there is a left child
	//then recurse to it, incrementing to next depth
	if( boundaryForest[treeIdx].Ref(nodeIdx).HasChild() ) 
	{
		return MakeRegionsAux(treeIdx, rgnIdx, (depth+1), boundaryForest[treeIdx].Ref(nodeIdx).GetLChildIdx());
	}
	//if there is a right sibling
	//then recurse to it without incrementing depth
	if( boundaryForest[treeIdx].Ref(nodeIdx).HasSibling() )
	{
		return MakeRegionsAux(treeIdx, rgnIdx, depth, boundaryForest[treeIdx].Ref(nodeIdx).GetRSiblingIdx());
	}
	//otherwise error
	return -1;
}
