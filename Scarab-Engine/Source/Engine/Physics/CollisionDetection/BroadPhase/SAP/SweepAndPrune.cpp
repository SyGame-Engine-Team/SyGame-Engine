/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/CollisionDetection/BroadPhase/SAP/SweepAndPrune.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Persitent-Incremental Sweep-And-Prune BroadPhase using
//               segmented interval lists ...
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
#include "SweepAndPrune.h"

#include "../../../PhysicsManager.h"

/////////////////////////////////////////////////////////////////////////////////
// SweepAndPrune implementation
SweepAndPrune::SweepAndPrune( PairListener * pPairListener ):
    BroadPhase( pPairListener ),
    m_mapProxies()
{
    // Proxy map
    m_mapProxies.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("Broadphase") );
    m_mapProxies.SetComparator( _Compare_ProxyIDs );
    m_mapProxies.Create();

    // Segmented interval list
    for( UInt k = 0; k < SAP_DIMENSION; ++k ) {
        // Create dimension array
        m_arrIntervals[k].UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("Broadphase") );
        m_arrIntervals[k].Create();

        // Create first empty interval
        m_arrIntervals[k].Push( _AllocateInterval( (k == 0) ) );
    }
}
SweepAndPrune::~SweepAndPrune()
{
    // Segmented interval list
    for( UInt k = 0; k < SAP_DIMENSION; ++k ) {
        // Destroy all intervals
        Array<SAPInterval*> & arrIntervals = m_arrIntervals[k];
        for( UInt i = 0; i < arrIntervals.Count(); ++i )
            _FreeInterval( arrIntervals[i], (k == 0) );

        // Destroy dimension array
        m_arrIntervals[k].Destroy();
    }

    // Proxy map
    m_mapProxies.Destroy();
}

Void SweepAndPrune::CreateProxy( IslandNode * pNode, const BVAABox & aabProxy )
{
    // Allocate a new proxy
    SAPProxyID idProxy = _GetProxyID( pNode );
    SAPProxy * pProxy = _AllocateProxy( pNode );

    // Already exists
    if ( pProxy == NULL )
        return;

    // Extract old/new values
    SAPBoundValues oldValues, newValues;
    oldValues.dwMinBounds[0] = 0xffffffff;
    oldValues.dwMinBounds[1] = 0xffffffff;
    oldValues.dwMinBounds[2] = 0xffffffff;
    oldValues.dwMaxBounds[0] = 0xffffffff;
    oldValues.dwMaxBounds[1] = 0xffffffff;
    oldValues.dwMaxBounds[2] = 0xffffffff;
    _ExtractBoundValues( &newValues, aabProxy );

    // Create Bounds
    UInt iMinInterval, iMaxInterval;
    SAPInterval *pMinInterval, *pMaxInterval;
    UInt iMinBound, iMaxBound;

    for( UInt k = 0; k < SAP_DIMENSION; ++k ) {
        Array<SAPInterval*> & arrIntervals = m_arrIntervals[k];

        // Locate min interval
        iMinInterval = _Interval_Lookup( k, newValues.dwMinBounds[k] );
        pMinInterval = arrIntervals[iMinInterval];

        // Check min interval for split
        if ( pMinInterval->iBoundCount >= SAP_INTERVAL_SIZE ) {
            _Interval_Split( k, iMinInterval );

            // Min goes right case
            if ( newValues.dwMinBounds[k] > arrIntervals[iMinInterval+1]->arrBounds[0].dwBoundValue ) {
                ++iMinInterval;
                pMinInterval = arrIntervals[iMinInterval];
            }
        }

        // Init Proxy links
        iMinBound = pMinInterval->iBoundCount;
        pProxy->iMinIntervals[k] = iMinInterval;
        pProxy->iMinBounds[k] = iMinBound;

        // Insert min bound
        pMinInterval->arrBounds[iMinBound].SetProxyID( idProxy, false );
        pMinInterval->arrBounds[iMinBound].dwBoundValue = newValues.dwMinBounds[k];
        ++(pMinInterval->iBoundCount);

        _PropagateBound( k, iMinInterval, iMinBound, pProxy, true, newValues, true );

        // Locate max interval
        iMaxInterval = _Interval_Lookup( k, newValues.dwMaxBounds[k] );
        pMaxInterval = arrIntervals[iMaxInterval];

        // Check max interval for split
        if ( pMaxInterval->iBoundCount >= SAP_INTERVAL_SIZE ) {
            _Interval_Split( k, iMaxInterval );

            // Max goes right case
            if ( newValues.dwMaxBounds[k] > arrIntervals[iMaxInterval+1]->arrBounds[0].dwBoundValue ) {
                ++iMaxInterval;
                pMaxInterval = arrIntervals[iMaxInterval];
            }
        }

        // Init Proxy links
        iMaxBound = pMaxInterval->iBoundCount;
        pProxy->iMaxIntervals[k] = iMaxInterval;
        pProxy->iMaxBounds[k] = iMaxBound;

        // Insert max bound
        pMaxInterval->arrBounds[iMaxBound].SetProxyID( idProxy, true );
        pMaxInterval->arrBounds[iMaxBound].dwBoundValue = newValues.dwMaxBounds[k];
        ++(pMaxInterval->iBoundCount);

        _PropagateBound( k, iMaxInterval, iMaxBound, pProxy, true, oldValues, true );

        // Lookup CheckProxies
        if ( k == 0 ) {
            SAPInterval * pInterval;
            CheckProxySet::Iterator it;
            SAPProxy * pCheckProxy;
            SAPBoundValues checkValues;

            for( UInt iInterval = iMinInterval; iInterval < iMaxInterval; ++iInterval ) {
                pInterval = arrIntervals[iInterval];

                // Signal overlaps
                for( it = pInterval->setCheckProxies.Begin(); !(it.IsNull()); ++it ) {
                    pCheckProxy = _GetProxy( it.Get() );
                    _ExtractBoundValues( &checkValues, pCheckProxy );
                    if ( _OverlapTest(newValues, checkValues) )
                        m_pPairManager->CreatePair( pProxy->pNode, pCheckProxy->pNode );
                }

                // Update CheckProxies
                pInterval->AddCheckProxy( idProxy );
            }
        }
    }
}

