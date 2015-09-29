/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Features/Facets/MeshQuadManifoldN.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Quad features : N-Manifold MeshGraphs
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_FEATURES_FACETS_MESHQUADMANIFOLDN_H
#define SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_FEATURES_FACETS_MESHQUADMANIFOLDN_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../MeshFeature.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The MeshQuadManifoldN class
class MeshQuadManifoldN : public MeshFacetQuad
{
public:
    MeshQuadManifoldN( const MeshQuadKey & quadKey );
    virtual ~MeshQuadManifoldN();

    // Type
    inline virtual MeshQuadType GetQuadType() const;

    // Morphing
    //virtual MeshFacet * Grow( const MeshFacetKey & newKey );
    //virtual MeshFacet * Shrink( const MeshFacetKey & newKey );

    //inline virtual MeshFacet * Upgrade();
    //virtual MeshFacet * Downgrade();
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshQuadManifoldN_F class
class MeshQuadManifoldN_F : public MeshQuadManifoldN
{
public:
    MeshQuadManifoldN_F( const MeshQuadKey & quadKey );
    virtual ~MeshQuadManifoldN_F();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_DYNARRAY( Facet, Facets )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshQuadManifoldN_VF class
class MeshQuadManifoldN_VF : public MeshQuadManifoldN
{
public:
    MeshQuadManifoldN_VF( const MeshQuadKey & quadKey );
    virtual ~MeshQuadManifoldN_VF();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_N( Vertex, Vertices, 4 )
    _MESHFEATURE_DECLARE_DYNARRAY( Facet, Facets )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshQuadManifoldN_EF class
class MeshQuadManifoldN_EF : public MeshQuadManifoldN
{
public:
    MeshQuadManifoldN_EF( const MeshQuadKey & quadKey );
    virtual ~MeshQuadManifoldN_EF();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_N( Edge, Edges, 4 )
    _MESHFEATURE_DECLARE_DYNARRAY( Facet, Facets )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshQuadManifoldN_VEF class
class MeshQuadManifoldN_VEF : public MeshQuadManifoldN
{
public:
    MeshQuadManifoldN_VEF( const MeshQuadKey & quadKey );
    virtual ~MeshQuadManifoldN_VEF();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_N( Vertex, Vertices, 4 )
    _MESHFEATURE_DECLARE_N( Edge, Edges, 4 )
    _MESHFEATURE_DECLARE_DYNARRAY( Facet, Facets )
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "MeshQuadManifoldN.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_FEATURES_FACETS_MESHQUADMANIFOLDN_H

