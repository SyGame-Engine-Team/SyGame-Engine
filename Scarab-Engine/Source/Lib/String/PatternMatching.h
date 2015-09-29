/////////////////////////////////////////////////////////////////////////////////
// File : Lib/String/PatternMatching.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Almost all pattern-matching algorithms ...
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
#ifndef SCARAB_LIB_STRING_PATTERNMATCHING_H
#define SCARAB_LIB_STRING_PATTERNMATCHING_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../ThirdParty/System/String.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define PATTERNMATCHING_MEMORY 32768 // should not use more ...

#define MatchFn PatternMatching::GetInstancePtr()

/////////////////////////////////////////////////////////////////////////////////
// The PatternMatching class
class PatternMatching
{
    // Discrete singleton interface
public:
    inline static PatternMatching * GetInstancePtr();

private:
    PatternMatching();
    ~PatternMatching();

public:
    // Chained searches system
    // WARNING : Use only ONE algorithm in a given chain !
    //           Even if some are compatible (Colussi/GG, ...) this is non-sense.
    // You must assure symmetric calls ...
    inline Void EnterChain();
    inline Void LeaveChain();

    // Patter-Matching alogtihms :
    // ALLWAYS use Null-Terminated strings ! ( strings are considered (length+1) array size ! )
    // NEVER use a pattern length of 0 or > text len ! This will lead to underflows, not handled for speed concerns !
    // All algos use the sliding-window method, starting from the brute force naive approach to very optimized variants ...
    // DFA-like recognition is not preset here, we keep this for parsers ...
    // You'll notice I also dropped the few ones having O(a^2) space preprocessing,
    // horrible with UFT-16/UTF-8 : ZhuTakaoka, BerryRavindran, ... (ReverseColussi is also strongly discouraged)
    // Galil-Seiferas is elegant & tempting but recursion isn't gonna make it and iterative version
    // kills the space advantage of the algo, plus it's quite hard to code !
    // We provide only first-occurence versions for sake of simplicity, needed additional lines are commented if any.
    // Here is a little comparative : ( n = size(text), m = size(pattern), a = size(alphabet), * = eligible for chaining )
    //               Preprocess Space / Time          Worst Time  Max Comps / Best Time
    // BruteForce                          0              O(m*n)                   (2n)
    // BackwardBruteForce                  0              O(m*n)                   (2n) // Right to Left
    // NotSoNaive                       O(1)              O(m*n)                   (<n) // Quite good
    // KarpRabin                 O(1) / O(m)              O(m*n)                  (m+n) // Good with short alphabet
    // *ShiftOr             O(m+aw) / O(m+a)                O(n)                    (n) // (m <= WORDSIZE), we use 64 (QWord), quite efficient
    // *BackwardNDDawg         O(a) / O(m+a)                O(n)          (n*loga(m)/m) // (m <= WORDSIZE), we use 64 (QWord), Excellent !
    // *MorrisPratt                     O(m)              O(m+n)                 (2n-1) // Original MP, delay bounded by m
    // *KnuthMorrisPratt                O(m)              O(m+n)                 (2n-1) // KMP-likes, delay bounded by logphi(m) (phi=golden ratio)
    // *Colussi                         O(m)                O(n)                 (3n/2) // KMP complex variant
    // *GalilGiancarlo                  O(m)                O(n)                 (4n/3) // Improved Colussi
    // *ApostolicoCrochemore            O(m)                O(n)                 (3n/2) // Best KMP, poorest stats overall in practice
    // *KMPSkipSearch                 O(m+a)                O(n)                    (n) // The only bearable version (no list), One of the bests !
    // *BoyerMoore                    O(m+a)              O(m*n)                   (3n) // BM-likes, best case O(n/m) Optimal
    // *BMTurbo                       O(m+a)                O(n)                   (2n) // Quite cool ...
    // *ApostolicoGiancarlo           O(m+a)                O(n)                 (3n/2) // A little more evolved
    // *ReverseColussi        O(ma) / O(m^2)                O(n)                   (2n) // The very best one, but you pay preprocess !
    // *HorsPool               O(a) / O(m+a)              O(m*n)       (1/a) to (2/a+1) // Simplified BMs, best case O(n/m), One of the bests !
    // *QuickSearch            O(a) / O(m+a)              O(m*n)              heuristic // Good with small pattern large alphabet
    // *BMTuned                       O(m+a)              O(m*n)              heuristic // Hacked BM, Excellent !
    // *Smith                  O(a) / O(m+a)              O(m*n)              heuristic // Mixed BM/QS
    // *Raita                  O(a) / O(m+a)              O(m*n)              heuristic // Sparsed comparisons, One of the bests !
    // *TwoWay                   O(1) / O(m)                O(n)                 (2n-m) // Very good ...
    //
    // StressTest clearly shows a great top :
    // - ReverseColussi (if you can afford the preprocess)
    // - Raita / Horspool / KMPSkipSearch
    // - BMTuned / BackwardNDDawg
    // The final word : Raita beats it all !
    //
    // NOTE : Maxime Crochemore gave me a bad text-algo exam note once !
    //        Still glad to have one of my elder teacher's name in my code !
    //        As a vengeance I'll just add his algo just has one of the poorest average
    //        performance accross all algos, both for match and no-match !

