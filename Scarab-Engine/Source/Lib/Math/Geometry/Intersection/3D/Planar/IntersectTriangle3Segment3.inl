/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Intersection/3D/Planar/IntersectTriangle3Segment3.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Intersection 3D : Planar, Triangle3 and Segment3
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TIntersectTriangle3Segment3 implementation
template<typename Real>
TIntersectTriangle3Segment3<Real>::TIntersectTriangle3Segment3( const TTriangle3<Real> & rTriangle, const TSegment3<Real> & rSegment,
                                                                Real fZeroTolerance ):
    TIntersector3<Real>( fZeroTolerance ), m_vTriangleParameters()
{
    m_pTriangle = &rTriangle;
    m_pSegment = &rSegment;
}
template<typename Real>
TIntersectTriangle3Segment3<Real>::~TIntersectTriangle3Segment3()
{
    // nothing to do
}

template<typename Real>
inline const TTriangle3<Real> & TIntersectTriangle3Segment3<Real>::GetTriangle() const {
    return (*m_pTriangle);
}
template<typename Real>
inline const TSegment3<Real> & TIntersectTriangle3Segment3<Real>::GetSegment() const {
    return (*m_pSegment);
}

template<typename Real>
inline Real TIntersectTriangle3Segment3<Real>::GetSegmentParameterAB() const {
    return m_fSegmentParameterAB;
}
template<typename Real>
inline Real TIntersectTriangle3Segment3<Real>::GetSegmentParameterCDE() const {
    return m_fSegmentParameterCDE;
}
template<typename Real>
inline const TVertex3<Real> & TIntersectTriangle3Segment3<Real>::GetTriangleParameters() const {
    return m_vTriangleParameters;
}

template<typename Real>
inline UInt TIntersectTriangle3Segment3<Real>::GetContactCount() const {
    return m_iContactCount;
}
template<typename Real>
inline const TVertex3<Real> & TIntersectTriangle3Segment3<Real>::GetContact( UInt iContact ) const {
    Assert( iContact < m_iContactCount );
    return m_arrContacts[iContact];
}

template<typename Real>
Bool TIntersectTriangle3Segment3<Real>::Test()
{
    // Solve AP + t*Dir = bary1 * AB + bary2 * AC
    //  <=> bary1 * |Dir.N| =  sign(Dir.N) * (Dir.(AP^AC))
    //      bary2 * |Dir.N| =  sign(Dir.N) * (Dir.(AB^AP))
    //          t * |Dir.N| = -sign(Dir.N) * (AP.N)
    TVector3<Real> vAP = ( m_pSegment->Center - m_pTriangle->A );
    TVector3<Real> vAB = ( m_pTriangle->B - m_pTriangle->A );
    TVector3<Real> vAC = ( m_pTriangle->C - m_pTriangle->A );
    TVector3<Real> vNormal = ( vAB ^ vAC );
    Real fDot = ( m_pSegment->Direction * vNormal );
    Real fSign;
    if ( fDot < -ZeroTolerance ) {
        fSign = (Real)(-1);
        fDot = -fDot;
    } else if ( fDot > ZeroTolerance )
        fSign = (Real)(+1);
    else {
        // Parallel, return no intersection in this case,
        // even if the line is on the same plane
        m_iType = INTERSECT3D_EMPTY;
        return false;
    }
    Real fDdAPxAC = ( m_pSegment->Direction * (vAP ^ vAC) ) * fSign;
    if ( fDdAPxAC >= (Real)0 ) {
        Real fDdABxAP = ( m_pSegment->Direction * (vAB ^ vAP) ) * fSign;
        if ( fDdABxAP >= (Real)0 ) {
            if ( (fDdAPxAC + fDdABxAP) <= fDot ) {
                Real fAPdN = ( vAP * vNormal ) * (-fSign);
                Real fExtDot = ( m_pSegment->Extent * fDot );
                if ( fAPdN >= -fExtDot && fAPdN <= fExtDot ) {
                    m_iType = INTERSECT3D_POINT;
                    return true;
                }
            }
        }
    }
    m_iType = INTERSECT3D_EMPTY;
    return false;
}
template<typename Real>
Bool TIntersectTriangle3Segment3<Real>::Compute()
{
    // Solve AP + t*Dir = bary1 * AB + bary2 * AC
    //  <=> bary1 * |Dir.N| =  sign(Dir.N) * (Dir.(AP^AC))
    //      bary2 * |Dir.N| =  sign(Dir.N) * (Dir.(AB^AP))
    //          t * |Dir.N| = -sign(Dir.N) * (AP.N)
    TVector3<Real> vAP = ( m_pSegment->Center - m_pTriangle->A );
    TVector3<Real> vAB = ( m_pTriangle->B - m_pTriangle->A );
    TVector3<Real> vAC = ( m_pTriangle->C - m_pTriangle->A );
    TVector3<Real> vNormal = ( vAB ^ vAC );
    Real fDot = ( m_pSegment->Direction * vNormal );
    Real fSign;
    if ( fDot < -ZeroTolerance ) {
        fSign = (Real)(-1);
        fDot = -fDot;
    } else if ( fDot > ZeroTolerance )
        fSign = (Real)(+1);
    else {
        // Parallel, return no intersection in this case,
        // even if the line is on the same plane
        m_iType = INTERSECT3D_EMPTY;
        return false;
    }
    Real fDdAPxAC = ( m_pSegment->Direction * (vAP ^ vAC) ) * fSign;
    if ( fDdAPxAC >= (Real)0 ) {
        Real fDdABxAP = ( m_pSegment->Direction * (vAB ^ vAP) ) * fSign;
        if ( fDdABxAP >= (Real)0 ) {
            if ( (fDdAPxAC + fDdABxAP) <= fDot ) {
                Real fAPdN = ( vAP * vNormal ) * (-fSign);
                Real fExtDot = ( m_pSegment->Extent * fDot );
                if ( fAPdN >= -fExtDot && fAPdN <= fExtDot ) {
                    m_iType = INTERSECT3D_POINT;
                    Real fInvDot = MathRealFn->Invert(fDot);
                    m_fSegmentParameterCDE = ( fAPdN * fInvDot );
                    m_fSegmentParameterAB = ( ( (m_fSegmentParameterCDE / m_pSegment->Extent) + (Real)1 ) * (Real)0.5f );
                    m_vTriangleParameters[1] = ( fDdAPxAC * fInvDot );
                    m_vTriangleParameters[2] = ( fDdABxAP * fInvDot );
                    m_vTriangleParameters[0] = ( ((Real)1) - m_vTriangleParameters[1] - m_vTriangleParameters[2] );
                    return true;
                }
            }
        }
    }
    m_iType = INTERSECT3D_EMPTY;
    return false;
}


