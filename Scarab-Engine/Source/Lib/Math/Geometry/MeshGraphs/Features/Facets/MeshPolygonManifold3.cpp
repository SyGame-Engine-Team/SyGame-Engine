/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Features/Facets/MeshPolygonManifold3.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Polygon features : 3-Manifold MeshGraphs
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
#include "MeshPolygonManifold3.h"

/////////////////////////////////////////////////////////////////////////////////
// MeshPolygonManifold3 implementation
MeshPolygonManifold3::MeshPolygonManifold3( const MeshPolygonKey & polygonKey ):
    MeshFacetPolygon( polygonKey )
{
    // nothing to do
}
MeshPolygonManifold3::~MeshPolygonManifold3()
{
    // nothing to do
}

//MeshFacet * MeshPolygonManifold3::Grow( const MeshFacetKey & newKey )
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
//                case MESH_LINKAGE_V:   pGrownFacet = New MeshPolygonManifold3_V( (const MeshPolygonKey &)newKey );
//                                       bUpdateVertices = true;                                            break;
//                case MESH_LINKAGE_E:   pGrownFacet = New MeshPolygonManifold3_E( (const MeshPolygonKey &)newKey );
//                                       bUpdateEdges = true;                                               break;
//                case MESH_LINKAGE_F:   pGrownFacet = New MeshPolygonManifold3_F( (const MeshPolygonKey &)newKey );
//                                       bUpdateFacets = true;                                              break;
//                case MESH_LINKAGE_VE:  pGrownFacet = New MeshPolygonManifold3_VE( (const MeshPolygonKey &)newKey );
//                                       bUpdateVertices = true; bUpdateEdges = true;                       break;
//                case MESH_LINKAGE_VF:  pGrownFacet = New MeshPolygonManifold3_VF( (const MeshPolygonKey &)newKey );
//                                       bUpdateVertices = true; bUpdateFacets = true;                      break;
//                case MESH_LINKAGE_EF:  pGrownFacet = New MeshPolygonManifold3_EF( (const MeshPolygonKey &)newKey );
//                                       bUpdateEdges = true;    bUpdateFacets = true;                      break;
//                case MESH_LINKAGE_VEF: pGrownFacet = New MeshPolygonManifold3_VEF( (const MeshPolygonKey &)newKey );
//                                       bUpdateVertices = true; bUpdateEdges = true; bUpdateFacets = true; break;
//            }
//            break;
//        default: Assert( false ); break;
//    }
//    ModelingFn->UnSelectMemory();
//
//    _UpdateAdjacency( pGrownFacet, bUpdateVertices ? Key.SIZE : 0,
//                                   bUpdateEdges ? Key.SIZE : 0,
//                                   bUpdateFacets ? Key.SIZE : 0 );
//
//    // End Grow
//    return pGrownFacet;
//}
//MeshFacet * MeshPolygonManifold3::Shrink( const MeshFacetKey & newKey )
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
//                case MESH_LINKAGE_V:   pShrunkFacet = New MeshTriangleManifold3_V( (const MeshTriangleKey &)newKey );
//                                       bUpdateVertices = true;                                            break;
//                case MESH_LINKAGE_E:   pShrunkFacet = New MeshTriangleManifold3_E( (const MeshTriangleKey &)newKey );
//                                       bUpdateEdges = true;                                               break;
//                case MESH_LINKAGE_F:   pShrunkFacet = New MeshTriangleManifold3_F( (const MeshTriangleKey &)newKey );
//                                       bUpdateFacets = true;                                              break;
//                case MESH_LINKAGE_VE:  pShrunkFacet = New MeshTriangleManifold3_VE( (const MeshTriangleKey &)newKey );
//                                       bUpdateVertices = true; bUpdateEdges = true;                       break;
//                case MESH_LINKAGE_VF:  pShrunkFacet = New MeshTriangleManifold3_VF( (const MeshTriangleKey &)newKey );
//                                       bUpdateVertices = true; bUpdateFacets = true;                      break;
//                case MESH_LINKAGE_EF:  pShrunkFacet = New MeshTriangleManifold3_EF( (const MeshTriangleKey &)newKey );
//                                       bUpdateEdges = true;    bUpdateFacets = true;                      break;
//                case MESH_LINKAGE_VEF: pShrunkFacet = New MeshTriangleManifold3_VEF( (const MeshTriangleKey &)newKey );
//                                       bUpdateVertices = true; bUpdateEdges = true; bUpdateFacets = true; break;
//            }
//            break;
//        case MESH_FACET_QUAD:
//            switch( GetLinkageType() ) {
//                case MESH_LINKAGE_V:   pShrunkFacet = New MeshQuadManifold3_V( (const MeshQuadKey &)newKey );
//                                       bUpdateVertices = true;                                            break;
//                case MESH_LINKAGE_E:   pShrunkFacet = New MeshQuadManifold3_E( (const MeshQuadKey &)newKey );
//                                       bUpdateEdges = true;                                               break;
//                case MESH_LINKAGE_F:   pShrunkFacet = New MeshQuadManifold3_F( (const MeshQuadKey &)newKey );
//                                       bUpdateFacets = true;                                              break;
//                case MESH_LINKAGE_VE:  pShrunkFacet = New MeshQuadManifold3_VE( (const MeshQuadKey &)newKey );
//                                       bUpdateVertices = true; bUpdateEdges = true;                       break;
//                case MESH_LINKAGE_VF:  pShrunkFacet = New MeshQuadManifold3_VF( (const MeshQuadKey &)newKey );
//                                       bUpdateVertices = true; bUpdateFacets = true;                      break;
//                case MESH_LINKAGE_EF:  pShrunkFacet = New MeshQuadManifold3_EF( (const MeshQuadKey &)newKey );
//                                       bUpdateEdges = true;    bUpdateFacets = true;                      break;
//                case MESH_LINKAGE_VEF: pShrunkFacet = New MeshQuadManifold3_VEF( (const MeshQuadKey &)newKey );
//                                       bUpdateVertices = true; bUpdateEdges = true; bUpdateFacets = true; break;
//            }
//            break;
//        case MESH_FACET_POLYGON:
//            switch( GetLinkageType() ) {
//                case MESH_LINKAGE_V:   pShrunkFacet = New MeshPolygonManifold3_V( (const MeshPolygonKey &)newKey );
//                                       bUpdateVertices = true;                                            break;
//                case MESH_LINKAGE_E:   pShrunkFacet = New MeshPolygonManifold3_E( (const MeshPolygonKey &)newKey );
//                                       bUpdateEdges = true;                                               break;
//                case MESH_LINKAGE_F:   pShrunkFacet = New MeshPolygonManifold3_F( (const MeshPolygonKey &)newKey );
//                                       bUpdateFacets = true;                                              break;
//                case MESH_LINKAGE_VE:  pShrunkFacet = New MeshPolygonManifold3_VE( (const MeshPolygonKey &)newKey );
//                                       bUpdateVertices = true; bUpdateEdges = true;                       break;
//                case MESH_LINKAGE_VF:  pShrunkFacet = New MeshPolygonManifold3_VF( (const MeshPolygonKey &)newKey );
//                                       bUpdateVertices = true; bUpdateFacets = true;                      break;
//                case MESH_LINKAGE_EF:  pShrunkFacet = New MeshPolygonManifold3_EF( (const MeshPolygonKey &)newKey );
//                                       bUpdateEdges = true;    bUpdateFacets = true;                      break;
//                case MESH_LINKAGE_VEF: pShrunkFacet = New MeshPolygonManifold3_VEF( (const MeshPolygonKey &)newKey );
//                                       bUpdateVertices = true; bUpdateEdges = true; bUpdateFacets = true; break;
//            }
//            break;
//        default: Assert( false ); break;
//    }
//    ModelingFn->UnSelectMemory();
//
//    _UpdateAdjacency( pShrunkFacet, bUpdateVertices ? newKey.GetFacetSize() : 0,
//                                    bUpdateEdges ? newKey.GetFacetSize() : 0,
//                                    bUpdateFacets ? newKey.GetFacetSize() : 0 );
//
//    // End Shrink
//    return pShrunkFacet;
//}

