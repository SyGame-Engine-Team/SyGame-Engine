/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Color/Color3.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : RGB 3-channels color
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TColor3 implementation
template<typename Real>
const Real TColor3<Real>::ZERO = (Real)0;
template<typename Real>
const Real TColor3<Real>::ONE = (Real)1;

template<typename Real>
const TColor3<Real> TColor3<Real>::Black =     TColor3<Real>( ZERO, ZERO, ZERO );
template<typename Real>
const TColor3<Real> TColor3<Real>::White =     TColor3<Real>( ONE,  ONE,  ONE );
template<typename Real>
const TColor3<Real> TColor3<Real>::LightGray = TColor3<Real>( (Real)0.75f, (Real)0.75f, (Real)0.75f );
template<typename Real>
const TColor3<Real> TColor3<Real>::Gray =      TColor3<Real>( (Real)0.5f,  (Real)0.5f,  (Real)0.5f );
template<typename Real>
const TColor3<Real> TColor3<Real>::DarkGray =  TColor3<Real>( (Real)0.25f, (Real)0.25f, (Real)0.25f );
template<typename Real>
const TColor3<Real> TColor3<Real>::Red =       TColor3<Real>( ONE, ZERO, ZERO );
template<typename Real>
const TColor3<Real> TColor3<Real>::Green =     TColor3<Real>( ZERO, ONE, ZERO );
template<typename Real>
const TColor3<Real> TColor3<Real>::Blue =      TColor3<Real>( ZERO, ZERO, ONE );
template<typename Real>
const TColor3<Real> TColor3<Real>::Cyan =      TColor3<Real>( ZERO, ONE, ONE );
template<typename Real>
const TColor3<Real> TColor3<Real>::Magenta =   TColor3<Real>( ONE, ZERO, ONE );
template<typename Real>
const TColor3<Real> TColor3<Real>::Yellow =    TColor3<Real>( ONE, ONE, ZERO );

template<typename Real>
TColor3<Real>::TColor3()
{
    // nothing to do
}
template<typename Real>
TColor3<Real>::TColor3(Real r, Real g, Real b)
{
    R = r;
    G = g;
    B = b;
}
template<typename Real>
TColor3<Real>::TColor3(const Real vArr[3])
{
    R = vArr[0];
    G = vArr[1];
    B = vArr[2];
}
template<typename Real>
TColor3<Real>::TColor3(const TColor3<Real> & rhs)
{
    R = rhs.R;
    G = rhs.G;
    B = rhs.B;
}
template<typename Real>
TColor3<Real>::TColor3(const TColor4<Real> & rhs)
{
    R = rhs.R;
    G = rhs.G;
    B = rhs.B;
}
template<typename Real>
TColor3<Real>::~TColor3()
{
    // nothing to do
}

template<typename Real>
inline TColor3<Real> & TColor3<Real>::operator=(const TColor3<Real> & rhs) {
    R = rhs.R;
    G = rhs.G;
    B = rhs.B;
    return (*this);
}

template<typename Real>
inline TColor3<Real>::operator Real*() const {
    return (Real*)this;
}
template<typename Real>
inline TColor3<Real>::operator const Real*() const {
    return (const Real*)this;
}

template<typename Real>
inline Real & TColor3<Real>::operator[](Int i) {
    return *( ((Real*)this) + i );
}
template<typename Real>
inline Real TColor3<Real>::operator[](Int i) const {
    return *( ((const Real*)this) + i );
}
template<typename Real>
inline Real & TColor3<Real>::operator[](UInt i) {
    return *( ((Real*)this) + i );
}
template<typename Real>
inline Real TColor3<Real>::operator[](UInt i) const {
    return *( ((const Real*)this) + i );
}

template<typename Real>
inline TColor3<Real> TColor3<Real>::operator+() const {
    return TColor3<Real>(R, G, B);
}
template<typename Real>
inline TColor3<Real> TColor3<Real>::operator-() const {
    return TColor3<Real>(-R, -G, -B);
}

