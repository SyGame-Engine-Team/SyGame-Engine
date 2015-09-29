/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Matrix/MatrixN.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Generic NxM-dimensional matrix
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : Beware, this is dynamic allocation, you should use fixed-size
//              pools or stacks if you want more than a few ...
//              TODO : SSE loop-packed specializations
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_LIB_MATH_TYPES_MATRIX_MATRIXN_H
#define SCARAB_LIB_MATH_TYPES_MATRIX_MATRIXN_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Vector/VectorN.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TMatrixN class
template<typename Real>
class TMatrixN
{
public:
	// Constructors
	TMatrixN(); // unallocated
	TMatrixN( UInt iRows, UInt iColumns ); // uninitialized
    TMatrixN( UInt iRows, UInt iColumns, const Real * arrMat, Bool bRows = true );
	TMatrixN( UInt iRows, UInt iColumns, const Real ** arrMat, Bool bRows = true );
    TMatrixN( UInt iRows, UInt iColumns, const TVectorN<Real> * arrMat, Bool bRows = true );
	TMatrixN(const TMatrixN<Real> & rhs);
	~TMatrixN();

    // Deferred construction
    inline Void SetSize( UInt iRows, UInt iColumns );

	// Assignment operator
	inline TMatrixN<Real> & operator=(const TMatrixN<Real> & rhs);

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
	inline TMatrixN<Real> operator+() const;
	inline TMatrixN<Real> operator-() const;

	// Boolean operations
	Bool operator==(const TMatrixN<Real> & rhs) const;
	Bool operator!=(const TMatrixN<Real> & rhs) const;

    // Real operations
	inline TMatrixN<Real> operator*(Real rhs) const;
	inline TMatrixN<Real> operator/(Real rhs) const;

    inline TMatrixN<Real> & operator*=(Real rhs);
	inline TMatrixN<Real> & operator/=(Real rhs);

    // Vector operations
	TVectorN<Real> operator*(const TVectorN<Real> & rhs) const;

	// Matrix operations
	TMatrixN<Real> operator+(const TMatrixN<Real> & rhs) const;
	TMatrixN<Real> operator-(const TMatrixN<Real> & rhs) const;
	TMatrixN<Real> operator*(const TMatrixN<Real> & rhs) const;

	TMatrixN<Real> & operator+=(const TMatrixN<Real> & rhs);
	TMatrixN<Real> & operator-=(const TMatrixN<Real> & rhs);

	// Methods : Row & Column access
    inline UInt GetRowCount() const;
    inline UInt GetColumnCount() const;
    inline UInt GetSize() const;

	inline Void GetRow(TVectorN<Real> & outRow, UInt iRow) const;
	inline Void SetRow(UInt iRow, const Real * vRow);
	inline Void SetRow(UInt iRow, const TVectorN<Real> & vRow);

    inline Void GetColumn(TVectorN<Real> & outColumn, UInt iColumn) const;
	inline Void SetColumn(UInt iColumn, const Real * vCol);
	inline Void SetColumn(UInt iColumn, const TVectorN<Real> & vCol);

    inline Void SwapRows( UInt iRow0, UInt iRow1 );

    // Methods : Makers
	inline Void MakeNull();
	inline Void MakeIdentity();
	inline Void MakeMatrix( const Real * arrMat, Bool bRows = true );
	inline Void MakeMatrix( const Real ** arrMat, Bool bRows = true );
	inline Void MakeMatrix( const TVectorN<Real> * arrMat, Bool bRows = true );

    // Methods : Linear algebra stuff
    inline Void Transpose(TMatrixN<Real> & outTransposedMatrix) const;
    //inline TMatrixN<Real> TransposeMul(const TMatrixN<Real> & rhs) const; // Transpose(M) * rhs
    //inline TMatrixN<Real> MulTranspose(const TMatrixN<Real> & rhs) const; // M * Transpose(rhs)
    //inline TMatrixN<Real> TransposeMulTranspose(const TMatrixN<Real> & rhs) const; // Transpose(M) * Transpose(rhs)

    // WARNING : This routine needs temp memory allocation, beware with context !
	Bool Invert(TMatrixN<Real> & outInvMatrix, Real fZeroTolerance = (Real)SCALAR_ERROR) const;

    inline Real QuadraticForm(const TVectorN<Real> & v0, const TVectorN<Real> & v1) const; // Transpose(v0) * M * v1

private:
    // Helpers
    Void _AllocateMatrix( UInt iRows, UInt iColumns );
    Void _DestroyMatrix();

    Void _SetMatrix( const Real * arrMat, Bool bRows );
    Void _SetMatrix( const Real ** arrMat, Bool bRows );
    Void _SetMatrix( const TVectorN<Real> * arrMat, Bool bRows );

    static const Real ZERO;
    static const Real ONE;

    UInt m_iRows, m_iColumns, m_iSize;
	Real * m_arrMatrix; // Row-major storage
    Real ** m_arrRows; // Row pointers
};

// Explicit instanciation
typedef TMatrixN<Float> MatrixNf;
typedef TMatrixN<Double> MatrixNd;
typedef TMatrixN<Scalar> MatrixN;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "MatrixN.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_TYPES_MATRIX_MATRIXN_H

