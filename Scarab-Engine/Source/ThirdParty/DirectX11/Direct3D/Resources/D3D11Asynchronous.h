/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/Resources/D3D11Asynchronous.h
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
// Header prelude
#ifndef SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_RESOURCES_D3D11ASYNCHRONOUS_H
#define SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_RESOURCES_D3D11ASYNCHRONOUS_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "D3D11DeferredContext.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Asynchronous types
enum D3D11AsynchronousType {
    D3D11ASYNCHRONOUS_QUERY = 0,
    D3D11ASYNCHRONOUS_PREDICATE,
    D3D11ASYNCHRONOUS_COUNTER
};

// Prototypes
class D3D11Renderer;

/////////////////////////////////////////////////////////////////////////////////
// The D3D11Asynchronous class
class D3D11Asynchronous
{
public:
    D3D11Asynchronous( D3D11Renderer * pRenderer );
    virtual ~D3D11Asynchronous();

    // Deferred construction
    inline Bool IsCreated() const;

    Void Create();
    Void Destroy();

    Void OnDestroyDevice();
    Void OnRestoreDevice();

    // Getters
    virtual D3D11AsynchronousType GetType() const = 0;

    // Methods
    UInt GetDataSize() const;

    virtual Bool Update( D3D11DeferredContext * pContext = NULL, Bool bFlush = true ) = 0;

protected:
    friend class D3D11Renderer;
    D3D11Renderer * m_pRenderer;

    ID3D11Asynchronous * m_pAsynchronous;

    // Auto-Regen system
    virtual Void _NakedCreate() = 0;
    virtual Void _NakedDestroy() = 0;

    Bool m_bTemporaryDestroyed;
};

/////////////////////////////////////////////////////////////////////////////////
// The D3D11Query class
class D3D11Query : public D3D11Asynchronous
{
public:
    D3D11Query( D3D11Renderer * pRenderer );
    virtual ~D3D11Query();

    // Getters
    inline virtual D3D11AsynchronousType GetType() const;

    inline D3D11QueryType GetQueryType() const;

    // Methods
    virtual Bool Update( D3D11DeferredContext * pContext = NULL, Bool bFlush = true ) = 0;

protected:
    friend class D3D11Renderer;

    __D3D11_QUERY_DESC m_hQueryDesc;
    ID3D11Query * m_pQuery;

    // Auto-Regen system
    virtual Void _NakedCreate();
    virtual Void _NakedDestroy();
};

/////////////////////////////////////////////////////////////////////////////////
// The D3D11QueryCommandProcessing class
class D3D11QueryCommandProcessing : public D3D11Query
{
public:
    D3D11QueryCommandProcessing( D3D11Renderer * pRenderer, Bool * pFinished );
    virtual ~D3D11QueryCommandProcessing();

    // Methods
    virtual Bool Update( D3D11DeferredContext * pContext = NULL, Bool bFlush = true );

protected:
    Bool * m_pFinished;
};

/////////////////////////////////////////////////////////////////////////////////
// The D3D11QueryOcclusion class
class D3D11QueryOcclusion : public D3D11Query
{
public:
    D3D11QueryOcclusion( D3D11Renderer * pRenderer, UInt64 * pVisibleSampleCount );
    virtual ~D3D11QueryOcclusion();

    // Methods
    virtual Bool Update( D3D11DeferredContext * pContext = NULL, Bool bFlush = true );

protected:
    UInt64 * m_pVisibleSampleCount;
};

/////////////////////////////////////////////////////////////////////////////////
// The D3D11QueryTimeStampFrequency class
class D3D11QueryTimeStampFrequency : public D3D11Query
{
public:
    D3D11QueryTimeStampFrequency( D3D11Renderer * pRenderer, UInt64 * pFrequency, Bool * pReliableTimeStamps );
    virtual ~D3D11QueryTimeStampFrequency();

    // Methods
    virtual Bool Update( D3D11DeferredContext * pContext = NULL, Bool bFlush = true );

protected:
    UInt64 * m_pFrequency;
    Bool * m_pReliableTimeStamps;
};

/////////////////////////////////////////////////////////////////////////////////
// The D3D11QueryTimeStamp class
class D3D11QueryTimeStamp : public D3D11Query
{
public:
    D3D11QueryTimeStamp( D3D11Renderer * pRenderer, UInt64 * pTimeStamp );
    virtual ~D3D11QueryTimeStamp();

    // Methods
    virtual Bool Update( D3D11DeferredContext * pContext = NULL, Bool bFlush = true );

protected:
    UInt64 * m_pTimeStamp;
};

