/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/TextureCoord/TextureCoord4.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : 4D texture coordinates
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TTextureCoord4 implementation
template<typename Real>
const TTextureCoord4<Real> TTextureCoord4<Real>::Null = TTextureCoord4<Real>( (Real)0, (Real)0, (Real)0, (Real)0 );

template<typename Real>
TTextureCoord4<Real>::TTextureCoord4()
{
    // nothing to do
}
template<typename Real>
TTextureCoord4<Real>::TTextureCoord4(Real t)
{
    T = t;
    U = (Real)0;
    V = (Real)0;
    W = (Real)0;
}
template<typename Real>
TTextureCoord4<Real>::TTextureCoord4(Real t, Real u)
{
    T = t;
    U = u;
    V = (Real)0;
    W = (Real)0;
}
template<typename Real>
TTextureCoord4<Real>::TTextureCoord4(Real t, Real u, Real v)
{
    T = t;
    U = u;
    V = v;
    W = (Real)0;
}
template<typename Real>
TTextureCoord4<Real>::TTextureCoord4(Real t, Real u, Real v, Real w)
{
    T = t;
    U = u;
    V = v;
    W = w;
}
template<typename Real>
TTextureCoord4<Real>::TTextureCoord4(const Real vArr[4])
{
    T = vArr[0];
    U = vArr[1];
    V = vArr[2];
    W = vArr[3];
}
template<typename Real>
TTextureCoord4<Real>::TTextureCoord4(const TVector2<Real> & rhs)
{
    T = rhs.X;
    U = rhs.Y;
    V = (Real)0;
    W = (Real)0;
}
template<typename Real>
TTextureCoord4<Real>::TTextureCoord4(const TVector3<Real> & rhs)
{
    T = rhs.X;
    U = rhs.Y;
    V = rhs.Z;
    W = (Real)0;
}
template<typename Real>
TTextureCoord4<Real>::TTextureCoord4(const TVector4<Real> & rhs)
{
    T = rhs.X;
    U = rhs.Y;
    V = rhs.Z;
    W = rhs.W;
}
template<typename Real>
TTextureCoord4<Real>::TTextureCoord4(const TTextureCoord1<Real> & rhs)
{
    T = rhs.T;
    U = (Real)0;
    V = (Real)0;
    W = (Real)0;
}
template<typename Real>
TTextureCoord4<Real>::TTextureCoord4(const TTextureCoord2<Real> & rhs)
{
    T = rhs.T;
    U = rhs.U;
    V = (Real)0;
    W = (Real)0;
}
template<typename Real>
TTextureCoord4<Real>::TTextureCoord4(const TTextureCoord3<Real> & rhs)
{
    T = rhs.T;
    U = rhs.U;
    V = rhs.V;
    W = (Real)0;
}
template<typename Real>
TTextureCoord4<Real>::TTextureCoord4(const TTextureCoord4<Real> & rhs)
{
    T = rhs.T;
    U = rhs.U;
    V = rhs.V;
    W = rhs.W;
}
template<typename Real>
TTextureCoord4<Real>::~TTextureCoord4()
{
    // nothing to do
}

template<typename Real>
inline TTextureCoord4<Real> & TTextureCoord4<Real>::operator=(const TTextureCoord4<Real> & rhs) {
    T = rhs.T;
    U = rhs.U;
    V = rhs.V;
    W = rhs.W;
    return (*this);
}

template<typename Real>
inline TTextureCoord4<Real>::operator Real*() const {
    return (Real*)this;
}
template<typename Real>
inline TTextureCoord4<Real>::operator const Real*() const {
    return (const Real*)this;
}

template<typename Real>
inline Real & TTextureCoord4<Real>::operator[](Int i) {
    return *( ((Real*)this) + i );
}
template<typename Real>
inline Real TTextureCoord4<Real>::operator[](Int i) const {
    return *( ((const Real*)this) + i );
}
template<typename Real>
inline Real & TTextureCoord4<Real>::operator[](UInt i) {
    return *( ((Real*)this) + i );
}
template<typename Real>
inline Real TTextureCoord4<Real>::operator[](UInt i) const {
    return *( ((const Real*)this) + i );
}

