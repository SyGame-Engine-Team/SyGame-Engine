/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Shape/TransformedShape.cpp
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
// Includes
#include "TransformedShape.h"

/////////////////////////////////////////////////////////////////////////////////
// TransformedShape implementation
TransformedShape::TransformedShape( const Shape * pShape, const Transform3 & vTransform ):
    m_vTransform( vTransform ), m_bvWorldAAB()
{
    m_pShape = pShape;

    // Compute world AAB
    _ComputeWorldAAB();
}
TransformedShape::~TransformedShape()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// TransformedShapeConvex implementation
TransformedShapeConvex::TransformedShapeConvex( const ShapeConvex * pShape, const Transform3 & vTransform ):
    TransformedShape( pShape, vTransform )
{
    // nothing to do
}
TransformedShapeConvex::~TransformedShapeConvex()
{
    // nothing to do
}

Void TransformedShapeConvex::QueryExtremalVertices( const Vector3 & vDirection, UInt * outMax, UInt * outMin ) const
{
    const ShapeConvex * pShapeConvex = (const ShapeConvex *)m_pShape;

    Transform3 vInvTransform;
    m_vTransform.Invert( vInvTransform );
    Vector3 vLocalDirection = ( vInvTransform * vDirection );
    vLocalDirection.Normalize();

    pShapeConvex->QueryExtremalVertices( vLocalDirection, outMax, outMin );
}
Void TransformedShapeConvex::QueryExtremalVertices( const Vector3 & vDirection, Vertex3 * outMax, Vertex3 * outMin ) const
{
    const ShapeConvex * pShapeConvex = (const ShapeConvex *)m_pShape;

    Transform3 vInvTransform;
    m_vTransform.Invert( vInvTransform );
    Vector3 vLocalDirection = ( vInvTransform * vDirection );
    vLocalDirection.Normalize();

    pShapeConvex->QueryExtremalVertices( vLocalDirection, outMax, outMin );

    if ( outMax != NULL )
        *outMax = ( m_vTransform * (*outMax) );
    if ( outMin != NULL )
        *outMin = ( m_vTransform * (*outMin) );
}

/////////////////////////////////////////////////////////////////////////////////
// TransformedShapeConcave implementation
TransformedShapeConcave::TransformedShapeConcave( const ShapeConcave * pShape, const Transform3 & vTransform ):
    TransformedShape( pShape, vTransform )
{
    // nothing to do
}
TransformedShapeConcave::~TransformedShapeConcave()
{
    // nothing to do
}

TransformedShapeConcave::HTransformedNodeQuery::HTransformedNodeQuery():
    ShapeConcave::HNodeQuery()
{
    pWorldTransformA = NULL;
    pWorldTransformB = NULL;
}
TransformedShapeConcave::HTransformedNodeQuery::~HTransformedNodeQuery()
{
    // nothing to do
}

Void TransformedShapeConcave::HTransformedNodeQuery::Process( const ShapeConcave::NodeLeaf & nodeLeaf )
{
    ShapeConcave::NodeLeaf trNodeLeaf;
    trNodeLeaf.vNode.Center = ( (*pWorldTransformA) * nodeLeaf.vNode.Center );
    trNodeLeaf.vNode.Radius = nodeLeaf.vNode.Radius;
    trNodeLeaf.dwNodeID = nodeLeaf.dwNodeID;
    trNodeLeaf.pLeaf = nodeLeaf.pLeaf;

    ProcessTransformed( trNodeLeaf );
}
Void TransformedShapeConcave::HTransformedNodeQuery::Process( const ShapeConcave::NodeLeaf & nodeLeafA,
                                                              const ShapeConcave::NodeLeaf & nodeLeafB )
{
    ShapeConcave::NodeLeaf trNodeLeafA;
    trNodeLeafA.vNode.Center = ( (*pWorldTransformA) * nodeLeafA.vNode.Center );
    trNodeLeafA.vNode.Radius = nodeLeafA.vNode.Radius;
    trNodeLeafA.dwNodeID = nodeLeafA.dwNodeID;
    trNodeLeafA.pLeaf = nodeLeafA.pLeaf;

    ShapeConcave::NodeLeaf trNodeLeafB;
    trNodeLeafB.vNode.Center = ( (*pWorldTransformB) * nodeLeafB.vNode.Center );
    trNodeLeafB.vNode.Radius = nodeLeafB.vNode.Radius;
    trNodeLeafB.dwNodeID = nodeLeafB.dwNodeID;
    trNodeLeafB.pLeaf = nodeLeafB.pLeaf;

    ProcessTransformed( trNodeLeafA, trNodeLeafB );
}

