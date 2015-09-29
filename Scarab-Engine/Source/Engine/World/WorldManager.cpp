/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/WorldManager.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Core-Class for manipulating the WorldGraph system
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
#include "WorldManager.h"

/////////////////////////////////////////////////////////////////////////////////
// WorldManager implementation
WorldManager::WorldManager():
    Manager<WorldManager>()
{
    m_pWorldRoot = NULL;

    m_pWorldCamera = NULL;

    m_pWorldCuller = NULL;
    m_pWorldCaster = NULL;
    m_pWorldPathFinder = NULL;

    m_pGlobalEffect = NULL;

    m_bRunAnimation = true;
    m_bStepAnimation = false;

    m_bEnableDebugDraw = false;
    m_pWorldDebugDraw = NULL;
}
WorldManager::~WorldManager()
{
    // nothing to do
}

Void WorldManager::Initialize()
{
    // Create WorldRoot
    SelectMemory( TEXT("Nodes") );
    m_pWorldRoot = New WorldNode( TEXT("<Root>") );
    UnSelectMemory();

    // Create culler
    SelectMemory( TEXT("Scratch") );
    m_pWorldCuller = New WorldCuller();
    UnSelectMemory();

    // Create caster
    SelectMemory( TEXT("Scratch") );
    m_pWorldCaster = New WorldCaster();
    UnSelectMemory();

    // Create pathfinder
    SelectMemory( TEXT("Scratch") );
    m_pWorldPathFinder = New WorldPathFinder();
    UnSelectMemory();

    // Create debug-draw
    SelectMemory( TEXT("Scratch") );
    m_pWorldDebugDraw = New WorldDebugDraw();
    UnSelectMemory();
}
Void WorldManager::Cleanup()
{
    // Destroy debug-draw
    SelectMemory( TEXT("Scratch") );
    Delete( m_pWorldDebugDraw );
    m_pWorldDebugDraw = NULL;
    UnSelectMemory();

    // Destroy pathfinder
    SelectMemory( TEXT("Scratch") );
    Delete( m_pWorldPathFinder );
    m_pWorldPathFinder = NULL;
    UnSelectMemory();

    // Destroy caster
    SelectMemory( TEXT("Scratch") );
    Delete( m_pWorldCaster );
    m_pWorldCaster = NULL;
    UnSelectMemory();

    // Destroy culler
    SelectMemory( TEXT("Scratch") );
    Delete( m_pWorldCuller );
    m_pWorldCuller = NULL;
    UnSelectMemory();

    // Destroy WorldRoot
    SelectMemory( TEXT("Nodes") );
    _rec_DestroyNodes( m_pWorldRoot );
    m_pWorldRoot = NULL;
    UnSelectMemory();
}

WorldBaseNode * WorldManager::GetWorldNode( const GChar * strPath ) const
{
    // Root case
    if ( strPath == NULL )
        return m_pWorldRoot;

    // Skip leading slash if present
    if ( *strPath == TEXT('/') )
        ++strPath;

    // Root case
    if ( *strPath == NULLBYTE )
        return m_pWorldRoot;

    // Parse path
    UInt iBuf = 0;
    GChar strBuffer[WORLDGRAPH_NAME_LENGTH];
    WorldBaseNode * pNode = m_pWorldRoot;

    while( *strPath != NULLBYTE ) {
        if ( *strPath == TEXT('/') ) {
            if ( pNode->IsLeaf() )
                return NULL; // invalid path

            strBuffer[iBuf] = NULLBYTE;
            WorldBaseNode * pChild = ((WorldNode*)pNode)->GetChild( strBuffer );
            if ( pChild == NULL )
                return NULL; // invalid path

            pNode = pChild;
            iBuf = 0;
        } else
            strBuffer[iBuf++] = *strPath;
        ++strPath;
    }

    if ( iBuf > 0 ) {
        if ( pNode->IsLeaf() )
            return NULL; // invalid path

        strBuffer[iBuf] = NULLBYTE;
        WorldBaseNode * pChild = ((WorldNode*)pNode)->GetChild( strBuffer );
        if ( pChild == NULL )
            return NULL; // invalid path

        pNode = pChild;
    }

    return pNode;
}

Void WorldManager::DestroyWorld()
{
    SelectMemory( TEXT("Nodes") );

    // Destroy WorldRoot children
    m_pWorldRoot->EnumChildren();
    WorldBaseNode * pChild = m_pWorldRoot->EnumNextChild();
    while( pChild != NULL ) {
        _rec_DestroyNodes( pChild );
        pChild = m_pWorldRoot->EnumNextChild();
    }
    m_pWorldRoot->RemoveAllChildren();

    UnSelectMemory();
}

