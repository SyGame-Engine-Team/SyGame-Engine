/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/IslandSolver/Edges/JointEdge.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Island edges : Joint
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
#ifndef SCARAB_ENGINE_PHYSICS_ISLANDSOLVER_EDGES_JOINTEDGE_H
#define SCARAB_ENGINE_PHYSICS_ISLANDSOLVER_EDGES_JOINTEDGE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Nodes/BodyNode.h"

#include "../../Joints/JointConstraint.h"

#include "../IslandEdge.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The JointEdge class
class JointEdge : public IslandEdge
{
public:
    JointEdge( BodyNode * pNodeA, BodyNode * pNodeB, JointConstraint * pJoint );
    virtual ~JointEdge();

    // Type
    inline virtual IslandEdgeType GetType() const;

    // Bodies access
    inline BodyNode * GetBodyA() const;
    inline BodyNode * GetBodyB() const;

    // Constraint access
    inline JointConstraint * GetJoint() const;

private:
    JointConstraint * m_pJointConstraint;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "JointEdge.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_ISLANDSOLVER_EDGES_JOINTEDGE_H
