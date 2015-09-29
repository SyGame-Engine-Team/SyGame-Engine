/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Features/Edges/MeshEdgeManifold3.cpp
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
// Includes
#include "MeshEdgeManifold3.h"

/////////////////////////////////////////////////////////////////////////////////
// MeshEdgeManifold3 implementation
MeshEdgeManifold3::MeshEdgeManifold3( const MeshEdgeKey & edgeKey ):
    MeshEdge( edgeKey )
{
    // nothing to do
}
MeshEdgeManifold3::~MeshEdgeManifold3()
{
    // nothing to do
}

//MeshEdge * MeshEdgeManifold3::Upgrade()
//{
//    // Begin Upgrade
//    MeshEdge * pUpgradedEdge = NULL;
//    Bool bUpdateVertices = false;
//    Bool bUpdateEdges = false;
//    Bool bUpdateFacets = false;
//
//    ModelingFn->SelectMemory( TEXT("MeshGraphs") );
//    switch( GetLinkageType() ) {
//        case MESH_LINKAGE_E:   return NULL;
//        case MESH_LINKAGE_F:   pUpgradedEdge = New MeshEdgeManifoldN_F( Key );  bUpdateFacets = true;                         break;
//        case MESH_LINKAGE_VE:  return NULL;
//        case MESH_LINKAGE_VF:  pUpgradedEdge = New MeshEdgeManifoldN_VF( Key ); bUpdateVertices = true; bUpdateFacets = true; break;
//        case MESH_LINKAGE_EF:  pUpgradedEdge = New MeshEdgeManifoldN_EF( Key ); bUpdateEdges = true;    bUpdateFacets = true; break;
//        case MESH_LINKAGE_VEF: pUpgradedEdge = New MeshEdgeManifoldN_VEF( Key );
//                                                           bUpdateVertices = true; bUpdateEdges = true;    bUpdateFacets = true; break;
//        default: Assert( false ); break;
//    }
//    ModelingFn->UnSelectMemory();
//
//    _UpdateAdjacency( pUpgradedEdge, bUpdateVertices ? 2 : 0,
//                                     bUpdateEdges ? GetEdgeCount() : 0,
//                                     bUpdateFacets ? 2 : 0 );
//
//    // End Upgrade
//    return pUpgradedEdge;
//}
//MeshEdge * MeshEdgeManifold3::Downgrade()
//{
//    // Begin Downgrade
//    MeshEdge * pDowngradedEdge = NULL;
//    Bool bUpdateVertices = false;
//    Bool bUpdateEdges = false;
//
//    ModelingFn->SelectMemory( TEXT("MeshGraphs") );
//    switch( GetLinkageType() ) {
//        case MESH_LINKAGE_E:   pDowngradedEdge = New MeshEdgeManifold2_E( Key );  bUpdateEdges = true;                         break;
//        case MESH_LINKAGE_F:   return NULL;
//        case MESH_LINKAGE_VE:  pDowngradedEdge = New MeshEdgeManifold2_VE( Key ); bUpdateVertices = true; bUpdateEdges = true; break;
//        case MESH_LINKAGE_VF:  pDowngradedEdge = New MeshEdgeManifold2_V( Key );  bUpdateVertices = true;                      break;
//        case MESH_LINKAGE_EF:  pDowngradedEdge = New MeshEdgeManifold2_E( Key );  bUpdateEdges = true;                         break;
//        case MESH_LINKAGE_VEF: pDowngradedEdge = New MeshEdgeManifold2_VE( Key ); bUpdateVertices = true; bUpdateEdges = true; break;
//        default: Assert( false ); break;
//    }
//    ModelingFn->UnSelectMemory();
//
//    _UpdateAdjacency( pDowngradedEdge, bUpdateVertices ? 2 : 0,
//                                       bUpdateEdges ? 2 : 0,
//                                       0 );
//
//    // End Downgrade
//    return pDowngradedEdge;
//}

