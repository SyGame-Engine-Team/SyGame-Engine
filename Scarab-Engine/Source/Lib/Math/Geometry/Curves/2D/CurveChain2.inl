/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Curves/2D/CurveChain2.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Curves 2D : Chained patches definition
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TCurveChain2 implementation
template<typename Real>
TCurveChain2<Real>::TCurveChain2( const Real * arrParameters, UInt iPatchCount ):
    TCurve2<Real>( arrParameters[0], arrParameters[iPatchCount] )
{
    Assert( iPatchCount >= 2 );

    UInt i;

    m_iPatchCount = iPatchCount;
    m_arrParameters = New Real[m_iPatchCount + 1];
    for( i = 0; i <= m_iPatchCount; ++i )
        m_arrParameters[i] = arrParameters[i];

    m_arrLengths = New Real[m_iPatchCount];
    m_arrAccumLengths = New Real[m_iPatchCount];

    for( i = 0; i < m_iPatchCount; ++i )
        m_arrLengths[i] = _GetPatchLength( i, (Real)0, m_arrParameters[i+1] - m_arrParameters[i] );

    m_arrAccumLengths[0] = m_arrLengths[0];
    for( i = 1; i < m_iPatchCount; ++i )
        m_arrAccumLengths[i] = m_arrAccumLengths[i-1] + m_arrLengths[i];
}
template<typename Real>
TCurveChain2<Real>::~TCurveChain2()
{
    DeleteA( m_arrParameters );
    DeleteA( m_arrLengths );
    DeleteA( m_arrAccumLengths );
}

template<typename Real>
Real TCurveChain2<Real>::Length( Real fT0, Real fT1 ) const
{
    Assert( fT0 >= m_fMinT && fT0 <= m_fMaxT );
    Assert( fT1 >= m_fMinT && fT1 <= m_fMaxT );
    Assert( fT0 <= fT1 );
 
    UInt iPatch0, iPatch1;
    Real fDT0, fDT1;
    _GetPatchInfo( fT0, iPatch0, fDT0 );
    _GetPatchInfo( fT1, iPatch1, fDT1 );
    
    if ( iPatch0 == iPatch1 )
        return _GetPatchLength( iPatch0, fDT0, fDT1 );

    Assert( iPatch0 < iPatch1 );
    Real fLength = ( m_arrAccumLengths[iPatch1 - 1] - m_arrAccumLengths[iPatch0] );
    fLength += _GetPatchLength( iPatch0, fDT0, m_arrParameters[iPatch0 + 1] - m_arrParameters[iPatch0] );
    fLength += _GetPatchLength( iPatch1, (Real)0, fDT1 );
    return fLength;
}
template<typename Real>
Real TCurveChain2<Real>::Parameter( Real fLength, UInt iMaxIterations, Real fTolerance ) const
{
    // Use Newton's root finding method (mixed with bisection)

    // Out of range
    if ( fLength <= (Real)0 )
        return m_fMinT;
    if ( fLength >= m_arrAccumLengths[m_iPatchCount - 1] )
        return m_fMaxT;

    // Patch infos
    UInt iPatch;
    for ( iPatch = 0; iPatch < m_iPatchCount; ++iPatch ) {
        if ( fLength < m_arrAccumLengths[iPatch] )
            break;
    }
    if ( iPatch >= m_iPatchCount )
        return m_fMaxT;

    // Lengths
    Real fLen = fLength;
    if ( iPatch > 0 )
        fLen -= m_arrAccumLengths[iPatch - 1];
    Real fPatchLen = m_arrLengths[iPatch];

    // Initial guess
    Real fDT1 = m_arrParameters[iPatch + 1] - m_arrParameters[iPatch];
    Real fDT0 = fDT1 * ( fLen / fPatchLen );

    // Interval bounding
    Real fLow = (Real)0;
    Real fHigh = fDT1;
    Real fDelta, fTry;
    for( UInt i = 0; i < iMaxIterations; ++i ) {
        fDelta = ( _GetPatchLength(iPatch, (Real)0, fDT0) - fLen );
        if ( MathRealFn->Abs(fDelta) < fTolerance )
            return m_arrParameters[iPatch] + fDT0; // close enough
        fTry = fDT0 - ( fDelta / _GetPatchSpeed(iPatch, fDT0) );
        if ( fDelta > (Real)0 ) {
            fHigh = fDT0;
            if ( fTry <= fLow ) // out of bound
                fDT0 = ( fLow + fHigh ) * (Real)0.5f;
            else
                fDT0 = fTry;
        } else {
            fLow = fDT0;
            if ( fTry >= fHigh ) // out of bound
                fDT0 = ( fLow + fHigh ) * (Real)0.5f;
            else
                fDT0 = fTry;
        }
    }
    return m_arrParameters[iPatch] + fDT0;
}

template<typename Real>
inline UInt TCurveChain2<Real>::GetPatchCount() const {
    return m_iPatchCount;
}
template<typename Real>
inline Void TCurveChain2<Real>::GetPatch( UInt iPatch, Real & outMinT, Real & outMaxT ) const {
    Assert( iPatch < m_iPatchCount );
    outMinT = m_arrParameters[iPatch];
    outMaxT = m_arrParameters[iPatch + 1];
}
template<typename Real>
Void TCurveChain2<Real>::SetPatch( UInt iPatch, Real fMinT, Real fMaxT )
{
    Assert( iPatch < m_iPatchCount );
    Assert( fMinT < fMaxT );
    if ( iPatch == 0 ) {
        Assert( fMaxT < m_arrParameters[2] );
        m_arrParameters[0] = fMinT;
        m_arrParameters[1] = fMaxT;
        m_fMinT = fMinT;
        return;
    }
    if ( iPatch == (m_iPatchCount - 1) ) {
        Assert( fMinT > m_arrParameters[m_iPatchCount - 2] );
        m_arrParameters[m_iPatchCount - 1] = fMinT;
        m_arrParameters[m_iPatchCount] = fMaxT;
        m_fMaxT = fMaxT;
        return;
    }
    Assert( fMinT > m_arrParameters[iPatch - 1] );
    Assert( fMaxT < m_arrParameters[iPatch + 2] );
    m_arrParameters[iPatch] = fMinT;
    m_arrParameters[iPatch + 1] = fMaxT;
}

/////////////////////////////////////////////////////////////////////////////////

template<typename Real>
inline Real TCurveChain2<Real>::_SpeedFunction( Real fT, Void * pUserData ) {
    _PatchUserData * pPUD = (_PatchUserData*)pUserData;
    return pPUD->pThis->_GetPatchSpeed( pPUD->iPatch, fT );
}

template<typename Real>
Void TCurveChain2<Real>::_GetPatchInfo( Real fT, UInt & outPatch, Real & outDT ) const
{
    if ( fT <= m_arrParameters[0] ) {
        outPatch = 0;
        outDT = (Real)0;
    } else if ( fT >= m_arrParameters[m_iPatchCount] ) {
        outPatch = m_iPatchCount - 1;
        outDT = ( m_arrParameters[m_iPatchCount] - m_arrParameters[m_iPatchCount - 1] );
    } else {
        for ( UInt i = 0; i < m_iPatchCount; ++i ) {
            if ( fT < m_arrParameters[i + 1] ) {
                outPatch = i;
                outDT = ( fT - m_arrParameters[i] );
                break;
            }
        }
    }
}

