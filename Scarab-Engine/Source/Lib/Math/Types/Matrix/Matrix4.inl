/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Matrix/Matrix4.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Homogeneous 4D matrix
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TMatrix4 implementation
template<typename Real>
const Real TMatrix4<Real>::ZERO = (Real)0;
template<typename Real>
const Real TMatrix4<Real>::ONE = (Real)1;
template<typename Real>
const Real TMatrix4<Real>::TWO = (Real)2;

template<typename Real>
const TMatrix4<Real> TMatrix4<Real>::Null = TMatrix4<Real>( ZERO, ZERO, ZERO, ZERO,
                                                            ZERO, ZERO, ZERO, ZERO,
                                                            ZERO, ZERO, ZERO, ZERO,
                                                            ZERO, ZERO, ZERO, ZERO );
template<typename Real>
const TMatrix4<Real> TMatrix4<Real>::Identity = TMatrix4<Real>( ONE,  ZERO, ZERO, ZERO,
                                                                ZERO, ONE,  ZERO, ZERO,
                                                                ZERO, ZERO, ONE,  ZERO,
                                                                ZERO, ZERO, ZERO, ONE );

template<typename Real>
TMatrix4<Real>::TMatrix4()
{
    // nothing to do
}
template<typename Real>
TMatrix4<Real>::TMatrix4( Real a00, Real a01, Real a02, Real a03,
                          Real a10, Real a11, Real a12, Real a13,
                          Real a20, Real a21, Real a22, Real a23,
                          Real a30, Real a31, Real a32, Real a33 )
{
    m00 = a00; m01 = a01; m02 = a02; m03 = a03;
    m10 = a10; m11 = a11; m12 = a12; m13 = a13;
    m20 = a20; m21 = a21; m22 = a22; m23 = a23;
    m30 = a30; m31 = a31; m32 = a32; m33 = a33;
}
template<typename Real>
TMatrix4<Real>::TMatrix4(const Real v0[4], const Real v1[4], const Real v2[4], const Real v3[4], Bool bRows)
{
	if (bRows) {
        m00 = v0[0]; m01 = v0[1]; m02 = v0[2]; m03 = v0[3];
        m10 = v1[0]; m11 = v1[1]; m12 = v1[2]; m13 = v1[3];
        m20 = v2[0]; m21 = v2[1]; m22 = v2[2]; m23 = v2[3];
        m30 = v3[0]; m31 = v3[1]; m32 = v3[2]; m33 = v3[3];
    } else {
        m00 = v0[0]; m01 = v1[0]; m02 = v2[0]; m03 = v3[0];
        m10 = v0[1]; m11 = v1[1]; m12 = v2[1]; m13 = v3[1];
        m20 = v0[2]; m21 = v1[2]; m22 = v2[2]; m23 = v3[2];
        m30 = v0[3]; m31 = v1[3]; m32 = v2[3]; m33 = v3[3];
    }
}
template<typename Real>
TMatrix4<Real>::TMatrix4(const Real arrMat[16], Bool bRows)
{
	if (bRows) {
        m00 = arrMat[0];  m01 = arrMat[1];  m02 = arrMat[2];  m03 = arrMat[3];
        m10 = arrMat[4];  m11 = arrMat[5];  m12 = arrMat[6];  m13 = arrMat[7];
        m20 = arrMat[8];  m21 = arrMat[9];  m22 = arrMat[10]; m23 = arrMat[11];
        m30 = arrMat[12]; m31 = arrMat[13]; m32 = arrMat[14]; m33 = arrMat[15];
    } else {
        m00 = arrMat[0];  m01 = arrMat[4];  m02 = arrMat[8];  m03 = arrMat[12];
        m10 = arrMat[1];  m11 = arrMat[5];  m12 = arrMat[9];  m13 = arrMat[13];
        m20 = arrMat[2];  m21 = arrMat[6];  m22 = arrMat[10]; m23 = arrMat[14];
        m30 = arrMat[3];  m31 = arrMat[7];  m32 = arrMat[11]; m33 = arrMat[15];
    }
}
template<typename Real>
TMatrix4<Real>::TMatrix4(const TVector4<Real> & v0, const TVector4<Real> & v1,
                         const TVector4<Real> & v2, const TVector4<Real> & v3, Bool bRows)
{
	if (bRows) {
        m00 = v0.X; m01 = v0.Y; m02 = v0.Z; m03 = v0.W;
        m10 = v1.X; m11 = v1.Y; m12 = v1.Z; m13 = v1.W;
        m20 = v2.X; m21 = v2.Y; m22 = v2.Z; m23 = v2.W;
        m30 = v3.X; m31 = v3.Y; m32 = v3.Z; m33 = v3.W;
    } else {
        m00 = v0.X; m01 = v1.X; m02 = v2.X; m03 = v3.X;
        m10 = v0.Y; m11 = v1.Y; m12 = v2.Y; m13 = v3.Y;
        m20 = v0.Z; m21 = v1.Z; m22 = v2.Z; m23 = v3.Z;
        m30 = v0.W; m31 = v1.W; m32 = v2.W; m33 = v3.W;
    }
}
template<typename Real>
TMatrix4<Real>::TMatrix4(const TVector4<Real> vMat[4], Bool bRows)
{
	if (bRows) {
        m00 = vMat[0].X; m01 = vMat[0].Y; m02 = vMat[0].Z; m03 = vMat[0].W;
        m10 = vMat[1].X; m11 = vMat[1].Y; m12 = vMat[1].Z; m13 = vMat[1].W;
        m20 = vMat[2].X; m21 = vMat[2].Y; m22 = vMat[2].Z; m23 = vMat[2].W;
        m30 = vMat[3].X; m31 = vMat[3].Y; m32 = vMat[3].Z; m33 = vMat[3].W;
    } else {
        m00 = vMat[0].X; m01 = vMat[1].X; m02 = vMat[2].X; m03 = vMat[3].X;
        m10 = vMat[0].Y; m11 = vMat[1].Y; m12 = vMat[2].Y; m13 = vMat[3].Y;
        m20 = vMat[0].Z; m21 = vMat[1].Z; m22 = vMat[2].Z; m23 = vMat[3].Z;
        m30 = vMat[0].W; m31 = vMat[1].W; m32 = vMat[2].W; m33 = vMat[3].W;
    }
}
template<typename Real>
TMatrix4<Real>::TMatrix4(const TMatrix2<Real> & rhs)
{
    m00 = rhs.m00; m01 = rhs.m01; m02 = ZERO; m03 = ZERO;
    m10 = rhs.m10; m11 = rhs.m11; m12 = ZERO; m13 = ZERO;
    m20 = ZERO;   m21 = ZERO;     m22 = ONE;  m23 = ZERO;
    m30 = ZERO;   m31 = ZERO;     m32 = ZERO; m33 = ONE;
}
template<typename Real>
TMatrix4<Real>::TMatrix4(const TMatrix3<Real> & rhs)
{
    m00 = rhs.m00; m01 = rhs.m01; m02 = rhs.m02; m03 = ZERO;
    m10 = rhs.m10; m11 = rhs.m11; m12 = rhs.m12; m13 = ZERO;
    m20 = rhs.m20; m21 = rhs.m21; m22 = rhs.m22; m23 = ZERO;
    m30 = ZERO;    m31 = ZERO;    m32 = ZERO;    m33 = ONE;
}
template<typename Real>
TMatrix4<Real>::TMatrix4(const TMatrix4<Real> & rhs)
{
    m00 = rhs.m00; m01 = rhs.m01; m02 = rhs.m02; m03 = rhs.m03;
    m10 = rhs.m10; m11 = rhs.m11; m12 = rhs.m12; m13 = rhs.m13;
    m20 = rhs.m20; m21 = rhs.m21; m22 = rhs.m22; m23 = rhs.m23;
    m30 = rhs.m30; m31 = rhs.m31; m32 = rhs.m32; m33 = rhs.m33;
}
template<typename Real>
TMatrix4<Real>::~TMatrix4()
{
    // nothing to do
}

