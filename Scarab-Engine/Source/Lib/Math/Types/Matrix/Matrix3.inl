/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Matrix/Matrix3.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : 3D matrix
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TMatrix3 implementation
template<typename Real>
const Real TMatrix3<Real>::ZERO = (Real)0;
template<typename Real>
const Real TMatrix3<Real>::ONE = (Real)1;
template<typename Real>
const Real TMatrix3<Real>::TWO = (Real)2;

template<typename Real>
const TMatrix3<Real> TMatrix3<Real>::Null = TMatrix3<Real>( ZERO, ZERO, ZERO,
                                                            ZERO, ZERO, ZERO,
                                                            ZERO, ZERO, ZERO );
template<typename Real>
const TMatrix3<Real> TMatrix3<Real>::Identity = TMatrix3<Real>( ONE,  ZERO, ZERO,
                                                                ZERO, ONE,  ZERO,
                                                                ZERO, ZERO, ONE );

template<typename Real>
TMatrix3<Real>::TMatrix3()
{
    // nothing to do
}
template<typename Real>
TMatrix3<Real>::TMatrix3( Real a00, Real a01, Real a02,
                          Real a10, Real a11, Real a12,
                          Real a20, Real a21, Real a22 )
{
    m00 = a00; m01 = a01; m02 = a02;
    m10 = a10; m11 = a11; m12 = a12;
    m20 = a20; m21 = a21; m22 = a22;
}
template<typename Real>
TMatrix3<Real>::TMatrix3(const Real v0[3], const Real v1[3], const Real v2[3], Bool bRows)
{
	if (bRows) {
        m00 = v0[0]; m01 = v0[1]; m02 = v0[2];
        m10 = v1[0]; m11 = v1[1]; m12 = v1[2];
        m20 = v2[0]; m21 = v2[1]; m22 = v2[2];
    } else {
        m00 = v0[0]; m01 = v1[0]; m02 = v2[0];
        m10 = v0[1]; m11 = v1[1]; m12 = v2[1];
        m20 = v0[2]; m21 = v1[2]; m22 = v2[2];
    }
}
template<typename Real>
TMatrix3<Real>::TMatrix3(const Real arrMat[9], Bool bRows)
{
	if (bRows) {
        m00 = arrMat[0];  m01 = arrMat[1];  m02 = arrMat[2];
        m10 = arrMat[3];  m11 = arrMat[4];  m12 = arrMat[5];
        m20 = arrMat[6];  m21 = arrMat[7];  m22 = arrMat[8];
    } else {
        m00 = arrMat[0];  m01 = arrMat[3];  m02 = arrMat[6];
        m10 = arrMat[1];  m11 = arrMat[4];  m12 = arrMat[7];
        m20 = arrMat[2];  m21 = arrMat[5];  m22 = arrMat[8];
    }
}
template<typename Real>
TMatrix3<Real>::TMatrix3(const TVector3<Real> & v0, const TVector3<Real> & v1, const TVector3<Real> & v2, Bool bRows)
{
	if (bRows) {
        m00 = v0.X; m01 = v0.Y; m02 = v0.Z;
        m10 = v1.X; m11 = v1.Y; m12 = v1.Z;
        m20 = v2.X; m21 = v2.Y; m22 = v2.Z;
    } else {
        m00 = v0.X; m01 = v1.X; m02 = v2.X;
        m10 = v0.Y; m11 = v1.Y; m12 = v2.Y;
        m20 = v0.Z; m21 = v1.Z; m22 = v2.Z;
    }
}
template<typename Real>
TMatrix3<Real>::TMatrix3(const TVector3<Real> vMat[3], Bool bRows)
{
	if (bRows) {
        m00 = vMat[0].X; m01 = vMat[0].Y; m02 = vMat[0].Z;
        m10 = vMat[1].X; m11 = vMat[1].Y; m12 = vMat[1].Z;
        m20 = vMat[2].X; m21 = vMat[2].Y; m22 = vMat[2].Z;
    } else {
        m00 = vMat[0].X; m01 = vMat[1].X; m02 = vMat[2].X;
        m10 = vMat[0].Y; m11 = vMat[1].Y; m12 = vMat[2].Y;
        m20 = vMat[0].Z; m21 = vMat[1].Z; m22 = vMat[2].Z;
    }
}
template<typename Real>
TMatrix3<Real>::TMatrix3(const TMatrix2<Real> & rhs)
{
    m00 = rhs.m00; m01 = rhs.m01; m02 = ZERO;
    m10 = rhs.m10; m11 = rhs.m11; m12 = ZERO;
    m20 = ZERO;   m21 = ZERO;     m22 = ONE;
}
template<typename Real>
TMatrix3<Real>::TMatrix3(const TMatrix3<Real> & rhs)
{
    m00 = rhs.m00; m01 = rhs.m01; m02 = rhs.m02;
    m10 = rhs.m10; m11 = rhs.m11; m12 = rhs.m12;
    m20 = rhs.m20; m21 = rhs.m21; m22 = rhs.m22;
}
template<typename Real>
TMatrix3<Real>::TMatrix3(const TMatrix4<Real> & rhs)
{
    m00 = rhs.m00; m01 = rhs.m01; m02 = rhs.m02;
    m10 = rhs.m10; m11 = rhs.m11; m12 = rhs.m12;
    m20 = rhs.m20; m21 = rhs.m21; m22 = rhs.m22;
}
template<typename Real>
TMatrix3<Real>::~TMatrix3()
{
    // nothing to do
}