//MeshFacet * MeshPolygonManifold3::Upgrade()
//{
//    // Begin Upgrade
//    MeshFacet * pUpgradedFacet = NULL;
//    Bool bUpdateVertices = false;
//    Bool bUpdateEdges = false;
//    Bool bUpdateFacets = false;
//
//    ModelingFn->SelectMemory( TEXT("MeshGraphs") );
//    switch( GetLinkageType() ) {
//        case MESH_LINKAGE_V:   return NULL;
//        case MESH_LINKAGE_E:   return NULL;
//        case MESH_LINKAGE_F:   pUpgradedFacet = New MeshPolygonManifoldN_F( Key );   bUpdateFacets = true;                        break;
//        case MESH_LINKAGE_VE:  return NULL;
//        case MESH_LINKAGE_VF:  pUpgradedFacet = New MeshPolygonManifoldN_VF( Key );  bUpdateVertices = true; bUpdateFacets = true; break;
//        case MESH_LINKAGE_EF:  pUpgradedFacet = New MeshPolygonManifoldN_EF( Key );  bUpdateEdges = true;    bUpdateFacets = true; break;
//        case MESH_LINKAGE_VEF: pUpgradedFacet = New MeshPolygonManifoldN_VEF( Key );
//                                                                bUpdateVertices = true; bUpdateEdges = true;    bUpdateFacets = true; break;
//        default: Assert( false ); break;
//    }
//    ModelingFn->UnSelectMemory();
//
//    _UpdateAdjacency( pUpgradedFacet, bUpdateVertices ? Key.SIZE : 0,
//                                      bUpdateEdges ? Key.SIZE : 0,
//                                      bUpdateFacets ? Key.SIZE : 0 );
//
//    // End Upgrade
//    return pUpgradedFacet;
//}

