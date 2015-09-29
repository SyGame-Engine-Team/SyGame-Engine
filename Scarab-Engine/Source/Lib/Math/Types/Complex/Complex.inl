/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Complex/Complex.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Complex numbers
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TComplex implementation
template<typename Real>
const TComplex<Real> TComplex<Real>::Null = TComplex<Real>( (Real)0, (Real)0 );

template<typename Real>
TComplex<Real>::TComplex()
{
    // nothing to do
}
template<typename Real>
TComplex<Real>::TComplex( Real fR, Real fI )
{
    R = fR;
    I = fI;
}
template<typename Real>
TComplex<Real>::TComplex( const Real vArr[2] )
{
    R = vArr[0];
    I = vArr[1];
}
template<typename Real>
TComplex<Real>::~TComplex()
{
    // nothing to do
}

template<typename Real>
inline TComplex<Real> & TComplex<Real>::operator=( const TComplex<Real> & rhs ) {
    R = rhs.R;
    I = rhs.I;
    return (*this);
}

template<typename Real>
inline TComplex<Real>::operator Real*() const {
    return (Real*)this;
}
template<typename Real>
inline TComplex<Real>::operator const Real*() const {
    return (const Real*)this;
}

template<typename Real>
inline TComplex<Real> TComplex<Real>::operator+() const {
    return TComplex<Real>( R, I );
}
template<typename Real>
inline TComplex<Real> TComplex<Real>::operator-() const {
    return TComplex<Real>( -R, -I );
}

template<typename Real>
inline Bool TComplex<Real>::operator==( const TComplex<Real> & rhs ) const {
    return ( MathRealFn->Equals(R,rhs.R) && MathRealFn->Equals(I,rhs.I) );
}
template<typename Real>
inline Bool TComplex<Real>::operator!=( const TComplex<Real> & rhs ) const {
    return ( !(MathRealFn->Equals(R,rhs.R)) || !(MathRealFn->Equals(I,rhs.I)) );
}

template<typename Real>
inline TComplex<Real> TComplex<Real>::operator+( Real rhs ) const {
    return TComplex<Real>( R + rhs, I );
}
template<typename Real>
inline TComplex<Real> TComplex<Real>::operator-( Real rhs ) const {
    return TComplex<Real>( R - rhs, I );
}
template<typename Real>
inline TComplex<Real> TComplex<Real>::operator*( Real rhs ) const {
    return TComplex<Real>( R * rhs, I * rhs );
}
template<>
inline TComplex<Float> TComplex<Float>::operator*( Float rhs ) const {
    #ifdef MATH_USE_SSE
        TComplex<Float> vRes;
        MathSSEFn->Push( R, I );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->MulPF();
        MathSSEFn->Pop( vRes.R, vRes.I );
        return vRes;
    #else
        return TComplex<Float>( R * rhs, I * rhs );
    #endif
}
template<>
inline TComplex<Double> TComplex<Double>::operator*( Double rhs ) const {
    #ifdef MATH_USE_SSE
        TComplex<Double> vRes;
        MathSSEFn->Push( R, I );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( vRes.R, vRes.I );
        return vRes;
    #else
        return TComplex<Double>( R * rhs, I * rhs );
    #endif
}
template<typename Real>
inline TComplex<Real> TComplex<Real>::operator/( Real rhs ) const {
    return TComplex<Real>( R / rhs, I / rhs );
}
template<>
inline TComplex<Float> TComplex<Float>::operator/(Float rhs) const {
    #ifdef MATH_USE_SSE
        TComplex<Float> vRes;
        MathSSEFn->Push( R, I );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->DivPF();
        MathSSEFn->Pop( vRes.R, vRes.I );
        return vRes;
    #else
        return TComplex<Float>( R / rhs, I / rhs );
    #endif
}
template<>
inline TComplex<Double> TComplex<Double>::operator/(Double rhs) const {
    #ifdef MATH_USE_SSE
        TComplex<Double> vRes;
        MathSSEFn->Push( R, I );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( vRes.R, vRes.I );
        return vRes;
    #else
        return TComplex<Double>( R / rhs, I / rhs );
    #endif
}

