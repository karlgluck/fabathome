// Process.cpp: implementation of the CProcess class.
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
CProcess::CProcess()
//--------------------------------------------------------------------
{

}

//--------------------------------------------------------------------
CProcess::~CProcess()
//--------------------------------------------------------------------
{

}

//---------------------------------------------------------------------------
void CProcess::Serialize(CArchive& ar)
//---------------------------------------------------------------------------
{// Save / Load

	int version = 10;

	if (ar.IsStoring()) { //add storing code here

		ar << version;

		// save data

		layer.Serialize(ar);

		ar << version;

	}	else	{  // add loading code here

		int version1, version2;
		ar >> version1; if (version1 < 10 || version1 > version) throw 6;

		// load data

		layer.Serialize(ar);

		ar >> version2; if (version2 != version1) throw 6;

	}
}

//---------------------------------------------------------------------------
CProcess::CProcess(CProcess& s)
//---------------------------------------------------------------------------
{
	*this = s;
}

//---------------------------------------------------------------------------
CProcess& CProcess::operator=(const CProcess& s)
//---------------------------------------------------------------------------
{

	layer.Copy(s.layer);

	return *this;
}

//---------------------------------------------------------------------------
void CProcess::DrawLayers(CModel *pModel, bool bPathNormals, int firstlayer, int lastlayer)
//---------------------------------------------------------------------------
{

	if (layer.GetSize() == 0) 
		return;

	if (firstlayer == -1 && lastlayer == -1) {
		firstlayer = 0;
		lastlayer = layer.GetSize() -1;
	}

	ASSERT(firstlayer >= 0 && firstlayer < layer.GetSize());
	ASSERT(lastlayer >= 0 && lastlayer < layer.GetSize());
	ASSERT(lastlayer >= firstlayer);

	for (int i=firstlayer; i<=lastlayer; i++) {
		layer[i].Draw(bPathNormals, pModel);
	}

}
