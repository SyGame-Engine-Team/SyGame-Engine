/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/CollisionDetection/NarrowPhase/GJK/GJK.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Narrow-Phase : Convex, Gilbert-Johnson-Keerti algorithm coupled
//               with Expanding-Polytope-Algorithm or Minkowski-Portal-Refinement
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
#include "GJK.h"

/////////////////////////////////////////////////////////////////////////////////
// GJK implementation
GJK::GJK( TransformedShapeConvex * pConvexA, TransformedShapeConvex * pConvexB ):
    NarrowPhaseConvex( pConvexA, pConvexB ),
    m_hSimplex(), m_hGJKSolver(), m_hEPASolver(), m_hMPRSolver()
{
    // nothing to do
}
GJK::~GJK()
{
    // nothing to do
}

Void GJK::Initialize()
{
    // Guess for first frame
    m_vGuess = ( m_pShapeA->GetTransform().GetTranslate() - m_pShapeB->GetTransform().GetTranslate() );
    m_vGuess.Normalize();
}
Bool GJK::DetectCollision( Bool bGenerateContactPoints )
{
    // GJK-EPA algorithm
    Vector3 vA, vB;
    Vector3 vNormal;
    Scalar fDistance;
    Bool bCollision;

    _GJK_Initialize();
    GJKState iGJKState = _GJK_Evaluate( m_vGuess );
    if ( iGJKState == GJKSTATE_VALID ) {
        Vector3 v0 = Vector3::Null;
        Vector3 v1 = Vector3::Null;
        for( UInt i = 0; i < m_hSimplex.iRank; ++i ) {
            _GetSupportA( &vA, m_hSimplex.arrVertices[i]->vDir );
            _GetSupportB( &vB, -(m_hSimplex.arrVertices[i]->vDir) );
            v0 += vA * m_hSimplex.arrWeights[i];
            v1 += vB * m_hSimplex.arrWeights[i];
        }
        bCollision = false;
        vA = v0;
        vB = v1;
        vNormal = ( vA - vB );
        fDistance = vNormal.Norm();
        if ( fDistance > GJK_MIN_DISTANCE ) 
            vNormal *= MathFn->Invert( fDistance );
    } else if ( iGJKState == GJKSTATE_INSIDE ) {
        _GJK_Initialize();
        iGJKState = _GJK_Evaluate( -m_vGuess );
        if ( iGJKState == GJKSTATE_VALID ) {
            Vector3 v0 = Vector3::Null;
            Vector3 v1 = Vector3::Null;
            for( UInt i = 0; i < m_hSimplex.iRank; ++i ) {
                _GetSupportA( &vA, (m_hSimplex.arrVertices[i]->vDir) );
                _GetSupportB( &vB, -(m_hSimplex.arrVertices[i]->vDir) );
                v0 += vA * m_hSimplex.arrWeights[i];
                v1 += vB * m_hSimplex.arrWeights[i];
            }
            bCollision = false;
            vA = v0;
            vB = v1;
            vNormal = ( vA - vB );
            fDistance = vNormal.Norm();
            if ( fDistance > GJK_MIN_DISTANCE ) 
                vNormal *= MathFn->Invert( fDistance );
        } else if ( iGJKState == GJKSTATE_INSIDE ) {
            _EPA_Initialize();
            EPAState iEPAState = _EPA_Evaluate( -m_vGuess );
            if ( iEPAState != EPASTATE_FAILED ) {
                Vector3 v0 = Vector3::Null;
                for( UInt i = 0; i < m_hSimplex.iRank; ++i ) {
                    _GetSupportA( &vA, m_hSimplex.arrVertices[i]->vDir );
                    v0 += vA * m_hSimplex.arrWeights[i];
                }
                bCollision = true;
                vA = v0;
                vB = (v0 - m_hEPASolver.vNormal * m_hEPASolver.fDepth);
                vNormal = -(m_hEPASolver.vNormal);
                vNormal.Normalize();
                fDistance = -(m_hEPASolver.fDepth);
            } else {
                Assert( false );
            }
        } else {
            Assert( false );
        }
    } else {
        Assert( false );
    }
    m_vGuess = vNormal;

    // MPR algorithm
    //Vector3 vPosition;
    //Vector3 vNormal;
    //Scalar fDistance;
    //_MPR_Initialize();
    //Bool bCollision = _MPR_Evaluate( &vPosition, &vNormal, &fDistance );
    //fDistance = -fDistance;

    // Generate contact points
    if ( bGenerateContactPoints )
        _GenerateContactPoints( vNormal );

    // Done
    return bCollision;
}

/////////////////////////////////////////////////////////////////////////////////

