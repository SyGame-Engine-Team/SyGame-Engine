/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Curves/3D/CurveLibrary/BezierPatch3.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Curves 3D : Bezier curves
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_CURVES_3D_CURVELIBRARY_BEZIERPATCH3_H
#define SCARAB_LIB_MATH_GEOMETRY_CURVES_3D_CURVELIBRARY_BEZIERPATCH3_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../../Memory/MemoryManager.h"

#include "../CurvePatch3.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TBezierPatch3 class
template<typename Real>
class TBezierPatch3 : public TCurvePatch3<Real>
{
public:
    TBezierPatch3( const TVertex3<Real> * arrControlPoints, UInt iCount );
    virtual ~TBezierPatch3();

    // Getters
    inline UInt GetDegree() const;

    // Control points access
    inline UInt GetControlPointCount() const;
    inline const TVertex3<Real> & GetControlPoint( UInt iControlPoint ) const;
    Void SetControlPoint( UInt iControlPoint, const TVertex3<Real> & vPoint );

    // Position and derivatives
    virtual TVertex3<Real> P( Real fT ) const;
    virtual TVector3<Real> PdT( Real fT ) const;
    virtual TVector3<Real> PdT2( Real fT ) const;
    virtual TVector3<Real> PdT3( Real fT ) const;

private:
    // Curve basis
    UInt m_iDegree;
    Real * m_arrBinomial;

    // Control points
    UInt m_iControlPointCount;
    TVertex3<Real> * m_arrControlPoints;

    // Derivatives
    TVector3<Real> * m_arrPdT;
    TVector3<Real> * m_arrPdT2;
    TVector3<Real> * m_arrPdT3;
};

// Explicit instanciation
typedef TBezierPatch3<Float> BezierPatch3f;
typedef TBezierPatch3<Double> BezierPatch3d;
typedef TBezierPatch3<Scalar> BezierPatch3;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "BezierPatch3.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_CURVES_3D_CURVELIBRARY_BEZIERPATCH3_H

