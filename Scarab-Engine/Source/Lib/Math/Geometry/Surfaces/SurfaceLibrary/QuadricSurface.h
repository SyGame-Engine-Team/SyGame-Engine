/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Surfaces/SurfaceLibrary/QuadricSurface.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Implicit Surfaces : Quadrics
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_SURFACES_SURFACELIBRARY_QUADRICSURFACE_H
#define SCARAB_LIB_MATH_GEOMETRY_SURFACES_SURFACELIBRARY_QUADRICSURFACE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Formats/Rational.h"

#include "../ImplicitSurface.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
enum QuadricSurfaceType
{
    QUADRIC_UNDEFINED = 0,

    QUADRIC_POINT,
    QUADRIC_LINE,
    QUADRIC_PLANE,
    QUADRIC_TWO_PLANES,
    QUADRIC_CONE_ELLIPTIC,
    QUADRIC_CYLINDER_PARABOLIC,
    QUADRIC_CYLINDER_ELLIPTIC,
    QUADRIC_CYLINDER_HYPERBOLIC,
    QUADRIC_PARABOLOID_ELLIPTIC,
    QUADRIC_PARABOLOID_HYPERBOLIC,
    QUADRIC_HYPERBOLOID_ONE_SHEET,
    QUADRIC_HYPERBOLOID_TWO_SHEETS,
    QUADRIC_ELLIPSOID
};

/////////////////////////////////////////////////////////////////////////////////
// The TQuadricSurface class
template<typename Real>
class TQuadricSurface : public TImplicitSurface<Real>
{
public:
    // A Quadric surface is defined by an array of 10 coefficients :
    //
    //     ( c0 + c1*X + c2*Y + c3*Z + c4*XX + c5*XY + c6*XZ + c7*YY + c8*YZ + c9*ZZ ) = 0
    //
    // Let U = (X,Y,Z), Ut = Transpose(u),
    //     A = (   c4, c5/2, c6/2 ), Bt = Transpose(c1,c2,c3), C = c0,
    //         ( c5/2,   c7, c8/2 )
    //         ( c6/2, c8/2,   c9 ) // A is symmetric
    //
    // We now have :  Ut*A*U + Bt*U + C = 0
    TQuadricSurface();  // all coefficients zero
    TQuadricSurface( const Real arrCoeffs[10] );
    virtual ~TQuadricSurface();

    // Coefficients access
    inline Real GetCoefficient( UInt iCoeff ) const;
    inline Void SetCoefficient( UInt iCoeff, Real fValue );

    inline const TMatrix3<Real> & GetA() const;
    inline Void SetA( const TMatrix3<Real> & matValue );

    inline const TVector3<Real> & GetB() const;
    inline Void SetB( const TVector3<Real> & vValue );

    inline Real GetC() const;
    inline Void SetC( Real fValue );

    // Implicit function and derivatives
    virtual Real F( const TVertex3<Real> & vPosition ) const;
    virtual Real FdX( const TVertex3<Real> & vPosition ) const;
    virtual Real FdY( const TVertex3<Real> & vPosition ) const;
    virtual Real FdZ( const TVertex3<Real> & vPosition ) const;
    virtual Real FdX2( const TVertex3<Real> & vPosition ) const;
    virtual Real FdY2( const TVertex3<Real> & vPosition ) const;
    virtual Real FdZ2( const TVertex3<Real> & vPosition ) const;
    virtual Real FdXdY( const TVertex3<Real> & vPosition ) const;
    virtual Real FdXdZ( const TVertex3<Real> & vPosition ) const;
    virtual Real FdYdZ( const TVertex3<Real> & vPosition ) const;

    // Equation classification
    QuadricSurfaceType GetType() const;

protected:
    Real m_arrCoefficients[10];
    TMatrix3<Real> m_matA;
    TVector3<Real> m_vB;
    Real m_fC;

private:
    // Classification using exact rational arithmetic
    typedef Rational<16> QRational;
    typedef TVector3<QRational> QVector;

    class EqRootRepr
    {
    public:
        EqRootRepr( const Real arrCoeffs[10] );

        // A / B / C
        QRational A00, A01, A02, A11, A12, A22;
        QVector B;
        QRational C;

        // Determinants of minors of A (comatrix)
        QRational Det00, Det01, Det02, Det11, Det12, Det22;

        // Characteristic polynomial L^3 - P2*L^2 + P1*L - P0.
        QVector P;

        // Sturm sequence
        QVector PSturm;
    };

    // Use Sturm sequences to compute the signs of the roots
    static UInt _Compute_SignChanges( const QRational * arrValues, UInt iCount );
    static Void _Compute_RootSigns( EqRootRepr & rootRepr, UInt & outPositiveRoots, UInt & outNegativeRoots, UInt & outZeroRoots );

    // Classification
    static QuadricSurfaceType _Classify_Roots0( const EqRootRepr & rootRepr, UInt iPositiveRoots );

    static QuadricSurfaceType _Classify_Roots1( const EqRootRepr & rootRepr, UInt iPositiveRoots );
    static QuadricSurfaceType _Classify_Roots1( const EqRootRepr & rootRepr, UInt iPositiveRoots,
                                                const QVector & vI, const QVector & vJ, const QVector & vK );

    static QuadricSurfaceType _Classify_Roots2( const EqRootRepr & rootRepr, UInt iPositiveRoots );
    static QuadricSurfaceType _Classify_Roots2( const EqRootRepr & rootRepr, UInt iPositiveRoots,
                                                const QVector & vI, const QVector & vJ, const QVector & vK );

    static QuadricSurfaceType _Classify_Roots3( const EqRootRepr & rootRepr );
};

// Explicit instanciation
typedef TQuadricSurface<Float> QuadricSurfacef;
typedef TQuadricSurface<Double> QuadricSurfaced;
typedef TQuadricSurface<Scalar> QuadricSurface;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "QuadricSurface.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_SURFACES_SURFACELIBRARY_QUADRICSURFACE_H


