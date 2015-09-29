/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/IslandSolver/IslandNode.cpp
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
// Includes
#include "IslandNode.h"

#include "../PhysicsManager.h"

/////////////////////////////////////////////////////////////////////////////////
// IslandNode implementation
IslandNode::IslandNode()
{
    // Island
    m_pIsland = NULL;
    m_iIslandRefCount = 0;

    m_iStackLayer = INVALID_OFFSET;

    m_bFixed = false;

    // Edges
    m_iContactCount = 0;
    m_iJointCount = 0;

    m_pListContacts = NULL;
    m_pListJoints = NULL;

    // List linkage
    m_pNextInIsland = NULL;
    m_pPrevInIsland = NULL;

    m_pNextInWorld = NULL;
    m_pPrevInWorld = NULL;

    // User data
    m_pUserData = NULL;
}
IslandNode::~IslandNode()
{
    // nothing to do
}

Void IslandNode::AddContact( ContactEdge * pContactEdge )
{
    pContactEdge->_SetPrevInNode( this, NULL );
    pContactEdge->_SetNextInNode( this, m_pListContacts );

    if ( m_pListContacts != NULL )
        m_pListContacts->_SetPrevInNode( this, pContactEdge );
    m_pListContacts = pContactEdge;
    
    ++m_iContactCount;
}
Void IslandNode::AddJoint( JointEdge * pJointEdge )
{
    pJointEdge->_SetPrevInNode( this, NULL );
    pJointEdge->_SetNextInNode( this, m_pListJoints );

    if ( m_pListJoints != NULL )
        m_pListJoints->_SetPrevInNode( this, pJointEdge );
    m_pListJoints = pJointEdge;

    ++m_iJointCount;
}

Void IslandNode::RemoveContact( ContactEdge * pContactEdge )
{
    IslandEdge * pPrevEdge = pContactEdge->GetPrevInNode( this );
    IslandEdge * pNextEdge = pContactEdge->GetNextInNode( this );

    if ( pPrevEdge != NULL )
        pPrevEdge->_SetNextInNode( this, pNextEdge );
    else {
        Assert( m_pListContacts == pContactEdge );
        m_pListContacts = (ContactEdge*)pNextEdge;
    }
    if ( pNextEdge != NULL )
        pNextEdge->_SetPrevInNode( this, pPrevEdge );

    pContactEdge->_SetPrevInNode( this, NULL );
    pContactEdge->_SetNextInNode( this, NULL );

    --m_iContactCount;
}
Void IslandNode::RemoveJoint( JointEdge * pJointEdge )
{
    IslandEdge * pPrevEdge = pJointEdge->GetPrevInNode( this );
    IslandEdge * pNextEdge = pJointEdge->GetNextInNode( this );

    if ( pPrevEdge != NULL )
        pPrevEdge->_SetNextInNode( this, pNextEdge );
    else {
        Assert( m_pListJoints == pJointEdge );
        m_pListJoints = (JointEdge*)pNextEdge;
    }
    if ( pNextEdge != NULL )
        pNextEdge->_SetPrevInNode( this, pPrevEdge );

    pJointEdge->_SetPrevInNode( this, NULL );
    pJointEdge->_SetNextInNode( this, NULL );

    --m_iJointCount;
}

Void IslandNode::RemoveAllContacts()
{
    IslandEdge * pNextEdge;

    ContactEdge * pContactEdge = m_pListContacts;    
    while( pContactEdge != NULL ) {
        pNextEdge = pContactEdge->GetNextInNode( this );

        pContactEdge->_SetPrevInNode( this, NULL );
        pContactEdge->_SetNextInNode( this, NULL );

        pContactEdge = (ContactEdge*)pNextEdge;
    }

    m_iContactCount = 0;
    m_pListContacts = NULL;
}
Void IslandNode::RemoveAllJoints()
{
    IslandEdge * pNextEdge;

    JointEdge * pJointEdge = m_pListJoints;    
    while( pJointEdge != NULL ) {
        pNextEdge = pJointEdge->GetNextInNode( this );

        pJointEdge->_SetPrevInNode( this, NULL );
        pJointEdge->_SetNextInNode( this, NULL );

        pJointEdge = (JointEdge*)pNextEdge;
    }

    m_iJointCount = 0;
    m_pListJoints = NULL;
}



