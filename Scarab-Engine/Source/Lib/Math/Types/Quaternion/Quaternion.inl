/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Quaternion/Quaternion.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Homogeneous 4D quaternion
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TQuaternion implementation
template<typename Real>
const Real TQuaternion<Real>::ZERO = (Real)0;
template<typename Real>
const Real TQuaternion<Real>::ONE = (Real)1;
template<typename Real>
const Real TQuaternion<Real>::TWO = (Real)2;

template<typename Real>
const TQuaternion<Real> TQuaternion<Real>::Null = TQuaternion<Real>( ZERO, ZERO, ZERO, ZERO );
template<typename Real>
const TQuaternion<Real> TQuaternion<Real>::Identity = TQuaternion<Real>( ZERO, ZERO, ZERO, ONE );

template<typename Real>
TQuaternion<Real>::TQuaternion()
{
    // nothing to do
}
template<typename Real>
TQuaternion<Real>::TQuaternion(Real x, Real y, Real z, Real w)
{
    X = x;
    Y = y;
    Z = z;
    W = w;
}
template<typename Real>
TQuaternion<Real>::TQuaternion(const Real vArr[4])
{
    X = vArr[0];
    Y = vArr[1];
    Z = vArr[2];
    W = vArr[3];
}
template<typename Real>
TQuaternion<Real>::TQuaternion(const TVector4<Real> & vQuaternion)
{
    X = vQuaternion.X;
    Y = vQuaternion.Y;
    Z = vQuaternion.Z;
    W = vQuaternion.W;
}
template<typename Real>
TQuaternion<Real>::TQuaternion(const TQuaternion<Real> & rhs)
{
    X = rhs.X;
    Y = rhs.Y;
    Z = rhs.Z;
    W = rhs.W;
}
template<typename Real>
TQuaternion<Real>::TQuaternion(const TVector3<Real> & vAxis, Real fAngle)
{
    _AxisAngle_To_Quaternion( vAxis, fAngle );
}
template<typename Real>
TQuaternion<Real>::TQuaternion(const TMatrix3<Real> & matRotation)
{
    _Matrix_To_Quaternion( matRotation );
}
template<typename Real>
TQuaternion<Real>::~TQuaternion()
{
    // nothing to do
}

template<typename Real>
inline TQuaternion<Real> & TQuaternion<Real>::operator=(const TQuaternion<Real> & rhs) {
    X = rhs.X;
    Y = rhs.Y;
    Z = rhs.Z;
    W = rhs.W;
    return (*this);
}

template<typename Real>
inline TQuaternion<Real>::operator Real*() const {
    return (Real*)this;
}
template<typename Real>
inline TQuaternion<Real>::operator const Real*() const {
    return (const Real*)this;
}

template<typename Real>
inline Real & TQuaternion<Real>::operator[](Int i) {
    return *( ((Real*)this) + i );
}
template<typename Real>
inline Real TQuaternion<Real>::operator[](Int i) const {
    return *( ((const Real*)this) + i );
}
template<typename Real>
inline Real & TQuaternion<Real>::operator[](UInt i) {
    return *( ((Real*)this) + i );
}
template<typename Real>
inline Real TQuaternion<Real>::operator[](UInt i) const {
    return *( ((const Real*)this) + i );
}

template<typename Real>
inline TQuaternion<Real> TQuaternion<Real>::operator+() const {
    return TQuaternion<Real>(X, Y, Z, W);
}
template<typename Real>
inline TQuaternion<Real> TQuaternion<Real>::operator-() const {
    return TQuaternion<Real>(-X, -Y, -Z, -W);
}
template<typename Real>
inline TQuaternion<Real> TQuaternion<Real>::operator~() const {
    return TQuaternion<Real>(-X, -Y, -Z, W);
}

template<typename Real>
inline Bool TQuaternion<Real>::operator==(const TQuaternion<Real> & rhs) const {
    return ( MathRealFn->Equals(X,rhs.X) && MathRealFn->Equals(Y,rhs.Y) &&
             MathRealFn->Equals(Z,rhs.Z) && MathRealFn->Equals(W,rhs.W) );
}
template<typename Real>
inline Bool TQuaternion<Real>::operator!=(const TQuaternion<Real> & rhs) const {
    return ( !(MathRealFn->Equals(X,rhs.X)) || !(MathRealFn->Equals(Y,rhs.Y)) ||
             !(MathRealFn->Equals(Z,rhs.Z)) || !(MathRealFn->Equals(W,rhs.W)) );
}

