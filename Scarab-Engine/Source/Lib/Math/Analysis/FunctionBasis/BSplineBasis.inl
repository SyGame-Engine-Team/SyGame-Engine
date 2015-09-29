/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Analysis/FuctionBasis/BSplineBasis.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Function basis, BSpline N(i,k) basis
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TBSplineBasis implementation
template<typename Real>
TBSplineBasis<Real>::TBSplineBasis()
{
    m_iControlPointCount = 0;
    m_iDegree = 0;
    m_iBasisPitch = 0;
    m_iBasisDim = 0;

    m_bUniform = false;
    m_bPeriodic = false;

    m_iKnotCount = 0;
    m_vKnots = NULL;

    m_arrN = NULL;
    m_arrNdT = NULL;
    m_arrNdT2 = NULL;
    m_arrNdT3 = NULL;
}
template<typename Real>
TBSplineBasis<Real>::~TBSplineBasis()
{
    if ( m_vKnots != NULL )
        DeleteA( m_vKnots );

    if ( m_arrN != NULL )
        DeleteA( m_arrN );
    if ( m_arrNdT != NULL )
        DeleteA( m_arrNdT );
    if ( m_arrNdT2 != NULL )
        DeleteA( m_arrNdT2 );
    if ( m_arrNdT3 != NULL )
        DeleteA( m_arrNdT3 );
}

template<typename Real>
Void TBSplineBasis<Real>::InitUniform( UInt iControlPointCount, UInt iDegree, Bool bPeriodic )
{
    Assert( m_arrN == NULL );

    m_bUniform = true;
    _Initialize( iControlPointCount, iDegree, bPeriodic );

    Int iCPC = (signed)m_iControlPointCount;
    Int iD = (signed)m_iDegree;
    Int i, iKC = (signed)m_iKnotCount;
    Real fFactor = MathRealFn->Invert( (Real)(iCPC - iD) ); // 1 / n-d+1
    if ( m_bPeriodic ) {
        for ( i = 0; i < iKC; ++i )
            m_vKnots[i] = fFactor * (Real)( i - iD );
    } else {
        for ( i = 0; i <= iD; ++i )
            m_vKnots[i] = (Real)0;
        for ( /**/; i < iCPC; ++i )
            m_vKnots[i] = fFactor * (Real)( i - iD );
        for ( /**/; i < iKC; ++i )
            m_vKnots[i] = (Real)1;
    }
}
template<typename Real>
Void TBSplineBasis<Real>::InitNonUniform( UInt iControlPointCount, UInt iDegree, const Real * arrKnotVector )
{
    Assert( m_arrN == NULL );

    m_bUniform = false;
    _Initialize( iControlPointCount, iDegree, false );

    UInt i;
    for ( i = 0; i <= m_iDegree; ++i )
        m_vKnots[i] = (Real)0;
    for ( /**/; i < m_iControlPointCount; ++i )
        m_vKnots[i] = *(arrKnotVector++);
    for ( /**/; i < m_iKnotCount; ++i )
        m_vKnots[i] = (Real)1;
}

template<typename Real>
inline UInt TBSplineBasis<Real>::GetControlPointCount() const {
    return m_iControlPointCount;
}
template<typename Real>
inline UInt TBSplineBasis<Real>::GetDegree() const {
    return m_iDegree;
}

template<typename Real>
inline Bool TBSplineBasis<Real>::IsUniform() const {
    return m_bUniform;
}
template<typename Real>
inline Bool TBSplineBasis<Real>::IsPeriodic() const {
    return m_bPeriodic;
}

template<typename Real>
inline UInt TBSplineBasis<Real>::GetKnotCount() const {
    return ( m_iControlPointCount - m_iBasisDim );
}
template<typename Real>
inline Real TBSplineBasis<Real>::GetKnot( UInt iKnot ) const {
    Assert( iKnot <= (m_iControlPointCount - m_iBasisDim) );
    return m_vKnots[m_iBasisDim + iKnot];
}
template<typename Real>
inline Void TBSplineBasis<Real>::SetKnot( UInt iKnot, Real fKnotValue ) const {
    Assert( !m_bUniform );
    Assert( iKnot <= (m_iControlPointCount - m_iBasisDim) );
    m_vKnots[m_iBasisDim + iKnot] = fKnotValue;
}

