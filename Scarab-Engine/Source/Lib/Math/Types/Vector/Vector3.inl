/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Vector/Vector3.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : 3D vector
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TVector3 implementation
template<typename Real>
const TVector3<Real> TVector3<Real>::Null = TVector3<Real>( (Real)0, (Real)0, (Real)0 );
template<typename Real>
const TVector3<Real> TVector3<Real>::eI = TVector3<Real>( (Real)1, (Real)0, (Real)0 );
template<typename Real>
const TVector3<Real> TVector3<Real>::eJ = TVector3<Real>( (Real)0, (Real)1, (Real)0 );
template<typename Real>
const TVector3<Real> TVector3<Real>::eK = TVector3<Real>( (Real)0, (Real)0, (Real)1 );

template<typename Real>
TVector3<Real>::TVector3()
{
    // nothing to do
}
template<typename Real>
TVector3<Real>::TVector3(Real x, Real y, Real z)
{
    X = x;
    Y = y;
    Z = z;
}
template<typename Real>
TVector3<Real>::TVector3(const Real vArr[3])
{
    X = vArr[0];
    Y = vArr[1];
    Z = vArr[2];
}
template<typename Real>
TVector3<Real>::TVector3(const TVector2<Real> & rhs)
{
    X = rhs.X;
    Y = rhs.Y;
    Z = (Real)0;
}
template<typename Real>
TVector3<Real>::TVector3(const TVector3<Real> & rhs)
{
    X = rhs.X;
    Y = rhs.Y;
    Z = rhs.Z;
}
template<typename Real>
TVector3<Real>::TVector3(const TVector4<Real> & rhs)
{
    X = rhs.X;
    Y = rhs.Y;
    Z = rhs.Z;
}
template<typename Real>
TVector3<Real>::~TVector3()
{
    // nothing to do
}

template<typename Real>
inline TVector3<Real> & TVector3<Real>::operator=(const TVector3<Real> & rhs) {
    X = rhs.X;
    Y = rhs.Y;
    Z = rhs.Z;
    return (*this);
}

template<typename Real>
inline TVector3<Real>::operator Real*() const {
    return (Real*)this;
}
template<typename Real>
inline TVector3<Real>::operator const Real*() const {
    return (const Real*)this;
}

template<typename Real>
inline Real & TVector3<Real>::operator[](Int i) {
    return *( ((Real*)this) + i );
}
template<typename Real>
inline Real TVector3<Real>::operator[](Int i) const {
    return *( ((const Real*)this) + i );
}
template<typename Real>
inline Real & TVector3<Real>::operator[](UInt i) {
    return *( ((Real*)this) + i );
}
template<typename Real>
inline Real TVector3<Real>::operator[](UInt i) const {
    return *( ((const Real*)this) + i );
}

template<typename Real>
inline TVector3<Real> TVector3<Real>::operator+() const {
    return TVector3<Real>(X, Y, Z);
}
template<typename Real>
inline TVector3<Real> TVector3<Real>::operator-() const {
    return TVector3<Real>(-X, -Y, -Z);
}

template<typename Real>
inline Bool TVector3<Real>::operator==(const TVector3<Real> & rhs) const {
    return ( MathRealFn->Equals(X,rhs.X) && MathRealFn->Equals(Y,rhs.Y) &&
             MathRealFn->Equals(Z,rhs.Z) );
}
template<typename Real>
inline Bool TVector3<Real>::operator!=(const TVector3<Real> & rhs) const {
    return ( !(MathRealFn->Equals(X,rhs.X)) || !(MathRealFn->Equals(Y,rhs.Y)) ||
             !(MathRealFn->Equals(Z,rhs.Z)) );
}

