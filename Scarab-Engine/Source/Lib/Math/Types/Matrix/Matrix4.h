/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Matrix/Matrix4.h
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
// Known Bugs : We use row-wise matrix representation, vertices & vectors are
//              considered as column tuples and transformations use right-hand
//              products such that u' = M*u.
//              Let M be a n*n matrix (n = 2,3,4), and Mij be the value at
//              row i, column j.
//              Representation as array[16] puts Mij at index (i*n + j).
//              Representation as individual values sets mIJ = Mij.
//              As an example, m12 is value at row 1, column 2, index (1*4+2 = 6),
//              in a 4*4 matrix.
//              In 4D, translations are stored in last column (m03,m13,m23,m33).
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_LIB_MATH_TYPES_MATRIX_MATRIX4_H
#define SCARAB_LIB_MATH_TYPES_MATRIX_MATRIX4_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Vector/Vector3.h"
#include "../Vector/Vector4.h"
#include "../Vertex/Vertex3.h"
#include "../Vertex/Vertex4.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#ifndef _MATRIX_AXIS_EULER_ANGLES_DECLARED
#define _MATRIX_AXIS_EULER_ANGLES_DECLARED
    enum Axis
    {
	    AXIS_X = 0,
	    AXIS_Y = 1,
	    AXIS_Z = 2
    };
    enum EulerAngles
    {
        // udv
	    EULER_ANGLES_XYZ = ( (AXIS_X << 4) | (AXIS_Y << 2) | AXIS_Z ),
	    EULER_ANGLES_XZY = ( (AXIS_X << 4) | (AXIS_Z << 2) | AXIS_Y ),
	    EULER_ANGLES_YXZ = ( (AXIS_Y << 4) | (AXIS_X << 2) | AXIS_Z ),
	    EULER_ANGLES_YZX = ( (AXIS_Y << 4) | (AXIS_Z << 2) | AXIS_X ),
	    EULER_ANGLES_ZXY = ( (AXIS_Z << 4) | (AXIS_X << 2) | AXIS_Y ),
	    EULER_ANGLES_ZYX = ( (AXIS_Z << 4) | (AXIS_Y << 2) | AXIS_X ),
        // udu
	    EULER_ANGLES_XYX = ( (AXIS_X << 4) | (AXIS_Y << 2) | AXIS_X ),
	    EULER_ANGLES_XZX = ( (AXIS_X << 4) | (AXIS_Z << 2) | AXIS_X ),
	    EULER_ANGLES_YXY = ( (AXIS_Y << 4) | (AXIS_X << 2) | AXIS_Y ),
	    EULER_ANGLES_YZY = ( (AXIS_Y << 4) | (AXIS_Z << 2) | AXIS_Y ),
	    EULER_ANGLES_ZXZ = ( (AXIS_Z << 4) | (AXIS_X << 2) | AXIS_Z ),
	    EULER_ANGLES_ZYZ = ( (AXIS_Z << 4) | (AXIS_Y << 2) | AXIS_Z )
    };
#endif

    // prototypes
template<typename Real>
class TMatrix2;
template<typename Real>
class TMatrix3;

/////////////////////////////////////////////////////////////////////////////////
// The TMatrix4 class
template<typename Real>
class TMatrix4
{
public:
    // Constant values
    static const TMatrix4<Real> Null; // Null matrix
    static const TMatrix4<Real> Identity; // Identity matrix

	// Constructors
	TMatrix4(); // uninitialized
	TMatrix4( Real a00, Real a01, Real a02, Real a03,
			  Real a10, Real a11, Real a12, Real a13,
			  Real a20, Real a21, Real a22, Real a23,
			  Real a30, Real a31, Real a32, Real a33 );
	TMatrix4(const Real v0[4], const Real v1[4], const Real v2[4], const Real v3[4], Bool bRows = true);
	TMatrix4(const Real arrMat[16], Bool bRows = true);
	TMatrix4(const TVector4<Real> & v0, const TVector4<Real> & v1,
             const TVector4<Real> & v2, const TVector4<Real> & v3, Bool bRows = true);
	TMatrix4(const TVector4<Real> vMat[4], Bool bRows = true);
	TMatrix4(const TMatrix2<Real> & rhs);
	TMatrix4(const TMatrix3<Real> & rhs);
	TMatrix4(const TMatrix4<Real> & rhs);
	~TMatrix4();

	// Assignment operator
	inline TMatrix4<Real> & operator=(const TMatrix4<Real> & rhs);

	// Casting operations
	inline operator Real*() const;
    inline operator const Real*() const;

