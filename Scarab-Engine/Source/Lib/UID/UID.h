/////////////////////////////////////////////////////////////////////////////////
// File : Lib/UID/UID.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Universally Unique IDentifiers support. (RFC 4122, 1750)
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
#ifndef SCARAB_LIB_UID_UID_H
#define SCARAB_LIB_UID_UID_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../ThirdParty/System/System.h"

#include "../Error/ErrorManager.h"

#include "../Math/Hash/MD5.h"
#include "../Math/Hash/SHA1.h"
#include "../Math/Random/Random.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define UniqueIdFn UIDGenerator::GetInstancePtr()

typedef struct _uuid
{
    DWord iTimeLow;
    Word iTimeMid; 
    Word iTimeHigh;  // + version
    Byte iClockSequenceHigh; // + variant
    Byte iClockSequenceLow;
    Byte iNode[6];

    inline Bool operator==(const struct _uuid & rhs) const;
    inline Bool operator!=(const struct _uuid & rhs) const;
} UUID;

// UUID state file
#define UUID_STATEFILE TEXT("Data/UUIDState.bin")

// Named UUIDs
enum UUIDNameHasher
{
    UUIDNAME_SHA1 = 0,
    UUIDNAME_MD5
};

// Namespaces
    // DNS namespace (fully-qualified domain name)
extern const UUID NameSpaceDNS; // 6ba7b810-9dad-11d1-80b4-00c04fd430c8
    // URL namespace (Uniform Resource Locator)
extern const UUID NameSpaceURL; // 6ba7b811-9dad-11d1-80b4-00c04fd430c8
    // OID namespace (ISO Object IDentifier)
extern const UUID NameSpaceOID; // 6ba7b812-9dad-11d1-80b4-00c04fd430c8


/////////////////////////////////////////////////////////////////////////////////
// The UIDGenerator class
class UIDGenerator
{
    // Discrete singleton interface
public:
    inline static UIDGenerator * GetInstancePtr();

private:
    UIDGenerator();
    ~UIDGenerator();

public:
    // UUID algorithm
    Void GenUUID( UUID * outID );
    Void GenNameUUID( UUID * outID, const UUID & uuidNameSpace, const GChar * strName, UUIDNameHasher iHasher = UUIDNAME_SHA1 ) const;

private:
    // Time stamp support
    inline UInt64 _Time_UUID() const;
    QWord _GetTimeStamp() const;

    // True random support
    Void _TrueSeed( Byte outTrueSeed[20] ) const;
    Void _TrueRandom( DWord outTrueRandom256[8], const Byte iTrueSeed[20] ) const;

    // Spatial node support
    Void _GetNode_IEEE( Byte outNode[6], Bool bRandomize = false );
    //Void _GetNode_TrueRandom( Byte outNode[6] ) const;

    HashSHA1 m_iHashedMACAddress;
    Bool m_bHasMACAddress;

    // Persistent state support
    typedef struct _uuid_state
    {
        Byte iNode[6];
        QWord iTimeStamp;
        Word iClockSequence;
    } _UUIDState;
    
    Bool _StateLoad( _UUIDState * outState );
    Void _StateSave( const _UUIDState * pState );

    _UUIDState m_hUUIDState;
    Bool m_bStateInitialized;
    QWord m_qwStateNextSave;
    HMutex m_hGlobalLock;

    // Output helpers
    Void _FormatUUID_v1( UUID * outID, const _UUIDState * pState ) const;
    Void _FormatUUID_v3_v5( UUID * outID, const Byte hashValue[16], UInt iVersion ) const;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "UID.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_UID_UID_H