template<typename Real>
inline TVector3<Real> TVector3<Real>::operator+(Real rhs) const {
    return TVector3<Real>( X + rhs, Y + rhs, Z + rhs );
}
template<>
inline TVector3<Float> TVector3<Float>::operator+(Float rhs) const {
    #ifdef MATH_USE_SSE
        TVector3<Float> vRes;
        MathSSEFn->Push( X, Y, Z );
        MathSSEFn->Push( rhs, rhs, rhs );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( vRes.X, vRes.Y, vRes.Z );
        return vRes;
    #else
        return TVector3<Float>( X + rhs, Y + rhs, Z + rhs );
    #endif
}
template<>
inline TVector3<Double> TVector3<Double>::operator+(Double rhs) const {
    #ifdef MATH_USE_SSE
        TVector3<Double> vRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( vRes.X, vRes.Y );
        MathSSEFn->Push( Z );
        MathSSEFn->Push( rhs );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( vRes.Z );
        return vRes;
    #else
        return TVector3<Double>( X + rhs, Y + rhs, Z + rhs );
    #endif
}
template<typename Real>
inline TVector3<Real> TVector3<Real>::operator-(Real rhs) const {
    return TVector3<Real>( X - rhs, Y - rhs, Z - rhs );
}
template<>
inline TVector3<Float> TVector3<Float>::operator-(Float rhs) const {
    #ifdef MATH_USE_SSE
        TVector3<Float> vRes;
        MathSSEFn->Push( X, Y, Z );
        MathSSEFn->Push( rhs, rhs, rhs );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( vRes.X, vRes.Y, vRes.Z );
        return vRes;
    #else
        return TVector3<Float>( X - rhs, Y - rhs, Z - rhs );
    #endif
}
template<>
inline TVector3<Double> TVector3<Double>::operator-(Double rhs) const {
    #ifdef MATH_USE_SSE
        TVector3<Double> vRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( vRes.X, vRes.Y );
        MathSSEFn->Push( Z );
        MathSSEFn->Push( rhs );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( vRes.Z );
        return vRes;
    #else
        return TVector3<Double>( X - rhs, Y - rhs, Z - rhs );
    #endif
}
template<typename Real>
inline TVector3<Real> TVector3<Real>::operator*(Real rhs) const {
    return TVector3<Real>( X * rhs, Y * rhs, Z * rhs );
}
template<>
inline TVector3<Float> TVector3<Float>::operator*(Float rhs) const {
    #ifdef MATH_USE_SSE
        TVector3<Float> vRes;
        MathSSEFn->Push( X, Y, Z );
        MathSSEFn->Push( rhs, rhs, rhs );
        MathSSEFn->MulPF();
        MathSSEFn->Pop( vRes.X, vRes.Y, vRes.Z );
        return vRes;
    #else
        return TVector3<Float>( X * rhs, Y * rhs, Z * rhs );
    #endif
}
template<>
inline TVector3<Double> TVector3<Double>::operator*(Double rhs) const {
    #ifdef MATH_USE_SSE
        TVector3<Double> vRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( vRes.X, vRes.Y );
        MathSSEFn->Push( Z );
        MathSSEFn->Push( rhs );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( vRes.Z );
        return vRes;
    #else
        return TVector3<Double>( X * rhs, Y * rhs, Z * rhs );
    #endif
}
template<typename Real>
inline TVector3<Real> TVector3<Real>::operator/(Real rhs) const {
    return TVector3<Real>( X / rhs, Y / rhs, Z / rhs );
}
template<>
inline TVector3<Float> TVector3<Float>::operator/(Float rhs) const {
    #ifdef MATH_USE_SSE
        TVector3<Float> vRes;
        MathSSEFn->Push( X, Y, Z );
        MathSSEFn->Push( rhs, rhs, rhs );
        MathSSEFn->DivPF();
        MathSSEFn->Pop( vRes.X, vRes.Y, vRes.Z );
        return vRes;
    #else
        return TVector3<Float>( X / rhs, Y / rhs, Z / rhs );
    #endif
}
template<>
inline TVector3<Double> TVector3<Double>::operator/(Double rhs) const {
    #ifdef MATH_USE_SSE
        TVector3<Double> vRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( vRes.X, vRes.Y );
        MathSSEFn->Push( Z );
        MathSSEFn->Push( rhs );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( vRes.Z );
        return vRes;
    #else
        return TVector3<Double>( X / rhs, Y / rhs, Z / rhs );
    #endif
}

