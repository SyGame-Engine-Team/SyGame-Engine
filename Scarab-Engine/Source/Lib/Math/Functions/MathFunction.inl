/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Functions/MathFunction.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Basic functions
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// MathFunction implementation
template<typename Real>
inline MathFunction<Real> * MathFunction<Real>::GetInstancePtr() {
    static MathFunction<Real> s_Instance;
    return &s_Instance;
}

template<typename Real>
MathFunction<Real>::MathFunction()
{
    // nothing to do
}
template<typename Real>
MathFunction<Real>::~MathFunction()
{
    // nothing to do
}

template<typename Real>
const Real MathFunction<Real>::Zero = (Real)0;
template<typename Real>
const Real MathFunction<Real>::Half = (Real)0.5f;
template<typename Real>
const Real MathFunction<Real>::One = (Real)1;
template<typename Real>
const Real MathFunction<Real>::Two = (Real)2;
template<typename Real>
const Real MathFunction<Real>::Infinity = (Real)FLOAT_INFINITE;

template<typename Real>
inline Bool MathFunction<Real>::EqualsZero( Real f, Real fThreshold ) const {
    return ( Abs(f) <= fThreshold );
}
template<typename Real>
inline Bool MathFunction<Real>::Equals( Real f, Real g, Real fThreshold ) const {
    return ( Abs(f - g) <= fThreshold );
}

template<typename Real>
inline Bool MathFunction<Real>::IsNan( Real f ) const {
    return ( f != f );
}
template<>
inline Bool MathFunction<Float>::IsNan( Float f ) const {
    return ( FPUFn->Inspect(f) == FPU_CLASS_NAN );
}
template<>
inline Bool MathFunction<Double>::IsNan( Double f ) const {
    return ( FPUFn->Inspect(f) == FPU_CLASS_NAN );
}

template<typename Real>
inline Bool MathFunction<Real>::IsInfinite( Real f ) const {
    return ( f == Infinity || f == (-Infinity) );
}
template<>
inline Bool MathFunction<Float>::IsInfinite( Float f ) const {
    return ( FPUFn->Inspect(f) == FPU_CLASS_INFINITY );
}
template<>
inline Bool MathFunction<Double>::IsInfinite( Double f ) const {
    return ( FPUFn->Inspect(f) == FPU_CLASS_INFINITY );
}

template<typename Real>
inline Bool MathFunction<Real>::IsZero( Real f ) const {
    return ( f == Zero || f == -Zero );
}
template<>
inline Bool MathFunction<Float>::IsZero( Float f ) const {
    return ( FPUFn->Inspect(f) == FPU_CLASS_ZERO );
}
template<>
inline Bool MathFunction<Double>::IsZero( Double f ) const {
    return ( FPUFn->Inspect(f) == FPU_CLASS_ZERO );
}

template<typename Real>
inline Int MathFunction<Real>::Floor( Real f ) const {
    return (Int)f;
}
template<>
inline Int MathFunction<Float>::Floor( Float f ) const {
    return FPUFn->Floor(f);
}
template<>
inline Int MathFunction<Double>::Floor( Double f ) const {
    return FPUFn->Floor(f);
}

template<typename Real>
inline Int MathFunction<Real>::Ceil( Real f ) const {
    Int iFloor = Floor(f);
    if ( (f - (Real)iFloor) > Zero )
        return iFloor + 1;
    return iFloor;
}
template<>
inline Int MathFunction<Float>::Ceil( Float f ) const {
    return FPUFn->Ceil(f);
}
template<>
inline Int MathFunction<Double>::Ceil( Double f ) const {
    return FPUFn->Ceil(f);
}

template<typename Real>
inline Int MathFunction<Real>::Round( Real f ) const {
    return Floor( f + Half );
}
template<>
inline Int MathFunction<Float>::Round( Float f ) const {
    return FPUFn->Round(f);
}
template<>
inline Int MathFunction<Double>::Round( Double f ) const {
    return FPUFn->Round(f);
}

