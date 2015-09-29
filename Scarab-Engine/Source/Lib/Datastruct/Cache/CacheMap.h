/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Datastruct/Cache/CacheMap.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : CacheMap template definition.
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
#ifndef SCARAB_LIB_DATASTRUCT_CACHE_CACHEMAP_H
#define SCARAB_LIB_DATASTRUCT_CACHE_CACHEMAP_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Table/TreeMap.h"

///////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define CACHEMAP_DEFAULT_SIZE 256

/////////////////////////////////////////////////////////////////////////////////
// The CacheMap template
template<typename Key, typename T>
class CacheMap : public Datastruct
{
private:
    // nested node
	typedef struct _cachemap_node
	{
		UInt iLastAccess;
		T nodeItem;
	} CacheMapNode;

public:
    // comparator : returns (l#r = +1), (r#l = -1), (l==r = 0)
    //              '#' may represent any ordering relation you wish (<,>,<=,>=,...).
    typedef Int (*Comparator)( const Key & rLeft, const Key & rRight, Void * pUserData );

    // loader : returns data to be cached
    typedef Void (*Loader)( T * outItem, const Key & rKey, Void * pUserData );

    // unloader : destroys cached data
    typedef Void (*Unloader)( const Key & rKey, const T & rItem, Void * pUserData );

public:
    // constructors
	CacheMap( UInt iCacheSize = CACHEMAP_DEFAULT_SIZE );
	virtual ~CacheMap();

    // delayed construction
    inline virtual Void UseMemoryContext( MemoryContextID idMemoryContext, const GChar * strAllocatorName );

    inline Void SetComparator( Comparator pfCompare, Void * pUserData = NULL );
    inline Void SetLoader( Loader pfLoad, Void * pUserData = NULL );
    inline Void SetUnloader( Unloader pfUnload );

    // common minimal methods
    inline virtual Void Create();
    inline virtual Void Destroy();

    inline virtual UInt MemorySize() const;
    inline virtual UInt Count() const;

    inline virtual Void Clear();

	// getters
    inline Bool IsFull() const;

	// methods
    inline Bool Contains( const Key & rKey ) const;

	const T & Query( const Key & rKey );
	const T & Load( const Key & rKey );

    Void Update();
    Void Release();

private:
    Loader m_pfLoad;
    Unloader m_pfUnload;
    Void * m_pUserData;

    UInt m_iCacheSize, m_iMostRecentAccess;
    TreeMap<Key,CacheMapNode> m_mapCache;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "CacheMap.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_DATASTRUCT_CACHE_CACHEMAP_H
