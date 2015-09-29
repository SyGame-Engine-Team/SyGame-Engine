/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/CollisionDetection/NarrowPhase/VCLip/VoronoiClip.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Narrow-Phase : Convex, V-Clip
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : none
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "VoronoiClip.h"

/////////////////////////////////////////////////////////////////////////////////
// VoronoiClip implementation
VoronoiClip::VoronoiClip( TransformedShapeConvex * pConvexA, TransformedShapeConvex * pConvexB ):
    NarrowPhaseConvex( pConvexA, pConvexB )
{
    m_pConvex[VCLIP_CONVEX_A] = pConvexA;
    m_pConvex[VCLIP_CONVEX_B] = pConvexB;

    // Null state
    m_vcState = VCLIP_STATE_SEPARATED;
    m_pClosestFeature[VCLIP_CONVEX_A] = NULL;
    m_pClosestFeature[VCLIP_CONVEX_B] = NULL;
}
VoronoiClip::~VoronoiClip()
{
    // nothing to do
}

Void VoronoiClip::Initialize()
{
    // Compute an approximate vector from A to B
    Vector3 vAB = ( m_pConvex[VCLIP_CONVEX_B]->GetAAB().GetCenter() -
                    m_pConvex[VCLIP_CONVEX_A]->GetAAB().GetCenter() );

    // Query extremal vertices along AB
    UInt iVertexA = INVALID_OFFSET;
    UInt iVertexB = INVALID_OFFSET;

    m_pConvex[VCLIP_CONVEX_A]->QueryExtremalVertices( vAB, &iVertexA, NULL );
    Assert( iVertexA != INVALID_OFFSET );

    m_pConvex[VCLIP_CONVEX_B]->QueryExtremalVertices( -vAB, &iVertexB, NULL );
    Assert( iVertexB != INVALID_OFFSET );    

    // Initial state
    m_vcState = VCLIP_STATE_VV;
    m_pClosestFeature[VCLIP_CONVEX_A] = m_pConvex[VCLIP_CONVEX_A]->GetGraph()->GetVertex( MeshVertexKey(iVertexA) );
    m_pClosestFeature[VCLIP_CONVEX_B] = m_pConvex[VCLIP_CONVEX_B]->GetGraph()->GetVertex( MeshVertexKey(iVertexB) );
}
Bool VoronoiClip::DetectCollision( Bool bGenerateContactPoints )
{
    // Save previous frame state
    VClipState iPrevFrameState = m_vcState;
    const MeshFeature * pPrevFrameFeatureA = m_pClosestFeature[VCLIP_CONVEX_A];
    const MeshFeature * pPrevFrameFeatureB = m_pClosestFeature[VCLIP_CONVEX_B];

    // Voronoi-Clip state machine
    const MeshFeature *pFeatureA, *pFeatureB;
    MeshFeatureType iTypeA ,iTypeB;

    VClipState iPrevStepState;

    UInt iIterations = 0;

    while( true ) {
        pFeatureA = m_pClosestFeature[VCLIP_CONVEX_A];
        pFeatureB = m_pClosestFeature[VCLIP_CONVEX_B];
        iTypeA = pFeatureA->GetType();
        iTypeB = pFeatureB->GetType();

        // Save previous step state
        if ( m_vcState >= VCLIP_STATE_VV )
            iPrevStepState = m_vcState;

        // Check for infinite loops
        if ( iIterations > VCLIP_MAX_ITERATIONS ) {
// Debug
            DebugPrint( TEXT("V-Clip infinite loop avoided ...\n") );
// Debug
            m_vcState = VCLIP_STATE_PENETRATION;
        }

        // Process current state
        switch( m_vcState ) {
            case VCLIP_STATE_PENETRATION:
                m_vcState = iPrevFrameState;
                m_pClosestFeature[VCLIP_CONVEX_A] = pPrevFrameFeatureA;
                m_pClosestFeature[VCLIP_CONVEX_B] = pPrevFrameFeatureB;
                if ( bGenerateContactPoints )
                    _GenerateContactPoints( m_pClosestFeature[VCLIP_CONVEX_A], m_pClosestFeature[VCLIP_CONVEX_B] );
                return true; // Done
            case VCLIP_STATE_SEPARATED:
                m_vcState = iPrevStepState;
                if ( bGenerateContactPoints )
                    _GenerateContactPoints( m_pClosestFeature[VCLIP_CONVEX_A], m_pClosestFeature[VCLIP_CONVEX_B] );
                return false; // Done

            case VCLIP_STATE_VV:
                Assert( iTypeA == MESH_FEATURE_VERTEX );
                Assert( iTypeB == MESH_FEATURE_VERTEX );
                m_vcState = _ProcessState_VV( VCLIP_CONVEX_A, (const Vertex *)pFeatureA,
                                              VCLIP_CONVEX_B, (const Vertex *)pFeatureB );
                break; // Continue
            case VCLIP_STATE_VE:
                if ( iTypeA == MESH_FEATURE_VERTEX ) {
                    Assert( iTypeB == MESH_FEATURE_EDGE );
                    m_vcState = _ProcessState_VE( VCLIP_CONVEX_A, (const Vertex *)pFeatureA,
                                                  VCLIP_CONVEX_B, (const Edge *)pFeatureB );
                } else {
                    Assert( iTypeA == MESH_FEATURE_EDGE );
                    Assert( iTypeB == MESH_FEATURE_VERTEX );
                    m_vcState = _ProcessState_VE( VCLIP_CONVEX_B, (const Vertex *)pFeatureB,
                                                  VCLIP_CONVEX_A, (const Edge *)pFeatureA );
                }
                break; // Continue
            case VCLIP_STATE_VF:
                if ( iTypeA == MESH_FEATURE_VERTEX ) {
                    Assert( iTypeB == MESH_FEATURE_FACET );
                    m_vcState = _ProcessState_VF( VCLIP_CONVEX_A, (const Vertex *)pFeatureA,
                                                  VCLIP_CONVEX_B, (const MeshFacet *)pFeatureB );
                } else {
                    Assert( iTypeA == MESH_FEATURE_FACET );
                    Assert( iTypeB == MESH_FEATURE_VERTEX );
                    m_vcState = _ProcessState_VF( VCLIP_CONVEX_B, (const Vertex *)pFeatureB,
                                                  VCLIP_CONVEX_A, (const MeshFacet *)pFeatureA );
                }
                break; // Continue
            case VCLIP_STATE_EE:
                Assert( iTypeA == MESH_FEATURE_EDGE );
                Assert( iTypeB == MESH_FEATURE_EDGE );
                m_vcState = _ProcessState_EE( VCLIP_CONVEX_A, (const Edge *)pFeatureA,
                                              VCLIP_CONVEX_B, (const Edge *)pFeatureB );
                break; // Continue
            case VCLIP_STATE_EF:
                if ( iTypeA == MESH_FEATURE_EDGE ) {
                    Assert( iTypeB == MESH_FEATURE_FACET );
                    m_vcState = _ProcessState_EF( VCLIP_CONVEX_A, (const Edge *)pFeatureA,
                                                  VCLIP_CONVEX_B, (const MeshFacet *)pFeatureB );
                } else {
                    Assert( iTypeA == MESH_FEATURE_FACET );
                    Assert( iTypeB == MESH_FEATURE_EDGE );
                    m_vcState = _ProcessState_EF( VCLIP_CONVEX_B, (const Edge *)pFeatureB,
                                                  VCLIP_CONVEX_A, (const MeshFacet *)pFeatureA );
                }
                break; // Continue
            default: Assert(false); break;
        }
        ++iIterations; 
    }
}

/////////////////////////////////////////////////////////////////////////////////