template<typename Real>
inline Real MathFunction<Real>::Floorf( Real f ) const {
    return (Real)Floor(f);
}
template<typename Real>
inline Real MathFunction<Real>::Ceilf( Real f ) const {
    return (Real)Ceil(f);
}
template<typename Real>
inline Real MathFunction<Real>::Roundf( Real f ) const {
    return (Real)Round(f);
}

template<typename Real>
inline Real MathFunction<Real>::FractPart( Real f ) const {
    return ( f - Floorf(f) );
}
template<typename Real>
inline Real MathFunction<Real>::TruncateDecimal( Real f, UInt iLastDecimal ) const {
    Real fOrder = Power10( (Int)iLastDecimal );
    Real fScaled = (f * fOrder);
    return ( Floorf(fScaled) / fOrder );
}
template<typename Real>
inline Real MathFunction<Real>::RoundDecimal( Real f, UInt iLastDecimal ) const {
    Real fOrder = Power10( (Int)iLastDecimal );
    Real fScaled = (f * fOrder);
    return ( Roundf(fScaled) / fOrder );
}

template<typename Real>
inline Real MathFunction<Real>::Sign( Real f ) const {
    return ( f < Zero ) ? -One : One;
}
template<typename Real>
inline Int MathFunction<Real>::SignI( Real f, Real fThreshold ) const {
    if ( f < -fThreshold )
        return -1;
    if ( f > +fThreshold )
        return +1;
    return 0;
}

template<typename Real>
inline Real MathFunction<Real>::Abs( Real f ) const {
    return ( f < Zero ) ? -f : f;
}
template<>
inline Float MathFunction<Float>::Abs( Float f ) const {
    return FPUFn->Abs(f);
}
template<>
inline Double MathFunction<Double>::Abs( Double f ) const {
    return FPUFn->Abs(f);
}
template<>
inline Fixed MathFunction<Fixed>::Abs( Fixed f ) const {
    return Fixed::Abs(f);
}

template<typename Real>
inline Real MathFunction<Real>::Mod( Real f, Real g ) const {
    return (Real)( MathFn->Mod( (Scalar)f, (Scalar)g ) );
}
template<>
inline Float MathFunction<Float>::Mod( Float f, Float g ) const {
    return FPUFn->Mod( f, g );
}
template<>
inline Double MathFunction<Double>::Mod( Double f, Double g ) const {
    return FPUFn->Mod( f, g );
}

template<typename Real>
inline Real MathFunction<Real>::Sqr( Real f ) const {
    return ( f * f );
}
template<typename Real>
inline Real MathFunction<Real>::Cube( Real f ) const {
    return ( f * f * f );
}

template<typename Real>
inline Real MathFunction<Real>::Invert( Real f ) const {
    return ( One / f );
}
template<>
inline Float MathFunction<Float>::Invert( Float f ) const {
#if defined(MATHFUNCTION_USE_TRICKS)
    return _Tricked_Invert(f);
#else
    return ( 1.0f / f );
#endif
}
template<>
inline Double MathFunction<Double>::Invert( Double f ) const {
#if defined(MATHFUNCTION_USE_TRICKS)
    return _Tricked_Invert(f);
#else
    return ( 1.0 / f );
#endif
}

template<typename Real>
inline Real MathFunction<Real>::Sqrt( Real f ) const {
#if defined(MATHFUNCTION_MISSING_TAYLOR_EXPR)
    return SqrtTaylor(f);
#elif defined(MATHFUNCTION_MISSING_SCALAR_CAST)
    return (Real)( MathFn->Sqrt((Scalar)f) );
#endif
}
template<>
inline Float MathFunction<Float>::Sqrt( Float f ) const {
    return FPUFn->Sqrt(f);
}
template<>
inline Double MathFunction<Double>::Sqrt( Double f ) const {
    return FPUFn->Sqrt(f);
}
template<>
inline Fixed MathFunction<Fixed>::Sqrt( Fixed f ) const {
    return Fixed::Sqrt(f);
}

