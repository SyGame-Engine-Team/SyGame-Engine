/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/Outdoor/WorldOutdoorArea.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Node : Outdoor, Area
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
#include "WorldOutdoorArea.h"

#include "../WorldManager.h"

/////////////////////////////////////////////////////////////////////////////////
// WorldOutdoorArea implementation
WorldOutdoorArea::WorldOutdoorArea( const Vertex2 & vOrigin, const Vector2 & vSize, UInt iGridWidth, UInt iGridHeight, const GChar * strName ):
    WorldNode( strName ),
	m_vOrigin( vOrigin ), m_vSize( vSize ), m_hAttachedWaypoint()
{
    Assert( iGridWidth > 1 );
    Assert( iGridHeight > 1 );

    // Path-Finding support
    m_iGridWidth = iGridWidth;
    m_iGridHeight = iGridHeight;
    m_iGridSize = ( m_iGridWidth * m_iGridHeight );
    m_vGridCellSize.X = ( vSize.X / (Scalar)iGridWidth );
    m_vGridCellSize.Y = ( vSize.Y / (Scalar)iGridHeight );

    WorldFn->SelectMemory( TEXT("PathData") );
    m_arrCostGrid = New Scalar[m_iGridSize];
    m_arrWaypointGrid = New WorldPathNanoNode[m_iGridSize];
    WorldFn->UnSelectMemory();

    for( UInt i = 0; i < m_iGridSize; ++i )
        m_arrCostGrid[i] = 0.0f;

    _PathFindingGraph_Initialize();
}
WorldOutdoorArea::~WorldOutdoorArea()
{
    // Path-Finding support
    WorldFn->SelectMemory( TEXT("PathData") );
    DeleteA( m_arrWaypointGrid );
    DeleteA( m_arrCostGrid );
    WorldFn->UnSelectMemory();
}

Bool WorldOutdoorArea::GetGridPosition( UInt * outGridX, UInt * outGridY, const Vertex2 & vPosition ) const
{
     // Top-Left convention, round toward -inf
    Vertex2 vLocalPos = ( vPosition - m_vOrigin );
    Bool bClipped = true;

    Int iX = MathFn->Floor( vLocalPos.X / m_vSize.X );
    if ( iX < 0 ) {
        iX = 0;
        bClipped = false;
    } else if ( iX > (Int)(m_iGridWidth - 1) ) {
        iX = (Int)(m_iGridWidth - 1);
        bClipped = false;
    }
    *outGridX = (UInt)iX;

    Int iY = MathFn->Floor( vLocalPos.Y / m_vSize.Y );
    if ( iY < 0 ) {
        iY = 0;
        bClipped = false;
    } else if ( iY > (Int)(m_iGridHeight - 1) ) {
        iY = (Int)(m_iGridHeight - 1);
        bClipped = false;
    }
    *outGridY = (UInt)iY;

    return bClipped;
}

/////////////////////////////////////////////////////////////////////////////////

