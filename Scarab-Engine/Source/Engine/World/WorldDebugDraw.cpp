/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/WorldDebugDraw.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Graph : Debug drawing operations
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "WorldDebugDraw.h"

#include "WorldManager.h"

/////////////////////////////////////////////////////////////////////////////////
// WorldDebugDraw implementation
WorldDebugDraw::WorldDebugDraw():
    m_hDebugDrawEffect()
{
    m_pDebugDrawEffectInstance = m_hDebugDrawEffect.CreateInstance();

    m_idDebugDrawBatch = RenderingFn->BatchCreate( m_pDebugDrawEffectInstance, GPUSHADER_PRIMITIVETOPOLOGY_LINELIST );
}
WorldDebugDraw::~WorldDebugDraw()
{
    RenderingFn->BatchDestroy( m_idDebugDrawBatch );
    m_idDebugDrawBatch = INVALID_OFFSET;

    Effect::DestroyInstance( m_pDebugDrawEffectInstance );
    m_pDebugDrawEffectInstance = NULL;
}

Void WorldDebugDraw::RenderStart() const
{
    RenderingFn->BatchStart( m_idDebugDrawBatch );
}
Void WorldDebugDraw::RenderEnd() const
{
    RenderingFn->BatchFinish( m_idDebugDrawBatch );
}

Void WorldDebugDraw::RenderNode( WorldBaseNode * pNode ) const
{
    DWord dwDebugFlags = pNode->GetDebugFlags();

    // Leaf case
    if ( pNode->IsLeaf() ) {
        WorldLeaf * pLeaf = (WorldLeaf*)pNode;

        // Mesh attributes
        const Mesh * pMesh = pLeaf->GetMesh();
        const TriangleMesh * pTriangleMesh = NULL;

        if ( pMesh != NULL && pMesh->IsTriangleMesh() ) {
            pTriangleMesh = (const TriangleMesh *)pMesh;

            if ( dwDebugFlags & WORLD_DEBUGDRAW_NORMALS )
                _RenderMesh_Normals( pTriangleMesh, pLeaf->WorldTransform );
            if ( dwDebugFlags & WORLD_DEBUGDRAW_TANGENTS )
                _RenderMesh_Tangents( pTriangleMesh, pLeaf->WorldTransform );
            if ( dwDebugFlags & WORLD_DEBUGDRAW_BINORMALS )
                _RenderMesh_Binormals( pTriangleMesh, pLeaf->WorldTransform );
            if ( dwDebugFlags & WORLD_DEBUGDRAW_FACETNORMALS )
                _RenderMesh_FacetNormals( pTriangleMesh, pLeaf->WorldTransform );
        }

        // Body attributes
        const BodyNode * pBody = pLeaf->GetBody();
        if ( pBody != NULL ) {
            if ( dwDebugFlags & WORLD_DEBUGDRAW_PROXIES )
                _RenderBody_Proxy( pBody );
            if ( dwDebugFlags & WORLD_DEBUGDRAW_CONTACTS )
                _RenderBody_Contacts( pBody );
            if ( dwDebugFlags & WORLD_DEBUGDRAW_JOINTS )
                _RenderBody_Joints( pBody );
            if ( dwDebugFlags & WORLD_DEBUGDRAW_CONTACTPOINTS )
                _RenderBody_ContactPoints( pBody );

            if ( dwDebugFlags & WORLD_DEBUGDRAW_LINEARVELOCITY )
                _RenderBody_LinearVelocity( pBody );
            if ( dwDebugFlags & WORLD_DEBUGDRAW_ANGULARVELOCITY )
                _RenderBody_AngularVelocity( pBody );
        }
    }

    // BSP cases
    if ( pNode->GetType() == WORLDNODE_BSP || pNode->GetType() == WORLDNODE_INDOOR_AREA ) {
        // Node attributes
        if ( dwDebugFlags & WORLD_DEBUGDRAW_BSPPLANES )
            _RenderNode_BSPPlane( (const WorldBSPNode *)pNode );
    }

    // ConvexRoom case
    if ( pNode->GetType() == WORLDNODE_INDOOR_CONVEXROOM ) {
        // Node attributes
        if ( dwDebugFlags & WORLD_DEBUGDRAW_PORTALS )
            _RenderNode_Portals( (const WorldConvexRoom *)pNode );
    }

    // OutdoorArea case
    if ( pNode->GetType() == WORLDNODE_OUTDOOR_AREA ) {
        // Node attributes
        if ( dwDebugFlags & WORLD_DEBUGDRAW_PATHLINKS )
            _RenderNode_PathLinks( (WorldOutdoorArea*)pNode );
    }

    // Node attributes
    if ( dwDebugFlags & WORLD_DEBUGDRAW_GIZMOS )
        _RenderNode_Gizmo( pNode );
    if ( dwDebugFlags & WORLD_DEBUGDRAW_WORLDBVS )
        _RenderNode_WorldBV( pNode );
}

