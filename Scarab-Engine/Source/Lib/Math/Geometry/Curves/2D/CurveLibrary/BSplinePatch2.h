/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Curves/2D/CurveLibrary/BSplinePatch2.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Curves 2D : BSpline curves
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_CURVES_2D_CURVELIBRARY_BSPLINEPATCH2_H
#define SCARAB_LIB_MATH_GEOMETRY_CURVES_2D_CURVELIBRARY_BSPLINEPATCH2_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../../Memory/MemoryManager.h"

#include "../../../../Analysis/FunctionBasis/BSplineBasis.h"

#include "../CurvePatch2.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TBSplinePatch2 class
template<typename Real>
class TBSplinePatch2 : public TCurvePatch2<Real>
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
    TBSplinePatch2( const TVertex2<Real> * arrControlPoints, UInt iControlPointCount,
                    UInt iDegree, Bool bLoop, Bool bPeriodic );

    // Non-Uniform constructor, Knot array contains (n-d) values, all in [0;1]
    //                          Knot values must be in a non-decreasing order
    TBSplinePatch2( const TVertex2<Real> * arrControlPoints, UInt iControlPointCount,
                    UInt iDegree, Bool bLoop, const Real * arrKnots );

    virtual ~TBSplinePatch2();

    // Getters
    inline UInt GetDegree() const;

    inline Bool IsUniform() const;
    inline Bool IsPeriodic() const; // Open/Periodic spline
    inline Bool IsLoop() const; // Open/Closed curve

    // Control points access
    inline UInt GetControlPointCount() const;
    inline const TVertex2<Real> & GetControlPoint( UInt iControlPoint ) const;
    inline Void SetControlPoint( UInt iControlPoint, const TVertex2<Real> & vPoint );

    // Knot vector access
    inline UInt GetKnotCount() const;
    inline Real GetKnot( UInt iKnot ) const;
    inline Void SetKnot( UInt iKnot, Real fKnotValue );

    // Position and derivatives
    virtual TVertex2<Real> P( Real fT ) const;
    virtual TVector2<Real> PdT( Real fT ) const;
    virtual TVector2<Real> PdT2( Real fT ) const;
    virtual TVector2<Real> PdT3( Real fT ) const;

    Void Evaluate( Real fT, TVertex2<Real> * outP, TVector2<Real> * outPdT = NULL,
                            TVector2<Real> * outPdT2 = NULL, TVector2<Real> * outPdT3 = NULL ) const;

private:
    // Helpers
    Void _CreateControlPoints( const TVertex2<Real> * arrControlPoints );

    // Curve basis
    TBSplineBasis<Real> m_Basis;

    // Control points
    Bool m_bLoop;

    UInt m_iControlPointCount;
    UInt m_iReplicateCount;
    TVertex2<Real> * m_arrControlPoints;
};

// Explicit instanciation
typedef TBSplinePatch2<Float> BSplinePatch2f;
typedef TBSplinePatch2<Double> BSplinePatch2d;
typedef TBSplinePatch2<Scalar> BSplinePatch2;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "BSplinePatch2.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_CURVES_2D_CURVELIBRARY_BSPLINEPATCH2_H

