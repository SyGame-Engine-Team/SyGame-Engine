/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/MeshGraph.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Base classes for adjacency & topology information.
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
#include "MeshGraph.h"

/////////////////////////////////////////////////////////////////////////////////
// MeshGraph implementation
MeshGraph::MeshGraph():
    Datastruct()
{
    // nothing to do
}
MeshGraph::~MeshGraph()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

MeshGraph::_LinkageResult MeshGraph::_Connect( MeshFacet * pFacetA, MeshFacet * pFacetB )
{
    Assert( pFacetA != NULL && pFacetB != NULL );

    // Test linkage caps
    UInt iCapacityA = pFacetA->GetFacetCapacity();
    UInt iCapacityB = pFacetB->GetFacetCapacity();
    if ( iCapacityA == 0 && iCapacityB == 0 )
        return _LINKAGE_NOLINK;

    // Test for Upgrade
    _LinkageResult iLinkRes = _LINKAGE_DONE;
    if ( iCapacityA != 0 && iCapacityA != INVALID_SIZE ) {
        if ( pFacetA->GetFacetCount() >= iCapacityA )
            iLinkRes |= _LINKAGE_GRADE_A;
    }
    if ( iCapacityB != 0 && iCapacityB != INVALID_SIZE ) {
        if ( pFacetB->GetFacetCount() >= iCapacityB )
            iLinkRes |= _LINKAGE_GRADE_B;
    }
    if ( iLinkRes != _LINKAGE_DONE )
        return iLinkRes;

    // Linkage
    if ( iCapacityA != 0 ) {
        UInt iFacetB = pFacetA->ConnectFacet( pFacetB );
        Assert( iFacetB != INVALID_OFFSET );
    }
    if ( iCapacityB != 0 ) {
        UInt iFacetA = pFacetB->ConnectFacet( pFacetA );
        Assert( iFacetA != INVALID_OFFSET );
    }

    return _LINKAGE_DONE;
}
MeshGraph::_LinkageResult MeshGraph::_Connect( MeshFacet * pFacetA, MeshEdge * pEdgeB )
{
    Assert( pFacetA != NULL && pEdgeB != NULL );

    // Test linkage caps
    UInt iCapacityA = pFacetA->GetEdgeCapacity();
    UInt iCapacityB = pEdgeB->GetFacetCapacity();
    if ( iCapacityA == 0 && iCapacityB == 0 )
        return _LINKAGE_NOLINK;

    // Test for Grow / Upgrade
    _LinkageResult iLinkRes = _LINKAGE_DONE;
    if ( iCapacityA != 0 && iCapacityA != INVALID_SIZE ) {
        if ( pFacetA->GetEdgeCount() >= iCapacityA )
            iLinkRes |= _LINKAGE_FACETSIZE_A;
    }
    if ( iCapacityB != 0 && iCapacityB != INVALID_SIZE ) {
        if ( pEdgeB->GetFacetCount() >= iCapacityB )
            iLinkRes |= _LINKAGE_GRADE_B;
    }
    if ( iLinkRes != _LINKAGE_DONE )
        return iLinkRes;

    // Linkage
    if ( iCapacityA != 0 ) {
        UInt iEdgeB = pFacetA->ConnectEdge( pEdgeB );
        Assert( iEdgeB != INVALID_OFFSET );
    }
    if ( iCapacityB != 0 ) {
        UInt iFacetA = pEdgeB->ConnectFacet( pFacetA );
        Assert( iFacetA != INVALID_OFFSET );
    }

    return _LINKAGE_DONE;
}
MeshGraph::_LinkageResult MeshGraph::_Connect( MeshFacet * pFacetA, MeshVertex * pVertexB )
{
    Assert( pFacetA != NULL && pVertexB != NULL );

    // Test linkage caps
    UInt iCapacityA = pFacetA->GetVertexCapacity();
    UInt iCapacityB = pVertexB->GetFacetCapacity();
    if ( iCapacityA == 0 && iCapacityB == 0 )
        return _LINKAGE_NOLINK;

    // Test for Grow / Upgrade
    _LinkageResult iLinkRes = _LINKAGE_DONE;
    if ( iCapacityA != 0 && iCapacityA != INVALID_SIZE ) {
        if ( pFacetA->GetVertexCount() >= iCapacityA )
            iLinkRes |= _LINKAGE_FACETSIZE_A;
    }
    if ( iCapacityB != 0 && iCapacityB != INVALID_SIZE ) {
        if ( pVertexB->GetFacetCount() >= iCapacityB )
            iLinkRes |= _LINKAGE_GRADE_B;
    }
    if ( iLinkRes != _LINKAGE_DONE )
        return iLinkRes;

    // Linkage
    if ( iCapacityA != 0 ) {
        UInt iVertexB = pFacetA->ConnectVertex( pVertexB );
        Assert( iVertexB != INVALID_OFFSET );
    }
    if ( iCapacityB != 0 ) {
        UInt iFacetA = pVertexB->ConnectFacet( pFacetA );
        Assert( iFacetA != INVALID_OFFSET );
    }

    return _LINKAGE_DONE;
}
MeshGraph::_LinkageResult MeshGraph::_Connect( MeshEdge * pEdgeA, MeshEdge * pEdgeB )
{
    Assert( pEdgeA != NULL && pEdgeB != NULL );

    // Test linkage caps
    UInt iCapacityA = pEdgeA->GetEdgeCapacity();
    UInt iCapacityB = pEdgeB->GetEdgeCapacity();
    if ( iCapacityA == 0 && iCapacityB == 0 )
        return _LINKAGE_NOLINK;

    // Test for Upgrade
    _LinkageResult iLinkRes = _LINKAGE_DONE;
    if ( iCapacityA != 0 && iCapacityA != INVALID_SIZE ) {
        if ( pEdgeA->GetEdgeCount() >= iCapacityA )
            iLinkRes |= _LINKAGE_GRADE_A;
    }
    if ( iCapacityB != 0 && iCapacityB != INVALID_SIZE ) {
        if ( pEdgeB->GetEdgeCount() >= iCapacityB )
            iLinkRes |= _LINKAGE_GRADE_B;
    }
    if ( iLinkRes != _LINKAGE_DONE )
        return iLinkRes;

    // Linkage
    if ( iCapacityA != 0 ) {
        UInt iEdgeB = pEdgeA->ConnectEdge( pEdgeB );
        Assert( iEdgeB != INVALID_OFFSET );
    }
    if ( iCapacityB != 0 ) {
        UInt iEdgeA = pEdgeB->ConnectEdge( pEdgeA );
        Assert( iEdgeA != INVALID_OFFSET );
    }

    return _LINKAGE_DONE;
}
MeshGraph::_LinkageResult MeshGraph::_Connect( MeshEdge * pEdgeA, MeshVertex * pVertexB )
{
    Assert( pEdgeA != NULL && pVertexB != NULL );

    // Test linkage caps
    UInt iCapacityA = pEdgeA->GetVertexCapacity();
    UInt iCapacityB = pVertexB->GetEdgeCapacity();
    if ( iCapacityA == 0 && iCapacityB == 0 )
        return _LINKAGE_NOLINK;

    // Test for Upgrade
    if ( iCapacityA != 0 ) {
        if ( pEdgeA->GetVertexCount() >= iCapacityA )
            return _LINKAGE_FULLEDGE;
    }
    if ( iCapacityB != 0 && iCapacityB != INVALID_SIZE ) {
        if ( pVertexB->GetEdgeCount() >= iCapacityB )
            return _LINKAGE_GRADE_B;
    }

    // Linkage
    if ( iCapacityA != 0 ) {
        UInt iVertexB = pEdgeA->ConnectVertex( pVertexB );
        Assert( iVertexB != INVALID_OFFSET );
    }
    if ( iCapacityB != 0 ) {
        UInt iEdgeA = pVertexB->ConnectEdge( pEdgeA );
        Assert( iEdgeA != INVALID_OFFSET );
    }

    return _LINKAGE_DONE;
}
MeshGraph::_LinkageResult MeshGraph::_Connect( MeshVertex * pVertexA, MeshVertex * pVertexB )
{
    Assert( pVertexA != NULL && pVertexB != NULL );

    // Test linkage caps
    UInt iCapacityA = pVertexA->GetVertexCapacity();
    UInt iCapacityB = pVertexB->GetVertexCapacity();
    if ( iCapacityA == 0 && iCapacityB == 0 )
        return _LINKAGE_NOLINK;

    // Test for Upgrade
    _LinkageResult iLinkRes = _LINKAGE_DONE;
    if ( iCapacityA != 0 && iCapacityA != INVALID_SIZE ) {
        if ( pVertexA->GetVertexCount() >= iCapacityA )
            iLinkRes |= _LINKAGE_GRADE_A;
    }
    if ( iCapacityB != 0 && iCapacityB != INVALID_SIZE ) {
        if ( pVertexB->GetVertexCount() >= iCapacityB )
            iLinkRes |= _LINKAGE_GRADE_B;
    }
    if ( iLinkRes != _LINKAGE_DONE )
        return iLinkRes;

    // Linkage
    if ( iCapacityA != 0 ) {
        UInt iVertexB = pVertexA->ConnectVertex( pVertexB );
        Assert( iVertexB != INVALID_OFFSET );
    }
    if ( iCapacityB != 0 ) {
        UInt iVertexA = pVertexB->ConnectVertex( pVertexA );
        Assert( iVertexA != INVALID_OFFSET );
    }

    return _LINKAGE_DONE;
}

