/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Shape/Concave/ShapeDeformable.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Shapes : Concave, Deformable
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
#ifndef SCARAB_ENGINE_PHYSICS_SHAPE_CONCAVE_SHAPEDEFORMABLE_H
#define SCARAB_ENGINE_PHYSICS_SHAPE_CONCAVE_SHAPEDEFORMABLE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../Lib/Math/Geometry/MeshGraphs/Graphs/MeshDeformableGraph.h"

#include "../../CollisionDetection/NarrowPhase/ContactManifold.h"

#include "../Shape.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The ShapeDeformable class
class ShapeDeformable : public ShapeConcave
{
public: // Nested types
    class Node : public MeshDeformableGraph::Vertex { public:
        Node( const MeshVertexKey & vertexKey );
        virtual ~Node();

        // Shape access
        inline Sphere ComputeShape() const;

        // Index access
        inline UInt GetIndex() const;

        // State derivates
        Vector3 Normal; // Updated from facet normals
        Scalar Area;    // Updated from facet areas
    };
    class Link : public MeshDeformableGraph::Edge { public:
        Link( const MeshEdgeKey & edgeKey );
        virtual ~Link();

        // Nodes access
        inline Node * GetNode( UInt iNode ) const;
    };
    class Facet : public MeshDeformableGraph::Triangle { public:
        Facet( const MeshTriangleKey & triangleKey );
        virtual ~Facet();

        // Shape access
        inline Triangle3 ComputeShape() const;

        // Nodes access
        inline Node * GetNode( UInt iNode ) const;        
    };

    inline static MeshVertex * DefaultNodeBuilder( const MeshVertexKey & vertexKey );
    inline static MeshEdge *   DefaultLinkBuilder( const MeshEdgeKey & edgeKey );
    inline static MeshFacet *  DefaultFacetBuilder( const MeshFacetKey & facetKey );

public:
    ShapeDeformable( MeshDeformableGraph * pGraph, TriangleMesh * pMesh = NULL );
    ShapeDeformable( TriangleMesh * pMesh, MeshManifold3::VertexBuilder pfNodeBuilder = DefaultNodeBuilder,
                                           MeshManifold3::EdgeBuilder pfLinkBuilder = DefaultLinkBuilder,
                                           MeshManifold3::FacetBuilder pfFacetBuilder = DefaultFacetBuilder );
    virtual ~ShapeDeformable();

    // Shape type
    inline virtual ShapeType GetType() const;

    // Deformable access
    inline UInt GetNodeCount() const;
    inline Void EnumNodes() const;
    inline Node * EnumNextNode() const;
    inline Node * GetNode( UInt iNode ) const;

    inline UInt GetLinkCount() const;
    inline Void EnumLinks() const;
    inline Link * EnumNextLink() const;

    inline UInt GetFacetCount() const;
    inline Void EnumFacets() const;
    inline Facet * EnumNextFacet() const;

    inline MeshDeformableGraph * GetGraph() const;

    inline const AABoxTree * GetNodeTree() const;
    inline const AABoxTree * GetFacetTree() const;

    // Mass properties (in local coordinates)
    virtual Void ComputeMassProperties( Scalar fDensity, Scalar * outMass, Vertex3 * outCenterOfMass, Matrix3 * outInertiaTensor ) const;

    // Node queries support
    inline virtual Void QueryNodes( const BVAABox & bvAABox, HNodeQuery * pQuery ) const;

    // Triangle queries support
    inline virtual Void QueryTriangles( const BVAABox & bvAABox, HTriangleQuery * pQuery ) const;

    // Self collisions support
    inline Void ToggleSelfCollisions( Bool bEnable );
    Void SelfCollisionDetection();

    inline UInt GetSelfContactCount() const;
    inline const ContactPoint * GetSelfContact( UInt iSelfContact ) const;

    // Update support
    virtual Void UpdateCollisionData(); // BVTrees, LocalBV, CollisionGraphs
    virtual Void UpdateMesh();          // Mesh dynamic update

private:
    // Graph data
    Void _UpdateNormalsAreas() const;

    Bool m_bOwningGraph;
    MeshDeformableGraph * m_pGraph;
    const AABoxTree * m_pNodeTree;
    const AABoxTree * m_pFacetTree;

    // Self collisions support
    class SelfCollider : public AABoxTree::HCollider { public:
        SelfCollider();
        virtual ~SelfCollider();

        virtual Void Process( const AABoxNode * pLeafA, const AABoxNode * pLeafB );

        Array<ContactPoint> arrSelfContacts;
    };

    Bool m_bUseSelfCollisions;
    SelfCollider m_hSelfCollider;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "ShapeDeformable.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_SHAPE_CONCAVE_SHAPEDEFORMABLE_H