Void SweepAndPrune::DestroyProxy( IslandNode * pNode )
{
    // Get Proxy
    SAPProxyID idProxy = _GetProxyID( pNode );
    SAPProxy * pProxy = _GetProxy( idProxy );

    // Doesn't exist
    if ( pProxy == NULL )
        return;

    // Extract old/new values
    SAPBoundValues oldValues, newValues;
    _ExtractBoundValues( &oldValues, pProxy );
    newValues.dwMinBounds[0] = 0xffffffff;
    newValues.dwMinBounds[1] = 0xffffffff;
    newValues.dwMinBounds[2] = 0xffffffff;
    newValues.dwMaxBounds[0] = 0xffffffff;
    newValues.dwMaxBounds[1] = 0xffffffff;
    newValues.dwMaxBounds[2] = 0xffffffff;

    // Destroy Bounds
    UInt iMinInterval, iMaxInterval;
    SAPInterval *pMinInterval, *pMaxInterval;
    UInt iMinBound, iMaxBound;

    UInt iHalfIntervalSize = ( SAP_INTERVAL_SIZE >> 1 );

    for( UInt k = 0; k < SAP_DIMENSION; ++k ) {
        Array<SAPInterval*> & arrIntervals = m_arrIntervals[k];

        // Lookup CheckProxies
        if ( k == 0 ) {
            iMinInterval = pProxy->iMinIntervals[k];
            iMaxInterval = pProxy->iMaxIntervals[k];
            SAPInterval * pInterval;
            CheckProxySet::Iterator it;
            SAPProxy * pCheckProxy;
            SAPBoundValues checkValues;
            Bool bRemoved;

            for( UInt iInterval = iMinInterval; iInterval < iMaxInterval; ++iInterval ) {
                pInterval = arrIntervals[iInterval];

                // Update CheckProxies
                if ( pInterval->setRemovedCheckProxies.Search(idProxy) )
                    Assert( false );
                bRemoved = pInterval->setCheckProxies.Remove( idProxy );
                Assert( bRemoved );

                // Signal overlaps
                for( it = pInterval->setCheckProxies.Begin(); !(it.IsNull()); ++it ) {
                    pCheckProxy = _GetProxy( it.Get() );
                    _ExtractBoundValues( &checkValues, pCheckProxy );
                    if ( _OverlapTest( oldValues, checkValues ) )
                        m_pPairManager->DestroyPair( pProxy->pNode, pCheckProxy->pNode );
                }
            }
        }

        // Get max interval/bound
        iMaxInterval = pProxy->iMaxIntervals[k];
        pMaxInterval = arrIntervals[iMaxInterval];
        iMaxBound = pProxy->iMaxBounds[k];

        // Remove max bound
        pMaxInterval->arrBounds[iMaxBound].dwBoundValue = newValues.dwMaxBounds[k];
        _PropagateBound( k, iMaxInterval, iMaxBound, pProxy, false, newValues, true );

        iMaxBound = ( pMaxInterval->iBoundCount - 1 );
        pMaxInterval->arrBounds[iMaxBound].dwFlaggedProxyID = 0;
        pMaxInterval->arrBounds[iMaxBound].dwBoundValue = 0;
        --(pMaxInterval->iBoundCount);

        // Check max interval for merge
        if ( pMaxInterval->iBoundCount < iHalfIntervalSize ) {
            if ( iMaxInterval > 0 &&
                 (pMaxInterval->iBoundCount + arrIntervals[iMaxInterval-1]->iBoundCount) < SAP_INTERVAL_SIZE ) {
                _Interval_Merge( k, iMaxInterval - 1 );

                // Max goes left case
                --iMaxInterval;
                pMaxInterval = arrIntervals[iMaxInterval];
            } else if ( iMaxInterval < (arrIntervals.Count() - 1) &&
                        (pMaxInterval->iBoundCount + arrIntervals[iMaxInterval+1]->iBoundCount) < SAP_INTERVAL_SIZE )
                _Interval_Merge( k, iMaxInterval );
        }

        // Get min interval/bound
        iMinInterval = pProxy->iMinIntervals[k];
        pMinInterval = arrIntervals[iMinInterval];
        iMinBound = pProxy->iMinBounds[k];

        // Remove min bound
        pMinInterval->arrBounds[iMinBound].dwBoundValue = newValues.dwMinBounds[k];
        _PropagateBound( k, iMinInterval, iMinBound, pProxy, false, oldValues, true );

        iMinBound = ( pMinInterval->iBoundCount - 1 );
        pMinInterval->arrBounds[iMinBound].dwFlaggedProxyID = 0;
        pMinInterval->arrBounds[iMinBound].dwBoundValue = 0;
        --(pMinInterval->iBoundCount);

        // Check min interval for merge
        if ( pMinInterval->iBoundCount < iHalfIntervalSize ) {
            if ( iMinInterval > 0 &&
                 (pMinInterval->iBoundCount + arrIntervals[iMinInterval-1]->iBoundCount) < SAP_INTERVAL_SIZE ) {
                _Interval_Merge( k, iMinInterval - 1 );

                // Min goes left case
                --iMinInterval;
                pMinInterval = arrIntervals[iMinInterval];
            } else if ( iMinInterval < (arrIntervals.Count() - 1) &&
                        (pMinInterval->iBoundCount + arrIntervals[iMinInterval+1]->iBoundCount) < SAP_INTERVAL_SIZE )
                _Interval_Merge( k, iMinInterval );
        }

        // Clear proxy links
        pProxy->iMinIntervals[k] = INVALID_OFFSET;
        pProxy->iMinBounds[k] = INVALID_OFFSET;
        pProxy->iMaxIntervals[k] = INVALID_OFFSET;
        pProxy->iMaxBounds[k] = INVALID_OFFSET;
    }

    // Free the proxy
    _FreeProxy( idProxy );
}