Scalar GJK::_GJK_ProjectOrigin( const Vector3 & vA, const Vector3 & vB, Scalar * arrWeights, UInt * pMask )
{
    Vector3 vDir = ( vB - vA );

    Scalar fSqrLen = vDir.NormSqr();
    if ( fSqrLen <= GJK_EPSILON_S2 )
        return -1.0f;

    Scalar fT = ( fSqrLen > 0.0f ) ? -(vA*vDir)/fSqrLen : 0.0f;

    if ( fT >= 1.0f ) {
        arrWeights[0] = 0.0f;
        arrWeights[1] = 1.0f;
        *pMask = 2;
        return vB.NormSqr();
    }

    if ( fT <= 0.0f ) {
        arrWeights[0] = 1.0f;
        arrWeights[1] = 0.0f;
        *pMask = 1;
        return vA.NormSqr();
    }

    arrWeights[0] = ( 1.0f - fT );
    arrWeights[1] = fT;
    *pMask = 3;
    return (vA + vDir * fT).NormSqr();
}
Scalar GJK::_GJK_ProjectOrigin( const Vector3 & vA, const Vector3 & vB, const Vector3 & vC, Scalar * arrWeights, UInt * pMask )
{
    static const UInt s_arrNext[3] = { 1, 2, 0 };

    const Vector3 * vTri[3] = { &vA, &vB, &vC };
    Vector3 vDelta[3] = { (vA - vB), (vB - vC), (vC - vA) };

    Vector3 vNormal = ( vDelta[0] ^ vDelta[1] );
    Scalar fSqrLen = vNormal.NormSqr();
    if ( fSqrLen <= GJK_EPSILON_S3 )
        return -1.0f;

    Scalar fMinDist = -1.0f;
    Scalar arrSubWeights[2] = { 0.0f, 0.0f };
    UInt iSubMask = 0;
    for( UInt i = 0; i < 3; ++i ) {
        if ( ( *(vTri[i]) * (vDelta[i] ^ vNormal) ) > 0.0f ) {
            UInt iNext = s_arrNext[i];
            Scalar fSubDist = _GJK_ProjectOrigin( *(vTri[i]), *(vTri[iNext]), arrSubWeights, &iSubMask );
            if ( fMinDist < 0.0f || fSubDist < fMinDist  ) {
                fMinDist = fSubDist;
                arrWeights[i] = arrSubWeights[0];
                arrWeights[iNext] = arrSubWeights[1];
                arrWeights[s_arrNext[iNext]] = 0.0f;
                *pMask = ( ((iSubMask&1) ? (1<<i) : 0) + ((iSubMask&2) ? (1<<iNext) : 0) );
            }
        }
    }

    if ( fMinDist < 0.0f ) {
        Scalar fDist = ( vA * vNormal );
        Scalar fLen = MathFn->Sqrt( fSqrLen );
        Vector3 vProj = vNormal * ( fDist / fLen );
        fMinDist = vProj.NormSqr();
        arrWeights[0] = ( vDelta[1] ^ (vB - vProj) ).Norm() / fLen;
        arrWeights[1] = ( vDelta[2] ^ (vC - vProj) ).Norm() / fLen;
        arrWeights[2] = 1.0f - ( arrWeights[0] + arrWeights[1] );
        *pMask = 7;
    }

    return fMinDist;
}
Scalar GJK::_GJK_ProjectOrigin( const Vector3 & vA, const Vector3 & vB, const Vector3 & vC, const Vector3 & vD, Scalar * arrWeights, UInt * pMask )
{
    static const UInt s_arrNext[3] = { 1, 2, 0 };

    const Vector3 * vTetra[4] = { &vA, &vB, &vC, &vD };
    Vector3 vDelta[3] = { (vA - vD), (vB - vD), (vC - vD) };

    Scalar fDet = _GJK_Det( vDelta[0], vDelta[1], vDelta[2] );
    Scalar fInvDet = MathFn->Invert( fDet );
    Bool bNonSingular = ( ( fDet * (vA * ((vB-vC) ^ (vA-vB))) ) <= 0 );
    if ( !bNonSingular || MathFn->Abs(fDet) <= GJK_EPSILON_S4 )
        return -1.0f;

    Scalar fMinDist = -1.0f;
    Scalar arrSubWeights[3] = { 0.0f, 0.0f };
    UInt iSubMask = 0;
    for( UInt i = 0; i < 3; ++i ) {
        UInt iNext = s_arrNext[i];
        if ( ( fDet * (vD * (vDelta[i] ^ vDelta[iNext])) ) > 0.0f ) {
            Scalar fSubDist = _GJK_ProjectOrigin( *(vTetra[i]), *(vTetra[iNext]), vD, arrSubWeights, &iSubMask );
            if ( fMinDist < 0.0f || fSubDist < fMinDist ) {
                fMinDist = fSubDist;
                arrWeights[i] = arrSubWeights[0];
                arrWeights[iNext] = arrSubWeights[1];
                arrWeights[s_arrNext[iNext]] = 0.0f;
                arrWeights[3] = arrSubWeights[2];
                *pMask = ( ((iSubMask&1) ? (1<<i) : 0) + ((iSubMask&2) ? (1<<iNext) : 0) + ((iSubMask&4) ? 8 : 0) );
            }
        }
    }

    if ( fMinDist < 0.0f ) {
        fMinDist = 0.0f;
        arrWeights[0] = _GJK_Det(vC,vB,vD) * fInvDet;
        arrWeights[1] = _GJK_Det(vA,vC,vD) * fInvDet;
        arrWeights[2] = _GJK_Det(vB,vA,vD) * fInvDet;
        arrWeights[3] = 1.0f - ( arrWeights[0] + arrWeights[1] + arrWeights[2] );
        *pMask = 15;
    }

    return fMinDist;
}

