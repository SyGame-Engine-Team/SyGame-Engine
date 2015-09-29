/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Resources/Light.inl
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
// Light implementation
inline LightType Light::GetType() const {
    return m_iType;
}
inline Void Light::SetType( LightType iType ) {
    m_iType = iType;
}

inline Scalar Light::GetAttenuation( Scalar fDistance ) const {
    return Intensity / ( FadeConstant + fDistance * ( FadeLinear + (fDistance * FadeQuadratic) ) );
}

inline Void Light::SetDirection( const Vector3 & vDir ) {
    Direction = vDir;
    Direction.Normalize();
    Vector3::MakeComplementBasis( SideVect, UpVect, Direction );
}

inline Void Light::SetSpotAngle( Scalar fAngle ) {
    Assert( (fAngle > 0.0f) && (fAngle <= SCALAR_PI) );
    SpotAngle = fAngle;
    MathFn->SinCos( &SpotSin, &SpotCos, SpotAngle );
}
