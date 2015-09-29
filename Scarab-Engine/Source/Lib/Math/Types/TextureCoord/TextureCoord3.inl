/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/TextureCoord/TextureCoord3.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : 3D texture coordinates
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TTextureCoord3 implementation
template<typename Real>
const TTextureCoord3<Real> TTextureCoord3<Real>::Null = TTextureCoord3<Real>( (Real)0, (Real)0, (Real)0 );

template<typename Real>
TTextureCoord3<Real>::TTextureCoord3()
{
    // nothing to do
}
template<typename Real>
TTextureCoord3<Real>::TTextureCoord3(Real t)
{
    T = t;
    U = (Real)0;
    V = (Real)0;
}
template<typename Real>
TTextureCoord3<Real>::TTextureCoord3(Real t, Real u)
{
    T = t;
    U = u;
    V = (Real)0;
}
template<typename Real>
TTextureCoord3<Real>::TTextureCoord3(Real t, Real u, Real v)
{
    T = t;
    U = u;
    V = v;
}
template<typename Real>
TTextureCoord3<Real>::TTextureCoord3(const Real vArr[3])
{
    T = vArr[0];
    U = vArr[1];
    V = vArr[2];
}
template<typename Real>
TTextureCoord3<Real>::TTextureCoord3(const TVector2<Real> & rhs)
{
    T = rhs.X;
    U = rhs.Y;
    V = (Real)0;
}
template<typename Real>
TTextureCoord3<Real>::TTextureCoord3(const TVector3<Real> & rhs)
{
    T = rhs.X;
    U = rhs.Y;
    V = rhs.Z;
}
template<typename Real>
TTextureCoord3<Real>::TTextureCoord3(const TVector4<Real> & rhs)
{
    T = rhs.X;
    U = rhs.Y;
    V = rhs.Z;
}
template<typename Real>
TTextureCoord3<Real>::TTextureCoord3(const TTextureCoord1<Real> & rhs)
{
    T = rhs.T;
    U = (Real)0;
    V = (Real)0;
}
template<typename Real>
TTextureCoord3<Real>::TTextureCoord3(const TTextureCoord2<Real> & rhs)
{
    T = rhs.T;
    U = rhs.U;
    V = (Real)0;
}
template<typename Real>
TTextureCoord3<Real>::TTextureCoord3(const TTextureCoord3<Real> & rhs)
{
    T = rhs.T;
    U = rhs.U;
    V = rhs.V;
}
template<typename Real>
TTextureCoord3<Real>::TTextureCoord3(const TTextureCoord4<Real> & rhs)
{
    T = rhs.T;
    U = rhs.U;
    V = rhs.V;
}
template<typename Real>
TTextureCoord3<Real>::~TTextureCoord3()
{
    // nothing to do
}

template<typename Real>
inline TTextureCoord3<Real> & TTextureCoord3<Real>::operator=(const TTextureCoord3<Real> & rhs) {
    T = rhs.T;
    U = rhs.U;
    V = rhs.V;
    return (*this);
}

template<typename Real>
inline TTextureCoord3<Real>::operator Real*() const {
    return (Real*)this;
}
template<typename Real>
inline TTextureCoord3<Real>::operator const Real*() const {
    return (const Real*)this;
}

template<typename Real>
inline Real & TTextureCoord3<Real>::operator[](Int i) {
    return *( ((Real*)this) + i );
}
template<typename Real>
inline Real TTextureCoord3<Real>::operator[](Int i) const {
    return *( ((const Real*)this) + i );
}
template<typename Real>
inline Real & TTextureCoord3<Real>::operator[](UInt i) {
    return *( ((Real*)this) + i );
}
template<typename Real>
inline Real TTextureCoord3<Real>::operator[](UInt i) const {
    return *( ((const Real*)this) + i );
}

