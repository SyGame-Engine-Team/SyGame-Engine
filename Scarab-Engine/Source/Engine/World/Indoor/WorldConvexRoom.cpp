/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/Indoor/WorldConvexRoom.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Graph : Interior, Convex Room
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : none
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "WorldConvexRoom.h"

#include "../WorldManager.h"

/////////////////////////////////////////////////////////////////////////////////
// WorldConvexRoom implementation
WorldConvexRoom::WorldConvexRoom( UInt iPortalCount, const GChar * strName ):
    WorldNode( strName )
{
    m_bVisited = false;

    WorldFn->SelectMemory( TEXT("Indoor") );
    m_iPortalCount = iPortalCount;
    m_arrPortals = New WorldPortal*[m_iPortalCount];
    WorldFn->UnSelectMemory();

    for( UInt i = 0; i < m_iPortalCount; ++i )
        m_arrPortals[i] = NULL;
}
WorldConvexRoom::~WorldConvexRoom()
{
    WorldFn->SelectMemory( TEXT("Indoor") );

    for( UInt i = 0; i < m_iPortalCount; ++i ) {
        if ( m_arrPortals[i] != NULL )
            Delete( m_arrPortals[i] );
    }
    DeleteA( m_arrPortals );

    WorldFn->UnSelectMemory();
}

WorldPortal * WorldConvexRoom::CreatePortal( UInt iPortal, const Vertex3 * arrPolygon, UInt iPolygonSize )
{
    Assert( iPortal < m_iPortalCount );
    Assert( m_arrPortals[iPortal] == NULL );

    WorldFn->SelectMemory( TEXT("Indoor") );
    m_arrPortals[iPortal] = New WorldPortal( arrPolygon, iPolygonSize );
    WorldFn->UnSelectMemory();

    return m_arrPortals[iPortal];
}
Void WorldConvexRoom::DestroyPortal( UInt iPortal )
{
    Assert( iPortal < m_iPortalCount );

    WorldFn->SelectMemory( TEXT("Indoor") );
    Delete( m_arrPortals[iPortal] );
    WorldFn->UnSelectMemory();

    m_arrPortals[iPortal] = NULL;
}

Bool WorldConvexRoom::Contains( const Vertex3 & vPoint ) const
{
    // Default behaviour : test bv only
    return WorldBV.Contains( vPoint );
}

/////////////////////////////////////////////////////////////////////////////////

Void WorldConvexRoom::_OnVisibleGroup( WorldCuller & wCuller, Bool bNoCull )
{
    // Already visited during this query
    if ( m_bVisited )
        return;

    // Mark visited for this query
    m_bVisited = true;

    // Recurse on portals
    for( UInt i = 0; i < m_iPortalCount; ++i ) {
        if ( m_arrPortals[i] != NULL )
            m_arrPortals[i]->_OnVisibleGroup( (WorldPortalCuller&)wCuller, bNoCull );
    }

    // Recurse on children
    WorldNode::_OnVisibleGroup( wCuller, bNoCull );

    // Mark unvisited for future queries
    m_bVisited = false;
}