template<typename Real>
inline Bool TColor3<Real>::operator==(const TColor3<Real> & rhs) const {
    return ( MathRealFn->Equals(R,rhs.R) && MathRealFn->Equals(G,rhs.G) &&
             MathRealFn->Equals(B,rhs.B) );
}
template<typename Real>
inline Bool TColor3<Real>::operator!=(const TColor3<Real> & rhs) const {
    return ( !(MathRealFn->Equals(R,rhs.R)) || !(MathRealFn->Equals(G,rhs.G)) ||
             !(MathRealFn->Equals(B,rhs.B)) );
}

template<typename Real>
inline TColor3<Real> TColor3<Real>::operator+(Real rhs) const {
    return TColor3<Real>( R + rhs, G + rhs, B + rhs );
}
template<>
inline TColor3<Float> TColor3<Float>::operator+(Float rhs) const {
    #ifdef MATH_USE_SSE
        TColor3<Float> vRes;
        MathSSEFn->Push( R, G, B );
        MathSSEFn->Push( rhs, rhs, rhs );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( vRes.R, vRes.G, vRes.B );
        return vRes;
    #else
        return TColor3<Float>( R + rhs, G + rhs, B + rhs );
    #endif
}
template<>
inline TColor3<Double> TColor3<Double>::operator+(Double rhs) const {
    #ifdef MATH_USE_SSE
        TColor3<Double> vRes;
        MathSSEFn->Push( R, G );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( vRes.R, vRes.G );
        MathSSEFn->Push( B );
        MathSSEFn->Push( rhs );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( vRes.B );
        return vRes;
    #else
        return TColor3<Double>( R + rhs, G + rhs, B + rhs );
    #endif
}
template<typename Real>
inline TColor3<Real> TColor3<Real>::operator-(Real rhs) const {
    return TColor3<Real>( R - rhs, G - rhs, B - rhs );
}
template<>
inline TColor3<Float> TColor3<Float>::operator-(Float rhs) const {
    #ifdef MATH_USE_SSE
        TColor3<Float> vRes;
        MathSSEFn->Push( R, G, B );
        MathSSEFn->Push( rhs, rhs, rhs );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( vRes.R, vRes.G, vRes.B );
        return vRes;
    #else
        return TColor3<Float>( R - rhs, G - rhs, B - rhs );
    #endif
}
template<>
inline TColor3<Double> TColor3<Double>::operator-(Double rhs) const {
    #ifdef MATH_USE_SSE
        TColor3<Double> vRes;
        MathSSEFn->Push( R, G );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( vRes.R, vRes.G );
        MathSSEFn->Push( B );
        MathSSEFn->Push( rhs );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( vRes.B );
        return vRes;
    #else
        return TColor3<Double>( R - rhs, G - rhs, B - rhs );
    #endif
}
template<typename Real>
inline TColor3<Real> TColor3<Real>::operator*(Real rhs) const {
    return TColor3<Real>( R * rhs, G * rhs, B * rhs );
}
template<>
inline TColor3<Float> TColor3<Float>::operator*(Float rhs) const {
    #ifdef MATH_USE_SSE
        TColor3<Float> vRes;
        MathSSEFn->Push( R, G, B );
        MathSSEFn->Push( rhs, rhs, rhs );
        MathSSEFn->MulPF();
        MathSSEFn->Pop( vRes.R, vRes.G, vRes.B );
        return vRes;
    #else
        return TColor3<Float>( R * rhs, G * rhs, B * rhs );
    #endif
}
template<>
inline TColor3<Double> TColor3<Double>::operator*(Double rhs) const {
    #ifdef MATH_USE_SSE
        TColor3<Double> vRes;
        MathSSEFn->Push( R, G );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( vRes.R, vRes.G );
        MathSSEFn->Push( B );
        MathSSEFn->Push( rhs );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( vRes.B );
        return vRes;
    #else
        return TColor3<Double>( R * rhs, G * rhs, B * rhs );
    #endif
}
template<typename Real>
inline TColor3<Real> TColor3<Real>::operator/(Real rhs) const {
    return TColor3<Real>( R / rhs, G / rhs, B / rhs );
}
template<>
inline TColor3<Float> TColor3<Float>::operator/(Float rhs) const {
    #ifdef MATH_USE_SSE
        TColor3<Float> vRes;
        MathSSEFn->Push( R, G, B );
        MathSSEFn->Push( rhs, rhs, rhs );
        MathSSEFn->DivPF();
        MathSSEFn->Pop( vRes.R, vRes.G, vRes.B );
        return vRes;
    #else
        return TColor3<Float>( R / rhs, G / rhs, B / rhs );
    #endif
}
template<>
inline TColor3<Double> TColor3<Double>::operator/(Double rhs) const {
    #ifdef MATH_USE_SSE
        TColor3<Double> vRes;
        MathSSEFn->Push( R, G );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( vRes.R, vRes.G );
        MathSSEFn->Push( B );
        MathSSEFn->Push( rhs );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( vRes.B );
        return vRes;
    #else
        return TColor3<Double>( R / rhs, G / rhs, B / rhs );
    #endif
}