Void WorldDebugDraw::RenderBSPTree( WorldBSPNode * pNode ) const
{
    RenderNode( pNode );

    WorldBaseNode * pNegNode = pNode->GetNegativeChild();
    if ( pNegNode != NULL && pNegNode->GetType() == WORLDNODE_BSP )
        RenderBSPTree( (WorldBSPNode*)pNegNode );

    WorldBaseNode * pPosNode = pNode->GetPositiveChild();
    if ( pPosNode != NULL && pPosNode->GetType() == WORLDNODE_BSP )
        RenderBSPTree( (WorldBSPNode*)pPosNode );
}

/////////////////////////////////////////////////////////////////////////////////

const Color4 WorldDebugDraw::sm_arrIslandPalette[27] = {
    Color4(0.0f, 1.0f, 1.0f), Color4(0.0f, 0.5f, 0.0f),
    Color4(1.0f, 0.0f, 1.0f), Color4(0.0f, 0.0f, 0.5f),
    Color4(1.0f, 1.0f, 0.0f), Color4(0.5f, 0.0f, 0.0f),

    Color4(0.0f, 0.5f, 1.0f), Color4(0.5f, 1.0f, 0.5f),
    Color4(1.0f, 0.0f, 0.5f), Color4(0.5f, 0.5f, 1.0f),
    Color4(0.5f, 1.0f, 0.0f), Color4(1.0f, 0.5f, 0.5f),

    Color4(0.0f, 0.5f, 0.5f), Color4(0.0f, 1.0f, 0.0f),
    Color4(0.5f, 0.0f, 0.5f), Color4(0.0f, 0.0f, 1.0f),
    Color4(0.5f, 0.5f, 0.0f), Color4(1.0f, 0.0f, 0.0f),

    Color4(0.0f, 1.0f, 0.5f), Color4(1.0f, 0.5f, 1.0f), Color4::White,
    Color4(0.5f, 0.0f, 1.0f), Color4(1.0f, 1.0f, 0.5f), Color4::Gray,
    Color4(1.0f, 0.5f, 0.0f), Color4(0.5f, 1.0f, 1.0f), Color4::Black
};

