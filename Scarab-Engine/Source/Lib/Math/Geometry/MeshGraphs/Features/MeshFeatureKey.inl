/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Features/MeshFeatureKey.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Vertex-Edge-Facet feature keys for MeshGraphs
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// MeshFeatureKey implementation

/////////////////////////////////////////////////////////////////////////////////
// MeshVertexKey implementation
inline MeshFeatureType MeshVertexKey::GetType() const {
    return MESH_FEATURE_VERTEX;
}

inline MeshVertexKey & MeshVertexKey::operator=( const MeshVertexKey & rhs ) {
    V = rhs.V;
    return (*this);
}

inline Bool MeshVertexKey::operator==( const MeshFeatureKey & rhs ) const {
    if ( rhs.GetType() != MESH_FEATURE_VERTEX )
        return false;
    const MeshVertexKey & rhsKey = (const MeshVertexKey &)rhs;
    return ( V == rhsKey.V );
}
inline Bool MeshVertexKey::operator!=( const MeshFeatureKey & rhs ) const {
    if ( rhs.GetType() != MESH_FEATURE_VERTEX )
        return true;
    const MeshVertexKey & rhsKey = (const MeshVertexKey &)rhs;
    return ( V != rhsKey.V );
}

inline UInt MeshVertexKey::operator[]( UInt rhs ) const {
    Assert( rhs < 1 );
    return V;
}
inline UInt & MeshVertexKey::operator[]( UInt rhs ) {
    Assert( rhs < 1 );
    return V;
}
inline UInt MeshVertexKey::operator[]( Int rhs ) const {
    Assert( rhs >= 0 && rhs < 1 );
    return V;
}
inline UInt & MeshVertexKey::operator[]( Int rhs ) {
    Assert( rhs >= 0 && rhs < 1 );
    return V;
}

/////////////////////////////////////////////////////////////////////////////////
// MeshEdgeKey implementation
inline MeshFeatureType MeshEdgeKey::GetType() const {
    return MESH_FEATURE_EDGE;
}

inline MeshEdgeKey & MeshEdgeKey::operator=( const MeshEdgeKey & rhs ) {
    A = rhs.A; B = rhs.B;
    return (*this);
}

inline Bool MeshEdgeKey::operator==( const MeshFeatureKey & rhs ) const {
    if ( rhs.GetType() != MESH_FEATURE_EDGE )
        return false;
    const MeshEdgeKey & rhsKey = (const MeshEdgeKey &)rhs;
    return ( ( A == rhsKey.A ) && ( B == rhsKey.B ) );
}
inline Bool MeshEdgeKey::operator!=( const MeshFeatureKey & rhs ) const {
    if ( rhs.GetType() != MESH_FEATURE_EDGE )
        return true;
    const MeshEdgeKey & rhsKey = (const MeshEdgeKey &)rhs;
    return ( ( A != rhsKey.A ) || ( B != rhsKey.B ) );
}

inline UInt MeshEdgeKey::operator[]( UInt rhs ) const {
    Assert( rhs < 2 );
    return V[rhs];
}
inline UInt & MeshEdgeKey::operator[]( UInt rhs ) {
    Assert( rhs < 2 );
    return V[rhs];
}
inline UInt MeshEdgeKey::operator[]( Int rhs ) const {
    Assert( rhs >= 0 && rhs < 2 );
    return V[rhs];
}
inline UInt & MeshEdgeKey::operator[]( Int rhs ) {
    Assert( rhs >= 0 && rhs < 2 );
    return V[rhs];
}

inline MeshVertexKey MeshEdgeKey::GetVertexKey( UInt iVertex ) const {
    Assert( iVertex < 2 );
    return MeshVertexKey( V[iVertex] );
}

/////////////////////////////////////////////////////////////////////////////////
// MeshFacetKey implementation
inline MeshFeatureType MeshFacetKey::GetType() const {
    return MESH_FEATURE_FACET;
}

/////////////////////////////////////////////////////////////////////////////////
// MeshTriangleKey implementation
inline MeshFacetType MeshTriangleKey::GetFacetType() const {
    return MESH_FACET_TRIANGLE;
}
inline UInt MeshTriangleKey::GetFacetSize() const {
    return 3;
}