template<typename Real>
inline TTextureCoord4<Real> TTextureCoord4<Real>::operator+() const {
    return TTextureCoord4<Real>(T, U, V, W);
}
template<typename Real>
inline TTextureCoord4<Real> TTextureCoord4<Real>::operator-() const {
    return TTextureCoord4<Real>(-T, -U, -V, -W);
}

template<typename Real>
inline Bool TTextureCoord4<Real>::operator==(const TTextureCoord4<Real> & rhs) const {
    return ( MathRealFn->Equals(T,rhs.T) && MathRealFn->Equals(U,rhs.U) &&
             MathRealFn->Equals(V,rhs.V) && MathRealFn->Equals(W,rhs.W) );
}
template<typename Real>
inline Bool TTextureCoord4<Real>::operator!=(const TTextureCoord4<Real> & rhs) const {
    return ( !(MathRealFn->Equals(T,rhs.T)) || !(MathRealFn->Equals(U,rhs.U)) ||
             !(MathRealFn->Equals(V,rhs.V)) || !(MathRealFn->Equals(W,rhs.W)) );
}

template<typename Real>
inline TTextureCoord4<Real> TTextureCoord4<Real>::operator+(Real rhs) const {
    return TTextureCoord4<Real>( T + rhs, U + rhs, V + rhs, W + rhs );
}
template<>
inline TTextureCoord4<Float> TTextureCoord4<Float>::operator+(Float rhs) const {
    #ifdef MATH_USE_SSE
        TTextureCoord4<Float> vRes;
        MathSSEFn->Push( T, U, V, W );
        MathSSEFn->Push( rhs, rhs, rhs, rhs );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( vRes.T, vRes.U, vRes.V, vRes.W );
        return vRes;
    #else
        return TTextureCoord4<Float>( T + rhs, U + rhs, V + rhs, W + rhs );
    #endif
}
template<>
inline TTextureCoord4<Double> TTextureCoord4<Double>::operator+(Double rhs) const {
    #ifdef MATH_USE_SSE
        TTextureCoord4<Double> vRes;
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( vRes.T, vRes.U );
        MathSSEFn->Push( V, W );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( vRes.V, vRes.W );
        return vRes;
    #else
        return TTextureCoord4<Double>( T + rhs, U + rhs, V + rhs, W + rhs );
    #endif
}
template<typename Real>
inline TTextureCoord4<Real> TTextureCoord4<Real>::operator-(Real rhs) const {
    return TTextureCoord4<Real>( T - rhs, U - rhs, V - rhs, W - rhs );
}
template<>
inline TTextureCoord4<Float> TTextureCoord4<Float>::operator-(Float rhs) const {
    #ifdef MATH_USE_SSE
        TTextureCoord4<Float> vRes;
        MathSSEFn->Push( T, U, V, W );
        MathSSEFn->Push( rhs, rhs, rhs, rhs );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( vRes.T, vRes.U, vRes.V, vRes.W );
        return vRes;
    #else
        return TTextureCoord4<Float>( T - rhs, U - rhs, V - rhs, W - rhs );
    #endif
}
template<>
inline TTextureCoord4<Double> TTextureCoord4<Double>::operator-(Double rhs) const {
    #ifdef MATH_USE_SSE
        TTextureCoord4<Double> vRes;
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( vRes.T, vRes.U );
        MathSSEFn->Push( V, W );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( vRes.V, vRes.W );
        return vRes;
    #else
        return TTextureCoord4<Double>( T - rhs, U - rhs, V - rhs, W - rhs );
    #endif
}
template<typename Real>
inline TTextureCoord4<Real> TTextureCoord4<Real>::operator*(Real rhs) const {
    return TTextureCoord4<Real>( T * rhs, U * rhs, V * rhs, W * rhs );
}
template<>
inline TTextureCoord4<Float> TTextureCoord4<Float>::operator*(Float rhs) const {
    #ifdef MATH_USE_SSE
        TTextureCoord4<Float> vRes;
        MathSSEFn->Push( T, U, V, W );
        MathSSEFn->Push( rhs, rhs, rhs, rhs );
        MathSSEFn->MulPF();
        MathSSEFn->Pop( vRes.T, vRes.U, vRes.V, vRes.W );
        return vRes;
    #else
        return TTextureCoord4<Float>( T * rhs, U * rhs, V * rhs, W * rhs );
    #endif
}
template<>
inline TTextureCoord4<Double> TTextureCoord4<Double>::operator*(Double rhs) const {
    #ifdef MATH_USE_SSE
        TTextureCoord4<Double> vRes;
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( vRes.T, vRes.U );
        MathSSEFn->Push( V, W );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( vRes.V, vRes.W );
        return vRes;
    #else
        return TTextureCoord4<Double>( T * rhs, U * rhs, V * rhs, W * rhs );
    #endif
}
template<typename Real>
inline TTextureCoord4<Real> TTextureCoord4<Real>::operator/(Real rhs) const {
    return TTextureCoord4<Real>( T / rhs, U / rhs, V / rhs, W / rhs );
}
template<>
inline TTextureCoord4<Float> TTextureCoord4<Float>::operator/(Float rhs) const {
    #ifdef MATH_USE_SSE
        TTextureCoord4<Float> vRes;
        MathSSEFn->Push( T, U, V, W );
        MathSSEFn->Push( rhs, rhs, rhs, rhs );
        MathSSEFn->DivPF();
        MathSSEFn->Pop( vRes.T, vRes.U, vRes.V, vRes.W );
        return vRes;
    #else
        return TTextureCoord4<Float>( T / rhs, U / rhs, V / rhs, W / rhs );
    #endif
}
template<>
inline TTextureCoord4<Double> TTextureCoord4<Double>::operator/(Double rhs) const {
    #ifdef MATH_USE_SSE
        TTextureCoord4<Double> vRes;
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( vRes.T, vRes.U );
        MathSSEFn->Push( V, W );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( vRes.V, vRes.W );
        return vRes;
    #else
        return TTextureCoord4<Double>( T / rhs, U / rhs, V / rhs, W / rhs );
    #endif
}

