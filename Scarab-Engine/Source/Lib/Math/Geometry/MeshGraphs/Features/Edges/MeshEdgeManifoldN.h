/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Features/Edges/MeshEdgeManifoldN.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Edge features : N-Manifold MeshGraphs
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_FEATURES_EDGES_MESHEDGEMANIFOLDN_H
#define SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_FEATURES_EDGES_MESHEDGEMANIFOLDN_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../MeshFeature.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The MeshEdgeManifoldN class
class MeshEdgeManifoldN : public MeshEdge
{
public:
    MeshEdgeManifoldN( const MeshEdgeKey & edgeKey );
    virtual ~MeshEdgeManifoldN();

    // Type
    inline virtual MeshEdgeType GetEdgeType() const;

    // Morphing
    //inline virtual MeshEdge * Upgrade();
    //virtual MeshEdge * Downgrade();
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshEdgeManifoldN_F class
class MeshEdgeManifoldN_F : public MeshEdgeManifoldN
{
public:
    MeshEdgeManifoldN_F( const MeshEdgeKey & edgeKey );
    virtual ~MeshEdgeManifoldN_F();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_DYNARRAY( Facet, Facets )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshEdgeManifoldN_VF class
class MeshEdgeManifoldN_VF : public MeshEdgeManifoldN
{
public:
    MeshEdgeManifoldN_VF( const MeshEdgeKey & edgeKey );
    virtual ~MeshEdgeManifoldN_VF();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_N( Vertex, Vertices, 2 )
    _MESHFEATURE_DECLARE_DYNARRAY( Facet, Facets )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshEdgeManifoldN_EF class
class MeshEdgeManifoldN_EF : public MeshEdgeManifoldN
{
public:
    MeshEdgeManifoldN_EF( const MeshEdgeKey & edgeKey );
    virtual ~MeshEdgeManifoldN_EF();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_DYNARRAY( Edge, Edges )
    _MESHFEATURE_DECLARE_DYNARRAY( Facet, Facets )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshEdgeManifoldN_VEF class
class MeshEdgeManifoldN_VEF : public MeshEdgeManifoldN
{
public:
    MeshEdgeManifoldN_VEF( const MeshEdgeKey & edgeKey );
    virtual ~MeshEdgeManifoldN_VEF();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_N( Vertex, Vertices, 2 )
    _MESHFEATURE_DECLARE_DYNARRAY( Edge, Edges )
    _MESHFEATURE_DECLARE_DYNARRAY( Facet, Facets )
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "MeshEdgeManifoldN.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_FEATURES_EDGES_MESHEDGEMANIFOLDN_H



