/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Primitives/2D/ConvexPolygon2.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives 2D : Convex Polygon
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TConvexPolygon2 implementation
template<typename Real>
TConvexPolygon2<Real>::TConvexPolygon2()
{
    m_iVertexCount = 0;
}
template<typename Real>
TConvexPolygon2<Real>::TConvexPolygon2( TVertex2<Real> * arrVertices, UInt iVertexCount )
{
    Assert( iVertexCount <= CONVEXPOLYGON2_MAX_VERTICES );
    m_iVertexCount = iVertexCount;
    for( UInt i = 0; i < m_iVertexCount; ++i )
        m_arrVertices[i] = arrVertices[i];
}
template<typename Real>
TConvexPolygon2<Real>::TConvexPolygon2( const TConvexPolygon2<Real> & rhs )
{
    m_iVertexCount = rhs.m_iVertexCount;
    for( UInt i = 0; i < m_iVertexCount; ++i )
        m_arrVertices[i] = rhs.m_arrVertices[i];
}
template<typename Real>
TConvexPolygon2<Real>::~TConvexPolygon2()
{
    // nothing to do
}

template<typename Real>
TConvexPolygon2<Real> & TConvexPolygon2<Real>::operator=( const TConvexPolygon2<Real> & rhs )
{
    m_iVertexCount = rhs.m_iVertexCount;
    for( UInt i = 0; i < m_iVertexCount; ++i )
        m_arrVertices[i] = rhs.m_arrVertices[i];
    return (*this);
}

template<typename Real>
inline UInt TConvexPolygon2<Real>::GetVertexCount() const {
    return m_iVertexCount;
}
template<typename Real>
inline const TVertex2<Real> & TConvexPolygon2<Real>::GetVertex( UInt iVertex ) const {
    Assert( iVertex < m_iVertexCount );
    return m_arrVertices[iVertex];
}

template<typename Real>
inline Void TConvexPolygon2<Real>::SetVertex( UInt iVertex, const TVertex2<Real> & vValue ) {
    Assert( iVertex < m_iVertexCount );
    m_arrVertices[iVertex] = vValue;
}

template<typename Real>
inline Void TConvexPolygon2<Real>::PushVertex( const TVertex2<Real> & vValue ) {
    Assert( m_iVertexCount < CONVEXPOLYGON2_MAX_VERTICES );
    m_arrVertices[m_iVertexCount] = vValue;
    ++m_iVertexCount;
}
template<typename Real>
inline Void TConvexPolygon2<Real>::PopVertex( TVertex2<Real> * outValue ) {
    Assert( m_iVertexCount > 0 );
    --m_iVertexCount;
    if ( outValue != NULL )
        *outValue = m_arrVertices[m_iVertexCount];
}

template<typename Real>
Void TConvexPolygon2<Real>::AddVertex( UInt iVertex, const TVertex2<Real> & vValue )
{
    Assert( m_iVertexCount < CONVEXPOLYGON2_MAX_VERTICES );
    Assert( iVertex < m_iVertexCount );
    for( UInt i = m_iVertexCount; i > iVertex; --i )
        m_arrVertices[i] = m_arrVertices[i-1];
    m_arrVertices[iVertex] = vValue;
    ++m_iVertexCount;
}
template<typename Real>
Void TConvexPolygon2<Real>::RemoveVertex( UInt iVertex, TVertex2<Real> * outValue )
{
    Assert( iVertex < m_iVertexCount );
    --m_iVertexCount;
    if ( outValue != NULL )
        *outValue = m_arrVertices[iVertex];
    for( UInt i = iVertex; i < m_iVertexCount; ++i )
        m_arrVertices[i] = m_arrVertices[i+1];
}

template<typename Real>
inline Void TConvexPolygon2<Real>::Clear() {
    m_iVertexCount = 0;
}

template<typename Real>
inline UInt TConvexPolygon2<Real>::GetEdgeCount() const {
    return m_iVertexCount;
}
template<typename Real>
inline Void TConvexPolygon2<Real>::GetEdge( TSegment2<Real> * outEdge, UInt iEdge ) const {
    Assert( iEdge < m_iVertexCount );
    outEdge->EndA = m_arrVertices[iEdge];
    outEdge->EndB = m_arrVertices[(iEdge + 1) % m_iVertexCount];
    outEdge->UpdateCenterDirExtent();
}

template<typename Real>
Bool TConvexPolygon2<Real>::TestInclusion( const TVertex2<Real> & vPoint, Real fThreshold ) const
{
    Assert( false );
    return false;
}

template<typename Real>
UInt TConvexPolygon2<Real>::ClosestFeature( const TVertex2<Real> & vPoint, Bool * outIsEdgeElseVertex ) const
{
    Assert( false );
    return INVALID_OFFSET;
}
template<typename Real>
TVertex2<Real> TConvexPolygon2<Real>::ClosestPoint( const TVertex2<Real> & vPoint ) const
{
    Assert( false );
    return TVertex2<Real>::Null;
}

template<typename Real>
Real TConvexPolygon2<Real>::Distance( const TVertex2<Real> & vPoint ) const
{
    Assert( false );
    return (Real)0;
}

