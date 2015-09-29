/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/IslandSolver/IslandEdge.cpp
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
// Includes
#include "IslandEdge.h"

/////////////////////////////////////////////////////////////////////////////////
// IslandEdgeID implementation
IslandEdgeID::IslandEdgeID()
{
    pNodeA = NULL;
    pNodeB = NULL;
}
IslandEdgeID::IslandEdgeID( const IslandNode * pA, const IslandNode * pB )
{
    // Ensure unicity
    if ( pA < pB ) {
        pNodeA = pA;
        pNodeB = pB;
    } else {
        pNodeA = pB;
        pNodeB = pA;
    }
}
IslandEdgeID::IslandEdgeID( const IslandEdgeID & rhs )
{
    pNodeA = rhs.pNodeA;
    pNodeB = rhs.pNodeB;
}
IslandEdgeID::~IslandEdgeID()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// IslandEdge implementation
IslandEdge::IslandEdge( IslandNode * pNodeA, IslandNode * pNodeB ):
    m_idEdge( pNodeA, pNodeB )
{
    // Color
    m_iColor = 0;

    // Island
    m_pIsland = NULL;
    m_iStackLayer = INVALID_OFFSET;

    // Nodes
    m_pNodeA = pNodeA;
    m_pNodeB = pNodeB;

    // Edges
    m_pNextInWorld = NULL;
    m_pPrevInWorld = NULL;

    m_pNextInIsland = NULL;
    m_pPrevInIsland = NULL;

    m_pNextInNodeA = NULL;
    m_pPrevInNodeA = NULL;
    m_pNextInNodeB = NULL;
    m_pPrevInNodeB = NULL;
}
IslandEdge::~IslandEdge()
{
    // nothing to do
}

