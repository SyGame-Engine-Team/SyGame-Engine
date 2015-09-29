/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/CollisionDetection/NarrowPhase/NarrowPhase.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Narrow-Phase layer for collision detection
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
#include "NarrowPhase.h"

#include "../../PhysicsManager.h"

/////////////////////////////////////////////////////////////////////////////////
// NarrowPhase implementation
NarrowPhase::NarrowPhase()
{
    ZeroTolerance = NARROWPHASE_ZEROTOLERANCE;
}
NarrowPhase::~NarrowPhase()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// NarrowPhaseConvex implementation
NarrowPhaseConvex::NarrowPhaseConvex( TransformedShapeConvex * pShapeA, TransformedShapeConvex * pShapeB ):
    NarrowPhase()
{
    m_pShapeA = pShapeA;
    m_pShapeB = pShapeB;

    PhysicsFn->SelectMemory( TEXT("Narrowphase") );
    m_pCollisionManifold = New SinglePlaneManifold();
    PhysicsFn->UnSelectMemory();
}
NarrowPhaseConvex::~NarrowPhaseConvex()
{
    PhysicsFn->SelectMemory( TEXT("Narrowphase") );
    Delete( m_pCollisionManifold );
    PhysicsFn->UnSelectMemory();
}

/////////////////////////////////////////////////////////////////////////////////