template<typename Real>
inline TVector3<Real> & TVector3<Real>::operator+=(Real rhs) {
    X += rhs;
    Y += rhs;
    Z += rhs;
    return (*this);
}
template<>
inline TVector3<Float> & TVector3<Float>::operator+=(Float rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y, Z );
        MathSSEFn->Push( rhs, rhs, rhs );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( X, Y, Z );
    #else
        X += rhs;
        Y += rhs;
        Z += rhs;
    #endif
    return (*this);
}
template<>
inline TVector3<Double> & TVector3<Double>::operator+=(Double rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( X, Y );
        MathSSEFn->Push( Z );
        MathSSEFn->Push( rhs );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( Z );
    #else
        X += rhs;
        Y += rhs;
        Z += rhs;
    #endif
    return (*this);
}
template<typename Real>
inline TVector3<Real> & TVector3<Real>::operator-=(Real rhs) {
    X -= rhs;
    Y -= rhs;
    Z -= rhs;
    return (*this);
}
template<>
inline TVector3<Float> & TVector3<Float>::operator-=(Float rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y, Z );
        MathSSEFn->Push( rhs, rhs, rhs );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( X, Y, Z );
    #else
        X -= rhs;
        Y -= rhs;
        Z -= rhs;
    #endif
    return (*this);
}
template<>
inline TVector3<Double> & TVector3<Double>::operator-=(Double rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( X, Y );
        MathSSEFn->Push( Z );
        MathSSEFn->Push( rhs );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( Z );
    #else
        X -= rhs;
        Y -= rhs;
        Z -= rhs;
    #endif
    return (*this);
}
template<typename Real>
inline TVector3<Real> & TVector3<Real>::operator*=(Real rhs) {
    X *= rhs;
    Y *= rhs;
    Z *= rhs;
    return (*this);
}
template<>
inline TVector3<Float> & TVector3<Float>::operator*=(Float rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y, Z );
        MathSSEFn->Push( rhs, rhs, rhs );
        MathSSEFn->MulPF();
        MathSSEFn->Pop( X, Y, Z );
    #else
        X *= rhs;
        Y *= rhs;
        Z *= rhs;
    #endif
    return (*this);
}
template<>
inline TVector3<Double> & TVector3<Double>::operator*=(Double rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( X, Y );
        MathSSEFn->Push( Z );
        MathSSEFn->Push( rhs );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( Z );
    #else
        X *= rhs;
        Y *= rhs;
        Z *= rhs;
    #endif
    return (*this);
}
template<typename Real>
inline TVector3<Real> & TVector3<Real>::operator/=(Real rhs) {
    X /= rhs;
    Y /= rhs;
    Z /= rhs;
    return (*this);
}
template<>
inline TVector3<Float> & TVector3<Float>::operator/=(Float rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y, Z );
        MathSSEFn->Push( rhs, rhs, rhs );
        MathSSEFn->DivPF();
        MathSSEFn->Pop( X, Y, Z );
    #else
        X /= rhs;
        Y /= rhs;
        Z /= rhs;
    #endif
    return (*this);
}
template<>
inline TVector3<Double> & TVector3<Double>::operator/=(Double rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( X, Y );
        MathSSEFn->Push( Z );
        MathSSEFn->Push( rhs );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( Z );
    #else
        X /= rhs;
        Y /= rhs;
        Z /= rhs;
    #endif
    return (*this);
}

template<typename Real>
inline TVector3<Real> TVector3<Real>::operator+(const TVector3<Real> & rhs) const {
    return TVector3<Real>( X + rhs.X, Y + rhs.Y, Z + rhs.Z );
}
template<>
inline TVector3<Float> TVector3<Float>::operator+(const TVector3<Float> & rhs) const {
    #ifdef MATH_USE_SSE
        TVector3<Float> vRes;
        MathSSEFn->Push( X, Y, Z );
        MathSSEFn->Push( rhs.X, rhs.Y, rhs.Z );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( vRes.X, vRes.Y, vRes.Z );
        return vRes;
    #else
        return TVector3<Float>( X + rhs.X, Y + rhs.Y, Z + rhs.Z );
    #endif
}
template<>
inline TVector3<Double> TVector3<Double>::operator+(const TVector3<Double> & rhs) const {
    #ifdef MATH_USE_SSE
        TVector3<Double> vRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs.X, rhs.Y );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( vRes.X, vRes.Y );
        MathSSEFn->Push( Z );
        MathSSEFn->Push( rhs.Z );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( vRes.Z );
        return vRes;
    #else
        return TVector3<Double>( X + rhs.X, Y + rhs.Y, Z + rhs.Z );
    #endif
}
template<typename Real>
inline TVector3<Real> TVector3<Real>::operator-(const TVector3<Real> & rhs) const {
    return TVector3<Real>( X - rhs.X, Y - rhs.Y, Z - rhs.Z );
}
template<>
inline TVector3<Float> TVector3<Float>::operator-(const TVector3<Float> & rhs) const {
    #ifdef MATH_USE_SSE
        TVector3<Float> vRes;
        MathSSEFn->Push( X, Y, Z );
        MathSSEFn->Push( rhs.X, rhs.Y, rhs.Z );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( vRes.X, vRes.Y, vRes.Z );
        return vRes;
    #else
        return TVector3<Float>( X - rhs.X, Y - rhs.Y, Z - rhs.Z );
    #endif
}
template<>
inline TVector3<Double> TVector3<Double>::operator-(const TVector3<Double> & rhs) const {
    #ifdef MATH_USE_SSE
        TVector3<Double> vRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs.X, rhs.Y );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( vRes.X, vRes.Y );
        MathSSEFn->Push( Z );
        MathSSEFn->Push( rhs.Z );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( vRes.Z );
        return vRes;
    #else
        return TVector3<Double>( X - rhs.X, Y - rhs.Y, Z - rhs.Z );
    #endif
}