template<typename Real>
inline TQuaternion<Real> TQuaternion<Real>::operator*(Real rhs) const {
    return TQuaternion<Real>( X * rhs, Y * rhs, Z * rhs, W * rhs );
}
template<>
inline TQuaternion<Float> TQuaternion<Float>::operator*(Float rhs) const {
    #ifdef MATH_USE_SSE
        TQuaternion<Float> vRes;
        MathSSEFn->Push( X, Y, Z, W );
        MathSSEFn->Push( rhs, rhs, rhs, rhs );
        MathSSEFn->MulPF();
        MathSSEFn->Pop( vRes.X, vRes.Y, vRes.Z, vRes.W );
        return vRes;
    #else
        return TQuaternion<Float>( X * rhs, Y * rhs, Z * rhs, W * rhs );
    #endif
}
template<>
inline TQuaternion<Double> TQuaternion<Double>::operator*(Double rhs) const {
    #ifdef MATH_USE_SSE
        TQuaternion<Double> vRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( vRes.X, vRes.Y );
        MathSSEFn->Push( Z, W );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( vRes.Z, vRes.W );
        return vRes;
    #else
        return TQuaternion<Double>( X * rhs, Y * rhs, Z * rhs, W * rhs );
    #endif
}
template<typename Real>
inline TQuaternion<Real> TQuaternion<Real>::operator/(Real rhs) const {
    return TQuaternion<Real>( X / rhs, Y / rhs, Z / rhs, W / rhs );
}
template<>
inline TQuaternion<Float> TQuaternion<Float>::operator/(Float rhs) const {
    #ifdef MATH_USE_SSE
        TQuaternion<Float> vRes;
        MathSSEFn->Push( X, Y, Z, W );
        MathSSEFn->Push( rhs, rhs, rhs, rhs );
        MathSSEFn->DivPF();
        MathSSEFn->Pop( vRes.X, vRes.Y, vRes.Z, vRes.W );
        return vRes;
    #else
        return TQuaternion<Float>( X / rhs, Y / rhs, Z / rhs, W / rhs );
    #endif
}
template<>
inline TQuaternion<Double> TQuaternion<Double>::operator/(Double rhs) const {
    #ifdef MATH_USE_SSE
        TQuaternion<Double> vRes;
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( vRes.X, vRes.Y );
        MathSSEFn->Push( Z, W );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( vRes.Z, vRes.W );
        return vRes;
    #else
        return TQuaternion<Double>( X / rhs, Y / rhs, Z / rhs, W / rhs );
    #endif
}

template<typename Real>
inline TQuaternion<Real> & TQuaternion<Real>::operator*=(Real rhs) {
    X *= rhs;
    Y *= rhs;
    Z *= rhs;
    W *= rhs;
    return (*this);
}
template<>
inline TQuaternion<Float> & TQuaternion<Float>::operator*=(Float rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y, Z, W );
        MathSSEFn->Push( rhs, rhs, rhs, rhs );
        MathSSEFn->MulPF();
        MathSSEFn->Pop( X, Y, Z, W );
    #else
        X *= rhs;
        Y *= rhs;
        Z *= rhs;
        W *= rhs;
    #endif
    return (*this);
}
template<>
inline TQuaternion<Double> & TQuaternion<Double>::operator*=(Double rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( X, Y );
        MathSSEFn->Push( Z, W );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( Z, W );
    #else
        X *= rhs;
        Y *= rhs;
        Z *= rhs;
        W *= rhs;
    #endif
    return (*this);
}
template<typename Real>
inline TQuaternion<Real> & TQuaternion<Real>::operator/=(Real rhs) {
    X /= rhs;
    Y /= rhs;
    Z /= rhs;
    W /= rhs;
    return (*this);
}
template<>
inline TQuaternion<Float> & TQuaternion<Float>::operator/=(Float rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y, Z, W );
        MathSSEFn->Push( rhs, rhs, rhs, rhs );
        MathSSEFn->DivPF();
        MathSSEFn->Pop( X, Y, Z, W );
    #else
        X /= rhs;
        Y /= rhs;
        Z /= rhs;
        W /= rhs;
    #endif
    return (*this);
}
template<>
inline TQuaternion<Double> & TQuaternion<Double>::operator/=(Double rhs) {
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( X, Y );
        MathSSEFn->Push( Z, W );
        MathSSEFn->Push( rhs, rhs );
        MathSSEFn->DivPD();
        MathSSEFn->Pop( Z, W );
    #else
        X /= rhs;
        Y /= rhs;
        Z /= rhs;
        W /= rhs;
    #endif
    return (*this);
}

