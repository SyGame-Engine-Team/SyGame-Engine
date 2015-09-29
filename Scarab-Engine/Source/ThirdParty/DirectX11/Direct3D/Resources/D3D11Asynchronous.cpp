/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/Resources/D3D11Asynchronous.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU resources : Asynchronous queries.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Third-Party Includes
#pragma warning(disable:4005)

#define WIN32_LEAN_AND_MEAN
#include <d3d11.h>

#undef DebugAssert

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "D3D11Asynchronous.h"

#include "../D3D11Renderer.h"

/////////////////////////////////////////////////////////////////////////////////
// D3D11Asynchronous implementation
D3D11Asynchronous::D3D11Asynchronous( D3D11Renderer * pRenderer )
{
    m_pRenderer = pRenderer;

    m_pAsynchronous = NULL;

    m_bTemporaryDestroyed = false;
}
D3D11Asynchronous::~D3D11Asynchronous()
{
    // nothing to do
}

Void D3D11Asynchronous::Create()
{
    DebugAssert( !(IsCreated()) );

    _NakedCreate();
}
Void D3D11Asynchronous::Destroy()
{
    DebugAssert( IsCreated() );

    if ( m_bTemporaryDestroyed )
        m_bTemporaryDestroyed = false;
    else
        _NakedDestroy();
}

Void D3D11Asynchronous::OnDestroyDevice()
{
    DebugAssert( !m_bTemporaryDestroyed );

    if ( m_pAsynchronous != NULL ) {
        _NakedDestroy();
        m_bTemporaryDestroyed = true;
    }
}
Void D3D11Asynchronous::OnRestoreDevice()
{
    DebugAssert( m_pAsynchronous == NULL );

    if ( m_bTemporaryDestroyed ) {
        _NakedCreate();
        m_bTemporaryDestroyed = false;
    }
}

