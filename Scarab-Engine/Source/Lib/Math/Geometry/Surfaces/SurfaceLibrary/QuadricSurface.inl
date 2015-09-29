/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Surfaces/SurfaceLibrary/QuadricSurface.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Implicit Surfaces : Quadrics
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TQuadricSurface implementation
template<typename Real>
TQuadricSurface<Real>::TQuadricSurface():
    TImplicitSurface<Real>(), m_matA(), m_vB()
{
    for( UInt i = 0; i < 10; ++i )
        m_arrCoefficients[i] = (Real)0;
}
template<typename Real>
TQuadricSurface<Real>::TQuadricSurface( const Real arrCoeffs[10] ):
    TImplicitSurface<Real>(), m_matA(), m_vB()
{
    for( UInt i = 0; i < 10; ++i )
        m_arrCoefficients[i] = arrCoeffs[i];

    m_fC = arrCoeffs[0];
    m_vB.X = arrCoeffs[1];
    m_vB.Y = arrCoeffs[2];
    m_vB.Z = arrCoeffs[3];
    m_matA.m00 = arrCoeffs[4];
    m_matA.m01 = arrCoeffs[5] * (Real)0.5f;
    m_matA.m02 = arrCoeffs[6] * (Real)0.5f;
    m_matA.m10 = m_matA.m01;
    m_matA.m11 = arrCoeffs[7];
    m_matA.m12 = arrCoeffs[8] * (Real)0.5f;
    m_matA.m20 = m_matA.m02;
    m_matA.m21 = m_matA.m12;
    m_matA.m22 = arrCoeffs[9];
}
template<typename Real>
TQuadricSurface<Real>::~TQuadricSurface()
{
    // nothing to do
}

template<typename Real>
inline Real TQuadricSurface<Real>::GetCoefficient( UInt iCoeff ) const {
    Assert( iCoeff < 10 );
    return m_arrCoefficients[iCoeff];
}
template<typename Real>
inline Void TQuadricSurface<Real>::SetCoefficient( UInt iCoeff, Real fValue ) {
    Assert( iCoeff < 10 );
    m_arrCoefficients[iCoeff] = fValue;
}

template<typename Real>
inline const TMatrix3<Real> & TQuadricSurface<Real>::GetA() const {
    return m_matA;
}
template<typename Real>
inline Void TQuadricSurface<Real>::SetA( const TMatrix3<Real> & matValue ) {
    m_matA = matValue;
}

template<typename Real>
inline const TVector3<Real> & TQuadricSurface<Real>::GetB() const {
    return m_vB;
}
template<typename Real>
inline Void TQuadricSurface<Real>::SetB( const TVector3<Real> & vValue ) {
    m_vB = vValue;
}

template<typename Real>
inline Real TQuadricSurface<Real>::GetC() const {
    return m_fC;
}
template<typename Real>
inline Void TQuadricSurface<Real>::SetC( Real fValue ) {
    m_fC = fValue;
}