template<typename Real>
inline TVector3<Real> TQuaternion<Real>::operator*(const TVector3<Real> & rhs) const {
    TMatrix3<Real> matRot;
    _Quaternion_To_Matrix(matRot);
    return ( matRot * rhs );
}
template<typename Real>
inline TVector4<Real> TQuaternion<Real>::operator*(const TVector4<Real> & rhs) const {
    TMatrix3<Real> matRot;
    _Quaternion_To_Matrix(matRot);
    return ( TMatrix4<Real>(matRot) * rhs );
}

template<typename Real>
inline TQuaternion<Real> TQuaternion<Real>::operator+(const TQuaternion<Real> & rhs) const {
    return TQuaternion<Real>( X + rhs.X, Y + rhs.Y, Z + rhs.Z, W + rhs.W );
}
template<>
inline TQuaternion<Float> TQuaternion<Float>::operator+(const TQuaternion<Float> & rhs) const {
    #ifdef MATH_USE_SSE
        TQuaternion<Float> vRes;
        MathSSEFn->Push( X, Y, Z, W );
        MathSSEFn->Push( rhs.X, rhs.Y, rhs.Z, rhs.W );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( vRes.X, vRes.Y, vRes.Z, vRes.W );
        return vRes;
    #else
        return TQuaternion<Float>( X + rhs.X, Y + rhs.Y, Z + rhs.Z, W + rhs.W );
    #endif
}
template<>
inline TQuaternion<Double> TQuaternion<Double>::operator+(const TQuaternion<Double> & rhs) const {
    #ifdef MATH_USE_SSE
        TQuaternion<Double> vRes;
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
        return TQuaternion<Double>( X + rhs.X, Y + rhs.Y, Z + rhs.Z, W + rhs.W );
    #endif
}
template<typename Real>
inline TQuaternion<Real> TQuaternion<Real>::operator-(const TQuaternion<Real> & rhs) const {
    return TQuaternion<Real>( X - rhs.X, Y - rhs.Y, Z - rhs.Z, W - rhs.W );
}
template<>
inline TQuaternion<Float> TQuaternion<Float>::operator-(const TQuaternion<Float> & rhs) const {
    #ifdef MATH_USE_SSE
        TQuaternion<Float> vRes;
        MathSSEFn->Push( X, Y, Z, W );
        MathSSEFn->Push( rhs.X, rhs.Y, rhs.Z, rhs.W );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( vRes.X, vRes.Y, vRes.Z, vRes.W );
        return vRes;
    #else
        return TQuaternion<Float>( X - rhs.X, Y - rhs.Y, Z - rhs.Z, W - rhs.W );
    #endif
}
template<>
inline TQuaternion<Double> TQuaternion<Double>::operator-(const TQuaternion<Double> & rhs) const {
    #ifdef MATH_USE_SSE
        TQuaternion<Double> vRes;
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
        return TQuaternion<Double>( X - rhs.X, Y - rhs.Y, Z - rhs.Z, W - rhs.W );
    #endif
}

template<typename Real>
TQuaternion<Real> TQuaternion<Real>::operator*(const TQuaternion<Real> & rhs) const
{
    return TQuaternion<Real> (
	    (W * rhs.X) + (X * rhs.W) + (Y * rhs.Z - Z * rhs.Y),
	    (W * rhs.Y) + (Y * rhs.W) + (Z * rhs.X - X * rhs.Z),
	    (W * rhs.Z) + (Z * rhs.W) + (X * rhs.Y - Y * rhs.X),
	    (W * rhs.W) - (X * rhs.X + Y * rhs.Y + Z * rhs.Z)
    );
}