template<typename Real>
inline Real TBSplineBasis<Real>::N( UInt i ) const {
    Assert( i < m_iBasisPitch );
    return m_arrN[ m_iDegree * m_iBasisPitch + i ];
}
template<typename Real>
inline Real TBSplineBasis<Real>::NdT( UInt i ) const {
    Assert( i < m_iBasisPitch );
    return m_arrNdT[ m_iDegree * m_iBasisPitch + i ];
}
template<typename Real>
inline Real TBSplineBasis<Real>::NdT2( UInt i ) const {
    Assert( i < m_iBasisPitch );
    return m_arrNdT2[ m_iDegree * m_iBasisPitch + i ];
}
template<typename Real>
inline Real TBSplineBasis<Real>::NdT3( UInt i ) const {
    Assert( i < m_iBasisPitch );
    return m_arrNdT3[ m_iDegree * m_iBasisPitch + i ];
}

template<typename Real>
Void TBSplineBasis<Real>::Evaluate( Real fT, UInt iOrder, UInt & outMinIndex, UInt & outMaxIndex ) const
{
    Assert( iOrder <= 3 );

    UInt iKnot = _GetKnot( fT );

    m_arrN[iKnot] = (Real)1;
    if ( iOrder >= 1 ) {
        m_arrNdT[iKnot] = (Real)0;
        if ( iOrder >= 2 ) {
            m_arrNdT2[iKnot] = (Real)0;
            if ( iOrder >= 3 )
                m_arrNdT3[iKnot] = (Real)0;
        }
    }

    Real fN0 = fT - m_vKnots[iKnot];
    Real fN1 = m_vKnots[iKnot+1] - fT;
    Real fInvD0, fInvD1;

    Real *pRow, *pPrevRow, *pSavedPrevRow;
    UInt iRow, iPrevRow, i, j;

    for( i = 1; i <= m_iDegree; ++i ) {
        iRow = ( i * m_iBasisPitch );
        iPrevRow = ( iRow - m_iBasisPitch );

        fInvD0 = MathRealFn->Invert( m_vKnots[iKnot+i] - m_vKnots[iKnot] );
        fInvD1 = MathRealFn->Invert( m_vKnots[iKnot+1] - m_vKnots[iKnot-i+1] );

        pRow = m_arrN + iRow;
        pPrevRow = m_arrN + iPrevRow;

        pRow[iKnot]   = ( fN0 * pPrevRow[iKnot]     ) * fInvD0;
        pRow[iKnot-i] = ( fN1 * pPrevRow[iKnot-i+1] ) * fInvD1;

        if ( iOrder >= 1 ) {
            pSavedPrevRow = pPrevRow;
            pRow = m_arrNdT + iRow;
            pPrevRow = m_arrNdT + iPrevRow;

            pRow[iKnot]   = ( fN0 * pPrevRow[iKnot]     + pSavedPrevRow[iKnot]     ) * fInvD0;
            pRow[iKnot-i] = ( fN1 * pPrevRow[iKnot-i+1] - pSavedPrevRow[iKnot-i+1] ) * fInvD1;

            if ( iOrder >= 2 ) {
                pSavedPrevRow = pPrevRow;
                pRow = m_arrNdT2 + iRow;
                pPrevRow = m_arrNdT2 + iPrevRow;

                pRow[iKnot]   = ( fN0 * pPrevRow[iKnot]     + ((Real)2) * pSavedPrevRow[iKnot]     ) * fInvD0;
                pRow[iKnot-i] = ( fN1 * pPrevRow[iKnot-i+1] - ((Real)2) * pSavedPrevRow[iKnot-i+1] ) * fInvD1;

                if ( iOrder >= 3 ) {
                    pSavedPrevRow = pPrevRow;
                    pRow = m_arrNdT3 + iRow;
                    pPrevRow = m_arrNdT3 + iPrevRow;

                    pRow[iKnot]   = ( fN0 * pPrevRow[iKnot]     + ((Real)3) * pSavedPrevRow[iKnot]     ) * fInvD0;
                    pRow[iKnot-i] = ( fN1 * pPrevRow[iKnot-i+1] - ((Real)3) * pSavedPrevRow[iKnot-i+1] ) * fInvD1;
                }
            }
        }
    }
    for( i = 2; i <= m_iDegree; ++i ) {
        iRow = ( i * m_iBasisPitch );
        iPrevRow = ( iRow - m_iBasisPitch );

        for( j = iKnot - i + 1; j < iKnot; ++j ) {
            fN0 = fT - m_vKnots[j];
            fN1 = m_vKnots[i+j+1] - fT;
            fInvD0 = MathRealFn->Invert( m_vKnots[i+j] - m_vKnots[j] );
            fInvD1 = MathRealFn->Invert( m_vKnots[i+j+1] - m_vKnots[j+1] );

            pRow = m_arrN + iRow;
            pPrevRow = m_arrN + iPrevRow;

            pRow[j] = ( ( fN0 * pPrevRow[j] ) * fInvD0 +
                        ( fN1 * pPrevRow[j+1] ) * fInvD1 );

            if ( iOrder >= 1 ) {
                pSavedPrevRow = pPrevRow;
                pRow = m_arrNdT + iRow;
                pPrevRow = m_arrNdT + iPrevRow;

                pRow[j] = ( ( fN0 * pPrevRow[j]   + pSavedPrevRow[j]   ) * fInvD0 +
                            ( fN1 * pPrevRow[j+1] - pSavedPrevRow[j+1] ) * fInvD1 );

                if ( iOrder >= 2 ) {
                    pSavedPrevRow = pPrevRow;
                    pRow = m_arrNdT2 + iRow;
                    pPrevRow = m_arrNdT2 + iPrevRow;

                    pRow[j] = ( ( fN0 * pPrevRow[j]   + ((Real)2) * pSavedPrevRow[j]   ) * fInvD0 +
                                ( fN1 * pPrevRow[j+1] - ((Real)2) * pSavedPrevRow[j+1] ) * fInvD1 );

                    if ( iOrder >= 3 ) {
                        pSavedPrevRow = pPrevRow;
                        pRow = m_arrNdT3 + iRow;
                        pPrevRow = m_arrNdT3 + iPrevRow;

                        pRow[j] = ( ( fN0 * pPrevRow[j]   + ((Real)3) * pSavedPrevRow[j]   ) * fInvD0 +
                                    ( fN1 * pPrevRow[j+1] - ((Real)3) * pSavedPrevRow[j+1] ) * fInvD1 );
                    }
                }
            }
        }
    }
    outMinIndex = iKnot - m_iDegree;
    outMaxIndex = iKnot;
}