template<typename Real>
inline TVector3<Real> & TVector3<Real>::operator+=(const TVector3<Real> & rhs) {
    X += rhs.X;
    Y += rhs.Y;
    Z += rhs.Z;
    return (*this);
}
template<>
inline TVector3<Float> & TVector3<Float>::operator+=(const TVector3<Float> & rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y, Z );
        MathSSEFn->Push( rhs.X, rhs.Y, rhs.Z );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( X, Y, Z );
    #else
        X += rhs.X;
        Y += rhs.Y;
        Z += rhs.Z;
    #endif
    return (*this);
}
template<>
inline TVector3<Double> & TVector3<Double>::operator+=(const TVector3<Double> & rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs.X, rhs.Y );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( X, Y );
        MathSSEFn->Push( Z );
        MathSSEFn->Push( rhs.Z );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( Z );
    #else
        X += rhs.X;
        Y += rhs.Y;
        Z += rhs.Z;
    #endif
    return (*this);
}
template<typename Real>
inline TVector3<Real> & TVector3<Real>::operator-=(const TVector3<Real> & rhs) {
    X -= rhs.X;
    Y -= rhs.Y;
    Z -= rhs.Z;
    return (*this);
}
template<>
inline TVector3<Float> & TVector3<Float>::operator-=(const TVector3<Float> & rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y, Z );
        MathSSEFn->Push( rhs.X, rhs.Y, rhs.Z );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( X, Y, Z );
    #else
        X -= rhs.X;
        Y -= rhs.Y;
        Z -= rhs.Z;
    #endif
    return (*this);
}
template<>
inline TVector3<Double> & TVector3<Double>::operator-=(const TVector3<Double> & rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs.X, rhs.Y );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( X, Y );
        MathSSEFn->Push( Z );
        MathSSEFn->Push( rhs.Z );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( Z );
    #else
        X -= rhs.X;
        Y -= rhs.Y;
        Z -= rhs.Z;
    #endif
    return (*this);
}

template<typename Real>
inline Real TVector3<Real>::operator*(const TVector3<Real> & rhs) const {
    return ( (X * rhs.X) + (Y * rhs.Y) + (Z * rhs.Z) );
}
template<>
inline Float TVector3<Float>::operator*(const TVector3<Float> & rhs) const {
    #ifdef MATH_USE_SSE
        Float fRes;
        MathSSEFn->Push( X, Y, Z, 0.0f );
        MathSSEFn->Push( rhs.X, rhs.Y, rhs.Z, 0.0f );
        MathSSEFn->MulPF();
        MathSSEFn->HAddF(0);
        MathSSEFn->HAddF(0);
        MathSSEFn->Pop( fRes );
        return fRes;
    #else
        return ( (X * rhs.X) + (Y * rhs.Y) + (Z * rhs.Z) );
    #endif
}
template<>
inline Double TVector3<Double>::operator*(const TVector3<Double> & rhs) const {
    #ifdef MATH_USE_SSE
        Double fRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs.X, rhs.Y );
        MathSSEFn->MulPD();
        MathSSEFn->HAddD(0);
        MathSSEFn->Push( Z, 0.0 );
        MathSSEFn->Push( rhs.Z, 0.0 );
        MathSSEFn->MulPD();
        MathSSEFn->HAddD(0);
        MathSSEFn->AddPD();
        MathSSEFn->Pop( fRes );
        return fRes;
    #else
        return ( (X * rhs.X) + (Y * rhs.Y) + (Z * rhs.Z) );
    #endif
}

