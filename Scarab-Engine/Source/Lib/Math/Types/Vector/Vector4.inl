/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Vector/Vector4.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Homogeneous 4D vector
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TVector4 implementation
template<typename Real>
const TVector4<Real> TVector4<Real>::Null = TVector4<Real>( (Real)0, (Real)0, (Real)0, (Real)0 );
template<typename Real>
const TVector4<Real> TVector4<Real>::eI = TVector4<Real>( (Real)1, (Real)0, (Real)0, (Real)0 );
template<typename Real>
const TVector4<Real> TVector4<Real>::eJ = TVector4<Real>( (Real)0, (Real)1, (Real)0, (Real)0 );
template<typename Real>
const TVector4<Real> TVector4<Real>::eK = TVector4<Real>( (Real)0, (Real)0, (Real)1, (Real)0 );

template<typename Real>
TVector4<Real>::TVector4()
{
    // nothing to do
}
template<typename Real>
TVector4<Real>::TVector4(Real x, Real y, Real z, Real w)
{
    X = x;
    Y = y;
    Z = z;
    W = w;
}
template<typename Real>
TVector4<Real>::TVector4(const Real vArr[4])
{
    X = vArr[0];
    Y = vArr[1];
    Z = vArr[2];
    W = vArr[3];
}
template<typename Real>
TVector4<Real>::TVector4(const TVector2<Real> & rhs)
{
    X = rhs.X;
    Y = rhs.Y;
    Z = (Real)0;
    W = (Real)0;
}
template<typename Real>
TVector4<Real>::TVector4(const TVector3<Real> & rhs)
{
    X = rhs.X;
    Y = rhs.Y;
    Z = rhs.Z;
    W = (Real)0;
}
template<typename Real>
TVector4<Real>::TVector4(const TVector4<Real> & rhs)
{
    X = rhs.X;
    Y = rhs.Y;
    Z = rhs.Z;
    W = rhs.W;
}
template<typename Real>
TVector4<Real>::~TVector4()
{
    // nothing to do
}

template<typename Real>
inline TVector4<Real> & TVector4<Real>::operator=(const TVector4<Real> & rhs) {
    X = rhs.X;
    Y = rhs.Y;
    Z = rhs.Z;
    W = rhs.W;
    return (*this);
}

template<typename Real>
inline TVector4<Real>::operator Real*() const {
    return (Real*)this;
}
template<typename Real>
inline TVector4<Real>::operator const Real*() const {
    return (const Real*)this;
}

template<typename Real>
inline Real & TVector4<Real>::operator[](Int i) {
    return *( ((Real*)this) + i );
}
template<typename Real>
inline Real TVector4<Real>::operator[](Int i) const {
    return *( ((const Real*)this) + i );
}
template<typename Real>
inline Real & TVector4<Real>::operator[](UInt i) {
    return *( ((Real*)this) + i );
}
template<typename Real>
inline Real TVector4<Real>::operator[](UInt i) const {
    return *( ((const Real*)this) + i );
}

template<typename Real>
inline TVector4<Real> TVector4<Real>::operator+() const {
    return TVector4<Real>(X, Y, Z, W);
}
template<typename Real>
inline TVector4<Real> TVector4<Real>::operator-() const {
    return TVector4<Real>(-X, -Y, -Z, W);
}

template<typename Real>
inline Bool TVector4<Real>::operator==(const TVector4<Real> & rhs) const {
    return ( MathRealFn->Equals(X,rhs.X) && MathRealFn->Equals(Y,rhs.Y) &&
             MathRealFn->Equals(Z,rhs.Z) && MathRealFn->Equals(W,rhs.W) );
}
template<typename Real>
inline Bool TVector4<Real>::operator!=(const TVector4<Real> & rhs) const {
    return ( !(MathRealFn->Equals(X,rhs.X)) || !(MathRealFn->Equals(Y,rhs.Y)) ||
             !(MathRealFn->Equals(Z,rhs.Z)) || !(MathRealFn->Equals(W,rhs.W)) );
}

