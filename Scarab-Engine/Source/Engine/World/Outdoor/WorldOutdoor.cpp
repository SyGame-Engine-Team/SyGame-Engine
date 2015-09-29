/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/Outdoor/WorldOutdoor.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Node : Outdoor
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
#include "WorldOutdoor.h"

#include "../WorldManager.h"

/////////////////////////////////////////////////////////////////////////////////
// WorldOutdoor implementation
WorldOutdoor::WorldOutdoor( const Vertex2 & vOrigin, const Vector2 & vAreaSize, UInt iViewDistance, const GChar * strName ):
    WorldNode( strName ),
	m_vOrigin( vOrigin ), m_vAreaSize( vAreaSize ), m_vEyeArea()
{
    Assert( iViewDistance > 0 );

    // Initialize visible layout
    const Vertex3 & vEyePos = WorldFn->GetWorldCamera()->GetCamera()->GetViewEye();
    _GetAreaCoords( &m_vEyeArea, vEyePos );

    m_iViewDistance = iViewDistance;
    m_iVisibleAreasPitch = (m_iViewDistance << 1) + 1;

    // Create visible areas
    WorldFn->SelectMemory( TEXT("Outdoor") );
    m_arrVisibleAreas = New WorldOutdoorArea*[m_iVisibleAreasPitch * m_iVisibleAreasPitch];
	WorldFn->UnSelectMemory();

    // User must call _Initialize() right after this !
}
WorldOutdoor::~WorldOutdoor()
{
    // Destroy visible region instances
	WorldFn->SelectMemory( TEXT("Outdoor") );
    
    for( UInt iY = 0; iY < m_iVisibleAreasPitch; ++iY ) {
        for( UInt iX = 0; iX < m_iVisibleAreasPitch; ++iX ) {
            UInt iIndex = _GetVisibleAreaIndex( iX, iY );
            Delete( m_arrVisibleAreas[iIndex] );
        }
    }
    DeleteA( m_arrVisibleAreas );

    WorldFn->UnSelectMemory();
}

WorldOutdoorArea * WorldOutdoor::GetArea( const Point2 & vAreaCoords, UInt * outVisibleX, UInt * outVisibleY ) const
{
    if ( outVisibleX != NULL )
        *outVisibleX = INVALID_OFFSET;
    if ( outVisibleY != NULL )
        *outVisibleY = INVALID_OFFSET;

    UInt iVisibleX = 0, iVisibleY = 0;
    Bool bClipped = _GetVisibleCoords( &iVisibleX, &iVisibleY, vAreaCoords, m_vEyeArea );
    if ( !bClipped )
        return NULL;

    if ( outVisibleX != NULL )
        *outVisibleX = iVisibleX;
    if ( outVisibleY != NULL )
        *outVisibleY = iVisibleY;

    return _GetVisibleArea( iVisibleX, iVisibleY );
}
WorldOutdoorArea * WorldOutdoor::GetVisibleArea( UInt iVisibleX, UInt iVisibleY, Point2 * outAreaCoords ) const
{
    Assert( iVisibleX < m_iVisibleAreasPitch && iVisibleY < m_iVisibleAreasPitch );

    Point2 vAreaCoords;
    _GetVisibleAreaCoords( &vAreaCoords, iVisibleX, iVisibleY, m_vEyeArea );

    if ( outAreaCoords != NULL )
        *outAreaCoords = vAreaCoords;

    return _GetVisibleArea( iVisibleX, iVisibleY );
}

WorldOutdoorArea * WorldOutdoor::GetArea( const Vertex2 & vPosition, Point2 * outAreaCoords, UInt * outVisibleX, UInt * outVisibleY ) const
{
    if ( outVisibleX != NULL )
        *outVisibleX = INVALID_OFFSET;
    if ( outVisibleY != NULL )
        *outVisibleY = INVALID_OFFSET;

    Point2 vAreaCoords;
    _GetAreaCoords( &vAreaCoords, vPosition );

    if ( outAreaCoords != NULL )
        *outAreaCoords = vAreaCoords;
    
    UInt iVisibleX = 0, iVisibleY = 0;
    Bool bClipped = _GetVisibleCoords( &iVisibleX, &iVisibleY, vAreaCoords, m_vEyeArea );
    if ( !bClipped )
        return NULL;

    if ( outVisibleX != NULL )
        *outVisibleX = iVisibleX;
    if ( outVisibleY != NULL )
        *outVisibleY = iVisibleY;

    return _GetVisibleArea( iVisibleX, iVisibleY );
}

Void WorldOutdoor::SetViewDistance( UInt iViewDistance )
{
}

/////////////////////////////////////////////////////////////////////////////////

Void WorldOutdoor::_Initialize()
{
    // Create visible areas
    WorldFn->SelectMemory( TEXT("Outdoor") );

    for( UInt iY = 0; iY < m_iVisibleAreasPitch; ++iY ) {
        for( UInt iX = 0; iX < m_iVisibleAreasPitch; ++iX ) {
            Point2 vAreaCoords;
            _GetVisibleAreaCoords( &vAreaCoords, iX, iY, m_vEyeArea );

            Vertex2 vAreaOrigin = m_vOrigin;
			vAreaOrigin.X += ( m_vAreaSize.X * (Scalar)(vAreaCoords.X) );
			vAreaOrigin.Y += ( m_vAreaSize.Y * (Scalar)(vAreaCoords.Y) );

			GChar strAreaName[256];
			StringFn->Format( strAreaName, TEXT("Area_%d_%d"), iX, iY );

            UInt iIndex = _GetVisibleAreaIndex( iX, iY );
            m_arrVisibleAreas[iIndex] = _CreateArea( vAreaOrigin, m_vAreaSize, strAreaName );
            AddChild( m_arrVisibleAreas[iIndex] );
        }
    }

	WorldFn->UnSelectMemory();

    // First-pass update
    for( UInt iY = 0; iY < m_iVisibleAreasPitch; ++iY ) {
        for( UInt iX = 0; iX < m_iVisibleAreasPitch; ++iX ) {
            Point2 vAreaCoords;
            _GetVisibleAreaCoords( &vAreaCoords, iX, iY, m_vEyeArea );

            _UpdateArea( iX, iY, vAreaCoords );
        }
    }
    for( UInt iY = 0; iY < m_iVisibleAreasPitch; ++iY ) {
        for( UInt iX = 0; iX < m_iVisibleAreasPitch; ++iX )
            _PathFindingGraph_UpdateAdjacency_Pass1( iX, iY, true );
    }
    for( UInt iY = 0; iY < m_iVisibleAreasPitch; ++iY ) {
        for( UInt iX = 0; iX < m_iVisibleAreasPitch; ++iX )
            _PathFindingGraph_UpdateAdjacency_Pass2( iX, iY );
    }
}

