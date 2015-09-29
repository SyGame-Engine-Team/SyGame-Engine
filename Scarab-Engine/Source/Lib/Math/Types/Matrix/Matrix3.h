/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Matrix/Matrix3.h
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
#ifndef SCARAB_LIB_MATH_TYPES_MATRIX_MATRIX3_H
#define SCARAB_LIB_MATH_TYPES_MATRIX_MATRIX3_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Vector/Vector2.h"
#include "../Vector/Vector3.h"
#include "../Vertex/Vertex2.h"
#include "../Vertex/Vertex3.h"

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
class TMatrix4;

/////////////////////////////////////////////////////////////////////////////////
// The TMatrix3 class
template<typename Real>
class TMatrix3
{
public:
    // Constant values
    static const TMatrix3<Real> Null; // Null matrix
    static const TMatrix3<Real> Identity; // Identity matrix

	// Constructors
	TMatrix3(); // uninitialized
	TMatrix3( Real a00, Real a01, Real a02,
			  Real a10, Real a11, Real a12,
			  Real a20, Real a21, Real a22 );
	TMatrix3(const Real v0[3], const Real v1[3], const Real v2[3], Bool bRows = true);
	TMatrix3(const Real arrMat[9], Bool bRows = true);
	TMatrix3(const TVector3<Real> & v0, const TVector3<Real> & v1, const TVector3<Real> & v2, Bool bRows = true);
	TMatrix3(const TVector3<Real> vMat[3], Bool bRows = true);
	TMatrix3(const TMatrix2<Real> & rhs);
	TMatrix3(const TMatrix3<Real> & rhs);
	TMatrix3(const TMatrix4<Real> & rhs);
	~TMatrix3();

	// Assignment operator
	inline TMatrix3<Real> & operator=(const TMatrix3<Real> & rhs);

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
	inline TMatrix3<Real> operator+() const;
	inline TMatrix3<Real> operator-() const;

	// Boolean operations
	Bool operator==(const TMatrix3<Real> & rhs) const;
	Bool operator!=(const TMatrix3<Real> & rhs) const;

    // Real operations
	TMatrix3<Real> operator*(Real rhs) const;
	TMatrix3<Real> operator/(Real rhs) const;

    TMatrix3<Real> & operator*=(Real rhs);
	TMatrix3<Real> & operator/=(Real rhs);

	// Vertex operations
	TVertex3<Real> operator*(const TVertex3<Real> & rhs) const;

	// Vector operations
	TVector3<Real> operator*(const TVector3<Real> & rhs) const;

	// Matrix operations
	TMatrix3<Real> operator+(const TMatrix3<Real> & rhs) const;
	TMatrix3<Real> operator-(const TMatrix3<Real> & rhs) const;
	TMatrix3<Real> operator*(const TMatrix3<Real> & rhs) const;

	TMatrix3<Real> & operator+=(const TMatrix3<Real> & rhs);
	TMatrix3<Real> & operator-=(const TMatrix3<Real> & rhs);
	TMatrix3<Real> & operator*=(const TMatrix3<Real> & rhs);

	// Methods : Row & Column access
	inline Void GetRow(TVector3<Real> & outRow, UInt iRow) const;
	inline Void SetRow(UInt iRow, Real fRow0, Real fRow1, Real fRow2);
	inline Void SetRow(UInt iRow, const Real vRow[3]);
	inline Void SetRow(UInt iRow, const TVector3<Real> & vRow);

    inline Void GetColumn(TVector3<Real> & outColumn, UInt iColumn) const;
	inline Void SetColumn(UInt iColumn, Real fCol0, Real fCol1, Real fCol2);
	inline Void SetColumn(UInt iColumn, const Real vCol[3]);
	inline Void SetColumn(UInt iColumn, const TVector3<Real> & vCol);

    // Methods : Makers
	inline Void MakeNull();
	inline Void MakeIdentity();
	inline Void MakeDiagonal(const TVector3<Real> & vDiagonal);
	inline Void MakeScale(const TVector3<Real> & vScale);
    inline Void MakeBasis(const TVertex2<Real> & vOrigin, const TVector2<Real> & vI, const TVector2<Real> & vJ);
    inline Void MakeBasis(const TVector3<Real> & vI, const TVector3<Real> & vJ, const TVector3<Real> & vK);
    inline Void MakeSkewSymmetric(const TVector3<Real> & vSkew);
	Void MakeRotate(Axis rotAxis, Real rotAngle);
	Void MakeRotate(const TVector3<Real> & vAxis, Real rotAngle);
    Void MakeRotate(Real fYaw, Real fPitch, Real fRoll, EulerAngles eulerAnglesOrder);
    Void MakeTensorProduct(const TVector3<Real> & vU, const TVector3<Real> & vV);

    // Methods : Linear algebra stuff
    Void GetAxisAngle(TVector3<Real> & outAxis, Real & outAngle) const;

    inline Void Transpose(TMatrix3<Real> & outTransposedMatrix) const;
    //inline TMatrix3<Real> TransposeMul(const TMatrix3<Real> & rhs) const; // Transpose(M) * rhs
    //inline TMatrix3<Real> MulTranspose(const TMatrix3<Real> & rhs) const; // M * Transpose(rhs)
    //inline TMatrix3<Real> TransposeMulTranspose(const TMatrix3<Real> & rhs) const; // Transpose(M) * Transpose(rhs)
    //inline TMatrix3<Real> DiagonalMul(const TVector3<Real> & rhs) const; // Diag(rhs) * M
    //inline TMatrix3<Real> MulDiagonal(const TVector3<Real> & rhs) const; // M * Diag(rhs)

