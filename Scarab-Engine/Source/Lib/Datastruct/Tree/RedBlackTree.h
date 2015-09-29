/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Datastruct/Tree/RedBlackTree.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : The smart partial-balanced tree ...
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
#ifndef SCARAB_LIB_DATASTRUCT_TREE_REDBLACKTREE_H
#define SCARAB_LIB_DATASTRUCT_TREE_REDBLACKTREE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Datastruct.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
    // Childs
enum RBTreeChild
{
    RBTREE_CHILD_LEFT = 0,
    RBTREE_CHILD_RIGHT = 1
};
#define RBTREE_INVERTDIR(_dir) ( (RBTreeChild)(1 - (_dir)) )

    // Colors
enum RBTreeColor
{
    RBTREE_COLOR_BLACK = 0x00000000,
    RBTREE_COLOR_RED   = 0xffffffff
};

/////////////////////////////////////////////////////////////////////////////////
// The RedBlackTree class
template<typename T>
class RedBlackTree : public Datastruct
{
private:
    // nested node
    typedef struct _red_black_node
    {
        struct _red_black_node * pParent;
        struct _red_black_node * pChild[2];
        struct _red_black_node * pPrev;
        struct _red_black_node * pNext;
        RBTreeColor iColor;
        T Item;
    } RedBlackNode;

public:
    // comparator : returns (l#r = +1), (r#l = -1), (l==r = 0)
    //              '#' may represent any ordering relation you wish (<,>,<=,>=,...).
    typedef Int (*Comparator)( const T & rLeft, const T & rRight, Void * pUserData );

    // nested Iterator
    class Iterator
    {
    public:
        static const Iterator Null;

	    Iterator() {
            m_pNode = NULL;
        }
	    Iterator( const Iterator & rhs ) {
            m_pNode = rhs.m_pNode;
        }
	    ~Iterator() {
            // nothing to do
        }

	    inline Iterator & operator=( const Iterator & rhs ) {
            m_pNode = rhs.m_pNode;
            return (*this);
        }

	    inline Bool operator==( const Iterator & rhs ) const {
            return ( m_pNode == rhs.m_pNode );
        }
	    inline Bool operator!=( const Iterator & rhs ) const {
            return ( m_pNode != rhs.m_pNode );
        }

	    inline Iterator & operator++() {
            Assert( m_pNode != NULL );
            m_pNode = m_pNode->pNext;
            return (*this);
        }
	    inline Iterator & operator--() {
            Assert( m_pNode != NULL );
            m_pNode = m_pNode->pPrev;
            return (*this);
        }
	    inline Iterator operator++(int) {
            Assert( m_pNode != NULL );
            Iterator it( m_pNode );
            m_pNode = m_pNode->pNext;
            return it;
        }
	    inline Iterator operator--(int) {
            Assert( m_pNode != NULL );
            Iterator it( m_pNode );
            m_pNode = m_pNode->pPrev;
            return it;
        }

	    inline Bool IsNull() const {
            return ( m_pNode == NULL );
        }
	    inline Iterator Next() const {
            Assert( m_pNode != NULL );
            return Iterator( m_pNode->pNext );
        }
	    inline Iterator Previous() const {
            Assert( m_pNode != NULL );
            return Iterator( m_pNode->pPrev );
        }

        inline T & Get() const {
            Assert( m_pNode != NULL );
            return m_pNode->Item;
        }
        inline T * GetPtr() const {
            Assert( m_pNode != NULL );
            return &( m_pNode->Item );
        }

    private:
	    friend class RedBlackTree<T>;
	    Iterator( RedBlackNode * pNode ) {
            m_pNode = pNode;
        }

	    RedBlackNode * m_pNode;
    };

public:
	// constructors
	RedBlackTree();
	virtual ~RedBlackTree();

    // delayed construction
    inline Void SetComparator( Comparator pfCompare, Void * pUserData = NULL );

    // common minimal methods
    virtual Void Create();
    virtual Void Destroy();

    inline virtual UInt MemorySize() const;
    inline virtual UInt Count() const;

    virtual Void Clear();

    // methods
        // Returns true/false if item was found or not
        // If outpItem is specified, returns iterator to found
        // item or NULL if item was not found.
	Bool Search( const T & rItem, Iterator * outItem = NULL ) const;
    inline Bool Contains( const T & rItem ) const;

        // Returns true/false if insertion was done or not
        // If outpItem is specified, returns iterator to found
        // item if already exists or to newly created item
	Bool Insert( const T & rItem, Iterator * outItem = NULL );

        // Returns true/false if removal was done or not
        // If outItem is specified, returns value of deleted
        // item or nothing if item was not found (thus not deleted).
	Bool Remove( const T & rItem, T * outItem = NULL );

        // Iteration helpers
    inline Iterator Begin() const;
	inline Iterator End() const;

private:
    // Helpers
    Void _DeleteAll( RedBlackNode * pNode );
    Void _RotateOnce( RedBlackNode * pNode, RBTreeChild rotDir );

    // Insert
    Bool _Insert_Search( RedBlackNode ** outParentNode, RBTreeChild * outLeaf, const T & rItem ) const;
    Void _Insert_Rebalance( RedBlackNode * pNode );

    // Remove
    Bool _Remove_Search( RedBlackNode ** outNode, RBTreeChild * outNodeDir, const T & rItem ) const;
    Void _Remove_Swap( RedBlackNode ** ppNode, RBTreeChild iNodeDir );
    Void _Remove_Rebalance( RedBlackNode * pParent, RBTreeChild iNodeDir );

    // Enumeration
    //RedBlackNode * _Enumerate_First( RBTreeChild iEnumDir ) const;
    //static RedBlackNode * _Enumerate_Step( RedBlackNode * pNode, RBTreeChild iEnumDir );

	// Data
	Comparator m_pfCompare;
    Void * m_pUserData;

    UInt m_iNodeCount;
    RedBlackNode * m_pRoot;

    RedBlackNode *m_pHead, *m_pEnd;

    static RedBlackNode sm_SentinelNode;
    static RedBlackNode * RBTREE_LEAF;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "RedBlackTree.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_DATASTRUCT_TREE_REDBLACKTREE_H