template<typename Real>
Real TQuadricSurface<Real>::F( const TVertex3<Real> & vPosition ) const
{
    TVector3<Real> vPos = vPosition.ToVector();
    return ( vPos * (m_matA * vPos + m_vB) ) + m_fC;
}
template<typename Real>
Real TQuadricSurface<Real>::FdX( const TVertex3<Real> & vPosition ) const
{
    Real fSum = ( m_matA.m00 * vPosition.X +
                  m_matA.m01 * vPosition.Y +
                  m_matA.m02 * vPosition.Z );
    return ( m_vB.X + (fSum * (Real)2) );
}
template<typename Real>
Real TQuadricSurface<Real>::FdY( const TVertex3<Real> & vPosition ) const
{
    Real fSum = ( m_matA.m10 * vPosition.X +
                  m_matA.m11 * vPosition.Y +
                  m_matA.m12 * vPosition.Z );
    return ( m_vB.Y + (fSum * (Real)2) );
}
template<typename Real>
Real TQuadricSurface<Real>::FdZ( const TVertex3<Real> & vPosition ) const
{
    Real fSum = ( m_matA.m20 * vPosition.X +
                  m_matA.m21 * vPosition.Y +
                  m_matA.m22 * vPosition.Z );
    return ( m_vB.Z + (fSum * (Real)2) );
}
template<typename Real>
Real TQuadricSurface<Real>::FdX2( const TVertex3<Real> & vPosition ) const
{
    return ( m_matA.m00 * (Real)2 );
}
template<typename Real>
Real TQuadricSurface<Real>::FdY2( const TVertex3<Real> & vPosition ) const
{
    return ( m_matA.m11 * (Real)2 );
}
template<typename Real>
Real TQuadricSurface<Real>::FdZ2( const TVertex3<Real> & vPosition ) const
{
    return ( m_matA.m22 * (Real)2 );
}
template<typename Real>
Real TQuadricSurface<Real>::FdXdY( const TVertex3<Real> & vPosition ) const
{
    return ( m_matA.m01 * (Real)2 );
}
template<typename Real>
Real TQuadricSurface<Real>::FdXdZ( const TVertex3<Real> & vPosition ) const
{
    return ( m_matA.m02 * (Real)2 );
}
template<typename Real>
Real TQuadricSurface<Real>::FdYdZ( const TVertex3<Real> & vPosition ) const
{
    return ( m_matA.m12 * (Real)2 );
}

template<typename Real>
QuadricSurfaceType TQuadricSurface<Real>::GetType() const
{
    // Switch to Rational computing
    EqRootRepr eqRootRepr( m_arrCoefficients );

    // Determine the signs of the roots
    UInt iPositiveRoots, iNegativeRoots, iZeroRoots;
    _Compute_RootSigns( eqRootRepr, iPositiveRoots, iNegativeRoots, iZeroRoots );

    // Classify
    QuadricSurfaceType iType = QUADRIC_UNDEFINED;
    switch( iZeroRoots ) {
        case 0: iType = _Classify_Roots0( eqRootRepr, iPositiveRoots ); break;
        case 1: iType = _Classify_Roots1( eqRootRepr, iPositiveRoots ); break;
        case 2: iType = _Classify_Roots2( eqRootRepr, iPositiveRoots ); break;
        case 3: iType = _Classify_Roots3( eqRootRepr ); break;
        default: Assert(false); break;
    }
    return iType;
}

/////////////////////////////////////////////////////////////////////////////////

template<typename Real>
TQuadricSurface<Real>::EqRootRepr::EqRootRepr( const Real arrCoeffs[10] )
{
    QRational Half( 1u, 2u );

    C = QRational( arrCoeffs[0] );
    B.X = QRational( arrCoeffs[1] );
    B.Y = QRational( arrCoeffs[2] );
    B.Z = QRational( arrCoeffs[3] );
    A00 = QRational( arrCoeffs[4] );
    A01 = QRational( arrCoeffs[5] ) * Half;
    A02 = QRational( arrCoeffs[6] ) * Half;
    A11 = QRational( arrCoeffs[7] );
    A12 = QRational( arrCoeffs[8] ) * Half;
    A22 = QRational( arrCoeffs[9] );

    Det00 = ( (A11 * A22) - (A12 * A12) );
    Det01 = ( (A01 * A22) - (A02 * A12) );
    Det02 = ( (A01 * A12) - (A02 * A11) );
    Det11 = ( (A00 * A22) - (A02 * A02) );
    Det12 = ( (A00 * A12) - (A02 * A01) );
    Det22 = ( (A00 * A11) - (A01 * A01) );

    P.X = ( (A00 * Det00) - (A01 * Det01) + (A02 * Det02) );
    P.Y = ( Det00 + Det11 + Det22 );
    P.Z = ( A00 + A11 + A22 );
}

