/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Features/Vertices/MeshVertexManifoldN.cpp
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
// Includes
#include "MeshVertexManifoldN.h"

/////////////////////////////////////////////////////////////////////////////////
// MeshVertexManifoldN implementation
MeshVertexManifoldN::MeshVertexManifoldN( const MeshVertexKey & vertexKey ):
    MeshVertex( vertexKey )
{
    // nothing to do
}
MeshVertexManifoldN::~MeshVertexManifoldN()
{
    // nothing to do
}

//MeshVertex * MeshVertexManifoldN::Downgrade()
//{
//    // Begin Downgrade
//    MeshVertex * pDowngradedVertex = NULL;
//    Bool bUpdateVertices = false;
//    Bool bUpdateEdges = false;
//
//    ModelingFn->SelectMemory( TEXT("MeshGraphs") );
//    switch( GetLinkageType() ) {
//        case MESH_LINKAGE_V:   pDowngradedVertex = New MeshVertexManifold2_V( Key );  bUpdateVertices = true;                       break;
//        case MESH_LINKAGE_E:   pDowngradedVertex = New MeshVertexManifold2_E( Key );  bUpdateEdges = true;                          break;
//        case MESH_LINKAGE_F:   return NULL;
//        case MESH_LINKAGE_VE:  pDowngradedVertex = New MeshVertexManifold2_VE( Key ); bUpdateVertices = true; bUpdateEdges = true;  break;
//        case MESH_LINKAGE_VF:  pDowngradedVertex = New MeshVertexManifold2_V( Key );  bUpdateVertices = true;                       break;
//        case MESH_LINKAGE_EF:  pDowngradedVertex = New MeshVertexManifold2_E( Key );  bUpdateEdges = true;                          break;
//        case MESH_LINKAGE_VEF: pDowngradedVertex = New MeshVertexManifold2_VE( Key ); bUpdateVertices = true; bUpdateEdges = true;  break;
//        default: Assert( false ); break;
//    }
//    ModelingFn->UnSelectMemory();
//
//    _UpdateAdjacency( pDowngradedVertex, bUpdateVertices ? 2 : 0,
//                                         bUpdateEdges ? 2 : 0,
//                                         0 );
//
//    // End Downgrade
//    return pDowngradedVertex;
//}

/////////////////////////////////////////////////////////////////////////////////
// MeshVertexManifoldN_V implementation
MeshVertexManifoldN_V::MeshVertexManifoldN_V( const MeshVertexKey & vertexKey ):
    MeshVertexManifoldN( vertexKey ), Vertices( 8 )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_DYNARRAY( Vertices )
}
MeshVertexManifoldN_V::~MeshVertexManifoldN_V()
{
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_DYNARRAY( Vertices )
}

_MESHFEATURE_IMPLEMENT_DYNARRAY( MeshVertexManifoldN_V, Vertex, Vertices )

/////////////////////////////////////////////////////////////////////////////////
// MeshVertexManifoldN_E implementation
MeshVertexManifoldN_E::MeshVertexManifoldN_E( const MeshVertexKey & vertexKey ):
    MeshVertexManifoldN( vertexKey ), Edges( 8 )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_DYNARRAY( Edges )
}
MeshVertexManifoldN_E::~MeshVertexManifoldN_E()
{
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_DYNARRAY( Edges )
}

_MESHFEATURE_IMPLEMENT_DYNARRAY( MeshVertexManifoldN_E, Edge, Edges )

/////////////////////////////////////////////////////////////////////////////////
// MeshVertexManifoldN_F implementation
MeshVertexManifoldN_F::MeshVertexManifoldN_F( const MeshVertexKey & vertexKey ):
    MeshVertexManifoldN( vertexKey ), Facets( 8 )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_DYNARRAY( Facets )
}
MeshVertexManifoldN_F::~MeshVertexManifoldN_F()
{
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_DYNARRAY( Facets )
}

_MESHFEATURE_IMPLEMENT_DYNARRAY( MeshVertexManifoldN_F, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshVertexManifoldN_VE implementation
MeshVertexManifoldN_VE::MeshVertexManifoldN_VE( const MeshVertexKey & vertexKey ):
    MeshVertexManifoldN( vertexKey ), Vertices( 8 ), Edges( 8 )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_DYNARRAY( Vertices )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_DYNARRAY( Edges )
}
MeshVertexManifoldN_VE::~MeshVertexManifoldN_VE()
{
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_DYNARRAY( Vertices )
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_DYNARRAY( Edges )
}

_MESHFEATURE_IMPLEMENT_DYNARRAY( MeshVertexManifoldN_VE, Vertex, Vertices )
_MESHFEATURE_IMPLEMENT_DYNARRAY( MeshVertexManifoldN_VE, Edge, Edges )

/////////////////////////////////////////////////////////////////////////////////
// MeshVertexManifoldN_VF implementation
MeshVertexManifoldN_VF::MeshVertexManifoldN_VF( const MeshVertexKey & vertexKey ):
    MeshVertexManifoldN( vertexKey ), Vertices( 8 ), Facets( 8 )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_DYNARRAY( Vertices )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_DYNARRAY( Facets )
}
MeshVertexManifoldN_VF::~MeshVertexManifoldN_VF()
{
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_DYNARRAY( Vertices )
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_DYNARRAY( Facets )
}

_MESHFEATURE_IMPLEMENT_DYNARRAY( MeshVertexManifoldN_VF, Vertex, Vertices )
_MESHFEATURE_IMPLEMENT_DYNARRAY( MeshVertexManifoldN_VF, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshVertexManifoldN_EF implementation
MeshVertexManifoldN_EF::MeshVertexManifoldN_EF( const MeshVertexKey & vertexKey ):
    MeshVertexManifoldN( vertexKey ), Edges( 8 ), Facets( 8 )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_DYNARRAY( Edges )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_DYNARRAY( Facets )
}
MeshVertexManifoldN_EF::~MeshVertexManifoldN_EF()
{
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_DYNARRAY( Edges )
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_DYNARRAY( Facets )
}

_MESHFEATURE_IMPLEMENT_DYNARRAY( MeshVertexManifoldN_EF, Edge, Edges )
_MESHFEATURE_IMPLEMENT_DYNARRAY( MeshVertexManifoldN_EF, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshVertexManifoldN_VEF implementation
MeshVertexManifoldN_VEF::MeshVertexManifoldN_VEF( const MeshVertexKey & vertexKey ):
    MeshVertexManifoldN( vertexKey ), Vertices( 8 ), Edges( 8 ), Facets( 8 )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_DYNARRAY( Vertices )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_DYNARRAY( Edges )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_DYNARRAY( Facets )
}
MeshVertexManifoldN_VEF::~MeshVertexManifoldN_VEF()
{
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_DYNARRAY( Vertices )
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_DYNARRAY( Edges )
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_DYNARRAY( Facets )
}

_MESHFEATURE_IMPLEMENT_DYNARRAY( MeshVertexManifoldN_VEF, Vertex, Vertices )
_MESHFEATURE_IMPLEMENT_DYNARRAY( MeshVertexManifoldN_VEF, Edge, Edges )
_MESHFEATURE_IMPLEMENT_DYNARRAY( MeshVertexManifoldN_VEF, Facet, Facets )


