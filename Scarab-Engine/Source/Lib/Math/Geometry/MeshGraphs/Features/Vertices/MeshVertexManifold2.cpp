/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Features/Vertices/MeshVertexManifold2.cpp
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
// Includes
#include "MeshVertexManifold2.h"

/////////////////////////////////////////////////////////////////////////////////
// MeshVertexManifold2 implementation
MeshVertexManifold2::MeshVertexManifold2( const MeshVertexKey & vertexKey ):
    MeshVertex( vertexKey )
{
    // nothing to do
}
MeshVertexManifold2::~MeshVertexManifold2()
{
    // nothing to do
}

//MeshVertex * MeshVertexManifold2::Upgrade()
//{
//    // Begin Upgrade
//    MeshVertex * pUpgradedVertex = NULL;
//    Bool bUpdateVertices = false;
//    Bool bUpdateEdges = false;
//
//    ModelingFn->SelectMemory( TEXT("MeshGraphs") );
//    switch( GetLinkageType() ) {
//        case MESH_LINKAGE_V:  pUpgradedVertex = New MeshVertexManifoldN_V( Key );  bUpdateVertices = true;                      break;
//        case MESH_LINKAGE_E:  pUpgradedVertex = New MeshVertexManifoldN_E( Key );  bUpdateEdges = true;                         break;
//        case MESH_LINKAGE_VE: pUpgradedVertex = New MeshVertexManifoldN_VE( Key ); bUpdateVertices = true; bUpdateEdges = true; break;
//        default: Assert( false ); break;
//    }
//    ModelingFn->UnSelectMemory();
//
//    _UpdateAdjacency( pUpgradedVertex, bUpdateVertices ? 2 : 0,
//                                       bUpdateEdges ? 2 : 0,
//                                       0 );
//
//    // End Upgrade
//    return pUpgradedVertex;
//}

/////////////////////////////////////////////////////////////////////////////////
// MeshVertexManifold2_V implementation
MeshVertexManifold2_V::MeshVertexManifold2_V( const MeshVertexKey & vertexKey ):
    MeshVertexManifold2( vertexKey )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_2( Vertices )
}
MeshVertexManifold2_V::~MeshVertexManifold2_V()
{
    // nothing to do
}

_MESHFEATURE_IMPLEMENT_2( MeshVertexManifold2_V, Vertex, Vertices )

/////////////////////////////////////////////////////////////////////////////////
// MeshVertexManifold2_E implementation
MeshVertexManifold2_E::MeshVertexManifold2_E( const MeshVertexKey & vertexKey ):
    MeshVertexManifold2( vertexKey )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_2( Edges )
}
MeshVertexManifold2_E::~MeshVertexManifold2_E()
{
    // nothing to do
}

_MESHFEATURE_IMPLEMENT_2( MeshVertexManifold2_E, Edge, Edges )

/////////////////////////////////////////////////////////////////////////////////
// MeshVertexManifold2_VE implementation
MeshVertexManifold2_VE::MeshVertexManifold2_VE( const MeshVertexKey & vertexKey ):
    MeshVertexManifold2( vertexKey )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_2( Vertices )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_2( Edges )
}
MeshVertexManifold2_VE::~MeshVertexManifold2_VE()
{
    // nothing to do
}

_MESHFEATURE_IMPLEMENT_2( MeshVertexManifold2_VE, Vertex, Vertices )
_MESHFEATURE_IMPLEMENT_2( MeshVertexManifold2_VE, Edge, Edges )

