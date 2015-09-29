/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Features/Facets/MeshTriangleManifold3.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Triangle features : 3-Manifold MeshGraphs
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_FEATURES_FACETS_MESHTRIANGLEMANIFOLD3_H
#define SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_FEATURES_FACETS_MESHTRIANGLEMANIFOLD3_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../MeshFeature.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The MeshTriangleManifold3 class
class MeshTriangleManifold3 : public MeshFacetTriangle
{
public:
    MeshTriangleManifold3( const MeshTriangleKey & triangleKey );
    virtual ~MeshTriangleManifold3();

    // Type
    inline virtual MeshTriangleType GetTriangleType() const;

    // Morphing
    //virtual MeshFacet * Grow( const MeshFacetKey & newKey );

    //virtual MeshFacet * Upgrade();
    //inline virtual MeshFacet * Downgrade();
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshTriangleManifold3_V class
class MeshTriangleManifold3_V : public MeshTriangleManifold3
{
public:
    MeshTriangleManifold3_V( const MeshTriangleKey & triangleKey );
    virtual ~MeshTriangleManifold3_V();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_N( Vertex, Vertices, 3 )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshTriangleManifold3_E class
class MeshTriangleManifold3_E : public MeshTriangleManifold3
{
public:
    MeshTriangleManifold3_E( const MeshTriangleKey & triangleKey );
    virtual ~MeshTriangleManifold3_E();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_N( Edge, Edges, 3 )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshTriangleManifold3_F class
class MeshTriangleManifold3_F : public MeshTriangleManifold3
{
public:
    MeshTriangleManifold3_F( const MeshTriangleKey & triangleKey );
    virtual ~MeshTriangleManifold3_F();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_N( Facet, Facets, 3 )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshTriangleManifold3_VE class
class MeshTriangleManifold3_VE : public MeshTriangleManifold3
{
public:
    MeshTriangleManifold3_VE( const MeshTriangleKey & triangleKey );
    virtual ~MeshTriangleManifold3_VE();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_N( Vertex, Vertices, 3 )
    _MESHFEATURE_DECLARE_N( Edge, Edges, 3 )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshTriangleManifold3_VF class
class MeshTriangleManifold3_VF : public MeshTriangleManifold3
{
public:
    MeshTriangleManifold3_VF( const MeshTriangleKey & triangleKey );
    virtual ~MeshTriangleManifold3_VF();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_N( Vertex, Vertices, 3 )
    _MESHFEATURE_DECLARE_N( Facet, Facets, 3 )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshTriangleManifold3_EF class
class MeshTriangleManifold3_EF : public MeshTriangleManifold3
{
public:
    MeshTriangleManifold3_EF( const MeshTriangleKey & triangleKey );
    virtual ~MeshTriangleManifold3_EF();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_N( Edge, Edges, 3 )
    _MESHFEATURE_DECLARE_N( Facet, Facets, 3 )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshTriangleManifold3_VEF class
class MeshTriangleManifold3_VEF : public MeshTriangleManifold3
{
public:
    MeshTriangleManifold3_VEF( const MeshTriangleKey & triangleKey );
    virtual ~MeshTriangleManifold3_VEF();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_N( Vertex, Vertices, 3 )
    _MESHFEATURE_DECLARE_N( Edge, Edges, 3 )
    _MESHFEATURE_DECLARE_N( Facet, Facets, 3 )
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "MeshTriangleManifold3.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_FEATURES_FACETS_MESHTRIANGLEMANIFOLD3_H