        // Naives
    UInt BruteForce( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP = 1 );
    UInt BackwardBruteForce( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP = 1 );
    UInt NotSoNaive( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP = 1 );
    UInt KarpRabin( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP = 1 );

        // Optimals (with constraints though)
    UInt ShiftOr( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP = 1 );
    UInt BackwardNDDawg( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP = 1 );

        // KMP family
    UInt MorrisPratt( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP = 1 );
    UInt KnuthMorrisPratt( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP = 1 );
    UInt Colussi( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP = 1 );
    UInt GalilGiancarlo( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP = 1 );
    UInt ApostolicoCrochemore( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP = 1 );
    UInt KMPSkipSearch( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP = 1 );

        // BM family
    UInt BoyerMoore( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP = 1 );
    UInt BMTurbo( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP = 1 );
    UInt ApostolicoGiancarlo( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP = 1 );
    UInt ReverseColussi( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP = 1 );
    UInt HorsPool( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP = 1 );
    UInt QuickSearch( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP = 1 );
    UInt BMTuned( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP = 1 );
    UInt Smith( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP = 1 );
    UInt Raita( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP = 1 );
    UInt TwoWay( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP = 1 );

    // Debug tests
    typedef UInt (__thiscall PatternMatching::*PatternMatchingFunction)(const GChar *, UInt, const GChar *, UInt);
    typedef Void (*DebugLogFunction)(const GChar *);
    Void StressTest( DebugLogFunction pfLog, PatternMatchingFunction pfPMAlgo,
                     const GChar * strAlgoName, const GChar * strAlphabet,
                     UInt iPatternSamples, UInt iTextSamples, Bool bBackWard = false );
    Void StressTestAll( DebugLogFunction pfLog, const GChar * strAlphabet, UInt iPatternSamples, UInt iTextSamples );

private:
    // internal helpers
    Int _KMPSkipSearch_Attempt( const GChar * strText, const GChar * strPattern, UInt iLengthP, Int iStart, Int iWall ) const;

    // preprocess phases
    QWord _Preprocess_ShiftOr( const GChar * strPattern, UInt iLengthP, QWord * arrShifts );
    Void _Preprocess_BNDD( const GChar * strPattern, UInt iLengthP, QWord * arrBTable );

    Void _Preprocess_MP( const GChar * strPattern, UInt iLengthP, Int * arrMPNext );
    Void _Preprocess_KMP( const GChar * strPattern, UInt iLengthP, Int * arrKMPNext );
    Int _Preprocess_Colussi( const GChar * strPattern, UInt iLengthP, Int * arrHoles, Int * arrNext, Int * arrShift );

    Void _Preprocess_BM_BadCharShift( const GChar * strPattern, UInt iLengthP, Int * arrBCShift );
    Void _Preprocess_BM_QS_BadCharShift( const GChar * strPattern, UInt iLengthP, Int * arrBCShift );
    Void _Preprocess_BM_Suffixes( const GChar * strPattern, UInt iLengthP, Int * arrSuffixes );
    Void _Preprocess_BM_GoodSuffShift( const GChar * strPattern, UInt iLengthP, Int * arrGSShift, Int * arrSuffixes = NULL );
    Void _Preprocess_ReverseColussi( const GChar * strPattern, UInt iLengthP, Int * arrHoles, Int * arrGSShift, Int * arrBCShift );

    Int _Preprocess_TW_MaxSuffix( const GChar * strPattern, UInt iLengthP, Int * pP );
    Int _Preprocess_TW_MaxSuffixInv( const GChar * strPattern, UInt iLengthP, Int * pQ );

    // break allocator
    inline Byte * _Break(UInt iSize);
    inline Void _Unbreak(UInt iSize);
    inline Void _Free();
    Byte m_arrScratch[PATTERNMATCHING_MEMORY];
    Byte * m_pFirstBreak;
    Byte * m_pLastBreak;
    Byte * m_pBreak;
    Bool m_bKeepMemory;   // are we in a persistent sequence ?
    Bool m_bMemoryFilled; // does the memory contains valid preprocess data ?
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "PatternMatching.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_STRING_PATTERNMATCHING_H
