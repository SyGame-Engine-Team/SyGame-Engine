/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Shape/Shape.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Collision shape abstraction
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
#ifndef SCARAB_ENGINE_PHYSICS_SHAPE_SHAPE_H
#define SCARAB_ENGINE_PHYSICS_SHAPE_SHAPE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Math/Types/Transform/Transform3.h"
#include "../../../Lib/Math/Types/Quaternion/Quaternion.h"

#include "../../../Lib/Math/Geometry/BoundingShapes/3D/BVAABox.h"
#include "../../../Lib/Math/Geometry/Primitives/3D/Sphere.h"
#include "../../../Lib/Math/Geometry/Primitives/3D/Planar/Triangle3.h"

#include "../../../Lib/Math/Geometry/MeshGraphs/Graphs/MeshVoronoiGraph.h"
#include "../../../Lib/Math/Geometry/MeshGraphs/Graphs/MeshDeformableGraph.h"
#include "../../../Lib/Math/Geometry/SpacePartitions/AABoxTree.h"

#include "../../Rendering/Resources/Mesh.h"

#include "../CollisionDetection/NarrowPhase/ContactManifold.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
    // Shape types
enum ShapeType
{
    SHAPE_PRIMITIVE_TRIANGLE = 0,
    SHAPE_PRIMITIVE_SPHERE,
    SHAPE_PRIMITIVE_BOX,

    SHAPE_CONVEX_MESH,

    SHAPE_CONCAVE_MESH,
    SHAPE_CONCAVE_HEIGHTFIELD,
    SHAPE_CONCAVE_DEFORMABLE,

    SHAPE_COUNT
};

    // Shape margin
#define SHAPE_MARGIN 0.1f

    // ShapeDeformable
#define SHAPECONCAVE_NODE_RADIUS     0.01f
#define SHAPECONCAVE_FACET_THICKNESS 0.01f

    // Prototypes
class TransformedShape;

/*
/////////////////////////////////////////////////////////////////////////////////
// The ShapePose class
class ShapePose
{
public:
    ShapePose( Shape * pShape );
    virtual ~ShapePose();

    // Getters
    inline Shape * GetShape() const;

protected:
    Shape * m_pShape;

    // Mass properties
    Vertex3 m_vCenterOfMass;
    Scalar m_fMass, m_fInvMass;
    Matrix3 m_matInertiaTensor;    // in COM
    Matrix3 m_matInvInertiaTensor; // space

    // Vertices
    Array<Vector3> m_arrVertices; // in COM space
    Array<Scalar> m_arrWeights;   // Mass weights

    // Bounding volume
    BVAABox m_bvLocalAAB;
};
//*/

/////////////////////////////////////////////////////////////////////////////////
// The Shape class
class Shape
{
public:
    Shape( Mesh * pMesh );
    virtual ~Shape();

    // Shape type
    virtual ShapeType GetType() const = 0;

    inline Bool IsConvex() const;
    inline Bool IsConcave() const;
    inline Bool IsDeformable() const;

    // AABox access
    inline const BVAABox & GetAAB() const;

    // Mesh access
    inline Mesh * GetMesh() const;

    // Mass properties (local coordinates)
    virtual Void ComputeMassProperties( Scalar fDensity, Scalar * outMass, Vertex3 * outCenterOfMass, Matrix3 * outInertiaTensor ) const = 0;

protected:
    // Mesh this shape is bound to
    Mesh * m_pMesh;
    BVAABox m_bvLocalAAB;
    /*
    // Shape properties
    Scalar m_fDensity;

    // Shape poses
    ShapePose m_hBindPose;
    ShapePose m_hLastPose;
    ShapePose m_hCurrentPose;
    //*/
};

/////////////////////////////////////////////////////////////////////////////////
// The ShapeConvex class
class ShapeConvex : public Shape
{
public:
    ShapeConvex( Mesh * pMesh );
    virtual ~ShapeConvex();

    // V-Clip/SAT support
    typedef MeshVoronoiGraph::Vertex   Vertex;
    typedef MeshVoronoiGraph::Edge     Edge;
    typedef MeshVoronoiGraph::Triangle Triangle;
    typedef MeshVoronoiGraph::Quad     Quad;
    typedef MeshVoronoiGraph::Polygon  Polygon;

    virtual const MeshVoronoiGraph * GetGraph() const = 0;

    // GJK/MPR support
    virtual Void QueryExtremalVertices( const Vector3 & vDirection, UInt * outMax = NULL, UInt * outMin = NULL ) const = 0;
    virtual Void QueryExtremalVertices( const Vector3 & vDirection, Vertex3 * outMax = NULL, Vertex3 * outMin = NULL ) const = 0;
};

/////////////////////////////////////////////////////////////////////////////////
// The ShapeConcave class
class ShapeConcave : public Shape
{
public:
    ShapeConcave( Mesh * pMesh );
    virtual ~ShapeConcave();

    // Node queries support
    typedef struct _node_leaf
    {
        Sphere vNode;
        DWord dwNodeID;
        AABoxNode * pLeaf;
    } NodeLeaf;

    class HNodeQuery : public AABoxTree::HCollider { public:
        HNodeQuery();
        virtual ~HNodeQuery();

        inline virtual Void Process( const AABoxNode * pLeaf );
        inline virtual Void Process( const AABoxNode * pLeafA, const AABoxNode * pLeafB );

        virtual Void Process( const NodeLeaf & nodeLeaf ) = 0;
        virtual Void Process( const NodeLeaf & nodeLeafA, const NodeLeaf & nodeLeafB ) = 0;
    };

    virtual Void QueryNodes( const BVAABox & bvAABox, HNodeQuery * pQuery ) const = 0;

    // Triangle queries support
    typedef struct _triangle_leaf
    {
        Triangle3 vTriangle;
        DWord dwTriangleID;
        AABoxNode * pLeaf;
    } TriangleLeaf;

    class HTriangleQuery : public AABoxTree::HCollider { public:
        HTriangleQuery();
        virtual ~HTriangleQuery();

        inline virtual Void Process( const AABoxNode * pLeaf );
        inline virtual Void Process( const AABoxNode * pLeafA, const AABoxNode * pLeafB );

        virtual Void Process( const TriangleLeaf & triLeaf ) = 0;
        virtual Void Process( const TriangleLeaf & triLeafA, const TriangleLeaf & triLeafB ) = 0;
    };

    virtual Void QueryTriangles( const BVAABox & bvAABox, HTriangleQuery * pQuery ) const = 0;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Shape.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_SHAPE_SHAPE_H