template<typename Real>
inline TMatrix4<Real> & TMatrix4<Real>::operator=(const TMatrix4<Real> & rhs) {
    m00 = rhs.m00; m01 = rhs.m01; m02 = rhs.m02; m03 = rhs.m03;
    m10 = rhs.m10; m11 = rhs.m11; m12 = rhs.m12; m13 = rhs.m13;
    m20 = rhs.m20; m21 = rhs.m21; m22 = rhs.m22; m23 = rhs.m23;
    m30 = rhs.m30; m31 = rhs.m31; m32 = rhs.m32; m33 = rhs.m33;
    return (*this);
}

template<typename Real>
inline TMatrix4<Real>::operator Real*() const {
    return (Real*)this;
}
template<typename Real>
inline TMatrix4<Real>::operator const Real*() const {
    return (const Real*)this;
}

template<typename Real>
inline Real & TMatrix4<Real>::operator[](Int i) {
    return *( ((Real*)this) + i );
}
template<typename Real>
inline Real TMatrix4<Real>::operator[](Int i) const {
    return *( ((const Real*)this) + i );
}
template<typename Real>
inline Real & TMatrix4<Real>::operator[](UInt i) {
    return *( ((Real*)this) + i );
}
template<typename Real>
inline Real TMatrix4<Real>::operator[](UInt i) const {
    return *( ((const Real*)this) + i );
}

template<typename Real>
inline Real & TMatrix4<Real>::operator()(Int iRow, Int iColumn) {
    return *( ((Real*)this) + ((iRow<<2) + iColumn) );
}
template<typename Real>
inline Real TMatrix4<Real>::operator()(Int iRow, Int iColumn) const {
    return *( ((const Real*)this) + ((iRow<<2) + iColumn) );
}
template<typename Real>
inline Real & TMatrix4<Real>::operator()(UInt iRow, UInt iColumn) {
    return *( ((Real*)this) + ((iRow<<2) + iColumn) );
}
template<typename Real>
inline Real TMatrix4<Real>::operator()(UInt iRow, UInt iColumn) const {
    return *( ((const Real*)this) + ((iRow<<2) + iColumn) );
}

template<typename Real>
inline TMatrix4<Real> TMatrix4<Real>::operator+() const {
    return TMatrix4<Real>( m00, m01, m02, m03,
                           m10, m11, m12, m13,
                           m20, m21, m22, m23,
                           m30, m31, m32, m33 );
}
template<typename Real>
inline TMatrix4<Real> TMatrix4<Real>::operator-() const {
    return TMatrix4<Real>( -m00, -m01, -m02, -m03,
                           -m10, -m11, -m12, -m13,
                           -m20, -m21, -m22, -m23,
                           -m30, -m31, -m32, -m33 );
}

template<typename Real>
Bool TMatrix4<Real>::operator==(const TMatrix4<Real> & rhs) const
{
    return ( MathRealFn->Equals(m00, rhs.m00) && MathRealFn->Equals(m01, rhs.m01) &&
             MathRealFn->Equals(m02, rhs.m02) && MathRealFn->Equals(m03, rhs.m03) &&
             MathRealFn->Equals(m10, rhs.m10) && MathRealFn->Equals(m11, rhs.m11) &&
             MathRealFn->Equals(m12, rhs.m12) && MathRealFn->Equals(m13, rhs.m13) &&
             MathRealFn->Equals(m20, rhs.m20) && MathRealFn->Equals(m21, rhs.m21) &&
             MathRealFn->Equals(m22, rhs.m22) && MathRealFn->Equals(m23, rhs.m23) &&
             MathRealFn->Equals(m30, rhs.m30) && MathRealFn->Equals(m31, rhs.m31) &&
             MathRealFn->Equals(m32, rhs.m32) && MathRealFn->Equals(m33, rhs.m33) );
}
template<typename Real>
Bool TMatrix4<Real>::operator!=(const TMatrix4<Real> & rhs) const
{
    return ( !(MathRealFn->Equals(m00, rhs.m00)) || !(MathRealFn->Equals(m01, rhs.m01)) ||
             !(MathRealFn->Equals(m02, rhs.m02)) || !(MathRealFn->Equals(m03, rhs.m03)) ||
             !(MathRealFn->Equals(m10, rhs.m10)) || !(MathRealFn->Equals(m11, rhs.m11)) ||
             !(MathRealFn->Equals(m12, rhs.m12)) || !(MathRealFn->Equals(m13, rhs.m13)) ||
             !(MathRealFn->Equals(m20, rhs.m20)) || !(MathRealFn->Equals(m21, rhs.m21)) ||
             !(MathRealFn->Equals(m22, rhs.m22)) || !(MathRealFn->Equals(m23, rhs.m23)) ||
             !(MathRealFn->Equals(m30, rhs.m30)) || !(MathRealFn->Equals(m31, rhs.m31)) ||
             !(MathRealFn->Equals(m32, rhs.m32)) || !(MathRealFn->Equals(m33, rhs.m33)) );
}