UInt D3D11Asynchronous::GetDataSize() const
{
    DebugAssert( IsCreated() );

    return m_pAsynchronous->GetDataSize();
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11Query implementation
D3D11Query::D3D11Query( D3D11Renderer * pRenderer ):
    D3D11Asynchronous( pRenderer )
{
    m_pQuery = NULL;
}
D3D11Query::~D3D11Query()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

Void D3D11Query::_NakedCreate()
{
    m_pQuery = NULL;
    HRESULT hRes = m_pRenderer->m_pDevice->CreateQuery( (const D3D11_QUERY_DESC *)&m_hQueryDesc, &m_pQuery );
    DebugAssert( hRes == S_OK && m_pQuery != NULL );

    m_pAsynchronous = NULL;
    hRes = m_pQuery->QueryInterface( __uuidof(ID3D11Asynchronous), (Void**)&m_pAsynchronous );
    DebugAssert( hRes == S_OK && m_pAsynchronous != NULL );
}
Void D3D11Query::_NakedDestroy()
{
    m_pAsynchronous->Release();
    m_pAsynchronous = NULL;

    m_pQuery->Release();
    m_pQuery = NULL;
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11QueryCommandProcessing implementation
D3D11QueryCommandProcessing::D3D11QueryCommandProcessing( D3D11Renderer * pRenderer, Bool * pFinished ):
    D3D11Query( pRenderer )
{
    m_hQueryDesc.Query = D3D11_QUERY_EVENT;
    m_hQueryDesc.MiscFlags = 0;

    m_pFinished = pFinished;
    *m_pFinished = false;
}
D3D11QueryCommandProcessing::~D3D11QueryCommandProcessing()
{
    if ( IsCreated() )
        Destroy();
}

Bool D3D11QueryCommandProcessing::Update( D3D11DeferredContext * pContext, Bool bFlush )
{
    DebugAssert( IsCreated() );

    UInt iFlags = 0;
    if ( !bFlush )
        iFlags |= D3D11_ASYNC_GETDATA_DONOTFLUSH;

    ID3D11DeviceContext * pDeviceContext = m_pRenderer->m_pImmediateContext;
    if ( pContext != NULL )
        pDeviceContext = pContext->m_pDeferredContext;

    BOOL bResult = FALSE;
    HRESULT hRes = pDeviceContext->GetData( m_pQuery, &bResult, sizeof(BOOL), iFlags );
    if ( hRes == S_OK ) {
        *m_pFinished = ( bResult != FALSE );
        return true;
    }
    if ( hRes == S_FALSE ) {
        *m_pFinished = false;
        return false;
    }
    DebugAssert( false );
    return false;
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11QueryOcclusion implementation
D3D11QueryOcclusion::D3D11QueryOcclusion( D3D11Renderer * pRenderer, UInt64 * pVisibleSampleCount ):
    D3D11Query( pRenderer )
{
    m_hQueryDesc.Query = D3D11_QUERY_OCCLUSION;
    m_hQueryDesc.MiscFlags = 0;

    m_pVisibleSampleCount = pVisibleSampleCount;
    *m_pVisibleSampleCount = 0;
}
D3D11QueryOcclusion::~D3D11QueryOcclusion()
{
    if ( IsCreated() )
        Destroy();
}

Bool D3D11QueryOcclusion::Update( D3D11DeferredContext * pContext, Bool bFlush )
{
    DebugAssert( IsCreated() );

    UInt iFlags = 0;
    if ( !bFlush )
        iFlags |= D3D11_ASYNC_GETDATA_DONOTFLUSH;

    ID3D11DeviceContext * pDeviceContext = m_pRenderer->m_pImmediateContext;
    if ( pContext != NULL )
        pDeviceContext = pContext->m_pDeferredContext;

    UINT64 iResult = 0;
    HRESULT hRes = pDeviceContext->GetData( m_pQuery, &iResult, sizeof(UINT64), iFlags );
    if ( hRes == S_OK ) {
        *m_pVisibleSampleCount = iResult;
        return true;
    }
    if ( hRes == S_FALSE ) {
        *m_pVisibleSampleCount = 0;
        return false;
    }
    DebugAssert( false );
    return false;
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11QueryTimeStampFrequency implementation
D3D11QueryTimeStampFrequency::D3D11QueryTimeStampFrequency( D3D11Renderer * pRenderer, UInt64 * pFrequency, Bool * pReliableTimeStamps ):
    D3D11Query( pRenderer )
{
    m_hQueryDesc.Query = D3D11_QUERY_TIMESTAMP_DISJOINT;
    m_hQueryDesc.MiscFlags = 0;

    m_pFrequency = pFrequency;
    m_pReliableTimeStamps = pReliableTimeStamps;
    *m_pFrequency = 0;
    *m_pReliableTimeStamps = false;
}
D3D11QueryTimeStampFrequency::~D3D11QueryTimeStampFrequency()
{
    if ( IsCreated() )
        Destroy();
}

Bool D3D11QueryTimeStampFrequency::Update( D3D11DeferredContext * pContext, Bool bFlush )
{
    DebugAssert( IsCreated() );

    UInt iFlags = 0;
    if ( !bFlush )
        iFlags |= D3D11_ASYNC_GETDATA_DONOTFLUSH;

    ID3D11DeviceContext * pDeviceContext = m_pRenderer->m_pImmediateContext;
    if ( pContext != NULL )
        pDeviceContext = pContext->m_pDeferredContext;

    D3D11_QUERY_DATA_TIMESTAMP_DISJOINT hResult;
    HRESULT hRes = pDeviceContext->GetData( m_pQuery, &hResult, sizeof(D3D11_QUERY_DATA_TIMESTAMP_DISJOINT), iFlags );
    if ( hRes == S_OK ) {
        *m_pFrequency = hResult.Frequency;
        *m_pReliableTimeStamps = ( hResult.Disjoint == FALSE );
        return true;
    }
    if ( hRes == S_FALSE ) {
        *m_pFrequency = 0;
        *m_pReliableTimeStamps = false;
        return false;
    }
    DebugAssert( false );
    return false;
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11QueryTimeStamp implementation
D3D11QueryTimeStamp::D3D11QueryTimeStamp( D3D11Renderer * pRenderer, UInt64 * pTimeStamp ):
    D3D11Query( pRenderer )
{
    m_hQueryDesc.Query = D3D11_QUERY_TIMESTAMP;
    m_hQueryDesc.MiscFlags = 0;

    m_pTimeStamp = pTimeStamp;
    *m_pTimeStamp = 0;
}
D3D11QueryTimeStamp::~D3D11QueryTimeStamp()
{
    if ( IsCreated() )
        Destroy();
}

Bool D3D11QueryTimeStamp::Update( D3D11DeferredContext * pContext, Bool bFlush )
{
    DebugAssert( IsCreated() );

    UInt iFlags = 0;
    if ( !bFlush )
        iFlags |= D3D11_ASYNC_GETDATA_DONOTFLUSH;

    ID3D11DeviceContext * pDeviceContext = m_pRenderer->m_pImmediateContext;
    if ( pContext != NULL )
        pDeviceContext = pContext->m_pDeferredContext;

    UINT64 iResult = 0;
    HRESULT hRes = pDeviceContext->GetData( m_pQuery, &iResult, sizeof(UINT64), iFlags );
    if ( hRes == S_OK ) {
        *m_pTimeStamp = iResult;
        return true;
    }
    if ( hRes == S_FALSE ) {
        *m_pTimeStamp = 0;
        return false;
    }
    DebugAssert( false );
    return false;
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11QueryStatsPipeline implementation
D3D11QueryStatsPipeline::D3D11QueryStatsPipeline( D3D11Renderer * pRenderer, UInt64 * pIAVertexCount, UInt64 * pIAPrimitiveCount, UInt64 * pVSInvocationCount, UInt64 * pGSInvocationCount, UInt64 * pGSPrimitiveCount,
                                                  UInt64 * pPSInvocationCount, UInt64 * pHSInvocationCount, UInt64 * pDSInvocationCount, UInt64 * pCSInvocationCount, UInt64 * pRasterizedPrimitiveCount, UInt64 * pRenderedPrimitiveCount ):
    D3D11Query( pRenderer )
{
    m_hQueryDesc.Query = D3D11_QUERY_PIPELINE_STATISTICS;
    m_hQueryDesc.MiscFlags = 0;

    m_pIAVertexCount = pIAVertexCount;
    m_pIAPrimitiveCount = pIAPrimitiveCount;
    m_pVSInvocationCount = pVSInvocationCount;
    m_pGSInvocationCount = pGSInvocationCount;
    m_pGSPrimitiveCount = pGSPrimitiveCount;
    m_pPSInvocationCount = pPSInvocationCount;
    m_pHSInvocationCount = pHSInvocationCount;
    m_pDSInvocationCount = pDSInvocationCount;
    m_pCSInvocationCount = pCSInvocationCount;
    m_pRasterizedPrimitiveCount = pRasterizedPrimitiveCount;
    m_pRenderedPrimitiveCount = pRenderedPrimitiveCount;
    *m_pIAVertexCount = 0;
    *m_pIAPrimitiveCount = 0;
    *m_pVSInvocationCount = 0;
    *m_pGSInvocationCount = 0;
    *m_pGSPrimitiveCount = 0;
    *m_pPSInvocationCount = 0;
    *m_pHSInvocationCount = 0;
    *m_pDSInvocationCount = 0;
    *m_pCSInvocationCount = 0;
    *m_pRasterizedPrimitiveCount = 0;
    *m_pRenderedPrimitiveCount = 0;
}
D3D11QueryStatsPipeline::~D3D11QueryStatsPipeline()
{
    if ( IsCreated() )
        Destroy();
}

Bool D3D11QueryStatsPipeline::Update( D3D11DeferredContext * pContext, Bool bFlush )
{
    DebugAssert( IsCreated() );

    UInt iFlags = 0;
    if ( !bFlush )
        iFlags |= D3D11_ASYNC_GETDATA_DONOTFLUSH;

    ID3D11DeviceContext * pDeviceContext = m_pRenderer->m_pImmediateContext;
    if ( pContext != NULL )
        pDeviceContext = pContext->m_pDeferredContext;

    D3D11_QUERY_DATA_PIPELINE_STATISTICS hResult;
    HRESULT hRes = pDeviceContext->GetData( m_pQuery, &hResult, sizeof(D3D11_QUERY_DATA_PIPELINE_STATISTICS), iFlags );
    if ( hRes == S_OK ) {
        *m_pIAVertexCount = hResult.IAVertices;
        *m_pIAPrimitiveCount = hResult.IAPrimitives;
        *m_pVSInvocationCount = hResult.VSInvocations;
        *m_pGSInvocationCount = hResult.GSInvocations;
        *m_pGSPrimitiveCount = hResult.GSPrimitives;
        *m_pPSInvocationCount = hResult.PSInvocations;
        *m_pHSInvocationCount = hResult.HSInvocations;
        *m_pDSInvocationCount = hResult.DSInvocations;
        *m_pCSInvocationCount = hResult.CSInvocations;
        *m_pRasterizedPrimitiveCount = hResult.CInvocations;
        *m_pRenderedPrimitiveCount = hResult.CPrimitives;
        return true;
    }
    if ( hRes == S_FALSE ) {
        *m_pIAVertexCount = 0;
        *m_pIAPrimitiveCount = 0;
        *m_pVSInvocationCount = 0;
        *m_pGSInvocationCount = 0;
        *m_pGSPrimitiveCount = 0;
        *m_pPSInvocationCount = 0;
        *m_pHSInvocationCount = 0;
        *m_pDSInvocationCount = 0;
        *m_pCSInvocationCount = 0;
        *m_pRasterizedPrimitiveCount = 0;
        *m_pRenderedPrimitiveCount = 0;
        return false;
    }
    DebugAssert( false );
    return false;
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11QueryStatsStreamOutput implementation
D3D11QueryStatsStreamOutput::D3D11QueryStatsStreamOutput( D3D11Renderer * pRenderer, UInt64 * pRealPrimitiveCount, UInt64 * pWrittenPrimitiveCount, UInt iStream ):
    D3D11Query( pRenderer )
{
    if ( iStream == INVALID_OFFSET )
        m_hQueryDesc.Query = D3D11_QUERY_SO_STATISTICS;
    else {
        DebugAssert( iStream < D3D11_SO_BUFFER_SLOT_COUNT );
        m_hQueryDesc.Query = (D3D11_QUERY)( D3D11_QUERY_SO_STATISTICS_STREAM0 + (iStream << 1) );
    }
    m_hQueryDesc.MiscFlags = 0;

    m_pRealPrimitiveCount = pRealPrimitiveCount;
    m_pWrittenPrimitiveCount = pWrittenPrimitiveCount;
    *m_pRealPrimitiveCount = 0;
    *m_pWrittenPrimitiveCount = 0;
}
D3D11QueryStatsStreamOutput::~D3D11QueryStatsStreamOutput()
{
    if ( IsCreated() )
        Destroy();
}

Bool D3D11QueryStatsStreamOutput::Update( D3D11DeferredContext * pContext, Bool bFlush )
{
    DebugAssert( IsCreated() );

    UInt iFlags = 0;
    if ( !bFlush )
        iFlags |= D3D11_ASYNC_GETDATA_DONOTFLUSH;

    ID3D11DeviceContext * pDeviceContext = m_pRenderer->m_pImmediateContext;
    if ( pContext != NULL )
        pDeviceContext = pContext->m_pDeferredContext;

    D3D11_QUERY_DATA_SO_STATISTICS hResult;
    HRESULT hRes = pDeviceContext->GetData( m_pQuery, &hResult, sizeof(D3D11_QUERY_DATA_SO_STATISTICS), iFlags );
    if ( hRes == S_OK ) {
        *m_pRealPrimitiveCount = hResult.PrimitivesStorageNeeded;
        *m_pWrittenPrimitiveCount = hResult.NumPrimitivesWritten;
        return true;
    }
    if ( hRes == S_FALSE ) {
        *m_pRealPrimitiveCount = 0;
        *m_pWrittenPrimitiveCount = 0;
        return false;
    }
    DebugAssert( false );
    return false;
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11Predicate implementation
D3D11Predicate::D3D11Predicate( D3D11Renderer * pRenderer ):
    D3D11Asynchronous( pRenderer )
{
    m_pPredicate = NULL;
    m_pPredicateValue = NULL;
}
D3D11Predicate::~D3D11Predicate()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

Void D3D11Predicate::_NakedCreate()
{
    m_pPredicate = NULL;
    HRESULT hRes = m_pRenderer->m_pDevice->CreatePredicate( (const D3D11_QUERY_DESC *)&m_hPredicateDesc, &m_pPredicate );
    DebugAssert( hRes == S_OK && m_pPredicate != NULL );

    m_pAsynchronous = NULL;
    hRes = m_pPredicate->QueryInterface( __uuidof(ID3D11Asynchronous), (Void**)&m_pAsynchronous );
    DebugAssert( hRes == S_OK && m_pAsynchronous != NULL );
}
Void D3D11Predicate::_NakedDestroy()
{
    m_pAsynchronous->Release();
    m_pAsynchronous = NULL;

    m_pPredicate->Release();
    m_pPredicate = NULL;
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11PredicateOcclusion implementation
D3D11PredicateOcclusion::D3D11PredicateOcclusion( D3D11Renderer * pRenderer, Bool * pVisible, Bool bDrawWhenUnsure ):
    D3D11Predicate( pRenderer )
{
    m_hPredicateDesc.Query = D3D11_QUERY_OCCLUSION_PREDICATE;
    m_hPredicateDesc.MiscFlags = 0;
    if ( bDrawWhenUnsure )
        m_hPredicateDesc.MiscFlags |= D3D11_QUERY_MISC_PREDICATEHINT;

    m_pPredicateValue = pVisible;
    *m_pPredicateValue = false;
}
D3D11PredicateOcclusion::~D3D11PredicateOcclusion()
{
    if ( IsCreated() )
        Destroy();
}

Bool D3D11PredicateOcclusion::DrawWhenUnsure() const
{
    return ( (m_hPredicateDesc.MiscFlags & D3D11_QUERY_MISC_PREDICATEHINT) != 0 );
}

Bool D3D11PredicateOcclusion::Update( D3D11DeferredContext * pContext, Bool bFlush )
{
    DebugAssert( IsCreated() );

    UInt iFlags = 0;
    if ( !bFlush )
        iFlags |= D3D11_ASYNC_GETDATA_DONOTFLUSH;

    ID3D11DeviceContext * pDeviceContext = m_pRenderer->m_pImmediateContext;
    if ( pContext != NULL )
        pDeviceContext = pContext->m_pDeferredContext;

    BOOL bResult = FALSE;
    HRESULT hRes = pDeviceContext->GetData( m_pPredicate, &bResult, sizeof(BOOL), iFlags );
    if ( hRes == S_OK ) {
        *m_pPredicateValue = ( bResult != FALSE );
        return true;
    }
    if ( hRes == S_FALSE ) {
        *m_pPredicateValue = false;
        return false;
    }
    DebugAssert( false );
    return false;
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11PredicateStreamOutputOverflow implementation
D3D11PredicateStreamOutputOverflow::D3D11PredicateStreamOutputOverflow( D3D11Renderer * pRenderer, Bool * pOverflow, UInt iStream ):
    D3D11Predicate( pRenderer )
{
    if ( iStream == INVALID_OFFSET )
        m_hPredicateDesc.Query = D3D11_QUERY_SO_OVERFLOW_PREDICATE;
    else {
        DebugAssert( iStream < D3D11_SO_BUFFER_SLOT_COUNT );
        m_hPredicateDesc.Query = (D3D11_QUERY)( D3D11_QUERY_SO_OVERFLOW_PREDICATE_STREAM0 + (iStream << 1) );
    }
    m_hPredicateDesc.MiscFlags = 0;

    m_pPredicateValue = pOverflow;
    *m_pPredicateValue = false;
}
D3D11PredicateStreamOutputOverflow::~D3D11PredicateStreamOutputOverflow()
{
    if ( IsCreated() )
        Destroy();
}

Bool D3D11PredicateStreamOutputOverflow::Update( D3D11DeferredContext * pContext, Bool bFlush )
{
    DebugAssert( IsCreated() );

    UInt iFlags = 0;
    if ( !bFlush )
        iFlags |= D3D11_ASYNC_GETDATA_DONOTFLUSH;

    ID3D11DeviceContext * pDeviceContext = m_pRenderer->m_pImmediateContext;
    if ( pContext != NULL )
        pDeviceContext = pContext->m_pDeferredContext;

    BOOL bResult = FALSE;
    HRESULT hRes = pDeviceContext->GetData( m_pPredicate, &bResult, sizeof(BOOL), iFlags );
    if ( hRes == S_OK ) {
        *m_pPredicateValue = ( bResult != FALSE );
        return true;
    }
    if ( hRes == S_FALSE ) {
        *m_pPredicateValue = false;
        return false;
    }
    DebugAssert( false );
    return false;
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11Counter implementation
D3D11Counter::D3D11Counter( D3D11Renderer * pRenderer, UInt iCounterID, D3D11CounterType * pType, Void * pCount ):
    D3D11Asynchronous( pRenderer )
{
    m_pCounter = NULL;

    m_hCounterDesc.Counter = (D3D11_COUNTER)( D3D11_COUNTER_DEVICE_DEPENDENT_0 + iCounterID );
    m_hCounterDesc.MiscFlags = 0;

    D3D11_COUNTER_TYPE iType;
    UInt iActiveHardwareCounters;
    HRESULT hRes = m_pRenderer->m_pDevice->CheckCounter( (const D3D11_COUNTER_DESC *)&m_hCounterDesc, &iType, &iActiveHardwareCounters, NULL, NULL, NULL, NULL, NULL, NULL );
    DebugAssert( hRes == S_OK && iActiveHardwareCounters > 0 );

    m_pType = pType;
    m_pCount = pCount;
    *m_pType = D3D11CounterTypeFromD3D11[iType];
    switch( *m_pType ) {
        case D3D11COUNTER_FLOAT: *((Float*)m_pCount) = 0.0f; break;
        case D3D11COUNTER_WORD:  *((Word*)m_pCount) = 0; break;
        case D3D11COUNTER_DWORD: *((DWord*)m_pCount) = 0; break;
        case D3D11COUNTER_QWORD: *((QWord*)m_pCount) = 0; break;
        default: DebugAssert( false ); break;
    }
}
D3D11Counter::~D3D11Counter()
{
    if ( IsCreated() )
        Destroy();
}

Void D3D11Counter::CheckDesc( D3D11Renderer * pRenderer, UInt iCounterID, D3D11CounterDesc * outCounterDesc )
{
    outCounterDesc->iCounterID = iCounterID;
    outCounterDesc->iFlags = 0;

    D3D11_COUNTER_DESC hDesc;
    hDesc.Counter = (D3D11_COUNTER)( D3D11_COUNTER_DEVICE_DEPENDENT_0 + iCounterID );
    hDesc.MiscFlags = 0;

    D3D11_COUNTER_TYPE iTmpType = D3D11_COUNTER_TYPE_FLOAT32;
    HRESULT hRes = pRenderer->m_pDevice->CheckCounter( &hDesc, &iTmpType, &(outCounterDesc->iActiveHardwareCounters), outCounterDesc->strCounterName, &(outCounterDesc->iCounterNameLength),
                                                       outCounterDesc->strUnitName, &(outCounterDesc->iUnitNameLength), outCounterDesc->strDescription, &(outCounterDesc->iDescriptionLength) );
    DebugAssert( hRes == S_OK );

    outCounterDesc->iType = D3D11CounterTypeFromD3D11[iTmpType];
}

Bool D3D11Counter::Update( D3D11DeferredContext * pContext, Bool bFlush )
{
    DebugAssert( IsCreated() );

    UInt iFlags = 0;
    if ( !bFlush )
        iFlags |= D3D11_ASYNC_GETDATA_DONOTFLUSH;

    ID3D11DeviceContext * pDeviceContext = m_pRenderer->m_pImmediateContext;
    if ( pContext != NULL )
        pDeviceContext = pContext->m_pDeferredContext;

    switch( *m_pType ) {
        case D3D11COUNTER_FLOAT: {
                FLOAT fResult = 0.0f;
                HRESULT hRes = pDeviceContext->GetData( m_pCounter, &fResult, sizeof(FLOAT), iFlags );
                if ( hRes == S_OK ) {
                    *((Float*)m_pCount) = fResult;
                    return true;
                }
                if ( hRes == S_FALSE ) {
                    *((Float*)m_pCount) = 0.0f;
                    return false;
                }
            } break;
        case D3D11COUNTER_WORD: {
                USHORT iResult = 0;
                HRESULT hRes = pDeviceContext->GetData( m_pCounter, &iResult, sizeof(USHORT), iFlags );
                if ( hRes == S_OK ) {
                    *((Word*)m_pCount) = iResult;
                    return true;
                }
                if ( hRes == S_FALSE ) {
                    *((Word*)m_pCount) = 0;
                    return false;
                }
            } break;
        case D3D11COUNTER_DWORD: {
                ULONG iResult = 0;
                HRESULT hRes = pDeviceContext->GetData( m_pCounter, &iResult, sizeof(ULONG), iFlags );
                if ( hRes == S_OK ) {
                    *((DWord*)m_pCount) = iResult;
                    return true;
                }
                if ( hRes == S_FALSE ) {
                    *((DWord*)m_pCount) = 0;
                    return false;
                }
            } break;
        case D3D11COUNTER_QWORD: {
                UINT64 iResult = 0;
                HRESULT hRes = pDeviceContext->GetData( m_pCounter, &iResult, sizeof(UINT64), iFlags );
                if ( hRes == S_OK ) {
                    *((QWord*)m_pCount) = iResult;
                    return true;
                }
                if ( hRes == S_FALSE ) {
                    *((QWord*)m_pCount) = 0;
                    return false;
                }
            } break;
        default: DebugAssert( false ); break;
    }
    DebugAssert( false );
    return false;
}

/////////////////////////////////////////////////////////////////////////////////

Void D3D11Counter::_NakedCreate()
{
    m_pCounter = NULL;
    HRESULT hRes = m_pRenderer->m_pDevice->CreateCounter( (const D3D11_COUNTER_DESC *)&m_hCounterDesc, &m_pCounter );
    DebugAssert( hRes == S_OK && m_pCounter != NULL );

    m_pAsynchronous = NULL;
    hRes = m_pCounter->QueryInterface( __uuidof(ID3D11Asynchronous), (Void**)&m_pAsynchronous );
    DebugAssert( hRes == S_OK && m_pAsynchronous != NULL );
}
Void D3D11Counter::_NakedDestroy()
{
    m_pAsynchronous->Release();
    m_pAsynchronous = NULL;

    m_pCounter->Release();
    m_pCounter = NULL;
}