Void SweepAndPrune::UpdateProxy( IslandNode * pNode, const BVAABox & aabProxy )
{
    // Get proxy
    SAPProxyID idProxy = _GetProxyID( pNode );
    SAPProxy * pProxy = _GetProxy( idProxy );

    // Doesn't exist
    if ( pProxy == NULL ) {
        Assert( false );
        return;
    }

    // Extract old & new values
    SAPBoundValues oldValues, newValues;
    _ExtractBoundValues( &oldValues, pProxy );
    _ExtractBoundValues( &newValues, aabProxy );

    // Update bounds
    UInt iMinInterval, iMaxInterval;
    UInt iMinBound, iMaxBound;

    DWord dwOldMin, dwOldMax;
    DWord dwNewMin, dwNewMax;

    for( UInt k = 0; k < SAP_DIMENSION; ++k ) {
        // Get interval array
        Array<SAPInterval*> & arrIntervals = m_arrIntervals[k];

        // Get proxy links
        iMinInterval = pProxy->iMinIntervals[k];
        iMaxInterval = pProxy->iMaxIntervals[k];
        iMinBound = pProxy->iMinBounds[k];
        iMaxBound = pProxy->iMaxBounds[k];

        // Saved values
        dwOldMin = oldValues.dwMinBounds[k];
        dwOldMax = oldValues.dwMaxBounds[k];
        dwNewMin = newValues.dwMinBounds[k];
        dwNewMax = newValues.dwMaxBounds[k];

        // Update values
        arrIntervals[iMinInterval]->arrBounds[iMinBound].dwBoundValue = dwNewMin;
        arrIntervals[iMaxInterval]->arrBounds[iMaxBound].dwBoundValue = dwNewMax;

        // Grow proxy : Min bound moving left
        if ( dwNewMin < dwOldMin )
            _PropagateBound( k, iMinInterval, iMinBound, pProxy, true, newValues, false );

        // Grow proxy : Max bound moving right
        if ( dwNewMax > dwOldMax )
            _PropagateBound( k, iMaxInterval, iMaxBound, pProxy, false, newValues, false );

        // Shrink proxy : Min bound moving right
        if ( dwNewMin > dwOldMin )
            _PropagateBound( k, iMinInterval, iMinBound, pProxy, false, oldValues, false );

        // Shrink proxy : Max bound moving left
        if ( dwNewMax < dwOldMax )
            _PropagateBound( k, iMaxInterval, iMaxBound, pProxy, true, oldValues, false );
    }
}

