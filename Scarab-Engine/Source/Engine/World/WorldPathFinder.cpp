/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/WorldPathFinder.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Graph : PathFinding operations
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
#include "WorldPathFinder.h"

#include "WorldManager.h"

/////////////////////////////////////////////////////////////////////////////////
// WorldPathWaypoint implementation
WorldPathWaypoint::WorldPathWaypoint():
    m_vPosition()
{
    m_vPosition = Vertex3::Null;

    m_fCost = 1.0f;
}
WorldPathWaypoint::~WorldPathWaypoint()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// WorldPathMacroNode implementation
WorldPathMacroNode::WorldPathMacroNode():
    WorldPathWaypoint()
{
    m_iAdjacencyCount = 0;
    for( UInt i = 0; i < WORLD_PATHFINDER_MAX_ADJACENCY_MACRO; ++i )
        m_arrAdjacentWaypoints[i] = NULL;

    m_pGridRoot = NULL;
}
WorldPathMacroNode::~WorldPathMacroNode()
{
    // nothing to do
}

Bool WorldPathMacroNode::HasAdjacentWaypoint( WorldPathWaypoint * pAdjacentWaypoint ) const
{
    for( UInt i = 0; i < m_iAdjacencyCount; ++i ) {
        if ( m_arrAdjacentWaypoints[i] == pAdjacentWaypoint )
            return true;
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////////
// WorldPathMicroNode implementation
WorldPathMicroNode::WorldPathMicroNode():
    WorldPathWaypoint()
{
    m_iAdjacencyCount = 0;
    for( UInt i = 0; i < WORLD_PATHFINDER_MAX_ADJACENCY_MICRO; ++i )
        m_arrAdjacentWaypoints[i] = NULL;

    m_pGridParent = NULL;
    m_pGridRoot = NULL;
}
WorldPathMicroNode::~WorldPathMicroNode()
{
    // nothing to do
}

Bool WorldPathMicroNode::HasAdjacentWaypoint( WorldPathWaypoint * pAdjacentWaypoint ) const
{
    for( UInt i = 0; i < m_iAdjacencyCount; ++i ) {
        if ( m_arrAdjacentWaypoints[i] == pAdjacentWaypoint )
            return true;
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////////
// WorldPathNanoNode implementation
WorldPathNanoNode::WorldPathNanoNode():
    WorldPathWaypoint()
{
    m_iAdjacencyCount = 0;
    for( UInt i = 0; i < WORLD_PATHFINDER_MAX_ADJACENCY_NANO; ++i )
        m_arrAdjacentWaypoints[i] = NULL;

    m_pGridParent = NULL;
}
WorldPathNanoNode::~WorldPathNanoNode()
{
    // nothing to do
}

Bool WorldPathNanoNode::HasAdjacentWaypoint( WorldPathWaypoint * pAdjacentWaypoint ) const
{
    for( UInt i = 0; i < m_iAdjacencyCount; ++i ) {
        if ( m_arrAdjacentWaypoints[i] == pAdjacentWaypoint )
            return true;
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////////
// WaypointHeap implementation
WaypointHeap::WaypointHeap():
    m_arrNodes()
{
    // nothing to do
}
WaypointHeap::~WaypointHeap()
{
    // nothing to do
}

Void WaypointHeap::Create()
{
    m_arrNodes.UseMemoryContext( WorldFn->GetMemoryContext(), TEXT("PathData") );
    m_arrNodes.Create();

    // Bury index 0
    m_arrNodes.Push( NULL );
}
Void WaypointHeap::Destroy()
{
    m_arrNodes.Destroy();
}

Void WaypointHeap::Merge( WaypointState * pWaypointState )
{
    pWaypointState->iHeapNode = m_arrNodes.Count();
    m_arrNodes.Push( pWaypointState );

    _Heap_UpdateUp( pWaypointState->iHeapNode );
}
WaypointState * WaypointHeap::Extract( Bool bRemove )
{
    UInt iHeapSize = ( m_arrNodes.Count() - 1 );
    Assert( iHeapSize > 0 );

	WaypointState * pWaypointState = m_arrNodes[1];
	if ( bRemove ) {
		if ( iHeapSize == 1 )
			m_arrNodes.Pop( NULL, 1 );
        else {
		    m_arrNodes[1] = m_arrNodes[iHeapSize];
            m_arrNodes[1]->iHeapNode = 1;
            m_arrNodes.Pop( NULL, 1 );
		    _Heap_UpdateDown( 1 );
        }
	}

    return pWaypointState;
}
Void WaypointHeap::UpdateUp( WaypointState * pWaypointState )
{
    UInt iHeapSize = ( m_arrNodes.Count() - 1 );
    Assert( iHeapSize > 0 );

    _Heap_UpdateUp( pWaypointState->iHeapNode );
}

/////////////////////////////////////////////////////////////////////////////////

Void WaypointHeap::_Heap_UpdateUp( UInt iNode )
{
    if ( iNode == 1 )
        return;

    UInt iParent = ( iNode >> 1 );

    Int iCmp = _Compare( m_arrNodes[iNode], m_arrNodes[iParent] );
    if ( iCmp <= 0 )
        return;

    WaypointState * pSwap = m_arrNodes[iNode];
    m_arrNodes[iNode] = m_arrNodes[iParent];
    m_arrNodes[iParent] = pSwap;
    m_arrNodes[iNode]->iHeapNode = iNode;
    m_arrNodes[iParent]->iHeapNode = iParent;

    _Heap_UpdateUp( iParent );
}
Void WaypointHeap::_Heap_UpdateDown( UInt iNode )
{
    UInt iNodeCount = ( m_arrNodes.Count() - 1 );
    if ( iNode > (iNodeCount >> 1) )
        return;

    UInt iLeftChild = ( iNode << 1 );
    UInt iRightChild = iLeftChild + 1;
    UInt iMinChild;

    Int iCmp;
    if ( iRightChild > iNodeCount )
        iMinChild = iLeftChild;
    else {
        iCmp = _Compare( m_arrNodes[iLeftChild], m_arrNodes[iRightChild] );
        iMinChild = ( iCmp >= 0 ) ? iLeftChild : iRightChild;
    }

    iCmp = _Compare( m_arrNodes[iMinChild], m_arrNodes[iNode] );
    if ( iCmp <= 0 )
        return;

    WaypointState * pSwap = m_arrNodes[iNode];
    m_arrNodes[iNode] = m_arrNodes[iMinChild];
    m_arrNodes[iMinChild] = pSwap;
    m_arrNodes[iNode]->iHeapNode = iNode;
    m_arrNodes[iMinChild]->iHeapNode = iMinChild;

    _Heap_UpdateDown( iMinChild );
}

/////////////////////////////////////////////////////////////////////////////////
// WorldPathFinder implementation
WorldPathFinder::PathFinderHeuristicFunction WorldPathFinder::m_arrHeuristics[PATHFIND_HEURISTIC_COUNT] = {
    WorldPathFinder::_Heuristic_Distance,
    WorldPathFinder::_Heuristic_DistanceSquared,
    WorldPathFinder::_Heuristic_DistanceManhattan,
};

WorldPathFinder::WorldPathFinder():
    m_arrSearchInstances()
{
    m_arrSearchInstances.UseMemoryContext( WorldFn->GetMemoryContext(), TEXT("PathData") );
    m_arrSearchInstances.Create();

    m_arrHierarchicSearchInstances.UseMemoryContext( WorldFn->GetMemoryContext(), TEXT("PathData") );
    m_arrHierarchicSearchInstances.Create();

    _SearchInstance_Init();
    _HierarchicSearchInstance_Init();

    m_pfHeuristicCost = m_arrHeuristics[PATHFIND_HEURISTIC_DISTANCE_SQR];
}
WorldPathFinder::~WorldPathFinder()
{
    for( UInt i = 0; i < m_arrHierarchicSearchInstances.Count(); ++i ) {
        if ( _HierarchicSearchInstance_IsAllocated(i) )
            EndSearch( i | (SearchInstanceID)(m_arrHierarchicSearchInstances[i].iType) );
    }
    m_arrHierarchicSearchInstances.Destroy();

    for( UInt i = 0; i < m_arrSearchInstances.Count(); ++i ) {
        if ( _SearchInstance_IsAllocated(i) )
            EndSearch( i | PATHFIND_TYPE_SIMPLE );
    }
    m_arrSearchInstances.Destroy();
}

Bool WorldPathFinder::IsFinished( SearchInstanceID idSearch ) const
{
    PathFindType iType = (PathFindType)( idSearch & 0xc0000000 );
    idSearch &= 0x3fffffff;
    switch( iType ) {
        case PATHFIND_TYPE_SIMPLE:
            Assert( _SearchInstance_IsAllocated(idSearch) );
            return m_arrSearchInstances[idSearch].bFinished;
        case PATHFIND_TYPE_HIERARCHIC_2:
        case PATHFIND_TYPE_HIERARCHIC_3:
            Assert( _HierarchicSearchInstance_IsAllocated(idSearch) );
            return m_arrHierarchicSearchInstances[idSearch].bFinished;
        default: Assert(false); return false;
    }
}
Bool WorldPathFinder::IsPathFound( SearchInstanceID idSearch ) const
{
    PathFindType iType = (PathFindType)( idSearch & 0xc0000000 );
    idSearch &= 0x3fffffff;
    switch( iType ) {
        case PATHFIND_TYPE_SIMPLE:
            Assert( _SearchInstance_IsAllocated(idSearch) );
            return m_arrSearchInstances[idSearch].bPathFound;
        case PATHFIND_TYPE_HIERARCHIC_2:
        case PATHFIND_TYPE_HIERARCHIC_3:
            Assert( _HierarchicSearchInstance_IsAllocated(idSearch) );
            return m_arrHierarchicSearchInstances[idSearch].bPathFound;
        default: Assert(false); return false;
    }
}
UInt WorldPathFinder::GetIterationCount( SearchInstanceID idSearch ) const
{
    PathFindType iType = (PathFindType)( idSearch & 0xc0000000 );
    idSearch &= 0x3fffffff;
    switch( iType ) {
        case PATHFIND_TYPE_SIMPLE:
            Assert( _SearchInstance_IsAllocated(idSearch) );
            return m_arrSearchInstances[idSearch].iIterationCount;
        case PATHFIND_TYPE_HIERARCHIC_2:
        case PATHFIND_TYPE_HIERARCHIC_3:
            Assert( _HierarchicSearchInstance_IsAllocated(idSearch) );
            return m_arrHierarchicSearchInstances[idSearch].iIterationCount;
        default: Assert(false); return false;
    }
}
    
WorldPathWaypoint * WorldPathFinder::GetStart( SearchInstanceID idSearch ) const
{
    PathFindType iType = (PathFindType)( idSearch & 0xc0000000 );
    idSearch &= 0x3fffffff;
    switch( iType ) {
        case PATHFIND_TYPE_SIMPLE:
            Assert( _SearchInstance_IsAllocated(idSearch) );
            return m_arrSearchInstances[idSearch].pStart;
        case PATHFIND_TYPE_HIERARCHIC_2:
        case PATHFIND_TYPE_HIERARCHIC_3:
            Assert( _HierarchicSearchInstance_IsAllocated(idSearch) );
            return m_arrHierarchicSearchInstances[idSearch].pStart;
        default: Assert(false); return false;
    }
}
WorldPathWaypoint * WorldPathFinder::GetTarget( SearchInstanceID idSearch ) const
{
    PathFindType iType = (PathFindType)( idSearch & 0xc0000000 );
    idSearch &= 0x3fffffff;
    switch( iType ) {
        case PATHFIND_TYPE_SIMPLE:
            Assert( _SearchInstance_IsAllocated(idSearch) );
            return m_arrSearchInstances[idSearch].pTarget;
        case PATHFIND_TYPE_HIERARCHIC_2:
        case PATHFIND_TYPE_HIERARCHIC_3:
            Assert( _HierarchicSearchInstance_IsAllocated(idSearch) );
            return m_arrHierarchicSearchInstances[idSearch].pTarget;
        default: Assert(false); return false;
    }
}

SearchInstanceID WorldPathFinder::StartSearch( WorldPathWaypoint * pStart, WorldPathWaypoint * pTarget, PathFindType iType )
{
    SearchInstanceID idSearch = INVALID_OFFSET;
    switch( iType ) {
        case PATHFIND_TYPE_SIMPLE:       idSearch = _SimpleSearch_Start( pStart, pTarget ); break;
        case PATHFIND_TYPE_HIERARCHIC_2: idSearch = _Hierarchic2Search_Start( pStart, pTarget ); break;
        case PATHFIND_TYPE_HIERARCHIC_3: idSearch = _Hierarchic3Search_Start( pStart, pTarget ); break;
        default: Assert(false); return INVALID_OFFSET;
    }
    return ( idSearch | (SearchInstanceID)iType );
}
Void WorldPathFinder::EndSearch( SearchInstanceID idSearch )
{
    PathFindType iType = (PathFindType)( idSearch & 0xc0000000 );
    idSearch &= 0x3fffffff;
    switch( iType ) {
        case PATHFIND_TYPE_SIMPLE:       _SimpleSearch_End( idSearch ); break;
        case PATHFIND_TYPE_HIERARCHIC_2: _Hierarchic2Search_End( idSearch ); break;
        case PATHFIND_TYPE_HIERARCHIC_3: _Hierarchic3Search_End( idSearch ); break;
        default: Assert(false); break;
    }
}
Bool WorldPathFinder::UpdateSearch( SearchInstanceID idSearch, UInt iIterationCount )
{
    PathFindType iType = (PathFindType)( idSearch & 0xc0000000 );
    idSearch &= 0x3fffffff;
    switch( iType ) {
        case PATHFIND_TYPE_SIMPLE:       return _SimpleSearch_Update( idSearch, iIterationCount );
        case PATHFIND_TYPE_HIERARCHIC_2: return _Hierarchic2Search_Update( idSearch, iIterationCount );
        case PATHFIND_TYPE_HIERARCHIC_3: return _Hierarchic3Search_Update( idSearch, iIterationCount );
        default: Assert(false); return false;
    }
}
Bool WorldPathFinder::GetPath( SearchInstanceID idSearch, const Array<WorldPathWaypoint*> ** outPath )
{
    PathFindType iType = (PathFindType)( idSearch & 0xc0000000 );
    idSearch &= 0x3fffffff;

    switch( iType ) {
        case PATHFIND_TYPE_SIMPLE: {
                Assert( _SearchInstance_IsAllocated(idSearch) );
                _SearchInstance * pSearch = ( m_arrSearchInstances + idSearch );
                if ( outPath != NULL ) {
                    if ( !(pSearch->bFinished) )
                        _SimpleSearch_BuildPath( idSearch );
                    *outPath = &(pSearch->arrPath);
                }
                return pSearch->bPathFound;
            } break;
        case PATHFIND_TYPE_HIERARCHIC_2: {
                Assert( _HierarchicSearchInstance_IsAllocated(idSearch) );
                _HierarchicSearchInstance * pSearch = ( m_arrHierarchicSearchInstances + idSearch );
                if ( outPath != NULL ) {
                    if ( !(pSearch->bFinished) )
                        _Hierarchic2Search_BuildPath( idSearch );
                    *outPath = &(pSearch->arrPath);
                }
                return pSearch->bPathFound;
            } break;
        case PATHFIND_TYPE_HIERARCHIC_3: {
                Assert( _HierarchicSearchInstance_IsAllocated(idSearch) );
                _HierarchicSearchInstance * pSearch = ( m_arrHierarchicSearchInstances + idSearch );
                if ( outPath != NULL ) {
                    if ( !(pSearch->bFinished) )
                        _Hierarchic3Search_BuildPath( idSearch );
                    *outPath = &(pSearch->arrPath);
                }
                return pSearch->bPathFound;
            } break;
        default: Assert(false); break;
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////////

SearchInstanceID WorldPathFinder::_SimpleSearch_Start( WorldPathWaypoint * pStart, WorldPathWaypoint * pTarget )
{
    // Create a search instance
    SearchInstanceID idSearch = _SearchInstance_Alloc();
    _SearchInstance * pSearch = ( m_arrSearchInstances + idSearch );
    Assert( pSearch->idSearch == idSearch );

    pSearch->iIterationCount = 0;
    pSearch->bFinished = false;
    pSearch->bPathFound = false;

    pSearch->pStart = pStart;
    pSearch->pTarget = pTarget;

    pSearch->hExploreMap.SetComparator( _Compare_Waypoints, NULL );
    pSearch->hExploreMap.UseMemoryContext( WorldFn->GetMemoryContext(), TEXT("PathData") );
    pSearch->hExploreMap.Create();

    pSearch->hOpenList.Create();
    pSearch->hClosedList.Create();

    pSearch->arrPath.UseMemoryContext( WorldFn->GetMemoryContext(), TEXT("PathData") );
    pSearch->arrPath.Create();

    // Initialize the search
        // New waypoint (start)
    WaypointState hWaypointState;
    hWaypointState.pWaypoint = pStart;

    hWaypointState.iHeapNode = INVALID_OFFSET;
    hWaypointState.bInOpenList = false;
    hWaypointState.bInClosedList = false;

    hWaypointState.pPathParent = NULL;

    hWaypointState.fCurrentCost = 0.0f;
    hWaypointState.fEstimatedCost = m_pfHeuristicCost( pStart, pTarget );
    hWaypointState.fPathCost = ( hWaypointState.fCurrentCost + hWaypointState.fEstimatedCost );

        // Add to explore map
    _WaypointMap::Iterator itWaypoint;
    Bool bInserted = pSearch->hExploreMap.Insert( pStart, hWaypointState, &itWaypoint );
    Assert( bInserted && !(itWaypoint.IsNull()) );

    WaypointState * pWaypointState = itWaypoint.GetItemPtr();

        // Add to open list
    pSearch->hOpenList.Merge( pWaypointState );
    pWaypointState->bInOpenList = true;

    // Warm-Up
    _SimpleSearch_Update( idSearch, WORLD_PATHFINDER_SEARCH_WARMUP );

    // Done
    return idSearch;
}
Void WorldPathFinder::_SimpleSearch_End( SearchInstanceID idSearch )
{
    Assert( _SearchInstance_IsAllocated(idSearch) );

    _SearchInstance * pSearch = ( m_arrSearchInstances + idSearch );
    if ( !(pSearch->bFinished) ) {
        pSearch->hOpenList.Destroy();
        pSearch->hClosedList.Destroy();
        pSearch->hExploreMap.Destroy();
    }
    pSearch->arrPath.Destroy();

    _SearchInstance_Free( idSearch );
}
Bool WorldPathFinder::_SimpleSearch_Update( SearchInstanceID idSearch, UInt iIterationCount )
{
    Assert( _SearchInstance_IsAllocated(idSearch) );

    _SearchInstance * pSearch = ( m_arrSearchInstances + idSearch );
    if ( pSearch->bFinished )
        return false;

    for( UInt iIteration = 0; iIteration < iIterationCount; ++iIteration ) {
        ++(pSearch->iIterationCount);

        // No path found case
        if ( pSearch->hOpenList.IsEmpty() ) {
            pSearch->bFinished = true;
            pSearch->bPathFound = false;

            pSearch->arrPath.Clear();

            // Destroy search data
            pSearch->hOpenList.Destroy();
            pSearch->hClosedList.Destroy();
            pSearch->hExploreMap.Destroy();

            // Done
            return false;
        }

        // Extract lowest cost waypoint
        WaypointState * pCurrentWaypointState = pSearch->hOpenList.Extract( true );
        pCurrentWaypointState->bInOpenList = false;
        WorldPathWaypoint * pCurrentWaypoint = pCurrentWaypointState->pWaypoint;

        // Add to closed list
        pSearch->hClosedList.Merge( pCurrentWaypointState );
        pCurrentWaypointState->bInClosedList = true;

        // End of search case
        if ( pCurrentWaypoint == pSearch->pTarget ) {
            pSearch->bFinished = true;
            pSearch->bPathFound = true;

            _SimpleSearch_BuildPath( idSearch );

            // Destroy search data
            pSearch->hOpenList.Destroy();
            pSearch->hClosedList.Destroy();
            pSearch->hExploreMap.Destroy();

            // Done
            return false;
        }

        // Explore adjacent waypoints
        UInt iAdjacencyCount = pCurrentWaypoint->GetAdjacencyCount();
        for( UInt i = 0; i < iAdjacencyCount; ++i ) {
            WorldPathWaypoint * pAdjacentWaypoint = pCurrentWaypoint->GetAdjacentWaypoint( i );

            // Blocked waypoints
            if ( pAdjacentWaypoint->GetCost() == WORLD_PATHFINDER_COST_INFINITE )
                continue;

            // Check explored waypoints
            _WaypointMap::Iterator itAdjacentWaypoint = pSearch->hExploreMap.Get( pAdjacentWaypoint );
            if ( itAdjacentWaypoint.IsNull() ) {
                // New waypoint ecountered
                WaypointState hWaypointState;
                hWaypointState.pWaypoint = pAdjacentWaypoint;

                hWaypointState.iHeapNode = INVALID_OFFSET;
                hWaypointState.bInOpenList = false;
                hWaypointState.bInClosedList = false;

                hWaypointState.pPathParent = pCurrentWaypointState;

                hWaypointState.fCurrentCost = ( pCurrentWaypointState->fCurrentCost + pAdjacentWaypoint->GetCost() );
                hWaypointState.fEstimatedCost = m_pfHeuristicCost( pAdjacentWaypoint, pSearch->pTarget );
                hWaypointState.fPathCost = ( hWaypointState.fCurrentCost + hWaypointState.fEstimatedCost );

                // Add to explore map
                Bool bInserted = pSearch->hExploreMap.Insert( pAdjacentWaypoint, hWaypointState, &itAdjacentWaypoint );
                Assert( bInserted && !(itAdjacentWaypoint.IsNull()) );

                WaypointState * pAdjacentWaypointState = itAdjacentWaypoint.GetItemPtr();

                // Add to open list
                pSearch->hOpenList.Merge( pAdjacentWaypointState );
                pAdjacentWaypointState->bInOpenList = true;
            } else {
                WaypointState * pAdjacentWaypointState = itAdjacentWaypoint.GetItemPtr();

                // Already explored waypoint
                if ( pAdjacentWaypointState->bInClosedList )
                    continue;

                // Must be in open list then
                Assert( pAdjacentWaypointState->bInOpenList );

                // Check for a better path
                Scalar fNewCost = ( pCurrentWaypointState->fCurrentCost + pAdjacentWaypoint->GetCost() );
                if ( fNewCost < pAdjacentWaypointState->fCurrentCost ) {
                    // Update path & cost
                    pAdjacentWaypointState->pPathParent = pCurrentWaypointState;

                    pAdjacentWaypointState->fCurrentCost = fNewCost;
                    pAdjacentWaypointState->fPathCost = ( pAdjacentWaypointState->fCurrentCost + pAdjacentWaypointState->fEstimatedCost );

                    // Update position in open list (cost has decreased)
                    pSearch->hOpenList.UpdateUp( pAdjacentWaypointState );
                }
            }
        }
    }

    return true;
}
Void WorldPathFinder::_SimpleSearch_BuildPath( SearchInstanceID idSearch )
{
    Assert( _SearchInstance_IsAllocated(idSearch) );

    _SearchInstance * pSearch = ( m_arrSearchInstances + idSearch );

    pSearch->arrPath.Clear();

    WaypointState * pPathNode = NULL;
    if ( pSearch->bFinished )
        pPathNode = pSearch->hExploreMap.Get(pSearch->pTarget).GetItemPtr();
    else
        pPathNode = pSearch->hOpenList.Extract( false );

    while( pPathNode != NULL ) {
        pSearch->arrPath.Unshift( pPathNode->pWaypoint );
        pPathNode = pPathNode->pPathParent;
    }
    Assert( pSearch->arrPath[0] == pSearch->pStart );
}

SearchInstanceID WorldPathFinder::_Hierarchic2Search_Start( WorldPathWaypoint * pStart, WorldPathWaypoint * pTarget )
{
    Assert( (pStart->IsMicroNode() && pTarget->IsMicroNode()) ||
            (pStart->IsNanoNode() && pTarget->IsNanoNode()) );

    // Create 2-hierarchic search
    SearchInstanceID idHierarchicSearch = _HierarchicSearchInstance_Alloc();
    _HierarchicSearchInstance * pHierarchicSearch = ( m_arrHierarchicSearchInstances + idHierarchicSearch );
    Assert( pHierarchicSearch->idSearch == idHierarchicSearch );

    pHierarchicSearch->iType = PATHFIND_TYPE_HIERARCHIC_2;

    pHierarchicSearch->iIterationCount = 0;
    pHierarchicSearch->bFinished = false;
    pHierarchicSearch->bPathFound = false;
    pHierarchicSearch->bBiasFirst = false;
    pHierarchicSearch->bBiasLast = false;

    pHierarchicSearch->pStart = pStart;
    pHierarchicSearch->pTarget = pTarget;

    pHierarchicSearch->idParentSearch = INVALID_OFFSET;
    pHierarchicSearch->arrSubSearches.UseMemoryContext( WorldFn->GetMemoryContext(), TEXT("PathData") );
    pHierarchicSearch->arrSubSearches.Create();

    pHierarchicSearch->arrPath.UseMemoryContext( WorldFn->GetMemoryContext(), TEXT("PathData") );
    pHierarchicSearch->arrPath.Create();

    // Create parent search
    pHierarchicSearch->idParentSearch = _SimpleSearch_Start( pStart->GetGridParent(), pTarget->GetGridParent() );
    _SearchInstance * pParentSearch = ( m_arrSearchInstances + pHierarchicSearch->idParentSearch );

    if ( pParentSearch->bFinished ) {
        if ( !(pParentSearch->bPathFound) ) {
            pHierarchicSearch->bFinished = true;
            pHierarchicSearch->bPathFound = false;

            pHierarchicSearch->arrPath.Clear();

            // Destroy search data
            _SimpleSearch_End( pHierarchicSearch->idParentSearch );
            pHierarchicSearch->idParentSearch = INVALID_OFFSET;

            pHierarchicSearch->arrSubSearches.Destroy();

            // Done
            return idHierarchicSearch;
        }
    }

    // Extract parent path (may be intermediate)
    if ( !(pParentSearch->bFinished) )
        _SimpleSearch_BuildPath( pHierarchicSearch->idParentSearch );
    const Array<WorldPathWaypoint*> & arrParentPath = pParentSearch->arrPath;
    Assert( arrParentPath.Count() > 0 );

    // Select realistic starting node
    WorldPathWaypoint * pFirstNode = NULL;
    if ( arrParentPath.Count() > 1 ) {
        pFirstNode = arrParentPath[1]->GetGridRoot();
        pHierarchicSearch->bBiasFirst = true;
    } else
        pFirstNode = arrParentPath[0]->GetGridRoot();

    // Create ascent search
    SearchInstanceID idAscentSearch = _SimpleSearch_Start( pStart, pFirstNode );
    pHierarchicSearch->arrSubSearches.Push( idAscentSearch );

    // Warm-Up
    _Hierarchic2Search_Update( idHierarchicSearch, WORLD_PATHFINDER_SEARCH_WARMUP );

    // Done
    return idHierarchicSearch;
}
Void WorldPathFinder::_Hierarchic2Search_End( SearchInstanceID idSearch )
{
    Assert( _HierarchicSearchInstance_IsAllocated(idSearch) );

    _HierarchicSearchInstance * pHierarchicSearch = ( m_arrHierarchicSearchInstances + idSearch );
    Assert( pHierarchicSearch->iType == PATHFIND_TYPE_HIERARCHIC_2 );

    if ( !(pHierarchicSearch->bFinished) ) {
        _SimpleSearch_End( pHierarchicSearch->idParentSearch );

        for( UInt i = 0; i < pHierarchicSearch->arrSubSearches.Count(); ++i )
            _SimpleSearch_End( pHierarchicSearch->arrSubSearches[i] );
        pHierarchicSearch->arrSubSearches.Destroy();
    }
    pHierarchicSearch->arrPath.Destroy();

    _HierarchicSearchInstance_Free( idSearch );
}
Bool WorldPathFinder::_Hierarchic2Search_Update( SearchInstanceID idSearch, UInt iIterationCount )
{
    Assert( _HierarchicSearchInstance_IsAllocated(idSearch) );

    _HierarchicSearchInstance * pHierarchicSearch = ( m_arrHierarchicSearchInstances + idSearch );
    Assert( pHierarchicSearch->iType == PATHFIND_TYPE_HIERARCHIC_2 );
    if ( pHierarchicSearch->bFinished )
        return false;

    // Update parent path
    _SearchInstance * pParentSearch = ( m_arrSearchInstances + pHierarchicSearch->idParentSearch );
    Bool bLaunchSubSearches = false;

    if ( !(pParentSearch->bFinished) ) {
        Bool bContinue = _SimpleSearch_Update( pHierarchicSearch->idParentSearch, iIterationCount );
        pHierarchicSearch->iIterationCount += iIterationCount;
        if ( !bContinue ) {
            if ( !(pParentSearch->bPathFound) ) {
                pHierarchicSearch->bFinished = true;
                pHierarchicSearch->bPathFound = false;

                pHierarchicSearch->arrPath.Clear();

                // Destroy search data
                _SimpleSearch_End( pHierarchicSearch->idParentSearch );
                pHierarchicSearch->idParentSearch = INVALID_OFFSET;

                for( UInt i = 0; i < pHierarchicSearch->arrSubSearches.Count(); ++i )
                    _SimpleSearch_End( pHierarchicSearch->arrSubSearches[i] );
                pHierarchicSearch->arrSubSearches.Destroy();

                // Done
                return false;
            }
            bLaunchSubSearches = true;
        } else
            bLaunchSubSearches = ( pParentSearch->iIterationCount > WORLD_PATHFINDER_SUBSEARCH_DELAY );
    } else {
        Assert( pParentSearch->bPathFound );
        bLaunchSubSearches = true;
    }

    // Launch sub searches
    if ( bLaunchSubSearches ) {
        // Extract parent path (may be intermediate)
        if ( !(pParentSearch->bFinished) )
            _SimpleSearch_BuildPath( pHierarchicSearch->idParentSearch );
        const Array<WorldPathWaypoint*> & arrParentPath = pParentSearch->arrPath;
        UInt iParentPathSize = arrParentPath.Count();
        Assert( iParentPathSize > 0 );

        if ( pParentSearch->bFinished ) {
            // Select realistic ending node
            WorldPathWaypoint * pLastNode = NULL;
            if ( iParentPathSize > 1 ) {
                pLastNode = arrParentPath[iParentPathSize - 2]->GetGridRoot();
                pHierarchicSearch->bBiasLast = true;
            } else
                pLastNode = arrParentPath[0]->GetGridRoot();

            // Create all remaining sub searches
            UInt iPathIndex = ( pHierarchicSearch->arrSubSearches.Count() - 1 );
            if ( pHierarchicSearch->bBiasFirst )
                ++iPathIndex;
            UInt iPathEnd = ( iParentPathSize - 1 );
            if ( pHierarchicSearch->bBiasLast )
                --iPathEnd;

            if ( iParentPathSize > 1 ) {
                while( iPathIndex < iPathEnd ) {
                    SearchInstanceID idSubSearch = _SimpleSearch_Start( arrParentPath[iPathIndex]->GetGridRoot(),
                                                                        arrParentPath[iPathIndex+1]->GetGridRoot() );
                    pHierarchicSearch->arrSubSearches.Push( idSubSearch );
                    ++iPathIndex;
                }
            }

            // Create descent search
            SearchInstanceID idDescentSearch = _SimpleSearch_Start( pLastNode, pHierarchicSearch->pTarget );
            pHierarchicSearch->arrSubSearches.Push( idDescentSearch );
        } else {
            // Create remaining sub searches, keep some margin
            UInt iPathIndex = ( pHierarchicSearch->arrSubSearches.Count() - 1 );
            if ( pHierarchicSearch->bBiasFirst )
                ++iPathIndex;
            UInt iPathEnd = ( iParentPathSize - 2 - WORLD_PATHFINDER_SUBSEARCH_DELAY );

            if ( iParentPathSize > 1 ) {
                while( iPathIndex < iPathEnd ) {
                    SearchInstanceID idSubSearch = _SimpleSearch_Start( arrParentPath[iPathIndex]->GetGridRoot(),
                                                                        arrParentPath[iPathIndex+1]->GetGridRoot() );
                    pHierarchicSearch->arrSubSearches.Push( idSubSearch );
                    ++iPathIndex;
                }
            }
        }
    }

    // Finish only when parent search is done
    pHierarchicSearch->bFinished = pParentSearch->bFinished;

    // Update sub searches
    for( UInt i = 0; i < pHierarchicSearch->arrSubSearches.Count(); ++i ) {
        SearchInstanceID idSubSearch = pHierarchicSearch->arrSubSearches[i];
        _SearchInstance * pSubSearch = ( m_arrSearchInstances + idSubSearch );

        if ( !(pSubSearch->bFinished) ) {
            Bool bContinue = _SimpleSearch_Update( idSubSearch, iIterationCount );
            pHierarchicSearch->iIterationCount += iIterationCount;
            if ( !bContinue ) {
                if ( !(pSubSearch->bPathFound) ) {
                    // No sub path ! This should never happen, since it would mean 2 parent waypoints
                    // have an adjacency while there is no possible path in their child grid ...
                    // This is a construction error in the waypoint graph !
                    Assert( false );
                }
            } else {
                // At least one subsearch is not done => not finished
                pHierarchicSearch->bFinished = false;
            }
        }
    }

    // Check if we're done with all searches
    if ( pHierarchicSearch->bFinished ) {
        pHierarchicSearch->bPathFound = true;

        _Hierarchic2Search_BuildPath( idSearch );

        // Destroy search data
        _SimpleSearch_End( pHierarchicSearch->idParentSearch );
        pHierarchicSearch->idParentSearch = INVALID_OFFSET;

        for( UInt i = 0; i < pHierarchicSearch->arrSubSearches.Count(); ++i )
            _SimpleSearch_End( pHierarchicSearch->arrSubSearches[i] );
        pHierarchicSearch->arrSubSearches.Destroy();

        // Done
        return false;
    }

    return true;
}
Void WorldPathFinder::_Hierarchic2Search_BuildPath( SearchInstanceID idSearch )
{
    Assert( _HierarchicSearchInstance_IsAllocated(idSearch) );

    _HierarchicSearchInstance * pHierarchicSearch = ( m_arrHierarchicSearchInstances + idSearch );
    Assert( pHierarchicSearch->iType == PATHFIND_TYPE_HIERARCHIC_2 );

    pHierarchicSearch->arrPath.Clear();

    UInt iSubSearchCount = pHierarchicSearch->arrSubSearches.Count();
    for( UInt i = 0; i < iSubSearchCount; ++i ) {
        SearchInstanceID idSubSearch = pHierarchicSearch->arrSubSearches[i];
        _SearchInstance * pSubSearch = ( m_arrSearchInstances + idSubSearch );

        if ( !(pHierarchicSearch->bFinished) ) {
            if ( !(pSubSearch->bFinished ) ) {
                _SimpleSearch_BuildPath( idSubSearch );
                pHierarchicSearch->arrPath.Push( pSubSearch->arrPath );
                break;
            }
        }
        pHierarchicSearch->arrPath.Push( pSubSearch->arrPath );
    }
}

SearchInstanceID WorldPathFinder::_Hierarchic3Search_Start( WorldPathWaypoint * /*pStart*/, WorldPathWaypoint * /*pTarget*/ )
{
    ////////////////////////////////////////////////
    Assert( false );
    return INVALID_OFFSET;
}
Void WorldPathFinder::_Hierarchic3Search_End( SearchInstanceID /*idSearch*/ )
{
    ////////////////////////////////////////////////
    Assert( false );
}
Bool WorldPathFinder::_Hierarchic3Search_Update( SearchInstanceID /*idSearch*/, UInt /*iIterationCount*/ )
{
    ////////////////////////////////////////////////
    Assert( false );
    return false;
}
Void WorldPathFinder::_Hierarchic3Search_BuildPath( SearchInstanceID /*idSearch*/ )
{
    ////////////////////////////////////////////////
    Assert( false );
}

Void WorldPathFinder::_SearchInstance_Init()
{
    m_iSearchInstanceCount = 0;
    m_iNextFreeSearchInstance = 0;

    _SearchInstance hSearchInstance;
    hSearchInstance.iIterationCount = 0;
    hSearchInstance.bFinished = false;
    hSearchInstance.bPathFound = false;

    hSearchInstance.pStart = NULL;
    hSearchInstance.pTarget = NULL;

    for( UInt i = 0; i < 256; ++i ) {
        hSearchInstance.idSearch = ( i + 1 );
        m_arrSearchInstances.Push( hSearchInstance );
    }
    m_arrSearchInstances[255].idSearch = INVALID_OFFSET;
}
SearchInstanceID WorldPathFinder::_SearchInstance_Alloc()
{
    if ( m_iNextFreeSearchInstance == INVALID_OFFSET ) {
        Assert( m_iSearchInstanceCount == m_arrSearchInstances.Count() );

        // Grow array
        m_iNextFreeSearchInstance = m_arrSearchInstances.Count();

        _SearchInstance hSearchInstance;
        hSearchInstance.iIterationCount = 0;
        hSearchInstance.bFinished = false;
        hSearchInstance.bPathFound = false;

        hSearchInstance.pStart = NULL;
        hSearchInstance.pTarget = NULL;

        UInt iCount = ( m_arrSearchInstances.Count() << 1 );
        for( UInt i = m_iNextFreeSearchInstance; i < iCount; ++i ) {
            hSearchInstance.idSearch = ( i + 1 );
            m_arrSearchInstances.Push( hSearchInstance );
        }
        m_arrSearchInstances[iCount - 1].idSearch = INVALID_OFFSET;
    }

    SearchInstanceID idSearch = m_iNextFreeSearchInstance;

    m_iNextFreeSearchInstance = m_arrSearchInstances[idSearch].idSearch;
    m_arrSearchInstances[idSearch].idSearch = idSearch;

    ++m_iSearchInstanceCount;

    return idSearch;
}
Void WorldPathFinder::_SearchInstance_Free( SearchInstanceID idSearch )
{
    m_arrSearchInstances[idSearch].idSearch = m_iNextFreeSearchInstance;
    m_arrSearchInstances[idSearch].iIterationCount = 0;
    m_arrSearchInstances[idSearch].bFinished = false;
    m_arrSearchInstances[idSearch].bPathFound = false;

    m_arrSearchInstances[idSearch].pStart = NULL;
    m_arrSearchInstances[idSearch].pTarget = NULL;

    m_iNextFreeSearchInstance = idSearch;

    --m_iSearchInstanceCount;
}

Void WorldPathFinder::_HierarchicSearchInstance_Init()
{
    m_iHierarchicSearchInstanceCount = 0;
    m_iNextFreeHierarchicSearchInstance = 0;

    _HierarchicSearchInstance hSearchInstance;
    hSearchInstance.iType = PATHFIND_TYPE_SIMPLE;

    hSearchInstance.bFinished = false;
    hSearchInstance.bPathFound = false;
    hSearchInstance.bBiasFirst = false;
    hSearchInstance.bBiasLast = false;

    hSearchInstance.pStart = NULL;
    hSearchInstance.pTarget = NULL;

    hSearchInstance.idParentSearch = INVALID_OFFSET;

    for( UInt i = 0; i < 256; ++i ) {
        hSearchInstance.idSearch = ( i + 1 );
        m_arrHierarchicSearchInstances.Push( hSearchInstance );
    }
    m_arrHierarchicSearchInstances[255].idSearch = INVALID_OFFSET;
}
SearchInstanceID WorldPathFinder::_HierarchicSearchInstance_Alloc()
{
    if ( m_iNextFreeHierarchicSearchInstance == INVALID_OFFSET ) {
        Assert( m_iHierarchicSearchInstanceCount == m_arrHierarchicSearchInstances.Count() );

        // Grow array
        m_iNextFreeHierarchicSearchInstance = m_arrHierarchicSearchInstances.Count();

        _HierarchicSearchInstance hSearchInstance;
        hSearchInstance.iType = PATHFIND_TYPE_SIMPLE;

        hSearchInstance.bFinished = false;
        hSearchInstance.bPathFound = false;
        hSearchInstance.bBiasFirst = false;
        hSearchInstance.bBiasLast = false;

        hSearchInstance.pStart = NULL;
        hSearchInstance.pTarget = NULL;

        hSearchInstance.idParentSearch = INVALID_OFFSET;

        UInt iCount = ( m_arrHierarchicSearchInstances.Count() << 1 );
        for( UInt i = m_iNextFreeHierarchicSearchInstance; i < iCount; ++i ) {
            hSearchInstance.idSearch = ( i + 1 );
            m_arrHierarchicSearchInstances.Push( hSearchInstance );
        }
        m_arrHierarchicSearchInstances[iCount - 1].idSearch = INVALID_OFFSET;
    }

    SearchInstanceID idSearch = m_iNextFreeHierarchicSearchInstance;

    m_iNextFreeHierarchicSearchInstance = m_arrHierarchicSearchInstances[idSearch].idSearch;
    m_arrHierarchicSearchInstances[idSearch].idSearch = idSearch;

    ++m_iHierarchicSearchInstanceCount;

    return idSearch;
}
Void WorldPathFinder::_HierarchicSearchInstance_Free( SearchInstanceID idSearch )
{
    m_arrHierarchicSearchInstances[idSearch].iType = PATHFIND_TYPE_SIMPLE;

    m_arrHierarchicSearchInstances[idSearch].idSearch = m_iNextFreeHierarchicSearchInstance;
    m_arrHierarchicSearchInstances[idSearch].bFinished = false;
    m_arrHierarchicSearchInstances[idSearch].bPathFound = false;
    m_arrHierarchicSearchInstances[idSearch].bBiasFirst = false;
    m_arrHierarchicSearchInstances[idSearch].bBiasLast = false;

    m_arrHierarchicSearchInstances[idSearch].pStart = NULL;
    m_arrHierarchicSearchInstances[idSearch].pTarget = NULL;

    m_arrHierarchicSearchInstances[idSearch].idParentSearch = INVALID_OFFSET;

    m_iNextFreeHierarchicSearchInstance = idSearch;

    --m_iHierarchicSearchInstanceCount;
}