template<typename Real>
inline TVector4<Real> TVector4<Real>::operator+(Real rhs) const {
    return TVector4<Real>( X + rhs, Y + rhs, Z + rhs, W );
}
template<>
inline TVector4<Float> TVector4<Float>::operator+(Float rhs) const {
    #ifdef MATH_USE_SSE
        TVector4<Float> vRes;
        MathSSEFn->Push( X, Y, Z, W );
        MathSSEFn->Push( rhs, rhs, rhs, 0.0f );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( vRes.X, vRes.Y, vRes.Z, vRes.W );
        return vRes;
    #else
        return TVector4<Float>( X + rhs, Y + rhs, Z + rhs, W );
    #endif
}
template<>
inline TVector4<Double> TVector4<Double>::operator+(Double rhs) const {
    #ifdef MATH_USE_SSE
        TVector4<Double> vRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( vRes.X, vRes.Y );
        MathSSEFn->Push( Z, W );
        MathSSEFn->Push( rhs, 0.0 );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( vRes.Z, vRes.W );
        return vRes;
    #else
        return TVector4<Double>( X + rhs, Y + rhs, Z + rhs, W );
    #endif
}
template<typename Real>
inline TVector4<Real> TVector4<Real>::operator-(Real rhs) const {
    return TVector4<Real>( X - rhs, Y - rhs, Z - rhs, W );
}
template<>
inline TVector4<Float> TVector4<Float>::operator-(Float rhs) const {
    #ifdef MATH_USE_SSE
        TVector4<Float> vRes;
        MathSSEFn->Push( X, Y, Z, W );
        MathSSEFn->Push( rhs, rhs, rhs, 0.0f );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( vRes.X, vRes.Y, vRes.Z, vRes.W );
        return vRes;
    #else
        return TVector4<Float>( X - rhs, Y - rhs, Z - rhs, W );
    #endif
}
template<>
inline TVector4<Double> TVector4<Double>::operator-(Double rhs) const {
    #ifdef MATH_USE_SSE
        TVector4<Double> vRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( vRes.X, vRes.Y );
        MathSSEFn->Push( Z, W );
        MathSSEFn->Push( rhs, 0.0 );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( vRes.Z, vRes.W );
        return vRes;
    #else
        return TVector4<Double>( X - rhs, Y - rhs, Z - rhs, W );
    #endif
}
template<typename Real>
inline TVector4<Real> TVector4<Real>::operator*(Real rhs) const {
    return TVector4<Real>( X * rhs, Y * rhs, Z * rhs, W );
}
template<>
inline TVector4<Float> TVector4<Float>::operator*(Float rhs) const {
    #ifdef MATH_USE_SSE
        TVector4<Float> vRes;
        MathSSEFn->Push( X, Y, Z, W );
        MathSSEFn->Push( rhs, rhs, rhs, 1.0f );
        MathSSEFn->MulPF();
        MathSSEFn->Pop( vRes.X, vRes.Y, vRes.Z, vRes.W );
        return vRes;
    #else
        return TVector4<Float>( X * rhs, Y * rhs, Z * rhs, W );
    #endif
}
template<>
inline TVector4<Double> TVector4<Double>::operator*(Double rhs) const {
    #ifdef MATH_USE_SSE
        TVector4<Double> vRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( vRes.X, vRes.Y );
        MathSSEFn->Push( Z, W );
        MathSSEFn->Push( rhs, 1.0 );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( vRes.Z, vRes.W );
        return vRes;
    #else
        return TVector4<Double>( X * rhs, Y * rhs, Z * rhs, W );
    #endif
}
template<typename Real>
inline TVector4<Real> TVector4<Real>::operator/(Real rhs) const {
    return TVector4<Real>( X / rhs, Y / rhs, Z / rhs, W );
}
template<>
inline TVector4<Float> TVector4<Float>::operator/(Float rhs) const {
    #ifdef MATH_USE_SSE
        TVector4<Float> vRes;
        MathSSEFn->Push( X, Y, Z, W );
        MathSSEFn->Push( rhs, rhs, rhs, 1.0f );
        MathSSEFn->DivPF();
        MathSSEFn->Pop( vRes.X, vRes.Y, vRes.Z, vRes.W );
        return vRes;
    #else
        return TVector4<Float>( X / rhs, Y / rhs, Z / rhs, W );
    #endif
}
template<>
inline TVector4<Double> TVector4<Double>::operator/(Double rhs) const {
    #ifdef MATH_USE_SSE
        TVector4<Double> vRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( vRes.X, vRes.Y );
        MathSSEFn->Push( Z, W );
        MathSSEFn->Push( rhs, 1.0 );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( vRes.Z, vRes.W );
        return vRes;
    #else
        return TVector4<Double>( X / rhs, Y / rhs, Z / rhs, W );
    #endif
}