/////////////////////////////////////////////////////////////////////////////////

Void SweepAndPrune::_ExtractBoundValues( SAPBoundValues * outBoundValues, const BVAABox & aabProxy )
{
    // Pick min-max
    Vertex3 vMin = aabProxy.GetMin();
    Vertex3 vMax = aabProxy.GetMax();

    // Fill bound values
    for( UInt k = 0; k < SAP_DIMENSION; ++k ) {
        // Extract values
        outBoundValues->dwMinBounds[k] = _EncodeScalar( vMin[k] );
        outBoundValues->dwMaxBounds[k] = _EncodeScalar( vMax[k] );
    }
}
Void SweepAndPrune::_ExtractBoundValues( SAPBoundValues * outBoundValues, const SAPProxy * pProxy ) const
{
    // Fill bound values
    UInt iMinInterval, iMaxInterval;
    UInt iMinBound, iMaxBound;

    for( UInt k = 0; k < SAP_DIMENSION; ++k ) {
        // Get interval array
        const Array<SAPInterval*> & arrIntervals = m_arrIntervals[k];

        // Get proxy's indices
        iMinInterval = pProxy->iMinIntervals[k];
        iMinBound = pProxy->iMinBounds[k];
        iMaxInterval = pProxy->iMaxIntervals[k];
        iMaxBound = pProxy->iMaxBounds[k];

        // Extract values
        outBoundValues->dwMinBounds[k] = arrIntervals[iMinInterval]->arrBounds[iMinBound].dwBoundValue;
        outBoundValues->dwMaxBounds[k] = arrIntervals[iMaxInterval]->arrBounds[iMaxBound].dwBoundValue;
    }
}

Bool SweepAndPrune::_OverlapTest( const SAPBoundValues & leftAAB, const SAPBoundValues & rightAAB )
{
    for( UInt k = 0; k < SAP_DIMENSION; ++k ) {
        if ( leftAAB.dwMinBounds[k] > rightAAB.dwMaxBounds[k] )
	        return false;
        if ( leftAAB.dwMaxBounds[k] < rightAAB.dwMinBounds[k] )
	        return false;
    }
    return true;
}