Void WorldOutdoor::_OnVisibleGroup( WorldCuller & wCuller, Bool /*bNoCull*/ )
{
    Vertex2 vEyePosition = wCuller.GetCamera()->GetViewEye();
    _OnEyeUpdate( &vEyePosition );

    // Update visible regions
    _UpdateVisibleAreas( vEyePosition );

    // Recurse
    WorldNode::_OnVisibleGroup( wCuller, true );
}

Void WorldOutdoor::_OnEyeUpdate( Vertex2 * /*outEye*/ ) const
{
    // stub method, do nothing
}

/////////////////////////////////////////////////////////////////////////////////

Bool WorldOutdoor::_GetVisibleCoords( UInt * outVisibleX, UInt * outVisibleY, const Point2 & vAreaCoords, const Point2 & vEyeArea ) const
{
    *outVisibleX = INVALID_OFFSET;
    *outVisibleY = INVALID_OFFSET;
    Bool bClipped = true;

    Offset2 vUnclipped = vAreaCoords - ( vEyeArea - (Int)m_iViewDistance );

    if ( vUnclipped.X < 0 ) {
        vUnclipped.X = 0;
        bClipped = false;
    } else if ( vUnclipped.X > (Int)(m_iVisibleAreasPitch - 1) ) {
        vUnclipped.X = (Int)(m_iVisibleAreasPitch - 1);
        bClipped = false;
    }
    *outVisibleX = (UInt)( vUnclipped.X );

    if ( vUnclipped.Y < 0 ) {
        vUnclipped.Y = 0;
        bClipped = false;
    } else if ( vUnclipped.Y > (Int)(m_iVisibleAreasPitch - 1) ) {
        vUnclipped.Y = (Int)(m_iVisibleAreasPitch - 1);
        bClipped = false;
    }
    *outVisibleY = (UInt)( vUnclipped.Y );

    return bClipped;
}

