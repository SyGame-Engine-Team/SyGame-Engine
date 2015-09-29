/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Color/Color4.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : RGBA 4-channels color
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TColor4 implementation
template<typename Real>
const Real TColor4<Real>::ZERO = (Real)0;
template<typename Real>
const Real TColor4<Real>::ONE = (Real)1;

template<typename Real>
const TColor4<Real> TColor4<Real>::Black =     TColor4<Real>( ZERO, ZERO, ZERO, ONE );
template<typename Real>
const TColor4<Real> TColor4<Real>::White =     TColor4<Real>( ONE,  ONE,  ONE, ONE );
template<typename Real>
const TColor4<Real> TColor4<Real>::LightGray = TColor4<Real>( (Real)0.75f, (Real)0.75f, (Real)0.75f, ONE );
template<typename Real>
const TColor4<Real> TColor4<Real>::Gray =      TColor4<Real>( (Real)0.5f,  (Real)0.5f,  (Real)0.5f,  ONE );
template<typename Real>
const TColor4<Real> TColor4<Real>::DarkGray =  TColor4<Real>( (Real)0.25f, (Real)0.25f, (Real)0.25f, ONE );
template<typename Real>
const TColor4<Real> TColor4<Real>::Red =       TColor4<Real>( ONE, ZERO, ZERO, ONE );
template<typename Real>
const TColor4<Real> TColor4<Real>::Green =     TColor4<Real>( ZERO, ONE, ZERO, ONE );
template<typename Real>
const TColor4<Real> TColor4<Real>::Blue =      TColor4<Real>( ZERO, ZERO, ONE, ONE );
template<typename Real>
const TColor4<Real> TColor4<Real>::Cyan =      TColor4<Real>( ZERO, ONE, ONE, ONE );
template<typename Real>
const TColor4<Real> TColor4<Real>::Magenta =   TColor4<Real>( ONE, ZERO, ONE, ONE );
template<typename Real>
const TColor4<Real> TColor4<Real>::Yellow =    TColor4<Real>( ONE, ONE, ZERO, ONE );

template<typename Real>
TColor4<Real>::TColor4()
{
    // nothing to do
}
template<typename Real>
TColor4<Real>::TColor4(Real r, Real g, Real b, Real a)
{
    R = r;
    G = g;
    B = b;
    A = a;
}
template<typename Real>
TColor4<Real>::TColor4(const Real vArr[4])
{
    R = vArr[0];
    G = vArr[1];
    B = vArr[2];
    A = vArr[3];
}
template<typename Real>
TColor4<Real>::TColor4(const TColor3<Real> & rhs)
{
    R = rhs.R;
    G = rhs.G;
    B = rhs.B;
    A = ONE;
}
template<typename Real>
TColor4<Real>::TColor4(const TColor4<Real> & rhs)
{
    R = rhs.R;
    G = rhs.G;
    B = rhs.B;
    A = rhs.A;
}
template<typename Real>
TColor4<Real>::~TColor4()
{
    // nothing to do
}

template<typename Real>
inline TColor4<Real> & TColor4<Real>::operator=(const TColor4<Real> & rhs) {
    R = rhs.R;
    G = rhs.G;
    B = rhs.B;
    A = rhs.A;
    return (*this);
}

template<typename Real>
inline TColor4<Real>::operator Real*() const {
    return (Real*)this;
}
template<typename Real>
inline TColor4<Real>::operator const Real*() const {
    return (const Real*)this;
}

template<typename Real>
inline Real & TColor4<Real>::operator[](Int i) {
    return *( ((Real*)this) + i );
}
template<typename Real>
inline Real TColor4<Real>::operator[](Int i) const {
    return *( ((const Real*)this) + i );
}
template<typename Real>
inline Real & TColor4<Real>::operator[](UInt i) {
    return *( ((Real*)this) + i );
}
template<typename Real>
inline Real TColor4<Real>::operator[](UInt i) const {
    return *( ((const Real*)this) + i );
}

template<typename Real>
inline TColor4<Real> TColor4<Real>::operator+() const {
    return TColor4<Real>(R, G, B, A);
}
template<typename Real>
inline TColor4<Real> TColor4<Real>::operator-() const {
    return TColor4<Real>(-R, -G, -B, -A);
}