template<typename Real>
inline Real MathFunction<Real>::InvSqrt( Real f ) const {
#if defined(MATHFUNCTION_MISSING_TAYLOR_EXPR)
    return InvSqrtTaylor(f);
#elif defined(MATHFUNCTION_MISSING_SCALAR_CAST)
    return (Real)( MathFn->InvSqrt((Scalar)f) );
#endif
}
template<>
inline Float MathFunction<Float>::InvSqrt( Float f ) const {
#if defined(MATHFUNCTION_USE_TRICKS)
    return _Tricked_InvSqrt(f);
#else
    return ( 1.0f / FPUFn->Sqrt(f) );
#endif
}
template<>
inline Double MathFunction<Double>::InvSqrt( Double f ) const {
#if defined(MATHFUNCTION_USE_TRICKS)
    return _Tricked_InvSqrt(f);
#else
    return ( 1.0 / FPUFn->Sqrt(f) );
#endif
}
template<>
inline Fixed MathFunction<Fixed>::InvSqrt( Fixed f ) const {
    return ( Fixed::One / Fixed::Sqrt(f) );
}

template<typename Real>
inline Real MathFunction<Real>::RootN( Int n, Real f ) const {
    return ExpN( f, One / (Real)n );
}

template<typename Real>
inline Real MathFunction<Real>::Ln( Real f ) const {
#if defined(MATHFUNCTION_MISSING_TAYLOR_EXPR)
    return LnTaylor(f);
#elif defined(MATHFUNCTION_MISSING_SCALAR_CAST)
    return (Real)( MathFn->Ln((Scalar)f) );
#endif
}
template<>
inline Float MathFunction<Float>::Ln( Float f ) const {
    return FPUFn->Ln(f);
}
template<>
inline Double MathFunction<Double>::Ln( Double f ) const {
    return FPUFn->Ln(f);
}

template<typename Real>
inline Real MathFunction<Real>::Log2( Real f ) const {
#if defined(MATHFUNCTION_MISSING_TAYLOR_EXPR)
    return Log2Taylor(f);
#elif defined(MATHFUNCTION_MISSING_SCALAR_CAST)
    return (Real)( MathFn->Log2((Scalar)f) );
#endif
}
template<>
inline Float MathFunction<Float>::Log2( Float f ) const {
    return FPUFn->Log2(f);
}
template<>
inline Double MathFunction<Double>::Log2( Double f ) const {
    return FPUFn->Log2(f);
}

template<typename Real>
inline Real MathFunction<Real>::Log10( Real f ) const {
#if defined(MATHFUNCTION_MISSING_TAYLOR_EXPR)
    return Log10Taylor(f);
#elif defined(MATHFUNCTION_MISSING_SCALAR_CAST)
    return (Real)( MathFn->Log10((Scalar)f) );
#endif
}
template<>
inline Float MathFunction<Float>::Log10( Float f ) const {
    return FPUFn->Log10(f);
}
template<>
inline Double MathFunction<Double>::Log10( Double f ) const {
    return FPUFn->Log10(f);
}

template<typename Real>
inline Real MathFunction<Real>::LogN( Real n, Real f ) const {
#if defined(MATHFUNCTION_MISSING_TAYLOR_EXPR)
    return LogNTaylor( n, f );
#elif defined(MATHFUNCTION_MISSING_SCALAR_CAST)
    return (Real)( MathFn->LogN((Scalar)n, (Scalar)f) );
#endif
}
template<>
inline Float MathFunction<Float>::LogN( Float n, Float f ) const {
    return FPUFn->LogN( n, f );
}
template<>
inline Double MathFunction<Double>::LogN( Double n, Double f ) const {
    return FPUFn->LogN( n, f );
}

template<typename Real>
inline Real MathFunction<Real>::Exp( Real f ) const {
#if defined(MATHFUNCTION_MISSING_TAYLOR_EXPR)
    return ExpTaylor(f);
#elif defined(MATHFUNCTION_MISSING_SCALAR_CAST)
    return (Real)( MathFn->Exp((Scalar)f) );
#endif
}
template<>
inline Float MathFunction<Float>::Exp( Float f ) const {
    return FPUFn->Exp(f);
}
template<>
inline Double MathFunction<Double>::Exp( Double f ) const {
    return FPUFn->Exp(f);
}

