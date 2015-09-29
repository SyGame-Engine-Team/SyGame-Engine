/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Features/MeshFeature.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Vertex-Edge-Facet features for MeshGraphs
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_FEATURES_MESHFEATURE_H
#define SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_FEATURES_MESHFEATURE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../Datastruct/Array/Array.h"
#include "../../../../Datastruct/Table/TreeMap.h"

#include "MeshFeatureKey.h"
#include "MeshFeatureDeclarations.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Prototypes
class MeshFeature;

class MeshVertex;
class MeshEdge;
class MeshFacet;

class MeshFacetTriangle;
class MeshFacetQuad;
class MeshFacetPolygon;

class MeshVertexManifold2;
class MeshVertexManifoldN;

class MeshEdgeManifold2;
class MeshEdgeManifold3;
class MeshEdgeManifoldN;

class MeshTriangleManifold3;
class MeshTriangleManifoldN;

class MeshQuadManifold3;
class MeshQuadManifoldN;

class MeshPolygonManifold3;
class MeshPolygonManifoldN;

class MeshVertexManifold2_V;  // 2 Vertices / 0 Edges / 0 Facets
class MeshVertexManifold2_E;  // 0 Vertices / 2 Edges / 0 Facets
class MeshVertexManifold2_VE; // 2 Vertices / 2 Edges / 0 Facets

class MeshVertexManifoldN_V;   // N Vertices / 0 Edges / 0 Facets
class MeshVertexManifoldN_E;   // 0 Vertices / N Edges / 0 Facets
class MeshVertexManifoldN_F;   // 0 Vertices / 0 Edges / N Facets
class MeshVertexManifoldN_VE;  // N Vertices / N Edges / 0 Facets
class MeshVertexManifoldN_VF;  // N Vertices / 0 Edges / N Facets
class MeshVertexManifoldN_EF;  // 0 Vertices / N Edges / N Facets
class MeshVertexManifoldN_VEF; // N Vertices / N Edges / N Facets

class MeshEdgeManifold2_V;  // 2 Vertices / 0 Edges / 0 Facets
class MeshEdgeManifold2_E;  // 0 Vertices / 2 Edges / 0 Facets
class MeshEdgeManifold2_VE; // 2 Vertices / 2 Edges / 0 Facets

class MeshEdgeManifold3_E;   // 0 Vertices / N Edges / 0 Facets
class MeshEdgeManifold3_F;   // 0 Vertices / 0 Edges / 2 Facets
class MeshEdgeManifold3_VE;  // 2 Vertices / N Edges / 0 Facets
class MeshEdgeManifold3_VF;  // 2 Vertices / 0 Edges / 2 Facets
class MeshEdgeManifold3_EF;  // 0 Vertices / N Edges / 2 Facets
class MeshEdgeManifold3_VEF; // 2 Vertices / N Edges / 2 Facets

class MeshEdgeManifoldN_F;   // 0 Vertices / 0 Edges / N Facets
class MeshEdgeManifoldN_VF;  // 2 Vertices / 0 Edges / N Facets
class MeshEdgeManifoldN_EF;  // 0 Vertices / N Edges / N Facets
class MeshEdgeManifoldN_VEF; // 2 Vertices / N Edges / N Facets

class MeshTriangleManifold3_V;   // 3 Vertices / 0 Edges / 0 Facets
class MeshTriangleManifold3_E;   // 0 Vertices / 3 Edges / 0 Facets
class MeshTriangleManifold3_F;   // 0 Vertices / 0 Edges / 3 Facets
class MeshTriangleManifold3_VE;  // 3 Vertices / 3 Edges / 0 Facets
class MeshTriangleManifold3_VF;  // 3 Vertices / 0 Edges / 3 Facets
class MeshTriangleManifold3_EF;  // 0 Vertices / 3 Edges / 3 Facets
class MeshTriangleManifold3_VEF; // 3 Vertices / 3 Edges / 3 Facets

class MeshTriangleManifoldN_F;   // 0 Vertices / 0 Edges / 3N Facets
class MeshTriangleManifoldN_VF;  // 3 Vertices / 0 Edges / 3N Facets
class MeshTriangleManifoldN_EF;  // 0 Vertices / 3 Edges / 3N Facets
class MeshTriangleManifoldN_VEF; // 3 Vertices / 3 Edges / 3N Facets

