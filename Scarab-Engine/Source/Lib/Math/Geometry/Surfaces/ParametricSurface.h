/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Surfaces/ParametricSurface.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Surfaces : Base class for parametric surfaces
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_SURFACES_PARAMETRICSURFACE_H
#define SCARAB_LIB_MATH_GEOMETRY_SURFACES_PARAMETRICSURFACE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Error/ErrorManager.h"

#include "../../Types/Vector/Vector3.h"
#include "../../Types/Vertex/Vertex3.h"
#include "../../Types/Matrix/Matrix2.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TParametricSurface interface
template<typename Real>
class TParametricSurface
{
public:
    // Parametric domain can be triangular or rectangular :
    // Parameters are fU in [minU;maxU] and fV in [minV;maxV]
    // Triangular domain adds a constraint :
    //     (maxV-minV)*(fU-minU) + (maxU-minU)*(fV-maxV) <= 0
    TParametricSurface( Real fMinU, Real fMaxU, Real fMinV, Real fMaxV, Bool bTriangular = false );
    virtual ~TParametricSurface();

    // Getters
    inline Bool IsTriangular() const;

    // Definition domain
    inline Real GetMinU() const;
    inline Real GetMaxU() const;
    inline Real GetMinV() const;
    inline Real GetMaxV() const;
    inline Void SetIntervalU( Real fMinU, Real fMaxU );
    inline Void SetIntervalV( Real fMinV, Real fMaxV );

    // Position and derivatives
    virtual TVertex3<Real> P( Real fU, Real fV ) const = 0;
    virtual TVector3<Real> PdU( Real fU, Real fV ) const = 0;
    virtual TVector3<Real> PdV( Real fU, Real fV ) const = 0;
    virtual TVector3<Real> PdU2( Real fU, Real fV ) const = 0;
    virtual TVector3<Real> PdV2( Real fU, Real fV ) const = 0;
    virtual TVector3<Real> PdUdV( Real fU, Real fV ) const = 0;

    // Geometric quantities
    inline TVertex3<Real> Position( Real fU, Real fV ) const;

    inline TVector3<Real> TangentU( Real fU, Real fV ) const;
    inline TVector3<Real> TangentV( Real fU, Real fV ) const;
    TVector3<Real> Normal( Real fU, Real fV ) const;

    Void Frame( Real fU, Real fV, TVertex3<Real> & outPosition, TVector3<Real> & outNormal,
                                  TVector3<Real> & outTangentU, TVector3<Real> & outTangentV ) const;

    // Surface properties
    Void PrincipalCurvature( Real fU, Real fV, Real & outCurvature0, Real & outCurvature1,
                                               TVector3<Real> & outAxis0, TVector3<Real> & outAxis1 ) const;

    // Sub-Division
    Void SubDivideByParameter( Real * outParametersU, UInt iCountU, Real * outParametersV, UInt iCountV ) const;

protected:
    Bool m_bTriangular;

    Real m_fMinU, m_fMaxU;
    Real m_fMinV, m_fMaxV;
};

// Explicit instanciation
typedef TParametricSurface<Float> ParametricSurfacef;
typedef TParametricSurface<Double> ParametricSurfaced;
typedef TParametricSurface<Scalar> ParametricSurface;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "ParametricSurface.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_SURFACES_PARAMETRICSURFACE_H


