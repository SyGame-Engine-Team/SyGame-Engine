/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Vertex/Vertex3.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : 3D vertex
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TVertex3 implementation
template<typename Real>
const TVertex3<Real> TVertex3<Real>::Null = TVertex3<Real>( (Real)0, (Real)0, (Real)0 );

template<typename Real>
TVertex3<Real>::TVertex3()
{
    // nothing to do
}
template<typename Real>
TVertex3<Real>::TVertex3(Real x, Real y, Real z)
{
    X = x;
    Y = y;
    Z = z;
}
template<typename Real>
TVertex3<Real>::TVertex3(const Real vArr[3])
{
    X = vArr[0];
    Y = vArr[1];
    Z = vArr[2];
}
template<typename Real>
TVertex3<Real>::TVertex3(const TVertex2<Real> & rhs)
{
    X = rhs.X;
    Y = rhs.Y;
    Z = (Real)0;
}
template<typename Real>
TVertex3<Real>::TVertex3(const TVertex3<Real> & rhs)
{
    X = rhs.X;
    Y = rhs.Y;
    Z = rhs.Z;
}
template<typename Real>
TVertex3<Real>::TVertex3(const TVertex4<Real> & rhs)
{
    X = rhs.X;
    Y = rhs.Y;
    Z = rhs.Z;
}
template<typename Real>
TVertex3<Real>::~TVertex3()
{
    // nothing to do
}

template<typename Real>
inline TVertex3<Real> & TVertex3<Real>::operator=(const TVertex3<Real> & rhs) {
    X = rhs.X;
    Y = rhs.Y;
    Z = rhs.Z;
    return (*this);
}

template<typename Real>
inline TVertex3<Real>::operator Real*() const {
    return (Real*)this;
}
template<typename Real>
inline TVertex3<Real>::operator const Real*() const {
    return (const Real*)this;
}

template<typename Real>
inline TVector3<Real> TVertex3<Real>::ToVector() const {
    return TVector3<Real>( X, Y, Z );
}

template<typename Real>
inline Real & TVertex3<Real>::operator[](Int i) {
    return *( ((Real*)this) + i );
}
template<typename Real>
inline Real TVertex3<Real>::operator[](Int i) const {
    return *( ((const Real*)this) + i );
}
template<typename Real>
inline Real & TVertex3<Real>::operator[](UInt i) {
    return *( ((Real*)this) + i );
}
template<typename Real>
inline Real TVertex3<Real>::operator[](UInt i) const {
    return *( ((const Real*)this) + i );
}

template<typename Real>
inline TVertex3<Real> TVertex3<Real>::operator+() const {
    return TVertex3<Real>(X, Y, Z);
}
template<typename Real>
inline TVertex3<Real> TVertex3<Real>::operator-() const {
    return TVertex3<Real>(-X, -Y, -Z);
}

template<typename Real>
inline Bool TVertex3<Real>::operator==(const TVertex3<Real> & rhs) const {
    return ( MathRealFn->Equals(X,rhs.X) && MathRealFn->Equals(Y,rhs.Y) &&
             MathRealFn->Equals(Z,rhs.Z) );
}
template<typename Real>
inline Bool TVertex3<Real>::operator!=(const TVertex3<Real> & rhs) const {
    return ( !(MathRealFn->Equals(X,rhs.X)) || !(MathRealFn->Equals(Y,rhs.Y)) ||
             !(MathRealFn->Equals(Z,rhs.Z)) );
}

