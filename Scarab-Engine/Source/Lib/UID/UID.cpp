/////////////////////////////////////////////////////////////////////////////////
// File : Lib/UID/UID.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Universally Unique IDentifiers support.
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
#include "UID.h"

// Namespaces
    // DNS namespace (fully-qualified domain name)
const UUID NameSpaceDNS = {
   0x6ba7b810, 0x9dad, 0x11d1,
   0x80, 0xb4,
   0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8
};
    // URL namespace (Uniform Resource Locator)
const UUID NameSpaceURL = {
   0x6ba7b811, 0x9dad, 0x11d1,
   0x80, 0xb4,
   0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8
};
    // OID namespace (ISO Object IDentifier)
const UUID NameSpaceOID = {
   0x6ba7b812, 0x9dad, 0x11d1,
   0x80, 0xb4,
   0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8
};

/////////////////////////////////////////////////////////////////////////////////
// UIDGenerator implementation
UIDGenerator::UIDGenerator():
    m_hGlobalLock( SystemFn->CreateMutex() )
{
    MemZero( &m_hUUIDState, sizeof(_UUIDState) );
    m_bStateInitialized = false;
    m_qwStateNextSave = 0;

    MemZero( m_iHashedMACAddress, sizeof(HashSHA1) );
    m_bHasMACAddress = false;
}
UIDGenerator::~UIDGenerator()
{
    m_hGlobalLock.Close();
}

Void UIDGenerator::GenUUID( UUID * outID )
{
    _UUIDState lastState;
    _UUIDState curState;

    // Lock
    WaitResult hErr = SystemFn->WaitOne( &m_hGlobalLock );
    Assert( hErr == WAIT_OK );

    // Process-safe code
    curState.iTimeStamp = _GetTimeStamp();
    _GetNode_IEEE( curState.iNode );

    Bool bFilePresent = _StateLoad( &lastState );
    if ( !bFilePresent || MemCmp(lastState.iNode, curState.iNode, 6) != 0 ) {
        Byte iTrueSeed[20];
        DWord iTrueRandom256[8];
        _TrueSeed( iTrueSeed );
        _TrueRandom( iTrueRandom256, iTrueSeed );
        curState.iClockSequence = (Word)( (iTrueRandom256[0] >> 16) & 0xffff );
        if ( !bFilePresent )
            m_bStateInitialized = true;
    } else {
        curState.iClockSequence = lastState.iClockSequence;
        if ( curState.iTimeStamp < lastState.iTimeStamp )
            ++(curState.iClockSequence);
    }
    _StateSave( &curState );

    // Unlock
    m_hGlobalLock.Release();

    _FormatUUID_v1( outID, &curState );
}
Void UIDGenerator::GenNameUUID( UUID * outID, const UUID & uuidNameSpace, const GChar * strName, UUIDNameHasher iHasher ) const
{
    UInt iNameLength = StringFn->Length( strName );

    // Switch to network byte order for hashing
    UUID uuidNetName;
    MemCopy( &uuidNetName, &uuidNameSpace, sizeof(UUID) );
    uuidNetName.iTimeLow = SystemFn->NetByteOrderDW( uuidNetName.iTimeLow );
    uuidNetName.iTimeMid = SystemFn->NetByteOrderW( uuidNetName.iTimeMid );
    uuidNetName.iTimeHigh = SystemFn->NetByteOrderW( uuidNetName.iTimeHigh );

    if ( iHasher == UUIDNAME_SHA1 ) {
        HashSHA1 hashValue;
        SHA1Hasher hSHA1;
        hSHA1.Update( &uuidNetName, sizeof(UUID) );
        hSHA1.Update( strName, iNameLength * sizeof(GChar) );
        hSHA1.GetHash( hashValue );
        _FormatUUID_v3_v5( outID, hashValue, 5 );
    } else if ( iHasher == UUIDNAME_MD5 ) {
        HashMD5 hashValue;
        MD5Hasher hMD5;
        hMD5.Update( &uuidNetName, sizeof(UUID) );
        hMD5.Update( strName, iNameLength * sizeof(GChar) );
        hMD5.GetHash( hashValue );
        _FormatUUID_v3_v5( outID, hashValue, 3 );
    }
}

