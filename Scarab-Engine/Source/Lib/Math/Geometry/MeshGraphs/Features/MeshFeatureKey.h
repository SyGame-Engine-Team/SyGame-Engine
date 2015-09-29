/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Features/MeshFeatureKey.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Vertex-Edge-Facet feature keys for MeshGraphs
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_FEATURES_MESHFEATUREKEY_H
#define SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_FEATURES_MESHFEATUREKEY_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../Memory/MemoryManager.h"
#include "../../../../Object/Patterns/VirtualHolder.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
enum MeshFeatureType
{
    MESH_FEATURE_VERTEX = 0,
    MESH_FEATURE_EDGE,
    MESH_FEATURE_FACET
};

enum MeshVertexType
{
    MESH_VERTEX_MANIFOLD2 = 0,
    MESH_VERTEX_MANIFOLDN
};
enum MeshEdgeType
{
    MESH_EDGE_MANIFOLD2 = 0,
    MESH_EDGE_MANIFOLD3,
    MESH_EDGE_MANIFOLDN
};
enum MeshFacetType
{
    MESH_FACET_TRIANGLE = 0,
    MESH_FACET_QUAD,
    MESH_FACET_POLYGON
};
enum MeshTriangleType
{
    MESH_TRIANGE_MANIFOLD3 = 0,
    MESH_TRIANGE_MANIFOLDN
};
enum MeshQuadType
{
    MESH_QUAD_MANIFOLD3 = 0,
    MESH_QUAD_MANIFOLDN
};
enum MeshPolygonType
{
    MESH_POLYGON_MANIFOLD3 = 0,
    MESH_POLYGON_MANIFOLDN
};

enum MeshLinkageType
{
    MESH_LINKAGE_NULL = 0,

    MESH_LINKAGE_V,
    MESH_LINKAGE_E,
    MESH_LINKAGE_F,
    MESH_LINKAGE_VE,
    MESH_LINKAGE_VF,
    MESH_LINKAGE_EF,
    MESH_LINKAGE_VEF
};

// Prototypes
class MeshFeatureKey;

class MeshVertexKey;
class MeshEdgeKey;
class MeshFacetKey;

class MeshTriangleKey;
class MeshQuadKey;
class MeshPolygonKey;

class MeshFacetKeyHolder;

/////////////////////////////////////////////////////////////////////////////////
// The MeshFeatureKey class
class MeshFeatureKey
{
public:
    MeshFeatureKey();
    virtual ~MeshFeatureKey();

    // Type
    virtual MeshFeatureType GetType() const = 0;

    // Operators
    virtual Bool operator==( const MeshFeatureKey & rhs ) const = 0;
    virtual Bool operator!=( const MeshFeatureKey & rhs ) const = 0;

    virtual UInt operator[]( UInt rhs ) const = 0;
    virtual UInt & operator[]( UInt rhs ) = 0;
    virtual UInt operator[]( Int rhs ) const = 0;
    virtual UInt & operator[]( Int rhs ) = 0;
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshVertexKey class
class MeshVertexKey : public MeshFeatureKey
{
public:
    static const MeshVertexKey Null;

    MeshVertexKey();
    MeshVertexKey( UInt iV );
    MeshVertexKey( const MeshVertexKey & rhs );
    virtual ~MeshVertexKey();

    // Type
    inline virtual MeshFeatureType GetType() const;

    // Operators
    inline virtual MeshVertexKey & operator=( const MeshVertexKey & rhs );

    inline virtual Bool operator==( const MeshFeatureKey & rhs ) const;
    inline virtual Bool operator!=( const MeshFeatureKey & rhs ) const;

    inline virtual UInt operator[]( UInt rhs ) const;
    inline virtual UInt & operator[]( UInt rhs );
    inline virtual UInt operator[]( Int rhs ) const;
    inline virtual UInt & operator[]( Int rhs );

    // Comparator
    static Int Compare( const MeshVertexKey * const & rLeft, const MeshVertexKey * const & rRight, Void * pUserData );

    // Key data
	UInt V;
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshEdgeKey class
class MeshEdgeKey : public MeshFeatureKey
{
public:
    static const MeshEdgeKey Null;

    MeshEdgeKey();
    MeshEdgeKey( UInt iA, UInt iB );
    MeshEdgeKey( const MeshEdgeKey & rhs );
    virtual ~MeshEdgeKey();

    // Type
    inline virtual MeshFeatureType GetType() const;

    // Operators
    inline virtual MeshEdgeKey & operator=( const MeshEdgeKey & rhs );

    inline virtual Bool operator==( const MeshFeatureKey & rhs ) const;
    inline virtual Bool operator!=( const MeshFeatureKey & rhs ) const;

    inline virtual UInt operator[]( UInt rhs ) const;
    inline virtual UInt & operator[]( UInt rhs );
    inline virtual UInt operator[]( Int rhs ) const;
    inline virtual UInt & operator[]( Int rhs );

    // Comparator
    static Int Compare( const MeshEdgeKey * const & rLeft, const MeshEdgeKey * const & rRight, Void * pUserData );

    // Key reduction
    inline MeshVertexKey GetVertexKey( UInt iVertex ) const;

