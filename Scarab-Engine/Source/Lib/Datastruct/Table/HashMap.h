/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Datastruct/Table/HashMap.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : The standard HashMap for generic indexing ...
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
#ifndef SCARAB_LIB_DATASTRUCT_TABLE_HASHMAP_H
#define SCARAB_LIB_DATASTRUCT_TABLE_HASHMAP_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Datastruct.h"

#include "../../Math/Hash/Hashing.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define HASHMAP_DEFAULT_PAGESIZE 1024 // Optimal load = 70-80%

/////////////////////////////////////////////////////////////////////////////////
// The HashMap template
template<typename Key, typename T>
class HashMap : public Datastruct
{
private:
    // nested node
	typedef struct _hashmap_node
	{
        Bool bAllocated;
        struct _hashmap_node * pPrev;
        struct _hashmap_node * pNext;
		Key nodeKey;
		T Item;
	} HashMapNode;

public:
    // nested Iterator
    class Iterator
    {
    public:
        static const Iterator Null;

	    Iterator()                       { m_pNode = NULL; }
	    Iterator( const Iterator & rhs ) { m_pNode = rhs.m_pNode; }
	    ~Iterator()                      { }

	    inline Iterator & operator=( const Iterator & rhs ) { m_pNode = rhs.m_pNode; return (*this); }

	    inline Bool operator==( const Iterator & rhs ) const { return (m_pNode == rhs.m_pNode); }
	    inline Bool operator!=( const Iterator & rhs ) const { return (m_pNode != rhs.m_pNode); }

	    inline Iterator & operator++() { Assert(m_pNode != NULL); m_pNode = m_pNode->pNext; return (*this); }
	    inline Iterator & operator--() { Assert(m_pNode != NULL); m_pNode = m_pNode->pPrev; return (*this); }

	    inline Iterator operator++(int) { Assert(m_pNode != NULL); Iterator it(m_pNode); m_pNode = m_pNode->pNext; return it; }
	    inline Iterator operator--(int) { Assert(m_pNode != NULL); Iterator it(m_pNode); m_pNode = m_pNode->pPrev; return it; }

	    inline Key & GetKey() { Assert(m_pNode != NULL); return m_pNode->nodeKey; }
	    inline T & GetData()  { Assert(m_pNode != NULL); return m_pNode->Item; }

	    inline Bool IsNull() const       { return (m_pNode == NULL); }
	    inline Bool IsBegin() const      { if (m_pNode == NULL) return false; return (m_pNode->pPrev == NULL); }
	    inline Bool IsEnd() const        { if (m_pNode == NULL) return false; return (m_pNode->pNext == NULL); }
	    inline Iterator Next() const     { Assert(m_pNode != NULL); return Iterator(m_pNode->pNext); }
	    inline Iterator Previous() const { Assert(m_pNode != NULL); return Iterator(m_pNode->pPrev); }

    private:
	    friend class HashMap<Key,T>;
	    Iterator( HashMapNode * pNode ) { m_pNode = pNode; }

	    HashMapNode * m_pNode;
    };

public:
    // constructors
	HashMap( UInt iPageSize = HASHMAP_DEFAULT_PAGESIZE );
	virtual ~HashMap();

    // common minimal methods
    virtual Void Create();
    virtual Void Destroy();

    inline virtual UInt MemorySize() const;
    inline virtual UInt Count() const;

    virtual Void Clear();

    // methods
    T & operator[]( const Key & rhs );
	const T & operator[]( const Key & rhs ) const;

	inline Bool Contains( const Key & rKey ) const;
    inline Iterator Begin() const;
	inline Iterator End() const;

	Void Insert( const Key & rKey, const T & rData );
	Void Remove( const Key & rKey );

private:
    // Helpers
	UInt _Hash( const Key & rKey ) const;
	Void _GrowPage();

    // Data
    UInt m_iPageSize;

    UInt m_iTableSize;
	HashMapNode * m_arrTable;

    UInt m_iEntryCount;
    HashMapNode *m_pHead, *m_pEnd;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "HashMap.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_DATASTRUCT_TABLE_HASHMAP_H

