/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Intersection/3D/Planar/IntersectPlaneLine3.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Intersection 3D : Planar, Plane and Line3
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TIntersectPlaneLine3 implementation
template<typename Real>
TIntersectPlaneLine3<Real>::TIntersectPlaneLine3( const TPlane<Real> & rPlane, const TLine3<Real> & rLine, Real fZeroTolerance ):
    TIntersector3<Real>( fZeroTolerance )
{
    m_pPlane = &rPlane;
    m_pLine = &rLine;
}
template<typename Real>
TIntersectPlaneLine3<Real>::~TIntersectPlaneLine3()
{
    // nothing to do
}

template<typename Real>
inline const TPlane<Real> & TIntersectPlaneLine3<Real>::GetPlane() const {
    return (*m_pPlane);
}
template<typename Real>
inline const TLine3<Real> & TIntersectPlaneLine3<Real>::GetLine() const {
    return (*m_pLine);
}

template<typename Real>
inline Real TIntersectPlaneLine3<Real>::GetLineParameter() const {
    return m_fLineParameter;
}


template<typename Real>
Bool TIntersectPlaneLine3<Real>::Test()
{
    Real fDot = ( m_pLine->Direction * m_pPlane->Normal );
    if ( MathRealFn->Abs(fDot) >= ZeroTolerance ) {
        m_iType = INTERSECT3D_POINT;
        return true;
    }
    Real fDistance = m_pPlane->DistanceNC( m_pLine->Position );
    if ( MathRealFn->Abs(fDistance) < ZeroTolerance ) {
        m_iType = INTERSECT3D_LINE;
        return true;
    }
    m_iType = INTERSECT3D_EMPTY;
    return false;
}
template<typename Real>
Bool TIntersectPlaneLine3<Real>::Compute()
{
    Real fDot = ( m_pLine->Direction * m_pPlane->Normal );
    Real fDistance = m_pPlane->DistanceNC( m_pLine->Position );
    if ( MathRealFn->Abs(fDot) >= ZeroTolerance ) {
        m_iType = INTERSECT3D_POINT;
        m_fLineParameter = -( fDistance / fDot );
        return true;
    }
    if ( MathRealFn->Abs(fDistance) < ZeroTolerance ) {
        m_iType = INTERSECT3D_LINE;
        m_fLineParameter = (Real)0;
        return true;
    }
    m_iType = INTERSECT3D_EMPTY;
    return false;
}