template<typename Real>
inline TQuaternion<Real> & TQuaternion<Real>::operator+=(const TQuaternion<Real> & rhs) {
    X += rhs.X;
    Y += rhs.Y;
    Z += rhs.Z;
    W += rhs.W;
    return (*this);
}
template<>
inline TQuaternion<Float> & TQuaternion<Float>::operator+=(const TQuaternion<Float> & rhs) {
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
inline TQuaternion<Double> & TQuaternion<Double>::operator+=(const TQuaternion<Double> & rhs) {
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
inline TQuaternion<Real> & TQuaternion<Real>::operator-=(const TQuaternion<Real> & rhs) {
    X -= rhs.X;
    Y -= rhs.Y;
    Z -= rhs.Z;
    W -= rhs.W;
    return (*this);
}
template<>
inline TQuaternion<Float> & TQuaternion<Float>::operator-=(const TQuaternion<Float> & rhs) {
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
inline TQuaternion<Double> & TQuaternion<Double>::operator-=(const TQuaternion<Double> & rhs) {
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
TQuaternion<Real> & TQuaternion<Real>::operator*=(const TQuaternion<Real> & rhs)
{
    Real f0 = (W * rhs.X) + (X * rhs.W) + (Y * rhs.Z - Z * rhs.Y);
    Real f1 = (W * rhs.Y) + (Y * rhs.W) + (Z * rhs.X - X * rhs.Z);
    Real f2 = (W * rhs.Z) + (Z * rhs.W) + (X * rhs.Y - Y * rhs.X);
    Real f3 = (W * rhs.W) - (X * rhs.X + Y * rhs.Y + Z * rhs.Z);
    X = f0;
    Y = f1;
    Z = f2;
    W = f3;
    return (*this);
}

template<typename Real>
inline Void TQuaternion<Real>::GetAxisAngle(TVector3<Real> & outAxis, Real & outAngle) const {
    _Quaternion_To_AxisAngle(outAxis, outAngle);
}
template<typename Real>
inline Void TQuaternion<Real>::GetMatrix(TMatrix3<Real> & outRotation) const {
    _Quaternion_To_Matrix(outRotation);
}

template<typename Real>
inline Real TQuaternion<Real>::NormSqr() const {
    return ( X*X + Y*Y + Z*Z + W*W );
}
template<>
inline Float TQuaternion<Float>::NormSqr() const {
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
inline Double TQuaternion<Double>::NormSqr() const {
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
inline Real TQuaternion<Real>::Norm() const {
    return MathRealFn->Sqrt( NormSqr() );
}
template<typename Real>
inline Real TQuaternion<Real>::InvNormSqr() const {
    return MathRealFn->Invert( NormSqr() );
}
template<typename Real>
inline Real TQuaternion<Real>::InvNorm() const {
    return MathRealFn->InvSqrt( NormSqr() );
}

template<typename Real>
inline Real TQuaternion<Real>::Normalize() {
    Real fNorm = Norm(), fInvNorm;
    if ( MathRealFn->EqualsZero(fNorm) ) {
        X = ZERO;
        Y = ZERO;
        Z = ZERO;
        W = ZERO;
        return ZERO;
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
inline Void TQuaternion<Real>::Nearest( TQuaternion<Real> & qOut, const TQuaternion<Real> & qTarget ) const {
    TQuaternion<Real> qDiff, qSum;
    qDiff = ( *this - qTarget );
    qSum = ( *this + qTarget );
    if ( qDiff.NormSqr() < qSum.NormSqr() )
        qOut = qTarget;
    else
        qOut = -qTarget;
}
template<typename Real>
inline Void TQuaternion<Real>::Farthest( TQuaternion<Real> & qOut, const TQuaternion<Real> & qTarget ) const {
    TQuaternion<Real> qDiff, qSum;
    qDiff = ( *this - qTarget );
    qSum = ( *this + qTarget );
    if ( qDiff.NormSqr() > qSum.NormSqr() )
        qOut = qTarget;
    else
        qOut = -qTarget;
}

template<typename Real>
inline Real TQuaternion<Real>::Dot(const TQuaternion<Real> & rhs) const {
    return ( (X * rhs.X) + (Y * rhs.Y) + (Z * rhs.Z) + (W * rhs.W) );
}
template<>
inline Float TQuaternion<Float>::Dot(const TQuaternion<Float> & rhs) const {
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
inline Double TQuaternion<Double>::Dot(const TQuaternion<Double> & rhs) const {
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
Void TQuaternion<Real>::Invert(TQuaternion<Real> & qOut) const
{
    Real fNormSquared = NormSqr();
    if ( fNormSquared > ZERO ) {
        Real fInvNormSquared = MathRealFn->Invert(fNormSquared);
        qOut.X = -X * fInvNormSquared;
        qOut.Y = -Y * fInvNormSquared;
        qOut.Z = -Z * fInvNormSquared;
        qOut.W = W * fInvNormSquared;
    } else {
        qOut.X = ZERO;
        qOut.Y = ZERO;
        qOut.Z = ZERO;
        qOut.W = ZERO;
    }
}

template<typename Real>
inline Void TQuaternion<Real>::InvertN(TQuaternion<Real> & qOut) const {
    qOut.X = -X;
    qOut.Y = -Y;
    qOut.Z = -Z;
    qOut.W = W;
}

template<typename Real>
Void TQuaternion<Real>::Exp(TQuaternion<Real> & qOut) const
{
    Real fAngle = MathRealFn->Sqrt( X*X + Y*Y + Z*Z );
    Real fSin, fCos;
    MathRealFn->SinCos( &fSin, &fCos, fAngle );
    if ( MathRealFn->EqualsZero(fSin) ) {
        qOut.X = X;
        qOut.Y = Y;
        qOut.Z = Z;
    } else {
        fSin /= fAngle;
        qOut.X = X * fSin;
        qOut.Y = Y * fSin;
        qOut.Z = Z * fSin;
    }
    qOut.W = fCos;
}

template<typename Real>
Void TQuaternion<Real>::Ln(TQuaternion<Real> & qOut) const
{
    if ( MathRealFn->Abs(W) < ONE ) {
        Real fAngle = MathRealFn->ArcCos(W);
        Real fSin = MathRealFn->Sin(fAngle);
        if ( MathRealFn->EqualsZero(fSin) ) {
            qOut.X = X;
            qOut.Y = Y;
            qOut.Z = Z;
        } else {
            fAngle /= fSin;
            qOut.X = X * fAngle;
            qOut.Y = Y * fAngle;
            qOut.Z = Z * fAngle;
        }
    } else {
        qOut.X = X;
        qOut.Y = Y;
        qOut.Z = Z;
    }
    qOut.W = ZERO;
}

template<typename Real>
Void TQuaternion<Real>::MakeRotate(Real fYaw, Real fPitch, Real fRoll, EulerAngles eulerAnglesOrder)
{
    static TVector3<Real> arrAxes[3] = { TVector3<Real>::eI, TVector3<Real>::eJ, TVector3<Real>::eK };
    TQuaternion<Real> qYaw(   arrAxes[(Axis)( (eulerAnglesOrder>>4) & 0x03 )], fYaw );
    TQuaternion<Real> qPitch( arrAxes[(Axis)( (eulerAnglesOrder>>2) & 0x03 )], fPitch );
    TQuaternion<Real> qRoll(  arrAxes[(Axis)( eulerAnglesOrder & 0x03 )], fRoll );
    (*this) = ( qRoll * qPitch * qYaw );
}

template<typename Real>
Void TQuaternion<Real>::MakeRotate(const TVector3<Real> & vV1, const TVector3<Real> & vV2)
{
    TVector3<Real> vCross = ( vV1 ^ vV2 );
    Real fDot = ( vV1 * vV2 );

    // Bisector is undefined, pick any orthogonal vector
    if ( fDot < (-ONE + (Real)SCALAR_EPSILON) ) {
        TVector3<Real> vTmp, vNormal;
        TVector3<Real>::MakeComplementBasis( vNormal, vTmp, vV1 );
        X = vNormal.X;
        Y = vNormal.Y;
        Z = vNormal.Z;
        W = 0.0f;
        return;
    }

    Real fBiSectorNorm = MathRealFn->Sqrt( 2.0f * (1.0f + fDot) );
    Real fInvBiSectorNorm = MathRealFn->Invert( fBiSectorNorm );
    X = vCross.X * fInvBiSectorNorm;
    Y = vCross.Y * fInvBiSectorNorm;
    Z = vCross.Z * fInvBiSectorNorm;
    W = 0.5f * fBiSectorNorm;
}

template<typename Real>
inline Void TQuaternion<Real>::SplitTwistSwing(const TVector3<Real> & vV1, TQuaternion<Real> & outTwist, TQuaternion<Real> & outSwing) {
    TVector3<Real> vV2 = ((*this) * vV1);
    outSwing.MakeRotate(vV1,vV2);
    outTwist = (*this) * (~outSwing);
}
template<typename Real>
inline Void TQuaternion<Real>::SplitSwingTwist(const TVector3<Real> & vV1, TQuaternion<Real> & outSwing, TQuaternion<Real> & outTwist) {
    TVector3<Real> vV2 = ((*this) * vV1);
    outSwing.MakeRotate(vV1,vV2);
    outTwist = (~outSwing) * (*this);
}

template<typename Real>
Void TQuaternion<Real>::SLerp(const TQuaternion<Real> & qSource, const TQuaternion<Real> & qTarget, Real fT)
{
    Real fCos = qSource.Dot(qTarget);
    Real fAngle = MathRealFn->ArcCos(fCos);
    if ( MathRealFn->EqualsZero(fAngle) ) {
        (*this) = qSource;
        return;
    }
    Real fInvSin = MathRealFn->Sin(fAngle);
    fInvSin = MathRealFn->Invert(fInvSin);
    Real fAngleT = ( fAngle * fT );
    Real fC0 = MathRealFn->Sin( fAngle - fAngleT ) * fInvSin;
    Real fC1 = MathRealFn->Sin( fAngleT ) * fInvSin;
    (*this) = ( (qSource * fC0) + (qTarget * fC1) );
}
template<typename Real>
Void TQuaternion<Real>::SLerpSpins(const TQuaternion<Real> & qSource, const TQuaternion<Real> & qTarget, Real fT, Int iSpins)
{
    Real fCos = qSource.Dot(qTarget);
    Real fAngle = MathRealFn->ArcCos(fCos);
    if ( MathRealFn->EqualsZero(fAngle) ) {
        (*this) = qSource;
        return;
    }
    Real fInvSin = MathRealFn->Sin(fAngle);
    fInvSin = MathRealFn->Invert(fInvSin);
    Real fPhase = ( ((Real)SCALAR_PI) * ((Real)iSpins) * fT );
    Real fC0 = MathRealFn->Sin( (ONE - fT) * fAngle - fPhase ) * fInvSin;
    Real fC1 = MathRealFn->Sin( fT * fTAngle + fPhase ) * fInvSin;
    (*this) = ( (qSource * fC0) + (qTarget * fC1) );
}
template<typename Real>
Void TQuaternion<Real>::SQuad(const TQuaternion<Real> & qQ0, const TQuaternion<Real> & qA0,
                              const TQuaternion<Real> & qA1, const TQuaternion<Real> & qQ1, Real fT) const
{
    TQuaternion<Real> qSlerpP, qSlerpQ;
    qSlerpP.SLerp( qQ0, qQ1, fT );
    qSlerpQ.SLerp( qA0, qA1, fT );
    Real fSlerpT = TWO * fT * (ONE - fT);
    SLerp( qSlerpP, qSlerpQ, fSlerpT );
}
template<typename Real>
Void TQuaternion<Real>::SQuadIntermediate(const TQuaternion<Real> & qQ0, const TQuaternion<Real> & qQ1, const TQuaternion<Real> & qQ2)
{
    TQuaternion<Real> qTmp1 = ~qQ1;
    TQuaternion<Real> qTmp0 = ( qTmp1 * qQ0 );
    TQuaternion<Real> qTmp2 = ( qTmp1 * qQ2 );
    TQuaternion<Real> qLnP0, qLnP2;
    qTmp0.Ln( qLnP0 );
    qTmp2.Ln( qLnP2 );
    qTmp1 = ( qLnP0 + qLnP2 ) * (Real)(-0.25f);
    qTmp1.Exp(qTmp0);
    (*this) = ( qQ1 * qTmp0 );
}

/////////////////////////////////////////////////////////////////////////////////

template<typename Real>
Void TQuaternion<Real>::_Quaternion_To_AxisAngle(TVector3<Real> & outAxis, Real & outAngle) const
{
    Real fLengthSquared = ( X*X + Y*Y + Z*Z );
    if ( fLengthSquared > ZERO ) {
        outAngle = TWO * MathRealFn->ArcCos(W);
        Real fInvLength = MathRealFn->InvSqrt(fLengthSquared);
        outAxis.X = X * fInvLength;
        outAxis.Y = Y * fInvLength;
        outAxis.Z = Z * fInvLength;
    } else {
        outAngle = ZERO;
        outAxis.X = ONE;
        outAxis.Y = ZERO;
        outAxis.Z = ZERO;
    }
}

template<typename Real>
Void TQuaternion<Real>::_Quaternion_To_Matrix(TMatrix3<Real> & outRot) const
{
    Real fTx  = X * TWO;
    Real fTy  = Y * TWO;
    Real fTz  = Z * TWO;
    Real fTwx = W * fTx;
    Real fTwy = W * fTy;
    Real fTwz = W * fTz;
    Real fTxx = X * fTx;
    Real fTxy = X * fTy;
    Real fTxz = X * fTz;
    Real fTyy = Y * fTy;
    Real fTyz = Y * fTz;
    Real fTzz = Z * fTz;

    outRot.m00 = ONE - (fTyy + fTzz); outRot.m01 = fTxy - fTwz;         outRot.m02 = fTxz + fTwy;
    outRot.m10 = fTxy + fTwz;         outRot.m11 = ONE - (fTxx + fTzz); outRot.m12 = fTyz - fTwx;
    outRot.m20 = fTxz - fTwy;         outRot.m21 = fTyz + fTwx;         outRot.m22 = ONE - (fTxx + fTyy);
}

template<typename Real>
Void TQuaternion<Real>::_AxisAngle_To_Quaternion(const TVector3<Real> & vAxis, Real fAngle)
{
    Real fHalfAngle = fAngle * (Real)0.5f;
    Real fSin, fCos;
    MathRealFn->SinCos( &fSin, &fCos, fHalfAngle );
    X = vAxis.X * fSin;
    Y = vAxis.Y * fSin;
    Z = vAxis.Z * fSin;
    W = fCos;
}

template<typename Real>
Void TQuaternion<Real>::_Matrix_To_Quaternion(const TMatrix3<Real> & matRot)
{
    // Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
    // article "Quaternion Calculus and Fast Animation".
    static Int s_arrNext[3] = { 1, 2, 0 };
    const Real HALF = (Real)0.5f;
    Real fTrace = matRot.m00 + matRot.m11 + matRot.m22;
    Real fRoot;
    if ( fTrace > ZERO ) {
        fRoot = MathRealFn->Sqrt( fTrace + ONE );
        W = HALF * fRoot;
        fRoot = HALF / fRoot;
        X = ( matRot.m21 - matRot.m12 ) * fRoot;
        Y = ( matRot.m02 - matRot.m20 ) * fRoot;
        Z = ( matRot.m10 - matRot.m01 ) * fRoot;
    } else {
        Int i = 0;
        if ( matRot.m11 > matRot.m00 )
            i = 1;
        if ( matRot.m22 > matRot(i,i) )
            i = 2;
        Int j = s_arrNext[i];
        Int k = s_arrNext[j];
        Real * Values = (Real*)this;

        fRoot = MathRealFn->Sqrt( matRot(i,i) - matRot(j,j) - matRot(k,k) + ONE );
        Values[i] = HALF * fRoot;
        fRoot = HALF / fRoot;
        W = ( matRot(k,j) - matRot(j,k) ) * fRoot;
        Values[j] = ( matRot(j,i) - matRot(i,j) ) * fRoot;
        Values[k] = ( matRot(k,i) - matRot(i,k) ) * fRoot;
    }
}