template<typename Real>
inline TVertex3<Real> TVertex3<Real>::operator+(Real rhs) const {
    return TVertex3<Real>( X + rhs, Y + rhs, Z + rhs );
}
template<>
inline TVertex3<Float> TVertex3<Float>::operator+(Float rhs) const {
    #ifdef MATH_USE_SSE
        TVertex3<Float> vRes;
        MathSSEFn->Push( X, Y, Z );
        MathSSEFn->Push( rhs, rhs, rhs );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( vRes.X, vRes.Y, vRes.Z );
        return vRes;
    #else
        return TVertex3<Float>( X + rhs, Y + rhs, Z + rhs );
    #endif
}
template<>
inline TVertex3<Double> TVertex3<Double>::operator+(Double rhs) const {
    #ifdef MATH_USE_SSE
        TVertex3<Double> vRes;
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
        return TVertex3<Double>( X + rhs, Y + rhs, Z + rhs );
    #endif
}
template<typename Real>
inline TVertex3<Real> TVertex3<Real>::operator-(Real rhs) const {
    return TVertex3<Real>( X - rhs, Y - rhs, Z - rhs );
}
template<>
inline TVertex3<Float> TVertex3<Float>::operator-(Float rhs) const {
    #ifdef MATH_USE_SSE
        TVertex3<Float> vRes;
        MathSSEFn->Push( X, Y, Z );
        MathSSEFn->Push( rhs, rhs, rhs );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( vRes.X, vRes.Y, vRes.Z );
        return vRes;
    #else
        return TVertex3<Float>( X - rhs, Y - rhs, Z - rhs );
    #endif
}
template<>
inline TVertex3<Double> TVertex3<Double>::operator-(Double rhs) const {
    #ifdef MATH_USE_SSE
        TVertex3<Double> vRes;
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
        return TVertex3<Double>( X - rhs, Y - rhs, Z - rhs );
    #endif
}
template<typename Real>
inline TVertex3<Real> TVertex3<Real>::operator*(Real rhs) const {
    return TVertex3<Real>( X * rhs, Y * rhs, Z * rhs );
}
template<>
inline TVertex3<Float> TVertex3<Float>::operator*(Float rhs) const {
    #ifdef MATH_USE_SSE
        TVertex3<Float> vRes;
        MathSSEFn->Push( X, Y, Z );
        MathSSEFn->Push( rhs, rhs, rhs );
        MathSSEFn->MulPF();
        MathSSEFn->Pop( vRes.X, vRes.Y, vRes.Z );
        return vRes;
    #else
        return TVertex3<Float>( X * rhs, Y * rhs, Z * rhs );
    #endif
}
template<>
inline TVertex3<Double> TVertex3<Double>::operator*(Double rhs) const {
    #ifdef MATH_USE_SSE
        TVertex3<Double> vRes;
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
        return TVertex3<Double>( X * rhs, Y * rhs, Z * rhs );
    #endif
}
template<typename Real>
inline TVertex3<Real> TVertex3<Real>::operator/(Real rhs) const {
    return TVertex3<Real>( X / rhs, Y / rhs, Z / rhs );
}
template<>
inline TVertex3<Float> TVertex3<Float>::operator/(Float rhs) const {
    #ifdef MATH_USE_SSE
        TVertex3<Float> vRes;
        MathSSEFn->Push( X, Y, Z );
        MathSSEFn->Push( rhs, rhs, rhs );
        MathSSEFn->DivPF();
        MathSSEFn->Pop( vRes.X, vRes.Y, vRes.Z );
        return vRes;
    #else
        return TVertex3<Float>( X / rhs, Y / rhs, Z / rhs );
    #endif
}
template<>
inline TVertex3<Double> TVertex3<Double>::operator/(Double rhs) const {
    #ifdef MATH_USE_SSE
        TVertex3<Double> vRes;
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
        return TVertex3<Double>( X / rhs, Y / rhs, Z / rhs );
    #endif
}

