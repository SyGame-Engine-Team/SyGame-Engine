/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Features/Facets/MeshTriangleManifoldN.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Triangle features : N-Manifold MeshGraphs
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
#include "MeshTriangleManifoldN.h"

/////////////////////////////////////////////////////////////////////////////////
// MeshTriangleManifoldN implementation
MeshTriangleManifoldN::MeshTriangleManifoldN( const MeshTriangleKey & triangleKey ):
    MeshFacetTriangle( triangleKey )
{
    // nothing to do
}
MeshTriangleManifoldN::~MeshTriangleManifoldN()
{
    // nothing to do
}

//MeshFacet * MeshTriangleManifoldN::Grow( const MeshFacetKey & newKey )
//{
//    Assert( newKey.GetFacetSize() > 3 );
//
//    // Begin Grow
//    MeshFacet * pGrownFacet = NULL;
//    Bool bUpdateVertices = false;
//    Bool bUpdateEdges = false;
//    Bool bUpdateFacets = false;
//
//    ModelingFn->SelectMemory( TEXT("MeshGraphs") );
//    switch( newKey.GetFacetType() ) {
//        case MESH_FACET_QUAD:
//            switch( GetLinkageType() ) {
//                case MESH_LINKAGE_F:   pGrownFacet = New MeshQuadManifoldN_F( (const MeshQuadKey &)newKey );
//                                       bUpdateFacets = true;                                              break;
//                case MESH_LINKAGE_VF:  pGrownFacet = New MeshQuadManifoldN_VF( (const MeshQuadKey &)newKey );
//                                       bUpdateVertices = true; bUpdateFacets = true;                      break;
//                case MESH_LINKAGE_EF:  pGrownFacet = New MeshQuadManifoldN_EF( (const MeshQuadKey &)newKey );
//                                       bUpdateEdges = true;    bUpdateFacets = true;                      break;
//                case MESH_LINKAGE_VEF: pGrownFacet = New MeshQuadManifoldN_VEF( (const MeshQuadKey &)newKey );
//                                       bUpdateVertices = true; bUpdateEdges = true; bUpdateFacets = true; break;
//            }
//            break;
//        case MESH_FACET_POLYGON:
//            switch( GetLinkageType() ) {
//                case MESH_LINKAGE_F:   pGrownFacet = New MeshPolygonManifoldN_F( (const MeshPolygonKey &)newKey );
//                                       bUpdateFacets = true;                                              break;
//                case MESH_LINKAGE_VF:  pGrownFacet = New MeshPolygonManifoldN_VF( (const MeshPolygonKey &)newKey );
//                                       bUpdateVertices = true; bUpdateFacets = true;                      break;
//                case MESH_LINKAGE_EF:  pGrownFacet = New MeshPolygonManifoldN_EF( (const MeshPolygonKey &)newKey );
//                                       bUpdateEdges = true;    bUpdateFacets = true;                      break;
//                case MESH_LINKAGE_VEF: pGrownFacet = New MeshPolygonManifoldN_VEF( (const MeshPolygonKey &)newKey );
//                                       bUpdateVertices = true; bUpdateEdges = true; bUpdateFacets = true; break;
//            }
//            break;
//        default: Assert( false ); break;
//    }
//    ModelingFn->UnSelectMemory();
//
//    _UpdateAdjacency( pGrownFacet, bUpdateVertices ? 3 : 0,
//                                   bUpdateEdges ? 3 : 0,
//                                   bUpdateFacets ? GetFacetCount() : 0 );
//
//    // End Grow
//    return pGrownFacet;
//}

