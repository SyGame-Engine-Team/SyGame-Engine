/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Features/Edges/MeshEdgeManifold2.cpp
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
// Includes
#include "MeshEdgeManifold2.h"

/////////////////////////////////////////////////////////////////////////////////
// MeshEdgeManifold2 implementation
MeshEdgeManifold2::MeshEdgeManifold2( const MeshEdgeKey & edgeKey ):
    MeshEdge( edgeKey )
{
    // nothing to do
}
MeshEdgeManifold2::~MeshEdgeManifold2()
{
    // nothing to do
}

//MeshEdge * MeshEdgeManifold2::Upgrade()
//{
//    // Begin Upgrade
//    MeshEdge * pUpgradedEdge = NULL;
//    Bool bUpdateVertices = false;
//    Bool bUpdateEdges = false;
//
//    ModelingFn->SelectMemory( TEXT("MeshGraphs") );
//    switch( GetLinkageType() ) {
//        case MESH_LINKAGE_V:  return NULL;
//        case MESH_LINKAGE_E:  pUpgradedEdge = New MeshEdgeManifold3_E( Key );  bUpdateEdges = true;                         break;
//        case MESH_LINKAGE_VE: pUpgradedEdge = New MeshEdgeManifold3_VE( Key ); bUpdateVertices = true; bUpdateEdges = true; break;
//        default: Assert( false ); break;
//    }
//    ModelingFn->UnSelectMemory();
//
//    _UpdateAdjacency( pUpgradedEdge, bUpdateVertices ? 2 : 0,
//                                     bUpdateEdges ? 2 : 0,
//                                     0 );
//
//    // End Upgrade
//    return pUpgradedEdge;
//}

/////////////////////////////////////////////////////////////////////////////////
// MeshEdgeManifold2_V implementation
MeshEdgeManifold2_V::MeshEdgeManifold2_V( const MeshEdgeKey & edgeKey ):
    MeshEdgeManifold2( edgeKey )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_2( Vertices )
}
MeshEdgeManifold2_V::~MeshEdgeManifold2_V()
{
    // nothing to do
}

_MESHFEATURE_IMPLEMENT_2( MeshEdgeManifold2_V, Vertex, Vertices )

/////////////////////////////////////////////////////////////////////////////////
// MeshEdgeManifold2_E implementation
MeshEdgeManifold2_E::MeshEdgeManifold2_E( const MeshEdgeKey & edgeKey ):
    MeshEdgeManifold2( edgeKey )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_2( Edges )
}
MeshEdgeManifold2_E::~MeshEdgeManifold2_E()
{
    // nothing to do
}

_MESHFEATURE_IMPLEMENT_2( MeshEdgeManifold2_E, Edge, Edges )

/////////////////////////////////////////////////////////////////////////////////
// MeshEdgeManifold2_VE implementation
MeshEdgeManifold2_VE::MeshEdgeManifold2_VE( const MeshEdgeKey & edgeKey ):
    MeshEdgeManifold2( edgeKey )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_2( Vertices )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_2( Edges )
}
MeshEdgeManifold2_VE::~MeshEdgeManifold2_VE()
{
    // nothing to do
}

_MESHFEATURE_IMPLEMENT_2( MeshEdgeManifold2_VE, Vertex, Vertices )
_MESHFEATURE_IMPLEMENT_2( MeshEdgeManifold2_VE, Edge, Edges )

