/////////////////////////////////////////////////////////////////////////////////
// File : Lib/String/PatternMatching.cpp
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
// Includes
#include "PatternMatching.h"

/////////////////////////////////////////////////////////////////////////////////
// PatternMatching implementation
PatternMatching::PatternMatching()
{
    m_pFirstBreak = (Byte*)m_arrScratch;
    m_pLastBreak = (Byte*)(m_arrScratch + PATTERNMATCHING_MEMORY);
    m_pBreak = m_pFirstBreak;
    m_bKeepMemory = false;
    m_bMemoryFilled = false;
}
PatternMatching::~PatternMatching()
{
}

UInt PatternMatching::BruteForce( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP )
{
    DebugAssert( iLengthP < iLengthT );

    const GChar * pT = strText;
    const GChar * pTEnd = strText + iLengthT - iLengthP + 1;
    while(pT < pTEnd) {
        if ( StringFn->NCmp( strPattern, pT, iLengthP ) == 0 )
            return ( pT - strText );
        ++pT;
    }
    return INVALID_OFFSET;
}
UInt PatternMatching::BackwardBruteForce( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP )
{
    DebugAssert( iLengthP < iLengthT );

    const GChar * pT = strText + iLengthT - iLengthP;
    const GChar * pTEnd = strText - 1;
    while(pT > pTEnd) {
        if ( StringFn->NCmp( strPattern, pT, iLengthP ) == 0 )
            return (pT - strText);
        --pT;
    }
    return INVALID_OFFSET;
}
UInt PatternMatching::NotSoNaive( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP )
{
    DebugAssert( iLengthP < iLengthT );

    // Preprocess phase
    Int iK, iL;
    if ( strPattern[0] == strPattern[1] ) {
        iK = 2;
        iL = 1;
    }
    else {
        iK = 1;
        iL = 2;
    }

    // Search phase
    const GChar * pT = strText;
    const GChar * pTEnd = strText + iLengthT - iLengthP + 1;
    while(pT < pTEnd) {
        if ( strPattern[1] != *(pT + 1) )
            pT += iK;
        else {
            if ( ( StringFn->NCmp(strPattern + 2, pT + 2, iLengthP - 2) == 0 ) &&
                 ( strPattern[0] == *pT ) )
                return (pT - strText);
            pT += iL;
        }
    }
    return INVALID_OFFSET;
}
UInt PatternMatching::KarpRabin( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP )
{
    DebugAssert( iLengthP < iLengthT );

    const GChar * pP = strPattern;
    const GChar * pPEnd = strPattern + iLengthP;
    const GChar * pT = strText;
    const GChar * pTEnd = strText + iLengthT - iLengthP + 1;

    // Preprocess phase
    Int iPow2 = ( 1 << (iLengthP - 1) );
    Int hP = 0, hT = 0;
    while(pP < pPEnd) {
        hP = ( (hP << 1) + *(pP++) );
        hT = ( (hT << 1) + *(pT++) );
    }

    // Search phase
    pT = strText;
    while(pT < pTEnd) {
        if ( (hP == hT) && ( StringFn->NCmp(strPattern, pT, iLengthP) == 0 ) )
            return (pT - strText);
        hT = ( ( hT - iPow2 * (*pT) ) << 1 ) + *(pT + iLengthP);
        ++pT;
    }
    return INVALID_OFFSET;
}

UInt PatternMatching::ShiftOr( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP )
{
    DebugAssert( iLengthP < iLengthT );

    if ( iLengthP > (sizeof(QWord) << 3) )
        return INVALID_OFFSET;

    UInt iResult = INVALID_OFFSET;

    // Preprocess phase
    QWord * pLimit;
    QWord * arrShifts;
    if ( m_bMemoryFilled ) {
        pLimit = (QWord*)( m_pFirstBreak );
        arrShifts = (QWord*)( m_pFirstBreak + sizeof(QWord) );
    } else {
        pLimit = (QWord*)_Break( sizeof(QWord) );
        arrShifts = (QWord*)_Break( sizeof(QWord) << 8 ); // *256
        *pLimit = _Preprocess_ShiftOr( strPattern, iLengthP, arrShifts );

        m_bMemoryFilled = true;
    }

    // Search phase
    QWord qwState = 0xffffffffffffffffui64;
    const GChar * pT = strText;
    const GChar * pTEnd = strText + iLengthT;
    while(pT < pTEnd) {
        qwState = (qwState << 1) | arrShifts[*pT];
        if ( qwState < *pLimit ) {
            iResult = ( (pT - strText) - iLengthP + 1 );
            break;
        }
        ++pT;
    }

    // Cleanup memory
    if ( !m_bKeepMemory ) {
        _Free();
        m_bMemoryFilled = false;
    }

    return iResult;
}
UInt PatternMatching::BackwardNDDawg( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP )
{
    DebugAssert( iLengthP < iLengthT );

    UInt iResult = INVALID_OFFSET;

    // Preprocess phase
    QWord * arrBTable;
    if ( m_bMemoryFilled ) {
        arrBTable = (QWord*)m_pFirstBreak;
    } else {
        arrBTable = (QWord*)_Break( sizeof(QWord) << 8 ); // *256
        _Preprocess_BNDD( strPattern, iLengthP, arrBTable );

        m_bMemoryFilled = true;
    }

    // Search phase
    QWord qwState;
    Int iIndex, iLast;
    Int iCh = 0;
    Int iEnd = ( (Int)iLengthT - (Int)iLengthP );
    while( iCh <= iEnd ) {
        iIndex = iLengthP - 1;
        iLast = iLengthP;
        qwState = 0xffffffffffffffffui64;
        while( iIndex >= 0 && qwState != 0ui64 ) {
            qwState &= arrBTable[strText[iCh + iIndex]];
            --iIndex;
            if ( qwState != 0ui64 ) {
                if ( iIndex >= 0 )
                    iLast = iIndex + 1;
                else {
                    iResult = iCh;
                    break;
                }
            }
            qwState <<= 1;
        }
        iCh += iLast;
    }

    // Cleanup memory
    if ( !m_bKeepMemory ) {
        _Free();
        m_bMemoryFilled = false;
    }

    return iResult;
}