template<typename Real>
inline TTextureCoord4<Real> & TTextureCoord4<Real>::operator+=(Real rhs) {
    T += rhs;
    U += rhs;
    V += rhs;
    W += rhs;
    return (*this);
}
template<>
inline TTextureCoord4<Float> & TTextureCoord4<Float>::operator+=(Float rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( T, U, V, W );
        MathSSEFn->Push( rhs, rhs, rhs, rhs );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( T, U, V, W );
    #else
        T += rhs;
        U += rhs;
        V += rhs;
        W += rhs;
    #endif
    return (*this);
}
template<>
inline TTextureCoord4<Double> & TTextureCoord4<Double>::operator+=(Double rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( T, U );
        MathSSEFn->Push( V, W );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( V, W );
    #else
        T += rhs;
        U += rhs;
        V += rhs;
        W += rhs;
    #endif
    return (*this);
}
template<typename Real>
inline TTextureCoord4<Real> & TTextureCoord4<Real>::operator-=(Real rhs) {
    T -= rhs;
    U -= rhs;
    V -= rhs;
    W -= rhs;
    return (*this);
}
template<>
inline TTextureCoord4<Float> & TTextureCoord4<Float>::operator-=(Float rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( T, U, V, W );
        MathSSEFn->Push( rhs, rhs, rhs, rhs );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( T, U, V, W );
    #else
        T -= rhs;
        U -= rhs;
        V -= rhs;
        W -= rhs;
    #endif
    return (*this);
}
template<>
inline TTextureCoord4<Double> & TTextureCoord4<Double>::operator-=(Double rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( T, U );
        MathSSEFn->Push( V, W );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( V, W );
    #else
        T -= rhs;
        U -= rhs;
        V -= rhs;
        W -= rhs;
    #endif
    return (*this);
}
template<typename Real>
inline TTextureCoord4<Real> & TTextureCoord4<Real>::operator*=(Real rhs) {
    T *= rhs;
    U *= rhs;
    V *= rhs;
    W *= rhs;
    return (*this);
}
template<>
inline TTextureCoord4<Float> & TTextureCoord4<Float>::operator*=(Float rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( T, U, V, W );
        MathSSEFn->Push( rhs, rhs, rhs, rhs );
        MathSSEFn->MulPF();
        MathSSEFn->Pop( T, U, V, W );
    #else
        T *= rhs;
        U *= rhs;
        V *= rhs;
        W *= rhs;
    #endif
    return (*this);
}
template<>
inline TTextureCoord4<Double> & TTextureCoord4<Double>::operator*=(Double rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( T, U );
        MathSSEFn->Push( V, W );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( V, W );
    #else
        T *= rhs;
        U *= rhs;
        V *= rhs;
        W *= rhs;
    #endif
    return (*this);
}
template<typename Real>
inline TTextureCoord4<Real> & TTextureCoord4<Real>::operator/=(Real rhs) {
    T /= rhs;
    U /= rhs;
    V /= rhs;
    W /= rhs;
    return (*this);
}
template<>
inline TTextureCoord4<Float> & TTextureCoord4<Float>::operator/=(Float rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( T, U, V, W );
        MathSSEFn->Push( rhs, rhs, rhs, rhs );
        MathSSEFn->DivPF();
        MathSSEFn->Pop( T, U, V, W );
    #else
        T /= rhs;
        U /= rhs;
        V /= rhs;
        W /= rhs;
    #endif
    return (*this);
}
template<>
inline TTextureCoord4<Double> & TTextureCoord4<Double>::operator/=(Double rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( T, U );
        MathSSEFn->Push( V, W );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( V, W );
    #else
        T /= rhs;
        U /= rhs;
        V /= rhs;
        W /= rhs;
    #endif
    return (*this);
}

