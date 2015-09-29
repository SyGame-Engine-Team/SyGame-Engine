/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Transform/Transform3.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Generic transformations in 3D
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
#ifndef SCARAB_LIB_MATH_TYPES_TRANSFORM_TRANSFORM3_H
#define SCARAB_LIB_MATH_TYPES_TRANSFORM_TRANSFORM3_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Error/ErrorManager.h"

#include "../Matrix/Matrix3.h"
#include "../Matrix/Matrix4.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TTransform3 class
template<typename Real>
class TTransform3
{
public:
    // Constant values
    static const TTransform3<Real> Identity;

    // Constructors
    TTransform3();
    TTransform3( const TMatrix3<Real> & matRotate, const TVector3<Real> & vScale, const TVector3<Real> & vTranslate );
    TTransform3( const TMatrix3<Real> & matRotate, Real fUniformScale, const TVector3<Real> & vTranslate );
    TTransform3( const TMatrix3<Real> & matTransform, const TVector3<Real> & vTranslate );
    TTransform3( const TTransform3<Real> & rhs );
    ~TTransform3();

    // Assignment operator
    TTransform3<Real> & operator=( const TTransform3<Real> & rhs );

    // Vertex operations
	inline TVertex4<Real> operator*( const TVertex4<Real> & rhs ) const;
	inline TVertex3<Real> operator*( const TVertex3<Real> & rhs ) const;

	// Vector operations
	inline TVector4<Real> operator*( const TVector4<Real> & rhs ) const;
	inline TVector3<Real> operator*( const TVector3<Real> & rhs ) const;

    // Transform operations
	TTransform3<Real> operator*( const TTransform3<Real> & rhs ) const;

    // Construction
    Void SetTransform( const TMatrix3<Real> & matTransform );
    Void SetRotate( const TMatrix3<Real> & matRotate );
    Void SetScale( const TVector3<Real> & vScale );
    Void SetUniformScale( Real fScale );
    Void SetTranslate( const TVector3<Real> & vTranslate );

    // Getters
    inline Bool IsIdentity() const;
    inline Bool IsRotScale() const;
    inline Bool IsUniformScale() const;

    inline const TMatrix3<Real> & GetTransform() const;
    inline const TMatrix3<Real> & GetRotate() const;
    inline const TVector3<Real> & GetScale() const;
    inline Real GetUniformScale() const;
    inline const TVector3<Real> & GetTranslate() const;

    inline const TMatrix4<Real> & GetHMatrix() const;

    // Methods
    Void MakeIdentity();
    Void MakeUnitScale();

    Real GetMaxScale() const;

    Bool Invert( TTransform3<Real> & outInvTransform ) const;

private:
    Void _UpdateHMatrix();
    Bool _UpdateInverse() const;

    // Flags
    Bool m_bIsIdentity;
    Bool m_bIsRotScale;
    Bool m_bIsUniformScale;
    mutable Bool m_bUpdateInverse;

    // Components
    TMatrix3<Real> m_matTransform; // M (or R)
    TVector3<Real> m_vScale;       // S
    TVector3<Real> m_vTranslate;   // T

    // Homogeneous matrix H = ( M*S T )
    //                        (   0 1 )
    TMatrix4<Real> m_matHMatrix;

    // Invert support
    mutable TMatrix3<Real> m_matInvTransform; // InvM (or InvR)
    mutable TVector3<Real> m_vInvScale;       // InvS = 1 / S
    mutable TVector3<Real> m_vInvTranslate;   // InvT = -( (InvM * T) * InvS )
};

// Explicit instanciation
typedef TTransform3<Float> Transform3f;
typedef TTransform3<Double> Transform3d;
typedef TTransform3<Scalar> Transform3;

// Specializations
template<>
Void TTransform3<Float>::_UpdateHMatrix();
template<>
Void TTransform3<Double>::_UpdateHMatrix();

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Transform3.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_TYPES_TRANSFORM_TRANSFORM3_H