template<typename Real>
inline Bool TColor4<Real>::operator==(const TColor4<Real> & rhs) const {
    return ( MathRealFn->Equals(R,rhs.R) && MathRealFn->Equals(G,rhs.G) &&
             MathRealFn->Equals(B,rhs.B) && MathRealFn->Equals(A,rhs.A) );
}
template<typename Real>
inline Bool TColor4<Real>::operator!=(const TColor4<Real> & rhs) const {
    return ( !(MathRealFn->Equals(R,rhs.R)) || !(MathRealFn->Equals(G,rhs.G)) ||
             !(MathRealFn->Equals(B,rhs.B)) || !(MathRealFn->Equals(A,rhs.A)) );
}

template<typename Real>
inline TColor4<Real> TColor4<Real>::operator+(Real rhs) const {
    return TColor4<Real>( R + rhs, G + rhs, B + rhs, A + rhs );
}
template<>
inline TColor4<Float> TColor4<Float>::operator+(Float rhs) const {
    #ifdef MATH_USE_SSE
        TColor4<Float> vRes;
        MathSSEFn->Push( R, G, B, A );
        MathSSEFn->Push( rhs, rhs, rhs, rhs );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( vRes.R, vRes.G, vRes.B, vRes.A );
        return vRes;
    #else
        return TColor4<Float>( R + rhs, G + rhs, B + rhs, A + rhs );
    #endif
}
template<>
inline TColor4<Double> TColor4<Double>::operator+(Double rhs) const {
    #ifdef MATH_USE_SSE
        TColor4<Double> vRes;
        MathSSEFn->Push( R, G );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( vRes.R, vRes.G );
        MathSSEFn->Push( B, A );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( vRes.B, vRes.A );
        return vRes;
    #else
        return TColor4<Double>( R + rhs, G + rhs, B + rhs, A + rhs );
    #endif
}
template<typename Real>
inline TColor4<Real> TColor4<Real>::operator-(Real rhs) const {
    return TColor4<Real>( R - rhs, G - rhs, B - rhs, A - rhs );
}
template<>
inline TColor4<Float> TColor4<Float>::operator-(Float rhs) const {
    #ifdef MATH_USE_SSE
        TColor4<Float> vRes;
        MathSSEFn->Push( R, G, B, A );
        MathSSEFn->Push( rhs, rhs, rhs, rhs );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( vRes.R, vRes.G, vRes.B, vRes.A );
        return vRes;
    #else
        return TColor4<Float>( R - rhs, G - rhs, B - rhs, A - rhs );
    #endif
}
template<>
inline TColor4<Double> TColor4<Double>::operator-(Double rhs) const {
    #ifdef MATH_USE_SSE
        TColor4<Double> vRes;
        MathSSEFn->Push( R, G );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( vRes.R, vRes.G );
        MathSSEFn->Push( B, A );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( vRes.B, vRes.A );
        return vRes;
    #else
        return TColor4<Double>( R - rhs, G - rhs, B - rhs, A - rhs );
    #endif
}
template<typename Real>
inline TColor4<Real> TColor4<Real>::operator*(Real rhs) const {
    return TColor4<Real>( R * rhs, G * rhs, B * rhs, A * rhs );
}
template<>
inline TColor4<Float> TColor4<Float>::operator*(Float rhs) const {
    #ifdef MATH_USE_SSE
        TColor4<Float> vRes;
        MathSSEFn->Push( R, G, B, A );
        MathSSEFn->Push( rhs, rhs, rhs, rhs );
        MathSSEFn->MulPF();
        MathSSEFn->Pop( vRes.R, vRes.G, vRes.B, vRes.A );
        return vRes;
    #else
        return TColor4<Float>( R * rhs, G * rhs, B * rhs, A * rhs );
    #endif
}
template<>
inline TColor4<Double> TColor4<Double>::operator*(Double rhs) const {
    #ifdef MATH_USE_SSE
        TColor4<Double> vRes;
        MathSSEFn->Push( R, G );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( vRes.R, vRes.G );
        MathSSEFn->Push( B, A );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( vRes.B, vRes.A );
        return vRes;
    #else
        return TColor4<Double>( R * rhs, G * rhs, B * rhs, A * rhs );
    #endif
}
template<typename Real>
inline TColor4<Real> TColor4<Real>::operator/(Real rhs) const {
    return TColor4<Real>( R / rhs, G / rhs, B / rhs, A / rhs );
}
template<>
inline TColor4<Float> TColor4<Float>::operator/(Float rhs) const {
    #ifdef MATH_USE_SSE
        TColor4<Float> vRes;
        MathSSEFn->Push( R, G, B, A );
        MathSSEFn->Push( rhs, rhs, rhs, rhs );
        MathSSEFn->DivPF();
        MathSSEFn->Pop( vRes.R, vRes.G, vRes.B, vRes.A );
        return vRes;
    #else
        return TColor4<Float>( R / rhs, G / rhs, B / rhs, A / rhs );
    #endif
}
template<>
inline TColor4<Double> TColor4<Double>::operator/(Double rhs) const {
    #ifdef MATH_USE_SSE
        TColor4<Double> vRes;
        MathSSEFn->Push( R, G );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( vRes.R, vRes.G );
        MathSSEFn->Push( B, A );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( vRes.B, vRes.A );
        return vRes;
    #else
        return TColor4<Double>( R / rhs, G / rhs, B / rhs, A / rhs );
    #endif
}

