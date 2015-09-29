/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Vertex/Vertex4.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Homogeneous 4D vertex
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TVertex4 implementation
template<typename Real>
const TVertex4<Real> TVertex4<Real>::Null = TVertex4<Real>( (Real)0, (Real)0, (Real)0, (Real)1 );

template<typename Real>
TVertex4<Real>::TVertex4()
{
    // nothing to do
}
template<typename Real>
TVertex4<Real>::TVertex4(Real x, Real y, Real z, Real w)
{
    X = x;
    Y = y;
    Z = z;
    W = w;
}
template<typename Real>
TVertex4<Real>::TVertex4(const Real vArr[4])
{
    X = vArr[0];
    Y = vArr[1];
    Z = vArr[2];
    W = vArr[3];
}
template<typename Real>
TVertex4<Real>::TVertex4(const TVertex2<Real> & rhs)
{
    X = rhs.X;
    Y = rhs.Y;
    Z = (Real)0;
    W = (Real)1;
}
template<typename Real>
TVertex4<Real>::TVertex4(const TVertex3<Real> & rhs)
{
    X = rhs.X;
    Y = rhs.Y;
    Z = rhs.Z;
    W = (Real)1;
}
template<typename Real>
TVertex4<Real>::TVertex4(const TVertex4<Real> & rhs)
{
    X = rhs.X;
    Y = rhs.Y;
    Z = rhs.Z;
    W = rhs.W;
}
template<typename Real>
TVertex4<Real>::~TVertex4()
{
    // nothing to do
}

template<typename Real>
inline TVertex4<Real> & TVertex4<Real>::operator=(const TVertex4<Real> & rhs) {
    X = rhs.X;
    Y = rhs.Y;
    Z = rhs.Z;
    W = rhs.W;
    return (*this);
}

template<typename Real>
inline TVertex4<Real>::operator Real*() const {
    return (Real*)this;
}
template<typename Real>
inline TVertex4<Real>::operator const Real*() const {
    return (const Real*)this;
}

template<typename Real>
inline TVector4<Real> TVertex4<Real>::ToVector() const {
    return TVector4<Real>( X, Y, Z, 0.0f );
}

template<typename Real>
inline Real & TVertex4<Real>::operator[](Int i) {
    return *( ((Real*)this) + i );
}
template<typename Real>
inline Real TVertex4<Real>::operator[](Int i) const {
    return *( ((const Real*)this) + i );
}
template<typename Real>
inline Real & TVertex4<Real>::operator[](UInt i) {
    return *( ((Real*)this) + i );
}
template<typename Real>
inline Real TVertex4<Real>::operator[](UInt i) const {
    return *( ((const Real*)this) + i );
}

template<typename Real>
inline TVertex4<Real> TVertex4<Real>::operator+() const {
    return TVertex4<Real>(X, Y, Z, W);
}
template<typename Real>
inline TVertex4<Real> TVertex4<Real>::operator-() const {
    return TVertex4<Real>(-X, -Y, -Z, W);
}

template<typename Real>
inline Bool TVertex4<Real>::operator==(const TVertex4<Real> & rhs) const {
    return ( MathRealFn->Equals(X,rhs.X) && MathRealFn->Equals(Y,rhs.Y) &&
             MathRealFn->Equals(Z,rhs.Z) && MathRealFn->Equals(W,rhs.W) );
}
template<typename Real>
inline Bool TVertex4<Real>::operator!=(const TVertex4<Real> & rhs) const {
    return ( !(MathRealFn->Equals(X,rhs.X)) || !(MathRealFn->Equals(Y,rhs.Y)) ||
             !(MathRealFn->Equals(Z,rhs.Z)) || !(MathRealFn->Equals(W,rhs.W)) );
}

