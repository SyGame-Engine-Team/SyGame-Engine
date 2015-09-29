/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Features/Facets/MeshQuadManifold3.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Quad features : 3-Manifold MeshGraphs
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_FEATURES_FACETS_MESHQUADMANIFOLD3_H
#define SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_FEATURES_FACETS_MESHQUADMANIFOLD3_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../MeshFeature.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The MeshQuadManifold3 class
class MeshQuadManifold3 : public MeshFacetQuad
{
public:
    MeshQuadManifold3( const MeshQuadKey & quadKey );
    virtual ~MeshQuadManifold3();

    // Type
    inline virtual MeshQuadType GetQuadType() const;

    // Morphing
    //virtual MeshFacet * Grow( const MeshFacetKey & newKey );
    //virtual MeshFacet * Shrink( const MeshFacetKey & newKey );

    //virtual MeshFacet * Upgrade();
    //inline virtual MeshFacet * Downgrade();
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshQuadManifold3_V class
class MeshQuadManifold3_V : public MeshQuadManifold3
{
public:
    MeshQuadManifold3_V( const MeshQuadKey & quadKey );
    virtual ~MeshQuadManifold3_V();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_N( Vertex, Vertices, 4 )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshQuadManifold3_E class
class MeshQuadManifold3_E : public MeshQuadManifold3
{
public:
    MeshQuadManifold3_E( const MeshQuadKey & quadKey );
    virtual ~MeshQuadManifold3_E();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_N( Edge, Edges, 4 )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshQuadManifold3_F class
class MeshQuadManifold3_F : public MeshQuadManifold3
{
public:
    MeshQuadManifold3_F( const MeshQuadKey & quadKey );
    virtual ~MeshQuadManifold3_F();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_N( Facet, Facets, 4 )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshQuadManifold3_VE class
class MeshQuadManifold3_VE : public MeshQuadManifold3
{
public:
    MeshQuadManifold3_VE( const MeshQuadKey & quadKey );
    virtual ~MeshQuadManifold3_VE();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_N( Vertex, Vertices, 4 )
    _MESHFEATURE_DECLARE_N( Edge, Edges, 4 )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshQuadManifold3_VF class
class MeshQuadManifold3_VF : public MeshQuadManifold3
{
public:
    MeshQuadManifold3_VF( const MeshQuadKey & quadKey );
    virtual ~MeshQuadManifold3_VF();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_N( Vertex, Vertices, 4 )
    _MESHFEATURE_DECLARE_N( Facet, Facets, 4 )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshQuadManifold3_EF class
class MeshQuadManifold3_EF : public MeshQuadManifold3
{
public:
    MeshQuadManifold3_EF( const MeshQuadKey & quadKey );
    virtual ~MeshQuadManifold3_EF();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_N( Edge, Edges, 4 )
    _MESHFEATURE_DECLARE_N( Facet, Facets, 4 )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshQuadManifold3_VEF class
class MeshQuadManifold3_VEF : public MeshQuadManifold3
{
public:
    MeshQuadManifold3_VEF( const MeshQuadKey & quadKey );
    virtual ~MeshQuadManifold3_VEF();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_N( Vertex, Vertices, 4 )
    _MESHFEATURE_DECLARE_N( Edge, Edges, 4 )
    _MESHFEATURE_DECLARE_N( Facet, Facets, 4 )
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "MeshQuadManifold3.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_FEATURES_FACETS_MESHQUADMANIFOLD3_H