Void WorldOutdoor::_UpdateVisibleAreas( const Vertex2 & vEyePosition )
{
    // Get new eye region
    Point2 vNewEyeArea;
    _GetAreaCoords( &vNewEyeArea, vEyePosition );

    // Compute displacement
    Offset2 vDelta = ( vNewEyeArea - m_vEyeArea );
    if ( vDelta == Offset2::Null )
        return;

    // Warp/Translate cases
    if ( vDelta.X < -1 || vDelta.X > 1 || vDelta.Y < -1 || vDelta.Y > 1 )
        _VisibleAreas_Warp( vNewEyeArea );
    else {
        if ( vDelta.X == 0 )
            _VisibleAreas_TranslateY( vNewEyeArea, vDelta.Y );
        else if ( vDelta.Y == 0 )
            _VisibleAreas_TranslateX( vNewEyeArea, vDelta.X );
        else
            _VisibleAreas_TranslateXY( vNewEyeArea, vDelta.X, vDelta.Y );
    }

    // Update eye area
    m_vEyeArea = vNewEyeArea;
}
Void WorldOutdoor::_VisibleAreas_Warp( const Point2 & vNewEyeArea )
{
    UInt iX, iY;
    Point2 vAreaCoords;

    // Update all regions
    for( iY = 0; iY < m_iVisibleAreasPitch; ++iY ) {
        for( iX = 0; iX < m_iVisibleAreasPitch; ++iX ) {
			_GetVisibleAreaCoords( &vAreaCoords, iX, iY, vNewEyeArea );
            _UpdateArea( iX, iY, vAreaCoords );
        }
    }

    // Adjacency update
    for( iY = 0; iY < m_iVisibleAreasPitch; ++iY ) {
        for( iX = 0; iX < m_iVisibleAreasPitch; ++iX )
            _PathFindingGraph_UpdateAdjacency_Pass1( iX, iY, true );
    }
    for( iY = 0; iY < m_iVisibleAreasPitch; ++iY ) {
        for( iX = 0; iX < m_iVisibleAreasPitch; ++iX )
            _PathFindingGraph_UpdateAdjacency_Pass2( iX, iY );
    }
}
Void WorldOutdoor::_VisibleAreas_TranslateX( const Point2 & vNewEyeArea, Int iDX )
{
    UInt iX, iY;
    Point2 vAreaCoords;

    // Circular-Swap each row
    UInt iVisibleAreasPitchM1 = ( m_iVisibleAreasPitch - 1 );
    UInt iStartX = ( iDX < 0 ) ? iVisibleAreasPitchM1 : 0;
    UInt iEndX = iVisibleAreasPitchM1 - iStartX;

    WorldOutdoorArea ** pSwapScanLine = m_arrVisibleAreas;
	WorldOutdoorArea * pSwapArea;

    for( iY = 0; iY < m_iVisibleAreasPitch; ++iY ) {
        pSwapArea = pSwapScanLine[iStartX];
        for( iX = iStartX; iX != iEndX; iX += iDX )
            pSwapScanLine[iX] = pSwapScanLine[iX + iDX];
        pSwapScanLine[iEndX] = pSwapArea;

        _GetVisibleAreaCoords( &vAreaCoords, iEndX, iY, vNewEyeArea );
        _UpdateArea( iEndX, iY, vAreaCoords );

        pSwapScanLine += m_iVisibleAreasPitch;
    }

    // Adjacency update
    for( iY = 0; iY < m_iVisibleAreasPitch; ++iY ) {
        _PathFindingGraph_UpdateAdjacency_Pass1( iStartX, iY, false );
        _PathFindingGraph_UpdateAdjacency_Pass1( iEndX, iY, true );
        _PathFindingGraph_UpdateAdjacency_Pass1( iEndX - iDX, iY, false );
    }
    for( iY = 0; iY < m_iVisibleAreasPitch; ++iY ) {
        _PathFindingGraph_UpdateAdjacency_Pass2( iStartX, iY );
        _PathFindingGraph_UpdateAdjacency_Pass2( iEndX, iY );
        _PathFindingGraph_UpdateAdjacency_Pass2( iEndX - iDX, iY );
    }
}
Void WorldOutdoor::_VisibleAreas_TranslateY( const Point2 & vNewEyeArea, Int iDY )
{
    UInt iX;
    Point2 vAreaCoords;

    // Circular-Swap each column
    UInt iVisibleAreasPitchM1 = ( m_iVisibleAreasPitch - 1 );
    UInt iStartY = ( iDY < 0 ) ? iVisibleAreasPitchM1 : 0;
    UInt iEndY = iVisibleAreasPitchM1 - iStartY;

    WorldOutdoorArea ** pSwapScanLine;
    WorldOutdoorArea ** pSwapScanLineEnd;
	WorldOutdoorArea * pSwapArea;

    for( iX = 0; iX < m_iVisibleAreasPitch; ++iX ) {
        pSwapScanLine = m_arrVisibleAreas + ( iStartY * m_iVisibleAreasPitch );
        pSwapScanLineEnd = m_arrVisibleAreas + ( iEndY * m_iVisibleAreasPitch );
        
		pSwapArea = pSwapScanLine[iX];
        while( pSwapScanLine != pSwapScanLineEnd ) {
            pSwapScanLine[iX] = (pSwapScanLine + (iDY * m_iVisibleAreasPitch))[iX];
            pSwapScanLine += (iDY * m_iVisibleAreasPitch);
        }
        pSwapScanLineEnd[iX] = pSwapArea;

        _GetVisibleAreaCoords( &vAreaCoords, iX, iEndY, vNewEyeArea );
        _UpdateArea( iX, iEndY, vAreaCoords );
    }

    // Adjacency update
    for( iX = 0; iX < m_iVisibleAreasPitch; ++iX ) {
        _PathFindingGraph_UpdateAdjacency_Pass1( iX, iStartY, false );
        _PathFindingGraph_UpdateAdjacency_Pass1( iX, iEndY, true );
        _PathFindingGraph_UpdateAdjacency_Pass1( iX, iEndY - iDY, false );
    }
    for( iX = 0; iX < m_iVisibleAreasPitch; ++iX ) {
        _PathFindingGraph_UpdateAdjacency_Pass2( iX, iStartY );
        _PathFindingGraph_UpdateAdjacency_Pass2( iX, iEndY );
        _PathFindingGraph_UpdateAdjacency_Pass2( iX, iEndY - iDY );
    }
}
Void WorldOutdoor::_VisibleAreas_TranslateXY( const Point2 & vNewEyeArea, Int iDX, Int iDY )
{
    UInt iX, iY;
    Point2 vAreaCoords;

    // Circular-Swap main diagonal
    UInt iVisibleAreasPitchM1 = ( m_iVisibleAreasPitch - 1 );
    UInt iStartX = ( iDX < 0 ) ? iVisibleAreasPitchM1 : 0;
    UInt iStartY = ( iDY < 0 ) ? iVisibleAreasPitchM1 : 0;
    UInt iEndX = iVisibleAreasPitchM1 - iStartX;
    UInt iEndY = iVisibleAreasPitchM1 - iStartY;

    WorldOutdoorArea ** pSwapScanLine = m_arrVisibleAreas + ( iStartY * m_iVisibleAreasPitch ) + iStartX;
	WorldOutdoorArea * pSwapArea;

	pSwapArea = *pSwapScanLine;
    for( iY = iStartY; iY != iEndY; iY += iDY ) {
        *pSwapScanLine = *( pSwapScanLine + (iDY * m_iVisibleAreasPitch) + iDX );
        pSwapScanLine += ( (iDY * m_iVisibleAreasPitch) + iDX );
    }
    *pSwapScanLine = pSwapArea;

    _GetVisibleAreaCoords( &vAreaCoords, iEndX, iEndY, vNewEyeArea );
    _UpdateArea( iEndX, iEndY, vAreaCoords );

    // Circular-Swap side diagonals
    UInt iDiagCount = ( m_iVisibleAreasPitch - 2 );
    UInt iDiagK;

    for( UInt iDiag = 0; iDiag < iDiagCount; ++iDiag ) {
        iDiagK = ( iDiag + 1 );

        // Top-side diagonal
        if ( iDY < 0 ) {
            iStartX = ( iDX < 0 ) ? (iVisibleAreasPitchM1 - iDiagK) : iDiagK;
            iStartY = iVisibleAreasPitchM1;
            iEndX = ( iDX < 0 ) ? 0 : iVisibleAreasPitchM1;
            iEndY = iDiagK;
        } else {
            iStartX = ( iDX < 0 ) ? iVisibleAreasPitchM1 : 0;
            iStartY = iDiagK;
            iEndX = ( iDX < 0 ) ? iDiagK : (iVisibleAreasPitchM1 - iDiagK);
            iEndY = iVisibleAreasPitchM1;
        }
        pSwapScanLine = m_arrVisibleAreas + ( iStartY * m_iVisibleAreasPitch ) + iStartX;

		pSwapArea = *pSwapScanLine;
        for( iY = iStartY; iY != iEndY; iY += iDY ) {
            *pSwapScanLine = *( pSwapScanLine + (iDY * m_iVisibleAreasPitch) + iDX );
            pSwapScanLine += ( (iDY * m_iVisibleAreasPitch) + iDX );
        }
        *pSwapScanLine = pSwapArea;

        _GetVisibleAreaCoords( &vAreaCoords, iEndX, iEndY, vNewEyeArea );
        _UpdateArea( iEndX, iEndY, vAreaCoords );

        // Down-side diagonal
        if ( iDY < 0 ) {
            iStartX = ( iDX < 0 ) ? iVisibleAreasPitchM1 : 0;
            iStartY = iVisibleAreasPitchM1 - iDiagK;
            iEndX = ( iDX < 0 ) ? iDiagK : (iVisibleAreasPitchM1 - iDiagK);
            iEndY = 0;
        } else {
            iStartX = ( iDX < 0 ) ? (iVisibleAreasPitchM1 - iDiagK) : iDiagK;
            iStartY = 0;
            iEndX = ( iDX < 0 ) ? 0 : iVisibleAreasPitchM1;
            iEndY = iVisibleAreasPitchM1 - iDiagK;
        }
        pSwapScanLine = m_arrVisibleAreas + ( iStartY * m_iVisibleAreasPitch ) + iStartX;

		pSwapArea = *pSwapScanLine;
        for( iY = iStartY; iY != iEndY; iY += iDY ) {
            *pSwapScanLine = *( pSwapScanLine + (iDY * m_iVisibleAreasPitch) + iDX );
            pSwapScanLine += ( (iDY * m_iVisibleAreasPitch) + iDX );
        }
        *pSwapScanLine = pSwapArea;

        _GetVisibleAreaCoords( &vAreaCoords, iEndX, iEndY, vNewEyeArea );
        _UpdateArea( iEndX, iEndY, vAreaCoords );
    }

    // Update side corners
    iX = ( iDX == iDY ) ? iVisibleAreasPitchM1 : 0;
    iY = 0;

    _GetVisibleAreaCoords( &vAreaCoords, iX, iY, vNewEyeArea );
    _UpdateArea( iX, iY, vAreaCoords );

    iX = iVisibleAreasPitchM1 - iX;
    iY = iVisibleAreasPitchM1;

    _GetVisibleAreaCoords( &vAreaCoords, iX, iY, vNewEyeArea );
    _UpdateArea( iX, iY, vAreaCoords );

    // Adjacency update
    iStartX = ( iDX < 0 ) ? iVisibleAreasPitchM1 : 0;
    iStartY = ( iDY < 0 ) ? iVisibleAreasPitchM1 : 0;
    iEndX = iVisibleAreasPitchM1 - iStartX;
    iEndY = iVisibleAreasPitchM1 - iStartY;

        // New perimeter
    for( iX = iStartX; iX != iEndX; iX += iDX ) {
        _PathFindingGraph_UpdateAdjacency_Pass1( iX, iStartY, false );
        _PathFindingGraph_UpdateAdjacency_Pass1( iX, iEndY, true );
    }
    _PathFindingGraph_UpdateAdjacency_Pass1( iEndX, iStartY, true );
    _PathFindingGraph_UpdateAdjacency_Pass1( iEndX, iEndY, true );
    for( iY = iStartY + iDY; iY != iEndY; iY += iDY ) {
        _PathFindingGraph_UpdateAdjacency_Pass1( iStartX, iY, false );
        _PathFindingGraph_UpdateAdjacency_Pass1( iEndX, iY, true );
    }

        // Old perimeter (exclude part already in new perimeter)
    for( iX = iStartX + iDX; iX != iEndX; iX += iDX )
        _PathFindingGraph_UpdateAdjacency_Pass1( iX, iEndY - iDY, false );
    for( iY = iStartY + iDY; iY != (iEndY - iDY); iY += iDY )
        _PathFindingGraph_UpdateAdjacency_Pass1( iEndX - iDX, iY, false );

        // New perimeter
    for( iX = iStartX; iX != iEndX; iX += iDX ) {
        _PathFindingGraph_UpdateAdjacency_Pass2( iX, iStartY );
        _PathFindingGraph_UpdateAdjacency_Pass2( iX, iEndY );
    }
    _PathFindingGraph_UpdateAdjacency_Pass2( iEndX, iStartY );
    _PathFindingGraph_UpdateAdjacency_Pass2( iEndX, iEndY );
    for( iY = iStartY + iDY; iY != iEndY; iY += iDY ) {
        _PathFindingGraph_UpdateAdjacency_Pass2( iStartX, iY );
        _PathFindingGraph_UpdateAdjacency_Pass2( iEndX, iY );
    }

        // Old perimeter (exclude part already in new perimeter)
    for( iX = iStartX + iDX; iX != iEndX; iX += iDX )
        _PathFindingGraph_UpdateAdjacency_Pass2( iX, iEndY - iDY );
    for( iY = iStartY + iDY; iY != (iEndY - iDY); iY += iDY )
        _PathFindingGraph_UpdateAdjacency_Pass2( iEndX - iDX, iY );
}

