/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Datastruct/Array/Array.inl
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
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Array implementation
template<typename T>
Array<T>::Array( UInt iPageSize ):
    Datastruct()
{
    Assert( iPageSize > 0 );

    m_iPageSize = iPageSize;
    m_iShrinkFactor = ( m_iPageSize >> 1 );

    m_iBufferSize = 0;
    m_arrBuffer = NULL;

    m_iNextFree = INVALID_OFFSET;
}
template<typename T>
Array<T>::~Array()
{
    // nothing to do
    // Call Destroy explicitly
}

template<typename T>
Void Array<T>::Create()
{
    Assert( m_arrBuffer == NULL );

    m_iBufferSize = m_iPageSize;

    _SelectMemory();
    m_arrBuffer = New T[m_iBufferSize];
    _UnSelectMemory();

    m_iNextFree = 0;
}
template<typename T>
Void Array<T>::Destroy()
{
    Assert( m_arrBuffer != NULL );

    m_iBufferSize = 0;

    _SelectMemory();
    DeleteA( m_arrBuffer );
    m_arrBuffer = NULL;
    _UnSelectMemory();

	m_iNextFree = INVALID_OFFSET;

}

template<typename T>
inline UInt Array<T>::MemorySize() const {
    return ( m_iBufferSize * sizeof(T) );
}
template<typename T>
inline UInt Array<T>::Count() const {
    Assert( m_arrBuffer != NULL );
    return m_iNextFree;
}

template<typename T>
Void Array<T>::Clear()
{
    Assert( m_arrBuffer != NULL );

    if ( m_iShrinkFactor != INVALID_OFFSET )
        _ArrayResize( m_iPageSize, false );

    m_iNextFree = 0;
}

template<typename T>
inline Array<T>::operator T*() {
    Assert( m_arrBuffer != NULL );
    return m_arrBuffer;
}
template<typename T>
inline Array<T>::operator const T*() const {
    Assert( m_arrBuffer != NULL );
    return m_arrBuffer;
}

template<typename T>
inline T & Array<T>::operator[]( Int i ) {
    Assert( m_arrBuffer != NULL );
    Assert( i >= 0 && i < (Int)m_iNextFree );
    return m_arrBuffer[i];
}
template<typename T>
inline const T & Array<T>::operator[]( Int i ) const {
    Assert( m_arrBuffer != NULL );
    Assert( i >= 0 && i < (Int)m_iNextFree );
    return m_arrBuffer[i];
}
template<typename T>
inline T & Array<T>::operator[]( UInt i ) {
    Assert( m_arrBuffer != NULL );
    Assert( i < m_iNextFree );
    return m_arrBuffer[i];
}
template<typename T>
inline const T & Array<T>::operator[]( UInt i ) const {
    Assert( m_arrBuffer != NULL );
    Assert( i < m_iNextFree );
    return m_arrBuffer[i];
}

template<typename T>
Array<T> & Array<T>::operator=( const Array<T> & rhs )
{
    Assert( m_arrBuffer != NULL );
    Assert( rhs.m_arrBuffer != NULL );

    Bool bOk = EnsureCapacity( rhs.m_iNextFree );
    if (bOk) {
        MemCopy( m_arrBuffer, rhs.m_arrBuffer, rhs.m_iNextFree * sizeof(T) );
        m_iNextFree = rhs.m_iNextFree;
    } else {
        // Can't resize, truncate input
        UInt iMinSize = Min<UInt>( rhs.m_iNextFree, m_iBufferSize );
        MemCopy( m_arrBuffer, rhs.m_arrBuffer, iMinSize * sizeof(T) );
        m_iNextFree = iMinSize;
    }

    return (*this);
}
template<typename T>
Void Array<T>::Affect( const T * arrItems, UInt iLength )
{
    Assert( m_arrBuffer != NULL );
    Assert( arrItems != NULL );

    Bool bOk = EnsureCapacity( iLength );
    if (bOk) {
        MemCopy( m_arrBuffer, arrItems, iLength * sizeof(T) );
        m_iNextFree = iLength;
    } else {
        // Can't resize, truncate input
        UInt iMinSize = Min<UInt>( iLength, m_iBufferSize );
        MemCopy( m_arrBuffer, arrItems, iMinSize * sizeof(T) );
        m_iNextFree = iMinSize;
    }
}

