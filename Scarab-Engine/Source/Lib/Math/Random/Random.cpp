/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Random/Random.cpp
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
// Includes
#include "Random.h"

/////////////////////////////////////////////////////////////////////////////////
// RandomGenerator implementation
RandomGenerator::RandomGenerator()
{
    LecuyerSeed(RANDOM_SEED_DEFAULT);
    KnuthSeed(RANDOM_SEED_DEFAULT);
    RanRotSeed(RANDOM_SEED64_DEFAULT);
    MersenneTwisterSeed(RANDOM_SEED_DEFAULT);
    MotherOfAll32Seed(RANDOM_SEED_DEFAULT);
    MotherOfAll16Seed(RANDOM_SEED_DEFAULT);
    CompMWCSeed(RANDOM_SEED_DEFAULT);
    XorShift128Seed(RANDOM_SEED_DEFAULT);
    XorShiftWeylSeed(RANDOM_SEED_DEFAULT);
}
RandomGenerator::~RandomGenerator()
{
}

Void RandomGenerator::LecuyerSeed( ULong iSeed, UInt iWarmUp )
{
    if ( iSeed < 1 )
	    iSeed = 1;
    m_iLecuyerSeed1 = (signed)iSeed;
    m_iLecuyerSeed2 = m_iLecuyerSeed1;
    Int i;

    // Initialize (period 1 only)
    Long iK;
    for(i = (sm_iLecuyerTableSize + 7); i >= 0; --i) {
	    iK = m_iLecuyerSeed1 / 53668;
	    m_iLecuyerSeed1 = 40014 * (m_iLecuyerSeed1 - iK * 53668) - iK * 12211;
	    if (m_iLecuyerSeed1 < 0)
		    m_iLecuyerSeed1 += sm_iLecuyerM1;
        if (i < sm_iLecuyerTableSize)
	        m_arrLecuyerTable[i] = m_iLecuyerSeed1;
    }
    m_iLecuyerCurrent = m_arrLecuyerTable[0];

    // Warmup
    for(i = 0; i < (Int)iWarmUp; ++i)
        Lecuyer();
}
ULong RandomGenerator::Lecuyer()
{
    Long iK;

    // First period
    iK = m_iLecuyerSeed1 / 53668;
    m_iLecuyerSeed1 = 40014 * (m_iLecuyerSeed1 - iK * 53668) - iK * 12211;
    if (m_iLecuyerSeed1 < 0)
        m_iLecuyerSeed1 += sm_iLecuyerM1;

    // Second period
    iK = m_iLecuyerSeed2 / 52774;
    m_iLecuyerSeed2 = 40692 * (m_iLecuyerSeed2 - iK * 52774) - iK * 3791;
    if (m_iLecuyerSeed2 < 0)
        m_iLecuyerSeed2 += 2147483399;

    // Period combination
    iK = m_iLecuyerCurrent / sm_iLecuyerNDIVS;
    m_iLecuyerCurrent = m_arrLecuyerTable[iK] - m_iLecuyerSeed2;
    m_arrLecuyerTable[iK] = m_iLecuyerSeed1;
    if (m_iLecuyerCurrent < 1)
        m_iLecuyerCurrent += (sm_iLecuyerM1 - 1);

    return (unsigned)(m_iLecuyerCurrent - 1);
}

Void RandomGenerator::KnuthSeed( ULong iSeed, UInt iWarmUp )
{
    Int i, j;

    Long iSubstract = (161803398 + (Int)iSeed);
	if (iSubstract < 0)
		iSubstract = (-iSubstract);
	iSubstract %= sm_iKnuthBig;
	m_arrKnuthTable[sm_iKnuthTableSize - 1] = iSubstract;

    Long iK = 1;
	for(i = 1; i < sm_iKnuthTableSize - 1; ++i) {
		j = (i * 21) % (sm_iKnuthTableSize - 1);
		m_arrKnuthTable[j] = iK;
		iK = iSubstract - iK;
		if (iK < 0)
			iK += sm_iKnuthBig;
		iSubstract = m_arrKnuthTable[j];
	}
	for(i = 1; i <= 4; ++i) {
		for(j = 1; j < sm_iKnuthTableSize; ++j) {
			m_arrKnuthTable[j] -= m_arrKnuthTable[1 + (j + 30) % (sm_iKnuthTableSize - 1)];
			if (m_arrKnuthTable[j] < 0)
				m_arrKnuthTable[j] += sm_iKnuthBig;
		}	
	}
	m_iKnuthNext = 0;
	m_iKnuthNextP = 31;

    // Warmup
    for(i = 0; i < (Int)iWarmUp; ++i)
        Knuth();
}
ULong RandomGenerator::Knuth()
{
    if ( ++m_iKnuthNext >= sm_iKnuthTableSize )
	    m_iKnuthNext = 1;
    if ( ++m_iKnuthNextP >= sm_iKnuthTableSize )
	    m_iKnuthNextP = 1;
    Long iSubstract = ( m_arrKnuthTable[m_iKnuthNext] - m_arrKnuthTable[m_iKnuthNextP] );
    if ( iSubstract < 0 )
	    iSubstract += sm_iKnuthBig;
    m_arrKnuthTable[m_iKnuthNext] = iSubstract;
    return iSubstract;
}