/////////////////////////////////////////////////////////////////////////////////
// MeshEdgeManifold3_E implementation
MeshEdgeManifold3_E::MeshEdgeManifold3_E( const MeshEdgeKey & edgeKey ):
    MeshEdgeManifold3( edgeKey ), Edges( 8 )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_DYNARRAY( Edges )
}
MeshEdgeManifold3_E::~MeshEdgeManifold3_E()
{
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_DYNARRAY( Edges )
}

_MESHFEATURE_IMPLEMENT_DYNARRAY( MeshEdgeManifold3_E, Edge, Edges )

/////////////////////////////////////////////////////////////////////////////////
// MeshEdgeManifold3_F implementation
MeshEdgeManifold3_F::MeshEdgeManifold3_F( const MeshEdgeKey & edgeKey ):
    MeshEdgeManifold3( edgeKey )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_2( Facets )
}
MeshEdgeManifold3_F::~MeshEdgeManifold3_F()
{
    // nothing to do
}

_MESHFEATURE_IMPLEMENT_2( MeshEdgeManifold3_F, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshEdgeManifold3_VE implementation
MeshEdgeManifold3_VE::MeshEdgeManifold3_VE( const MeshEdgeKey & edgeKey ):
    MeshEdgeManifold3( edgeKey ), Edges( 8 )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_2( Vertices )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_DYNARRAY( Edges )
}
MeshEdgeManifold3_VE::~MeshEdgeManifold3_VE()
{
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_DYNARRAY( Edges )
}

_MESHFEATURE_IMPLEMENT_2( MeshEdgeManifold3_VE, Vertex, Vertices )
_MESHFEATURE_IMPLEMENT_DYNARRAY( MeshEdgeManifold3_VE, Edge, Edges )

/////////////////////////////////////////////////////////////////////////////////
// MeshEdgeManifold3_VF implementation
MeshEdgeManifold3_VF::MeshEdgeManifold3_VF( const MeshEdgeKey & edgeKey ):
    MeshEdgeManifold3( edgeKey )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_2( Vertices )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_2( Facets )
}
MeshEdgeManifold3_VF::~MeshEdgeManifold3_VF()
{
    // nothing to do
}

_MESHFEATURE_IMPLEMENT_2( MeshEdgeManifold3_VF, Vertex, Vertices )
_MESHFEATURE_IMPLEMENT_2( MeshEdgeManifold3_VF, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshEdgeManifold3_EF implementation
MeshEdgeManifold3_EF::MeshEdgeManifold3_EF( const MeshEdgeKey & edgeKey ):
    MeshEdgeManifold3( edgeKey ), Edges( 8 )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_DYNARRAY( Edges )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_2( Facets )
}
MeshEdgeManifold3_EF::~MeshEdgeManifold3_EF()
{
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_DYNARRAY( Edges )
}

_MESHFEATURE_IMPLEMENT_DYNARRAY( MeshEdgeManifold3_EF, Edge, Edges )
_MESHFEATURE_IMPLEMENT_2( MeshEdgeManifold3_EF, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshEdgeManifold3_VEF implementation
MeshEdgeManifold3_VEF::MeshEdgeManifold3_VEF( const MeshEdgeKey & edgeKey ):
    MeshEdgeManifold3( edgeKey ), Edges( 8 )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_2( Vertices )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_DYNARRAY( Edges )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_2( Facets )
}
MeshEdgeManifold3_VEF::~MeshEdgeManifold3_VEF()
{
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_DYNARRAY( Edges )
}

_MESHFEATURE_IMPLEMENT_2( MeshEdgeManifold3_VEF, Vertex, Vertices )
_MESHFEATURE_IMPLEMENT_DYNARRAY( MeshEdgeManifold3_VEF, Edge, Edges )
_MESHFEATURE_IMPLEMENT_2( MeshEdgeManifold3_VEF, Facet, Facets )