Void WorldOutdoor::_PathFindingGraph_UpdateAdjacency_Pass1( UInt iVisibleX, UInt iVisibleY, Bool bAreaUpdate )
{
    UInt iX, iY;

    // Update intra-area adjacency
    WorldOutdoorArea * pArea = _GetVisibleArea( iVisibleX, iVisibleY );
    WorldPathNanoNode *pCell, *pAdjacentCell;

    if ( bAreaUpdate ) {
        // Update all cells
        for( iY = 0; iY < pArea->GetGridHeight(); ++iY ) {
            for( iX = 0; iX < pArea->GetGridWidth(); ++iX ) {
                // Quad
                pCell = pArea->GetGridWaypoint( iX, iY );
                pCell->ClearAdjacency();
                if ( pCell->GetCost() == WORLD_PATHFINDER_COST_INFINITE )
                    continue;

                if ( iY > 0 ) {
                    pAdjacentCell = pArea->GetGridWaypoint( iX, iY - 1 );
                    if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE )
                        pCell->AddAdjacentWaypoint( pAdjacentCell );
                }
                if ( iY < (pArea->GetGridHeight() - 1) ) {
                    pAdjacentCell = pArea->GetGridWaypoint( iX, iY + 1 );
                    if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE )
                        pCell->AddAdjacentWaypoint( pAdjacentCell );
                }
                if ( iX > 0 ) {
                    pAdjacentCell = pArea->GetGridWaypoint( iX - 1, iY );
                    if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE )
                        pCell->AddAdjacentWaypoint( pAdjacentCell );
                    if ( iY > 0 ) {
                        pAdjacentCell = pArea->GetGridWaypoint( iX - 1, iY - 1 );
                        if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE )
                            pCell->AddAdjacentWaypoint( pAdjacentCell );
                    }
                    if ( iY < (pArea->GetGridHeight() - 1) ) {
                        pAdjacentCell = pArea->GetGridWaypoint( iX - 1, iY + 1 );
                        if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE )
                            pCell->AddAdjacentWaypoint( pAdjacentCell );
                    }
                }
                if ( iX < (pArea->GetGridWidth() - 1) ) {
                    pAdjacentCell = pArea->GetGridWaypoint( iX + 1, iY );
                    if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE )
                        pCell->AddAdjacentWaypoint( pAdjacentCell );
                    if ( iY > 0 ) {
                        pAdjacentCell = pArea->GetGridWaypoint( iX + 1, iY - 1 );
                        if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE )
                            pCell->AddAdjacentWaypoint( pAdjacentCell );
                    }
                    if ( iY < (pArea->GetGridHeight() - 1) ) {
                        pAdjacentCell = pArea->GetGridWaypoint( iX + 1, iY + 1 );
                        if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE )
                            pCell->AddAdjacentWaypoint( pAdjacentCell );
                    }
                }
            }
        }
    } else {
        // Update only perimeter cells
        for( UInt iX = 0; iX < pArea->GetGridWidth(); ++iX ) {
            // Quads at Y == 0
            pCell = pArea->GetGridWaypoint( iX, 0 );
            pCell->ClearAdjacency();
            if ( pCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE ) {
                pAdjacentCell = pArea->GetGridWaypoint( iX, 1 );
                if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE )
                    pCell->AddAdjacentWaypoint( pAdjacentCell );
                if ( iX > 0 ) {
                    pAdjacentCell = pArea->GetGridWaypoint( iX - 1, 0 );
                    if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE )
                        pCell->AddAdjacentWaypoint( pAdjacentCell );
                    pAdjacentCell = pArea->GetGridWaypoint( iX - 1, 1 );
                    if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE )
                        pCell->AddAdjacentWaypoint( pAdjacentCell );
                }
                if ( iX < (pArea->GetGridWidth() - 1) ) {
                    pAdjacentCell = pArea->GetGridWaypoint( iX + 1, 0 );
                    if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE )
                        pCell->AddAdjacentWaypoint( pAdjacentCell );
                    pAdjacentCell = pArea->GetGridWaypoint( iX + 1, 1 );
                    if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE )
                        pCell->AddAdjacentWaypoint( pAdjacentCell );
                }
            }

            // Quads at Y == Height-1
            pCell = pArea->GetGridWaypoint( iX, pArea->GetGridHeight() - 1 );
            pCell->ClearAdjacency();
            if ( pCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE ) {
                pAdjacentCell = pArea->GetGridWaypoint( iX, pArea->GetGridHeight() - 2 );
                if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE )
                    pCell->AddAdjacentWaypoint( pAdjacentCell );
                if ( iX > 0 ) {
                    pAdjacentCell = pArea->GetGridWaypoint( iX - 1, pArea->GetGridHeight() - 1 );
                    if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE )
                        pCell->AddAdjacentWaypoint( pAdjacentCell );
                    pAdjacentCell = pArea->GetGridWaypoint( iX - 1, pArea->GetGridHeight() - 2 );
                    if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE )
                        pCell->AddAdjacentWaypoint( pAdjacentCell );
                }
                if ( iX < (pArea->GetGridWidth() - 1) ) {
                    pAdjacentCell = pArea->GetGridWaypoint( iX + 1, pArea->GetGridHeight() - 1 );
                    if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE )
                        pCell->AddAdjacentWaypoint( pAdjacentCell );
                    pAdjacentCell = pArea->GetGridWaypoint( iX + 1, pArea->GetGridHeight() - 2 );
                    if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE )
                        pCell->AddAdjacentWaypoint( pAdjacentCell );
                }
            }
        }
        for( UInt iY = 0; iY < pArea->GetGridHeight(); ++iY ) {
            // Quads at X == 0
            pCell = pArea->GetGridWaypoint( 0, iY );
            pCell->ClearAdjacency();
            if ( pCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE ) {
                pAdjacentCell = pArea->GetGridWaypoint( 1, iY );
                if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE )
                    pCell->AddAdjacentWaypoint( pAdjacentCell );
                if ( iY > 0 ) {
                    pAdjacentCell = pArea->GetGridWaypoint( 0, iY - 1 );
                    if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE )
                        pCell->AddAdjacentWaypoint( pAdjacentCell );
                    pAdjacentCell = pArea->GetGridWaypoint( 1, iY - 1 );
                    if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE )
                        pCell->AddAdjacentWaypoint( pAdjacentCell );
                }
                if ( iY < (pArea->GetGridHeight() - 1) ) {
                    pAdjacentCell = pArea->GetGridWaypoint( 0, iY + 1 );
                    if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE )
                        pCell->AddAdjacentWaypoint( pAdjacentCell );
                    pAdjacentCell = pArea->GetGridWaypoint( 1, iY + 1 );
                    if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE )
                        pCell->AddAdjacentWaypoint( pAdjacentCell );
                }
            }

            // Quads at X == Width-1
            pCell = pArea->GetGridWaypoint( pArea->GetGridWidth() - 1, iY );
            pCell->ClearAdjacency();
            if ( pCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE ) {
                pAdjacentCell = pArea->GetGridWaypoint( pArea->GetGridWidth() - 2, iY );
                if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE )
                    pCell->AddAdjacentWaypoint( pAdjacentCell );
                if ( iY > 0 ) {
                    pAdjacentCell = pArea->GetGridWaypoint( pArea->GetGridWidth() - 1, iY - 1 );
                    if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE )
                        pCell->AddAdjacentWaypoint( pAdjacentCell );
                    pAdjacentCell = pArea->GetGridWaypoint( pArea->GetGridWidth() - 2, iY - 1 );
                    if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE )
                        pCell->AddAdjacentWaypoint( pAdjacentCell );
                }
                if ( iY < (pArea->GetGridHeight() - 1) ) {
                    pAdjacentCell = pArea->GetGridWaypoint( pArea->GetGridWidth() - 1, iY + 1 );
                    if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE )
                        pCell->AddAdjacentWaypoint( pAdjacentCell );
                    pAdjacentCell = pArea->GetGridWaypoint( pArea->GetGridWidth() - 2, iY + 1 );
                    if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE )
                        pCell->AddAdjacentWaypoint( pAdjacentCell );
                }
            }
        }
    }

    // Update inter-area adjacency
    UInt iAdjacentX, iAdjacentY;
    Bool bHasPath;

    WorldPathMicroNode * pAreaWaypoint = pArea->GetAttachedWaypoint();
    pAreaWaypoint->ClearAdjacency();

        // Horizontal & Vertical links
    if ( iVisibleX > 0 ) {
        // Left area
        WorldOutdoorArea * pAreaL = _GetVisibleArea( iVisibleX - 1, iVisibleY );
        bHasPath = false;

        // Nano Waypoints
        iX = 0;
        iAdjacentX = ( pAreaL->GetGridWidth() - 1 );
        for( iY = 0; iY < pArea->GetGridHeight(); ++iY ) {
            pCell = pArea->GetGridWaypoint( iX, iY );
            if ( pCell->GetCost() == WORLD_PATHFINDER_COST_INFINITE )
                continue;

            pAdjacentCell = pAreaL->GetGridWaypoint( iAdjacentX, iY );
            if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE ) {
                pCell->AddAdjacentWaypoint( pAdjacentCell );
                bHasPath = true;
            }
            if ( iY > 0 ) {
                pAdjacentCell = pAreaL->GetGridWaypoint( iAdjacentX, iY - 1 );
                if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE ) {
                    pCell->AddAdjacentWaypoint( pAdjacentCell );
                    bHasPath = true;
                }
            }
            if ( iY < (pArea->GetGridHeight() - 1) ) {
                pAdjacentCell = pAreaL->GetGridWaypoint( iAdjacentX, iY + 1 );
                if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE ) {
                    pCell->AddAdjacentWaypoint( pAdjacentCell );
                    bHasPath = true;
                }
            }
        }

        // Micro Waypoints
        if ( bHasPath )
            pAreaWaypoint->AddAdjacentWaypoint( pAreaL->GetAttachedWaypoint() );
    }
    if ( iVisibleX < (m_iVisibleAreasPitch - 1) ) {
        // Right area
        WorldOutdoorArea * pAreaR = _GetVisibleArea( iVisibleX + 1, iVisibleY );
        bHasPath = false;

        // Nano Waypoints
        iX = ( pArea->GetGridWidth() - 1 );
        iAdjacentX = 0;
        for( iY = 0; iY < pArea->GetGridHeight(); ++iY ) {
            pCell = pArea->GetGridWaypoint( iX, iY );
            if ( pCell->GetCost() == WORLD_PATHFINDER_COST_INFINITE )
                continue;

            pAdjacentCell = pAreaR->GetGridWaypoint( iAdjacentX, iY );
            if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE ) {
                pCell->AddAdjacentWaypoint( pAdjacentCell );
                bHasPath = true;
            }
            if ( iY > 0 ) {
                pAdjacentCell = pAreaR->GetGridWaypoint( iAdjacentX, iY - 1 );
                if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE ) {
                    pCell->AddAdjacentWaypoint( pAdjacentCell );
                    bHasPath = true;
                }
            }
            if ( iY < (pArea->GetGridHeight() - 1) ) {
                pAdjacentCell = pAreaR->GetGridWaypoint( iAdjacentX, iY + 1 );
                if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE ) {
                    pCell->AddAdjacentWaypoint( pAdjacentCell );
                    bHasPath = true;
                }
            }
        }

        // Micro Waypoints
        if ( bHasPath )
            pAreaWaypoint->AddAdjacentWaypoint( pAreaR->GetAttachedWaypoint() );
    }
    if ( iVisibleY > 0 ) {
        // Bottom area
        WorldOutdoorArea * pAreaB = _GetVisibleArea( iVisibleX, iVisibleY - 1 );
        bHasPath = false;

        // Nano Waypoints
        iY = 0;
        iAdjacentY = ( pAreaB->GetGridHeight() - 1 );
        for( iX = 0; iX < pArea->GetGridWidth(); ++iX ) {
            pCell = pArea->GetGridWaypoint( iX, iY );
            if ( pCell->GetCost() == WORLD_PATHFINDER_COST_INFINITE )
                continue;

            pAdjacentCell = pAreaB->GetGridWaypoint( iX, iAdjacentY );
            if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE ) {
                pCell->AddAdjacentWaypoint( pAdjacentCell );
                bHasPath = true;
            }
            if ( iX > 0 ) {
                pAdjacentCell = pAreaB->GetGridWaypoint( iX - 1, iAdjacentY );
                if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE ) {
                    pCell->AddAdjacentWaypoint( pAdjacentCell );
                    bHasPath = true;
                }
            }
            if ( iX < (pArea->GetGridWidth() - 1) ) {
                pAdjacentCell = pAreaB->GetGridWaypoint( iX + 1, iAdjacentY );
                if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE ) {
                    pCell->AddAdjacentWaypoint( pAdjacentCell );
                    bHasPath = true;
                }
            }
        }

        // Micro Waypoints
        if ( bHasPath )
            pAreaWaypoint->AddAdjacentWaypoint( pAreaB->GetAttachedWaypoint() );
    }
    if ( iVisibleY < (m_iVisibleAreasPitch - 1) ) {
        // Top area
        WorldOutdoorArea * pAreaT = _GetVisibleArea( iVisibleX, iVisibleY + 1 );
        bHasPath = false;

        // Nano Waypoints
        iY = ( pArea->GetGridHeight() - 1 );
        iAdjacentY = 0;
        for( iX = 0; iX < pArea->GetGridWidth(); ++iX ) {
            pCell = pArea->GetGridWaypoint( iX, iY );
            if ( pCell->GetCost() == WORLD_PATHFINDER_COST_INFINITE )
                continue;

            pAdjacentCell = pAreaT->GetGridWaypoint( iX, iAdjacentY );
            if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE ) {
                pCell->AddAdjacentWaypoint( pAdjacentCell );
                bHasPath = true;
            }
            if ( iX > 0 ) {
                pAdjacentCell = pAreaT->GetGridWaypoint( iX - 1, iAdjacentY );
                if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE ) {
                    pCell->AddAdjacentWaypoint( pAdjacentCell );
                    bHasPath = true;
                }
            }
            if ( iX < (pArea->GetGridWidth() - 1) ) {
                pAdjacentCell = pAreaT->GetGridWaypoint( iX + 1, iAdjacentY );
                if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE ) {
                    pCell->AddAdjacentWaypoint( pAdjacentCell );
                    bHasPath = true;
                }
            }
        }

        // Micro Waypoints
        if ( bHasPath )
            pAreaWaypoint->AddAdjacentWaypoint( pAreaT->GetAttachedWaypoint() );
    }
}
Void WorldOutdoor::_PathFindingGraph_UpdateAdjacency_Pass2( UInt iVisibleX, UInt iVisibleY )
{
    WorldOutdoorArea * pArea = _GetVisibleArea( iVisibleX, iVisibleY );
    WorldPathMicroNode * pAreaWaypoint = pArea->GetAttachedWaypoint();

    WorldPathMicroNode *pAdjacentAreaWaypoint, *pCornerAreaWaypoint;
    WorldPathNanoNode *pCell, *pAdjacentCell;
    Bool bHasPath;

    // Update inter-area adjacency
        // Diagonal links
    if ( iVisibleX < (m_iVisibleAreasPitch - 1) ) {
        if ( iVisibleY < (m_iVisibleAreasPitch - 1) ) {
            // Right-Top Area
            WorldOutdoorArea * pAreaRT = _GetVisibleArea( iVisibleX + 1, iVisibleY + 1 );
            pCornerAreaWaypoint = pAreaRT->GetAttachedWaypoint();
            bHasPath = false;

            // Nano Waypoints
            pCell = pArea->GetGridWaypoint( pArea->GetGridWidth() - 1, pArea->GetGridHeight() - 1 );
            if ( pCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE ) {
                pAdjacentCell = pAreaRT->GetGridWaypoint( 0, 0 );
                if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE ) {
                    pCell->AddAdjacentWaypoint( pAdjacentCell );
                    bHasPath = true;
                }
            }

            // Micro Waypoints
            if ( !bHasPath ) {
                // Check Right border
                WorldOutdoorArea * pAreaR = _GetVisibleArea( iVisibleX + 1, iVisibleY );
                pAdjacentAreaWaypoint = pAreaR->GetAttachedWaypoint();
                bHasPath = pAreaWaypoint->HasAdjacentWaypoint( pAdjacentAreaWaypoint );
                if ( !bHasPath ) {
                    // Check Top border
                    WorldOutdoorArea * pAreaT = _GetVisibleArea( iVisibleX, iVisibleY + 1 );
                    pAdjacentAreaWaypoint = pAreaT->GetAttachedWaypoint();
                    bHasPath = pAreaWaypoint->HasAdjacentWaypoint( pAdjacentAreaWaypoint );
                }
                if ( bHasPath )
                    bHasPath = pAdjacentAreaWaypoint->HasAdjacentWaypoint( pCornerAreaWaypoint );
            }

            if ( bHasPath )
                pAreaWaypoint->AddAdjacentWaypoint( pCornerAreaWaypoint );
        }
        if ( iVisibleY > 0 ) {
            // Right-Bottom Area
            WorldOutdoorArea * pAreaRB = _GetVisibleArea( iVisibleX + 1, iVisibleY - 1 );
            pCornerAreaWaypoint = pAreaRB->GetAttachedWaypoint();
            bHasPath = false;
            
            // Nano Waypoints
            pCell = pArea->GetGridWaypoint( pArea->GetGridWidth() - 1, 0 );
            if ( pCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE ) {
                pAdjacentCell = pAreaRB->GetGridWaypoint( 0, pAreaRB->GetGridHeight() - 1 );
                if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE ) {
                    pCell->AddAdjacentWaypoint( pAdjacentCell );
                    bHasPath = true;
                }
            }

            // Micro Waypoints
            if ( !bHasPath ) {
                // Check Bottom Border
                WorldOutdoorArea * pAreaB = _GetVisibleArea( iVisibleX, iVisibleY - 1 );
                pAdjacentAreaWaypoint = pAreaB->GetAttachedWaypoint();
                bHasPath = pAreaWaypoint->HasAdjacentWaypoint( pAdjacentAreaWaypoint );
                if ( !bHasPath ) {
                    // Check Right border
                    WorldOutdoorArea * pAreaR = _GetVisibleArea( iVisibleX + 1, iVisibleY );
                    pAdjacentAreaWaypoint = pAreaR->GetAttachedWaypoint();
                    bHasPath = pAreaWaypoint->HasAdjacentWaypoint( pAdjacentAreaWaypoint );
                }
                if ( bHasPath )
                    bHasPath = pAdjacentAreaWaypoint->HasAdjacentWaypoint( pCornerAreaWaypoint );
            }

            if ( bHasPath )
                pAreaWaypoint->AddAdjacentWaypoint( pCornerAreaWaypoint );
        }
    }
    if ( iVisibleX > 0 ) {
        if ( iVisibleY < (m_iVisibleAreasPitch - 1) ) {
            // Left-Top Area
            WorldOutdoorArea * pAreaLT = _GetVisibleArea( iVisibleX - 1, iVisibleY + 1 );
            pCornerAreaWaypoint = pAreaLT->GetAttachedWaypoint();
            bHasPath = false;

            // Nano Waypoints
            pCell = pArea->GetGridWaypoint( 0, pArea->GetGridHeight() - 1 );
            if ( pCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE ) {
                pAdjacentCell = pAreaLT->GetGridWaypoint( pAreaLT->GetGridWidth() - 1, 0 );
                if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE ) {
                    pCell->AddAdjacentWaypoint( pAdjacentCell );
                    bHasPath = true;
                }
            }

            // Micro Waypoints
            if ( !bHasPath ) {
                // Check Top Border
                WorldOutdoorArea * pAreaT = _GetVisibleArea( iVisibleX, iVisibleY + 1 );
                pAdjacentAreaWaypoint = pAreaT->GetAttachedWaypoint();
                bHasPath = pAreaWaypoint->HasAdjacentWaypoint( pAdjacentAreaWaypoint );
                if ( !bHasPath ) {
                    // Check Left border
                    WorldOutdoorArea * pAreaL = _GetVisibleArea( iVisibleX - 1, iVisibleY );
                    pAdjacentAreaWaypoint = pAreaL->GetAttachedWaypoint();
                    bHasPath = pAreaWaypoint->HasAdjacentWaypoint( pAdjacentAreaWaypoint );
                }
                if ( bHasPath )
                    bHasPath = pAdjacentAreaWaypoint->HasAdjacentWaypoint( pCornerAreaWaypoint );
            }
            
            if ( bHasPath )
                pAreaWaypoint->AddAdjacentWaypoint( pCornerAreaWaypoint );
        }
        if ( iVisibleY > 0 ) {
            // Left-Bottom Area
            WorldOutdoorArea * pAreaLB = _GetVisibleArea( iVisibleX - 1, iVisibleY - 1 );
            pCornerAreaWaypoint = pAreaLB->GetAttachedWaypoint();
            bHasPath = false;
            
            // Nano Waypoints
            pCell = pArea->GetGridWaypoint( 0, 0 );
            if ( pCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE ) {
                pAdjacentCell = pAreaLB->GetGridWaypoint( pAreaLB->GetGridWidth() - 1, pAreaLB->GetGridHeight() - 1 );
                if ( pAdjacentCell->GetCost() != WORLD_PATHFINDER_COST_INFINITE ) {
                    pCell->AddAdjacentWaypoint( pAdjacentCell );
                    bHasPath = true;
                }
            }

            // Micro Waypoints
            if ( !bHasPath ) {
                // Check Top Border
                WorldOutdoorArea * pAreaL = _GetVisibleArea( iVisibleX - 1, iVisibleY );
                pAdjacentAreaWaypoint = pAreaL->GetAttachedWaypoint();
                bHasPath = pAreaWaypoint->HasAdjacentWaypoint( pAdjacentAreaWaypoint );
                if ( !bHasPath ) {
                    // Check Left border
                    WorldOutdoorArea * pAreaB = _GetVisibleArea( iVisibleX, iVisibleY - 1 );
                    pAdjacentAreaWaypoint = pAreaB->GetAttachedWaypoint();
                    bHasPath = pAreaWaypoint->HasAdjacentWaypoint( pAdjacentAreaWaypoint );
                }
                if ( bHasPath )
                    bHasPath = pAdjacentAreaWaypoint->HasAdjacentWaypoint( pCornerAreaWaypoint );
            }

            if ( bHasPath )
                pAreaWaypoint->AddAdjacentWaypoint( pCornerAreaWaypoint );
        }
    }
}