template<typename Real>
inline TColor3<Real> & TColor3<Real>::operator+=(Real rhs) {
    R += rhs;
    G += rhs;
    B += rhs;
    return (*this);
}
template<>
inline TColor3<Float> & TColor3<Float>::operator+=(Float rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( R, G, B );
        MathSSEFn->Push( rhs, rhs, rhs );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( R, G, B );
    #else
        R += rhs;
        G += rhs;
        B += rhs;
    #endif
    return (*this);
}
template<>
inline TColor3<Double> & TColor3<Double>::operator+=(Double rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( R, G );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( R, G );
        MathSSEFn->Push( B );
        MathSSEFn->Push( rhs );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( B );
    #else
        R += rhs;
        G += rhs;
        B += rhs;
    #endif
    return (*this);
}
template<typename Real>
inline TColor3<Real> & TColor3<Real>::operator-=(Real rhs) {
    R -= rhs;
    G -= rhs;
    B -= rhs;
    return (*this);
}
template<>
inline TColor3<Float> & TColor3<Float>::operator-=(Float rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( R, G, B );
        MathSSEFn->Push( rhs, rhs, rhs );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( R, G, B );
    #else
        R -= rhs;
        G -= rhs;
        B -= rhs;
    #endif
    return (*this);
}
template<>
inline TColor3<Double> & TColor3<Double>::operator-=(Double rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( R, G );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( R, G );
        MathSSEFn->Push( B );
        MathSSEFn->Push( rhs );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( B );
    #else
        R -= rhs;
        G -= rhs;
        B -= rhs;
    #endif
    return (*this);
}
template<typename Real>
inline TColor3<Real> & TColor3<Real>::operator*=(Real rhs) {
    R *= rhs;
    G *= rhs;
    B *= rhs;
    return (*this);
}
template<>
inline TColor3<Float> & TColor3<Float>::operator*=(Float rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( R, G, B );
        MathSSEFn->Push( rhs, rhs, rhs );
        MathSSEFn->MulPF();
        MathSSEFn->Pop( R, G, B );
    #else
        R *= rhs;
        G *= rhs;
        B *= rhs;
    #endif
    return (*this);
}
template<>
inline TColor3<Double> & TColor3<Double>::operator*=(Double rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( R, G );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( R, G );
        MathSSEFn->Push( B );
        MathSSEFn->Push( rhs );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( B );
    #else
        R *= rhs;
        G *= rhs;
        B *= rhs;
    #endif
    return (*this);
}
template<typename Real>
inline TColor3<Real> & TColor3<Real>::operator/=(Real rhs) {
    R /= rhs;
    G /= rhs;
    B /= rhs;
    return (*this);
}
template<>
inline TColor3<Float> & TColor3<Float>::operator/=(Float rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( R, G, B );
        MathSSEFn->Push( rhs, rhs, rhs );
        MathSSEFn->DivPF();
        MathSSEFn->Pop( R, G, B );
    #else
        R /= rhs;
        G /= rhs;
        B /= rhs;
    #endif
    return (*this);
}
template<>
inline TColor3<Double> & TColor3<Double>::operator/=(Double rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( R, G );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( R, G );
        MathSSEFn->Push( B );
        MathSSEFn->Push( rhs );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( B );
    #else
        R /= rhs;
        G /= rhs;
        B /= rhs;
    #endif
    return (*this);
}