/////////////////////////////////////////////////////////////////////////////////

QWord UIDGenerator::_GetTimeStamp() const
{
    static const UInt s_iUUIDsByTick = 10000000;
    static UInt s_iUUIDsThisTick;
    static QWord s_iLastTime = 0;
    if ( s_iLastTime == 0 ) {
        s_iLastTime = _Time_UUID();
        s_iUUIDsThisTick = s_iUUIDsByTick;
    }
    QWord iTime;
    while( true ) {
        iTime = _Time_UUID();
        if ( iTime != s_iLastTime ) {
            s_iLastTime = iTime;
            s_iUUIDsThisTick = 0;
            break;
        }
        if ( s_iUUIDsThisTick < s_iUUIDsByTick ) {
            ++s_iUUIDsThisTick;
            break;
        }
    }
    return ( iTime + (QWord)s_iUUIDsThisTick );
}

Void UIDGenerator::_TrueSeed( Byte outTrueSeed[20] ) const
{
    // Use some source of true randomness ...
    // processes memory
    // heap/stack alloc addresses
    // ticks since startup
    // hardware IDs
    // hard disk speed measure
    // physical measure (thermal noise, radioactiv decay)
    // Sound/Video analogic noise
    // etc ... (any idea ? see RFC 1750)

    // Static seeds computed from atmospheric noise (www.random.org)
    static Byte s_arrTrueSeeds[16][20] = {
        { 118, 171, 122, 187,  64, 195, 187, 223,  85,  32,  82,  18, 155, 229,  55,  30, 220,  40, 209, 129 },
        { 135,  16,  91, 247, 193, 152, 106,   6,   4, 101, 162, 193,  42,   8, 207,  87,  51, 176, 163, 105 },
        { 175, 144,  71, 234,  55,  39,  31,  57,  95,  90,  54,  67, 192,  29, 235, 189, 145, 187,  76, 107 },
        { 146,  80, 146, 117, 166, 240,  26, 236,  59,  71, 230,  94, 160, 164,  12,  10, 134,   1, 150, 211 },
        { 247,  35,   6, 253,  86,  75, 234, 125,   9, 219, 217, 179, 117,  67,  74,  13,  18,  26, 224, 148 },
        { 221,  71, 166,  25, 159, 230, 211,   3, 236, 227,  56,  83, 235,  49,  66, 192, 116, 236,  54, 186 },
        { 200,  59,  68, 247,  49, 123, 132, 162, 244,  89, 142,  62,  13, 226, 216, 121, 106, 197,  80,   7 },
        { 239, 142,  43,  95,  32, 151, 150, 205, 208, 144, 203,  64, 194, 237, 156, 181, 219,  53, 143, 164 },
        { 239, 245,  83,  45, 107, 251, 147,  98,  93, 123,  46, 118, 172, 104, 227,  25, 103, 129, 137,  37 },
        { 250,  41, 172, 110,  12, 102, 158, 111, 163,  35,  49, 157,  93, 167,  97, 232, 150, 194,  75, 141 },
        {  29,   0, 117, 129, 238, 164, 255, 162,  78, 207,  95, 119, 190, 195, 112, 176, 127,  35, 143,  23 },
        {  18,  76, 209,  56, 184, 222, 179, 102,  38,  57,  79, 101,  89,  96,  91,  14, 206, 142, 103, 183 },
        { 246,  54, 207, 200,  18,  87, 132, 114, 210,  20, 191,  38,   6, 210,  86, 205, 169,  96,  46, 247 },
        { 104, 104, 161, 201,  34, 146,  46, 172, 147, 207,  45, 175,  36,  78,  55, 121, 186,  81,  89,  83 },
        { 181, 170, 151,   9,  26,  38,  74, 117,  97, 230,  80, 100,  85,  40, 250, 171,  77,  83,  56,  90 },
        {  93, 211, 149, 152,  84, 185,  14, 183, 146, 111,  69, 160,  36, 115, 153,  70,   3,   3, 191,  18 }
    };
    ULong iIndex = RandFn->CompMWC();
    MemCopy( outTrueSeed, s_arrTrueSeeds[iIndex & 0x0f], 20 );
}
Void UIDGenerator::_TrueRandom( DWord outTrueRandom256[8], const Byte iTrueSeed[20] ) const
{
    // Use Mersenne-Twist to combine random seeds
    // TableSize = 8, RecOffset = 5
    static const ULong iMatrixA[2] = { 0x00000000, 0x9908b0df };
    ULong iScratch1, iScratch2, iX, iY;

    // Init
    const DWord * dwSeed = (const DWord *)iTrueSeed;
    outTrueRandom256[0] = dwSeed[0];
    outTrueRandom256[1] = dwSeed[1];
    outTrueRandom256[2] = dwSeed[2];
    outTrueRandom256[3] = RandFn->XorShiftWeyl();
    outTrueRandom256[4] = dwSeed[3];
    outTrueRandom256[5] = RandFn->CompMWC();
    outTrueRandom256[6] = dwSeed[4];
    outTrueRandom256[7] = RandFn->MotherOfAll32();
    DWord * pState = outTrueRandom256 + 7;
    iScratch1 = *pState;

    // Pass 1
    pState -= 2;
    iScratch2 = pState[1];
    iScratch1 = (iScratch1 & 0x80000000) | (iScratch2 & 0x7fffffff);
    iX = pState[2 - 5]; iY = iScratch1;
    pState[2] = ( iX ^ (iY >> 1) ^ iMatrixA[iY & 1] );
    iScratch1 = pState[0];
    iScratch2 = (iScratch2 & 0x80000000) | (iScratch1 & 0x7fffffff);
    iX = pState[1 - 5]; iY = iScratch2;
    pState[1] = ( iX ^ (iY >> 1) ^ iMatrixA[iY & 1] );

    // Interlude
    iScratch2 = *(--pState);
    iScratch1 = (iScratch1 & 0x80000000) | (iScratch2 & 0x7fffffff);
    iX = pState[1 - 5]; iY = iScratch1;
    pState[1] = ( iX ^ (iY >> 1) ^ iMatrixA[iY & 1] );

    // Pass 2
    for( UInt i = 0; i < 2; ++i ) {
        pState -= 2;
        iScratch1 = pState[1];
        iScratch2 = (iScratch2 & 0x80000000) | (iScratch1 & 0x7fffffff);
        iX = pState[8 + 2 - 5]; iY = iScratch2;
        pState[2] = ( iX ^ (iY >> 1) ^ iMatrixA[iY & 1] );
        iScratch2 = pState[0];
        iScratch1 = (iScratch1 & 0x80000000) | (iScratch2 & 0x7fffffff);
        iX = pState[8 + 1 - 5]; iY = iScratch1;
        pState[1] = ( iX ^ (iY >> 1) ^ iMatrixA[iY & 1] );
    }
    iScratch1 = (iScratch2 & 0x80000000) | (outTrueRandom256[7] & 0x7fffffff);
    iX = pState[8 - 5]; iY = iScratch1;
    *pState = ( iX ^ (iY >> 1) ^ iMatrixA[iY & 1] );
}