const Int           VoronoiClip::NullDerivationFlag = (Int)0xfffffff0;
const MeshFeature * VoronoiClip::RaisedPenetration = (const MeshFeature *)0xfffffff0;
Int VoronoiClip::_EvaluateDerivativeSign( const Vertex3 & vEdgeA, const Vertex3 & vEdgeB, Scalar fT, const Vertex3 & vVertexPosition ) const
{
    Vector3 vE = ( vEdgeB - vEdgeA );
    Vertex3 vT = vEdgeA + ( vE * fT );
    Vector3 vU = ( vT - vVertexPosition );
    if ( vU == Vector3::Null )
        return NullDerivationFlag;
    Scalar fDot = ( vU * vE );
    return MathFn->SignI( fDot, ZeroTolerance );
}
Int VoronoiClip::_EvaluateDerivativeSign( const Vertex3 & vEdgeA, const Vertex3 & vEdgeB, Scalar fT, const Plane & vFacetPlane ) const
{
    Vector3 vE = ( vEdgeB - vEdgeA );
    Vertex3 vT = vEdgeA + ( vE * fT );
    Scalar fDist = vFacetPlane.DistanceNP( vT );
    if ( MathFn->EqualsZero(fDist, ZeroTolerance) )
        return NullDerivationFlag;
    Scalar fDot = ( vFacetPlane.Normal * vE );
    if ( fDist < 0.0f ) fDot = -fDot;
    return MathFn->SignI( fDot, ZeroTolerance );
}

const MeshFeature * VoronoiClip::_ClipVertex_V( VClipConvexID idConvex, const Vertex * pVertex, const Vertex3 & vVertexV ) const
{
    // Run through Voronoi planes
    UInt iPlaneCount = m_pConvex[idConvex]->GetVoronoiPlaneCount( pVertex );
    Plane vPlane;

    Scalar fDistance;
    
    for( UInt i = 0; i < iPlaneCount; ++i ) {
        m_pConvex[idConvex]->GetVoronoiPlane( &vPlane, pVertex, i );
        fDistance = vPlane.DistanceNP( vVertexV );

        // Exclusion case
        if ( fDistance > 0.0f )
            return pVertex->Edges[i];
    }

    // Inclusion case
    return NULL;
}
const MeshFeature * VoronoiClip::_ClipVertex_E( VClipConvexID idConvex, const Edge * pEdge, const Vertex3 & vVertexV ) const
{
    // Run through Voronoi planes (EV planes first)
    UInt iPlaneCount = m_pConvex[idConvex]->GetVoronoiPlaneCount( pEdge );
    Plane vPlane;

    Scalar fDistance;

    for( UInt i = 0; i < iPlaneCount; ++i ) {
        m_pConvex[idConvex]->GetVoronoiPlane( &vPlane, pEdge, i );
        fDistance = vPlane.DistanceNP( vVertexV );

        // Exclusion case
        if ( fDistance > 0.0f ) {
            if ( i < 2 )
                return pEdge->Vertices[i];
            else
                return pEdge->Facets[i - 2];
        }
    }

    // Inclusion case
    return NULL;
}
const MeshFeature * VoronoiClip::_ClipVertex_F( VClipConvexID idConvex, const MeshFacet * pFacet, const Vertex3 & vVertexV ) const
{
    // Run through Voronoi planes
    UInt iPlaneCount = m_pConvex[idConvex]->GetVoronoiPlaneCount( pFacet );
    Plane vPlane;

    Scalar fDistance;
    Scalar fMaxDistance = 0.0f;
    UInt iMaxDistancePlane = INVALID_OFFSET;

    for( UInt i = 0; i < iPlaneCount; ++i ) {
        m_pConvex[idConvex]->GetVoronoiPlane( &vPlane, pFacet, i );
        fDistance = vPlane.DistanceNP( vVertexV );

        // Compute max distance plane
        if ( fDistance > fMaxDistance ) {
            fMaxDistance = fDistance;
            iMaxDistancePlane = i;
        }
    }

    // Exclusion case
    if ( iMaxDistancePlane != INVALID_OFFSET )
        return pFacet->GetEdge(iMaxDistancePlane);

    // Inclusion case
    return NULL;
}

