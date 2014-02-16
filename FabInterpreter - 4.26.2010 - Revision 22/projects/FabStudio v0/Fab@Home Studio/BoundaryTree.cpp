// BoundaryTree.cpp: implementation of the CBoundaryTree class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BoundaryTree.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CBoundaryTree, CObject, 1 );

CBoundaryTree::CBoundaryTree()
{

}

CBoundaryTree::CBoundaryTree( const CPath& path )
{
	CBoundaryTreeNode rootNode(path);
	rootIdx = data.Add(rootNode);
}

CBoundaryTree::CBoundaryTree( const CBoundaryTree& s )
{
	data.Copy(s.data);
}

CBoundaryTree::~CBoundaryTree()
{
	FreeAll();
}

bool CBoundaryTree::IsValid()
{
	//make sure that the tree data structure is valid
	
	return(this->IsKindOf(RUNTIME_CLASS( CBoundaryTree ))>0);
}

bool CBoundaryTree::IsValidIndex( int idx )
{
	//make sure that the index is valid
	return( (idx < data.GetSize()) && (idx >= 0) );
}

void CBoundaryTree::FreeAll()
{
	data.RemoveAll();
	rootIdx = -1;
}

void CBoundaryTree::Draw(CVec basecolor, bool bPathNormals)
{
	for(int i = 0; i < data.GetSize(); i++)
	{
		data[i].boundary.Draw(bPathNormals, basecolor);
	}
}



/*//---------------------------------------------------------------------------
template <> void AFXAPI DestructElements <CBoundaryTree> ( CBoundaryTree* pObjects, int nCount )
//---------------------------------------------------------------------------
{// 
	for ( int i=0; i<nCount; i++, pObjects++ )    {
        // call default destructor directly
        pObjects->~CBoundaryTree();
	
	}
}*/

//---------------------------------------------------------------------------
template <> void AFXAPI SerializeElements <CBoundaryTree> ( CArchive& ar, CBoundaryTree* pNewObjects, int nCount )
//---------------------------------------------------------------------------
{// serialize an array
    
	for ( int i = 0; i < nCount; i++, pNewObjects++ )    {
        // Serialize each object        
		pNewObjects->Serialize(ar);
    }
}

 
void CBoundaryTree::Serialize(CArchive &ar)
{// Save / Load

	int version = 1;

	if (ar.IsStoring()) { //add storing code here

		ar << version;

		// save data
		ar << rootIdx;
		data.Serialize(ar);

		ar << version;

	}	else	{  // add loading code here

		int version1, version2;
		ar >> version1; if (version1 < 1 || version1 > version) throw 6;

		// load data
		ar >> rootIdx;
		data.Serialize(ar);
    
		ar >> version2; if (version2 != version1) throw 6;
	}
}

int CBoundaryTree::InsertRootNode( CPath& path )
{
	//empty the tree, then insert a new root node with path as boundary
	FreeAll();
	CBoundaryTreeNode root(path);
	root.nodeIdx = 0;
	rootIdx = data.Add(root);
	return(rootIdx);
}
	
CBoundaryTreeNode& CBoundaryTree::Root()
{
  //return the root node
  ASSERT(IsValidIndex(rootIdx));
	return( data[rootIdx] );
}

CBoundaryTreeNode* CBoundaryTree::pRoot()
{
  //return a pointer to the root node
  if(IsValidIndex(rootIdx))
	  return( &(data[rootIdx]) );
  return NULL;
}

int CBoundaryTree::RootIdx()
{
	return( rootIdx );
}

int CBoundaryTree::InsertChildNode( int parIdx, CPath& path )
{
	//Insert the node into the tree as a child of the node at parIdx.  
	//Return the array index of the inserted node.
	//The node should be a leaf - no parent, children, or siblings.
	//If any errors, return -1.

	//check the validity of inputs
	if( !IsValidIndex(parIdx) ) return -1;

	CBoundaryTreeNode node(path);
	//set the parentIdx for the node
	node.parentIdx = parIdx;

	//insert the node and record its location
	node.nodeIdx = data.Add(node);
	data[node.nodeIdx].nodeIdx = node.nodeIdx;

	//check if the parent has any children; if so, establish siblings
	if(IsValidIndex(data[parIdx].lChildIdx))
	{
		int tempIdx = data[parIdx].lChildIdx;
		while(IsValidIndex(data[tempIdx].rSiblingIdx))
		{
			tempIdx = data[tempIdx].rSiblingIdx;
		}
		//add the new node as the right sibling of the rightmost child of the node at parIdx
		data[tempIdx].rSiblingIdx = node.nodeIdx;
	}
	else
	{
		//no left children, so set left child to the new node's index
		data[parIdx].lChildIdx = node.nodeIdx;
	}
	return node.nodeIdx;
}

