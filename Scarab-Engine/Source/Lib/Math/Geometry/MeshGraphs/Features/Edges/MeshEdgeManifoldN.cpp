/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Features/Edges/MeshEdgeManifoldN.cpp
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
// Includes
#include "MeshEdgeManifoldN.h"

/////////////////////////////////////////////////////////////////////////////////
// MeshEdgeManifoldN implementation
MeshEdgeManifoldN::MeshEdgeManifoldN( const MeshEdgeKey & edgeKey ):
    MeshEdge( edgeKey )
{
    // nothing to do
}
MeshEdgeManifoldN::~MeshEdgeManifoldN()
{
    // nothing to do
}

//MeshEdge * MeshEdgeManifoldN::Downgrade()
//{
//    // Begin Downgrade
//    MeshEdge * pDowngradedEdge = NULL;
//    Bool bUpdateVertices = false;
//    Bool bUpdateEdges = false;
//    Bool bUpdateFacets = false;
//
//    ModelingFn->SelectMemory( TEXT("MeshGraphs") );
//    switch( GetLinkageType() ) {
//        case MESH_LINKAGE_F:   pDowngradedEdge = New MeshEdgeManifold3_F( Key );  bUpdateFacets = true;                         break;
//        case MESH_LINKAGE_VF:  pDowngradedEdge = New MeshEdgeManifold3_VF( Key ); bUpdateVertices = true; bUpdateFacets = true; break;
//        case MESH_LINKAGE_EF:  pDowngradedEdge = New MeshEdgeManifold3_EF( Key ); bUpdateEdges = true;    bUpdateFacets = true; break;
//        case MESH_LINKAGE_VEF: pDowngradedEdge = New MeshEdgeManifold3_VEF( Key );
//                                                             bUpdateVertices = true; bUpdateEdges = true;    bUpdateFacets = true; break;
//        default: Assert( false ); break;
//    }
//    ModelingFn->UnSelectMemory();
//
//    _UpdateAdjacency( pDowngradedEdge, bUpdateVertices ? 2 : 0,
//                                       bUpdateEdges ? GetEdgeCount() : 0,
//                                       bUpdateFacets ? 2 : 0 );
//
//    // End Downgrade
//    return pDowngradedEdge;
//}

/////////////////////////////////////////////////////////////////////////////////
// MeshEdgeManifoldN_F implementation
MeshEdgeManifoldN_F::MeshEdgeManifoldN_F( const MeshEdgeKey & edgeKey ):
    MeshEdgeManifoldN( edgeKey ), Facets( 8 )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_DYNARRAY( Facets )
}
MeshEdgeManifoldN_F::~MeshEdgeManifoldN_F()
{
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_DYNARRAY( Facets )
}

_MESHFEATURE_IMPLEMENT_DYNARRAY( MeshEdgeManifoldN_F, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshEdgeManifoldN_VF implementation
MeshEdgeManifoldN_VF::MeshEdgeManifoldN_VF( const MeshEdgeKey & edgeKey ):
    MeshEdgeManifoldN( edgeKey ), Facets( 8 )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_2( Vertices )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_DYNARRAY( Facets )
}
MeshEdgeManifoldN_VF::~MeshEdgeManifoldN_VF()
{
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_DYNARRAY( Facets )
}

_MESHFEATURE_IMPLEMENT_2( MeshEdgeManifoldN_VF, Vertex, Vertices )
_MESHFEATURE_IMPLEMENT_DYNARRAY( MeshEdgeManifoldN_VF, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshEdgeManifoldN_EF implementation
MeshEdgeManifoldN_EF::MeshEdgeManifoldN_EF( const MeshEdgeKey & edgeKey ):
    MeshEdgeManifoldN( edgeKey ), Edges( 8 ), Facets( 8 )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_DYNARRAY( Edges )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_DYNARRAY( Facets )
}
MeshEdgeManifoldN_EF::~MeshEdgeManifoldN_EF()
{
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_DYNARRAY( Edges )
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_DYNARRAY( Facets )
}

_MESHFEATURE_IMPLEMENT_DYNARRAY( MeshEdgeManifoldN_EF, Edge, Edges )
_MESHFEATURE_IMPLEMENT_DYNARRAY( MeshEdgeManifoldN_EF, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshEdgeManifoldN_VEF implementation
MeshEdgeManifoldN_VEF::MeshEdgeManifoldN_VEF( const MeshEdgeKey & edgeKey ):
    MeshEdgeManifoldN( edgeKey ), Edges( 8 ), Facets( 8 )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_2( Vertices )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_DYNARRAY( Edges )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_DYNARRAY( Facets )
}
MeshEdgeManifoldN_VEF::~MeshEdgeManifoldN_VEF()
{
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_DYNARRAY( Edges )
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_DYNARRAY( Facets )
}

_MESHFEATURE_IMPLEMENT_2( MeshEdgeManifoldN_VEF, Vertex, Vertices )
_MESHFEATURE_IMPLEMENT_DYNARRAY( MeshEdgeManifoldN_VEF, Edge, Edges )
_MESHFEATURE_IMPLEMENT_DYNARRAY( MeshEdgeManifoldN_VEF, Facet, Facets )

