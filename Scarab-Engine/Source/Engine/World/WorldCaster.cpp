/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/WorldCaster.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Graph : Casting operations
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : none
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "WorldCaster.h"

#include "WorldManager.h"

/////////////////////////////////////////////////////////////////////////////////
// WorldCaster implementation
WorldCaster::WorldCaster():
    m_vRay(), m_vSphere(),
    m_arrRayCastResults( WORLDCASTER_RESULTS_PAGESIZE ), m_arrSphereCastResults( WORLDCASTER_RESULTS_PAGESIZE )
{
    m_fRayMinDistance = 0.0f;
    m_fRayMaxDistance = 0.0f;

    m_iClosestRayCastResult = INVALID_OFFSET;
    m_iClosestPositiveRayCastResult = INVALID_OFFSET;
    m_iClosestNegativeRayCastResult = INVALID_OFFSET;
    m_bSortedRayCastResults = false;

    m_arrRayCastResults.UseMemoryContext( WorldFn->GetMemoryContext(), TEXT("Scratch") );
    m_arrRayCastResults.Create();

    m_arrSphereCastResults.UseMemoryContext( WorldFn->GetMemoryContext(), TEXT("Scratch") );
    m_arrSphereCastResults.Create();
}
WorldCaster::~WorldCaster()
{
    m_arrSphereCastResults.Destroy();

    m_arrRayCastResults.Destroy();
}

Void WorldCaster::RayCast( WorldBaseNode * pWorldGraph, const Line3 & vRay, Scalar fMinDistance, Scalar fMaxDistance )
{
    Assert( pWorldGraph != NULL );
    Assert( fMinDistance <= fMaxDistance );

    m_vRay = vRay;
    m_fRayMinDistance = fMinDistance;
    m_fRayMaxDistance = fMaxDistance;

    m_iClosestRayCastResult = INVALID_OFFSET - 1;         //
    m_iClosestPositiveRayCastResult = INVALID_OFFSET - 1; // Mark uncomputed
    m_iClosestNegativeRayCastResult = INVALID_OFFSET - 1; //
    m_bSortedRayCastResults = false;

    m_arrRayCastResults.Clear();
    _Rec_RayCast( pWorldGraph );
}

