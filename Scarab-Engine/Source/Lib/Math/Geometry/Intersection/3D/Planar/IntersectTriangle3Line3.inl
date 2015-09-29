/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Intersection/3D/Planar/IntersectTriangle3Line3.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Intersection 3D : Planar, Triangle3 and Line3
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TIntersectTriangle3Line3 implementation
template<typename Real>
TIntersectTriangle3Line3<Real>::TIntersectTriangle3Line3( const TTriangle3<Real> & rTriangle, const TLine3<Real> & rLine,
                                                          Real fZeroTolerance ):
    TIntersector3<Real>( fZeroTolerance ), m_vTriangleParameters()
{
    m_pTriangle = &rTriangle;
    m_pLine = &rLine;
}
template<typename Real>
TIntersectTriangle3Line3<Real>::~TIntersectTriangle3Line3()
{
    // nothing to do
}

template<typename Real>
inline const TTriangle3<Real> & TIntersectTriangle3Line3<Real>::GetTriangle() const {
    return (*m_pTriangle);
}
template<typename Real>
inline const TLine3<Real> & TIntersectTriangle3Line3<Real>::GetLine() const {
    return (*m_pLine);
}

template<typename Real>
inline Real TIntersectTriangle3Line3<Real>::GetLineParameter() const {
    return m_fLineParameter;
}
template<typename Real>
inline const TVertex3<Real> & TIntersectTriangle3Line3<Real>::GetTriangleParameters() const {
    return m_vTriangleParameters;
}

template<typename Real>
Bool TIntersectTriangle3Line3<Real>::Test()
{
    // Solve AP + t*Dir = bary1 * AB + bary2 * AC
    //  <=> bary1 * |Dir.N| =  sign(Dir.N) * (Dir.(AP^AC))
    //      bary2 * |Dir.N| =  sign(Dir.N) * (Dir.(AB^AP))
    //          t * |Dir.N| = -sign(Dir.N) * (AP.N)
    TVector3<Real> vAP = ( m_pLine->Position - m_pTriangle->A );
    TVector3<Real> vAB = ( m_pTriangle->B - m_pTriangle->A );
    TVector3<Real> vAC = ( m_pTriangle->C - m_pTriangle->A );
    TVector3<Real> vNormal = ( vAB ^ vAC );
    Real fDot = ( m_pLine->Direction * vNormal );
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
    Real fDdAPxAC = ( m_pLine->Direction * (vAP ^ vAC) ) * fSign;
    if ( fDdAPxAC >= (Real)0 ) {
        Real fDdABxAP = ( m_pLine->Direction * (vAB ^ vAP) ) * fSign;
        if ( fDdABxAP >= (Real)0 ) {
            if ( (fDdAPxAC + fDdABxAP) <= fDot ) {
                m_iType = INTERSECT3D_POINT;
                return true;
            }
        }
    }
    m_iType = INTERSECT3D_EMPTY;
    return false;
}
template<typename Real>
Bool TIntersectTriangle3Line3<Real>::Compute()
{
    // Solve AP + t*Dir = bary1 * AB + bary2 * AC
    //  <=> bary1 * |Dir.N| =  sign(Dir.N) * (Dir.(AP^AC))
    //      bary2 * |Dir.N| =  sign(Dir.N) * (Dir.(AB^AP))
    //          t * |Dir.N| = -sign(Dir.N) * (AP.N)
    TVector3<Real> vAP = ( m_pLine->Position - m_pTriangle->A );
    TVector3<Real> vAB = ( m_pTriangle->B - m_pTriangle->A );
    TVector3<Real> vAC = ( m_pTriangle->C - m_pTriangle->A );
    TVector3<Real> vNormal = ( vAB ^ vAC );
    Real fDot = ( m_pLine->Direction * vNormal );
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
    Real fDdAPxAC = ( m_pLine->Direction * (vAP ^ vAC) ) * fSign;
    if ( fDdAPxAC >= (Real)0 ) {
        Real fDdABxAP = ( m_pLine->Direction * (vAB ^ vAP) ) * fSign;
        if ( fDdABxAP >= (Real)0 ) {
            if ( (fDdAPxAC + fDdABxAP) <= fDot ) {
                m_iType = INTERSECT3D_POINT;
                Real fAPdN = ( vAP * vNormal ) * (-fSign);
                Real fInvDot = MathRealFn->Invert(fDot);
                m_fLineParameter = ( fAPdN * fInvDot );
                m_vTriangleParameters[1] = ( fDdAPxAC * fInvDot );
                m_vTriangleParameters[2] = ( fDdABxAP * fInvDot );
                m_vTriangleParameters[0] = ( ((Real)1) - m_vTriangleParameters[1] - m_vTriangleParameters[2] );
                return true;
            }
        }
    }
    m_iType = INTERSECT3D_EMPTY;
    return false;
}

