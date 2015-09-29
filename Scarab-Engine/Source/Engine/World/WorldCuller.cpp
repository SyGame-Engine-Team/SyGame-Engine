/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/WorldCuller.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Graph : Culling operations
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
#include "WorldCuller.h"

#include "WorldManager.h"

/////////////////////////////////////////////////////////////////////////////////
// VisibleGroup implementation
VisibleGroup::VisibleGroup():
    m_arrLeaves( VISIBLEGROUP_PAGESIZE )
{
    m_arrLeaves.UseMemoryContext( WorldFn->GetMemoryContext(), TEXT("VisibleGroups") );
    m_arrLeaves.Create();
}
VisibleGroup::~VisibleGroup()
{
    m_arrLeaves.Destroy();
}

/////////////////////////////////////////////////////////////////////////////////
// WorldCuller implementation
WorldCuller::WorldCuller():
    m_curVisibleGroup()
{
    m_pCamera = NULL;

    m_arrFrustrumState[FRUSTRUM_RIGHT] = 0.0f;
    m_arrFrustrumState[FRUSTRUM_LEFT] = 1.0f;
    m_arrFrustrumState[FRUSTRUM_BOTTOM] = 0.0f;
    m_arrFrustrumState[FRUSTRUM_TOP] = 1.0f;
    m_arrFrustrumState[FRUSTRUM_NEAR] = 0.0f;
    m_arrFrustrumState[FRUSTRUM_FAR] = 1.0f;

    m_iUserPlaneCount = 0;

    m_dwPlaneState = 0;
}
WorldCuller::~WorldCuller()
{
    // nothing to do
}

Void WorldCuller::SetCamera( const Camera * pCamera, Bool bSetFrustrum )
{
    Assert( pCamera != NULL );
    m_pCamera = pCamera;

    if ( bSetFrustrum ) {
        const Scalar * pFrustrum = m_pCamera->GetFrustrum();
        m_arrFrustrumState[FRUSTRUM_RIGHT] = pFrustrum[FRUSTRUM_RIGHT];
        m_arrFrustrumState[FRUSTRUM_LEFT] = pFrustrum[FRUSTRUM_LEFT];
        m_arrFrustrumState[FRUSTRUM_BOTTOM] = pFrustrum[FRUSTRUM_BOTTOM];
        m_arrFrustrumState[FRUSTRUM_TOP] = pFrustrum[FRUSTRUM_TOP];
        m_arrFrustrumState[FRUSTRUM_NEAR] = pFrustrum[FRUSTRUM_NEAR];
        m_arrFrustrumState[FRUSTRUM_FAR] = pFrustrum[FRUSTRUM_FAR];

        m_pCamera->ExtractFrustrumPlanes( m_arrFrustrumPlanes );
        m_pCamera->ExtractFrustrumVertices( m_arrFrustrumVertices );
    }
}

Void WorldCuller::UpdateCamera()
{
    Assert( m_pCamera != NULL );

    const Scalar * pFrustrum = m_pCamera->GetFrustrum();
    m_arrFrustrumState[FRUSTRUM_RIGHT] = pFrustrum[FRUSTRUM_RIGHT];
    m_arrFrustrumState[FRUSTRUM_LEFT] = pFrustrum[FRUSTRUM_LEFT];
    m_arrFrustrumState[FRUSTRUM_BOTTOM] = pFrustrum[FRUSTRUM_BOTTOM];
    m_arrFrustrumState[FRUSTRUM_TOP] = pFrustrum[FRUSTRUM_TOP];
    m_arrFrustrumState[FRUSTRUM_NEAR] = pFrustrum[FRUSTRUM_NEAR];
    m_arrFrustrumState[FRUSTRUM_FAR] = pFrustrum[FRUSTRUM_FAR];

    m_pCamera->ExtractFrustrumPlanes( m_arrFrustrumPlanes );
    m_pCamera->ExtractFrustrumVertices( m_arrFrustrumVertices );
}

Void WorldCuller::GetFrustrumState( Scalar * outFrustrum ) const
{
    outFrustrum[FRUSTRUM_RIGHT] = m_arrFrustrumState[FRUSTRUM_RIGHT];
    outFrustrum[FRUSTRUM_LEFT] = m_arrFrustrumState[FRUSTRUM_LEFT];
    outFrustrum[FRUSTRUM_BOTTOM] = m_arrFrustrumState[FRUSTRUM_BOTTOM];
    outFrustrum[FRUSTRUM_TOP] = m_arrFrustrumState[FRUSTRUM_TOP];
    outFrustrum[FRUSTRUM_NEAR] = m_arrFrustrumState[FRUSTRUM_NEAR];
    outFrustrum[FRUSTRUM_FAR] = m_arrFrustrumState[FRUSTRUM_FAR];
}
Void WorldCuller::SetFrustrumState( const Scalar * pFrustrum )
{
    Assert( m_pCamera != NULL );

    m_arrFrustrumState[FRUSTRUM_RIGHT] = pFrustrum[FRUSTRUM_RIGHT];
    m_arrFrustrumState[FRUSTRUM_LEFT] = pFrustrum[FRUSTRUM_LEFT];
    m_arrFrustrumState[FRUSTRUM_BOTTOM] = pFrustrum[FRUSTRUM_BOTTOM];
    m_arrFrustrumState[FRUSTRUM_TOP] = pFrustrum[FRUSTRUM_TOP];
    m_arrFrustrumState[FRUSTRUM_NEAR] = pFrustrum[FRUSTRUM_NEAR];
    m_arrFrustrumState[FRUSTRUM_FAR] = pFrustrum[FRUSTRUM_FAR];

    m_pCamera->ExtractFrustrumPlanes( m_arrFrustrumPlanes, m_arrFrustrumState );
    m_pCamera->ExtractFrustrumVertices( m_arrFrustrumVertices, m_arrFrustrumState );
}

