/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Graphs/MeshDeformableGraph.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Mesh Graph : Specialization for collision detection
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_GRAPHS_MESHDEFORMABLEGRAPH_H
#define SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_GRAPHS_MESHDEFORMABLEGRAPH_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../SpacePartitions/AABoxTree.h"

#include "../MeshManifold3.h" // Should be ManifoldN

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TMeshDeformableGraph class
template<typename Real>
class TMeshDeformableGraph : public MeshManifold3
{
public: // Features
    class Vertex : public MeshVertexManifoldN { public :
        Vertex( const MeshVertexKey & vertexKey );
        virtual ~Vertex();

        inline TBVAABox<Real> ComputeAAB( Real fRadius ) const;

        TVertex3<Real> Position;
        TAABoxNode<Real> * pLeaf;
    };
    class Edge : public MeshEdgeManifold2_V { public:
        Edge( const MeshEdgeKey & edgeKey );
        virtual ~Edge();
    };
    class Triangle : public MeshTriangleManifold3_V { public:
        Triangle( const MeshTriangleKey & triangleKey );
        virtual ~Triangle();

        inline Void UpdateNormalArea();
        inline TBVAABox<Real> ComputeAAB( Real fThickness ) const;

        TVector3<Real> Normal;
        Real Area;
        TAABoxNode<Real> * pLeaf;
    };

    inline static MeshVertex * DeformableVertexBuilder( const MeshVertexKey & vertexKey );
    inline static MeshEdge *   DeformableEdgeBuilder  ( const MeshEdgeKey & edgeKey );
    inline static MeshFacet *  DeformableFacetBuilder ( const MeshFacetKey & facetKey );

public:
    TMeshDeformableGraph( VertexBuilder pfVertexBuilder = DeformableVertexBuilder,
                          EdgeBuilder   pfEdgeBuilder   = DeformableEdgeBuilder,
                          FacetBuilder  pfFacetBuilder  = DeformableFacetBuilder );
    virtual ~TMeshDeformableGraph();

    // delayed construction
    inline virtual Void UseMemoryContext( MemoryContextID idMemoryContext, const GChar * strAllocatorName );

    // Common minimal methods
    inline virtual Void Create();
    inline virtual Void Destroy();

    inline virtual UInt MemorySize() const;
    inline virtual UInt Count() const;

    inline virtual Void Clear();

    // Feature data filling
    inline Void ComputeFeatureData( const TVertex3<Real> * arrVertices, Real fVertexRadius, Real fTriangleThickness );
    Void ComputeFeatureData( const Byte * arrVertices, UInt iStride, Real fVertexRadius, Real fTriangleThickness );

    // AABoxTrees access
    inline const TAABoxTree<Real> * GetVertexTree() const;
    inline const TAABoxTree<Real> * GetTriangleTree() const;

    Void UpdateTrees( Real fVertexRadius, Real fTriangleThickness );

private:
    // Helpers
    inline static const TVertex3<Real> & _GetPosition( UInt iVertex, const Byte * arrVertices, UInt iStride );

    // AAboxTrees
    TAABoxTree<Real> m_hVertexTree;
    TAABoxTree<Real> m_hTriangleTree;
};

// Explicit instanciation
typedef TMeshDeformableGraph<Float> MeshDeformableGraphf;
typedef TMeshDeformableGraph<Double> MeshDeformableGraphd;
typedef TMeshDeformableGraph<Scalar> MeshDeformableGraph;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "MeshDeformableGraph.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_GRAPHS_MESHDEFORMABLEGRAPH_H

