/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/Nodes/WorldLeaf.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Node : Leaves
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
#include "WorldLeaf.h"

#include "../WorldManager.h"

/////////////////////////////////////////////////////////////////////////////////
// WorldLeaf implementation
WorldLeaf::WorldLeaf( const GChar * strName ):
    WorldBaseNode( strName ), ModelBV()
{
    CastShadows = false;

    m_pMesh = NULL;
    m_pEffectInstance = NULL;

    m_pBody = NULL;
}
WorldLeaf::~WorldLeaf()
{
    // nothing to do
}

Void WorldLeaf::Render() const
{
    if ( m_pMesh != NULL && m_pEffectInstance != NULL )
        RenderingFn->Draw( m_pMesh, &WorldTransform, m_pEffectInstance );
}

/////////////////////////////////////////////////////////////////////////////////

Void WorldLeaf::_UpdateWorldTransform( Scalar /*fTimeStep*/ )
{
    // Update ModelBV
    if ( m_pBody != NULL ) {
        // Update from body's proxy
        if ( m_pBody->GetLocalShape()->IsDeformable() || ModelBV.IsEmpty() )
            ModelBV = m_pBody->GetLocalShape()->GetAAB();
    } else if ( m_pMesh != NULL ) {
        // Update from mesh
        if ( ModelBV.IsEmpty() ) {
            UInt iOffset, iSize;

            m_pMesh->GetIL()->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_POSITION, 0 );
            const Byte * arrPositions = m_pMesh->GetVB()->GetData() + iOffset;

            ModelBV.Compute( arrPositions, m_pMesh->GetVertexCount(), m_pMesh->GetVertexSize() );
        }
    } else {
        // Logical leaf, let the user manage ModelBV himself
        // nothing to do
    }

    // Update WorldTransform
    if ( m_pBody != NULL ) {
        // Update from physical/controlled animation
        WorldTransform = m_pBody->GetTransform();
    } else {
        // Using custom animation, let the user manage WorldTransform himself
        // nothing to do
    }
}
Void WorldLeaf::_UpdateWorldBV()
{
    // Update world BV
    if ( m_pBody != NULL ) {
        // Update from body's proxy
        WorldBV = m_pBody->GetWorldShape()->GetAAB();
    } else {
        // Transform from ModelBV
        ModelBV.Transform( WorldBV, WorldTransform );
    }
}

Void WorldLeaf::_OnVisibleGroup( WorldCuller & wCuller, Bool /*bNoCull*/ )
{
    wCuller.OnVisibleNode( this );
}



