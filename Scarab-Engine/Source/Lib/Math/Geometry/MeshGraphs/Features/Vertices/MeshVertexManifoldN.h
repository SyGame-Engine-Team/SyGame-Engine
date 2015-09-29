/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Features/Vertices/MeshVertexManifoldN.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Vertex features : 3-Manifold, N-Manifold MeshGraphs
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_FEATURES_VERTICES_MESHVERTEXMANIFOLDN_H
#define SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_FEATURES_VERTICES_MESHVERTEXMANIFOLDN_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../MeshFeature.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The MeshVertexManifoldN class
class MeshVertexManifoldN : public MeshVertex
{
public:
    MeshVertexManifoldN( const MeshVertexKey & vertexKey );
    virtual ~MeshVertexManifoldN();

    // Type
    inline virtual MeshVertexType GetVertexType() const;

    // Morphing
    //inline virtual MeshVertex * Upgrade();
    //virtual MeshVertex * Downgrade();
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshVertexManifoldN_V class
class MeshVertexManifoldN_V : public MeshVertexManifoldN
{
public:
    MeshVertexManifoldN_V( const MeshVertexKey & vertexKey );
    virtual ~MeshVertexManifoldN_V();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_DYNARRAY( Vertex, Vertices )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshVertexManifoldN_E class
class MeshVertexManifoldN_E : public MeshVertexManifoldN
{
public:
    MeshVertexManifoldN_E( const MeshVertexKey & vertexKey );
    virtual ~MeshVertexManifoldN_E();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_DYNARRAY( Edge, Edges )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshVertexManifoldN_F class
class MeshVertexManifoldN_F : public MeshVertexManifoldN
{
public:
    MeshVertexManifoldN_F( const MeshVertexKey & vertexKey );
    virtual ~MeshVertexManifoldN_F();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_DYNARRAY( Facet, Facets )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshVertexManifoldN_VE class
class MeshVertexManifoldN_VE : public MeshVertexManifoldN
{
public:
    MeshVertexManifoldN_VE( const MeshVertexKey & vertexKey );
    virtual ~MeshVertexManifoldN_VE();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_DYNARRAY( Vertex, Vertices )
    _MESHFEATURE_DECLARE_DYNARRAY( Edge, Edges )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshVertexManifoldN_VF class
class MeshVertexManifoldN_VF : public MeshVertexManifoldN
{
public:
    MeshVertexManifoldN_VF( const MeshVertexKey & vertexKey );
    virtual ~MeshVertexManifoldN_VF();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_DYNARRAY( Vertex, Vertices )
    _MESHFEATURE_DECLARE_DYNARRAY( Facet, Facets )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshVertexManifoldN_EF class
class MeshVertexManifoldN_EF : public MeshVertexManifoldN
{
public:
    MeshVertexManifoldN_EF( const MeshVertexKey & vertexKey );
    virtual ~MeshVertexManifoldN_EF();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_DYNARRAY( Edge, Edges )
    _MESHFEATURE_DECLARE_DYNARRAY( Facet, Facets )
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshVertexManifoldN_VEF class
class MeshVertexManifoldN_VEF : public MeshVertexManifoldN
{
public:
    MeshVertexManifoldN_VEF( const MeshVertexKey & vertexKey );
    virtual ~MeshVertexManifoldN_VEF();

    // Type
    inline virtual MeshLinkageType GetLinkageType() const;

    // Linkage
    _MESHFEATURE_DECLARE_DYNARRAY( Vertex, Vertices )
    _MESHFEATURE_DECLARE_DYNARRAY( Edge, Edges )
    _MESHFEATURE_DECLARE_DYNARRAY( Facet, Facets )
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "MeshVertexManifoldN.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_FEATURES_VERTICES_MESHVERTEXMANIFOLDN_H

