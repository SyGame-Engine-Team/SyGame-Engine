/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Resources/GPUAsynchronous.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU Resource : Asynchronous queries, predicates & counters
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "GPUAsynchronous.h"

#include "../RenderingManager.h"

/////////////////////////////////////////////////////////////////////////////////
// GPUAsynchronous implementation
GPUAsynchronous::GPUAsynchronous()
{
    m_pDeviceAsynchronous = NULL;
}
GPUAsynchronous::~GPUAsynchronous()
{
    // nothing to do
}

Void GPUAsynchronous::Bind()
{
    Assert( !(IsBound()) );

    m_pDeviceAsynchronous->Create();

    RenderingFn->_RegisterAsynchronous( this );
}
Void GPUAsynchronous::UnBind()
{
    Assert( IsBound() );

    RenderingFn->_UnRegisterAsynchronous( this );

    m_pDeviceAsynchronous->Destroy();
}

Bool GPUAsynchronous::Update( GPUDeferredContext * pContext, Bool bFlush )
{
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    return m_pDeviceAsynchronous->Update( pDeviceContext, bFlush );
}

/////////////////////////////////////////////////////////////////////////////////
// GPUQuery implementation
GPUQuery::GPUQuery()
{
    m_pDeviceQuery = NULL;
}
GPUQuery::~GPUQuery()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// GPUQueryCommandProcessing implementation
GPUQueryCommandProcessing::GPUQueryCommandProcessing():
    m_hDeviceQueryCommandProcessing( RenderingFn->m_pDeviceRenderer, &Finished )
{
    m_pDeviceQuery = &m_hDeviceQueryCommandProcessing;
    m_pDeviceAsynchronous = m_pDeviceQuery;
}
GPUQueryCommandProcessing::~GPUQueryCommandProcessing()
{
    if ( IsBound() )
        UnBind();
}

/////////////////////////////////////////////////////////////////////////////////
// GPUQueryOcclusion implementation
GPUQueryOcclusion::GPUQueryOcclusion():
    m_hDeviceQueryOcclusion( RenderingFn->m_pDeviceRenderer, &VisibleSampleCount )
{
    m_pDeviceQuery = &m_hDeviceQueryOcclusion;
    m_pDeviceAsynchronous = m_pDeviceQuery;
}
GPUQueryOcclusion::~GPUQueryOcclusion()
{
    if ( IsBound() )
        UnBind();
}

/////////////////////////////////////////////////////////////////////////////////
// GPUQueryTimeStampFrequency implementation
GPUQueryTimeStampFrequency::GPUQueryTimeStampFrequency():
    m_hDeviceQueryTimeStampFrequency( RenderingFn->m_pDeviceRenderer, &Frequency, &ReliableTimeStamps )
{
    m_pDeviceQuery = &m_hDeviceQueryTimeStampFrequency;
    m_pDeviceAsynchronous = m_pDeviceQuery;
}
GPUQueryTimeStampFrequency::~GPUQueryTimeStampFrequency()
{
    if ( IsBound() )
        UnBind();
}

/////////////////////////////////////////////////////////////////////////////////
// GPUQueryTimeStamp implementation
GPUQueryTimeStamp::GPUQueryTimeStamp():
    m_hDeviceQueryTimeStamp( RenderingFn->m_pDeviceRenderer, &TimeStamp )
{
    m_pDeviceQuery = &m_hDeviceQueryTimeStamp;
    m_pDeviceAsynchronous = m_pDeviceQuery;
}
GPUQueryTimeStamp::~GPUQueryTimeStamp()
{
    if ( IsBound() )
        UnBind();
}

/////////////////////////////////////////////////////////////////////////////////
// GPUQueryStatsPipeline implementation
GPUQueryStatsPipeline::GPUQueryStatsPipeline():
    m_hDeviceQueryStatsPipeline( RenderingFn->m_pDeviceRenderer, &IAVertexCount, &IAPrimitiveCount, &VSInvocationCount, &GSInvocationCount, &GSPrimitiveCount,
                                 &PSInvocationCount, &HSInvocationCount, &DSInvocationCount, &CSInvocationCount, &RasterizedPrimitiveCount, &RenderedPrimitiveCount )
{
    m_pDeviceQuery = &m_hDeviceQueryStatsPipeline;
    m_pDeviceAsynchronous = m_pDeviceQuery;
}
GPUQueryStatsPipeline::~GPUQueryStatsPipeline()
{
    if ( IsBound() )
        UnBind();
}

/////////////////////////////////////////////////////////////////////////////////
// GPUQueryStatsStreamOutput implementation
GPUQueryStatsStreamOutput::GPUQueryStatsStreamOutput( UInt iStream ):
    m_hDeviceQueryStatsStreamOutput( RenderingFn->m_pDeviceRenderer, &RealPrimitiveCount, &WrittenPrimitiveCount, iStream )
{
    m_pDeviceQuery = &m_hDeviceQueryStatsStreamOutput;
    m_pDeviceAsynchronous = m_pDeviceQuery;
}
GPUQueryStatsStreamOutput::~GPUQueryStatsStreamOutput()
{
    if ( IsBound() )
        UnBind();
}

/////////////////////////////////////////////////////////////////////////////////
// GPUPredicate implementation
GPUPredicate::GPUPredicate()
{
    m_pDevicePredicate = NULL;
}
GPUPredicate::~GPUPredicate()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// GPUPredicateOcclusion implementation
GPUPredicateOcclusion::GPUPredicateOcclusion( Bool bDrawWhenUnsure ):
    m_hDevicePredicateOcclusion( RenderingFn->m_pDeviceRenderer, &Visible, bDrawWhenUnsure )
{
    m_pDevicePredicate = &m_hDevicePredicateOcclusion;
    m_pDeviceAsynchronous = m_pDevicePredicate;
}
GPUPredicateOcclusion::~GPUPredicateOcclusion()
{
    if ( IsBound() )
        UnBind();
}

/////////////////////////////////////////////////////////////////////////////////
// GPUPredicateStreamOutputOverflow implementation
GPUPredicateStreamOutputOverflow::GPUPredicateStreamOutputOverflow( UInt iStream ):
    m_hDevicePredicateStreamOutputOverflow( RenderingFn->m_pDeviceRenderer, &Overflow, iStream )
{
    m_pDevicePredicate = &m_hDevicePredicateStreamOutputOverflow;
    m_pDeviceAsynchronous = m_pDevicePredicate;
}
GPUPredicateStreamOutputOverflow::~GPUPredicateStreamOutputOverflow()
{
    if ( IsBound() )
        UnBind();
}

/////////////////////////////////////////////////////////////////////////////////
// GPUCounter implementation
GPUCounter::GPUCounter( UInt iCounterID ):
    m_hDeviceCounter( RenderingFn->m_pDeviceRenderer, iCounterID, (DeviceCounterType*)&Type, (Void*)&CountQW )
{
    m_pDeviceAsynchronous = &m_hDeviceCounter;
}
GPUCounter::~GPUCounter()
{
    if ( IsBound() )
        UnBind();
}

Void GPUCounter::CheckDesc( UInt iCounterID, GPUCounterDesc * outCounterDesc )
{
    DeviceCounter::CheckDesc( RenderingFn->m_pDeviceRenderer, iCounterID, (DeviceCounterDesc*)outCounterDesc );
}