const MeshFeature * VoronoiClip::_ClipEdge_V( VClipConvexID idConvex, const Vertex * pVertex, const Vertex3 & vEdgeA, const Vertex3 & vEdgeB ) const
{
    // Run through Voronoi planes
    UInt iPlaneCount = m_pConvex[idConvex]->GetVoronoiPlaneCount( pVertex );
    Plane vPlane;

    Scalar fDistanceA, fDistanceB;
    Scalar fT;

    Scalar fEnter = 0.0f;
    Scalar fExit = 1.0f;
    UInt iEnterPlane = INVALID_OFFSET;
    UInt iExitPlane = INVALID_OFFSET;

    for( UInt i = 0; i < iPlaneCount; ++i ) {
        m_pConvex[idConvex]->GetVoronoiPlane( &vPlane, pVertex, i );
        fDistanceA = vPlane.DistanceNP( vEdgeA );
        fDistanceB = vPlane.DistanceNP( vEdgeB );

        // Simple exclusion case
        if ( fDistanceA > 0.0f && fDistanceB > 0.0f )
            return pVertex->Edges[i];

        // Clip cases
        if ( fDistanceA > 0.0f ) {
            // Clip at A's side
            fT = ( fDistanceA / (fDistanceA - fDistanceB) );
            if ( fT > fEnter ) {
                fEnter = fT;
                iEnterPlane = i;

                // Compound exclusion case
                if ( fEnter > fExit )
                    break;
            }
        } else if ( fDistanceB > 0.0f ) {
            // Clip at B's side
            fT = ( fDistanceA / (fDistanceA - fDistanceB) );
            if ( fT < fExit ) {
                fExit = fT;
                iExitPlane = i;

                // Compound exclusion case
                if ( fEnter > fExit )
                    break;
            }
        }
    }

    // Compound exclusion / Clipped cases
    Vertex3 vPosition;
    m_pConvex[idConvex]->GetVertexPosition( &vPosition, pVertex );

    if ( iEnterPlane != INVALID_OFFSET ) {        
        Int iSign = _EvaluateDerivativeSign( vEdgeA, vEdgeB, fEnter, vPosition );
        if ( iSign == NullDerivationFlag )
            return RaisedPenetration;
        if ( iSign > 0 )
            return pVertex->Edges[iEnterPlane];
    }
    if ( iExitPlane != INVALID_OFFSET ) {
        Int iSign = _EvaluateDerivativeSign( vEdgeA, vEdgeB, fExit, vPosition );
        if ( iSign == NullDerivationFlag )
            return RaisedPenetration;
        if ( iSign < 0 )
            return pVertex->Edges[iExitPlane];
    }

    // Inclusion case
    return NULL;
}
const MeshFeature * VoronoiClip::_ClipEdge_E( VClipConvexID idConvex, const Edge * pEdge, const Vertex3 & vEdgeA, const Vertex3 & vEdgeB ) const
{
    // Get all Voronoi planes
    Plane arrVoronoiRegion[4];
    for( UInt i = 0; i < 4; ++i )
        m_pConvex[idConvex]->GetVoronoiPlane( arrVoronoiRegion + i, pEdge, i );

    // Clipping data
    VClipResponse clipResponseEV, clipResponseEF, clipResponse;
    UInt iEnterPlaneEV, iExitPlaneEV, iEnterPlane;
    UInt iEnterPlaneEF, iExitPlaneEF, iExitPlane;
    Scalar fEnterEV, fExitEV, fEnter;
    Scalar fEnterEF, fExitEF, fExit;

    Int iSign;
    Vertex3 vPosition;
    Plane facetPlane;

    // Clip E1 against VR(E0), EV planes only
    clipResponseEV = _ClipEdge_Generic( arrVoronoiRegion, 2, vEdgeA, vEdgeB,
                                      &iEnterPlaneEV, &fEnterEV, &iExitPlaneEV, &fExitEV );

    // Simple exclusion case
    if ( clipResponseEV == VCLIP_EXCLUDED_SIMPLE ) {
        Assert( (iEnterPlaneEV == iExitPlaneEV) && (iEnterPlaneEV != INVALID_OFFSET) );

        return pEdge->Vertices[iEnterPlaneEV];
    }

    // Clip E1 against VR(E0), EF planes only
    clipResponseEF = _ClipEdge_Generic( arrVoronoiRegion + 2, 2, vEdgeA, vEdgeB,
                                      &iEnterPlaneEF, &fEnterEF, &iExitPlaneEF, &fExitEF );
    if ( iEnterPlaneEF != INVALID_OFFSET )
        iEnterPlaneEF += 2;
    if ( iExitPlaneEF != INVALID_OFFSET )
        iExitPlaneEF += 2;

    // Simple exclusion case
    if ( clipResponseEF == VCLIP_EXCLUDED_SIMPLE ) {
        Assert( (iEnterPlaneEF == iExitPlaneEF) && (iEnterPlaneEF != INVALID_OFFSET) );

        if ( iEnterPlaneEV != INVALID_OFFSET ) {
            m_pConvex[idConvex]->GetVertexPosition( &vPosition, pEdge->Vertices[iEnterPlaneEV] );
            iSign = _EvaluateDerivativeSign( vEdgeA, vEdgeB, fEnterEV, vPosition );
            if ( iSign == NullDerivationFlag )
                return RaisedPenetration;
            if ( iSign > 0 )
                return pEdge->Vertices[iEnterPlaneEV];
        }
        if ( iExitPlaneEV != INVALID_OFFSET ) {
            m_pConvex[idConvex]->GetVertexPosition( &vPosition, pEdge->Vertices[iExitPlaneEV] );
            iSign = _EvaluateDerivativeSign( vEdgeA, vEdgeB, fExitEV, vPosition );
            if ( iSign == NullDerivationFlag )
                return RaisedPenetration;
            if ( iSign < 0 )
                return pEdge->Vertices[iExitPlaneEV];
        }
        return pEdge->Facets[iEnterPlaneEF - 2];
    }

    // Compute mixed result
    fEnter = fEnterEV;
    fExit = fExitEV;
    iEnterPlane = iEnterPlaneEV;
    iExitPlane = iExitPlaneEV;
    if ( iEnterPlaneEF != INVALID_OFFSET ) {
        if ( fEnterEF > fEnter ) {
            fEnter = fEnterEF;
            iEnterPlane = iEnterPlaneEF;
        }
    }
    if ( iExitPlaneEF != INVALID_OFFSET ) {
        if ( fExitEF < fExit ) {
            fExit = fExitEF;
            iExitPlane = iExitPlaneEF;
        }
    }
    if ( fEnter > fExit )
        clipResponse = VCLIP_EXCLUDED_COMPOUND;
    else {
        if ( iEnterPlane != INVALID_OFFSET || iExitPlane != INVALID_OFFSET )
            clipResponse = VCLIP_CLIPPED;
        else
            clipResponse = VCLIP_INCLUDED;
    }

    // Compound exclusion case
    if ( clipResponse == VCLIP_EXCLUDED_COMPOUND ) {
        Assert( (iEnterPlane != INVALID_OFFSET) && (iExitPlane != INVALID_OFFSET) );
        // iEnterPlane is an EV-Plane
        if ( iEnterPlane < 2 ) {
            m_pConvex[idConvex]->GetVertexPosition( &vPosition, pEdge->Vertices[iEnterPlane] );
            iSign = _EvaluateDerivativeSign( vEdgeA, vEdgeB, fEnter, vPosition );
            if ( iSign == NullDerivationFlag )
                return RaisedPenetration;
            if ( iSign >= 0 )
                return pEdge->Vertices[iEnterPlane];
            if ( iExitPlane < 2 )
                return pEdge->Vertices[iExitPlane];
            return pEdge->Facets[iExitPlane - 2];
        }
        // iExitPlane is an EV-Plane
        if ( iExitPlane < 2 ) {
            m_pConvex[idConvex]->GetVertexPosition( &vPosition, pEdge->Vertices[iExitPlane] );
            iSign = _EvaluateDerivativeSign( vEdgeA, vEdgeB, fExit, vPosition );
            if ( iSign == NullDerivationFlag )
                return RaisedPenetration;
            if ( iSign <= 0 )
                return pEdge->Vertices[iExitPlane];
            if ( iEnterPlane < 2 )
                return pEdge->Vertices[iEnterPlane];
            return pEdge->Facets[iEnterPlane - 2];
        }
        // Both iEnterPlane & iExitPlane are EF-Planes
        m_pConvex[idConvex]->GetFacetPlane( &facetPlane, pEdge->Facets[iEnterPlane - 2] );
        iSign = _EvaluateDerivativeSign( vEdgeA, vEdgeB, fEnter, facetPlane );
        if ( iSign == NullDerivationFlag )
            return RaisedPenetration;
        if ( iSign > 0 )
            return pEdge->Facets[iEnterPlane - 2];
        return pEdge->Facets[iExitPlane - 2];
    }

    // Clipped case
    if ( clipResponse == VCLIP_CLIPPED ) {
        if ( iEnterPlane != INVALID_OFFSET ) {
            if ( iEnterPlane < 2 ) {
                m_pConvex[idConvex]->GetVertexPosition( &vPosition, pEdge->Vertices[iEnterPlane] );
                iSign = _EvaluateDerivativeSign( vEdgeA, vEdgeB, fEnter, vPosition );
                if ( iSign == NullDerivationFlag )
                    return RaisedPenetration;
                if ( iSign > 0 )
                    return pEdge->Vertices[iEnterPlane];
            } else {
                m_pConvex[idConvex]->GetFacetPlane( &facetPlane, pEdge->Facets[iEnterPlane - 2] );
                iSign = _EvaluateDerivativeSign( vEdgeA, vEdgeB, fEnter, facetPlane );
                if ( iSign == NullDerivationFlag )
                    return RaisedPenetration;
                if ( iSign > 0 )
                    return pEdge->Facets[iEnterPlane - 2];
            }
        }
        if ( iExitPlane != INVALID_OFFSET ) {
            if ( iExitPlane < 2 ) {
                m_pConvex[idConvex]->GetVertexPosition( &vPosition, pEdge->Vertices[iExitPlane] );
                iSign = _EvaluateDerivativeSign( vEdgeA, vEdgeB, fExit, vPosition );
                if ( iSign == NullDerivationFlag )
                    return RaisedPenetration;
                if ( iSign < 0 )
                    return pEdge->Vertices[iExitPlane];
            } else {
                m_pConvex[idConvex]->GetFacetPlane( &facetPlane, pEdge->Facets[iExitPlane - 2] );
                iSign = _EvaluateDerivativeSign( vEdgeA, vEdgeB, fExit, facetPlane );
                if ( iSign == NullDerivationFlag )
                    return RaisedPenetration;
                if ( iSign < 0 )
                    return pEdge->Facets[iExitPlane - 2];
            }
        }
    }

    // Included case
    return NULL;
}
VClipResponse VoronoiClip::_ClipEdge_F( VClipConvexID idConvex, const MeshFacet * pFacet, const Vertex3 & vEdgeA, const Vertex3 & vEdgeB,
                                        UInt * outEnterPlane, Scalar * outEnter, UInt * outExitPlane, Scalar * outExit ) const
{
    // Run through Voronoi planes
    UInt iPlaneCount = m_pConvex[idConvex]->GetVoronoiPlaneCount( pFacet );
    Plane vPlane;

    Scalar fDistanceA, fDistanceB;
    Scalar fT;

    *outEnter = 0.0f;
    *outExit = 1.0f;
    *outEnterPlane = INVALID_OFFSET;
    *outExitPlane = INVALID_OFFSET;

    for( UInt i = 0; i < iPlaneCount; ++i ) {
        m_pConvex[idConvex]->GetVoronoiPlane( &vPlane, pFacet, i );
        fDistanceA = vPlane.DistanceNP( vEdgeA );
        fDistanceB = vPlane.DistanceNP( vEdgeB );

        // Simple exclusion case
        if ( fDistanceA > 0.0f && fDistanceB > 0.0f ) {
            *outEnterPlane = i;
            *outExitPlane = i;
            return VCLIP_EXCLUDED_SIMPLE;
        }

        // Clip cases
        if ( fDistanceA > 0.0f ) {
            // Clip at A's side
            fT = ( fDistanceA / (fDistanceA - fDistanceB) );
            if ( fT > *outEnter ) {
                *outEnter = fT;
                *outEnterPlane = i;

                // Compound exclusion case
                if ( *outEnter > *outExit )
                    return VCLIP_EXCLUDED_COMPOUND;
            }
        } else if ( fDistanceB > 0.0f ) {
            // Clip at B's side
            fT = ( fDistanceA / (fDistanceA - fDistanceB) );
            if ( fT < *outExit ) {
                *outExit = fT;
                *outExitPlane = i;

                // Compound exclusion case
                if ( *outEnter > *outExit )
                    return VCLIP_EXCLUDED_COMPOUND;
            }
        }
    }

    // Clipped case
    if ( *outEnterPlane != INVALID_OFFSET || *outExitPlane != INVALID_OFFSET )
        return VCLIP_CLIPPED;

    // Inclusion case
    return VCLIP_INCLUDED;
}
VClipResponse VoronoiClip::_ClipEdge_Generic( const Plane * arrVoronoiPlanes, UInt iPlaneCount, const Vertex3 & vEdgeA, const Vertex3 & vEdgeB,
                                              UInt * outEnterPlane, Scalar * outEnter, UInt * outExitPlane, Scalar * outExit ) const
{
    // Run through Voronoi planes
    Plane vPlane;

    Scalar fDistanceA, fDistanceB;
    Scalar fT;

    *outEnter = 0.0f;
    *outExit = 1.0f;
    *outEnterPlane = INVALID_OFFSET;
    *outExitPlane = INVALID_OFFSET;

    for( UInt i = 0; i < iPlaneCount; ++i ) {
        fDistanceA = arrVoronoiPlanes[i].DistanceNP( vEdgeA );
        fDistanceB = arrVoronoiPlanes[i].DistanceNP( vEdgeB );

        // Simple exclusion case
        if ( fDistanceA > 0.0f && fDistanceB > 0.0f ) {
            *outEnterPlane = i;
            *outExitPlane = i;
            return VCLIP_EXCLUDED_SIMPLE;
        }

        // Clip cases
        if ( fDistanceA > 0.0f ) {
            // Clip at A's side
            fT = ( fDistanceA / (fDistanceA - fDistanceB) );
            if ( fT > *outEnter ) {
                *outEnter = fT;
                *outEnterPlane = i;

                // Compound exclusion case
                if ( *outEnter > *outExit )
                    return VCLIP_EXCLUDED_COMPOUND;
            }
        } else if ( fDistanceB > 0.0f ) {
            // Clip at B's side
            fT = ( fDistanceA / (fDistanceA - fDistanceB) );
            if ( fT < *outExit ) {
                *outExit = fT;
                *outExitPlane = i;

                // Compound exclusion case
                if ( *outEnter > *outExit )
                    return VCLIP_EXCLUDED_COMPOUND;
            }
        }
    }

    // Clipped case
    if ( *outEnterPlane != INVALID_OFFSET || *outExitPlane != INVALID_OFFSET )
        return VCLIP_CLIPPED;

    // Inclusion case
    return VCLIP_INCLUDED;
}