Void NarrowPhaseConvex::_GenerateContactPoints_ClipPolygons( Vertex3 * outClippedPolygon, ContactPointID * outClippedCPIDs, UInt * outClippedPolygonSize, const Vector3 & vSeparatingAxis,
                                                             const Vertex3 * arrPolyA, const ContactPointID * arrCPIDsA, UInt iPolygonASize,
                                                             const Vertex3 * arrPolyB, const ContactPointID * arrCPIDsB, UInt iPolygonBSize )
{
    static Vertex3 arrScratch0[NARROWPHASE_CONTACTPOLYGONS_MAX_VERTICES << 1];
    static Vertex3 arrScratch1[NARROWPHASE_CONTACTPOLYGONS_MAX_VERTICES << 1];
    static ContactPointID arrScratch0CPIDs[NARROWPHASE_CONTACTPOLYGONS_MAX_VERTICES << 1];
    static ContactPointID arrScratch1CPIDs[NARROWPHASE_CONTACTPOLYGONS_MAX_VERTICES << 1];

    UInt iSrcPolygonSize = iPolygonASize;
    UInt iDstPolygonSize = 0;
    Vertex3 * arrSrcPolygon = arrScratch0;
    ContactPointID * arrSrcCPIDs = arrScratch0CPIDs;
    Vertex3 * arrDestPolygon = arrScratch1;
    ContactPointID * arrDestCPIDs = arrScratch1CPIDs;
    Vertex3 * arrPolySwap;
    ContactPointID * arrCPIDsSwap;
    for( UInt i = 0; i < iSrcPolygonSize; ++i ) {
        arrSrcPolygon[i] = arrPolyA[i];
        arrSrcCPIDs[i] = arrCPIDsA[i];
    }

    Vertex3 vVertexB0, vVertexB1;
    Vector3 vEdgeB;
    Plane vClipPlane;

    Vertex3 vVertexA0, vVertexA1;
    Scalar fDistance0, fDistance1, fT;

    // Walk through all edges of polygon B
    vVertexB0 = arrPolyB[iPolygonBSize - 1];

    for( UInt i = 0; i < iPolygonBSize; ++i ) {
        vVertexB1 = arrPolyB[i];
        vEdgeB = ( vVertexB1 - vVertexB0 );
        
        // Compute clip plane for that edge
        vClipPlane.Normal = ( vSeparatingAxis ^ vEdgeB );
        vClipPlane.Normal.Normalize();
        vClipPlane.Position = vVertexB0;
        vClipPlane.UpdateConstant();

        // Clip polygon A against that plane, walk through all edges of polygon A
        vVertexA0 = arrSrcPolygon[iSrcPolygonSize - 1];
        fDistance0 = vClipPlane.DistanceNP( vVertexA0 );

        for( UInt j = 0; j < iSrcPolygonSize; ++j ) {
            vVertexA1 = arrSrcPolygon[j];
            fDistance1 = vClipPlane.DistanceNP( vVertexA1 );

            if ( fDistance0 > 0.0f ) {
                if ( fDistance1 > 0.0f ) {
                    // 0 & 1 on positive side of plane, output 1
                    arrDestPolygon[iDstPolygonSize] = vVertexA1;
                    arrDestCPIDs[iDstPolygonSize] = arrSrcCPIDs[j];
                    ++iDstPolygonSize;
                } else {
                    // 0 on positive side, 1 on negative side, output intersection
                    fT = ( fDistance0 / (fDistance0 - fDistance1) );
                    arrDestPolygon[iDstPolygonSize] = vVertexA0 + ( (vVertexA1 - vVertexA0) * fT );
                    arrDestCPIDs[iDstPolygonSize].dwFeatureA = arrSrcCPIDs[j].dwFeatureA;
                    arrDestCPIDs[iDstPolygonSize].dwSpecifierA = arrSrcCPIDs[j].dwSpecifierA;
                    arrDestCPIDs[iDstPolygonSize].dwFeatureB = arrCPIDsB[i].dwFeatureB;
                    arrDestCPIDs[iDstPolygonSize].dwSpecifierB = arrCPIDsB[i].dwSpecifierB;
                    ++iDstPolygonSize;
                }
            } else {
                if ( fDistance1 > 0.0f ) {
                    // 0 on negative side, 1 on positive side, output intersection & 1
                    fT = ( fDistance0 / (fDistance0 - fDistance1) );
                    arrDestPolygon[iDstPolygonSize] = vVertexA0 + ( (vVertexA1 - vVertexA0) * fT );
                    arrDestCPIDs[iDstPolygonSize].dwFeatureA = arrSrcCPIDs[j].dwFeatureA;
                    arrDestCPIDs[iDstPolygonSize].dwSpecifierA = arrSrcCPIDs[j].dwSpecifierA;
                    arrDestCPIDs[iDstPolygonSize].dwFeatureB = arrCPIDsB[i].dwFeatureB;
                    arrDestCPIDs[iDstPolygonSize].dwSpecifierB = arrCPIDsB[i].dwSpecifierB;
                    ++iDstPolygonSize;

                    arrDestPolygon[iDstPolygonSize] = vVertexA1;
                    arrDestCPIDs[iDstPolygonSize] = arrSrcCPIDs[j];
                    ++iDstPolygonSize;
                }
                // else 0 & 1 on negative side, output nothing
            }

            // Next edge on A
            vVertexA0 = vVertexA1;
            fDistance0 = fDistance1;
        }

        // Swap input/output polygon & CPID arrays
        arrPolySwap = arrSrcPolygon;
        arrSrcPolygon = arrDestPolygon;
        arrDestPolygon = arrPolySwap;
        arrCPIDsSwap = arrSrcCPIDs;
        arrSrcCPIDs = arrDestCPIDs;
        arrDestCPIDs = arrCPIDsSwap;
        iSrcPolygonSize = iDstPolygonSize;
        iDstPolygonSize = 0;

        // Next edge on B
        vVertexB0 = vVertexB1;
    }

    // Done
    for( UInt i = 0; i < iSrcPolygonSize; ++i ) {
        outClippedPolygon[i] = arrSrcPolygon[i];
        outClippedCPIDs[i] = arrSrcCPIDs[i];
    }
    *outClippedPolygonSize = iSrcPolygonSize;
}

