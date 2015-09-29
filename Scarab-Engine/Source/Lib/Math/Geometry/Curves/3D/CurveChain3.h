/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Curves/3D/CurveChain3.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Curves 3D : Chained patches definition
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_CURVES_3D_CURVECHAIN3_H
#define SCARAB_LIB_MATH_GEOMETRY_CURVES_3D_CURVECHAIN3_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../Memory/MemoryManager.h"

#include "Curve3.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TCurveChain3 class
template<typename Real>
class TCurveChain3 : public TCurve3<Real>
{
public:
    // arrParameters contains a list of (N+1) Ti such that
    // Patch(i) has minT = arrParameters[i], maxT = arrParameters[i+1]
    TCurveChain3( const Real * arrParameters, UInt iPatchCount );
    virtual ~TCurveChain3();

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
        const TCurveChain3<Real> * pThis;
        UInt iPatch;
    } _PatchUserData;

    inline static Real _SpeedFunction( Real fT, Void * pUserData );

    virtual Real _GetPatchSpeed( UInt iPatch, Real fT ) const = 0;
    virtual Real _GetPatchLength( UInt iPatch, Real fT0, Real fT1 ) const = 0;

    Void _GetPatchInfo( Real fT, UInt & outPatch, Real & outDT ) const;

    UInt m_iPatchCount;
    Real * m_arrParameters;
    Real * m_arrLengths;
    Real * m_arrAccumLengths;
};

// Explicit instanciation
typedef TCurveChain3<Float> CurveChain3f;
typedef TCurveChain3<Double> CurveChain3d;
typedef TCurveChain3<Scalar> CurveChain3;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "CurveChain3.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_CURVES_3D_CURVECHAIN3_H

