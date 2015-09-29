/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Datastruct/Table/TreeMap.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : The standard red/black-tree based map implementation
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_LIB_DATASTRUCT_TABLE_TREEMAP_H
#define SCARAB_LIB_DATASTRUCT_TABLE_TREEMAP_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Tree/RedBlackTree.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TreeMap class
template<typename Key, typename T>
class TreeMap : public Datastruct
{
private:
    // nested node
	typedef struct _treemap_node
	{
        inline struct _treemap_node & operator=( const struct _treemap_node & rhs ) {
            nodeKey = rhs.nodeKey;
            nodeItem = rhs.nodeItem;
            return (*this);
        }

		Key nodeKey;
		T nodeItem;
	} TreeMapNode;

public:
    // comparator : returns (l#r = +1), (r#l = -1), (l==r = 0)
    //              '#' may represent any ordering relation you wish (<,>,<=,>=,...).
    typedef Int (*Comparator)( const Key & rLeft, const Key & rRight, Void * pUserData );

    // nested Iterator
    class Iterator
    {
    public:
        static const Iterator Null;

        Iterator():m_itNode() {
            // nothing to do
        }
        Iterator( const Iterator & rhs ):m_itNode(rhs.m_itNode) {
            // nothing to do
        }
	    ~Iterator() {
            // nothing to do
        }

	    inline Iterator & operator=( const Iterator & rhs ) {
            m_itNode = rhs.m_itNode;
            return (*this);
        }

	    inline Bool operator==( const Iterator & rhs ) const {
            return ( m_itNode == rhs.m_itNode);
        }
	    inline Bool operator!=( const Iterator & rhs ) const {
            return ( m_itNode != rhs.m_itNode);
        }

	    inline Iterator & operator++() {
            Assert( !( m_itNode.IsNull() ) );
            ++m_itNode;
            return (*this);
        }
	    inline Iterator & operator--() {
            Assert( !( m_itNode.IsNull() ) );
            --m_itNode;
            return (*this);
        }
	    inline Iterator operator++(int) {
            Assert( !( m_itNode.IsNull() ) );
            return Iterator( m_itNode++ );
        }
	    inline Iterator operator--(int) {
            Assert( !( m_itNode.IsNull() ) );
            return Iterator( m_itNode-- );
        }

	    inline Bool IsNull() const {
            return m_itNode.IsNull();
        }
	    inline Iterator Next() const {
            Assert( !( m_itNode.IsNull() ) );
            return Iterator( m_itNode.Next() );
        }
	    inline Iterator Previous() const {
            Assert( !( m_itNode.IsNull() ) );
            return Iterator( m_itNode.Previous() );
        }

        inline const Key & GetKey() const {
            Assert( !( m_itNode.IsNull() ) );
            return m_itNode.Get().nodeKey;
        }
        inline const Key * GetKeyPtr() const {
            Assert( !( m_itNode.IsNull() ) );
            return &( m_itNode.GetPtr()->nodeKey );
        }

        inline T & GetItem() const {
            Assert( !( m_itNode.IsNull() ) );
            return m_itNode.Get().nodeItem;
        }
        inline T * GetItemPtr() const {
            Assert( !( m_itNode.IsNull() ) );
            return &( m_itNode.GetPtr()->nodeItem );
        }

    private:
	    friend class TreeMap<Key,T>;
        Iterator( typename RedBlackTree<TreeMapNode>::Iterator & itNode ) {
            m_itNode = itNode;
        }

	    typename RedBlackTree<TreeMapNode>::Iterator m_itNode;
    };

public:
    // constructors
	TreeMap();
	virtual ~TreeMap();

    // delayed construction
    inline virtual Void UseMemoryContext( MemoryContextID idMemoryContext, const GChar * strAllocatorName );

    inline Void SetComparator( Comparator pfCompare, Void * pUserData = NULL );

    // common minimal methods
    inline virtual Void Create();
    inline virtual Void Destroy();

    inline virtual UInt MemorySize() const;
    inline virtual UInt Count() const;

    inline virtual Void Clear();

    // methods
    T & operator[]( const Key & rhs );
	const T & operator[]( const Key & rhs ) const;

    inline Iterator Begin() const;
	inline Iterator End() const;
	Iterator Get( const Key & rhs ) const;

    inline Bool Contains( const Key & rKey ) const;

	Bool Insert( const Key & rKey, const T & rItem, Iterator * outItem = NULL );
	Bool Remove( const Key & rKey, T * outItem = NULL );

private:
    // comparator function
    inline static Int _NodeCompare( const TreeMapNode & rLeft, const TreeMapNode & rRight, Void * pUserData );

    // Data
	Comparator m_pfCompare;
    Void * m_pUserData;

    RedBlackTree<TreeMapNode> m_rbTreeMap;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "TreeMap.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_DATASTRUCT_TABLE_TREEMAP_H

