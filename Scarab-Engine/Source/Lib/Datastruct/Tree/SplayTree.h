/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Datastruct/Tree/SplayTree.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : The heuristic self-optimizing tree ...
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
#ifndef SCARAB_LIB_DATASTRUCT_TREE_SPLAYTREE_H
#define SCARAB_LIB_DATASTRUCT_TREE_SPLAYTREE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Datastruct.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
    // Childs
enum SplayTreeChild
{
    SPLAYTREE_CHILD_LEFT = 0,
    SPLAYTREE_CHILD_RIGHT = 1
};
#define SPLAYTREE_INVERTDIR(_dir) ( (SplayTreeChild)(1 - (_dir)) )

/////////////////////////////////////////////////////////////////////////////////
// The SplayTree class
template<typename T>
class SplayTree : public Datastruct
{
private:
    // nested node
    typedef struct _splay_node
    {
        struct _splay_node * pParent;
        struct _splay_node * pChild[2];
        T Item;
    } SplayNode;

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
            m_pNode = SplayTree<T>::_Enumerate_Step( m_pNode, SPLAYTREE_CHILD_LEFT );
            return (*this);
        }
	    inline Iterator & operator--() {
            Assert( m_pNode != NULL );
            m_pNode = SplayTree<T>::_Enumerate_Step( m_pNode, SPLAYTREE_CHILD_RIGHT );
            return (*this);
        }
	    inline Iterator operator++(int) {
            Assert( m_pNode != NULL );
            Iterator it( m_pNode );
            m_pNode = SplayTree<T>::_Enumerate_Step( m_pNode, SPLAYTREE_CHILD_LEFT );
            return it;
        }
	    inline Iterator operator--(int) {
            Assert( m_pNode != NULL );
            Iterator it( m_pNode );
            m_pNode = SplayTree<T>::_Enumerate_Step( m_pNode, SPLAYTREE_CHILD_RIGHT );
            return it;
        }

	    inline Bool IsNull() const {
            return ( m_pNode == NULL );
        }
	    inline Iterator Next() const {
            Assert( m_pNode != NULL );
            return Iterator( SplayTree<T>::_Enumerate_Step( m_pNode, SPLAYTREE_CHILD_LEFT ) );
        }
	    inline Iterator Previous() const {
            Assert( m_pNode != NULL );
            return Iterator( SplayTree<T>::_Enumerate_Step( m_pNode, SPLAYTREE_CHILD_RIGHT ) );
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
	    friend class SplayTree<T>;
	    Iterator( SplayNode * pNode ) {
            m_pNode = pNode;
        }

	    SplayNode * m_pNode;
    };

public:
    SplayTree();
    virtual ~SplayTree();

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

        // Returns true/false if insertion was done or not
        // If outpItem is specified, returns iterator to found
        // item if already exists or to newly created item
	Bool Insert( const T & rItem, Iterator * outItem = NULL );

        // Returns true/false if removal was done or not
        // If outItem is specified, returns value of deleted
        // item or nothing if item was not found (thus not deleted).
	Bool Remove( const T & rItem, T * outItem = NULL );

        // Splay the node containing input item to the root
    Bool Raise( const T & rItem );

        // Pops most recently accessed item (root)
    Bool Extract( T * outItem = NULL );

        // Iteration helpers
    inline Iterator Begin() const;
	inline Iterator End() const;

private:
    // Helpers
    Void _DeleteAll( SplayNode * pNode );
    Void _RotateOnce( SplayNode * pNode, SplayTreeChild rotDir );
    Void _Splay( SplayNode * pNode );

    // Enumeration
    SplayNode * _Enumerate_First( SplayTreeChild iEnumDir ) const;
    static SplayNode * _Enumerate_Step( SplayNode * pNode, SplayTreeChild iEnumDir );

	// Data
	Comparator m_pfCompare;
    Void * m_pUserData;

    UInt m_iNodeCount;
    SplayNode * m_pRoot;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "SplayTree.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_DATASTRUCT_TREE_SPLAYTREE_H

