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

// Chunk.cpp: implementation of the CChunk class.
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
CChunk::CChunk()
// --------------------------------------------------------------------------
{

	id = 0;
	selected = false;
	m_nPriority = 0;
	m_dVtheor = 0;
	m_dVmat = 0;
	m_nNumLayers = 0;
	m_dPathTravel = 0;
	m_dJogTravel = 0;
	m_ctsTimeEst = 0;
	m_bBoundFirst = true;
	m_ctType = CHUNKTYPE_UNDEFINED;
}

// --------------------------------------------------------------------------
CChunk::~CChunk()
// --------------------------------------------------------------------------
{

}

//---------------------------------------------------------------------------
void CChunk::Serialize(CArchive& ar)
//---------------------------------------------------------------------------
{// Save / Load

	int version = 10;

	if (ar.IsStoring()) { //add storing code here

		ar << version;

		// save data

		ar << id;
		ar << selected;
		geometry.Serialize(ar);
		color.Serialize(ar);
		ar << alpha;
		ar << materialname;
		ar << toolname;
		ar << m_nPriority;
		ar << m_dVtheor;
		ar << m_dVmat;
		ar << m_nNumLayers;
		ar << m_dPathTravel;
		ar << m_dJogTravel;
		ar << m_sName;
		ar << m_bBoundFirst;
		ar << (int)m_ctType;

		ar << version;

	}	else	{  // add loading code here

		int version1, version2;
		ar >> version1; if (version1 < 10 || version1 > version) throw 6;

		// load data

		ar >> id;
		ar >> selected;
		geometry.Serialize(ar);
		color.Serialize(ar);
		ar >> alpha;
		ar >> materialname;
		ar >> toolname;
		ar >> m_nPriority;
		ar >>m_dVtheor;
		ar >>m_dVmat;
		ar >>m_nNumLayers;
		ar >>m_dPathTravel;
		ar >>m_dJogTravel;
		ar >> m_sName;
		ar >> m_bBoundFirst;
		int tp;
		ar >> tp;
		m_ctType = (CHUNKTYPE)tp;
		ar >> version2; if (version2 != version1) throw 6;

	}
}

//---------------------------------------------------------------------------
CChunk::CChunk(CChunk& s)
//---------------------------------------------------------------------------
{
	*this = s;
}

//---------------------------------------------------------------------------
CChunk& CChunk::operator=(const CChunk& s)
//---------------------------------------------------------------------------
{

	geometry = s.geometry;
	id = s.id;
	selected = s.selected;
	color = s.color;
	alpha = s.alpha;
	materialname = s.materialname;
	toolname = s.toolname;
	m_nPriority = s.m_nPriority;
	m_dVtheor = s.m_dVtheor;
	m_dVmat = s.m_dVmat;
	m_nNumLayers = s.m_nNumLayers;
	m_dPathTravel = s.m_dPathTravel;
	m_dJogTravel = s.m_dJogTravel;
	m_ctsTimeEst = s.m_ctsTimeEst;
	m_sName = s.m_sName;
	m_bBoundFirst = s.m_bBoundFirst;
	m_ctType = s.m_ctType;
	return *this;
}

//---------------------------------------------------------------------------
void CChunk::SetName(CString &filename)
//---------------------------------------------------------------------------
{
	//get the name from the file name
	m_sName = filename;
}

//---------------------------------------------------------------------------
void CChunk::SetBoundFirst(bool bBoundFirst)
//---------------------------------------------------------------------------
{
	//set user pref for whether boundary paths should be deposited
	//before or after raster fill paths
	if(bBoundFirst != m_bBoundFirst)
	{
		//
		m_bBuildReady = false;
		m_bBoundFirst = bBoundFirst;
	}
}

//---------------------------------------------------------------------------
void CChunk::SetPriority(int priority)
//---------------------------------------------------------------------------
{
	//update the build priority value for this chunk
	if(priority != m_nPriority)
	{
		m_nPriority = priority;
		m_bBuildReady = false;
	}
}

//---------------------------------------------------------------------------
void CChunk::ComputeBoundingBox(CVec &pmin, CVec &pmax)
//---------------------------------------------------------------------------
{
	geometry.ComputeBoundingBox(pmin,pmax);
}

