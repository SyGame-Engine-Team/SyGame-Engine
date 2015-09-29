/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Features/Facets/MeshPolygonManifoldN.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Polygon features : N-Manifold MeshGraphs
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
#include "MeshPolygonManifoldN.h"

/////////////////////////////////////////////////////////////////////////////////
// MeshPolygonManifoldN implementation
MeshPolygonManifoldN::MeshPolygonManifoldN( const MeshPolygonKey & polygonKey ):
    MeshFacetPolygon( polygonKey )
{
    // nothing to do
}
MeshPolygonManifoldN::~MeshPolygonManifoldN()
{
    // nothing to do
}

//MeshFacet * MeshPolygonManifoldN::Grow( const MeshFacetKey & newKey )
//{
//    Assert( newKey.GetFacetSize() > Key.SIZE );
//
//    // Begin Grow
//    MeshFacet * pGrownFacet = NULL;
//    Bool bUpdateVertices = false;
//    Bool bUpdateEdges = false;
//    Bool bUpdateFacets = false;
//
//    ModelingFn->SelectMemory( TEXT("MeshGraphs") );
//    switch( newKey.GetFacetType() ) {
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
//    _UpdateAdjacency( pGrownFacet, bUpdateVertices ? Key.SIZE : 0,
//                                   bUpdateEdges ? Key.SIZE : 0,
//                                   bUpdateFacets ? GetFacetCount() : 0 );
//
//    // End Grow
//    return pGrownFacet;
//}
//MeshFacet * MeshPolygonManifoldN::Shrink( const MeshFacetKey & newKey )
//{
//    Assert( newKey.GetFacetSize() < Key.SIZE );
//
//    // Begin Shrink
//    MeshFacet * pShrunkFacet = NULL;
//    Bool bUpdateVertices = false;
//    Bool bUpdateEdges = false;
//    Bool bUpdateFacets = false;
//
//    ModelingFn->SelectMemory( TEXT("MeshGraphs") );
//    switch( newKey.GetFacetType() ) {
//        case MESH_FACET_TRIANGLE:
//            switch( GetLinkageType() ) {
//                case MESH_LINKAGE_F:   pShrunkFacet = New MeshTriangleManifoldN_F( (const MeshTriangleKey &)newKey );
//                                       bUpdateFacets = true;                                              break;
//                case MESH_LINKAGE_VF:  pShrunkFacet = New MeshTriangleManifoldN_VF( (const MeshTriangleKey &)newKey );
//                                       bUpdateVertices = true; bUpdateFacets = true;                      break;
//                case MESH_LINKAGE_EF:  pShrunkFacet = New MeshTriangleManifoldN_EF( (const MeshTriangleKey &)newKey );
//                                       bUpdateEdges = true;    bUpdateFacets = true;                      break;
//                case MESH_LINKAGE_VEF: pShrunkFacet = New MeshTriangleManifoldN_VEF( (const MeshTriangleKey &)newKey );
//                                       bUpdateVertices = true; bUpdateEdges = true; bUpdateFacets = true; break;
//            }
//            break;
//        case MESH_FACET_QUAD:
//            switch( GetLinkageType() ) {
//                case MESH_LINKAGE_F:   pShrunkFacet = New MeshQuadManifoldN_F( (const MeshQuadKey &)newKey );
//                                       bUpdateFacets = true;                                              break;
//                case MESH_LINKAGE_VF:  pShrunkFacet = New MeshQuadManifoldN_VF( (const MeshQuadKey &)newKey );
//                                       bUpdateVertices = true; bUpdateFacets = true;                      break;
//                case MESH_LINKAGE_EF:  pShrunkFacet = New MeshQuadManifoldN_EF( (const MeshQuadKey &)newKey );
//                                       bUpdateEdges = true;    bUpdateFacets = true;                      break;
//                case MESH_LINKAGE_VEF: pShrunkFacet = New MeshQuadManifoldN_VEF( (const MeshQuadKey &)newKey );
//                                       bUpdateVertices = true; bUpdateEdges = true; bUpdateFacets = true; break;
//            }
//            break;
//        case MESH_FACET_POLYGON:
//            switch( GetLinkageType() ) {
//                case MESH_LINKAGE_F:   pShrunkFacet = New MeshPolygonManifoldN_F( (const MeshPolygonKey &)newKey );
//                                       bUpdateFacets = true;                                              break;
//                case MESH_LINKAGE_VF:  pShrunkFacet = New MeshPolygonManifoldN_VF( (const MeshPolygonKey &)newKey );
//                                       bUpdateVertices = true; bUpdateFacets = true;                      break;
//                case MESH_LINKAGE_EF:  pShrunkFacet = New MeshPolygonManifoldN_EF( (const MeshPolygonKey &)newKey );
//                                       bUpdateEdges = true;    bUpdateFacets = true;                      break;
//                case MESH_LINKAGE_VEF: pShrunkFacet = New MeshPolygonManifoldN_VEF( (const MeshPolygonKey &)newKey );
//                                       bUpdateVertices = true; bUpdateEdges = true; bUpdateFacets = true; break;
//            }
//            break;
//        default: Assert( false ); break;
//    }
//    ModelingFn->UnSelectMemory();
//
//    _UpdateAdjacency( pShrunkFacet, bUpdateVertices ? newKey.GetFacetSize() : 0,
//                                    bUpdateEdges ? newKey.GetFacetSize() : 0,
//                                    bUpdateFacets ? GetFacetCount() : 0 );
//
//    // End Shrink
//    return pShrunkFacet;
//}

