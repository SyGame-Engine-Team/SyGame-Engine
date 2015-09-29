/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/IslandSolver/Island.cpp
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
// Includes
#include "Island.h"

#include "../PhysicsManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Island implementation
Island::Island()
{
    // Color
    m_iColor = 0;

    // Stack layers
    m_iStackLayerCount = 0;

    // Nodes
    m_iBodyCount = 0;

    m_pListBodies = NULL;

    // Edges
    m_iContactCount = 0;
    m_iJointCount = 0;

    m_pListContacts = NULL;
    m_pListJoints = NULL;

    // Islands
    m_pNextInWorld = NULL;
    m_pPrevInWorld = NULL;
}
Island::~Island()
{
    // nothing to do
}

Void Island::AddContact( ContactEdge * pContactEdge )
{
    Assert( pContactEdge->GetIsland() == NULL );

    pContactEdge->_SetIsland( this );
    pContactEdge->_SetPrevInIsland( NULL );
    pContactEdge->_SetNextInIsland( m_pListContacts );

    if ( m_pListContacts != NULL )
        m_pListContacts->_SetPrevInIsland( pContactEdge );
    m_pListContacts = pContactEdge;

    ++m_iContactCount;

    BodyNode * pBodyA = pContactEdge->GetBodyA();
    BodyNode * pBodyB = pContactEdge->GetBodyB();
    if ( !(pBodyA->IsInsulator()) )
        _AddBody( pBodyA );
    if ( !(pBodyB->IsInsulator()) )
        _AddBody( pBodyB );
}
Void Island::AddJoint( JointEdge * pJointEdge )
{
    Assert( pJointEdge->GetIsland() == NULL );

    pJointEdge->_SetIsland( this );
    pJointEdge->_SetPrevInIsland( NULL );
    pJointEdge->_SetNextInIsland( m_pListJoints );

    if ( m_pListJoints != NULL )
        m_pListJoints->_SetPrevInIsland( pJointEdge );
    m_pListJoints = pJointEdge;

    ++m_iJointCount;

    BodyNode * pBodyA = pJointEdge->GetBodyA();
    BodyNode * pBodyB = pJointEdge->GetBodyB();
    if ( !(pBodyA->IsInsulator()) )
        _AddBody( pBodyA );
    if ( !(pBodyB->IsInsulator()) )
        _AddBody( pBodyB );
}

Void Island::RemoveContact( ContactEdge * pContactEdge )
{
    Assert( pContactEdge->GetIsland() == this );

    BodyNode * pBodyA = pContactEdge->GetBodyA();
    BodyNode * pBodyB = pContactEdge->GetBodyB();
    if ( !(pBodyA->IsInsulator()) )
        _RemoveBody( pBodyA );
    if ( !(pBodyB->IsInsulator()) )
        _RemoveBody( pBodyB );

    IslandEdge * pPrevEdge = pContactEdge->GetPrevInIsland();
    IslandEdge * pNextEdge = pContactEdge->GetNextInIsland();

    if ( pPrevEdge != NULL )
        pPrevEdge->_SetNextInIsland( pNextEdge );
    else {
        Assert( m_pListContacts == pContactEdge );
        m_pListContacts = (ContactEdge*)pNextEdge;
    }
    if ( pNextEdge != NULL )
        pNextEdge->_SetPrevInIsland( pPrevEdge );

    pContactEdge->_SetIsland( NULL );
    pContactEdge->_SetPrevInIsland( NULL );
    pContactEdge->_SetNextInIsland( NULL );

    --m_iContactCount;
}
Void Island::RemoveJoint( JointEdge * pJointEdge )
{
    Assert( pJointEdge->GetIsland() == this );

    BodyNode * pBodyA = pJointEdge->GetBodyA();
    BodyNode * pBodyB = pJointEdge->GetBodyB();
    if ( !(pBodyA->IsInsulator()) )
        _RemoveBody( pBodyA );
    if ( !(pBodyB->IsInsulator()) )
        _RemoveBody( pBodyB );

    IslandEdge * pPrevEdge = pJointEdge->GetPrevInIsland();
    IslandEdge * pNextEdge = pJointEdge->GetNextInIsland();

    if ( pPrevEdge != NULL )
        pPrevEdge->_SetNextInIsland( pNextEdge );
    else {
        Assert( m_pListJoints == pJointEdge );
        m_pListJoints = (JointEdge*)pNextEdge;
    }
    if ( pNextEdge != NULL )
        pNextEdge->_SetPrevInIsland( pPrevEdge );

    pJointEdge->_SetIsland( NULL );
    pJointEdge->_SetPrevInIsland( NULL );
    pJointEdge->_SetNextInIsland( NULL );

    --m_iJointCount;
}

/////////////////////////////////////////////////////////////////////////////////

Void Island::_AddBody( BodyNode * pBody )
{
    if ( pBody->GetIsland() != NULL ) {
        if ( pBody->GetIsland() == this )
            pBody->_AddIslandRef();
        return;
    }
    pBody->_AddIslandRef();

    pBody->_SetIsland( this );
    pBody->_SetPrevInIsland( NULL );
    pBody->_SetNextInIsland( m_pListBodies );

    if ( m_pListBodies != NULL )
        m_pListBodies->_SetPrevInIsland( pBody );
    m_pListBodies = pBody;

    ++m_iBodyCount;
}
Void Island::_RemoveBody( BodyNode * pBody )
{
    if ( pBody->GetIsland() != this )
        return;
    if ( !(pBody->_RemoveIslandRef()) )
        return;

    IslandNode * pPrevNode = pBody->GetPrevInIsland();
    IslandNode * pNextNode = pBody->GetNextInIsland();

    if ( pPrevNode != NULL )
        pPrevNode->_SetNextInIsland( pNextNode );
    else {
        Assert( m_pListBodies == pBody );
        m_pListBodies = (BodyNode*)pNextNode;
    }
    if ( pNextNode != NULL )
        pNextNode->_SetPrevInIsland( pPrevNode );

    pBody->_SetIsland( NULL );
    pBody->_SetPrevInIsland( NULL );
    pBody->_SetNextInIsland( NULL );

    --m_iBodyCount;
}

