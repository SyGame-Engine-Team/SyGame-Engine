/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Shape/Convex/ShapeConvexMesh.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Shapes : Convex, Mesh
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : I should try Hill-Climbing + Tunneling ... Not really sure if
//              its better than BSP approach, might look at an heuristic to
//              switch from polyhedron complexity, size or something ...
//              If not too much of an overhead, this could be decisive ...
//              Could allow to use an easy to update space partition ...
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_PHYSICS_SHAPE_CONVEX_SHAPECONVEXMESH_H
#define SCARAB_ENGINE_PHYSICS_SHAPE_CONVEX_SHAPECONVEXMESH_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../Lib/Math/Geometry/SpacePartitions/GaussTree.h"

#include "../Shape.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The ShapeConvexMesh class
class ShapeConvexMesh : public ShapeConvex
{
public:
    ShapeConvexMesh( MeshVoronoiGraph * pGraph, TriangleMesh * pMesh = NULL );
    ShapeConvexMesh( TriangleMesh * pMesh );
    virtual ~ShapeConvexMesh();

    // Shape type
    inline virtual ShapeType GetType() const;

    // Mass properties
    virtual Void ComputeMassProperties( Scalar fDensity, Scalar * outMass, Vertex3 * outCenterOfMass, Matrix3 * outInertiaTensor ) const;

    // V-Clip support
    inline virtual const MeshVoronoiGraph * GetGraph() const;

    // GJK/MPR support
    virtual Void QueryExtremalVertices( const Vector3 & vDirection, UInt * outMax = NULL, UInt * outMin = NULL ) const;
    virtual Void QueryExtremalVertices( const Vector3 & vDirection, Vertex3 * outMax = NULL, Vertex3 * outMin = NULL ) const;

private:
    // Helpers
    static Void _UpdateMassIntegral( Scalar arrIntegrals[10], const Vertex3 & vA, const Vertex3 & vB, const Vertex3 & vC );

    // Graph data
    Bool m_bOwningGraph;
    MeshVoronoiGraph * m_pGraph;

    // GaussTree for extremal queries
    class ExtremalQuery : public GaussTree::HQuery { public:
        ExtremalQuery();
        virtual ~ExtremalQuery();

        inline virtual Void Result( UInt iVertex );

        UInt * pOutResult;
    };

    GaussTree m_hGaussTree;
    mutable ExtremalQuery m_hExtremalQuery;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "ShapeConvexMesh.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_SHAPE_CONVEX_SHAPECONVEXMESH_H