SAPProxy * SweepAndPrune::_AllocateProxy( IslandNode * pNode )
{
    // Get ID
    SAPProxyID idProxy = _GetProxyID( pNode );

    // Already exists
    if ( m_mapProxies.Contains(idProxy) )
        return NULL;

    // Initialize
    SAPProxy newProxy;
    for( UInt k = 0; k < SAP_DIMENSION; ++k ) {
        newProxy.iMinIntervals[k] = INVALID_OFFSET;
        newProxy.iMinBounds[k] = INVALID_OFFSET;
        newProxy.iMaxIntervals[k] = INVALID_OFFSET;
        newProxy.iMaxBounds[k] = INVALID_OFFSET;
    }
    newProxy.pNode = pNode;

    // Insert in map
    ProxyMap::Iterator itProxy;
    Bool bInserted = m_mapProxies.Insert( idProxy, newProxy, &itProxy );
    Assert( bInserted && !(itProxy.IsNull()) );

    return itProxy.GetItemPtr();
}
Void SweepAndPrune::_FreeProxy( SAPProxyID idProxy )
{
    // Remove from map
    Bool bRemoved = m_mapProxies.Remove( idProxy );
    Assert( bRemoved );
}

SAPInterval * SweepAndPrune::_AllocateInterval( Bool bCheckProxies )
{
    // Create interval
    PhysicsFn->SelectMemory( TEXT("Broadphase") );
    SAPInterval * pInterval = New SAPInterval();
    PhysicsFn->UnSelectMemory();

    pInterval->iBoundCount = 0;
    for( UInt i = 0; i < SAP_INTERVAL_SIZE; ++i ) {
        pInterval->arrBounds[i].dwFlaggedProxyID = 0;
        pInterval->arrBounds[i].dwBoundValue = 0;
    }

    // Create CheckProxies
    if ( bCheckProxies ) {
        pInterval->setCheckProxies.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("Broadphase") );
        pInterval->setCheckProxies.SetComparator( _Compare_ProxyIDs );
        pInterval->setCheckProxies.Create();
        pInterval->setRemovedCheckProxies.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("Broadphase") );
        pInterval->setRemovedCheckProxies.SetComparator( _Compare_ProxyIDs );
        pInterval->setRemovedCheckProxies.Create();
    }

    return pInterval;
}
Void SweepAndPrune::_FreeInterval( SAPInterval * pInterval, Bool bCheckProxies )
{
    // Destroy CheckProxies
    if ( bCheckProxies ) {
        pInterval->setCheckProxies.Destroy();
        pInterval->setRemovedCheckProxies.Destroy();
    }

    // Destroy interval
    PhysicsFn->SelectMemory( TEXT("Broadphase") );
    Delete( pInterval );
    PhysicsFn->UnSelectMemory();
}

Void SweepAndPrune::_SignalPairs( SAPProxy * pCrossedProxy, SAPBound * pCrossedBound,
                                  SAPProxy * pMovingProxy, SAPBound * pMovingBound,
                                  Bool bLeft, const SAPBoundValues & testValues )
{
    if ( pCrossedBound->IsMin() == pMovingBound->IsMin() )
        return;

    if ( bLeft == pMovingBound->IsMin() ) {
        SAPBoundValues proxyValues;
        _ExtractBoundValues( &proxyValues, pCrossedProxy );

        if ( _OverlapTest(proxyValues, testValues) )
            m_pPairManager->CreatePair( pCrossedProxy->pNode, pMovingProxy->pNode );
    } else
        m_pPairManager->DestroyPair( pCrossedProxy->pNode, pMovingProxy->pNode );
}

Void SweepAndPrune::_OnSimpleSwap( SAPProxy * pCrossedProxy, SAPBound * pCrossedBound,
                                   SAPProxy * pMovingProxy, SAPBound * pMovingBound,
                                   UInt /*iDimension*/, Bool bLeft, const SAPBoundValues & testValues )
{
    // Signal pair changes
    _SignalPairs( pCrossedProxy, pCrossedBound, pMovingProxy, pMovingBound, bLeft, testValues );
}
Void SweepAndPrune::_OnCrossSwap( SAPInterval * pEnterInterval, SAPInterval * pLeaveInterval,
                                  SAPProxy * pCrossedProxy, SAPBound * pCrossedBound,
                                  SAPProxy * pMovingProxy, SAPBound * pMovingBound,
                                  UInt iDimension, Bool bLeft, const SAPBoundValues & testValues )
{
    // Signal pair changes
    _SignalPairs( pCrossedProxy, pCrossedBound, pMovingProxy, pMovingBound, bLeft, testValues );

    // Update CheckProxies
    if ( iDimension == 0 ) {
        if ( bLeft ) {
            if ( pMovingBound->IsMin() )
                pEnterInterval->AddCheckProxy( pMovingBound->GetProxyID() );
            else
                pEnterInterval->RemoveCheckProxy( pMovingBound->GetProxyID() );
            if ( pCrossedBound->IsMin() )
                pEnterInterval->RemoveCheckProxy( pCrossedBound->GetProxyID() );
            else
                pEnterInterval->AddCheckProxy( pCrossedBound->GetProxyID() );
        } else {
            if ( pMovingBound->IsMin() )
                pLeaveInterval->RemoveCheckProxy( pMovingBound->GetProxyID() );
            else
                pLeaveInterval->AddCheckProxy( pMovingBound->GetProxyID() );
            if ( pCrossedBound->IsMin() )
                pLeaveInterval->AddCheckProxy( pCrossedBound->GetProxyID() );
            else
                pLeaveInterval->RemoveCheckProxy( pCrossedBound->GetProxyID() );
        }
    }
}

