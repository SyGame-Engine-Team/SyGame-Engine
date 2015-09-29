/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Features/Facets/MeshQuadManifoldN.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Quad features : N-Manifold MeshGraphs
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
#include "MeshQuadManifoldN.h"

/////////////////////////////////////////////////////////////////////////////////
// MeshQuadManifoldN implementation
MeshQuadManifoldN::MeshQuadManifoldN( const MeshQuadKey & quadKey ):
    MeshFacetQuad( quadKey )
{
    // nothing to do
}
MeshQuadManifoldN::~MeshQuadManifoldN()
{
    // nothing to do
}

//MeshFacet * MeshQuadManifoldN::Grow( const MeshFacetKey & newKey )
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
//    _UpdateAdjacency( pGrownFacet, bUpdateVertices ? 4 : 0,
//                                   bUpdateEdges ? 4 : 0,
//                                   bUpdateFacets ? GetFacetCount() : 0 );
//
//    // End Grow
//    return pGrownFacet;
//}
//MeshFacet * MeshQuadManifoldN::Shrink( const MeshFacetKey & newKey )
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
//        default: Assert( false ); break;
//    }
//    ModelingFn->UnSelectMemory();
//
//    _UpdateAdjacency( pShrunkFacet, bUpdateVertices ? 3 : 0,
//                                    bUpdateEdges ? 3 : 0,
//                                    bUpdateFacets ? GetFacetCount() : 0 );
//
//    // End Shrink
//    return pShrunkFacet;
//}

//MeshFacet * MeshQuadManifoldN::Downgrade()
//{
//    // Begin Downgrade
//    MeshFacet * pDowngradedFacet = NULL;
//    Bool bUpdateVertices = false;
//    Bool bUpdateEdges = false;
//    Bool bUpdateFacets = false;
//
//    ModelingFn->SelectMemory( TEXT("MeshGraphs") );
//    switch( GetLinkageType() ) {
//        case MESH_LINKAGE_F:   pDowngradedFacet = New MeshQuadManifold3_F( Key );   bUpdateFacets = true;                         break;
//        case MESH_LINKAGE_VF:  pDowngradedFacet = New MeshQuadManifold3_VF( Key );  bUpdateVertices = true; bUpdateFacets = true; break;
//        case MESH_LINKAGE_EF:  pDowngradedFacet = New MeshQuadManifold3_EF( Key );  bUpdateEdges = true;    bUpdateFacets = true; break;
//        case MESH_LINKAGE_VEF: pDowngradedFacet = New MeshQuadManifold3_VEF( Key );
//                                                               bUpdateVertices = true; bUpdateEdges = true;    bUpdateFacets = true; break;
//        default: Assert( false ); break;
//    }
//    ModelingFn->UnSelectMemory();
//
//    _UpdateAdjacency( pDowngradedFacet, bUpdateVertices ? 4 : 0,
//                                        bUpdateEdges ? 4 : 0,
//                                        bUpdateFacets ? 4 : 0 );
//
//    // End Downgrade
//    return pDowngradedFacet;
//}

/////////////////////////////////////////////////////////////////////////////////
// MeshQuadManifoldN_F implementation
MeshQuadManifoldN_F::MeshQuadManifoldN_F( const MeshQuadKey & quadKey ):
    MeshQuadManifoldN( quadKey )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_DYNARRAY( Facets )
}
MeshQuadManifoldN_F::~MeshQuadManifoldN_F()
{
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_DYNARRAY( Facets )
}

_MESHFEATURE_IMPLEMENT_DYNARRAY( MeshQuadManifoldN_F, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshQuadManifoldN_VF implementation
MeshQuadManifoldN_VF::MeshQuadManifoldN_VF( const MeshQuadKey & quadKey ):
    MeshQuadManifoldN( quadKey )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_4( Vertices )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_DYNARRAY( Facets )
}
MeshQuadManifoldN_VF::~MeshQuadManifoldN_VF()
{
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_DYNARRAY( Facets )
}

_MESHFEATURE_IMPLEMENT_4( MeshQuadManifoldN_VF, Vertex, Vertices )
_MESHFEATURE_IMPLEMENT_DYNARRAY( MeshQuadManifoldN_VF, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshQuadManifoldN_EF implementation
MeshQuadManifoldN_EF::MeshQuadManifoldN_EF( const MeshQuadKey & quadKey ):
    MeshQuadManifoldN( quadKey )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_4( Edges )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_DYNARRAY( Facets )
}
MeshQuadManifoldN_EF::~MeshQuadManifoldN_EF()
{
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_DYNARRAY( Facets )
}

_MESHFEATURE_IMPLEMENT_4( MeshQuadManifoldN_EF, Edge, Edges )
_MESHFEATURE_IMPLEMENT_DYNARRAY( MeshQuadManifoldN_EF, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshQuadManifoldN_VEF implementation
MeshQuadManifoldN_VEF::MeshQuadManifoldN_VEF( const MeshQuadKey & quadKey ):
    MeshQuadManifoldN( quadKey )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_4( Vertices )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_4( Edges )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_DYNARRAY( Facets )
}
MeshQuadManifoldN_VEF::~MeshQuadManifoldN_VEF()
{
    _MESHFEATURE_IMPLEMENT_DESTRUCTOR_DYNARRAY( Facets )
}

_MESHFEATURE_IMPLEMENT_4( MeshQuadManifoldN_VEF, Vertex, Vertices )
_MESHFEATURE_IMPLEMENT_4( MeshQuadManifoldN_VEF, Edge, Edges )
_MESHFEATURE_IMPLEMENT_DYNARRAY( MeshQuadManifoldN_VEF, Facet, Facets )

