/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Resources/GPUAsynchronous.h
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
// Header prelude
#ifndef SCARAB_ENGINE_RENDERING_RESOURCES_GPUASYNCHRONOUS_H
#define SCARAB_ENGINE_RENDERING_RESOURCES_GPUASYNCHRONOUS_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "GPUDeferredContext.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// GPUAsynchronous types
enum GPUAsynchronousType {
    GPUASYNCHRONOUS_QUERY     = DEVICEASYNCHRONOUS_QUERY,
    GPUASYNCHRONOUS_PREDICATE = DEVICEASYNCHRONOUS_PREDICATE,
    GPUASYNCHRONOUS_COUNTER   = DEVICEASYNCHRONOUS_COUNTER
};

// GPUQuery types
enum GPUQueryType {
    GPUQUERY_EVENT                = DEVICEQUERY_EVENT,
    GPUQUERY_OCCLUSION            = DEVICEQUERY_OCCLUSION,
    GPUQUERY_TIMESTAMP_FREQUENCY  = DEVICEQUERY_TIMESTAMP_FREQUENCY,
    GPUQUERY_TIMESTAMP            = DEVICEQUERY_TIMESTAMP,
    GPUQUERY_STATS_PIPELINE       = DEVICEQUERY_STATS_PIPELINE,
    GPUQUERY_STATS_STREAMOUTPUT   = DEVICEQUERY_STATS_STREAMOUTPUT,
    GPUQUERY_STATS_STREAMOUTPUT_0 = DEVICEQUERY_STATS_STREAMOUTPUT_0,
    GPUQUERY_STATS_STREAMOUTPUT_1 = DEVICEQUERY_STATS_STREAMOUTPUT_1,
    GPUQUERY_STATS_STREAMOUTPUT_2 = DEVICEQUERY_STATS_STREAMOUTPUT_2,
    GPUQUERY_STATS_STREAMOUTPUT_3 = DEVICEQUERY_STATS_STREAMOUTPUT_3
};

// GPUPredicate types
enum GPUPredicateType {
    GPUPREDICATE_OCCLUSION               = DEVICEPREDICATE_OCCLUSION,
    GPUPREDICATE_OVERFLOW_STREAMOUTPUT   = DEVICEPREDICATE_OVERFLOW_STREAMOUTPUT,
    GPUPREDICATE_OVERFLOW_STREAMOUTPUT_0 = DEVICEPREDICATE_OVERFLOW_STREAMOUTPUT_0,
    GPUPREDICATE_OVERFLOW_STREAMOUTPUT_1 = DEVICEPREDICATE_OVERFLOW_STREAMOUTPUT_1,
    GPUPREDICATE_OVERFLOW_STREAMOUTPUT_2 = DEVICEPREDICATE_OVERFLOW_STREAMOUTPUT_2,
    GPUPREDICATE_OVERFLOW_STREAMOUTPUT_3 = DEVICEPREDICATE_OVERFLOW_STREAMOUTPUT_3
};

// GPUCounter types
enum GPUCounterType {
    GPUCOUNTER_FLOAT = DEVICECOUNTER_FLOAT,
    GPUCOUNTER_WORD  = DEVICECOUNTER_WORD,
    GPUCOUNTER_DWORD = DEVICECOUNTER_DWORD,
    GPUCOUNTER_QWORD = DEVICECOUNTER_QWORD
};

typedef struct _gpu_counter_desc {
    UInt iCounterID;
    UInt iFlags; // Reserved : 0

    GPUCounterType iType;
    UInt iActiveHardwareCounters;

    UInt iCounterNameLength;
    AChar strCounterName[64];

    UInt iUnitNameLength;
    AChar strUnitName[64];

    UInt iDescriptionLength;
    AChar strDescription[128];
} GPUCounterDesc;

// Prototypes
class RenderingManager;

/////////////////////////////////////////////////////////////////////////////////
// The GPUAsynchronous class
class GPUAsynchronous
{
protected:
    GPUAsynchronous();
public:
    virtual ~GPUAsynchronous();

    // Binding
    inline Bool IsBound() const;

    Void Bind();
    Void UnBind();

    // Getters
    inline GPUAsynchronousType GetType() const;

    // Methods
    inline UInt GetDataSize() const;

    Bool Update( GPUDeferredContext * pContext = NULL, Bool bFlush = true );

protected:
    friend class RenderingManager;

    // Device data
    DeviceAsynchronous * m_pDeviceAsynchronous;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPUQuery class
class GPUQuery : public GPUAsynchronous
{
protected:
    GPUQuery();
public:
    virtual ~GPUQuery();

    // Getters
    inline GPUQueryType GetQueryType() const;

protected:
    friend class RenderingManager;

