/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Vertex/Vertex2.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : 2D vertex
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TVertex2 implementation
template<typename Real>
const TVertex2<Real> TVertex2<Real>::Null = TVertex2<Real>( (Real)0, (Real)0 );

template<typename Real>
TVertex2<Real>::TVertex2()
{
    // nothing to do
}
template<typename Real>
TVertex2<Real>::TVertex2(Real x, Real y)
{
    X = x;
    Y = y;
}
template<typename Real>
TVertex2<Real>::TVertex2(const Real vArr[2])
{
    X = vArr[0];
    Y = vArr[1];
}
template<typename Real>
TVertex2<Real>::TVertex2(const TVertex2<Real> & rhs)
{
    X = rhs.X;
    Y = rhs.Y;
}
template<typename Real>
TVertex2<Real>::TVertex2(const TVertex3<Real> & rhs)
{
    X = rhs.X;
    Y = rhs.Y;
}
template<typename Real>
TVertex2<Real>::TVertex2(const TVertex4<Real> & rhs)
{
    X = rhs.X;
    Y = rhs.Y;
}
template<typename Real>
TVertex2<Real>::~TVertex2()
{
    // nothing to do
}

template<typename Real>
inline TVertex2<Real> & TVertex2<Real>::operator=(const TVertex2<Real> & rhs) {
    X = rhs.X;
    Y = rhs.Y;
    return (*this);
}

template<typename Real>
inline TVertex2<Real>::operator Real*() const {
    return (Real*)this;
}
template<typename Real>
inline TVertex2<Real>::operator const Real*() const {
    return (const Real*)this;
}

template<typename Real>
inline TVector2<Real> TVertex2<Real>::ToVector() const {
    return TVector2<Real>( X, Y );
}

template<typename Real>
inline Real & TVertex2<Real>::operator[](Int i) {
    return *( ((Real*)this) + i );
}
template<typename Real>
inline Real TVertex2<Real>::operator[](Int i) const {
    return *( ((const Real*)this) + i );
}
template<typename Real>
inline Real & TVertex2<Real>::operator[](UInt i) {
    return *( ((Real*)this) + i );
}
template<typename Real>
inline Real TVertex2<Real>::operator[](UInt i) const {
    return *( ((const Real*)this) + i );
}

template<typename Real>
inline TVertex2<Real> TVertex2<Real>::operator+() const {
    return TVertex2<Real>(X, Y);
}
template<typename Real>
inline TVertex2<Real> TVertex2<Real>::operator-() const {
    return TVertex2<Real>(-X, -Y);
}

template<typename Real>
inline Bool TVertex2<Real>::operator==(const TVertex2<Real> & rhs) const {
    return ( MathRealFn->Equals(X,rhs.X) && MathRealFn->Equals(Y,rhs.Y) );
}
template<typename Real>
inline Bool TVertex2<Real>::operator!=(const TVertex2<Real> & rhs) const {
    return ( !(MathRealFn->Equals(X,rhs.X)) || !(MathRealFn->Equals(Y,rhs.Y)) );
}