template<typename Real>
inline TMatrix3<Real> & TMatrix3<Real>::operator=(const TMatrix3<Real> & rhs) {
    m00 = rhs.m00; m01 = rhs.m01; m02 = rhs.m02;
    m10 = rhs.m10; m11 = rhs.m11; m12 = rhs.m12;
    m20 = rhs.m20; m21 = rhs.m21; m22 = rhs.m22;
    return (*this);
}

template<typename Real>
inline TMatrix3<Real>::operator Real*() const {
    return (Real*)this;
}
template<typename Real>
inline TMatrix3<Real>::operator const Real*() const {
    return (const Real*)this;
}

template<typename Real>
inline Real & TMatrix3<Real>::operator[](Int i) {
    return *( ((Real*)this) + i );
}
template<typename Real>
inline Real TMatrix3<Real>::operator[](Int i) const {
    return *( ((const Real*)this) + i );
}
template<typename Real>
inline Real & TMatrix3<Real>::operator[](UInt i) {
    return *( ((Real*)this) + i );
}
template<typename Real>
inline Real TMatrix3<Real>::operator[](UInt i) const {
    return *( ((const Real*)this) + i );
}

template<typename Real>
inline Real & TMatrix3<Real>::operator()(Int iRow, Int iColumn) {
    return *( ((Real*)this) + (((iRow<<1) + iRow) + iColumn) );
}
template<typename Real>
inline Real TMatrix3<Real>::operator()(Int iRow, Int iColumn) const {
    return *( ((const Real*)this) + (((iRow<<1) + iRow) + iColumn) );
}
template<typename Real>
inline Real & TMatrix3<Real>::operator()(UInt iRow, UInt iColumn) {
    return *( ((Real*)this) + (((iRow<<1) + iRow) + iColumn) );
}
template<typename Real>
inline Real TMatrix3<Real>::operator()(UInt iRow, UInt iColumn) const {
    return *( ((const Real*)this) + (((iRow<<1) + iRow) + iColumn) );
}

template<typename Real>
inline TMatrix3<Real> TMatrix3<Real>::operator+() const {
    return TMatrix3<Real>( m00, m01, m02,
                           m10, m11, m12,
                           m20, m21, m22 );
}
template<typename Real>
inline TMatrix3<Real> TMatrix3<Real>::operator-() const {
    return TMatrix3<Real>( -m00, -m01, -m02,
                           -m10, -m11, -m12,
                           -m20, -m21, -m22 );
}

