/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/TextureCoord/TextureCoord2.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : 2D texture coordinates
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TTextureCoord2 implementation
template<typename Real>
const TTextureCoord2<Real> TTextureCoord2<Real>::Null = TTextureCoord2<Real>( (Real)0, (Real)0 );

template<typename Real>
TTextureCoord2<Real>::TTextureCoord2()
{
    // nothing to do
}
template<typename Real>
TTextureCoord2<Real>::TTextureCoord2(Real t)
{
    T = t;
    U = (Real)0;
}
template<typename Real>
TTextureCoord2<Real>::TTextureCoord2(Real t, Real u)
{
    T = t;
    U = u;
}
template<typename Real>
TTextureCoord2<Real>::TTextureCoord2(const Real vArr[2])
{
    T = vArr[0];
    U = vArr[1];
}
template<typename Real>
TTextureCoord2<Real>::TTextureCoord2(const TVector2<Real> & rhs)
{
    T = rhs.X;
    U = rhs.Y;
}
template<typename Real>
TTextureCoord2<Real>::TTextureCoord2(const TVector3<Real> & rhs)
{
    T = rhs.X;
    U = rhs.Y;
}
template<typename Real>
TTextureCoord2<Real>::TTextureCoord2(const TVector4<Real> & rhs)
{
    T = rhs.X;
    U = rhs.Y;
}
template<typename Real>
TTextureCoord2<Real>::TTextureCoord2(const TTextureCoord1<Real> & rhs)
{
    T = rhs.T;
    U = (Real)0;
}
template<typename Real>
TTextureCoord2<Real>::TTextureCoord2(const TTextureCoord2<Real> & rhs)
{
    T = rhs.T;
    U = rhs.U;
}
template<typename Real>
TTextureCoord2<Real>::TTextureCoord2(const TTextureCoord3<Real> & rhs)
{
    T = rhs.T;
    U = rhs.U;
}
template<typename Real>
TTextureCoord2<Real>::TTextureCoord2(const TTextureCoord4<Real> & rhs)
{
    T = rhs.T;
    U = rhs.U;
}
template<typename Real>
TTextureCoord2<Real>::~TTextureCoord2()
{
    // nothing to do
}

template<typename Real>
inline TTextureCoord2<Real> & TTextureCoord2<Real>::operator=(const TTextureCoord2<Real> & rhs) {
    T = rhs.T;
    U = rhs.U;
    return (*this);
}

template<typename Real>
inline TTextureCoord2<Real>::operator Real*() const {
    return (Real*)this;
}
template<typename Real>
inline TTextureCoord2<Real>::operator const Real*() const {
    return (const Real*)this;
}

template<typename Real>
inline Real & TTextureCoord2<Real>::operator[](Int i) {
    return *( ((Real*)this) + i );
}
template<typename Real>
inline Real TTextureCoord2<Real>::operator[](Int i) const {
    return *( ((const Real*)this) + i );
}
template<typename Real>
inline Real & TTextureCoord2<Real>::operator[](UInt i) {
    return *( ((Real*)this) + i );
}
template<typename Real>
inline Real TTextureCoord2<Real>::operator[](UInt i) const {
    return *( ((const Real*)this) + i );
}

template<typename Real>
inline TTextureCoord2<Real> TTextureCoord2<Real>::operator+() const {
    return TTextureCoord2<Real>(T, U);
}
template<typename Real>
inline TTextureCoord2<Real> TTextureCoord2<Real>::operator-() const {
    return TTextureCoord2<Real>(-T, -U);
}

template<typename Real>
inline Bool TTextureCoord2<Real>::operator==(const TTextureCoord2<Real> & rhs) const {
    return ( MathRealFn->Equals(T,rhs.T) && MathRealFn->Equals(U,rhs.U) );
}
template<typename Real>
inline Bool TTextureCoord2<Real>::operator!=(const TTextureCoord2<Real> & rhs) const {
    return ( !(MathRealFn->Equals(T,rhs.T)) || !(MathRealFn->Equals(U,rhs.U)) );
}

