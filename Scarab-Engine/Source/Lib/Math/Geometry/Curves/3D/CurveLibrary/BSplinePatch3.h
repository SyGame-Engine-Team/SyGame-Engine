/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Curves/3D/CurveLibrary/BSplinePatch3.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Curves 3D : BSpline curves
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_CURVES_3D_CURVELIBRARY_BSPLINEPATCH3_H
#define SCARAB_LIB_MATH_GEOMETRY_CURVES_3D_CURVELIBRARY_BSPLINEPATCH3_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../../Memory/MemoryManager.h"

#include "../../../../Analysis/FunctionBasis/BSplineBasis.h"

#include "../CurvePatch3.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TBSplinePatch3 class
template<typename Real>
class TBSplinePatch3 : public TCurvePatch3<Real>
{
public:
    // Uniform constructor, iControlPointCount = (n+1) must be >= 2
    //                      iDegree = d satisfies 1 <= d <= n
    //                      bLoop indicates an open or closed curve, the curve is closed
    //                      by adding 1 control point such that P[n+1] = P[0] for open splines
    //                      or adding d control points, replicates of the d first points for
    //                      periodic splines
    //                      bPeriodic indicates an open or periodic spline
    // BEWARE ! Don't confuse open curve & open spline terms ...
    TBSplinePatch3( const TVertex3<Real> * arrControlPoints, UInt iControlPointCount,
                    UInt iDegree, Bool bLoop, Bool bPeriodic );

    // Non-Uniform constructor, Knot array contains (n-d) values, all in [0;1]
    //                          Knot values must be in a non-decreasing order
    TBSplinePatch3( const TVertex3<Real> * arrControlPoints, UInt iControlPointCount,
                    UInt iDegree, Bool bLoop, const Real * arrKnots );

    virtual ~TBSplinePatch3();

    // Getters
    inline UInt GetDegree() const;

    inline Bool IsUniform() const;
    inline Bool IsPeriodic() const; // Open/Periodic spline
    inline Bool IsLoop() const; // Open/Closed curve

    // Control points access
    inline UInt GetControlPointCount() const;
    inline const TVertex3<Real> & GetControlPoint( UInt iControlPoint ) const;
    inline Void SetControlPoint( UInt iControlPoint, const TVertex3<Real> & vPoint );

    // Knot vector access
    inline UInt GetKnotCount() const;
    inline Real GetKnot( UInt iKnot ) const;
    inline Void SetKnot( UInt iKnot, Real fKnotValue );

    // Position and derivatives
    virtual TVertex3<Real> P( Real fT ) const;
    virtual TVector3<Real> PdT( Real fT ) const;
    virtual TVector3<Real> PdT2( Real fT ) const;
    virtual TVector3<Real> PdT3( Real fT ) const;

    Void Evaluate( Real fT, TVertex3<Real> * outP, TVector3<Real> * outPdT = NULL,
                            TVector3<Real> * outPdT2 = NULL, TVector3<Real> * outPdT3 = NULL ) const;

private:
    // Helpers
    Void _CreateControlPoints( const TVertex3<Real> * arrControlPoints );

    // Curve basis
    TBSplineBasis<Real> m_Basis;

    // Control points
    Bool m_bLoop;

    UInt m_iControlPointCount;
    UInt m_iReplicateCount;
    TVertex3<Real> * m_arrControlPoints;
};

// Explicit instanciation
typedef TBSplinePatch3<Float> BSplinePatch3f;
typedef TBSplinePatch3<Double> BSplinePatch3d;
typedef TBSplinePatch3<Scalar> BSplinePatch3;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "BSplinePatch3.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_CURVES_3D_CURVELIBRARY_BSPLINEPATCH3_H

