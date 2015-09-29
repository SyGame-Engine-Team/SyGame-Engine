/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/Container/WorldBSPNode.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Container : BSP nodes
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
#include "WorldBSPNode.h"

#include "../WorldCuller.h"

/////////////////////////////////////////////////////////////////////////////////
// WorldBSPNode implementation
WorldBSPNode::WorldBSPNode( const GChar * strName ):
    WorldNode(strName), WorldPlane()
{
    m_pPositive = NULL;
    m_pCoplanar = NULL;
    m_pNegative = NULL;
}
WorldBSPNode::~WorldBSPNode()
{
    // nothing to do
}

WorldBaseNode * WorldBSPNode::GetContainingNode( const Vertex3 & vPosition ) const
{
    Int iSide = WorldPlane.ClassifyPointNP( vPosition );

    if ( iSide > 0 ) {
        if ( m_pPositive == NULL )
            return NULL;
        if ( m_pPositive->GetType() != WORLDNODE_BSP )
            return m_pPositive;
        return ((WorldBSPNode*)m_pPositive)->GetContainingNode( vPosition );
    }

    if ( iSide < 0 ) {
        if ( m_pNegative == NULL )
            return NULL;
        if ( m_pNegative->GetType() != WORLDNODE_BSP )
            return m_pNegative;
        return ((WorldBSPNode*)m_pNegative)->GetContainingNode( vPosition );
    }

    return m_pCoplanar;
}

/////////////////////////////////////////////////////////////////////////////////

Void WorldBSPNode::_OnVisibleGroup( WorldCuller & wCuller, Bool bNoCull )
{
    // Back to front ordering
    const Camera * pCamera = wCuller.GetCamera();

    Int iEyeSide = WorldPlane.ClassifyPointNP( pCamera->GetViewEye() );
    Int iFrustrumSide = wCuller.WhichSide( WorldPlane );

    if ( iEyeSide > 0 ) {
        if ( iFrustrumSide <= 0 ) {
            if ( m_pNegative != NULL )
                m_pNegative->_QueryVisibleGroup( wCuller, bNoCull );
        }
        if ( iFrustrumSide == 0 ) {
            if ( m_pCoplanar != NULL )
                m_pCoplanar->_QueryVisibleGroup( wCuller, bNoCull );
        }
        if ( iFrustrumSide >= 0 ) {
            if ( m_pPositive != NULL )
                m_pPositive->_QueryVisibleGroup( wCuller, bNoCull );
        }
    } else if ( iEyeSide < 0 ) {
        if ( iFrustrumSide >= 0 ) {
            if ( m_pPositive != NULL )
                m_pPositive->_QueryVisibleGroup( wCuller, bNoCull );
        }
        if ( iFrustrumSide == 0 ) {
            if ( m_pCoplanar != NULL )
                m_pCoplanar->_QueryVisibleGroup( wCuller, bNoCull );
        }
        if ( iFrustrumSide <= 0 ) {
            if ( m_pNegative != NULL )
                m_pNegative->_QueryVisibleGroup( wCuller, bNoCull );
        }
    } else {
        if ( iFrustrumSide < 0 ) {
            if ( m_pNegative != NULL )
                m_pNegative->_QueryVisibleGroup( wCuller, bNoCull );
        } else if ( iFrustrumSide > 0 ) {
            if ( m_pPositive != NULL )
                m_pPositive->_QueryVisibleGroup( wCuller, bNoCull );
        } else {
            Scalar fDot = ( WorldPlane.Normal * pCamera->GetViewDir() );
            if ( fDot >= 0.0f ) {
                if ( m_pPositive != NULL )
                    m_pPositive->_QueryVisibleGroup( wCuller, bNoCull );
                if ( m_pCoplanar != NULL )
                    m_pCoplanar->_QueryVisibleGroup( wCuller, bNoCull );
                if ( m_pNegative != NULL )
                    m_pNegative->_QueryVisibleGroup( wCuller, bNoCull );
            } else {
                if ( m_pNegative != NULL )
                    m_pNegative->_QueryVisibleGroup( wCuller, bNoCull );
                if ( m_pCoplanar != NULL )
                    m_pCoplanar->_QueryVisibleGroup( wCuller, bNoCull );
                if ( m_pPositive != NULL )
                    m_pPositive->_QueryVisibleGroup( wCuller, bNoCull );
            }
        }
    }
}