/////////////////////////////////////////////////////////////////////////////////
// MeshPolygonManifold3_V implementation
MeshPolygonManifold3_V::MeshPolygonManifold3_V( const MeshPolygonKey & polygonKey ):
    MeshPolygonManifold3( polygonKey )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_ARRAY( Vertex, Vertices, Key.SIZE )
}
MeshPolygonManifold3_V::~MeshPolygonManifold3_V()
{
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_ARRAY( Vertices )
}

_MESHFEATURE_IMPLEMENT_ARRAY( MeshPolygonManifold3_V, Vertex, Vertices, Key.SIZE )

/////////////////////////////////////////////////////////////////////////////////
// MeshPolygonManifold3_E implementation
MeshPolygonManifold3_E::MeshPolygonManifold3_E( const MeshPolygonKey & polygonKey ):
    MeshPolygonManifold3( polygonKey )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_ARRAY( Edge, Edges, Key.SIZE )
}
MeshPolygonManifold3_E::~MeshPolygonManifold3_E()
{
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_ARRAY( Edges )
}

_MESHFEATURE_IMPLEMENT_ARRAY( MeshPolygonManifold3_E, Edge, Edges, Key.SIZE )

/////////////////////////////////////////////////////////////////////////////////
// MeshPolygonManifold3_F implementation
MeshPolygonManifold3_F::MeshPolygonManifold3_F( const MeshPolygonKey & polygonKey ):
    MeshPolygonManifold3( polygonKey )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_ARRAY( Facet, Facets, Key.SIZE )
}
MeshPolygonManifold3_F::~MeshPolygonManifold3_F()
{
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_ARRAY( Facets )
}

