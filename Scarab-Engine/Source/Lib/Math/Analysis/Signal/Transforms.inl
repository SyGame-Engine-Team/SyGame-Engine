/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Analysis/Signal/Transforms.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Signal analysis : Various transforms for signal analysis
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TSignalTransform implementation
template <typename Real>
TSignalTransform<Real>::TSignalTransform( const TComplex<Real> * arrSignal, UInt iSampleCount )
{
    Assert( arrSignal != NULL );
    Assert( iSampleCount > 0 );

    m_iSampleCount = iSampleCount;
    m_arrBaseSignal = New TComplex<Real>[m_iSampleCount];
    m_arrBaseSignal_R = NULL;

    for( UInt i = 0; i < m_iSampleCount; ++i )
        m_arrBaseSignal[i] = arrSignal[i];
}
template <typename Real>
TSignalTransform<Real>::TSignalTransform( const Real * arrSignal, UInt iSampleCount )
{
    Assert( arrSignal != NULL );
    Assert( iSampleCount > 0 );

    m_iSampleCount = iSampleCount;
    m_arrBaseSignal = NULL;
    m_arrBaseSignal_R = New Real[m_iSampleCount];

    for( UInt i = 0; i < m_iSampleCount; ++i )
        m_arrBaseSignal_R[i] = arrSignal[i];
}
template <typename Real>
TSignalTransform<Real>::~TSignalTransform()
{
    if ( m_arrBaseSignal != NULL )
        DeleteA( m_arrBaseSignal );
    if ( m_arrBaseSignal_R != NULL )
        DeleteA( m_arrBaseSignal_R );
}

template <typename Real>
inline Bool TSignalTransform<Real>::IsComplexSignal() const {
    return ( m_arrBaseSignal != NULL );
}
template <typename Real>
inline Bool TSignalTransform<Real>::IsRealSignal() const {
    return ( m_arrBaseSignal_R != NULL );
}
template <typename Real>
inline const Void * TSignalTransform<Real>::GetSignal() const {
    if ( m_arrBaseSignal != NULL )
        return m_arrBaseSignal;
    return m_arrBaseSignal_R;
}

template <typename Real>
inline UInt TSignalTransform<Real>::GetSampleCount() const {
    return m_iSampleCount;
}
template <typename Real>
inline Void TSignalTransform<Real>::GetSignalSample( TComplex<Real> * outValue, UInt iSample ) const {
    Assert( m_arrBaseSignal != NULL );
    Assert( iSample < m_iSampleCount );
    *outValue = m_arrBaseSignal[iSample];
}
template <typename Real>
inline Void TSignalTransform<Real>::GetSignalSample( Real * outValue, UInt iSample ) const {
    Assert( m_arrBaseSignal_R != NULL );
    Assert( iSample < m_iSampleCount );
    *outValue = m_arrBaseSignal_R[iSample];
}

template <typename Real>
Void TSignalTransform<Real>::DFT( TComplex<Real> * arrOutputSignal )
{
    // Lacking better implementation for now, fallback to default
    _DFT_Reference( arrOutputSignal, false );
}
template <typename Real>
Void TSignalTransform<Real>::InvDFT( TComplex<Real> * arrOutputSignal )
{    
    // Lacking better implementation for now, fallback to default
    _DFT_Reference( arrOutputSignal, true );
}

template <typename Real>
Void TSignalTransform<Real>::DFT( Real * arrOutputSignal )
{
    // Lacking better implementation for now, fallback to default
    _DFT_Reference_R( arrOutputSignal, false );
}
template <typename Real>
Void TSignalTransform<Real>::InvDFT( Real * arrOutputSignal )
{
    // Lacking better implementation for now, fallback to default
    _DFT_Reference_R( arrOutputSignal, true );
}

/////////////////////////////////////////////////////////////////////////////////

template <typename Real>
Void TSignalTransform<Real>::_DFT_Reference( TComplex<Real> * arrOutput, Bool bInvert )
{
    // Reference implementation
    UInt iN = m_iSampleCount;
    Real fInvN = MathRealFn->Invert( (Real)iN );
    Real f2PI_N = fInvN * (Real)SCALAR_2PI;
    TComplex<Real> cTwiddleFactor;

    for( UInt k = 0; k < iN; ++k ) {
        arrOutput[k] = TComplex<Real>::Null;
        for( UInt p = 0; p < iN; ++p ) {
            cTwiddleFactor.SetPolar( MathFunction<Real>::One, -f2PI_N * (Real)(k*p) );
            if ( bInvert )
                arrOutput[k] += ( m_arrBaseSignal[p].Conjugate() * cTwiddleFactor );
            else
                arrOutput[k] += ( m_arrBaseSignal[p] * cTwiddleFactor );
        }
        if ( bInvert )
            arrOutput[k] = arrOutput[k].Conjugate() * fInvN;
    }
}
template <typename Real>
Void TSignalTransform<Real>::_DFT_Reference_R( Real * arrOutput, Bool bInvert )
{
    // Reference implementation
    UInt iN = m_iSampleCount;
    Real fInvN = MathRealFn->Invert( (Real)iN );
    Real f2PI_N = fInvN * (Real)SCALAR_2PI;
    Real fTwiddleFactor;

    for( UInt k = 0; k < iN; ++k ) {
        arrOutput[k] = MathFunction<Real>::Zero;
        for( UInt p = 0; p < iN; ++p ) {
            fTwiddleFactor = MathRealFn->Cos( -f2PI_N * (Real)(k*p) );
            arrOutput[k] += ( m_arrBaseSignal_R[p] * fTwiddleFactor );
        }
        if ( bInvert )
            arrOutput[k] *= fInvN;
    }
}