template<typename Real>
Bool TMatrix3<Real>::operator==(const TMatrix3<Real> & rhs) const
{
    return ( MathRealFn->Equals(m00, rhs.m00) && MathRealFn->Equals(m01, rhs.m01) && MathRealFn->Equals(m02, rhs.m02) &&
             MathRealFn->Equals(m10, rhs.m10) && MathRealFn->Equals(m11, rhs.m11) && MathRealFn->Equals(m12, rhs.m12) &&
             MathRealFn->Equals(m20, rhs.m20) && MathRealFn->Equals(m21, rhs.m21) && MathRealFn->Equals(m22, rhs.m22) );
}
template<typename Real>
Bool TMatrix3<Real>::operator!=(const TMatrix3<Real> & rhs) const
{
    return ( !(MathRealFn->Equals(m00, rhs.m00)) || !(MathRealFn->Equals(m01, rhs.m01)) || !(MathRealFn->Equals(m02, rhs.m02)) ||
             !(MathRealFn->Equals(m10, rhs.m10)) || !(MathRealFn->Equals(m11, rhs.m11)) || !(MathRealFn->Equals(m12, rhs.m12)) ||
             !(MathRealFn->Equals(m20, rhs.m20)) || !(MathRealFn->Equals(m21, rhs.m21)) || !(MathRealFn->Equals(m22, rhs.m22)) );
}

template<typename Real>
TMatrix3<Real> TMatrix3<Real>::operator*(Real rhs) const
{
    return TMatrix3<Real>( m00 * rhs, m01 * rhs, m02 * rhs,
                           m10 * rhs, m11 * rhs, m12 * rhs,
                           m20 * rhs, m21 * rhs, m22 * rhs );
}
template<typename Real>
TMatrix3<Real> TMatrix3<Real>::operator/(Real rhs) const
{
    return TMatrix3<Real>( m00 / rhs, m01 / rhs, m02 / rhs,
                           m10 / rhs, m11 / rhs, m12 / rhs,
                           m20 / rhs, m21 / rhs, m22 / rhs );
}

template<typename Real>
TMatrix3<Real> & TMatrix3<Real>::operator*=(Real rhs)
{
    m00 *= rhs; m01 *= rhs; m02 *= rhs;
    m10 *= rhs; m11 *= rhs; m12 *= rhs;
    m20 *= rhs; m21 *= rhs; m22 *= rhs;
    return (*this);
}
template<typename Real>
TMatrix3<Real> & TMatrix3<Real>::operator/=(Real rhs)
{
    m00 /= rhs; m01 /= rhs; m02 /= rhs;
    m10 /= rhs; m11 /= rhs; m12 /= rhs;
    m20 /= rhs; m21 /= rhs; m22 /= rhs;
    return (*this);
}

template<typename Real>
TVertex3<Real> TMatrix3<Real>::operator*(const TVertex3<Real> & rhs) const
{
    return TVertex3<Real>( (m00 * rhs.X + m01 * rhs.Y + m02 * rhs.Z),
                           (m10 * rhs.X + m11 * rhs.Y + m12 * rhs.Z),
                           (m20 * rhs.X + m21 * rhs.Y + m22 * rhs.Z) );
}
template<typename Real>
TVector3<Real> TMatrix3<Real>::operator*(const TVector3<Real> & rhs) const
{
    return TVector3<Real>( (m00 * rhs.X + m01 * rhs.Y + m02 * rhs.Z),
                           (m10 * rhs.X + m11 * rhs.Y + m12 * rhs.Z),
                           (m20 * rhs.X + m21 * rhs.Y + m22 * rhs.Z) );
}