UInt PatternMatching::MorrisPratt( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP )
{
    DebugAssert( iLengthP < iLengthT );

    UInt iResult = INVALID_OFFSET;

    // Preprocess phase
    Int * arrMPNext;
    if ( m_bMemoryFilled )
        arrMPNext = (Int*)m_pFirstBreak;
    else {
        arrMPNext = (Int*)_Break( (iLengthP + 1) * sizeof(Int) );
        _Preprocess_MP( strPattern, iLengthP, arrMPNext );

        m_bMemoryFilled = true;
    }

    // Search phase
    Int iIndex = 0;
    const GChar * pT = strText;
    const GChar * pTEnd = strText + iLengthT;
    while(pT < pTEnd) {
        while( (iIndex > -1) && (strPattern[iIndex] != *pT) )
            iIndex = arrMPNext[iIndex];
        ++iIndex;
        ++pT;
        if ( iIndex >= (Int)iLengthP ) {
            iResult = (pT - strText) - iIndex;
            //iIndex = arrMPNext[iIndex]; // not used, needed for next occurences
            break;
        }
    }

    // Cleanup memory
    if ( !m_bKeepMemory ) {
        _Free();
        m_bMemoryFilled = false;
    }

    return iResult;
}
UInt PatternMatching::KnuthMorrisPratt( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP )
{
    DebugAssert( iLengthP < iLengthT );

    UInt iResult = INVALID_OFFSET;

    // Preprocess phase
    Int * arrKMPNext;
    if ( m_bMemoryFilled )
        arrKMPNext = (Int*)m_pFirstBreak;
    else {
        arrKMPNext = (Int*)_Break( (iLengthP + 1) * sizeof(Int) );
        _Preprocess_KMP( strPattern, iLengthP, arrKMPNext );

        m_bMemoryFilled = true;
    }

    // Search phase
    Int iIndex = 0;
    const GChar * pT = strText;
    const GChar * pTEnd = strText + iLengthT;
    while(pT < pTEnd) {
        while( (iIndex > -1) && (strPattern[iIndex] != *pT) )
            iIndex = arrKMPNext[iIndex];
        ++iIndex;
        ++pT;
        if ( iIndex >= (Int)iLengthP ) {
            iResult = (pT - strText) - iIndex;
            //iIndex = arrKMPNext[iIndex]; // not used, needed for next occurences
            break;
        }
    }

    // Cleanup memory
    if ( !m_bKeepMemory ) {
        _Free();
        m_bMemoryFilled = false;
    }

    return iResult;
}
UInt PatternMatching::Colussi( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP )
{
    DebugAssert( iLengthP < iLengthT );

    UInt iResult = INVALID_OFFSET;

    // Preprocess phase
    Int * pND;
    Int * arrHoles;
    Int * arrNext;
    Int * arrShift;
    UInt iSize = (iLengthP + 1) * sizeof(Int);
    if ( m_bMemoryFilled ) {
        Byte * pBreak = m_pFirstBreak;
        pND      = (Int*)pBreak; pBreak += sizeof(Int);
        arrHoles = (Int*)pBreak; pBreak += iSize;
        arrNext  = (Int*)pBreak; pBreak += iSize;
        arrShift = (Int*)pBreak;
    } else {
        pND      = (Int*)_Break( sizeof(Int) );
        arrHoles = (Int*)_Break( iSize );
        arrNext  = (Int*)_Break( iSize );
        arrShift = (Int*)_Break( iSize );
        *pND = _Preprocess_Colussi( strPattern, iLengthP, arrHoles, arrNext, arrShift );

        m_bMemoryFilled = true;
    }

    // Search phase
    Int iLast = -1;
    Int iIndex = 0;
    Int iCh = 0;
    Int iEnd = ( (Int)iLengthT - (Int)iLengthP );
    while( iCh <= iEnd ) {
        while( ( iIndex < (Int)iLengthP ) &&
               ( iLast < iCh + arrHoles[iIndex] ) &&
               ( strPattern[arrHoles[iIndex]] == strText[iCh + arrHoles[iIndex]] ) )
            ++iIndex;
        if ( iIndex >= (Int)iLengthP || iLast >= iCh + arrHoles[iIndex] ) {
            iResult = iCh;
            //iIndex = iLengthP; // not used, needed for next occurences
            break;
        }
        if ( iIndex > (*pND) )
            iLast = iCh + iLengthP - 1;
        iCh += arrShift[iIndex];
        iIndex = arrNext[iIndex];
    }

    // Cleanup memory
    if ( !m_bKeepMemory ) {
        _Free();
        m_bMemoryFilled = false;
    }

    return iResult;
}
UInt PatternMatching::GalilGiancarlo( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP )
{
    DebugAssert( iLengthP < iLengthT );

    UInt iResult = INVALID_OFFSET;
    Int iL;
    for(iL = 0; strPattern[iL] == strPattern[iL+1]; ++iL) {}

    // Special case : power of a single char
    if ( iL == (Int)(iLengthP - 1) ) {
        iL = 0;
        const GChar * pT = strText;
        const GChar * pTEnd = strText + iLengthT;
        while(pT < pTEnd) {
            if ( strPattern[0] == *pT ) {
                ++iL;
                if ( iL >= (Int)iLengthP )
                    return (pT - strText) - iLengthP + 1;
            } else
                iL = 0;
            ++pT;
        }
        return INVALID_OFFSET;
    }

    // Preprocess phase
    Int * pND;
    Int * arrHoles;
    Int * arrNext;
    Int * arrShift;
    UInt iSize = (iLengthP + 1) * sizeof(Int);
    if ( m_bMemoryFilled ) {
        Byte * pBreak = m_pFirstBreak;
        pND      = (Int*)pBreak; pBreak += sizeof(Int);
        arrHoles = (Int*)pBreak; pBreak += iSize;
        arrNext  = (Int*)pBreak; pBreak += iSize;
        arrShift = (Int*)pBreak;
    } else {
        pND      = (Int*)_Break( sizeof(Int) );
        arrHoles = (Int*)_Break( iSize );
        arrNext  = (Int*)_Break( iSize );
        arrShift = (Int*)_Break( iSize );
        *pND = _Preprocess_Colussi( strPattern, iLengthP, arrHoles, arrNext, arrShift );

        m_bMemoryFilled = true;
    }

    // Search phase
    Int iHeavy = 0;
    Int iLast = -1;
    Int iIndex = 0;
    Int iCh = 0, iK;
    Int iEnd = (Int)( iLengthT - iLengthP );
    while( iCh <= iEnd ) {
        if ( iHeavy && (iIndex == 0) ) {
            iK = iLast - iCh + 1;
            while( strPattern[0] == strText[iCh + iK] )
                ++iK;
            if ( iK <= iL || strPattern[iL + 1] != strText[iCh + iK] ) {
                iIndex = 0;
                iCh += iK + 1;
                iLast = iCh - 1;
            } else {
                iIndex = 1;
                iLast = iCh + iK;
                iCh = iLast - (iL + 1);
            }
            //iHeavy = 0; // useless
        } else {
            while( ( iIndex < (Int)iLengthP ) &&
                   ( iLast < iCh + arrHoles[iIndex] ) &&
                   ( strPattern[arrHoles[iIndex]] == strText[iCh + arrHoles[iIndex]] ) )
                ++iIndex;
            if ( iIndex >= (Int)iLengthP || iLast >= iCh + arrHoles[iIndex] ) {
                iResult = iCh;
                //iIndex = iLengthP; // not used, needed for next occurences
                break;
            }
            if ( iIndex > (*pND) )
                iLast = iCh + iLengthP - 1;
            iCh += arrShift[iIndex];
            iIndex = arrNext[iIndex];
        }
        iHeavy = (iCh > iLast) ? 0 : 1;
    }

    // Cleanup memory
    if ( !m_bKeepMemory ) {
        _Free();
        m_bMemoryFilled = false;
    }

    return iResult;
}
UInt PatternMatching::ApostolicoCrochemore( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP )
{
    DebugAssert( iLengthP < iLengthT );

    UInt iResult = INVALID_OFFSET;

    // Preprocess phase
    Int * pL;
    Int * arrKMPNext;
    if ( m_bMemoryFilled ) {
        pL = (Int*)m_pFirstBreak;
        arrKMPNext = (Int*)( m_pFirstBreak + sizeof(Int) );
    } else {
        pL = (Int*)_Break( sizeof(Int) );
        arrKMPNext = (Int*)_Break( (iLengthP + 1) * sizeof(Int) );

        for(*pL = 1; strPattern[(*pL) - 1] == strPattern[*pL]; ++(*pL)) {}
        if ( (*pL) == (Int)iLengthP )
            *pL = 0;
        _Preprocess_KMP( strPattern, iLengthP, arrKMPNext );

        m_bMemoryFilled = true;
    }

    // Search phase
    Int iIndex = *pL, iK = 0;
    Int iCh = 0;
    Int iEnd = ( (Int)iLengthT - (Int)iLengthP );
    while( iCh <= iEnd ) {
        while( iIndex < (Int)iLengthP && strPattern[iIndex] == strText[iIndex + iCh] )
            ++iIndex;
        if ( iIndex >= (Int)iLengthP ) {
            while( iK < *pL && strPattern[iK] == strText[iK + iCh] )
                ++iK;
            if (iK >= *pL) {
                iResult = iCh;
                break;
            }
        }
        iCh += ( iIndex - arrKMPNext[iIndex] );
        if (iIndex == *pL)
            iK = Max<Int>(0, iK - 1);
        else {
            if ( arrKMPNext[iIndex] <= *pL ) {
                iK = Max<Int>(0, arrKMPNext[iIndex]);
                iIndex = *pL;
            } else {
                iK = *pL;
                iIndex = arrKMPNext[iIndex];
            }
        }
    }

    // Cleanup memory
    if ( !m_bKeepMemory ) {
        _Free();
        m_bMemoryFilled = false;
    }

    return iResult;
}
UInt PatternMatching::KMPSkipSearch( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP )
{
    DebugAssert( iLengthP < iLengthT );

    UInt iResult = INVALID_OFFSET;

    // Preprocess phase
    Int * arrMPNext;
    Int * arrKMPNext;
    Int * arrList;
    Int * arrZ;
    UInt iSize = (iLengthP + 1) * sizeof(Int);
    if ( m_bMemoryFilled ) {
        Byte * pBreak = m_pFirstBreak;
        arrMPNext  = (Int*)pBreak; pBreak += iSize;
        arrKMPNext = (Int*)pBreak; pBreak += iSize;
        arrList    = (Int*)pBreak; pBreak += iSize;
        arrZ       = (Int*)pBreak;
    } else {
        arrMPNext  = (Int*)_Break( iSize );
        arrKMPNext = (Int*)_Break( iSize );
        arrList    = (Int*)_Break( iSize );
        arrZ       = (Int*)_Break( sizeof(Int) << 8 ); // *256
        _Preprocess_MP( strPattern, iLengthP, arrMPNext );
        _Preprocess_KMP( strPattern, iLengthP, arrKMPNext );
        MemSet( arrZ, 0xff, sizeof(Int) << 8 ); // *256
        arrList[0] = -1;
        arrZ[strPattern[0]] = 0;
        for(UInt i = 1; i < iLengthP; ++i) {
            arrList[i] = arrZ[strPattern[i]];
            arrZ[strPattern[i]] = i;
        }

        m_bMemoryFilled = true;
    }

    // Search phase
    Int iKMPStart, iStart, iWall = 0;
    //Int iPR = iLengthP - arrKMPNext[iLengthP];
    Int iIndex = -1, iK;
    Int iCh = -1;
    Int iEnd = ( (Int)iLengthT - (Int)iLengthP );
    do {
        iCh += iLengthP;
    } while( iCh < (Int)iLengthT && arrZ[strText[iCh]] < 0 );
    if ( iCh >= (Int)iLengthT )
        goto _KMPSKIPSEARCH_END; // no occurence
    iIndex = arrZ[strText[iCh]];
    iStart = iCh - iIndex;
    while( iStart <= iEnd ) {
        if (iStart > iWall)
            iWall = iStart;
        iK = _KMPSkipSearch_Attempt( strText, strPattern, iLengthP, iStart, iWall );
        iWall = iStart + iK;
        if ( iK == (Int)iLengthP ) {
            iResult = iStart;
            //iIndex -= iPR; // not used, needed for next occurences
            goto _KMPSKIPSEARCH_END;
        } else
            iIndex = arrList[iIndex];
        if (iIndex < 0) {
            do {
                iCh += iLengthP;
            } while( iCh < (Int)iLengthT && arrZ[strText[iCh]] < 0 );
            if ( iCh >= (Int)iLengthT )
                goto _KMPSKIPSEARCH_END; // no occurence
            iIndex = arrZ[strText[iCh]];
        }
        iKMPStart = iStart + iK - arrKMPNext[iK];
        iK = arrKMPNext[iK];
        iStart = iCh - iIndex;
        while( iStart < iKMPStart || (iKMPStart < iStart && iStart < iWall) ) {
            if (iStart < iKMPStart) {
                iIndex = arrList[iIndex];
                if (iIndex < 0) {
                    do {
                        iCh += iLengthP;
                    } while( iCh < (Int)iLengthT && arrZ[strText[iCh]] < 0 );
                    if ( iCh >= (Int)iLengthT )
                        goto _KMPSKIPSEARCH_END; // no occurence
                    iIndex = arrZ[strText[iCh]];
                }
                iStart = iCh - iIndex;
            } else {
                iKMPStart += (iK - arrMPNext[iK]);
                iK = arrMPNext[iK];
            }
        }
    }
_KMPSKIPSEARCH_END:

    // Cleanup memory
    if ( !m_bKeepMemory ) {
        _Free();
        m_bMemoryFilled = false;
    }

    return iResult;
}