WorldNode * WorldManager::CreateNode( const GChar * strName )
{
    SelectMemory( TEXT("Nodes") );
    WorldNode * pNode = New WorldNode( strName );
    UnSelectMemory();

    return pNode;
}
WorldLeaf * WorldManager::CreateLeaf( const GChar * strName )
{
    SelectMemory( TEXT("Nodes") );
    WorldLeaf * pLeaf = New WorldLeaf( strName );
    UnSelectMemory();

    return pLeaf;
}

WorldBSPNode * WorldManager::CreateBSPNode( const GChar * strName )
{
    SelectMemory( TEXT("Nodes") );
    WorldBSPNode * pBSPNode = New WorldBSPNode( strName );
    UnSelectMemory();

    return pBSPNode;
}

Void WorldManager::DestroyNode( WorldBaseNode * pNode, Bool bRecursive )
{
    SelectMemory( TEXT("Nodes") );
    if ( bRecursive )
        _rec_DestroyNodes( pNode );
    else {
        if ( pNode->IsNode() )
            ((WorldNode*)pNode)->RemoveAllChildren();
        Delete( pNode );
    }
    UnSelectMemory();
}

Void WorldManager::UpdateWorld( Scalar fTimeStep )
{
    Assert( m_pWorldCamera != NULL );

    if ( m_bRunAnimation || m_bStepAnimation ) {
        // Update physical world
        PhysicsFn->SimulateWorld( fTimeStep );

        // Update logical world
        m_pWorldRoot->Update( fTimeStep );

        // Update camera
        m_pWorldCamera->Update( fTimeStep );
        m_pWorldCuller->UpdateCamera();

        m_bStepAnimation = false;
    }
}
Void WorldManager::RenderWorld() const
{
    Assert( m_pWorldCamera != NULL );

    UInt i;

    // Compute visible set
    m_pWorldCuller->ComputeVisibleGroup( m_pWorldRoot );
    const VisibleGroup & visibleLeaves = m_pWorldCuller->GetVisibleGroup();

    // Render visible set
    for( i = 0; i < visibleLeaves.GetCount(); ++i )
        visibleLeaves.GetLeaf(i)->Render();

    // Render global effect
    //if ( m_pGlobalEffect != NULL ) {
    //    ///////////////////////////////////
    //}

    // Debug drawing
    if ( m_bEnableDebugDraw ) {
        m_pWorldDebugDraw->RenderStart();

        //m_pWorldDebugDraw->RenderFrustrum( m_pWorldCuller );

        // Render world
        m_pWorldDebugDraw->RenderNode( m_pWorldRoot );

        for( i = 0; i < visibleLeaves.GetCount(); ++i ) {
            // Render visible leaves
            m_pWorldDebugDraw->RenderNode( visibleLeaves.GetLeaf(i) );
            
            // Render visible parents
            WorldNode * pParent = (WorldNode*)( visibleLeaves.GetLeaf(i)->GetParent() );
            switch( pParent->GetType() ) {
                case WORLDNODE_BSP:
                case WORLDNODE_INDOOR_CONVEXROOM:
                case WORLDNODE_OUTDOOR_AREA:
                    m_pWorldDebugDraw->RenderNode( pParent );
                    break;
                default: break;
            }
        }

        // Render BSP Tree, if any
        WorldBSPNode * pBSPTree = (WorldBSPNode*)( m_pWorldRoot->GetChild( TEXT("BSPTree_Root") ) );
        if ( pBSPTree != NULL )
            m_pWorldDebugDraw->RenderBSPTree( pBSPTree );

        m_pWorldDebugDraw->RenderEnd();
    }
}

/////////////////////////////////////////////////////////////////////////////////

Void WorldManager::_rec_DestroyNodes( WorldBaseNode * pNode )
{
    // Leaf case
    if ( pNode->IsLeaf() ) {
        Delete( pNode );
        return;
    }

    // Destroy children
    WorldNode * pCurNode = (WorldNode*)pNode;
    pCurNode->EnumChildren();
    WorldBaseNode * pChild = pCurNode->EnumNextChild();
    while( pChild != NULL ) {
        _rec_DestroyNodes( pChild );
        pChild = pCurNode->EnumNextChild();
    }

    // Destroy node
    Delete( pNode );
}

