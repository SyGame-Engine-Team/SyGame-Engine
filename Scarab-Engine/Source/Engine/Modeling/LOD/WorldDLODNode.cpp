/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Modeling/LOD/WorldDLODNode.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Discrete Level Of Detail by model switching
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
#include "WorldDLODNode.h"

#include "../ModelingManager.h"

/////////////////////////////////////////////////////////////////////////////////
// WorldDLODNode implementation
WorldDLODNode::WorldDLODNode( const GChar * strName, UInt iLODCount ):
    WorldSwitchNode( strName ), m_vModelCenter(), m_vWorldCenter()
{
    m_iLODCount = iLODCount;

    ModelingFn->SelectMemory( TEXT("Geometry") );

    m_arrModelMinDist = New Scalar[m_iLODCount];
    m_arrModelMaxDist = New Scalar[m_iLODCount];
    m_arrWorldMinDist = New Scalar[m_iLODCount];
    m_arrWorldMaxDist = New Scalar[m_iLODCount];

    ModelingFn->UnSelectMemory();
}
WorldDLODNode::~WorldDLODNode()
{
    ModelingFn->SelectMemory( TEXT("Geometry") );

    DeleteA( m_arrModelMinDist );
    DeleteA( m_arrModelMaxDist );
    DeleteA( m_arrWorldMinDist );
    DeleteA( m_arrWorldMaxDist );

    ModelingFn->UnSelectMemory();
}

/////////////////////////////////////////////////////////////////////////////////

Void WorldDLODNode::_SelectLevelOfDetail( const Camera * pCamera )
{
    Assert( m_iLODCount == GetChildCount() );

    UInt i;

    // Compute world center
    m_vWorldCenter = ( WorldTransform * m_vModelCenter );

    // Compute world intervals
    for( i = 0; i < m_iLODCount; ++i ) {
        m_arrWorldMinDist[i] = WorldTransform.GetUniformScale() * m_arrModelMinDist[i];
        m_arrWorldMaxDist[i] = WorldTransform.GetUniformScale() * m_arrModelMaxDist[i];
    }

    // Select the LOD model
    Vector3 vToCamera = ( m_vWorldCenter - pCamera->GetViewEye() );
    Scalar fDistSqr = vToCamera.NormSqr();

    EnumChildren();
    const WorldBaseNode * pChild = EnumNextChild();
    for( i = 0; i < m_iLODCount; ++i ) {
        if ( fDistSqr >= m_arrWorldMinDist[i] && fDistSqr < m_arrWorldMaxDist[i] ) {
            SetActiveChild( pChild->GetName() );
            break;
        }
        pChild = EnumNextChild();
    }
}

Void WorldDLODNode::_OnVisibleGroup( WorldCuller & wCuller, Bool bNoCull )
{
    _SelectLevelOfDetail( wCuller.GetCamera() );
    WorldSwitchNode::_OnVisibleGroup( wCuller, bNoCull );
}

