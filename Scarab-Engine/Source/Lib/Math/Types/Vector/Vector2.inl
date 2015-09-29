/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Vector/Vector2.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : 2D vector
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TVector2 implementation
template<typename Real>
const TVector2<Real> TVector2<Real>::Null = TVector2<Real>( (Real)0, (Real)0 );
template<typename Real>
const TVector2<Real> TVector2<Real>::eI = TVector2<Real>( (Real)1, (Real)0 );
template<typename Real>
const TVector2<Real> TVector2<Real>::eJ = TVector2<Real>( (Real)0, (Real)1 );

template<typename Real>
TVector2<Real>::TVector2()
{
    // nothing to do
}
template<typename Real>
TVector2<Real>::TVector2(Real x, Real y)
{
    X = x;
    Y = y;
}
template<typename Real>
TVector2<Real>::TVector2(const Real vArr[2])
{
    X = vArr[0];
    Y = vArr[1];
}
template<typename Real>
TVector2<Real>::TVector2(const TVector2<Real> & rhs)
{
    X = rhs.X;
    Y = rhs.Y;
}
template<typename Real>
TVector2<Real>::TVector2(const TVector3<Real> & rhs)
{
    X = rhs.X;
    Y = rhs.Y;
}
template<typename Real>
TVector2<Real>::TVector2(const TVector4<Real> & rhs)
{
    X = rhs.X;
    Y = rhs.Y;
}
template<typename Real>
TVector2<Real>::~TVector2()
{
    // nothing to do
}

template<typename Real>
inline TVector2<Real> & TVector2<Real>::operator=(const TVector2<Real> & rhs) {
    X = rhs.X;
    Y = rhs.Y;
    return (*this);
}

template<typename Real>
inline TVector2<Real>::operator Real*() const {
    return (Real*)this;
}
template<typename Real>
inline TVector2<Real>::operator const Real*() const {
    return (const Real*)this;
}

template<typename Real>
inline Real & TVector2<Real>::operator[](Int i) {
    return *( ((Real*)this) + i );
}
template<typename Real>
inline Real TVector2<Real>::operator[](Int i) const {
    return *( ((const Real*)this) + i );
}
template<typename Real>
inline Real & TVector2<Real>::operator[](UInt i) {
    return *( ((Real*)this) + i );
}
template<typename Real>
inline Real TVector2<Real>::operator[](UInt i) const {
    return *( ((const Real*)this) + i );
}

template<typename Real>
inline TVector2<Real> TVector2<Real>::operator+() const {
    return TVector2<Real>(X, Y);
}
template<typename Real>
inline TVector2<Real> TVector2<Real>::operator-() const {
    return TVector2<Real>(-X, -Y);
}

template<typename Real>
inline Bool TVector2<Real>::operator==(const TVector2<Real> & rhs) const {
    return ( MathRealFn->Equals(X,rhs.X) && MathRealFn->Equals(Y,rhs.Y) );
}
template<typename Real>
inline Bool TVector2<Real>::operator!=(const TVector2<Real> & rhs) const {
    return ( !(MathRealFn->Equals(X,rhs.X)) || !(MathRealFn->Equals(Y,rhs.Y)) );
}