template<typename Real>
UInt TQuadricSurface<Real>::_Compute_SignChanges( const QRational * arrValues, UInt iCount )
{
    UInt iSignChanges = 0;
    QRational qZero(0u,1u);
    QRational qNext, qPrev( arrValues[0] );
    for( UInt i = 1; i < iCount; ++i ) {
        qNext = arrValues[i];
        if ( qNext != qZero ) {
            if ( qPrev * qNext < qZero )
                ++iSignChanges;
            qPrev = qNext;
        }
    }
    return iSignChanges;
}
template<typename Real>
Void TQuadricSurface<Real>::_Compute_RootSigns( EqRootRepr & rootRepr, UInt & outPositiveRoots, UInt & outNegativeRoots, UInt & outZeroRoots )
{
    QRational qZero(0u,1u);
    QRational qOne(1u,1u);

    // Use Sturm sequences
    UInt iSignChangeMI, iSignChange0, iSignChangePI, iDistinctNonZero;
    QRational arrValues[4];

    if ( rootRepr.P.X != qZero ) {
        rootRepr.PSturm.X = ( QRational(2u,9u) * rootRepr.P.Z * rootRepr.P.Z ) - ( QRational(2u,3u) * rootRepr.P.Y );
        rootRepr.PSturm.Y = rootRepr.P.X - ( QRational(1u,9u) * rootRepr.P.Y * rootRepr.P.Z );

        if ( rootRepr.PSturm.X != qZero ) {
            rootRepr.PSturm.Z = -( rootRepr.P.Y + (
                ( (QRational(2u,1u) * rootRepr.P.Z * rootRepr.PSturm.X + QRational(3u,1u) * rootRepr.PSturm.Y) * rootRepr.PSturm.Y )
                / ( rootRepr.PSturm.X * rootRepr.PSturm.X )
            ) );

            arrValues[0] = qOne;
            arrValues[1] = -rootRepr.PSturm.X;
            arrValues[2] = rootRepr.PSturm.Z;
            iSignChangeMI = 1 + _Compute_SignChanges( arrValues, 3 );

            arrValues[0] = -rootRepr.P.X;
            arrValues[1] = rootRepr.P.Y;
            arrValues[2] = rootRepr.PSturm.Y;
            arrValues[3] = rootRepr.PSturm.Z;
            iSignChange0 = _Compute_SignChanges( arrValues, 4 );

            arrValues[0] = qOne;
            arrValues[1] = rootRepr.PSturm.X;
            arrValues[2] = rootRepr.PSturm.Z;
            iSignChangePI = _Compute_SignChanges( arrValues, 3 );
        } else {
            arrValues[0] = -rootRepr.P.X;
            arrValues[1] = rootRepr.P.Y;
            arrValues[2] = rootRepr.PSturm.Y;
            iSignChange0 = _Compute_SignChanges( arrValues, 3 );

            arrValues[0] = qOne;
            arrValues[1] = rootRepr.PSturm.Y;
            iSignChangePI = _Compute_SignChanges( arrValues, 2 );

            iSignChangeMI = 1 + iSignChangePI;
        }

        Assert( iSignChange0 >= iSignChangePI );
        Assert( iSignChangeMI >= iSignChange0 );
        outPositiveRoots = iSignChange0 - iSignChangePI;
        outNegativeRoots = iSignChangeMI - iSignChange0;
        outZeroRoots = 0;

        iDistinctNonZero = outPositiveRoots + outNegativeRoots;
        if ( iDistinctNonZero == 2 ) {
            if ( outPositiveRoots == 2 )
                outPositiveRoots = 3;
            else if ( outNegativeRoots == 2 )
                outNegativeRoots = 3;
            else {
                QRational qTmp = QRational(1u,3u) * rootRepr.P.Z;
                QRational qPoly = ( qTmp * ( qTmp * (qTmp - rootRepr.P.Z) + rootRepr.P.Y ) ) - rootRepr.P.X;
                if ( qPoly > qZero )
                    outPositiveRoots = 2;
                else
                    outNegativeRoots = 2;
            }
        } else if ( iDistinctNonZero == 1 ) {
            if ( outPositiveRoots == 1 )
                outPositiveRoots = 3;
            else
                outNegativeRoots = 3;
        }

        return;
    }

    if ( rootRepr.P.Y != qZero ) {
        rootRepr.PSturm.X = ( QRational(1u,4u) * rootRepr.P.Z * rootRepr.P.Z ) - rootRepr.P.Y;

        arrValues[0] = -qOne;
        arrValues[1] = rootRepr.PSturm.X;
        iSignChangeMI = 1 + _Compute_SignChanges( arrValues, 2 );

        arrValues[0] = rootRepr.P.Y;
        arrValues[1] = -rootRepr.P.Z;
        arrValues[2] = rootRepr.PSturm.X;
        iSignChange0 = _Compute_SignChanges( arrValues, 3 );

        arrValues[0] = qOne;
        arrValues[1] = rootRepr.PSturm.X;
        iSignChangePI = _Compute_SignChanges( arrValues, 2 );

        Assert( iSignChange0 >= iSignChangePI );
        Assert( iSignChangeMI >= iSignChange0 );
        outPositiveRoots = iSignChange0 - iSignChangePI;
        outNegativeRoots = iSignChangeMI - iSignChange0;
        outZeroRoots = 1;

        iDistinctNonZero = outPositiveRoots + outNegativeRoots;
        if ( iDistinctNonZero == 1 )
            outPositiveRoots = 2;

        return;
    }

    if ( rootRepr.P.Z != qZero ) {
        outZeroRoots = 2;

        if ( rootRepr.P.Z > qZero ) {
            outPositiveRoots = 1;
            outNegativeRoots = 0;
        } else {
            outPositiveRoots = 0;
            outNegativeRoots = 1;
        }

        return;
    }

    outPositiveRoots = 0;
    outNegativeRoots = 0;
    outZeroRoots = 3;
}