template<typename Real>
TMatrix3<Real> TMatrix3<Real>::operator+(const TMatrix3<Real> & rhs) const
{
    return TMatrix3<Real>( m00 + rhs.m00, m01 + rhs.m01, m02 + rhs.m02,
                           m10 + rhs.m10, m11 + rhs.m11, m12 + rhs.m12,
                           m20 + rhs.m20, m21 + rhs.m21, m22 + rhs.m22 );
}
template<typename Real>
TMatrix3<Real> TMatrix3<Real>::operator-(const TMatrix3<Real> & rhs) const
{
    return TMatrix3<Real>( m00 - rhs.m00, m01 - rhs.m01, m02 - rhs.m02,
                           m10 - rhs.m10, m11 - rhs.m11, m12 - rhs.m12,
                           m20 - rhs.m20, m21 - rhs.m21, m22 - rhs.m22 );
}
template<typename Real>
TMatrix3<Real> TMatrix3<Real>::operator*(const TMatrix3<Real> & rhs) const
{
    return TMatrix3<Real> (
        (m00 * rhs.m00 + m01 * rhs.m10 + m02 * rhs.m20),
        (m00 * rhs.m01 + m01 * rhs.m11 + m02 * rhs.m21),
        (m00 * rhs.m02 + m01 * rhs.m12 + m02 * rhs.m22),

	    (m10 * rhs.m00 + m11 * rhs.m10 + m12 * rhs.m20),
        (m10 * rhs.m01 + m11 * rhs.m11 + m12 * rhs.m21),
        (m10 * rhs.m02 + m11 * rhs.m12 + m12 * rhs.m22),

	    (m20 * rhs.m00 + m21 * rhs.m10 + m22 * rhs.m20),
        (m20 * rhs.m01 + m21 * rhs.m11 + m22 * rhs.m21),
        (m20 * rhs.m02 + m21 * rhs.m12 + m22 * rhs.m22)
    );
}

template<typename Real>
TMatrix3<Real> & TMatrix3<Real>::operator+=(const TMatrix3<Real> & rhs)
{
    m00 += rhs.m00; m01 += rhs.m01; m02 += rhs.m02;
	m10 += rhs.m10; m11 += rhs.m11; m12 += rhs.m12;
	m20 += rhs.m20; m21 += rhs.m21; m22 += rhs.m22;
    return (*this);
}
template<typename Real>
TMatrix3<Real> & TMatrix3<Real>::operator-=(const TMatrix3<Real> & rhs)
{
    m00 -= rhs.m00; m01 -= rhs.m01; m02 -= rhs.m02;
	m10 -= rhs.m10; m11 -= rhs.m11; m12 -= rhs.m12;
	m20 -= rhs.m20; m21 -= rhs.m21; m22 -= rhs.m22;
    return (*this);
}
template<typename Real>
TMatrix3<Real> & TMatrix3<Real>::operator*=(const TMatrix3<Real> & rhs)
{
    Real f0 = (m00 * rhs.m00 + m01 * rhs.m10 + m02 * rhs.m20);
    Real f1 = (m00 * rhs.m01 + m01 * rhs.m11 + m02 * rhs.m21);
    Real f2 = (m00 * rhs.m02 + m01 * rhs.m12 + m02 * rhs.m22);
    m00 = f0; m01 = f1; m02 = f2;

    f0 = (m10 * rhs.m00 + m11 * rhs.m10 + m12 * rhs.m20);
    f1 = (m10 * rhs.m01 + m11 * rhs.m11 + m12 * rhs.m21);
    f2 = (m10 * rhs.m02 + m11 * rhs.m12 + m12 * rhs.m22);
    m10 = f0; m11 = f1; m12 = f2;

    f0 = (m20 * rhs.m00 + m21 * rhs.m10 + m22 * rhs.m20);
    f1 = (m20 * rhs.m01 + m21 * rhs.m11 + m22 * rhs.m21);
    f2 = (m20 * rhs.m02 + m21 * rhs.m12 + m22 * rhs.m22);
    m20 = f0; m21 = f1; m22 = f2;

    return (*this);
}