template<typename Real>
inline TVector2<Real> TVector2<Real>::operator+(Real rhs) const {
    return TVector2<Real>( X + rhs, Y + rhs );
}
template<>
inline TVector2<Float> TVector2<Float>::operator+(Float rhs) const {
    #ifdef MATH_USE_SSE
        TVector2<Float> vRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( vRes.X, vRes.Y );
        return vRes;
    #else
        return TVector2<Float>( X + rhs, Y + rhs );
    #endif
}
template<>
inline TVector2<Double> TVector2<Double>::operator+(Double rhs) const {
    #ifdef MATH_USE_SSE
        TVector2<Double> vRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( vRes.X, vRes.Y );
        return vRes;
    #else
        return TVector2<Double>( X + rhs, Y + rhs );
    #endif
}
template<typename Real>
inline TVector2<Real> TVector2<Real>::operator-(Real rhs) const {
    return TVector2<Real>( X - rhs, Y - rhs );
}
template<>
inline TVector2<Float> TVector2<Float>::operator-(Float rhs) const {
    #ifdef MATH_USE_SSE
        TVector2<Float> vRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( vRes.X, vRes.Y );
        return vRes;
    #else
        return TVector2<Float>( X - rhs, Y - rhs );
    #endif
}
template<>
inline TVector2<Double> TVector2<Double>::operator-(Double rhs) const {
    #ifdef MATH_USE_SSE
        TVector2<Double> vRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( vRes.X, vRes.Y );
        return vRes;
    #else
        return TVector2<Double>( X - rhs, Y - rhs );
    #endif
}
template<typename Real>
inline TVector2<Real> TVector2<Real>::operator*(Real rhs) const {
    return TVector2<Real>( X * rhs, Y * rhs );
}
template<>
inline TVector2<Float> TVector2<Float>::operator*(Float rhs) const {
    #ifdef MATH_USE_SSE
        TVector2<Float> vRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->MulPF();
        MathSSEFn->Pop( vRes.X, vRes.Y );
        return vRes;
    #else
        return TVector2<Float>( X * rhs, Y * rhs );
    #endif
}
template<>
inline TVector2<Double> TVector2<Double>::operator*(Double rhs) const {
    #ifdef MATH_USE_SSE
        TVector2<Double> vRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( vRes.X, vRes.Y );
        return vRes;
    #else
        return TVector2<Double>( X * rhs, Y * rhs );
    #endif
}
template<typename Real>
inline TVector2<Real> TVector2<Real>::operator/(Real rhs) const {
    return TVector2<Real>( X / rhs, Y / rhs );
}
template<>
inline TVector2<Float> TVector2<Float>::operator/(Float rhs) const {
    #ifdef MATH_USE_SSE
        TVector2<Float> vRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->DivPF();
        MathSSEFn->Pop( vRes.X, vRes.Y );
        return vRes;
    #else
        return TVector2<Float>( X / rhs, Y / rhs );
    #endif
}
template<>
inline TVector2<Double> TVector2<Double>::operator/(Double rhs) const {
    #ifdef MATH_USE_SSE
        TVector2<Double> vRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( vRes.X, vRes.Y );
        return vRes;
    #else
        return TVector2<Double>( X / rhs, Y / rhs );
    #endif
}

template<typename Real>
inline TVector2<Real> & TVector2<Real>::operator+=(Real rhs) {
    X += rhs;
    Y += rhs;
    return (*this);
}
template<>
inline TVector2<Float> & TVector2<Float>::operator+=(Float rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( X, Y );
    #else
        X += rhs;
        Y += rhs;
    #endif
    return (*this);
}
template<>
inline TVector2<Double> & TVector2<Double>::operator+=(Double rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( X, Y );
    #else
        X += rhs;
        Y += rhs;
    #endif
    return (*this);
}
template<typename Real>
inline TVector2<Real> & TVector2<Real>::operator-=(Real rhs) {
    X -= rhs;
    Y -= rhs;
    return (*this);
}
template<>
inline TVector2<Float> & TVector2<Float>::operator-=(Float rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( X, Y );
    #else
        X -= rhs;
        Y -= rhs;
    #endif
    return (*this);
}
template<>
inline TVector2<Double> & TVector2<Double>::operator-=(Double rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( X, Y );
    #else
        X -= rhs;
        Y -= rhs;
    #endif
    return (*this);
}
template<typename Real>
inline TVector2<Real> & TVector2<Real>::operator*=(Real rhs) {
    X *= rhs;
    Y *= rhs;
    return (*this);
}
template<>
inline TVector2<Float> & TVector2<Float>::operator*=(Float rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->MulPF();
        MathSSEFn->Pop( X, Y );
    #else
        X *= rhs;
        Y *= rhs;
    #endif
    return (*this);
}
template<>
inline TVector2<Double> & TVector2<Double>::operator*=(Double rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( X, Y );
    #else
        X *= rhs;
        Y *= rhs;
    #endif
    return (*this);
}
template<typename Real>
inline TVector2<Real> & TVector2<Real>::operator/=(Real rhs) {
    X /= rhs;
    Y /= rhs;
    return (*this);
}
template<>
inline TVector2<Float> & TVector2<Float>::operator/=(Float rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->DivPF();
        MathSSEFn->Pop( X, Y );
    #else
        X /= rhs;
        Y /= rhs;
    #endif
    return (*this);
}
template<>
inline TVector2<Double> & TVector2<Double>::operator/=(Double rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( X, Y );
    #else
        X /= rhs;
        Y /= rhs;
    #endif
    return (*this);
}

