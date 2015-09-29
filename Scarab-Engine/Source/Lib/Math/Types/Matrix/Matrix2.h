/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Matrix/Matrix2.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : 2D matrix
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
#ifndef SCARAB_LIB_MATH_TYPES_MATRIX_MATRIX2_H
#define SCARAB_LIB_MATH_TYPES_MATRIX_MATRIX2_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Vector/Vector2.h"
#include "../Vertex/Vertex2.h"

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
class TMatrix3;
template<typename Real>
class TMatrix4;

/////////////////////////////////////////////////////////////////////////////////
// The TMatrix2 class
template<typename Real>
class TMatrix2
{
public:
    // Constant values
    static const TMatrix2<Real> Null; // Null matrix
    static const TMatrix2<Real> Identity; // Identity matrix

	// Constructors
	TMatrix2(); // uninitialized
	TMatrix2( Real a00, Real a01,
			  Real a10, Real a11 );
	TMatrix2(const Real v0[2], const Real v1[2], Bool bRows = true);
	TMatrix2(const Real arrMat[4], Bool bRows = true);
	TMatrix2(const TVector2<Real> & v0, const TVector2<Real> & v1, Bool bRows = true);
	TMatrix2(const TVector2<Real> vMat[2], Bool bRows = true);
	TMatrix2(const TMatrix2<Real> & rhs);
	TMatrix2(const TMatrix3<Real> & rhs);
	TMatrix2(const TMatrix4<Real> & rhs);
	~TMatrix2();

	// Assignment operator
	inline TMatrix2<Real> & operator=(const TMatrix2<Real> & rhs);

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
	inline TMatrix2<Real> operator+() const;
	inline TMatrix2<Real> operator-() const;

	// Boolean operations
	Bool operator==(const TMatrix2<Real> & rhs) const;
	Bool operator!=(const TMatrix2<Real> & rhs) const;

    // Real operations
	TMatrix2<Real> operator*(Real rhs) const;
	TMatrix2<Real> operator/(Real rhs) const;

    TMatrix2<Real> & operator*=(Real rhs);
	TMatrix2<Real> & operator/=(Real rhs);

	// Vertex operations
	TVertex2<Real> operator*(const TVertex2<Real> & rhs) const;

	// Vector operations
	TVector2<Real> operator*(const TVector2<Real> & rhs) const;

	// Matrix operations
	TMatrix2<Real> operator+(const TMatrix2<Real> & rhs) const;
	TMatrix2<Real> operator-(const TMatrix2<Real> & rhs) const;
	TMatrix2<Real> operator*(const TMatrix2<Real> & rhs) const;

	TMatrix2<Real> & operator+=(const TMatrix2<Real> & rhs);
	TMatrix2<Real> & operator-=(const TMatrix2<Real> & rhs);
	TMatrix2<Real> & operator*=(const TMatrix2<Real> & rhs);

	// Methods : Row & Column access
	inline Void GetRow(TVector2<Real> & outRow, UInt iRow) const;
	inline Void SetRow(UInt iRow, Real fRow0, Real fRow1);
	inline Void SetRow(UInt iRow, const Real vRow[2]);
	inline Void SetRow(UInt iRow, const TVector2<Real> & vRow);

    inline Void GetColumn(TVector2<Real> & outColumn, UInt iColumn) const;
	inline Void SetColumn(UInt iColumn, Real fCol0, Real fCol1);
	inline Void SetColumn(UInt iColumn, const Real vCol[2]);
	inline Void SetColumn(UInt iColumn, const TVector2<Real> & vCol);

    // Methods : Makers
	inline Void MakeNull();
	inline Void MakeIdentity();
	inline Void MakeDiagonal(const TVector2<Real> & vDiagonal);
	inline Void MakeScale(const TVector2<Real> & vScale);
    inline Void MakeBasis(const TVector2<Real> & vI, const TVector2<Real> & vJ);
	Void MakeRotate(Real rotAngle);
    Void MakeTensorProduct(const TVector2<Real> & vU, const TVector2<Real> & vV);

    // Methods : Linear algebra stuff
    inline Void GetAngle(Real & outAngle) const;

