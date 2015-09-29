/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/Nodes/WorldNode.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Node : Internal nodes
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "WorldNode.h"

#include "../WorldManager.h"

/////////////////////////////////////////////////////////////////////////////////
// WorldNode implementation
WorldNode::WorldNode( const GChar * strName ):
    WorldBaseNode(strName), m_mapChildren(), m_itEnumerate()
{
    m_mapChildren.UseMemoryContext( WorldFn->GetMemoryContext(), TEXT("Nodes") );
    m_mapChildren.SetComparator( _Name_Comparator, NULL );
    m_mapChildren.Create();
}
WorldNode::~WorldNode()
{
    // Children become orphans
    ChildMap::Iterator it;
    for( it = m_mapChildren.Begin(); !(it.IsNull()); ++it )
        it.GetItem()->m_pParent = NULL;
    m_mapChildren.Destroy();
}

Bool WorldNode::AddChild( WorldBaseNode * pNode )
{
    const GChar * strName = pNode->GetName();
    if ( m_mapChildren.Contains(strName) )
        return false;

    Bool bInserted = m_mapChildren.Insert( strName, pNode );
    Assert( bInserted );

    Assert( pNode->m_pParent == NULL );
    pNode->m_pParent = this;

    return true;
}
WorldBaseNode * WorldNode::RemoveChild( const GChar * strName )
{
    WorldBaseNode * pNode = NULL;
    Bool bRemoved = m_mapChildren.Remove( strName, &pNode );
    if ( !bRemoved || (pNode == NULL) )
        return NULL;

    Assert( pNode->m_pParent == this );
    pNode->m_pParent = NULL;

    return pNode;    
}
Void WorldNode::RemoveAllChildren()
{
    ChildMap::Iterator it;
    for( it = m_mapChildren.Begin(); !(it.IsNull()); ++it ) {
        Assert( it.GetItem()->m_pParent == this );
        it.GetItem()->m_pParent = NULL;
    }

    m_mapChildren.Clear();
}

/////////////////////////////////////////////////////////////////////////////////

Void WorldNode::_UpdateWorldTransform( Scalar fTimeStep )
{
    // Recurse on children
    ChildMap::Iterator it;
    for( it = m_mapChildren.Begin(); !(it.IsNull()); ++it )
        it.GetItem()->Update( fTimeStep );
}
Void WorldNode::_UpdateWorldBV()
{
    ChildMap::Iterator it;

    // Update from children BVs
    Bool bInitialized = false;
    for( it = m_mapChildren.Begin(); !(it.IsNull()); ++it ) {
        if ( !bInitialized ) {
            WorldBV = it.GetItem()->WorldBV;
            bInitialized = true;
        } else
            WorldBV.Grow( it.GetItem()->WorldBV );
    }
}

Void WorldNode::_OnVisibleGroup( WorldCuller & wCuller, Bool bNoCull )
{
    // Recurse on children
    ChildMap::Iterator it;
    for( it = m_mapChildren.Begin(); !(it.IsNull()); ++it )
        it.GetItem()->_QueryVisibleGroup( wCuller, bNoCull );
}

