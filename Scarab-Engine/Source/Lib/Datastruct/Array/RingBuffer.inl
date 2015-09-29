/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Datastruct/Array/RingBuffer.inl
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
// Known Bugs :
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// RingBuffer implementation
template<typename T>
RingBuffer<T>::RingBuffer( UInt iBufferSize ):
    Datastruct()
{
    Assert( iBufferSize > 0 );

    m_iBufferSize = iBufferSize;
    m_arrBuffer = NULL;

    m_iCount = 0;
	m_iRead = 0;
	m_iWrite = 0;
}
template<typename T>
RingBuffer<T>::~RingBuffer()
{
	// nothing to do
    // Call Destroy explicitly
}

template<typename T>
Void RingBuffer<T>::Create()
{
    Assert( m_arrBuffer == NULL );

    _SelectMemory();
    m_arrBuffer = New T[m_iBufferSize];
    _UnSelectMemory();

    m_iCount = 0;
	m_iRead = 0;
	m_iWrite = 0;
}
template<typename T>
Void RingBuffer<T>::Destroy()
{
    Assert( m_arrBuffer != NULL );

    _SelectMemory();
    DeleteA( m_arrBuffer );
    m_arrBuffer = NULL;
    _UnSelectMemory();

    m_iCount = 0;
	m_iRead = 0;
	m_iWrite = 0;
}

template<typename T>
inline UInt RingBuffer<T>::MemorySize() const {
    if ( m_arrBuffer == NULL )
        return 0;
    return ( m_iBufferSize * sizeof(T) );
}
template<typename T>
inline UInt RingBuffer<T>::Count() const {
    Assert( m_arrBuffer != NULL );
    return m_iCount;
}

template<typename T>
inline Void RingBuffer<T>::Clear() {
    Assert( m_arrBuffer != NULL );
    m_iCount = 0;
    m_iRead = 0;
    m_iWrite = 0;
}

template<typename T>
inline T & RingBuffer<T>::operator[]( Int i ) {
    Assert( m_arrBuffer != NULL );
    Assert( i >= 0 && i < m_iCount );
    return m_arrBuffer[ (m_iRead + i) % m_iBufferSize ];
}
template<typename T>
inline const T & RingBuffer<T>::operator[]( Int i ) const {
    Assert( m_arrBuffer != NULL );
    Assert( i >= 0 && i < m_iCount );
    return m_arrBuffer[ (m_iRead + i) % m_iBufferSize ];
}
template<typename T>
inline T & RingBuffer<T>::operator[]( UInt i ) {
    Assert( m_arrBuffer != NULL );
    Assert( i < m_iCount );
    return m_arrBuffer[ (m_iRead + i) % m_iBufferSize ];
}
template<typename T>
inline const T & RingBuffer<T>::operator[]( UInt i ) const {
    Assert( m_arrBuffer != NULL );
    Assert( i < m_iCount );
    return m_arrBuffer[ (m_iRead + i) % m_iBufferSize ];
}

template<typename T>
inline Bool RingBuffer<T>::IsFull() const {
    Assert( m_arrBuffer != NULL );
    return ( m_iCount == m_iBufferSize );
}

template<typename T>
Void RingBuffer<T>::Push( const T & rItem )
{
    Assert( m_arrBuffer != NULL );
    Assert( m_iCount < m_iBufferSize );

    m_arrBuffer[m_iWrite] = rItem;
    m_iWrite = ( m_iWrite + 1 ) % m_iBufferSize;
	++m_iCount;
}
template<typename T>
Void RingBuffer<T>::Unshift( const T & rItem )
{
    Assert( m_arrBuffer != NULL );
    Assert( m_iCount < m_iBufferSize );

    m_iRead = ( m_iRead + m_iBufferSize - 1 ) % m_iBufferSize;
    m_arrBuffer[m_iRead] = rItem;
	++m_iCount;
}

template<typename T>
Void RingBuffer<T>::Pop( T & rOut )
{
    Assert( m_arrBuffer != NULL );
	Assert( m_iCount > 0 );

    m_iWrite = ( m_iWrite + m_iBufferSize - 1 ) % m_iBufferSize;
    rOut = m_arrBuffer[m_iWrite];
	--m_iCount;
}
template<typename T>
Void RingBuffer<T>::Shift( T & rOut )
{
    Assert( m_arrBuffer != NULL );
	Assert( m_iCount > 0 );

    rOut = m_arrBuffer[m_iRead];
    m_iRead = ( m_iRead + 1 ) % m_iBufferSize;
	--m_iCount;
}

template<typename T>
Void RingBuffer<T>::PeekNextRead( T & rOut ) const
{
    Assert( m_arrBuffer != NULL );
    Assert( m_iCount > 0 );

	rOut = m_arrBuffer[m_iRead];
}
template<typename T>
Void RingBuffer<T>::PeekLastWrite( T & rOut ) const
{
    Assert( m_arrBuffer != NULL );
    Assert( m_iCount > 0 );

    UInt iWrite = ( m_iWrite + m_iBufferSize - 1 ) % m_iBufferSize;
	rOut = m_arrBuffer[iWrite];
}

template<typename T>
Void RingBuffer<T>::DiscardReads( UInt iCount )
{
    Assert( m_arrBuffer != NULL );
    Assert( m_iCount >= iCount );

    m_iRead = ( m_iRead + iCount ) % m_iBufferSize;
	m_iCount -= iCount;
}
template<typename T>
Void RingBuffer<T>::DiscardWrites( UInt iCount )
{
    Assert( m_arrBuffer != NULL );
    Assert( m_iCount >= iCount );

    m_iWrite = ( m_iWrite + m_iBufferSize - iCount ) % m_iBufferSize;
	m_iCount -= iCount;
}