template<typename Real>
TMatrix4<Real> TMatrix4<Real>::operator*(Real rhs) const
{
    return TMatrix4<Real>( m00 * rhs, m01 * rhs, m02 * rhs, m03 * rhs,
                           m10 * rhs, m11 * rhs, m12 * rhs, m13 * rhs,
                           m20 * rhs, m21 * rhs, m22 * rhs, m23 * rhs,
                           m30 * rhs, m31 * rhs, m32 * rhs, m33 * rhs );
}
template<typename Real>
TMatrix4<Real> TMatrix4<Real>::operator/(Real rhs) const
{
    return TMatrix4<Real>( m00 / rhs, m01 / rhs, m02 / rhs, m03 / rhs,
                           m10 / rhs, m11 / rhs, m12 / rhs, m13 / rhs,
                           m20 / rhs, m21 / rhs, m22 / rhs, m23 / rhs,
                           m30 / rhs, m31 / rhs, m32 / rhs, m33 / rhs );
}

template<typename Real>
TMatrix4<Real> & TMatrix4<Real>::operator*=(Real rhs)
{
    m00 *= rhs; m01 *= rhs; m02 *= rhs; m03 *= rhs;
    m10 *= rhs; m11 *= rhs; m12 *= rhs; m13 *= rhs;
    m20 *= rhs; m21 *= rhs; m22 *= rhs; m23 *= rhs;
    m30 *= rhs; m31 *= rhs; m32 *= rhs; m33 *= rhs;
    return (*this);
}
template<typename Real>
TMatrix4<Real> & TMatrix4<Real>::operator/=(Real rhs)
{
    m00 /= rhs; m01 /= rhs; m02 /= rhs; m03 /= rhs;
    m10 /= rhs; m11 /= rhs; m12 /= rhs; m13 /= rhs;
    m20 /= rhs; m21 /= rhs; m22 /= rhs; m23 /= rhs;
    m30 /= rhs; m31 /= rhs; m32 /= rhs; m33 /= rhs;
    return (*this);
}

template<typename Real>
TVertex4<Real> TMatrix4<Real>::operator*(const TVertex4<Real> & rhs) const
{
    return TVertex4<Real>( (m00 * rhs.X + m01 * rhs.Y + m02 * rhs.Z + m03 * rhs.W),
                           (m10 * rhs.X + m11 * rhs.Y + m12 * rhs.Z + m13 * rhs.W),
                           (m20 * rhs.X + m21 * rhs.Y + m22 * rhs.Z + m23 * rhs.W),
                           (m30 * rhs.X + m31 * rhs.Y + m32 * rhs.Z + m33 * rhs.W) );
}
template<typename Real>
TVector4<Real> TMatrix4<Real>::operator*(const TVector4<Real> & rhs) const
{
    return TVector4<Real>( (m00 * rhs.X + m01 * rhs.Y + m02 * rhs.Z + m03 * rhs.W),
                           (m10 * rhs.X + m11 * rhs.Y + m12 * rhs.Z + m13 * rhs.W),
                           (m20 * rhs.X + m21 * rhs.Y + m22 * rhs.Z + m23 * rhs.W),
                           (m30 * rhs.X + m31 * rhs.Y + m32 * rhs.Z + m33 * rhs.W) );
}

template<typename Real>
TMatrix4<Real> TMatrix4<Real>::operator+(const TMatrix4<Real> & rhs) const
{
    return TMatrix4<Real>( m00 + rhs.m00, m01 + rhs.m01, m02 + rhs.m02, m03 + rhs.m03,
                           m10 + rhs.m10, m11 + rhs.m11, m12 + rhs.m12, m13 + rhs.m13,
                           m20 + rhs.m20, m21 + rhs.m21, m22 + rhs.m22, m23 + rhs.m23,
                           m30 + rhs.m30, m31 + rhs.m31, m32 + rhs.m32, m33 + rhs.m33 );
}
template<typename Real>
TMatrix4<Real> TMatrix4<Real>::operator-(const TMatrix4<Real> & rhs) const
{
    return TMatrix4<Real>( m00 - rhs.m00, m01 - rhs.m01, m02 - rhs.m02, m03 - rhs.m03,
                           m10 - rhs.m10, m11 - rhs.m11, m12 - rhs.m12, m13 - rhs.m13,
                           m20 - rhs.m20, m21 - rhs.m21, m22 - rhs.m22, m23 - rhs.m23,
                           m30 - rhs.m30, m31 - rhs.m31, m32 - rhs.m32, m33 - rhs.m33 );
}
template<typename Real>
TMatrix4<Real> TMatrix4<Real>::operator*(const TMatrix4<Real> & rhs) const
{
    return TMatrix4<Real> (
        (m00 * rhs.m00 + m01 * rhs.m10 + m02 * rhs.m20 + m03 * rhs.m30),
        (m00 * rhs.m01 + m01 * rhs.m11 + m02 * rhs.m21 + m03 * rhs.m31),
        (m00 * rhs.m02 + m01 * rhs.m12 + m02 * rhs.m22 + m03 * rhs.m32),
        (m00 * rhs.m03 + m01 * rhs.m13 + m02 * rhs.m23 + m03 * rhs.m33),

	    (m10 * rhs.m00 + m11 * rhs.m10 + m12 * rhs.m20 + m13 * rhs.m30),
        (m10 * rhs.m01 + m11 * rhs.m11 + m12 * rhs.m21 + m13 * rhs.m31),
        (m10 * rhs.m02 + m11 * rhs.m12 + m12 * rhs.m22 + m13 * rhs.m32),
        (m10 * rhs.m03 + m11 * rhs.m13 + m12 * rhs.m23 + m13 * rhs.m33),

	    (m20 * rhs.m00 + m21 * rhs.m10 + m22 * rhs.m20 + m23 * rhs.m30),
        (m20 * rhs.m01 + m21 * rhs.m11 + m22 * rhs.m21 + m23 * rhs.m31),
        (m20 * rhs.m02 + m21 * rhs.m12 + m22 * rhs.m22 + m23 * rhs.m32),
        (m20 * rhs.m03 + m21 * rhs.m13 + m22 * rhs.m23 + m23 * rhs.m33),

	    (m30 * rhs.m00 + m31 * rhs.m10 + m32 * rhs.m20 + m33 * rhs.m30),
        (m30 * rhs.m01 + m31 * rhs.m11 + m32 * rhs.m21 + m33 * rhs.m31),
        (m30 * rhs.m02 + m31 * rhs.m12 + m32 * rhs.m22 + m33 * rhs.m32),
        (m30 * rhs.m03 + m31 * rhs.m13 + m32 * rhs.m23 + m33 * rhs.m33)
    );
}

