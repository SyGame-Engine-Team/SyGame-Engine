/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/Container/WorldLight.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Container : Lights
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
#include "WorldLight.h"

/////////////////////////////////////////////////////////////////////////////////
// WorldLight implementation
WorldLight::WorldLight( Light * pLight, const GChar * strName ):
    WorldLeaf( strName )
{
    m_pLight = pLight;
}
WorldLight::~WorldLight()
{
    // nothing to do
}

Void WorldLight::SetLight( Light * pLight )
{
    m_pLight = pLight;

    if ( m_pLight != NULL ) {
        m_pLight->Position = ( Vertex3::Null + WorldTransform.GetTranslate() );
        WorldTransform.GetRotate().GetColumn( m_pLight->SideVect, 0 );
        WorldTransform.GetRotate().GetColumn( m_pLight->UpVect, 1 );
        WorldTransform.GetRotate().GetColumn( m_pLight->Direction, 2 );
    }
}

/////////////////////////////////////////////////////////////////////////////////

Void WorldLight::_UpdateWorldTransform( Scalar fTimeStep )
{
    WorldLeaf::_UpdateWorldTransform( fTimeStep );

    if ( m_pLight != NULL ) {
        m_pLight->Position = ( Vertex3::Null + WorldTransform.GetTranslate() );
        WorldTransform.GetRotate().GetColumn( m_pLight->SideVect, 0 );
        WorldTransform.GetRotate().GetColumn( m_pLight->UpVect, 1 );
        WorldTransform.GetRotate().GetColumn( m_pLight->Direction, 2 );
    }
}