/*//---------------------------------------------------------------------------
bool CChunk::Slice(CModel *pModel, CProgressDlg* pProgDlg)
//---------------------------------------------------------------------------
{
	// slice chunk according to material parameters
	// if chunk only contains vector information, then no need to slice

	CVec pmin, pmax;
	geometry.ComputeBoundingBox(pmin, pmax);
	if(geometry.mesh.GetSize() > 0)
	{

		// get material

		double thick, width;
		bool ok = GetPathProperties(thick, width);
		if (!ok)
			return false;

		// slice chunk

		CLayer layer;
		CString msg;
		m_nNumLayers = 0;
		int nLayers = (pmax.z-pmin.z)/thick;
		for (double z=pmin.z+thick; z<pmax.z; z+=thick)
		{
			m_nNumLayers++;
			msg.Format("Slicing %s, Layer %d of %d", m_sName,m_nNumLayers,nLayers);
			pModel->SetStatusMsg(msg);
			layer.boundary.RemoveAll();
			layer.contour.RemoveAll();
			layer.path.RemoveAll();
			bool ok = geometry.ComputeZSlice(z, layer.boundary);
			if (!ok) 
				return false;
			//terminate early if requested
			if(pModel->IsSlicingCancelled())
				return false;
			//set the path type for the generated boundary paths
			CPath::SetPathTypes(layer.boundary,PATHTYPE_BOUNDARY);
			//clean up the paths to remove excess vertices
			CPath::Consolidate(layer.boundary, width/3);
			CArray<CPath,CPath> offsetcontour;
			//offset the boundary by 1/2 the width of a toolpath
			//to create the contour paths at layer edge
			layer.contour.Copy(layer.boundary);
			CPath::Offset(layer.contour, width/2.0, CVec(0,0,1), offsetcontour);
			layer.contour.Copy(offsetcontour);
			CPath::SetPathTypes(layer.contour,PATHTYPE_CONTOUR);
			//now randomize the start points of the contour paths
			//to prevent colocation of start/stop flaws
			layer.RandStartCntr();
			layer.chunkid = id;
			layer.toolname = toolname;
			layer.matname = materialname;
			//for each layer, orient the raster fills
			//where orientation has reduced fill density below threshold
			//find orientation which improves fill density
			CArray<CPath,CPath> rasterContour;
			rasterContour.Copy(layer.contour);
			CPath::Clean(rasterContour,width/10);
			CPath::Offset(rasterContour, width, CVec(0,0,1), offsetcontour);
			layer.path.Copy(offsetcontour);
			double fillRatio;
			int trials = 0;
			double angle = PI*(m_nNumLayers)/4+PI/4;
			do{
				layer.path.RemoveAll();
				layer.RasterizeSimple(offsetcontour, 0, width, CVec(cos(angle),sin(angle),0),layer.path);
				CPath::SetPathTypes(layer.path, PATHTYPE_RASTER);
				layer.CalcLayerStats();
				fillRatio = layer.GetRoadVolume()/layer.GetTheoreticalVolume();
				trials++;
				angle += 15.0/180.0*PI;
				TRACE("Rasterizing: Angle:%f, FillRatio:%f\n",angle, fillRatio);
			} while((fillRatio < 0.5) && (fillRatio > 0.0) && trials < 6);

			//now minimize travel over the paths
			//layer.MinimizeTravel();
				
			CSingleLock accessLock(&pModel->m_mutAccess);
			accessLock.Lock();
			pModel->fab.layer.Add(layer);
			accessLock.Unlock();
			if (pProgDlg != 0)
			{
				CString text;
				text.Format("Slicing z = %.2f", z);
				pProgDlg->m_text.SetWindowText(text);
				pProgDlg->Increment();
				if (pProgDlg->canceled)
					return false;
			}
		}
	}
	//now handle the vector data, if any
	if(geometry.vectors.GetSize() > 0)
	{
		//just create a layer to hold the vector paths
		CLayer vectorLayer;
		vectorLayer.path.RemoveAll();
		vectorLayer.path.Copy(geometry.vectors);
		vectorLayer.chunkid = id;
		vectorLayer.toolname = toolname;
		vectorLayer.matname = materialname;
		CPath::SetPathTypes(vectorLayer.path,PATHTYPE_VECTOR);
		pModel->fab.layer.Add(vectorLayer);
	}

	//should be build-ready now
	m_bBuildReady = true;
	return m_bBuildReady;
}*/