/////////////////////////////////////////////////////////////////////////////////
// The D3D11QueryStatsPipeline class
class D3D11QueryStatsPipeline : public D3D11Query
{
public:
    D3D11QueryStatsPipeline( D3D11Renderer * pRenderer, UInt64 * pIAVertexCount, UInt64 * pIAPrimitiveCount, UInt64 * pVSInvocationCount, UInt64 * pGSInvocationCount, UInt64 * pGSPrimitiveCount,
                             UInt64 * pPSInvocationCount, UInt64 * pHSInvocationCount, UInt64 * pDSInvocationCount, UInt64 * pCSInvocationCount, UInt64 * pRasterizedPrimitiveCount, UInt64 * pRenderedPrimitiveCount );
    virtual ~D3D11QueryStatsPipeline();

    // Methods
    virtual Bool Update( D3D11DeferredContext * pContext = NULL, Bool bFlush = true );

protected:
    UInt64 * m_pIAVertexCount;
    UInt64 * m_pIAPrimitiveCount;
    UInt64 * m_pVSInvocationCount;
    UInt64 * m_pGSInvocationCount;
    UInt64 * m_pGSPrimitiveCount;
    UInt64 * m_pPSInvocationCount;
    UInt64 * m_pHSInvocationCount;
    UInt64 * m_pDSInvocationCount;
    UInt64 * m_pCSInvocationCount;
    UInt64 * m_pRasterizedPrimitiveCount;
    UInt64 * m_pRenderedPrimitiveCount; // Can be <= or > to raster count since clipped primitives can be culled or splitted.
};

/////////////////////////////////////////////////////////////////////////////////
// The D3D11QueryStatsStreamOutput class
class D3D11QueryStatsStreamOutput : public D3D11Query
{
public:
    D3D11QueryStatsStreamOutput( D3D11Renderer * pRenderer, UInt64 * pRealPrimitiveCount, UInt64 * pWrittenPrimitiveCount, UInt iStream = INVALID_OFFSET );
    virtual ~D3D11QueryStatsStreamOutput();

    // Methods
    virtual Bool Update( D3D11DeferredContext * pContext = NULL, Bool bFlush = true );

protected:
    UInt64 * m_pRealPrimitiveCount;
    UInt64 * m_pWrittenPrimitiveCount; // < RealPrimitiveCount => Overflow
};

/////////////////////////////////////////////////////////////////////////////////
// The D3D11Predicate class
class D3D11Predicate : public D3D11Asynchronous
{
public:
    D3D11Predicate( D3D11Renderer * pRenderer );
    virtual ~D3D11Predicate();

    // Getters
    inline virtual D3D11AsynchronousType GetType() const;

    inline D3D11PredicateType GetPredicateType() const;

    // Methods
    virtual Bool Update( D3D11DeferredContext * pContext = NULL, Bool bFlush = true ) = 0;

protected:
    friend class D3D11Renderer;

    __D3D11_QUERY_DESC m_hPredicateDesc;
    ID3D11Predicate * m_pPredicate;
    Bool * m_pPredicateValue;

    // Auto-Regen system
    virtual Void _NakedCreate();
    virtual Void _NakedDestroy();
};

/////////////////////////////////////////////////////////////////////////////////
// The D3D11PredicateOcclusion class
class D3D11PredicateOcclusion : public D3D11Predicate
{
public:
    D3D11PredicateOcclusion( D3D11Renderer * pRenderer, Bool * pVisible, Bool bDrawWhenUnsure = false );
    virtual ~D3D11PredicateOcclusion();

    // Getters
    Bool DrawWhenUnsure() const;

    // Methods
    virtual Bool Update( D3D11DeferredContext * pContext = NULL, Bool bFlush = true );
};

/////////////////////////////////////////////////////////////////////////////////
// The D3D11PredicateStreamOutputOverflow class
class D3D11PredicateStreamOutputOverflow : public D3D11Predicate
{
public:
    D3D11PredicateStreamOutputOverflow( D3D11Renderer * pRenderer, Bool * pOverflow, UInt iStream = INVALID_OFFSET );
    virtual ~D3D11PredicateStreamOutputOverflow();

    // Methods
    virtual Bool Update( D3D11DeferredContext * pContext = NULL, Bool bFlush = true );
};

/////////////////////////////////////////////////////////////////////////////////
// The D3D11Counter class
class D3D11Counter : public D3D11Asynchronous
{
public:
    D3D11Counter( D3D11Renderer * pRenderer, UInt iCounterID, D3D11CounterType * pType, Void * pCount );
    virtual ~D3D11Counter();

    // Getters
    inline virtual D3D11AsynchronousType GetType() const;

    // Methods
    static Void CheckDesc( D3D11Renderer * pRenderer, UInt iCounterID, D3D11CounterDesc * outCounterDesc );

    virtual Bool Update( D3D11DeferredContext * pContext = NULL, Bool bFlush = true );

protected:
    friend class D3D11Renderer;

    __D3D11_COUNTER_DESC m_hCounterDesc;
    ID3D11Counter * m_pCounter;

    D3D11CounterType * m_pType;
    Void * m_pCount;

    // Auto-Regen system
    virtual Void _NakedCreate();
    virtual Void _NakedDestroy();
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "D3D11Asynchronous.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_RESOURCES_D3D11ASYNCHRONOUS_H