template<typename Real>
inline TVector4<Real> & TVector4<Real>::operator+=(Real rhs) {
    X += rhs;
    Y += rhs;
    Z += rhs;
    return (*this);
}
template<>
inline TVector4<Float> & TVector4<Float>::operator+=(Float rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y, Z, W );
        MathSSEFn->Push( rhs, rhs, rhs, 0.0f );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( X, Y, Z, W );
    #else
        X += rhs;
        Y += rhs;
        Z += rhs;
    #endif
    return (*this);
}
template<>
inline TVector4<Double> & TVector4<Double>::operator+=(Double rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( X, Y );
        MathSSEFn->Push( Z, W );
        MathSSEFn->Push( rhs, 0.0 );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( Z, W );
    #else
        X += rhs;
        Y += rhs;
        Z += rhs;
    #endif
    return (*this);
}
template<typename Real>
inline TVector4<Real> & TVector4<Real>::operator-=(Real rhs) {
    X -= rhs;
    Y -= rhs;
    Z -= rhs;
    return (*this);
}
template<>
inline TVector4<Float> & TVector4<Float>::operator-=(Float rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y, Z, W );
        MathSSEFn->Push( rhs, rhs, rhs, 0.0f );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( X, Y, Z, W );
    #else
        X -= rhs;
        Y -= rhs;
        Z -= rhs;
    #endif
    return (*this);
}
template<>
inline TVector4<Double> & TVector4<Double>::operator-=(Double rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( X, Y );
        MathSSEFn->Push( Z, W );
        MathSSEFn->Push( rhs, 0.0 );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( Z, W );
    #else
        X -= rhs;
        Y -= rhs;
        Z -= rhs;
    #endif
    return (*this);
}
template<typename Real>
inline TVector4<Real> & TVector4<Real>::operator*=(Real rhs) {
    X *= rhs;
    Y *= rhs;
    Z *= rhs;
    return (*this);
}
template<>
inline TVector4<Float> & TVector4<Float>::operator*=(Float rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y, Z, W );
        MathSSEFn->Push( rhs, rhs, rhs, 1.0f );
        MathSSEFn->MulPF();
        MathSSEFn->Pop( X, Y, Z, W );
    #else
        X *= rhs;
        Y *= rhs;
        Z *= rhs;
    #endif
    return (*this);
}
template<>
inline TVector4<Double> & TVector4<Double>::operator*=(Double rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( X, Y );
        MathSSEFn->Push( Z, W );
        MathSSEFn->Push( rhs, 1.0 );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( Z, W );
    #else
        X *= rhs;
        Y *= rhs;
        Z *= rhs;
    #endif
    return (*this);
}
template<typename Real>
inline TVector4<Real> & TVector4<Real>::operator/=(Real rhs) {
    X /= rhs;
    Y /= rhs;
    Z /= rhs;
    return (*this);
}
template<>
inline TVector4<Float> & TVector4<Float>::operator/=(Float rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y, Z, W );
        MathSSEFn->Push( rhs, rhs, rhs, 1.0f );
        MathSSEFn->DivPF();
        MathSSEFn->Pop( X, Y, Z, W );
    #else
        X /= rhs;
        Y /= rhs;
        Z /= rhs;
    #endif
    return (*this);
}
template<>
inline TVector4<Double> & TVector4<Double>::operator/=(Double rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( X, Y );
        MathSSEFn->Push( Z, W );
        MathSSEFn->Push( rhs, 1.0 );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( Z, W );
    #else
        X /= rhs;
        Y /= rhs;
        Z /= rhs;
    #endif
    return (*this);
}

