/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/WorldPathFinder.inl
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
// WaypointState implementation
inline WaypointState & WaypointState::operator=( const WaypointState & rhs ) {
    pWaypoint = rhs.pWaypoint;

    iHeapNode = rhs.iHeapNode;
    bInOpenList = rhs.bInOpenList;
    bInClosedList = rhs.bInClosedList;

    pPathParent = rhs.pPathParent;

    fCurrentCost = rhs.fCurrentCost;
    fEstimatedCost = rhs.fEstimatedCost;
    fPathCost = rhs.fPathCost;

    return (*this);
}

/////////////////////////////////////////////////////////////////////////////////
// WorldPathWaypoint implementation
inline const Vertex3 & WorldPathWaypoint::GetPosition() const {
    return m_vPosition;
}
inline Void WorldPathWaypoint::SetPosition( const Vertex3 & vPosition ) {
    m_vPosition = vPosition;
}

inline Scalar WorldPathWaypoint::GetCost() const {
    return m_fCost;
}
inline Void WorldPathWaypoint::SetCost( Scalar fCost ) {
    Assert( fCost >= 0.0f );
    m_fCost = fCost;
}

/////////////////////////////////////////////////////////////////////////////////
// WorldPathMacroNode implementation
inline Bool WorldPathMacroNode::IsMacroNode() const {
    return true;
}
inline Bool WorldPathMacroNode::IsMicroNode() const {
    return false;
}
inline Bool WorldPathMacroNode::IsNanoNode() const {
    return false;
}

inline UInt WorldPathMacroNode::GetAdjacencyCount() const {
    return m_iAdjacencyCount;
}
inline WorldPathWaypoint * WorldPathMacroNode::GetAdjacentWaypoint( UInt iIndex ) const {
    Assert( iIndex < m_iAdjacencyCount );
    return m_arrAdjacentWaypoints[iIndex];
}

inline Void WorldPathMacroNode::AddAdjacentWaypoint( WorldPathWaypoint * pAdjacentWaypoint ) {
    Assert( m_iAdjacencyCount < WORLD_PATHFINDER_MAX_ADJACENCY_MACRO );
    Assert( pAdjacentWaypoint->IsMacroNode() );
    m_arrAdjacentWaypoints[m_iAdjacencyCount] = (WorldPathMacroNode*)pAdjacentWaypoint;
    ++m_iAdjacencyCount;
}
inline Void WorldPathMacroNode::ClearAdjacency() {
    m_iAdjacencyCount = 0;
}

inline WorldPathWaypoint * WorldPathMacroNode::GetGridParent() const {
    return NULL;
}
inline WorldPathWaypoint * WorldPathMacroNode::GetGridRoot() const {
    return m_pGridRoot;
}

inline Void WorldPathMacroNode::SetGridRoot( WorldPathMicroNode * pGridRoot ) {
    m_pGridRoot = pGridRoot;
}

/////////////////////////////////////////////////////////////////////////////////
// WorldPathMicroNode implementation
inline Bool WorldPathMicroNode::IsMacroNode() const {
    return false;
}
inline Bool WorldPathMicroNode::IsMicroNode() const {
    return true;
}
inline Bool WorldPathMicroNode::IsNanoNode() const {
    return false;
}

inline UInt WorldPathMicroNode::GetAdjacencyCount() const {
    return m_iAdjacencyCount;
}
inline WorldPathWaypoint * WorldPathMicroNode::GetAdjacentWaypoint( UInt iIndex ) const {
    Assert( iIndex < m_iAdjacencyCount );
    return m_arrAdjacentWaypoints[iIndex];
}

inline Void WorldPathMicroNode::AddAdjacentWaypoint( WorldPathWaypoint * pAdjacentWaypoint ) {
    Assert( m_iAdjacencyCount < WORLD_PATHFINDER_MAX_ADJACENCY_MICRO );
    Assert( pAdjacentWaypoint->IsMicroNode() );
    m_arrAdjacentWaypoints[m_iAdjacencyCount] = (WorldPathMicroNode*)pAdjacentWaypoint;
    ++m_iAdjacencyCount;
}
inline Void WorldPathMicroNode::ClearAdjacency() {
    m_iAdjacencyCount = 0;
}

inline WorldPathWaypoint * WorldPathMicroNode::GetGridParent() const {
    return m_pGridParent;
}
inline WorldPathWaypoint * WorldPathMicroNode::GetGridRoot() const {
    return m_pGridRoot;
}

inline Void WorldPathMicroNode::SetGridParent( WorldPathMacroNode * pGridParent ) {
    m_pGridParent = pGridParent;
}
inline Void WorldPathMicroNode::SetGridRoot( WorldPathNanoNode * pGridRoot ) {
    m_pGridRoot = pGridRoot;
}

/////////////////////////////////////////////////////////////////////////////////
// WorldPathNanoNode implementation
inline Bool WorldPathNanoNode::IsMacroNode() const {
    return false;
}
inline Bool WorldPathNanoNode::IsMicroNode() const {
    return false;
}
inline Bool WorldPathNanoNode::IsNanoNode() const {
    return true;
}

inline UInt WorldPathNanoNode::GetAdjacencyCount() const {
    return m_iAdjacencyCount;
}
inline WorldPathWaypoint * WorldPathNanoNode::GetAdjacentWaypoint( UInt iIndex ) const {
    Assert( iIndex < m_iAdjacencyCount );
    return m_arrAdjacentWaypoints[iIndex];
}