const RayCastResult * WorldCaster::GetClosestRayCastResult()
{
    if ( m_iClosestRayCastResult != (INVALID_OFFSET - 1) ) {
        if ( m_iClosestRayCastResult == INVALID_OFFSET )
            return NULL;
        return &(m_arrRayCastResults[m_iClosestRayCastResult]);
    }

    const UInt iCount = m_arrRayCastResults.Count();
    if ( iCount == 0 ) {
        m_iClosestRayCastResult = INVALID_OFFSET;
        return NULL;
    }

    m_iClosestRayCastResult = INVALID_OFFSET;
    Scalar fMinDistance = SCALAR_INFINITE;
    Scalar fDistance;
    for( UInt i = 0; i < iCount; ++i ) {
        fDistance = MathFn->Abs( m_arrRayCastResults[i].fDistance );
        if ( fDistance < fMinDistance ) {
            fMinDistance = fDistance;
            m_iClosestRayCastResult = i;
        }
    }
    if ( m_iClosestRayCastResult == INVALID_OFFSET )
        return NULL;
    return &(m_arrRayCastResults[m_iClosestRayCastResult]);
}
const RayCastResult * WorldCaster::GetClosestPositiveRayCastResult()
{
    if ( m_iClosestPositiveRayCastResult != (INVALID_OFFSET - 1) ) {
        if ( m_iClosestPositiveRayCastResult == INVALID_OFFSET )
            return NULL;
        return &(m_arrRayCastResults[m_iClosestPositiveRayCastResult]);
    }

    const UInt iCount = m_arrRayCastResults.Count();
    if ( iCount == 0 ) {
        m_iClosestPositiveRayCastResult = INVALID_OFFSET;
        return NULL;
    }

    m_iClosestPositiveRayCastResult = INVALID_OFFSET;
    Scalar fMinDistance = SCALAR_INFINITE;
    Scalar fDistance;
    for( UInt i = 0; i < iCount; ++i ) {
        fDistance = m_arrRayCastResults[i].fDistance;
        if ( fDistance >= 0.0f && fDistance < fMinDistance ) {
            fMinDistance = fDistance;
            m_iClosestPositiveRayCastResult = i;
        }
    }
    if ( m_iClosestPositiveRayCastResult == INVALID_OFFSET )
        return NULL;
    return &(m_arrRayCastResults[m_iClosestPositiveRayCastResult]);
}
const RayCastResult * WorldCaster::GetClosestNegativeRayCastResult()
{
    if ( m_iClosestNegativeRayCastResult != (INVALID_OFFSET - 1) ) {
        if ( m_iClosestNegativeRayCastResult == INVALID_OFFSET )
            return NULL;
        return &(m_arrRayCastResults[m_iClosestNegativeRayCastResult]);
    }

    const UInt iCount = m_arrRayCastResults.Count();
    if ( iCount == 0 ) {
        m_iClosestNegativeRayCastResult = INVALID_OFFSET;
        return NULL;
    }

    m_iClosestNegativeRayCastResult = INVALID_OFFSET;
    Scalar fMinDistance = SCALAR_INFINITE;
    Scalar fDistance;
    for( UInt i = 0; i < iCount; ++i ) {
        fDistance = m_arrRayCastResults[i].fDistance;
        if ( fDistance <= 0.0f && (-fDistance) < fMinDistance ) {
            fMinDistance = (-fDistance);
            m_iClosestNegativeRayCastResult = i;
        }
    }
    if ( m_iClosestNegativeRayCastResult == INVALID_OFFSET )
        return NULL;
    return &(m_arrRayCastResults[m_iClosestNegativeRayCastResult]);
}
Void WorldCaster::SortRayCastResults()
{
    if ( m_bSortedRayCastResults )
        return;

    const UInt iCount = m_arrRayCastResults.Count();
    if ( iCount == 0 )
        return;

    // Sort results
    m_arrRayCastResults.QuickSort( _ResultOrdering_RayCast );

    // Quickly find Closest
    m_iClosestRayCastResult = 0;

    // Quickly find ClosestPositive
    if ( m_iClosestPositiveRayCastResult != INVALID_OFFSET ) {
        m_iClosestPositiveRayCastResult = 0;
        while( m_arrRayCastResults[m_iClosestPositiveRayCastResult].fDistance < 0.0f ) {
            ++m_iClosestPositiveRayCastResult;
            if ( m_iClosestPositiveRayCastResult >= iCount ) {
                m_iClosestPositiveRayCastResult = INVALID_OFFSET;
                break;
            }
        }
    }

    // Quickly find ClosestNegative
    if ( m_iClosestNegativeRayCastResult != INVALID_OFFSET ) {
        m_iClosestNegativeRayCastResult = 0;
        while( m_arrRayCastResults[m_iClosestNegativeRayCastResult].fDistance > 0.0f ) {
            ++m_iClosestNegativeRayCastResult;
            if ( m_iClosestNegativeRayCastResult >= iCount ) {
                m_iClosestNegativeRayCastResult = INVALID_OFFSET;
                break;
            }
        }
    }

    m_bSortedRayCastResults = true;
}

Void WorldCaster::SphereCast( WorldBaseNode * pWorldGraph, const Sphere & vSphere )
{
    Assert( pWorldGraph != NULL );
    Assert( vSphere.Radius > 0.0f );

    m_vSphere = vSphere;

    m_iClosestSphereCastResult = INVALID_OFFSET - 1; // Mark uncomputed
    m_bSortedSphereCastResults = false;

    m_arrSphereCastResults.Clear();
    _Rec_SphereCast( pWorldGraph );
}

