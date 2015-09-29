/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Modeling/Curves/RevolutionSurfaceMesh.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Curved Surface Mesh, Revolution surfaces
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
#ifndef SCARAB_ENGINE_MODELING_CURVES_REVOLUTIONSURFACEMESH_H
#define SCARAB_ENGINE_MODELING_CURVES_REVOLUTIONSURFACEMESH_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Math/Geometry/Curves/2D/Curve2.h"

#include "../../Rendering/Resources/Mesh.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
//enum RevolutionSurfaceTopology
//{
//    REVOLUTION_DISK = 0,
//    REVOLUTION_CYLINDER,
//    REVOLUTION_SPHERE,
//    REVOLUTION_TORUS,
//
//    REVOLUTION_COUNT
//};

/////////////////////////////////////////////////////////////////////////////////
// The RevolutionSurfaceMesh class
//class RevolutionSurfaceMesh : public WorldTriangleMesh
//{
//public:
//    // Revolution of a 2D curve in (X,Z) plane around the Z-axis. Curve is
//    // (x(t), z(t)) with t in [minT;maxT], x(t) >= 0. The curve is assumed
//    // to be non self-intersecting.
//    // If the curve is open, 3 cases :
//    //  -  x(minT) = 0 and x(maxT) > 0
//    //  or x(minT) > 0 and x(maxT) = 0  =>  topology of a disk
//    //  -  x(minT) > 0 and x(maxT) > 0  =>  topology of a cylinder
//    //  -  x(minT) = 0 and x(maxT) = 0  =>  topology of a sphere
//    // If the curve is closed, (x(minT),z(minT)) == (x(maxT),z(maxT)) and the
//    // mesh has the topology of a torus.
//    // fCenterX is an offset of the real rotation axis along X in the
//    // revolution plane (X,Z).
//    // BEWARE : dynamic updating MUST preserve mesh topology.
//    RevolutionSurfaceMesh( RevolutionSurfaceTopology iTopology, Curve2 * pCurve,
//                           UInt iRadialSamples, UInt iCurveSamples, Scalar fCenterX,
//                           Bool bUseArcLengthSampling, Bool bInterior,
//                           const VertexFormat * pVF, Bool bDynamic = false );
//    virtual ~RevolutionSurfaceMesh();
//
//    // Getters
//    inline RevolutionSurfaceTopology GetTopology() const;
//    inline const Curve2 * GetCurve() const;
//    inline UInt GetRadialSamples() const;
//    inline UInt GetCurveSamples() const;
//    inline Bool UseArcLengthSampling() const;
//
//    // Curve access
//    inline Void SetCurve( Curve2 * pCurve );
//    inline Void SetArcLengthSampling( Bool bUseArcLengthSampling );
//
//    // Mesh methods
//    Void Update();
//
//private:
//    // Dynamic update
//    Void _Update_Disk();
//    Void _Update_Cylinder();
//    Void _Update_Sphere();
//    Void _Update_Torus();
//
//    Bool m_bDynamic;
//
//    RevolutionSurfaceTopology m_iTopology;
//    Curve2 * m_pCurve;
//    UInt m_iRadialSamples;
//    UInt m_iCurveSamples;
//    Scalar m_fCenterX;
//    Bool m_bArcLengthSampling;
//
//    // Dynamic update
//    Scalar * m_arrSin;
//    Scalar * m_arrCos;
//    Vertex3 * m_arrSamples;
//};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "RevolutionSurfaceMesh.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_MODELING_CURVES_REVOLUTIONSURFACEMESH_H