template<typename Real>
inline TVertex3<Real> & TVertex3<Real>::operator+=(Real rhs) {
    X += rhs;
    Y += rhs;
    Z += rhs;
    return (*this);
}
template<>
inline TVertex3<Float> & TVertex3<Float>::operator+=(Float rhs) {
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
inline TVertex3<Double> & TVertex3<Double>::operator+=(Double rhs) {
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
inline TVertex3<Real> & TVertex3<Real>::operator-=(Real rhs) {
    X -= rhs;
    Y -= rhs;
    Z -= rhs;
    return (*this);
}
template<>
inline TVertex3<Float> & TVertex3<Float>::operator-=(Float rhs) {
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
inline TVertex3<Double> & TVertex3<Double>::operator-=(Double rhs) {
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
inline TVertex3<Real> & TVertex3<Real>::operator*=(Real rhs) {
    X *= rhs;
    Y *= rhs;
    Z *= rhs;
    return (*this);
}
template<>
inline TVertex3<Float> & TVertex3<Float>::operator*=(Float rhs) {
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
inline TVertex3<Double> & TVertex3<Double>::operator*=(Double rhs) {
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
inline TVertex3<Real> & TVertex3<Real>::operator/=(Real rhs) {
    X /= rhs;
    Y /= rhs;
    Z /= rhs;
    return (*this);
}
template<>
inline TVertex3<Float> & TVertex3<Float>::operator/=(Float rhs) {
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
inline TVertex3<Double> & TVertex3<Double>::operator/=(Double rhs) {
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
inline TVector3<Real> TVertex3<Real>::operator-(const TVertex3<Real> & rhs) const {
    return TVector3<Real>( X - rhs.X, Y - rhs.Y, Z - rhs.Z );
}
template<>
inline TVector3<Float> TVertex3<Float>::operator-(const TVertex3<Float> & rhs) const {
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
inline TVector3<Double> TVertex3<Double>::operator-(const TVertex3<Double> & rhs) const {
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
inline TVertex3<Real> TVertex3<Real>::operator+(const TVector3<Real> & rhs) const {
    return TVertex3<Real>( X + rhs.X, Y + rhs.Y, Z + rhs.Z );
}
template<>
inline TVertex3<Float> TVertex3<Float>::operator+(const TVector3<Float> & rhs) const {
    #ifdef MATH_USE_SSE
        TVertex3<Float> vRes;
        MathSSEFn->Push( X, Y, Z );
        MathSSEFn->Push( rhs.X, rhs.Y, rhs.Z );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( vRes.X, vRes.Y, vRes.Z );
        return vRes;
    #else
        return TVertex3<Float>( X + rhs.X, Y + rhs.Y, Z + rhs.Z );
    #endif
}
template<>
inline TVertex3<Double> TVertex3<Double>::operator+(const TVector3<Double> & rhs) const {
    #ifdef MATH_USE_SSE
        TVertex3<Double> vRes;
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
        return TVertex3<Double>( X + rhs.X, Y + rhs.Y, Z + rhs.Z );
    #endif
}
template<typename Real>
inline TVertex3<Real> TVertex3<Real>::operator-(const TVector3<Real> & rhs) const {
    return TVertex3<Real>( X - rhs.X, Y - rhs.Y, Z - rhs.Z );
}
template<>
inline TVertex3<Float> TVertex3<Float>::operator-(const TVector3<Float> & rhs) const {
    #ifdef MATH_USE_SSE
        TVertex3<Float> vRes;
        MathSSEFn->Push( X, Y, Z );
        MathSSEFn->Push( rhs.X, rhs.Y, rhs.Z );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( vRes.X, vRes.Y, vRes.Z );
        return vRes;
    #else
        return TVertex3<Float>( X - rhs.X, Y - rhs.Y, Z - rhs.Z );
    #endif
}
template<>
inline TVertex3<Double> TVertex3<Double>::operator-(const TVector3<Double> & rhs) const {
    #ifdef MATH_USE_SSE
        TVertex3<Double> vRes;
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
        return TVertex3<Double>( X - rhs.X, Y - rhs.Y, Z - rhs.Z );
    #endif
}

template<typename Real>
inline TVertex3<Real> & TVertex3<Real>::operator+=(const TVector3<Real> & rhs) {
    X += rhs.X;
    Y += rhs.Y;
    Z += rhs.Z;
    return (*this);
}
template<>
inline TVertex3<Float> & TVertex3<Float>::operator+=(const TVector3<Float> & rhs) {
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
inline TVertex3<Double> & TVertex3<Double>::operator+=(const TVector3<Double> & rhs) {
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
inline TVertex3<Real> & TVertex3<Real>::operator-=(const TVector3<Real> & rhs) {
    X -= rhs.X;
    Y -= rhs.Y;
    Z -= rhs.Z;
    return (*this);
}
template<>
inline TVertex3<Float> & TVertex3<Float>::operator-=(const TVector3<Float> & rhs) {
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
inline TVertex3<Double> & TVertex3<Double>::operator-=(const TVector3<Double> & rhs) {
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
inline Real TVertex3<Real>::DistSqr() const {
    return ( X*X + Y*Y + Z*Z );
}
template<>
inline Float TVertex3<Float>::DistSqr() const {
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
inline Double TVertex3<Double>::DistSqr() const {
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
inline Real TVertex3<Real>::Dist() const {
    return MathRealFn->Sqrt( DistSqr() );
}
template<typename Real>
inline Real TVertex3<Real>::InvDistSqr() const {
    return MathRealFn->Invert( DistSqr() );
}
template<typename Real>
inline Real TVertex3<Real>::InvDist() const {
    return MathRealFn->InvSqrt( DistSqr() );
}

template<typename Real>
inline Void TVertex3<Real>::FromCylindric( Real fRadius, Real fTheta, Real fZ ) {
    Real fSin, fCos;
	MathRealFn->SinCos( &fSin, &fCos, fTheta );
	X = fRadius * fCos;
	Y = fRadius * fSin;
	Z = fZ;
}
template<typename Real>
inline Void TVertex3<Real>::ToCylindric( Real & outRadius, Real & outTheta, Real & outZ ) const {
    outRadius = Dist();
	outTheta = MathRealFn->ArcTan2(Y, X);
	outZ = Z;
}

template<typename Real>
inline Void TVertex3<Real>::FromSpherical( Real fRadius, Real fTheta, Real fPhi ) {
    Real sinT, cosT, sinP, cosP;
	MathRealFn->SinCos( &sinT, &cosT, fTheta );
	MathRealFn->SinCos( &sinP, &cosP, fPhi );
    Real fTmp = fRadius * sinP;
	X = fTmp * cosT;
	Y = fTmp * sinT;
	Z = fRadius * cosP;
}
template<typename Real>
inline Void TVertex3<Real>::ToSpherical( Real & outRadius, Real & outTheta, Real & outPhi ) const {
    outRadius = Dist();
	outTheta = MathRealFn->ArcTan2(Y, X);
	outPhi = MathRealFn->ArcCos(Z / outRadius);
}