template<typename Real>
inline TTextureCoord4<Real> TTextureCoord4<Real>::operator+(const TTextureCoord4<Real> & rhs) const {
    return TTextureCoord4<Real>( T + rhs.T, U + rhs.U, V + rhs.V, W + rhs.W );
}
template<>
inline TTextureCoord4<Float> TTextureCoord4<Float>::operator+(const TTextureCoord4<Float> & rhs) const {
    #ifdef MATH_USE_SSE
        TTextureCoord4<Float> vRes;
        MathSSEFn->Push( T, U, V, W );
        MathSSEFn->Push( rhs.T, rhs.U, rhs.V, rhs.W );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( vRes.T, vRes.U, vRes.V, vRes.W );
        return vRes;
    #else
        return TTextureCoord4<Float>( T + rhs.T, U + rhs.U, V + rhs.V, W + rhs.W );
    #endif
}
template<>
inline TTextureCoord4<Double> TTextureCoord4<Double>::operator+(const TTextureCoord4<Double> & rhs) const {
    #ifdef MATH_USE_SSE
        TTextureCoord4<Double> vRes;
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs.T, rhs.U );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( vRes.T, vRes.U );
        MathSSEFn->Push( V, W );
        MathSSEFn->Push( rhs.V, rhs.W );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( vRes.V, vRes.W );
        return vRes;
    #else
        return TTextureCoord4<Double>( T + rhs.T, U + rhs.U, V + rhs.V, W + rhs.W );
    #endif
}
template<typename Real>
inline TTextureCoord4<Real> TTextureCoord4<Real>::operator-(const TTextureCoord4<Real> & rhs) const {
    return TTextureCoord4<Real>( T - rhs.T, U - rhs.U, V - rhs.V, W - rhs.W );
}
template<>
inline TTextureCoord4<Float> TTextureCoord4<Float>::operator-(const TTextureCoord4<Float> & rhs) const {
    #ifdef MATH_USE_SSE
        TTextureCoord4<Float> vRes;
        MathSSEFn->Push( T, U, V, W );
        MathSSEFn->Push( rhs.T, rhs.U, rhs.V, rhs.W );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( vRes.T, vRes.U, vRes.V, vRes.W );
        return vRes;
    #else
        return TTextureCoord4<Float>( T - rhs.T, U - rhs.U, V - rhs.V, W - rhs.W );
    #endif
}
template<>
inline TTextureCoord4<Double> TTextureCoord4<Double>::operator-(const TTextureCoord4<Double> & rhs) const {
    #ifdef MATH_USE_SSE
        TTextureCoord4<Double> vRes;
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs.T, rhs.U );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( vRes.T, vRes.U );
        MathSSEFn->Push( V, W );
        MathSSEFn->Push( rhs.V, rhs.W );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( vRes.V, vRes.W );
        return vRes;
    #else
        return TTextureCoord4<Double>( T - rhs.T, U - rhs.U, V - rhs.V, W - rhs.W );
    #endif
}

