/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Intersection/Intersector3.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Base class for intersection of primitives in 3D
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_LIB_MATH_GEOMETRY_INTERSECTION_INTERSECTOR3_H
#define SCARAB_LIB_MATH_GEOMETRY_INTERSECTION_INTERSECTOR3_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Error/ErrorManager.h"

#include "../../Types/Vector/Vector3.h"
#include "../../Types/Vertex/Vertex3.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
enum Intersect3Type
{
    INTERSECT3D_EMPTY = 0,

    INTERSECT3D_POINT,
    INTERSECT3D_LINE,
    INTERSECT3D_RAY,
    INTERSECT3D_SEGMENT,
    INTERSECT3D_POLYGON,

    INTERSECT3D_PLANE,
    INTERSECT3D_POLYHEDRON,

    INTERSECT3D_OTHER
};

typedef DWord Intersect3Specifier;
#define INTERSECT3D_SPECIFIER_NONE 0

/////////////////////////////////////////////////////////////////////////////////
// The TIntersector3 interface
template<typename Real>
class TIntersector3
{
public:
    TIntersector3( Real fZeroTolerance = (Real)SCALAR_ERROR );
    virtual ~TIntersector3();

    // Precision management
    Real ZeroTolerance;

    // Computed results access
    inline Intersect3Type GetType() const;
    inline Intersect3Specifier GetSpecifier() const;

    inline Real GetTOI() const;

    // Static intersection
    virtual Bool Test();
    virtual Bool Compute();

    // Dynamic intersection
    virtual Bool Test( const TVector3<Real> & vSpeedA, const TVector3<Real> & vSpeedB, Real fMaxTime );
    virtual Bool Compute( const TVector3<Real> & vSpeedA, const TVector3<Real> & vSpeedB, Real fMaxTime );

protected:
    Intersect3Type m_iType;
    Intersect3Specifier m_iSpecifier;

    Real m_fTimeOfImpact;
};

// Explicit instanciation
typedef TIntersector3<Float> Intersector3f;
typedef TIntersector3<Double> Intersector3d;
typedef TIntersector3<Scalar> Intersector3;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Intersector3.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_INTERSECTION_INTERSECTOR3_H

