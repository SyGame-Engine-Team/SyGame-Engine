/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/IslandSolver/Edges/ContactEdge.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Island edges : Contact
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
#ifndef SCARAB_ENGINE_PHYSICS_ISLANDSOLVER_EDGES_CONTACTEDGE_H
#define SCARAB_ENGINE_PHYSICS_ISLANDSOLVER_EDGES_CONTACTEDGE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Nodes/BodyNode.h"

#include "../../CollisionDetection/NarrowPhase/NarrowPhase.h"

#include "../IslandEdge.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
//enum ContactEdgeType
//{
//    CONTACT_EDGE_RIGID_RIGID = 0,
//    CONTACT_EDGE_RIGID_SOFT,
//    CONTACT_EDGE_SOFT_SOFT,
//
//    CONTACT_EDGE_COUNT
//};
//
//    // Prototypes
//class RigidBody;
//class SoftBody;

/////////////////////////////////////////////////////////////////////////////////
// The ContactEdge class
class ContactEdge : public IslandEdge
{
public:
    ContactEdge( BodyNode * pNodeA, BodyNode * pNodeB );
    virtual ~ContactEdge();

    // Type
    inline virtual IslandEdgeType GetType() const;
    //virtual ContactEdgeType GetContactType() const = 0;

    // Bodies access
    inline BodyNode * GetBodyA() const;
    inline BodyNode * GetBodyB() const;

    // Narrow phase access
    inline NarrowPhase * GetNarrowPhase() const;

    // Contact manifold access
    inline const ContactManifold * GetManifold() const;
    inline ContactManifold * GetManifold();

protected:
    // Narrow phase data
    static NarrowPhase * _SelectNarrowAlgorithm( TransformedShape * pShapeA, TransformedShape * pShapeB );

    NarrowPhase * m_pNarrowPhase;
    ContactManifold * m_pContactManifold;
};

///////////////////////////////////////////////////////////////////////////////////
//// The ContactRigidRigidEdge class
//class ContactRigidRigidEdge : public ContactEdge
//{
//public:
//    ContactRigidRigidEdge( RigidBody * pRigidBodyA, RigidBody * pRigidBodyB );
//    virtual ~ContactRigidRigidEdge();
//
//    // Type
//    inline virtual ContactEdgeType GetContactType() const;
//
//private:
//    // Rigid - Rigid contacts
//};
//
///////////////////////////////////////////////////////////////////////////////////
//// The ContactRigidSoftEdge class
//class ContactRigidSoftEdge : public ContactEdge
//{
//public:
//    ContactRigidSoftEdge( RigidBody * pRigidBodyA, SoftBody * pSoftBodyB );
//    ContactRigidSoftEdge( SoftBody * pSoftBodyA, RigidBody * pRigidBodyB );
//    virtual ~ContactRigidSoftEdge();
//
//    // Type
//    inline virtual ContactEdgeType GetContactType() const;
//
//private:
//    // Rigid - SoftBody::Cluster contacts
//    // Rigid - SoftBody::Tetra contacts
//    // Rigid - SoftBody::Facet contacts
//    // Rigid - SoftBody::Node contacts
//};
//
///////////////////////////////////////////////////////////////////////////////////
//// The ContactSoftSoftEdge class
//class ContactSoftSoftEdge : public ContactEdge
//{
//public:
//    ContactSoftSoftEdge( SoftBody * pSoftBodyA, SoftBody * pSoftBodyB );
//    virtual ~ContactSoftSoftEdge();
//
//    // Type
//    inline virtual ContactEdgeType GetContactType() const;
//
//private:
//    // SoftBody::Cluster - SoftBody::Cluster contacts
//    // SoftBody::Cluster - SoftBody::Tetra contacts
//    // SoftBody::Cluster - SoftBody::Facet contacts
//    // SoftBody::Cluster - SoftBody::Node contacts
//    // SoftBody::Tetra - SoftBody::Tetra contacts
//    // SoftBody::Tetra - SoftBody::Facet contacts
//    // SoftBody::Tetra - SoftBody::Node contacts
//    // SoftBody::Facet - SoftBody::Facet contacts
//    // SoftBody::Facet - SoftBody::Node contacts
//};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "ContactEdge.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_ISLANDSOLVER_EDGES_CONTACTEDGE_H

