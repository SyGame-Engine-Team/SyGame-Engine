/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Intersection/3D/Planar/IntersectPlaneSegment3.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Intersection 3D : Planar, Plane and Segment3
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TIntersectPlaneSegment3 implementation
template<typename Real>
TIntersectPlaneSegment3<Real>::TIntersectPlaneSegment3( const TPlane<Real> & rPlane, const TSegment3<Real> & rSegment, Real fZeroTolerance ):
    TIntersector3<Real>( fZeroTolerance )
{
    m_pPlane = &rPlane;
    m_pSegment = &rSegment;
}
template<typename Real>
TIntersectPlaneSegment3<Real>::~TIntersectPlaneSegment3()
{
    // nothing to do
}

template<typename Real>
inline const TPlane<Real> & TIntersectPlaneSegment3<Real>::GetPlane() const {
    return (*m_pPlane);
}
template<typename Real>
inline const TSegment3<Real> & TIntersectPlaneSegment3<Real>::GetSegment() const {
    return (*m_pSegment);
}

template<typename Real>
inline Real TIntersectPlaneSegment3<Real>::GetSegmentParameterAB() const {
    return m_fSegmentParameterAB;
}
template<typename Real>
inline Real TIntersectPlaneSegment3<Real>::GetSegmentParameterCDE() const {
    return m_fSegmentParameterCDE;
}

template<typename Real>
Bool TIntersectPlaneSegment3<Real>::Test()
{
    const Real ZERO = (Real)0;

    Real fDistanceA = m_pPlane->DistanceNC( m_pSegment->EndA );
    if ( MathRealFn->Abs(fDistanceA) < ZeroTolerance )
        fDistanceA = ZERO;
    Real fDistanceB = m_pPlane->DistanceNC( m_pSegment->EndB );
    if ( MathRealFn->Abs(fDistanceB) < ZeroTolerance )
        fDistanceB = ZERO;
    Real fProd = ( fDistanceA * fDistanceB );
    if ( fProd < ZERO ) {
        m_iType = INTERSECT3D_POINT;
        return true;
    }
    if ( fProd > ZERO ) {
        m_iType = INTERSECT3D_EMPTY;
        return false;
    }
    if ( fDistanceA != ZERO || fDistanceB != ZERO )
        m_iType = INTERSECT3D_POINT;
    else
        m_iType = INTERSECT3D_SEGMENT;
    return true;
}
template<typename Real>
Bool TIntersectPlaneSegment3<Real>::Compute()
{
    Real fDot = ( m_pSegment->Direction * m_pPlane->Normal );
    Real fDistance = m_pPlane->DistanceNC( m_pSegment->Center );
    if ( MathRealFn->Abs(fDot) >= ZeroTolerance ) {
        m_iType = INTERSECT3D_POINT;
        m_fSegmentParameterCDE = -( fDistance / fDot );
        m_fSegmentParameterAB = ( ( (m_fSegmentParameterCDE / m_pSegment->Extent) + (Real)1 ) * (Real)0.5f );
        return ( MathRealFn->Abs(m_fSegmentParameterCDE) <= m_pSegment->Extent );
    }
    if ( MathRealFn->Abs(fDistance) < ZeroTolerance ) {
        m_iType = INTERSECT3D_LINE;
        m_fSegmentParameterCDE = (Real)0;
        m_fSegmentParameterAB = (Real)0;
        return true;
    }
    m_iType = INTERSECT3D_EMPTY;
    return false;
}


