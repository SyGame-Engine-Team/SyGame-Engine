/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Intersection/Intersector2.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Base class for intersection of primitives in 2D
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TIntersector2 implementation
template<typename Real>
TIntersector2<Real>::TIntersector2( Real fZeroTolerance )
{
    m_iType = INTERSECT2D_EMPTY;
    m_iSpecifier = INTERSECT2D_SPECIFIER_NONE;

    m_fTimeOfImpact = (Real)0;

    ZeroTolerance = fZeroTolerance;
}
template<typename Real>
TIntersector2<Real>::~TIntersector2()
{
    // nothing to do
}

template<typename Real>
inline Intersect2Type TIntersector2<Real>::GetType() const {
    return m_iType;
}
template<typename Real>
inline Intersect2Specifier TIntersector2<Real>::GetSpecifier() const {
    return m_iSpecifier;
}

template<typename Real>
inline Real TIntersector2<Real>::GetTOI() const {
    return m_fTimeOfImpact;
}

template<typename Real>
Bool TIntersector2<Real>::Test()
{
    Assert(false); // Unimplemented
    return false;
}
template<typename Real>
Bool TIntersector2<Real>::Compute()
{
    Assert(false); // Unimplemented
    return false;
}

template<typename Real>
Bool TIntersector2<Real>::Test( const TVector2<Real> & /*vSpeedA*/, const TVector2<Real> & /*vSpeedB*/, Real /*fMaxTime*/ )
{
    Assert(false); // Unimplemented
    return false;
}
template<typename Real>
Bool TIntersector2<Real>::Compute( const TVector2<Real> & /*vSpeedA*/, const TVector2<Real> & /*vSpeedB*/, Real /*fMaxTime*/ )
{
    Assert(false); // Unimplemented
    return false;
}