const MeshFeature * VoronoiClip::_ClosestEdgeTowardFacet( VClipConvexID idConvex, const Vertex * pVertex, const Vertex3 & vPosition,
                                                          Scalar fDistanceV, const Plane & facetPlane ) const
{
    // Run through adjacent edges
    UInt iEdgeCount = pVertex->GetEdgeCount();
    const Edge * pEdge;

    UInt iOppositeVertex;
    const Vertex * pOppositeVertex;
    Vertex3 vOppositePosition;
    Vector3 vEdge;

    Scalar fDot;
    Scalar fBestEdgeDot = 0.0f;
    UInt iBestEdge = INVALID_OFFSET;

    for( UInt i = 0; i < iEdgeCount; ++i ) {
        pEdge = (const Edge *)( pVertex->Edges[i] );

        // Get edge vector
        iOppositeVertex = ( 1 - pEdge->FindVertex(pVertex) );
        pOppositeVertex = (const Vertex *)( pEdge->Vertices[iOppositeVertex] );
        m_pConvex[idConvex]->GetVertexPosition( &vOppositePosition, pOppositeVertex );
        vEdge = (vOppositePosition - vPosition);

        // Compute best edge
        fDot = ( vEdge * facetPlane.Normal );
        if ( fDistanceV > 0.0f )
            fDot = -fDot;
        if ( fDot > fBestEdgeDot ) {
            fBestEdgeDot = fDot;
            iBestEdge = i;
        }
    }

    // Found an edge toward facet
    if ( iBestEdge != INVALID_OFFSET )
        return pVertex->Edges[iBestEdge];

    // All edges are away from the facet
    return NULL;
}
const MeshFeature * VoronoiClip::_ClosestFeatureTowardEdge( VClipConvexID idConvex, const MeshFacet * pFacet, UInt iStartEdge,
                                                            const Vertex3 & vEdgeA, const Vertex3 & vEdgeB ) const
{
    UInt iFacetSize = pFacet->GetFacetSize();

    // Clipping data
    VClipResponse clipResponse;
    UInt iEnterPlane, iExitPlane;
    Scalar fEnter, fExit;

    // Phase 1 : Find a region that clips the edge
    const Edge * pEdge;
    const Vertex * pVertex;

    Plane arrEdgeVoronoiRegion[2];
    Plane arrVertexVoronoiRegion[2];
    Vertex3 vPosition;
    const Edge * pOtherEdge;
    UInt iOtherEdge;

    Bool bEdgeRegion = true;
    UInt iEdge = iStartEdge;
    UInt iVertex = INVALID_OFFSET;

    while( true ) {
        if ( bEdgeRegion ) {
            // Visit edge region
            pEdge = (const Edge *)( pFacet->GetEdge(iEdge) );

            // Clip Edge AB against VR(E), EV planes only
            m_pConvex[idConvex]->GetVoronoiPlane( arrEdgeVoronoiRegion + 0, pEdge, 0 );
            m_pConvex[idConvex]->GetVoronoiPlane( arrEdgeVoronoiRegion + 1, pEdge, 1 );
            clipResponse = _ClipEdge_Generic( arrEdgeVoronoiRegion, 2, vEdgeA, vEdgeB,
                                              &iEnterPlane, &fEnter, &iExitPlane, &fExit );

            // Found a clipped region
            if ( clipResponse == VCLIP_CLIPPED || clipResponse == VCLIP_INCLUDED )
                break;

            // Simple exclusion => Search in closing direction
            Assert( clipResponse == VCLIP_EXCLUDED_SIMPLE );
            Assert( iEnterPlane == iExitPlane && iEnterPlane != INVALID_OFFSET );
            if ( iEnterPlane == 0 ) {
                // Previous vertex
                iVertex = iEdge;

                // Setup vertex voronoi planes (borrow from edges and reverse normals)
                iOtherEdge = ( iVertex + iFacetSize - 1 ) % iFacetSize;
                pOtherEdge = (const Edge *)( pFacet->GetEdge(iOtherEdge) );

                m_pConvex[idConvex]->GetVoronoiPlane( arrVertexVoronoiRegion + 0, pOtherEdge, 1 );
                m_pConvex[idConvex]->GetVoronoiPlane( arrVertexVoronoiRegion + 1, pEdge, 0 );
                arrVertexVoronoiRegion[0].Normal = -( arrVertexVoronoiRegion[0].Normal );
                arrVertexVoronoiRegion[1].Normal = -( arrVertexVoronoiRegion[1].Normal );
            } else {
                Assert( iEnterPlane == 1 );
                // Next vertex
                iVertex = ( iEdge + 1 ) % iFacetSize;

                // Setup vertex voronoi planes (borrow from edges and reverse normals)
                iOtherEdge = iVertex;
                pOtherEdge = (const Edge *)( pFacet->GetEdge(iOtherEdge) );

                m_pConvex[idConvex]->GetVoronoiPlane( arrVertexVoronoiRegion + 0, pEdge, 1 );
                m_pConvex[idConvex]->GetVoronoiPlane( arrVertexVoronoiRegion + 1, pOtherEdge, 0 );
                arrVertexVoronoiRegion[0].Normal = -( arrVertexVoronoiRegion[0].Normal );
                arrVertexVoronoiRegion[1].Normal = -( arrVertexVoronoiRegion[1].Normal );
            }
            bEdgeRegion = false;
        } else {
            // Visit vertex region
            pVertex = (const Vertex *)( pFacet->GetVertex(iVertex) );

            // Clip Edge AB against VR(V), EV planes only
            clipResponse = _ClipEdge_Generic( arrVertexVoronoiRegion, 2, vEdgeA, vEdgeB,
                                              &iEnterPlane, &fEnter, &iExitPlane, &fExit );

            // Found a clipped region
            if ( clipResponse == VCLIP_CLIPPED || clipResponse == VCLIP_INCLUDED )
                break;

            // Simple / Compound exclusion => Search in same direction
            if ( iEdge == iVertex ) {
                // Going backward => Previous edge
                iEdge = ( iVertex + iFacetSize - 1 ) % iFacetSize;
            } else {
                // Going forward => Next edge
                iEdge = iVertex;
            }
            bEdgeRegion = true;
        }
    }

    // Phase Transition : avoid wasting last clip
    if ( bEdgeRegion ) {
        // Inclusion case
        if ( clipResponse == VCLIP_INCLUDED )
            return pEdge;

        // Clipped case
        if ( iEnterPlane != INVALID_OFFSET ) {
            m_pConvex[idConvex]->GetVertexPosition( &vPosition, pEdge->Vertices[0] );
            Int iSign = _EvaluateDerivativeSign( vEdgeA, vEdgeB, fEnter, vPosition );
            if ( iSign == NullDerivationFlag )
                return RaisedPenetration;
            if ( iSign > 0 ) {
                // Previous vertex
                iVertex = iEdge;

                // Setup vertex voronoi planes (borrow from edges and reverse normals)
                iOtherEdge = ( iVertex + iFacetSize - 1 ) % iFacetSize;
                pOtherEdge = (const Edge *)( pFacet->GetEdge(iOtherEdge) );

                m_pConvex[idConvex]->GetVoronoiPlane( arrVertexVoronoiRegion + 0, pOtherEdge, 1 );
                m_pConvex[idConvex]->GetVoronoiPlane( arrVertexVoronoiRegion + 1, pEdge, 0 );
                arrVertexVoronoiRegion[0].Normal = -( arrVertexVoronoiRegion[0].Normal );
                arrVertexVoronoiRegion[1].Normal = -( arrVertexVoronoiRegion[1].Normal );

                bEdgeRegion = false;
                goto __Label_Phase2;
            }
        }
        if ( iExitPlane != INVALID_OFFSET ) {
            m_pConvex[idConvex]->GetVertexPosition( &vPosition, pEdge->Vertices[1] );
            Int iSign = _EvaluateDerivativeSign( vEdgeA, vEdgeB, fExit, vPosition );
            if ( iSign == NullDerivationFlag )
                return RaisedPenetration;
            if ( iSign < 0 ) {
                // Next vertex
                iVertex = ( iEdge + 1 ) % iFacetSize;

                // Setup vertex voronoi planes (borrow from edges and reverse normals)
                iOtherEdge = iVertex;
                pOtherEdge = (const Edge *)( pFacet->GetEdge(iOtherEdge) );

                m_pConvex[idConvex]->GetVoronoiPlane( arrVertexVoronoiRegion + 0, pEdge, 1 );
                m_pConvex[idConvex]->GetVoronoiPlane( arrVertexVoronoiRegion + 1, pOtherEdge, 0 );
                arrVertexVoronoiRegion[0].Normal = -( arrVertexVoronoiRegion[0].Normal );
                arrVertexVoronoiRegion[1].Normal = -( arrVertexVoronoiRegion[1].Normal );

                bEdgeRegion = false;
                goto __Label_Phase2;
            }
        }
        return pEdge; // Can't get closer
    } else {
        // Inclusion case
        if ( clipResponse == VCLIP_INCLUDED )
            return pVertex;

        // Clipped case
        m_pConvex[idConvex]->GetVertexPosition( &vPosition, pVertex );
        if ( iEnterPlane != INVALID_OFFSET ) {
            Int iSign = _EvaluateDerivativeSign( vEdgeA, vEdgeB, fEnter, vPosition );
            if ( iSign == NullDerivationFlag )
                return RaisedPenetration;
            if ( iSign > 0 ) {
                iEdge = ( iVertex + iFacetSize - 1 ) % iFacetSize;

                bEdgeRegion = true;
                goto __Label_Phase2;
            }
        }
        if ( iExitPlane != INVALID_OFFSET ) {
            Int iSign = _EvaluateDerivativeSign( vEdgeA, vEdgeB, fExit, vPosition );
            if ( iSign == NullDerivationFlag )
                return RaisedPenetration;
            if ( iSign < 0 ) {
                iEdge = iVertex;

                bEdgeRegion = true;
                goto __Label_Phase2;
            }
        }
        return pVertex; // Can't get closer
    }

    // Phase 2 : Follow regions using derivatives
    // TODO : Correct infinite loop rare case (drop a cube over a flat ground by the edge)
__Label_Phase2:
    while( true ) {
        if ( bEdgeRegion ) {
            // Visit edge region
            pEdge = (const Edge *)( pFacet->GetEdge(iEdge) );

            // Clip Edge AB against VR(E), EV planes only
            m_pConvex[idConvex]->GetVoronoiPlane( arrEdgeVoronoiRegion + 0, pEdge, 0 );
            m_pConvex[idConvex]->GetVoronoiPlane( arrEdgeVoronoiRegion + 1, pEdge, 1 );
            clipResponse = _ClipEdge_Generic( arrEdgeVoronoiRegion, 2, vEdgeA, vEdgeB,
                                              &iEnterPlane, &fEnter, &iExitPlane, &fExit );

            // Found an excluding region, last vertex is the closest feature
            if ( clipResponse == VCLIP_EXCLUDED_SIMPLE || clipResponse == VCLIP_EXCLUDED_COMPOUND )
                return pVertex;

            // Inclusion case
            if ( clipResponse == VCLIP_INCLUDED )
                return pEdge;

            // Clipped case
            if ( iEnterPlane != INVALID_OFFSET ) {
                m_pConvex[idConvex]->GetVertexPosition( &vPosition, pEdge->Vertices[0] );
                Int iSign = _EvaluateDerivativeSign( vEdgeA, vEdgeB, fEnter, vPosition );
                if ( iSign == NullDerivationFlag )
                    return RaisedPenetration;
                if ( iSign > 0 ) {
                    // Previous vertex
                    iVertex = iEdge;

                    // Setup vertex voronoi planes (borrow from edges and reverse normals)
                    iOtherEdge = ( iVertex + iFacetSize - 1 ) % iFacetSize;
                    pOtherEdge = (const Edge *)( pFacet->GetEdge(iOtherEdge) );

                    m_pConvex[idConvex]->GetVoronoiPlane( arrVertexVoronoiRegion + 0, pOtherEdge, 1 );
                    m_pConvex[idConvex]->GetVoronoiPlane( arrVertexVoronoiRegion + 1, pEdge, 0 );
                    arrVertexVoronoiRegion[0].Normal = -( arrVertexVoronoiRegion[0].Normal );
                    arrVertexVoronoiRegion[1].Normal = -( arrVertexVoronoiRegion[1].Normal );

                    bEdgeRegion = false;
                    continue;
                }
            }
            if ( iExitPlane != INVALID_OFFSET ) {
                m_pConvex[idConvex]->GetVertexPosition( &vPosition, pEdge->Vertices[1] );
                Int iSign = _EvaluateDerivativeSign( vEdgeA, vEdgeB, fExit, vPosition );
                if ( iSign == NullDerivationFlag )
                    return RaisedPenetration;
                if ( iSign < 0 ) {
                    // Next vertex
                    iVertex = ( iEdge + 1 ) % iFacetSize;

                    // Setup vertex voronoi planes (borrow from edges and reverse normals)
                    iOtherEdge = iVertex;
                    pOtherEdge = (const Edge *)( pFacet->GetEdge(iOtherEdge) );

                    m_pConvex[idConvex]->GetVoronoiPlane( arrVertexVoronoiRegion + 0, pEdge, 1 );
                    m_pConvex[idConvex]->GetVoronoiPlane( arrVertexVoronoiRegion + 1, pOtherEdge, 0 );
                    arrVertexVoronoiRegion[0].Normal = -( arrVertexVoronoiRegion[0].Normal );
                    arrVertexVoronoiRegion[1].Normal = -( arrVertexVoronoiRegion[1].Normal );

                    bEdgeRegion = false;
                    continue;
                }
            }
            return pEdge; // Can't get closer
        } else {
            // Visit vertex region
            pVertex = (const Vertex *)( pFacet->GetVertex(iVertex) );

            // Clip Edge AB against VR(V), EV planes only
            clipResponse = _ClipEdge_Generic( arrVertexVoronoiRegion, 2, vEdgeA, vEdgeB,
                                              &iEnterPlane, &fEnter, &iExitPlane, &fExit );

            // Found an excluding region, last edge is the closest feature
            if ( clipResponse == VCLIP_EXCLUDED_SIMPLE || clipResponse == VCLIP_EXCLUDED_COMPOUND )
                return pEdge;

            // Inclusion case
            if ( clipResponse == VCLIP_INCLUDED )
                return pVertex;

            // Clipped case
            m_pConvex[idConvex]->GetVertexPosition( &vPosition, pVertex );
            if ( iEnterPlane != INVALID_OFFSET ) {
                Int iSign = _EvaluateDerivativeSign( vEdgeA, vEdgeB, fEnter, vPosition );
                if ( iSign == NullDerivationFlag )
                    return RaisedPenetration;
                if ( iSign > 0 ) {
                    iEdge = ( iVertex + iFacetSize - 1 ) % iFacetSize;

                    bEdgeRegion = true;
                    continue;
                }
            }
            if ( iExitPlane != INVALID_OFFSET ) {
                Int iSign = _EvaluateDerivativeSign( vEdgeA, vEdgeB, fExit, vPosition );
                if ( iSign == NullDerivationFlag )
                    return RaisedPenetration;
                if ( iSign < 0 ) {
                    iEdge = iVertex;

                    bEdgeRegion = true;
                    continue;
                }
            }
            return pVertex; // Can't get closer
        }
    }
}