/////////////////////////////////////////////////////////////////////////////////

template<typename Real>
Void TBSplineBasis<Real>::_Initialize( UInt iControlPointCount, UInt iDegree, Bool bPeriodic )
{
    Assert( iControlPointCount >= 2 );                           // 1 <= n
    Assert( iDegree >= 1 && iDegree <= (iControlPointCount-1) ); // 1 <= d <= n

    m_iControlPointCount = iControlPointCount;        // n+1
    m_iDegree = iDegree;                              // d
    m_iBasisPitch = m_iControlPointCount + m_iDegree; // n+d+1
    m_iBasisDim = m_iDegree + 1;                      // d+1

    m_bPeriodic = bPeriodic;

    m_iKnotCount = m_iBasisPitch + 1; // n+d+2
    m_vKnots = New Real[m_iKnotCount];

    UInt i, iBasisSize = ( m_iBasisPitch * m_iBasisDim );

    m_arrN = New Real[iBasisSize];
    for( i = 0; i < iBasisSize; ++i )
        m_arrN[i] = (Real)0;
    m_arrNdT = New Real[iBasisSize];
    for( i = 0; i < iBasisSize; ++i )
        m_arrNdT[i] = (Real)0;
    m_arrNdT2 = New Real[iBasisSize];
    for( i = 0; i < iBasisSize; ++i )
        m_arrNdT2[i] = (Real)0;
    m_arrNdT3 = New Real[iBasisSize];
    for( i = 0; i < iBasisSize; ++i )
        m_arrNdT3[i] = (Real)0;
}
template<typename Real>
UInt TBSplineBasis<Real>::_GetKnot( Real & fT ) const
{
    if ( m_bPeriodic ) {
        // Wrap periodic spline
        if ( (fT < (Real)0) || (fT > (Real)1) )
            fT -= MathRealFn->Floorf(fT);
    } else {
        // Clamp open spline
        if ( fT <= (Real)0 ) {
            fT = (Real)0;
            return m_iDegree;
        }
        if ( fT >= (Real)1 ) {
            fT = (Real)1;
            return m_iControlPointCount - 1;
        }
    }

    UInt iKnot;
    if ( m_bUniform )
        iKnot = m_iDegree + MathRealFn->Floor( fT * (Real)(m_iControlPointCount - m_iDegree) );
    else {
        for( iKnot = m_iDegree + 1; iKnot <= m_iControlPointCount; ++iKnot ) {
            if ( fT < m_vKnots[iKnot] )
                break;
        }
        --iKnot;
    }
    return iKnot;
}