    inline Void Transpose(TMatrix2<Real> & outTransposedMatrix) const;
    //inline TMatrix2<Real> TransposeMul(const TMatrix2<Real> & rhs) const; // Transpose(M) * rhs
    //inline TMatrix2<Real> MulTranspose(const TMatrix2<Real> & rhs) const; // M * Transpose(rhs)
    //inline TMatrix2<Real> TransposeMulTranspose(const TMatrix2<Real> & rhs) const; // Transpose(M) * Transpose(rhs)
    //inline TMatrix2<Real> DiagonalMul(const TVector2<Real> & rhs) const; // Diag(rhs) * M
    //inline TMatrix2<Real> MulDiagonal(const TVector2<Real> & rhs) const; // M * Diag(rhs)

    inline Void Minor(Real & outMinor, UInt iRow, UInt iColumn) const;
	inline Real Determinant() const;
    Void Adjoint(TMatrix2<Real> & outAdjointMatrix) const; // Transposed Co-Matrix
	Bool Invert(TMatrix2<Real> & outInvMatrix, Real fZeroTolerance = (Real)SCALAR_ERROR) const;

    Void OrthoNormalize(); // Gram-Schmidt

    inline Real QuadraticForm(const TVector2<Real> & v0, const TVector2<Real> & v1) const; // Transpose(v0) * M * v1

	// Data
	Real m00, m01; // Row 0
	Real m10, m11; // Row 1

private:
    static const Real ZERO;
    static const Real ONE;
    static const Real TWO;
};

// Explicit instanciation
typedef TMatrix2<Float> Matrix2f;
typedef TMatrix2<Double> Matrix2d;
typedef TMatrix2<Scalar> Matrix2;

// Specializations
template<>
TMatrix2<Float> TMatrix2<Float>::operator*(Float rhs) const;
template<>
TMatrix2<Double> TMatrix2<Double>::operator*(Double rhs) const;
template<>
TMatrix2<Float> TMatrix2<Float>::operator/(Float rhs) const;
template<>
TMatrix2<Double> TMatrix2<Double>::operator/(Double rhs) const;

template<>
TMatrix2<Float> & TMatrix2<Float>::operator*=(Float rhs);
template<>
TMatrix2<Double> & TMatrix2<Double>::operator*=(Double rhs);
template<>
TMatrix2<Float> & TMatrix2<Float>::operator/=(Float rhs);
template<>
TMatrix2<Double> & TMatrix2<Double>::operator/=(Double rhs);

template<>
TVertex2<Float> TMatrix2<Float>::operator*(const TVertex2<Float> & rhs) const;
template<>
TVertex2<Double> TMatrix2<Double>::operator*(const TVertex2<Double> & rhs) const;

template<>
TVector2<Float> TMatrix2<Float>::operator*(const TVector2<Float> & rhs) const;
template<>
TVector2<Double> TMatrix2<Double>::operator*(const TVector2<Double> & rhs) const;

template<>
TMatrix2<Float> TMatrix2<Float>::operator+(const TMatrix2<Float> & rhs) const;
template<>
TMatrix2<Double> TMatrix2<Double>::operator+(const TMatrix2<Double> & rhs) const;
template<>
TMatrix2<Float> TMatrix2<Float>::operator-(const TMatrix2<Float> & rhs) const;
template<>
TMatrix2<Double> TMatrix2<Double>::operator-(const TMatrix2<Double> & rhs) const;
template<>
TMatrix2<Float> TMatrix2<Float>::operator*(const TMatrix2<Float> & rhs) const;
template<>
TMatrix2<Double> TMatrix2<Double>::operator*(const TMatrix2<Double> & rhs) const;

template<>
TMatrix2<Float> & TMatrix2<Float>::operator+=(const TMatrix2<Float> & rhs);
template<>
TMatrix2<Double> & TMatrix2<Double>::operator+=(const TMatrix2<Double> & rhs);
template<>
TMatrix2<Float> & TMatrix2<Float>::operator-=(const TMatrix2<Float> & rhs);
template<>
TMatrix2<Double> & TMatrix2<Double>::operator-=(const TMatrix2<Double> & rhs);
template<>
TMatrix2<Float> & TMatrix2<Float>::operator*=(const TMatrix2<Float> & rhs);
template<>
TMatrix2<Double> & TMatrix2<Double>::operator*=(const TMatrix2<Double> & rhs);

template<>
Bool TMatrix2<Float>::Invert(TMatrix2<Float> & outInvertMatrix, Float fZeroTolerance) const;
template<>
Bool TMatrix2<Double>::Invert(TMatrix2<Double> & outInvertMatrix, Double fZeroTolerance) const;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Matrix2.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_TYPES_MATRIX_MATRIX2_H