Void GJK::_GJK_Initialize()
{
    m_hGJKSolver.iState = GJKSTATE_FAILED;

    m_hGJKSolver.vDirection = Vector3::Null;
    m_hGJKSolver.fDistance = 0.0f;

    m_hGJKSolver.iCurrentSimplex = 0;
    m_hGJKSolver.arrSimplices[0]._Initialize();
    m_hGJKSolver.arrSimplices[1]._Initialize();

    m_hGJKSolver.hSVAllocator._Reset();
}
GJKState GJK::_GJK_Evaluate( const Vector3 & vGuess )
{
    Scalar fSqrGuessLen = vGuess.NormSqr();

    // Initialize
    m_hGJKSolver.iState = GJKSTATE_VALID;

    _PushVertex( m_hGJKSolver.arrSimplices + 0, (fSqrGuessLen > 0.0f) ? -vGuess : Vector3::eI );
    m_hGJKSolver.arrSimplices[0].arrWeights[0] = 1.0f;

    m_hGJKSolver.vDirection = m_hGJKSolver.arrSimplices[0].arrVertices[0]->vSupport;
    Scalar fSqrDistance = fSqrGuessLen;

    Vector3 vLastSupport[4] = { m_hGJKSolver.vDirection, m_hGJKSolver.vDirection, m_hGJKSolver.vDirection, m_hGJKSolver.vDirection };
    UInt iLastSupport = 0;

    // Main loop
    UInt iIterations = 0;
    Scalar fAlpha = 0.0f;

    while( m_hGJKSolver.iState == GJKSTATE_VALID ) {
        UInt iNextSimplex = ( 1 - m_hGJKSolver.iCurrentSimplex );
        _Simplex * pCurSimplex = ( m_hGJKSolver.arrSimplices + m_hGJKSolver.iCurrentSimplex );
        _Simplex * pNextSimplex = ( m_hGJKSolver.arrSimplices + iNextSimplex );

        // Check for null ray
        Scalar fRayLen = m_hGJKSolver.vDirection.Norm();
        if ( fRayLen < GJK_MIN_DISTANCE ) {
            m_hGJKSolver.iState = GJKSTATE_INSIDE;
            break; // Inside / Touching
        }

        // New vertex in (-v) direction
        _PushVertex( pCurSimplex, -(m_hGJKSolver.vDirection) );
        const Vector3 & vSupport = pCurSimplex->arrVertices[pCurSimplex->iRank - 1]->vSupport;

        Bool bFound = false;
        for( UInt i = 0; i < 4; ++i ) {
            if ( (vSupport - vLastSupport[i]).NormSqr() < GJK_EPSILON_DUP ) {
                bFound = true;
                break;
            }
        }
        if ( bFound ) {
            _PopVertex( pCurSimplex );
            break;
        } else {
            iLastSupport = (iLastSupport + 1) % 3;
            vLastSupport[iLastSupport] = vSupport;
        }

        // Check if we're done
        Scalar fOmega = ( m_hGJKSolver.vDirection * vSupport ) / fRayLen;
        fAlpha = Max<Scalar>( fAlpha, fOmega );
        if ( ((fRayLen-fAlpha) - (GJK_ACCURACY*fRayLen)) <= 0.0f ) {
            _PopVertex( pCurSimplex );
            break;
        }

        // Simplex reduction
        Scalar arrWeights[4];
        UInt iMask = 0;
        switch( pCurSimplex->iRank ) {
            case 2: fSqrDistance = _GJK_ProjectOrigin( pCurSimplex->arrVertices[0]->vSupport, pCurSimplex->arrVertices[1]->vSupport, arrWeights, &iMask ); break;
            case 3: fSqrDistance = _GJK_ProjectOrigin( pCurSimplex->arrVertices[0]->vSupport,
                                                       pCurSimplex->arrVertices[1]->vSupport, pCurSimplex->arrVertices[2]->vSupport, arrWeights, &iMask ); break;
            case 4: fSqrDistance = _GJK_ProjectOrigin( pCurSimplex->arrVertices[0]->vSupport, pCurSimplex->arrVertices[1]->vSupport,
                                                       pCurSimplex->arrVertices[2]->vSupport, pCurSimplex->arrVertices[3]->vSupport, arrWeights, &iMask ); break;
        }
        if ( fSqrDistance >= 0.0f ) {
            m_hGJKSolver.iCurrentSimplex = iNextSimplex;
            pNextSimplex->iRank = 0;

            m_hGJKSolver.vDirection = Vector3::Null;
            for( UInt i = 0; i < pCurSimplex->iRank; ++i ) {
                if ( iMask & (1<<i) ) {
                    m_hGJKSolver.vDirection += pCurSimplex->arrVertices[i]->vSupport * arrWeights[i];
                    pNextSimplex->arrVertices[pNextSimplex->iRank] = pCurSimplex->arrVertices[i];
                    pNextSimplex->arrWeights[pNextSimplex->iRank] = arrWeights[i];
                    ++(pNextSimplex->iRank);
                } else
                    m_hGJKSolver.hSVAllocator._Free( pCurSimplex->arrVertices[i] );
            }

            if ( iMask == 15 )
                m_hGJKSolver.iState = GJKSTATE_INSIDE;
        } else {
            _PopVertex( pCurSimplex );
            break;
        }

        // Update state
        ++iIterations;
        if ( iIterations >= GJK_MAX_ITERATIONS )
            m_hGJKSolver.iState = GJKSTATE_FAILED;
    }

    // Done
    m_hSimplex.iRank = m_hGJKSolver.arrSimplices[m_hGJKSolver.iCurrentSimplex].iRank;
    for( UInt i = 0; i < m_hSimplex.iRank; ++i ) {
        m_hSimplex.arrVertices[i] = m_hGJKSolver.arrSimplices[m_hGJKSolver.iCurrentSimplex].arrVertices[i];
        m_hSimplex.arrWeights[i] = m_hGJKSolver.arrSimplices[m_hGJKSolver.iCurrentSimplex].arrWeights[i];
    }

    switch( m_hGJKSolver.iState ) {
        case GJKSTATE_VALID:  m_hGJKSolver.fDistance = m_hGJKSolver.vDirection.Normalize(); break;
        case GJKSTATE_INSIDE: m_hGJKSolver.fDistance = 0.0f; break;
        default: break;
    }

    return m_hGJKSolver.iState;
}
Bool GJK::_GJK_ContainsOrigin()
{
    switch( m_hSimplex.iRank ) {
        case 1: {
                for( UInt i = 0; i < 3; ++i ) {
                    Vector3 vAxis = Vector3::Null;
                    vAxis[i] = 1.0f;

                    _PushVertex( &m_hSimplex, vAxis );
                    if ( _GJK_ContainsOrigin() )
                        return true;
                    _PopVertex( &m_hSimplex );

                    _PushVertex( &m_hSimplex, -vAxis );
                    if ( _GJK_ContainsOrigin() )
                        return true;
                    _PopVertex( &m_hSimplex );
                }
            } break;
        case 2: {
                Vector3 vDir = ( m_hSimplex.arrVertices[1]->vSupport - m_hSimplex.arrVertices[0]->vSupport );
                for( UInt i = 0; i < 3; ++i ) {
                    Vector3 vAxis = Vector3::Null;
                    vAxis[i] = 1.0f;

                    vAxis = ( vDir ^ vAxis );
                    if ( vAxis.NormSqr() > 0.0f ) {
                        _PushVertex( &m_hSimplex, vAxis );
                        if ( _GJK_ContainsOrigin() )
                            return true;
                        _PopVertex( &m_hSimplex );

                        _PushVertex( &m_hSimplex, -vAxis );
                        if ( _GJK_ContainsOrigin() )
                            return true;
                        _PopVertex( &m_hSimplex );
                    }
                }
            } break;
        case 3: {
                Vector3 vAxis = ( (m_hSimplex.arrVertices[1]->vSupport - m_hSimplex.arrVertices[0]->vSupport)
                                ^ (m_hSimplex.arrVertices[2]->vSupport - m_hSimplex.arrVertices[0]->vSupport) );
                if ( vAxis.NormSqr() > 0.0f ) {
                    _PushVertex( &m_hSimplex, vAxis );
                    if ( _GJK_ContainsOrigin() )
                        return true;
                    _PopVertex( &m_hSimplex );

                    _PushVertex( &m_hSimplex, -vAxis );
                    if ( _GJK_ContainsOrigin() )
                        return true;
                    _PopVertex( &m_hSimplex );
                }
            } break;
        case 4: {
                if ( MathFn->Abs( _GJK_Det( m_hSimplex.arrVertices[0]->vSupport - m_hSimplex.arrVertices[3]->vSupport,
                                            m_hSimplex.arrVertices[1]->vSupport - m_hSimplex.arrVertices[3]->vSupport,
                                            m_hSimplex.arrVertices[2]->vSupport - m_hSimplex.arrVertices[3]->vSupport ) ) > 0.0f )
                    return true;
            } break;
        default: Assert(false); break;
    }

    return false;
}

