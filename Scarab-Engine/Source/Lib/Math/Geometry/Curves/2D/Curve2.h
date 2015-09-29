/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Curves/2D/Curve2.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Curves 2D : Base class
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_CURVES_2D_CURVE2_H
#define SCARAB_LIB_MATH_GEOMETRY_CURVES_2D_CURVE2_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../Error/ErrorManager.h"

#include "../../../Types/Vector/Vector2.h"
#include "../../../Types/Vertex/Vertex2.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TCurve2 class
template<typename Real>
class TCurve2
{
public:
    TCurve2( Real fMinT, Real fMaxT );
    virtual ~TCurve2();

    // Definition domain
    inline Real GetMinT() const;
    inline Real GetMaxT() const;
    inline Void SetIntervalT( Real fMinT, Real fMaxT );

    // Position and derivatives
    virtual TVertex2<Real> P( Real fT ) const = 0;
    virtual TVector2<Real> PdT( Real fT ) const = 0;
    virtual TVector2<Real> PdT2( Real fT ) const = 0;
    virtual TVector2<Real> PdT3( Real fT ) const = 0;

    // Length access
    inline Real Length() const;
    virtual Real Length( Real fT0, Real fT1 ) const = 0;
    virtual Real Parameter( Real fLength, UInt iMaxIterations = 32, Real fTolerance = (Real)SCALAR_ERROR ) const = 0;

    // Geometric quantities
    inline TVertex2<Real> Position( Real fT ) const;
    inline TVector2<Real> Velocity( Real fT ) const;
    inline Real Speed( Real fT ) const;

    inline TVector2<Real> Tangent( Real fT ) const;
    inline TVector2<Real> Normal( Real fT ) const;

    Void Frame( Real fT, TVertex2<Real> & outPosition, TVector2<Real> & outTangent,
                         TVector2<Real> & outNormal ) const;

    // Curve properties
    Real Curvature( Real fT ) const;

    // Sub-Division
    Void SubDivideByParameter( Real * outParameters, UInt iCount ) const;
    Void SubDivideByLength( Real * outParameters, UInt iCount ) const;

protected:
    Real m_fMinT;
    Real m_fMaxT;
};

// Explicit instanciation
typedef TCurve2<Float> Curve2f;
typedef TCurve2<Double> Curve2d;
typedef TCurve2<Scalar> Curve2;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Curve2.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_CURVES_2D_CURVE2_H


