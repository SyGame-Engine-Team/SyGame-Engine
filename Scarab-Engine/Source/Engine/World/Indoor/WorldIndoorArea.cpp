/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/Indoor/WorldIndoorArea.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Graph : Indoor Area.
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
#include "WorldIndoorArea.h"

/////////////////////////////////////////////////////////////////////////////////
// WorldIndoorArea implementation
WorldIndoorArea::WorldIndoorArea( const GChar * strName ):
    WorldBSPNode(strName), m_PortalCuller()
{
    // nothing to do
}
WorldIndoorArea::~WorldIndoorArea()
{
    // nothing to do
}

WorldConvexRoom * WorldIndoorArea::GetContainingRoom( const Vertex3 & vPoint ) const
{
    WorldConvexRoom * pConvexRoom = (WorldConvexRoom*)( GetContainingNode(vPoint) );
    if ( pConvexRoom == NULL )
        return NULL;

    if ( pConvexRoom->Contains(vPoint) )
        return pConvexRoom;

    return NULL;
}

/////////////////////////////////////////////////////////////////////////////////

Void WorldIndoorArea::_OnVisibleGroup( WorldCuller & wCuller, Bool bNoCull )
{
    // Locate the room containing camera's eye
    WorldConvexRoom * pRoom = GetContainingRoom( wCuller.GetCamera()->GetViewEye() );

    // Eye is outside
    if ( pRoom == NULL ) {
        WorldBaseNode * pOutside = GetOutsideNode();
        if ( pOutside != NULL )
            pOutside->_OnVisibleGroup( wCuller, bNoCull );
        return;
    }

    // Compute VisibleGroup using portals
    Scalar arrFrustrum[FRUSTRUM_COUNT];
    wCuller.GetFrustrumState( arrFrustrum );

    m_PortalCuller.SetCamera( wCuller.GetCamera(), false );    
    m_PortalCuller.SetFrustrumState( arrFrustrum );
    m_PortalCuller.Clear();
    m_PortalCuller.ComputeVisibleGroup( pRoom );

    // Give results to parent culler
    m_PortalCuller.EnumVisibles();
    WorldLeaf * pVisibleLeaf = m_PortalCuller.EnumNextVisible();
    while( pVisibleLeaf != NULL ) {
        wCuller.OnVisibleNode( pVisibleLeaf );
        pVisibleLeaf = m_PortalCuller.EnumNextVisible();
    }
}