MeshGraph::_LinkageResult MeshGraph::_Disconnect( MeshFacet * pFacetA, MeshFacet * pFacetB )
{
    Assert( pFacetA != NULL && pFacetB != NULL );

    // Test linkage caps
    UInt iCapacityA = pFacetA->GetFacetCapacity();
    UInt iCapacityB = pFacetB->GetFacetCapacity();
    if ( iCapacityA == 0 && iCapacityB == 0 )
        return _LINKAGE_NOLINK;

    // Linkage
    if ( iCapacityA != 0 ) {
        UInt iFacetB = pFacetA->FindFacet( pFacetB );
        if ( iFacetB == INVALID_OFFSET )
            return _LINKAGE_NOLINK;
        pFacetA->DisconnectFacet( iFacetB );
    }
    if ( iCapacityB != 0 ) {
        UInt iFacetA = pFacetB->FindFacet( pFacetA );
        if ( iFacetA == INVALID_OFFSET )
            return _LINKAGE_NOLINK;
        pFacetB->DisconnectFacet( iFacetA );
    }

    return _LINKAGE_DONE;
}
MeshGraph::_LinkageResult MeshGraph::_Disconnect( MeshFacet * pFacetA, MeshEdge * pEdgeB )
{
    Assert( pFacetA != NULL && pEdgeB != NULL );

    // Test linkage caps
    UInt iCapacityA = pFacetA->GetEdgeCapacity();
    UInt iCapacityB = pEdgeB->GetFacetCapacity();
    if ( iCapacityA == 0 && iCapacityB == 0 )
        return _LINKAGE_NOLINK;

    // Linkage
    if ( iCapacityA != 0 ) {
        UInt iEdgeB = pFacetA->FindEdge( pEdgeB );
        if ( iEdgeB == INVALID_OFFSET )
            return _LINKAGE_NOLINK;
        pFacetA->DisconnectEdge( iEdgeB );
    }
    if ( iCapacityB != 0 ) {
        UInt iFacetA = pEdgeB->FindFacet( pFacetA );
        if ( iFacetA == INVALID_OFFSET )
            return _LINKAGE_NOLINK;
        pEdgeB->DisconnectFacet( iFacetA );
    }

    return _LINKAGE_DONE;
}
MeshGraph::_LinkageResult MeshGraph::_Disconnect( MeshFacet * pFacetA, MeshVertex * pVertexB )
{
    Assert( pFacetA != NULL && pVertexB != NULL );

    // Test linkage caps
    UInt iCapacityA = pFacetA->GetVertexCapacity();
    UInt iCapacityB = pVertexB->GetFacetCapacity();
    if ( iCapacityA == 0 && iCapacityB == 0 )
        return _LINKAGE_NOLINK;

    // Linkage
    if ( iCapacityA != 0 ) {
        UInt iVertexB = pFacetA->FindVertex( pVertexB );
        if ( iVertexB == INVALID_OFFSET )
            return _LINKAGE_NOLINK;
        pFacetA->DisconnectVertex( iVertexB );
    }
    if ( iCapacityB != 0 ) {
        UInt iFacetA = pVertexB->FindFacet( pFacetA );
        if ( iFacetA == INVALID_OFFSET )
            return _LINKAGE_NOLINK;
        pVertexB->DisconnectFacet( iFacetA );
    }

    return _LINKAGE_DONE;
}
MeshGraph::_LinkageResult MeshGraph::_Disconnect( MeshEdge * pEdgeA, MeshEdge * pEdgeB )
{
    Assert( pEdgeA != NULL && pEdgeB != NULL );

    // Test linkage caps
    UInt iCapacityA = pEdgeA->GetEdgeCapacity();
    UInt iCapacityB = pEdgeB->GetEdgeCapacity();
    if ( iCapacityA == 0 && iCapacityB == 0 )
        return _LINKAGE_NOLINK;

    // Linkage
    if ( iCapacityA != 0 ) {
        UInt iEdgeB = pEdgeA->FindEdge( pEdgeB );
        if ( iEdgeB == INVALID_OFFSET )
            return _LINKAGE_NOLINK;
        pEdgeA->DisconnectEdge( iEdgeB );
    }
    if ( iCapacityB != 0 ) {
        UInt iEdgeA = pEdgeB->FindEdge( pEdgeA );
        if ( iEdgeA == INVALID_OFFSET )
            return _LINKAGE_NOLINK;
        pEdgeB->DisconnectEdge( iEdgeA );
    }

    return _LINKAGE_DONE;
}
MeshGraph::_LinkageResult MeshGraph::_Disconnect( MeshEdge * pEdgeA, MeshVertex * pVertexB )
{
    Assert( pEdgeA != NULL && pVertexB != NULL );

    // Test linkage caps
    UInt iCapacityA = pEdgeA->GetVertexCapacity();
    UInt iCapacityB = pVertexB->GetEdgeCapacity();
    if ( iCapacityA == 0 && iCapacityB == 0 )
        return _LINKAGE_NOLINK;

    // Linkage
    if ( iCapacityA != 0 ) {
        UInt iVertexB = pEdgeA->FindVertex( pVertexB );
        if ( iVertexB == INVALID_OFFSET )
            return _LINKAGE_NOLINK;
        pEdgeA->DisconnectVertex( iVertexB );
    }
    if ( iCapacityB != 0 ) {
        UInt iEdgeA = pVertexB->FindEdge( pEdgeA );
        if ( iEdgeA == INVALID_OFFSET )
            return _LINKAGE_NOLINK;
        pVertexB->DisconnectEdge( iEdgeA );
    }

    return _LINKAGE_DONE;
}
MeshGraph::_LinkageResult MeshGraph::_Disconnect( MeshVertex * pVertexA, MeshVertex * pVertexB )
{
    Assert( pVertexA != NULL && pVertexB != NULL );

    // Test linkage caps
    UInt iCapacityA = pVertexA->GetVertexCapacity();
    UInt iCapacityB = pVertexB->GetVertexCapacity();
    if ( iCapacityA == 0 && iCapacityB == 0 )
        return _LINKAGE_NOLINK;

    // Linkage
    if ( iCapacityA != 0 ) {
        UInt iVertexB = pVertexA->FindVertex( pVertexB );
        if ( iVertexB == INVALID_OFFSET )
            return _LINKAGE_NOLINK;
        pVertexA->DisconnectVertex( iVertexB );
    }
    if ( iCapacityB != 0 ) {
        UInt iVertexA = pVertexB->FindVertex( pVertexA );
        if ( iVertexA == INVALID_OFFSET )
            return _LINKAGE_NOLINK;
        pVertexB->DisconnectVertex( iVertexA );
    }

    return _LINKAGE_DONE;
}

