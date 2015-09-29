/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Graphs/MeshDeformableGraph.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Mesh Graph : Specialization for collision detection
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TMeshDeformableGraph:Features implementation
template<typename Real>
TMeshDeformableGraph<Real>::Vertex::Vertex( const MeshVertexKey & vertexKey ):
    MeshVertexManifoldN( vertexKey ), Position()
{
    Position = TVertex3<Real>::Null;
    pLeaf = NULL;
}
template<typename Real>
TMeshDeformableGraph<Real>::Vertex::~Vertex()
{
    // nothing to do
}

template<typename Real>
inline TBVAABox<Real> TMeshDeformableGraph<Real>::Vertex::ComputeAAB( Real fRadius ) const {
    return TBVAABox<Real>( Position, fRadius );
}

template<typename Real>
TMeshDeformableGraph<Real>::Edge::Edge( const MeshEdgeKey & edgeKey ):
    MeshEdgeManifold2_V( edgeKey )
{
    // nothing to do
}
template<typename Real>
TMeshDeformableGraph<Real>::Edge::~Edge()
{
    // nothing to do
}

template<typename Real>
TMeshDeformableGraph<Real>::Triangle::Triangle( const MeshTriangleKey & triangleKey ):
    MeshTriangleManifold3_V( triangleKey ), Normal()
{
    Normal = TVector3<Real>::Null;
    Area = (Real)0;
    pLeaf = NULL;
}
template<typename Real>
TMeshDeformableGraph<Real>::Triangle::~Triangle()
{
    // nothing to do
}

template<typename Real>
inline Void TMeshDeformableGraph<Real>::Triangle::UpdateNormalArea() {
    const TVertex3<Real> & vA = ( (Vertex*)(Vertices[0]) )->Position;
    const TVertex3<Real> & vB = ( (Vertex*)(Vertices[1]) )->Position;
    const TVertex3<Real> & vC = ( (Vertex*)(Vertices[2]) )->Position;
    Normal = ( (vB - vA) ^ (vC - vA) );
    Area = Normal.Normalize();
}
template<typename Real>
inline TBVAABox<Real> TMeshDeformableGraph<Real>::Triangle::ComputeAAB( Real fThickness ) const {
    const TVertex3<Real> & vA = ( (Vertex*)(Vertices[0]) )->Position;
    const TVertex3<Real> & vB = ( (Vertex*)(Vertices[1]) )->Position;
    const TVertex3<Real> & vC = ( (Vertex*)(Vertices[2]) )->Position;
    TVector3<Real> vThickness = ( Normal * fThickness );
    TBVAABox<Real> bvAABox;
    bvAABox.Reset( vA - vThickness ); bvAABox.Grow( vA + vThickness );
    bvAABox.Grow( vB - vThickness );  bvAABox.Grow( vB + vThickness );
    bvAABox.Grow( vC - vThickness );  bvAABox.Grow( vC + vThickness );
    return bvAABox;
}

/////////////////////////////////////////////////////////////////////////////////
// TMeshDeformableGraph implementation
template<typename Real>
inline MeshVertex * TMeshDeformableGraph<Real>::DeformableVertexBuilder( const MeshVertexKey & vertexKey ) {
    return New Vertex( vertexKey );
}
template<typename Real>
inline MeshEdge * TMeshDeformableGraph<Real>::DeformableEdgeBuilder( const MeshEdgeKey & edgeKey ) {
    return New Edge( edgeKey );
}
template<typename Real>
inline MeshFacet * TMeshDeformableGraph<Real>::DeformableFacetBuilder( const MeshFacetKey & facetKey ) {
    Assert( facetKey.GetFacetType() == MESH_FACET_TRIANGLE );
    return New Triangle( (const MeshTriangleKey &)facetKey );
}

template<typename Real>
TMeshDeformableGraph<Real>::TMeshDeformableGraph( VertexBuilder pfVertexBuilder, EdgeBuilder pfEdgeBuilder, FacetBuilder pfFacetBuilder ):
    MeshManifold3( pfVertexBuilder, pfEdgeBuilder, pfFacetBuilder ),
    m_hVertexTree(), m_hTriangleTree()
{
    // nothing to do
}
template<typename Real>
TMeshDeformableGraph<Real>::~TMeshDeformableGraph()
{
    // nothing to do
    // Call Destroy explicitly
}

template<typename Real>
inline Void TMeshDeformableGraph<Real>::UseMemoryContext( MemoryContextID idMemoryContext, const GChar * strAllocatorName ) {
    MeshManifold3::UseMemoryContext( idMemoryContext, strAllocatorName );
    m_hVertexTree.UseMemoryContext( idMemoryContext, strAllocatorName );
    m_hTriangleTree.UseMemoryContext( idMemoryContext, strAllocatorName );
}

