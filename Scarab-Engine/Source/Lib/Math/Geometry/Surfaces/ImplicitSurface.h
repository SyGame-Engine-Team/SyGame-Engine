/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Surfaces/ImplicitSurface.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Surfaces : Base class for surfaces defined by a function
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_SURFACES_IMPLICITSURFACE_H
#define SCARAB_LIB_MATH_GEOMETRY_SURFACES_IMPLICITSURFACE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Error/ErrorManager.h"

#include "../../Types/Matrix/Matrix3.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TImplicitSurface interface
template<typename Real>
class TImplicitSurface
{
public:
    TImplicitSurface();
    virtual ~TImplicitSurface();

    // Implicit function and derivatives
    virtual Real F( const TVertex3<Real> & vPosition ) const = 0;
    virtual Real FdX( const TVertex3<Real> & vPosition ) const = 0;
    virtual Real FdY( const TVertex3<Real> & vPosition ) const = 0;
    virtual Real FdZ( const TVertex3<Real> & vPosition ) const = 0;
    virtual Real FdX2( const TVertex3<Real> & vPosition ) const = 0;
    virtual Real FdY2( const TVertex3<Real> & vPosition ) const = 0;
    virtual Real FdZ2( const TVertex3<Real> & vPosition ) const = 0;
    virtual Real FdXdY( const TVertex3<Real> & vPosition ) const = 0;
    virtual Real FdXdZ( const TVertex3<Real> & vPosition ) const = 0;
    virtual Real FdYdZ( const TVertex3<Real> & vPosition ) const = 0;

    // Check is a point is below(-1), above(+1) or on(0) the surface ...
    inline Int ClassifyPoint( const TVertex3<Real> & vPosition, Real fTolerance = (Real)SCALAR_ERROR ) const;

    // Geometric quantities
    inline TVector3<Real> GetGradient( const TVertex3<Real> & vPosition ) const;
    TMatrix3<Real> GetHessian( const TVertex3<Real> & vPosition ) const;

    Void Frame( const TVertex3<Real> & vPosition, TVector3<Real> & outNormal,
                TVector3<Real> & outTangentU, TVector3<Real> & outTangentV ) const;

    // Surface properties
    Bool PrincipalCurvature( const TVertex3<Real> & vPosition, Real & outCurvature0, Real & outCurvature1,
                                                               TVector3<Real> & outAxis0, TVector3<Real> & outAxis1 ) const;
};

// Explicit instanciation
typedef TImplicitSurface<Float> ImplicitSurfacef;
typedef TImplicitSurface<Double> ImplicitSurfaced;
typedef TImplicitSurface<Scalar> ImplicitSurface;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "ImplicitSurface.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_SURFACES_IMPLICITSURFACE_H

