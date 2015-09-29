/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Features/Vertices/MeshVertexManifold2.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Vertex features : 2-Manifold MeshGraphs
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_FEATURES_VERTICES_MESHVERTEXMANIFOLD2_H
#define SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_FEATURES_VERTICES_MESHVERTEXMANIFOLD2_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../MeshFeature.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The MeshVertexManifold2 class
class MeshVertexManifold2 : public MeshVertex
{
public:
    MeshVertexManifold2( const MeshVertexKey & vertexKey );
    virtual ~MeshVertexManifold2();

    // Type
    inline virtual MeshVertexType GetVertexType() const;

    // Morphing
    //virtual MeshVertex * Upgrade();
    //inline virtual MeshVertex * Downgrade();
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshVertexManifold2_V class
class MeshVertexManifold2_V : public MeshVertexManifold2
{
public:
    MeshVertexManifold2_V( const MeshVertexKey & vertexKey );
    virtual ~MeshVertexManifold2_V();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_N( Vertex, Vertices, 2 )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshVertexManifold2_E class
class MeshVertexManifold2_E : public MeshVertexManifold2
{
public:
    MeshVertexManifold2_E( const MeshVertexKey & vertexKey );
    virtual ~MeshVertexManifold2_E();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_N( Edge, Edges, 2 )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshVertexManifold2_VE class
class MeshVertexManifold2_VE : public MeshVertexManifold2
{
public:
    MeshVertexManifold2_VE( const MeshVertexKey & vertexKey );
    virtual ~MeshVertexManifold2_VE();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_N( Vertex, Vertices, 2 )
    _MESHFEATURE_DECLARE_N( Edge, Edges, 2 )
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "MeshVertexManifold2.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_FEATURES_VERTICES_MESHVERTEXMANIFOLD2_H



