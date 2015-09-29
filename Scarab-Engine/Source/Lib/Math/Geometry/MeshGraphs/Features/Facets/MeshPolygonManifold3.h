/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Features/Facets/MeshPolygonManifold3.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Polygon features : 3-Manifold MeshGraphs
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_FEATURES_FACETS_MESHPOLYGONMANIFOLD3_H
#define SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_FEATURES_FACETS_MESHPOLYGONMANIFOLD3_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../MeshFeature.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The MeshPolygonManifold3 class
class MeshPolygonManifold3 : public MeshFacetPolygon
{
public:
    MeshPolygonManifold3( const MeshPolygonKey & polygonKey );
    virtual ~MeshPolygonManifold3();

    // Type
    inline virtual MeshPolygonType GetPolygonType() const;

    // Morphing
    //virtual MeshFacet * Grow( const MeshFacetKey & newKey );
    //virtual MeshFacet * Shrink( const MeshFacetKey & newKey );

    //virtual MeshFacet * Upgrade();
    //inline virtual MeshFacet * Downgrade();
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshPolygonManifold3_V class
class MeshPolygonManifold3_V : public MeshPolygonManifold3
{
public:
    MeshPolygonManifold3_V( const MeshPolygonKey & polygonKey );
    virtual ~MeshPolygonManifold3_V();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_ARRAY( Vertex, Vertices )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshPolygonManifold3_E class
class MeshPolygonManifold3_E : public MeshPolygonManifold3
{
public:
    MeshPolygonManifold3_E( const MeshPolygonKey & polygonKey );
    virtual ~MeshPolygonManifold3_E();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_ARRAY( Edge, Edges )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshPolygonManifold3_F class
class MeshPolygonManifold3_F : public MeshPolygonManifold3
{
public:
    MeshPolygonManifold3_F( const MeshPolygonKey & polygonKey );
    virtual ~MeshPolygonManifold3_F();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_ARRAY( Facet, Facets )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshPolygonManifold3_VE class
class MeshPolygonManifold3_VE : public MeshPolygonManifold3
{
public:
    MeshPolygonManifold3_VE( const MeshPolygonKey & polygonKey );
    virtual ~MeshPolygonManifold3_VE();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_ARRAY( Vertex, Vertices )
    _MESHFEATURE_DECLARE_ARRAY( Edge, Edges )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshPolygonManifold3_VF class
class MeshPolygonManifold3_VF : public MeshPolygonManifold3
{
public:
    MeshPolygonManifold3_VF( const MeshPolygonKey & polygonKey );
    virtual ~MeshPolygonManifold3_VF();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_ARRAY( Vertex, Vertices )
    _MESHFEATURE_DECLARE_ARRAY( Facet, Facets )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshPolygonManifold3_EF class
class MeshPolygonManifold3_EF : public MeshPolygonManifold3
{
public:
    MeshPolygonManifold3_EF( const MeshPolygonKey & polygonKey );
    virtual ~MeshPolygonManifold3_EF();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_ARRAY( Edge, Edges )
    _MESHFEATURE_DECLARE_ARRAY( Facet, Facets )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshPolygonManifold3_VEF class
class MeshPolygonManifold3_VEF : public MeshPolygonManifold3
{
public:
    MeshPolygonManifold3_VEF( const MeshPolygonKey & polygonKey );
    virtual ~MeshPolygonManifold3_VEF();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_ARRAY( Vertex, Vertices )
    _MESHFEATURE_DECLARE_ARRAY( Edge, Edges )
    _MESHFEATURE_DECLARE_ARRAY( Facet, Facets )
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "MeshPolygonManifold3.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_FEATURES_FACETS_MESHPOLYGONMANIFOLD3_H