//Scalar Triangle3::Distance( const Vertex3 & vPoint ) const
//{
//    Vector3 W = ( A - vPoint );
//    Vector3 vEdge0 = ( B - A );
//    Vector3 vEdge1 = ( C - A );
//    Scalar fA00 = vEdge0.NormSqr();
//    Scalar fA11 = vEdge1.NormSqr();
//    Scalar fA01 = ( vEdge0 * vEdge1 );
//    Scalar fB0 = ( W * vEdge0 );
//    Scalar fB1 = ( W * vEdge1 );
//    Scalar fC = W.NormSqr();
//    Scalar fDet = MathFn->Abs( fA00 * fA11 - fA01 * fA01 );
//    Scalar fS = fA01 * fB1 - fA11 * fB0;
//    Scalar fT = fA01 * fB0 - fA00 * fB1;
//    Scalar fDistanceSqr;
//
//    if ( fS + fT <= fDet ) {
//        if ( fS < 0.0f ) {
//            if ( fT < 0.0f ) { // Region 4
//                if ( fB0 < 0.0f ) {
//                    if ( (-fB0) >= fA00 )
//                        fDistanceSqr = ( fA00 + (2.0f * fB0) + fC );
//                    else
//                        fDistanceSqr = ( fC - (fB0 * fB0 / fA00) );
//                } else {
//                    if ( fB1 >= 0.0f )
//                        fDistanceSqr = fC;
//                    else if ( (-fB1) >= fA11 )
//                        fDistanceSqr = ( fA11 + (2.0f * fB1) + fC );
//                    else
//                        fDistanceSqr = ( fC - (fB1 * fB1 / fA11) );
//                }
//            } else { // Region 3
//                if ( fB1 >= 0.0f )
//                    fDistanceSqr = fC;
//                else if ( (-fB1) >= fA11 )
//                    fDistanceSqr = ( fA11 + (2.0f * fB1) + fC );
//                else
//                    fDistanceSqr = ( fC - (fB1 * fB1 / fA11) );
//            }
//        } else if ( fT < 0.0f ) { // Region 5
//            if ( fB0 >= 0.0f )
//                fDistanceSqr = fC;
//            else if ( (-fB0) >= fA00 )
//                fDistanceSqr = ( fA00 + (2.0f * fB0) + fC );
//            else
//                fDistanceSqr = ( (fB0 * fS) + fC - (fB0 * fB0 / fA00) );
//        } else { // Region 0 (interior)
//            Scalar fInvDet = MathFn->Invert(fDet);
//            fS *= fInvDet; fT *= fInvDet;
//            fDistanceSqr = ( fS * ( (fA00 * fS) + (fA01 * fT) + (2.0f * fB0) ) +
//                             fT * ( (fA01 * fS) + (fA11 * fT) + (2.0f * fB1) ) + fC );
//        }
//    } else {
//        Scalar fNum, fDenom, fTmp0, fTmp1;
//        if ( fS < 0.0f ) { // Region 2
//            fTmp0 = fA01 + fB0; fTmp1 = fA11 + fB1;
//            if ( fTmp1 > fTmp0 ) {
//                fNum = ( fTmp1 - fTmp0 );
//                fDenom = ( fA00 - (2.0f * fA01) + fA11 );
//                if ( fNum >= fDenom )
//                    fDistanceSqr = ( fA00 + (2.0f * fB0) + fC );
//                else {
//                    fS = fNum / fDenom; fT = (1.0f - fS);
//                    fDistanceSqr = ( fS * ( (fA00 * fS) + (fA01 * fT) + (2.0f * fB0) ) +
//                                     fT * ( (fA01 * fS) + (fA11 * fT) + (2.0f * fB1) ) + fC );
//                }
//            } else {
//                if ( fTmp1 <= 0.0f )
//                    fDistanceSqr = ( fA11 + (2.0f * fB1) + fC );
//                else if ( fB1 >= 0.0f )
//                    fDistanceSqr = fC;
//                else
//                    fDistanceSqr = ( fC - (fB1 * fB1 / fA11) );
//            }
//        } else if ( fT < 0.0f ) { // Region 6
//            fTmp0 = fA01 + fB1; fTmp1 = fA00 + fB0;
//            if ( fTmp1 > fTmp0 ) {
//                fNum = ( fTmp1 - fTmp0 );
//                fDenom = ( fA00 - (2.0f * fA01) + fA11 );
//                if ( fNum >= fDenom ) {
//                    fS = 0.0f; fT = 1.0f;
//                    fDistanceSqr = ( fA11 + (2.0f * fB1) + fC );
//                } else {
//                    fT = fNum / fDenom; fS = (1.0f - fT);
//                    fDistanceSqr = ( fS * ( (fA00 * fS) + (fA01 * fT) + (2.0f * fB0) ) +
//                                     fT * ( (fA01 * fS) + (fA11 * fT) + (2.0f * fB1) ) + fC );
//                }
//            } else {
//                if ( fTmp1 <= 0.0f )
//                    fDistanceSqr = ( fA00 + (2.0f * fB0) + fC );
//                else if ( fB0 >= 0.0f )
//                    fDistanceSqr = fC;
//                else
//                    fDistanceSqr = ( fC - (fB0 * fB0 / fA00) );
//            }
//        } else { // Region 1
//            fNum = ( fA11 - fA01 + fB1 - fB0 );
//            if ( fNum <= 0.0f )
//                fDistanceSqr = ( fA11 + (2.0f * fB1) + fC );
//            else {
//                fDenom = ( fA00 - (2.0f * fA01) + fA11 );
//                if ( fNum >= fDenom )
//                    fDistanceSqr = ( fA00 + (2.0f * fB0) + fC );
//                else {
//                    fS = fNum / fDenom; fT = (1.0f - fS);
//                    fDistanceSqr = ( fS * ( (fA00 * fS) + (fA01 * fT) + (2.0f * fB0) ) +
//                                     fT * ( (fA01 * fS) + (fA11 * fT) + (2.0f * fB1) ) + fC );
//                }
//            }
//        }
//    }
//    return MathFn->Sqrt( MathFn->Abs(fDistanceSqr) );
//}

