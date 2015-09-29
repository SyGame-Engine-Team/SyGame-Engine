/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Shape/Shape.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Collision shape abstraction
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Shape implementation
inline Bool Shape::IsConvex() const {
    return ( GetType() <= SHAPE_CONVEX_MESH );
}
inline Bool Shape::IsConcave() const {
    return ( GetType() >= SHAPE_CONCAVE_MESH );
}
inline Bool Shape::IsDeformable() const {
    return ( GetType() == SHAPE_CONCAVE_DEFORMABLE );
}

inline const BVAABox & Shape::GetAAB() const {
    return m_bvLocalAAB;
}

inline Mesh * Shape::GetMesh() const {
    return m_pMesh;
}

/////////////////////////////////////////////////////////////////////////////////
// ShapeConvex implementation

/////////////////////////////////////////////////////////////////////////////////
// ShapeConcave implementation
inline Void ShapeConcave::HNodeQuery::Process( const AABoxNode * pLeaf ) {
    ShapeConcave::NodeLeaf * pNode = (ShapeConcave::NodeLeaf *)( pLeaf->pLeafData );
    Process( *pNode );
}
inline Void ShapeConcave::HNodeQuery::Process( const AABoxNode * pLeafA, const AABoxNode * pLeafB ) {
    ShapeConcave::NodeLeaf * pNodeA = (ShapeConcave::NodeLeaf *)( pLeafA->pLeafData );
    ShapeConcave::NodeLeaf * pNodeB = (ShapeConcave::NodeLeaf *)( pLeafB->pLeafData );
    Process( *pNodeA, *pNodeB );
}

inline Void ShapeConcave::HTriangleQuery::Process( const AABoxNode * pLeaf ) {
    ShapeConcave::TriangleLeaf * pTriangle = (ShapeConcave::TriangleLeaf *)( pLeaf->pLeafData );
    Process( *pTriangle );
}
inline Void ShapeConcave::HTriangleQuery::Process( const AABoxNode * pLeafA, const AABoxNode * pLeafB ) {
    ShapeConcave::TriangleLeaf * pTriangleA = (ShapeConcave::TriangleLeaf *)( pLeafA->pLeafData );
    ShapeConcave::TriangleLeaf * pTriangleB = (ShapeConcave::TriangleLeaf *)( pLeafB->pLeafData );
    Process( *pTriangleA, *pTriangleB );
}