Void NarrowPhaseConvex::_GenerateContactPoints_NormalPerturbation( Vertex3 * outPolyA, ContactPointID * outCPIDsA, Vertex3 * outPolyB, ContactPointID * outCPIDsB,
                                                                   const Vector3 & vSeparatingAxis, Scalar fPerturbationOffset, UInt iSampleCount )
{
    const Scalar s_fAngleStep = ( SCALAR_2PI / (Scalar)iSampleCount );

    // Build a basis for normal perturbation
    Vector3 vU, vV;
    Vector3::MakeComplementBasis( vU, vV, vSeparatingAxis );

    // Encode separating axis for CPIDs (poor man's solution)
    FloatConverter hFC;
    hFC.f = vSeparatingAxis.X;
    DWord dwFeature = hFC.i;
    hFC.f = vSeparatingAxis.Y;
    dwFeature ^= hFC.i;
    hFC.f = vSeparatingAxis.Z;
    dwFeature ^= hFC.i;

    // Scan (u,v)-plane radially, counter clockwise
    Scalar fTheta = 0.0f;
    Scalar fSin, fCos;
    Vector3 vPerturbation, vPerturbedAxis;

    for( UInt i = 0; i < iSampleCount; ++i ) {
        // Get sampling direction in (u,v) coordinates
        MathFn->SinCos( &fSin, &fCos, fTheta );

        // Compute perturbed axis
        vPerturbation = ( ( vU * fCos ) + ( vV * fSin ) ); // Norm = 1
        vPerturbation *= fPerturbationOffset;

        vPerturbedAxis = ( vSeparatingAxis + vPerturbation );
        vPerturbedAxis.Normalize();

        // Get a sample point on each shape (vSeparatingAxis allways points B->A)
        m_pShapeA->QueryExtremalVertices( -vPerturbedAxis, outPolyA + i, NULL );
        m_pShapeB->QueryExtremalVertices( vPerturbedAxis, outPolyB + i, NULL );

        

        outCPIDsA[i].dwFeatureA = dwFeature;
        outCPIDsA[i].dwSpecifierA = i;
        outCPIDsA[i].dwFeatureB = 0;
        outCPIDsA[i].dwSpecifierB = 0;

        outCPIDsB[i].dwFeatureA = 0;
        outCPIDsB[i].dwSpecifierA = 0;
        outCPIDsB[i].dwFeatureB = dwFeature;
        outCPIDsB[i].dwSpecifierB = i;

        // Next sampling direction
        fTheta += s_fAngleStep;
    }
}

