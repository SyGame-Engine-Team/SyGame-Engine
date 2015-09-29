/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Datastruct/Array/RingBuffer.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : RingBuffer template definition.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : Beware to match ( BUFFER_SIZE * sizeof(T) ) with pre-allocated
//              size when using WrappedArray mechanism here.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_LIB_DATASTRUCT_ARRAY_RINGBUFFER_H
#define SCARAB_LIB_DATASTRUCT_ARRAY_RINGBUFFER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Datastruct.h"

///////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define RINGBUFFER_DEFAULT_SIZE 256

/////////////////////////////////////////////////////////////////////////////////
// The RingBuffer template
template<typename T>
class RingBuffer : public Datastruct
{
public:
    // constructors
	RingBuffer( UInt iBufferSize = RINGBUFFER_DEFAULT_SIZE );
	virtual ~RingBuffer();

    // common minimal methods
    virtual Void Create();
    virtual Void Destroy();

    inline virtual UInt MemorySize() const;
    inline virtual UInt Count() const;

    inline virtual Void Clear();

    // operators
        // indexing
    inline T & operator[]( Int i );
	inline const T & operator[]( Int i ) const;
    inline T & operator[]( UInt i );
	inline const T & operator[]( UInt i ) const;

	// getters
    inline Bool IsFull() const;

	// methods
	Void Push( const T & rItem );
    Void Unshift( const T & rItem );

	Void Pop( T & rOut );
	Void Shift( T & rOut );

	Void PeekNextRead( T & rOut ) const;
	Void PeekLastWrite( T & rOut ) const;

	Void DiscardReads( UInt iCount = 1 );
	Void DiscardWrites( UInt iCount = 1 );

private:
    UInt m_iBufferSize;
    T * m_arrBuffer;

    UInt m_iCount;
    UInt m_iRead, m_iWrite;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "RingBuffer.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_DATASTRUCT_ARRAY_RINGBUFFER_H
