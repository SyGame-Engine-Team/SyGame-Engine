/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Random/Random.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Random generators of various quality ...
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_LIB_MATH_RANDOM_RANDOM_H
#define SCARAB_LIB_MATH_RANDOM_RANDOM_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../ThirdParty/System/Platform.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define RANDOM_SEED_DEFAULT   0xdeadbeeful
#define RANDOM_SEED64_DEFAULT 0x12345678deadbeefui64

#define RandFn RandomGenerator::GetInstancePtr()

/////////////////////////////////////////////////////////////////////////////////
// The RandomGenerator class
class RandomGenerator
{
    // Discrete singleton interface
public:
    inline static RandomGenerator * GetInstancePtr();

private:
    RandomGenerator();
    ~RandomGenerator();

public:
    // Random generator alogtihms :
    // Be sure to distinguish pseudo-random generators (uniform) with true random generators,
    // also called stochastic generators (non uniform) !
    // *Max function returns MAX such that the range is [0 ; MAX]
    // *Seed function inits the generators with an arbitrary user-value
    // LCG = Linear Congruential Generator
    // LF = Lagged Fibonacci
    // GFSR = Generalized Feedback Shift Register
    // MWC = Multiply With Carry
    // CMWC = Complementary Multiply With Carry
    // XS = Xor-Shift
    // Mixing generators allways gives more uniform values, use if needed and cost is ok.
    // The most used combination is MT + MOA or XorShift + MT/MOA ...
    // Final Words are : LEcuyer, Knuth and RanRot are historical mostly, periods ~ 2^32 or less,
    //                   don't pass much tests ...
    //                   MT, MOA, CMWC and XS all pass DieHard tests successfully.
    //                   MT is slow, MOA is excellent and XS is much faster !
    //                   MT and CMWC are reserved for cases you need an extremely long period !
    //                   I personnally recommend XS ;)
    //
    // IMPORTANT : None of those function can do better than 2^32 prevision complexity,
    // which is NOT suitable for cryptographic-level security, use higher-order generators
    // like AES or TreeFish if you need cryptographic-level randomization ...

    // Uniform distributions
        // LCG : L'Ecuyer's method with Bays-Durham shuffle,
	    //       uses Schrage's no-overflow method
    inline ULong LecuyerMax() const;
    Void LecuyerSeed( ULong iSeed, UInt iWarmUp = 0 );
    ULong Lecuyer();
        // LCG : Knuth's substractive method
    inline ULong KnuthMax() const;
    Void KnuthSeed( ULong iSeed, UInt iWarmUp = 0 );
    ULong Knuth();
        // LF : RanRot, W version
    inline UInt64 RanRotMax() const;
    Void RanRotSeed( UInt64 iSeed, UInt iWarmUp = 0 );
    UInt64 RanRot();
        // GFSR : Mersenne Twister, M. Matsumoto & T. Nishimura
        // Period = 2^19937;
    inline ULong MersenneTwisterMax() const;
    Void MersenneTwisterSeed( ULong iSeed, UInt iWarmUp = 0 );
    ULong MersenneTwister();
        // MWC : Mother Of All, George Marsaglia, 32-bits version
        //       and mixed 16-bits version ...
        // Period ~ 2^150; (MOA32)
        // Period ~ 2^250; (MOA16)
    inline ULong MotherOfAllMax() const;
    Void MotherOfAll32Seed( ULong iSeed, UInt iWarmUp = 0 );
    ULong MotherOfAll32();
    Void MotherOfAll16Seed( ULong iSeed, UInt iWarmUp = 0 );
    ULong MotherOfAll16();
        // CMWC : CMWC4096 version, George Marsaglia
        // Period = 2^131104; (Record when I picked it)
    inline ULong CompMWCMax() const;
    Void CompMWCSeed( ULong iSeed, UInt iWarmUp = 0 );
    ULong CompMWC();
        // XS : XorShift, George Marsaglia, 128 version
        //      and Weyl sequence version
        // Period = 2^128 - 1; (128)
        // Period = 2^192 - 1; (Weyl)
    inline ULong XorShiftMax() const;
    Void XorShift128Seed( ULong iSeed, UInt iWarmUp = 0 );
    ULong XorShift128();
    Void XorShiftWeylSeed( ULong iSeed, UInt iWarmUp = 0 );
    ULong XorShiftWeyl();

    // Helpers
    inline Void SeedAll( ULong iSeed, UInt iWarmUp = 0 );

    // Non-Uniform distributions