template <typename Real>
inline Void TSignalTransform<Real>::_FFT_BaseCase_1_R( Real * arrOutput, UInt iStride ) {
    // X0 = x0
    arrOutput[0] = m_arrBaseSignal[0];
}
template <typename Real>
inline Void TSignalTransform<Real>::_FFT_BaseCase_2_R( Real * arrOutput, UInt iStride ) {
    UInt i1 = iStride;

    // X0 = x0 + x1
    // X1 = x0 - x1
    arrOutput[0] = m_arrBaseSignal[0] + m_arrBaseSignal[i1];
    arrOutput[1] = m_arrBaseSignal[0] - m_arrBaseSignal[i1];
}
template <typename Real>
inline Void TSignalTransform<Real>::_FFT_BaseCase_3_R( Real * arrOutput, UInt iStride ) {
    UInt i1 = iStride;
    UInt i2 = ( iStride << 1 );

    // X0 = x0 + x1 + x2
    // X1 = x0 - 2*x1 - 2*x2
    // X2 = x0 - 2*x1 - 2*x2
    Real fTmp = ( m_arrBaseSignal[i1] + m_arrBaseSignal[i2] );
    arrOutput[0] = m_arrBaseSignal[0] + fTmp;
    arrOutput[1] = m_arrBaseSignal[0] - (MathFunction<Real>::Two * fTmp);
    arrOutput[2] = arrOutput[1];
}
template <typename Real>
inline Void TSignalTransform<Real>::_FFT_BaseCase_4_R( Real * arrOutput, UInt iStride ) {
    UInt i1 = iStride;
    UInt i2 = ( iStride << 1 );
    UInt i3 = ( i2 + iStride );

    // X0 = x0 + x1 + x2 + x3
    // X1 = x0 - x2
    // X2 = x0 - x1 + x2 - x3
    // X3 = x0 - x2
    Real fTmp0 = ( m_arrBaseSignal[0] + m_arrBaseSignal[i2] );
    Real fTmp1 = ( m_arrBaseSignal[i1] + m_arrBaseSignal[i3] );
    arrOutput[0] = fTmp0 + fTmp1;
    arrOutput[1] = m_arrBaseSignal[0] - m_arrBaseSignal[i2];
    arrOutput[2] = fTmp0 - fTmp1;
    arrOutput[3] = arrOutput[1];
}

template <typename Real>
inline Void TSignalTransform<Real>::_FFT_BaseCase_N_R( Real * arrOutput, UInt iN, UInt iStride ) {
    Assert( iN <= 4 );
    return sm_arrBaseCases_R[iN-1]( arrOutput, iStride );
}

template <typename Real>
Void TSignalTransform<Real>::_FFT_GenerateTwiddleFactors_R( UInt iN )
{
    UInt iDepth = MathRealFn->Log2OfPower2( iN );
    m_arrTwiddleFactors_R = New Real*[iDepth];

    for( UInt i = 0; i < iDepth; ++i ) {
        UInt iFactorCount = ( iN >> 1 );
        m_arrTwiddleFactors_R[i] = New Real[iFactorCount];

        Real fTmp = -( ((Real)SCALAR_2PI) / (Real)iN );
        for( UInt k = 0; k < iFactorCount; ++k )
            m_arrTwiddleFactors_R[i][j] = MathRealFn->Cos( fTmp * (Real)k );

        iN >>= 1;
    }
}

template <typename Real>
Void TSignalTransform<Real>::_FFT_Radix2_R( Real * arrOutput, const Real * arrInput, UInt iN, UInt iStride )
{
    // Base cases
    if ( iN <= 4 ) {
        _FFT_BaseCase_R( arrOutput, arrInput, iN, iStride );
        return;
    }

    // Recurse
    UInt iHalfN = ( iN >> 1 );
    _FFT_Radix2_R( arrOutput, arrInput, iHalfN, (iStride << 1) );
    _FFT_Radix2_R( arrOutput + iHalfN, arrInput + iStride, iHalfN, (iStride << 1) );
    
    // Combine
    UInt iDepth = MathRealFn->Log2OfPower2( iN );
    Real * arrTwiddleFactors = m_arrTwiddleFactors[iDepth];

    Real fTmp;
    for( UInt k = 0; k < iHalfN; ++k ) {
        fTmp = arrOutput[k];
        arrOutput[k]          = fTmp + ( arrOutput[iHalfN + k] * arrTwiddleFactors[k] );
        arrOutput[iHalfN + k] = fTmp - ( arrOutput[iHalfN + k] * arrTwiddleFactors[k] );
    }
}