template<typename Real>
TMatrix4<Real> & TMatrix4<Real>::operator+=(const TMatrix4<Real> & rhs)
{
    m00 += rhs.m00; m01 += rhs.m01; m02 += rhs.m02; m03 += rhs.m03;
	m10 += rhs.m10; m11 += rhs.m11; m12 += rhs.m12; m13 += rhs.m13;
	m20 += rhs.m20; m21 += rhs.m21; m22 += rhs.m22; m23 += rhs.m23;
	m30 += rhs.m30; m31 += rhs.m31; m32 += rhs.m32; m33 += rhs.m33;
    return (*this);
}
template<typename Real>
TMatrix4<Real> & TMatrix4<Real>::operator-=(const TMatrix4<Real> & rhs)
{
    m00 -= rhs.m00; m01 -= rhs.m01; m02 -= rhs.m02; m03 -= rhs.m03;
	m10 -= rhs.m10; m11 -= rhs.m11; m12 -= rhs.m12; m13 -= rhs.m13;
	m20 -= rhs.m20; m21 -= rhs.m21; m22 -= rhs.m22; m23 -= rhs.m23;
	m30 -= rhs.m30; m31 -= rhs.m31; m32 -= rhs.m32; m33 -= rhs.m33;
    return (*this);
}
template<typename Real>
TMatrix4<Real> & TMatrix4<Real>::operator*=(const TMatrix4<Real> & rhs)
{
    Real f0 = (m00 * rhs.m00 + m01 * rhs.m10 + m02 * rhs.m20 + m03 * rhs.m30);
    Real f1 = (m00 * rhs.m01 + m01 * rhs.m11 + m02 * rhs.m21 + m03 * rhs.m31);
    Real f2 = (m00 * rhs.m02 + m01 * rhs.m12 + m02 * rhs.m22 + m03 * rhs.m32);
    Real f3 = (m00 * rhs.m03 + m01 * rhs.m13 + m02 * rhs.m23 + m03 * rhs.m33);
    m00 = f0; m01 = f1; m02 = f2; m03 = f3;

    f0 = (m10 * rhs.m00 + m11 * rhs.m10 + m12 * rhs.m20 + m13 * rhs.m30);
    f1 = (m10 * rhs.m01 + m11 * rhs.m11 + m12 * rhs.m21 + m13 * rhs.m31);
    f2 = (m10 * rhs.m02 + m11 * rhs.m12 + m12 * rhs.m22 + m13 * rhs.m32);
    f3 = (m10 * rhs.m03 + m11 * rhs.m13 + m12 * rhs.m23 + m13 * rhs.m33);
    m10 = f0; m11 = f1; m12 = f2; m13 = f3;

    f0 = (m20 * rhs.m00 + m21 * rhs.m10 + m22 * rhs.m20 + m23 * rhs.m30);
    f1 = (m20 * rhs.m01 + m21 * rhs.m11 + m22 * rhs.m21 + m23 * rhs.m31);
    f2 = (m20 * rhs.m02 + m21 * rhs.m12 + m22 * rhs.m22 + m23 * rhs.m32);
    f3 = (m20 * rhs.m03 + m21 * rhs.m13 + m22 * rhs.m23 + m23 * rhs.m33);
    m20 = f0; m21 = f1; m22 = f2; m23 = f3;

    f0 = (m30 * rhs.m00 + m31 * rhs.m10 + m32 * rhs.m20 + m33 * rhs.m30);
    f1 = (m30 * rhs.m01 + m31 * rhs.m11 + m32 * rhs.m21 + m33 * rhs.m31);
    f2 = (m30 * rhs.m02 + m31 * rhs.m12 + m32 * rhs.m22 + m33 * rhs.m32);
    f3 = (m30 * rhs.m03 + m31 * rhs.m13 + m32 * rhs.m23 + m33 * rhs.m33);
    m30 = f0; m31 = f1; m32 = f2; m33 = f3;

    return (*this);
}