template<typename Real>
inline TVector4<Real> TVector4<Real>::operator+(const TVector4<Real> & rhs) const {
    return TVector4<Real>( X + rhs.X, Y + rhs.Y, Z + rhs.Z, W + rhs.W );
}
template<>
inline TVector4<Float> TVector4<Float>::operator+(const TVector4<Float> & rhs) const {
    #ifdef MATH_USE_SSE
        TVector4<Float> vRes;
        MathSSEFn->Push( X, Y, Z, W );
        MathSSEFn->Push( rhs.X, rhs.Y, rhs.Z, rhs.W );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( vRes.X, vRes.Y, vRes.Z, vRes.W );
        return vRes;
    #else
        return TVector4<Float>( X + rhs.X, Y + rhs.Y, Z + rhs.Z, W + rhs.W );
    #endif
}
template<>
inline TVector4<Double> TVector4<Double>::operator+(const TVector4<Double> & rhs) const {
    #ifdef MATH_USE_SSE
        TVector4<Double> vRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs.X, rhs.Y );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( vRes.X, vRes.Y );
        MathSSEFn->Push( Z, W );
        MathSSEFn->Push( rhs.Z, rhs.W );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( vRes.Z, vRes.W );
        return vRes;
    #else
        return TVector4<Double>( X + rhs.X, Y + rhs.Y, Z + rhs.Z, W + rhs.W );
    #endif
}
template<typename Real>
inline TVector4<Real> TVector4<Real>::operator-(const TVector4<Real> & rhs) const {
    return TVector4<Real>( X - rhs.X, Y - rhs.Y, Z - rhs.Z, W - rhs.W );
}
template<>
inline TVector4<Float> TVector4<Float>::operator-(const TVector4<Float> & rhs) const {
    #ifdef MATH_USE_SSE
        TVector4<Float> vRes;
        MathSSEFn->Push( X, Y, Z, W );
        MathSSEFn->Push( rhs.X, rhs.Y, rhs.Z, rhs.W );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( vRes.X, vRes.Y, vRes.Z, vRes.W );
        return vRes;
    #else
        return TVector4<Float>( X - rhs.X, Y - rhs.Y, Z - rhs.Z, W - rhs.W );
    #endif
}
template<>
inline TVector4<Double> TVector4<Double>::operator-(const TVector4<Double> & rhs) const {
    #ifdef MATH_USE_SSE
        TVector4<Double> vRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs.X, rhs.Y );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( vRes.X, vRes.Y );
        MathSSEFn->Push( Z, W );
        MathSSEFn->Push( rhs.Z, rhs.W );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( vRes.Z, vRes.W );
        return vRes;
    #else
        return TVector4<Double>( X - rhs.X, Y - rhs.Y, Z - rhs.Z, W - rhs.W );
    #endif
}

template<typename Real>
inline TVector4<Real> & TVector4<Real>::operator+=(const TVector4<Real> & rhs) {
    X += rhs.X;
    Y += rhs.Y;
    Z += rhs.Z;
    W += rhs.W;
    return (*this);
}
template<>
inline TVector4<Float> & TVector4<Float>::operator+=(const TVector4<Float> & rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y, Z, W );
        MathSSEFn->Push( rhs.X, rhs.Y, rhs.Z, rhs.W );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( X, Y, Z, W );
    #else
        X += rhs.X;
        Y += rhs.Y;
        Z += rhs.Z;
        W += rhs.W;
    #endif
    return (*this);
}
template<>
inline TVector4<Double> & TVector4<Double>::operator+=(const TVector4<Double> & rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs.X, rhs.Y );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( X, Y );
        MathSSEFn->Push( Z, W );
        MathSSEFn->Push( rhs.Z, rhs.W );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( Z, W );
    #else
        X += rhs.X;
        Y += rhs.Y;
        Z += rhs.Z;
        W += rhs.W;
    #endif
    return (*this);
}
template<typename Real>
inline TVector4<Real> & TVector4<Real>::operator-=(const TVector4<Real> & rhs) {
    X -= rhs.X;
    Y -= rhs.Y;
    Z -= rhs.Z;
    W -= rhs.W;
    return (*this);
}
template<>
inline TVector4<Float> & TVector4<Float>::operator-=(const TVector4<Float> & rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y, Z, W );
        MathSSEFn->Push( rhs.X, rhs.Y, rhs.Z, rhs.W );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( X, Y, Z, W );
    #else
        X -= rhs.X;
        Y -= rhs.Y;
        Z -= rhs.Z;
        W -= rhs.W;
    #endif
    return (*this);
}
template<>
inline TVector4<Double> & TVector4<Double>::operator-=(const TVector4<Double> & rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs.X, rhs.Y );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( X, Y );
        MathSSEFn->Push( Z, W );
        MathSSEFn->Push( rhs.Z, rhs.W );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( Z, W );
    #else
        X -= rhs.X;
        Y -= rhs.Y;
        Z -= rhs.Z;
        W -= rhs.W;
    #endif
    return (*this);
}