/////////////////////////////////////////////////////////////////////////////////

Bool GJK::_EPA_GetEdgeDistance( Scalar * outDistance, const _SimplexFacet * pFacet, const _SupportVertex * pA, const _SupportVertex * pB )
{
    Vector3 vAB = ( pB->vSupport - pA->vSupport );

    // Check if we're within the edge
    Scalar fDot = ( pA->vSupport * (vAB ^ pFacet->vNormal) );
    if ( fDot >= 0.0f )
        return false;

    // Outside of edge AB
    Scalar fDotA = ( pA->vSupport * vAB );
    Scalar fDotB = ( pB->vSupport * vAB );

    if ( fDotA > 0.0f )
        *outDistance = pA->vSupport.Norm();
    else if ( fDotB < 0.0f )
        *outDistance = pB->vSupport.Norm();
    else {
        Scalar fSqrLength = vAB.NormSqr();
        fDot = ( pA->vSupport * pB->vSupport );
        *outDistance = MathFn->Sqrt( Max<Scalar>( (pA->vSupport.NormSqr() * pB->vSupport.NormSqr() - fDot * fDot) / fSqrLength, 0.0f ) );
    }

    return true;
}
GJK::_SimplexFacet * GJK::_EPA_CreateFacet( _SupportVertex * pA, _SupportVertex * pB, _SupportVertex * pC, Bool bCheckConvexity )
{
    // Check for degenerated facet
    Vector3 vNormal = ( (pB->vSupport - pA->vSupport) ^ (pC->vSupport - pA->vSupport) );
    Scalar fLength = vNormal.Norm();
    if ( fLength <= EPA_ACCURACY ) {
        m_hEPASolver.iState = EPASTATE_DEGENERATED;
        return NULL;
    }

    // Allocate new facet
    _SimplexFacet * pFacet = m_hEPASolver.hSFAllocator._Allocate();
    pFacet->_Initialize();

    pFacet->iPass = 0;
    pFacet->vNormal = vNormal;
    
    pFacet->arrVertices[0] = pA;
    pFacet->arrVertices[1] = pB;
    pFacet->arrVertices[2] = pC;

    // Compute facet distance
    Bool bExterior = _EPA_GetEdgeDistance( &(pFacet->fDistance), pFacet, pA, pB );
    bExterior = ( bExterior || _EPA_GetEdgeDistance( &(pFacet->fDistance), pFacet, pB, pC ) );
    bExterior = ( bExterior || _EPA_GetEdgeDistance( &(pFacet->fDistance), pFacet, pC, pA ) );

    Scalar fInvLength = MathFn->Invert( fLength );
    if ( !bExterior )
        pFacet->fDistance = ( pA->vSupport * pFacet->vNormal ) * fInvLength;
    vNormal *= fInvLength;

    // Check convexity
    if ( bCheckConvexity && pFacet->fDistance < -EPA_EPSILON ) {
        m_hEPASolver.iState = EPASTATE_NON_CONVEX;
        m_hEPASolver.hSFAllocator._Free( pFacet );
        return NULL;
    }

    // Done
    m_hEPASolver.hHull._Append( pFacet );
    return pFacet;
}