template<typename Real>
inline TVertex2<Real> TVertex2<Real>::operator+(Real rhs) const {
    return TVertex2<Real>( X + rhs, Y + rhs );
}
template<>
inline TVertex2<Float> TVertex2<Float>::operator+(Float rhs) const {
    #ifdef MATH_USE_SSE
        TVertex2<Float> vRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( vRes.X, vRes.Y );
        return vRes;
    #else
        return TVertex2<Float>( X + rhs, Y + rhs );
    #endif
}
template<>
inline TVertex2<Double> TVertex2<Double>::operator+(Double rhs) const {
    #ifdef MATH_USE_SSE
        TVertex2<Double> vRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( vRes.X, vRes.Y );
        return vRes;
    #else
        return TVertex2<Double>( X + rhs, Y + rhs );
    #endif
}
template<typename Real>
inline TVertex2<Real> TVertex2<Real>::operator-(Real rhs) const {
    return TVertex2<Real>( X - rhs, Y - rhs );
}
template<>
inline TVertex2<Float> TVertex2<Float>::operator-(Float rhs) const {
    #ifdef MATH_USE_SSE
        TVertex2<Float> vRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( vRes.X, vRes.Y );
        return vRes;
    #else
        return TVertex2<Float>( X - rhs, Y - rhs );
    #endif
}
template<>
inline TVertex2<Double> TVertex2<Double>::operator-(Double rhs) const {
    #ifdef MATH_USE_SSE
        TVertex2<Double> vRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( vRes.X, vRes.Y );
        return vRes;
    #else
        return TVertex2<Double>( X - rhs, Y - rhs );
    #endif
}
template<typename Real>
inline TVertex2<Real> TVertex2<Real>::operator*(Real rhs) const {
    return TVertex2<Real>( X * rhs, Y * rhs );
}
template<>
inline TVertex2<Float> TVertex2<Float>::operator*(Float rhs) const {
    #ifdef MATH_USE_SSE
        TVertex2<Float> vRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->MulPF();
        MathSSEFn->Pop( vRes.X, vRes.Y );
        return vRes;
    #else
        return TVertex2<Float>( X * rhs, Y * rhs );
    #endif
}
template<>
inline TVertex2<Double> TVertex2<Double>::operator*(Double rhs) const {
    #ifdef MATH_USE_SSE
        TVertex2<Double> vRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( vRes.X, vRes.Y );
        return vRes;
    #else
        return TVertex2<Double>( X * rhs, Y * rhs );
    #endif
}
template<typename Real>
inline TVertex2<Real> TVertex2<Real>::operator/(Real rhs) const {
    return TVertex2<Real>( X / rhs, Y / rhs );
}
template<>
inline TVertex2<Float> TVertex2<Float>::operator/(Float rhs) const {
    #ifdef MATH_USE_SSE
        TVertex2<Float> vRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->DivPF();
        MathSSEFn->Pop( vRes.X, vRes.Y );
        return vRes;
    #else
        return TVertex2<Float>( X / rhs, Y / rhs );
    #endif
}
template<>
inline TVertex2<Double> TVertex2<Double>::operator/(Double rhs) const {
    #ifdef MATH_USE_SSE
        TVertex2<Double> vRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( vRes.X, vRes.Y );
        return vRes;
    #else
        return TVertex2<Double>( X / rhs, Y / rhs );
    #endif
}