Void WorldDebugDraw::_RenderAAB( const Vertex3 & vCenter, const Vector3 & vExtents, const Color4 & vColor ) const
{
    Vector3 vDX( vExtents.X, 0.0f, 0.0f );
    Vector3 vDY( 0.0f, vExtents.Y, 0.0f );
    Vector3 vDZ( 0.0f, 0.0f, vExtents.Z );

    EffectColorVertex vSegment[2];
    vSegment[0].vColor = vColor;
    vSegment[1].vColor = vColor;

    vSegment[0].vPosition = vCenter - vDX + vDY + vDZ;
    vSegment[1].vPosition = vCenter + vDX + vDY + vDZ;
    RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );
    vSegment[0].vPosition = vCenter - vDX + vDY - vDZ;
    vSegment[1].vPosition = vCenter + vDX + vDY - vDZ;
    RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );
    vSegment[0].vPosition = vCenter - vDX - vDY - vDZ;
    vSegment[1].vPosition = vCenter + vDX - vDY - vDZ;
    RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );
    vSegment[0].vPosition = vCenter - vDX - vDY + vDZ;
    vSegment[1].vPosition = vCenter + vDX - vDY + vDZ;
    RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );

    vSegment[0].vPosition = vCenter + vDX - vDY + vDZ;
    vSegment[1].vPosition = vCenter + vDX + vDY + vDZ;
    RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );
    vSegment[0].vPosition = vCenter + vDX - vDY - vDZ;
    vSegment[1].vPosition = vCenter + vDX + vDY - vDZ;
    RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );
    vSegment[0].vPosition = vCenter - vDX - vDY - vDZ;
    vSegment[1].vPosition = vCenter - vDX + vDY - vDZ;
    RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );
    vSegment[0].vPosition = vCenter - vDX - vDY + vDZ;
    vSegment[1].vPosition = vCenter - vDX + vDY + vDZ;
    RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );

    vSegment[0].vPosition = vCenter + vDX + vDY - vDZ;
    vSegment[1].vPosition = vCenter + vDX + vDY + vDZ;
    RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );
    vSegment[0].vPosition = vCenter + vDX - vDY - vDZ;
    vSegment[1].vPosition = vCenter + vDX - vDY + vDZ;
    RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );
    vSegment[0].vPosition = vCenter - vDX - vDY - vDZ;
    vSegment[1].vPosition = vCenter - vDX - vDY + vDZ;
    RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );
    vSegment[0].vPosition = vCenter - vDX + vDY - vDZ;
    vSegment[1].vPosition = vCenter - vDX + vDY + vDZ;
    RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );
}

Void WorldDebugDraw::_RenderNode_Gizmo( const WorldBaseNode * pNode ) const
{
    Vertex3 vOrigin = pNode->WorldTransform * Vertex3::Null;
    Vector3 vAxisI = pNode->WorldTransform * Vector3::eI;
    Vector3 vAxisJ = pNode->WorldTransform * Vector3::eJ;
    Vector3 vAxisK = pNode->WorldTransform * Vector3::eK;
    vAxisI.Normalize();
    vAxisJ.Normalize();
    vAxisK.Normalize();

    EffectColorVertex vSegment[2];

    vSegment[0].vPosition = vOrigin;
    vSegment[1].vPosition = vOrigin + ( vAxisI * 5.0f );
    vSegment[0].vColor = Color4::Red;
    vSegment[1].vColor = Color4::Red;
    RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );

    vSegment[0].vPosition = vOrigin;
    vSegment[1].vPosition = vOrigin + ( vAxisJ * 5.0f );
    vSegment[0].vColor = Color4::Green;
    vSegment[1].vColor = Color4::Green;
    RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );

    vSegment[0].vPosition = vOrigin;
    vSegment[1].vPosition = vOrigin + ( vAxisK * 5.0f );
    vSegment[0].vColor = Color4::Blue;
    vSegment[1].vColor = Color4::Blue;
    RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );
}
Void WorldDebugDraw::_RenderNode_WorldBV( const WorldBaseNode * pNode ) const
{
    const Vertex3 & vCenter = pNode->WorldBV.GetCenter();
    const Vector3 & vExtents = pNode->WorldBV.GetExtents();
    _RenderAAB( vCenter, vExtents, Color4::Magenta );
}