Void RandomGenerator::RanRotSeed( UInt64 iSeed, UInt iWarmUp )
{
    Int i;

    // Init
    ULong iSeed1 = (iSeed & 0x00000000ffffffffui64);
    ULong iSeed2 = (iSeed >> 32);
    for(i = 0; i < (Int)sm_iRanRotTableSize; ++i) {
        iSeed1 = iSeed1 * 2891336453 + 1;
        m_arrRanRotTable[i][0] = iSeed1;
        iSeed2 = iSeed2 * 2891336453 + 1;
        m_arrRanRotTable[i][1] = iSeed2;
    }
    m_iRanRotP1 = 0;
    m_iRanRotP2 = 10;

    // Warmup
    for(i = 0; i < (Int)iWarmUp; ++i)
        RanRot();
}
UInt64 RandomGenerator::RanRot()
{
    ULong iZ = BitRotL( m_arrRanRotTable[m_iRanRotP1][0], 19 ) + m_arrRanRotTable[m_iRanRotP2][0];
    ULong iY = BitRotL( m_arrRanRotTable[m_iRanRotP1][1], 27 ) + m_arrRanRotTable[m_iRanRotP2][1];
    m_arrRanRotTable[m_iRanRotP1][0] = iY;
    m_arrRanRotTable[m_iRanRotP1][1] = iZ;
    if ( --m_iRanRotP1 < 0 )
        m_iRanRotP1 = sm_iRanRotTableSize - 1;
    if ( --m_iRanRotP2 < 0 )
        m_iRanRotP2 = sm_iRanRotTableSize - 1;
    UInt64 iRes = iZ;
    return ( (iRes << 32) | (UInt64)iY );
}

Void RandomGenerator::MersenneTwisterSeed( ULong iSeed, UInt iWarmUp )
{
    Int i;

    ULong iKnuthVal;
    m_arrMersTwistTable[sm_iMersTwistStateSize - 1] = iSeed;
    for(i = sm_iMersTwistStateSize - 2;  i >= 0;  --i) {
	    iKnuthVal = (m_arrMersTwistTable[i + 1] >> 30);
	    iKnuthVal ^= m_arrMersTwistTable[i + 1];
	    iKnuthVal *= 1812433253ul;
	    iKnuthVal += (sm_iMersTwistStateSize - 1) - i;
	    m_arrMersTwistTable[i] = iKnuthVal;
    }
    _MT_RefreshTable();

    // Warmup
    for(i = 0; i < (Int)iWarmUp; ++i)
        MersenneTwister();
}
ULong RandomGenerator::MersenneTwister()
{
    if (m_iMersTwistCurrent <= 0)
        _MT_RefreshTable();
    ULong iValue = m_arrMersTwistTable[--m_iMersTwistCurrent];
    iValue ^= (iValue >> 11);
    iValue ^= (iValue << 7) & 0x9d2c5680;
    iValue ^= (iValue << 15) & 0xefc60000;
    iValue ^= (iValue >> 18);
    return iValue;
}