template<typename Real>
inline Void TMatrix3<Real>::GetRow(TVector3<Real> & outRow, UInt iRow) const {
    const Real * Values = ( ((const Real*)this) + ((iRow<<1) + iRow) );
    outRow.X = Values[0];
    outRow.Y = Values[1];
    outRow.Z = Values[2];
}
template<typename Real>
inline Void TMatrix3<Real>::SetRow(UInt iRow, Real fRow0, Real fRow1, Real fRow2) {
    Real * Values = ( ((Real*)this) + ((iRow<<1) + iRow) );
    Values[0] = fRow0;
    Values[1] = fRow1;
    Values[2] = fRow2;
}
template<typename Real>
inline Void TMatrix3<Real>::SetRow(UInt iRow, const Real vRow[3]) {
    Real * Values = ( ((Real*)this) + ((iRow<<1) + iRow) );
    Values[0] = vRow[0];
    Values[1] = vRow[1];
    Values[2] = vRow[2];
}
template<typename Real>
inline Void TMatrix3<Real>::SetRow(UInt iRow, const TVector3<Real> & vRow) {
    Real * Values = ( ((Real*)this) + ((iRow<<1) + iRow) );
    Values[0] = vRow.X;
    Values[1] = vRow.Y;
    Values[2] = vRow.Z;
}

template<typename Real>
inline Void TMatrix3<Real>::GetColumn(TVector3<Real> & outColumn, UInt iColumn) const {
    const Real * Values = ( ((const Real*)this) + iColumn );
    outColumn.X = Values[0];
    outColumn.Y = Values[3];
    outColumn.Z = Values[6];
}
template<typename Real>
inline Void TMatrix3<Real>::SetColumn(UInt iColumn, Real fCol0, Real fCol1, Real fCol2) {
    Real * Values = ( ((Real*)this) + iColumn );
    Values[0] = fCol0;
    Values[3] = fCol1;
    Values[6] = fCol2;
}
template<typename Real>
inline Void TMatrix3<Real>::SetColumn(UInt iColumn, const Real vCol[3]) {
    Real * Values = ( ((Real*)this) + iColumn );
    Values[0] = vCol[0];
    Values[3] = vCol[1];
    Values[6] = vCol[2];
}
template<typename Real>
inline Void TMatrix3<Real>::SetColumn(UInt iColumn, const TVector3<Real> & vCol) {
    Real * Values = ( ((Real*)this) + iColumn );
    Values[0] = vCol.X;
    Values[3] = vCol.Y;
    Values[6] = vCol.Z;
}

template<typename Real>
inline Void TMatrix3<Real>::MakeNull() {
    m00 = ZERO; m01 = ZERO; m02 = ZERO;
    m10 = ZERO; m11 = ZERO; m12 = ZERO;
    m20 = ZERO; m21 = ZERO; m22 = ZERO;
}
template<typename Real>
inline Void TMatrix3<Real>::MakeIdentity() {
    m00 = ONE;  m01 = ZERO; m02 = ZERO;
    m10 = ZERO; m11 = ONE;  m12 = ZERO;
    m20 = ZERO; m21 = ZERO; m22 = ONE;
}
template<typename Real>
inline Void TMatrix3<Real>::MakeDiagonal(const TVector3<Real> & vDiagonal) {
    m00 = vDiagonal.X; m01 = ZERO;        m02 = ZERO;
    m10 = ZERO;        m11 = vDiagonal.Y; m12 = ZERO;
    m20 = ZERO;        m21 = ZERO;        m22 = vDiagonal.Z;
}
template<typename Real>
inline Void TMatrix3<Real>::MakeScale(const TVector3<Real> & vScale) {
    m00 = vScale.X; m01 = ZERO;     m02 = ZERO;
    m10 = ZERO;     m11 = vScale.Y; m12 = ZERO;
    m20 = ZERO;     m21 = ZERO;     m22 = vScale.Z;
}
template<typename Real>
inline Void TMatrix3<Real>::MakeBasis(const TVertex2<Real> & vOrigin, const TVector2<Real> & vI, const TVector2<Real> & vJ) {
    m00 = vI.X; m01 = vJ.X; m02 = vOrigin.X;
    m10 = vI.Y; m11 = vJ.Y; m12 = vOrigin.Y;
    m20 = ZERO; m21 = ZERO; m22 = ONE;
}
template<typename Real>
inline Void TMatrix3<Real>::MakeBasis(const TVector3<Real> & vI, const TVector3<Real> & vJ, const TVector3<Real> & vK) {
    m00 = vI.X; m01 = vJ.X; m02 = vK.X;
    m10 = vI.Y; m11 = vJ.Y; m12 = vK.Y;
    m20 = vI.Z; m21 = vJ.Z; m22 = vK.Z;
}
template<typename Real>
inline Void TMatrix3<Real>::MakeSkewSymmetric(const TVector3<Real> & vSkew) {
    m00 = ZERO;		m01 = -vSkew.Z; m02 = vSkew.Y;
	m10 = vSkew.Z;	m11 = ZERO;		m12 = -vSkew.X;
	m20 = -vSkew.Y; m21 = vSkew.X;	m22 = ZERO;
}

