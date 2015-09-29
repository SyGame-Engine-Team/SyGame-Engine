/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/CollisionDetection/NarrowPhase/VCLip/SingleSideVClip.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Narrow-Phase : Convex, V-Clip, Single-sided
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
#ifndef SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_NARROWPHASE_VCLIP_SINGLESIDEVCLIP_H
#define SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_NARROWPHASE_VCLIP_SINGLESIDEVCLIP_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../../Lib/Math/Geometry/Primitives/3D/Linear/Segment3.h"
#include "../../../../../Lib/Math/Geometry/Primitives/3D/Planar/Plane.h"

#include "../../../Shape/Primitive/ShapeSphere.h"
#include "../../../Shape/TransformedShape.h"

#include "../NarrowPhase.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define SSVCLIP_MAX_ITERATIONS 20

enum SingleSideVClipState
{
    SSVCLIP_STATE_SEPARATED = 0,
    SSVCLIP_STATE_PENETRATION,

    SSVCLIP_STATE_V,
    SSVCLIP_STATE_E,
    SSVCLIP_STATE_F
};

/////////////////////////////////////////////////////////////////////////////////
// The SingleSideVClip class
class SingleSideVClip : public NarrowPhaseConvex
{
public:
    SingleSideVClip( TransformedShapeConvex * pConvexA, TransformedShapeConvex * pConvexB );
    virtual ~SingleSideVClip();

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
        // Clipping operations
    const MeshFeature * _ClipVertex_V( const Vertex * pVertex,   const Vertex3 & vVertexV ) const;
    const MeshFeature * _ClipVertex_E( const Edge * pEdge,       const Vertex3 & vVertexV ) const;
    const MeshFeature * _ClipVertex_F( const MeshFacet * pFacet, const Vertex3 & vVertexV ) const;

        // Local Minimum handler
    SingleSideVClipState _EscapeLocalMinimum( const MeshFacet * pFacet, const Vertex3 & vPosition );

    // State routines
    SingleSideVClipState _ProcessState_V( const Vertex * pVertex, const Vertex3 & vSpherePosition );
    SingleSideVClipState _ProcessState_E( const Edge * pEdge, const Vertex3 & vSpherePosition );
    SingleSideVClipState _ProcessState_F( const MeshFacet * pFacet, const Vertex3 & vSpherePosition );

    // Contact points generation
    Bool _GenerateContactPoints( const Vertex3 & vSpherePosition, Scalar fSphereRadius, Bool bGenerateContactPoints );

    // Collision data
    TransformedShapeConvex * m_pConvex;
    TransformedShapeConvex * m_pSphere;

    Bool m_bReverseNormal;

    // State
    SingleSideVClipState m_vcState;
    const MeshFeature * m_pClosestFeature;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "SingleSideVClip.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_NARROWPHASE_VCLIP_SINGLESIDEVCLIP_H