UInt PatternMatching::BoyerMoore( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP )
{
    DebugAssert( iLengthP < iLengthT );

    UInt iResult = INVALID_OFFSET;

    // Preprocess phase
    Int * arrGSShift;
    Int * arrBCShift;
    UInt iSize = (iLengthP + 1) * sizeof(Int);
    if ( m_bMemoryFilled ) {
        arrGSShift = (Int*)( m_pFirstBreak );
        arrBCShift = (Int*)( m_pFirstBreak + iSize );
    } else {
        arrGSShift = (Int*)_Break( iSize );
        arrBCShift = (Int*)_Break( sizeof(Int) << 8 ); // *256
        _Preprocess_BM_GoodSuffShift( strPattern, iLengthP, arrGSShift );
        _Preprocess_BM_BadCharShift( strPattern, iLengthP, arrBCShift );

        m_bMemoryFilled = true;
    }

    // Search phase
    Int iIndex;
    Int iCh = 0;
    Int iEnd = ( (Int)iLengthT - (Int)iLengthP );
    --iLengthP; // hack
    while( iCh <= iEnd ) {
        iIndex = iLengthP;
        while( iIndex >= 0 && strPattern[iIndex] == strText[iCh + iIndex] )
            --iIndex;
        if (iIndex < 0) {
            iResult = iCh;
            //iCh += arrGSShift[0]; // not used, needed for next occurences
            break;
        } else {
            iCh += Max<Int>( arrGSShift[iIndex],
                             arrBCShift[strText[iCh + iIndex]] - iLengthP + iIndex );
        }
    }

    // Cleanup memory
    if ( !m_bKeepMemory ) {
        _Free();
        m_bMemoryFilled = false;
    }

    return iResult;
}
UInt PatternMatching::BMTurbo( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP )
{
    DebugAssert( iLengthP < iLengthT );

    UInt iResult = INVALID_OFFSET;

    // Preprocess phase
    Int * arrGSShift;
    Int * arrBCShift;
    UInt iSize = (iLengthP + 1) * sizeof(Int);
    if ( m_bMemoryFilled ) {
        arrGSShift = (Int*)( m_pFirstBreak );
        arrBCShift = (Int*)( m_pFirstBreak + iSize );
    } else {
        arrGSShift = (Int*)_Break( iSize );
        arrBCShift = (Int*)_Break( sizeof(Int) << 8 ); // *256
        _Preprocess_BM_GoodSuffShift( strPattern, iLengthP, arrGSShift );
        _Preprocess_BM_BadCharShift( strPattern, iLengthP, arrBCShift );

        m_bMemoryFilled = true;
    }

    // Search phase
    Int shiftTurbo, shiftBC, shiftGS = iLengthP;
    Int iIndex, iU = 0, iV;
    Int iCh = 0;
    Int iEnd = ( (Int)iLengthT - (Int)iLengthP );
    --iLengthP; // hack
    while( iCh <= iEnd ) {
        iIndex = iLengthP;
        while( iIndex >= 0 && strPattern[iIndex] == strText[iCh + iIndex] ) {
            --iIndex;
            if ( iU != 0 && iIndex == (((Int)iLengthP) - shiftGS) )
                iIndex -= iU;
        }
        if (iIndex < 0) {
            iResult = iCh;
            //shiftGS = arrGSShift[0]; // not used, needed for next occurences
            //iU = iLengthP + 1 - shiftGS;
            break;
        } else {
            iV = iLengthP - iIndex;
            shiftTurbo = iU - iV;
            shiftBC = arrBCShift[strText[iCh + iIndex]] - iV;
            shiftGS = Max<Int>( shiftTurbo, shiftBC );
            if ( shiftGS <= arrGSShift[iIndex] ) {
                shiftGS = arrGSShift[iIndex];
                iU = Min<Int>( iLengthP + 1 - shiftGS, iV );
            } else {
                if ( shiftTurbo < shiftBC && shiftGS < iU + 1 )
                    shiftGS = iU + 1;
                iU = 0;
            }
        }
        iCh += shiftGS;
    }

    // Cleanup memory
    if ( !m_bKeepMemory ) {
        _Free();
        m_bMemoryFilled = false;
    }

    return iResult;
}
UInt PatternMatching::ApostolicoGiancarlo( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP )
{
    DebugAssert( iLengthP < iLengthT );

    UInt iResult = INVALID_OFFSET;

    // Preprocess phase
    Int * arrSkip;
    Int * arrSuffixes;
    Int * arrGSShift;
    Int * arrBCShift;
    UInt iSize = (iLengthP + 1) * sizeof(Int);
    if ( m_bMemoryFilled ) {
        Byte * pBreak = m_pFirstBreak;
        arrSkip     = (Int*)pBreak; pBreak += iSize;
        arrSuffixes = (Int*)pBreak; pBreak += iSize;
        arrGSShift  = (Int*)pBreak; pBreak += iSize;
        arrBCShift  = (Int*)pBreak;

        MemZero( arrSkip, iSize );
    } else {
        arrSkip     = (Int*)_Break( iSize );
        arrSuffixes = (Int*)_Break( iSize );
        arrGSShift  = (Int*)_Break( iSize );
        arrBCShift  = (Int*)_Break( sizeof(Int) << 8 ); // *256
        _Preprocess_BM_Suffixes( strPattern, iLengthP, arrSuffixes );
        _Preprocess_BM_GoodSuffShift( strPattern, iLengthP, arrGSShift, arrSuffixes );
        _Preprocess_BM_BadCharShift( strPattern, iLengthP, arrBCShift );

        MemZero( arrSkip, iSize );

        m_bMemoryFilled = true;
    }

    // Search phase
    Int shiftGS, iTmp;
    Int iIndex, iSkip, iSuff;
    Int iCh = 0;
    Int iEnd = ( (Int)iLengthT - (Int)iLengthP );
    --iLengthP; // hack
    while( iCh <= iEnd ) {
        iIndex = iLengthP;
        while( iIndex >= 0 ) {
            iSkip = arrSkip[iIndex];
            iSuff = arrSuffixes[iIndex];
            if (iSkip > 0) {
                if (iSkip > iSuff) {
                    iIndex = (iIndex + 1 == iSuff) ? -1 : (iIndex - iSuff);
                    break;
                } else {
                    iIndex -= iSkip;
                    if (iSkip < iSuff)
                        break;
                }
            } else {
                if ( strPattern[iIndex] == strText[iCh + iIndex] )
                    --iIndex;
                else
                    break;
            }
        }
        if (iIndex < 0) {
            iResult = iCh;
            //arrSkip[iLengthP] = iLengthP + 1; // not used, needed for next occurences
            //shiftGS = arrGSShift[0];
            break;
        } else {
            arrSkip[iLengthP] = iLengthP - iIndex;
            shiftGS = Max<Int>( arrGSShift[iIndex],
                                arrBCShift[strText[iCh + iIndex]] - iLengthP + iIndex );
        }
        iCh += shiftGS;
        iTmp = iLengthP + 1 - shiftGS;
        MemCopy( arrSkip, arrSkip + shiftGS, iTmp * sizeof(Int) );
        MemZero( arrSkip + iTmp, shiftGS * sizeof(Int) );
    }

    // Cleanup memory
    if ( !m_bKeepMemory ) {
        _Free();
        m_bMemoryFilled = false;
    }

    return iResult;
}
UInt PatternMatching::ReverseColussi( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP )
{
    DebugAssert( iLengthP < iLengthT );

    UInt iResult = INVALID_OFFSET;

    // Preprocess phase
    Int * arrHoles;
    Int * arrGSShift;
    Int * arrBCShift;
    UInt iSize = (iLengthP + 1) * sizeof(Int);
    if ( m_bMemoryFilled ) {
        Byte * pBreak = m_pFirstBreak;
        arrHoles   = (Int*)pBreak; pBreak += iSize;
        arrGSShift = (Int*)pBreak; pBreak += iSize;
        arrBCShift = (Int*)pBreak;
    } else {
        arrHoles   = (Int*)_Break( iSize );
        arrGSShift = (Int*)_Break( iSize );
        arrBCShift = (Int*)_Break( iSize << 8 ); // *256
        _Preprocess_ReverseColussi( strPattern, iLengthP, arrHoles, arrGSShift, arrBCShift );

        m_bMemoryFilled = true;
    }

    // Search phase
    Int iIndex, iS = iLengthP;
    Int iCh = 0;
    Int iEnd = ( (Int)iLengthT - (Int)iLengthP );
    while( iCh <= iEnd ) {
        while( iCh <= iEnd && strPattern[iLengthP - 1] != strText[iCh + iLengthP - 1] ) {
            iS = arrBCShift[ ( (iLengthP + 1) * strText[iCh + iLengthP - 1] ) + iS ];
            iCh += iS;
        }
        iIndex = 1;
        while( iIndex < (Int)iLengthP && strPattern[arrHoles[iIndex]] == strText[iCh + arrHoles[iIndex]] )
            ++iIndex;
        if ( iIndex >= (Int)iLengthP && iCh <= iEnd ) {
            iResult = iCh;
            break;
        }
        iS = arrGSShift[iIndex];
        iCh += iS;
    }

    // Cleanup memory
    if ( !m_bKeepMemory ) {
        _Free();
        m_bMemoryFilled = false;
    }

    return iResult;
}
UInt PatternMatching::HorsPool( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP )
{
    DebugAssert( iLengthP < iLengthT );

    UInt iResult = INVALID_OFFSET;

    // Preprocess phase
    Int * arrBCShift;
    if ( m_bMemoryFilled ) {
        arrBCShift = (Int*)m_pFirstBreak;
    } else {
        arrBCShift = (Int*)_Break( sizeof(Int) << 8 ); // *256
        _Preprocess_BM_BadCharShift( strPattern, iLengthP, arrBCShift );

        m_bMemoryFilled = true;
    }

    // Search phase
    GChar ch;
    Int iCh = 0;
    Int iEnd = ( (Int)iLengthT - (Int)iLengthP );
    --iLengthP; // hack
    while( iCh <= iEnd ) {
        ch = strText[iCh + iLengthP];
        if ( strPattern[iLengthP] == ch ) {
            if ( StringFn->NCmp( strPattern, strText + iCh, iLengthP ) == 0 ) {
                iResult = iCh;
                break;
            }
        }
        iCh += arrBCShift[ch];
    }

    // Cleanup memory
    if ( !m_bKeepMemory ) {
        _Free();
        m_bMemoryFilled = false;
    }

    return iResult;
}
UInt PatternMatching::QuickSearch( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP )
{
    DebugAssert( iLengthP < iLengthT );

    UInt iResult = INVALID_OFFSET;

    // Preprocess phase
    Int * arrBCShift;
    if ( m_bMemoryFilled ) {
        arrBCShift = (Int*)m_pFirstBreak;
    } else {
        arrBCShift = (Int*)_Break( sizeof(Int) << 8 ); // *256
        _Preprocess_BM_QS_BadCharShift( strPattern, iLengthP, arrBCShift );

        m_bMemoryFilled = true;
    }

    // Search phase
    Int iCh = 0;
    Int iEnd = ( (Int)iLengthT - (Int)iLengthP );
    while( iCh <= iEnd ) {
        if ( StringFn->NCmp( strPattern, strText + iCh, iLengthP ) == 0 ) {
            iResult = iCh;
            break;
        }
        iCh += arrBCShift[strText[iCh + iLengthP]];
    }

    // Cleanup memory
    if ( !m_bKeepMemory ) {
        _Free();
        m_bMemoryFilled = false;
    }

    return iResult;
}
UInt PatternMatching::BMTuned( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP )
{
    DebugAssert( iLengthP < iLengthT );

    UInt iResult = INVALID_OFFSET;

    // Preprocess phase
    GChar * pChPad;
    Int * pShiftBC;
    Int * arrBCShift;
    if ( m_bMemoryFilled ) {
        Byte * pBreak = m_pFirstBreak;
        pChPad     = (GChar*)pBreak; pBreak += sizeof(GChar);
        pShiftBC   =  (Int*)pBreak; pBreak += sizeof(Int);
        arrBCShift =  (Int*)pBreak;
    } else {
        pChPad     = (GChar*)_Break( sizeof(GChar) );
        pShiftBC   =  (Int*)_Break( sizeof(Int) );
        arrBCShift =  (Int*)_Break( sizeof(Int) << 8 ); // *256
        _Preprocess_BM_BadCharShift( strPattern, iLengthP, arrBCShift );
        *pChPad = strPattern[iLengthP - 1];
        *pShiftBC = arrBCShift[*pChPad];
        arrBCShift[*pChPad] = 0;

        m_bMemoryFilled = true;
    }

    // Search phase
    Int iIndex, iK;
    Int iCh = 0;
    Int iEnd = (Int)iLengthT;
    --iLengthP; // hack
    while( iCh < iEnd ) {
        iIndex = iCh + iLengthP;
        iK = (iIndex < iEnd) ? arrBCShift[strText[iIndex]] : arrBCShift[*pChPad];
        while(iK != 0) {
            iCh += iK; iIndex = iCh + iLengthP;
            iK = (iIndex < iEnd) ? arrBCShift[strText[iIndex]] : arrBCShift[*pChPad];
            iCh += iK; iIndex = iCh + iLengthP;
            iK = (iIndex < iEnd) ? arrBCShift[strText[iIndex]] : arrBCShift[*pChPad];
            iCh += iK; iIndex = iCh + iLengthP;
            iK = (iIndex < iEnd) ? arrBCShift[strText[iIndex]] : arrBCShift[*pChPad];
        }
        if ( StringFn->NCmp( strPattern, strText + iCh, iLengthP ) == 0 && iCh < (iEnd - (Int)iLengthP) ) {
            iResult = iCh;
            break;
        }
        iCh += *pShiftBC;
    }

    // Cleanup memory
    if ( !m_bKeepMemory ) {
        _Free();
        m_bMemoryFilled = false;
    }

    return iResult;
}
UInt PatternMatching::Smith( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP )
{
    DebugAssert( iLengthP < iLengthT );

    UInt iResult = INVALID_OFFSET;

    // Preprocess phase
    Int * arrBCShift;
    Int * arrQSBCShift;
    UInt iSize = sizeof(Int) << 8; // *256
    if ( m_bMemoryFilled ) {
        arrBCShift   = (Int*)( m_pFirstBreak );
        arrQSBCShift = (Int*)( m_pFirstBreak + iSize );
    } else {
        arrBCShift   = (Int*)_Break( iSize );
        arrQSBCShift = (Int*)_Break( iSize );
        _Preprocess_BM_BadCharShift( strPattern, iLengthP, arrBCShift );
        _Preprocess_BM_QS_BadCharShift( strPattern, iLengthP, arrQSBCShift );

        m_bMemoryFilled = true;
    }

    // Search phase
    Int iCh = 0;
    Int iEnd = ( (Int)iLengthT - (Int)iLengthP );
    while( iCh <= iEnd ) {
        if ( StringFn->NCmp( strPattern, strText + iCh, iLengthP ) == 0 ) {
            iResult = iCh;
            break;
        }
        iCh += Max<Int>( arrBCShift[strText[iCh + iLengthP - 1]], arrQSBCShift[strText[iCh + iLengthP]] );
    }

    // Cleanup memory
    if ( !m_bKeepMemory ) {
        _Free();
        m_bMemoryFilled = false;
    }

    return iResult;
}
UInt PatternMatching::Raita( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP )
{
    DebugAssert( iLengthP < iLengthT );

    UInt iResult = INVALID_OFFSET;

    // Preprocess phase
    const GChar ** ppChSecond;
    GChar * pChFirst;
    GChar * pChMiddle;
    GChar * pChLast;
    Int * arrBCShift;
    if ( m_bMemoryFilled ) {
        Byte * pBreak = m_pFirstBreak;
        ppChSecond = (const GChar**)pBreak; pBreak += sizeof(const GChar*);
        pChFirst   =        (GChar*)pBreak; pBreak += sizeof(GChar);
        pChMiddle  =        (GChar*)pBreak; pBreak += sizeof(GChar);
        pChLast    =        (GChar*)pBreak; pBreak += sizeof(GChar);
        arrBCShift =         (Int*)pBreak;
    } else {
        ppChSecond = (const GChar**)_Break( sizeof(const GChar*) );
        pChFirst   =        (GChar*)_Break( sizeof(GChar) );
        pChMiddle  =        (GChar*)_Break( sizeof(GChar) );
        pChLast    =        (GChar*)_Break( sizeof(GChar) );
        arrBCShift =         (Int*)_Break( sizeof(Int) << 8 ); // *256
        _Preprocess_BM_BadCharShift( strPattern, iLengthP, arrBCShift );
        *ppChSecond = strPattern + 1;
        *pChFirst  = strPattern[0];
        *pChMiddle = strPattern[iLengthP >> 1];
        *pChLast   = strPattern[iLengthP - 1];

        m_bMemoryFilled = true;
    }

    // Search phase
    GChar ch;
    Int iCh = 0;
    Int iEnd = ( (Int)iLengthT - (Int)iLengthP );
    while( iCh <= iEnd ) {
        ch = strText[iCh + iLengthP - 1];
        if ( ( *pChLast == ch ) &&
             ( *pChMiddle == strText[iCh + (iLengthP >> 1)] ) &&
             ( *pChFirst == strText[iCh] ) &&
             ( StringFn->NCmp( *ppChSecond, strText + iCh + 1, iLengthP - 2 ) == 0 ) ) {
            iResult = iCh;
            break;
        }
        iCh += arrBCShift[ch];
    }

    // Cleanup memory
    if ( !m_bKeepMemory ) {
        _Free();
        m_bMemoryFilled = false;
    }

    return iResult;
}
UInt PatternMatching::TwoWay( const GChar * strText, UInt iLengthT, const GChar * strPattern, UInt iLengthP )
{
    DebugAssert( iLengthP < iLengthT );

    UInt iResult = INVALID_OFFSET;

    // Preprocess phase
    Int * pLL;
    Int * pPR;
    if ( m_bMemoryFilled ) {
        pLL = (Int*)( m_pFirstBreak );
        pPR = (Int*)( m_pFirstBreak + sizeof(Int) );
    } else {
        pLL = (Int*)_Break( sizeof(Int) );
        pPR = (Int*)_Break( sizeof(Int) );
        Int i,j,p,q;
        i = _Preprocess_TW_MaxSuffix( strPattern, iLengthP, &p );
        j = _Preprocess_TW_MaxSuffixInv( strPattern, iLengthP, &q );
        if (i > j) {
            *pLL = i;
            *pPR = p;
        } else {
            *pLL = j;
            *pPR = q;
        }

        m_bMemoryFilled = true;
    }

    // Search phase
    Int iMemory;
    Int iIndex;
    Int iCh = 0;
    Int iEnd = ( (Int)iLengthT - (Int)iLengthP );
    if ( StringFn->NCmp( strPattern, strPattern + (*pPR), (*pLL) + 1 ) == 0 ) { // Way 1
        iMemory = -1;
        while( iCh <= iEnd ) {
            iIndex = Max<Int>(*pLL, iMemory) + 1;
            while( iIndex < (Int)iLengthP && strPattern[iIndex] == strText[iCh + iIndex] )
                ++iIndex;
            if (iIndex >= (Int)iLengthP) {
                iIndex = *pLL;
                while( iIndex > iMemory && strPattern[iIndex] == strText[iCh + iIndex] )
                    --iIndex;
                if (iIndex <= iMemory) {
                    iResult = iCh;
                    break;
                }
                iCh += *pPR;
                iMemory = iLengthP - (*pPR) - 1;
            } else {
                iCh += ( iIndex - (*pLL) );
                iMemory = -1;
            }
        }
    } else { // Way 2
        *pPR = Max<Int>( (*pLL) + 1, iLengthP - (*pLL) - 1 ) + 1;
        while( iCh <= iEnd ) {
            iIndex = (*pLL) + 1;
            while( iIndex < (Int)iLengthP && strPattern[iIndex] == strText[iCh + iIndex] )
                ++iIndex;
            if (iIndex >= (Int)iLengthP) {
                iIndex = *pLL;
                while( iIndex >= 0 && strPattern[iIndex] == strText[iCh + iIndex] )
                    --iIndex;
                if (iIndex < 0) {
                    iResult = iCh;
                    break;
                }
                iCh += *pPR;
            } else
                iCh += ( iIndex - (*pLL) );
        }
    }

    // Cleanup memory
    if ( !m_bKeepMemory ) {
        _Free();
        m_bMemoryFilled = false;
    }

    return iResult;
}