Void UIDGenerator::_GetNode_IEEE( Byte outNode[6], Bool bRandomize )
{
    if ( !m_bHasMACAddress ) {
        // Get MAC from adapter
        Byte arrMACBuffer[8];
        SystemFn->GetMACAddress( arrMACBuffer );
        // Hash with SHA1, we don't want to expose
        SHA1Hasher hSHA1;
        hSHA1.Update( arrMACBuffer, 6 );
        hSHA1.GetHash( m_iHashedMACAddress );
        hSHA1.Reset();
        m_bHasMACAddress = true;
    }
    if ( bRandomize ) {
        DWord trueRandom[8];
        _TrueRandom( trueRandom, m_iHashedMACAddress );
        // Pick 6 first byte values
        MemCopy( outNode, trueRandom, 6 );
    } else {
        // Pick arbitrary offsets, here 6 first bytes
        MemCopy( outNode, m_iHashedMACAddress, 6 );
    }
}

Bool UIDGenerator::_StateLoad( _UUIDState * outState )
{
    if ( !m_bStateInitialized ) {
        HFile hFile = SystemFn->OpenFile( UUID_STATEFILE, FILE_READ );
        if ( !hFile.IsValid() )
            return false; // File not present
        Bool bOk = hFile.ReadBytes( (Byte*)&m_hUUIDState, sizeof(_UUIDState) );
        Assert( bOk );
        hFile.Close();
        m_bStateInitialized = true;
    }

    outState->iTimeStamp = m_hUUIDState.iTimeStamp;
    outState->iClockSequence = m_hUUIDState.iClockSequence;
    MemCopy( outState->iNode, m_hUUIDState.iNode, 6 );
    return true;
}
Void UIDGenerator::_StateSave( const _UUIDState * pState )
{
    if ( m_qwStateNextSave == 0 )
        m_qwStateNextSave = pState->iTimeStamp;
    m_hUUIDState.iTimeStamp = pState->iTimeStamp;
    m_hUUIDState.iClockSequence = pState->iClockSequence;
    MemCopy( m_hUUIDState.iNode, pState->iNode, 6 );

    if ( pState->iTimeStamp >= m_qwStateNextSave ) {
        HFile hFile = SystemFn->OpenFile( UUID_STATEFILE, FILE_WRITE );
        Assert( hFile.IsValid() );
        Bool bOk = hFile.WriteBytes( (const Byte*)&m_hUUIDState, sizeof(_UUIDState) );
        Assert( bOk );
        hFile.Close();
        m_qwStateNextSave = pState->iTimeStamp + 100000000; // 10s
    }
}