template<typename T>
Bool Array<T>::EnsureCapacity( UInt iRequiredSize, Bool bShrinkTo )
{
    Assert( m_arrBuffer != NULL );

    // Disable Shrinking
    m_iShrinkFactor = INVALID_OFFSET;

    // Aligned size size
    UInt iAlignedSize = _AlignSize( iRequiredSize );
    if ( m_iBufferSize == iAlignedSize )
        return true;

    // Shrink case
    if ( m_iBufferSize > iAlignedSize ) {
        if ( !bShrinkTo )
            return true;
        return _ArrayResize( iAlignedSize, true ); 
    }

    // Grow case
    return _ArrayResize( iAlignedSize, true );
}
template<typename T>
Void Array<T>::SetShrinking( Bool bEnable, UInt iShrinkFactor )
{
    Assert( m_arrBuffer != NULL );

    // Setup shrink factor
    if ( bEnable ) {
        m_iShrinkFactor = ( m_iPageSize >> 1 );
        if ( iShrinkFactor != INVALID_OFFSET ) {
            Assert( iShrinkFactor <= m_iPageSize );
            m_iShrinkFactor = iShrinkFactor;
        }
    } else
        m_iShrinkFactor = INVALID_OFFSET;
}

template<typename T>
Void Array<T>::Push( const T & rItem )
{
    Assert( m_arrBuffer != NULL );

    Bool bOk = _ArrayGrow( 1 );
    Assert(bOk);
    m_arrBuffer[m_iNextFree] = rItem;
    ++m_iNextFree;
}
template<typename T>
Void Array<T>::Push( const T * arrItems, UInt iCount )
{
    Assert( m_arrBuffer != NULL );

    Bool bOk = _ArrayGrow( iCount );
    Assert(bOk);
    MemCopy( m_arrBuffer + m_iNextFree, arrItems, iCount * sizeof(T) );
    m_iNextFree += iCount;
}
template<typename T>
Void Array<T>::Push( const Array<T> & rArray, UInt iOffset, UInt iCount )
{
    Assert( m_arrBuffer != NULL );

    UInt iLength = rArray.m_iNextFree;
    Assert( iOffset <= iLength );
    if ( iCount == INVALID_OFFSET )
        iCount = iLength - iOffset;
    Assert( iOffset + iCount <= iLength );

    Push( rArray.m_arrBuffer + iOffset, iCount );
}

template<typename T>
Void Array<T>::Unshift( const T & rItem )
{
    Assert( m_arrBuffer != NULL );

    Bool bOk = _ArrayGrow( 1 );
    Assert(bOk);
    _ShiftExtend( 1, 0 );
    m_arrBuffer[0] = rItem;
}
template<typename T>
Void Array<T>::Unshift( const T * arrItems, UInt iCount )
{
    Assert( m_arrBuffer != NULL );

    Bool bOk = _ArrayGrow( iCount );
    Assert(bOk);
    _ShiftExtend( iCount, 0 );
    MemCopy( m_arrBuffer, arrItems, iCount * sizeof(T) );
}
template<typename T>
Void Array<T>::Unshift( const Array<T> & rArray, UInt iOffset, UInt iCount )
{
    Assert( m_arrBuffer != NULL );

    UInt iLength = rArray.m_iNextFree;
    Assert( iOffset <= iLength );
    if ( iCount == INVALID_OFFSET )
        iCount = iLength - iOffset;
    Assert( iOffset + iCount <= iLength );

    Unshift( rArray.m_arrBuffer + iOffset, iCount );
}

template<typename T>
Void Array<T>::Insert( UInt iAt, const T & rItem )
{
    Assert( m_arrBuffer != NULL );
    Assert( iAt <= m_iNextFree );

    Bool bOk = _ArrayGrow( 1 );
    Assert(bOk);
    _ShiftExtend( 1, iAt );
    m_arrBuffer[iAt] = rItem;
}
template<typename T>
Void Array<T>::Insert( UInt iAt, const T * arrItems, UInt iCount )
{
    Assert( m_arrBuffer != NULL );
    Assert( iAt <= m_iNextFree );

    Bool bOk = _ArrayGrow( iCount );
    Assert(bOk);
    _ShiftExtend( iCount, iAt );
    MemCopy( m_arrBuffer + iAt, arrItems, iCount * sizeof(T) );
}
template<typename T>
Void Array<T>::Insert( UInt iAt, const Array<T> & rArray, UInt iOffset, UInt iCount )
{
    Assert( m_arrBuffer != NULL );

    UInt iLength = rArray.m_iNextFree;
    Assert( iOffset <= iLength );
    if ( iCount == INVALID_OFFSET )
        iCount = iLength - iOffset;
    Assert( iOffset + iCount <= iLength );

    Insert( iAt, rArray.m_arrBuffer + iOffset, iCount );
}