/////////////////////////////////////////////////////////////////////////////////

Int PatternMatching::_KMPSkipSearch_Attempt( const GChar * strText, const GChar * strPattern, UInt iLengthP, Int iStart, Int iWall ) const
{
    Int iK = iWall - iStart;
    while( iK < (Int)iLengthP && strPattern[iK] == strText[iK + iStart] )
        ++iK;
    return iK;
}

QWord PatternMatching::_Preprocess_ShiftOr( const GChar * strPattern, UInt iLengthP, QWord * arrShifts )
{
    QWord qwLimit = 0ui64;
    QWord qwBit = 1ui64;
    MemSet64( arrShifts, 0xff, sizeof(QWord) << 8 ); // *256
    const GChar * pP = strPattern;
    const GChar * pPEnd = strPattern + iLengthP;
    while(pP < pPEnd) {
        arrShifts[*pP] &= ~qwBit;
        qwLimit |= qwBit;
        ++pP; qwBit <<= 1;
    }
    return ~(qwLimit >> 1);
}
Void PatternMatching::_Preprocess_BNDD( const GChar * strPattern, UInt iLengthP, QWord * arrBTable )
{
    QWord qwBit = 1ui64;
    MemZero64( arrBTable, sizeof(QWord) << 8 ); // *256
    const GChar * pP = strPattern + iLengthP - 1;
    const GChar * pPEnd = strPattern - 1;
    while(pP > pPEnd) {
        arrBTable[*pP] |= qwBit;
        --pP; qwBit <<= 1;
    }
}

