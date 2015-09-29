/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Curves/3D/CurveLibrary/NaturalSplineChain3.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Curves 3D : Natural Spline curves
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TNaturalSplineChain3 implementation
template<typename Real>
TNaturalSplineChain3<Real>::TNaturalSplineChain3( const Real * arrParameters, UInt iPatchCount,
                                                  const TVertex3<Real> * arrControlPoints, NaturalSplineType iType ):
    TCurveChain3<Real>( arrParameters, iPatchCount )
{
    m_iControlPointCount = iPatchCount + 1;
    m_arrControlPoints = New TVertex3<Real>[m_iControlPointCount];
    for( UInt i = 0; i < m_iControlPointCount; ++i )
        m_arrControlPoints[i] = arrControlPoints[i];

    switch( iType ) {
        case NATURAL_SPLINE_FREE:    _CreateFreeSpline(); break;
        case NATURAL_SPLINE_CLAMPED: _CreateClampedSpline(); break;
        case NATURAL_SPLINE_CLOSED : _CreateClosedSpline(); break;
        default: Assert(false); break;
    }
}
template<typename Real>
TNaturalSplineChain3<Real>::~TNaturalSplineChain3()
{
    DeleteA( m_arrControlPoints );
    DeleteA( m_arrPdT );
    DeleteA( m_arrPdT2 );
    DeleteA( m_arrPdT3 );
}

template<typename Real>
inline UInt TNaturalSplineChain3<Real>::GetControlPointCount() const {
    return m_iControlPointCount;
}
template<typename Real>
inline const TVertex3<Real> & TNaturalSplineChain3<Real>::GetControlPoint( UInt iControlPoint ) const {
    Assert( iControlPoint < m_iControlPointCount );
    return m_arrControlPoints[iControlPoint];
}

template<typename Real>
TVertex3<Real> TNaturalSplineChain3<Real>::P( Real fT ) const
{
    UInt iPatch;
    Real fDT;
    _GetPatchInfo( fT, iPatch, fDT );

    TVertex3<Real> vRes( TVertex3<Real>::Null );
    vRes += m_arrPdT3[iPatch];
    vRes *= fDT;
    vRes += m_arrPdT2[iPatch];
    vRes *= fDT;
    vRes += m_arrPdT[iPatch];
    vRes *= fDT;
    vRes += m_arrControlPoints[iPatch].ToVector();
    return vRes;
}
template<typename Real>
TVector3<Real> TNaturalSplineChain3<Real>::PdT( Real fT ) const
{
    UInt iPatch;
    Real fDT;
    _GetPatchInfo( fT, iPatch, fDT );

    TVector3<Real> vRes;
    vRes  = ( m_arrPdT3[iPatch] * (Real)3 );
    vRes *= fDT;
    vRes += ( m_arrPdT2[iPatch] * (Real)2 );
    vRes *= fDT;
    vRes += m_arrPdT[iPatch];
    return vRes;
}
template<typename Real>
TVector3<Real> TNaturalSplineChain3<Real>::PdT2( Real fT ) const
{
    UInt iPatch;
    Real fDT;
    _GetPatchInfo( fT, iPatch, fDT );

    TVector3<Real> vRes;
    vRes  = ( m_arrPdT3[iPatch] * (Real)6 );
    vRes *= fDT;
    vRes += ( m_arrPdT2[iPatch] * (Real)2 );
    return vRes;
}
template<typename Real>
TVector3<Real> TNaturalSplineChain3<Real>::PdT3( Real fT ) const
{
    UInt iPatch;
    Real fDT;
    _GetPatchInfo( fT, iPatch, fDT );
    return ( m_arrPdT3[iPatch] * (Real)6 );
}

/////////////////////////////////////////////////////////////////////////////////

template<typename Real>
Real TNaturalSplineChain3<Real>::_GetPatchSpeed( UInt iPatch, Real fT ) const
{
    TVector3<Real> vSpeed = ( m_arrPdT3[iPatch] * (Real)3 );
    vSpeed *= fT;
    vSpeed += ( m_arrPdT2[iPatch] * (Real)2 );
    vSpeed *= fT;
    vSpeed += m_arrPdT[iPatch];
    return vSpeed.Norm();
}
template<typename Real>
Real TNaturalSplineChain3<Real>::_GetPatchLength( UInt iPatch, Real fT0, Real fT1 ) const
{
    _PatchUserData tmpPUD;
    tmpPUD.pThis = this;
    tmpPUD.iPatch = iPatch;
    TIntegrator1<Real> speedIntegral( _SpeedFunction, &tmpPUD );
    return speedIntegral.RombergIntegral( fT0, fT1 );
}