template<typename Real>
inline TTextureCoord2<Real> TTextureCoord2<Real>::operator+(Real rhs) const {
    return TTextureCoord2<Real>( T + rhs, U + rhs );
}
template<>
inline TTextureCoord2<Float> TTextureCoord2<Float>::operator+(Float rhs) const {
    #ifdef MATH_USE_SSE
        TTextureCoord2<Float> vRes;
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( vRes.T, vRes.U );
        return vRes;
    #else
        return TTextureCoord2<Float>( T + rhs, U + rhs );
    #endif
}
template<>
inline TTextureCoord2<Double> TTextureCoord2<Double>::operator+(Double rhs) const {
    #ifdef MATH_USE_SSE
        TTextureCoord2<Double> vRes;
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( vRes.T, vRes.U );
        return vRes;
    #else
        return TTextureCoord2<Double>( T + rhs, U + rhs );
    #endif
}
template<typename Real>
inline TTextureCoord2<Real> TTextureCoord2<Real>::operator-(Real rhs) const {
    return TTextureCoord2<Real>( T - rhs, U - rhs );
}
template<>
inline TTextureCoord2<Float> TTextureCoord2<Float>::operator-(Float rhs) const {
    #ifdef MATH_USE_SSE
        TTextureCoord2<Float> vRes;
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( vRes.T, vRes.U );
        return vRes;
    #else
        return TTextureCoord2<Float>( T - rhs, U - rhs );
    #endif
}
template<>
inline TTextureCoord2<Double> TTextureCoord2<Double>::operator-(Double rhs) const {
    #ifdef MATH_USE_SSE
        TTextureCoord2<Double> vRes;
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( vRes.T, vRes.U );
        return vRes;
    #else
        return TTextureCoord2<Double>( T - rhs, U - rhs );
    #endif
}
template<typename Real>
inline TTextureCoord2<Real> TTextureCoord2<Real>::operator*(Real rhs) const {
    return TTextureCoord2<Real>( T * rhs, U * rhs );
}
template<>
inline TTextureCoord2<Float> TTextureCoord2<Float>::operator*(Float rhs) const {
    #ifdef MATH_USE_SSE
        TTextureCoord2<Float> vRes;
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->MulPF();
        MathSSEFn->Pop( vRes.T, vRes.U );
        return vRes;
    #else
        return TTextureCoord2<Float>( T * rhs, U * rhs );
    #endif
}
template<>
inline TTextureCoord2<Double> TTextureCoord2<Double>::operator*(Double rhs) const {
    #ifdef MATH_USE_SSE
        TTextureCoord2<Double> vRes;
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( vRes.T, vRes.U );
        return vRes;
    #else
        return TTextureCoord2<Double>( T * rhs, U * rhs );
    #endif
}
template<typename Real>
inline TTextureCoord2<Real> TTextureCoord2<Real>::operator/(Real rhs) const {
    return TTextureCoord2<Real>( T / rhs, U / rhs );
}
template<>
inline TTextureCoord2<Float> TTextureCoord2<Float>::operator/(Float rhs) const {
    #ifdef MATH_USE_SSE
        TTextureCoord2<Float> vRes;
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->DivPF();
        MathSSEFn->Pop( vRes.T, vRes.U );
        return vRes;
    #else
        return TTextureCoord2<Float>( T / rhs, U / rhs );
    #endif
}
template<>
inline TTextureCoord2<Double> TTextureCoord2<Double>::operator/(Double rhs) const {
    #ifdef MATH_USE_SSE
        TTextureCoord2<Double> vRes;
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( vRes.T, vRes.U );
        return vRes;
    #else
        return TTextureCoord2<Double>( T / rhs, U / rhs );
    #endif
}