template<typename Real>
Void TMatrix3<Real>::MakeRotate(Axis rotAxis, Real rotAngle)
{
    Real fSin, fCos;
	MathRealFn->SinCos( &fSin, &fCos, rotAngle );
	switch(rotAxis) {
	    case AXIS_X:
		    m00 = ONE;  m01 = ZERO; m02 = ZERO;
		    m10 = ZERO; m11 = fCos;	m12 = -fSin;
		    m20 = ZERO; m21 = fSin;	m22 = fCos;
		    break;
	    case AXIS_Y:
		    m00 = fCos;	 m01 = ZERO; m02 = fSin;
		    m10 = ZERO;  m11 = ONE;  m12 = ZERO;
		    m20 = -fSin; m21 = ZERO; m22 = fCos;
		    break;
	    case AXIS_Z:
		    m00 = fCos;	m01 = -fSin; m02 = ZERO;
		    m10 = fSin;	m11 = fCos;	 m12 = ZERO;
		    m20 = ZERO; m21 = ZERO;  m22 = ONE;
		    break;
	    default: break;
	}
}
template<typename Real>
Void TMatrix3<Real>::MakeRotate(const TVector3<Real> & vAxis, Real rotAngle)
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
	m00 = (t*xx + fCos); m01 = (t*xy - sz);	  m02 = (t*xz + sy);
	m10 = (t*xy + sz);	 m11 = (t*yy + fCos); m12 = (t*yz - sx);
	m20 = (t*xz - sy);	 m21 = (t*yz + sx);	  m22 = (t*zz + fCos);
}
template<typename Real>
Void TMatrix3<Real>::MakeRotate(Real fYaw, Real fPitch, Real fRoll, EulerAngles eulerAnglesOrder)
{
    TMatrix3<Real> matYaw, matPitch, matRoll;
    matYaw.MakeRotate(   (Axis)( (eulerAnglesOrder>>4) & 0x03 ), fYaw );
    matPitch.MakeRotate( (Axis)( (eulerAnglesOrder>>2) & 0x03 ), fPitch );
    matRoll.MakeRotate(  (Axis)( eulerAnglesOrder & 0x03 ), fRoll );
    (*this) = ( matRoll * matPitch * matYaw );
}
template<typename Real>
Void TMatrix3<Real>::MakeTensorProduct(const TVector3<Real> & vU, const TVector3<Real> & vV)
{
    m00 = (vU.X * vV.X); m01 = (vU.X * vV.Y); m02 = (vU.X * vV.Z);
    m10 = (vU.Y * vV.X); m11 = (vU.Y * vV.Y); m22 = (vU.Y * vV.Z);
    m20 = (vU.Z * vV.X); m21 = (vU.Z * vV.Y); m22 = (vU.Z * vV.Z);
}