//---------------------------------------------------------------------------
bool CChunk::SliceVectors(CModel *pModel)
//---------------------------------------------------------------------------
{
	if(GetType()==CHUNKTYPE_VCT)
	{
		CLayer tempLayer;
		CRegion tempRegion;
		tempLayer.m_coordSys.SetOrg(CVec(0,0,geometry.vectors[0].v[0].z));
		m_nNumLayers++;
		CString msg;
		msg.Format("Adding %s to paths", m_sName);
		pModel->SetStatusMsg(msg);
		tempRegion.path.Copy(geometry.vectors);
		CPath::SetPathTypes(tempRegion.path,PATHTYPE_VECTOR);
		tempLayer.region.Add(tempRegion);
		tempLayer.chunkid = id;
		tempLayer.toolname = toolname;
		tempLayer.matname = materialname;

		CSingleLock accessLock(&pModel->fab.m_mutAccess);
		accessLock.Lock();
		pModel->fab.layer.Add(tempLayer);
		accessLock.Unlock();
		AfxGetMainWnd()->Invalidate();
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------
bool CChunk::Slice(CModel *pModel, CProgressDlg* pProgDlg)
//---------------------------------------------------------------------------
{
	// slice a chunk according to tool/material parameters
	//store the chunk boundary contours in a boundary tree inside a layer
	if(geometry.vectors.GetSize() > 0)
		return SliceVectors(pModel);
	if(geometry.mesh.GetSize() <= 0)
		return false;

	CVec pmin, pmax;
	geometry.ComputeBoundingBox(pmin, pmax);
	if (pmin.z == pmax.z)
		return true;

	double thick, width, minLen;
	bool ok = GetPathProperties(thick, width, minLen);
	if (!ok)
		return false;

	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();

	// slice chunk
	//set up a temporary Layer, and an array of paths to hold boundaries
	m_nNumLayers = 0; //reset the layers count
	CLayer tempLayer;
	CArray<CPath,CPath> boundaries;
	CString msg;
	int nLayers = (int)floor((pmax.z-pmin.z)/thick);
	//the first slice is at 1 toolpath thickness above the origin plane
	//the last slice will be at <=pmax.z
	for (double z=pmin.z+thick; z<=pmax.z; z+=thick)
	{
		//give the layer an origin
		tempLayer.m_coordSys.SetOrg(CVec(0,0,z));
		//calculate the contour (intersection of the layer with the cutting plane)
		//use tempLayer.region[0].path for scratch
		m_nNumLayers++;
		msg.Format("Slicing %s, Layer %d of %d", m_sName,m_nNumLayers,nLayers);
		pModel->SetStatusMsg(msg);
		//sometimes a slice will fail because z is outside of the model by a rounding tolerance 
		if(!geometry.ComputeZSlice(z, boundaries))
			if(!geometry.ComputeZSlice(z-0.01*thick, boundaries)) 
				continue;
		
		//set the path types to BOUNDARY
		CPath::SetPathTypes(boundaries, PATHTYPE_BOUNDARY);
		tempLayer.chunkid = id;
		//ComputeZSlice returns unordered, unconnected segments, 
		//so order and glue them together at a certain resolution
		CPath::Consolidate(boundaries, width/10.0); 
		CPath::Clean(boundaries, width/10.0);
		//this should result in 1 or more boundary paths which may be nested
		//now load the boundary paths into the boundaryTree by nesting depth
		//TRACE("Nesting boundaries for layer %f\n", z/thick);
		if(!pModel->fab.NestBoundaries(boundaries, tempLayer.boundaryForest)) return false;
		//generate regions using these boundaries
		tempLayer.MakeRegions();
		double angle;
		switch(nLayers)
		{
		case 1: 
			angle = 0.0; break;
		default:
			angle = PI*(m_nNumLayers-1)/2.0 + PI/4.0;
		}

		for(int i = 0; i < tempLayer.region.GetSize(); i++)
		{
			tempLayer.region[i].RasterizeOffset(width, CVec(cos(angle),sin(angle),0));
			for(int j = 0; j < tempLayer.region[i].path.GetSize(); j++)
			{
				if((tempLayer.region[i].path[j].type == PATHTYPE_RASTER) &&
					(tempLayer.region[i].path[j].GetLength() < minLen))
					tempLayer.region[i].path.RemoveAt(j);
				else if(tempLayer.region[i].path[j].type == PATHTYPE_CONTOUR)
					tempLayer.region[i].path[j].CircShift(rand()%tempLayer.region[i].path[j].v.GetSize());
			}
			//terminate early if requested
			if(pModel->IsSlicingCancelled())
				return false;
		}
		//load the layer with its forest into the the process
		CSingleLock accessLock(&pModel->m_mutAccess);
		accessLock.Lock();
		pModel->fab.layer.Add(tempLayer);
		//clean the temporaries
		tempLayer.region.RemoveAll();
		boundaries.RemoveAll();
		tempLayer.boundaryForest.RemoveAll();
		AfxGetMainWnd()->Invalidate();
		accessLock.Unlock();
	}
	return true;
}

//---------------------------------------------------------------------------
bool CChunk::CalcChunkStats(CModel *pModel)
//---------------------------------------------------------------------------
{
	//calculate some statistics for this chunk
	m_dJogTravel = 0;
	m_dPathTravel = 0;
	m_dVmat = 0;
	m_dVtheor = 0;
	m_ctsTimeEst = 0;
	for(int i = 0; i < pModel->fab.layer.GetSize(); i++)
	{
		if(pModel->fab.layer[i].chunkid == id)
		{
			//calculate properties for this layer
			pModel->fab.layer[i].CalcLayerStats(m_ctType);
			//accumulate stats for chunk
			m_dJogTravel += pModel->fab.layer[i].GetJogTravel();
			m_dPathTravel += pModel->fab.layer[i].GetPathTravel();
			m_dVmat += pModel->fab.layer[i].GetMaterialVolume();
			m_dVtheor += pModel->fab.layer[i].GetTheoreticalVolume();
			m_ctsTimeEst += pModel->fab.layer[i].GetTimeEst();
		}
	}
	return true;
}


//---------------------------------------------------------------------------
int CChunk::CountSlices()
//---------------------------------------------------------------------------
{

	CVec pmin, pmax;
	geometry.ComputeBoundingBox(pmin, pmax);
	if (pmin.z == pmax.z)
		return 0;

	// get path properties

	double thick, width, minLen;
	bool ok = GetPathProperties(thick, width, minLen);
	if (!ok)
		return -1;

	// count slices

	return (int) ((pmax.z-pmin.z) / thick);
}

//---------------------------------------------------------------------------
bool CChunk::IsProcessReady()
//---------------------------------------------------------------------------
{
	//is the chunk ready to be processed
	//check that valid tool and material properties have been assigned
	//TODO: check for suitable size and position in work volume
	return (CTool::SearchByName(toolname) >= 0);
}

//---------------------------------------------------------------------------
bool CChunk::IsBuildReady()
//---------------------------------------------------------------------------
{
	//is the chunk ready to be built?
	return m_bBuildReady;
}

//---------------------------------------------------------------------------
void CChunk::Draw(CModel *pModel, bool bModelhNormals, bool bShaded)
//---------------------------------------------------------------------------
{
	
	// get material

	glColor4d(color.x, color.y, color.z, alpha);

	geometry.Draw(bModelhNormals, bShaded);

	if (selected) {
		CVec pmin, pmax;
		geometry.ComputeBoundingBox(pmin, pmax);
		glPushMatrix();
		glTranslated(pmin.x,pmin.y,pmin.z);
		glScaled(pmax.x-pmin.x,pmax.y-pmin.y,pmax.z-pmin.z);
		CGLUtils::DrawCube();
		glPopMatrix();
	}
}

//---------------------------------------------------------------------------
bool CChunk::GetPathProperties(double& thick, double& width, double& minLen)
//---------------------------------------------------------------------------
{// width path width and layer thickness for current chunk

	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();

	int toolidx = CTool::SearchByName(toolname);
	if (toolidx == -1) {
		AfxMessageBox("Model tool does not match printer tool.");
		return false;
	} else {
		thick = pApp->tool[toolidx].pathheight;
		width = pApp->tool[toolidx].pathwidth;
		minLen = pApp->tool[toolidx].GetMinPathLength();
	}

	return true;
}