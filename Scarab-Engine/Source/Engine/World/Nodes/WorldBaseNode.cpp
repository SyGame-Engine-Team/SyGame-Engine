/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/Nodes/WorldBaseNode.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Base Node class for the WorldGraph manager
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
#include "WorldBaseNode.h"

#include "../WorldManager.h"

/////////////////////////////////////////////////////////////////////////////////
// WorldBaseNode implementation
UInt WorldBaseNode::sm_iNoNameCount = 0;

WorldBaseNode::WorldBaseNode( const GChar * strName ):
    WorldTransform(), WorldBV()
{
    UpdateWorldTransform = true;
    UpdateWorldBV = true;
    CullMode = WORLD_CULL_DYNAMIC;

    m_pParent = NULL;

    if ( strName == NULL ) {
        StringFn->Format( m_strName, TEXT("_NoName_%ud"), sm_iNoNameCount );
        ++sm_iNoNameCount;
    } else
        StringFn->NCopy( m_strName, strName, WORLDGRAPH_NAME_LENGTH - 1 );

    m_dwDebugFlags = 0;
}
WorldBaseNode::~WorldBaseNode()
{
    // nothing to do
}

Void WorldBaseNode::Update( Scalar fTimeStep )
{
    // Update WorldTransform
    if ( UpdateWorldTransform )
        _UpdateWorldTransform( fTimeStep );

    // Update WorldBV
    if ( UpdateWorldBV )
        _UpdateWorldBV();
}

/////////////////////////////////////////////////////////////////////////////////

Void WorldBaseNode::_QueryVisibleGroup( WorldCuller & wCuller, Bool bNoCull )
{
    if ( CullMode == WORLD_CULL_ALLWAYS )
        return;
    if ( CullMode == WORLD_CULL_NEVER )
        bNoCull = true;

    UInt iSavedPlaneState = wCuller.GetPlaneState();
    if ( bNoCull || wCuller.IsVisible( WorldBV ) )
        _OnVisibleGroup( wCuller, bNoCull );
    wCuller.SetPlaneState( iSavedPlaneState );
}