Void SweepAndPrune::_PropagateBound( UInt iDimension, UInt iInterval, UInt iBound, SAPProxy * pProxy,
                                     Bool bLeft, const SAPBoundValues & testValues, Bool bClipToInterval )
{
    // Get interval array
    Array<SAPInterval*> & arrIntervals = m_arrIntervals[iDimension];

    // Get current interval
    UInt iCurInterval = iInterval;
    SAPInterval * pCurInterval = arrIntervals[iCurInterval];
    UInt iNextInterval;
    SAPInterval * pNextInterval;

    // Get current bound
    UInt iCurBound = iBound;
    SAPBound * pCurBound = ( pCurInterval->arrBounds + iCurBound );
    DWord dwBoundValue = ( pCurBound->dwBoundValue );
    UInt iNextBound;
    SAPBound * pNextBound;
    SAPProxy * pNextProxy;

    // Set walk direction
    Int iIncrement;
    UInt iLastInterval, iLastBound;
    if ( bLeft ) {
        iIncrement = -1;
        iLastInterval = 0;
        iLastBound = 0;
    } else {
        iIncrement = +1;
        iLastInterval = ( arrIntervals.Count() - 1 );
        iLastBound = ( pCurInterval->iBoundCount - 1 );
    }

    // Walk through intervals
    while( true ) {
        // Walk through bounds
        while( iCurBound != iLastBound ) {
            // Get next bound
            iNextBound = ( iCurBound + iIncrement );
            pNextBound = ( pCurInterval->arrBounds + iNextBound );
            pNextProxy = _GetProxy( pNextBound->GetProxyID() );

            // Good place, done
            if ( bLeft ) {
                if ( pNextBound->dwBoundValue <= dwBoundValue )
                    return;
            } else {
                if ( dwBoundValue <= pNextBound->dwBoundValue )
                    return;
            }

            // Signal simple swap
            _OnSimpleSwap( pNextProxy, pNextBound,
                           pProxy, pCurBound,
                           iDimension, bLeft, testValues );

            // Update Proxy links
            if ( pCurBound->IsMin() )
                pProxy->iMinBounds[iDimension] += iIncrement;
            else
                pProxy->iMaxBounds[iDimension] += iIncrement;
            if ( pNextBound->IsMin() )
                pNextProxy->iMinBounds[iDimension] -= iIncrement;
            else
                pNextProxy->iMaxBounds[iDimension] -= iIncrement;

            // Swap and continue
		    Swap<SAPBound>( pCurBound, pNextBound );
		    iCurBound = iNextBound;
            pCurBound = pNextBound;
        }

        // Check for end of array
        if ( (iCurInterval == iLastInterval) || bClipToInterval )
            return;

        // Get next interval
        iNextInterval = ( iCurInterval + iIncrement );
        pNextInterval = arrIntervals[iNextInterval];

        // Get next bound
        iNextBound = ( bLeft ? (pNextInterval->iBoundCount - 1) : 0 );
        pNextBound = ( pNextInterval->arrBounds + iNextBound );
        pNextProxy = _GetProxy( pNextBound->GetProxyID() );

        // Good place, done
        if ( bLeft ) {
            if ( pNextBound->dwBoundValue <= dwBoundValue )
                return;
        } else {
            if ( dwBoundValue <= pNextBound->dwBoundValue )
                return;
        }

        // Signal cross swap
        _OnCrossSwap( pNextInterval, pCurInterval,
                      pNextProxy, pNextBound,
                      pProxy, pCurBound,
                      iDimension, bLeft, testValues );

        // Update Proxy links
        if ( pCurBound->IsMin() ) {
            pProxy->iMinIntervals[iDimension] += iIncrement;
            pProxy->iMinBounds[iDimension] = iNextBound;
        } else {
            pProxy->iMaxIntervals[iDimension] += iIncrement;
            pProxy->iMaxBounds[iDimension] = iNextBound;
        }
        if ( pNextBound->IsMin() ) {
            pNextProxy->iMinIntervals[iDimension] -= iIncrement;
            pNextProxy->iMinBounds[iDimension] = iCurBound;
        } else {
            pNextProxy->iMaxIntervals[iDimension] -= iIncrement;
            pNextProxy->iMaxBounds[iDimension] = iCurBound;
        }

        // Swap and continue
		Swap<SAPBound>( pCurBound, pNextBound );
        iCurInterval = iNextInterval;
        pCurInterval = pNextInterval;
        iCurBound = iNextBound;
        pCurBound = pNextBound;
        iLastBound = ( bLeft ? 0 : (pCurInterval->iBoundCount - 1) );
    }
}