template<typename Real>
Void TMatrix3<Real>::GetAxisAngle(TVector3<Real> & outAxis, Real & outAngle) const
{
    const Real HALF = (Real)0.5f;
    Real fTrace = m00 + m11 + m22;
    Real fCos = HALF * (fTrace - ONE);
    outAngle = MathRealFn->ArcCos(fCos);
    if ( outAngle > ZERO ) {
        if ( outAngle < (Real)SCALAR_PI ) {
            outAxis.X = m21 - m12;
            outAxis.Y = m02 - m20;
            outAxis.Z = m10 - m01;
            outAxis.Normalize();
        } else {
            Real fHalfInverse;
            if (m00 >= m11) {
                if (m00 >= m22) {
                    outAxis.X = HALF * MathRealFn->Sqrt( m00 - m11 - m22 + ONE );
                    fHalfInverse = HALF / outAxis.X;
                    outAxis.Y = fHalfInverse * m01;
                    outAxis.Z = fHalfInverse * m02;
                } else {
                    outAxis.Z = HALF * MathRealFn->Sqrt( m22 - m00 - m11 + ONE );
                    fHalfInverse = HALF / outAxis.Z;
                    outAxis.X = fHalfInverse * m02;
                    outAxis.Y = fHalfInverse * m12;
                }
            } else {
                if (m11 >= m22) {
                    outAxis.Y = HALF * MathRealFn->Sqrt( m11 - m00 - m22 + ONE );
                    fHalfInverse = HALF / outAxis.Y;
                    outAxis.X = fHalfInverse * m01;
                    outAxis.Z = fHalfInverse * m12;
                } else {
                    outAxis.Z = HALF * MathRealFn->Sqrt( m22 - m00 - m11 + ONE );
                    fHalfInverse = HALF / outAxis.Z;
                    outAxis.X = fHalfInverse * m02;
                    outAxis.Y = fHalfInverse * m12;
                }
            }
        }
    } else {
        outAxis.X = ONE;
        outAxis.Y = ZERO;
        outAxis.Z = ZERO;
    }
}

template<typename Real>
inline Void TMatrix3<Real>::Transpose(TMatrix3<Real> & outTransposedMatrix) const {
    outTransposedMatrix.m00 = m00; outTransposedMatrix.m01 = m10; outTransposedMatrix.m02 = m20;
    outTransposedMatrix.m10 = m01; outTransposedMatrix.m11 = m11; outTransposedMatrix.m12 = m21;
    outTransposedMatrix.m20 = m02; outTransposedMatrix.m21 = m12; outTransposedMatrix.m22 = m22;
}

template<typename Real>
inline Void TMatrix3<Real>::Minor(TMatrix2<Real> & outMinor, UInt iRow, UInt iColumn) const {
    // Ok we get tricky here ... use simple polynoms to map
	// rows correctly according to the index to exclude
	// This is much faster than branching
	// First row/col mapping is :  0=>1, 1=>0, 2=>0 so p0(x) = (x-1)(x-2)/2
	// Second row/col mapping is : 0=>2, 1=>2, 2=>1 so p1(x) = -(x*(x-1) - 4)/2
    // then we have Row0 = 3*p0(x) = (x-1)(x-2) * 3/2
    //          and Row1 = 3*p1(x) = -(x*(x-1) - 4) * 3/2
    Int sRow = (signed)iRow, sCol = (signed)iColumn;
	Int Row0 = ( ( 3 * (sRow-1) * (sRow-2) ) >> 1 );
	Int Col0 = ( ( (sCol-1) * (sCol-2) ) >> 1 );
	Int Row1 = -( ( 3 * (sRow * (sRow-1) - 4) ) >> 1 );
	Int Col1 = -( (sCol * (sCol-1) - 4) >> 1 );
    const Real * Values = ( (const Real*)this );
    outMinor.m00 = Values[Row0 + Col0]; outMinor.m01 = Values[Row0 + Col1];
    outMinor.m10 = Values[Row1 + Col0]; outMinor.m11 = Values[Row1 + Col1];
}

template<typename Real>
Real TMatrix3<Real>::Determinant() const
{
    Real fC00, fC01, fC02, fDet;

    fC00 = m11 * m22 - m21 * m12;
    fC01 = m10 * m22 - m20 * m12;
    fC02 = m10 * m21 - m20 * m11;

    fDet = m00 * fC00 - m01 * fC01 + m02 * fC02;
    return fDet;
}

