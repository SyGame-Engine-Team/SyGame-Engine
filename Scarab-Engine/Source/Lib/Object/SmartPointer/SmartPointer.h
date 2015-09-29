/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Object/SmartPointer/SmartPointer.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Smart-Pointers & Garbage collection using reference counting.
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
#ifndef SCARAB_LIB_OBJECT_SMARTPOINTER_SMARTPOINTER_H
#define SCARAB_LIB_OBJECT_SMARTPOINTER_SMARTPOINTER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../ThirdParty/System/Platform.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
typedef UInt GarbageCollectorMode;
#define GCMODE_PARANOID    0xff // collect each time a pointer goes out of scope
#define GCMODE_STRICT      0    // collect each time a RefCount reaches 0, default
#define GCMODE_LISTSIZE    1    // collect each time the list reaches max size, defined below
#define GCMODE_UNREFCOUNT  2    // collect each time max count pointers have gone out of scope
#define GCMODE_MEMORYUSAGE 4    // collect each time max size bytes are in use

///////////////////////////////////////////////////////////////////////////////////
// The _GCInfo class
template<typename T>
class _GCInfo
{
public:
	_GCInfo(T * pAddress, UInt iArraySize = 0);
	~_GCInfo();

	inline Bool operator==(const _GCInfo<T> & rhs) const;

	T * m_pAddress;
	UInt m_iRefCount;
	Bool m_bIsArray;
	UInt m_iArraySize;

	_GCInfo<T> *m_pNext, *m_pPrev;
};

///////////////////////////////////////////////////////////////////////////////////
// The _GCList class
template<typename T>
class _GCList
{
public:
	_GCList();
	~_GCList();

	inline _GCInfo<T> * Begin() const;
	inline UInt GetSize() const;
	inline UInt GetMemoryUsage() const;

	Void PushFront( _GCInfo<T> * pNewNode );
	Void Remove( _GCInfo<T> * pNode );
	_GCInfo<T> * Find( T * pAddress );

private:
	_GCInfo<T> * m_pHead;
	UInt m_iSize;
	UInt m_iMemoryUsed;
};

///////////////////////////////////////////////////////////////////////////////////
// The GCPointer class
template<typename T, UInt ARRAYSIZE = 0>
class GCPointer
{
public:
	// constructors & destructor
	GCPointer(T * pAddress = NULL);
	GCPointer(const GCPointer<T,ARRAYSIZE> & rhs);
	~GCPointer();

	// operators
	T * operator=(T * rhs);
	GCPointer<T,ARRAYSIZE> & operator=(const GCPointer<T,ARRAYSIZE> & rhs);
	inline T & operator*() const;
	inline T * operator->() const;
	inline T & operator[](UInt i) const;
	inline operator T*() const;

	// getters & setters
	inline static GarbageCollectorMode GetMode();
    inline static UInt GetMaxListSize();
    inline static UInt GetMaxUnrefs();
    inline static UInt GetMaxMemoryUse();
	inline static Void SetMode(GarbageCollectorMode iMode);
	inline static Void SetMaxListSize(UInt iMaxListSize);
	inline static Void SetMaxUnrefs(UInt iMaxUnrefs);
	inline static Void SetMaxMemoryUse(UInt iMaxMemoryUse);

	// methods
	static Bool CollectGarbage();
	static Void CollectRemaining(); // free all

private:
    static GarbageCollectorMode sm_iGCMode;
	static _GCList<T> sm_listGC;
	static UInt sm_iGCUnrefCount;

	static UInt sm_iGCMaxListSize;
	static UInt sm_iGCMaxUnref;
	static UInt sm_iGCMaxMemoryUse;

	T * m_pAddress;
	Bool m_bIsArray;
	UInt m_iArraySize;
};

// static initializations
template<typename T, UInt ARRAYSIZE>
GarbageCollectorMode GCPointer<T,ARRAYSIZE>::sm_iGCMode = GCMODE_STRICT;
template<typename T, UInt ARRAYSIZE>
_GCList<T> GCPointer<T,ARRAYSIZE>::sm_listGC = _GCList<T>();
template<typename T, UInt ARRAYSIZE>
UInt GCPointer<T,ARRAYSIZE>::sm_iGCUnrefCount = 0;

template<typename T, UInt ARRAYSIZE>
UInt GCPointer<T,ARRAYSIZE>::sm_iGCMaxListSize = 1024;
template<typename T, UInt ARRAYSIZE>
UInt GCPointer<T,ARRAYSIZE>::sm_iGCMaxUnref = 64;
template<typename T, UInt ARRAYSIZE>
UInt GCPointer<T,ARRAYSIZE>::sm_iGCMaxMemoryUse = 4194304;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "SmartPointer.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_OBJECT_SMARTPOINTER_SMARTPOINTER_H