template<typename Real>
inline Void TMatrix4<Real>::GetRow(TVector4<Real> & outRow, UInt iRow) const {
    const Real * Values = ( ((const Real*)this) + (iRow << 2) );
    outRow.X = Values[0];
    outRow.Y = Values[1];
    outRow.Z = Values[2];
    outRow.W = Values[3];
}
template<typename Real>
inline Void TMatrix4<Real>::SetRow(UInt iRow, Real fRow0, Real fRow1, Real fRow2, Real fRow3) {
    Real * Values = ( ((Real*)this) + (iRow << 2) );
    Values[0] = fRow0;
    Values[1] = fRow1;
    Values[2] = fRow2;
    Values[3] = fRow3;
}
template<typename Real>
inline Void TMatrix4<Real>::SetRow(UInt iRow, const Real vRow[4]) {
    Real * Values = ( ((Real*)this) + (iRow << 2) );
    Values[0] = vRow[0];
    Values[1] = vRow[1];
    Values[2] = vRow[2];
    Values[3] = vRow[3];
}
template<typename Real>
inline Void TMatrix4<Real>::SetRow(UInt iRow, const TVector4<Real> & vRow) {
    Real * Values = ( ((Real*)this) + (iRow << 2) );
    Values[0] = vRow.X;
    Values[1] = vRow.Y;
    Values[2] = vRow.Z;
    Values[3] = vRow.W;
}

template<typename Real>
inline Void TMatrix4<Real>::GetColumn(TVector4<Real> & outColumn, UInt iColumn) const {
    const Real * Values = ( ((const Real*)this) + iColumn );
    outColumn.X = Values[0];
    outColumn.Y = Values[4];
    outColumn.Z = Values[8];
    outColumn.W = Values[12];
}
template<typename Real>
inline Void TMatrix4<Real>::SetColumn(UInt iColumn, Real fCol0, Real fCol1, Real fCol2, Real fCol3) {
    Real * Values = ( ((Real*)this) + iColumn );
    Values[0] = fCol0;
    Values[4] = fCol1;
    Values[8] = fCol2;
    Values[12] = fCol3;
}
template<typename Real>
inline Void TMatrix4<Real>::SetColumn(UInt iColumn, const Real vCol[4]) {
    Real * Values = ( ((Real*)this) + iColumn );
    Values[0] = vCol[0];
    Values[4] = vCol[1];
    Values[8] = vCol[2];
    Values[12] = vCol[3];
}
template<typename Real>
inline Void TMatrix4<Real>::SetColumn(UInt iColumn, const TVector4<Real> & vCol) {
    Real * Values = ( ((Real*)this) + iColumn );
    Values[0] = vCol.X;
    Values[4] = vCol.Y;
    Values[8] = vCol.Z;
    Values[12] = vCol.W;
}

template<typename Real>
inline Void TMatrix4<Real>::MakeNull() {
    m00 = ZERO; m01 = ZERO; m02 = ZERO; m03 = ZERO;
    m10 = ZERO; m11 = ZERO; m12 = ZERO; m13 = ZERO;
    m20 = ZERO; m21 = ZERO; m22 = ZERO; m23 = ZERO;
    m30 = ZERO; m31 = ZERO; m32 = ZERO; m33 = ZERO;
}
template<typename Real>
inline Void TMatrix4<Real>::MakeIdentity() {
    m00 = ONE;  m01 = ZERO; m02 = ZERO; m03 = ZERO;
    m10 = ZERO; m11 = ONE;  m12 = ZERO; m13 = ZERO;
    m20 = ZERO; m21 = ZERO; m22 = ONE;  m23 = ZERO;
    m30 = ZERO; m31 = ZERO; m32 = ZERO; m33 = ONE;
}
template<typename Real>
inline Void TMatrix4<Real>::MakeDiagonal(const TVector3<Real> & vDiagonal) {
    m00 = vDiagonal.X; m01 = ZERO;        m02 = ZERO;        m03 = ZERO;
    m10 = ZERO;        m11 = vDiagonal.Y; m12 = ZERO;        m13 = ZERO;
    m20 = ZERO;        m21 = ZERO;        m22 = vDiagonal.Z; m23 = ZERO;
    m30 = ZERO;        m31 = ZERO;        m32 = ZERO;        m33 = ONE;
}
template<typename Real>
inline Void TMatrix4<Real>::MakeTranslate(const TVector3<Real> & vTranslate) {
    m00 = ONE;  m01 = ZERO; m02 = ZERO; m03 = vTranslate.X;
    m10 = ZERO; m11 = ONE;  m12 = ZERO; m13 = vTranslate.Y;
    m20 = ZERO; m21 = ZERO; m22 = ONE;  m23 = vTranslate.Z;
    m30 = ZERO; m31 = ZERO; m32 = ZERO; m33 = ONE;
}
template<typename Real>
inline Void TMatrix4<Real>::MakeScale(const TVector3<Real> & vScale) {
    m00 = vScale.X; m01 = ZERO;     m02 = ZERO;     m03 = ZERO;
    m10 = ZERO;     m11 = vScale.Y; m12 = ZERO;     m13 = ZERO;
    m20 = ZERO;     m21 = ZERO;     m22 = vScale.Z; m23 = ZERO;
    m30 = ZERO;     m31 = ZERO;     m32 = ZERO;     m33 = ONE;
}
template<typename Real>
inline Void TMatrix4<Real>::MakeBasis(const TVertex3<Real> & vOrigin, const TVector3<Real> & vI,
                                      const TVector3<Real> & vJ, const TVector3<Real> & vK) {
    m00 = vI.X; m01 = vJ.X; m02 = vK.X; m03 = vOrigin.X;
    m10 = vI.Y; m11 = vJ.Y; m12 = vK.Y; m13 = vOrigin.Y;
    m20 = vI.Z; m21 = vJ.Z; m22 = vK.Z; m23 = vOrigin.Z;
    m30 = ZERO; m31 = ZERO; m32 = ZERO; m33 = ONE;
}

