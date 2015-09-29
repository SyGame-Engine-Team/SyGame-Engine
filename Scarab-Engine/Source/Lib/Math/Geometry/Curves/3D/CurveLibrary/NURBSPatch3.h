/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Curves/3D/CurveLibrary/NURBSPatch3.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Curves 3D : (Non-)Uniform Rational BSpline curves
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_CURVES_3D_CURVELIBRARY_NURBSPATCH3_H
#define SCARAB_LIB_MATH_GEOMETRY_CURVES_3D_CURVELIBRARY_NURBSPATCH3_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../../Memory/MemoryManager.h"

#include "../../../../Analysis/FunctionBasis/BSplineBasis.h"

#include "../CurvePatch3.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TNURBSPatch3 class
template<typename Real>
class TNURBSPatch3 : public TCurvePatch3<Real>
{
public:
    // Uniform constructor, iControlPointCount = (n+1) must be >= 2
    //                      Weight values are 4D homogeneous coordinates for control points
    //                      iDegree = d satisfies 1 <= d <= n
    //                      bLoop indicates an open or closed curve, the curve is closed
    //                      by adding 1 control point such that P[n+1] = P[0] for open splines
    //                      or adding d control points, replicates of the d first points for
    //                      periodic splines
    //                      bPeriodic indicates an open or periodic spline
    // BEWARE ! Don't confuse open curve & open spline terms ...
    TNURBSPatch3( const TVertex3<Real> * arrControlPoints, const Real * arrWeights, UInt iControlPointCount,
                  UInt iDegree, Bool bLoop, Bool bPeriodic );

    // Non-Uniform constructor, Knot array contains (n-d) values, all in [0;1]
    //                          Knot values must be in a non-decreasing order
    TNURBSPatch3( const TVertex3<Real> * arrControlPoints, const Real * arrWeights, UInt iControlPointCount,
                  UInt iDegree, Bool bLoop, const Real * arrKnots );

    virtual ~TNURBSPatch3();

    // Getters
    inline UInt GetDegree() const;

    inline Bool IsUniform() const;
    inline Bool IsPeriodic() const; // Open/Periodic spline
    inline Bool IsLoop() const; // Open/Closed curve

    // Control points access
    inline UInt GetControlPointCount() const;
    inline const TVertex3<Real> & GetControlPoint( UInt iControlPoint ) const;
    inline Void SetControlPoint( UInt iControlPoint, const TVertex3<Real> & vPoint );

    inline Real GetWeight( UInt iControlPoint ) const;
    inline Void SetWeight( UInt iControlPoint, Real fWeightValue );

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
    Void _CreateControlPoints( const TVertex3<Real> * arrControlPoints, const Real * arrWeights );

    // Curve basis
    TBSplineBasis<Real> m_Basis;

    // Control points
    Bool m_bLoop;

    UInt m_iControlPointCount;
    UInt m_iReplicateCount;
    TVertex3<Real> * m_arrControlPoints;
    Real * m_arrWeights;
};

// Explicit instanciation
typedef TNURBSPatch3<Float> NURBSPatch3f;
typedef TNURBSPatch3<Double> NURBSPatch3d;
typedef TNURBSPatch3<Scalar> NURBSPatch3;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "NURBSPatch3.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_CURVES_3D_CURVELIBRARY_NURBSPATCH3_H