template<typename Real>
inline TTextureCoord4<Real> & TTextureCoord4<Real>::operator+=(const TTextureCoord4<Real> & rhs) {
    T += rhs.T;
    U += rhs.U;
    V += rhs.V;
    W += rhs.W;
    return (*this);
}
template<>
inline TTextureCoord4<Float> & TTextureCoord4<Float>::operator+=(const TTextureCoord4<Float> & rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( T, U, V, W );
        MathSSEFn->Push( rhs.T, rhs.U, rhs.V, rhs.W );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( T, U, V, W );
    #else
        T += rhs.T;
        U += rhs.U;
        V += rhs.V;
        W += rhs.W;
    #endif
    return (*this);
}
template<>
inline TTextureCoord4<Double> & TTextureCoord4<Double>::operator+=(const TTextureCoord4<Double> & rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs.T, rhs.U );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( T, U );
        MathSSEFn->Push( V, W );
        MathSSEFn->Push( rhs.V, rhs.W );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( V, W );
    #else
        T += rhs.T;
        U += rhs.U;
        V += rhs.V;
        W += rhs.W;
    #endif
    return (*this);
}
template<typename Real>
inline TTextureCoord4<Real> & TTextureCoord4<Real>::operator-=(const TTextureCoord4<Real> & rhs) {
    T -= rhs.T;
    U -= rhs.U;
    V -= rhs.V;
    W -= rhs.W;
    return (*this);
}
template<>
inline TTextureCoord4<Float> & TTextureCoord4<Float>::operator-=(const TTextureCoord4<Float> & rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( T, U, V, W );
        MathSSEFn->Push( rhs.T, rhs.U, rhs.V, rhs.W );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( T, U, V, W );
    #else
        T -= rhs.T;
        U -= rhs.U;
        V -= rhs.V;
        W -= rhs.W;
    #endif
    return (*this);
}
template<>
inline TTextureCoord4<Double> & TTextureCoord4<Double>::operator-=(const TTextureCoord4<Double> & rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs.T, rhs.U );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( T, U );
        MathSSEFn->Push( V, W );
        MathSSEFn->Push( rhs.V, rhs.W );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( V, W );
    #else
        T -= rhs.T;
        U -= rhs.U;
        V -= rhs.V;
        W -= rhs.W;
    #endif
    return (*this);
}

template<typename Real>
inline Real TTextureCoord4<Real>::operator*(const TTextureCoord4<Real> & rhs) const {
    return ( (T * rhs.T) + (U * rhs.U) + (V * rhs.V) + (W * rhs.W) );
}
template<>
inline Float TTextureCoord4<Float>::operator*(const TTextureCoord4<Float> & rhs) const {
    #ifdef MATH_USE_SSE
        Float fRes;
        MathSSEFn->Push( T, U, V, W );
        MathSSEFn->Push( rhs.T, rhs.U, rhs.V, rhs.W );
        MathSSEFn->MulPF();
        MathSSEFn->HAddF(0);
        MathSSEFn->HAddF(0);
        MathSSEFn->Pop( fRes );
        return fRes;
    #else
        return ( (T * rhs.T) + (U * rhs.U) + (V * rhs.V) + (W * rhs.W) );
    #endif
}
template<>
inline Double TTextureCoord4<Double>::operator*(const TTextureCoord4<Double> & rhs) const {
    #ifdef MATH_USE_SSE
        Double fRes;
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( rhs.T, rhs.U );
        MathSSEFn->MulPD();
        MathSSEFn->HAddD(0);
        MathSSEFn->Push( V, W );
        MathSSEFn->Push( rhs.V, rhs.W );
        MathSSEFn->MulPD();
        MathSSEFn->HAddD(0);
        MathSSEFn->AddPD();
        MathSSEFn->Pop( fRes );
        return fRes;
    #else
        return ( (T * rhs.T) + (U * rhs.U) + (V * rhs.V) + (W * rhs.W) );
    #endif
}