template<typename Real>
inline TVertex4<Real> TVertex4<Real>::operator+(Real rhs) const {
    return TVertex4<Real>( X + rhs, Y + rhs, Z + rhs, W );
}
template<>
inline TVertex4<Float> TVertex4<Float>::operator+(Float rhs) const {
    #ifdef MATH_USE_SSE
        TVertex4<Float> vRes;
        MathSSEFn->Push( X, Y, Z, W );
        MathSSEFn->Push( rhs, rhs, rhs, 0.0f );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( vRes.X, vRes.Y, vRes.Z, vRes.W );
        return vRes;
    #else
        return TVertex4<Float>( X + rhs, Y + rhs, Z + rhs, W );
    #endif
}
template<>
inline TVertex4<Double> TVertex4<Double>::operator+(Double rhs) const {
    #ifdef MATH_USE_SSE
        TVertex4<Double> vRes;
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
        return TVertex4<Double>( X + rhs, Y + rhs, Z + rhs, W );
    #endif
}
template<typename Real>
inline TVertex4<Real> TVertex4<Real>::operator-(Real rhs) const {
    return TVertex4<Real>( X - rhs, Y - rhs, Z - rhs, W );
}
template<>
inline TVertex4<Float> TVertex4<Float>::operator-(Float rhs) const {
    #ifdef MATH_USE_SSE
        TVertex4<Float> vRes;
        MathSSEFn->Push( X, Y, Z, W );
        MathSSEFn->Push( rhs, rhs, rhs, 0.0f );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( vRes.X, vRes.Y, vRes.Z, vRes.W );
        return vRes;
    #else
        return TVertex4<Float>( X - rhs, Y - rhs, Z - rhs, W );
    #endif
}
template<>
inline TVertex4<Double> TVertex4<Double>::operator-(Double rhs) const {
    #ifdef MATH_USE_SSE
        TVertex4<Double> vRes;
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
        return TVertex4<Double>( X - rhs, Y - rhs, Z - rhs, W );
    #endif
}
template<typename Real>
inline TVertex4<Real> TVertex4<Real>::operator*(Real rhs) const {
    return TVertex4<Real>( X * rhs, Y * rhs, Z * rhs, W );
}
template<>
inline TVertex4<Float> TVertex4<Float>::operator*(Float rhs) const {
    #ifdef MATH_USE_SSE
        TVertex4<Float> vRes;
        MathSSEFn->Push( X, Y, Z, W );
        MathSSEFn->Push( rhs, rhs, rhs, 1.0f );
        MathSSEFn->MulPF();
        MathSSEFn->Pop( vRes.X, vRes.Y, vRes.Z, vRes.W );
        return vRes;
    #else
        return TVertex4<Float>( X * rhs, Y * rhs, Z * rhs, W );
    #endif
}
template<>
inline TVertex4<Double> TVertex4<Double>::operator*(Double rhs) const {
    #ifdef MATH_USE_SSE
        TVertex4<Double> vRes;
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
        return TVertex4<Double>( X * rhs, Y * rhs, Z * rhs, W );
    #endif
}
template<typename Real>
inline TVertex4<Real> TVertex4<Real>::operator/(Real rhs) const {
    return TVertex4<Real>( X / rhs, Y / rhs, Z / rhs, W );
}
template<>
inline TVertex4<Float> TVertex4<Float>::operator/(Float rhs) const {
    #ifdef MATH_USE_SSE
        TVertex4<Float> vRes;
        MathSSEFn->Push( X, Y, Z, W );
        MathSSEFn->Push( rhs, rhs, rhs, 1.0f );
        MathSSEFn->DivPF();
        MathSSEFn->Pop( vRes.X, vRes.Y, vRes.Z, vRes.W );
        return vRes;
    #else
        return TVertex4<Float>( X / rhs, Y / rhs, Z / rhs, W );
    #endif
}
template<>
inline TVertex4<Double> TVertex4<Double>::operator/(Double rhs) const {
    #ifdef MATH_USE_SSE
        TVertex4<Double> vRes;
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
        return TVertex4<Double>( X / rhs, Y / rhs, Z / rhs, W );
    #endif
}

