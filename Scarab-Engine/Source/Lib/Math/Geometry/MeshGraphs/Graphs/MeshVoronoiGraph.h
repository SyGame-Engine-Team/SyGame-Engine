/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Graphs/MeshVoronoiGraph.h
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_GRAPHS_MESHVORONOIGRAPH_H
#define SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_GRAPHS_MESHVORONOIGRAPH_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Primitives/3D/Planar/Plane.h"

#include "../MeshManifold3.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TMeshVoronoiGraph class
template<typename Real>
class TMeshVoronoiGraph : public MeshManifold3
{
public: // Features
    class Vertex : public MeshVertexManifoldN_E { public :
        Vertex( const MeshVertexKey & vertexKey );
        virtual ~Vertex();

        TVertex3<Real> Position;
        TPlane<Real> * VoronoiRegion; // plane i <=> edge i
    };
    class Edge : public MeshEdgeManifold3_VF { public:
        Edge( const MeshEdgeKey & edgeKey );
        virtual ~Edge();

        TPlane<Real> VoronoiRegion[4]; // 0-1 = vertices, 2-3 = facets
    };
    class Triangle : public MeshTriangleManifold3_VE { public:
        Triangle( const MeshTriangleKey & triangleKey );
        virtual ~Triangle();

        TVector3<Real> Normal;
        TPlane<Real> VoronoiRegion[3]; // plane i <=> edge i
    };
    class Quad : public MeshQuadManifold3_VE { public:
        Quad( const MeshQuadKey & quadKey );
        virtual ~Quad();

        TVector3<Real> Normal;
        TPlane<Real> VoronoiRegion[4]; // plane i <=> edge i
    };
    class Polygon : public MeshPolygonManifold3_VE { public:
        Polygon( const MeshPolygonKey & polygonKey );
        virtual ~Polygon();

        TVector3<Real> Normal;
        TPlane<Real> * VoronoiRegion; // plane i <=> edge i
    };

public:
    TMeshVoronoiGraph();
    virtual ~TMeshVoronoiGraph();

    // Feature data access
    inline const TVertex3<Real> & GetVertexPosition( const MeshVertexKey & vertexKey ) const;
    inline const TVertex3<Real> & GetVertexPosition( const MeshVertex * pVertex ) const;

    inline const TVector3<Real> & GetFacetNormal( const MeshFacetKey & facetKey ) const;
    const TVector3<Real> & GetFacetNormal( const MeshFacet * pFacet ) const;

    inline UInt GetVoronoiPlaneCount( const MeshFeatureKey & featureKey ) const;
    UInt GetVoronoiPlaneCount( const MeshFeature * pFeature ) const;

    inline const TPlane<Real> & GetVoronoiPlane( const MeshFeatureKey & featureKey, UInt iPlane ) const;
    const TPlane<Real> & GetVoronoiPlane( const MeshFeature * pFeature, UInt iPlane ) const;

    // Feature data filling
    inline Void ComputeFeatureData( const TVertex3<Real> * arrVertices );
    Void ComputeFeatureData( const Byte * arrVertices, UInt iStride );

private:
    // Feature builders
    inline static MeshVertex * _VoronoiVertexBuilder( const MeshVertexKey & vertexKey );
    inline static MeshEdge *   _VoronoiEdgeBuilder  ( const MeshEdgeKey & edgeKey );
    inline static MeshFacet *  _VoronoiFacetBuilder ( const MeshFacetKey & facetKey );

    // Helpers
    inline static const TVertex3<Real> & _GetPosition( UInt iVertex, const Byte * arrVertices, UInt iStride );

    static Void _ComputeFacetNormal( Triangle * pTriangle );
    static Void _ComputeFacetNormal( Quad * pQuad );
    static Void _ComputeFacetNormal( Polygon * pPolygon );

    Void _ComputeVoronoiRegion( Vertex * pVertex );
    static Void _ComputeVoronoiRegion( Edge * pEdge );
    static Void _ComputeVoronoiRegion( Triangle * pTriangle );
    static Void _ComputeVoronoiRegion( Quad * pQuad );
    Void _ComputeVoronoiRegion( Polygon * pPolygon );
};

// Explicit instanciation
typedef TMeshVoronoiGraph<Float> MeshVoronoiGraphf;
typedef TMeshVoronoiGraph<Double> MeshVoronoiGraphd;
typedef TMeshVoronoiGraph<Scalar> MeshVoronoiGraph;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "MeshVoronoiGraph.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_GRAPHS_MESHVORONOIGRAPH_H

