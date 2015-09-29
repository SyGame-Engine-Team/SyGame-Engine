/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Features/Facets/MeshQuadManifold3.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Quad features : 3-Manifold MeshGraphs
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
#include "MeshQuadManifold3.h"

/////////////////////////////////////////////////////////////////////////////////
// MeshQuadManifold3 implementation
MeshQuadManifold3::MeshQuadManifold3( const MeshQuadKey & quadKey ):
    MeshFacetQuad( quadKey )
{
    // nothing to do
}
MeshQuadManifold3::~MeshQuadManifold3()
{
    // nothing to do
}

//MeshFacet * MeshQuadManifold3::Grow( const MeshFacetKey & newKey )
//{
//    Assert( newKey.GetFacetSize() > 4 );
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
//    _UpdateAdjacency( pGrownFacet, bUpdateVertices ? 4 : 0,
//                                   bUpdateEdges ? 4 : 0,
//                                   bUpdateFacets ? 4 : 0 );
//
//    // End Grow
//    return pGrownFacet;
//}
//MeshFacet * MeshQuadManifold3::Shrink( const MeshFacetKey & newKey )
//{
//    Assert( newKey.GetFacetSize() < 4 );
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
//        default: Assert( false ); break;
//    }
//    ModelingFn->UnSelectMemory();
//
//    _UpdateAdjacency( pShrunkFacet, bUpdateVertices ? 3 : 0,
//                                    bUpdateEdges ? 3 : 0,
//                                    bUpdateFacets ? 3 : 0 );
//
//    // End Shrink
//    return pShrunkFacet;
//}

//MeshFacet * MeshQuadManifold3::Upgrade()
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
//        case MESH_LINKAGE_F:   pUpgradedFacet = New MeshQuadManifoldN_F( Key );   bUpdateFacets = true;                         break;
//        case MESH_LINKAGE_VE:  return NULL;
//        case MESH_LINKAGE_VF:  pUpgradedFacet = New MeshQuadManifoldN_VF( Key );  bUpdateVertices = true; bUpdateFacets = true; break;
//        case MESH_LINKAGE_EF:  pUpgradedFacet = New MeshQuadManifoldN_EF( Key );  bUpdateEdges = true;    bUpdateFacets = true; break;
//        case MESH_LINKAGE_VEF: pUpgradedFacet = New MeshQuadManifoldN_VEF( Key );
//                                                             bUpdateVertices = true; bUpdateEdges = true;    bUpdateFacets = true; break;
//        default: Assert( false ); break;
//    }
//    ModelingFn->UnSelectMemory();
//
//    _UpdateAdjacency( pUpgradedFacet, bUpdateVertices ? 4 : 0,
//                                      bUpdateEdges ? 4 : 0,
//                                      bUpdateFacets ? 4 : 0 );
//
//    // End Upgrade
//    return pUpgradedFacet;
//}

/////////////////////////////////////////////////////////////////////////////////
// MeshQuadManifold3_V implementation
MeshQuadManifold3_V::MeshQuadManifold3_V( const MeshQuadKey & quadKey ):
    MeshQuadManifold3( quadKey )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_4( Vertices )
}
MeshQuadManifold3_V::~MeshQuadManifold3_V()
{
    // nothing to do
}

_MESHFEATURE_IMPLEMENT_4( MeshQuadManifold3_V, Vertex, Vertices )

/////////////////////////////////////////////////////////////////////////////////
// MeshQuadManifold3_E implementation
MeshQuadManifold3_E::MeshQuadManifold3_E( const MeshQuadKey & quadKey ):
    MeshQuadManifold3( quadKey )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_4( Edges )
}
MeshQuadManifold3_E::~MeshQuadManifold3_E()
{
    // nothing to do
}

_MESHFEATURE_IMPLEMENT_4( MeshQuadManifold3_E, Edge, Edges )

/////////////////////////////////////////////////////////////////////////////////
// MeshQuadManifold3_F implementation
MeshQuadManifold3_F::MeshQuadManifold3_F( const MeshQuadKey & quadKey ):
    MeshQuadManifold3( quadKey )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_4( Facets )
}
MeshQuadManifold3_F::~MeshQuadManifold3_F()
{
    // nothing to do
}

_MESHFEATURE_IMPLEMENT_4( MeshQuadManifold3_F, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshQuadManifold3_VE implementation
MeshQuadManifold3_VE::MeshQuadManifold3_VE( const MeshQuadKey & quadKey ):
    MeshQuadManifold3( quadKey )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_4( Vertices )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_4( Edges )
}
MeshQuadManifold3_VE::~MeshQuadManifold3_VE()
{
    // nothing to do
}

_MESHFEATURE_IMPLEMENT_4( MeshQuadManifold3_VE, Vertex, Vertices )
_MESHFEATURE_IMPLEMENT_4( MeshQuadManifold3_VE, Edge, Edges )

/////////////////////////////////////////////////////////////////////////////////
// MeshQuadManifold3_VF implementation
MeshQuadManifold3_VF::MeshQuadManifold3_VF( const MeshQuadKey & quadKey ):
    MeshQuadManifold3( quadKey )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_4( Vertices )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_4( Facets )
}
MeshQuadManifold3_VF::~MeshQuadManifold3_VF()
{
    // nothing to do
}

_MESHFEATURE_IMPLEMENT_4( MeshQuadManifold3_VF, Vertex, Vertices )
_MESHFEATURE_IMPLEMENT_4( MeshQuadManifold3_VF, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshQuadManifold3_EF implementation
MeshQuadManifold3_EF::MeshQuadManifold3_EF( const MeshQuadKey & quadKey ):
    MeshQuadManifold3( quadKey )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_4( Edges )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_4( Facets )
}
MeshQuadManifold3_EF::~MeshQuadManifold3_EF()
{
    // nothing to do
}

_MESHFEATURE_IMPLEMENT_4( MeshQuadManifold3_EF, Edge, Edges )
_MESHFEATURE_IMPLEMENT_4( MeshQuadManifold3_EF, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshQuadManifold3_VEF implementation
MeshQuadManifold3_VEF::MeshQuadManifold3_VEF( const MeshQuadKey & quadKey ):
    MeshQuadManifold3( quadKey )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_4( Vertices )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_4( Edges )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_4( Facets )
}
MeshQuadManifold3_VEF::~MeshQuadManifold3_VEF()
{
    // nothing to do
}

_MESHFEATURE_IMPLEMENT_4( MeshQuadManifold3_VEF, Vertex, Vertices )
_MESHFEATURE_IMPLEMENT_4( MeshQuadManifold3_VEF, Edge, Edges )
_MESHFEATURE_IMPLEMENT_4( MeshQuadManifold3_VEF, Facet, Facets )