Void WorldDebugDraw::_RenderNode_BSPPlane( const WorldBSPNode * pBSPNode ) const
{
    const Plane & vPlane = pBSPNode->WorldPlane;

    Vector3 vPlaneAxis0, vPlaneAxis1;
    Vector3::MakeComplementBasis( vPlaneAxis0, vPlaneAxis1, vPlane.Normal );
    Vertex3 vA = vPlane.Position - vPlaneAxis0 * 5.0f - vPlaneAxis1 * 5.0f;
    Vertex3 vB = vPlane.Position + vPlaneAxis0 * 5.0f - vPlaneAxis1 * 5.0f;
    Vertex3 vC = vPlane.Position + vPlaneAxis0 * 5.0f + vPlaneAxis1 * 5.0f;
    Vertex3 vD = vPlane.Position - vPlaneAxis0 * 5.0f + vPlaneAxis1 * 5.0f;

    EffectColorVertex vSegment[2];
    vSegment[0].vColor = Color4::Cyan;
    vSegment[1].vColor = Color4::Cyan;

    vSegment[0].vPosition = vA;
    vSegment[1].vPosition = vB;
    RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );

    vSegment[0].vPosition = vB;
    vSegment[1].vPosition = vC;
    RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );

    vSegment[0].vPosition = vC;
    vSegment[1].vPosition = vD;
    RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );

    vSegment[0].vPosition = vD;
    vSegment[1].vPosition = vA;
    RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );

    vSegment[0].vPosition = vA;
    vSegment[1].vPosition = vC;
    RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );

    vSegment[0].vPosition = vB;
    vSegment[1].vPosition = vD;
    RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );
}
Void WorldDebugDraw::_RenderNode_Portals( const WorldConvexRoom * pConvexRoom ) const
{
    UInt iPortalCount = pConvexRoom->GetPortalCount();
    WorldPortal * pPortal;

    UInt iPolygonSize, iPolygonSizeM1, i, j;
    UInt iHalfSize, iQuartSize;
    EffectColorVertex vSegment[2];

    for( j = 0; j < iPortalCount; ++j ) {
        pPortal = pConvexRoom->GetPortal(j);

        if ( pPortal->Closed ) {
            vSegment[0].vColor = Color4::Red;
            vSegment[1].vColor = Color4::Red;
        } else {
            vSegment[0].vColor = Color4::Green;
            vSegment[1].vColor = Color4::Green;
        }

        iPolygonSize = pPortal->GetPolygonSize();
        iPolygonSizeM1 = iPolygonSize - 1;
        for( i = 0; i < iPolygonSizeM1; ++i ) {
            vSegment[0].vPosition = pPortal->GetPolygon(i);
            vSegment[1].vPosition = pPortal->GetPolygon(i+1);
            RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );
        }
        vSegment[0].vPosition = pPortal->GetPolygon(iPolygonSizeM1);
        vSegment[1].vPosition = pPortal->GetPolygon(0);
        RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );

        if ( iPolygonSize > 3 ) {
            iHalfSize = ( iPolygonSize >> 1 );
            iQuartSize = ( iPolygonSize >> 2 );

            vSegment[0].vPosition = pPortal->GetPolygon(0);
            vSegment[1].vPosition = pPortal->GetPolygon(iHalfSize);
            RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );

            vSegment[0].vPosition = pPortal->GetPolygon(iQuartSize);
            vSegment[1].vPosition = pPortal->GetPolygon(iQuartSize + iHalfSize);
            RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );
        }
    }
}
Void WorldDebugDraw::_RenderNode_PathLinks( WorldOutdoorArea * pArea ) const
{
    const WorldPathMicroNode * pAttachedNode = pArea->GetAttachedWaypoint();

    Color4 vMicroLinksColor = Color4::Blue;
    Color4 vTriangleLinksColor = Color4::Yellow;

    EffectColorVertex vSegment[2];
    vSegment[0].vColor = vMicroLinksColor;
    vSegment[1].vColor = vMicroLinksColor;

    UInt iAdjacencyCount = pAttachedNode->GetAdjacencyCount();
    for( UInt i = 0; i < iAdjacencyCount; ++i ) {
        vSegment[0].vPosition = pAttachedNode->GetPosition();
        vSegment[1].vPosition = pAttachedNode->GetAdjacentWaypoint(i)->GetPosition();
        RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );
    }

    vSegment[0].vColor = vTriangleLinksColor;
    vSegment[1].vColor = vTriangleLinksColor;

    UInt iWidth = pArea->GetGridWidth();
    UInt iHeight = pArea->GetGridHeight();
    for( UInt y = 0; y < iHeight; ++y ) {
        for( UInt x = 0; x < iWidth; ++x ) {
            const WorldPathNanoNode * pNode = pArea->GetGridWaypoint( x, y );
            iAdjacencyCount = pNode->GetAdjacencyCount();
            for( UInt i = 0; i < iAdjacencyCount; ++i ) {
                vSegment[0].vPosition = pNode->GetPosition();
                vSegment[1].vPosition = pNode->GetAdjacentWaypoint(i)->GetPosition();
                RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );
            }
        }
    }
}

