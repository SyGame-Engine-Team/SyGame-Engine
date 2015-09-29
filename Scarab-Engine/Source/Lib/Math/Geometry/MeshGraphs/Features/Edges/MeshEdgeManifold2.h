/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Features/Edges/MeshEdgeManifold2.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Edge features : 2-Manifold MeshGraphs
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_FEATURES_EDGES_MESHEDGEMANIFOLD2_H
#define SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_FEATURES_EDGES_MESHEDGEMANIFOLD2_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../MeshFeature.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The MeshEdgeManifold2 class
class MeshEdgeManifold2 : public MeshEdge
{
public:
    MeshEdgeManifold2( const MeshEdgeKey & edgeKey );
    virtual ~MeshEdgeManifold2();

    // Type
    inline virtual MeshEdgeType GetEdgeType() const;

    // Morphing
    //virtual MeshEdge * Upgrade();
    //inline virtual MeshEdge * Downgrade();
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshEdgeManifold2_V class
class MeshEdgeManifold2_V : public MeshEdgeManifold2
{
public:
    MeshEdgeManifold2_V( const MeshEdgeKey & edgeKey );
    virtual ~MeshEdgeManifold2_V();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_N( Vertex, Vertices, 2 )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshEdgeManifold2_E class
class MeshEdgeManifold2_E : public MeshEdgeManifold2
{
public:
    MeshEdgeManifold2_E( const MeshEdgeKey & edgeKey );
    virtual ~MeshEdgeManifold2_E();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_N( Edge, Edges, 2 )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshEdgeManifold2_VE class
class MeshEdgeManifold2_VE : public MeshEdgeManifold2
{
public:
    MeshEdgeManifold2_VE( const MeshEdgeKey & edgeKey );
    virtual ~MeshEdgeManifold2_VE();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_N( Vertex, Vertices, 2 )
    _MESHFEATURE_DECLARE_N( Edge, Edges, 2 )
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "MeshEdgeManifold2.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_FEATURES_EDGES_MESHEDGEMANIFOLD2_H



