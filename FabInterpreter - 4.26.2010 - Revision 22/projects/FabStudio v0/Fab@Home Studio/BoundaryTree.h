// BoundaryTree.h: interface for the CBoundaryTree class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOUNDARYTREE_H__C5C38779_71C6_4560_B63B_6533CCCD6765__INCLUDED_)
#define AFX_BOUNDARYTREE_H__C5C38779_71C6_4560_B63B_6533CCCD6765__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBoundaryTreeNode : public CObject
{
	DECLARE_SERIAL( CBoundaryTreeNode );
	friend class CBoundaryTree;
public:
	CBoundaryTreeNode();
	CBoundaryTreeNode(const CBoundaryTreeNode& s);
	CBoundaryTreeNode(const CPath& path);
	CBoundaryTreeNode(const CPath& path, int ndIdx, int pIdx, int lCIdx, int rSIdx);
	virtual ~CBoundaryTreeNode();
	CBoundaryTreeNode& operator=(const CBoundaryTreeNode& s);
	bool operator==(const CBoundaryTreeNode& s);
	bool operator==(const CPath& path);
	void Serialize( CArchive& ar );
	bool IsValid();
	bool HasChild();
	bool HasSibling();
  int GetNodeIdx(){return nodeIdx;};
  int GetLChildIdx(){return lChildIdx;};
  int GetRSiblingIdx(){return rSiblingIdx;};
  int GetParentIdx(){return parentIdx;};
  CPath& GetBoundary(){return boundary;};
private:
	CPath boundary;
  int nodeIdx;  //the index of this node
	int parentIdx;
	int lChildIdx;
	int rSiblingIdx;
};

//template <> void AFXAPI DestructElements <CBoundaryTreeNode> ( CBoundaryTreeNode* pObjects, int nCount );
template <> void AFXAPI SerializeElements <CBoundaryTreeNode> ( CArchive& ar, CBoundaryTreeNode* pNewObjects, int nCount );

#endif // !defined(AFX_BoundaryTREE_H__C5C38779_71C6_4560_B63B_6533CCCD6765__INCLUDED_)

class CBoundaryTree : public CObject  
{
	DECLARE_SERIAL( CBoundaryTree );
public:
	CBoundaryTreeNode* Find(CPath& path);
	int InsertRootNode( CPath& path );
	int InsertChildNode( int parIdx, CPath& path );
	void ReplaceSubtree( CBoundaryTree& subtree );
	void FreeAll();
	void Serialize( CArchive& ar);
	CBoundaryTree();
	CBoundaryTree( const CBoundaryTree& s );
	CBoundaryTree( const CPath& path );
	virtual ~CBoundaryTree();
  int RootIdx();
  CBoundaryTreeNode& Root();
  CBoundaryTreeNode* pRoot();
	CBoundaryTreeNode& Ref( int nodeIdx );
  CBoundaryTreeNode* pNode( int nodeIdx );
  int ParentIdx( CBoundaryTreeNode& node ){return node.parentIdx;};
  int LChildIdx( CBoundaryTreeNode& node ){return node.lChildIdx;};
  int RSiblingIdx( CBoundaryTreeNode& node ){return node.rSiblingIdx;};
  CBoundaryTreeNode* pParent(CBoundaryTreeNode* pNode);
  CBoundaryTreeNode* pLChild(CBoundaryTreeNode* pNode);
  CBoundaryTreeNode* pRSibling(CBoundaryTreeNode* pNode);
	CBoundaryTree& operator=(const CBoundaryTree& s);
	bool operator==(const CBoundaryTree& s);
	int Depth();
	bool Sibling(CBoundaryTree& node);
	bool IsValid();
	bool IsValidIndex( int idx );
	void Draw(CVec basecolor, bool bPathNormals = false);
	CMutex m_mutAccess;

private:
	CArray<CBoundaryTreeNode, CBoundaryTreeNode> data;
	int rootIdx;
};

//template <> void AFXAPI DestructElements <CBoundaryTree> ( CBoundaryTree* pObjects, int nCount );
template <> void AFXAPI SerializeElements <CBoundaryTree> ( CArchive& ar, CBoundaryTree* pNewObjects, int nCount );