Void RandomGenerator::MotherOfAll32Seed( ULong iSeed, UInt iWarmUp )
{
    Int i;

    // init
    for(Int i = 0; i < 5; ++i) {
        iSeed = iSeed * 29943829 - 1;
        m_arrMOA32Table[i] = iSeed;
    }

    // Warmup
    for(i = 0; i < (Int)iWarmUp; ++i)
        MotherOfAll32();
}
ULong RandomGenerator::MotherOfAll32()
{
    // Summing
    UInt64 iMWC = 2111111111ui64 * (UInt64)(m_arrMOA32Table[3]) +
                  1492ui64 * (UInt64)(m_arrMOA32Table[2]) +
                  1776ui64 * (UInt64)(m_arrMOA32Table[1]) +
                  5115ui64 * (UInt64)(m_arrMOA32Table[0]) +
                  (UInt64)(m_arrMOA32Table[4]);
    // Update
    m_arrMOA32Table[3] = m_arrMOA32Table[2]; //
    m_arrMOA32Table[2] = m_arrMOA32Table[1]; // History
    m_arrMOA32Table[1] = m_arrMOA32Table[0]; //
    m_arrMOA32Table[4] = (ULong)(iMWC >> 32); // Carry
    m_arrMOA32Table[0] = (ULong)(iMWC & 0x00000000ffffffffui64); // Random
    return m_arrMOA32Table[0];
}
Void RandomGenerator::MotherOfAll16Seed( ULong iSeed, UInt iWarmUp )
{
    Int i;

    // init
    UShort iSh = (UShort)(iSeed & 0x0000ffff);
    iSeed = (iSeed & 0x7fffffff);
    for(i = 0; i < 10; ++i) {
        iSeed = 30903ul * iSh + (iSeed >> 16);
        iSh = (UShort)(iSeed & 0x0000ffff);
        m_arrMOA16Table1[i] = iSh;
    }
    for(i = 0; i < 10; ++i) {
        iSeed = 30903ul * iSh + (iSeed >> 16);
        iSh = (UShort)(iSeed & 0x0000ffff);
        m_arrMOA16Table2[i] = iSh;
    }

    // Warmup
    for(i = 0; i < (Int)iWarmUp; ++i)
        MotherOfAll16();
}
ULong RandomGenerator::MotherOfAll16()
{
    // History
    for(Int i = 9; i >= 2; --i) {
        m_arrMOA16Table1[i] = m_arrMOA16Table1[i-1];
        m_arrMOA16Table2[i] = m_arrMOA16Table2[i-1];
    }
    // Summing
    ULong iMWC1 = 1941ul * m_arrMOA16Table1[2] + 1860ul * m_arrMOA16Table1[3] +
                  1812ul * m_arrMOA16Table1[4] + 1776ul * m_arrMOA16Table1[5] +
                  1492ul * m_arrMOA16Table1[6] + 1215ul * m_arrMOA16Table1[7] +
                  1066ul * m_arrMOA16Table1[8] + 12013ul * m_arrMOA16Table1[9] +
                  m_arrMOA16Table1[0];
    ULong iMWC2 = 1111ul * m_arrMOA16Table2[2] + 2222ul * m_arrMOA16Table2[3] +
                  3333ul * m_arrMOA16Table2[4] + 4444ul * m_arrMOA16Table2[5] +
                  5555ul * m_arrMOA16Table2[6] + 6666ul * m_arrMOA16Table2[7] +
                  7777ul * m_arrMOA16Table2[8] + 9272ul * m_arrMOA16Table2[9] +
                  m_arrMOA16Table2[0];
    // Update
    m_arrMOA16Table1[0] = (UShort)(iMWC1 >> 16); //
    m_arrMOA16Table2[0] = (UShort)(iMWC2 >> 16); // Carries
    m_arrMOA16Table1[1] = (UShort)(iMWC1 & 0x0000fffful); //
    m_arrMOA16Table2[1] = (UShort)(iMWC2 & 0x0000fffful); // Randoms
    return ( ((ULong)m_arrMOA16Table1[1]) << 16 ) | ((ULong)m_arrMOA16Table2[1]);
}

Void RandomGenerator::CompMWCSeed( ULong iSeed, UInt iWarmUp )
{
    Int i;

    // Init
    for(i = 0; i < sm_iCMWCTableSize; ++i) {
        iSeed = iSeed * 29943829 - 1;
        m_arrCMWCTable[i] = iSeed;
    }
    m_iCMWCCarry = 362436; // choose initial carry < 809430660
    m_iCMWCCurrent = sm_iCMWCTableSize - 1;

    // Warmup
    for(i = 0; i < (Int)iWarmUp; ++i)
        CompMWC();
}
ULong RandomGenerator::CompMWC()
{
    if ( ++m_iCMWCCurrent >= sm_iCMWCTableSize )
        m_iCMWCCurrent = 0;
    UInt64 iT = 18782ui64 * m_arrCMWCTable[m_iCMWCCurrent] + m_iCMWCCarry;
    m_iCMWCCarry = (ULong)(iT >> 32);
    ULong iX = (ULong)(iT & 0x00000000ffffffffui64) + m_iCMWCCarry;
    if (iX < m_iCMWCCarry) {
        ++iX; ++m_iCMWCCarry;
    }
    m_arrCMWCTable[m_iCMWCCurrent] = 0xfffffffeul - iX;
    return m_arrCMWCTable[m_iCMWCCurrent];
}