const SphereCastResult * WorldCaster::GetClosestSphereCastResult()
{
    if ( m_iClosestSphereCastResult != (INVALID_OFFSET - 1) ) {
        if ( m_iClosestSphereCastResult == INVALID_OFFSET )
            return NULL;
        return &(m_arrSphereCastResults[m_iClosestSphereCastResult]);
    }

    const UInt iCount = m_arrSphereCastResults.Count();
    if ( iCount == 0 ) {
        m_iClosestSphereCastResult = INVALID_OFFSET;
        return NULL;
    }

    m_iClosestSphereCastResult = INVALID_OFFSET;
    Scalar fMinDistance = SCALAR_INFINITE;
    Scalar fDistance;
    for( UInt i = 0; i < iCount; ++i ) {
        fDistance = MathFn->Abs( m_arrSphereCastResults[i].fDistance );
        if ( fDistance < fMinDistance ) {
            fMinDistance = fDistance;
            m_iClosestSphereCastResult = i;
        }
    }
    if ( m_iClosestSphereCastResult == INVALID_OFFSET )
        return NULL;
    return &(m_arrSphereCastResults[m_iClosestSphereCastResult]);
}
Void WorldCaster::SortSphereCastResults()
{
    if ( m_bSortedSphereCastResults )
        return;

    const UInt iCount = m_arrSphereCastResults.Count();
    if ( iCount == 0 )
        return;

    // Sort results
    m_arrSphereCastResults.QuickSort( _ResultOrdering_SphereCast );

    // Quickly find Closest
    m_iClosestSphereCastResult = 0;

    m_bSortedSphereCastResults = true;
}


/////////////////////////////////////////////////////////////////////////////////