template<typename Real>
inline TTextureCoord2<Real> & TTextureCoord2<Real>::operator+=(Real rhs) {
    T += rhs;
    U += rhs;
    return (*this);
}
template<>
inline TTextureCoord2<Float> & TTextureCoord2<Float>::operator+=(Float rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( T, U );
    #else
        T += rhs;
        U += rhs;
    #endif
    return (*this);
}
template<>
inline TTextureCoord2<Double> & TTextureCoord2<Double>::operator+=(Double rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( T, U );
    #else
        T += rhs;
        U += rhs;
    #endif
    return (*this);
}
template<typename Real>
inline TTextureCoord2<Real> & TTextureCoord2<Real>::operator-=(Real rhs) {
    T -= rhs;
    U -= rhs;
    return (*this);
}
template<>
inline TTextureCoord2<Float> & TTextureCoord2<Float>::operator-=(Float rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( T, U );
    #else
        T -= rhs;
        U -= rhs;
    #endif
    return (*this);
}
template<>
inline TTextureCoord2<Double> & TTextureCoord2<Double>::operator-=(Double rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( T, U );
    #else
        T -= rhs;
        U -= rhs;
    #endif
    return (*this);
}
template<typename Real>
inline TTextureCoord2<Real> & TTextureCoord2<Real>::operator*=(Real rhs) {
    T *= rhs;
    U *= rhs;
    return (*this);
}
template<>
inline TTextureCoord2<Float> & TTextureCoord2<Float>::operator*=(Float rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->MulPF();
        MathSSEFn->Pop( T, U );
    #else
        T *= rhs;
        U *= rhs;
    #endif
    return (*this);
}
template<>
inline TTextureCoord2<Double> & TTextureCoord2<Double>::operator*=(Double rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( T, U );
    #else
        T *= rhs;
        U *= rhs;
    #endif
    return (*this);
}
template<typename Real>
inline TTextureCoord2<Real> & TTextureCoord2<Real>::operator/=(Real rhs) {
    T /= rhs;
    U /= rhs;
    return (*this);
}
template<>
inline TTextureCoord2<Float> & TTextureCoord2<Float>::operator/=(Float rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->DivPF();
        MathSSEFn->Pop( T, U );
    #else
        T /= rhs;
        U /= rhs;
    #endif
    return (*this);
}
template<>
inline TTextureCoord2<Double> & TTextureCoord2<Double>::operator/=(Double rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( T, U );
    #else
        T /= rhs;
        U /= rhs;
    #endif
    return (*this);
}

template<typename Real>
inline TTextureCoord2<Real> TTextureCoord2<Real>::operator+(const TTextureCoord2<Real> & rhs) const {
    return TTextureCoord2<Real>( T + rhs.T, U + rhs.U );
}
template<>
inline TTextureCoord2<Float> TTextureCoord2<Float>::operator+(const TTextureCoord2<Float> & rhs) const {
    #ifdef MATH_USE_SSE
        TTextureCoord2<Float> vRes;
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs.T, rhs.U );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( vRes.T, vRes.U );
        return vRes;
    #else
        return TTextureCoord2<Float>( T + rhs.T, U + rhs.U );
    #endif
}
template<>
inline TTextureCoord2<Double> TTextureCoord2<Double>::operator+(const TTextureCoord2<Double> & rhs) const {
    #ifdef MATH_USE_SSE
        TTextureCoord2<Double> vRes;
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs.T, rhs.U );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( vRes.T, vRes.U );
        return vRes;
    #else
        return TTextureCoord2<Double>( T + rhs.T, U + rhs.U );
    #endif
}
template<typename Real>
inline TTextureCoord2<Real> TTextureCoord2<Real>::operator-(const TTextureCoord2<Real> & rhs) const {
    return TTextureCoord2<Real>( T - rhs.T, U - rhs.U );
}
template<>
inline TTextureCoord2<Float> TTextureCoord2<Float>::operator-(const TTextureCoord2<Float> & rhs) const {
    #ifdef MATH_USE_SSE
        TTextureCoord2<Float> vRes;
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs.T, rhs.U );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( vRes.T, vRes.U );
        return vRes;
    #else
        return TTextureCoord2<Float>( T - rhs.T, U - rhs.U );
    #endif
}
template<>
inline TTextureCoord2<Double> TTextureCoord2<Double>::operator-(const TTextureCoord2<Double> & rhs) const {
    #ifdef MATH_USE_SSE
        TTextureCoord2<Double> vRes;
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs.T, rhs.U );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( vRes.T, vRes.U );
        return vRes;
    #else
        return TTextureCoord2<Double>( T - rhs.T, U - rhs.U );
    #endif
}