template<typename Real>
inline TVertex2<Real> & TVertex2<Real>::operator+=(Real rhs) {
    X += rhs;
    Y += rhs;
    return (*this);
}
template<>
inline TVertex2<Float> & TVertex2<Float>::operator+=(Float rhs) {
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
inline TVertex2<Double> & TVertex2<Double>::operator+=(Double rhs) {
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
inline TVertex2<Real> & TVertex2<Real>::operator-=(Real rhs) {
    X -= rhs;
    Y -= rhs;
    return (*this);
}
template<>
inline TVertex2<Float> & TVertex2<Float>::operator-=(Float rhs) {
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
inline TVertex2<Double> & TVertex2<Double>::operator-=(Double rhs) {
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
inline TVertex2<Real> & TVertex2<Real>::operator*=(Real rhs) {
    X *= rhs;
    Y *= rhs;
    return (*this);
}
template<>
inline TVertex2<Float> & TVertex2<Float>::operator*=(Float rhs) {
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
inline TVertex2<Double> & TVertex2<Double>::operator*=(Double rhs) {
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
inline TVertex2<Real> & TVertex2<Real>::operator/=(Real rhs) {
    X /= rhs;
    Y /= rhs;
    return (*this);
}
template<>
inline TVertex2<Float> & TVertex2<Float>::operator/=(Float rhs) {
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
inline TVertex2<Double> & TVertex2<Double>::operator/=(Double rhs) {
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
inline TVector2<Real> TVertex2<Real>::operator-(const TVertex2<Real> & rhs) const {
    return TVector2<Real>( X - rhs.X, Y - rhs.Y );
}
template<>
inline TVector2<Float> TVertex2<Float>::operator-(const TVertex2<Float> & rhs) const {
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
inline TVector2<Double> TVertex2<Double>::operator-(const TVertex2<Double> & rhs) const {
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
inline TVertex2<Real> TVertex2<Real>::operator+(const TVector2<Real> & rhs) const {
    return TVertex2<Real>( X + rhs.X, Y + rhs.Y );
}
template<>
inline TVertex2<Float> TVertex2<Float>::operator+(const TVector2<Float> & rhs) const {
    #ifdef MATH_USE_SSE
        TVertex2<Float> vRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs.X, rhs.Y );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( vRes.X, vRes.Y );
        return vRes;
    #else
        return TVertex2<Float>( X + rhs.X, Y + rhs.Y );
    #endif
}
template<>
inline TVertex2<Double> TVertex2<Double>::operator+(const TVector2<Double> & rhs) const {
    #ifdef MATH_USE_SSE
        TVertex2<Double> vRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs.X, rhs.Y );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( vRes.X, vRes.Y );
        return vRes;
    #else
        return TVertex2<Double>( X + rhs.X, Y + rhs.Y );
    #endif
}
template<typename Real>
inline TVertex2<Real> TVertex2<Real>::operator-(const TVector2<Real> & rhs) const {
    return TVertex2<Real>( X - rhs.X, Y - rhs.Y );
}
template<>
inline TVertex2<Float> TVertex2<Float>::operator-(const TVector2<Float> & rhs) const {
    #ifdef MATH_USE_SSE
        TVertex2<Float> vRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs.X, rhs.Y );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( vRes.X, vRes.Y );
        return vRes;
    #else
        return TVertex2<Float>( X - rhs.X, Y - rhs.Y );
    #endif
}
template<>
inline TVertex2<Double> TVertex2<Double>::operator-(const TVector2<Double> & rhs) const {
    #ifdef MATH_USE_SSE
        TVertex2<Double> vRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs.X, rhs.Y );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( vRes.X, vRes.Y );
        return vRes;
    #else
        return TVertex2<Double>( X - rhs.X, Y - rhs.Y );
    #endif
}

template<typename Real>
inline TVertex2<Real> & TVertex2<Real>::operator+=(const TVector2<Real> & rhs) {
    X += rhs.X;
    Y += rhs.Y;
    return (*this);
}
template<>
inline TVertex2<Float> & TVertex2<Float>::operator+=(const TVector2<Float> & rhs) {
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
inline TVertex2<Double> & TVertex2<Double>::operator+=(const TVector2<Double> & rhs) {
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
inline TVertex2<Real> & TVertex2<Real>::operator-=(const TVector2<Real> & rhs) {
    X -= rhs.X;
    Y -= rhs.Y;
    return (*this);
}
template<>
inline TVertex2<Float> & TVertex2<Float>::operator-=(const TVector2<Float> & rhs) {
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
inline TVertex2<Double> & TVertex2<Double>::operator-=(const TVector2<Double> & rhs) {
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
inline Real TVertex2<Real>::DistSqr() const {
    return ( X*X + Y*Y );
}
template<>
inline Float TVertex2<Float>::DistSqr() const {
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
inline Double TVertex2<Double>::DistSqr() const {
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
inline Real TVertex2<Real>::Dist() const {
    return MathRealFn->Sqrt( DistSqr() );
}
template<typename Real>
inline Real TVertex2<Real>::InvDistSqr() const {
    return MathRealFn->Invert( DistSqr() );
}
template<typename Real>
inline Real TVertex2<Real>::InvDist() const {
    return MathRealFn->InvSqrt( DistSqr() );
}

template<typename Real>
inline Void TVertex2<Real>::FromPolar( Real fRadius, Real fTheta ) {
    Real fSin, fCos;
	MathRealFn->SinCos( &fSin, &fCos, fTheta );
	X = fRadius * fCos;
	Y = fRadius * fSin;
}
template<typename Real>
inline Void TVertex2<Real>::ToPolar( Real & outRadius, Real & outTheta ) const {
    outRadius = Dist();
	outTheta = MathRealFn->ArcTan2(Y, X);
}