Void PatternMatching::_Preprocess_MP( const GChar * strPattern, UInt iLengthP, Int * arrMPNext )
{
    arrMPNext[0] = -1;
    Int iIndex = -1;
    UInt iCh = 0;
    while( iCh < iLengthP ) {
        while( iIndex > -1 && strPattern[iCh] != strPattern[iIndex] )
            iIndex = arrMPNext[iIndex];
        arrMPNext[++iCh] = ++iIndex;
    }
}
Void PatternMatching::_Preprocess_KMP( const GChar * strPattern, UInt iLengthP, Int * arrKMPNext )
{
    arrKMPNext[0] = -1;
    Int iIndex = -1;
    UInt iCh = 0;
    while( iCh < iLengthP ) {
        while( (iIndex > -1) && (strPattern[iIndex] != strPattern[iCh]) )
            iIndex = arrKMPNext[iIndex];
        ++iIndex;
        ++iCh;
        if (strPattern[iIndex] == strPattern[iCh])
            arrKMPNext[iCh] = arrKMPNext[iIndex];
        else
            arrKMPNext[iCh] = iIndex;
    }
}
Int PatternMatching::_Preprocess_Colussi( const GChar * strPattern, UInt iLengthP, Int * arrHoles, Int * arrNext, Int * arrShift )
{
    // Alloc Temp data
    UInt iSize = (iLengthP + 1) * sizeof(Int);
    Int * arrHolesMax   = (Int*)_Break( iSize );
    Int * arrPeriodsEnd = (Int*)_Break( iSize );
    Int * arrNoHoles    = (Int*)_Break( iSize );
    Int * arrPeriodsMin = (Int*)_Break( iSize );
    iSize = (iSize << 2); // *4

    Int iCh, kH, qH, rP, sH;
    Int iND = 0;

    // Compute arrHolesMax
    iCh = kH = 1;
    do {
        while( strPattern[iCh] == strPattern[iCh - kH] )
            ++iCh;
        arrHolesMax[kH] = iCh;
        qH = kH + 1;
        while( (arrHolesMax[qH - kH] + kH) < iCh ) {
            arrHolesMax[qH] = arrHolesMax[qH - kH] + kH;
            ++qH;
        }
        kH = qH;
        if (kH == iCh + 1)
            iCh = kH;
    } while( kH <= (Int)iLengthP );

    // Compute arrPeriodsEnd
    MemZero( arrPeriodsEnd, iSize );
    for(iCh = (Int)iLengthP; iCh > 0; --iCh) {
        if ( arrHolesMax[iCh] < (Int)iLengthP )
            arrPeriodsEnd[arrHolesMax[iCh]] = iCh;
    }

    // Compute arrPeriodsMin
    rP = 0;
    for(iCh = (Int)iLengthP - 1; iCh >= 0; --iCh) {
        if ( arrHolesMax[iCh + 1] == (Int)iLengthP )
            rP = iCh + 1;
        if ( arrPeriodsEnd[iCh] == 0 )
            arrPeriodsMin[iCh] = rP;
        else
            arrPeriodsMin[iCh] = 0;
    }

    // Compute arrHoles & ND
    sH = -1;
    rP = (Int)iLengthP;
    for (iCh = 0; iCh < (Int)iLengthP; ++iCh) {
        if ( arrPeriodsEnd[iCh] == 0 )
            arrHoles[--rP] = iCh;
        else
            arrHoles[++sH] = iCh;
    }
    iND = sH;

    // Compute Shift
    for (iCh = 0; iCh <= iND; ++iCh)
        arrShift[iCh] = arrPeriodsEnd[arrHoles[iCh]];
    for (iCh = iND + 1; iCh < (Int)iLengthP; ++iCh)
        arrShift[iCh] = arrPeriodsMin[arrHoles[iCh]];
    arrShift[iLengthP] = arrPeriodsMin[0];

    // Compute arrNoHoles
    sH = 0;
    for (iCh = 0; iCh < (Int)iLengthP; ++iCh) {
       arrNoHoles[iCh] = sH;
       if (arrPeriodsEnd[iCh] > 0)
          ++sH;
    }

    // Compute Next
    for (iCh = 0; iCh <= iND; ++iCh)
       arrNext[iCh] = arrNoHoles[ arrHoles[iCh] - arrPeriodsEnd[arrHoles[iCh]] ];
    for (iCh = iND + 1; iCh < (Int)iLengthP; ++iCh)
       arrNext[iCh] = arrNoHoles[ iLengthP - arrPeriodsMin[arrHoles[iCh]] ];
    arrNext[iLengthP] = arrNoHoles[ iLengthP - arrPeriodsMin[arrHoles[iLengthP - 1]] ];

    // Free Temp data
    _Unbreak(iSize);

    return iND;
}