template<typename Real>
inline TVertex4<Real> & TVertex4<Real>::operator+=(Real rhs) {
    X += rhs;
    Y += rhs;
    Z += rhs;
    return (*this);
}
template<>
inline TVertex4<Float> & TVertex4<Float>::operator+=(Float rhs) {
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
inline TVertex4<Double> & TVertex4<Double>::operator+=(Double rhs) {
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
inline TVertex4<Real> & TVertex4<Real>::operator-=(Real rhs) {
    X -= rhs;
    Y -= rhs;
    Z -= rhs;
    return (*this);
}
template<>
inline TVertex4<Float> & TVertex4<Float>::operator-=(Float rhs) {
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
inline TVertex4<Double> & TVertex4<Double>::operator-=(Double rhs) {
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
inline TVertex4<Real> & TVertex4<Real>::operator*=(Real rhs) {
    X *= rhs;
    Y *= rhs;
    Z *= rhs;
    return (*this);
}
template<>
inline TVertex4<Float> & TVertex4<Float>::operator*=(Float rhs) {
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
inline TVertex4<Double> & TVertex4<Double>::operator*=(Double rhs) {
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
inline TVertex4<Real> & TVertex4<Real>::operator/=(Real rhs) {
    X /= rhs;
    Y /= rhs;
    Z /= rhs;
    return (*this);
}
template<>
inline TVertex4<Float> & TVertex4<Float>::operator/=(Float rhs) {
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
inline TVertex4<Double> & TVertex4<Double>::operator/=(Double rhs) {
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
inline TVector4<Real> TVertex4<Real>::operator-(const TVertex4<Real> & rhs) const {
    return TVector4<Real>( X - rhs.X, Y - rhs.Y, Z - rhs.Z, W - rhs.W );
}
template<>
inline TVector4<Float> TVertex4<Float>::operator-(const TVertex4<Float> & rhs) const {
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
inline TVector4<Double> TVertex4<Double>::operator-(const TVertex4<Double> & rhs) const {
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
inline TVertex4<Real> TVertex4<Real>::operator+(const TVector4<Real> & rhs) const {
    return TVertex4<Real>( X + rhs.X, Y + rhs.Y, Z + rhs.Z, W + rhs.W );
}
template<>
inline TVertex4<Float> TVertex4<Float>::operator+(const TVector4<Float> & rhs) const {
    #ifdef MATH_USE_SSE
        TVertex4<Float> vRes;
        MathSSEFn->Push( X, Y, Z, W );
        MathSSEFn->Push( rhs.X, rhs.Y, rhs.Z, rhs.W );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( vRes.X, vRes.Y, vRes.Z, vRes.W );
        return vRes;
    #else
        return TVertex4<Float>( X + rhs.X, Y + rhs.Y, Z + rhs.Z, W + rhs.W );
    #endif
}
template<>
inline TVertex4<Double> TVertex4<Double>::operator+(const TVector4<Double> & rhs) const {
    #ifdef MATH_USE_SSE
        TVertex4<Double> vRes;
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
        return TVertex4<Double>( X + rhs.X, Y + rhs.Y, Z + rhs.Z, W + rhs.W );
    #endif
}
template<typename Real>
inline TVertex4<Real> TVertex4<Real>::operator-(const TVector4<Real> & rhs) const {
    return TVertex4<Real>( X - rhs.X, Y - rhs.Y, Z - rhs.Z, W - rhs.W );
}
template<>
inline TVertex4<Float> TVertex4<Float>::operator-(const TVector4<Float> & rhs) const {
    #ifdef MATH_USE_SSE
        TVertex4<Float> vRes;
        MathSSEFn->Push( X, Y, Z, W );
        MathSSEFn->Push( rhs.X, rhs.Y, rhs.Z, rhs.W );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( vRes.X, vRes.Y, vRes.Z, vRes.W );
        return vRes;
    #else
        return TVertex4<Float>( X - rhs.X, Y - rhs.Y, Z - rhs.Z, W - rhs.W );
    #endif
}
template<>
inline TVertex4<Double> TVertex4<Double>::operator-(const TVector4<Double> & rhs) const {
    #ifdef MATH_USE_SSE
        TVertex4<Double> vRes;
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
        return TVertex4<Double>( X - rhs.X, Y - rhs.Y, Z - rhs.Z, W - rhs.W );
    #endif
}

template<typename Real>
inline TVertex4<Real> & TVertex4<Real>::operator+=(const TVector4<Real> & rhs) {
    X += rhs.X;
    Y += rhs.Y;
    Z += rhs.Z;
    W += rhs.W;
    return (*this);
}
template<>
inline TVertex4<Float> & TVertex4<Float>::operator+=(const TVector4<Float> & rhs) {
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
inline TVertex4<Double> & TVertex4<Double>::operator+=(const TVector4<Double> & rhs) {
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
inline TVertex4<Real> & TVertex4<Real>::operator-=(const TVector4<Real> & rhs) {
    X -= rhs.X;
    Y -= rhs.Y;
    Z -= rhs.Z;
    W -= rhs.W;
    return (*this);
}
template<>
inline TVertex4<Float> & TVertex4<Float>::operator-=(const TVector4<Float> & rhs) {
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
inline TVertex4<Double> & TVertex4<Double>::operator-=(const TVector4<Double> & rhs) {
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
inline Real TVertex4<Real>::DistSqr() const {
    return ( X*X + Y*Y + Z*Z );
}
template<>
inline Float TVertex4<Float>::DistSqr() const {
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
inline Double TVertex4<Double>::DistSqr() const {
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
inline Real TVertex4<Real>::Dist() const {
    return MathRealFn->Sqrt( DistSqr() );
}
template<typename Real>
inline Real TVertex4<Real>::InvDistSqr() const {
    return MathRealFn->Invert( DistSqr() );
}
template<typename Real>
inline Real TVertex4<Real>::InvDist() const {
    return MathRealFn->InvSqrt( DistSqr() );
}

template<typename Real>
inline Void TVertex4<Real>::NormalizeW() {
    Real fInvW = MathRealFn->Invert(W);
    X *= fInvW;
    Y *= fInvW;
    Z *= fInvW;
    W = (Real)1;
}
template<>
inline Void TVertex4<Float>::NormalizeW() {
    Float fInvW = MathFFn->Invert( W );
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y, Z, W );
        MathSSEFn->Push( fInvW, fInvW, fInvW, fInvW );
        MathSSEFn->MulPF();
        MathSSEFn->Pop( X, Y, Z, W );
    #else
        X *= fInvW;
        Y *= fInvW;
        Z *= fInvW;
        W = 1.0f;
    #endif
}
template<>
inline Void TVertex4<Double>::NormalizeW() {
    Double fInvW = MathDFn->Invert( W );
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( fInvW, fInvW );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( X, Y );
        MathSSEFn->Push( Z, W );
        MathSSEFn->Push( fInvW, fInvW );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( Z, W );
    #else
        X *= fInvW;
        Y *= fInvW;
        Z *= fInvW;
        W = 1.0;
    #endif
}

