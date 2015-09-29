/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Curves/2D/CurveChain2.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Curves 2D : Chained patches definition
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_CURVES_2D_CURVECHAIN2_H
#define SCARAB_LIB_MATH_GEOMETRY_CURVES_2D_CURVECHAIN2_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../Memory/MemoryManager.h"

#include "Curve2.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TCurveChain2 class
template<typename Real>
class TCurveChain2 : public TCurve2<Real>
{
public:
    // arrParameters contains a list of (N+1) Ti such that
    // Patch(i) has minT = arrParameters[i], maxT = arrParameters[i+1]
    TCurveChain2( const Real * arrParameters, UInt iPatchCount );
    virtual ~TCurveChain2();

    // Length access
    virtual Real Length( Real fT0, Real fT1 ) const;
    virtual Real Parameter( Real fLength, UInt iMaxIterations = 32, Real fTolerance = (Real)SCALAR_ERROR ) const;

    // Patches access
    inline UInt GetPatchCount() const;
    inline Void GetPatch( UInt iPatch, Real & outMinT, Real & outMaxT ) const;
    Void SetPatch( UInt iPatch, Real fMinT, Real fMaxT );

protected:
    // Patches management
    typedef struct _patch_userdata {
        const TCurveChain2<Real> * pThis;
        UInt iPatch;
    } _PatchUserData;

    inline static Real _SpeedFunction( Real fT, Void * pUserData );

    virtual Real _GetPatchSpeed( UInt iPatch, Real fT ) const = 0;
    virtual Real _GetPatchLength( UInt iPatch, Real fT0, Real fT1 ) const = 0;

    Void _ComputePatchLengths() const;
    Void _GetPatchInfo( Real fT, UInt & outPatch, Real & outDT ) const;

    UInt m_iPatchCount;
    Real * m_arrParameters;
    Real * m_arrLengths;
    Real * m_arrAccumLengths;
};

// Explicit instanciation
typedef TCurveChain2<Float> CurveChain2f;
typedef TCurveChain2<Double> CurveChain2d;
typedef TCurveChain2<Scalar> CurveChain2;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "CurveChain2.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_CURVES_2D_CURVECHAIN2_H