//MeshFacet * MeshTriangleManifoldN::Downgrade()
//{
//    // Begin Downgrade
//    MeshFacet * pDowngradedFacet = NULL;
//    Bool bUpdateVertices = false;
//    Bool bUpdateEdges = false;
//    Bool bUpdateFacets = false;
//
//    ModelingFn->SelectMemory( TEXT("MeshGraphs") );
//    switch( GetLinkageType() ) {
//        case MESH_LINKAGE_F:   pDowngradedFacet = New MeshTriangleManifold3_F( Key );   bUpdateFacets = true;                         break;
//        case MESH_LINKAGE_VF:  pDowngradedFacet = New MeshTriangleManifold3_VF( Key );  bUpdateVertices = true; bUpdateFacets = true; break;
//        case MESH_LINKAGE_EF:  pDowngradedFacet = New MeshTriangleManifold3_EF( Key );  bUpdateEdges = true;    bUpdateFacets = true; break;
//        case MESH_LINKAGE_VEF: pDowngradedFacet = New MeshTriangleManifold3_VEF( Key );
//                                                                   bUpdateVertices = true; bUpdateEdges = true;    bUpdateFacets = true; break;
//        default: Assert( false ); break;
//    }
//    ModelingFn->UnSelectMemory();
//
//    _UpdateAdjacency( pDowngradedFacet, bUpdateVertices ? 3 : 0,
//                                        bUpdateEdges ? 3 : 0,
//                                        bUpdateFacets ? 3 : 0 );
//
//    // End Downgrade
//    return pDowngradedFacet;
//}

/////////////////////////////////////////////////////////////////////////////////
// MeshTriangleManifoldN_F implementation
MeshTriangleManifoldN_F::MeshTriangleManifoldN_F( const MeshTriangleKey & triangleKey ):
    MeshTriangleManifoldN( triangleKey )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_DYNARRAY( Facets )
}
MeshTriangleManifoldN_F::~MeshTriangleManifoldN_F()
{
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_DYNARRAY( Facets )
}

_MESHFEATURE_IMPLEMENT_DYNARRAY( MeshTriangleManifoldN_F, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshTriangleManifoldN_VF implementation
MeshTriangleManifoldN_VF::MeshTriangleManifoldN_VF( const MeshTriangleKey & triangleKey ):
    MeshTriangleManifoldN( triangleKey )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_3( Vertices )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_DYNARRAY( Facets )
}
MeshTriangleManifoldN_VF::~MeshTriangleManifoldN_VF()
{
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_DYNARRAY( Facets )
}

_MESHFEATURE_IMPLEMENT_3( MeshTriangleManifoldN_VF, Vertex, Vertices )
_MESHFEATURE_IMPLEMENT_DYNARRAY( MeshTriangleManifoldN_VF, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshTriangleManifoldN_EF implementation
MeshTriangleManifoldN_EF::MeshTriangleManifoldN_EF( const MeshTriangleKey & triangleKey ):
    MeshTriangleManifoldN( triangleKey )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_3( Edges )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_DYNARRAY( Facets )
}
MeshTriangleManifoldN_EF::~MeshTriangleManifoldN_EF()
{
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_DYNARRAY( Facets )
}

_MESHFEATURE_IMPLEMENT_3( MeshTriangleManifoldN_EF, Edge, Edges )
_MESHFEATURE_IMPLEMENT_DYNARRAY( MeshTriangleManifoldN_EF, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshTriangleManifoldN_VEF implementation
MeshTriangleManifoldN_VEF::MeshTriangleManifoldN_VEF( const MeshTriangleKey & triangleKey ):
    MeshTriangleManifoldN( triangleKey )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_3( Vertices )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_3( Edges )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_DYNARRAY( Facets )
}
MeshTriangleManifoldN_VEF::~MeshTriangleManifoldN_VEF()
{
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_DYNARRAY( Facets )
}

_MESHFEATURE_IMPLEMENT_3( MeshTriangleManifoldN_VEF, Vertex, Vertices )
_MESHFEATURE_IMPLEMENT_3( MeshTriangleManifoldN_VEF, Edge, Edges )
_MESHFEATURE_IMPLEMENT_DYNARRAY( MeshTriangleManifoldN_VEF, Facet, Facets )