Void UIDGenerator::_FormatUUID_v1( UUID * outID, const _UUIDState * pState ) const
{
    outID->iTimeLow = (DWord)( pState->iTimeStamp & 0xffffffff );
    outID->iTimeMid = (Word)( (pState->iTimeStamp >> 32) & 0xffff );
    outID->iTimeHigh = (Word)( (pState->iTimeStamp >> 48) & 0x0fff );
    outID->iTimeHigh |= (1 << 12); // version
    outID->iClockSequenceLow = ( pState->iClockSequence & 0xff );
    outID->iClockSequenceHigh = ( (pState->iClockSequence >> 8) & 0x3f );
    outID->iClockSequenceHigh |= 0x80; // variant
    MemCopy( outID->iNode, pState->iNode, 6 );
}
Void UIDGenerator::_FormatUUID_v3_v5( UUID * outID, const Byte hashValue[16], UInt iVersion ) const
{
    MemCopy( outID, hashValue, sizeof(UUID) );

    // Switch back to host byte order after hashing
    outID->iTimeLow = SystemFn->HostByteOrderDW(outID->iTimeLow);
    outID->iTimeMid = SystemFn->HostByteOrderW(outID->iTimeMid);
    outID->iTimeHigh = SystemFn->HostByteOrderW(outID->iTimeHigh);

    outID->iTimeHigh &= 0x0fff;
    outID->iTimeHigh |= (iVersion << 12); // version
    outID->iClockSequenceHigh &= 0x3f;
    outID->iClockSequenceHigh |= 0x80; // variant
}