template<typename Real>
inline TColor3<Real> TColor3<Real>::operator+(const TColor3<Real> & rhs) const {
    return TColor3<Real>( R + rhs.R, G + rhs.G, B + rhs.B );
}
template<>
inline TColor3<Float> TColor3<Float>::operator+(const TColor3<Float> & rhs) const {
    #ifdef MATH_USE_SSE
        TColor3<Float> vRes;
        MathSSEFn->Push( R, G, B );
        MathSSEFn->Push( rhs.R, rhs.G, rhs.B );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( vRes.R, vRes.G, vRes.B );
        return vRes;
    #else
        return TColor3<Float>( R + rhs.R, G + rhs.G, B + rhs.B );
    #endif
}
template<>
inline TColor3<Double> TColor3<Double>::operator+(const TColor3<Double> & rhs) const {
    #ifdef MATH_USE_SSE
        TColor3<Double> vRes;
        MathSSEFn->Push( R, G );
        MathSSEFn->Push( rhs.R, rhs.G );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( vRes.R, vRes.G );
        MathSSEFn->Push( B );
        MathSSEFn->Push( rhs.B );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( vRes.B );
        return vRes;
    #else
        return TColor3<Double>( R + rhs.R, G + rhs.G, B + rhs.B );
    #endif
}
template<typename Real>
inline TColor3<Real> TColor3<Real>::operator-(const TColor3<Real> & rhs) const {
    return TColor3<Real>( R - rhs.R, G - rhs.G, B - rhs.B );
}
template<>
inline TColor3<Float> TColor3<Float>::operator-(const TColor3<Float> & rhs) const {
    #ifdef MATH_USE_SSE
        TColor3<Float> vRes;
        MathSSEFn->Push( R, G, B );
        MathSSEFn->Push( rhs.R, rhs.G, rhs.B );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( vRes.R, vRes.G, vRes.B );
        return vRes;
    #else
        return TColor3<Float>( R - rhs.R, G - rhs.G, B - rhs.B );
    #endif
}
template<>
inline TColor3<Double> TColor3<Double>::operator-(const TColor3<Double> & rhs) const {
    #ifdef MATH_USE_SSE
        TColor3<Double> vRes;
        MathSSEFn->Push( R, G );
        MathSSEFn->Push( rhs.R, rhs.G );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( vRes.R, vRes.G );
        MathSSEFn->Push( B );
        MathSSEFn->Push( rhs.B );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( vRes.B );
        return vRes;
    #else
        return TColor3<Double>( R - rhs.R, G - rhs.G, B - rhs.B );
    #endif
}
template<typename Real>
inline TColor3<Real> TColor3<Real>::operator*(const TColor3<Real> & rhs) const {
    return TColor3<Real>( R * rhs.R, G * rhs.G, B * rhs.B );
}
template<>
inline TColor3<Float> TColor3<Float>::operator*(const TColor3<Float> & rhs) const {
    #ifdef MATH_USE_SSE
        TColor3<Float> vRes;
        MathSSEFn->Push( R, G, B );
        MathSSEFn->Push( rhs.R, rhs.G, rhs.B );
        MathSSEFn->MulPF();
        MathSSEFn->Pop( vRes.R, vRes.G, vRes.B );
        return vRes;
    #else
        return TColor3<Float>( R * rhs.R, G * rhs.G, B * rhs.B );
    #endif
}
template<>
inline TColor3<Double> TColor3<Double>::operator*(const TColor3<Double> & rhs) const {
    #ifdef MATH_USE_SSE
        TColor3<Double> vRes;
        MathSSEFn->Push( R, G );
        MathSSEFn->Push( rhs.R, rhs.G );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( vRes.R, vRes.G );
        MathSSEFn->Push( B );
        MathSSEFn->Push( rhs.B );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( vRes.B );
        return vRes;
    #else
        return TColor3<Double>( R * rhs.R, G * rhs.G, B * rhs.B );
    #endif
}
template<typename Real>
inline TColor3<Real> TColor3<Real>::operator/(const TColor3<Real> & rhs) const {
    return TColor3<Real>( R / rhs.R, G / rhs.G, B / rhs.B );
}
template<>
inline TColor3<Float> TColor3<Float>::operator/(const TColor3<Float> & rhs) const {
    #ifdef MATH_USE_SSE
        TColor3<Float> vRes;
        MathSSEFn->Push( R, G, B );
        MathSSEFn->Push( rhs.R, rhs.G, rhs.B );
        MathSSEFn->DivPF();
        MathSSEFn->Pop( vRes.R, vRes.G, vRes.B );
        return vRes;
    #else
        return TColor3<Float>( R / rhs.R, G / rhs.G, B / rhs.B );
    #endif
}
template<>
inline TColor3<Double> TColor3<Double>::operator/(const TColor3<Double> & rhs) const {
    #ifdef MATH_USE_SSE
        TColor3<Double> vRes;
        MathSSEFn->Push( R, G );
        MathSSEFn->Push( rhs.R, rhs.G );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( vRes.R, vRes.G );
        MathSSEFn->Push( B );
        MathSSEFn->Push( rhs.B );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( vRes.B );
        return vRes;
    #else
        return TColor3<Double>( R / rhs.R, G / rhs.G, B / rhs.B );
    #endif
}