VClipState VoronoiClip::_EscapeLocalMinimum( VClipConvexID idConvex, const MeshFacet * pFacet, const Vertex3 & vPosition )
{
    // Get whole graph
    const MeshVoronoiGraph * pGraph = m_pConvex[idConvex]->GetGraph();

    // Walk through all facets of the polyhedron
    Plane facetPlane;

    Scalar fDistance;
    Scalar fMaxDistance = -SCALAR_INFINITE;
    const MeshFacet * pMaxDistanceFacet = NULL;

    pGraph->EnumFacets();
    const MeshFacet * pTestFacet = pGraph->EnumNextFacet();
    while( pTestFacet != NULL ) {
        if ( pTestFacet == pFacet ) {
            pTestFacet = pGraph->EnumNextFacet();
            continue;
        }

        // Get Facet plane
        m_pConvex[idConvex]->GetFacetPlane( &facetPlane, pTestFacet );

        // Compute max distance facet
        fDistance = facetPlane.DistanceNP( vPosition );
        if ( fDistance > fMaxDistance ) {
            fMaxDistance = fDistance;
            pMaxDistanceFacet = pTestFacet;
        }

        // Next facet
        pTestFacet = pGraph->EnumNextFacet();
    }

    // Move to max distance facet
    Assert( pMaxDistanceFacet != NULL );
    m_pClosestFeature[idConvex] = pMaxDistanceFacet;

    // Found a penetration witness pair
    if ( fMaxDistance < ZeroTolerance )
        return VCLIP_STATE_PENETRATION;

    // Escape local minimum
    return VCLIP_STATE_VF;
}