Void PatternMatching::_Preprocess_BM_BadCharShift( const GChar * strPattern, UInt iLengthP, Int * arrBCShift )
{
    UInt i;
    for(i = 0; i < 256; ++i)
        arrBCShift[i] = iLengthP;
    --iLengthP; // hack
    for(i = 0; i < iLengthP; ++i)
        arrBCShift[strPattern[i]] = iLengthP - i;
}
Void PatternMatching::_Preprocess_BM_QS_BadCharShift( const GChar * strPattern, UInt iLengthP, Int * arrBCShift )
{
    UInt i;
    ++iLengthP; // hack
    for(i = 0; i < 256; ++i)
        arrBCShift[i] = iLengthP;
    --iLengthP; // hack
    for(i = 0; i < iLengthP; ++i)
        arrBCShift[strPattern[i]] = iLengthP - i;
}
Void PatternMatching::_Preprocess_BM_Suffixes( const GChar * strPattern, UInt iLengthP, Int * arrSuffixes )
{
    Int iF = 0;
    Int iG = iLengthP - 1;
    arrSuffixes[iG] = iLengthP;
    --iLengthP; // hack
    for(Int i = iLengthP - 1; i >= 0; --i) {
        if ( i > iG && arrSuffixes[i + iLengthP - iF] < (i - iG) )
            arrSuffixes[i] = arrSuffixes[i + iLengthP - iF];
        else {
            if (i < iG)
                iG = i;
            iF = i;
            while( iG >= 0 && strPattern[iG] == strPattern[iG + iLengthP - iF] )
                --iG;
            arrSuffixes[i] = iF - iG;
        }
    }
}
Void PatternMatching::_Preprocess_BM_GoodSuffShift( const GChar * strPattern, UInt iLengthP, Int * arrGSShift, Int * arrSuffixes )
{
    UInt iSize = iLengthP * sizeof(Int);
    Int * arrSuffs;
    if (arrSuffixes == NULL) {
        arrSuffs = (Int*)_Break( iSize );
        _Preprocess_BM_Suffixes( strPattern, iLengthP, arrSuffs );
    } else
        arrSuffs = arrSuffixes;

    Int i, iS = 0;
    for( i = 0; i < (Int)iLengthP; ++i )
        arrGSShift[i] = iLengthP;
    --iLengthP; // hack
    for( i = iLengthP; i >= 0; --i ) {
        if ( arrSuffs[i] == i + 1 ) {
            for( ; iS < ((Int)iLengthP - i); ++iS ) {
                if( arrGSShift[iS] == (Int)(iLengthP + 1) )
                    arrGSShift[iS] = iLengthP - i;
            }
        }
    }
    for(i = 0; i < (Int)iLengthP; ++i)
        arrGSShift[iLengthP - arrSuffs[i]] = iLengthP - i;

    if (arrSuffixes == NULL)
        _Unbreak( iSize );
}
Void PatternMatching::_Preprocess_ReverseColussi( const GChar * strPattern, UInt iLengthP, Int * arrHoles, Int * arrGSShift, Int * arrBCShift )
{
    // Alloc Temp data
    UInt iSize = (iLengthP + 1) * sizeof(Int);
    Int * arrHolesMin   = (Int*)_Break( iSize );
    Int * arrPeriodsEnd = (Int*)_Break( iSize );
    Int * arrPeriodsMin = (Int*)_Break( iSize );
    Int * arrLink       = (Int*)_Break( iSize );
    Int * arrLocc       = (Int*)_Break( (sizeof(Int) << 8) ); // *256
    iSize = (iSize << 2) + (sizeof(Int) << 8); // *4, *256

    Int iIndex, iA, iS, iK, iQ, iR;
    Int iCh;

    // Compute Locc/Link
    MemSet( arrLocc, 0xff, sizeof(Int) << 8 ); // *256
    arrLink[0] = -1;
    for(iIndex = 0; iIndex < (Int)iLengthP - 1; ++iIndex) {
        arrLink[iIndex + 1] = arrLocc[strPattern[iIndex]];
        arrLocc[strPattern[iIndex]] = iIndex;
    }

    // Compute arrBCShift
    for(iA = 0; iA < 256; ++iA) {
        for(iS = 1; iS <= (Int)iLengthP; ++iS) {
            iIndex = arrLocc[iA];
            iCh = arrLink[iLengthP - iS];
            while( iIndex - iCh != iS && iCh >= 0 ) {
                if ( iIndex - iCh > iS )
                    iIndex = arrLink[iIndex + 1];
                else
                    iCh = arrLink[iCh + 1];
            }
            while( iIndex - iCh > iS )
                iIndex = arrLink[iIndex + 1];
            arrBCShift[ ( (iLengthP + 1) * iA ) + iS ] = iLengthP - 1 - iIndex;
        }
    }

    // Compute arrHolesMin
    iK = 1;
    iIndex = iLengthP - 1;
    while( iK <= (Int)iLengthP ) {
        while( iIndex - iK >= 0 && strPattern[iIndex - iK] == strPattern[iIndex] )
            --iIndex;
        arrHolesMin[iK] = iIndex;
        iQ = iK + 1;
        while( arrHolesMin[iQ - iK] - (iQ - iK) > iIndex ) {
            arrHolesMin[iQ] = arrHolesMin[iQ - iK];
            ++iQ;
        }
        iIndex += (iQ - iK);
        iK = iQ;
        if ( iIndex == (Int)iLengthP )
            iIndex = iLengthP - 1;
    }

    // Compute arrPeriodsEnd
    MemZero( arrPeriodsEnd, iLengthP * sizeof(Int) );
    for(iK = iLengthP; iK > 0; --iK)
        arrPeriodsEnd[arrHolesMin[iK]] = iK;

    // Compute arrPeriodsMin
    iR = iLengthP;
    for(iIndex = iLengthP - 1; iIndex >= 0; --iIndex) {
        if (arrHolesMin[iIndex + 1] == iIndex)
            iR = iIndex + 1;
        arrPeriodsMin[iIndex] = iR;
    }

    // Compute arrGSShift and arrHoles
    iIndex = 1;
    for(iK = 1; iK <= (Int)iLengthP; ++iK) {
        if ( arrHolesMin[iK] != ((Int)iLengthP) - 1 && arrPeriodsEnd[arrHolesMin[iK]] == iK ) {
            arrHoles[iIndex] = arrHolesMin[iK];
            arrGSShift[iIndex++] = iK;
        }
    }
    iIndex = iLengthP - 1;
    for(iCh = iLengthP - 2; iCh >= 0; --iCh) {
        if (arrPeriodsEnd[iCh] == 0) {
            arrHoles[iIndex] = iCh;
            arrGSShift[iIndex--] = arrPeriodsMin[iCh];
        }
    }
    arrGSShift[iLengthP] = arrPeriodsMin[0];

    // Free Temp data
    _Unbreak(iSize); // *4
}