Void TransformedShapeConcave::HTransformedNodeQuery::ProcessTransformed( const ShapeConcave::NodeLeaf & /*nodeLeaf*/ )
{
    // Stub method, do nothing
}
Void TransformedShapeConcave::HTransformedNodeQuery::ProcessTransformed( const ShapeConcave::NodeLeaf & /*nodeLeafA*/,
                                                                         const ShapeConcave::NodeLeaf & /*nodeLeafB*/ )
{
    // Stub method, do nothing
}

Void TransformedShapeConcave::QueryNodes( const BVAABox & vAABox, HTransformedNodeQuery * pQuery ) const
{
    const ShapeConcave * pShapeConcave = (const ShapeConcave *)m_pShape;

    // Transform the box to local coordinates, translation only
    BVAABox vLocalAAB( vAABox.GetCenter() - m_vTransform.GetTranslate(), vAABox.GetExtents() );

    // Make the query in local space
    pQuery->pWorldTransformA = &m_vTransform;
    pShapeConcave->QueryNodes( vLocalAAB, pQuery );
    pQuery->pWorldTransformA = NULL;
}

TransformedShapeConcave::HTransformedTriangleQuery::HTransformedTriangleQuery():
    ShapeConcave::HTriangleQuery()
{
    pWorldTransformA = NULL;
    pWorldTransformB = NULL;
}
TransformedShapeConcave::HTransformedTriangleQuery::~HTransformedTriangleQuery()
{
    // nothing to do
}

Void TransformedShapeConcave::HTransformedTriangleQuery::Process( const ShapeConcave::TriangleLeaf & triLeaf )
{
    ShapeConcave::TriangleLeaf trTriLeaf;
    trTriLeaf.vTriangle.A = ( (*pWorldTransformA) * triLeaf.vTriangle.A );
    trTriLeaf.vTriangle.B = ( (*pWorldTransformA) * triLeaf.vTriangle.B );
    trTriLeaf.vTriangle.C = ( (*pWorldTransformA) * triLeaf.vTriangle.C );
    trTriLeaf.dwTriangleID = triLeaf.dwTriangleID;
    trTriLeaf.pLeaf = triLeaf.pLeaf;

    ProcessTransformed( trTriLeaf );
}
Void TransformedShapeConcave::HTransformedTriangleQuery::Process( const ShapeConcave::TriangleLeaf & triLeafA,
                                                                  const ShapeConcave::TriangleLeaf & triLeafB )
{
    ShapeConcave::TriangleLeaf trTriLeafA;
    trTriLeafA.vTriangle.A = ( (*pWorldTransformA) * triLeafA.vTriangle.A );
    trTriLeafA.vTriangle.B = ( (*pWorldTransformA) * triLeafA.vTriangle.B );
    trTriLeafA.vTriangle.C = ( (*pWorldTransformA) * triLeafA.vTriangle.C );
    trTriLeafA.dwTriangleID = triLeafA.dwTriangleID;
    trTriLeafA.pLeaf = triLeafA.pLeaf;

    ShapeConcave::TriangleLeaf trTriLeafB;
    trTriLeafB.vTriangle.A = ( (*pWorldTransformB) * triLeafB.vTriangle.A );
    trTriLeafB.vTriangle.B = ( (*pWorldTransformB) * triLeafB.vTriangle.B );
    trTriLeafB.vTriangle.C = ( (*pWorldTransformB) * triLeafB.vTriangle.C );
    trTriLeafB.dwTriangleID = triLeafB.dwTriangleID;
    trTriLeafB.pLeaf = triLeafB.pLeaf;

    ProcessTransformed( trTriLeafA, trTriLeafB );
}

Void TransformedShapeConcave::HTransformedTriangleQuery::ProcessTransformed( const ShapeConcave::TriangleLeaf & /*triLeaf*/ )
{
    // Stub method, do nothing
}
Void TransformedShapeConcave::HTransformedTriangleQuery::ProcessTransformed( const ShapeConcave::TriangleLeaf & /*triLeafA*/,
                                                                             const ShapeConcave::TriangleLeaf & /*triLeafB*/ )
{
    // Stub method, do nothing
}

Void TransformedShapeConcave::QueryTriangles( const BVAABox & vAABox, HTransformedTriangleQuery * pQuery ) const
{
    const ShapeConcave * pShapeConcave = (const ShapeConcave *)m_pShape;

    // Transform the box to local coordinates, translation only
    BVAABox vLocalAAB( vAABox.GetCenter() - m_vTransform.GetTranslate(), vAABox.GetExtents() );

    // Make the query in local space
    pQuery->pWorldTransformA = &m_vTransform;
    pShapeConcave->QueryTriangles( vLocalAAB, pQuery );
    pQuery->pWorldTransformA = NULL;
}