CBoundaryTreeNode& CBoundaryTree::Ref( int nodeIdx )
{ 
	//return reference to the node at nodeIdx
	ASSERT( (nodeIdx < data.GetSize()) && (nodeIdx >= 0) );
	return( data[nodeIdx] );
}

CBoundaryTreeNode* CBoundaryTree::pNode( int nodeIdx )
{
  //return a pointer to the node at nodeIdx
   if(IsValidIndex(nodeIdx))
	  return( &(data[nodeIdx]) );
  return NULL;
}

CBoundaryTreeNode* CBoundaryTree::pParent(CBoundaryTreeNode* pNode)
{
  //return a pointer to the parent node of this node
  //if the node has no parent (e.g. is a root), return NULL
  ASSERT( pNode->IsValid() );
  if( IsValidIndex(pNode->parentIdx) )
  {
    return( &(data[pNode->parentIdx]) );
  }
  return(NULL);
}

CBoundaryTreeNode* CBoundaryTree::pLChild(CBoundaryTreeNode* pNode)
{
  //return a reference to the left child node of this node
  //if the node has no left child, return NULL
  ASSERT( pNode->IsValid() );
  if( IsValidIndex(pNode->lChildIdx) )
  {
    return( &(data[pNode->lChildIdx]) );
  }
  return(NULL);
}

CBoundaryTreeNode* CBoundaryTree::pRSibling(CBoundaryTreeNode* pNode)
{
  //return a reference to the right sibling node of this node
  //if the node has not right sibling, return NULL
  ASSERT( pNode->IsValid() );
  if( IsValidIndex(pNode->rSiblingIdx) )
  {
    return( &(data[pNode->rSiblingIdx]) );
  }
  return(NULL);
}

/*void CBoundaryTree::InsertChildSubtree( CBoundaryTree& subtree )
//Insert the subtree, assigning THIS as parentIdx
{
	//start by assigning THIS as parentIdx
	subtree.parentIdx = this;
	//if THIS has no children, update THIS show subtree as left child
	if(lChildIdx == NULL)
	{
		lChildIdx = &subtree;
	}
	else
	{
		//insert after the rightmost sibling of this's left child
		CBoundaryTree *temp;
		temp = this->lChildIdx;
		while(temp->rSiblingIdx != NULL)
		{
			temp = temp->rSiblingIdx;
		}
		temp->rSiblingIdx = &subtree;
	}
}

	
void CBoundaryTree::ReplaceSubtree( CBoundaryTree& subtree )
//Insert subtree, replacing the subtree Boundary at THIS
//Free the memory associated with THIS subtree
{
	ASSERT(subtree != NULL);
	ASSERT(subtree.rSiblingIdx == NULL);

	//set the parent of subtree to the parent of THIS
	subtree.parentIdx = parentIdx;
	if(parentIdx != NULL)
	{
		if(parentIdx->lChildIdx == this)
		{
			 //THIS is the left child, so adopt subtree
			parentIdx->lChildIdx == &subtree;
		}
	}
	if(rSiblingIdx != NULL)
	{
		//THIS has right siblings, so make subtree a sibling
		subtree.rSiblingIdx = rSiblingIdx;
	}

	//then set the parent of THIS to NULL
	parentIdx = NULL;
	//and free THIS subtree
	FreeAll();
}*/


CBoundaryTree& CBoundaryTree::operator=(const CBoundaryTree& s)
{
	data.Copy(s.data);
	return(*this);
}


bool CBoundaryTree::operator==(const CBoundaryTree& s)
{
	//assumes that the data is all in the same order
	for(int i = 0; i < data.GetSize(); i++)
	{
		if(!(data[i] == s.data[i]))
			return false;
	}
	return true;
}


/*CBoundaryTree* CBoundaryTree::FindItem(<Class T>& it)
{
	//search the subtree Boundary at THIS for the item (it)
	//return the first node containing it or NULL if not found
	//recursive
	if(it == NULL) return NULL;
	CBoundaryTree* temp;
	temp = this;
	if(temp->item == it)
		return temp;
	else
	{
		if( temp->pLeftChild != NULL )
		{
			return( temp->pLeftChild->FindItem(it) );
		}
		else
		{
			return( temp->pRightSibling->FindItem(it) );
		}
	}
}*/



