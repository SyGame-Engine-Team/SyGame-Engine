/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Features/Facets/MeshPolygonManifoldN.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Polygon features : N-Manifold MeshGraphs
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_FEATURES_FACETS_MESHPOLYGONMANIFOLDN_H
#define SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_FEATURES_FACETS_MESHPOLYGONMANIFOLDN_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../MeshFeature.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The MeshPolygonManifoldN class
class MeshPolygonManifoldN : public MeshFacetPolygon
{
public:
    MeshPolygonManifoldN( const MeshPolygonKey & polygonKey );
    virtual ~MeshPolygonManifoldN();

    // Type
    inline virtual MeshPolygonType GetPolygonType() const;

    // Morphing
    //virtual MeshFacet * Grow( const MeshFacetKey & newKey );
    //virtual MeshFacet * Shrink( const MeshFacetKey & newKey );

    //inline virtual MeshFacet * Upgrade();
    //virtual MeshFacet * Downgrade();
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshPolygonManifoldN_F class
class MeshPolygonManifoldN_F : public MeshPolygonManifoldN
{
public:
    MeshPolygonManifoldN_F( const MeshPolygonKey & polygonKey );
    virtual ~MeshPolygonManifoldN_F();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_DYNARRAY( Facet, Facets )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshPolygonManifoldN_VF class
class MeshPolygonManifoldN_VF : public MeshPolygonManifoldN
{
public:
    MeshPolygonManifoldN_VF( const MeshPolygonKey & polygonKey );
    virtual ~MeshPolygonManifoldN_VF();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_ARRAY( Vertex, Vertices )
    _MESHFEATURE_DECLARE_DYNARRAY( Facet, Facets )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshPolygonManifoldN_EF class
class MeshPolygonManifoldN_EF : public MeshPolygonManifoldN
{
public:
    MeshPolygonManifoldN_EF( const MeshPolygonKey & polygonKey );
    virtual ~MeshPolygonManifoldN_EF();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_ARRAY( Edge, Edges )
    _MESHFEATURE_DECLARE_DYNARRAY( Facet, Facets )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshPolygonManifoldN_VEF class
class MeshPolygonManifoldN_VEF : public MeshPolygonManifoldN
{
public:
    MeshPolygonManifoldN_VEF( const MeshPolygonKey & polygonKey );
    virtual ~MeshPolygonManifoldN_VEF();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_ARRAY( Vertex, Vertices )
    _MESHFEATURE_DECLARE_ARRAY( Edge, Edges )
    _MESHFEATURE_DECLARE_DYNARRAY( Facet, Facets )
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "MeshPolygonManifoldN.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_FEATURES_FACETS_MESHPOLYGONMANIFOLDN_H