    // Device data
    DeviceQuery * m_pDeviceQuery;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPUQueryCommandProcessing class
class GPUQueryCommandProcessing : public GPUQuery
{
public:
    GPUQueryCommandProcessing();
    virtual ~GPUQueryCommandProcessing();

    Bool Finished;

protected:
    friend class RenderingManager;

    // Device data
    DeviceQueryCommandProcessing m_hDeviceQueryCommandProcessing;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPUQueryOcclusion class
class GPUQueryOcclusion : public GPUQuery
{
public:
    GPUQueryOcclusion();
    virtual ~GPUQueryOcclusion();

    UInt64 VisibleSampleCount;

protected:
    friend class RenderingManager;

    // Device data
    DeviceQueryOcclusion m_hDeviceQueryOcclusion;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPUQueryTimeStampFrequency class
class GPUQueryTimeStampFrequency : public GPUQuery
{
public:
    GPUQueryTimeStampFrequency();
    virtual ~GPUQueryTimeStampFrequency();

    UInt64 Frequency;
    Bool ReliableTimeStamps;

protected:
    friend class RenderingManager;

    // Device data
    DeviceQueryTimeStampFrequency m_hDeviceQueryTimeStampFrequency;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPUQueryTimeStamp class
class GPUQueryTimeStamp : public GPUQuery
{
public:
    GPUQueryTimeStamp();
    virtual ~GPUQueryTimeStamp();

    UInt64 TimeStamp;

protected:
    friend class RenderingManager;

    // Device data
    DeviceQueryTimeStamp m_hDeviceQueryTimeStamp;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPUQueryStatsPipeline class
class GPUQueryStatsPipeline : public GPUQuery
{
public:
    GPUQueryStatsPipeline();
    virtual ~GPUQueryStatsPipeline();

    UInt64 IAVertexCount;
    UInt64 IAPrimitiveCount;
    UInt64 VSInvocationCount;
    UInt64 GSInvocationCount;
    UInt64 GSPrimitiveCount;
    UInt64 PSInvocationCount;
    UInt64 HSInvocationCount;
    UInt64 DSInvocationCount;
    UInt64 CSInvocationCount;
    UInt64 RasterizedPrimitiveCount;
    UInt64 RenderedPrimitiveCount; // Can be <= or > to raster count since clipped primitives can be culled/splitted.

protected:
    friend class RenderingManager;

    // Device data
    DeviceQueryStatsPipeline m_hDeviceQueryStatsPipeline;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPUQueryStatsStreamOutput class
class GPUQueryStatsStreamOutput : public GPUQuery
{
public:
    GPUQueryStatsStreamOutput( UInt iStream = INVALID_OFFSET );
    virtual ~GPUQueryStatsStreamOutput();

    UInt64 RealPrimitiveCount;
    UInt64 WrittenPrimitiveCount; // < RealPrimitiveCount => Overflow

protected:
    friend class RenderingManager;

    // Device data
    DeviceQueryStatsStreamOutput m_hDeviceQueryStatsStreamOutput;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPUPredicate class
class GPUPredicate : public GPUAsynchronous
{
protected:
    GPUPredicate();
public:
    virtual ~GPUPredicate();

    // Getters
    inline GPUPredicateType GetPredicateType() const;

protected:
    friend class RenderingManager;

    // Device data
    DevicePredicate * m_pDevicePredicate;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPUPredicateOcclusion class
class GPUPredicateOcclusion : public GPUPredicate
{
public:
    GPUPredicateOcclusion( Bool bDrawWhenUnsure = false );
    virtual ~GPUPredicateOcclusion();

    // Getters
    inline Bool DrawWhenUnsure() const;

    Bool Visible;

protected:
    friend class RenderingManager;

    // Device data
    DevicePredicateOcclusion m_hDevicePredicateOcclusion;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPUPredicateStreamOutputOverflow class
class GPUPredicateStreamOutputOverflow : public GPUPredicate
{
public:
    GPUPredicateStreamOutputOverflow( UInt iStream = INVALID_OFFSET );
    virtual ~GPUPredicateStreamOutputOverflow();

    Bool Overflow;

protected:
    friend class RenderingManager;

    // Device data
    DevicePredicateStreamOutputOverflow m_hDevicePredicateStreamOutputOverflow;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPUCounter class
class GPUCounter : public GPUAsynchronous
{
public:
    GPUCounter( UInt iCounterID );
    virtual ~GPUCounter();

    // Methods
    static Void CheckDesc( UInt iCounterID, GPUCounterDesc * outCounterDesc );

    GPUCounterType Type;
    union {
        Float CountF;
        Word  CountW;
        DWord CountDW;
        QWord CountQW;
    };

protected:
    friend class RenderingManager;

    // Device data
    DeviceCounter m_hDeviceCounter;
};


/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GPUAsynchronous.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RENDERING_RESOURCES_GPUASYNCHRONOUS_H