Void WorldCaster::_Rec_RayCast( WorldBaseNode * pNode )
{
    // Leaf case
    if ( pNode->IsLeaf() ) {
        WorldLeaf * pLeaf = (WorldLeaf*)pNode;

        // Reject empty geometry
        Mesh * pMesh = pLeaf->GetMesh();
        if ( pMesh == NULL )
            return;

        // Reject non-solid geometry
        if ( !(pMesh->IsTriangleMesh()) )
            return;

        //TriangleMesh * pTriMesh = (TriangleMesh*)pMesh;

        // Reject against leaf's BV
        Scalar fMinDistance, fMaxDistance;
        Bool bHit = pLeaf->WorldBV.RayCast( m_vRay, &fMinDistance, &fMaxDistance );
        if ( !bHit )
            return;
        if ( fMaxDistance < m_fRayMinDistance || fMinDistance > m_fRayMaxDistance )
            return;

        // Report hit
        RayCastResult hResult;
        hResult.pHit = pLeaf;
        hResult.fDistance = (fMinDistance < m_fRayMinDistance) ? fMaxDistance : fMinDistance;
        hResult.iTriangleIndex = INVALID_OFFSET;
        hResult.vTriangleParameters = Vertex3::Null;
        m_arrRayCastResults.Push( hResult );

        // RayCast geometry
        //Matrix4 matInvWorld;
        //pLeaf->WorldTransform.Invert( matInvWorld );

        //Line3 vModelRay;
        //vModelRay.Position = (matInvWorld * m_vRay.Position);
        //vModelRay.Direction = (matInvWorld * m_vRay.Direction);

        //VertexBufferAccessor vba = pTriMesh->MakeVBA();
        //UInt iTriangleCount = pTriMesh->GetTriangleCount();

        //UInt iA, iB, iC;
        //Triangle3 vTriangle;
        //Scalar fLineParam;
        //WorldCastResult hResult;
        //for( UInt i = 0; i < iTriangleCount; ++i ) {
        //    pTriMesh->GetTriangle( i, iA, iB, iC );
        //    vTriangle.A = *( vba.GetPosition<Vertex3>( vba.GetVertex(iA) ) );
        //    vTriangle.B = *( vba.GetPosition<Vertex3>( vba.GetVertex(iB) ) );
        //    vTriangle.C = *( vba.GetPosition<Vertex3>( vba.GetVertex(iC) ) );

        //    // Test triangle
        //    IntersectTriangle3Line3 intTriLine( vTriangle, vModelRay );
        //    bHit = intTriLine.Compute();
        //    if ( !bHit )
        //        continue;
        //    fLineParam = intTriLine.GetLineParameter();
        //    if ( fLineParam < m_fMinDistance || fLineParam > m_fMaxDistance )
        //        continue;

        //    // Report hit
        //    hResult.pHit = pLeaf;
        //    hResult.fDistance = fLineParam;
        //    hResult.iTriangleIndex = i;
        //    hResult.vTriangleParameters = intTriLine.GetTriangleParameters();
        //    m_arrResults.Push( hResult );
        //}

        // Done
        return;
    }

    // Switch case
    if ( pNode->GetType() == WORLDNODE_SWITCH ) {
        WorldSwitchNode * pSwitch = (WorldSwitchNode*)pNode;

        // Reject disabled switch
        const GChar * strActiveChild = pSwitch->GetActiveChild();
        if ( strActiveChild == NULL )
            return;

        // Reject against node's BV
        Scalar fDistance;
        Bool bHit = pSwitch->WorldBV.RayCast( m_vRay, &fDistance );
        if ( !bHit )
            return;
        if ( fDistance < m_fRayMinDistance || fDistance > m_fRayMaxDistance )
            return;

        // Recurse
        WorldBaseNode * pChild = pSwitch->GetChild( strActiveChild );
        _Rec_RayCast( pChild );

        // Done
        return;
    }

    // Node case
    WorldNode * pInnerNode = (WorldNode*)pNode;

    // Reject against node's BV
    Scalar fMinDistance, fMaxDistance;
    Bool bHit = pInnerNode->WorldBV.RayCast( m_vRay, &fMinDistance, &fMaxDistance );
    if ( !bHit )
        return;
    if ( fMaxDistance < m_fRayMinDistance || fMinDistance > m_fRayMaxDistance )
        return;

    // Recurse
    pInnerNode->EnumChildren();
    WorldBaseNode * pChild = pInnerNode->EnumNextChild();
    while( pChild != NULL ) {
        _Rec_RayCast( pChild );
        pChild = pInnerNode->EnumNextChild();
    }
}
Void WorldCaster::_Rec_SphereCast( WorldBaseNode * pNode )
{
    // Leaf case
    if ( pNode->IsLeaf() ) {
        WorldLeaf * pLeaf = (WorldLeaf*)pNode;

        // Reject leaf against sphere
        Vector3 vW = ( pLeaf->WorldTransform.GetTranslate() - m_vSphere.Center.ToVector() );
        Scalar fSqrDistance = vW.NormSqr();
        if ( fSqrDistance > (m_vSphere.Radius * m_vSphere.Radius) )
            return;

        // Report hit
        SphereCastResult hResult;
        hResult.pHit = pLeaf;
        hResult.fDistance = MathFn->Sqrt( fSqrDistance );
        m_arrSphereCastResults.Push( hResult );

        return;
    }

    // Switch case
    if ( pNode->GetType() == WORLDNODE_SWITCH ) {
        WorldSwitchNode * pSwitch = (WorldSwitchNode*)pNode;

        // Reject disabled switch
        const GChar * strActiveChild = pSwitch->GetActiveChild();
        if ( strActiveChild == NULL )
            return;

        // Reject against node's BV
        Scalar fDistance = pSwitch->WorldBV.Distance( m_vSphere.Center );
        if ( fDistance > m_vSphere.Radius )
            return;

        // Recurse
        WorldBaseNode * pChild = pSwitch->GetChild( strActiveChild );
        _Rec_SphereCast( pChild );

        // Done
        return;
    }

    // Node case
    WorldNode * pInnerNode = (WorldNode*)pNode;

    // Reject against node's BV
    Scalar fDistance = pInnerNode->WorldBV.Distance( m_vSphere.Center );
    if ( fDistance > m_vSphere.Radius )
        return;

    // Recurse
    pInnerNode->EnumChildren();
    WorldBaseNode * pChild = pInnerNode->EnumNextChild();
    while( pChild != NULL ) {
        _Rec_SphereCast( pChild );
        pChild = pInnerNode->EnumNextChild();
    }
}

