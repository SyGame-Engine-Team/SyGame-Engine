/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Datastruct/Tree/Heap.inl
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
// Heap implementation
template<typename T>
Heap<T>::Heap( UInt iPageSize ):
    Datastruct()
{
    m_iPageSize = iPageSize;
    m_pfCompare = NULL;

    m_iBufferSize = 0;
	m_arrBuffer = NULL;

    m_iHeapSize = INVALID_OFFSET;
}
template<typename T>
Heap<T>::~Heap()
{
	// nothing to do
    // Call Destroy explicitly
}

template<typename T>
inline Void Heap<T>::SetComparator( Comparator pfCompare ) {
    Assert( m_arrBuffer == NULL );
    m_pfCompare = pfCompare;
}

template<typename T>
virtual Void Heap<T>::Create()
{
    Assert( m_pfCompare != NULL );
    Assert( m_arrBuffer == NULL );

    m_iBufferSize = m_iPageSize;

    _SelectMemory();
	m_arrBuffer = New T[m_iBufferSize];
    _UnSelectMemory();

    m_iHeapSize = 0;
}
template<typename T>
virtual Void Heap<T>::Destroy()
{
    Assert( m_arrBuffer != NULL );

    m_iBufferSize = 0;

    _SelectMemory();
    DeleteA( m_arrBuffer );
	m_arrBuffer = NULL;
    _UnSelectMemory();

    m_iHeapSize = INVALID_OFFSET;
}

template<typename T>
inline UInt Heap<T>::MemorySize() const {
    Assert( m_arrBuffer != NULL );
    return ( m_iBufferSize * sizeof(T) );
}
template<typename T>
inline UInt Heap<T>::Count() const {
    Assert( m_arrBuffer != NULL );
    return m_iHeapSize;
}

template<typename T>
inline Void Heap<T>::Clear() {
    Assert( m_arrBuffer != NULL );
    m_iHeapSize = 0;
}

template<typename T>
T * Heap<T>::Merge( const T & rItem )
{
    Assert( m_arrBuffer != NULL );

	if ( m_iHeapSize == 0 ) {
		m_arrBuffer[1] = rItem;
		m_iHeapSize = 1;
		return ( m_arrBuffer + 1 );
	}

    Bool bOk = _ArrayGrow( 1 );
    Assert( bOk );
	++m_iHeapSize;
	m_arrBuffer[m_iHeapSize] = rItem;
	return _rec_PercolateUp( m_iHeapSize );
}
template<typename T>
Void Heap<T>::Extract( T & rOut, Bool bRemove )
{
    Assert( m_arrBuffer != NULL );
    Assert( m_iHeapSize > 0 );

	rOut = m_arrBuffer[1];
	if ( bRemove ) {
		if ( m_iHeapSize == 1 ) {
			--m_iHeapSize;
			return;
		}

		m_arrBuffer[1] = m_arrBuffer[m_iHeapSize];
		--m_iHeapSize;
		_rec_PercolateDown(1);
        _ArrayShrink();
	}
}
template<typename T>
Void Heap<T>::Discard()
{
    Assert( m_arrBuffer != NULL );
    Assert( m_iHeapSize > 0 );

    if ( m_iHeapSize == 1 ) {
		--m_iHeapSize;
		return;
	}

	m_arrBuffer[1] = m_arrBuffer[m_iHeapSize];
	--m_iHeapSize;
	_rec_PercolateDown(1);
    _ArrayShrink();
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline UInt Heap<T>::_AlignSize( UInt iSize ) const {
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
Bool Heap<T>::_ArrayResize( UInt iAlignedSize, Bool bKeepData )
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

    return true;
}
template<typename T>
inline Bool Heap<T>::_ArrayGrow( UInt iGrowCount ) {
    UInt iNewCount = ( m_iHeapSize + iGrowCount );
    if ( iNewCount > m_iBufferSize )
        return _ArrayResize( _AlignSize(iNewCount), true );
    return true;
}
template<typename T>
inline Void Heap<T>::_ArrayShrink() {
    UInt iNewCount = m_iHeapSize;
    if ( (m_iBufferSize - iNewCount) >= ( m_iPageSize + (m_iPageSize >> 1) ) )
        _ArrayResize( _AlignSize(iNewCount), true );
}

template<typename T>
T * Heap<T>::_rec_PercolateUp( UInt iNode )
{
    if ( iNode == 1 )
        return ( m_arrBuffer + 1 );

    UInt iParent = (iNode >> 1);

    Int cmp = m_pfCompare( m_arrBuffer[iNode], m_arrBuffer[iParent] );
    if ( cmp <= 0 )
        return ( m_arrBuffer + iNode );

    Swap<T>( m_arrBuffer + iNode, m_arrBuffer + iParent );
    return _rec_PercolateUp( iParent );
}
template<typename T>
Void Heap<T>::_rec_PercolateDown( UInt iNode )
{
    if ( iNode > (m_iHeapSize >> 1) )
        return;

    UInt iLeftChild = (iNode << 1);
    UInt iRightChild = iLeftChild + 1;
    UInt iMinChild;

    Int cmp;
    if (iRightChild > m_iHeapSize)
        iMinChild = iLeftChild;
    else {
        cmp = m_pfCompare( m_arrBuffer[iLeftChild], m_arrBuffer[iRightChild] );
        iMinChild = (cmp >= 0) ? iLeftChild : iRightChild;
    }

    cmp = m_pfCompare( m_arrBuffer[iMinChild], m_arrBuffer[iNode] );
    if ( cmp <= 0 )
        return;

    Swap<T>( m_arrBuffer + iMinChild, m_arrBuffer + iNode );
    _rec_PercolateDown( iMinChild );
}