/////////////////////////////////////////////////////////////////////////////////

VClipState VoronoiClip::_ProcessState_VV( VClipConvexID idConvex0, const Vertex * pVertex0,
                                          VClipConvexID idConvex1, const Vertex * pVertex1 )
{
    // Get Positions
    Vertex3 vPosition0, vPosition1;
    m_pConvex[idConvex0]->GetVertexPosition( &vPosition0, pVertex0 );
    m_pConvex[idConvex1]->GetVertexPosition( &vPosition1, pVertex1 );

    const MeshFeature * pUpdated = NULL;

    // Clip V1 against VR(V0)
    pUpdated = _ClipVertex_V( idConvex0, pVertex0, vPosition1 );
    if ( pUpdated != NULL ) {
        m_pClosestFeature[idConvex0] = pUpdated;

        Assert( pUpdated->GetType() == MESH_FEATURE_EDGE );
        return VCLIP_STATE_VE;
    }

    // Clip V0 against VR(V1)
    pUpdated = _ClipVertex_V( idConvex1, pVertex1, vPosition0 );
    if ( pUpdated != NULL ) {
        m_pClosestFeature[idConvex1] = pUpdated;

        Assert( pUpdated->GetType() == MESH_FEATURE_EDGE );
        return VCLIP_STATE_VE;
    }

    // Found a closest feature pair
    Scalar fSqrDistance = (vPosition1 - vPosition0).NormSqr();
    if ( fSqrDistance < (ZeroTolerance * ZeroTolerance) )
        return VCLIP_STATE_PENETRATION;
    return VCLIP_STATE_SEPARATED;
}
VClipState VoronoiClip::_ProcessState_VE( VClipConvexID idConvex0, const Vertex * pVertex0,
                                          VClipConvexID idConvex1, const Edge * pEdge1 )
{
    // Get Positions
    Vertex3 vPosition0, vEdge1A, vEdge1B;
    m_pConvex[idConvex0]->GetVertexPosition( &vPosition0, pVertex0 );
    m_pConvex[idConvex1]->GetVertexPosition( &vEdge1A, pEdge1->Vertices[0] );
    m_pConvex[idConvex1]->GetVertexPosition( &vEdge1B, pEdge1->Vertices[1] );

    const MeshFeature * pUpdated = NULL;

    // Clip V0 against VR(E1)
    pUpdated = _ClipVertex_E( idConvex1, pEdge1, vPosition0 );
    if ( pUpdated != NULL ) {
        m_pClosestFeature[idConvex1] = pUpdated;

        if ( pUpdated->GetType() == MESH_FEATURE_VERTEX )
            return VCLIP_STATE_VV;
        Assert( pUpdated->GetType() == MESH_FEATURE_FACET );
        return VCLIP_STATE_VF;
    }

    // Clip E1 against VR(V0)
    pUpdated = _ClipEdge_V( idConvex0, pVertex0, vEdge1A, vEdge1B );
    if ( pUpdated == RaisedPenetration )
        return VCLIP_STATE_PENETRATION;
    if ( pUpdated != NULL ) {
        m_pClosestFeature[idConvex0] = pUpdated;

        Assert( pUpdated->GetType() == MESH_FEATURE_EDGE );
        return VCLIP_STATE_EE;
    }

    // Found a closest feature pair
    return VCLIP_STATE_SEPARATED;
}
VClipState VoronoiClip::_ProcessState_VF( VClipConvexID idConvex0, const Vertex * pVertex0,
                                          VClipConvexID idConvex1, const MeshFacet * pFacet1 )
{
    // Get Positions
    Vertex3 vPosition0;
    m_pConvex[idConvex0]->GetVertexPosition( &vPosition0, pVertex0 );

    const MeshFeature * pUpdated = NULL;

    // Clip V0 against VR(F1)
    pUpdated = _ClipVertex_F( idConvex1, pFacet1, vPosition0 );
    if ( pUpdated != NULL ) {
        m_pClosestFeature[idConvex1] = pUpdated;

        Assert( pUpdated->GetType() == MESH_FEATURE_EDGE );
        return VCLIP_STATE_VE;
    }

    // Get facet plane & vertex distance
    Plane facetPlane;
    m_pConvex[idConvex1]->GetFacetPlane( &facetPlane, pFacet1 );
    Scalar fDistanceV = facetPlane.DistanceNP( vPosition0 );

    // Check penetration
    if ( MathFn->EqualsZero(fDistanceV, ZeroTolerance) )
        return VCLIP_STATE_PENETRATION;

    // Compute closest edge on vertex toward facet
    pUpdated = _ClosestEdgeTowardFacet( idConvex0, pVertex0, vPosition0, fDistanceV, facetPlane );
    if ( pUpdated != NULL ) {
        m_pClosestFeature[idConvex0] = pUpdated;

        Assert( pUpdated->GetType() == MESH_FEATURE_EDGE );
        return VCLIP_STATE_EF;
    }

    // Stuck in a local minimum !
    if ( fDistanceV < 0.0f )
        return _EscapeLocalMinimum( idConvex1, pFacet1, vPosition0 );

    // Found a closest feature pair
    return VCLIP_STATE_SEPARATED;
}
VClipState VoronoiClip::_ProcessState_EE( VClipConvexID idConvex0, const Edge * pEdge0,
                                          VClipConvexID idConvex1, const Edge * pEdge1 )
{
    // Get Positions
    Vertex3 vEdge0A, vEdge0B, vEdge1A, vEdge1B;
    m_pConvex[idConvex0]->GetVertexPosition( &vEdge0A, pEdge0->Vertices[0] );
    m_pConvex[idConvex0]->GetVertexPosition( &vEdge0B, pEdge0->Vertices[1] );
    m_pConvex[idConvex1]->GetVertexPosition( &vEdge1A, pEdge1->Vertices[0] );
    m_pConvex[idConvex1]->GetVertexPosition( &vEdge1B, pEdge1->Vertices[1] );

    const MeshFeature * pUpdated = NULL;

    // Clip E0 against VR(E1)
    pUpdated = _ClipEdge_E( idConvex1, pEdge1, vEdge0A, vEdge0B );
    if ( pUpdated == RaisedPenetration )
        return VCLIP_STATE_PENETRATION;
    if ( pUpdated != NULL ) {
        m_pClosestFeature[idConvex1] = pUpdated;

        if ( pUpdated->GetType() == MESH_FEATURE_VERTEX )
            return VCLIP_STATE_VE;
        Assert( pUpdated->GetType() == MESH_FEATURE_FACET );
        return VCLIP_STATE_EF;
    }

    // Clip E1 against VR(E0)
    pUpdated = _ClipEdge_E( idConvex0, pEdge0, vEdge1A, vEdge1B );
    if ( pUpdated == RaisedPenetration )
        return VCLIP_STATE_PENETRATION;
    if ( pUpdated != NULL ) {
        m_pClosestFeature[idConvex0] = pUpdated;

        if ( pUpdated->GetType() == MESH_FEATURE_VERTEX )
            return VCLIP_STATE_VE;
        Assert( pUpdated->GetType() == MESH_FEATURE_FACET );
        return VCLIP_STATE_EF;
    }

    // Found a closest feature pair
    return VCLIP_STATE_SEPARATED;
}
VClipState VoronoiClip::_ProcessState_EF( VClipConvexID idConvex0, const Edge * pEdge0,
                                          VClipConvexID idConvex1, const MeshFacet * pFacet1 )
{
    // Get Positions
    Vertex3 vEdge0A, vEdge0B;
    m_pConvex[idConvex0]->GetVertexPosition( &vEdge0A, pEdge0->Vertices[0] );
    m_pConvex[idConvex0]->GetVertexPosition( &vEdge0B, pEdge0->Vertices[1] );

    const MeshFeature * pUpdated = NULL;

    // Clip E0 against VR(F1)
    VClipResponse clipResponse;
    UInt iEnterPlane, iExitPlane;
    Scalar fEnter, fExit;

    clipResponse = _ClipEdge_F( idConvex1, pFacet1, vEdge0A, vEdge0B,
                                &iEnterPlane, &fEnter, &iExitPlane, &fExit );

    // Simple / Compound exclusion case
    if ( clipResponse == VCLIP_EXCLUDED_SIMPLE || clipResponse == VCLIP_EXCLUDED_COMPOUND ) {
        Assert( iEnterPlane != INVALID_OFFSET );

        // Compute closest vertex/edge on facet toward edge
        pUpdated = _ClosestFeatureTowardEdge( idConvex1, pFacet1, iEnterPlane, vEdge0A, vEdge0B );
        if ( pUpdated == RaisedPenetration )
            return VCLIP_STATE_PENETRATION;

        Assert( pUpdated != NULL );
        m_pClosestFeature[idConvex1] = pUpdated;

        if ( pUpdated->GetType() == MESH_FEATURE_VERTEX )
            return VCLIP_STATE_VE;
        Assert( pUpdated->GetType() == MESH_FEATURE_EDGE );
        return VCLIP_STATE_EE;
    }

    // Get facet plane
    Plane facetPlane;
    m_pConvex[idConvex1]->GetFacetPlane( &facetPlane, pFacet1 );

    // Check penetration
    Vertex3 vEnter = vEdge0A;
    if ( iEnterPlane != INVALID_OFFSET )
        vEnter += (vEdge0B - vEdge0A) * fEnter;
    Vertex3 vExit = vEdge0B;
    if ( iExitPlane != INVALID_OFFSET )
        vExit -= (vEdge0B - vEdge0A) * (1.0f - fExit);

    Scalar fDistanceEnter = facetPlane.DistanceNP( vEnter );
    Scalar fDistanceExit = facetPlane.DistanceNP( vExit );
    if ( (fDistanceEnter < ZeroTolerance) && (fDistanceExit > -ZeroTolerance) )
        return VCLIP_STATE_PENETRATION;
    if ( (fDistanceEnter > -ZeroTolerance) && (fDistanceExit < ZeroTolerance) )
        return VCLIP_STATE_PENETRATION;

    // Clipped / Inclusion cases
    if ( iEnterPlane != INVALID_OFFSET ) {
        Int iSign = _EvaluateDerivativeSign( vEdge0A, vEdge0B, fEnter, facetPlane );
        if ( iSign == NullDerivationFlag )
            return VCLIP_STATE_PENETRATION;
        if ( iSign > 0 ) {
            m_pClosestFeature[idConvex1] = pFacet1->GetEdge( iEnterPlane );
            return VCLIP_STATE_EE;
        }
    }
    if ( iExitPlane != INVALID_OFFSET ) {
        Int iSign = _EvaluateDerivativeSign( vEdge0A, vEdge0B, fExit, facetPlane );
        if ( iSign == NullDerivationFlag )
            return VCLIP_STATE_PENETRATION;
        if ( iSign < 0 ) {
            m_pClosestFeature[idConvex1] = pFacet1->GetEdge( iExitPlane );
            return VCLIP_STATE_EE;
        }
    }
    if ( MathFn->Abs(fDistanceEnter) <= MathFn->Abs(fDistanceExit) )
        m_pClosestFeature[idConvex0] = pEdge0->Vertices[0];
    else
        m_pClosestFeature[idConvex0] = pEdge0->Vertices[1];
    return VCLIP_STATE_VF;
}