template<typename Real>
inline TTextureCoord2<Real> & TTextureCoord2<Real>::operator+=(const TTextureCoord2<Real> & rhs) {
    T += rhs.T;
    U += rhs.U;
    return (*this);
}
template<>
inline TTextureCoord2<Float> & TTextureCoord2<Float>::operator+=(const TTextureCoord2<Float> & rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs.T, rhs.U );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( T, U );
    #else
        T += rhs.T;
        U += rhs.U;
    #endif
    return (*this);
}
template<>
inline TTextureCoord2<Double> & TTextureCoord2<Double>::operator+=(const TTextureCoord2<Double> & rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs.T, rhs.U );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( T, U );
    #else
        T += rhs.T;
        U += rhs.U;
    #endif
    return (*this);
}
template<typename Real>
inline TTextureCoord2<Real> & TTextureCoord2<Real>::operator-=(const TTextureCoord2<Real> & rhs) {
    T -= rhs.T;
    U -= rhs.U;
    return (*this);
}
template<>
inline TTextureCoord2<Float> & TTextureCoord2<Float>::operator-=(const TTextureCoord2<Float> & rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs.T, rhs.U );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( T, U );
    #else
        T -= rhs.T;
        U -= rhs.U;
    #endif
    return (*this);
}
template<>
inline TTextureCoord2<Double> & TTextureCoord2<Double>::operator-=(const TTextureCoord2<Double> & rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs.T, rhs.U );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( T, U );
    #else
        T -= rhs.T;
        U -= rhs.U;
    #endif
    return (*this);
}

template<typename Real>
inline Real TTextureCoord2<Real>::operator*(const TTextureCoord2<Real> & rhs) const {
    return ( (T * rhs.T) + (U * rhs.U) );
}
template<>
inline Float TTextureCoord2<Float>::operator*(const TTextureCoord2<Float> & rhs) const {
    #ifdef MATH_USE_SSE
        Float fRes;
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs.T, rhs.U );
        MathSSEFn->MulPF();
        MathSSEFn->HAddF(0);
        MathSSEFn->Pop( fRes );
        return fRes;
    #else
        return ( (T * rhs.T) + (U * rhs.U) );
    #endif
}
template<>
inline Double TTextureCoord2<Double>::operator*(const TTextureCoord2<Double> & rhs) const {
    #ifdef MATH_USE_SSE
        Double fRes;
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs.T, rhs.U );
        MathSSEFn->MulPD();
        MathSSEFn->HAddD(0);
        MathSSEFn->Pop( fRes );
        return fRes;
    #else
        return ( (T * rhs.T) + (U * rhs.U) );
    #endif
}

template<typename Real>
inline Real TTextureCoord2<Real>::DistSqr() const {
    return ( T*T + U*U );
}
template<>
inline Float TTextureCoord2<Float>::DistSqr() const {
    #ifdef MATH_USE_SSE
        Float fRes;
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( T, U );
        MathSSEFn->MulPF();
        MathSSEFn->HAddF(0);
        MathSSEFn->Pop( fRes );
        return fRes;
    #else
        return ( T*T + U*U );
    #endif
}
template<>
inline Double TTextureCoord2<Double>::DistSqr() const {
    #ifdef MATH_USE_SSE
        Double fRes;
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( T, U );
        MathSSEFn->MulPD();
        MathSSEFn->HAddD(0);
        MathSSEFn->Pop( fRes );
        return fRes;
    #else
        return ( T*T + U*U );
    #endif
}

template<typename Real>
inline Real TTextureCoord2<Real>::Dist() const {
    return MathRealFn->Sqrt( DistSqr() );
}
template<typename Real>
inline Real TTextureCoord2<Real>::InvDistSqr() const {
    return MathRealFn->Invert( DistSqr() );
}
template<typename Real>
inline Real TTextureCoord2<Real>::InvDist() const {
    return MathRealFn->InvSqrt( DistSqr() );
}