template<typename Real>
inline TVector3<Real> TVector3<Real>::operator^(const TVector3<Real> & rhs) const {
    return TVector3<Real> (
        (Y * rhs.Z) - (Z * rhs.Y),
        (Z * rhs.X) - (X * rhs.Z),
        (X * rhs.Y) - (Y * rhs.X)
    );
}
template<>
inline TVector3<Float> TVector3<Float>::operator^(const TVector3<Float> & rhs) const {
    #ifdef MATH_USE_SSE
        TVector3<Float> vRes;
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
        return TVector3<Float> (
            (Y * rhs.Z) - (Z * rhs.Y),
            (Z * rhs.X) - (X * rhs.Z),
            (X * rhs.Y) - (Y * rhs.X)
        );
    #endif
}
template<>
inline TVector3<Double> TVector3<Double>::operator^(const TVector3<Double> & rhs) const {
    #ifdef MATH_USE_SSE
        TVector3<Double> vRes;
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
        MathSSEFn->Pop( vRes.Z );
        return vRes;
    #else
        return TVector3<Double> (
            (Y * rhs.Z) - (Z * rhs.Y),
            (Z * rhs.X) - (X * rhs.Z),
            (X * rhs.Y) - (Y * rhs.X)
        );
    #endif
}

template<typename Real>
inline Real TVector3<Real>::NormSqr() const {
    return ( X*X + Y*Y + Z*Z );
}
template<>
inline Float TVector3<Float>::NormSqr() const {
    #ifdef MATH_USE_SSE
        Float fRes;
        MathSSEFn->Push( X, Y, Z, 0.0f );
        MathSSEFn->Push( X, Y, Z, 0.0f );
        MathSSEFn->MulPF();
        MathSSEFn->HAddF(0);
        MathSSEFn->HAddF(0);
        MathSSEFn->Pop( fRes );
        return fRes;
    #else
        return ( X*X + Y*Y + Z*Z );
    #endif
}
template<>
inline Double TVector3<Double>::NormSqr() const {
    #ifdef MATH_USE_SSE
        Double fRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( X, Y );
        MathSSEFn->MulPD();
        MathSSEFn->HAddD(0);
        MathSSEFn->Push( Z, 0.0 );
        MathSSEFn->Push( Z, 0.0 );
        MathSSEFn->MulPD();
        MathSSEFn->HAddD(0);
        MathSSEFn->AddPD();
        MathSSEFn->Pop( fRes );
        return fRes;
    #else
        return ( X*X + Y*Y + Z*Z );
    #endif
}

template<typename Real>
inline Real TVector3<Real>::Norm() const {
    return MathRealFn->Sqrt( NormSqr() );
}
template<typename Real>
inline Real TVector3<Real>::InvNormSqr() const {
    return MathRealFn->Invert( NormSqr() );
}
template<typename Real>
inline Real TVector3<Real>::InvNorm() const {
    return MathRealFn->InvSqrt( NormSqr() );
}

template<typename Real>
inline Real TVector3<Real>::Normalize() {
    const Real fZero = (Real)0;
    Real fNorm = Norm(), fInvNorm;
    if ( MathRealFn->EqualsZero(fNorm) ) {
        X = fZero;
        Y = fZero;
        Z = fZero;
        return fZero;
    } else {
        fInvNorm = MathRealFn->Invert(fNorm);
        X *= fInvNorm;
        Y *= fInvNorm;
        Z *= fInvNorm;
        return fNorm;
    }
}

template<typename Real>
inline TVector3<Real> TVector3<Real>::ProjectToNormal(const TVector3<Real> & vNormal) const {
    return ( vNormal * ( (*this) * vNormal ) );
}
template<typename Real>
inline TVector3<Real> TVector3<Real>::ProjectToPlane(const TVector3<Real> & vNormal) const {
    return ( (*this) - ProjectToNormal(vNormal) );
}

template<typename Real>
Void TVector3<Real>::OrthoNormalize( TVector3<Real> & vI, TVector3<Real> & vJ, TVector3<Real> & vK )
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
Void TVector3<Real>::MakeComplementBasis( TVector3<Real> & vI, TVector3<Real> & vJ, const TVector3<Real> & vK )
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