Int PatternMatching::_Preprocess_TW_MaxSuffix( const GChar * strPattern, UInt iLengthP, Int * pP )
{
    Int iMS = -1;
    *pP = 1;
    GChar chA, chB;
    Int iCh = 0, iK = 1;
    while( iCh + iK < (Int)iLengthP ) {
        chA = strPattern[iCh + iK];
        chB = strPattern[iMS + iK];
        if (chA < chB) {
            iCh += iK;
            iK = 1;
            *pP = iCh - iMS;
        } else {
            if (chA == chB) {
                if (iK != *pP)
                    ++iK;
                else {
                    iCh += *pP;
                    iK = 1;
                }
            } else { // chA > chB
                iMS = iCh;
                iCh = iMS + 1;
                iK = 1;
                *pP = 1;
            }
        }
    }
    return iMS;
}
Int PatternMatching::_Preprocess_TW_MaxSuffixInv( const GChar * strPattern, UInt iLengthP, Int * pQ )
{
    Int iMS = -1;
    *pQ = 1;
    GChar chA, chB;
    Int iCh = 0, iK = 1;
    while( iCh + iK < (Int)iLengthP ) {
        chA = strPattern[iCh + iK];
        chB = strPattern[iMS + iK];
        if (chA > chB) {
            iCh += iK;
            iK = 1;
            *pQ = iCh - iMS;
        } else {
            if (chA == chB) {
                if (iK != *pQ)
                    ++iK;
                else {
                    iCh += *pQ;
                    iK = 1;
                }
            } else { // chA < chB
                iMS = iCh;
                iCh = iMS + 1;
                iK = 1;
                *pQ = 1;
            }
        }
    }
    return iMS;
}