Void WorldDebugDraw::_RenderMesh_Normals( const TriangleMesh * pMesh, const Transform3 & trWorld ) const
{
    if ( !(pMesh->GetIL()->HasField(GPUINPUTFIELD_SEMANTIC_NORMAL, 0)) )
        return;

    UInt iVertexSize = pMesh->GetVertexSize();
    UInt iVertexCount = pMesh->GetVertexCount();

    UInt iOffset, iSize;

    pMesh->GetIL()->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_POSITION, 0 );
    const Byte * arrPositions = pMesh->GetVB()->GetData() + iOffset;

    pMesh->GetIL()->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_NORMAL, 0 );
    const Byte * arrNormals = pMesh->GetVB()->GetData() + iOffset;

    const Vertex3 * pVertex;
    const Vector3 * pNormal;

    EffectColorVertex vSegment[2];
    vSegment[0].vColor = Color4::Blue;
    vSegment[1].vColor = Color4::Blue;

    for( UInt i = 0; i < iVertexCount; ++i ) {
        pVertex = (const Vertex3 *)( arrPositions );
        pNormal = (const Vector3 *)( arrNormals );

        vSegment[0].vPosition = trWorld * (*pVertex);
        vSegment[1].vPosition = trWorld * (*pVertex + *pNormal);
        RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );

        arrPositions += iVertexSize;
        arrNormals += iVertexSize;
    }
}
Void WorldDebugDraw::_RenderMesh_Tangents( const TriangleMesh * pMesh, const Transform3 & trWorld ) const
{
    if ( !(pMesh->GetIL()->HasField(GPUINPUTFIELD_SEMANTIC_TANGENT, 0)) )
        return;

    UInt iVertexSize = pMesh->GetVertexSize();
    UInt iVertexCount = pMesh->GetVertexCount();

    UInt iOffset, iSize;

    pMesh->GetIL()->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_POSITION, 0 );
    const Byte * arrPositions = pMesh->GetVB()->GetData() + iOffset;

    pMesh->GetIL()->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_TANGENT, 0 );
    const Byte * arrTangents = pMesh->GetVB()->GetData() + iOffset;

    const Vertex3 * pVertex;
    const Vector3 * pTangent;

    EffectColorVertex vSegment[2];
    vSegment[0].vColor = Color4::Red;
    vSegment[1].vColor = Color4::Red;

    for( UInt i = 0; i < iVertexCount; ++i ) {
        pVertex = (const Vertex3 *)( arrPositions );
        pTangent = (const Vector3 *)( arrTangents );

        vSegment[0].vPosition = trWorld * (*pVertex);
        vSegment[1].vPosition = trWorld * (*pVertex + *pTangent);
        RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );

        arrPositions += iVertexSize;
        arrTangents += iVertexSize;
    }
}
Void WorldDebugDraw::_RenderMesh_Binormals( const TriangleMesh * pMesh, const Transform3 & trWorld ) const
{
    if ( !(pMesh->GetIL()->HasField(GPUINPUTFIELD_SEMANTIC_BINORMAL, 0)) )
        return;

    UInt iVertexSize = pMesh->GetVertexSize();
    UInt iVertexCount = pMesh->GetVertexCount();

    UInt iOffset, iSize;

    pMesh->GetIL()->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_POSITION, 0 );
    const Byte * arrPositions = pMesh->GetVB()->GetData() + iOffset;

    pMesh->GetIL()->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_BINORMAL, 0 );
    const Byte * arrBiNormals = pMesh->GetVB()->GetData() + iOffset;

    const Vertex3 * pVertex;
    const Vector3 * pBiNormal;

    EffectColorVertex vSegment[2];
    vSegment[0].vColor = Color4::Green;
    vSegment[1].vColor = Color4::Green;

    for( UInt i = 0; i < iVertexCount; ++i ) {
        pVertex = (const Vertex3 *)( arrPositions );
        pBiNormal = (const Vector3 *)( arrBiNormals );

        vSegment[0].vPosition = trWorld * (*pVertex);
        vSegment[1].vPosition = trWorld * (*pVertex + *pBiNormal);
        RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );

        arrPositions += iVertexSize;
        arrBiNormals += iVertexSize;
    }
}
Void WorldDebugDraw::_RenderMesh_FacetNormals( const TriangleMesh * pMesh, const Transform3 & trWorld ) const
{
    UInt iVertexSize = pMesh->GetVertexSize();
    UInt iTriangleCount = pMesh->GetTriangleCount();

    UInt iOffset, iSize;

    pMesh->GetIL()->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_POSITION, 0 );
    const Byte * arrPositions = pMesh->GetVB()->GetData() + iOffset;

    const Vertex3 *pA, *pB, *pC;

    EffectColorVertex vSegment[2];
    vSegment[0].vColor = Color4::Yellow;
    vSegment[1].vColor = Color4::Yellow;

    UInt iA, iB, iC;
    Vector3 vAB, vAC, vFacetNormal;
    Vertex3 vFacetCenter;
    Scalar fInv3 = MathFn->Invert( 3.0f );

    for( UInt i = 0; i < iTriangleCount; ++i ) {
        pMesh->GetTriangle( i, iA, iB, iC );

        pA = (const Vertex3*)( arrPositions + (iA * iVertexSize) );
        pB = (const Vertex3*)( arrPositions + (iB * iVertexSize) );
        pC = (const Vertex3*)( arrPositions + (iC * iVertexSize) );

        vAB = ( *pB - *pA );
        vAC = ( *pC - *pA );
        vFacetNormal = ( vAB ^ vAC );
        vFacetNormal.Normalize();

        vFacetCenter = *pA + pB->ToVector() + pC->ToVector();
        vFacetCenter *= fInv3;

        vSegment[0].vPosition = trWorld * vFacetCenter;
        vSegment[1].vPosition = trWorld * (vFacetCenter + vFacetNormal);
        RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );
    }
}

