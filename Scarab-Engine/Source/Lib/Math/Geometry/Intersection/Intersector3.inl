/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Intersection/Intersector3.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Base class for intersection of primitives in 3D
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TIntersector3 implementation
template<typename Real>
TIntersector3<Real>::TIntersector3( Real fZeroTolerance )
{
    m_iType = INTERSECT3D_EMPTY;
    m_iSpecifier = INTERSECT3D_SPECIFIER_NONE;

    m_fTimeOfImpact = (Real)0;

    ZeroTolerance = fZeroTolerance;
}
template<typename Real>
TIntersector3<Real>::~TIntersector3()
{
    // nothing to do
}

template<typename Real>
inline Intersect3Type TIntersector3<Real>::GetType() const {
    return m_iType;
}
template<typename Real>
inline Intersect3Specifier TIntersector3<Real>::GetSpecifier() const {
    return m_iSpecifier;
}

template<typename Real>
inline Real TIntersector3<Real>::GetTOI() const {
    return m_fTimeOfImpact;
}

template<typename Real>
Bool TIntersector3<Real>::Test()
{
    Assert(false); // Unimplemented
    return false;
}
template<typename Real>
Bool TIntersector3<Real>::Compute()
{
    Assert(false); // Unimplemented
    return false;
}

template<typename Real>
Bool TIntersector3<Real>::Test( const TVector3<Real> & /*vSpeedA*/, const TVector3<Real> & /*vSpeedB*/, Real /*fMaxTime*/ )
{
    Assert(false); // Unimplemented
    return false;
}
template<typename Real>
Bool TIntersector3<Real>::Compute( const TVector3<Real> & /*vSpeedA*/, const TVector3<Real> & /*vSpeedB*/, Real /*fMaxTime*/ )
{
    Assert(false); // Unimplemented
    return false;
}