template<typename Real>
inline TTextureCoord3<Real> TTextureCoord3<Real>::operator+() const {
    return TTextureCoord3<Real>(T, U, V);
}
template<typename Real>
inline TTextureCoord3<Real> TTextureCoord3<Real>::operator-() const {
    return TTextureCoord3<Real>(-T, -U, -V);
}

template<typename Real>
inline Bool TTextureCoord3<Real>::operator==(const TTextureCoord3<Real> & rhs) const {
    return ( MathRealFn->Equals(T,rhs.T) && MathRealFn->Equals(U,rhs.U) &&
             MathRealFn->Equals(V,rhs.V) );
}
template<typename Real>
inline Bool TTextureCoord3<Real>::operator!=(const TTextureCoord3<Real> & rhs) const {
    return ( !(MathRealFn->Equals(T,rhs.T)) || !(MathRealFn->Equals(U,rhs.U)) ||
             !(MathRealFn->Equals(V,rhs.V)) );
}

template<typename Real>
inline TTextureCoord3<Real> TTextureCoord3<Real>::operator+(Real rhs) const {
    return TTextureCoord3<Real>( T + rhs, U + rhs, V + rhs );
}
template<>
inline TTextureCoord3<Float> TTextureCoord3<Float>::operator+(Float rhs) const {
    #ifdef MATH_USE_SSE
        TTextureCoord3<Float> vRes;
        MathSSEFn->Push( T, U, V );
        MathSSEFn->Push( rhs, rhs, rhs );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( vRes.T, vRes.U, vRes.V );
        return vRes;
    #else
        return TTextureCoord3<Float>( T + rhs, U + rhs, V + rhs );
    #endif
}
template<>
inline TTextureCoord3<Double> TTextureCoord3<Double>::operator+(Double rhs) const {
    #ifdef MATH_USE_SSE
        TTextureCoord3<Double> vRes;
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( vRes.T, vRes.U );
        MathSSEFn->Push( V );
        MathSSEFn->Push( rhs );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( vRes.V );
        return vRes;
    #else
        return TTextureCoord3<Double>( T + rhs, U + rhs, V + rhs );
    #endif
}
template<typename Real>
inline TTextureCoord3<Real> TTextureCoord3<Real>::operator-(Real rhs) const {
    return TTextureCoord3<Real>( T - rhs, U - rhs, V - rhs );
}
template<>
inline TTextureCoord3<Float> TTextureCoord3<Float>::operator-(Float rhs) const {
    #ifdef MATH_USE_SSE
        TTextureCoord3<Float> vRes;
        MathSSEFn->Push( T, U, V );
        MathSSEFn->Push( rhs, rhs, rhs );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( vRes.T, vRes.U, vRes.V );
        return vRes;
    #else
        return TTextureCoord3<Float>( T - rhs, U - rhs, V - rhs );
    #endif
}
template<>
inline TTextureCoord3<Double> TTextureCoord3<Double>::operator-(Double rhs) const {
    #ifdef MATH_USE_SSE
        TTextureCoord3<Double> vRes;
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( vRes.T, vRes.U );
        MathSSEFn->Push( V );
        MathSSEFn->Push( rhs );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( vRes.V );
        return vRes;
    #else
        return TTextureCoord3<Double>( T - rhs, U - rhs, V - rhs );
    #endif
}
template<typename Real>
inline TTextureCoord3<Real> TTextureCoord3<Real>::operator*(Real rhs) const {
    return TTextureCoord3<Real>( T * rhs, U * rhs, V * rhs );
}
template<>
inline TTextureCoord3<Float> TTextureCoord3<Float>::operator*(Float rhs) const {
    #ifdef MATH_USE_SSE
        TTextureCoord3<Float> vRes;
        MathSSEFn->Push( T, U, V );
        MathSSEFn->Push( rhs, rhs, rhs );
        MathSSEFn->MulPF();
        MathSSEFn->Pop( vRes.T, vRes.U, vRes.V );
        return vRes;
    #else
        return TTextureCoord3<Float>( T * rhs, U * rhs, V * rhs );
    #endif
}
template<>
inline TTextureCoord3<Double> TTextureCoord3<Double>::operator*(Double rhs) const {
    #ifdef MATH_USE_SSE
        TTextureCoord3<Double> vRes;
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( vRes.T, vRes.U );
        MathSSEFn->Push( V );
        MathSSEFn->Push( rhs );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( vRes.V );
        return vRes;
    #else
        return TTextureCoord3<Double>( T * rhs, U * rhs, V * rhs );
    #endif
}
template<typename Real>
inline TTextureCoord3<Real> TTextureCoord3<Real>::operator/(Real rhs) const {
    return TTextureCoord3<Real>( T / rhs, U / rhs, V / rhs );
}
template<>
inline TTextureCoord3<Float> TTextureCoord3<Float>::operator/(Float rhs) const {
    #ifdef MATH_USE_SSE
        TTextureCoord3<Float> vRes;
        MathSSEFn->Push( T, U, V );
        MathSSEFn->Push( rhs, rhs, rhs );
        MathSSEFn->DivPF();
        MathSSEFn->Pop( vRes.T, vRes.U, vRes.V );
        return vRes;
    #else
        return TTextureCoord3<Float>( T / rhs, U / rhs, V / rhs );
    #endif
}
template<>
inline TTextureCoord3<Double> TTextureCoord3<Double>::operator/(Double rhs) const {
    #ifdef MATH_USE_SSE
        TTextureCoord3<Double> vRes;
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( vRes.T, vRes.U );
        MathSSEFn->Push( V );
        MathSSEFn->Push( rhs );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( vRes.V );
        return vRes;
    #else
        return TTextureCoord3<Double>( T / rhs, U / rhs, V / rhs );
    #endif
}