	// Index operations
		// flat index 0-15 (Row-wise !)
	inline Real & operator[](Int i);
    inline Real operator[](Int i) const;
	inline Real & operator[](UInt i);
	inline Real operator[](UInt i) const;
		// (row,col) index
	inline Real & operator()(Int iRow, Int iColumn);
    inline Real operator()(Int iRow, Int iColumn) const;
	inline Real & operator()(UInt iRow, UInt iColumn);
	inline Real operator()(UInt iRow, UInt iColumn) const;

	// Unary operations
	inline TMatrix4<Real> operator+() const;
	inline TMatrix4<Real> operator-() const;

	// Boolean operations
	Bool operator==(const TMatrix4<Real> & rhs) const;
	Bool operator!=(const TMatrix4<Real> & rhs) const;

    // Real operations
	TMatrix4<Real> operator*(Real rhs) const;
	TMatrix4<Real> operator/(Real rhs) const;

    TMatrix4<Real> & operator*=(Real rhs);
	TMatrix4<Real> & operator/=(Real rhs);

	// Vertex operations
	TVertex4<Real> operator*(const TVertex4<Real> & rhs) const;

	// Vector operations
	TVector4<Real> operator*(const TVector4<Real> & rhs) const;

	// Matrix operations
	TMatrix4<Real> operator+(const TMatrix4<Real> & rhs) const;
	TMatrix4<Real> operator-(const TMatrix4<Real> & rhs) const;
	TMatrix4<Real> operator*(const TMatrix4<Real> & rhs) const;

	TMatrix4<Real> & operator+=(const TMatrix4<Real> & rhs);
	TMatrix4<Real> & operator-=(const TMatrix4<Real> & rhs);
	TMatrix4<Real> & operator*=(const TMatrix4<Real> & rhs);

	// Methods : Row & Column access
	inline Void GetRow(TVector4<Real> & outRow, UInt iRow) const;
	inline Void SetRow(UInt iRow, Real fRow0, Real fRow1, Real fRow2, Real fRow3);
	inline Void SetRow(UInt iRow, const Real vRow[4]);
	inline Void SetRow(UInt iRow, const TVector4<Real> & vRow);

    inline Void GetColumn(TVector4<Real> & outColumn, UInt iColumn) const;
	inline Void SetColumn(UInt iColumn, Real fCol0, Real fCol1, Real fCol2, Real fCol3);
	inline Void SetColumn(UInt iColumn, const Real vCol[4]);
	inline Void SetColumn(UInt iColumn, const TVector4<Real> & vCol);

    // Methods : Makers
	inline Void MakeNull();
	inline Void MakeIdentity();
	inline Void MakeDiagonal(const TVector3<Real> & vDiagonal);
	inline Void MakeTranslate(const TVector3<Real> & vTranslate);
	inline Void MakeScale(const TVector3<Real> & vScale);
    inline Void MakeBasis(const TVertex3<Real> & vOrigin, const TVector3<Real> & vI,
                          const TVector3<Real> & vJ, const TVector3<Real> & vK);
	Void MakeRotate(Axis rotAxis, Real rotAngle);
	Void MakeRotate(const TVector3<Real> & vAxis, Real rotAngle);
    Void MakeRotate(Real fYaw, Real fPitch, Real fRoll, EulerAngles eulerAnglesOrder);
    Void MakeReflection(const TVector3<Real> & vNormal, const TVertex3<Real> & vOrigin);
    Void MakeObliqueProjection(const TVector3<Real> & vNormal, const TVertex3<Real> & vOrigin, const TVector3<Real> & vDirection);
    Void MakePerspectiveProjection(const TVector3<Real> & vNormal, const TVertex3<Real> & vOrigin, const TVertex3<Real> & vPosition);

    // Methods : Linear algebra stuff
    inline Void Transpose(TMatrix4<Real> & outTransposedMatrix) const;
    //inline TMatrix4<Real> TransposeMul(const TMatrix4<Real> & rhs) const; // Transpose(M) * rhs
    //inline TMatrix4<Real> MulTranspose(const TMatrix4<Real> & rhs) const; // M * Transpose(rhs)
    //inline TMatrix4<Real> TransposeMulTranspose(const TMatrix4<Real> & rhs) const; // Transpose(M) * Transpose(rhs)
    //inline TMatrix4<Real> DiagonalMul(const TVector4<Real> & rhs) const; // Diag(rhs) * M
    //inline TMatrix4<Real> MulDiagonal(const TVector4<Real> & rhs) const; // M * Diag(rhs)

    inline Void Minor(TMatrix3<Real> & outMinor, UInt iRow, UInt iColumn) const;
	Real Determinant() const;
    Void Adjoint(TMatrix4<Real> & outAdjointMatrix) const; // Transposed Co-Matrix
	Bool Invert(TMatrix4<Real> & outInvMatrix, Real fZeroTolerance = (Real)SCALAR_ERROR) const;