Void RandomGenerator::XorShift128Seed( ULong iSeed, UInt iWarmUp )
{
    Int i;

    // Init
    m_iXorShiftX = iSeed;
    m_iXorShiftY = 362436069;
    m_iXorShiftZ = 521288629;
    m_iXorShiftW = 88675123;

    // Warmup
    for(i = 0; i < (Int)iWarmUp; ++i)
        XorShift128();
}
ULong RandomGenerator::XorShift128()
{
    ULong iT = m_iXorShiftX ^ (m_iXorShiftX << 11);
    m_iXorShiftX = m_iXorShiftY;
    m_iXorShiftY = m_iXorShiftZ;
    m_iXorShiftZ = m_iXorShiftW;
    m_iXorShiftW = m_iXorShiftW ^ (m_iXorShiftW >> 19) ^ iT ^ (iT >> 8);
    return m_iXorShiftW;
}

Void RandomGenerator::XorShiftWeylSeed( ULong iSeed, UInt iWarmUp )
{
    Int i;

    // Init
    m_iXorShiftX = iSeed;
    m_iXorShiftY = 362436069;
    m_iXorShiftZ = 521288629;
    m_iXorShiftW = 88675123;
    m_iXorShiftV = 5783321;
    m_iXorShiftD = 6615241;

    // Warmup
    for(i = 0; i < (Int)iWarmUp; ++i)
        XorShiftWeyl();
}
ULong RandomGenerator::XorShiftWeyl()
{
    ULong iT = m_iXorShiftX ^ (m_iXorShiftX >> 2);
    m_iXorShiftX = m_iXorShiftY;
    m_iXorShiftY = m_iXorShiftZ;
    m_iXorShiftZ = m_iXorShiftW;
    m_iXorShiftW = m_iXorShiftV;
    m_iXorShiftV = m_iXorShiftV ^ (m_iXorShiftV << 4) ^ iT ^ (iT << 1);
    m_iXorShiftD += 362437;
    return m_iXorShiftD + m_iXorShiftV;
}

/////////////////////////////////////////////////////////////////////////////////

Void RandomGenerator::_MT_RefreshTable()
{
    Int i;

    ULong iScratch1, iScratch2;
    ULong * pState = m_arrMersTwistTable + (sm_iMersTwistStateSize - 1);
    iScratch1 = *pState;

    // Pass 1
    i = (sm_iMersTwistStateSize - sm_iMersTwistRecOffset) >> 1; // /2
    while( --i >= 0 ) {
        pState -= 2;
        iScratch2 = pState[1];
        iScratch1 = _MT_CombineBits( iScratch1, iScratch2 );
        pState[2] = _MT_MultiplyMatrix( pState[2 - sm_iMersTwistRecOffset], iScratch1 );
        iScratch1 = pState[0];
        iScratch2 = _MT_CombineBits( iScratch2, iScratch1 );
        pState[1] = _MT_MultiplyMatrix( pState[1 - sm_iMersTwistRecOffset], iScratch2 );
    }
    iScratch2 = *(--pState);
    iScratch1 = _MT_CombineBits( iScratch1, iScratch2 );
    pState[1] = _MT_MultiplyMatrix( pState[1 - sm_iMersTwistRecOffset], iScratch1 );

    // Pass 2
    i = (sm_iMersTwistRecOffset - 1) >> 1; // /2
    while( --i >= 0 ) {
        pState -= 2;
        iScratch1 = pState[1];
        iScratch2 = _MT_CombineBits( iScratch2, iScratch1 );
        pState[2] = _MT_MultiplyMatrix( pState[sm_iMersTwistStateSize + 2 - sm_iMersTwistRecOffset], iScratch2 );
        iScratch2 = pState[0];
        iScratch1 = _MT_CombineBits( iScratch1, iScratch2 );
        pState[1] = _MT_MultiplyMatrix( pState[sm_iMersTwistStateSize + 1 - sm_iMersTwistRecOffset], iScratch1 );
    }
    iScratch1 = _MT_CombineBits( iScratch2, m_arrMersTwistTable[sm_iMersTwistStateSize - 1] );
    *pState = _MT_MultiplyMatrix( pState[sm_iMersTwistStateSize - sm_iMersTwistRecOffset], iScratch1 );

    // Ready
    m_iMersTwistCurrent = sm_iMersTwistStateSize;
}