template<typename Real>
inline TColor4<Real> & TColor4<Real>::operator+=(Real rhs) {
    R += rhs;
    G += rhs;
    B += rhs;
    A += rhs;
    return (*this);
}
template<>
inline TColor4<Float> & TColor4<Float>::operator+=(Float rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( R, G, B, A );
        MathSSEFn->Push( rhs, rhs, rhs, rhs );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( R, G, B, A );
    #else
        R += rhs;
        G += rhs;
        B += rhs;
        A += rhs;
    #endif
    return (*this);
}
template<>
inline TColor4<Double> & TColor4<Double>::operator+=(Double rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( R, G );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( R, G );
        MathSSEFn->Push( B, A );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( B, A );
    #else
        R += rhs;
        G += rhs;
        B += rhs;
        A += rhs;
    #endif
    return (*this);
}
template<typename Real>
inline TColor4<Real> & TColor4<Real>::operator-=(Real rhs) {
    R -= rhs;
    G -= rhs;
    B -= rhs;
    A -= rhs;
    return (*this);
}
template<>
inline TColor4<Float> & TColor4<Float>::operator-=(Float rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( R, G, B, A );
        MathSSEFn->Push( rhs, rhs, rhs, rhs );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( R, G, B, A );
    #else
        R -= rhs;
        G -= rhs;
        B -= rhs;
        A -= rhs;
    #endif
    return (*this);
}
template<>
inline TColor4<Double> & TColor4<Double>::operator-=(Double rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( R, G );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( R, G );
        MathSSEFn->Push( B, A );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( B, A );
    #else
        R -= rhs;
        G -= rhs;
        B -= rhs;
        A -= rhs;
    #endif
    return (*this);
}
template<typename Real>
inline TColor4<Real> & TColor4<Real>::operator*=(Real rhs) {
    R *= rhs;
    G *= rhs;
    B *= rhs;
    A *= rhs;
    return (*this);
}
template<>
inline TColor4<Float> & TColor4<Float>::operator*=(Float rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( R, G, B, A );
        MathSSEFn->Push( rhs, rhs, rhs, rhs );
        MathSSEFn->MulPF();
        MathSSEFn->Pop( R, G, B, A );
    #else
        R *= rhs;
        G *= rhs;
        B *= rhs;
        A *= rhs;
    #endif
    return (*this);
}
template<>
inline TColor4<Double> & TColor4<Double>::operator*=(Double rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( R, G );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( R, G );
        MathSSEFn->Push( B, A );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( B, A );
    #else
        R *= rhs;
        G *= rhs;
        B *= rhs;
        A *= rhs;
    #endif
    return (*this);
}
template<typename Real>
inline TColor4<Real> & TColor4<Real>::operator/=(Real rhs) {
    R /= rhs;
    G /= rhs;
    B /= rhs;
    A /= rhs;
    return (*this);
}
template<>
inline TColor4<Float> & TColor4<Float>::operator/=(Float rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( R, G, B, A );
        MathSSEFn->Push( rhs, rhs, rhs, rhs );
        MathSSEFn->DivPF();
        MathSSEFn->Pop( R, G, B, A );
    #else
        R /= rhs;
        G /= rhs;
        B /= rhs;
        A /= rhs;
    #endif
    return (*this);
}
template<>
inline TColor4<Double> & TColor4<Double>::operator/=(Double rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( R, G );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( R, G );
        MathSSEFn->Push( B, A );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( B, A );
    #else
        R /= rhs;
        G /= rhs;
        B /= rhs;
        A /= rhs;
    #endif
    return (*this);
}

