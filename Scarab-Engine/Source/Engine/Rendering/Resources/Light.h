/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Resources/Light.h
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
// Header prelude
#ifndef SCARAB_ENGINE_RENDERING_RESOURCES_LIGHT_H
#define SCARAB_ENGINE_RENDERING_RESOURCES_LIGHT_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Error/ErrorManager.h"

#include "../../../Lib/Math/Types/Vertex/Vertex3.h"
#include "../../../Lib/Math/Types/Vector/Vector3.h"
#include "../../../Lib/Math/Types/Color/Color4.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
enum LightType
{
	LIGHT_OMNI = 0,
    LIGHT_DIRECTIONAL,
	LIGHT_POINT,
	LIGHT_SPOT,

	LIGHT_COUNT
};

/////////////////////////////////////////////////////////////////////////////////
// The Light class
class Light
{
public:
    Light( LightType iType = LIGHT_OMNI );
    Light( const Light & rhs );
	~Light();

    Light & operator=( const Light & rhs );

    // Type
	inline LightType GetType() const;
    inline Void SetType( LightType iType );

    // Colors
    Color4 Ambient;
    Color4 Diffuse;
    Color4 Specular;

    // Attenuation ( Att = I / (C + L*d + Q*d*d) )
    Scalar Intensity;
    Scalar FadeConstant;
    Scalar FadeLinear;
    Scalar FadeQuadratic;

    inline Scalar GetAttenuation( Scalar fDistance ) const;

    // Frame
    Vertex3 Position;
	Vector3 SideVect;
	Vector3 UpVect;
	Vector3 Direction;

    inline Void SetDirection( const Vector3 & vDir );

    // Spot parameters
    Scalar SpotAngle;
    Scalar SpotCos;
    Scalar SpotSin;
    Scalar SpotExp;

    inline Void SetSpotAngle( Scalar fAngle );

protected:
	LightType m_iType;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Light.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RENDERING_RESOURCES_LIGHT_H