template<typename Real>
inline Real TVector4<Real>::operator*(const TVector4<Real> & rhs) const {
    return ( (X * rhs.X) + (Y * rhs.Y) + (Z * rhs.Z) + (W * rhs.W) );
}
template<>
inline Float TVector4<Float>::operator*(const TVector4<Float> & rhs) const {
    #ifdef MATH_USE_SSE
        Float fRes;
        MathSSEFn->Push( X, Y, Z, W );
        MathSSEFn->Push( rhs.X, rhs.Y, rhs.Z, rhs.W );
        MathSSEFn->MulPF();
        MathSSEFn->HAddF(0);
        MathSSEFn->HAddF(0);
        MathSSEFn->Pop( fRes );
        return fRes;
    #else
        return ( (X * rhs.X) + (Y * rhs.Y) + (Z * rhs.Z) + (W * rhs.W) );
    #endif
}
template<>
inline Double TVector4<Double>::operator*(const TVector4<Double> & rhs) const {
    #ifdef MATH_USE_SSE
        Double fRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs.X, rhs.Y );
        MathSSEFn->MulPD();
        MathSSEFn->HAddD(0);
        MathSSEFn->Push( Z, W );
        MathSSEFn->Push( rhs.Z, rhs.W );
        MathSSEFn->MulPD();
        MathSSEFn->HAddD(0);
        MathSSEFn->AddPD();
        MathSSEFn->Pop( fRes );
        return fRes;
    #else
        return ( (X * rhs.X) + (Y * rhs.Y) + (Z * rhs.Z) + (W * rhs.W) );
    #endif
}