template<typename Real>
Void TMatrix4<Real>::MakeRotate(Axis rotAxis, Real rotAngle)
{
    Real fSin, fCos;
	MathRealFn->SinCos( &fSin, &fCos, rotAngle );
	switch(rotAxis) {
	    case AXIS_X:
		    m00 = ONE;  m01 = ZERO; m02 = ZERO;  m03 = ZERO;
		    m10 = ZERO; m11 = fCos;	m12 = -fSin; m13 = ZERO;
		    m20 = ZERO; m21 = fSin;	m22 = fCos;	 m23 = ZERO;
		    m30 = ZERO; m31 = ZERO; m32 = ZERO;  m33 = ONE;
		    break;
	    case AXIS_Y:
		    m00 = fCos;	 m01 = ZERO; m02 = fSin; m03 = ZERO;
		    m10 = ZERO;  m11 = ONE;  m12 = ZERO; m13 = ZERO;
		    m20 = -fSin; m21 = ZERO; m22 = fCos; m23 = ZERO;
		    m30 = ZERO;  m31 = ZERO; m32 = ZERO; m33 = ONE;
		    break;
	    case AXIS_Z:
		    m00 = fCos;	m01 = -fSin; m02 = ZERO; m03 = ZERO;
		    m10 = fSin;	m11 = fCos;	 m12 = ZERO; m13 = ZERO;
		    m20 = ZERO; m21 = ZERO;  m22 = ONE;  m23 = ZERO;
		    m30 = ZERO; m31 = ZERO;  m32 = ZERO; m33 = ONE;
		    break;
	    default: break;
	}
}
template<typename Real>
Void TMatrix4<Real>::MakeRotate(const TVector3<Real> & vAxis, Real rotAngle)
{
    Real fSin, fCos;
	MathRealFn->SinCos( &fSin, &fCos, rotAngle );

	Real t = (ONE - fCos);
	Real xx = (vAxis.X * vAxis.X);
    Real yy = (vAxis.Y * vAxis.Y);
    Real zz = (vAxis.Z * vAxis.Z);
	Real xy = (vAxis.X * vAxis.Y);
    Real xz = (vAxis.X * vAxis.Z);
    Real yz = (vAxis.Y * vAxis.Z);
	Real sx = (fSin * vAxis.X);
    Real sy = (fSin * vAxis.Y);
    Real sz = (fSin * vAxis.Z);
	m00 = (t*xx + fCos); m01 = (t*xy - sz);	  m02 = (t*xz + sy);   m03 = ZERO;
	m10 = (t*xy + sz);	 m11 = (t*yy + fCos); m12 = (t*yz - sx);   m13 = ZERO;
	m20 = (t*xz - sy);	 m21 = (t*yz + sx);	  m22 = (t*zz + fCos); m23 = ZERO;
	m30 = ZERO;			 m31 = ZERO;		  m32 = ZERO;	       m33 = ONE;
}
template<typename Real>
Void TMatrix4<Real>::MakeRotate(Real fYaw, Real fPitch, Real fRoll, EulerAngles eulerAnglesOrder)
{
    TMatrix4<Real> matYaw, matPitch, matRoll;
    matYaw.MakeRotate(   (Axis)( (eulerAnglesOrder>>4) & 0x03 ), fYaw );
    matPitch.MakeRotate( (Axis)( (eulerAnglesOrder>>2) & 0x03 ), fPitch );
    matRoll.MakeRotate(  (Axis)( eulerAnglesOrder & 0x03 ), fRoll );
    (*this) = ( matRoll * matPitch * matYaw );
}
template<typename Real>
Void TMatrix4<Real>::MakeReflection(const TVector3<Real> & vNormal, const TVertex3<Real> & vOrigin)
{
    Real fDot = ( vNormal * TVector3<Real>((const Real*)vOrigin) ) * TWO;
    Real fXX = vNormal.X * vNormal.X;
    Real fYY = vNormal.Y * vNormal.Y;
    Real fZZ = vNormal.Z * vNormal.Z;
    Real fXY = vNormal.X * vNormal.Y;
    Real fXZ = vNormal.X * vNormal.Z;
    Real fYZ = vNormal.Y * vNormal.Z;
    m00 = (ONE - (fXX * TWO)); m01 = -(fXY * TWO);        m02 = -(fXZ * TWO);        m03 = fDot * vNormal.X;
    m10 = -(fXY * TWO);        m11 = (ONE - (fYY * TWO)); m12 = -(fYZ * TWO);        m13 = fDot * vNormal.Y;
    m20 = -(fXZ * TWO);        m21 = -(fYZ * TWO);        m22 = (ONE - (fZZ * TWO)); m23 = fDot * vNormal.Z;
    m30 = ZERO;                m31 = ZERO;                m32 = ZERO;                m33 = ONE;
}
template<typename Real>
Void TMatrix4<Real>::MakeObliqueProjection(const TVector3<Real> & vNormal, const TVertex3<Real> & vOrigin, const TVector3<Real> & vDirection)
{
    Real fDotD = ( vNormal * vDirection );
    Real fDotO = ( vNormal * TVector3<Real>((const Real*)vOrigin) );
    Real fXX = vDirection.X * vNormal.X;
    Real fXY = vDirection.X * vNormal.Y;
    Real fXZ = vDirection.X * vNormal.Z;
    Real fYX = vDirection.Y * vNormal.X;
    Real fYY = vDirection.Y * vNormal.Y;
    Real fYZ = vDirection.Y * vNormal.Z;
    Real fZX = vDirection.Z * vNormal.X;
    Real fZY = vDirection.Z * vNormal.Y;
    Real fZZ = vDirection.Z * vNormal.Z;
    m00 = (fXX - fDotD); m01 = fXY;           m02 = fXZ;           m03 = -fDotO * vDirection.X;
    m10 = fYX;           m11 = (fYY - fDotD); m12 = fYZ;           m13 = -fDotO * vDirection.Y;
    m20 = fZX;           m21 = fZY;           m22 = (fZZ - fDotD); m23 = -fDotO * vDirection.Z;
    m30 = ZERO;          m31 = ZERO;          m32 = ZERO;          m33 = -fDotD;
}
template<typename Real>
Void TMatrix4<Real>::MakePerspectiveProjection(const TVector3<Real> & vNormal, const TVertex3<Real> & vOrigin, const TVertex3<Real> & vPosition)
{
    Real fDot = ( vNormal * (vPosition - vOrigin) );
    Real fXX = vPosition.X * vNormal.X;
    Real fXY = vPosition.X * vNormal.Y;
    Real fXZ = vPosition.X * vNormal.Z;
    Real fYX = vPosition.Y * vNormal.X;
    Real fYY = vPosition.Y * vNormal.Y;
    Real fYZ = vPosition.Y * vNormal.Z;
    Real fZX = vPosition.Z * vNormal.X;
    Real fZY = vPosition.Z * vNormal.Y;
    Real fZZ = vPosition.Z * vNormal.Z;
    m00 = (fDot - fXX); m01 = -fXY;         m02 = -fXZ;         m03 = -( m00 * vPosition.X + m01 * vPosition.Y + m02 * vPosition.Z );
    m10 = -fYX;         m11 = (fDot - fYY); m12 = -fYZ;         m13 = -( m10 * vPosition.X + m11 * vPosition.Y + m12 * vPosition.Z );
    m20 = -fZX;         m21 = -fZY;         m22 = (fDot - fZZ); m23 = -( m20 * vPosition.X + m21 * vPosition.Y + m22 * vPosition.Z );
    m30 = -vNormal.X;   m31 = -vNormal.Y;   m32 = -vNormal.Z;   m33 = ( vNormal * TVector3<Real>((const Real*)vPosition) );
}