template<typename Real>
inline TColor4<Real> TColor4<Real>::operator+(const TColor4<Real> & rhs) const {
    return TColor4<Real>( R + rhs.R, G + rhs.G, B + rhs.B, A + rhs.A );
}
template<>
inline TColor4<Float> TColor4<Float>::operator+(const TColor4<Float> & rhs) const {
    #ifdef MATH_USE_SSE
        TColor4<Float> vRes;
        MathSSEFn->Push( R, G, B, A );
        MathSSEFn->Push( rhs.R, rhs.G, rhs.B, rhs.A );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( vRes.R, vRes.G, vRes.B, vRes.A );
        return vRes;
    #else
        return TColor4<Float>( R + rhs.R, G + rhs.G, B + rhs.B, A + rhs.A );
    #endif
}
template<>
inline TColor4<Double> TColor4<Double>::operator+(const TColor4<Double> & rhs) const {
    #ifdef MATH_USE_SSE
        TColor4<Double> vRes;
        MathSSEFn->Push( R, G );
        MathSSEFn->Push( rhs.R, rhs.G );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( vRes.R, vRes.G );
        MathSSEFn->Push( B, A );
        MathSSEFn->Push( rhs.B, rhs.A );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( vRes.B, vRes.A );
        return vRes;
    #else
        return TColor4<Double>( R + rhs.R, G + rhs.G, B + rhs.B, A + rhs.A );
    #endif
}
template<typename Real>
inline TColor4<Real> TColor4<Real>::operator-(const TColor4<Real> & rhs) const {
    return TColor4<Real>( R - rhs.R, G - rhs.G, B - rhs.B, A - rhs.A );
}
template<>
inline TColor4<Float> TColor4<Float>::operator-(const TColor4<Float> & rhs) const {
    #ifdef MATH_USE_SSE
        TColor4<Float> vRes;
        MathSSEFn->Push( R, G, B, A );
        MathSSEFn->Push( rhs.R, rhs.G, rhs.B, rhs.A );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( vRes.R, vRes.G, vRes.B, vRes.A );
        return vRes;
    #else
        return TColor4<Float>( R - rhs.R, G - rhs.G, B - rhs.B, A - rhs.A );
    #endif
}
template<>
inline TColor4<Double> TColor4<Double>::operator-(const TColor4<Double> & rhs) const {
    #ifdef MATH_USE_SSE
        TColor4<Double> vRes;
        MathSSEFn->Push( R, G );
        MathSSEFn->Push( rhs.R, rhs.G );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( vRes.R, vRes.G );
        MathSSEFn->Push( B, A );
        MathSSEFn->Push( rhs.B, rhs.A );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( vRes.B, vRes.A );
        return vRes;
    #else
        return TColor4<Double>( R - rhs.R, G - rhs.G, B - rhs.B, A - rhs.A );
    #endif
}
template<typename Real>
inline TColor4<Real> TColor4<Real>::operator*(const TColor4<Real> & rhs) const {
    return TColor4<Real>( R * rhs.R, G * rhs.G, B * rhs.B, A * rhs.A );
}
template<>
inline TColor4<Float> TColor4<Float>::operator*(const TColor4<Float> & rhs) const {
    #ifdef MATH_USE_SSE
        TColor4<Float> vRes;
        MathSSEFn->Push( R, G, B, A );
        MathSSEFn->Push( rhs.R, rhs.G, rhs.B, rhs.A );
        MathSSEFn->MulPF();
        MathSSEFn->Pop( vRes.R, vRes.G, vRes.B, vRes.A );
        return vRes;
    #else
        return TColor4<Float>( R * rhs.R, G * rhs.G, B * rhs.B, A * rhs.A );
    #endif
}
template<>
inline TColor4<Double> TColor4<Double>::operator*(const TColor4<Double> & rhs) const {
    #ifdef MATH_USE_SSE
        TColor4<Double> vRes;
        MathSSEFn->Push( R, G );
        MathSSEFn->Push( rhs.R, rhs.G );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( vRes.R, vRes.G );
        MathSSEFn->Push( B, A );
        MathSSEFn->Push( rhs.B, rhs.A );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( vRes.B, vRes.A );
        return vRes;
    #else
        return TColor4<Double>( R * rhs.R, G * rhs.G, B * rhs.B, A * rhs.A );
    #endif
}
template<typename Real>
inline TColor4<Real> TColor4<Real>::operator/(const TColor4<Real> & rhs) const {
    return TColor4<Real>( R / rhs.R, G / rhs.G, B / rhs.B, A / rhs.A );
}
template<>
inline TColor4<Float> TColor4<Float>::operator/(const TColor4<Float> & rhs) const {
    #ifdef MATH_USE_SSE
        TColor4<Float> vRes;
        MathSSEFn->Push( R, G, B, A );
        MathSSEFn->Push( rhs.R, rhs.G, rhs.B, rhs.A );
        MathSSEFn->DivPF();
        MathSSEFn->Pop( vRes.R, vRes.G, vRes.B, vRes.A );
        return vRes;
    #else
        return TColor4<Float>( R / rhs.R, G / rhs.G, B / rhs.B, A / rhs.A );
    #endif
}
template<>
inline TColor4<Double> TColor4<Double>::operator/(const TColor4<Double> & rhs) const {
    #ifdef MATH_USE_SSE
        TColor4<Double> vRes;
        MathSSEFn->Push( R, G );
        MathSSEFn->Push( rhs.R, rhs.G );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( vRes.R, vRes.G );
        MathSSEFn->Push( B, A );
        MathSSEFn->Push( rhs.B, rhs.A );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( vRes.B, vRes.A );
        return vRes;
    #else
        return TColor4<Double>( R / rhs.R, G / rhs.G, B / rhs.B, A / rhs.A );
    #endif
}

