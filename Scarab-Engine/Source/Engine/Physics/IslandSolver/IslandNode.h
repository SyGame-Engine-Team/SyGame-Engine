/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/IslandSolver/IslandNode.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Abstraction for island graphs nodes
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
#ifndef SCARAB_ENGINE_PHYSICS_ISLANDSOLVER_ISLANDNODE_H
#define SCARAB_ENGINE_PHYSICS_ISLANDSOLVER_ISLANDNODE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "IslandEdge.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
enum IslandNodeType
{
    ISLAND_NODE_BODY = 0,

    ISLAND_NODE_COUNT
};

// Prototypes
class ContactEdge;
class JointEdge;

/////////////////////////////////////////////////////////////////////////////////
// The IslandNode class
class IslandNode
{
public:
    IslandNode();
    virtual ~IslandNode();

    // Type
    virtual IslandNodeType GetType() const = 0;

    // Behaviour
    virtual Bool IsInsulator() const = 0;

    // Island access
    inline Island * GetIsland() const;

    inline UInt GetStackLayer() const;

    inline Bool IsFixed() const;
    inline Void SetFixed( Bool bFixed );

    // Edges access
    inline UInt GetContactCount() const;
    inline UInt GetJointCount() const;
    inline UInt GetTotalEdgeCount() const;

    inline ContactEdge * GetContactList() const;
    inline JointEdge * GetJointList() const;

    Void AddContact( ContactEdge * pContactEdge );
    Void AddJoint( JointEdge * pJointEdge );

    Void RemoveContact( ContactEdge * pContactEdge );
    Void RemoveJoint( JointEdge * pJointEdge );

    Void RemoveAllContacts();
    Void RemoveAllJoints();

    // Nodes access
    inline IslandNode * GetNextInWorld() const;
    inline IslandNode * GetPrevInWorld() const;

    inline IslandNode * GetNextInIsland() const;
    inline IslandNode * GetPrevInIsland() const;

    // User data access
    inline Void * GetUserData() const;
    inline Void SetUserData( Void * pUserData );

// Debug
    inline UInt _Debug_GetIslandRefCount() const {
        return m_iIslandRefCount;
    }
// Debug

private:
    friend class IslandSolver;
    friend class Island;

    // Island
    inline Void _SetIsland( Island * pIsland );
    inline Void _AddIslandRef();
    inline Bool _RemoveIslandRef();

    inline Void _SetStackLayer( UInt iStackLayer );

    Island * m_pIsland;
    UInt m_iIslandRefCount;

    UInt m_iStackLayer;

    Bool m_bFixed;

    // Edges
    UInt m_iContactCount;
    UInt m_iJointCount;

    ContactEdge * m_pListContacts;
    JointEdge * m_pListJoints;

    // World's node list
    inline Void _SetNextInWorld( IslandNode * pNode );
    inline Void _SetPrevInWorld( IslandNode * pNode );

    IslandNode * m_pNextInWorld;
    IslandNode * m_pPrevInWorld;

    // Island's node list
    inline Void _SetNextInIsland( IslandNode * pNode );
    inline Void _SetPrevInIsland( IslandNode * pNode );

    IslandNode * m_pNextInIsland;
    IslandNode * m_pPrevInIsland;

    // User data
    Void * m_pUserData;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "IslandNode.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_ISLANDSOLVER_ISLANDNODE_H


