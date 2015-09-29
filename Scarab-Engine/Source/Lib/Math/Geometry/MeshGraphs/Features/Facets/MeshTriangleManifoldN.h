/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Features/Facets/MeshTriangleManifoldN.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Triangle features : N-Manifold MeshGraphs
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_FEATURES_FACETS_MESHTRIANGLEMANIFOLDN_H
#define SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_FEATURES_FACETS_MESHTRIANGLEMANIFOLDN_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../MeshFeature.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The MeshTriangleManifoldN class
class MeshTriangleManifoldN : public MeshFacetTriangle
{
public:
    MeshTriangleManifoldN( const MeshTriangleKey & triangleKey );
    virtual ~MeshTriangleManifoldN();

    // Type
    inline virtual MeshTriangleType GetTriangleType() const;

    // Morphing
    //virtual MeshFacet * Grow( const MeshFacetKey & newKey );

    //inline virtual MeshFacet * Upgrade();
    //virtual MeshFacet * Downgrade();
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshTriangleManifoldN_F class
class MeshTriangleManifoldN_F : public MeshTriangleManifoldN
{
public:
    MeshTriangleManifoldN_F( const MeshTriangleKey & triangleKey );
    virtual ~MeshTriangleManifoldN_F();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_DYNARRAY( Facet, Facets )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshTriangleManifoldN_VF class
class MeshTriangleManifoldN_VF : public MeshTriangleManifoldN
{
public:
    MeshTriangleManifoldN_VF( const MeshTriangleKey & triangleKey );
    virtual ~MeshTriangleManifoldN_VF();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_N( Vertex, Vertices, 3 )
    _MESHFEATURE_DECLARE_DYNARRAY( Facet, Facets )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshTriangleManifoldN_EF class
class MeshTriangleManifoldN_EF : public MeshTriangleManifoldN
{
public:
    MeshTriangleManifoldN_EF( const MeshTriangleKey & triangleKey );
    virtual ~MeshTriangleManifoldN_EF();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_N( Edge, Edges, 3 )
    _MESHFEATURE_DECLARE_DYNARRAY( Facet, Facets )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshTriangleManifoldN_VEF class
class MeshTriangleManifoldN_VEF : public MeshTriangleManifoldN
{
public:
    MeshTriangleManifoldN_VEF( const MeshTriangleKey & triangleKey );
    virtual ~MeshTriangleManifoldN_VEF();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_N( Vertex, Vertices, 3 )
    _MESHFEATURE_DECLARE_N( Edge, Edges, 3 )
    _MESHFEATURE_DECLARE_DYNARRAY( Facet, Facets )
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "MeshTriangleManifoldN.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_FEATURES_FACETS_MESHTRIANGLEMANIFOLDN_H