template<typename Real>
inline TTextureCoord4<Real> TTextureCoord4<Real>::operator^(const TTextureCoord4<Real> & rhs) const {
    return TTextureCoord4<Real> (
        (U * rhs.V) - (V * rhs.U),
        (V * rhs.T) - (T * rhs.V),
        (T * rhs.U) - (U * rhs.T),
        (Real)0
    );
}
template<>
inline TTextureCoord4<Float> TTextureCoord4<Float>::operator^(const TTextureCoord4<Float> & rhs) const {
    #ifdef MATH_USE_SSE
        TTextureCoord4<Float> vRes;
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
        return TTextureCoord4<Float> (
            (U * rhs.V) - (V * rhs.U),
            (V * rhs.T) - (T * rhs.V),
            (T * rhs.U) - (U * rhs.T),
            0.0f
        );
    #endif
}
template<>
inline TTextureCoord4<Double> TTextureCoord4<Double>::operator^(const TTextureCoord4<Double> & rhs) const {
    #ifdef MATH_USE_SSE
        TTextureCoord4<Double> vRes;
        MathSSEFn->Push( U, V );
        MathSSEFn->Push( rhs.V, rhs.T );
        MathSSEFn->MulPD();
        MathSSEFn->Push( V, T );
        MathSSEFn->Push( rhs.U, rhs.V );
        MathSSEFn->MulPD();
        MathSSEFn->SubPD();
        MathSSEFn->Pop( vRes.T, vRes.U );
        MathSSEFn->Push( T, 0.0 );
        MathSSEFn->Push( rhs.U, 0.0 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( U, 0.0 );
        MathSSEFn->Push( rhs.T, 0.0 );
        MathSSEFn->MulPD();
        MathSSEFn->SubPD();
        MathSSEFn->Pop( vRes.V, vRes.W );
        return vRes;
    #else
        return TTextureCoord4<Double> (
            (U * rhs.V) - (V * rhs.U),
            (V * rhs.T) - (T * rhs.V),
            (T * rhs.U) - (U * rhs.T),
            0.0
        );
    #endif
}

template<typename Real>
inline Real TTextureCoord4<Real>::DistSqr() const {
    return ( T*T + U*U + V*V + W*W );
}
template<>
inline Float TTextureCoord4<Float>::DistSqr() const {
    #ifdef MATH_USE_SSE
        Float fRes;
        MathSSEFn->Push( T, U, V, W );
        MathSSEFn->Push( T, U, V, W );
        MathSSEFn->MulPF();
        MathSSEFn->HAddF(0);
        MathSSEFn->HAddF(0);
        MathSSEFn->Pop( fRes );
        return fRes;
    #else
        return ( T*T + U*U + V*V + W*W );
    #endif
}
template<>
inline Double TTextureCoord4<Double>::DistSqr() const {
    #ifdef MATH_USE_SSE
        Double fRes;
        MathSSEFn->Push( T, U );
        MathSSEFn->Push( T, U );
        MathSSEFn->MulPD();
        MathSSEFn->HAddD(0);
        MathSSEFn->Push( V, W );
        MathSSEFn->Push( V, W );
        MathSSEFn->MulPD();
        MathSSEFn->HAddD(0);
        MathSSEFn->AddPD();
        MathSSEFn->Pop( fRes );
        return fRes;
    #else
        return ( T*T + U*U + V*V + W*W );
    #endif
}

template<typename Real>
inline Real TTextureCoord4<Real>::Dist() const {
    return MathRealFn->Sqrt( DistSqr() );
}
template<typename Real>
inline Real TTextureCoord4<Real>::InvDistSqr() const {
    return MathRealFn->Invert( DistSqr() );
}
template<typename Real>
inline Real TTextureCoord4<Real>::InvDist() const {
    return MathRealFn->InvSqrt( DistSqr() );
}

