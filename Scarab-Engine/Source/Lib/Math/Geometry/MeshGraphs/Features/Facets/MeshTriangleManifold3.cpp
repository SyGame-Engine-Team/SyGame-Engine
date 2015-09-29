/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Features/Facets/MeshTriangleManifold3.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Triangle features : 3-Manifold MeshGraphs
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
#include "MeshTriangleManifold3.h"

/////////////////////////////////////////////////////////////////////////////////
// MeshTriangleManifold3 implementation
MeshTriangleManifold3::MeshTriangleManifold3( const MeshTriangleKey & triangleKey ):
    MeshFacetTriangle( triangleKey )
{
    // nothing to do
}
MeshTriangleManifold3::~MeshTriangleManifold3()
{
    // nothing to do
}

//MeshFacet * MeshTriangleManifold3::Grow( const MeshFacetKey & newKey )
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
//                case MESH_LINKAGE_V:   pGrownFacet = New MeshQuadManifold3_V( (const MeshQuadKey &)newKey );
//                                       bUpdateVertices = true;                                            break;
//                case MESH_LINKAGE_E:   pGrownFacet = New MeshQuadManifold3_E( (const MeshQuadKey &)newKey );
//                                       bUpdateEdges = true;                                               break;
//                case MESH_LINKAGE_F:   pGrownFacet = New MeshQuadManifold3_F( (const MeshQuadKey &)newKey );
//                                       bUpdateFacets = true;                                              break;
//                case MESH_LINKAGE_VE:  pGrownFacet = New MeshQuadManifold3_VE( (const MeshQuadKey &)newKey );
//                                       bUpdateVertices = true; bUpdateEdges = true;                       break;
//                case MESH_LINKAGE_VF:  pGrownFacet = New MeshQuadManifold3_VF( (const MeshQuadKey &)newKey );
//                                       bUpdateVertices = true; bUpdateFacets = true;                      break;
//                case MESH_LINKAGE_EF:  pGrownFacet = New MeshQuadManifold3_EF( (const MeshQuadKey &)newKey );
//                                       bUpdateEdges = true;    bUpdateFacets = true;                      break;
//                case MESH_LINKAGE_VEF: pGrownFacet = New MeshQuadManifold3_VEF( (const MeshQuadKey &)newKey );
//                                       bUpdateVertices = true; bUpdateEdges = true; bUpdateFacets = true; break;
//            }
//            break;
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
//    _UpdateAdjacency( pGrownFacet, bUpdateVertices ? 3 : 0,
//                                   bUpdateEdges ? 3 : 0,
//                                   bUpdateFacets ? 3 : 0 );
//
//    // End Grow
//    return pGrownFacet;
//}

//MeshFacet * MeshTriangleManifold3::Upgrade()
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
//        case MESH_LINKAGE_F:   pUpgradedFacet = New MeshTriangleManifoldN_F( Key );   bUpdateFacets = true;                        break;
//        case MESH_LINKAGE_VE:  return NULL;
//        case MESH_LINKAGE_VF:  pUpgradedFacet = New MeshTriangleManifoldN_VF( Key );  bUpdateVertices = true; bUpdateFacets = true; break;
//        case MESH_LINKAGE_EF:  pUpgradedFacet = New MeshTriangleManifoldN_EF( Key );  bUpdateEdges = true;    bUpdateFacets = true; break;
//        case MESH_LINKAGE_VEF: pUpgradedFacet = New MeshTriangleManifoldN_VEF( Key );
//                                                                 bUpdateVertices = true; bUpdateEdges = true;    bUpdateFacets = true; break;
//        default: Assert( false ); break;
//    }
//    ModelingFn->UnSelectMemory();
//
//    _UpdateAdjacency( pUpgradedFacet, bUpdateVertices ? 3 : 0,
//                                      bUpdateEdges ? 3 : 0,
//                                      bUpdateFacets ? 3 : 0 );
//
//    // End Upgrade
//    return pUpgradedFacet;
//}