    inline Void Minor(TMatrix2<Real> & outMinor, UInt iRow, UInt iColumn) const;
	Real Determinant() const;
    Void Adjoint(TMatrix3<Real> & outAdjointMatrix) const; // Transposed Co-Matrix
	Bool Invert(TMatrix3<Real> & outInvMatrix, Real fZeroTolerance = (Real)SCALAR_ERROR) const;

    Void OrthoNormalize(); // Gram-Schmidt

    Void PolarDecomposition( TMatrix3<Real> & outRotate, TMatrix3<Real> & outScale ) const;

    inline Real QuadraticForm(const TVector3<Real> & v0, const TVector3<Real> & v1) const; // Transpose(v0) * M * v1

    // Spherical interpolation
    Void SLerp(const TMatrix3<Real> & matSource, const TMatrix3<Real> & matTarget, Real fT);

	// Data
	Real m00, m01, m02; // Row 0
	Real m10, m11, m12; // Row 1
	Real m20, m21, m22; // Row 2

private:
    static const Real ZERO;
    static const Real ONE;
    static const Real TWO;
};

// Explicit instanciation
typedef TMatrix3<Float> Matrix3f;
typedef TMatrix3<Double> Matrix3d;
typedef TMatrix3<Scalar> Matrix3;

// Specializations
template<>
TMatrix3<Float> TMatrix3<Float>::operator*(Float rhs) const;
template<>
TMatrix3<Double> TMatrix3<Double>::operator*(Double rhs) const;
template<>
TMatrix3<Float> TMatrix3<Float>::operator/(Float rhs) const;
template<>
TMatrix3<Double> TMatrix3<Double>::operator/(Double rhs) const;

template<>
TMatrix3<Float> & TMatrix3<Float>::operator*=(Float rhs);
template<>
TMatrix3<Double> & TMatrix3<Double>::operator*=(Double rhs);
template<>
TMatrix3<Float> & TMatrix3<Float>::operator/=(Float rhs);
template<>
TMatrix3<Double> & TMatrix3<Double>::operator/=(Double rhs);

template<>
TVertex3<Float> TMatrix3<Float>::operator*(const TVertex3<Float> & rhs) const;
template<>
TVertex3<Double> TMatrix3<Double>::operator*(const TVertex3<Double> & rhs) const;

template<>
TVector3<Float> TMatrix3<Float>::operator*(const TVector3<Float> & rhs) const;
template<>
TVector3<Double> TMatrix3<Double>::operator*(const TVector3<Double> & rhs) const;

template<>
TMatrix3<Float> TMatrix3<Float>::operator+(const TMatrix3<Float> & rhs) const;
template<>
TMatrix3<Double> TMatrix3<Double>::operator+(const TMatrix3<Double> & rhs) const;
template<>
TMatrix3<Float> TMatrix3<Float>::operator-(const TMatrix3<Float> & rhs) const;
template<>
TMatrix3<Double> TMatrix3<Double>::operator-(const TMatrix3<Double> & rhs) const;
template<>
TMatrix3<Float> TMatrix3<Float>::operator*(const TMatrix3<Float> & rhs) const;
template<>
TMatrix3<Double> TMatrix3<Double>::operator*(const TMatrix3<Double> & rhs) const;

template<>
TMatrix3<Float> & TMatrix3<Float>::operator+=(const TMatrix3<Float> & rhs);
template<>
TMatrix3<Double> & TMatrix3<Double>::operator+=(const TMatrix3<Double> & rhs);
template<>
TMatrix3<Float> & TMatrix3<Float>::operator-=(const TMatrix3<Float> & rhs);
template<>
TMatrix3<Double> & TMatrix3<Double>::operator-=(const TMatrix3<Double> & rhs);
template<>
TMatrix3<Float> & TMatrix3<Float>::operator*=(const TMatrix3<Float> & rhs);
template<>
TMatrix3<Double> & TMatrix3<Double>::operator*=(const TMatrix3<Double> & rhs);

// TODO : SSE implementation
//template<>
//Void TMatrix3<Float>::MakeRotate(const TVector3<Float> & vAxis, Float rotAngle);
//template<>
//Void TMatrix3<Double>::MakeRotate(const TVector3<Double> & vAxis, Double rotAngle);

template<>
Float TMatrix3<Float>::Determinant() const;
template<>
Double TMatrix3<Double>::Determinant() const;

template<>
Void TMatrix3<Float>::Adjoint(TMatrix3<Float> & outAdjointMatrix) const;
template<>
Void TMatrix3<Double>::Adjoint(TMatrix3<Double> & outAdjointMatrix) const;

template<>
Bool TMatrix3<Float>::Invert(TMatrix3<Float> & outInvertMatrix, Float fZeroTolerance) const;
template<>
Bool TMatrix3<Double>::Invert(TMatrix3<Double> & outInvertMatrix, Double fZeroTolerance) const;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Matrix3.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_TYPES_MATRIX_MATRIX3_H