template<typename Real>
inline TTextureCoord3<Real> & TTextureCoord3<Real>::operator+=(Real rhs) {
    T += rhs;
    U += rhs;
    V += rhs;
    return (*this);
}
template<>
inline TTextureCoord3<Float> & TTextureCoord3<Float>::operator+=(Float rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( T, U, V );
        MathSSEFn->Push( rhs, rhs, rhs );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( T, U, V );
    #else
        T += rhs;
        U += rhs;
        V += rhs;
    #endif
    return (*this);
}
template<>
inline TTextureCoord3<Double> & TTextureCoord3<Double>::operator+=(Double rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( T, U );
        MathSSEFn->Push( V );
        MathSSEFn->Push( rhs );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( V );
    #else
        T += rhs;
        U += rhs;
        V += rhs;
    #endif
    return (*this);
}
template<typename Real>
inline TTextureCoord3<Real> & TTextureCoord3<Real>::operator-=(Real rhs) {
    T -= rhs;
    U -= rhs;
    V -= rhs;
    return (*this);
}
template<>
inline TTextureCoord3<Float> & TTextureCoord3<Float>::operator-=(Float rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( T, U, V );
        MathSSEFn->Push( rhs, rhs, rhs );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( T, U, V );
    #else
        T -= rhs;
        U -= rhs;
        V -= rhs;
    #endif
    return (*this);
}
template<>
inline TTextureCoord3<Double> & TTextureCoord3<Double>::operator-=(Double rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( T, U );
        MathSSEFn->Push( V );
        MathSSEFn->Push( rhs );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( V );
    #else
        T -= rhs;
        U -= rhs;
        V -= rhs;
    #endif
    return (*this);
}
template<typename Real>
inline TTextureCoord3<Real> & TTextureCoord3<Real>::operator*=(Real rhs) {
    T *= rhs;
    U *= rhs;
    V *= rhs;
    return (*this);
}
template<>
inline TTextureCoord3<Float> & TTextureCoord3<Float>::operator*=(Float rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( T, U, V );
        MathSSEFn->Push( rhs, rhs, rhs );
        MathSSEFn->MulPF();
        MathSSEFn->Pop( T, U, V );
    #else
        T *= rhs;
        U *= rhs;
        V *= rhs;
    #endif
    return (*this);
}
template<>
inline TTextureCoord3<Double> & TTextureCoord3<Double>::operator*=(Double rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( T, U );
        MathSSEFn->Push( V );
        MathSSEFn->Push( rhs );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( V );
    #else
        T *= rhs;
        U *= rhs;
        V *= rhs;
    #endif
    return (*this);
}
template<typename Real>
inline TTextureCoord3<Real> & TTextureCoord3<Real>::operator/=(Real rhs) {
    T /= rhs;
    U /= rhs;
    V /= rhs;
    return (*this);
}
template<>
inline TTextureCoord3<Float> & TTextureCoord3<Float>::operator/=(Float rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( T, U, V );
        MathSSEFn->Push( rhs, rhs, rhs );
        MathSSEFn->DivPF();
        MathSSEFn->Pop( T, U, V );
    #else
        T /= rhs;
        U /= rhs;
        V /= rhs;
    #endif
    return (*this);
}
template<>
inline TTextureCoord3<Double> & TTextureCoord3<Double>::operator/=(Double rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( T, U );
        MathSSEFn->Push( V );
        MathSSEFn->Push( rhs );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( V );
    #else
        T /= rhs;
        U /= rhs;
        V /= rhs;
    #endif
    return (*this);
}

