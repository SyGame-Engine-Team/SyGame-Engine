/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Transform/Transform2.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Generic transformations in 2D
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_LIB_MATH_TYPES_TRANSFORM_TRANSFORM2_H
#define SCARAB_LIB_MATH_TYPES_TRANSFORM_TRANSFORM2_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Error/ErrorManager.h"

#include "../Matrix/Matrix2.h"
#include "../Matrix/Matrix3.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TTransform2 class
template<typename Real>
class TTransform2
{
public:
    // Constant values
    static const TTransform2<Real> Identity;

    // Constructors
    TTransform2();
    TTransform2( const TMatrix2<Real> & matRotate, const TVector2<Real> & vScale, const TVector2<Real> & vTranslate );
    TTransform2( const TMatrix2<Real> & matRotate, Real fUniformScale, const TVector2<Real> & vTranslate );
    TTransform2( const TMatrix2<Real> & matTransform, const TVector2<Real> & vTranslate );
    TTransform2( const TTransform2<Real> & rhs );
    ~TTransform2();

    // Assignment operator
    TTransform2<Real> & operator=( const TTransform2<Real> & rhs );

    // Vertex operations
	inline TVertex3<Real> operator*( const TVertex3<Real> & rhs ) const;
	inline TVertex2<Real> operator*( const TVertex2<Real> & rhs ) const;

	// Vector operations
	inline TVector3<Real> operator*( const TVector3<Real> & rhs ) const;
	inline TVector2<Real> operator*( const TVector2<Real> & rhs ) const;

    // Transform operations
	TTransform2<Real> operator*( const TTransform2<Real> & rhs ) const;

    // Construction
    Void SetTransform( const TMatrix2<Real> & matTransform );
    Void SetRotate( const TMatrix2<Real> & matRotate );
    Void SetScale( const TVector2<Real> & vScale );
    Void SetUniformScale( Real fScale );
    Void SetTranslate( const TVector2<Real> & vTranslate );

    // Getters
    inline Bool IsIdentity() const;
    inline Bool IsRotScale() const;
    inline Bool IsUniformScale() const;

    inline const TMatrix2<Real> & GetTransform() const;
    inline const TMatrix2<Real> & GetRotate() const;
    inline const TVector2<Real> & GetScale() const;
    inline Real GetUniformScale() const;
    inline const TVector2<Real> & GetTranslate() const;

    inline const TMatrix3<Real> & GetHMatrix() const;

    // Methods
    Void MakeIdentity();
    Void MakeUnitScale();

    Real GetMaxScale() const;

    Bool Invert( TTransform2<Real> & outInvTransform ) const;

private:
    Void _UpdateHMatrix();
    Bool _UpdateInverse() const;

    // Flags
    Bool m_bIsIdentity;
    Bool m_bIsRotScale;
    Bool m_bIsUniformScale;
    mutable Bool m_bUpdateInverse;

    // Components
    TMatrix2<Real> m_matTransform; // M (or R)
    TVector2<Real> m_vScale;       // S
    TVector2<Real> m_vTranslate;   // T

    // Homogeneous matrix H = ( M*S T )
    //                        (   0 1 )
    TMatrix3<Real> m_matHMatrix;
    
    // Invert support
    mutable TMatrix2<Real> m_matInvTransform; // InvM (or InvR)
    mutable TVector2<Real> m_vInvScale;       // InvS = 1 / S
    mutable TVector2<Real> m_vInvTranslate;   // InvT = -( (InvM * T) * InvS )
};

// Explicit instanciation
typedef TTransform2<Float> Transform2f;
typedef TTransform2<Double> Transform2d;
typedef TTransform2<Scalar> Transform2;

// Specializations
template<>
Void TTransform2<Float>::_UpdateHMatrix();
template<>
Void TTransform2<Double>::_UpdateHMatrix();

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Transform2.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_TYPES_TRANSFORM_TRANSFORM2_H


