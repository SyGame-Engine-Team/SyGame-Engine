/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/CollisionDetection/BroadPhase/SAP/SweepAndPrune.h
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
// Known Bugs : TODO : Use some threshold to move bounds around just like in
//                     a B*Tree => help rebalance.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_BROADPHASE_SAP_SWEEPANDPRUNE_H
#define SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_BROADPHASE_SAP_SWEEPANDPRUNE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../../Lib/Datastruct/Array/Array.h"
#include "../../../../../Lib/Datastruct/Table/TreeMap.h"

#include "../BroadPhase.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define SAP_DIMENSION 3
#define SAP_INTERVAL_SIZE 32

// Proxy structures
typedef DWord SAPProxyID;
#define SAPPROXY_INVALID_ID 0

typedef struct _sap_proxy
{
    // Node pointer (=key)
    IslandNode * pNode;

    // Indices to min bounds
    UInt iMinIntervals[SAP_DIMENSION];
    UInt iMinBounds[SAP_DIMENSION];

    // Indices to max bounds
    UInt iMaxIntervals[SAP_DIMENSION];
	UInt iMaxBounds[SAP_DIMENSION];
} SAPProxy;

// Bound structures
typedef struct _sap_bound
{
    // Helpers
    inline struct _sap_bound & operator=( const struct _sap_bound & rhs );

    inline Bool IsMin() const;
    inline Bool IsMax() const;

    inline SAPProxyID GetProxyID() const;
    inline Void SetProxyID( SAPProxyID idProxy, Bool bIsMax );

    // Packed data
    DWord dwFlaggedProxyID; // (SAPProxyID), lsb used as min/max flag (alignment => allways 0)
    DWord dwBoundValue;     // Encoded Scalar
} SAPBound;

typedef struct _sap_bound_values
{
    DWord dwMinBounds[SAP_DIMENSION];
	DWord dwMaxBounds[SAP_DIMENSION];
} SAPBoundValues;

// Interval structures
typedef RedBlackTree<SAPProxyID> CheckProxySet;

typedef struct _sap_interval
{
    // Interval bounds
    UInt iBoundCount;
    SAPBound arrBounds[SAP_INTERVAL_SIZE];

    // Interval "checkpoint" proxies, only dimension 0
    // Proxies that cross the last bound of that interval
    inline Void AddCheckProxy( SAPProxyID idProxy );
    inline Void RemoveCheckProxy( SAPProxyID idProxy );

    CheckProxySet setCheckProxies;
    CheckProxySet setRemovedCheckProxies;
} SAPInterval;

/////////////////////////////////////////////////////////////////////////////////
// The SweepAndPrune class
class SweepAndPrune : public BroadPhase
{
public:
    SweepAndPrune( PairListener * pPairListener );
    virtual ~SweepAndPrune();

    // Broadphase operations
    inline virtual Bool HasProxy( IslandNode * pNode ) const;

    virtual Void CreateProxy( IslandNode * pNode, const BVAABox & aabProxy );
    virtual Void DestroyProxy( IslandNode * pNode );

    virtual Void UpdateProxy( IslandNode * pNode, const BVAABox & aabProxy );

private:
    // Helpers
    inline static DWord _EncodeScalar( Scalar fBoundValue );
    inline static Scalar _DecodeScalar( DWord dwBoundValue ); // not needed

    static Void _ExtractBoundValues( SAPBoundValues * outBoundValues, const BVAABox & aabProxy );
    Void _ExtractBoundValues( SAPBoundValues * outBoundValues, const SAPProxy * pProxy ) const;

    static Bool _OverlapTest( const SAPBoundValues & leftAAB, const SAPBoundValues & rightAAB );

    inline static Int _Compare_ProxyIDs( const SAPProxyID & idLeft, const SAPProxyID & idRight, Void * pUserData );
    inline static SAPProxyID _GetProxyID( IslandNode * pNode );

    inline SAPProxy * _GetProxy( SAPProxyID idProxy ) const;
    SAPProxy * _AllocateProxy( IslandNode * pNode );
    Void _FreeProxy( SAPProxyID idProxy );

    static SAPInterval * _AllocateInterval( Bool bCheckProxies );
    static Void _FreeInterval( SAPInterval * pInterval, Bool bCheckProxies );

    // Bound propagation
    Void _SignalPairs( SAPProxy * pCrossedProxy, SAPBound * pCrossedBound,
                       SAPProxy * pMovingProxy, SAPBound * pMovingBound,
                       Bool bLeft, const SAPBoundValues & testValues );
    
    Void _OnSimpleSwap( SAPProxy * pCrossedProxy, SAPBound * pCrossedBound,
                        SAPProxy * pMovingProxy, SAPBound * pMovingBound,
                        UInt iDimension, Bool bLeft, const SAPBoundValues & testValues );
    Void _OnCrossSwap( SAPInterval * pEnterInterval, SAPInterval * pLeaveInterval,
                       SAPProxy * pCrossedProxy, SAPBound * pCrossedBound,
                       SAPProxy * pMovingProxy, SAPBound * pMovingBound,
                       UInt iDimension, Bool bLeft, const SAPBoundValues & testValues );

    Void _PropagateBound( UInt iDimension, UInt iInterval, UInt iBound, SAPProxy * pProxy,
                          Bool bLeft, const SAPBoundValues & testValues, Bool bClipToInterval );

    // Interval operations
    UInt _Interval_Lookup( UInt iDimension, DWord dwBoundValue ) const;

    Void _Interval_UpdateProxyLinks( UInt iDimension, UInt iInterval, Int iIntervalIncrement, Int iBoundIncrement );
    Void _Interval_Split( UInt iDimension, UInt iInterval );
    Void _Interval_Merge( UInt iDimension, UInt iInterval );

    // Proxies
    typedef TreeMap<SAPProxyID, SAPProxy> ProxyMap;
    ProxyMap m_mapProxies;

    // Intervals & &Bounds
    Array<SAPInterval*> m_arrIntervals[SAP_DIMENSION];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "SweepAndPrune.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_BROADPHASE_SAP_SWEEPANDPRUNE_H