template<typename Real>
inline TTextureCoord3<Real> TTextureCoord3<Real>::operator+(const TTextureCoord3<Real> & rhs) const {
    return TTextureCoord3<Real>( T + rhs.T, U + rhs.U, V + rhs.V );
}
template<>
inline TTextureCoord3<Float> TTextureCoord3<Float>::operator+(const TTextureCoord3<Float> & rhs) const {
    #ifdef MATH_USE_SSE
        TTextureCoord3<Float> vRes;
        MathSSEFn->Push( T, U, V );
        MathSSEFn->Push( rhs.T, rhs.U, rhs.V );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( vRes.T, vRes.U, vRes.V );
        return vRes;
    #else
        return TTextureCoord3<Float>( T + rhs.T, U + rhs.U, V + rhs.V );
    #endif
}
template<>
inline TTextureCoord3<Double> TTextureCoord3<Double>::operator+(const TTextureCoord3<Double> & rhs) const {
    #ifdef MATH_USE_SSE
        TTextureCoord3<Double> vRes;
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs.T, rhs.U );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( vRes.T, vRes.U );
        MathSSEFn->Push( V );
        MathSSEFn->Push( rhs.V );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( vRes.V );
        return vRes;
    #else
        return TTextureCoord3<Double>( T + rhs.T, U + rhs.U, V + rhs.V );
    #endif
}
template<typename Real>
inline TTextureCoord3<Real> TTextureCoord3<Real>::operator-(const TTextureCoord3<Real> & rhs) const {
    return TTextureCoord3<Real>( T - rhs.T, U - rhs.U, V - rhs.V );
}
template<>
inline TTextureCoord3<Float> TTextureCoord3<Float>::operator-(const TTextureCoord3<Float> & rhs) const {
    #ifdef MATH_USE_SSE
        TTextureCoord3<Float> vRes;
        MathSSEFn->Push( T, U, V );
        MathSSEFn->Push( rhs.T, rhs.U, rhs.V );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( vRes.T, vRes.U, vRes.V );
        return vRes;
    #else
        return TTextureCoord3<Float>( T - rhs.T, U - rhs.U, V - rhs.V );
    #endif
}
template<>
inline TTextureCoord3<Double> TTextureCoord3<Double>::operator-(const TTextureCoord3<Double> & rhs) const {
    #ifdef MATH_USE_SSE
        TTextureCoord3<Double> vRes;
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs.T, rhs.U );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( vRes.T, vRes.U );
        MathSSEFn->Push( V );
        MathSSEFn->Push( rhs.V );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( vRes.V );
        return vRes;
    #else
        return TTextureCoord3<Double>( T - rhs.T, U - rhs.U, V - rhs.V );
    #endif
}