template<typename Real>
inline Real MathFunction<Real>::Exp2( Real f ) const {
#if defined(MATHFUNCTION_MISSING_TAYLOR_EXPR)
    return Exp2Taylor(f);
#elif defined(MATHFUNCTION_MISSING_SCALAR_CAST)
    return (Real)( MathFn->Exp2((Scalar)f) );
#endif
}
template<>
inline Float MathFunction<Float>::Exp2( Float f ) const {
    return FPUFn->Exp2(f);
}
template<>
inline Double MathFunction<Double>::Exp2( Double f ) const {
    return FPUFn->Exp2(f);
}

template<typename Real>
inline Real MathFunction<Real>::Exp10( Real f ) const {
#if defined(MATHFUNCTION_MISSING_TAYLOR_EXPR)
    return Exp10Taylor(f);
#elif defined(MATHFUNCTION_MISSING_SCALAR_CAST)
    return (Real)( MathFn->Exp10((Scalar)f) );
#endif
}
template<>
inline Float MathFunction<Float>::Exp10( Float f ) const {
    return FPUFn->Exp10(f);
}
template<>
inline Double MathFunction<Double>::Exp10( Double f ) const {
    return FPUFn->Exp10(f);
}

template<typename Real>
inline Real MathFunction<Real>::ExpN( Real n, Real f ) const {
#if defined(MATHFUNCTION_MISSING_TAYLOR_EXPR)
    return ExpNTaylor( n, f );
#elif defined(MATHFUNCTION_MISSING_SCALAR_CAST)
    return (Real)( MathFn->ExpN((Scalar)n, (Scalar)f) );
#endif
}
template<>
inline Float MathFunction<Float>::ExpN( Float n, Float f ) const {
    return FPUFn->ExpN( n, f );
}
template<>
inline Double MathFunction<Double>::ExpN( Double n, Double f ) const {
    return FPUFn->ExpN( n, f );
}

template<typename Real>
inline Real MathFunction<Real>::Power2( Int iExponent ) const {
#if defined(MATHFUNCTION_MISSING_TAYLOR_EXPR)
    return Power2Taylor(iExponent);
#elif defined(MATHFUNCTION_MISSING_SCALAR_CAST)
    return (Real)( MathFn->Power2(iExponent) );
#endif
}
template<>
inline Float MathFunction<Float>::Power2( Int iExponent ) const {
    return FPUFn->Power2f(iExponent);
}
template<>
inline Double MathFunction<Double>::Power2( Int iExponent ) const {
    return FPUFn->Power2d(iExponent);
}

template<typename Real>
inline Real MathFunction<Real>::Power10( Int iExponent ) const {
#if defined(MATHFUNCTION_MISSING_TAYLOR_EXPR)
    return Power10Taylor(iExponent);
#elif defined(MATHFUNCTION_MISSING_SCALAR_CAST)
    return (Real)( MathFn->Power10(iExponent) );
#endif
}
template<>
inline Float MathFunction<Float>::Power10( Int iExponent ) const {
    return FPUFn->Power10f(iExponent);
}
template<>
inline Double MathFunction<Double>::Power10( Int iExponent ) const {
    return FPUFn->Power10d(iExponent);
}

template<typename Real>
inline Real MathFunction<Real>::PowerN( Real n, Int iExponent ) const {
#if defined(MATHFUNCTION_MISSING_TAYLOR_EXPR)
    return PowerNTaylor( n, iExponent );
#elif defined(MATHFUNCTION_MISSING_SCALAR_CAST)
    return (Real)( MathFn->PowerN((Scalar)n, iExponent) );
#endif
}
template<>
inline Float MathFunction<Float>::PowerN( Float n, Int iExponent ) const {
    return FPUFn->PowerN( n, iExponent );
}
template<>
inline Double MathFunction<Double>::PowerN( Double n, Int iExponent ) const {
    return FPUFn->PowerN( n, iExponent );
}

