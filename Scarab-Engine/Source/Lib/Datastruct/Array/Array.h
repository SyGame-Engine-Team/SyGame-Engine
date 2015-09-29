/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Datastruct/Array/Array.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Array template definition.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : _Resize, _ShiftExtend and _ShiftDiscard can be generalized and
//              used as a basis to implement lefty, righty and balanced arrays
//              in a much more elegant and generalized way, abstracting the
//              index layout and adding _ResizeShiftExtend()/_ResizeShiftDiscard().
//              Unfortunately this was too hard to maintain and update for
//              very little use so I removed it all, back to simpler things.
//              The point was mainly doing as much operations "onthefly" as
//              possible, especially during resizes.
//              See Old files "_DenseArray.*", "_LeftyArray.*",
//                            "_RightyArray.*" and "_BalancedArray.*"
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_LIB_DATASTRUCT_ARRAY_ARRAY_H
#define SCARAB_LIB_DATASTRUCT_ARRAY_ARRAY_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Datastruct.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define ARRAY_DEFAULT_PAGESIZE 256 // MUST be power of 2

/////////////////////////////////////////////////////////////////////////////////
// The Array template
template<typename T>
class Array : public Datastruct
{
public:
    // comparator : returns (l#r = +1), (r#l = -1), (l==r = 0)
    //              '#' may represent any ordering relation you wish (<,>,<=,>=,...).
    typedef Int (*Comparator)( const T & rLeft, const T & rRight );

	// constructors
	Array( UInt iPageSize = ARRAY_DEFAULT_PAGESIZE );
	virtual ~Array();

    // common minimal methods
    virtual Void Create();
    virtual Void Destroy();

    inline virtual UInt MemorySize() const;
    inline virtual UInt Count() const;

    virtual Void Clear();

    // operators
        // casting
    inline operator T*();
    inline operator const T*() const;
        // indexing
    inline T & operator[]( Int i );
	inline const T & operator[]( Int i ) const;
    inline T & operator[]( UInt i );
	inline const T & operator[]( UInt i ) const;
        // affectation
    Array<T> & operator=( const Array<T> & rhs );
    Void Affect( const T * arrItems, UInt iLength );

    // sizing methods
        // Disables shrinking automatically
    Bool EnsureCapacity( UInt iRequiredSize, Bool bShrinkTo = false );
        // Shrink when free space >=( pagesize + fact ), with fact in [0;pagesize]
    Void SetShrinking( Bool bEnable, UInt iShrinkFactor = INVALID_OFFSET );

    // insert methods
    Void Push( const T & rItem );
    Void Push( const T * arrItems, UInt iCount );
    Void Push( const Array<T> & rArray, UInt iOffset = 0, UInt iCount = INVALID_OFFSET );

    Void Unshift( const T & rItem );
    Void Unshift( const T * arrItems, UInt iCount );
    Void Unshift( const Array<T> & rArray, UInt iOffset = 0, UInt iCount = INVALID_OFFSET );

    Void Insert( UInt iAt, const T & rItem );
    Void Insert( UInt iAt, const T * arrItems, UInt iCount );
    Void Insert( UInt iAt, const Array<T> & rArray, UInt iOffset = 0, UInt iCount = INVALID_OFFSET );

    // remove methods
    Void Pop( T & outItem );
    Void Pop( T * outItems, UInt iCount );
    Void Pop( Array<T> & outArray, UInt iCount );

    Void Shift( T & outItem );
    Void Shift( T * outItems, UInt iCount );
    Void Shift( Array<T> & outArray, UInt iCount );

	Void Remove( UInt iAt, T & outItem );
	Void Remove( UInt iAt, T * outItems, UInt iCount );
    Void Remove( UInt iAt, Array<T> & outArray, UInt iCount );

    // searching & sorting methods
    UInt Search( const T & rItem, UInt iAt = 0, UInt iCount = INVALID_OFFSET ) const;
	UInt Search( Comparator pfCompare, const T & rItem, UInt iAt = 0, UInt iCount = INVALID_OFFSET ) const;
	Void QuickSort( Comparator pfCompare, UInt iAt = 0, UInt iCount = INVALID_OFFSET );

private:
    // Helpers
    inline UInt _AlignSize( UInt iSize ) const;
    Bool _ArrayResize( UInt iAlignedSize, Bool bKeepData );

    inline Bool _ArrayGrow( UInt iGrowCount );
    inline Void _ArrayShrink();
    inline Void _ShiftExtend( UInt iShift, UInt iSplitOffset );
    inline Void _ShiftDiscard( UInt iShift, UInt iSplitOffset );

    // Data
    UInt m_iPageSize;
    UInt m_iShrinkFactor;

    UInt m_iBufferSize;
	T * m_arrBuffer;

    UInt m_iNextFree;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Array.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_DATASTRUCT_ARRAY_ARRAY_H
