// Process.h: interface for the CProcess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROCESS_H__7DBAE451_5108_4A09_BEB2_99B49ED47BAA__INCLUDED_)
#define AFX_PROCESS_H__7DBAE451_5108_4A09_BEB2_99B49ED47BAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CProcess : public CObject  
{
public:
	void DrawLayers(CModel *pModel, bool bPathNormals, int firstlayer, int lastlayer);
	CProcess();
	virtual ~CProcess();
	void Serialize(CArchive& ar);
	CProcess(CProcess& s);
	CProcess& operator=(const CProcess& s);

	CArray<CLayer,CLayer> layer;

};

#endif // !defined(AFX_PROCESS_H__7DBAE451_5108_4A09_BEB2_99B49ED47BAA__INCLUDED_)