class MeshQuadManifold3_V;   // 4 Vertices / 0 Edges / 0 Facets
class MeshQuadManifold3_E;   // 0 Vertices / 4 Edges / 0 Facets
class MeshQuadManifold3_F;   // 0 Vertices / 0 Edges / 4 Facets
class MeshQuadManifold3_VE;  // 4 Vertices / 4 Edges / 0 Facets
class MeshQuadManifold3_VF;  // 4 Vertices / 0 Edges / 4 Facets
class MeshQuadManifold3_EF;  // 0 Vertices / 4 Edges / 4 Facets
class MeshQuadManifold3_VEF; // 4 Vertices / 4 Edges / 4 Facets

class MeshQuadManifoldN_F;   // 0 Vertices / 0 Edges / 4N Facets
class MeshQuadManifoldN_VF;  // 4 Vertices / 0 Edges / 4N Facets
class MeshQuadManifoldN_EF;  // 0 Vertices / 4 Edges / 4N Facets
class MeshQuadManifoldN_VEF; // 4 Vertices / 4 Edges / 4N Facets

class MeshPolygonManifold3_V;   // N Vertices / 0 Edges / 0 Facets
class MeshPolygonManifold3_E;   // 0 Vertices / N Edges / 0 Facets
class MeshPolygonManifold3_F;   // 0 Vertices / 0 Edges / N Facets
class MeshPolygonManifold3_VE;  // N Vertices / N Edges / 0 Facets
class MeshPolygonManifold3_VF;  // N Vertices / 0 Edges / N Facets
class MeshPolygonManifold3_EF;  // 0 Vertices / N Edges / N Facets
class MeshPolygonManifold3_VEF; // N Vertices / N Edges / N Facets

class MeshPolygonManifoldN_F;   // 0 Vertices / 0 Edges / NN Facets
class MeshPolygonManifoldN_VF;  // N Vertices / 0 Edges / NN Facets
class MeshPolygonManifoldN_EF;  // 0 Vertices / N Edges / NN Facets
class MeshPolygonManifoldN_VEF; // N Vertices / N Edges / NN Facets

/////////////////////////////////////////////////////////////////////////////////
// The MeshFeature class
class MeshFeature
{
public:
    MeshFeature();
    virtual ~MeshFeature();

    // Type
    virtual MeshFeatureType GetType() const = 0;
    inline virtual MeshLinkageType GetLinkageType() const;

    // Vertex Links
    inline virtual UInt GetVertexCapacity() const;
    inline virtual UInt GetVertexCount() const;
    inline virtual MeshVertex * GetVertex( UInt iVertex ) const;

    inline virtual UInt FindVertex( const MeshVertex * pVertex ) const;
    inline virtual Void ReplaceVertex( UInt iVertex, MeshVertex * pVertex );

    inline virtual UInt ConnectVertex( MeshVertex * pVertex );
    inline virtual Void DisconnectVertex( UInt iVertex );
    inline virtual Void DisconnectVertices();

    // Edge Links
    inline virtual UInt GetEdgeCapacity() const;
    inline virtual UInt GetEdgeCount() const;
    inline virtual MeshEdge * GetEdge( UInt iEdge ) const;

    inline virtual UInt FindEdge( const MeshEdge * pEdge ) const;
    inline virtual Void ReplaceEdge( UInt iEdge, MeshEdge * pEdge );

    inline virtual UInt ConnectEdge( MeshEdge * pEdge );
    inline virtual Void DisconnectEdge( UInt iEdge );
    inline virtual Void DisconnectEdges();

    // Facet Links
    inline virtual UInt GetFacetCapacity() const;
    inline virtual UInt GetFacetCount() const;
    inline virtual MeshFacet * GetFacet( UInt iFacet ) const;

    inline virtual UInt FindFacet( const MeshFacet * pFacet ) const;
    inline virtual Void ReplaceFacet( UInt iFacet, MeshFacet * pFacet );

    inline virtual UInt ConnectFacet( MeshFacet * pFacet );
    inline virtual Void DisconnectFacet( UInt iFacet );
    inline virtual Void DisconnectFacets();