const MeshFacet * _SearchMostParallelFacet( const TransformedShapeConvex * pShape, const MeshVertex * pVertex, const Vector3 & vNormal )
{
    const MeshFacet * pMostParallelFacet = NULL;
    const MeshEdge *pCurEdge, *pNextEdge;
    const MeshFacet * pFacet;
    Vector3 vFacetNormal;
    Scalar fMaxAbsDot = 0.0f;
    Scalar fAbsDot;

    // All surrounding faces = facets adjacent to edges from the vertex
    UInt iEdgeCount = pVertex->GetEdgeCount();
    for( UInt i = 0; i < iEdgeCount; ++i ) {
        pCurEdge = pVertex->GetEdge(i);
        pNextEdge = pVertex->GetEdge( (i+1) % iEdgeCount );

        pFacet = pCurEdge->GetFacet(1);
        if ( pFacet->FindEdge(pNextEdge) == INVALID_OFFSET )
            pFacet = pCurEdge->GetFacet(0);

        pShape->GetFacetNormal( &vFacetNormal, pFacet );

        fAbsDot = MathFn->Abs( vNormal * vFacetNormal );
        if ( fAbsDot > fMaxAbsDot ) {
            pMostParallelFacet = pFacet;
            fMaxAbsDot = fAbsDot;
        }
    }

    Assert( pMostParallelFacet != NULL );
    return pMostParallelFacet;
}
Void NarrowPhaseConvex::_GenerateContactPoints_MostParallelFacet( Vertex3 * outPolyA, ContactPointID * outCPIDsA, UInt * outPolyASize,
                                                                  Vertex3 * outPolyB, ContactPointID * outCPIDsB, UInt * outPolyBSize, Vector3 * outSeparatingAxis,
                                                                  const MeshFeature * pClosestFeatureA, const MeshFeature * pClosestFeatureB )
{
    const MeshFacet * pFacetA = NULL;
    const MeshFacet * pFacetB = NULL;
    const MeshVertex * pVertexA0 = NULL;
    const MeshVertex * pVertexA1 = NULL;
    const MeshVertex * pVertexB0 = NULL;
    const MeshVertex * pVertexB1 = NULL;
    Vertex3 vA, vB, vTmpA, vTmpB;

    // Extract closest facets
    switch( pClosestFeatureA->GetType() ) {
        case MESH_FEATURE_VERTEX: {
                switch( pClosestFeatureB->GetType() ) {
                    case MESH_FEATURE_VERTEX: {
                            // Vertex-Vertex case
                            m_pShapeA->GetVertexPosition( &vA, (const MeshVertex *)pClosestFeatureA );
                            m_pShapeB->GetVertexPosition( &vB, (const MeshVertex *)pClosestFeatureB );

                            *outSeparatingAxis = ( vA - vB );
                            outSeparatingAxis->Normalize();

                            pFacetA = _SearchMostParallelFacet( m_pShapeA, (const MeshVertex *)pClosestFeatureA, *outSeparatingAxis );
                            pFacetB = _SearchMostParallelFacet( m_pShapeB, (const MeshVertex *)pClosestFeatureB, *outSeparatingAxis );
                        } break;
                    case MESH_FEATURE_EDGE: {
                            // Vertex-Edge case
                            m_pShapeA->GetVertexPosition( &vA, (const MeshVertex *)pClosestFeatureA );
                            pVertexB0 = ((const MeshEdge *)pClosestFeatureB)->GetVertex(0);
                            pVertexB1 = ((const MeshEdge *)pClosestFeatureB)->GetVertex(1);
                            m_pShapeB->GetVertexPosition( &vB, pVertexB0 );
                            m_pShapeB->GetVertexPosition( &vTmpB, pVertexB1 );

                            Segment3 vSegB( vB, vTmpB );
                            vB = vSegB.ClosestPoint( vA );
                            Scalar fT = vSegB.GetParameterCDE( vB );

                            *outSeparatingAxis = ( vA - vB );
                            outSeparatingAxis->Normalize();

                            pFacetA = _SearchMostParallelFacet( m_pShapeA, (const MeshVertex *)pClosestFeatureA, *outSeparatingAxis );
                            if ( fT < 0.0f )
                                pFacetB = _SearchMostParallelFacet( m_pShapeB, pVertexB0, *outSeparatingAxis );
                            else
                                pFacetB = _SearchMostParallelFacet( m_pShapeB, pVertexB1, *outSeparatingAxis );
                        } break;
                    case MESH_FEATURE_FACET: {
                            // Vertex-Facet case
                            pFacetB = (const MeshFacet *)pClosestFeatureB;
                            m_pShapeB->GetFacetNormal( outSeparatingAxis, pFacetB );

                            pFacetA = _SearchMostParallelFacet( m_pShapeA, (const MeshVertex *)pClosestFeatureA, *outSeparatingAxis );
                        } break;
                    default: Assert(false); break;
                }
            } break;
        case MESH_FEATURE_EDGE: {
                switch( pClosestFeatureB->GetType() ) {
                    case MESH_FEATURE_VERTEX: {
                            // Edge-Vertex case
                            pVertexA0 = ((const MeshEdge *)pClosestFeatureA)->GetVertex(0);
                            pVertexA1 = ((const MeshEdge *)pClosestFeatureA)->GetVertex(1);
                            m_pShapeA->GetVertexPosition( &vA, pVertexA0 );
                            m_pShapeA->GetVertexPosition( &vTmpA, pVertexA1 );
                            m_pShapeB->GetVertexPosition( &vB, (const MeshVertex *)pClosestFeatureB );

                            Segment3 vSegA( vA, vTmpA );
                            vA = vSegA.ClosestPoint( vB );
                            Scalar fT = vSegA.GetParameterCDE( vA );

                            *outSeparatingAxis = ( vA - vB );
                            outSeparatingAxis->Normalize();

                            if ( fT < 0.0f )
                                pFacetA = _SearchMostParallelFacet( m_pShapeA, pVertexA0, *outSeparatingAxis );
                            else
                                pFacetA = _SearchMostParallelFacet( m_pShapeA, pVertexA1, *outSeparatingAxis );
                            pFacetB = _SearchMostParallelFacet( m_pShapeB, (const MeshVertex *)pClosestFeatureB, *outSeparatingAxis );
                        } break;
                    case MESH_FEATURE_EDGE: {
                            // Edge-Edge case
                            pVertexA0 = ((const MeshEdge *)pClosestFeatureA)->GetVertex(0);
                            pVertexA1 = ((const MeshEdge *)pClosestFeatureA)->GetVertex(1);
                            pVertexB0 = ((const MeshEdge *)pClosestFeatureB)->GetVertex(0);
                            pVertexB1 = ((const MeshEdge *)pClosestFeatureB)->GetVertex(1);
                            m_pShapeA->GetVertexPosition( &vA, pVertexA0 );
                            m_pShapeA->GetVertexPosition( &vTmpA, pVertexA1 );
                            m_pShapeB->GetVertexPosition( &vB, pVertexB0 );
                            m_pShapeB->GetVertexPosition( &vTmpB, pVertexB1 );

                            Segment3 vSegA( vA, vTmpA );
                            Segment3 vSegB( vB, vTmpB );
                            IntersectSegment3 hIntersection( vSegA, vSegB, ZeroTolerance );
                            hIntersection.Compute();
                            vA = hIntersection.GetClosestPointA();
                            vB = hIntersection.GetClosestPointB();
                            Scalar fTA = vSegA.GetParameterCDE( vA );
                            Scalar fTB = vSegB.GetParameterCDE( vB );

                            *outSeparatingAxis = ( vA - vB );
                            outSeparatingAxis->Normalize();

                            if ( fTA < 0.0f )
                                pFacetA = _SearchMostParallelFacet( m_pShapeA, pVertexA0, *outSeparatingAxis );
                            else
                                pFacetA = _SearchMostParallelFacet( m_pShapeA, pVertexA1, *outSeparatingAxis );
                            if ( fTB < 0.0f )
                                pFacetB = _SearchMostParallelFacet( m_pShapeB, pVertexB0, *outSeparatingAxis );
                            else
                                pFacetB = _SearchMostParallelFacet( m_pShapeB, pVertexB1, *outSeparatingAxis );
                        } break;
                    case MESH_FEATURE_FACET: {
                            // Edge-Facet case
                            pVertexA0 = ((const MeshEdge *)pClosestFeatureA)->GetVertex(0);
                            pVertexA1 = ((const MeshEdge *)pClosestFeatureA)->GetVertex(1);
                            m_pShapeA->GetVertexPosition( &vA, pVertexA0 );
                            m_pShapeA->GetVertexPosition( &vTmpA, pVertexA1 );
                            pFacetB = (const MeshFacet *)pClosestFeatureB;

                            Plane vFacetPlane;
                            m_pShapeB->GetFacetPlane( &vFacetPlane, pFacetB );

                            Scalar fDist0 = vFacetPlane.DistanceNP( vA );
                            Scalar fDist1 = vFacetPlane.DistanceNP( vTmpA );

                            if ( fDist0 < fDist1 )
                                pFacetA = _SearchMostParallelFacet( m_pShapeA, pVertexA0, vFacetPlane.Normal );
                            else
                                pFacetA = _SearchMostParallelFacet( m_pShapeA, pVertexA1, vFacetPlane.Normal );
                        } break;
                    default: Assert(false); break;
                }
            } break;
        case MESH_FEATURE_FACET: {
                pFacetA = (const MeshFacet *)pClosestFeatureA;
                switch( pClosestFeatureB->GetType() ) {
                    case MESH_FEATURE_VERTEX: {
                            // Facet-Vertex case
                            m_pShapeA->GetFacetNormal( outSeparatingAxis, pFacetA );

                            pFacetB = _SearchMostParallelFacet( m_pShapeB, (const MeshVertex *)pClosestFeatureB, *outSeparatingAxis );
                        } break;
                    case MESH_FEATURE_EDGE: {
                            // Facet-Edge case
                            pVertexB0 = ((const MeshEdge *)pClosestFeatureB)->GetVertex(0);
                            pVertexB1 = ((const MeshEdge *)pClosestFeatureB)->GetVertex(1);
                            m_pShapeB->GetVertexPosition( &vB, pVertexB0 );
                            m_pShapeB->GetVertexPosition( &vTmpB, pVertexB1 );

                            Plane vFacetPlane;
                            m_pShapeA->GetFacetPlane( &vFacetPlane, pFacetA );

                            Scalar fDist0 = vFacetPlane.DistanceNP( vB );
                            Scalar fDist1 = vFacetPlane.DistanceNP( vTmpB );

                            if ( fDist0 < fDist1 )
                                pFacetB = _SearchMostParallelFacet( m_pShapeB, pVertexB0, vFacetPlane.Normal );
                            else
                                pFacetB = _SearchMostParallelFacet( m_pShapeB, pVertexB1, vFacetPlane.Normal );
                        } break;
                    case MESH_FEATURE_FACET: {
                            // Facet-Facet case
                            pFacetB = (const MeshFacet *)pClosestFeatureB;
                        } break;
                    default: Assert(false); break;
                }
            } break;
        default: Assert(false); break;
    }

    // Extract facet A's polygon
    *outPolyASize = pFacetA->GetVertexCount();
    for( UInt i = 0; i < *outPolyASize; ++i ) {
        m_pShapeA->GetVertexPosition( outPolyA + i, pFacetA->GetVertex(i) );
        outCPIDsA[i].dwFeatureA = (DWord)pFacetA;
        outCPIDsA[i].dwSpecifierA = pFacetA->GetVertex(i)->Key.V;
        outCPIDsA[i].dwFeatureB = 0;
        outCPIDsA[i].dwSpecifierB = 0;
    }

    // Extract facet B's polygon
    *outPolyBSize = pFacetB->GetVertexCount();
    for( UInt i = 0; i < *outPolyBSize; ++i ) {
        m_pShapeB->GetVertexPosition( outPolyB + i, pFacetB->GetVertex(i) );
        outCPIDsB[i].dwFeatureA = 0;
        outCPIDsB[i].dwSpecifierA = 0;
        outCPIDsB[i].dwFeatureB = (DWord)pFacetB;
        outCPIDsB[i].dwSpecifierB = pFacetB->GetVertex(i)->Key.V;
    }

    // Use normal from B as separating axis
    m_pShapeB->GetFacetNormal( outSeparatingAxis, pFacetB );
}