    // DieHard tests (if ever needed I'll code them one day)
    //Void BirthDaySpacing() {}
    //Void OverlappingPermutations() {}
    //Void MatricesRanks() {}
    //Void MonkeyTest() {}
    //Void CountTheOnes() {}
    //Void ParkingLot() {}
    //Void MinimalDistance() {}
    //Void RandomSpheres() {}
    //Void SqueezeTest() {}
    //Void OverlappingSums() {}
    //Void RunTest() {}
    //Void PlayCraps() {}
private:
    // Lecuyer
    static const Long sm_iLecuyerTableSize = 32;
    static const Long sm_iLecuyerM1 = 2147483563;
    static const Long sm_iLecuyerNDIVS = 1 + ( (sm_iLecuyerM1 - 1) / sm_iLecuyerTableSize );
    Long m_arrLecuyerTable[sm_iLecuyerTableSize];
    Long m_iLecuyerSeed1, m_iLecuyerSeed2;
    Long m_iLecuyerCurrent;

    // Knuth
    static const Long sm_iKnuthTableSize = 56;
    static const Long sm_iKnuthBig = 2147483647;
    Long m_arrKnuthTable[sm_iKnuthTableSize];
    ULong m_iKnuthNext, m_iKnuthNextP;

    // RanRot
    static const ULong sm_iRanRotTableSize = 17;
    ULong m_arrRanRotTable[sm_iRanRotTableSize][2];
    Long m_iRanRotP1, m_iRanRotP2;

    // Mersenne Twister
    inline ULong _MT_MultiplyMatrix(ULong iX, ULong iY) const;
    inline ULong _MT_CombineBits(ULong iX, ULong iY) const;
    Void _MT_RefreshTable();

    static const ULong sm_iMersTwistStateSize = 624;
    static const Long sm_iMersTwistRecOffset = 397;
    ULong m_arrMersTwistTable[sm_iMersTwistStateSize];
    UInt m_iMersTwistCurrent;

    // Mother of All
    ULong m_arrMOA32Table[5];
    UShort m_arrMOA16Table1[10];
    UShort m_arrMOA16Table2[10];

    // CMWC
    static const ULong sm_iCMWCTableSize = 4096;
    ULong m_arrCMWCTable[sm_iCMWCTableSize];
    ULong m_iCMWCCarry;
    UInt m_iCMWCCurrent;

    // XorShift
    ULong m_iXorShiftX;
    ULong m_iXorShiftY;
    ULong m_iXorShiftZ;
    ULong m_iXorShiftW;
    ULong m_iXorShiftV;
    ULong m_iXorShiftD;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Random.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_RANDOM_RANDOM_H

///////////////////////////////////////////////////////////////////////////////////
//// Constants definitions
//	// MERSENNE TWISTER Constants (all exceedingly magic, never change that, built experimentally)
//#define RAND_MERSENNETWISTER_STATE_SIZE				624 // Fundamental
//#define RAND_MERSENNETWISTER_RECURRENCE_OFFSET		397
//#define RAND_MERSENNETWISTER_SEED32_DEFAULT		    5489UL
//
//
///////////////////////////////////////////////////////////////////////////////////
//// The RandomMTG class
//class RandomMTG : public RandomAlgorithm
//{
//public:
//    RandomMTG();
//    ~RandomMTG();
//
//    inline ULong MaxValue() const;
//
//    inline Void Seed(ULong iSeed);
//    Void WarmUp(UInt iIterations);
//
//    inline ULong GenerateValue();
//
//private:
//    inline ULong _MatrixMultiply(ULong iValueX, ULong iValueY) const;
//    inline ULong _CombineBits(ULong iValueX, ULong iValueY) const;
//    inline ULong _TemperValue(ULong iValue) const;
//
//	Void _Initialize(ULong iSeed);
//    Void _RefreshTable();
//    inline ULong _NextValue();
//
//    ULong m_arrTable[RAND_MERSENNETWISTER_STATE_SIZE];
//    Int m_iCurrent;
//};
//
///////////////////////////////////////////////////////////////////////////////////
//// The RandomGenerator class
//class RandomGenerator
//{
//public:
//	RandomGenerator(RandomMode iMode = RAND_MODE_MERSENNE_TWISTER);
//	~RandomGenerator();
//
//    inline Void Seed(ULong iSeed);
//    inline Void WarmUp(UInt iIterations);
//
//	inline Void SetRange32(UInt iRange);
//	inline Void SetRange64(UInt64 iRange);
//
//    inline ULong GenInt32();  // [0;Range]
//    inline UInt64 GenInt64(); // [0;Range]
//    inline Float GenFloat32();    // [0;1]
//    inline Float GenFloat64();    // [0;1]
//    inline Double GenDouble32();  // [0;1]
//    inline Double GenDouble64();  // [0;1]
//
//    // Generate homogeneous directions, angles, etc ...
//
//private:
//    RandomAlgorithm * m_pAlgo;
//
//    Float _Integer32ToFloat;
//	Float _Integer64ToFloat;
//	Double _Integer32ToDouble;
//	Double _Integer64ToDouble;
//
//	UInt m_iRange32;
//	UInt64 m_iRange64;
//	RandomMode m_iMode;
//};