_MESHFEATURE_IMPLEMENT_ARRAY( MeshPolygonManifold3_F, Facet, Facets, Key.SIZE )

/////////////////////////////////////////////////////////////////////////////////
// MeshPolygonManifold3_VE implementation
MeshPolygonManifold3_VE::MeshPolygonManifold3_VE( const MeshPolygonKey & polygonKey ):
    MeshPolygonManifold3( polygonKey )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_ARRAY( Vertex, Vertices, Key.SIZE )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_ARRAY( Edge, Edges, Key.SIZE )
}
MeshPolygonManifold3_VE::~MeshPolygonManifold3_VE()
{
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_ARRAY( Vertices )
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_ARRAY( Edges )
}

_MESHFEATURE_IMPLEMENT_ARRAY( MeshPolygonManifold3_VE, Vertex, Vertices, Key.SIZE )
_MESHFEATURE_IMPLEMENT_ARRAY( MeshPolygonManifold3_VE, Edge, Edges, Key.SIZE )

/////////////////////////////////////////////////////////////////////////////////
// MeshPolygonManifold3_VF implementation
MeshPolygonManifold3_VF::MeshPolygonManifold3_VF( const MeshPolygonKey & polygonKey ):
    MeshPolygonManifold3( polygonKey )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_ARRAY( Vertex, Vertices, Key.SIZE )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_ARRAY( Facet, Facets, Key.SIZE )
}
MeshPolygonManifold3_VF::~MeshPolygonManifold3_VF()
{
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_ARRAY( Vertices )
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_ARRAY( Facets )
}

_MESHFEATURE_IMPLEMENT_ARRAY( MeshPolygonManifold3_VF, Vertex, Vertices, Key.SIZE )
_MESHFEATURE_IMPLEMENT_ARRAY( MeshPolygonManifold3_VF, Facet, Facets, Key.SIZE )

/////////////////////////////////////////////////////////////////////////////////
// MeshPolygonManifold3_EF implementation
MeshPolygonManifold3_EF::MeshPolygonManifold3_EF( const MeshPolygonKey & polygonKey ):
    MeshPolygonManifold3( polygonKey )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_ARRAY( Edge, Edges, Key.SIZE )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_ARRAY( Facet, Facets, Key.SIZE )
}
MeshPolygonManifold3_EF::~MeshPolygonManifold3_EF()
{
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_ARRAY( Edges )
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_ARRAY( Facets )
}

_MESHFEATURE_IMPLEMENT_ARRAY( MeshPolygonManifold3_EF, Edge, Edges, Key.SIZE )
_MESHFEATURE_IMPLEMENT_ARRAY( MeshPolygonManifold3_EF, Facet, Facets, Key.SIZE )

/////////////////////////////////////////////////////////////////////////////////
// MeshPolygonManifold3_VEF implementation
MeshPolygonManifold3_VEF::MeshPolygonManifold3_VEF( const MeshPolygonKey & polygonKey ):
    MeshPolygonManifold3( polygonKey )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_ARRAY( Vertex, Vertices, Key.SIZE )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_ARRAY( Edge, Edges, Key.SIZE )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_ARRAY( Facet, Facets, Key.SIZE )
}
MeshPolygonManifold3_VEF::~MeshPolygonManifold3_VEF()
{
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_ARRAY( Vertices )
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_ARRAY( Edges )
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_ARRAY( Facets )
}

_MESHFEATURE_IMPLEMENT_ARRAY( MeshPolygonManifold3_VEF, Vertex, Vertices, Key.SIZE )
_MESHFEATURE_IMPLEMENT_ARRAY( MeshPolygonManifold3_VEF, Edge, Edges, Key.SIZE )
_MESHFEATURE_IMPLEMENT_ARRAY( MeshPolygonManifold3_VEF, Facet, Facets, Key.SIZE )


