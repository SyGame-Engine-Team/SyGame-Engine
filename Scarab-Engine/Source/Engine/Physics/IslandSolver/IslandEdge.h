/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/IslandSolver/IslandEdge.h
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
// Header prelude
#ifndef SCARAB_ENGINE_PHYSICS_ISLANDSOLVER_ISLANDEDGE_H
#define SCARAB_ENGINE_PHYSICS_ISLANDSOLVER_ISLANDEDGE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Error/ErrorManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Edge types
enum IslandEdgeType
{
    ISLAND_EDGE_CONTACT = 0,
    ISLAND_EDGE_JOINT,

    ISLAND_EDGE_COUNT
};

// Prototypes
class Island;

class IslandNode;

/////////////////////////////////////////////////////////////////////////////////
// The IslandEdgeID class
class IslandEdgeID
{
public:
    IslandEdgeID();
    IslandEdgeID( const IslandNode * pA, const IslandNode * pB );
    IslandEdgeID( const IslandEdgeID & rhs );
    ~IslandEdgeID();

    inline IslandEdgeID & operator=( const IslandEdgeID & rhs );

    inline Bool operator==( const IslandEdgeID & rhs ) const;
    inline Bool operator!=( const IslandEdgeID & rhs ) const;

    inline static Int Compare( const IslandEdgeID & idLeft, const IslandEdgeID & idRight, Void * pUserData );

    const IslandNode * pNodeA;
    const IslandNode * pNodeB;
};

/////////////////////////////////////////////////////////////////////////////////
// The IslandEdge class
class IslandEdge
{
public:
    IslandEdge( IslandNode * pNodeA, IslandNode * pNodeB );
    virtual ~IslandEdge();

    // Type
    virtual IslandEdgeType GetType() const = 0;

    // ID & Color access
    inline const IslandEdgeID & GetID() const;

    inline Byte GetColor() const;
    inline Void SetColor( Byte iColor );

    // Island access
    inline Island * GetIsland() const;
    inline UInt GetStackLayer() const;

    // Nodes access
    inline IslandNode * GetNodeA() const;
    inline IslandNode * GetNodeB() const;

    // Edges access
    inline IslandEdge * GetNextInWorld() const;
    inline IslandEdge * GetPrevInWorld() const;

    inline IslandEdge * GetNextInIsland() const;
    inline IslandEdge * GetPrevInIsland() const;

    inline IslandEdge * GetNextInNodeA() const;
    inline IslandEdge * GetPrevInNodeA() const;
    inline IslandEdge * GetNextInNodeB() const;
    inline IslandEdge * GetPrevInNodeB() const;
    inline IslandEdge * GetNextInNode( const IslandNode * pNode ) const;
    inline IslandEdge * GetPrevInNode( const IslandNode * pNode ) const;

private:
    friend class IslandSolver;
    friend class Island;
    friend class IslandNode;

    // ID & Color
    IslandEdgeID m_idEdge;
    Byte m_iColor;

    // Island
    inline Void _SetIsland( Island * pIsland );
    inline Void _SetStackLayer( UInt iStackLayer );

    Island * m_pIsland;
    UInt m_iStackLayer;

    // Nodes
    IslandNode * m_pNodeA;
    IslandNode * m_pNodeB;

    // World's edge list
    inline Void _SetNextInWorld( IslandEdge * pEdge );
    inline Void _SetPrevInWorld( IslandEdge * pEdge );

    IslandEdge * m_pNextInWorld;
    IslandEdge * m_pPrevInWorld;

    // Island's edge list
    inline Void _SetNextInIsland( IslandEdge * pEdge );
    inline Void _SetPrevInIsland( IslandEdge * pEdge );

    IslandEdge * m_pNextInIsland;
    IslandEdge * m_pPrevInIsland;

    // Nodes's edge lists
    inline Void _SetNextInNodeA( IslandEdge * pEdge );
    inline Void _SetPrevInNodeA( IslandEdge * pEdge );
    inline Void _SetNextInNodeB( IslandEdge * pEdge );
    inline Void _SetPrevInNodeB( IslandEdge * pEdge );
    inline Void _SetNextInNode( IslandNode * pNode, IslandEdge * pEdge );
    inline Void _SetPrevInNode( IslandNode * pNode, IslandEdge * pEdge );

    IslandEdge * m_pNextInNodeA;
    IslandEdge * m_pPrevInNodeA;
    IslandEdge * m_pNextInNodeB;
    IslandEdge * m_pPrevInNodeB;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "IslandEdge.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_ISLANDSOLVER_ISLANDEDGE_H

