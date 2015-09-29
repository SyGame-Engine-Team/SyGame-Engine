/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/CollisionDetection/NarrowPhase/VCLip/VoronoiClip.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Narrow-Phase : Convex, V-Clip
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : none
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_NARROWPHASE_VCLIP_VORONOICLIP_H
#define SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_NARROWPHASE_VCLIP_VORONOICLIP_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../../Lib/Math/Geometry/Intersection/2D/IntersectConvexPolygon2.h"
#include "../../../../../Lib/Math/Geometry/Intersection/3D/Linear/IntersectSegment3.h"
#include "../../../../../Lib/Math/Geometry/Intersection/3D/Planar/IntersectPlaneLine3.h"

#include "../../../Shape/TransformedShape.h"

#include "../NarrowPhase.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define VCLIP_MAX_ITERATIONS 20

enum VClipConvexID
{
    VCLIP_CONVEX_A = 0,
    VCLIP_CONVEX_B = 1
};

enum VClipState
{
    VCLIP_STATE_SEPARATED = 0,
    VCLIP_STATE_PENETRATION,

    VCLIP_STATE_VV,
    VCLIP_STATE_VE,
    VCLIP_STATE_VF,
    VCLIP_STATE_EE,
    VCLIP_STATE_EF
};

enum VClipResponse
{
    VCLIP_CLIPPED = 0,
    VCLIP_INCLUDED,
    VCLIP_EXCLUDED_SIMPLE,
    VCLIP_EXCLUDED_COMPOUND
};

/////////////////////////////////////////////////////////////////////////////////
// The VoronoiClip class
class VoronoiClip : public NarrowPhaseConvex
{
public:
    VoronoiClip( TransformedShapeConvex * pConvexA, TransformedShapeConvex * pConvexB );
    virtual ~VoronoiClip();

    // Narrow Collision algorithm
    virtual Void Initialize();
    virtual Bool DetectCollision( Bool bGenerateContactPoints );

private:
    // Short names
    typedef ShapeConvex::Vertex   Vertex;
    typedef ShapeConvex::Edge     Edge;
    typedef ShapeConvex::Triangle Triangle;
    typedef ShapeConvex::Quad     Quad;
    typedef ShapeConvex::Polygon  Polygon;

    // Internal routines
        // Derivatives evaluation
    static const Int NullDerivationFlag;
    static const MeshFeature * RaisedPenetration;
    Int _EvaluateDerivativeSign( const Vertex3 & vEdgeA, const Vertex3 & vEdgeB, Scalar fT, const Vertex3 & vVertexPosition ) const;
    Int _EvaluateDerivativeSign( const Vertex3 & vEdgeA, const Vertex3 & vEdgeB, Scalar fT, const Plane & vFacetPlane ) const;

        // Clipping operations
    const MeshFeature * _ClipVertex_V( VClipConvexID idConvex, const Vertex * pVertex,   const Vertex3 & vVertexV ) const;
    const MeshFeature * _ClipVertex_E( VClipConvexID idConvex, const Edge * pEdge,       const Vertex3 & vVertexV ) const;
    const MeshFeature * _ClipVertex_F( VClipConvexID idConvex, const MeshFacet * pFacet, const Vertex3 & vVertexV ) const;

    const MeshFeature * _ClipEdge_V( VClipConvexID idConvex, const Vertex * pVertex,   const Vertex3 & vEdgeA, const Vertex3 & vEdgeB ) const;
    const MeshFeature * _ClipEdge_E( VClipConvexID idConvex, const Edge * pEdge,       const Vertex3 & vEdgeA, const Vertex3 & vEdgeB ) const;
    VClipResponse       _ClipEdge_F( VClipConvexID idConvex, const MeshFacet * pFacet, const Vertex3 & vEdgeA, const Vertex3 & vEdgeB,
                                     UInt * outEnterPlane, Scalar * outEnter, UInt * outExitPlane, Scalar * outExit ) const;
    VClipResponse       _ClipEdge_Generic( const Plane * arrVoronoiPlanes, UInt iPlaneCount, const Vertex3 & vEdgeA, const Vertex3 & vEdgeB,
                                           UInt * outEnterPlane, Scalar * outEnter, UInt * outExitPlane, Scalar * outExit ) const;

        // Closest feature operations
    const MeshFeature * _ClosestEdgeTowardFacet( VClipConvexID idConvex, const Vertex * pVertex, const Vertex3 & vPosition,
                                                 Scalar fDistanceV, const Plane & facetPlane ) const;
    const MeshFeature * _ClosestFeatureTowardEdge( VClipConvexID idConvex, const MeshFacet * pFacet, UInt iStartEdge,
                                                   const Vertex3 & vEdgeA, const Vertex3 & vEdgeB ) const;

        // Local Minimum handler
    VClipState _EscapeLocalMinimum( VClipConvexID idConvex, const MeshFacet * pFacet, const Vertex3 & vPosition );

    // State routines
    VClipState _ProcessState_VV( VClipConvexID idConvex0, const Vertex * pVertex0,
                                 VClipConvexID idConvex1, const Vertex * pVertex1 );

    VClipState _ProcessState_VE( VClipConvexID idConvex0, const Vertex * pVertex0,
                                 VClipConvexID idConvex1, const Edge * pEdge1 );

    VClipState _ProcessState_VF( VClipConvexID idConvex0, const Vertex * pVertex0,
                                 VClipConvexID idConvex1, const MeshFacet * pFacet1 );

    VClipState _ProcessState_EE( VClipConvexID idConvex0, const Edge * pEdge0,
                                 VClipConvexID idConvex1, const Edge * pEdge1 );

    VClipState _ProcessState_EF( VClipConvexID idConvex0, const Edge * pEdge0,
                                 VClipConvexID idConvex1, const MeshFacet * pFacet1 );

// Debug
    Void _DebugPrint_Header() const;
    Void _DebugPrint_ClosestFeatures() const;
    Void _DebugPrint_ContactPoints() const;
// Debug

    // Collision data
    TransformedShapeConvex * m_pConvex[2];

    // State
    VClipState m_vcState;
    const MeshFeature * m_pClosestFeature[2];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "VoronoiClip.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_NARROWPHASE_VCLIP_VORONOICLIP_H

