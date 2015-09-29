/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Curves/3D/CurveLibrary/CubicPolynomialPatch3.h
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_CURVES_3D_CURVELIBRARY_CUBICPOLYNOMIALPATCH3_H
#define SCARAB_LIB_MATH_GEOMETRY_CURVES_3D_CURVELIBRARY_CUBICPOLYNOMIALPATCH3_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "PolynomialPatch3.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TCubicPolynomialPatch3 class
template<typename Real>
class TCubicPolynomialPatch3 : public TPolynomialPatch3<Real>
{
public:
    TCubicPolynomialPatch3( const TPolynomial1<Real> & vPolyX,
                            const TPolynomial1<Real> & vPolyY,
                            const TPolynomial1<Real> & vPolyZ );
    virtual ~TCubicPolynomialPatch3();

    // Tesselation support
    Void Tesselate( UInt iLevel );

    inline UInt GetVertexCount() const;
    inline const TVertex3<Real> * GetVertices() const;
    inline const TVertex3<Real> & GetVertex( UInt iVertex ) const;

private:
    // Subdivision
    typedef struct _interval_parameters
    {
        UInt iBound0, iBound1;
        TVector3<Real> arrPdT2[2];
    } _IntervalParameters;

    Void _RecursiveSubdivision( _IntervalParameters & intervalParams, UInt iLevel, Real fSqrD );

    // Tessellation vertices
    UInt m_iVertexCount;
    TVertex3<Real> * m_arrVertices;
};

// Explicit instanciation
typedef TCubicPolynomialPatch3<Float> CubicPolynomialPatch3f;
typedef TCubicPolynomialPatch3<Double> CubicPolynomialPatch3d;
typedef TCubicPolynomialPatch3<Scalar> CubicPolynomialPatch3;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "CubicPolynomialPatch3.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_CURVES_3D_CURVELIBRARY_CUBICPOLYNOMIALPATCH3_H


