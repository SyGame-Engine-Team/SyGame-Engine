/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Resources/GPUBuffer.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU Resource : Buffers
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// GPUBuffer implementation
inline Bool GPUBuffer::IsBound() const {
    return m_pDeviceBuffer->IsCreated();
}

inline Bool GPUBuffer::HasCPUData() const {
    return ( m_arrBuffer != NULL );
}

inline ResourceType GPUBuffer::GetResourceType() const {
    return RESOURCE_TYPE_BUFFER;
}

inline UInt GPUBuffer::GetElementSize() const {
    return m_iElementSize;
}
inline UInt GPUBuffer::GetElementCount() const {
    return m_iElementCount;
}
inline UInt GPUBuffer::GetSize() const {
    return m_iSize;
}

inline const Byte * GPUBuffer::GetData( UInt iIndex ) const {
    Assert( HasCPUData() );
    Assert( iIndex < m_iElementCount );
    return ( m_arrBuffer + (iIndex * m_iElementSize) );
}
inline Byte * GPUBuffer::GetData( UInt iIndex ) {
    Assert( HasCPUData() );
    Assert( iIndex < m_iElementCount );
    return ( m_arrBuffer + (iIndex * m_iElementSize) );
}
inline Void GPUBuffer::SetData( const Void * pData, UInt iIndex, UInt iCount ) {
    Assert( HasCPUData() );
    Assert( iIndex < m_iElementCount );
    if ( iCount == INVALID_OFFSET )
        iCount = ( m_iElementCount - iIndex );
    Assert( (iIndex + iCount) <= m_iElementCount );
    MemCopy( m_arrBuffer + (iIndex * m_iElementSize), pData, (iCount * m_iElementSize) );
}

template<typename T>
inline const T * GPUBuffer::GetElement( UInt iIndex ) const {
    Assert( HasCPUData() );
    Assert( m_iElementSize == sizeof(T) );
    Assert( iIndex < m_iElementCount );
    return ( ((const T *)m_arrBuffer) + iIndex );
}
template<typename T>
inline T * GPUBuffer::GetElement( UInt iIndex ) {
    Assert( HasCPUData() );
    Assert( m_iElementSize == sizeof(T) );
    Assert( iIndex < m_iElementCount );
    return ( ((T*)m_arrBuffer) + iIndex );
}
template<typename T>
inline Void GPUBuffer::SetElement( const T * pElement, UInt iIndex ) {
    Assert( HasCPUData() );
    Assert( m_iElementSize == sizeof(T) );
    Assert( iIndex < m_iElementCount );
    ((T*)m_arrBuffer)[iIndex] = *pElement;
}
template<typename T>
inline Void GPUBuffer::SetElements( const T * arrElements, UInt iIndex, UInt iCount ) {
    Assert( HasCPUData() );
    Assert( m_iElementSize == sizeof(T) );
    Assert( iIndex < m_iElementCount );
    if ( iCount == INVALID_OFFSET )
        iCount = ( m_iElementCount - iIndex );
    Assert( (iIndex + iCount) <= m_iElementCount );
    for( UInt i = 0; i < iCount; ++i )
        ((T*)m_arrBuffer)[iIndex + i] = arrElements[i];
}

/////////////////////////////////////////////////////////////////////////////////
// GPUVertexBuffer implementation
inline Bool GPUVertexBuffer::IsIndirect() const {
    return m_hDeviceVertexBuffer.IsIndirect();
}
inline Void GPUVertexBuffer::SetIndirect( Bool bDrawIndirect ) {
    Assert( !(IsBound()) );
    m_hDeviceVertexBuffer.SetIndirect( bDrawIndirect );
}

/////////////////////////////////////////////////////////////////////////////////
// GPUIndexBuffer implementation
inline Bool GPUIndexBuffer::UseShorts() const {
    return ( m_iElementSize == sizeof(Word) );
}
inline Bool GPUIndexBuffer::UseLongs() const {
    return ( m_iElementSize == sizeof(DWord) );
}

inline Bool GPUIndexBuffer::IsIndirect() const {
    return m_hDeviceIndexBuffer.IsIndirect();
}
inline Void GPUIndexBuffer::SetIndirect( Bool bDrawIndirect ) {
    Assert( !(IsBound()) );
    m_hDeviceIndexBuffer.SetIndirect( bDrawIndirect );
}

/////////////////////////////////////////////////////////////////////////////////
// GPUConstantBuffer implementation

/////////////////////////////////////////////////////////////////////////////////
// GPURawBuffer implementation

/////////////////////////////////////////////////////////////////////////////////
// GPUStructuredBuffer implementation