template<typename Real>
inline Void TMeshDeformableGraph<Real>::Create() {
    MeshManifold3::Create();
    m_hVertexTree.Create();
    m_hTriangleTree.Create();
}
template<typename Real>
inline Void TMeshDeformableGraph<Real>::Destroy() {
    m_hTriangleTree.Destroy();
    m_hVertexTree.Destroy();
    MeshManifold3::Destroy();
}

template<typename Real>
inline UInt TMeshDeformableGraph<Real>::MemorySize() const {
    return ( MeshManifold3::MemorySize() + m_hVertexTree.MemorySize() + m_hTriangleTree.MemorySize() );
}
template<typename Real>
inline UInt TMeshDeformableGraph<Real>::Count() const {
    return MeshManifold3::Count();
}

template<typename Real>
inline Void TMeshDeformableGraph<Real>::Clear() {
    m_hTriangleTree.Clear();
    m_hVertexTree.Clear();
    MeshManifold3::Clear();
}

template<typename Real>
inline Void TMeshDeformableGraph<Real>::ComputeFeatureData( const TVertex3<Real> * arrVertices, Real fVertexRadius, Real fTriangleThickness ) {
    ComputeFeatureData( (const Byte *)arrVertices, sizeof(TVertex3<Real>), fVertexRadius, fTriangleThickness );
}
template<typename Real>
Void TMeshDeformableGraph<Real>::ComputeFeatureData( const Byte * arrVertices, UInt iStride, Real fVertexRadius, Real fTriangleThickness )
{
    m_hVertexTree.Clear();
    m_hTriangleTree.Clear();

    // Vertices
    EnumVertices();
    MeshVertex * pVertex = EnumNextVertex();
    while( pVertex != NULL ) {
        ((Vertex*)pVertex)->Position = _GetPosition( pVertex->Key.V, arrVertices, iStride );
        ((Vertex*)pVertex)->pLeaf = m_hVertexTree.Insert( ((Vertex*)pVertex)->ComputeAAB(fVertexRadius), pVertex );
        pVertex = EnumNextVertex();
    }
    //m_hVertexTree.OptimizeBottomUp();

    // Triangles
    EnumFacets();
    MeshFacet * pTriangle = EnumNextFacet();
    while( pTriangle != NULL ) {
        ((Triangle*)pTriangle)->UpdateNormalArea();
        ((Triangle*)pTriangle)->pLeaf = m_hTriangleTree.Insert( ((Triangle*)pTriangle)->ComputeAAB(fTriangleThickness), pTriangle );
        pTriangle = EnumNextFacet();
    }
    //m_hTriangleTree.OptimizeBottomUp();
}

template<typename Real>
inline const TAABoxTree<Real> * TMeshDeformableGraph<Real>::GetVertexTree() const {
    return &m_hVertexTree;
}
template<typename Real>
inline const TAABoxTree<Real> * TMeshDeformableGraph<Real>::GetTriangleTree() const {
    return &m_hTriangleTree;
}

template<typename Real>
Void TMeshDeformableGraph<Real>::UpdateTrees( Real fVertexRadius, Real fTriangleThickness )
{
    // Update Vertex Tree
    EnumVertices();
    MeshVertex * pVertex = EnumNextVertex();
    while( pVertex != NULL ) {
        m_hVertexTree.Update( ((Vertex*)pVertex)->pLeaf, ((Vertex*)pVertex)->ComputeAAB(fVertexRadius) );
        pVertex = EnumNextVertex();
    }

    // Update Triangle Tree
    EnumFacets();
    MeshFacet * pTriangle = EnumNextFacet();
    while( pTriangle != NULL ) {
        m_hTriangleTree.Update( ((Triangle*)pTriangle)->pLeaf, ((Triangle*)pTriangle)->ComputeAAB(fTriangleThickness) );
        pTriangle = EnumNextFacet();
    }

    // Optimize
    //m_hVertexTree.OptimizeIncremental( 1 );
    //m_hTriangleTree.OptimizeIncremental( 1 );
}

/////////////////////////////////////////////////////////////////////////////////

template<typename Real>
inline const TVertex3<Real> & TMeshDeformableGraph<Real>::_GetPosition( UInt iVertex, const Byte * arrVertices, UInt iStride ) {
    const TVertex3<Real> * pPosition = (const TVertex3<Real> *)( arrVertices + (iVertex * iStride) );
    return *pPosition;
}


