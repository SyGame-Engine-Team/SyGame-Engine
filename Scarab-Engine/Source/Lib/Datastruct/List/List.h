/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Datastruct/List/List.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : List template definition.
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
#ifndef SCARAB_LIB_DATASTRUCT_LIST_LIST_H
#define SCARAB_LIB_DATASTRUCT_LIST_LIST_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Datastruct.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define LIST_NODEOVERHEADSIZE ( sizeof(Void*) << 1 ) // already aligned

/////////////////////////////////////////////////////////////////////////////////
// The List template
template<typename T>
class List : public Datastruct
{
private:
    // nested node
    typedef struct _list_node
    {
	    struct _list_node * pPrev;
	    struct _list_node * pNext;
        T Item;
    } ListNode;

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

	    inline T & GetData() { Assert(m_pNode != NULL); return m_pNode->Item; }

	    inline Bool IsNull() const       { return (m_pNode == NULL); }
	    inline Bool IsBegin() const      { if (m_pNode == NULL) return false; return (m_pNode->pPrev == NULL); }
	    inline Bool IsEnd() const        { if (m_pNode == NULL) return false; return (m_pNode->pNext == NULL); }
	    inline Iterator Next() const     { Assert(m_pNode != NULL); return Iterator(m_pNode->pNext); }
	    inline Iterator Previous() const { Assert(m_pNode != NULL); return Iterator(m_pNode->pPrev); }

    private:
	    friend class List<T>;
	    Iterator( ListNode * pNode ) { m_pNode = pNode; }

	    ListNode * m_pNode;
    };

public:
	// constructors
	List();
	virtual ~List();

    // common minimal methods
    virtual Void Create();
    virtual Void Destroy();

    inline virtual UInt MemorySize() const;
    inline virtual UInt Count() const;

    virtual Void Clear();

	// methods
    inline Iterator Begin() const;
	inline Iterator End() const;

	Void Push( const T & rItem );
	Void Pop( T & outItem );
	Void Unshift( const T & rItem );
	Void Shift( T & outItem );
	Void Insert( Iterator & iAt, const T & rItem );
	Void Remove( Iterator & iAt, T & outItem );

	//Void Push( const List<T> & rList );
	//Void Pop( UInt iLength, List<T> * outList = NULL );
	//Void Unshift( const List<T> & rList );
	//Void Shift( UInt iLength, List<T> * outList = NULL );
	//Void Insert( Iterator & iAt, const List<T> & rList );
	//Void Remove( Iterator & iAt, UInt iLength, List<T> * outList = NULL );

    Iterator Search( const T & rItem, const Iterator & iAt = Iterator::Null ) const;

private:
	ListNode * m_pHead;
	ListNode * m_pEnd;
	UInt m_iLength;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "List.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_DATASTRUCT_LIST_LIST_H