template<typename Real>
inline TTextureCoord3<Real> & TTextureCoord3<Real>::operator+=(const TTextureCoord3<Real> & rhs) {
    T += rhs.T;
    U += rhs.U;
    V += rhs.V;
    return (*this);
}
template<>
inline TTextureCoord3<Float> & TTextureCoord3<Float>::operator+=(const TTextureCoord3<Float> & rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( T, U, V );
        MathSSEFn->Push( rhs.T, rhs.U, rhs.V );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( T, U, V );
    #else
        T += rhs.T;
        U += rhs.U;
        V += rhs.V;
    #endif
    return (*this);
}
template<>
inline TTextureCoord3<Double> & TTextureCoord3<Double>::operator+=(const TTextureCoord3<Double> & rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs.T, rhs.U );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( T, U );
        MathSSEFn->Push( V );
        MathSSEFn->Push( rhs.V );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( V );
    #else
        T += rhs.T;
        U += rhs.U;
        V += rhs.V;
    #endif
    return (*this);
}
template<typename Real>
inline TTextureCoord3<Real> & TTextureCoord3<Real>::operator-=(const TTextureCoord3<Real> & rhs) {
    T -= rhs.T;
    U -= rhs.U;
    V -= rhs.V;
    return (*this);
}
template<>
inline TTextureCoord3<Float> & TTextureCoord3<Float>::operator-=(const TTextureCoord3<Float> & rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( T, U, V );
        MathSSEFn->Push( rhs.T, rhs.U, rhs.V );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( T, U, V );
    #else
        T -= rhs.T;
        U -= rhs.U;
        V -= rhs.V;
    #endif
    return (*this);
}
template<>
inline TTextureCoord3<Double> & TTextureCoord3<Double>::operator-=(const TTextureCoord3<Double> & rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs.T, rhs.U );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( T, U );
        MathSSEFn->Push( V );
        MathSSEFn->Push( rhs.V );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( V );
    #else
        T -= rhs.T;
        U -= rhs.U;
        V -= rhs.V;
    #endif
    return (*this);
}

template<typename Real>
inline Real TTextureCoord3<Real>::operator*(const TTextureCoord3<Real> & rhs) const {
    return ( (T * rhs.T) + (U * rhs.U) + (V * rhs.V) );
}
template<>
inline Float TTextureCoord3<Float>::operator*(const TTextureCoord3<Float> & rhs) const {
    #ifdef MATH_USE_SSE
        Float fRes;
        MathSSEFn->Push( T, U, V, 0.0f );
        MathSSEFn->Push( rhs.T, rhs.U, rhs.V, 0.0f );
        MathSSEFn->MulPF();
        MathSSEFn->HAddF(0);
        MathSSEFn->HAddF(0);
        MathSSEFn->Pop( fRes );
        return fRes;
    #else
        return ( (T * rhs.T) + (U * rhs.U) + (V * rhs.V) );
    #endif
}
template<>
inline Double TTextureCoord3<Double>::operator*(const TTextureCoord3<Double> & rhs) const {
    #ifdef MATH_USE_SSE
        Double fRes;
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs.T, rhs.U );
        MathSSEFn->MulPD();
        MathSSEFn->HAddD(0);
        MathSSEFn->Push( V, 0.0 );
        MathSSEFn->Push( rhs.V, 0.0 );
        MathSSEFn->MulPD();
        MathSSEFn->HAddD(0);
        MathSSEFn->AddPD();
        MathSSEFn->Pop( fRes );
        return fRes;
    #else
        return ( (T * rhs.T) + (U * rhs.U) + (V * rhs.V) );
    #endif
}

