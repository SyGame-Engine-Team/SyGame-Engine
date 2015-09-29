/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Curves/3D/CurveLibrary/PolynomialPatch3.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Curves 3D : Polynomial curves
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_CURVES_3D_CURVELIBRARY_POLYNOMIALPATCH3_H
#define SCARAB_LIB_MATH_GEOMETRY_CURVES_3D_CURVELIBRARY_POLYNOMIALPATCH3_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../../Memory/MemoryManager.h"

#include "../../../../Analysis/Polynomial/Polynomial1.h"

#include "../CurvePatch3.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TPolynomialPatch3 class
template<typename Real>
class TPolynomialPatch3 : public TCurvePatch3<Real>
{
public:
    TPolynomialPatch3( const TPolynomial1<Real> & vPolyX,
                       const TPolynomial1<Real> & vPolyY,
                       const TPolynomial1<Real> & vPolyZ );
    virtual ~TPolynomialPatch3();

    // Getters
    inline UInt GetDegree() const;

    // Polynomials access
    inline const TPolynomial1<Real> & GetPolynomialX() const;
    inline const TPolynomial1<Real> & GetPolynomialY() const;
    inline const TPolynomial1<Real> & GetPolynomialZ() const;

    // Position and derivatives
    inline virtual TVertex3<Real> P( Real fT ) const;
    inline virtual TVector3<Real> PdT( Real fT ) const;
    inline virtual TVector3<Real> PdT2( Real fT ) const;
    inline virtual TVector3<Real> PdT3( Real fT ) const;

protected:
    TPolynomial1<Real> m_vPX,    m_vPY,    m_vPZ;
    TPolynomial1<Real> m_vPXdT,  m_vPYdT,  m_vPZdT;
    TPolynomial1<Real> m_vPXdT2, m_vPYdT2, m_vPZdT2;
    TPolynomial1<Real> m_vPXdT3, m_vPYdT3, m_vPZdT3;
};

// Explicit instanciation
typedef TPolynomialPatch3<Float> PolynomialPatch3f;
typedef TPolynomialPatch3<Double> PolynomialPatch3d;
typedef TPolynomialPatch3<Scalar> PolynomialPatch3;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "PolynomialPatch3.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_CURVES_3D_CURVELIBRARY_POLYNOMIALPATCH3_H