//Void MeshGraph::_ChangeGrade( MeshVertex ** ppVertex, _GradeChange iGradeChange )
//{
//    Assert( ppVertex != NULL && (*ppVertex) != NULL );
//    MeshVertex * pVertex = *ppVertex;
//
//    MeshVertex * pUpgradedVertex = (iGradeChange == _UPGRADE) ? pVertex->Upgrade()
//                                                              : pVertex->Downgrade();
//    Assert( pUpgradedVertex != NULL );
//
//    ModelingFn->SelectMemory( TEXT("MeshGraphs") );
//    Delete( pVertex );
//    ModelingFn->UnSelectMemory();
//
//    *ppVertex = pUpgradedVertex;
//}
//Void MeshGraph::_ChangeGrade( MeshEdge ** ppEdge, _GradeChange iGradeChange )
//{
//    Assert( ppEdge != NULL && (*ppEdge) != NULL );
//    MeshEdge * pEdge = *ppEdge;
//
//    MeshEdge * pUpgradedEdge = (iGradeChange == _UPGRADE) ? pEdge->Upgrade()
//                                                          : pEdge->Downgrade();
//    Assert( pUpgradedEdge != NULL );
//
//    ModelingFn->SelectMemory( TEXT("MeshGraphs") );
//    Delete( pEdge );
//    ModelingFn->UnSelectMemory();
//
//    *ppEdge = pUpgradedEdge;
//}
//Void MeshGraph::_ChangeGrade( MeshFacet ** ppFacet, _GradeChange iGradeChange )
//{
//    Assert( ppFacet != NULL && (*ppFacet) != NULL );
//    MeshFacet * pFacet = *ppFacet;
//
//    MeshFacet * pUpgradedFacet = (iGradeChange == _UPGRADE) ? pFacet->Upgrade()
//                                                            : pFacet->Downgrade();
//    Assert( pUpgradedFacet != NULL );
//
//    ModelingFn->SelectMemory( TEXT("MeshGraphs") );
//    Delete( pFacet );
//    ModelingFn->UnSelectMemory();
//
//    *ppFacet = pUpgradedFacet;
//}

