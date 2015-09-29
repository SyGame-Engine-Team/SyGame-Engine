/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/IslandSolver/IslandEdge.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Abstraction for island graphs edges
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// IslandEdgeID implementation
inline IslandEdgeID & IslandEdgeID::operator=( const IslandEdgeID & rhs ) {
    pNodeA = rhs.pNodeA;
    pNodeB = rhs.pNodeB;
    return (*this);
}

inline Bool IslandEdgeID::operator==( const IslandEdgeID & rhs ) const {
    return ( pNodeA == rhs.pNodeA && pNodeB == rhs.pNodeB );
}
inline Bool IslandEdgeID::operator!=( const IslandEdgeID & rhs ) const {
    return ( pNodeA != rhs.pNodeA || pNodeB != rhs.pNodeB );
}

inline Int IslandEdgeID::Compare( const IslandEdgeID & idLeft, const IslandEdgeID & idRight, Void * /*pUserData*/ ) {
    if ( idLeft.pNodeA < idRight.pNodeA )
        return +1;
    if ( idRight.pNodeA < idLeft.pNodeA )
        return -1;
    if ( idLeft.pNodeB < idRight.pNodeB )
        return +1;
    if ( idRight.pNodeB < idLeft.pNodeB )
        return -1;
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////
// IslandEdge implementation
inline const IslandEdgeID & IslandEdge::GetID() const {
    return m_idEdge;
}

inline Byte IslandEdge::GetColor() const {
    return m_iColor;
}
inline Void IslandEdge::SetColor( Byte iColor ) {
    m_iColor = iColor;
}

inline Island * IslandEdge::GetIsland() const {
    return m_pIsland;
}
inline UInt IslandEdge::GetStackLayer() const {
    return m_iStackLayer;
}

inline IslandNode * IslandEdge::GetNodeA() const {
    return m_pNodeA;
}
inline IslandNode * IslandEdge::GetNodeB() const {
    return m_pNodeB;
}

inline IslandEdge * IslandEdge::GetNextInWorld() const {
    return m_pNextInWorld;
}
inline IslandEdge * IslandEdge::GetPrevInWorld() const {
    return m_pPrevInWorld;
}

inline IslandEdge * IslandEdge::GetNextInIsland() const {
    return m_pNextInIsland;
}
inline IslandEdge * IslandEdge::GetPrevInIsland() const {
    return m_pPrevInIsland;
}

inline IslandEdge * IslandEdge::GetNextInNodeA() const {
    return m_pNextInNodeA;
}
inline IslandEdge * IslandEdge::GetPrevInNodeA() const {
    return m_pPrevInNodeA;
}
inline IslandEdge * IslandEdge::GetNextInNodeB() const {
    return m_pNextInNodeB;
}
inline IslandEdge * IslandEdge::GetPrevInNodeB() const {
    return m_pPrevInNodeB;
}
inline IslandEdge * IslandEdge::GetNextInNode( const IslandNode * pNode ) const {
    if ( pNode == m_pNodeA )
        return m_pNextInNodeA;
    Assert( pNode == m_pNodeB );
    return m_pNextInNodeB;
}
inline IslandEdge * IslandEdge::GetPrevInNode( const IslandNode * pNode ) const {
    if ( pNode == m_pNodeA )
        return m_pPrevInNodeA;
    Assert( pNode == m_pNodeB );
    return m_pPrevInNodeB;
}

/////////////////////////////////////////////////////////////////////////////////

inline Void IslandEdge::_SetIsland( Island * pIsland ) {
    m_pIsland = pIsland;
}
inline Void IslandEdge::_SetStackLayer( UInt iStackLayer ) {
    m_iStackLayer = iStackLayer;
}

inline Void IslandEdge::_SetNextInWorld( IslandEdge * pEdge ) {
    m_pNextInWorld = pEdge;
}
inline Void IslandEdge::_SetPrevInWorld( IslandEdge * pEdge ) {
    m_pPrevInWorld = pEdge;
}

inline Void IslandEdge::_SetNextInIsland( IslandEdge * pEdge ) {
    m_pNextInIsland = pEdge;
}
inline Void IslandEdge::_SetPrevInIsland( IslandEdge * pEdge ) {
    m_pPrevInIsland = pEdge;
}

inline Void IslandEdge::_SetNextInNodeA( IslandEdge * pEdge ) {
    m_pNextInNodeA = pEdge;
}
inline Void IslandEdge::_SetPrevInNodeA( IslandEdge * pEdge ) {
    m_pPrevInNodeA = pEdge;
}
inline Void IslandEdge::_SetNextInNodeB( IslandEdge * pEdge ) {
    m_pNextInNodeB = pEdge;
}
inline Void IslandEdge::_SetPrevInNodeB( IslandEdge * pEdge ) {
    m_pPrevInNodeB = pEdge;
}
inline Void IslandEdge::_SetNextInNode( IslandNode * pNode, IslandEdge * pEdge ) {
    if ( pNode == m_pNodeA )
        m_pNextInNodeA = pEdge;
    else {
        Assert( pNode == m_pNodeB );
        m_pNextInNodeB = pEdge;
    }
}
inline Void IslandEdge::_SetPrevInNode( IslandNode * pNode, IslandEdge * pEdge ) {
    if ( pNode == m_pNodeA )
        m_pPrevInNodeA = pEdge;
    else {
        Assert( pNode == m_pNodeB );
        m_pPrevInNodeB = pEdge;
    }
}