/////////////////////////////////////////////////////////////////////////////////
// MeshTriangleManifold3_V implementation
MeshTriangleManifold3_V::MeshTriangleManifold3_V( const MeshTriangleKey & triangleKey ):
    MeshTriangleManifold3( triangleKey )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_3( Vertices )
}
MeshTriangleManifold3_V::~MeshTriangleManifold3_V()
{
    // nothing to do
}

_MESHFEATURE_IMPLEMENT_3( MeshTriangleManifold3_V, Vertex, Vertices )

/////////////////////////////////////////////////////////////////////////////////
// MeshTriangleManifold3_E implementation
MeshTriangleManifold3_E::MeshTriangleManifold3_E( const MeshTriangleKey & triangleKey ):
    MeshTriangleManifold3( triangleKey )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_3( Edges )
}
MeshTriangleManifold3_E::~MeshTriangleManifold3_E()
{
    // nothing to do
}

_MESHFEATURE_IMPLEMENT_3( MeshTriangleManifold3_E, Edge, Edges )

/////////////////////////////////////////////////////////////////////////////////
// MeshTriangleManifold3_F implementation
MeshTriangleManifold3_F::MeshTriangleManifold3_F( const MeshTriangleKey & triangleKey ):
    MeshTriangleManifold3( triangleKey )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_3( Facets )
}
MeshTriangleManifold3_F::~MeshTriangleManifold3_F()
{
    // nothing to do
}

_MESHFEATURE_IMPLEMENT_3( MeshTriangleManifold3_F, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshTriangleManifold3_VE implementation
MeshTriangleManifold3_VE::MeshTriangleManifold3_VE( const MeshTriangleKey & triangleKey ):
    MeshTriangleManifold3( triangleKey )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_3( Vertices )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_3( Edges )
}
MeshTriangleManifold3_VE::~MeshTriangleManifold3_VE()
{
    // nothing to do
}

_MESHFEATURE_IMPLEMENT_3( MeshTriangleManifold3_VE, Vertex, Vertices )
_MESHFEATURE_IMPLEMENT_3( MeshTriangleManifold3_VE, Edge, Edges )

/////////////////////////////////////////////////////////////////////////////////
// MeshTriangleManifold3_VF implementation
MeshTriangleManifold3_VF::MeshTriangleManifold3_VF( const MeshTriangleKey & triangleKey ):
    MeshTriangleManifold3( triangleKey )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_3( Vertices )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_3( Facets )
}
MeshTriangleManifold3_VF::~MeshTriangleManifold3_VF()
{
    // nothing to do
}

_MESHFEATURE_IMPLEMENT_3( MeshTriangleManifold3_VF, Vertex, Vertices )
_MESHFEATURE_IMPLEMENT_3( MeshTriangleManifold3_VF, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshTriangleManifold3_EF implementation
MeshTriangleManifold3_EF::MeshTriangleManifold3_EF( const MeshTriangleKey & triangleKey ):
    MeshTriangleManifold3( triangleKey )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_3( Edges )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_3( Facets )
}
MeshTriangleManifold3_EF::~MeshTriangleManifold3_EF()
{
    // nothing to do
}

_MESHFEATURE_IMPLEMENT_3( MeshTriangleManifold3_EF, Edge, Edges )
_MESHFEATURE_IMPLEMENT_3( MeshTriangleManifold3_EF, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshTriangleManifold3_VEF implementation
MeshTriangleManifold3_VEF::MeshTriangleManifold3_VEF( const MeshTriangleKey & triangleKey ):
    MeshTriangleManifold3( triangleKey )
{
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_3( Vertices )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_3( Edges )
    _MESHFEATURE_IMPLEMENT_CONSTRUCTOR_3( Facets )
}
MeshTriangleManifold3_VEF::~MeshTriangleManifold3_VEF()
{
    // nothing to do
}

_MESHFEATURE_IMPLEMENT_3( MeshTriangleManifold3_VEF, Vertex, Vertices )
_MESHFEATURE_IMPLEMENT_3( MeshTriangleManifold3_VEF, Edge, Edges )
_MESHFEATURE_IMPLEMENT_3( MeshTriangleManifold3_VEF, Facet, Facets )