/*int CBoundaryTree::Depth()
{
	//calculate the depth of THIS node in its parent tree
	//root of tree is zero depth
	int d = 0;
	CBoundaryTree* temp = parentIdx;
	//walk up the tree toward the root
	while(temp != NULL)
	{
		d++;
		temp = temp.parentIdx;
	}
	return(d);
}*/


/*bool CBoundaryTree::Sibling( CBoundaryTree& node )
{
	//return true if THIS and node have same parent
	return(parentIdx == node.parentIdx);
}*/

/////////////////////////////////////////////////////////////////////////////////////////////
///////IMPLEMENTATION FOR CBOUNDARYTREENODE//////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL( CBoundaryTreeNode, CObject, 1 );


CBoundaryTreeNode::CBoundaryTreeNode()
{

}


CBoundaryTreeNode::CBoundaryTreeNode(const CBoundaryTreeNode& s)
{
	boundary = s.boundary;
  nodeIdx = s.nodeIdx;
	parentIdx = s.parentIdx;
	lChildIdx = s.lChildIdx;
	rSiblingIdx = s.rSiblingIdx;
}

CBoundaryTreeNode::CBoundaryTreeNode(const CPath& path)
{
	boundary = path;
  nodeIdx = -1;
	parentIdx = -1;
	lChildIdx = -1;
	rSiblingIdx = -1;
}

CBoundaryTreeNode::CBoundaryTreeNode(const CPath& path, int ndIdx, int pIdx,
																		 int lCIdx, int rSIdx)
{
	boundary = path;
  nodeIdx = ndIdx;
	parentIdx = pIdx;
	lChildIdx = lCIdx;
	rSiblingIdx = rSIdx;
}

bool CBoundaryTreeNode::IsValid()
{
	//make sure that the node is valid
	
	return(this->IsKindOf(RUNTIME_CLASS( CBoundaryTreeNode ))>0);
}

bool CBoundaryTreeNode::HasChild()
{
	//return true if this node has a child
	return(this->lChildIdx >= 0);
}

bool CBoundaryTreeNode::HasSibling()
{
	//return true if this node has a right sibling
	return(this->rSiblingIdx >= 0);
}
	
CBoundaryTreeNode::~CBoundaryTreeNode()
{

}


CBoundaryTreeNode& CBoundaryTreeNode::operator=(const CBoundaryTreeNode& s)
{
	boundary = s.boundary;
  nodeIdx = s.nodeIdx;
	parentIdx = s.parentIdx;
	lChildIdx = s.lChildIdx;
	rSiblingIdx = s.rSiblingIdx;
	return(*this);
}


bool CBoundaryTreeNode::operator==(const CBoundaryTreeNode& s)
{
	return((boundary == s.boundary) && (nodeIdx == s.nodeIdx) &&
    (parentIdx == s.parentIdx) && (lChildIdx == s.lChildIdx) && 
    (rSiblingIdx == s.rSiblingIdx));
}

bool CBoundaryTreeNode::operator==(const CPath& path)
{
	return(boundary == path);
}


/*//---------------------------------------------------------------------------
template <> void AFXAPI DestructElements <CBoundaryTreeNode> ( CBoundaryTreeNode* pObjects, int nCount )
//---------------------------------------------------------------------------
{// 
	for ( int i=0; i<nCount; i++, pObjects++ )    {
        // call default destructor directly
        pObjects->~CBoundaryTreeNode();
	
	}
}*/

//---------------------------------------------------------------------------
template <> void AFXAPI SerializeElements <CBoundaryTreeNode> ( CArchive& ar, CBoundaryTreeNode* pNewObjects, int nCount )
//---------------------------------------------------------------------------
{// serialize an array
    
	for ( int i = 0; i < nCount; i++, pNewObjects++ )    {
        // Serialize each object        
		pNewObjects->Serialize(ar);
    }
}


void CBoundaryTreeNode::Serialize(CArchive &ar)
{// Save / Load

	int version = 1;

	if (ar.IsStoring()) { //add storing code here

		ar << version;

		// save data
    ar << nodeIdx;
		ar << parentIdx;
		ar << lChildIdx;
		ar << rSiblingIdx;
		boundary.Serialize(ar);

		ar << version;

	}	else	{  // add loading code here

		int version1, version2;
		ar >> version1; if (version1 < 1 || version1 > version) throw 6;

		// load data
    ar >> nodeIdx;
		ar >> parentIdx;
		ar >> lChildIdx;
		ar >> rSiblingIdx;
		boundary.Serialize(ar);
    
		ar >> version2; if (version2 != version1) throw 6;
	}
}