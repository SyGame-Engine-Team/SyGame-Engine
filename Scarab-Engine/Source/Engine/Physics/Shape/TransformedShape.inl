/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Shape/TransformedShape.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Transformed collision shape
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TransformedShape implementation
inline ShapeType TransformedShape::GetType() const {
    return m_pShape->GetType();
}

inline Bool TransformedShape::IsConvex() const {
    return m_pShape->IsConvex();
}
inline Bool TransformedShape::IsConcave() const {
    return m_pShape->IsConcave();
}

inline const Shape * TransformedShape::GetShape() const {
    return m_pShape;
}
inline const Transform3 & TransformedShape::GetTransform() const {
    return m_vTransform;
}

inline Void TransformedShape::OnShapeUpdate() {
    _ComputeWorldAAB();
}
inline Void TransformedShape::OnTransformUpdate( const Transform3 & vTransform ) {
    m_vTransform = vTransform;
    _ComputeWorldAAB();
}

inline const BVAABox & TransformedShape::GetAAB() const {
    return m_bvWorldAAB;
}

/////////////////////////////////////////////////////////////////////////////////

inline Void TransformedShape::_ComputeWorldAAB() {
    if ( m_pShape->GetType() == SHAPE_PRIMITIVE_SPHERE ) {
        // Invariant by rotation
        m_bvWorldAAB = BVAABox( m_pShape->GetAAB().GetCenter() + m_vTransform.GetTranslate(),
                                m_pShape->GetAAB().GetExtents() );
    } else
        m_pShape->GetAAB().Transform( m_bvWorldAAB, m_vTransform );

    // Add proxy margin
    m_bvWorldAAB.AddMargin( SHAPE_MARGIN );
}

/////////////////////////////////////////////////////////////////////////////////
// TransformedShapeConvex implementation
inline const MeshVoronoiGraph * TransformedShapeConvex::GetGraph() const {
    return ((const ShapeConvex *)m_pShape)->GetGraph();
}

inline Void TransformedShapeConvex::GetVertexPosition( Vertex3 * outPosition, const MeshVertex * pVertex ) const {
    const MeshVoronoiGraph * pGraph = GetGraph();
    *outPosition = ( m_vTransform * pGraph->GetVertexPosition(pVertex) );
}

inline Void TransformedShapeConvex::GetFacetNormal( Vector3 * outNormal, const MeshFacet * pFacet ) const {
    const MeshVoronoiGraph * pGraph = GetGraph();
    *outNormal = ( m_vTransform * pGraph->GetFacetNormal(pFacet) );
    outNormal->Normalize();
}
inline Void TransformedShapeConvex::GetFacetPlane( Plane * outPlane, const MeshFacet * pFacet ) const {
    GetFacetNormal( &(outPlane->Normal), pFacet );
    GetVertexPosition( &(outPlane->Position), pFacet->GetVertex(0) );
    outPlane->UpdateConstant();
}

inline UInt TransformedShapeConvex::GetVoronoiPlaneCount( const MeshFeature * pFeature ) const {
    const MeshVoronoiGraph * pGraph = GetGraph();
    return pGraph->GetVoronoiPlaneCount( pFeature );
}
inline Void TransformedShapeConvex::GetVoronoiPlane( Plane * outPlane, const MeshFeature * pFeature, UInt iPlane ) const {
    const MeshVoronoiGraph * pGraph = GetGraph();
    const Plane & localPlane = pGraph->GetVoronoiPlane( pFeature, iPlane );
    outPlane->Normal = ( m_vTransform * localPlane.Normal );
    outPlane->Normal.Normalize();
    outPlane->Position = ( m_vTransform * localPlane.Position );
    outPlane->UpdateConstant();
}

/////////////////////////////////////////////////////////////////////////////////
// TransformedShapeConcave implementation