template<typename Real>
QuadricSurfaceType TQuadricSurface<Real>::_Classify_Roots0( const EqRootRepr & rootRepr, UInt iPositiveRoots )
{
    QRational qZero(0u,1u);
    QRational qForm = (
        rootRepr.B.X * ( (rootRepr.Det00 * rootRepr.B.X) - (rootRepr.Det01 * rootRepr.B.Y) + (rootRepr.Det02 * rootRepr.B.Z) ) -
        rootRepr.B.Y * ( (rootRepr.Det01 * rootRepr.B.X) - (rootRepr.Det11 * rootRepr.B.Y) + (rootRepr.Det12 * rootRepr.B.Z) ) +
        rootRepr.B.Z * ( (rootRepr.Det02 * rootRepr.B.X) - (rootRepr.Det12 * rootRepr.B.Y) + (rootRepr.Det22 * rootRepr.B.Z) )
    );
    QRational qR = ( QRational(1u,16u) * (qForm / rootRepr.P.X) ) - rootRepr.C;
    if ( qR > qZero ) {
        if ( iPositiveRoots == 3 )
            return QUADRIC_ELLIPSOID;
        else if ( iPositiveRoots == 2 )
            return QUADRIC_HYPERBOLOID_ONE_SHEET;
        else if ( iPositiveRoots == 1 )
            return QUADRIC_HYPERBOLOID_TWO_SHEETS;
        else
            return QUADRIC_UNDEFINED;
    } else if ( qR < qZero ) {
        if ( iPositiveRoots == 3 )
            return QUADRIC_UNDEFINED;
        else if ( iPositiveRoots == 2 )
            return QUADRIC_HYPERBOLOID_TWO_SHEETS;
        else if ( iPositiveRoots == 1 )
            return QUADRIC_HYPERBOLOID_ONE_SHEET;
        else
            return QUADRIC_ELLIPSOID;
    }
    if ( iPositiveRoots == 3 || iPositiveRoots == 0 )
        return QUADRIC_POINT;
    return QUADRIC_CONE_ELLIPTIC;
}