template<typename Real>
inline TVector2<Real> TVector2<Real>::operator+(const TVector2<Real> & rhs) const {
    return TVector2<Real>( X + rhs.X, Y + rhs.Y );
}
template<>
inline TVector2<Float> TVector2<Float>::operator+(const TVector2<Float> & rhs) const {
    #ifdef MATH_USE_SSE
        TVector2<Float> vRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs.X, rhs.Y );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( vRes.X, vRes.Y );
        return vRes;
    #else
        return TVector2<Float>( X + rhs.X, Y + rhs.Y );
    #endif
}
template<>
inline TVector2<Double> TVector2<Double>::operator+(const TVector2<Double> & rhs) const {
    #ifdef MATH_USE_SSE
        TVector2<Double> vRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs.X, rhs.Y );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( vRes.X, vRes.Y );
        return vRes;
    #else
        return TVector2<Double>( X + rhs.X, Y + rhs.Y );
    #endif
}
template<typename Real>
inline TVector2<Real> TVector2<Real>::operator-(const TVector2<Real> & rhs) const {
    return TVector2<Real>( X - rhs.X, Y - rhs.Y );
}
template<>
inline TVector2<Float> TVector2<Float>::operator-(const TVector2<Float> & rhs) const {
    #ifdef MATH_USE_SSE
        TVector2<Float> vRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs.X, rhs.Y );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( vRes.X, vRes.Y );
        return vRes;
    #else
        return TVector2<Float>( X - rhs.X, Y - rhs.Y );
    #endif
}
template<>
inline TVector2<Double> TVector2<Double>::operator-(const TVector2<Double> & rhs) const {
    #ifdef MATH_USE_SSE
        TVector2<Double> vRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs.X, rhs.Y );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( vRes.X, vRes.Y );
        return vRes;
    #else
        return TVector2<Double>( X - rhs.X, Y - rhs.Y );
    #endif
}

template<typename Real>
inline TVector2<Real> & TVector2<Real>::operator+=(const TVector2<Real> & rhs) {
    X += rhs.X;
    Y += rhs.Y;
    return (*this);
}
template<>
inline TVector2<Float> & TVector2<Float>::operator+=(const TVector2<Float> & rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs.X, rhs.Y );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( X, Y );
    #else
        X += rhs.X;
        Y += rhs.Y;
    #endif
    return (*this);
}
template<>
inline TVector2<Double> & TVector2<Double>::operator+=(const TVector2<Double> & rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs.X, rhs.Y );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( X, Y );
    #else
        X += rhs.X;
        Y += rhs.Y;
    #endif
    return (*this);
}
template<typename Real>
inline TVector2<Real> & TVector2<Real>::operator-=(const TVector2<Real> & rhs) {
    X -= rhs.X;
    Y -= rhs.Y;
    return (*this);
}
template<>
inline TVector2<Float> & TVector2<Float>::operator-=(const TVector2<Float> & rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs.X, rhs.Y );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( X, Y );
    #else
        X -= rhs.X;
        Y -= rhs.Y;
    #endif
    return (*this);
}
template<>
inline TVector2<Double> & TVector2<Double>::operator-=(const TVector2<Double> & rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs.X, rhs.Y );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( X, Y );
    #else
        X -= rhs.X;
        Y -= rhs.Y;
    #endif
    return (*this);
}

template<typename Real>
inline Real TVector2<Real>::operator*(const TVector2<Real> & rhs) const {
    return ( (X * rhs.X) + (Y * rhs.Y) );
}
template<>
inline Float TVector2<Float>::operator*(const TVector2<Float> & rhs) const {
    #ifdef MATH_USE_SSE
        Float fRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs.X, rhs.Y );
        MathSSEFn->MulPF();
        MathSSEFn->HAddF(0);
        MathSSEFn->Pop( fRes );
        return fRes;
    #else
        return ( (X * rhs.X) + (Y * rhs.Y) );
    #endif
}
template<>
inline Double TVector2<Double>::operator*(const TVector2<Double> & rhs) const {
    #ifdef MATH_USE_SSE
        Double fRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs.X, rhs.Y );
        MathSSEFn->MulPD();
        MathSSEFn->HAddD(0);
        MathSSEFn->Pop( fRes );
        return fRes;
    #else
        return ( (X * rhs.X) + (Y * rhs.Y) );
    #endif
}

