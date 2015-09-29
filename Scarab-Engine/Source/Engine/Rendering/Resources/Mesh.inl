/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Resources/Mesh.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Universal Mesh container for rendering geometry
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Mesh implementation
inline ResourceType Mesh::GetResourceType() const {
    return RESOURCE_TYPE_MESH;
}

inline Void Mesh::SetIL( GPUInputLayout * pIL ) {
    m_pIL = pIL;
    if ( m_pIL != NULL && m_pVB != NULL )
        Assert( m_pIL->GetSize() == m_pVB->GetElementSize() );
}
inline Void Mesh::SetVB( GPUVertexBuffer * pVB ) {
    m_pVB = pVB;
    if ( m_pVB != NULL ) {
        if ( m_pIL != NULL )
            Assert( m_pIL->GetSize() == m_pVB->GetElementSize() );
        Assert( !(m_pVB->IsIndirect()) );
        m_iVertexOffset = 0;
        m_iVertexCount = m_pVB->GetElementCount();
    }
}
inline Void Mesh::SetIB( GPUIndexBuffer * pIB ) {
    m_pIB = pIB;
    if ( m_pIB != NULL ) {
        Assert( m_pIL != NULL && m_pVB != NULL );
        Assert( !(m_pIB->IsIndirect()) );
        m_iIndexOffset = 0;
        m_iIndexCount = m_pIB->GetElementCount();
    }
}

inline Void Mesh::SetVertexRange( UInt iVertexOffset, UInt iVertexCount ) {
    Assert( m_pVB != NULL && (iVertexOffset + iVertexCount) <= m_pVB->GetElementCount() );
    m_iVertexOffset = iVertexOffset;
    m_iVertexCount = iVertexCount;
}
inline Void Mesh::SetIndexRange( UInt iIndexOffset, UInt iIndexCount ) {
    Assert( m_pIB != NULL && (iIndexOffset + iIndexCount) <= m_pIB->GetElementCount() );
    m_iIndexOffset = iIndexOffset;
    m_iIndexCount = iIndexCount;
}

inline Bool Mesh::IsPointMesh() const {
    return ( GetTopology() == GPUSHADER_PRIMITIVETOPOLOGY_POINTLIST );
}
inline Bool Mesh::IsLineMesh() const {
    GPUShaderPrimitiveTopology iTopology = GetTopology();
    return ( iTopology == GPUSHADER_PRIMITIVETOPOLOGY_LINELIST     || iTopology == GPUSHADER_PRIMITIVETOPOLOGY_LINESTRIP ||
             iTopology == GPUSHADER_PRIMITIVETOPOLOGY_LINELIST_ADJ || iTopology == GPUSHADER_PRIMITIVETOPOLOGY_LINESTRIP_ADJ );
}
inline Bool Mesh::IsTriangleMesh() const {
    GPUShaderPrimitiveTopology iTopology = GetTopology();
    return ( iTopology == GPUSHADER_PRIMITIVETOPOLOGY_TRIANGLELIST     || iTopology == GPUSHADER_PRIMITIVETOPOLOGY_TRIANGLESTRIP ||
             iTopology == GPUSHADER_PRIMITIVETOPOLOGY_TRIANGLELIST_ADJ || iTopology == GPUSHADER_PRIMITIVETOPOLOGY_TRIANGLESTRIP_ADJ );
}
inline Bool Mesh::IsPatchList() const {
    return ( GetTopology() >= GPUSHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_1 );
}

inline GPUInputLayout * Mesh::GetIL() const {
    return m_pIL;
}
inline GPUVertexBuffer * Mesh::GetVB() const {
    return m_pVB;
}
inline GPUIndexBuffer * Mesh::GetIB() const {
    return m_pIB;
}

inline UInt Mesh::GetVertexSize() const {
    Assert( m_pIL != NULL );
    return m_pIL->GetSize();
}
inline UInt Mesh::GetVertexCapacity() const {
    Assert( m_pVB != NULL );
    return m_pVB->GetElementCount();
}
inline UInt Mesh::GetVertexCount() const {
    Assert( m_pVB != NULL );
    return m_iVertexCount;
}
inline UInt Mesh::GetVertexOffset() const {
    Assert( m_pVB != NULL );
    return m_iVertexOffset;
}

inline Bool Mesh::HasIndexBuffer() const {
    return ( m_pIB != NULL );
}
inline UInt Mesh::GetIndexSize() const {
    Assert( m_pIB != NULL );
    return m_pIB->GetElementSize();
}
inline UInt Mesh::GetIndexCapacity() const {
    Assert( m_pIB != NULL );
    return m_pIB->GetElementCount();
}
inline UInt Mesh::GetIndexCount() const {
    Assert( m_pIB != NULL );
    return m_iIndexCount;
}
inline UInt Mesh::GetIndexOffset() const {
    Assert( m_pIB != NULL );
    return m_iIndexOffset;
}

/////////////////////////////////////////////////////////////////////////////////
// PointMesh implementation
inline GPUShaderPrimitiveTopology PointMesh::GetTopology() const {
    return GPUSHADER_PRIMITIVETOPOLOGY_POINTLIST;
}

inline UInt PointMesh::GetFacetCount() const {
    return GetPointCount();
}

/////////////////////////////////////////////////////////////////////////////////
// LineMesh implementation
inline UInt LineMesh::GetFacetCount() const {
    return GetLineCount();
}

inline Bool LineMesh::HasAdjacency() const {
    return m_bHasAdjacency;
}

/////////////////////////////////////////////////////////////////////////////////
// LineListMesh implementation
inline GPUShaderPrimitiveTopology LineListMesh::GetTopology() const {
    return (m_bHasAdjacency) ? GPUSHADER_PRIMITIVETOPOLOGY_LINELIST_ADJ : GPUSHADER_PRIMITIVETOPOLOGY_LINELIST;
}

/////////////////////////////////////////////////////////////////////////////////
// LineStripMesh implementation
inline GPUShaderPrimitiveTopology LineStripMesh::GetTopology() const {
    return (m_bHasAdjacency) ? GPUSHADER_PRIMITIVETOPOLOGY_LINESTRIP_ADJ : GPUSHADER_PRIMITIVETOPOLOGY_LINESTRIP;
}

/////////////////////////////////////////////////////////////////////////////////
// TriangleMesh implementation
inline UInt TriangleMesh::GetFacetCount() const {
    return GetTriangleCount();
}

inline Bool TriangleMesh::HasAdjacency() const {
    return m_bHasAdjacency;
}

/////////////////////////////////////////////////////////////////////////////////
// TriangleListMesh implementation
inline GPUShaderPrimitiveTopology TriangleListMesh::GetTopology() const {
    return (m_bHasAdjacency) ? GPUSHADER_PRIMITIVETOPOLOGY_TRIANGLELIST_ADJ : GPUSHADER_PRIMITIVETOPOLOGY_TRIANGLELIST;
}

/////////////////////////////////////////////////////////////////////////////////
// TriangleStripMesh implementation
inline GPUShaderPrimitiveTopology TriangleStripMesh::GetTopology() const {
    return (m_bHasAdjacency) ? GPUSHADER_PRIMITIVETOPOLOGY_TRIANGLESTRIP_ADJ : GPUSHADER_PRIMITIVETOPOLOGY_TRIANGLESTRIP;
}