template<typename Real>
inline TComplex<Real> & TComplex<Real>::operator+=( Real rhs ) {
    R += rhs;
    return (*this);
}
template<typename Real>
inline TComplex<Real> & TComplex<Real>::operator-=( Real rhs ) {
    R -= rhs;
    return (*this);
}
template<typename Real>
inline TComplex<Real> & TComplex<Real>::operator*=( Real rhs ) {
    R *= rhs;
    I *= rhs;
    return (*this);
}
template<>
inline TComplex<Float> & TComplex<Float>::operator*=( Float rhs ) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( R, I );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->MulPF();
        MathSSEFn->Pop( R, I );
    #else
        R *= rhs;
        I *= rhs;
    #endif
    return (*this);
}
template<>
inline TComplex<Double> & TComplex<Double>::operator*=( Double rhs ) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( R, I );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( R, I );
    #else
        R *= rhs;
        I *= rhs;
    #endif
    return (*this);
}
template<typename Real>
inline TComplex<Real> & TComplex<Real>::operator/=( Real rhs ) {
    R /= rhs;
    I /= rhs;
    return (*this);
}
template<>
inline TComplex<Float> & TComplex<Float>::operator/=( Float rhs ) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( R, I );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->DivPF();
        MathSSEFn->Pop( R, I );
    #else
        R /= rhs;
        I /= rhs;
    #endif
    return (*this);
}
template<>
inline TComplex<Double> & TComplex<Double>::operator/=( Double rhs ) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( R, I );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( R, I );
    #else
        R /= rhs;
        I /= rhs;
    #endif
    return (*this);
}

template<typename Real>
inline TComplex<Real> TComplex<Real>::operator+( const TComplex<Real> & rhs ) const {
    return TComplex<Real>( R + rhs.R, I + rhs.I );
}
template<>
inline TComplex<Float> TComplex<Float>::operator+( const TComplex<Float> & rhs ) const {
    #ifdef MATH_USE_SSE
        TComplex<Float> vRes;
        MathSSEFn->Push( R, I );
        MathSSEFn->Push( rhs.R, rhs.I );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( vRes.R, vRes.I );
        return vRes;
    #else
        return TComplex<Float>( R + rhs.R, I + rhs.I );
    #endif
}
template<>
inline TComplex<Double> TComplex<Double>::operator+( const TComplex<Double> & rhs ) const {
    #ifdef MATH_USE_SSE
        TComplex<Double> vRes;
        MathSSEFn->Push( R, I );
        MathSSEFn->Push( rhs.R, rhs.I );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( vRes.R, vRes.I );
        return vRes;
    #else
        return TComplex<Double>( R + rhs.R, I + rhs.I );
    #endif
}
template<typename Real>
inline TComplex<Real> TComplex<Real>::operator-( const TComplex<Real> & rhs ) const {
    return TComplex<Real>( R - rhs.R, I - rhs.I );
}
template<>
inline TComplex<Float> TComplex<Float>::operator-( const TComplex<Float> & rhs ) const {
    #ifdef MATH_USE_SSE
        TComplex<Float> vRes;
        MathSSEFn->Push( R, I );
        MathSSEFn->Push( rhs.R, rhs.I );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( vRes.R, vRes.I );
        return vRes;
    #else
        return TComplex<Float>( R - rhs.R, I - rhs.I );
    #endif
}
template<>
inline TComplex<Double> TComplex<Double>::operator-( const TComplex<Double> & rhs ) const {
    #ifdef MATH_USE_SSE
        TComplex<Double> vRes;
        MathSSEFn->Push( R, I );
        MathSSEFn->Push( rhs.R, rhs.I );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( vRes.R, vRes.I );
        return vRes;
    #else
        return TComplex<Double>( R - rhs.R, I - rhs.I );
    #endif
}
template<typename Real>
inline TComplex<Real> TComplex<Real>::operator*( const TComplex<Real> & rhs ) const {
    return TComplex<Real>( (R * rhs.R) - (I * rhs.I),
                           (I * rhs.R) + (R * rhs.I) );
}
template<typename Real>
inline TComplex<Real> TComplex<Real>::operator/( const TComplex<Real> & rhs ) const {
    Real fInvDenom = MathRealFn->Invert( (rhs.R * rhs.R) + (rhs.I * rhs.I) );
    return TComplex<Real>( ( (R * rhs.R) + (I * rhs.I) ) * fInvDenom,
                           ( (I * rhs.R) - (R * rhs.I) ) * fInvDenom );
}