template<typename Real>
inline TColor3<Real> & TColor3<Real>::operator+=(const TColor3<Real> & rhs) {
    R += rhs.R;
    G += rhs.G;
    B += rhs.B;
    return (*this);
}
template<>
inline TColor3<Float> & TColor3<Float>::operator+=(const TColor3<Float> & rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( R, G, B );
        MathSSEFn->Push( rhs.R, rhs.G, rhs.B );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( R, G, B );
    #else
        R += rhs.R;
        G += rhs.G;
        B += rhs.B;
    #endif
    return (*this);
}
template<>
inline TColor3<Double> & TColor3<Double>::operator+=(const TColor3<Double> & rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( R, G );
        MathSSEFn->Push( rhs.R, rhs.G );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( R, G );
        MathSSEFn->Push( B );
        MathSSEFn->Push( rhs.B );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( B );
    #else
        R += rhs.R;
        G += rhs.G;
        B += rhs.B;
    #endif
    return (*this);
}
template<typename Real>
inline TColor3<Real> & TColor3<Real>::operator-=(const TColor3<Real> & rhs) {
    R -= rhs.R;
    G -= rhs.G;
    B -= rhs.B;
    return (*this);
}
template<>
inline TColor3<Float> & TColor3<Float>::operator-=(const TColor3<Float> & rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( R, G, B );
        MathSSEFn->Push( rhs.R, rhs.G, rhs.B );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( R, G, B );
    #else
        R -= rhs.R;
        G -= rhs.G;
        B -= rhs.B;
    #endif
    return (*this);
}
template<>
inline TColor3<Double> & TColor3<Double>::operator-=(const TColor3<Double> & rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( R, G );
        MathSSEFn->Push( rhs.R, rhs.G );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( R, G );
        MathSSEFn->Push( B );
        MathSSEFn->Push( rhs.B );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( B );
    #else
        R -= rhs.R;
        G -= rhs.G;
        B -= rhs.B;
    #endif
    return (*this);
}
template<typename Real>
inline TColor3<Real> & TColor3<Real>::operator*=(const TColor3<Real> & rhs) {
    R *= rhs.R;
    G *= rhs.G;
    B *= rhs.B;
    return (*this);
}
template<>
inline TColor3<Float> & TColor3<Float>::operator*=(const TColor3<Float> & rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( R, G, B );
        MathSSEFn->Push( rhs.R, rhs.G, rhs.B );
        MathSSEFn->MulPF();
        MathSSEFn->Pop( R, G, B );
    #else
        R *= rhs.R;
        G *= rhs.G;
        B *= rhs.B;
    #endif
    return (*this);
}
template<>
inline TColor3<Double> & TColor3<Double>::operator*=(const TColor3<Double> & rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( R, G );
        MathSSEFn->Push( rhs.R, rhs.G );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( R, G );
        MathSSEFn->Push( B );
        MathSSEFn->Push( rhs.B );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( B );
    #else
        R *= rhs.R;
        G *= rhs.G;
        B *= rhs.B;
    #endif
    return (*this);
}
template<typename Real>
inline TColor3<Real> & TColor3<Real>::operator/=(const TColor3<Real> & rhs) {
    R /= rhs.R;
    G /= rhs.G;
    B /= rhs.B;
    return (*this);
}
template<>
inline TColor3<Float> & TColor3<Float>::operator/=(const TColor3<Float> & rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( R, G, B );
        MathSSEFn->Push( rhs.R, rhs.G, rhs.B );
        MathSSEFn->DivPF();
        MathSSEFn->Pop( R, G, B );
    #else
        R /= rhs.R;
        G /= rhs.G;
        B /= rhs.B;
    #endif
    return (*this);
}
template<>
inline TColor3<Double> & TColor3<Double>::operator/=(const TColor3<Double> & rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( R, G );
        MathSSEFn->Push( rhs.R, rhs.G );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( R, G );
        MathSSEFn->Push( B );
        MathSSEFn->Push( rhs.B );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( B );
    #else
        R /= rhs.R;
        G /= rhs.G;
        B /= rhs.B;
    #endif
    return (*this);
}