UInt SweepAndPrune::_Interval_Lookup( UInt iDimension, DWord dwBoundValue ) const
{
    // Get interval array
    const Array<SAPInterval*> & arrIntervals = m_arrIntervals[iDimension];
    UInt iLastInterval = ( arrIntervals.Count() - 1 );

    // Dichotomic search for the interval that should contain that bound
    UInt iMinInterval = 0;
    UInt iMaxInterval = iLastInterval;
    UInt iCurInterval;

    const SAPInterval * pInterval;
    DWord dwFirstBound, dwLastBound;

    while( iMinInterval < iMaxInterval ) {
        iCurInterval = ( (iMinInterval + iMaxInterval) >> 1 );
        pInterval = arrIntervals[iCurInterval];

        dwFirstBound = pInterval->arrBounds[0].dwBoundValue;
        dwLastBound = pInterval->arrBounds[pInterval->iBoundCount - 1].dwBoundValue;

        // Go left
        if ( dwBoundValue < dwFirstBound ) {
            if ( iCurInterval == 0 )
                return 0;
            iMaxInterval = iCurInterval - 1;
            continue;
        }

        // Go right
        if ( dwBoundValue > dwLastBound ) {
            if ( iCurInterval == iLastInterval )
                return iLastInterval;
            iMinInterval = iCurInterval + 1;
            continue;
        }

        // Found
        return iCurInterval;
    }

    // Not found, pick left one
    return iMaxInterval;
}