template<typename Real>
inline TTextureCoord3<Real> TTextureCoord3<Real>::operator^(const TTextureCoord3<Real> & rhs) const {
    return TTextureCoord3<Real> (
        (U * rhs.V) - (V * rhs.U),
        (V * rhs.T) - (T * rhs.V),
        (T * rhs.U) - (U * rhs.T)
    );
}
template<>
inline TTextureCoord3<Float> TTextureCoord3<Float>::operator^(const TTextureCoord3<Float> & rhs) const {
    #ifdef MATH_USE_SSE
        TTextureCoord3<Float> vRes;
        MathSSEFn->Push( U, V, T );
        MathSSEFn->Push( rhs.V, rhs.T, rhs.U );
        MathSSEFn->MulPF();
        MathSSEFn->Push( V, T, U );
        MathSSEFn->Push( rhs.U, rhs.V, rhs.T );
        MathSSEFn->MulPF();
        MathSSEFn->SubPF();
        MathSSEFn->Pop( vRes.T, vRes.U, vRes.V );
        return vRes;
    #else
        return TTextureCoord3<Float> (
            (U * rhs.V) - (V * rhs.U),
            (V * rhs.T) - (T * rhs.V),
            (T * rhs.U) - (U * rhs.T)
        );
    #endif
}
template<>
inline TTextureCoord3<Double> TTextureCoord3<Double>::operator^(const TTextureCoord3<Double> & rhs) const {
    #ifdef MATH_USE_SSE
        TTextureCoord3<Double> vRes;
        MathSSEFn->Push( U, V );
        MathSSEFn->Push( rhs.V, rhs.T );
        MathSSEFn->MulPD();
        MathSSEFn->Push( V, T );
        MathSSEFn->Push( rhs.U, rhs.V );
        MathSSEFn->MulPD();
        MathSSEFn->SubPD();
        MathSSEFn->Pop( vRes.T, vRes.U );
        MathSSEFn->Push( T );
        MathSSEFn->Push( rhs.U );
        MathSSEFn->MulPD();
        MathSSEFn->Push( U );
        MathSSEFn->Push( rhs.T );
        MathSSEFn->MulPD();
        MathSSEFn->SubPD();
        MathSSEFn->Pop( vRes.V );
        return vRes;
    #else
        return TTextureCoord3<Double> (
            (U * rhs.V) - (V * rhs.U),
            (V * rhs.T) - (T * rhs.V),
            (T * rhs.U) - (U * rhs.T)
        );
    #endif
}

template<typename Real>
inline Real TTextureCoord3<Real>::DistSqr() const {
    return ( T*T + U*U + V*V );
}
template<>
inline Float TTextureCoord3<Float>::DistSqr() const {
    #ifdef MATH_USE_SSE
        Float fRes;
        MathSSEFn->Push( T, U, V, 0.0f );
        MathSSEFn->Push( T, U, V, 0.0f );
        MathSSEFn->MulPF();
        MathSSEFn->HAddF(0);
        MathSSEFn->HAddF(0);
        MathSSEFn->Pop( fRes );
        return fRes;
    #else
        return ( T*T + U*U + V*V );
    #endif
}
template<>
inline Double TTextureCoord3<Double>::DistSqr() const {
    #ifdef MATH_USE_SSE
        Double fRes;
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( T, U );
        MathSSEFn->MulPD();
        MathSSEFn->HAddD(0);
        MathSSEFn->Push( V, 0.0 );
        MathSSEFn->Push( V, 0.0 );
        MathSSEFn->MulPD();
        MathSSEFn->HAddD(0);
        MathSSEFn->AddPD();
        MathSSEFn->Pop( fRes );
        return fRes;
    #else
        return ( T*T + U*U + V*V );
    #endif
}

template<typename Real>
inline Real TTextureCoord3<Real>::Dist() const {
    return MathRealFn->Sqrt( DistSqr() );
}
template<typename Real>
inline Real TTextureCoord3<Real>::InvDistSqr() const {
    return MathRealFn->Invert( DistSqr() );
}
template<typename Real>
inline Real TTextureCoord3<Real>::InvDist() const {
    return MathRealFn->InvSqrt( DistSqr() );
}