Bool WorldCuller::IsVisible( const BoundingVolume & testBV )
{
    Assert( m_pCamera != NULL );

    if ( testBV.IsEmpty() )
        return false;

    UInt iPlaneCount = ( FRUSTRUM_COUNT + m_iUserPlaneCount );
    Int iPlane = (Int)(iPlaneCount - 1);
    DWord dwMask = (1ul << iPlane);
    Int iSide;
    while( iPlane >= 0 ) {
        if ( (dwMask & m_dwPlaneState) != 0 ) {
            if ( iPlane < FRUSTRUM_COUNT )
                iSide = testBV.WhichSide( m_arrFrustrumPlanes[iPlane] );
            else
                iSide = testBV.WhichSide( m_arrUserPlanes[iPlane - FRUSTRUM_COUNT] );
            if ( iSide > 0 )
                return false;
            // Disable plane for subsequent children (BV content)
            if ( iSide < 0 )
                m_dwPlaneState &= (~dwMask);
        }
        --iPlane;
        dwMask >>= 1;
    }

    return true;
}
Bool WorldCuller::IsVisible( const Vertex3 * arrPolygon, UInt iPolygonSize, Bool bIgnoreNearPlane ) const
{
    Assert( m_pCamera != NULL );

    UInt iPlaneCount = ( FRUSTRUM_COUNT + m_iUserPlaneCount );
    Int iPlane = (Int)(iPlaneCount - 1);
    UInt iVertex;
    Int iSide;
    while( iPlane >= 0 ) {
        if ( bIgnoreNearPlane && iPlane == FRUSTRUM_NEAR ) {
            --iPlane;
            continue;
        }

        for( iVertex = 0; iVertex < iPolygonSize; ++iVertex ) {
            if ( iPlane < FRUSTRUM_COUNT )
                iSide = m_arrFrustrumPlanes[iPlane].ClassifyPointNP( arrPolygon[iVertex] );
            else
                iSide = m_arrUserPlanes[iPlane - FRUSTRUM_COUNT].ClassifyPointNP( arrPolygon[iVertex] );
            if ( iSide <= 0 )
                break;
        }
        if ( iVertex == iPolygonSize )
            return false;
        --iPlane;
    }

    return true;
}
Int WorldCuller::WhichSide( const Plane & testPlane ) const
{
    Assert( m_pCamera != NULL );

    // Count Positive & Negative vertices using signed distances
    UInt iPositives = 0;
    UInt iNegatives = 0;
    Scalar fSignedDistance;

    fSignedDistance = testPlane.DistanceNP( m_arrFrustrumVertices[0] );
    if ( fSignedDistance > 0.0f )
        ++iPositives;
    else if ( fSignedDistance < 0.0f )
        ++iNegatives;

    fSignedDistance = testPlane.DistanceNP( m_arrFrustrumVertices[1] );
    if ( fSignedDistance > 0.0f )
        ++iPositives;
    else if ( fSignedDistance < 0.0f )
        ++iNegatives;

    fSignedDistance = testPlane.DistanceNP( m_arrFrustrumVertices[2] );
    if ( fSignedDistance > 0.0f )
        ++iPositives;
    else if ( fSignedDistance < 0.0f )
        ++iNegatives;

    fSignedDistance = testPlane.DistanceNP( m_arrFrustrumVertices[3] );
    if ( fSignedDistance > 0.0f )
        ++iPositives;
    else if ( fSignedDistance < 0.0f )
        ++iNegatives;

    fSignedDistance = testPlane.DistanceNP( m_arrFrustrumVertices[4] );
    if ( fSignedDistance > 0.0f )
        ++iPositives;
    else if ( fSignedDistance < 0.0f )
        ++iNegatives;

    fSignedDistance = testPlane.DistanceNP( m_arrFrustrumVertices[5] );
    if ( fSignedDistance > 0.0f )
        ++iPositives;
    else if ( fSignedDistance < 0.0f )
        ++iNegatives;

    fSignedDistance = testPlane.DistanceNP( m_arrFrustrumVertices[6] );
    if ( fSignedDistance > 0.0f )
        ++iPositives;
    else if ( fSignedDistance < 0.0f )
        ++iNegatives;

    fSignedDistance = testPlane.DistanceNP( m_arrFrustrumVertices[7] );
    if ( fSignedDistance > 0.0f )
        ++iPositives;
    else if ( fSignedDistance < 0.0f )
        ++iNegatives;

    // Conclude
    if ( iPositives > 0 ) {
        if ( iNegatives > 0 )
            return 0;
        return +1;
    }
    return -1;
}

Void WorldCuller::ComputeVisibleGroup( WorldBaseNode * pWorldGraph )
{
    Assert( m_pCamera != NULL );

    // All planes active
    m_dwPlaneState = 0xffffffff;

    // Recursive culling
    m_curVisibleGroup.Clear();
    pWorldGraph->_QueryVisibleGroup( *this, false );
}

Void WorldCuller::OnVisibleNode( WorldLeaf * pVisibleLeaf )
{
    // Default behaviour = Append to VisibleGroup
    // You may override to sort the array for minimized render
    // state switches, etc ...
    m_curVisibleGroup.Insert( pVisibleLeaf );
}