Void WorldDebugDraw::_RenderBody_Proxy( const BodyNode * pBody ) const
{
    const Vertex3 & vCenter = pBody->GetWorldShape()->GetAAB().GetCenter();
    const Vector3 & vExtents = pBody->GetWorldShape()->GetAAB().GetExtents();
    if ( pBody->IsSleeping() )
        _RenderAAB( vCenter, vExtents, Color4::Green );
    else
        _RenderAAB( vCenter, vExtents, Color4::Yellow );
}
Void WorldDebugDraw::_RenderBody_Contacts( const BodyNode * pBody ) const
{
    Vertex3 vA, vB, vThird, vTwoThird;
    Vector3 vAB;

    EffectColorVertex vSegment[2];

    const ContactEdge * pContactEdge = pBody->GetContactList();
    while( pContactEdge != NULL ) {
        vA = pContactEdge->GetBodyA()->GetWorldShape()->GetAAB().GetCenter();
        vB = pContactEdge->GetBodyB()->GetWorldShape()->GetAAB().GetCenter();
        vAB = ( vB - vA );
        vThird = vA + (vAB * 0.333f);
        vTwoThird = vA + (vAB * 0.666f);

        vSegment[0].vPosition = vA;
        vSegment[1].vPosition = vThird;
        vSegment[0].vColor = sm_arrIslandPalette[pContactEdge->GetColor() % 27];
        vSegment[1].vColor = sm_arrIslandPalette[pContactEdge->GetColor() % 27];
        RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );

        vSegment[0].vPosition = vThird;
        vSegment[1].vPosition = vTwoThird;
        vSegment[0].vColor = Color4::Blue;
        vSegment[1].vColor = Color4::Blue;
        RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );

        vSegment[0].vPosition = vTwoThird;
        vSegment[1].vPosition = vB;
        vSegment[0].vColor = sm_arrIslandPalette[pContactEdge->GetColor() % 27];
        vSegment[1].vColor = sm_arrIslandPalette[pContactEdge->GetColor() % 27];
        RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );

        pContactEdge = (const ContactEdge *)( pContactEdge->GetNextInNode(pBody) );
    }
}
Void WorldDebugDraw::_RenderBody_Joints( const BodyNode * pBody ) const
{
    Vertex3 vA, vB, vThird, vTwoThird;
    Vector3 vAB;

    Transform3 vJointFrame;
    Vertex3 vOrigin;
    Vector3 vAxisI, vAxisJ, vAxisK;

    EffectColorVertex vSegment[2];

    const JointEdge * pJointEdge = pBody->GetJointList();
    while( pJointEdge != NULL ) {
        vA = pJointEdge->GetBodyA()->GetWorldShape()->GetAAB().GetCenter();
        vB = pJointEdge->GetBodyB()->GetWorldShape()->GetAAB().GetCenter();
        vAB = ( vB - vA );
        vThird = vA + (vAB * 0.333f);
        vTwoThird = vA + (vAB * 0.666f);

        // Joint edge
        vSegment[0].vPosition = vA;
        vSegment[1].vPosition = vThird;
        vSegment[0].vColor = sm_arrIslandPalette[pJointEdge->GetColor() % 27];
        vSegment[1].vColor = sm_arrIslandPalette[pJointEdge->GetColor() % 27];
        RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );

        vSegment[0].vPosition = vThird;
        vSegment[1].vPosition = vTwoThird;
        vSegment[0].vColor = Color4::Cyan;
        vSegment[1].vColor = Color4::Cyan;
        RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );

        vSegment[0].vPosition = vTwoThird;
        vSegment[1].vPosition = vB;
        vSegment[0].vColor = sm_arrIslandPalette[pJointEdge->GetColor() % 27];
        vSegment[1].vColor = sm_arrIslandPalette[pJointEdge->GetColor() % 27];
        RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );

        // Joint frame gizmo
        pJointEdge->GetJoint()->GetFrame( &vJointFrame );
        vOrigin = vJointFrame * Vertex3::Null;
        vAxisI = vJointFrame * Vector3::eI;
        vAxisJ = vJointFrame * Vector3::eJ;
        vAxisK = vJointFrame * Vector3::eK;
        vAxisI.Normalize();
        vAxisJ.Normalize();
        vAxisK.Normalize();

        vSegment[0].vPosition = vOrigin;
        vSegment[1].vPosition = vOrigin + vAxisI;
        vSegment[0].vColor = Color4::Magenta;
        vSegment[1].vColor = Color4::Magenta;
        RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );

        vSegment[0].vPosition = vOrigin;
        vSegment[1].vPosition = vOrigin + vAxisJ;
        vSegment[0].vColor = Color4::Yellow;
        vSegment[1].vColor = Color4::Yellow;
        RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );

        vSegment[0].vPosition = vOrigin;
        vSegment[1].vPosition = vOrigin + vAxisK;
        vSegment[0].vColor = Color4::Cyan;
        vSegment[1].vColor = Color4::Cyan;
        RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );

        pJointEdge = (const JointEdge *)( pJointEdge->GetNextInNode(pBody) );
    }
}
Void WorldDebugDraw::_RenderBody_ContactPoints( const BodyNode * pBody ) const
{
    const ContactManifold * pManifold;
    const ContactPoint * pCP;
    UInt i, iContactPointCount;

    EffectColorVertex vSegment[2];

    const ContactEdge * pContactEdge = pBody->GetContactList();
    while( pContactEdge != NULL ) {
        pManifold = pContactEdge->GetManifold();
        iContactPointCount = pManifold->GetContactPointCount();
        for( i = 0; i < iContactPointCount; ++i ) {
            pCP = pManifold->GetContactPoint( i );
            if ( i == 0 ) { // deepest penetrating point
                vSegment[0].vColor = Color4::Red;
                vSegment[1].vColor = Color4::Red;
            } else {
                vSegment[0].vColor = Color4::Green;
                vSegment[1].vColor = Color4::Green;
            }

            vSegment[0].vPosition = pCP->vWorldPointA - (Vector3::eI * 0.1f);
            vSegment[1].vPosition = pCP->vWorldPointA + (Vector3::eI * 0.1f);
            RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );

            vSegment[0].vPosition = pCP->vWorldPointA - (Vector3::eJ * 0.1f);
            vSegment[1].vPosition = pCP->vWorldPointA + (Vector3::eJ * 0.1f);
            RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );

            vSegment[0].vPosition = pCP->vWorldPointA - (Vector3::eK * 0.1f);
            vSegment[1].vPosition = pCP->vWorldPointA + (Vector3::eK * 0.1f);
            RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );

            vSegment[0].vPosition = pCP->vWorldPointB - (Vector3::eI * 0.1f);
            vSegment[1].vPosition = pCP->vWorldPointB + (Vector3::eI * 0.1f);
            RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );

            vSegment[0].vPosition = pCP->vWorldPointB - (Vector3::eJ * 0.1f);
            vSegment[1].vPosition = pCP->vWorldPointB + (Vector3::eJ * 0.1f);
            RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );

            vSegment[0].vPosition = pCP->vWorldPointB - (Vector3::eK * 0.1f);
            vSegment[1].vPosition = pCP->vWorldPointB + (Vector3::eK * 0.1f);
            RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );
        }

        pContactEdge = (const ContactEdge *)( pContactEdge->GetNextInNode(pBody) );
    }
}