//Void MeshGraph::_Grow( MeshFacet ** ppFacet, UInt iEdge, const MeshEdgeKey & edgeKey )
//{
//    Assert( ppFacet != NULL && (*ppFacet) != NULL );
//    MeshFacet * pFacet = *ppFacet;
//
//    MeshFacet * pGrownFacet = NULL;
//    switch( pFacet->GetFacetType() ) {
//        case MESH_FACET_TRIANGLE: {
//                MeshFacetTriangle * pTriangle = (MeshFacetTriangle*)pFacet;
//                MeshQuadKey quadKey;
//                UInt iOut = 0;
//                for( UInt iIn = 0; iIn < 3; ++iIn ) {
//                    if ( pTriangle->Key.V[iIn] == pEdge->Key.A || pTriangle->Key.V[iIn] == pEdge->Key.B ) {
//                        quadKey.V[iOut++] = pEdge->Key.A;
//                        quadKey.V[iOut++] = pEdge->Key.B;
//                    } else
//                        quadKey.V[iOut++] = pTriangle->Key.V[iIn];
//                }
//                Assert( iOut == 4 );
//                pGrownFacet = pFacet->Grow( quadKey );
//            } break;
//        case MESH_FACET_QUAD: {
//                MeshFacetQuad * pQuad = (MeshFacetQuad*)pFacet;
//                MeshPolygonKey polygonKey(5);
//                UInt iOut = 0;
//                for( UInt iIn = 0; iIn < 4; ++iIn ) {
//                    if ( pQuad->Key.V[iIn] == pEdge->Key.A || pQuad->Key.V[iIn] == pEdge->Key.B ) {
//                        polygonKey.V[iOut++] = pEdge->Key.A;
//                        polygonKey.V[iOut++] = pEdge->Key.B;
//                    } else
//                        polygonKey.V[iOut++] = pQuad->Key.V[iIn];
//                }
//                Assert( iOut == 5 );
//                pGrownFacet = pFacet->Grow( polygonKey );
//            } break;
//        case MESH_FACET_POLYGON: {
//                MeshFacetPolygon * pPolygon = (MeshFacetPolygon*)pFacet;
//                MeshPolygonKey polygonKey( pPolygon->Key.SIZE + 1 );
//                UInt iOut = 0;
//                for( UInt iIn = 0; iIn < pPolygon->Key.SIZE; ++iIn ) {
//                    if ( pPolygon->Key.V[iIn] == pEdge->Key.A || pPolygon->Key.V[iIn] == pEdge->Key.B ) {
//                        polygonKey.V[iOut++] = pEdge->Key.A;
//                        polygonKey.V[iOut++] = pEdge->Key.B;
//                    } else
//                        polygonKey.V[iOut++] = pPolygon->Key.V[iIn];
//                }
//                Assert( iOut == pPolygon->Key.SIZE + 1 );
//                pGrownFacet = pFacet->Grow( polygonKey );
//            } break;
//        default: Assert(false); break;
//    }
//
//    switch( pFacet->GetFacetType() ) {
//        case MESH_FACET_TRIANGLE: {
//                MeshFacetTriangle * pTriangle = (MeshFacetTriangle*)pFacet;
//                MeshQuadKey quadKey;
//                quadKey.A = pTriangle->Key.A; quadKey.B = pTriangle->Key.B; quadKey.C = pTriangle->Key.C;
//                quadKey.D = pVertex->Key.V;
//                pGrownFacet = pFacet->Grow( quadKey );
//            } break;
//        case MESH_FACET_QUAD: {
//                MeshFacetQuad * pQuad = (MeshFacetQuad*)pFacet;
//                MeshPolygonKey polygonKey(5);
//                polygonKey.V[0] = pQuad->Key.A; polygonKey.V[1] = pQuad->Key.B;
//                polygonKey.V[2] = pQuad->Key.C; polygonKey.V[3] = pQuad->Key.D;
//                polygonKey.V[4] = pVertex->Key.V;
//                pGrownFacet = pFacet->Grow( polygonKey );
//            } break;
//        case MESH_FACET_POLYGON: {
//                MeshFacetPolygon * pPolygon = (MeshFacetPolygon*)pFacet;
//                MeshPolygonKey polygonKey( pPolygon->Key.SIZE + 1 );
//                for( UInt i = 0; i < pPolygon->Key.SIZE; ++i )
//                    polygonKey.V[i] = pPolygon->Key.V[i];
//                polygonKey.V[pPolygon->Key.SIZE] = pVertex->Key.V;
//                pGrownFacet = pFacet->Grow( polygonKey );
//            } break;
//        default: Assert(false); break;
//    }
//    Assert( pGrownFacet != NULL );
//
//    ModelingFn->SelectMemory( TEXT("MeshGraphs") );
//    Delete( pFacet );
//    ModelingFn->UnSelectMemory();
//
//    *ppFacet = pGrownFacet;
//}
//Void MeshGraph::_Shrink( MeshFacet ** ppFacet, UInt iEdge, UInt iVertex )
//{
//    Assert( ppFacet != NULL && (*ppFacet) != NULL );
//    MeshFacet * pFacet = *ppFacet;
//
//    MeshFacet * pShrunkFacet = NULL;
//    ////////////////////////////////////////////
//    Assert( pShrunkFacet != NULL );
//
//    ModelingFn->SelectMemory( TEXT("MeshGraphs") );
//    Delete( pFacet );
//    ModelingFn->UnSelectMemory();
//
//    *ppFacet = pShrunkFacet;
//}