// Debug
Void VoronoiClip::_DebugPrint_Header() const
{
    if ( m_vcState == VCLIP_STATE_PENETRATION )
        DebugPrint( TEXT("V-Clip result = Penetration\n") );
    else {
        Assert( m_vcState == VCLIP_STATE_SEPARATED );
        DebugPrint( TEXT("V-Clip result = Separated\n") );
    }
}
Void VoronoiClip::_DebugPrint_ClosestFeatures() const
{
    const MeshFeature * pFeatureA = m_pClosestFeature[VCLIP_CONVEX_A];
    const MeshFeature * pFeatureB = m_pClosestFeature[VCLIP_CONVEX_B];
    const Vertex * pVertex;
    const Edge * pEdge;
    const MeshFacet * pFacet;
    const Triangle * pTriangle;
    const Quad * pQuad;
    const Polygon * pPolygon;

    // Closest feature on A
    switch( pFeatureA->GetType() ) {
        case MESH_FEATURE_VERTEX:
            pVertex = (const Vertex *)pFeatureA;
            DebugPrint( TEXT("\t Closest feature A = Vertex(%d)\n"), pVertex->Key.V );
            break;
        case MESH_FEATURE_EDGE:
            pEdge = (const Edge *)pFeatureA;
            DebugPrint( TEXT("\t Closest feature A = Edge(%d,%d)\n"), pEdge->Key.A, pEdge->Key.B );
            break;
        case MESH_FEATURE_FACET:
            pFacet = (const MeshFacet *)pFeatureA;
            switch( pFacet->GetFacetType() ) {
                case MESH_FACET_TRIANGLE:
                    pTriangle = (const Triangle *)pFacet;
                    DebugPrint( TEXT("\t Closest feature A = Triangle(%d,%d,%d)\n"), pTriangle->Key.A, pTriangle->Key.B, pTriangle->Key.C );
                    break;
                case MESH_FACET_QUAD:
                    pQuad = (const Quad *)pFacet;
                    DebugPrint( TEXT("\t Closest feature A = Quad(%d,%d,%d,%d)\n"), pQuad->Key.A, pQuad->Key.B, pQuad->Key.C, pQuad->Key.D );
                    break;
                case MESH_FACET_POLYGON:
                    pPolygon = (const Polygon *)pFacet;
                    DebugPrint( TEXT("\t Closest feature A = Polygon(%d"), pPolygon->Key.V[0] );
                    for( UInt i = 1; i < pPolygon->GetVertexCount(); ++i )
                        DebugPrint( TEXT(",%d"), pPolygon->Key.V[i] );
                    DebugPrint( TEXT(")\n") );
                    break;
                default: Assert(false); break;
            } break;
        default: Assert(false); break;
    }

    // Closest feature on B
    switch( pFeatureB->GetType() ) {
        case MESH_FEATURE_VERTEX:
            pVertex = (const Vertex *)pFeatureB;
            DebugPrint( TEXT("\t Closest feature B = Vertex(%d)\n"), pVertex->Key.V );
            break;
        case MESH_FEATURE_EDGE:
            pEdge = (const Edge *)pFeatureB;
            DebugPrint( TEXT("\t Closest feature B = Edge(%d,%d)\n"), pEdge->Key.A, pEdge->Key.B );
            break;
        case MESH_FEATURE_FACET:
            pFacet = (MeshFacet*)pFeatureB;
            switch( pFacet->GetFacetType() ) {
                case MESH_FACET_TRIANGLE:
                    pTriangle = (const Triangle *)pFacet;
                    DebugPrint( TEXT("\t Closest feature B = Triangle(%d,%d,%d)\n"), pTriangle->Key.A, pTriangle->Key.B, pTriangle->Key.C );
                    break;
                case MESH_FACET_QUAD:
                    pQuad = (const Quad *)pFacet;
                    DebugPrint( TEXT("\t Closest feature B = Quad(%d,%d,%d,%d)\n"), pQuad->Key.A, pQuad->Key.B, pQuad->Key.C, pQuad->Key.D );
                    break;
                case MESH_FACET_POLYGON:
                    pPolygon = (const Polygon *)pFacet;
                    DebugPrint( TEXT("\t Closest feature B = Polygon(%d"), pPolygon->Key.V[0] );
                    for( UInt i = 1; i < pPolygon->GetVertexCount(); ++i )
                        DebugPrint( TEXT(",%d"), pPolygon->Key.V[i] );
                    DebugPrint( TEXT(")\n") );
                    break;
                default: Assert(false); break;
            } break;
        default: Assert(false); break;
    }
}
Void VoronoiClip::_DebugPrint_ContactPoints() const
{
    // Contact manifold
    SinglePlaneManifold * pCollisionManifold = (SinglePlaneManifold*)m_pCollisionManifold;

    DebugPrint( TEXT("Detected contact manifold : (Count = %d)\n"), pCollisionManifold->GetContactPointCount() );
    for( UInt i = 0; i < pCollisionManifold->GetContactPointCount(); ++i ) {
        const ContactPoint * pCP = pCollisionManifold->GetContactPoint(i);

        const MeshFeature * pFeatA = (const MeshFeature *)( pCP->idCPID.dwFeatureA );
        const MeshFeature * pFeatB = (const MeshFeature *)( pCP->idCPID.dwFeatureB );

        UInt iKeyA = 0;
        switch( pFeatA->GetType() ) {
            case MESH_FEATURE_VERTEX: iKeyA  = ( ((const Vertex *)pFeatA)->Key.V ); break;
            case MESH_FEATURE_EDGE:   iKeyA  = ( ((const Edge *)pFeatA)->Key.A ) << 16;
                                      iKeyA |= ( ((const Edge *)pFeatA)->Key.B ); break;
            case MESH_FEATURE_FACET:
                switch( ((const MeshFacet *)pFeatA)->GetFacetType() ) {
                    case MESH_FACET_TRIANGLE: iKeyA  = ( ((const Triangle *)pFeatA)->Key.A ) << 16;
                                              iKeyA |= ( ((const Triangle *)pFeatA)->Key.B ) << 8;
                                              iKeyA |= ( ((const Triangle *)pFeatA)->Key.C ); break;
                    case MESH_FACET_QUAD:     iKeyA  = ( ((const Quad *)pFeatA)->Key.A ) << 24;
                                              iKeyA |= ( ((const Quad *)pFeatA)->Key.B ) << 16;
                                              iKeyA |= ( ((const Quad *)pFeatA)->Key.C ) << 8;
                                              iKeyA |= ( ((const Quad *)pFeatA)->Key.D ); break;
                    case MESH_FACET_POLYGON:  iKeyA = 0;
                                              for( Int i = ((const Polygon *)pFeatA)->Key.SIZE - 1; i >= 0; --i )
                                                  iKeyA |= ( ((const Polygon *)pFeatA)->Key.V[i] ) << (4*i);
                                              break;
                    default: Assert( false ); break;
                } break;
            default: Assert( false ); break;
        }
        UInt iKeyB = 0;
        switch( pFeatB->GetType() ) {
            case MESH_FEATURE_VERTEX: iKeyB  = ( ((const Vertex *)pFeatB)->Key.V ); break;
            case MESH_FEATURE_EDGE:   iKeyB  = ( ((const Edge *)pFeatB)->Key.A ) << 16;
                                      iKeyB |= ( ((const Edge *)pFeatB)->Key.B ); break;
            case MESH_FEATURE_FACET:
                switch( ((const MeshFacet *)pFeatB)->GetFacetType() ) {
                    case MESH_FACET_TRIANGLE: iKeyB  = ( ((const Triangle *)pFeatB)->Key.A ) << 16;
                                              iKeyB |= ( ((const Triangle *)pFeatB)->Key.B ) << 8;
                                              iKeyB |= ( ((const Triangle *)pFeatB)->Key.C ); break;
                    case MESH_FACET_QUAD:     iKeyB  = ( ((const Quad *)pFeatB)->Key.A ) << 24;
                                              iKeyB |= ( ((const Quad *)pFeatB)->Key.B ) << 16;
                                              iKeyB |= ( ((const Quad *)pFeatB)->Key.C ) << 8;
                                              iKeyB |= ( ((const Quad *)pFeatB)->Key.D ); break;
                    case MESH_FACET_POLYGON:  iKeyB = 0;
                                              for( Int i = ((const Polygon *)pFeatB)->Key.SIZE - 1; i >= 0; --i )
                                                  iKeyB |= ( ((const Polygon *)pFeatB)->Key.V[i] ) << (4*i);
                                              break;
                    default: Assert( false ); break;
                } break;
            default: Assert( false ); break;
        }

        DebugPrint( TEXT("\t -> (ID=%x/%x) : Features(%8x,%8x), Distance(%1f)\n"),
                    pCP->idCPID.dwFeatureA, pCP->idCPID.dwFeatureB, iKeyA, iKeyB, pCP->fDistance );
    }
    DebugPrint( TEXT("\n") );
}
// Debug