template<typename Real>
Void TNaturalSplineChain3<Real>::_CreateFreeSpline()
{
    UInt i;

    // Create temporary values
    Real * arrDT = New Real[m_iPatchCount];
    for( i = 0; i < m_iPatchCount; ++i )
        arrDT[i] = ( m_arrParameters[i+1] - m_arrParameters[i] );

    Real * arrD2T = New Real[m_iPatchCount];
    for( i = 1; i < m_iPatchCount; ++i )
        arrD2T[i] = ( m_arrParameters[i+1] - m_arrParameters[i-1] );

    TVector3<Real> * arrAlpha = New TVector3<Real>[m_iPatchCount];
    for( i = 1; i < m_iPatchCount; ++i ) {
        arrAlpha[i] = (
            ( m_arrControlPoints[i+1].ToVector() * arrDT[i-1] ) -
            ( m_arrControlPoints[i].ToVector()   * arrD2T[i]  ) +
            ( m_arrControlPoints[i-1].ToVector() * arrDT[i]   )
        ) * (Real)3;
        arrAlpha[i] *= MathRealFn->Invert( arrDT[i-1] * arrDT[i] );
    }

    Real * arrEll = New Real[m_iPatchCount + 1];
    Real * arrMu = New Real[m_iPatchCount];
    TVector3<Real> * arrZ = New TVector3<Real>[m_iPatchCount + 1];
    Real fInv;

    arrEll[0] = (Real)1;
    arrMu[0] = (Real)0;
    arrZ[0] = TVector3<Real>::Null;
    for( i = 1; i < m_iPatchCount; ++i ) {
        arrEll[i] = ( (arrD2T[i] * (Real)2) - (arrDT[i-1] * arrMu[i-1]) );
        fInv = MathRealFn->Invert( arrEll[i] );
        arrMu[i] = arrDT[i] * fInv;
        arrZ[i] = ( arrAlpha[i] - (arrZ[i-1] * arrDT[i-1]) ) * fInv;
    }
    arrEll[m_iPatchCount] = (Real)1;
    arrZ[m_iPatchCount] = TVector3<Real>::Null;

    // Build derivative arrays
    m_arrPdT = New TVector3<Real>[m_iPatchCount];
    m_arrPdT2 = New TVector3<Real>[m_iPatchCount + 1];
    m_arrPdT3 = New TVector3<Real>[m_iPatchCount];

    const Real fInv3 = MathRealFn->Invert( (Real)3 );

    m_arrPdT2[m_iPatchCount] = TVector3<Real>::Null;
    for( Int j = (signed)m_iPatchCount - 1; j >= 0; --j ) {
        m_arrPdT2[j] = ( arrZ[j] - (m_arrPdT2[j+1] * arrMu[j]) );
        fInv = MathRealFn->Invert( arrDT[j] );
        m_arrPdT[j] = ( m_arrControlPoints[j+1] - m_arrControlPoints[j] ) * fInv;
        m_arrPdT[j] -= ( m_arrPdT2[j+1] + (m_arrPdT2[j] * (Real)2) ) * arrDT[j] * fInv3;
        m_arrPdT3[j] = ( m_arrPdT2[j+1] - m_arrPdT2[j] ) * fInv * fInv3;
    }

    // Destroy temporary values
    DeleteA( arrZ );
    DeleteA( arrMu );
    DeleteA( arrEll );
    DeleteA( arrAlpha );
    DeleteA( arrD2T );
    DeleteA( arrDT );
}
template<typename Real>
Void TNaturalSplineChain3<Real>::_CreateClampedSpline()
{
    UInt i;

    // Create temporary values
    Real * arrDT = New Real[m_iPatchCount];
    for( i = 0; i < m_iPatchCount; ++i )
        arrDT[i] = ( m_arrParameters[i+1] - m_arrParameters[i] );

    Real * arrD2T = New Real[m_iPatchCount];
    for( i = 1; i < m_iPatchCount; ++i )
        arrD2T[i] = ( m_arrParameters[i+1] - m_arrParameters[i-1] );

    TVector3<Real> * arrAlpha = New TVector3<Real>[m_iPatchCount + 1];

    Real fInv = MathRealFn->Invert( arrDT[0] );
    arrAlpha[0] = ( m_arrControlPoints[1] - m_arrControlPoints[0] ) * ((fInv - 1) * (Real)3);

    fInv = MathRealFn->Invert( arrDT[m_iPatchCount - 1] );
    arrAlpha[m_iPatchCount] = ( m_arrControlPoints[m_iPatchCount] - m_arrControlPoints[m_iPatchCount - 1] ) * ((1 - fInv) * (Real)3);

    for( i = 1; i < m_iPatchCount; ++i ) {
        arrAlpha[i] = (
            ( m_arrControlPoints[i+1].ToVector() * arrDT[i-1] ) -
            ( m_arrControlPoints[i].ToVector() * arrD2T[i]    ) +
            ( m_arrControlPoints[i-1].ToVector() * arrDT[i]   )
        ) * (Real)3;
        arrAlpha[i] *= MathRealFn->Invert( arrDT[i-1] * arrDT[i] );
    }

    Real * arrEll = New Real[m_iPatchCount + 1];
    Real * arrMu = New Real[m_iPatchCount];
    TVector3<Real> * arrZ = New TVector3<Real>[m_iPatchCount + 1];

    arrEll[0] = arrDT[0] * (Real)2;
    arrMu[0] = (Real)0.5f;
    fInv = MathRealFn->Invert( arrEll[0] );
    arrZ[0] = arrAlpha[0] * fInv;
    for( i = 1; i < m_iPatchCount; ++i ) {
        arrEll[i] = ( (arrD2T[i] * (Real)2) - (arrDT[i-1] * arrMu[i-1]) );
        fInv = MathRealFn->Invert( arrEll[i] );
        arrMu[i] = arrDT[i] * fInv;
        arrZ[i] = ( arrAlpha[i] - (arrZ[i-1] * arrDT[i-1]) ) * fInv;
    }
    arrEll[m_iPatchCount] = ( arrDT[m_iPatchCount-1] * (((Real)2) - arrMu[m_iPatchCount-1]) );
    fInv = MathRealFn->Invert( arrEll[m_iPatchCount] );
    arrZ[m_iPatchCount] = ( arrAlpha[m_iPatchCount] - (arrZ[m_iPatchCount-1] * arrDT[m_iPatchCount-1]) ) * fInv;

    // Build derivative arrays
    m_arrPdT = New TVector3<Real>[m_iPatchCount];
    m_arrPdT2 = New TVector3<Real>[m_iPatchCount + 1];
    m_arrPdT3 = New TVector3<Real>[m_iPatchCount];

    const Real fInv3 = MathRealFn->Invert( (Real)3 );

    m_arrPdT2[m_iPatchCount] = arrZ[m_iPatchCount];
    for( Int j = (signed)m_iPatchCount - 1; j >= 0; --j ) {
        m_arrPdT2[j] = ( arrZ[j] - (m_arrPdT2[j+1] * arrMu[j]) );
        fInv = MathRealFn->Invert( arrDT[j] );
        m_arrPdT[j] = ( m_arrControlPoints[j+1] - m_arrControlPoints[j] ) * fInv;
        m_arrPdT[j] -= ( m_arrPdT2[j+1] + (m_arrPdT2[j] * (Real)2) ) * arrDT[j] * fInv3;
        m_arrPdT3[j] = ( m_arrPdT2[j+1] - m_arrPdT2[j] ) * fInv * fInv3;
    }

    // Destroy temporary values
    DeleteA( arrZ );
    DeleteA( arrMu );
    DeleteA( arrEll );
    DeleteA( arrAlpha );
    DeleteA( arrD2T );
    DeleteA( arrDT );
}
template<typename Real>
Void TNaturalSplineChain3<Real>::_CreateClosedSpline()
{
    // Using a general linear solver here, could use
    // a solver optimized for "cyclic banded" matrices ...
    // Anyway, not that critical here ...
    UInt i;

    // Create temporary values
    Real * arrDT = New Real[m_iPatchCount];
    for( i = 0; i < m_iPatchCount; ++i )
        arrDT[i] = ( m_arrParameters[i+1] - m_arrParameters[i] );

    // Build system's matrix
    TMatrixN<Real> matA( m_iPatchCount + 1, m_iPatchCount + 1 );
    matA.MakeNull();
    matA(0,0) = (Real)1;
    matA(0,m_iPatchCount) = (Real)-1;
    for( i = 1; i < m_iPatchCount; ++i ) {
        matA(i,i-1) = arrDT[i-1];
        matA(i,i)   = ( arrDT[i-1] + arrDT[i] ) * (Real)2;
        matA(i,i+1) = arrDT[i];
    }
    matA(m_iPatchCount,m_iPatchCount-1) = arrDT[m_iPatchCount-1];
    matA(m_iPatchCount,0)               = ( arrDT[m_iPatchCount-1] + arrDT[0] ) * (Real)2;
    matA(m_iPatchCount,1)               = arrDT[0];

    // Build second order derivatives
    m_arrPdT2 = New TVector3<Real>[m_iPatchCount + 1];
    m_arrPdT2[0] = TVector3<Real>::Null;
    Real fInv0, fInv1;
    for( i = 1; i < m_iPatchCount; ++i ) {
        fInv0 = MathRealFn->Invert( arrDT[i] );
        fInv1 = MathRealFn->Invert( arrDT[i-1] );
        m_arrPdT2[i] = ( m_arrControlPoints[i+1] - m_arrControlPoints[i] ) * fInv0;
        m_arrPdT2[i] -= ( m_arrControlPoints[i] - m_arrControlPoints[i-1] ) * fInv1;
        m_arrPdT2[i] *= (Real)3;
    }
    fInv0 = MathRealFn->Invert( arrDT[0] );
    fInv1 = MathRealFn->Invert( arrDT[m_iPatchCount-1] );
    m_arrPdT2[m_iPatchCount] = ( m_arrControlPoints[1] - m_arrControlPoints[0] ) * fInv0;
    m_arrPdT2[m_iPatchCount] -= ( m_arrControlPoints[0] - m_arrControlPoints[m_iPatchCount-1] ) * fInv1;
    m_arrPdT2[m_iPatchCount] *= (Real)3;

    // Solve linear systems
    TVectorN<Real> vB(m_iPatchCount + 1);

        // Solve for X
    for( i = 0; i <= m_iPatchCount; ++i )
        vB[i] = m_arrPdT2[i].X;

    TLinearSystemN<Real> linearSolverX( matA, vB );
    Bool bSolved = linearSolverX.SolveSystem();
    Assert( bSolved );

    for( i = 0; i <= m_iPatchCount; ++i )
        m_arrPdT2[i].X = linearSolverX.GetX()[i];

        // Solve for Y
    for( i = 0; i <= m_iPatchCount; ++i )
        vB[i] = m_arrPdT2[i].Y;

    TLinearSystemN<Real> linearSolverY( matA, vB );
    bSolved = linearSolverY.SolveSystem();
    Assert( bSolved );

    for( i = 0; i <= m_iPatchCount; ++i )
        m_arrPdT2[i].Y = linearSolverY.GetX()[i];

        // Solve for Z
    for( i = 0; i <= m_iPatchCount; ++i )
        vB[i] = m_arrPdT2[i].Z;

    TLinearSystemN<Real> linearSolverZ( matA, vB );
    bSolved = linearSolverZ.SolveSystem();
    Assert( bSolved );

    for( i = 0; i <= m_iPatchCount; ++i )
        m_arrPdT2[i].Z = linearSolverZ.GetX()[i];

    // Build first & third order derivatives
    m_arrPdT = New TVector3<Real>[m_iPatchCount];
    m_arrPdT3 = New TVector3<Real>[m_iPatchCount];

    const Real fInv3 = MathRealFn->Invert( (Real)3 );

    for( i = 0; i < m_iPatchCount; ++i ) {
        fInv0 = MathRealFn->Invert( arrDT[i] );
        m_arrPdT[i] = ( m_arrControlPoints[i+1] - m_arrControlPoints[i] ) * fInv0;
        m_arrPdT[i] -= ( m_arrPdT2[i+1] + (m_arrPdT2[i] * (Real)2) ) * arrDT[i] * fInv3;
        m_arrPdT3[i] = ( m_arrPdT2[i+1] - m_arrPdT2[i] ) * fInv0 * fInv3;
    }

    // Destroy temporary values
    DeleteA( arrDT );
}