template<typename Real>
inline TComplex<Real> & TComplex<Real>::operator+=( const TComplex<Real> & rhs ) {
    R += rhs.R;
    I += rhs.I;
    return (*this);
}
template<>
inline TComplex<Float> & TComplex<Float>::operator+=( const TComplex<Float> & rhs ) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( R, I );
        MathSSEFn->Push( rhs.R, rhs.I );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( R, I );
    #else
        R += rhs.R;
        I += rhs.I;
    #endif
    return (*this);
}
template<>
inline TComplex<Double> & TComplex<Double>::operator+=( const TComplex<Double> & rhs ) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( R, I );
        MathSSEFn->Push( rhs.R, rhs.I );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( R, I );
    #else
        R += rhs.R;
        I += rhs.I;
    #endif
    return (*this);
}
template<typename Real>
inline TComplex<Real> & TComplex<Real>::operator-=( const TComplex<Real> & rhs ) {
    R -= rhs.R;
    I -= rhs.I;
    return (*this);
}
template<>
inline TComplex<Float> & TComplex<Float>::operator-=( const TComplex<Float> & rhs ) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( R, I );
        MathSSEFn->Push( rhs.R, rhs.I );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( R, I );
    #else
        R -= rhs.R;
        I -= rhs.I;
    #endif
    return (*this);
}
template<>
inline TComplex<Double> & TComplex<Double>::operator-=( const TComplex<Double> & rhs ) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( R, I );
        MathSSEFn->Push( rhs.R, rhs.I );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( R, I );
    #else
        R -= rhs.R;
        I -= rhs.I;
    #endif
    return (*this);
}
template<typename Real>
inline TComplex<Real> & TComplex<Real>::operator*=( const TComplex<Real> & rhs ) {
    Real fOldR = R;
    R = ( (R * rhs.R) - (I * rhs.I) );
    I = ( (I * rhs.R) + (fOldR * rhs.I) );
    return (*this);
}
template<typename Real>
inline TComplex<Real> & TComplex<Real>::operator/=( const TComplex<Real> & rhs ) {
    Real fInvDenom = MathRealFn->Invert( (rhs.R * rhs.R) + (rhs.I * rhs.I) );
    Real fOldR = R;
    R = ( (R * rhs.R) + (I * rhs.I) ) * fInvDenom;
    I = ( (I * rhs.R) - (fOldR * rhs.I) ) * fInvDenom;
    return (*this);
}

template<typename Real>
inline Void TComplex<Real>::GetPolar( Real * outRadius, Real * outAngle ) const {
    *outRadius = MathRealFn->Sqrt( R*R + I*I );
    *outAngle = MathRealFn->ArcTan2( I, R );
}
template<typename Real>
inline Void TComplex<Real>::SetPolar( Real fRadius, Real fAngle ) {
    Real fSin, fCos;
    MathRealFn->SinCos( &fSin, &fCos, fAngle );
    R = ( fRadius * fCos );
    I = ( fRadius * fSin );
}

template<typename Real>
inline TMatrix2<Real> TComplex<Real>::GetMatrix() const {
    return TMatrix2<Real>( R, -I, I, R );
}

template<typename Real>
inline TComplex<Real> TComplex<Real>::Conjugate() const {
    return TComplex<Real>( R, -I );
}
template<typename Real>
inline TComplex<Real> TComplex<Real>::Sqrt() const {
    Real fModulus = MathRealFn->Sqrt( R*R + I*I );
    Real fLambda = MathRealFn->Sqrt( (fModulus + R) * MathFunction::Half );
    Real fDelta = MathRealFn->Sqrt( (fModulus - R) * MathFunction::Half );
    if ( I < MathFunction::Zero )
        fDelta = -fDelta;
    return TComplex<Real>( fLambda, fDelta );
}
template<typename Real>
inline TComplex<Real> TComplex<Real>::Ln() const {
    Real fRadius = MathRealFn->Sqrt( R*R + I*I );
    Real fAngle = MathRealFn->ArcTan2( I, R );
    return TComplex<Real>( MathRealFn->Ln(fRadius), fAngle );
}
template<typename Real>
inline TComplex<Real> TComplex<Real>::Exp() const {
    Real fRadius = MathRealFn->Exp( R );
    Real fSin, fCos;
    MathRealFn->SinCos( &fSin, &fCos, I );
    return TComplex<Real>( fRadius * fCos, fRadius * fSin );
}