template<typename T>
Void Array<T>::Pop( T & outItem )
{
    Assert( m_arrBuffer != NULL );
    Assert( m_iNextFree >= 1 );

    outItem = m_arrBuffer[m_iNextFree - 1];
    --m_iNextFree;
    _ArrayShrink();
}
template<typename T>
Void Array<T>::Pop( T * outItems, UInt iCount )
{
    Assert( m_arrBuffer != NULL );
    Assert( m_iNextFree >= iCount );

    if ( outItems != NULL )
        MemCopy( outItems, m_arrBuffer + m_iNextFree - iCount, iCount * sizeof(T) );
    m_iNextFree -= iCount;
    _ArrayShrink();
}
template<typename T>
Void Array<T>::Pop( Array<T> & outArray, UInt iCount )
{
    Assert( m_arrBuffer != NULL );

    Bool bOk = outArray.EnsureCapacity( iCount );
    if ( bOk )
        Pop( outArray.m_arrBuffer, iCount );
    else
        Pop( outArray.m_arrBuffer, outArray.m_iBufferSize );
}

template<typename T>
Void Array<T>::Shift( T & outItem )
{
    Assert( m_arrBuffer != NULL );
    Assert( m_iNextFree >= 1 );

    outItem = m_arrBuffer[0];
    _ShiftDiscard( 1, 0 );
    _ArrayShrink();
}
template<typename T>
Void Array<T>::Shift( T * outItems, UInt iCount )
{
    Assert( m_arrBuffer != NULL );
    Assert( m_iNextFree >= iCount );

    if ( outItems != NULL )
        MemCopy( outItems, m_arrBuffer, iCount * sizeof(T) );
    _ShiftDiscard( iCount, 0 );
    _ArrayShrink();
}
template<typename T>
Void Array<T>::Shift( Array<T> & outArray, UInt iCount )
{
    Assert( m_arrBuffer != NULL );

    Bool bOk = outArray.EnsureCapacity( iCount );
    if ( bOk )
        Shift( outArray.m_arrBuffer, iCount );
    else
        Shift( outArray.m_arrBuffer, outArray.m_iBufferSize );
}

template<typename T>
Void Array<T>::Remove( UInt iAt, T & outItem )
{
    Assert( m_arrBuffer != NULL );

    Assert( m_iNextFree >= 1 );
    Assert( iAt <= (m_iNextFree - 1) );

    outItem = m_arrBuffer[iAt];
    _ShiftDiscard( 1, iAt );
    _ArrayShrink();
}
template<typename T>
Void Array<T>::Remove( UInt iAt, T * outItems, UInt iCount )
{
    Assert( m_arrBuffer != NULL );

    Assert( m_iNextFree >= iCount );
    Assert( iAt <= (m_iNextFree - iCount) );

    if ( outItems != NULL )
        MemCopy( outItems, m_arrBuffer + iAt, iCount * sizeof(T) );
    _ShiftDiscard( iCount, iAt );
    _ArrayShrink();
}
template<typename T>
Void Array<T>::Remove( UInt iAt, Array<T> & outArray, UInt iCount )
{
    Assert( m_arrBuffer != NULL );

    Bool bOk = outArray.EnsureCapacity( iCount );
    if ( bOk )
        Remove( iAt, outArray.m_arrBuffer, iCount );
    else
        Remove( iAt, outArray.m_arrBuffer, outArray.m_iBufferSize );
}

