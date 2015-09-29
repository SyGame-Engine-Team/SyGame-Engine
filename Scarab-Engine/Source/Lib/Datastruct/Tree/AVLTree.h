/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Datastruct/Tree/AVLTree.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : The Adelson-Velskii-Landis Tree.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_LIB_DATASTRUCT_TREE_AVLTREE_H
#define SCARAB_LIB_DATASTRUCT_TREE_AVLTREE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Datastruct.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
    // Height changes
enum AVLTreeHeightChange
{
    AVLTREE_HEIGHT_NOCHANGE = 0,
    AVLTREE_HEIGHT_CHANGE = 1
};

    // Balance factors
#define AVLTREE_LEFT_HEAVY  (-1)
#define AVLTREE_BALANCED    0
#define AVLTREE_RIGHT_HEAVY 1
#define AVLTREE_IMBALANCE_LEFT(_bal) ( (_bal) < AVLTREE_LEFT_HEAVY )
#define AVLTREE_IMBALANCE_RIGHT(_bal) ( (_bal) > AVLTREE_RIGHT_HEAVY )

    // Childs
enum AVLTreeChild
{
    AVLTREE_CHILD_LEFT = 0,
    AVLTREE_CHILD_RIGHT = 1
};
#define AVLTREE_INVERTDIR(_dir) ( (AVLTreeChild)(1 - (_dir)) )

    // Searching
typedef DWord AVLTreeSearch;
#define AVLTREE_SEARCH_EQUAL     0x0001
#define AVLTREE_SEARCH_LESSER    0x0002
#define AVLTREE_SEARCH_GREATER   0x0004
#define AVLTREE_SEARCH_FIT_FIRST 0x0100
#define AVLTREE_SEARCH_FIT_BEST  0x0200
#define AVLTREE_SEARCH_FIT_WORST 0x0400

/////////////////////////////////////////////////////////////////////////////////
// The AVLTree class
template<typename T>
class AVLTree : public Datastruct
{
private:
    // nested node
    typedef struct _avl_node
    {
        struct _avl_node * pChild[2];
        Int iBalanceFactor;
        T Item;
    } AVLNode;

public:
    // comparator : returns (l#r = +1), (r#l = -1), (l==r = 0)
    //              '#' may represent any ordering relation you wish (<,>,<=,>=,...).
    typedef Int (*Comparator)( const T & rLeft, const T & rRight );

	// constructors
	AVLTree();
	virtual ~AVLTree();

    // delayed construction
    inline Void SetComparator( Comparator pfCompare );

    // common minimal methods
    virtual Void Create();
    virtual Void Destroy();

    inline virtual UInt MemorySize() const;
    inline virtual UInt Count() const;

    virtual Void Clear();

    // methods
	inline Bool Insert( const T & rItem );
	inline Bool Remove( const T & rItem );

	Bool Search( T & rOut, const T & rItem, AVLTreeSearch searchFlags ) const;

private:
    // AVL Rotations
    AVLTreeHeightChange _RotateOnce( AVLNode ** ppNode, AVLTreeChild rotDir );
	AVLTreeHeightChange _RotateTwice( AVLNode ** ppNode, AVLTreeChild rotDir );
	AVLTreeHeightChange _ReBalance( AVLNode ** ppNode );

    // Recursive insert & remove
	Bool _rec_Insert( AVLNode ** ppNode, AVLTreeHeightChange & heightChange, const T & rItem );
	Bool _rec_Remove( AVLNode ** ppNode, AVLTreeHeightChange & heightChange, const T & rItem );
    Void _rec_Destroy( AVLNode * pNode );

	// Data
	Comparator m_pfCompare;

    UInt m_iNodeCount;
    AVLNode * m_pRoot;

	AVLTreeHeightChange m_iHeightChange;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "AVLTree.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_DATASTRUCT_TREE_AVLTREE_H