inline MeshTriangleKey & MeshTriangleKey::operator=( const MeshTriangleKey & rhs ) {
    A = rhs.A; B = rhs.B; C = rhs.C;
    return (*this);
}

inline UInt MeshTriangleKey::operator[]( UInt rhs ) const {
    Assert( rhs < 3 );
    return V[rhs];
}
inline UInt & MeshTriangleKey::operator[]( UInt rhs ) {
    Assert( rhs < 3 );
    return V[rhs];
}
inline UInt MeshTriangleKey::operator[]( Int rhs ) const {
    Assert( rhs >= 0 && rhs < 3 );
    return V[rhs];
}
inline UInt & MeshTriangleKey::operator[]( Int rhs ) {
    Assert( rhs >= 0 && rhs < 3 );
    return V[rhs];
}

inline MeshVertexKey MeshTriangleKey::GetVertexKey( UInt iVertex ) const {
    Assert( iVertex < 3 );
    return MeshVertexKey( V[iVertex] );
}
inline MeshEdgeKey MeshTriangleKey::GetEdgeKey( UInt iEdge ) const {
    Assert( iEdge < 3 );
    return MeshEdgeKey( V[iEdge], V[(iEdge+1) % 3] );
}

/////////////////////////////////////////////////////////////////////////////////
// MeshQuadKey implementation
inline MeshFacetType MeshQuadKey::GetFacetType() const {
    return MESH_FACET_QUAD;
}
inline UInt MeshQuadKey::GetFacetSize() const {
    return 4;
}

inline MeshQuadKey & MeshQuadKey::operator=( const MeshQuadKey & rhs ) {
    A = rhs.A; B = rhs.B; C = rhs.C; D = rhs.D;
    return (*this);
}

inline UInt MeshQuadKey::operator[]( UInt rhs ) const {
    Assert( rhs < 4 );
    return V[rhs];
}
inline UInt & MeshQuadKey::operator[]( UInt rhs ) {
    Assert( rhs < 4 );
    return V[rhs];
}
inline UInt MeshQuadKey::operator[]( Int rhs ) const {
    Assert( rhs >= 0 && rhs < 4 );
    return V[rhs];
}
inline UInt & MeshQuadKey::operator[]( Int rhs ) {
    Assert( rhs >= 0 && rhs < 4 );
    return V[rhs];
}

inline MeshVertexKey MeshQuadKey::GetVertexKey( UInt iVertex ) const {
    Assert( iVertex < 4 );
    return MeshVertexKey( V[iVertex] );
}
inline MeshEdgeKey MeshQuadKey::GetEdgeKey( UInt iEdge ) const {
    Assert( iEdge < 4 );
    return MeshEdgeKey( V[iEdge], V[(iEdge+1) % 4] );
}

/////////////////////////////////////////////////////////////////////////////////
// MeshPolygonKey implementation
inline MeshFacetType MeshPolygonKey::GetFacetType() const {
    return MESH_FACET_POLYGON;
}
inline UInt MeshPolygonKey::GetFacetSize() const {
    return SIZE;
}

inline MeshPolygonKey & MeshPolygonKey::operator=( const MeshPolygonKey & rhs ) {
    for( UInt i = 0; i < SIZE; ++i )
        V[i] = rhs.V[i];
    return (*this);
}

inline UInt MeshPolygonKey::operator[]( UInt rhs ) const {
    Assert( rhs < SIZE );
    return V[rhs];
}
inline UInt & MeshPolygonKey::operator[]( UInt rhs ) {
    Assert( rhs < SIZE );
    return V[rhs];
}
inline UInt MeshPolygonKey::operator[]( Int rhs ) const {
    Assert( rhs >= 0 && rhs < (Int)SIZE );
    return V[rhs];
}
inline UInt & MeshPolygonKey::operator[]( Int rhs ) {
    Assert( rhs >= 0 && rhs < (Int)SIZE );
    return V[rhs];
}

inline MeshVertexKey MeshPolygonKey::GetVertexKey( UInt iVertex ) const {
    Assert( iVertex < SIZE );
    return MeshVertexKey( V[iVertex] );
}
inline MeshEdgeKey MeshPolygonKey::GetEdgeKey( UInt iEdge ) const {
    Assert( iEdge < SIZE );
    return MeshEdgeKey( V[iEdge], V[(iEdge+1) % SIZE] );
}