Void NarrowPhaseConvex::_GenerateContactPoints( const Vector3 & vSeparatingAxis, Scalar fMinDistance, Scalar fMaxDistance )
{
    // Generate virtual facets using normal perturbation
    Vertex3 arrPolyA[NARROWPHASE_NORMALPERTURBATION_SAMPLECOUNT];
    Vertex3 arrPolyB[NARROWPHASE_NORMALPERTURBATION_SAMPLECOUNT];
    ContactPointID arrCPIDsA[NARROWPHASE_NORMALPERTURBATION_SAMPLECOUNT];
    ContactPointID arrCPIDsB[NARROWPHASE_NORMALPERTURBATION_SAMPLECOUNT];

    _GenerateContactPoints_NormalPerturbation( arrPolyA, arrCPIDsA, arrPolyB, arrCPIDsB, vSeparatingAxis );

    // Get plane for facet on B
    Vector3 vEdge0 = ( arrPolyB[1] - arrPolyB[0] );
    Vector3 vEdge1 = ( arrPolyB[NARROWPHASE_NORMALPERTURBATION_SAMPLECOUNT >> 1] - arrPolyB[0] );
    Vector3 vFacetNormalB = ( vEdge0 ^ vEdge1 );
    vFacetNormalB.Normalize();

    Plane vFacetPlaneB( vFacetNormalB, arrPolyB[0] );

    // Clip polygons against each other
    UInt iClippedPolygonSize = 0;
    Vertex3 arrClippedPoly[NARROWPHASE_NORMALPERTURBATION_SAMPLECOUNT << 1];
    ContactPointID arrClippedCPIDs[NARROWPHASE_NORMALPERTURBATION_SAMPLECOUNT << 1];
    _GenerateContactPoints_ClipPolygons( arrClippedPoly, arrClippedCPIDs, &iClippedPolygonSize, vSeparatingAxis, arrPolyA, arrCPIDsA, NARROWPHASE_NORMALPERTURBATION_SAMPLECOUNT,
                                                                                                                 arrPolyB, arrCPIDsB, NARROWPHASE_NORMALPERTURBATION_SAMPLECOUNT );

    // Generate contact points
    m_pCollisionManifold->Clear();

    for( UInt i = 0; i < iClippedPolygonSize; ++i ) {
        // Get points on B, intersection of lines // to sep axis going
        // through A's points and B's facet's plane.
        Line3 vLine( arrClippedPoly[i], -vSeparatingAxis );
        
        IntersectPlaneLine3 hIntersection( vFacetPlaneB, vLine, ZeroTolerance );
        Bool bIntersection = hIntersection.Compute();
        Assert( bIntersection );

        // Add contact point
        Scalar fDistance = hIntersection.GetLineParameter();
        if ( fDistance < fMinDistance )
            fDistance = fMinDistance;
        if ( fDistance > fMaxDistance )
            continue;

        Vertex3 vA = arrClippedPoly[i];
        Vertex3 vB = vLine.GetPoint( fDistance );

        m_pCollisionManifold->AddPoint( arrClippedCPIDs[i], vA, vB, vSeparatingAxis, fDistance );
    }
}
Void NarrowPhaseConvex::_GenerateContactPoints( const MeshFeature * pClosestFeatureA, const MeshFeature * pClosestFeatureB, Scalar fMinDistance, Scalar fMaxDistance )
{
    // Retrieve most parallel facets for intersection
    UInt iPolyASize = 0, iPolyBSize = 0;
    Vertex3 arrPolyA[NARROWPHASE_CONTACTPOLYGONS_MAX_VERTICES];
    Vertex3 arrPolyB[NARROWPHASE_CONTACTPOLYGONS_MAX_VERTICES];
    ContactPointID arrCPIDsA[NARROWPHASE_CONTACTPOLYGONS_MAX_VERTICES];
    ContactPointID arrCPIDsB[NARROWPHASE_CONTACTPOLYGONS_MAX_VERTICES];
    Vector3 vSeparatingAxis;

    _GenerateContactPoints_MostParallelFacet( arrPolyA, arrCPIDsA, &iPolyASize, arrPolyB, arrCPIDsB, &iPolyBSize, &vSeparatingAxis,
                                              pClosestFeatureA, pClosestFeatureB );

    // Get plane for facet on B
    Vector3 vEdge0 = ( arrPolyB[1] - arrPolyB[0] );
    Vector3 vEdge1 = ( arrPolyB[iPolyBSize >> 1] - arrPolyB[0] );
    Vector3 vFacetNormalB = ( vEdge0 ^ vEdge1 );
    vFacetNormalB.Normalize();

    Plane vFacetPlaneB( vFacetNormalB, arrPolyB[0] );

    // Clip polygons against each other
    UInt iClippedPolygonSize = 0;
    Vertex3 arrClippedPoly[NARROWPHASE_CONTACTPOLYGONS_MAX_VERTICES << 1];
    ContactPointID arrClippedCPIDs[NARROWPHASE_CONTACTPOLYGONS_MAX_VERTICES << 1];
    _GenerateContactPoints_ClipPolygons( arrClippedPoly, arrClippedCPIDs, &iClippedPolygonSize, vSeparatingAxis, arrPolyA, arrCPIDsA, iPolyASize,
                                                                                                                 arrPolyB, arrCPIDsB, iPolyBSize );

    // Generate contact points
    m_pCollisionManifold->Clear();

    for( UInt i = 0; i < iClippedPolygonSize; ++i ) {
        // Get points on B, intersection of lines // to sep axis going
        // through A's points and B's facet's plane.
        Line3 vLine( arrClippedPoly[i], -vSeparatingAxis );
        
        IntersectPlaneLine3 hIntersection( vFacetPlaneB, vLine, ZeroTolerance );
        Bool bIntersection = hIntersection.Compute();
        Assert( bIntersection );

        // Add contact point
        Scalar fDistance = hIntersection.GetLineParameter();
        if ( fDistance < fMinDistance )
            fDistance = fMinDistance;
        if ( fDistance > fMaxDistance )
            continue;

        Vertex3 vA = arrClippedPoly[i];
        Vertex3 vB = vLine.GetPoint( fDistance );

        m_pCollisionManifold->AddPoint( arrClippedCPIDs[i], vA, vB, vSeparatingAxis, fDistance );
    }
}

/////////////////////////////////////////////////////////////////////////////////
// NarrowPhaseConcave implementation
NarrowPhaseConcave::NarrowPhaseConcave( TransformedShape * pShapeA, TransformedShape * pShapeB ):
    NarrowPhase()
{
    m_pShapeA = pShapeA;
    m_pShapeB = pShapeB;

    PhysicsFn->SelectMemory( TEXT("Narrowphase") );
    m_pCollisionManifold = New MultiPlaneManifold();
    PhysicsFn->UnSelectMemory();
}
NarrowPhaseConcave::~NarrowPhaseConcave()
{
    PhysicsFn->SelectMemory( TEXT("Narrowphase") );
    Delete( m_pCollisionManifold );
    PhysicsFn->UnSelectMemory();
}

