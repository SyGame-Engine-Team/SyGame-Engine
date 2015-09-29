/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Features/MeshFeature.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Vertex-Edge-Facet features for MeshGraphs
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
#include "MeshFeature.h"

/////////////////////////////////////////////////////////////////////////////////
// MeshFeature implementation
MeshFeature::MeshFeature()
{
    // nothing to do
}
MeshFeature::~MeshFeature()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// MeshVertex implementation
MeshVertex::MeshVertex( const MeshVertexKey & vertexKey ):
    MeshFeature(), Key(vertexKey)
{
    // nothing to do
}
MeshVertex::~MeshVertex()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

//Void MeshVertex::_UpdateAdjacency( MeshVertex * pNewVertex, UInt iVertexCount, UInt iEdgeCount, UInt iFacetCount )
//{
//    UInt iUpdate, iFound;
//
//    // Update adjacent vertices
//    for( iUpdate = 0; iUpdate < iVertexCount; ++iUpdate ) {
//        MeshVertex * pVertexUpdate = GetVertex( iUpdate );
//        if ( pVertexUpdate == NULL )
//            break;
//
//        iFound = pVertexUpdate->FindVertex( this );
//        if ( iFound != INVALID_OFFSET )
//            pVertexUpdate->ReplaceVertex( iFound, pNewVertex );
//
//        pNewVertex->ConnectVertex( pVertexUpdate );
//    }
//
//    // Update adjacent edges
//    for( iUpdate = 0; iUpdate < iEdgeCount; ++iUpdate ) {
//        MeshEdge * pEdgeUpdate = GetEdge( iUpdate );
//        if ( pEdgeUpdate == NULL )
//            break;
//
//        iFound = pEdgeUpdate->FindVertex( this );
//        if ( iFound != INVALID_OFFSET )
//            pEdgeUpdate->ReplaceVertex( iFound, pNewVertex );
//
//        pNewVertex->ConnectEdge( pEdgeUpdate );
//    }
//
//    // Update adjacent facets
//    for( iUpdate = 0; iUpdate < iFacetCount; ++iUpdate ) {
//        MeshFacet * pFacetUpdate = GetFacet( iUpdate );
//        if ( pFacetUpdate == NULL )
//            break;
//
//        iFound = pFacetUpdate->FindVertex( this );
//        if ( iFound != INVALID_OFFSET )
//            pFacetUpdate->ReplaceVertex( iFound, pNewVertex );
//
//        pNewVertex->ConnectFacet( pFacetUpdate );
//    }
//}

/////////////////////////////////////////////////////////////////////////////////
// MeshEdge implementation
MeshEdge::MeshEdge( const MeshEdgeKey & edgeKey ):
    MeshFeature(), Key(edgeKey)
{
    // nothing to do
}
MeshEdge::~MeshEdge()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

//Void MeshEdge::_UpdateAdjacency( MeshEdge * pNewEdge, UInt iVertexCount, UInt iEdgeCount, UInt iFacetCount )
//{
//    UInt iUpdate, iFound;
//
//    // Update adjacent vertices
//    for( iUpdate = 0; iUpdate < iVertexCount; ++iUpdate ) {
//        MeshVertex * pVertexUpdate = GetVertex( iUpdate );
//        if ( pVertexUpdate == NULL )
//            break;
//
//        iFound = pVertexUpdate->FindEdge( this );
//        if ( iFound != INVALID_OFFSET )
//            pVertexUpdate->ReplaceEdge( iFound, pNewEdge );
//
//        pNewEdge->ConnectVertex( pVertexUpdate );
//    }
//
//    // Update adjacent edges
//    for( iUpdate = 0; iUpdate < iEdgeCount; ++iUpdate ) {
//        MeshEdge * pEdgeUpdate = GetEdge( iUpdate );
//        if ( pEdgeUpdate == NULL )
//            break;
//
//        iFound = pEdgeUpdate->FindEdge( this );
//        if ( iFound != INVALID_OFFSET )
//            pEdgeUpdate->ReplaceEdge( iFound, pNewEdge );
//
//        pNewEdge->ConnectEdge( pEdgeUpdate );
//    }
//
//    // Update adjacent facets
//    for( iUpdate = 0; iUpdate < iFacetCount; ++iUpdate ) {
//        MeshFacet * pFacetUpdate = GetFacet( iUpdate );
//        if ( pFacetUpdate == NULL )
//            break;
//
//        iFound = pFacetUpdate->FindEdge( this );
//        if ( iFound != INVALID_OFFSET )
//            pFacetUpdate->ReplaceEdge( iFound, pNewEdge );
//
//        pNewEdge->ConnectFacet( pFacetUpdate );
//    }
//}

/////////////////////////////////////////////////////////////////////////////////
// MeshFacet implementation
MeshFacet::MeshFacet():
    MeshFeature()
{
    // nothing to do
}
MeshFacet::~MeshFacet()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

//Void MeshFacet::_UpdateAdjacency( MeshFacet * pNewFacet, UInt iVertexCount, UInt iEdgeCount, UInt iFacetCount )
//{
//    UInt iUpdate, iFound;
//
//    // Update adjacent vertices
//    for( iUpdate = 0; iUpdate < iVertexCount; ++iUpdate ) {
//        MeshVertex * pVertexUpdate = GetVertex( iUpdate );
//        if ( pVertexUpdate == NULL )
//            break;
//
//        iFound = pVertexUpdate->FindFacet( this );
//        if ( iFound != INVALID_OFFSET )
//            pVertexUpdate->ReplaceFacet( iFound, pNewFacet );
//
//        pNewFacet->ConnectVertex( pVertexUpdate );
//    }
//
//    // Update adjacent edges
//    for( iUpdate = 0; iUpdate < iEdgeCount; ++iUpdate ) {
//        MeshEdge * pEdgeUpdate = GetEdge( iUpdate );
//        if ( pEdgeUpdate == NULL )
//            break;
//
//        iFound = pEdgeUpdate->FindFacet( this );
//        if ( iFound != INVALID_OFFSET )
//            pEdgeUpdate->ReplaceFacet( iFound, pNewFacet );
//
//        pNewFacet->ConnectEdge( pEdgeUpdate );
//    }
//
//    // Update adjacent facets
//    for( iUpdate = 0; iUpdate < iFacetCount; ++iUpdate ) {
//        MeshFacet * pFacetUpdate = GetFacet( iUpdate );
//        if ( pFacetUpdate == NULL )
//            break;
//
//        iFound = pFacetUpdate->FindFacet( this );
//        if ( iFound != INVALID_OFFSET )
//            pFacetUpdate->ReplaceFacet( iFound, pNewFacet );
//
//        pNewFacet->ConnectFacet( pFacetUpdate );
//    }
//}

/////////////////////////////////////////////////////////////////////////////////
// MeshFacetTriangle implementation
MeshFacetTriangle::MeshFacetTriangle( const MeshTriangleKey & triangleKey ):
    MeshFacet(), Key(triangleKey)
{
    // nothing to do
}
MeshFacetTriangle::~MeshFacetTriangle()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// MeshFacetQuad implementation
MeshFacetQuad::MeshFacetQuad( const MeshQuadKey & quadKey ):
    MeshFacet(), Key(quadKey)
{
    // nothing to do
}
MeshFacetQuad::~MeshFacetQuad()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// MeshFacetPolygon implementation
MeshFacetPolygon::MeshFacetPolygon( const MeshPolygonKey & polygonKey ):
    MeshFacet(), Key(polygonKey)
{
    // nothing to do
}
MeshFacetPolygon::~MeshFacetPolygon()
{
    // nothing to do
}