Void SweepAndPrune::_Interval_UpdateProxyLinks( UInt iDimension, UInt iInterval, Int iIntervalIncrement, Int iBoundIncrement )
{
    // Get interval array
    Array<SAPInterval*> & arrIntervals = m_arrIntervals[iDimension];
    if ( iInterval >= arrIntervals.Count() )
        return;

    SAPInterval * pCurInterval;
    SAPBound * pCurBound;
    SAPProxy * pCurProxy;
    UInt i, j;

    // First interval need bound links updated too
    pCurInterval = arrIntervals[iInterval];
    for ( j = 0; j < pCurInterval->iBoundCount; ++j ) {
        pCurBound = ( pCurInterval->arrBounds + j );
        pCurProxy = _GetProxy( pCurBound->GetProxyID() );

        if ( pCurBound->IsMin() ) {
            pCurProxy->iMinIntervals[iDimension] += iIntervalIncrement;
            pCurProxy->iMinBounds[iDimension] += iBoundIncrement;
        } else {
            pCurProxy->iMaxIntervals[iDimension] += iIntervalIncrement;
            pCurProxy->iMaxBounds[iDimension] += iBoundIncrement;
        }
    }

    // Walk through following intervals
    for ( i = iInterval + 1; i < arrIntervals.Count(); ++i ) {
        pCurInterval = arrIntervals[i];
        for ( j = 0; j < pCurInterval->iBoundCount; ++j ) {
            pCurBound = ( pCurInterval->arrBounds + j );
            pCurProxy = _GetProxy( pCurBound->GetProxyID() );

            if ( pCurBound->IsMin() )
                pCurProxy->iMinIntervals[iDimension] += iIntervalIncrement;
            else
                pCurProxy->iMaxIntervals[iDimension] += iIntervalIncrement;
        }
    }
}
Void SweepAndPrune::_Interval_Split( UInt iDimension, UInt iInterval )
{
    // Get interval array
    Array<SAPInterval*> & arrIntervals = m_arrIntervals[iDimension];
    Assert( iInterval < arrIntervals.Count() );

    // Get left/right indices
    UInt iLeftInterval = iInterval;
    UInt iRightInterval = iInterval + 1;

    // Get left interval
    SAPInterval * pLeftInterval = arrIntervals[iLeftInterval];

    // Create right interval
    SAPInterval * pRightInterval = _AllocateInterval( (iDimension == 0) );

    // Left/Right counts
    UInt iLeftBoundCount = ( pLeftInterval->iBoundCount >> 1 );
    UInt iRightBoundCount = ( pLeftInterval->iBoundCount - iLeftBoundCount );

    // Split bound array
    MemCopy( pRightInterval->arrBounds,
             pLeftInterval->arrBounds + iLeftBoundCount,
             iRightBoundCount * sizeof(SAPBound) );
    pLeftInterval->iBoundCount = iLeftBoundCount;
    pRightInterval->iBoundCount = iRightBoundCount;

    // Update interval array
    arrIntervals.Insert( iRightInterval, pRightInterval );
    _Interval_UpdateProxyLinks( iDimension, iRightInterval, +1, -((Int)iLeftBoundCount) );

    // Update CheckProxies
    if ( iDimension == 0 ) {
        // Copy left => right
        CheckProxySet::Iterator it;
        for( it = pLeftInterval->setCheckProxies.Begin(); !(it.IsNull()); ++it )
            pRightInterval->setCheckProxies.Insert( it.Get() );
        for( it = pLeftInterval->setRemovedCheckProxies.Begin(); !(it.IsNull()); ++it )
            pRightInterval->setRemovedCheckProxies.Insert( it.Get() );

        // Update left
        SAPBound * pCurBound;
        for( Int i = (iRightBoundCount - 1); i >= 0; --i ) {
            pCurBound = ( pRightInterval->arrBounds + i );
            if ( pCurBound->IsMin() )
                pLeftInterval->RemoveCheckProxy( pCurBound->GetProxyID() );
            else
                pLeftInterval->AddCheckProxy( pCurBound->GetProxyID() );
        }
    }
}
Void SweepAndPrune::_Interval_Merge( UInt iDimension, UInt iInterval )
{
    // Get interval array
    Array<SAPInterval*> & arrIntervals = m_arrIntervals[iDimension];
    Assert( iInterval < (arrIntervals.Count() - 1) );

    // Get left/right indices
    UInt iLeftInterval = iInterval;
    UInt iRightInterval = iInterval + 1;

    // Get left/right intervals
    SAPInterval * pLeftInterval = arrIntervals[iLeftInterval];
    SAPInterval * pRightInterval = arrIntervals[iRightInterval];

    // Left/Right counts
    UInt iLeftBoundCount = pLeftInterval->iBoundCount;
    UInt iRightBoundCount = pRightInterval->iBoundCount;

    // Update interval array
    _Interval_UpdateProxyLinks( iDimension, iRightInterval, -1, +((Int)iLeftBoundCount) );
    arrIntervals.Remove( iRightInterval, NULL, 1 );

    // Merge bound arrays
    MemCopy( pLeftInterval->arrBounds + iLeftBoundCount,
             pRightInterval->arrBounds,
             iRightBoundCount * sizeof(SAPBound) );
    pLeftInterval->iBoundCount += iRightBoundCount;

    // Update CheckProxies
    if ( iDimension == 0 ) {
        // Clear left
        pLeftInterval->setCheckProxies.Clear();
        pLeftInterval->setRemovedCheckProxies.Clear();

        // Copy right => left
        CheckProxySet::Iterator it;
        for( it = pRightInterval->setCheckProxies.Begin(); !(it.IsNull()); ++it )
            pLeftInterval->setCheckProxies.Insert( it.Get() );
        for( it = pRightInterval->setRemovedCheckProxies.Begin(); !(it.IsNull()); ++it )
            pLeftInterval->setRemovedCheckProxies.Insert( it.Get() );
    }

    // Destroy right interval
    _FreeInterval( pRightInterval, (iDimension == 0) );
}