template<typename Real>
inline Void TColor3<Real>::Clamp() {
    R = Clamp<Real>( R, ZERO, ONE );
    G = Clamp<Real>( G, ZERO, ONE );
    B = Clamp<Real>( B, ZERO, ONE );
}

template<typename Real>
inline Void TColor3<Real>::FromYCbCr( Real fY, Real fCb, Real fCr ) {
    R = fY + (fCr * (Real)1.402f);
	G = fY - (fCb * (Real)0.34414f) - (fCr * (Real)0.71414f);
	B = fY + (fCb * (Real)1.772f);
}
template<typename Real>
inline Void TColor3<Real>::ToYCbCr( Real & outY, Real & outCb, Real & outCr ) const {
    outY =  (R * (Real)0.299f)   + (G * (Real)0.587f)  + (B * (Real)0.114f);
	outCb = (R * (Real)-0.1687f) - (G * (Real)0.3313f) + (B * (Real)0.5f);
	outCr = (R * (Real)0.5f)     - (G * (Real)0.4187f) - (B * (Real)0.0813f);
}

template<typename Real>
inline Void TColor3<Real>::ToMonochrome( TColor3<Real> & outMonochrome ) const {
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
inline Void TColor3<Real>::ToGrayscale( TColor3<Real> & outGrayscale ) const {
    Real Y, Cb, Cr;
    ToYCbCr(Y, Cb, Cr);
    outGrayscale.FromYCbCr( ZERO, Cb, Cr );
}