template<typename Real>
inline TColor4<Real> & TColor4<Real>::operator+=(const TColor4<Real> & rhs) {
    R += rhs.R;
    G += rhs.G;
    B += rhs.B;
    A += rhs.A;
    return (*this);
}
template<>
inline TColor4<Float> & TColor4<Float>::operator+=(const TColor4<Float> & rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( R, G, B, A );
        MathSSEFn->Push( rhs.R, rhs.G, rhs.B, rhs.A );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( R, G, B, A );
    #else
        R += rhs.R;
        G += rhs.G;
        B += rhs.B;
        A += rhs.A;
    #endif
    return (*this);
}
template<>
inline TColor4<Double> & TColor4<Double>::operator+=(const TColor4<Double> & rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( R, G );
        MathSSEFn->Push( rhs.R, rhs.G );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( R, G );
        MathSSEFn->Push( B, A );
        MathSSEFn->Push( rhs.B, rhs.A );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( B, A );
    #else
        R += rhs.R;
        G += rhs.G;
        B += rhs.B;
        A += rhs.A;
    #endif
    return (*this);
}
template<typename Real>
inline TColor4<Real> & TColor4<Real>::operator-=(const TColor4<Real> & rhs) {
    R -= rhs.R;
    G -= rhs.G;
    B -= rhs.B;
    A -= rhs.A;
    return (*this);
}
template<>
inline TColor4<Float> & TColor4<Float>::operator-=(const TColor4<Float> & rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( R, G, B, A );
        MathSSEFn->Push( rhs.R, rhs.G, rhs.B, rhs.A );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( R, G, B, A );
    #else
        R -= rhs.R;
        G -= rhs.G;
        B -= rhs.B;
        A -= rhs.A;
    #endif
    return (*this);
}
template<>
inline TColor4<Double> & TColor4<Double>::operator-=(const TColor4<Double> & rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( R, G );
        MathSSEFn->Push( rhs.R, rhs.G );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( R, G );
        MathSSEFn->Push( B, A );
        MathSSEFn->Push( rhs.B, rhs.A );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( B, A );
    #else
        R -= rhs.R;
        G -= rhs.G;
        B -= rhs.B;
        A -= rhs.A;
    #endif
    return (*this);
}
template<typename Real>
inline TColor4<Real> & TColor4<Real>::operator*=(const TColor4<Real> & rhs) {
    R *= rhs.R;
    G *= rhs.G;
    B *= rhs.B;
    A *= rhs.A;
    return (*this);
}
template<>
inline TColor4<Float> & TColor4<Float>::operator*=(const TColor4<Float> & rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( R, G, B, A );
        MathSSEFn->Push( rhs.R, rhs.G, rhs.B, rhs.A );
        MathSSEFn->MulPF();
        MathSSEFn->Pop( R, G, B, A );
    #else
        R *= rhs.R;
        G *= rhs.G;
        B *= rhs.B;
        A *= rhs.A;
    #endif
    return (*this);
}
template<>
inline TColor4<Double> & TColor4<Double>::operator*=(const TColor4<Double> & rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( R, G );
        MathSSEFn->Push( rhs.R, rhs.G );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( R, G );
        MathSSEFn->Push( B, A );
        MathSSEFn->Push( rhs.B, rhs.A );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( B, A );
    #else
        R *= rhs.R;
        G *= rhs.G;
        B *= rhs.B;
        A *= rhs.A;
    #endif
    return (*this);
}
template<typename Real>
inline TColor4<Real> & TColor4<Real>::operator/=(const TColor4<Real> & rhs) {
    R /= rhs.R;
    G /= rhs.G;
    B /= rhs.B;
    A /= rhs.A;
    return (*this);
}
template<>
inline TColor4<Float> & TColor4<Float>::operator/=(const TColor4<Float> & rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( R, G, B, A );
        MathSSEFn->Push( rhs.R, rhs.G, rhs.B, rhs.A );
        MathSSEFn->DivPF();
        MathSSEFn->Pop( R, G, B, A );
    #else
        R /= rhs.R;
        G /= rhs.G;
        B /= rhs.B;
        A /= rhs.A;
    #endif
    return (*this);
}
template<>
inline TColor4<Double> & TColor4<Double>::operator/=(const TColor4<Double> & rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( R, G );
        MathSSEFn->Push( rhs.R, rhs.G );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( R, G );
        MathSSEFn->Push( B, A );
        MathSSEFn->Push( rhs.B, rhs.A );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( B, A );
    #else
        R /= rhs.R;
        G /= rhs.G;
        B /= rhs.B;
        A /= rhs.A;
    #endif
    return (*this);
}

