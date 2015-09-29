/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/WorldDebugDraw.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Graph : Debug drawing operations
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : TODO = Add joint axes / parameters ...
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_WORLD_WORLDDEBUGDRAW_H
#define SCARAB_ENGINE_WORLD_WORLDDEBUGDRAW_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Rendering/RenderingManager.h"

#include "Nodes/WorldNode.h"
#include "Nodes/WorldLeaf.h"

#include "Container/WorldBSPNode.h"
#include "Container/WorldSwitchNode.h"
#include "Container/WorldLight.h"

#include "Outdoor/WorldOutdoorArea.h"
#include "Indoor/WorldIndoorArea.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

    // Debug drawing flags
#define WORLD_DEBUGDRAW_NONE            0x00000000

#define WORLD_DEBUGDRAW_GIZMOS          0x00000001
#define WORLD_DEBUGDRAW_WORLDBVS        0x00000002

#define WORLD_DEBUGDRAW_BSPPLANES       0x00000004
#define WORLD_DEBUGDRAW_PORTALS         0x00000008
#define WORLD_DEBUGDRAW_PATHLINKS       0x00000010

#define WORLD_DEBUGDRAW_NORMALS         0x00000020
#define WORLD_DEBUGDRAW_TANGENTS        0x00000040
#define WORLD_DEBUGDRAW_BINORMALS       0x00000080
#define WORLD_DEBUGDRAW_FACETNORMALS    0x00000100

#define WORLD_DEBUGDRAW_PROXIES         0x00000200
#define WORLD_DEBUGDRAW_CONTACTS        0x00000400
#define WORLD_DEBUGDRAW_JOINTS          0x00000800
#define WORLD_DEBUGDRAW_CONTACTPOINTS   0x00001000

#define WORLD_DEBUGDRAW_LINEARVELOCITY  0x00002000
#define WORLD_DEBUGDRAW_ANGULARVELOCITY 0x00004000

#define WORLD_DEBUGDRAW_FRUSTRUM        0x00008000

/////////////////////////////////////////////////////////////////////////////////
// The WorldDebugDraw class
class WorldDebugDraw
{
public:
    WorldDebugDraw();
    ~WorldDebugDraw();

    // Methods
    Void RenderStart() const;
    Void RenderEnd() const;

    inline Void RenderFrustrum( const WorldCuller * pCuller ) const;

    Void RenderNode( WorldBaseNode * pNode ) const;

    Void RenderBSPTree( WorldBSPNode * pNode ) const;

private:
    // Helpers
    Void _RenderAAB( const Vertex3 & vCenter, const Vector3 & vExtents, const Color4 & vColor ) const;

    // Node attributes rendering
        // Node attributes
    Void _RenderNode_Gizmo( const WorldBaseNode * pNode ) const;           // Red(X) / Green(Y) / Blue(Z)
    Void _RenderNode_WorldBV( const WorldBaseNode * pNode ) const;         // Magenta

    Void _RenderNode_BSPPlane( const WorldBSPNode * pBSPNode ) const;      // Cyan
    Void _RenderNode_Portals( const WorldConvexRoom * pConvexRoom ) const; // Red = closed / Green = open
    Void _RenderNode_PathLinks( WorldOutdoorArea * pArea ) const; // Blue = Micro / Yellow = Triangle

        // Mesh attributes
    Void _RenderMesh_Normals( const TriangleMesh * pMesh, const Transform3 & trWorld ) const;      // Blue
    Void _RenderMesh_Tangents( const TriangleMesh * pMesh, const Transform3 & trWorld ) const;     // Red
    Void _RenderMesh_Binormals( const TriangleMesh * pMesh, const Transform3 & trWorld ) const;    // Green
    Void _RenderMesh_FacetNormals( const TriangleMesh * pMesh, const Transform3 & trWorld ) const; // Yellow

        // Body attributes
    Void _RenderBody_Proxy( const BodyNode * pBody ) const;                // Yellow / Green when sleeping
    Void _RenderBody_Contacts( const BodyNode * pBody ) const;             // Blue
    Void _RenderBody_Joints( const BodyNode * pBody ) const;               // Cyan / Yellow for joint axes
    Void _RenderBody_ContactPoints( const BodyNode * pBody ) const;        // Green / Red when closest in manifold

    Void _RenderBody_LinearVelocity( const BodyNode * pBody ) const;       // Red
    Void _RenderBody_AngularVelocity( const BodyNode * pBody ) const;      // Magenta

    // Global attributes rendering
    Void _RenderGlobal_Frustrum( const WorldCuller * pCuller ) const;      // White

    // Debug Rendering
    EffectColor m_hDebugDrawEffect;
    EffectColorInstance * m_pDebugDrawEffectInstance;
    RenderingBatchID m_idDebugDrawBatch;

    static const Color4 sm_arrIslandPalette[27];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "WorldDebugDraw.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_WORLD_WORLDDEBUGDRAW_H