template<typename Real>
inline Void TMatrix4<Real>::Transpose(TMatrix4<Real> & outTransposedMatrix) const {
    outTransposedMatrix.m00 = m00; outTransposedMatrix.m01 = m10; outTransposedMatrix.m02 = m20; outTransposedMatrix.m03 = m30;
    outTransposedMatrix.m10 = m01; outTransposedMatrix.m11 = m11; outTransposedMatrix.m12 = m21; outTransposedMatrix.m13 = m31;
    outTransposedMatrix.m20 = m02; outTransposedMatrix.m21 = m12; outTransposedMatrix.m22 = m22; outTransposedMatrix.m23 = m32;
    outTransposedMatrix.m30 = m03; outTransposedMatrix.m31 = m13; outTransposedMatrix.m32 = m23; outTransposedMatrix.m33 = m33;
}

template<typename Real>
inline Void TMatrix4<Real>::Minor(TMatrix3<Real> & outMinor, UInt iRow, UInt iColumn) const {
    // Ok here we need to use branching, 3rd order polynoms won't make it
	UInt Row0 = ( (iRow == 0)  ? 1 : 0 ) << 2;
    UInt Row1 = ( (iRow < 2)   ? 2 : 1 ) << 2;
    UInt Row2 = ( (iRow == 3)  ? 2 : 3 ) << 2;
	UInt Col0 = (iColumn == 0) ? 1 : 0;
	UInt Col1 = (iColumn < 2)  ? 2 : 1;
	UInt Col2 = (iColumn == 3) ? 2 : 3;
    const Real * Values = ( (const Real*)this );
    outMinor.m00 = Values[Row0 + Col0]; outMinor.m01 = Values[Row0 + Col1]; outMinor.m02 = Values[Row0 + Col2];
    outMinor.m10 = Values[Row1 + Col0]; outMinor.m11 = Values[Row1 + Col1]; outMinor.m12 = Values[Row1 + Col2];
    outMinor.m20 = Values[Row2 + Col0]; outMinor.m21 = Values[Row2 + Col1]; outMinor.m22 = Values[Row2 + Col2];
}

template<typename Real>
Real TMatrix4<Real>::Determinant() const
{
    Real fA0, fA1, fA2, fA3, fA4, fA5;
    Real fB0, fB1, fB2, fB3, fB4, fB5, fDet;
    fA0 = m00 * m11 - m01 * m10;
    fA1 = m00 * m12 - m02 * m10;
    fA2 = m00 * m13 - m03 * m10;
    fA3 = m01 * m12 - m02 * m11;
    fA4 = m01 * m13 - m03 * m11;
    fA5 = m02 * m13 - m03 * m12;
    fB0 = m20 * m31 - m21 * m30;
    fB1 = m20 * m32 - m22 * m30;
    fB2 = m20 * m33 - m23 * m30;
    fB3 = m21 * m32 - m22 * m31;
    fB4 = m21 * m33 - m23 * m31;
    fB5 = m22 * m33 - m23 * m32;
    fDet = ( + fA0*fB5 - fA1*fB4 + fA2*fB3
             + fA3*fB2 - fA4*fB1 + fA5*fB0 );
    return fDet;
}

template<typename Real>
Void TMatrix4<Real>::Adjoint(TMatrix4<Real> & outAdjointMatrix) const
{
    Real fA0, fA1, fA2, fA3, fA4, fA5;
    Real fB0, fB1, fB2, fB3, fB4, fB5;

    fA0 = m00 * m11 - m01 * m10;
    fA1 = m00 * m12 - m02 * m10;
    fA2 = m00 * m13 - m03 * m10;
    fA3 = m01 * m12 - m02 * m11;
    fA4 = m01 * m13 - m03 * m11;
    fA5 = m02 * m13 - m03 * m12;
    fB0 = m20 * m31 - m21 * m30;
    fB1 = m20 * m32 - m22 * m30;
    fB2 = m20 * m33 - m23 * m30;
    fB3 = m21 * m32 - m22 * m31;
    fB4 = m21 * m33 - m23 * m31;
    fB5 = m22 * m33 - m23 * m32;

    outAdjointMatrix.m00 = + (m11 * fB5) - (m12 * fB4) + (m13 * fB3);
    outAdjointMatrix.m01 = - (m01 * fB5) + (m02 * fB4) - (m03 * fB3);
    outAdjointMatrix.m02 = + (m31 * fA5) - (m32 * fA4) + (m33 * fA3);
    outAdjointMatrix.m03 = - (m21 * fA5) + (m22 * fA4) - (m23 * fA3);

    outAdjointMatrix.m10 = - (m10 * fB5) + (m12 * fB2) - (m13 * fB1);
    outAdjointMatrix.m11 = + (m00 * fB5) - (m02 * fB2) + (m03 * fB1);
    outAdjointMatrix.m12 = - (m30 * fA5) + (m32 * fA2) - (m33 * fA1);
    outAdjointMatrix.m13 = + (m20 * fA5) - (m22 * fA2) + (m23 * fA1);

    outAdjointMatrix.m20 = + (m10 * fB4) - (m11 * fB2) + (m13 * fB0);
    outAdjointMatrix.m21 = - (m00 * fB4) + (m01 * fB2) - (m03 * fB0);
    outAdjointMatrix.m22 = + (m30 * fA4) - (m31 * fA2) + (m33 * fA0);
    outAdjointMatrix.m23 = - (m20 * fA4) + (m21 * fA2) - (m23 * fA0);

    outAdjointMatrix.m30 = - (m10 * fB3) + (m11 * fB1) - (m12 * fB0);
    outAdjointMatrix.m31 = + (m00 * fB3) - (m01 * fB1) + (m02 * fB0);
    outAdjointMatrix.m32 = - (m30 * fA3) + (m31 * fA1) - (m32 * fA0);
    outAdjointMatrix.m33 = + (m20 * fA3) - (m21 * fA1) + (m22 * fA0);
}