template<typename T>
UInt Array<T>::Search( const T & rItem, UInt iAt, UInt iCount ) const
{
    Assert( m_arrBuffer != NULL );

    UInt iLength = m_iNextFree;
    Assert( iAt <= iLength );
    if ( iCount == INVALID_OFFSET )
        iCount = iLength - iAt;
    Assert( iAt + iCount <= iLength );

    T * pItem = ( m_arrBuffer + iAt );
    T * pEnd = ( pItem + iCount );
    while( pItem < pEnd ) {
        if ( *pItem == rItem )
            return ( pItem - m_arrBuffer );
        ++pItem;
    }
	return INVALID_OFFSET;
}
template<typename T>
UInt Array<T>::Search( Comparator pfCompare, const T & rItem, UInt iAt, UInt iCount ) const
{
    Assert( m_arrBuffer != NULL );

    UInt iLength = m_iNextFree;
    Assert( iAt <= iLength );
    if ( iCount == INVALID_OFFSET )
        iCount = iLength - iAt;
    Assert( iAt + iCount <= iLength );

    Int iCmp;
    T * pBegin = ( m_arrBuffer + iAt );
    T * pPivot = ( pBegin + (iCount >> 1) );
    while( iCount > 0 ) {
        iCmp = pfCompare( *pPivot, rItem );
        if ( iCmp == 0 )
            return ( pPivot - m_arrBuffer );
        if ( iCmp < 0 )
            iCount = ( pPivot - pBegin );
        else { // ( iCmp > 0 )
            iCount -= ( (pPivot + 1) - pBegin );
            pBegin = pPivot + 1;
        }
        pPivot = ( pBegin + (iCount >> 1) );
    }
    return ( pPivot - m_arrBuffer );
}
template<typename T>
Void Array<T>::QuickSort( Comparator pfCompare, UInt iAt, UInt iCount )
{
    Assert( m_arrBuffer != NULL );

    UInt iLength = m_iNextFree;
    Assert( iAt <= iLength );
    if ( iCount == INVALID_OFFSET )
        iCount = iLength - iAt;
    Assert( iAt + iCount <= iLength );

    static Bool s_bSendLeft = true;

    if ( iCount <= 1 )
        return;

    Int iCmp;
    T * pFirst = ( m_arrBuffer + iAt );
    T * pLast = ( pFirst + (iCount - 1) );
    T * pPivot = pFirst;
    while( pPivot < pLast ) {
        iCmp = pfCompare( *pPivot, *(pPivot + 1) );
        if ( iCmp < 0 ) {
			Swap<T>( pPivot, pPivot + 1 );
			++pPivot;
        } else if ( iCmp > 0 ) {
            if ( pPivot + 1 != pLast )
			    Swap<T>( pPivot + 1, pLast );
			--pLast;
        } else { // ( iCmp == 0 )
            // balance distribution of equal cases to temper some annoying
            // cases like arrays filled with the same value ...
            if ( s_bSendLeft ) {
                //Swap<T>( pPivot, pPivot + 1 );
				++pPivot;
            } else {
				if ( pPivot + 1 != pLast )
			        Swap<T>( pPivot + 1, pLast );
				--pLast;
			}
			s_bSendLeft = !s_bSendLeft;
        }
    }

	UInt iLeft = ( pPivot - pFirst );
	UInt iRight = ( iCount - iLeft - 1 );
	if ( iLeft > 1 )
		QuickSort( pfCompare, iAt, iLeft );
	if ( iRight > 1 )
		QuickSort( pfCompare, ( (pPivot + 1) - m_arrBuffer ), iRight );
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline UInt Array<T>::_AlignSize( UInt iSize ) const {
    if ( iSize < m_iPageSize )
        iSize = m_iPageSize;
    else {
        UInt mod = ( iSize & (m_iPageSize - 1) );
        if ( mod > 0 )
	        iSize += ( m_iPageSize - mod );
    }
    return iSize;
}
template<typename T>
Bool Array<T>::_ArrayResize( UInt iAlignedSize, Bool bKeepData )
{
    if ( m_iBufferSize == iAlignedSize )
        return false;

    _SelectMemory();
    T * pNewBuffer = New T[iAlignedSize];
    _UnSelectMemory();

    if ( bKeepData ) {
        if ( iAlignedSize <= m_iBufferSize )
            MemCopy( pNewBuffer, m_arrBuffer, iAlignedSize * sizeof(T) );
        else
            MemCopy( pNewBuffer, m_arrBuffer, m_iBufferSize * sizeof(T) );
    }

    _SelectMemory();
    DeleteA( m_arrBuffer );
    _UnSelectMemory();

    m_iBufferSize = iAlignedSize;
    m_arrBuffer = pNewBuffer;
    if ( m_iNextFree > m_iBufferSize )
        m_iNextFree = m_iBufferSize;

    return true;
}

template<typename T>
inline Bool Array<T>::_ArrayGrow( UInt iGrowCount ) {
    UInt iNewCount = ( m_iNextFree + iGrowCount );
    if ( iNewCount > m_iBufferSize )
        return _ArrayResize( _AlignSize(iNewCount), true );
    return true;
}
template<typename T>
inline Void Array<T>::_ArrayShrink() {
    UInt iNewCount = m_iNextFree;
    if ( m_iShrinkFactor != INVALID_OFFSET ) {
        if ( (m_iBufferSize - iNewCount) >= (m_iPageSize + m_iShrinkFactor) ) {
            Bool bResized = _ArrayResize( _AlignSize(iNewCount), true );
            if ( !bResized )
                m_iShrinkFactor = INVALID_OFFSET;
        }
    }
}
template<typename T>
inline Void Array<T>::_ShiftExtend( UInt iShift, UInt iSplitOffset ) {
    UInt iOldLength = m_iNextFree; // Shift Right
    MemCopyBackward( m_arrBuffer + iSplitOffset + iShift,
                     m_arrBuffer + iSplitOffset,
                     (iOldLength - iSplitOffset) * sizeof(T) );
    m_iNextFree += iShift;
}
template<typename T>
inline Void Array<T>::_ShiftDiscard( UInt iShift, UInt iSplitOffset ) {
    UInt iNewLength = m_iNextFree - iShift; // Shift Left
    MemCopy( m_arrBuffer + iSplitOffset,
             m_arrBuffer + iSplitOffset + iShift,
             (iNewLength - iSplitOffset) * sizeof(T) );
    m_iNextFree -= iShift;
}