/////////////////////////////////////////////////////////////////////////////////
// Debug tests
#include "../../ThirdParty/System/System.h"
#include "../Math/Random/Random.h"
Void PatternMatching::StressTest( DebugLogFunction pfLog, PatternMatchingFunction pfPMAlgo,
                                  const GChar * strAlgoName, const GChar * strAlphabet,
                                  UInt iPatternSamples, UInt iTextSamples, Bool bBackWard )
{
    static GChar s_Buffer[4096];
    static GChar * s_pBuffPos;
    static Bool s_bInitRandTimer = true;
    if ( s_bInitRandTimer ) {
        RandFn->XorShift128Seed( (ULong)(SystemFn->TicksAbsolute()) );
        s_bInitRandTimer = false;
    }

    const UInt iPatternMaxLength = 16;
    const UInt iTextMaxLength = 4096;
    GChar strPattern[iPatternMaxLength + 1];
    GChar strText[iTextMaxLength + 1];
    const UInt iAlphabetLength = StringFn->Length( strAlphabet );

    s_pBuffPos = s_Buffer;
    s_pBuffPos = StringFn->Format( s_pBuffPos, TEXT("------- Starting Stress Test for : %s -------"), strAlgoName );
    s_pBuffPos = StringFn->Format( s_pBuffPos, TEXT("\n => Pattern maximum length = %ud"), iPatternMaxLength );
    s_pBuffPos = StringFn->Format( s_pBuffPos, TEXT("\n => Pattern samples = %ud"), iPatternSamples );
    s_pBuffPos = StringFn->Format( s_pBuffPos, TEXT("\n => Text maximum length = %ud"), iTextMaxLength );
    s_pBuffPos = StringFn->Format( s_pBuffPos, TEXT("\n => Text samples = %ud"), iTextSamples );
    s_pBuffPos = StringFn->Format( s_pBuffPos, TEXT("\n => Alphabet string is '%s'"), strAlphabet );
    s_pBuffPos = StringFn->Format( s_pBuffPos, TEXT("\n => Starting test ...")  );
    s_pBuffPos = StringFn->Format( s_pBuffPos, TEXT("\n\n") );
    pfLog(s_Buffer);

    // Stats
    UInt iMatchesCount = 0, iErrors = 0;
    UInt iLengthP[3] = { UINT_MAX, 0, 0 }; // Min Max Avg
    UInt iLengthT[3] = { UINT_MAX, 0, 0 }; // Min Max Avg
    Double fPreProcTime[3] = { DOUBLE_MAX, -1.0, 0.0 }; // Min Max Avg
    Double fSearchTimeF[3] = { DOUBLE_MAX, -1.0, 0.0 }; // Min Max Avg
    Double fSearchTimeNF[3] = { DOUBLE_MAX, -1.0, 0.0 }; // Min Max Avg

    // Main Loop
    UInt i, j, k, iPatternLen, iTextLen;
    UInt iMatchREF, iMatchTEST;
    Double fTimeStart, fTimeElapsed;
    for(i = 0; i < iPatternSamples; ++i) {
        // Generate random pattern
        iPatternLen = ( RandFn->XorShift128() % (iPatternMaxLength - 1) ) + 2;
        iLengthP[2] += iPatternLen;
        if (iPatternLen < iLengthP[0])
            iLengthP[0] = iPatternLen;
        if (iPatternLen > iLengthP[1])
            iLengthP[1] = iPatternLen;
        for(k = 0; k < iPatternLen; ++k) {
            strPattern[k] = strAlphabet[RandFn->XorShift128() % iAlphabetLength];
        }
        strPattern[k] = NULLBYTE;

        MatchFn->EnterChain();

        // Warm-up
        fTimeStart = SystemFn->TimeAbsolute( TIMEUNIT_MICROSECONDS );
        (MatchFn->*pfPMAlgo)( TEXT(""), 0, strPattern, iPatternLen );
        fTimeElapsed = SystemFn->TimeRelative( fTimeStart, TIMEUNIT_MICROSECONDS );

        // Update stats
        fPreProcTime[2] += fTimeElapsed;
        if (fTimeElapsed < fPreProcTime[0])
            fPreProcTime[0] = fTimeElapsed;
        if (fTimeElapsed > fPreProcTime[1])
            fPreProcTime[1] = fTimeElapsed;

        // Sub Loop
        for(j = 0; j < iTextSamples; ++j) {
            // Generate random text
            iTextLen = ( RandFn->XorShift128() % (iTextMaxLength - 1) ) + 2;
            iLengthT[2] += iTextLen;
            if (iTextLen < iLengthT[0])
                iLengthT[0] = iTextLen;
            if (iTextLen > iLengthT[1])
                iLengthT[1] = iTextLen;
            for(k = 0; k < iTextLen; ++k) {
                strText[k] = strAlphabet[RandFn->XorShift128() % iAlphabetLength];
            }
            strText[k] = NULLBYTE;

            // Reference is BruteForce
            if (bBackWard)
                iMatchREF = MatchFn->BackwardBruteForce(strText, iTextLen, strPattern, iPatternLen);
            else
                iMatchREF = MatchFn->BruteForce(strText, iTextLen, strPattern, iPatternLen);

            // Launch !
            fTimeStart = SystemFn->TimeAbsolute( TIMEUNIT_MICROSECONDS );
            iMatchTEST = (MatchFn->*pfPMAlgo)(strText, iTextLen, strPattern, iPatternLen);
            fTimeElapsed = SystemFn->TimeRelative( fTimeStart, TIMEUNIT_MICROSECONDS );

            // Log error (should happen no more)
            if (iMatchREF != iMatchTEST) {
                s_pBuffPos = s_Buffer;
                s_pBuffPos = StringFn->Format( s_pBuffPos, TEXT("INCORRECT RESULT :") );
                s_pBuffPos = StringFn->Format( s_pBuffPos, TEXT("\n - Pattern = '%s'"), strPattern );
                s_pBuffPos = StringFn->Format( s_pBuffPos, TEXT("\n - Pattern Length = %ud"), iPatternLen );
                s_pBuffPos = StringFn->Format( s_pBuffPos, TEXT("\n - Text = '%s'"), strText );
                s_pBuffPos = StringFn->Format( s_pBuffPos, TEXT("\n - Text Length = %ud"), iTextLen );
                s_pBuffPos = StringFn->Format( s_pBuffPos, TEXT("\n - Reference result = %ud"), iMatchREF );
                s_pBuffPos = StringFn->Format( s_pBuffPos, TEXT("\n - Algorithm result = %ud"), iMatchTEST );
                s_pBuffPos = StringFn->Format( s_pBuffPos, TEXT("\n\n") );
                pfLog(s_Buffer);
                ++iErrors;
                continue; // don't stat error cases
            }

            // Update stats
            if (iMatchTEST != INVALID_OFFSET) {
                fSearchTimeF[2] += fTimeElapsed;
                if (fTimeElapsed < fSearchTimeF[0])
                    fSearchTimeF[0] = fTimeElapsed;
                if (fTimeElapsed > fSearchTimeF[1])
                    fSearchTimeF[1] = fTimeElapsed;
                ++iMatchesCount;
            } else {
                fSearchTimeNF[2] += fTimeElapsed;
                if (fTimeElapsed < fSearchTimeNF[0])
                    fSearchTimeNF[0] = fTimeElapsed;
                if (fTimeElapsed > fSearchTimeNF[1])
                    fSearchTimeNF[1] = fTimeElapsed;
            }
        }

        MatchFn->LeaveChain();
    }
    fSearchTimeF[2] /= (Double)( iPatternSamples * iTextSamples );
    fSearchTimeNF[2] /= (Double)( iPatternSamples * iTextSamples );
    fPreProcTime[2] /= (Double)iPatternSamples;
    iLengthT[2] /= ( iPatternSamples * iTextSamples );
    iLengthP[2] /= iPatternSamples;

    s_pBuffPos = s_Buffer;
    s_pBuffPos = StringFn->Format( s_pBuffPos, TEXT(" => ... test Finished !") );
    s_pBuffPos = StringFn->Format( s_pBuffPos, TEXT("\n => Running times (in microseconds) :") );
    s_pBuffPos = StringFn->Format( s_pBuffPos, TEXT("\n    - Preprocess         = Min(%f) / Max(%f) / Avg(%f)"), fPreProcTime[0], fPreProcTime[1], fPreProcTime[2] );
    s_pBuffPos = StringFn->Format( s_pBuffPos, TEXT("\n    - Search (found)     = Min(%f) / Max(%f) / Avg(%f)"), fSearchTimeF[0], fSearchTimeF[1], fSearchTimeF[2] );
    s_pBuffPos = StringFn->Format( s_pBuffPos, TEXT("\n    - Search (not found) = Min(%f) / Max(%f) / Avg(%f)"), fSearchTimeNF[0], fSearchTimeNF[1], fSearchTimeNF[2] );
    s_pBuffPos = StringFn->Format( s_pBuffPos, TEXT("\n => Generated Lengths :") );
    s_pBuffPos = StringFn->Format( s_pBuffPos, TEXT("\n    - Pattern Lengths    = Min(%ud) / Max(%ud) / Avg(%ud)"), iLengthP[0], iLengthP[1], iLengthP[2] );
    s_pBuffPos = StringFn->Format( s_pBuffPos, TEXT("\n    - Text Lengths       = Min(%ud) / Max(%ud) / Avg(%ud)"), iLengthT[0], iLengthT[1], iLengthT[2] );
    s_pBuffPos = StringFn->Format( s_pBuffPos, TEXT("\n => Generated %ud matches over %ud samples"), iMatchesCount, iPatternSamples * iTextSamples );
    s_pBuffPos = StringFn->Format( s_pBuffPos, TEXT("\n => Total incorrect results = %ud"), iErrors );
    s_pBuffPos = StringFn->Format( s_pBuffPos, TEXT("\n------- End of Stress Test for : %s -------"), strAlgoName );
    s_pBuffPos = StringFn->Format( s_pBuffPos, TEXT("\n\n\n\n") );
    pfLog(s_Buffer);
}
Void PatternMatching::StressTestAll( DebugLogFunction pfLog, const GChar * strAlphabet, UInt iPatternSamples, UInt iTextSamples )
{
    StressTest( pfLog, &PatternMatching::NotSoNaive,           TEXT("NotSoNaive"),           strAlphabet, iPatternSamples, iTextSamples );
    StressTest( pfLog, &PatternMatching::KarpRabin,            TEXT("KarpRabin"),            strAlphabet, iPatternSamples, iTextSamples );
    StressTest( pfLog, &PatternMatching::ShiftOr,              TEXT("ShiftOr"),              strAlphabet, iPatternSamples, iTextSamples );
    StressTest( pfLog, &PatternMatching::BackwardNDDawg,       TEXT("BackwardNDDawg"),       strAlphabet, iPatternSamples, iTextSamples, true );
    StressTest( pfLog, &PatternMatching::MorrisPratt,          TEXT("MorrisPratt"),          strAlphabet, iPatternSamples, iTextSamples );
    StressTest( pfLog, &PatternMatching::KnuthMorrisPratt,     TEXT("KnuthMorrisPratt"),     strAlphabet, iPatternSamples, iTextSamples );
    StressTest( pfLog, &PatternMatching::Colussi,              TEXT("Colussi"),              strAlphabet, iPatternSamples, iTextSamples );
    StressTest( pfLog, &PatternMatching::GalilGiancarlo,       TEXT("GalilGiancarlo"),       strAlphabet, iPatternSamples, iTextSamples );
    StressTest( pfLog, &PatternMatching::ApostolicoCrochemore, TEXT("ApostolicoCrochemore"), strAlphabet, iPatternSamples, iTextSamples );
    StressTest( pfLog, &PatternMatching::KMPSkipSearch,        TEXT("KMPSkipSearch"),        strAlphabet, iPatternSamples, iTextSamples );
    StressTest( pfLog, &PatternMatching::BoyerMoore,           TEXT("BoyerMoore"),           strAlphabet, iPatternSamples, iTextSamples );
    StressTest( pfLog, &PatternMatching::BMTurbo,              TEXT("BMTurbo"),              strAlphabet, iPatternSamples, iTextSamples );
    StressTest( pfLog, &PatternMatching::ApostolicoGiancarlo,  TEXT("ApostolicoGiancarlo"),  strAlphabet, iPatternSamples, iTextSamples );
    StressTest( pfLog, &PatternMatching::ReverseColussi,       TEXT("ReverseColussi"),       strAlphabet, iPatternSamples, iTextSamples );
    StressTest( pfLog, &PatternMatching::HorsPool,             TEXT("HorsPool"),             strAlphabet, iPatternSamples, iTextSamples );
    StressTest( pfLog, &PatternMatching::QuickSearch,          TEXT("QuickSearch"),          strAlphabet, iPatternSamples, iTextSamples );
    StressTest( pfLog, &PatternMatching::BMTuned,              TEXT("BMTuned"),              strAlphabet, iPatternSamples, iTextSamples );
    StressTest( pfLog, &PatternMatching::Smith,                TEXT("Smith"),                strAlphabet, iPatternSamples, iTextSamples );
    StressTest( pfLog, &PatternMatching::Raita,                TEXT("Raita"),                strAlphabet, iPatternSamples, iTextSamples );
    StressTest( pfLog, &PatternMatching::TwoWay,               TEXT("TwoWay"),               strAlphabet, iPatternSamples, iTextSamples );
}