    // Key data
    union {
		struct {
			UInt A, B;
		};
		UInt V[2];
	};
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshFacetKey class
class MeshFacetKey : public MeshFeatureKey
{
public:
    MeshFacetKey();
    virtual ~MeshFacetKey();

    // Type
    inline virtual MeshFeatureType GetType() const;

    virtual MeshFacetType GetFacetType() const = 0;
    virtual UInt GetFacetSize() const = 0;

    // Comparator
    static Int Compare( const MeshFacetKey * const & rLeft, const MeshFacetKey * const & rRight, Void * pUserData );

    // Key reduction
    virtual MeshVertexKey GetVertexKey( UInt iVertex ) const = 0;
    virtual MeshEdgeKey GetEdgeKey( UInt iEdge ) const = 0;
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshTriangleKey class
class MeshTriangleKey : public MeshFacetKey
{
public:
    static const MeshTriangleKey Null;

    MeshTriangleKey();
    MeshTriangleKey( UInt iA, UInt iB, UInt iC );
    MeshTriangleKey( const MeshPolygonKey & rhs );
    MeshTriangleKey( const MeshTriangleKey & rhs );
    virtual ~MeshTriangleKey();

    // Type
    inline virtual MeshFacetType GetFacetType() const;
    inline virtual UInt GetFacetSize() const;

    // Operators
    inline virtual MeshTriangleKey & operator=( const MeshTriangleKey & rhs );

    virtual Bool operator==( const MeshFeatureKey & rhs ) const;
    virtual Bool operator!=( const MeshFeatureKey & rhs ) const;

    inline virtual UInt operator[]( UInt rhs ) const;
    inline virtual UInt & operator[]( UInt rhs );
    inline virtual UInt operator[]( Int rhs ) const;
    inline virtual UInt & operator[]( Int rhs );

    // Comparator
    static Int Compare( const MeshTriangleKey * const & rLeft, const MeshTriangleKey * const & rRight, Void * pUserData );

    // Key reduction
    inline virtual MeshVertexKey GetVertexKey( UInt iVertex ) const;
    inline virtual MeshEdgeKey GetEdgeKey( UInt iEdge ) const;

    // Key data
    union {
		struct {
			UInt A, B, C;
		};
		UInt V[3];
	};
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshQuadKey class
class MeshQuadKey : public MeshFacetKey
{
public:
    static const MeshQuadKey Null;

    MeshQuadKey();
    MeshQuadKey( UInt iA, UInt iB, UInt iC, UInt iD );
    MeshQuadKey( const MeshPolygonKey & rhs );
    MeshQuadKey( const MeshQuadKey & rhs );
    virtual ~MeshQuadKey();

    // Type
    inline virtual MeshFacetType GetFacetType() const;
    inline virtual UInt GetFacetSize() const;

    // Operators
    inline virtual MeshQuadKey & operator=( const MeshQuadKey & rhs );

    virtual Bool operator==( const MeshFeatureKey & rhs ) const;
    virtual Bool operator!=( const MeshFeatureKey & rhs ) const;

    inline virtual UInt operator[]( UInt rhs ) const;
    inline virtual UInt & operator[]( UInt rhs );
    inline virtual UInt operator[]( Int rhs ) const;
    inline virtual UInt & operator[]( Int rhs );

    // Comparator
    static Int Compare( const MeshQuadKey * const & rLeft, const MeshQuadKey * const & rRight, Void * pUserData );

    // Key reduction
    inline virtual MeshVertexKey GetVertexKey( UInt iVertex ) const;
    inline virtual MeshEdgeKey GetEdgeKey( UInt iEdge ) const;

    // Key data
    union {
		struct {
			UInt A, B, C, D;
		};
		UInt V[4];
	};
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshPolygonKey class
class MeshPolygonKey : public MeshFacetKey
{
public:
    static const MeshPolygonKey Null;

    MeshPolygonKey();
    MeshPolygonKey( UInt iSize );
    MeshPolygonKey( const UInt * arrVertices, UInt iSize );
    MeshPolygonKey( const MeshTriangleKey & rhs );
    MeshPolygonKey( const MeshQuadKey & rhs );
    MeshPolygonKey( const MeshPolygonKey & rhs );
    virtual ~MeshPolygonKey();

    // Type
    inline virtual MeshFacetType GetFacetType() const;
    inline virtual UInt GetFacetSize() const;

    // Operators
    inline virtual MeshPolygonKey & operator=( const MeshPolygonKey & rhs );

    virtual Bool operator==( const MeshFeatureKey & rhs ) const;
    virtual Bool operator!=( const MeshFeatureKey & rhs ) const;

    inline virtual UInt operator[]( UInt rhs ) const;
    inline virtual UInt & operator[]( UInt rhs );
    inline virtual UInt operator[]( Int rhs ) const;
    inline virtual UInt & operator[]( Int rhs );

    // Comparator
    static Int Compare( const MeshPolygonKey * const & rLeft, const MeshPolygonKey * const & rRight, Void * pUserData );

    // Key reduction
    inline virtual MeshVertexKey GetVertexKey( UInt iVertex ) const;
    inline virtual MeshEdgeKey GetEdgeKey( UInt iEdge ) const;

    // Key data
    const UInt SIZE;
    UInt * V;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "MeshFeatureKey.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_FEATURES_MESHFEATUREKEY_H