GJK::_SimplexFacet * GJK::_EPA_FindBestFacet() const
{
    _SimplexFacet * pMinFacet = m_hEPASolver.hHull.pHead;
    Scalar fMinDistSqr = ( pMinFacet->fDistance * pMinFacet->fDistance );

    _SimplexFacet * pFacet = pMinFacet->arrListLinks[1];
    while( pFacet != NULL ) {
        Scalar fDistSqr = ( pFacet->fDistance * pFacet->fDistance );
        if ( fDistSqr < fMinDistSqr ) {
            pMinFacet = pFacet;
            fDistSqr = fMinDistSqr;
        }
        pFacet = pFacet->arrListLinks[1];
    }

    return pMinFacet;
}
Bool GJK::_EPA_Expand( _SimplexHorizon * outHorizon, Byte iPass, _SupportVertex * pSV, _SimplexFacet * pFacet, Byte iEdge )
{
    static const Byte s_arrNext[3] = { 1, 2, 0 };

    // Skip facets within same pass
    if ( pFacet->iPass == iPass )
        return false;

    // Expand horizon
    Byte iEdge1 = s_arrNext[iEdge];
    if ( ((pFacet->vNormal * pSV->vSupport) - pFacet->fDistance) < -EPA_EPSILON ) {
        // Add new facet to horizon
        _SimplexFacet * pNewFacet = _EPA_CreateFacet( pFacet->arrVertices[iEdge1], pFacet->arrVertices[iEdge], pSV, true );
        if ( pNewFacet == NULL )
            return false;

        pNewFacet->_Link( 0, pFacet, iEdge );
        if ( outHorizon->pFacetC != NULL )
            outHorizon->pFacetC->_Link( 1, pNewFacet, 2 );
        else
            outHorizon->pFacetF = pNewFacet;
        outHorizon->pFacetC = pNewFacet;
        ++(outHorizon->iCount);
    } else {
        // Recurse on both edges
        Byte iEdge2 = s_arrNext[iEdge1];
        pFacet->iPass = iPass;

        Bool bExpanded = ( _EPA_Expand( outHorizon, iPass, pSV, pFacet->arrFacetLinks[iEdge1], pFacet->arrEdgeLinks[iEdge1] ) &&
                           _EPA_Expand( outHorizon, iPass, pSV, pFacet->arrFacetLinks[iEdge2], pFacet->arrEdgeLinks[iEdge2] ) );
        if ( !bExpanded )
            return false;

        // Destroy parent facet
        m_hEPASolver.hHull._Remove( pFacet );
        m_hEPASolver.hSFAllocator._Free( pFacet );
    }

    return true;
}

Void GJK::_EPA_Initialize()
{
    m_hEPASolver.iState = EPASTATE_FAILED;

    m_hEPASolver.vNormal = Vector3::Null;
    m_hEPASolver.fDepth = 0.0f;

    m_hEPASolver.hHull._Initialize();

    m_hEPASolver.hSVAllocator._Reset();
    m_hEPASolver.hSFAllocator._Reset();
}
EPAState GJK::_EPA_Evaluate( const Vector3 & vGuess )
{
    m_hEPASolver.iState = EPASTATE_VALID;

    // Check for degenerate GJK output
    if ( m_hSimplex.iRank <= 1 || !(_GJK_ContainsOrigin()) ) {
        m_hEPASolver.iState = EPASTATE_FALLBACK;

        m_hEPASolver.vNormal = -vGuess;
        Scalar fLength = m_hEPASolver.vNormal.Norm();
        if ( fLength > 0.0f )
            m_hEPASolver.vNormal *= MathFn->Invert( fLength );
        else
            m_hEPASolver.vNormal = Vector3::eI;
        m_hEPASolver.fDepth = 0.0f;

        m_hSimplex.iRank = 1;
        //m_hSimplex.arrVertices[0] = m_hSimplex.arrVertices[0];
        m_hSimplex.arrWeights[0] = 1.0f;

        return m_hEPASolver.iState;
    }

    // Simplex orientation
    if ( _GJK_Det( m_hSimplex.arrVertices[0]->vSupport - m_hSimplex.arrVertices[3]->vSupport,
                   m_hSimplex.arrVertices[1]->vSupport - m_hSimplex.arrVertices[3]->vSupport,
                   m_hSimplex.arrVertices[2]->vSupport - m_hSimplex.arrVertices[3]->vSupport ) < 0.0f ) {
        Swap<_SupportVertex>( m_hSimplex.arrVertices[0], m_hSimplex.arrVertices[1] );
        Swap<Scalar>( m_hSimplex.arrWeights + 0, m_hSimplex.arrWeights + 1 );
    }

    // Initial tetrahedron hull
    _SimplexFacet * arrTetrahedron[4] = {
        _EPA_CreateFacet( m_hSimplex.arrVertices[0], m_hSimplex.arrVertices[1], m_hSimplex.arrVertices[2], false ),
        _EPA_CreateFacet( m_hSimplex.arrVertices[1], m_hSimplex.arrVertices[0], m_hSimplex.arrVertices[3], false ),
        _EPA_CreateFacet( m_hSimplex.arrVertices[2], m_hSimplex.arrVertices[1], m_hSimplex.arrVertices[3], false ),
        _EPA_CreateFacet( m_hSimplex.arrVertices[0], m_hSimplex.arrVertices[2], m_hSimplex.arrVertices[3], false )
    };
    Assert( m_hEPASolver.hHull.iCount == 4 );

    arrTetrahedron[0]->_Link( 0, arrTetrahedron[1], 0 );
    arrTetrahedron[0]->_Link( 1, arrTetrahedron[2], 0 );
    arrTetrahedron[0]->_Link( 2, arrTetrahedron[3], 0 );
    arrTetrahedron[1]->_Link( 1, arrTetrahedron[3], 2 );
    arrTetrahedron[1]->_Link( 2, arrTetrahedron[2], 1 );
    arrTetrahedron[2]->_Link( 2, arrTetrahedron[3], 1 );

    // Main loop
    _SimplexFacet * pBestFacet = _EPA_FindBestFacet();
    _SimplexFacet hOuterFacet = *pBestFacet;

    Byte iPass = 0;
    UInt iIterations = 0;

    while( iIterations < EPA_MAX_ITERATIONS ) {
        // Initialize horizon
        _SimplexHorizon hHorizon;
        hHorizon._Initialize();
        _SupportVertex * pSV = m_hEPASolver.hSVAllocator._Allocate();

        Bool bValid = true;
        pBestFacet->iPass = ++iPass;
        _GetSupportVertex( pSV, pBestFacet->vNormal );

        // Check for accuracy limit
        Scalar fDistSV = ( (pBestFacet->vNormal * pSV->vSupport) - pBestFacet->fDistance );
        if ( fDistSV <= EPA_ACCURACY ) {
            m_hEPASolver.iState = EPASTATE_ACCURACY_REACHED;
            break;
        }

        // Expand horizon
        for( UInt i = 0; i < 3 && bValid; ++i )
            bValid = ( bValid && _EPA_Expand( &hHorizon, iPass, pSV, pBestFacet->arrFacetLinks[i], pBestFacet->arrEdgeLinks[i] ) );

        // Check for invalid hulls
        if ( !bValid || hHorizon.iCount < 3 ) {
            m_hEPASolver.iState = EPASTATE_INVALID_HULL;
            break;
        }

        // Close horizon linkage
        hHorizon.pFacetC->_Link( 1, hHorizon.pFacetF, 2 );

        // Destroy best facet
        m_hEPASolver.hHull._Remove( pBestFacet );
        m_hEPASolver.hSFAllocator._Free( pBestFacet );

        // Pick next best facet
        pBestFacet = _EPA_FindBestFacet();
        hOuterFacet = *pBestFacet;

        // Next iteration
        ++iIterations;
    }

    // Output outer facet as new simplex
    Vector3 vProj = ( hOuterFacet.vNormal * hOuterFacet.fDistance );

    m_hEPASolver.vNormal = hOuterFacet.vNormal;
    m_hEPASolver.fDepth = hOuterFacet.fDistance;

    m_hSimplex.iRank = 3;
    m_hSimplex.arrVertices[0] = hOuterFacet.arrVertices[0];
    m_hSimplex.arrVertices[1] = hOuterFacet.arrVertices[1];
    m_hSimplex.arrVertices[2] = hOuterFacet.arrVertices[2];
    m_hSimplex.arrWeights[0] = ( (hOuterFacet.arrVertices[1]->vSupport - vProj) ^ (hOuterFacet.arrVertices[2]->vSupport - vProj) ).Norm();
    m_hSimplex.arrWeights[1] = ( (hOuterFacet.arrVertices[2]->vSupport - vProj) ^ (hOuterFacet.arrVertices[0]->vSupport - vProj) ).Norm();
    m_hSimplex.arrWeights[2] = ( (hOuterFacet.arrVertices[0]->vSupport - vProj) ^ (hOuterFacet.arrVertices[1]->vSupport - vProj) ).Norm();

    Scalar fInvSum = MathFn->Invert( m_hSimplex.arrWeights[0] + m_hSimplex.arrWeights[1] + m_hSimplex.arrWeights[2] );
    m_hSimplex.arrWeights[0] *= fInvSum;
    m_hSimplex.arrWeights[1] *= fInvSum;
    m_hSimplex.arrWeights[2] *= fInvSum;

    return m_hEPASolver.iState;   
}

