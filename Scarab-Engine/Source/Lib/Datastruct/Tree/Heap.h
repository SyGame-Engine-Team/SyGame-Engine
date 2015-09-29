/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Datastruct/Tree/Heap.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : An Heap tree is a lightweight version of an optimised BST ...
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
#ifndef SCARAB_LIB_DATASTRUCT_TREE_HEAP_H
#define SCARAB_LIB_DATASTRUCT_TREE_HEAP_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Datastruct.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define HEAP_DEFAULT_PAGESIZE 256

/////////////////////////////////////////////////////////////////////////////////
// The Heap template
template<typename T>
class Heap : public Datastruct
{
public:
    // comparator : returns (l#r = +1), (r#l = -1), (l==r = 0)
    //              '#' may represent any ordering relation you wish (<,>,<=,>=,...).
    typedef Int (*Comparator)( const T & rLeft, const T & rRight );

    // constructors
	Heap( UInt iPageSize = HEAP_DEFAULT_PAGESIZE );
	virtual ~Heap();

    // delayed construction
    inline Void SetComparator( Comparator pfCompare );

    // common minimal methods
    virtual Void Create();
    virtual Void Destroy();

    inline virtual UInt MemorySize() const;
    inline virtual UInt Count() const;

    inline virtual Void Clear();

    // methods
	T * Merge( const T & rItem );
	Void Extract( T & rOut, Bool bRemove = true );
	Void Discard();

private:
    // Helpers
    inline UInt _AlignSize( UInt iSize ) const;
    Bool _ArrayResize( UInt iAlignedSize, Bool bKeepData );
    inline Bool _ArrayGrow( UInt iGrowCount );
    inline Void _ArrayShrink();

    T * _rec_PercolateUp( UInt iNode );
	Void _rec_PercolateDown( UInt iNode );

    // Data
    UInt m_iPageSize;
    Comparator m_pfCompare;

	UInt m_iBufferSize;
    T * m_arrBuffer; // Root at offset 1 for convenience

    UInt m_iHeapSize;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Heap.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_DATASTRUCT_TREE_HEAP_H