template<typename Real>
Bool TMatrix4<Real>::Invert(TMatrix4<Real> & outInvMatrix, Real fZeroTolerance) const
{
    Real fA0, fA1, fA2, fA3, fA4, fA5;
    Real fB0, fB1, fB2, fB3, fB4, fB5, fInvDet;

    fA0 = m00 * m11 - m01 * m10;
    fA1 = m00 * m12 - m02 * m10;
    fA2 = m00 * m13 - m03 * m10;
    fA3 = m01 * m12 - m02 * m11;
    fA4 = m01 * m13 - m03 * m11;
    fA5 = m02 * m13 - m03 * m12;
    fB0 = m20 * m31 - m21 * m30;
    fB1 = m20 * m32 - m22 * m30;
    fB2 = m20 * m33 - m23 * m30;
    fB3 = m21 * m32 - m22 * m31;
    fB4 = m21 * m33 - m23 * m31;
    fB5 = m22 * m33 - m23 * m32;

    fInvDet = ( + fA0*fB5 - fA1*fB4 + fA2*fB3
                + fA3*fB2 - fA4*fB1 + fA5*fB0 );
    if ( MathRealFn->Abs(fInvDet) < fZeroTolerance )
        return false;
    fInvDet = MathRealFn->Invert(fInvDet);

    outInvMatrix.m00 = ( + (m11 * fB5) - (m12 * fB4) + (m13 * fB3) ) * fInvDet;
    outInvMatrix.m01 = ( - (m01 * fB5) + (m02 * fB4) - (m03 * fB3) ) * fInvDet;
    outInvMatrix.m02 = ( + (m31 * fA5) - (m32 * fA4) + (m33 * fA3) ) * fInvDet;
    outInvMatrix.m03 = ( - (m21 * fA5) + (m22 * fA4) - (m23 * fA3) ) * fInvDet;

    outInvMatrix.m10 = ( - (m10 * fB5) + (m12 * fB2) - (m13 * fB1) ) * fInvDet;
    outInvMatrix.m11 = ( + (m00 * fB5) - (m02 * fB2) + (m03 * fB1) ) * fInvDet;
    outInvMatrix.m12 = ( - (m30 * fA5) + (m32 * fA2) - (m33 * fA1) ) * fInvDet;
    outInvMatrix.m13 = ( + (m20 * fA5) - (m22 * fA2) + (m23 * fA1) ) * fInvDet;

    outInvMatrix.m20 = ( + (m10 * fB4) - (m11 * fB2) + (m13 * fB0) ) * fInvDet;
    outInvMatrix.m21 = ( - (m00 * fB4) + (m01 * fB2) - (m03 * fB0) ) * fInvDet;
    outInvMatrix.m22 = ( + (m30 * fA4) - (m31 * fA2) + (m33 * fA0) ) * fInvDet;
    outInvMatrix.m23 = ( - (m20 * fA4) + (m21 * fA2) - (m23 * fA0) ) * fInvDet;

    outInvMatrix.m30 = ( - (m10 * fB3) + (m11 * fB1) - (m12 * fB0) ) * fInvDet;
    outInvMatrix.m31 = ( + (m00 * fB3) - (m01 * fB1) + (m02 * fB0) ) * fInvDet;
    outInvMatrix.m32 = ( - (m30 * fA3) + (m31 * fA1) - (m32 * fA0) ) * fInvDet;
    outInvMatrix.m33 = ( + (m20 * fA3) - (m21 * fA1) + (m22 * fA0) ) * fInvDet;

    return true;
}

template<typename Real>
Void TMatrix4<Real>::OrthoNormalize()
{
    Real fInvLen = MathRealFn->InvSqrt( (m00 * m00) + (m10 * m10) + (m20 * m20) );
    m00 *= fInvLen;
    m10 *= fInvLen;
    m20 *= fInvLen;

    Real fDot0 = ( (m00 * m01) + (m10 * m11) + (m20 * m21) );
    m01 -= ( fDot0 * m00 );
    m11 -= ( fDot0 * m10 );
    m21 -= ( fDot0 * m20 );

    fInvLen = MathRealFn->InvSqrt( (m01 * m01) + (m11 * m11) + (m21 * m21) );
    m01 *= fInvLen;
    m11 *= fInvLen;
    m21 *= fInvLen;

    Real fDot1 = ( (m01 * m02) + (m11 * m12) + (m21 * m22) );
    fDot0 = ( (m00 * m02) + (m10 * m12) + (m20 * m22) );
    m02 -= ( (fDot0 * m00) + (fDot1 * m01) );
    m12 -= ( (fDot0 * m10) + (fDot1 * m11) );
    m22 -= ( (fDot0 * m20) + (fDot1 * m21) );

    fInvLen = MathRealFn->InvSqrt( (m02 * m02) + (m12 * m12) + (m22 * m22) );
    m02 *= fInvLen;
    m12 *= fInvLen;
    m22 *= fInvLen;
}

template<typename Real>
inline Real TMatrix4<Real>::QuadraticForm(const TVector4<Real> & v0, const TVector4<Real> & v1) const {
    return ( v0 * ( (*this) * v1 ) );
}



