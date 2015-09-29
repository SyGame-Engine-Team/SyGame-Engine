/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Features/Edges/MeshEdgeManifold3.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Edge features : 3-Manifold MeshGraphs
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_FEATURES_EDGES_MESHEDGEMANIFOLD3_H
#define SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_FEATURES_EDGES_MESHEDGEMANIFOLD3_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../MeshFeature.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The MeshEdgeManifold3 class
class MeshEdgeManifold3 : public MeshEdge
{
public:
    MeshEdgeManifold3( const MeshEdgeKey & edgeKey );
    virtual ~MeshEdgeManifold3();

    // Type
    inline virtual MeshEdgeType GetEdgeType() const;

    // Morphing
    //virtual MeshEdge * Upgrade();
    //virtual MeshEdge * Downgrade();
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshEdgeManifold3_E class
class MeshEdgeManifold3_E : public MeshEdgeManifold3
{
public:
    MeshEdgeManifold3_E( const MeshEdgeKey & edgeKey );
    virtual ~MeshEdgeManifold3_E();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_DYNARRAY( Edge, Edges )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshEdgeManifold3_F class
class MeshEdgeManifold3_F : public MeshEdgeManifold3
{
public:
    MeshEdgeManifold3_F( const MeshEdgeKey & edgeKey );
    virtual ~MeshEdgeManifold3_F();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_N( Facet, Facets, 2 )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshEdgeManifold3_VE class
class MeshEdgeManifold3_VE : public MeshEdgeManifold3
{
public:
    MeshEdgeManifold3_VE( const MeshEdgeKey & edgeKey );
    virtual ~MeshEdgeManifold3_VE();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_N( Vertex, Vertices, 2 )
    _MESHFEATURE_DECLARE_DYNARRAY( Edge, Edges )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshEdgeManifold3_VF class
class MeshEdgeManifold3_VF : public MeshEdgeManifold3
{
public:
    MeshEdgeManifold3_VF( const MeshEdgeKey & edgeKey );
    virtual ~MeshEdgeManifold3_VF();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_N( Vertex, Vertices, 2 )
    _MESHFEATURE_DECLARE_N( Facet, Facets, 2 )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshEdgeManifold3_EF class
class MeshEdgeManifold3_EF : public MeshEdgeManifold3
{
public:
    MeshEdgeManifold3_EF( const MeshEdgeKey & edgeKey );
    virtual ~MeshEdgeManifold3_EF();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_DYNARRAY( Edge, Edges )
    _MESHFEATURE_DECLARE_N( Facet, Facets, 2 )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshEdgeManifold3_VEF class
class MeshEdgeManifold3_VEF : public MeshEdgeManifold3
{
public:
    MeshEdgeManifold3_VEF( const MeshEdgeKey & edgeKey );
    virtual ~MeshEdgeManifold3_VEF();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_N( Vertex, Vertices, 2 )
    _MESHFEATURE_DECLARE_DYNARRAY( Edge, Edges )
    _MESHFEATURE_DECLARE_N( Facet, Facets, 2 )
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "MeshEdgeManifold3.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_FEATURES_EDGES_MESHEDGEMANIFOLD3_H