/////////////////////////////////////////////////////////////////////////////////

Scalar GJK::_MPR_SegmentDistanceSqr( Vector3 * outNormal, const Vector3 & vA, const Vector3 & vB )
{
    // Let t in [0;1]
    // Segment(t) = vA + (t * (B-A))
    // DistSqr(t) = |Segment(t) - Origin|^2 where Origin is null vector, minimize this
    Vector3 vAB = ( vB - vA );
    Scalar fT = -( vA * vAB ) * vAB.InvNormSqr();
    if ( fT < SCALAR_EPSILON )
        *outNormal = vA;
    else if ( fT > 1.0f - SCALAR_EPSILON )
        *outNormal = vB;
    else
        *outNormal = ( vA + (vAB * fT) );
    return outNormal->NormSqr();
}
Scalar GJK::_MPR_TriangleDistanceSqr( Vector3 * outNormal, const Vector3 & vA, const Vector3 & vB, const Vector3 & vC )
{
    // Let (s,t) in [0;1]^2 and (s+t) < 1
    // Triangle(s,t) = vA + (s * (B-A)) + (t * (C-A))
    // DistSqr(s,t) = |Triangle(s,t) - Origin|^2 where Origin is null vector, minimize this
    Vector3 vAB = ( vB - vA );
    Vector3 vAC = ( vC - vA );
    Scalar fU = ( vAB * vAB );
    Scalar fV = ( vAC * vAC );
    Scalar fP = ( vA * vAB );
    Scalar fQ = ( vA * vAC );
    Scalar fR = ( vAB * vAC );
    Scalar fS = ( fQ * fR - fP * fV ) / (fU * fV - fR * fR);
    Scalar fT = ( -fS * fR - fQ ) / fV;

    if ( ( fS > -SCALAR_EPSILON ) && ( fS < 1.0f + SCALAR_EPSILON ) &&
         ( fT > -SCALAR_EPSILON ) && ( fT < 1.0f + SCALAR_EPSILON ) &&
         ( (fS+fT) < 1.0f + SCALAR_EPSILON ) ) {
        *outNormal = ( vA + (vAB * fS) + (vAC * fT) );
        return outNormal->NormSqr();
    }

    // Boundary case
    Vector3 vTmpNormal;
    Scalar fDistance = _MPR_SegmentDistanceSqr( outNormal, vA, vB );
    Scalar fTmpDistance = _MPR_SegmentDistanceSqr( &vTmpNormal, vA, vC );
    if ( fTmpDistance < fDistance ) {
        fDistance = fTmpDistance;
        *outNormal = vTmpNormal;
    }
    fTmpDistance = _MPR_SegmentDistanceSqr( &vTmpNormal, vB, vC );
    if ( fTmpDistance < fDistance ) {
        fDistance = fTmpDistance;
        *outNormal = vTmpNormal;
    }
    return fDistance;
}

