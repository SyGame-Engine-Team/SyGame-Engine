/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Random/Random.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Random generator implementation.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// RandomGenerator implementation
inline RandomGenerator * RandomGenerator::GetInstancePtr() {
    static RandomGenerator s_Instance;
    return &s_Instance;
}

inline ULong RandomGenerator::LecuyerMax() const {
    return (unsigned)(sm_iLecuyerM1 - 2);
}

inline ULong RandomGenerator::KnuthMax() const {
    return (unsigned)(sm_iKnuthBig - 1);
}

inline UInt64 RandomGenerator::RanRotMax() const {
    return UINT64_MAX;
}

inline ULong RandomGenerator::MersenneTwisterMax() const {
    return ULONG_MAX;
}

inline ULong RandomGenerator::MotherOfAllMax() const {
    return ULONG_MAX;
}

inline ULong RandomGenerator::CompMWCMax() const {
    return ULONG_MAX;
}

inline ULong RandomGenerator::XorShiftMax() const {
    return ULONG_MAX;
}

inline Void RandomGenerator::SeedAll( ULong iSeed, UInt iWarmUp ) {
    LecuyerSeed(iSeed, iWarmUp);
    KnuthSeed(iSeed, iWarmUp);
    RanRotSeed(iSeed, iWarmUp);
    MersenneTwisterSeed(iSeed, iWarmUp);
    MotherOfAll32Seed(iSeed, iWarmUp);
    MotherOfAll16Seed(iSeed, iWarmUp);
    CompMWCSeed(iSeed, iWarmUp);
    XorShift128Seed(iSeed, iWarmUp);
    XorShiftWeylSeed(iSeed, iWarmUp);
}

/////////////////////////////////////////////////////////////////////////////////

inline ULong RandomGenerator::_MT_MultiplyMatrix(ULong iX, ULong iY) const {
    static const ULong MatrixA[2] = { 0x00000000, 0x9908b0df };
    return ( iX ^ (iY >> 1) ^ MatrixA[iY & 1] );
}
inline ULong RandomGenerator::_MT_CombineBits(ULong iX, ULong iY) const {
    return ( (iX & 0x80000000ul) | (iY & 0x7ffffffful) );
}