template<typename Real>
inline Real TVector2<Real>::operator^(const TVector2<Real> & rhs) const {
    return ( (X * rhs.Y) - (Y * rhs.X) );
}
template<>
inline Float TVector2<Float>::operator^(const TVector2<Float> & rhs) const {
    #ifdef MATH_USE_SSE
        Float fRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs.Y, rhs.X );
        MathSSEFn->MulPF();
        MathSSEFn->HSubF(0);
        MathSSEFn->Pop( fRes );
        return fRes;
    #else
        return ( (X * rhs.Y) - (Y * rhs.X) );
    #endif
}
template<>
inline Double TVector2<Double>::operator^(const TVector2<Double> & rhs) const {
    #ifdef MATH_USE_SSE
        Double fRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs.Y, rhs.X );
        MathSSEFn->MulPD();
        MathSSEFn->HSubF(0);
        MathSSEFn->Pop( fRes );
        return fRes;
    #else
        return ( (X * rhs.Y) - (Y * rhs.X) );
    #endif
}

template<typename Real>
inline Real TVector2<Real>::NormSqr() const {
    return ( X*X + Y*Y );
}
template<>
inline Float TVector2<Float>::NormSqr() const {
    #ifdef MATH_USE_SSE
        Float fRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( X, Y );
        MathSSEFn->MulPF();
        MathSSEFn->HAddF(0);
        MathSSEFn->Pop( fRes );
        return fRes;
    #else
        return ( X*X + Y*Y );
    #endif
}
template<>
inline Double TVector2<Double>::NormSqr() const {
    #ifdef MATH_USE_SSE
        Double fRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( X, Y );
        MathSSEFn->MulPD();
        MathSSEFn->HAddD(0);
        MathSSEFn->Pop( fRes );
        return fRes;
    #else
        return ( X*X + Y*Y );
    #endif
}
template<typename Real>
inline Real TVector2<Real>::Norm() const {
    return MathRealFn->Sqrt( NormSqr() );
}
template<typename Real>
inline Real TVector2<Real>::InvNormSqr() const {
    return MathRealFn->Invert( NormSqr() );
}
template<typename Real>
inline Real TVector2<Real>::InvNorm() const {
    return MathRealFn->InvSqrt( NormSqr() );
}

template<typename Real>
inline Real TVector2<Real>::Normalize() {
    const Real fZero = (Real)0;
    Real fNorm = Norm(), fInvNorm;
    if ( MathRealFn->EqualsZero(fNorm) ) {
        X = fZero;
        Y = fZero;
        return fZero;
    } else {
        fInvNorm = MathRealFn->Invert(fNorm);
        X *= fInvNorm;
        Y *= fInvNorm;
        return fNorm;
    }
}

template<typename Real>
inline TVector2<Real> TVector2<Real>::Perp() const {
    return TVector2<Real>(Y, -X);
}

template<typename Real>
inline TVector2<Real> TVector2<Real>::ProjectToNormal(const TVector2<Real> & vNormal) const {
    return ( vNormal * ( (*this) * vNormal ) );
}
template<typename Real>
inline TVector2<Real> TVector2<Real>::ProjectToPlane(const TVector2<Real> & vNormal) const {
    return ( (*this) - ProjectToNormal(vNormal) );
}

template<typename Real>
Void TVector2<Real>::OrthoNormalize( TVector2<Real> & vI, TVector2<Real> & vJ )
{
    // Gram-Schmidt OrthoNormalization
    vI.Normalize();

    Real fDotI = (vI * vJ);
    vJ -= (vI * fDotI);
    vJ.Normalize();
}
template<typename Real>
inline Void TVector2<Real>::MakeComplementBasis( TVector2<Real> & vI, const TVector2<Real> & vJ ) {
    vI = vJ.Perp();
}

