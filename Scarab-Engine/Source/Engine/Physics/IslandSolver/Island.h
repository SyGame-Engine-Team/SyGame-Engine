/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/IslandSolver/Island.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : An Island represents a set of bodies in contact or close
//               proximity that must be integrated together.
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
#ifndef SCARAB_ENGINE_PHYSICS_ISLANDSOLVER_ISLAND_H
#define SCARAB_ENGINE_PHYSICS_ISLANDSOLVER_ISLAND_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "IslandEdge.h"
#include "IslandNode.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Prototypes
class BodyNode;

/////////////////////////////////////////////////////////////////////////////////
// The Island class
class Island
{
public:
    Island();
    ~Island();

    // Color access
    inline Byte GetColor() const;
    inline Void SetColor( Byte iColor );

    // Stack layers
    inline UInt GetStackLayerCount() const;

    // Nodes access
    inline UInt GetBodyCount() const;
    inline UInt GetTotalNodeCount() const;

    inline BodyNode * GetBodyList() const;

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

    // Islands access
    inline Island * GetNextInWorld() const;
    inline Island * GetPrevInWorld() const;

private:
    friend class IslandSolver;

    // Color
    Byte m_iColor;

    // Stack layers
    inline Void _SetStackLayerCount( UInt iStackLayerCount );

    UInt m_iStackLayerCount;

    // Nodes
    Void _AddBody( BodyNode * pBody );
    Void _RemoveBody( BodyNode * pBody );

    UInt m_iBodyCount;

    BodyNode * m_pListBodies;

    // Edges
    UInt m_iContactCount;
    UInt m_iJointCount;

    ContactEdge * m_pListContacts;
    JointEdge * m_pListJoints;

    // World's island list
    inline Void _SetNextInWorld( Island * pIsland );
    inline Void _SetPrevInWorld( Island * pIsland );

    Island * m_pNextInWorld;
    Island * m_pPrevInWorld;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Island.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_ISLANDSOLVER_ISLAND_H