    Void OrthoNormalize(); // Gram-Schmidt, upper 3x3 only

    inline Real QuadraticForm(const TVector4<Real> & v0, const TVector4<Real> & v1) const; // Transpose(v0) * M * v1

	// Data
	Real m00, m01, m02, m03; // Row 0
	Real m10, m11, m12, m13; // Row 1
	Real m20, m21, m22, m23; // Row 2
	Real m30, m31, m32, m33; // Row 3

private:
    static const Real ZERO;
    static const Real ONE;
    static const Real TWO;
};

// Explicit instanciation
typedef TMatrix4<Float> Matrix4f;
typedef TMatrix4<Double> Matrix4d;
typedef TMatrix4<Scalar> Matrix4;

// Specializations
template<>
TMatrix4<Float> TMatrix4<Float>::operator*(Float rhs) const;
template<>
TMatrix4<Double> TMatrix4<Double>::operator*(Double rhs) const;
template<>
TMatrix4<Float> TMatrix4<Float>::operator/(Float rhs) const;
template<>
TMatrix4<Double> TMatrix4<Double>::operator/(Double rhs) const;

template<>
TMatrix4<Float> & TMatrix4<Float>::operator*=(Float rhs);
template<>
TMatrix4<Double> & TMatrix4<Double>::operator*=(Double rhs);
template<>
TMatrix4<Float> & TMatrix4<Float>::operator/=(Float rhs);
template<>
TMatrix4<Double> & TMatrix4<Double>::operator/=(Double rhs);

template<>
TVertex4<Float> TMatrix4<Float>::operator*(const TVertex4<Float> & rhs) const;
template<>
TVertex4<Double> TMatrix4<Double>::operator*(const TVertex4<Double> & rhs) const;

template<>
TVector4<Float> TMatrix4<Float>::operator*(const TVector4<Float> & rhs) const;
template<>
TVector4<Double> TMatrix4<Double>::operator*(const TVector4<Double> & rhs) const;

template<>
TMatrix4<Float> TMatrix4<Float>::operator+(const TMatrix4<Float> & rhs) const;
template<>
TMatrix4<Double> TMatrix4<Double>::operator+(const TMatrix4<Double> & rhs) const;
template<>
TMatrix4<Float> TMatrix4<Float>::operator-(const TMatrix4<Float> & rhs) const;
template<>
TMatrix4<Double> TMatrix4<Double>::operator-(const TMatrix4<Double> & rhs) const;
template<>
TMatrix4<Float> TMatrix4<Float>::operator*(const TMatrix4<Float> & rhs) const;
template<>
TMatrix4<Double> TMatrix4<Double>::operator*(const TMatrix4<Double> & rhs) const;

template<>
TMatrix4<Float> & TMatrix4<Float>::operator+=(const TMatrix4<Float> & rhs);
template<>
TMatrix4<Double> & TMatrix4<Double>::operator+=(const TMatrix4<Double> & rhs);
template<>
TMatrix4<Float> & TMatrix4<Float>::operator-=(const TMatrix4<Float> & rhs);
template<>
TMatrix4<Double> & TMatrix4<Double>::operator-=(const TMatrix4<Double> & rhs);
template<>
TMatrix4<Float> & TMatrix4<Float>::operator*=(const TMatrix4<Float> & rhs);
template<>
TMatrix4<Double> & TMatrix4<Double>::operator*=(const TMatrix4<Double> & rhs);

// TODO : SSE implementation
//template<>
//Void TMatrix4<Float>::MakeRotate(const TVector3<Float> & vAxis, Float rotAngle);
//template<>
//Void TMatrix4<Double>::MakeRotate(const TVector3<Double> & vAxis, Double rotAngle);

template<>
Float TMatrix4<Float>::Determinant() const;
template<>
Double TMatrix4<Double>::Determinant() const;

template<>
Void TMatrix4<Float>::Adjoint(TMatrix4<Float> & outAdjointMatrix) const;
template<>
Void TMatrix4<Double>::Adjoint(TMatrix4<Double> & outAdjointMatrix) const;

template<>
Bool TMatrix4<Float>::Invert(TMatrix4<Float> & outInvertMatrix, Float fZeroTolerance) const;
template<>
Bool TMatrix4<Double>::Invert(TMatrix4<Double> & outInvertMatrix, Double fZeroTolerance) const;

template<>
Void TMatrix4<Float>::OrthoNormalize();
template<>
Void TMatrix4<Double>::OrthoNormalize();
template<>
Void TMatrix4<Fixed>::OrthoNormalize();

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Matrix4.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_TYPES_MATRIX_MATRIX4_H