template<typename Real>
inline Real MathFunction<Real>::NormalizeAngle( Real f ) const {
    f = Mod( f, SCALAR_2PI );
    if ( f < -SCALAR_PI )
        return f + SCALAR_2PI;
    if ( f > SCALAR_PI )
        return f - SCALAR_2PI;
    return f;
}

template<typename Real>
inline Real MathFunction<Real>::Sin( Real f ) const {
#if defined(MATHFUNCTION_MISSING_TAYLOR_EXPR)
    return SinTaylor(f);
#elif defined(MATHFUNCTION_MISSING_SCALAR_CAST)
    return (Real)( MathFn->Sin((Scalar)f) );
#endif
}
template<>
inline Float MathFunction<Float>::Sin( Float f ) const {
    return FPUFn->Sin(f);
}
template<>
inline Double MathFunction<Double>::Sin( Double f ) const {
    return FPUFn->Sin(f);
}

template<typename Real>
inline Real MathFunction<Real>::Cos( Real f ) const {
#if defined(MATHFUNCTION_MISSING_TAYLOR_EXPR)
    return CosTaylor(f);
#elif defined(MATHFUNCTION_MISSING_SCALAR_CAST)
    return (Real)( MathFn->Cos((Scalar)f) );
#endif
}
template<>
inline Float MathFunction<Float>::Cos( Float f ) const {
    return FPUFn->Cos(f);
}
template<>
inline Double MathFunction<Double>::Cos( Double f ) const {
    return FPUFn->Cos(f);
}

template<typename Real>
inline Void MathFunction<Real>::SinCos( Real * pOutSin, Real * pOutCos, Real f ) const {
#if defined(MATHFUNCTION_MISSING_TAYLOR_EXPR)
    SinCosTaylor( pOutSin, pOutCos, f );
#elif defined(MATHFUNCTION_MISSING_SCALAR_CAST)
    Scalar fSin, fCos;
    MathFn->SinCos( &fSin, &fCos, (Scalar)f );
    *pOutSin = (Real)fSin;
    *pOutCos = (Real)fCos;
#endif
}
template<>
inline Void MathFunction<Float>::SinCos( Float * pOutSin, Float * pOutCos, Float f ) const {
    FPUFn->SinCos( pOutSin, pOutCos, f );
}
template<>
inline Void MathFunction<Double>::SinCos( Double * pOutSin, Double * pOutCos, Double f ) const {
    FPUFn->SinCos( pOutSin, pOutCos, f );
}

template<typename Real>
inline Real MathFunction<Real>::Tan( Real f ) const {
#if defined(MATHFUNCTION_MISSING_TAYLOR_EXPR)
    return TanTaylor(f);
#elif defined(MATHFUNCTION_MISSING_SCALAR_CAST)
    return (Real)( MathFn->Tan((Scalar)f) );
#endif
}
template<>
inline Float MathFunction<Float>::Tan( Float f ) const {
    return FPUFn->Tan(f);
}
template<>
inline Double MathFunction<Double>::Tan( Double f ) const {
    return FPUFn->Tan(f);
}

template<typename Real>
inline Real MathFunction<Real>::ArcSin( Real f ) const {
#if defined(MATHFUNCTION_MISSING_TAYLOR_EXPR)
    return ArcSinTaylor(f);
#elif defined(MATHFUNCTION_MISSING_SCALAR_CAST)
    return (Real)( MathFn->ArcSin((Scalar)f) );
#endif
}
template<>
inline Float MathFunction<Float>::ArcSin( Float f ) const {
    return FPUFn->ArcSin(f);
}
template<>
inline Double MathFunction<Double>::ArcSin( Double f ) const {
    return FPUFn->ArcSin(f);
}

