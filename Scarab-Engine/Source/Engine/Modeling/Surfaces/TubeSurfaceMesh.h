/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Modeling/Curves/TubeSurfaceMesh.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Curved Surface Mesh, Tube surfaces
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
#ifndef SCARAB_ENGINE_MODELING_CURVES_TUBESURFACEMESH_H
#define SCARAB_ENGINE_MODELING_CURVES_TUBESURFACEMESH_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Math/Geometry/Curves/3D/Curve3.h"

#include "../../Rendering/Resources/Mesh.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TubeSurfaceMesh class
//class TubeSurfaceMesh : public WorldTriangleMesh
//{
//public:
//    typedef Scalar (*RadiusFunction)( Scalar );
//
//    // Extrusion of a circle with variable radius along a 3D curve.
//    // Passing a Null UpVect defaults to Frenet frame instead of tangent-based.
//    // Otherwise, it must be normalized.
//    TubeSurfaceMesh( Curve3 * pCurve, RadiusFunction pfRadial, const Vector3 & vUpVect,
//                     UInt iCurveSamples, UInt iRadialSamples,
//                     Bool bUseArcLengthSampling, Bool bClosed, Bool bInterior,
//                     const VertexFormat * pVF, Bool bDynamic = false,
//                     const TextureCoord2 * pTexCoordMin = NULL, const TextureCoord2 * pTexCoordMax = NULL );
//    virtual ~TubeSurfaceMesh();
//
//    // Getters
//    inline const Curve3 * GetCurve() const;
//    inline RadiusFunction GetRadial() const;
//    inline const Vector3 & GetUpVect() const;
//    inline UInt GetCurveSamples() const;
//    inline UInt GetRadialSamples() const;
//    inline Bool UseArcLengthSampling() const;
//
//    // Curve access
//    inline Void SetCurve( Curve3 * pCurve );
//    inline Void SetArcLengthSampling( Bool bUseArcLengthSampling );
//
//    // Radial access
//    inline Void SetRadial( RadiusFunction pfRadial );
//    inline Void SetUpVect( const Vector3 & vUpVect );
//
//    // Mesh methods
//    Void Update();
//
//    // Tube ends access, (iRadialSamples + 1) vertices.
//    Void GetStartSlice( Vertex3 * outSlice );
//    Void GetEndSlice( Vertex3 * outSlice );
//
//private:
//    inline UInt _GetIndex( UInt iCurve, UInt iRay ) const;
//
//    // Dynamic update
//    Void _Update_Vertices();
//    Void _Update_Normals();
//    Void _Build_TexCoords( UInt iUnit, const TextureCoord2 & tcMin, const TextureCoord2 & tcMax );
//    Void _Build_Indices( Bool bInterior );
//
//    Bool m_bDynamic;
//
//    Curve3 * m_pCurve;
//    RadiusFunction m_pfRadial;
//    Vector3 m_vUpVect;
//    UInt m_iCurveSamples;
//    UInt m_iRadialSamples;
//    Bool m_bArcLengthSampling, m_bClosed;
//
//    // Dynamic update
//    Scalar * m_arrSin;
//    Scalar * m_arrCos;
//};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "TubeSurfaceMesh.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_MODELING_CURVES_TUBESURFACEMESH_H