Void WorldDebugDraw::_RenderBody_LinearVelocity( const BodyNode * pBody ) const
{
    EffectColorVertex vSegment[2];
    vSegment[0].vColor = Color4::Red;
    vSegment[1].vColor = Color4::Red;

    vSegment[0].vPosition = pBody->GetPosition();
    vSegment[1].vPosition = pBody->GetPosition() + pBody->GetLinearSpeed();
    RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );
}
Void WorldDebugDraw::_RenderBody_AngularVelocity( const BodyNode * pBody ) const
{
    EffectColorVertex vSegment[2];
    vSegment[0].vColor = Color4::Magenta;
    vSegment[1].vColor = Color4::Magenta;

    vSegment[0].vPosition = pBody->GetPosition();
    vSegment[1].vPosition = pBody->GetPosition() + pBody->GetAngularSpeed();
    RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );
}

Void WorldDebugDraw::_RenderGlobal_Frustrum( const WorldCuller * pCuller ) const
{
    Vertex3 vCenter = Vertex3::Null;
    Vertex3 arrFrustrum[8];
    UInt i;
    for( i = 0; i < 8; ++i ) {
        arrFrustrum[i] = pCuller->GetFrustrumVertex(i);
        vCenter += arrFrustrum[i].ToVector();
    }
    vCenter *= 0.125f; // 1/8

    Vector3 vRay;
    Scalar fScale = 0.25f;
    for( i = 0; i < 8; ++i ) {
        vRay = ( vCenter - arrFrustrum[i] );
        vRay.Normalize();
        arrFrustrum[i] += (vRay * fScale);
    }

    EffectColorVertex vSegment[2];
    vSegment[0].vColor = Color4::White;
    vSegment[1].vColor = Color4::White;

    vSegment[0].vPosition = arrFrustrum[0];
    vSegment[1].vPosition = arrFrustrum[2];
    RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );

    vSegment[0].vPosition = arrFrustrum[2];
    vSegment[1].vPosition = arrFrustrum[3];
    RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );

    vSegment[0].vPosition = arrFrustrum[3];
    vSegment[1].vPosition = arrFrustrum[1];
    RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );

    vSegment[0].vPosition = arrFrustrum[1];
    vSegment[1].vPosition = arrFrustrum[0];
    RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );

    vSegment[0].vPosition = arrFrustrum[4];
    vSegment[1].vPosition = arrFrustrum[6];
    RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );

    vSegment[0].vPosition = arrFrustrum[6];
    vSegment[1].vPosition = arrFrustrum[7];
    RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );

    vSegment[0].vPosition = arrFrustrum[7];
    vSegment[1].vPosition = arrFrustrum[5];
    RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );

    vSegment[0].vPosition = arrFrustrum[5];
    vSegment[1].vPosition = arrFrustrum[4];
    RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );

    vSegment[0].vPosition = arrFrustrum[0];
    vSegment[1].vPosition = arrFrustrum[4];
    RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );

    vSegment[0].vPosition = arrFrustrum[2];
    vSegment[1].vPosition = arrFrustrum[6];
    RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );

    vSegment[0].vPosition = arrFrustrum[3];
    vSegment[1].vPosition = arrFrustrum[7];
    RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );

    vSegment[0].vPosition = arrFrustrum[1];
    vSegment[1].vPosition = arrFrustrum[5];
    RenderingFn->BatchPrimitive( m_idDebugDrawBatch, (const Byte *)vSegment );
}