inline Void WorldPathNanoNode::AddAdjacentWaypoint( WorldPathWaypoint * pAdjacentWaypoint ) {
    Assert( m_iAdjacencyCount < WORLD_PATHFINDER_MAX_ADJACENCY_NANO );
    Assert( pAdjacentWaypoint->IsNanoNode() );
    m_arrAdjacentWaypoints[m_iAdjacencyCount] = (WorldPathNanoNode*)pAdjacentWaypoint;
    ++m_iAdjacencyCount;
}
inline Void WorldPathNanoNode::ClearAdjacency() {
    m_iAdjacencyCount = 0;
}

inline WorldPathWaypoint * WorldPathNanoNode::GetGridParent() const {
    return m_pGridParent;
}
inline WorldPathWaypoint * WorldPathNanoNode::GetGridRoot() const {
    return NULL;
}

inline Void WorldPathNanoNode::SetGridParent( WorldPathMicroNode * pGridParent ) {
    m_pGridParent = pGridParent;
}

/////////////////////////////////////////////////////////////////////////////////
// WaypointHeap implementation
inline Bool WaypointHeap::IsEmpty() const {
    return ( m_arrNodes.Count() <= 1 );
}
inline UInt WaypointHeap::Count() const {
    return ( m_arrNodes.Count() - 1 );
}

/////////////////////////////////////////////////////////////////////////////////

inline Int WaypointHeap::_Compare( const WaypointState * pLeft, const WaypointState * pRight ) {
    if ( pLeft->fPathCost < pRight->fPathCost )
        return +1;
    if ( pLeft->fPathCost > pRight->fPathCost )
        return -1;
    if ( pLeft->pWaypoint < pRight->pWaypoint )
        return +1;
    if ( pLeft->pWaypoint > pRight->pWaypoint )
        return -1;
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////
// WorldPathFinder implementation
inline Void WorldPathFinder::SetHeuristicCostFunction( PathFindHeuristic idHeuristic ) {
    Assert( idHeuristic < PATHFIND_HEURISTIC_COUNT );
    m_pfHeuristicCost = m_arrHeuristics[idHeuristic];
}

/////////////////////////////////////////////////////////////////////////////////

inline Int WorldPathFinder::_Compare_Waypoints( WorldPathWaypoint * const & rLeft, WorldPathWaypoint * const & rRight, Void * /*pUserData*/ ) {
    if ( rLeft < rRight )
        return +1;
    if ( rLeft > rRight )
        return -1;
    return 0;
}

inline WorldPathFinder::_SearchInstance & WorldPathFinder::_SearchInstance::operator=( const _SearchInstance & rhs ) {
    idSearch = rhs.idSearch;
    iIterationCount = rhs.iIterationCount;
    bFinished = rhs.bFinished;
    bPathFound = rhs.bPathFound;

    pStart = rhs.pStart;
    pTarget = rhs.pTarget;

    MemCopy( &hExploreMap, &(rhs.hExploreMap), sizeof(_WaypointMap) );
    MemCopy( &hOpenList, &(rhs.hOpenList), sizeof(WaypointHeap) );
    MemCopy( &hClosedList, &(rhs.hClosedList), sizeof(WaypointHeap) );

    MemCopy( &arrPath, &(rhs.arrPath), sizeof(Array<WorldPathWaypoint*>) );

    return (*this);
}

inline WorldPathFinder::_HierarchicSearchInstance & WorldPathFinder::_HierarchicSearchInstance::operator=( const _HierarchicSearchInstance & rhs ) {
    iType = rhs.iType;

    idSearch = rhs.idSearch;
    iIterationCount = rhs.iIterationCount;
    bFinished = rhs.bFinished;
    bPathFound = rhs.bPathFound;
    bBiasFirst = rhs.bBiasFirst;
    bBiasLast = rhs.bBiasLast;

    pStart = rhs.pStart;
    pTarget = rhs.pTarget;

    idParentSearch = rhs.idParentSearch;
    MemCopy( &arrSubSearches, &(rhs.arrSubSearches), sizeof(Array<SearchInstanceID>) );

    MemCopy( &arrPath, &(rhs.arrPath), sizeof(Array<WorldPathWaypoint*>) );

    return (*this);
}

inline Bool WorldPathFinder::_SearchInstance_IsAllocated( SearchInstanceID idSearch ) const {
    Assert( idSearch < m_arrSearchInstances.Count() );
    return ( m_arrSearchInstances[idSearch].idSearch == idSearch );
}

inline Bool WorldPathFinder::_HierarchicSearchInstance_IsAllocated( SearchInstanceID idSearch ) const {
    Assert( idSearch < m_arrHierarchicSearchInstances.Count() );
    return ( m_arrHierarchicSearchInstances[idSearch].idSearch == idSearch );
}

inline Scalar WorldPathFinder::_Heuristic_Distance( WorldPathWaypoint * pCurrent, WorldPathWaypoint * pTarget ) {
    Vector3 vW = ( pTarget->GetPosition() - pCurrent->GetPosition() );
    return vW.Norm();
}
inline Scalar WorldPathFinder::_Heuristic_DistanceSquared( WorldPathWaypoint * pCurrent, WorldPathWaypoint * pTarget ) {
    Vector3 vW = ( pTarget->GetPosition() - pCurrent->GetPosition() );
    return vW.NormSqr();
}
inline Scalar WorldPathFinder::_Heuristic_DistanceManhattan( WorldPathWaypoint * pCurrent, WorldPathWaypoint * pTarget ) {
    Vector3 vW = ( pTarget->GetPosition() - pCurrent->GetPosition() );
    return ( MathFn->Abs(vW.X) + MathFn->Abs(vW.Y) + MathFn->Abs(vW.Z) );
}
