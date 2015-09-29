/////////////////////////////////////////////////////////////////////////////////
// File : Lib/UID/UID.inl
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

inline Bool UUID::operator==(const UUID & rhs) const {
    if (this == &rhs)
        return true;
    return ( MemCmp( this, &rhs, sizeof(UUID) ) == 0 );
}
inline Bool UUID::operator!=(const UUID & rhs) const {
    if (this == &rhs)
        return false;
    return ( MemCmp( this, &rhs, sizeof(UUID) ) != 0 );
}

/////////////////////////////////////////////////////////////////////////////////
// UIDGenerator implementation
inline UIDGenerator * UIDGenerator::GetInstancePtr() {
    static UIDGenerator s_Instance;
    return &s_Instance;
}

/////////////////////////////////////////////////////////////////////////////////

inline UInt64 UIDGenerator::_Time_UUID() const {
    // UUID Epoch = October 15, 1582 (Gregorian calendar reform)
    // UTC Epoch = January, 1st 1970
    // Difference is :
    // 17 Days in Oct + 30 (Nov) + 31 (Dec) + 17 years + 4 leap days (=> Jan,1 1600)
    // + 100 years + 25 leap days (1600 => 1700) + 100 years + 24 leap days (1700 => 1800)
    // + 100 years + 24 leap days (1800 => 1900) +  70 years + 17 leap days (1900 => 1970)
    // => Total = 387 years, 94 leap days
    // Finally : 141427 days = 12 219 292 800 seconds !
    // Return time has 100ns ticks units
    TimeValue iTime = SystemFn->GetUTCTime( TIMEUNIT_SECONDS );
    return ( (iTime + 12219292800ui64) * 10000000ui64 );
}
