/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Curves/3D/Curve3.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Curves 3D : Base class
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_CURVES_3D_CURVE3_H
#define SCARAB_LIB_MATH_GEOMETRY_CURVES_3D_CURVE3_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../Error/ErrorManager.h"

#include "../../../Types/Vector/Vector3.h"
#include "../../../Types/Vertex/Vertex3.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TCurve3 class
template<typename Real>
class TCurve3
{
public:
    TCurve3( Real fMinT, Real fMaxT );
    virtual ~TCurve3();

    // Definition domain
    inline Real GetMinT() const;
    inline Real GetMaxT() const;
    inline Void SetIntervalT( Real fMinT, Real fMaxT );

    // Position and derivatives
    virtual TVertex3<Real> P( Real fT ) const = 0;
    virtual TVector3<Real> PdT( Real fT ) const = 0;
    virtual TVector3<Real> PdT2( Real fT ) const = 0;
    virtual TVector3<Real> PdT3( Real fT ) const = 0;

    // Length access
    inline Real Length() const;
    virtual Real Length( Real fT0, Real fT1 ) const = 0;
    virtual Real Parameter( Real fLength, UInt iMaxIterations = 32, Real fTolerance = (Real)SCALAR_ERROR ) const = 0;

    // Geometric quantities
    inline TVertex3<Real> Position( Real fT ) const;
    inline TVector3<Real> Velocity( Real fT ) const;
    inline Real Speed( Real fT ) const;

    inline TVector3<Real> Tangent( Real fT ) const;
    TVector3<Real> Normal( Real fT ) const;
    TVector3<Real> BiNormal( Real fT ) const;

    Void Frame( Real fT, TVertex3<Real> & outPosition, TVector3<Real> & outTangent,
                         TVector3<Real> & outNormal, TVector3<Real> & outBiNormal ) const;

    // Curve properties
    Real Curvature( Real fT ) const;
    Real Torsion( Real fT ) const;

    // Sub-Division
    Void SubDivideByParameter( Real * outParameters, UInt iCount ) const;
    Void SubDivideByLength( Real * outParameters, UInt iCount ) const;

protected:
    Real m_fMinT;
    Real m_fMaxT;
};

// Explicit instanciation
typedef TCurve3<Float> Curve3f;
typedef TCurve3<Double> Curve3d;
typedef TCurve3<Scalar> Curve3;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Curve3.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_CURVES_3D_CURVE3_H



