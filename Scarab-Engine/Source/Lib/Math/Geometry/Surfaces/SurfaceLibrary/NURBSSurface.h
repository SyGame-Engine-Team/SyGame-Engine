/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Surfaces/SurfaceLibrary/NURBSSurface.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Parametric Surfaces : NURBS surfaces
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_SURFACES_SURFACELIBRARY_NURBSSURFACE_H
#define SCARAB_LIB_MATH_GEOMETRY_SURFACES_SURFACELIBRARY_NURBSSURFACE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../Memory/MemoryManager.h"

#include "../../../Analysis/FunctionBasis/BSplineBasis.h"

#include "../ParametricSurface.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TNURBSSurface class
template<typename Real>
class TNURBSSurface : public TParametricSurface<Real>
{
public:
    TNURBSSurface( const TVertex3<Real> * arrControlPoints, const Real * arrWeights,
                   UInt iControlPointCountU, UInt iControlPointCountV,
                   UInt iDegreeU, UInt iDegreeV, Bool bLoopU, Bool bLoopV,
                   Bool bPeriodicU, Bool bPeriodicV );
    TNURBSSurface( const TVertex3<Real> * arrControlPoints, const Real * arrWeights,
                   UInt iControlPointCountU, UInt iControlPointCountV,
                   UInt iDegreeU, UInt iDegreeV, Bool bLoopU, Bool bLoopV,
                   Bool bPeriodicU, const Real * arrKnotsV );
    TNURBSSurface( const TVertex3<Real> * arrControlPoints, const Real * arrWeights,
                   UInt iControlPointCountU, UInt iControlPointCountV,
                   UInt iDegreeU, UInt iDegreeV, Bool bLoopU, Bool bLoopV,
                   const Real * arrKnotsU, Bool bPeriodicV );
    TNURBSSurface( const TVertex3<Real> * arrControlPoints, const Real * arrWeights,
                   UInt iControlPointCountU, UInt iControlPointCountV,
                   UInt iDegreeU, UInt iDegreeV, Bool bLoopU, Bool bLoopV,
                   const Real * arrKnotsU, const Real * arrKnotsV );
    virtual ~TNURBSSurface();

    // Getters
    inline UInt GetDegreeU() const;
    inline UInt GetDegreeV() const;

    inline Bool IsUniformU() const;
    inline Bool IsUniformV() const;
    inline Bool IsPeriodicU() const;
    inline Bool IsPeriodicV() const;
    inline Bool IsLoopU() const;
    inline Bool IsLoopV() const;

    // Control points access
    inline UInt GetControlPointCountU() const;
    inline UInt GetControlPointCountV() const;
    inline const TVertex3<Real> & GetControlPoint( UInt iU, UInt iV ) const;
    Void SetControlPoint( UInt iU, UInt iV, const TVertex3<Real> & vPoint );

    inline Real GetWeight( UInt iU, UInt iV ) const;
    Void SetWeight( UInt iU, UInt iV, Real fWeightValue );

    // Knot vector access
    inline UInt GetKnotCountU() const;
    inline UInt GetKnotCountV() const;
    inline Real GetKnotU( UInt iKnot ) const;
    inline Real GetKnotV( UInt iKnot ) const;
    inline Void SetKnotU( UInt iKnot, Real fKnotValue );
    inline Void SetKnotV( UInt iKnot, Real fKnotValue );

    // Position and derivatives
    virtual TVertex3<Real> P( Real fU, Real fV ) const;
    virtual TVector3<Real> PdU( Real fU, Real fV ) const;
    virtual TVector3<Real> PdV( Real fU, Real fV ) const;
    virtual TVector3<Real> PdU2( Real fU, Real fV ) const;
    virtual TVector3<Real> PdV2( Real fU, Real fV ) const;
    virtual TVector3<Real> PdUdV( Real fU, Real fV ) const;

    Void Evaluate( Real fU, Real fV, TVertex3<Real> * outP, TVector3<Real> * outPdU = NULL, TVector3<Real> * outPdV = NULL,
                                     TVector3<Real> * outPdU2 = NULL, TVector3<Real> * outPdV2 = NULL, TVector3<Real> * outPdUdV = NULL ) const;

protected:
    // Helpers
    Void _CreateControlPoints( const TVertex3<Real> * arrControlPoints, const Real * arrWeights );

    // Surface Basis
    TBSplineBasis<Real> m_BasisU;
    TBSplineBasis<Real> m_BasisV;

    // Control points
    Bool m_bLoopU, m_bLoopV;

    UInt m_iControlPointCountU;
    UInt m_iControlPointCountV;
    UInt m_iReplicateCountU;
    UInt m_iReplicateCountV;
    UInt m_iControlPointsPitch;
    TVertex3<Real> * m_arrControlPoints;
    Real * m_arrWeights;
};

// Explicit instanciation
typedef TNURBSSurface<Float> NURBSSurfacef;
typedef TNURBSSurface<Double> NURBSSurfaced;
typedef TNURBSSurface<Scalar> NURBSSurface;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "NURBSSurface.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_SURFACES_SURFACELIBRARY_NURBSSURFACE_H