Void WorldOutdoorArea::_PathFindingGraph_Initialize()
{
    // Setup Waypoint grid
    Vertex3 vA, vB, vC, vD, vCenter;

    UInt iIndex = 0;
    for( UInt iY = 0; iY < m_iGridHeight; ++iY ) {
        for( UInt iX = 0; iX < m_iGridWidth; ++iX ) {
            // Quad
            vA.X = m_vOrigin.X + ( m_vGridCellSize.X * (Scalar)iX );
            vA.Y = m_vOrigin.Y + ( m_vGridCellSize.Y * (Scalar)iY );
            vA.Z = GetHeight( vA );

            vB.X = vA.X + m_vGridCellSize.X;
            vB.Y = vA.Y;
            vB.Z = GetHeight( vB );

            vD.X = vA.X;
            vD.Y = vA.Y + m_vGridCellSize.Y;
            vD.Z = GetHeight( vD );

            vC.X = vB.X;
            vC.Y = vD.Y;
            vC.Z = GetHeight( vC );

            vCenter.X = ( vA.X + vB.X + vC.X + vD.X ) * 0.25f;
            vCenter.Y = ( vA.Y + vB.Y + vC.Y + vD.Y ) * 0.25f;
            vCenter.Z = ( vA.Z + vB.Z + vC.Z + vD.Z ) * 0.25f;

            m_arrWaypointGrid[iIndex].SetPosition( vCenter );
            m_arrWaypointGrid[iIndex].SetCost( 0.0f );
            m_arrWaypointGrid[iIndex].SetGridParent( &m_hAttachedWaypoint );

            // Grid root
            if ( iY == (m_iGridHeight >> 1) && iX == (m_iGridWidth >> 1) ) {
                m_hAttachedWaypoint.SetPosition( vCenter );
                m_hAttachedWaypoint.SetCost( 0.0f );
                m_hAttachedWaypoint.SetGridParent( NULL );
                m_hAttachedWaypoint.SetGridRoot( m_arrWaypointGrid + iIndex );
            }

            ++iIndex;
        }
    }
}
Void WorldOutdoorArea::_PathFindingGraph_Update()
{
    // Update waypoint grid
    Vertex3 vA, vB, vC, vD, vCenter;
    Vector3 vNormal;
    Scalar fSlopeZ, fCost;
    Scalar fAvgCostSum = 0.0f;
    UInt iAvgCostCounter = 0;

    UInt iIndex = 0;
    const Scalar * pCurCost = m_arrCostGrid;
    for( UInt iY = 0; iY < m_iGridHeight; ++iY ) {
        for( UInt iX = 0; iX < m_iGridWidth; ++iX ) {
            // Quad
            vA.X = m_vOrigin.X + ( m_vGridCellSize.X * (Scalar)iX );
            vA.Y = m_vOrigin.Y + ( m_vGridCellSize.Y * (Scalar)iY );
            vA.Z = GetHeight( vA );

            vB.X = vA.X + m_vGridCellSize.X;
            vB.Y = vA.Y;
            vB.Z = GetHeight( vB );

            vD.X = vA.X;
            vD.Y = vA.Y + m_vGridCellSize.Y;
            vD.Z = GetHeight( vD );

            vC.X = vB.X;
            vC.Y = vD.Y;
            vC.Z = GetHeight( vC );

            vCenter.X = ( vA.X + vB.X + vC.X + vD.X ) * 0.25f;
            vCenter.Y = ( vA.Y + vB.Y + vC.Y + vD.Y ) * 0.25f;
            vCenter.Z = ( vA.Z + vB.Z + vC.Z + vD.Z ) * 0.25f;

            m_arrWaypointGrid[iIndex].SetPosition( vCenter );
            if ( *pCurCost == SCALAR_INFINITE )
                m_arrWaypointGrid[iIndex].SetCost( SCALAR_INFINITE );
            else {
                vNormal = ( (vB - vA) ^ (vC - vA) );
                fSlopeZ = ( 1.0f - (vNormal.Z * vNormal.InvNorm()) ); // Normalized Z
                if ( fSlopeZ >= WORLD_OUTDOOR_SLOPE_UNWALKABLE )
                    m_arrWaypointGrid[iIndex].SetCost( SCALAR_INFINITE );
                else {
                    fCost = *pCurCost + ( fSlopeZ * WORLD_OUTDOOR_SLOPE_COST );
                    m_arrWaypointGrid[iIndex].SetCost( fCost );
                    fAvgCostSum += fCost;
                    ++iAvgCostCounter;
                }
            }

            // Grid root
            if ( iY == (m_iGridHeight >> 1) && iX == (m_iGridWidth >> 1) )
                m_hAttachedWaypoint.SetPosition( vCenter );

            ++pCurCost;
            ++iIndex;
        }
    }

    // Update MicroNode cost
    Assert( iAvgCostCounter > 0 );
    Scalar fAvgCost = ( fAvgCostSum / iAvgCostCounter );
    m_hAttachedWaypoint.SetCost( fAvgCost * (SCALAR_SQRT2_2 * (Scalar)(m_iGridWidth + m_iGridHeight)) );
}