template<typename Real>
inline TVector4<Real> TVector4<Real>::operator^(const TVector4<Real> & rhs) const {
    return TVector4<Real> (
        (Y * rhs.Z) - (Z * rhs.Y),
        (Z * rhs.X) - (X * rhs.Z),
        (X * rhs.Y) - (Y * rhs.X),
        (Real)0
    );
}
template<>
inline TVector4<Float> TVector4<Float>::operator^(const TVector4<Float> & rhs) const {
    #ifdef MATH_USE_SSE
        TVector4<Float> vRes;
        MathSSEFn->Push( Y, Z, X );
        MathSSEFn->Push( rhs.Z, rhs.X, rhs.Y );
        MathSSEFn->MulPF();
        MathSSEFn->Push( Z, X, Y );
        MathSSEFn->Push( rhs.Y, rhs.Z, rhs.X );
        MathSSEFn->MulPF();
        MathSSEFn->SubPF();
        MathSSEFn->Pop( vRes.X, vRes.Y, vRes.Z );
        return vRes;
    #else
        return TVector4<Float> (
            (Y * rhs.Z) - (Z * rhs.Y),
            (Z * rhs.X) - (X * rhs.Z),
            (X * rhs.Y) - (Y * rhs.X),
            0.0f
        );
    #endif
}
template<>
inline TVector4<Double> TVector4<Double>::operator^(const TVector4<Double> & rhs) const {
    #ifdef MATH_USE_SSE
        TVector4<Double> vRes;
        MathSSEFn->Push( Y, Z );
        MathSSEFn->Push( rhs.Z, rhs.X );
        MathSSEFn->MulPD();
        MathSSEFn->Push( Z, X );
        MathSSEFn->Push( rhs.Y, rhs.Z );
        MathSSEFn->MulPD();
        MathSSEFn->SubPD();
        MathSSEFn->Pop( vRes.X, vRes.Y );
        MathSSEFn->Push( X, 0.0 );
        MathSSEFn->Push( rhs.Y, 0.0 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( Y, 0.0 );
        MathSSEFn->Push( rhs.X, 0.0 );
        MathSSEFn->MulPD();
        MathSSEFn->SubPD();
        MathSSEFn->Pop( vRes.Z, vRes.W );
        return vRes;
    #else
        return TVector4<Double> (
            (Y * rhs.Z) - (Z * rhs.Y),
            (Z * rhs.X) - (X * rhs.Z),
            (X * rhs.Y) - (Y * rhs.X),
            0.0
        );
    #endif
}

template<typename Real>
inline Real TVector4<Real>::NormSqr() const {
    return ( X*X + Y*Y + Z*Z + W*W );
}
template<>
inline Float TVector4<Float>::NormSqr() const {
    #ifdef MATH_USE_SSE
        Float fRes;
        MathSSEFn->Push( X, Y, Z, W );
        MathSSEFn->Push( X, Y, Z, W );
        MathSSEFn->MulPF();
        MathSSEFn->HAddF(0);
        MathSSEFn->HAddF(0);
        MathSSEFn->Pop( fRes );
        return fRes;
    #else
        return ( X*X + Y*Y + Z*Z + W*W );
    #endif
}
template<>
inline Double TVector4<Double>::NormSqr() const {
    #ifdef MATH_USE_SSE
        Double fRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( X, Y );
        MathSSEFn->MulPD();
        MathSSEFn->HAddD(0);
        MathSSEFn->Push( Z, W );
        MathSSEFn->Push( Z, W );
        MathSSEFn->MulPD();
        MathSSEFn->HAddD(0);
        MathSSEFn->AddPD();
        MathSSEFn->Pop( fRes );
        return fRes;
    #else
        return ( X*X + Y*Y + Z*Z + W*W );
    #endif
}

template<typename Real>
inline Real TVector4<Real>::Norm() const {
    return MathRealFn->Sqrt( NormSqr() );
}
template<typename Real>
inline Real TVector4<Real>::InvNormSqr() const {
    return MathRealFn->Invert( NormSqr() );
}
template<typename Real>
inline Real TVector4<Real>::InvNorm() const {
    return MathRealFn->InvSqrt( NormSqr() );
}

template<typename Real>
inline Real TVector4<Real>::Normalize() {
    const Real fZero = (Real)0;
    Real fNorm = Norm(), fInvNorm;
    if ( MathRealFn->EqualsZero(fNorm) ) {
        X = fZero;
        Y = fZero;
        Z = fZero;
        W = fZero;
        return fZero;
    } else {
        fInvNorm = MathRealFn->Invert(fNorm);
        X *= fInvNorm;
        Y *= fInvNorm;
        Z *= fInvNorm;
        W *= fInvNorm;
        return fNorm;
    }
}

template<typename Real>
inline TVector4<Real> TVector4<Real>::ProjectToNormal(const TVector4<Real> & vNormal) const {
    return ( vNormal * ( (*this) * vNormal ) );
}
template<typename Real>
inline TVector4<Real> TVector4<Real>::ProjectToPlane(const TVector4<Real> & vNormal) const {
    return ( (*this) - ProjectToNormal(vNormal) );
}

template<typename Real>
Void TVector4<Real>::OrthoNormalize( TVector4<Real> & vI, TVector4<Real> & vJ, TVector4<Real> & vK )
{
    // Gram-Schmidt OrthoNormalization
    vI.Normalize();

    Real fDotI = (vI * vJ);
    vJ -= (vI * fDotI);
    vJ.Normalize();

    Real fDotJ = (vJ * vK);
    fDotI = (vI * vK);
    vK -= ( (vI * fDotI) + (vJ * fDotJ) );
    vK.Normalize();
}
template<typename Real>
Void TVector4<Real>::MakeComplementBasis( TVector4<Real> & vI, TVector4<Real> & vJ, const TVector4<Real> & vK )
{
    Real fInvNorm;
    if ( MathRealFn->Abs(vK.X) >= MathRealFn->Abs(vK.Y) ) {
        fInvNorm = MathRealFn->InvSqrt( (vK.X * vK.X) + (vK.Z * vK.Z) );
        vI.X = -(vK.Z * fInvNorm);
        vI.Y = (Real)0;
        vI.Z = +(vK.X * fInvNorm);
        vJ.X = (vK.Y * vI.Z);
        vJ.Y = ( (vK.Z * vI.X) - (vK.X * vI.Z) );
        vJ.Z = -(vK.Y * vI.X);
    } else {
        fInvNorm = MathRealFn->InvSqrt( (vK.Y * vK.Y) + (vK.Z * vK.Z) );
        vI.X = (Real)0;
        vI.Y = +(vK.Z * fInvNorm);
        vI.Z = -(vK.Y * fInvNorm);
        vJ.X = ( (vK.Y * vI.Z) - (vK.Z * vI.Y) );
        vJ.Y = -(vK.X * vI.Z);
        vJ.Z = (vK.X * vI.Y);
    }
}