template<typename Real>
Void TMatrix3<Real>::Adjoint(TMatrix3<Real> & outAdjointMatrix) const
{
    outAdjointMatrix.m00 = +(m11 * m22 - m21 * m12);
    outAdjointMatrix.m01 = -(m01 * m22 - m21 * m02);
    outAdjointMatrix.m02 = +(m01 * m12 - m11 * m02);

    outAdjointMatrix.m10 = -(m10 * m22 - m20 * m12);
    outAdjointMatrix.m11 = +(m00 * m22 - m20 * m02);
    outAdjointMatrix.m12 = -(m00 * m12 - m10 * m02);

    outAdjointMatrix.m20 = +(m10 * m21 - m20 * m11);
    outAdjointMatrix.m21 = -(m00 * m21 - m20 * m01);
    outAdjointMatrix.m22 = +(m00 * m11 - m10 * m01);
}

template<typename Real>
Bool TMatrix3<Real>::Invert(TMatrix3<Real> & outInvMatrix, Real fZeroTolerance) const
{
    Real fC00, fC01, fC02;

    fC00 = m11 * m22 - m21 * m12;
    fC01 = m10 * m22 - m20 * m12;
    fC02 = m10 * m21 - m20 * m11;

    Real fInvDet = m00 * fC00 - m01 * fC01 + m02 * fC02;
    if ( MathRealFn->Abs(fInvDet) < fZeroTolerance )
        return false;
    fInvDet = MathRealFn->Invert(fInvDet);

    outInvMatrix.m00 = +fC00 * fInvDet;
    outInvMatrix.m01 = -(m01 * m22 - m21 * m02) * fInvDet;
    outInvMatrix.m02 = +(m01 * m12 - m11 * m02) * fInvDet;

    outInvMatrix.m10 = -fC01 * fInvDet;
    outInvMatrix.m11 = +(m00 * m22 - m20 * m02) * fInvDet;
    outInvMatrix.m12 = -(m00 * m12 - m10 * m02) * fInvDet;

    outInvMatrix.m20 = +fC02 * fInvDet;
    outInvMatrix.m21 = -(m00 * m21 - m20 * m01) * fInvDet;
    outInvMatrix.m22 = +(m00 * m11 - m10 * m01) * fInvDet;

    return true;
}

template<typename Real>
Void TMatrix3<Real>::OrthoNormalize()
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
Void TMatrix3<Real>::PolarDecomposition( TMatrix3<Real> & outRotate, TMatrix3<Real> & outScale ) const
{
    static const Real fPrecision = (Real)0.0001f;
    static const UInt iIterations = 16;

    TVector3<Real> vDiag( m00, m11, m22 );

    outRotate = (*this);
    outRotate *= ( vDiag.InvNorm() );
    Real fDet = outRotate.Determinant();
    if ( MathRealFn->EqualsZero(fDet) ) {
        outRotate.MakeIdentity();
        outScale.MakeIdentity();
        return;
    }

    TMatrix3<Real> matTemp, matInvert;
    Real fCurDet, fDetDiff;

    for( UInt i = 0; i < iIterations; ++i ) {
        outRotate.Adjoint( matTemp );
        matTemp *= MathRealFn->Invert( fDet );
        matTemp.Transpose( matInvert );
        outRotate += matInvert;
        outRotate *= (Real)0.5f;
        fCurDet = outRotate.Determinant();
        fDetDiff = ( fCurDet - fDet );
        if ( (fDetDiff * fDetDiff) <= fPrecision )
            break;
        fDet = fCurDet;
    }
    outRotate.OrthoNormalize();
    outRotate.Transpose( matTemp );
    outScale = ( matTemp * (*this) );
}

template<typename Real>
inline Real TMatrix3<Real>::QuadraticForm(const TVector3<Real> & v0, const TVector3<Real> & v1) const {
    return ( v0 * ( (*this) * v1 ) );
}

template<typename Real>
Void TMatrix3<Real>::SLerp(const TMatrix3<Real> & matSource, const TMatrix3<Real> & matTarget, Real fT)
{    
    matSource.Transpose(*this);
    (*this) *= matTarget;

    TVector3<Real> vAxis;
    Real fAngle;
    GetAxisAngle(vAxis, fAngle);
    MakeRotate(vAxis, fAngle * fT);
    (*this) = ( matSource * (*this) );
}