//MeshFacet * MeshPolygonManifoldN::Downgrade()
//{
//    // Begin Downgrade
//    MeshFacet * pDowngradedFacet = NULL;
//    Bool bUpdateVertices = false;
//    Bool bUpdateEdges = false;
//    Bool bUpdateFacets = false;
//
//    ModelingFn->SelectMemory( TEXT("MeshGraphs") );
//    switch( GetLinkageType() ) {
//        case MESH_LINKAGE_F:   pDowngradedFacet = New MeshPolygonManifold3_F( Key );   bUpdateFacets = true;                         break;
//        case MESH_LINKAGE_VF:  pDowngradedFacet = New MeshPolygonManifold3_VF( Key );  bUpdateVertices = true; bUpdateFacets = true; break;
//        case MESH_LINKAGE_EF:  pDowngradedFacet = New MeshPolygonManifold3_EF( Key );  bUpdateEdges = true;    bUpdateFacets = true; break;
//        case MESH_LINKAGE_VEF: pDowngradedFacet = New MeshPolygonManifold3_VEF( Key );
//                                                                  bUpdateVertices = true; bUpdateEdges = true;    bUpdateFacets = true; break;
//        default: Assert( false ); break;
//    }
//    ModelingFn->UnSelectMemory();
//
//    _UpdateAdjacency( pDowngradedFacet, bUpdateVertices ? Key.SIZE : 0,
//                                        bUpdateEdges ? Key.SIZE : 0,
//                                        bUpdateFacets ? Key.SIZE : 0 );
//
//    // End Downgrade
//    return pDowngradedFacet;
//}

/////////////////////////////////////////////////////////////////////////////////
// MeshPolygonManifoldN_F implementation
MeshPolygonManifoldN_F::MeshPolygonManifoldN_F( const MeshPolygonKey & polygonKey ):
    MeshPolygonManifoldN( polygonKey )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_DYNARRAY( Facets )
}
MeshPolygonManifoldN_F::~MeshPolygonManifoldN_F()
{
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_DYNARRAY( Facets )
}

_MESHFEATURE_IMPLEMENT_DYNARRAY( MeshPolygonManifoldN_F, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshPolygonManifoldN_VF implementation
MeshPolygonManifoldN_VF::MeshPolygonManifoldN_VF( const MeshPolygonKey & polygonKey ):
    MeshPolygonManifoldN( polygonKey )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_ARRAY( Vertex, Vertices, Key.SIZE )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_DYNARRAY( Facets )
}
MeshPolygonManifoldN_VF::~MeshPolygonManifoldN_VF()
{
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_ARRAY( Vertices )
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_DYNARRAY( Facets )
}

_MESHFEATURE_IMPLEMENT_ARRAY( MeshPolygonManifoldN_VF, Vertex, Vertices, Key.SIZE )
_MESHFEATURE_IMPLEMENT_DYNARRAY( MeshPolygonManifoldN_VF, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshPolygonManifoldN_EF implementation
MeshPolygonManifoldN_EF::MeshPolygonManifoldN_EF( const MeshPolygonKey & polygonKey ):
    MeshPolygonManifoldN( polygonKey )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_ARRAY( Edge, Edges, Key.SIZE )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_DYNARRAY( Facets )
}
MeshPolygonManifoldN_EF::~MeshPolygonManifoldN_EF()
{
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_ARRAY( Edges )
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_DYNARRAY( Facets )
}

_MESHFEATURE_IMPLEMENT_ARRAY( MeshPolygonManifoldN_EF, Edge, Edges, Key.SIZE )
_MESHFEATURE_IMPLEMENT_DYNARRAY( MeshPolygonManifoldN_EF, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshPolygonManifoldN_VEF implementation
MeshPolygonManifoldN_VEF::MeshPolygonManifoldN_VEF( const MeshPolygonKey & polygonKey ):
    MeshPolygonManifoldN( polygonKey )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_ARRAY( Vertex, Vertices, Key.SIZE )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_ARRAY( Edge, Edges, Key.SIZE )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_DYNARRAY( Facets )
}
MeshPolygonManifoldN_VEF::~MeshPolygonManifoldN_VEF()
{
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_ARRAY( Vertices )
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_ARRAY( Edges )
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_DYNARRAY( Facets )
}

_MESHFEATURE_IMPLEMENT_ARRAY( MeshPolygonManifoldN_VEF, Vertex, Vertices, Key.SIZE )
_MESHFEATURE_IMPLEMENT_ARRAY( MeshPolygonManifoldN_VEF, Edge, Edges, Key.SIZE )
_MESHFEATURE_IMPLEMENT_DYNARRAY( MeshPolygonManifoldN_VEF, Facet, Facets )

