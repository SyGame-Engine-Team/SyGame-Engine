/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Resources/Light.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Universal Light class.
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
#include "Light.h"

/////////////////////////////////////////////////////////////////////////////////
// Light implementation
Light::Light( LightType iType ):
    Ambient( Color4::Black ),
    Diffuse( Color4::Black ),
    Specular( Color4::Black ),
    Position( Vertex3::Null ),
    SideVect( Vector3::eI ),
    UpVect( Vector3::eJ ),
    Direction( Vector3::eK )
{
    m_iType = iType;

    Intensity = 1.0f;
    FadeConstant = 1.0f;
    FadeLinear = 0.0f;
    FadeQuadratic = 0.0f;
    SpotAngle = SCALAR_PI;
    SpotSin = 0;
    SpotCos = -1.0f;
    SpotExp = 1.0f;
}
Light::Light( const Light & rhs ):
    Ambient( rhs.Ambient ),
    Diffuse( rhs.Diffuse ),
    Specular( rhs.Specular ),
    Position( rhs.Position ),
    SideVect( rhs.SideVect ),
    UpVect( rhs.UpVect ),
    Direction( rhs.Direction )
{
    m_iType = rhs.m_iType;

    Intensity = rhs.Intensity;
    FadeConstant = rhs.FadeConstant;
    FadeLinear = rhs.FadeLinear;
    FadeQuadratic = rhs.FadeQuadratic;

    SpotAngle = rhs.SpotAngle;
    SpotSin = rhs.SpotSin;
    SpotCos = rhs.SpotCos;
    SpotExp = rhs.SpotExp;
}
Light::~Light()
{
    // nothing to do
}

Light & Light::operator=( const Light & rhs )
{
    m_iType = rhs.m_iType;

    Ambient = rhs.Ambient;
    Diffuse = rhs.Diffuse;
    Specular = rhs.Specular;

    Intensity = rhs.Intensity;
    FadeConstant = rhs.FadeConstant;
    FadeLinear = rhs.FadeLinear;
    FadeQuadratic = rhs.FadeQuadratic;

    Position = rhs.Position;
    SideVect = rhs.SideVect;
    UpVect = rhs.UpVect;
    Direction = rhs.Direction;

    SpotAngle = rhs.SpotAngle;
    SpotSin = rhs.SpotSin;
    SpotCos = rhs.SpotCos;
    SpotExp = rhs.SpotExp;

    return (*this);
}