template<typename Real>
QuadricSurfaceType TQuadricSurface<Real>::_Classify_Roots1( const EqRootRepr & rootRepr, UInt iPositiveRoots )
{
    // Generate an orthonormal set (I,J,K), where I is an eigenvector
    // of A corresponding to eigenvalue zero.
    QRational qZero(0u,1u);
    QVector vI, vJ, vK;
    if ( rootRepr.Det00 != qZero || rootRepr.Det01 != qZero || rootRepr.Det02 != qZero ) {
        vI.X = rootRepr.Det00;
        vI.Y = -rootRepr.Det01;
        vI.Z = rootRepr.Det02;
        vJ.X = rootRepr.A01;
        vJ.Y = rootRepr.A11;
        vJ.Z = rootRepr.A12;
        vK = ( vI ^ vJ );
        return _Classify_Roots1( rootRepr, iPositiveRoots, vI, vJ, vK );
    }
    if ( rootRepr.Det01 != qZero || rootRepr.Det11 != qZero || rootRepr.Det12 != qZero ) {
        vI.X = -rootRepr.Det01;
        vI.Y = rootRepr.Det11;
        vI.Z = -rootRepr.Det12;
        vJ.X = rootRepr.A02;
        vJ.Y = rootRepr.A12;
        vJ.Z = rootRepr.A22;
        vK = ( vI ^ vJ );
        return _Classify_Roots1( rootRepr, iPositiveRoots, vI, vJ, vK );
    }
    vI.X = rootRepr.Det02;
    vI.Y = -rootRepr.Det12;
    vI.Z = rootRepr.Det22;
    vJ.X = rootRepr.A00;
    vJ.Y = rootRepr.A01;
    vJ.Z = rootRepr.A02;
    vK = ( vI ^ vJ );
    return _Classify_Roots1( rootRepr, iPositiveRoots, vI, vJ, vK );
}
template<typename Real>
QuadricSurfaceType TQuadricSurface<Real>::_Classify_Roots1( const EqRootRepr & rootRepr, UInt iPositiveRoots,
                                                            const QVector & vI, const QVector & vJ, const QVector & vK )
{
    QRational qZero(0u,1u);
    QRational qE0 = ( vI * rootRepr.B );
    if ( qE0 != qZero ) {
        if ( iPositiveRoots == 1 )
            return QUADRIC_PARABOLOID_HYPERBOLIC;
        else
            return QUADRIC_PARABOLOID_ELLIPTIC;
    }

    QRational qF11 = (
        ( vJ.X * ( (rootRepr.A00 * vJ.X) + (rootRepr.A01 * vJ.Y) + (rootRepr.A02 * vJ.Z) ) ) +
        ( vJ.Y * ( (rootRepr.A01 * vJ.X) + (rootRepr.A11 * vJ.Y) + (rootRepr.A12 * vJ.Z) ) ) +
        ( vJ.Z * ( (rootRepr.A02 * vJ.X) + (rootRepr.A12 * vJ.Y) + (rootRepr.A22 * vJ.Z) ) )
    );
    QRational qF12 = (
        ( vK.X * ( (rootRepr.A00 * vJ.X) + (rootRepr.A01 * vJ.Y) + (rootRepr.A02 * vJ.Z) ) ) +
        ( vK.Y * ( (rootRepr.A01 * vJ.X) + (rootRepr.A11 * vJ.Y) + (rootRepr.A12 * vJ.Z) ) ) +
        ( vK.Z * ( (rootRepr.A02 * vJ.X) + (rootRepr.A12 * vJ.Y) + (rootRepr.A22 * vJ.Z) ) )
    );
    QRational qF22 = (
        ( vK.X * ( (rootRepr.A00 * vK.X) + (rootRepr.A01 * vK.Y) + (rootRepr.A02 * vK.Z) ) ) +
        ( vK.Y * ( (rootRepr.A01 * vK.X) + (rootRepr.A11 * vK.Y) + (rootRepr.A12 * vK.Z) ) ) +
        ( vK.Z * ( (rootRepr.A02 * vK.X) + (rootRepr.A12 * vK.Y) + (rootRepr.A22 * vK.Z) ) )
    );
    QRational qG1 = ( vJ * rootRepr.B );
    QRational qG2 = ( vK * rootRepr.B );

    QRational qDet4 = QRational(4u,1u) * ( (qF11 * qF22) - (qF12 * qF12) );
    QRational qR = ( ( qG1 * ((qF22 * qG1) - (qF12 * qG2)) )
                   + ( qG2 * ((qF11 * qG2) - (qF12 * qG1)) ) );
    qR = ( qR / qDet4 ) - rootRepr.C;

    if ( qR > qZero ) {
        if ( iPositiveRoots == 2 )
            return QUADRIC_CYLINDER_ELLIPTIC;
        else if ( iPositiveRoots == 1 )
            return QUADRIC_CYLINDER_HYPERBOLIC;
        else
            return QUADRIC_UNDEFINED;
    } else if ( qR < qZero ) {
        if ( iPositiveRoots == 2 )
            return QUADRIC_UNDEFINED;
        else if ( iPositiveRoots == 1 )
            return QUADRIC_CYLINDER_HYPERBOLIC;
        else
            return QUADRIC_CYLINDER_ELLIPTIC;
    }
    return ( iPositiveRoots == 1 ) ? QUADRIC_TWO_PLANES : QUADRIC_LINE;
}