Int GJK::_MPR_DiscoverPortal()
{
    // Portal center
    m_hMPRSolver.hPortal.arrVertices[0] = m_hMPRSolver.hSVAllocator._Allocate();
    _MPR_GetOrigin( m_hMPRSolver.hPortal.arrVertices[0] );
    m_hMPRSolver.hPortal.iRank = 1;

    // Intersection case, still need penetration data => cheat by moving center a bit
    if ( _MPR_TestEquality(m_hMPRSolver.hPortal.arrVertices[0]->vPointM, Vector3::Null) )
        m_hMPRSolver.hPortal.arrVertices[0]->vPointM += ( Vector3::eI * (SCALAR_EPSILON * 10.0f) );

    // Grow to line
    Vector3 vPortalDir = -( m_hMPRSolver.hPortal.arrVertices[0]->vPointM );
    vPortalDir.Normalize();

    m_hMPRSolver.hPortal.arrVertices[1] = m_hMPRSolver.hSVAllocator._Allocate();
    _MPR_GetSupport( m_hMPRSolver.hPortal.arrVertices[1], vPortalDir );

    Scalar fDot = ( m_hMPRSolver.hPortal.arrVertices[1]->vPointM * vPortalDir );
    if ( fDot < SCALAR_EPSILON ) {
        m_hMPRSolver.hSVAllocator._Free( m_hMPRSolver.hPortal.arrVertices[1] );
        m_hMPRSolver.hPortal.arrVertices[1] = NULL;
        return -1;
    }

    ++(m_hMPRSolver.hPortal.iRank);

    // Grow to triangle
    vPortalDir = ( m_hMPRSolver.hPortal.arrVertices[0]->vPointM ^ m_hMPRSolver.hPortal.arrVertices[1]->vPointM );
    if ( vPortalDir.NormSqr() < SCALAR_EPSILON ) {
        if ( _MPR_TestEquality(m_hMPRSolver.hPortal.arrVertices[1]->vPointM, Vector3::Null) )
            return 1; // Origin on v1
        else
            return 2; // Origin on v0-v1 segment
    }
    vPortalDir.Normalize();

    m_hMPRSolver.hPortal.arrVertices[2] = m_hMPRSolver.hSVAllocator._Allocate();
    _MPR_GetSupport( m_hMPRSolver.hPortal.arrVertices[2], vPortalDir );

    fDot = ( m_hMPRSolver.hPortal.arrVertices[2]->vPointM * vPortalDir );
    if ( fDot < SCALAR_EPSILON ) {
        m_hMPRSolver.hSVAllocator._Free( m_hMPRSolver.hPortal.arrVertices[2] );
        m_hMPRSolver.hPortal.arrVertices[2] = NULL;
        return -1;
    }

    ++(m_hMPRSolver.hPortal.iRank);

    // Grow to tetrahedron (orient faces outside origin, CCW)
    Vector3 vA = ( m_hMPRSolver.hPortal.arrVertices[1]->vPointM - m_hMPRSolver.hPortal.arrVertices[0]->vPointM );
    Vector3 vB = ( m_hMPRSolver.hPortal.arrVertices[2]->vPointM - m_hMPRSolver.hPortal.arrVertices[0]->vPointM );
    vPortalDir = ( vA ^ vB );
    vPortalDir.Normalize();

    fDot = ( m_hMPRSolver.hPortal.arrVertices[0]->vPointM * vPortalDir );
    if ( fDot > 0.0f ) {
        m_hMPRSolver.hPortal.SwapSupports( 1, 2 );
        vPortalDir = -vPortalDir;
    }

    m_hMPRSolver.hPortal.arrVertices[3] = m_hMPRSolver.hSVAllocator._Allocate();
    _MPR_GetSupport( m_hMPRSolver.hPortal.arrVertices[3], vPortalDir );
    
    Bool bContinue = true;
    while( bContinue ) {
        fDot = ( m_hMPRSolver.hPortal.arrVertices[3]->vPointM * vPortalDir );
        if ( fDot < SCALAR_EPSILON ) {
            m_hMPRSolver.hSVAllocator._Free( m_hMPRSolver.hPortal.arrVertices[3] );
            m_hMPRSolver.hPortal.arrVertices[3] = NULL;
            return -1;
        }

        bContinue = false;

        // Test if origin outside (1,0,3)
        vA = ( m_hMPRSolver.hPortal.arrVertices[1]->vPointM ^ m_hMPRSolver.hPortal.arrVertices[3]->vPointM );
        fDot = ( m_hMPRSolver.hPortal.arrVertices[0]->vPointM * vA );
        if ( fDot < -SCALAR_EPSILON ) {
            m_hMPRSolver.hPortal.SwapSupports( 2, 3 );
            bContinue = true;
        }
        if ( !bContinue ) {
            // Test if origin outside (3,0,2)
            vA = ( m_hMPRSolver.hPortal.arrVertices[3]->vPointM ^ m_hMPRSolver.hPortal.arrVertices[2]->vPointM );
            fDot = ( m_hMPRSolver.hPortal.arrVertices[0]->vPointM * vA );
            if ( fDot < -SCALAR_EPSILON ) {
                m_hMPRSolver.hPortal.SwapSupports( 1, 3 );
                bContinue = true;
            }
        }
        if ( !bContinue )
            break;

        // Next direction
        vA = ( m_hMPRSolver.hPortal.arrVertices[1]->vPointM - m_hMPRSolver.hPortal.arrVertices[0]->vPointM );
        vB = ( m_hMPRSolver.hPortal.arrVertices[2]->vPointM - m_hMPRSolver.hPortal.arrVertices[0]->vPointM );
        vPortalDir = ( vA ^ vB );
        vPortalDir.Normalize();
        _MPR_GetSupport( m_hMPRSolver.hPortal.arrVertices[3], vPortalDir );
    }

    ++(m_hMPRSolver.hPortal.iRank);
    return 0;
}
Int GJK::_MPR_RefinePortal()
{
    Vector3 vPortalDir;
    _MPRSupportVertex * pSupport = NULL;

    for( UInt i = 0; i < MPR_MAX_ITERATIONS; ++i ) {
        // Portal direction : v0 through (1,2,3)
        m_hMPRSolver.hPortal.GetPortalDir( &vPortalDir );

        // Test if origin is inside portal
        if ( m_hMPRSolver.hPortal.ContainsOrigin(vPortalDir) )
            return 0;

        // Next support
        pSupport = m_hMPRSolver.hSVAllocator._Allocate();
        _MPR_GetSupport( pSupport, vPortalDir );

        // Test if we have enough to reach origin without exceeding tolerance
        if ( !(_MPR_CanContainOrigin(pSupport,vPortalDir)) || m_hMPRSolver.hPortal.TestTolerance(pSupport,vPortalDir) ) {
            m_hMPRSolver.hSVAllocator._Free( pSupport );
            return -1;
        }

        // Refine
        _MPR_ExpandPortal( &(m_hMPRSolver.hPortal), pSupport );
    }

    return -1;
}

