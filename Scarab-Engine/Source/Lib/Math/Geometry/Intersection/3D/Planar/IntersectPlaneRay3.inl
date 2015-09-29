/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Intersection/3D/Planar/IntersectPlaneRay3.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Intersection 3D : Planar, Plane and Ray3
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TIntersectPlaneRay3 implementation
template<typename Real>
TIntersectPlaneRay3<Real>::TIntersectPlaneRay3( const TPlane<Real> & rPlane, const TRay3<Real> & rRay, Real fZeroTolerance ):
    TIntersector3<Real>( fZeroTolerance )
{
    m_pPlane = &rPlane;
    m_pRay = &rRay;
}
template<typename Real>
TIntersectPlaneRay3<Real>::~TIntersectPlaneRay3()
{
    // nothing to do
}

template<typename Real>
inline const TPlane<Real> & TIntersectPlaneRay3<Real>::GetPlane() const {
    return (*m_pPlane);
}
template<typename Real>
inline const TRay3<Real> & TIntersectPlaneRay3<Real>::GetRay() const {
    return (*m_pRay);
}

template<typename Real>
inline Real TIntersectPlaneRay3<Real>::GetRayParameter() const {
    return m_fRayParameter;
}

template<typename Real>
Bool TIntersectPlaneRay3<Real>::Test()
{
    Real fDot = ( m_pRay->Direction * m_pPlane->Normal );
    Real fDistance = m_pPlane->DistanceNC( m_pRay->Position );
    if ( MathRealFn->Abs(fDot) >= ZeroTolerance ) {
        m_iType = INTERSECT3D_POINT;
        return ( (fDistance / fDot) <= (Real)0 );
    }
    if ( MathRealFn->Abs(fDistance) < ZeroTolerance ) {
        m_iType = INTERSECT3D_LINE;
        return true;
    }
    m_iType = INTERSECT3D_EMPTY;
    return false;
}
template<typename Real>
Bool TIntersectPlaneRay3<Real>::Compute()
{
    Real fDot = ( m_pRay->Direction * m_pPlane->Normal );
    Real fDistance = m_pPlane->DistanceNC( m_pRay->Position );
    if ( MathRealFn->Abs(fDot) >= ZeroTolerance ) {
        m_iType = INTERSECT3D_POINT;
        m_fRayParameter = -( fDistance / fDot );
        return ( m_fRayParameter >= (Real)0 );
    }
    if ( MathRealFn->Abs(fDistance) < ZeroTolerance ) {
        m_iType = INTERSECT3D_LINE;
        m_fRayParameter = (Real)0;
        return true;
    }
    m_iType = INTERSECT3D_EMPTY;
    return false;
}