    // Full disconnection
    inline virtual Void DisconnectAll();
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshVertex class
class MeshVertex : public MeshFeature
{
public:
    MeshVertex( const MeshVertexKey & vertexKey );
    virtual ~MeshVertex();

    // Type
    inline virtual MeshFeatureType GetType() const;

    virtual MeshVertexType GetVertexType() const = 0;

    // Morphing
    //virtual MeshVertex * Upgrade() = 0;
    //virtual MeshVertex * Downgrade() = 0;

    //virtual MeshVertex * ExpandLinks() = 0;
    //virtual MeshVertex * CollapseLinks() = 0;

    // Key data
    const MeshVertexKey Key;

//protected:
//    Void _UpdateAdjacency( MeshVertex * pNewVertex, UInt iVertexCount, UInt iEdgeCount, UInt iFacetCount );
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshEdge class
class MeshEdge : public MeshFeature
{
public:
    MeshEdge( const MeshEdgeKey & edgeKey );
    virtual ~MeshEdge();

    // Type
    inline virtual MeshFeatureType GetType() const;

    virtual MeshEdgeType GetEdgeType() const = 0;

    // Morphing
    //virtual MeshEdge * Upgrade() = 0;
    //virtual MeshEdge * Downgrade() = 0;

    //virtual MeshVertex * ExpandLinks() = 0;
    //virtual MeshVertex * CollapseLinks() = 0;

    // Key data
    const MeshEdgeKey Key;

//protected:
//    Void _UpdateAdjacency( MeshEdge * pNewEdge, UInt iVertexCount, UInt iEdgeCount, UInt iFacetCount );
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshFacet class
class MeshFacet : public MeshFeature
{
public:
    MeshFacet();
    virtual ~MeshFacet();

    // Type
    inline virtual MeshFeatureType GetType() const;

    virtual MeshFacetType GetFacetType() const = 0;
    virtual UInt GetFacetSize() const = 0;

    // Morphing
    //virtual MeshFacet * Grow( const MeshFacetKey & newKey ) = 0;
    //virtual MeshFacet * Shrink( const MeshFacetKey & newKey ) = 0;

    //virtual MeshFacet * Upgrade() = 0;
    //virtual MeshFacet * Downgrade() = 0;

    //virtual MeshVertex * ExpandLinks() = 0;
    //virtual MeshVertex * CollapseLinks() = 0;

    // Key data
    virtual const MeshFacetKey * GetKey() const = 0;

//protected:
//    Void _UpdateAdjacency( MeshFacet * pNewFacet, UInt iVertexCount, UInt iEdgeCount, UInt iFacetCount );
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshFacetTriangle class
class MeshFacetTriangle : public MeshFacet
{
public:
    MeshFacetTriangle( const MeshTriangleKey & triangleKey );
    virtual ~MeshFacetTriangle();

    // Type
    inline virtual MeshFacetType GetFacetType() const;
    inline virtual UInt GetFacetSize() const;

    virtual MeshTriangleType GetTriangleType() const = 0;

    // Morphing
    //inline virtual MeshFacet * Shrink( const MeshFacetKey & newKey );

    // Key data
    inline virtual const MeshFacetKey * GetKey() const;
    const MeshTriangleKey Key;
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshFacetQuad class
class MeshFacetQuad : public MeshFacet
{
public:
    MeshFacetQuad( const MeshQuadKey & quadKey );
    virtual ~MeshFacetQuad();

    // Type
    inline virtual MeshFacetType GetFacetType() const;
    inline virtual UInt GetFacetSize() const;

    virtual MeshQuadType GetQuadType() const = 0;

    // Key data
    inline virtual const MeshFacetKey * GetKey() const;
    const MeshQuadKey Key;
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshFacetPolygon class
class MeshFacetPolygon : public MeshFacet
{
public:
    MeshFacetPolygon( const MeshPolygonKey & polygonKey );
    virtual ~MeshFacetPolygon();

    // Type
    inline virtual MeshFacetType GetFacetType() const;
    inline virtual UInt GetFacetSize() const;

    virtual MeshPolygonType GetPolygonType() const = 0;

    // Key data
    inline virtual const MeshFacetKey * GetKey() const;
    const MeshPolygonKey Key;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "MeshFeature.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_FEATURES_MESHFEATURE_H