Void GJK::_MPR_FindPosition( Vector3 * outPosition ) const
{
    Vector3 vPortalDir;
    m_hMPRSolver.hPortal.GetPortalDir( &vPortalDir );

    // Using barycentric coords
    Vector3 vTmp;
    Scalar fDot[4];

    vTmp = ( m_hMPRSolver.hPortal.arrVertices[1]->vPointM ^ m_hMPRSolver.hPortal.arrVertices[2]->vPointM );
    fDot[0] = ( m_hMPRSolver.hPortal.arrVertices[3]->vPointM * vTmp );
    vTmp = ( m_hMPRSolver.hPortal.arrVertices[3]->vPointM ^ m_hMPRSolver.hPortal.arrVertices[2]->vPointM );
    fDot[1] = ( m_hMPRSolver.hPortal.arrVertices[0]->vPointM * vTmp );
    vTmp = ( m_hMPRSolver.hPortal.arrVertices[0]->vPointM ^ m_hMPRSolver.hPortal.arrVertices[1]->vPointM );
    fDot[2] = ( m_hMPRSolver.hPortal.arrVertices[3]->vPointM * vTmp );
    vTmp = ( m_hMPRSolver.hPortal.arrVertices[2]->vPointM ^ m_hMPRSolver.hPortal.arrVertices[1]->vPointM );
    fDot[3] = ( m_hMPRSolver.hPortal.arrVertices[0]->vPointM * vTmp );

    Scalar fDotSum = ( fDot[0] + fDot[1] + fDot[2] + fDot[3] );
    if ( fDotSum < SCALAR_EPSILON ) {
        fDot[0] = 0.0f;

        vTmp = ( m_hMPRSolver.hPortal.arrVertices[2]->vPointM ^ m_hMPRSolver.hPortal.arrVertices[3]->vPointM );
        fDot[1] = ( vPortalDir * vTmp );
        vTmp = ( m_hMPRSolver.hPortal.arrVertices[3]->vPointM ^ m_hMPRSolver.hPortal.arrVertices[1]->vPointM );
        fDot[2] = ( vPortalDir * vTmp );
        vTmp = ( m_hMPRSolver.hPortal.arrVertices[1]->vPointM ^ m_hMPRSolver.hPortal.arrVertices[2]->vPointM );
        fDot[3] = ( vPortalDir * vTmp );

        fDotSum = ( fDot[1] + fDot[2] + fDot[3] );
    }

    Vector3 vA = Vector3::Null, vB = Vector3::Null;
    for( UInt i = 0; i < 4; ++i ) {
        vA += ( m_hMPRSolver.hPortal.arrVertices[i]->vPointA * fDot[i] );
        vB += ( m_hMPRSolver.hPortal.arrVertices[i]->vPointB * fDot[i] );
    }
    Scalar fInvDotSum = MathFn->Invert( fDotSum );
    vA *= fInvDotSum;
    vB *= fInvDotSum;

    *outPosition = ( vA + vB ) * 0.5f;
}
Void GJK::_MPR_FindPenetration( Vector3 * outPosition, Vector3 * outNormal, Scalar * outPenetration )
{
    Vector3 vPortalDir;
    _MPRSupportVertex * pSupport;

    for( UInt i = 0; i < MPR_MAX_ITERATIONS; ++i ) {
        // Portal direction : v0 through (1,2,3)
        m_hMPRSolver.hPortal.GetPortalDir( &vPortalDir );

        // Next support
        pSupport = m_hMPRSolver.hSVAllocator._Allocate();
        _MPR_GetSupport( pSupport, vPortalDir );

        // Test if we have reached tolerance
        if ( m_hMPRSolver.hPortal.TestTolerance(pSupport,vPortalDir) ) {
            *outPenetration = _MPR_TriangleDistanceSqr( outNormal, m_hMPRSolver.hPortal.arrVertices[1]->vPointM, m_hMPRSolver.hPortal.arrVertices[2]->vPointM, m_hMPRSolver.hPortal.arrVertices[3]->vPointM );
            *outPenetration = MathFn->Sqrt( *outPenetration );
            if ( _MPR_TestEquality(*outNormal, Vector3::Null) )
                *outNormal = vPortalDir;
            outNormal->Normalize();
            _MPR_FindPosition( outPosition );
            return;
        }

        // Refine
        _MPR_ExpandPortal( &(m_hMPRSolver.hPortal), pSupport );
    }
}

Void GJK::_MPR_Initialize()
{
    m_hMPRSolver.hPortal._Initialize();

    m_hMPRSolver.hSVAllocator._Reset();
}
Bool GJK::_MPR_Evaluate( Vector3 * outPosition, Vector3 * outNormal, Scalar * outPenetration )
{
    // Phase 1 : discovery
    Int iResult = _MPR_DiscoverPortal();

    // Phase 2 : refinement & penetration info
    switch( iResult ) {
        case 0: {
                iResult = _MPR_RefinePortal();
                if ( iResult < 0 )
                    return false;
                _MPR_FindPenetration( outPosition, outNormal, outPenetration );
            } break;
        case 1: {
                // Touching contact at v1
                *outPosition = ( m_hMPRSolver.hPortal.arrVertices[1]->vPointA + m_hMPRSolver.hPortal.arrVertices[1]->vPointB ) * 0.5f;
                *outNormal = Vector3::eI; // Arbitrary
                *outPenetration = 0.0f;
            } break;
        case 2: {
                // Segment contact on (0,1)
                *outPosition = ( m_hMPRSolver.hPortal.arrVertices[1]->vPointA + m_hMPRSolver.hPortal.arrVertices[1]->vPointB ) * 0.5f;
                *outNormal = m_hMPRSolver.hPortal.arrVertices[1]->vPointM;
                *outPenetration = outNormal->Normalize();
            } break;
        default:
            return false;
    }

    return true;
}