template<typename Real>
QuadricSurfaceType TQuadricSurface<Real>::_Classify_Roots2( const EqRootRepr & rootRepr, UInt iPositiveRoots )
{
    // Generate an orthonormal set (I,J,K), where I is an eigenvector
    // of A corresponding to eigenvalue zero. K will be an eigenvector
    // corresponding to eigenvalue P2.
    QRational qZero(0u,1u);
    QVector vI, vJ, vK;
    if ( rootRepr.A00 != qZero || rootRepr.A01 != qZero || rootRepr.A02 != qZero ) {
        vK.X = rootRepr.A00;
        vK.Y = rootRepr.A01;
        vK.Z = rootRepr.A02;
    } else if ( rootRepr.A01 != qZero || rootRepr.A11 != qZero || rootRepr.A12 != qZero ) {
        vK.X = rootRepr.A01;
        vK.Y = rootRepr.A11;
        vK.Z = rootRepr.A12;
    } else {
        vK.X = rootRepr.A02;
        vK.Y = rootRepr.A12;
        vK.Z = rootRepr.A22;
    }
    if ( vK.X != qZero ) {
        vJ.X = vK.Y;
        vJ.Y = -vK.X;
        vJ.Z = qZero;
    } else {
        vJ.X = qZero;
        vJ.Y = vK.Z;
        vJ.Z = -vK.Y;
    }
    vI = ( vJ ^ vK );
    return _Classify_Roots2( rootRepr, iPositiveRoots, vI, vJ, vK );
}
template<typename Real>
QuadricSurfaceType TQuadricSurface<Real>::_Classify_Roots2( const EqRootRepr & rootRepr, UInt iPositiveRoots,
                                                            const QVector & vI, const QVector & vJ, const QVector & vK )
{
    QRational qZero(0u,1u);
    QRational qE0 = ( vI * rootRepr.B );
    if ( qE0 != qZero )
        return QUADRIC_CYLINDER_PARABOLIC;
    QRational qE1 = ( vJ * rootRepr.B );
    if ( qE1 != qZero )
        return QUADRIC_CYLINDER_PARABOLIC;

    QRational qDot = ( vK * vK );
    QRational qF1 = rootRepr.P.Z * qDot;
    QRational qE2 = ( vK * rootRepr.B );
    QRational qR = ( (qE2 * qE2) / (QRational(4u,1u) * qF1) ) - rootRepr.C;
    if ( qR > qZero ) {
        if ( iPositiveRoots == 1 )
            return QUADRIC_TWO_PLANES;
        else
            return QUADRIC_UNDEFINED;
    } else if ( qR < qZero ) {
        if ( iPositiveRoots == 1 )
            return QUADRIC_UNDEFINED;
        else
            return QUADRIC_TWO_PLANES;
    }
    return QUADRIC_PLANE;
}

template<typename Real>
QuadricSurfaceType TQuadricSurface<Real>::_Classify_Roots3( const EqRootRepr & rootRepr )
{
    QRational qZero(0u,1u);
    if ( rootRepr.B.X != qZero || rootRepr.B.Y != qZero || rootRepr.B.Z != qZero )
        return QUADRIC_PLANE;
    return QUADRIC_UNDEFINED;
}