template<typename Real>
inline Real MathFunction<Real>::ArcCos( Real f ) const {
#if defined(MATHFUNCTION_MISSING_TAYLOR_EXPR)
    return ArcCosTaylor(f);
#elif defined(MATHFUNCTION_MISSING_SCALAR_CAST)
    return (Real)( MathFn->ArcCos((Scalar)f) );
#endif
}
template<>
inline Float MathFunction<Float>::ArcCos( Float f ) const {
    return FPUFn->ArcCos(f);
}
template<>
inline Double MathFunction<Double>::ArcCos( Double f ) const {
    return FPUFn->ArcCos(f);
}

template<typename Real>
inline Real MathFunction<Real>::ArcTan( Real f ) const {
#if defined(MATHFUNCTION_MISSING_TAYLOR_EXPR)
    return ArcTanTaylor(f);
#elif defined(MATHFUNCTION_MISSING_SCALAR_CAST)
    return (Real)( MathFn->ArcTan((Scalar)f) );
#endif
}
template<>
inline Float MathFunction<Float>::ArcTan( Float f ) const {
    return FPUFn->ArcTan(f);
}
template<>
inline Double MathFunction<Double>::ArcTan( Double f ) const {
    return FPUFn->ArcTan(f);
}

template<typename Real>
inline Real MathFunction<Real>::ArcTan2( Real f, Real g ) const {
#if defined(MATHFUNCTION_MISSING_TAYLOR_EXPR)
    return ArcTan2Taylor( f, g );
#elif defined(MATHFUNCTION_MISSING_SCALAR_CAST)
    return (Real)( MathFn->ArcTan2((Scalar)f, (Scalar)g) );
#endif
}
template<>
inline Float MathFunction<Float>::ArcTan2( Float f, Float g ) const {
    return FPUFn->ArcTan2( f, g );
}
template<>
inline Double MathFunction<Double>::ArcTan2( Double f, Double g ) const {
    return FPUFn->ArcTan2( f, g );
}

template<typename Real>
inline Real MathFunction<Real>::SinH( Real f ) const {
    Real ef = Exp(f);
    return Half * ( ef - Invert(ef) );
}
template<typename Real>
inline Real MathFunction<Real>::CosH( Real f ) const {
    Real ef = Exp(f);
    return Half * ( ef + Invert(ef) );
}
template<typename Real>
inline Real MathFunction<Real>::TanH( Real f) const {
    Real e2f = Exp(Two * f);
    return ( (e2f - One) / (e2f + One) );
}
template<typename Real>
inline Real MathFunction<Real>::ArgSinH( Real f ) const {
    return Ln( f + Sqrt( (f*f) + One ) );
}
template<typename Real>
inline Real MathFunction<Real>::ArgCosH( Real f ) const {
    return Ln( f + Sqrt( (f*f) - One ) );
}
template<typename Real>
inline Real MathFunction<Real>::ArgTanH( Real f ) const {
    return Half * Ln( (One + f) / (One - f) );
}

template<typename Real>
inline Bool MathFunction<Real>::IsPower2( UInt uiValue ) const {
    return ( (uiValue > 0) && ((uiValue & (uiValue-1)) == 0) );
}
template<typename Real>
inline UInt MathFunction<Real>::Log2OfPower2( UInt uiPower2Value ) const {
    UInt uiLog2 = ( uiPower2Value & 0xAAAAAAAA ) != 0;
    uiLog2 |= ( (uiPower2Value & 0xCCCCCCCC) != 0 ) << 1;
    uiLog2 |= ( (uiPower2Value & 0xF0F0F0F0) != 0 ) << 2;
    uiLog2 |= ( (uiPower2Value & 0xFF00FF00) != 0 ) << 3;
    uiLog2 |= ( (uiPower2Value & 0xFFFF0000) != 0 ) << 4;
    return uiLog2;
}

