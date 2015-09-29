/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Intersection/3D/Linear/IntersectSegment3.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Intersection 3D : Linear, Segment3 and Segment3
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TIntersectSegment3 implementation
template<typename Real>
TIntersectSegment3<Real>::TIntersectSegment3( const TSegment3<Real> & rSegmentA, const TSegment3<Real> & rSegmentB,
                                              Real fZeroTolerance ):
    TIntersector3<Real>( fZeroTolerance ), m_vClosestPointA(), m_vClosestPointB()
{
    m_pSegmentA = &rSegmentA;
    m_pSegmentB = &rSegmentB;

    m_fDistance = (Real)0;

    m_fParameterA = (Real)0;
    m_fParameterB = (Real)0;
}
template<typename Real>
TIntersectSegment3<Real>::~TIntersectSegment3()
{
    // nothing to do
}

template<typename Real>
inline const TSegment3<Real> & TIntersectSegment3<Real>::GetSegmentA() const {
     return *m_pSegmentA;
}
template<typename Real>
inline const TSegment3<Real> & TIntersectSegment3<Real>::GetSegmentB() const {
     return *m_pSegmentB;
}

template<typename Real>
inline Real TIntersectSegment3<Real>::GetDistance() const {
     return m_fDistance;
}

template<typename Real>
inline Real TIntersectSegment3<Real>::GetParameterA() const {
     return m_fParameterA;
}
template<typename Real>
inline Real TIntersectSegment3<Real>::GetParameterB() const {
     return m_fParameterB;
}

template<typename Real>
inline const TVertex3<Real> & TIntersectSegment3<Real>::GetClosestPointA() const {
     return m_vClosestPointA;
}
template<typename Real>
inline const TVertex3<Real> & TIntersectSegment3<Real>::GetClosestPointB() const {
     return m_vClosestPointB;
}

template<typename Real>
Bool TIntersectSegment3<Real>::Test()
{
    ////////////////////
    return false;
}
template<typename Real>
Bool TIntersectSegment3<Real>::Compute()
{
    TVector3<Real> vA = ( m_pSegmentA->EndB - m_pSegmentA->EndA );
    TVector3<Real> vB = ( m_pSegmentB->EndB - m_pSegmentB->EndA );
    TVector3<Real> vU = ( m_pSegmentA->EndA - m_pSegmentB->EndA );

    Real fSqrLenA = ( vA * vA );
    Real fSqrLenB = ( vB * vB );

    Assert( fSqrLenA > ZeroTolerance );
    Assert( fSqrLenB > ZeroTolerance );

    Real fDot =  ( vA * vB );
    Real fProjA = ( vA * vU );
    Real fProjB = ( vB * vU );

    Real fClampMin = ( -ZeroTolerance );
    Real fClampMax = ( +ZeroTolerance + (Real)1 );

    Real fDenom = ( (fSqrLenA * fSqrLenB) - (fDot * fDot) );
    if ( MathRealFn->EqualsZero(fDenom, ZeroTolerance) )
        m_fParameterA = (Real)0.5f; // parallel case, pick mid
    else {
        m_fParameterA = ( (fDot * fProjB) - (fProjA * fSqrLenB) ) / fDenom;
        m_fParameterA = Clamp<Real>( m_fParameterA, fClampMin, fClampMax );
    }

    m_fParameterB = ( (fDot * m_fParameterA) + fProjB );
    if ( m_fParameterB < ZeroTolerance ) {
        m_fParameterA = -( fProjA / fSqrLenA );
        m_fParameterA = Clamp<Real>( m_fParameterA, fClampMin, fClampMax );
        m_fParameterB = (Real)0;
    } else if ( m_fParameterB > (fSqrLenB - ZeroTolerance) ) {
        m_fParameterA = ( (fDot - fProjA) / fSqrLenA );
        m_fParameterA = Clamp<Real>( m_fParameterA, fClampMin, fClampMax );
        m_fParameterB = (Real)1;
    } else
        m_fParameterB /= fSqrLenB;

    m_vClosestPointA = m_pSegmentA->EndA + ( vA * m_fParameterA );
    m_vClosestPointB = m_pSegmentB->EndA + ( vB * m_fParameterB );

    m_fDistance = (m_vClosestPointB - m_vClosestPointA).Norm();
    if ( m_fDistance <= ZeroTolerance ) {
        m_iType = INTERSECT3D_POINT;
        return true;
    }
    m_iType = INTERSECT3D_EMPTY;
    return false;
}