template<typename Real>
inline Void TColor4<Real>::Clamp() {
    R = Clamp<Real>( R, ZERO, ONE );
    G = Clamp<Real>( G, ZERO, ONE );
    B = Clamp<Real>( B, ZERO, ONE );
    A = Clamp<Real>( A, ZERO, ONE );
}

template<typename Real>
inline Void TColor4<Real>::FromYCbCr( Real fY, Real fCb, Real fCr ) {
    R = fY + (fCr * (Real)1.402f);
	G = fY - (fCb * (Real)0.34414f) - (fCr * (Real)0.71414f);
	B = fY + (fCb * (Real)1.772f);
    A = ONE;
}
template<typename Real>
inline Void TColor4<Real>::ToYCbCr( Real & outY, Real & outCb, Real & outCr ) const {
    outY =  (R * (Real)0.299f)   + (G * (Real)0.587f)  + (B * (Real)0.114f);
	outCb = (R * (Real)-0.1687f) - (G * (Real)0.3313f) + (B * (Real)0.5f);
	outCr = (R * (Real)0.5f)     - (G * (Real)0.4187f) - (B * (Real)0.0813f);
}

template<typename Real>
inline Void TColor4<Real>::ToMonochrome( TColor4<Real> & outMonochrome ) const {
    ToGrayscale( outMonochrome );
    if ( outMonochrome.R < (Real)0.5f ) {
        outMonochrome.R = ZERO;
        outMonochrome.G = ZERO;
        outMonochrome.B = ZERO;
    } else {
        outMonochrome.R = ONE;
        outMonochrome.G = ONE;
        outMonochrome.B = ONE;
    }
}
template<typename Real>
inline Void TColor4<Real>::ToGrayscale( TColor4<Real> & outGrayscale ) const {
    Real Y, Cb, Cr;
    ToYCbCr(Y, Cb, Cr);
    outGrayscale.FromYCbCr( ZERO, Cb, Cr );
}