template<typename Real>
inline UInt MathFunction<Real>::ScaleUnit( Real f, UInt iBits ) const {
    UInt iMask = ( (1ul << iBits) - 1ul );
    return ( ((UInt)(f * (Real)iMask)) & iMask );
}
template<>
inline UInt MathFunction<Float>::ScaleUnit( Float f, UInt iBits ) const {
#if defined(MATHFUNCTION_USE_TRICKS)
    return _Tricked_ScaleUnit( f, iBits );
#else
    UInt iMask = ( (1ul << iBits) - 1ul );
    return ( ( (UInt)(f * (Float)iMask) ) & iMask );
#endif
}
template<>
inline UInt MathFunction<Double>::ScaleUnit( Double f, UInt iBits ) const {
#if defined(MATHFUNCTION_USE_TRICKS)
    return _Tricked_ScaleUnit( f, iBits );
#else
    UInt iMask = ( (1ul << iBits) - 1ul );
    return ( ( (UInt)(f * (Double)iMask) ) & iMask );
#endif
}

template<typename Real>
inline UInt64 MathFunction<Real>::ScaleUnit64( Real f, UInt iBits ) const {
    UInt64 iMask = ( (1ui64 << iBits) - 1ui64 );
    return ( ( (UInt64)(f * (Real)iMask) ) & iMask );
}
template<>
inline UInt64 MathFunction<Float>::ScaleUnit64( Float f, UInt iBits ) const {
#if defined(MATHFUNCTION_USE_TRICKS)
    return _Tricked_ScaleUnit64( f, iBits );
#else
    UInt64 iMask = ( (1ui64 << iBits) - 1ui64 );
    return ( ( (UInt64)(f * (Float)iMask) ) & iMask );
#endif
}
template<>
inline UInt64 MathFunction<Double>::ScaleUnit64( Double f, UInt iBits ) const {
#if defined(MATHFUNCTION_USE_TRICKS)
    return _Tricked_ScaleUnit64( f, iBits );
#else
    UInt64 iMask = ( (1ui64 << iBits) - 1ui64 );
    return ( ( (UInt64)(f * (Double)iMask) ) & iMask );
#endif
}

/////////////////////////////////////////////////////////////////////////////////

template<typename Real>
Real MathFunction<Real>::_Tricked_Invert( Real f ) const
{
    // Dummy stub, never called
    return Infinity;
}
template<typename Real>
Real MathFunction<Real>::_Tricked_InvSqrt( Real f ) const
{
    // Dummy stub, never called
    return Infinity;
}

template<typename Real>
UInt MathFunction<Real>::_Tricked_ScaleUnit( Float f, UInt iBits ) const
{
    // Untested //
    m_FloatConverter.f = f;
    DWord dwRepr = m_FloatConverter.i;
    Int iShift = ( FLOAT_EXP_BIAS + FLOAT_MANT_BITS - iBits - ((dwRepr >> FLOAT_MANT_BITS) & 0xff) );
    if ( iShift < FLOAT_MANT_BITS + 1 ) {
        DWord dwMask = ( 0x00000001ul << FLOAT_MANT_BITS );
	    Int iRes = ( (dwRepr & (dwMask-1)) | dwMask ) >> iShift;
	    if ( iRes == (1 << iBits) )
		    --iRes;
	    return (unsigned)iRes;
    } else
	    return 0;
}
template<typename Real>
UInt64 MathFunction<Real>::_Tricked_ScaleUnit64( Double f, UInt iBits ) const
{
    // Untested //
    m_DoubleConverter.f = f;
    QWord qwRepr = m_DoubleConverter.i;
    Int iShift = ( DOUBLE_EXP_BIAS + DOUBLE_MANT_BITS - iBits - ((qwRepr >> DOUBLE_MANT_BITS) & 0x7ff) );
    if ( iShift < DOUBLE_MANT_BITS + 1 ) {
        QWord qwMask = ( 0x0000000000000001ui64 << DOUBLE_MANT_BITS );
	    Int64 iRes = ( (qwRepr & (qwMask-1)) | qwMask ) >> iShift;
	    if ( iRes == (1i64 << iBits) )
		    --iRes;
	    return (unsigned)iRes;
    } else
	    return 0ui64;
}

