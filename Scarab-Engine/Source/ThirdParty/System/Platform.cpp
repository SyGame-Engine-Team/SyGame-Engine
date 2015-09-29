/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/System/Platform.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Platform independance standard definitions
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Third-Party Includes
#define WIN32_LEAN_AND_MEAN
#include <windows.h> // for OutputDebugStringW only

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Platform.h"

/////////////////////////////////////////////////////////////////////////////////
// Debug basic support
const char * _DebugAssert_UIntToStr( UInt iValue )
{
    static char s_strLineNumber[8];

    // Zero case
    if ( iValue == 0 ) {
        s_strLineNumber[0] = L'0';
        s_strLineNumber[1] = L'\0';
        return s_strLineNumber;
    }

    // Print digits
    char * pDigit = s_strLineNumber;
    UInt iDigit = 0;
    while( iValue != 0 ) {
        iDigit = iValue % 10;
        *pDigit++ = ( '0' + (char)iDigit );
        iValue -= iDigit;
        iValue /= 10;
    }
    *pDigit = L'\0';

    // Reverse digits
    char ch;
    char * strStart = s_strLineNumber;
    char * strEnd = pDigit - 1;
    while( strStart < strEnd ) {
        ch = *strStart;
        *strStart = *strEnd;
        *strEnd = ch;
        ++strStart; --strEnd;
    }

    // Done
    return s_strLineNumber;
}
void _DebugAssert_OutputDebugString( const char * strExpr, const char * strFilename, UInt iLine )
{
    static const char * s_strPart0 = "\nFatal Assert failed (low level) :\n\t Expr = ";
    static const char * s_strPart1 = "\n\t File = ";
    static const char * s_strPart2 = "\n\t Line = ";
    static wchar_t s_strMessage[1024];

    const char * strLineNumber = _DebugAssert_UIntToStr( iLine );

    wchar_t * pCh = s_strMessage;
    const char * pSrcCh;

    pSrcCh = s_strPart0;
    while( *pSrcCh != L'\0' )
        *pCh++ = (wchar_t)( *pSrcCh++ );
    pSrcCh = strExpr;
    while( *pSrcCh != L'\0' )
        *pCh++ = (wchar_t)( *pSrcCh++ );
    pSrcCh = s_strPart1;
    while( *pSrcCh != L'\0' )
        *pCh++ = (wchar_t)( *pSrcCh++ );
    pSrcCh = strFilename;
    while( *pSrcCh != L'\0' )
        *pCh++ = (wchar_t)( *pSrcCh++ );
    pSrcCh = s_strPart2;
    while( *pSrcCh != L'\0' )
        *pCh++ = (wchar_t)( *pSrcCh++ );
    pSrcCh = strLineNumber;
    while( *pSrcCh != L'\0' )
        *pCh++ = (wchar_t)( *pSrcCh++ );
    *pCh = L'\0';

    OutputDebugStringW( s_strMessage );
}

/////////////////////////////////////////////////////////////////////////////////
// Bit-wise functions
const Byte _sg_arrByteMirrors[256] = {
	0x00,0x80,0x40,0xC0,0x20,0xA0,0x60,0xE0, 0x10,0x90,0x50,0xD0,0x30,0xB0,0x70,0xF0,
	0x08,0x88,0x48,0xC8,0x28,0xA8,0x68,0xE8, 0x18,0x98,0x58,0xD8,0x38,0xB8,0x78,0xF8,
	0x04,0x84,0x44,0xC4,0x24,0xA4,0x64,0xE4, 0x14,0x94,0x54,0xD4,0x34,0xB4,0x74,0xF4,
	0x0C,0x8C,0x4C,0xCC,0x2C,0xAC,0x6C,0xEC, 0x1C,0x9C,0x5C,0xDC,0x3C,0xBC,0x7C,0xFC,
	0x02,0x82,0x42,0xC2,0x22,0xA2,0x62,0xE2, 0x12,0x92,0x52,0xD2,0x32,0xB2,0x72,0xF2,
	0x0A,0x8A,0x4A,0xCA,0x2A,0xAA,0x6A,0xEA, 0x1A,0x9A,0x5A,0xDA,0x3A,0xBA,0x7A,0xFA,
	0x06,0x86,0x46,0xC6,0x26,0xA6,0x66,0xE6, 0x16,0x96,0x56,0xD6,0x36,0xB6,0x76,0xF6,
	0x0E,0x8E,0x4E,0xCE,0x2E,0xAE,0x6E,0xEE, 0x1E,0x9E,0x5E,0xDE,0x3E,0xBE,0x7E,0xFE,
	0x01,0x81,0x41,0xC1,0x21,0xA1,0x61,0xE1, 0x11,0x91,0x51,0xD1,0x31,0xB1,0x71,0xF1,
	0x09,0x89,0x49,0xC9,0x29,0xA9,0x69,0xE9, 0x19,0x99,0x59,0xD9,0x39,0xB9,0x79,0xF9,
	0x05,0x85,0x45,0xC5,0x25,0xA5,0x65,0xE5, 0x15,0x95,0x55,0xD5,0x35,0xB5,0x75,0xF5,
	0x0D,0x8D,0x4D,0xCD,0x2D,0xAD,0x6D,0xED, 0x1D,0x9D,0x5D,0xDD,0x3D,0xBD,0x7D,0xFD,
	0x03,0x83,0x43,0xC3,0x23,0xA3,0x63,0xE3, 0x13,0x93,0x53,0xD3,0x33,0xB3,0x73,0xF3,
	0x0B,0x8B,0x4B,0xCB,0x2B,0xAB,0x6B,0xEB, 0x1B,0x9B,0x5B,0xDB,0x3B,0xBB,0x7B,0xFB,
	0x07,0x87,0x47,0xC7,0x27,0xA7,0x67,0xE7, 0x17,0x97,0x57,0xD7,0x37,0xB7,0x77,0xF7,
	0x0F,0x8F,0x4F,0xCF,0x2F,0xAF,0x6F,0xEF, 0x1F,0x9F,0x5F,0xDF,0x3F,0xBF,0x7F,0xFF
};

/////////////////////////////////////////////////////////////////////////////////
// Memory functions
Void MemCopy8(Void * pTo, const Void * pFrom, UChar size)
{
    if (size == 0u)
        return;

    const Byte * pByteFrom = (const Byte*)pFrom;
    Byte * pByteTo = (Byte*)pTo;
    Byte * pByteEnd = pByteTo + size;
    while(pByteTo < pByteEnd) {
		*pByteTo = *pByteFrom;
		++pByteTo; ++pByteFrom;
	}
}
Void MemCopy16(Void * pTo, const Void * pFrom, UShort size)
{
    if (size == 0u)
        return;

    const Word * pWFrom = (const Word*)pFrom;
    Word * pWTo = (Word*)pTo;
    Word * pWEnd = pWTo + (size >> 1); // /2
    while(pWTo < pWEnd) {
		*pWTo = *pWFrom;
		++pWTo; ++pWFrom;
    }

    const Byte * pByteFrom = (const Byte*)pWFrom;
    Byte * pByteTo = (Byte*)pWTo;
    Byte * pByteEnd = pByteTo + (size & 1u); // %2
    while(pByteTo < pByteEnd) {
		*pByteTo = *pByteFrom;
		++pByteTo; ++pByteFrom;
	}
}
Void MemCopy32(Void * pTo, const Void * pFrom, ULong size)
{
    if (size == 0ul)
        return;

    const DWord * pDWFrom = (const DWord*)pFrom;
    DWord * pDWTo = (DWord*)pTo;
    DWord * pDWEnd = pDWTo + (size >> 2); // /4
    while(pDWTo < pDWEnd) {
		*pDWTo = *pDWFrom;
		++pDWTo; ++pDWFrom;
    }

    const Byte * pByteFrom = (const Byte*)pDWFrom;
    Byte * pByteTo = (Byte*)pDWTo;
    Byte * pByteEnd = pByteTo + (size & 3ul); // %4
    while(pByteTo < pByteEnd) {
		*pByteTo = *pByteFrom;
		++pByteTo; ++pByteFrom;
	}
}
Void MemCopy64(Void * pTo, const Void * pFrom, UInt64 size)
{
    if (size == 0ui64)
        return;

    const QWord * pQWFrom = (const QWord*)pFrom;
    QWord * pQWTo = (QWord*)pTo;
    QWord * pQWEnd = pQWTo + (size >> 3); // /8
    while(pQWTo < pQWEnd) {
		*pQWTo = *pQWFrom;
		++pQWTo; ++pQWFrom;
    }

    const Byte * pByteFrom = (const Byte*)pQWFrom;
    Byte * pByteTo = (Byte*)pQWTo;
    Byte * pByteEnd = pByteTo + (size & 7ui64); // %8
    while(pByteTo < pByteEnd) {
		*pByteTo = *pByteFrom;
		++pByteTo; ++pByteFrom;
	}
}

Void MemCopyBackward8(Void * pTo, const Void * pFrom, UChar size)
{
    if (size == 0u)
        return;

    const Byte * pByteFrom = ( (const Byte*)pFrom ) + size - 1;
    Byte * pByteTo = ( (Byte*)pTo ) + size - 1;
    Byte * pByteEnd = pByteTo - size;
    while(pByteTo > pByteEnd) {
		*pByteTo = *pByteFrom;
		--pByteTo; --pByteFrom;
	}
}
Void MemCopyBackward16(Void * pTo, const Void * pFrom, UShort size)
{
    if (size == 0u)
        return;

    const Byte * pByteFrom = ( (const Byte*)pFrom ) + size - 1;
    Byte * pByteTo = ( (Byte*)pTo ) + size - 1;
    Byte * pByteEnd = pByteTo - (size & 1ul); // %2
    while(pByteTo > pByteEnd) {
		*pByteTo = *pByteFrom;
		--pByteTo; --pByteFrom;
	}

    const Word * pWFrom = ( (const Word*)(pByteFrom + 1) ) - 1;
    Word * pWTo = ( (Word*)(pByteTo + 1) ) - 1;
    Word * pWEnd = pWTo - (size >> 1); // /2
    while(pWTo > pWEnd) {
		*pWTo = *pWFrom;
		--pWTo; --pWFrom;
    }
}
Void MemCopyBackward32(Void * pTo, const Void * pFrom, ULong size)
{
    if (size == 0ul)
        return;

    const Byte * pByteFrom = ( (const Byte*)pFrom ) + size - 1;
    Byte * pByteTo = ( (Byte*)pTo ) + size - 1;
    Byte * pByteEnd = pByteTo - (size & 3ul); // %4
    while(pByteTo > pByteEnd) {
		*pByteTo = *pByteFrom;
		--pByteTo; --pByteFrom;
	}

    const DWord * pDWFrom = ( (const DWord*)(pByteFrom + 1) ) - 1;
    DWord * pDWTo = ( (DWord*)(pByteTo + 1) ) - 1;
    DWord * pDWEnd = pDWTo - (size >> 2); // /4
    while(pDWTo > pDWEnd) {
		*pDWTo = *pDWFrom;
		--pDWTo; --pDWFrom;
    }
}
Void MemCopyBackward64(Void * pTo, const Void * pFrom, UInt64 size)
{
    if (size == 0ui64)
        return;

    const Byte * pByteFrom = ( (const Byte*)pFrom ) + size - 1;
    Byte * pByteTo = ( (Byte*)pTo ) + size - 1;
    Byte * pByteEnd = pByteTo - (size & 7ui64); // %8
    while(pByteTo > pByteEnd) {
		*pByteTo = *pByteFrom;
		--pByteTo; --pByteFrom;
	}

    const QWord * pQWFrom = ( (const QWord*)(pByteFrom + 1) ) - 1;
    QWord * pQWTo = ( (QWord*)(pByteTo + 1) ) - 1;
    QWord * pQWEnd = pQWTo - (size >> 3); // /8
    while(pQWTo > pQWEnd) {
		*pQWTo = *pQWFrom;
		--pQWTo; --pQWFrom;
    }
}

Void MemSet8(Void * ptr, Byte value, UChar size)
{
    if (size == 0u)
        return;

    Byte * pByteTo = (Byte*)ptr;
    Byte * pByteEnd = pByteTo + size;
    while(pByteTo < pByteEnd) {
		*pByteTo = value;
		++pByteTo;
	}
}
Void MemSet16(Void * ptr, Byte value, UShort size)
{
    if (size == 0u)
        return;

    Word wValue = (Word)value;
    wValue |= (wValue << 8);

    Word * pWTo = (Word*)ptr;
    Word * pWEnd = pWTo + (size >> 1); // /2
    while(pWTo < pWEnd) {
		*pWTo = wValue;
		++pWTo;
    }

    Byte * pByteTo = (Byte*)pWTo;
    Byte * pByteEnd = pByteTo + (size & 1u); // %2
    while(pByteTo < pByteEnd) {
		*pByteTo = value;
		++pByteTo;
	}
}
Void MemSet32(Void * ptr, Byte value, ULong size)
{
    if (size == 0ul)
        return;

    DWord dwValue = (DWord)value;
    dwValue |= ( (dwValue << 24) | (dwValue << 16) | (dwValue <<  8) );

    DWord * pDWTo = (DWord*)ptr;
    DWord * pDWEnd = pDWTo + (size >> 2); // /4
    while(pDWTo < pDWEnd) {
		*pDWTo = dwValue;
		++pDWTo;
    }

    Byte * pByteTo = (Byte*)pDWTo;
    Byte * pByteEnd = pByteTo + (size & 3ul); // %4
    while(pByteTo < pByteEnd) {
		*pByteTo = value;
		++pByteTo;
	}
}
Void MemSet64(Void * ptr, Byte value, UInt64 size)
{
    if (size == 0ui64)
        return;

    QWord qwValue = (QWord)value;
    qwValue |= ( (qwValue << 56) | (qwValue << 48) | (qwValue << 40) | (qwValue << 32) |
                 (qwValue << 24) | (qwValue << 16) | (qwValue <<  8) );

    QWord * pQWTo = (QWord*)ptr;
    QWord * pQWEnd = pQWTo + (size >> 3); // /8
    while(pQWTo < pQWEnd) {
		*pQWTo = qwValue;
		++pQWTo;
    }

    Byte * pByteTo = (Byte*)pQWTo;
    Byte * pByteEnd = pByteTo + (size & 7ui64); // %8
    while(pByteTo < pByteEnd) {
		*pByteTo = value;
		++pByteTo;
	}
}

Void MemZero8(Void * ptr, UChar size)
{
    if (size == 0u)
        return;

    Byte * pByteTo = (Byte*)ptr;
    Byte * pByteEnd = pByteTo + size;
    while(pByteTo < pByteEnd) {
		*pByteTo = 0u;
		++pByteTo;
	}
}
Void MemZero16(Void * ptr, UShort size)
{
    if (size == 0u)
        return;

    Word * pWTo = (Word*)ptr;
    Word * pWEnd = pWTo + (size >> 1); // /2
    while(pWTo < pWEnd) {
		*pWTo = 0u;
		++pWTo;
    }

    Byte * pByteTo = (Byte*)pWTo;
    Byte * pByteEnd = pByteTo + (size & 1u); // %2
    while(pByteTo < pByteEnd) {
		*pByteTo = 0u;
		++pByteTo;
	}
}
Void MemZero32(Void * ptr, ULong size)
{
    if (size == 0ul)
        return;

    DWord * pDWTo = (DWord*)ptr;
    DWord * pDWEnd = pDWTo + (size >> 2); // /4
    while(pDWTo < pDWEnd) {
		*pDWTo = 0ul;
		++pDWTo;
    }

    Byte * pByteTo = (Byte*)pDWTo;
    Byte * pByteEnd = pByteTo + (size & 3ul); // %4
    while(pByteTo < pByteEnd) {
		*pByteTo = 0u;
		++pByteTo;
	}
}
Void MemZero64(Void * ptr, UInt64 size)
{
    if (size == 0ui64)
        return;

    QWord * pQWTo = (QWord*)ptr;
    QWord * pQWEnd = pQWTo + (size >> 3); // /8
    while(pQWTo < pQWEnd) {
		*pQWTo = 0ui64;
		++pQWTo;
    }

    Byte * pByteTo = (Byte*)pQWTo;
    Byte * pByteEnd = pByteTo + (size & 7ui64); // %8
    while(pByteTo < pByteEnd) {
		*pByteTo = 0u;
		++pByteTo;
	}
}

Int MemCmp8(const Void * pLeft, const Void * pRight, UChar size)
{
    if (size == 0u)
        return 0;

    const Byte * pByteRight = (const Byte*)pRight;
    Byte * pByteLeft = (Byte*)pLeft;
    Byte * pByteEnd = pByteLeft + size;
    while(pByteLeft < pByteEnd) {
		if ( *pByteLeft < *pByteRight )
            return +1;
        if ( *pByteLeft > *pByteRight )
            return -1;
		++pByteLeft; ++pByteRight;
	}

    return 0;
}
Int MemCmp16(const Void * pLeft, const Void * pRight, UShort size)
{
    if (size == 0u)
        return 0;

    const Word * pWRight = (const Word*)pRight;
    const Word * pWLeft = (Word*)pLeft;
    const Word * pWEnd = pWLeft + (size >> 1); // /2
    while(pWLeft < pWEnd) {
		if ( *pWLeft < *pWRight )
            return +1;
        if ( *pWLeft > *pWRight )
            return -1;
		++pWLeft; ++pWRight;
    }

    const Byte * pByteRight = (const Byte*)pWRight;
    Byte * pByteLeft = (Byte*)pWLeft;
    Byte * pByteEnd = pByteLeft + (size & 1u); // %2
    while(pByteLeft < pByteEnd) {
		if ( *pByteLeft < *pByteRight )
            return +1;
        if ( *pByteLeft > *pByteRight )
            return -1;
		++pByteLeft; ++pByteRight;
	}

    return 0;
}
Int MemCmp32(const Void * pLeft, const Void * pRight, ULong size)
{
    if (size == 0ul)
        return 0;

    const DWord * pDWRight = (const DWord*)pRight;
    const DWord * pDWLeft = (DWord*)pLeft;
    const DWord * pDWEnd = pDWLeft + (size >> 2); // /4
    while(pDWLeft < pDWEnd) {
		if ( *pDWLeft < *pDWRight )
            return +1;
        if ( *pDWLeft > *pDWRight )
            return -1;
		++pDWLeft; ++pDWRight;
    }

    const Byte * pByteRight = (const Byte*)pDWRight;
    Byte * pByteLeft = (Byte*)pDWLeft;
    Byte * pByteEnd = pByteLeft + (size & 3ul); // %4
    while(pByteLeft < pByteEnd) {
		if ( *pByteLeft < *pByteRight )
            return +1;
        if ( *pByteLeft > *pByteRight )
            return -1;
		++pByteLeft; ++pByteRight;
	}

    return 0;
}
Int MemCmp64(const Void * pLeft, const Void * pRight, UInt64 size)
{
    if (size == 0ui64)
        return 0;

    const QWord * pQWRight = (const QWord*)pRight;
    const QWord * pQWLeft = (QWord*)pLeft;
    const QWord * pQWEnd = pQWLeft + (size >> 3); // /8
    while(pQWLeft < pQWEnd) {
		if ( *pQWLeft < *pQWRight )
            return +1;
        if ( *pQWLeft > *pQWRight )
            return -1;
		++pQWLeft; ++pQWRight;
    }

    const Byte * pByteRight = (const Byte*)pQWRight;
    Byte * pByteLeft = (Byte*)pQWLeft;
    Byte * pByteEnd = pByteLeft + (size & 7ui64); // %8
    while(pByteLeft < pByteEnd) {
		if ( *pByteLeft < *pByteRight )
            return +1;
        if ( *pByteLeft > *pByteRight )
            return -1;
		++pByteLeft; ++pByteRight;
	}

    return 0;
}

/////////////////////////////////////////////////////////////////////////////////
// PixelFormat definitions
UInt PixelFormatBits( PixelFormat iFormat )
{
    static UInt s_arrPixelFormatBits[PIXEL_FMT_COUNT] = {
        0,
        128, 128, 128, 128,
        96, 96, 96, 96,
        64, 64, 64, 64, 64, 64,
        64, 64, 64, 64,
        32, 32, 32, 32, 32, 32,
        32, 32, 32,
        32, 32, 32,
        32, 32, 32,
        32, 32, 32, 32, 32, 32,
        32, 32, 32, 32,
        16,
        16,
        16, 16, 16, 16, 16,
        16, 16, 16, 16, 16, 16,
        8, 8, 8, 8, 8,
        8,
        1,
        64,
        32,
        32,
        16,
        64, 64, 64,
        32, 32, 32,
        32,
        32,
        32,
        32,
        32,
        8, 16,
        16,
        4, 4, 4,
        8, 8, 8,
        8, 8, 8,
        4, 4, 4,
        8, 8, 8,
        8, 8, 8,
        8, 8, 8,
        INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, //32, 32, 64, 12, 20, 32,
        INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, //16, 32, 32, 12, 32, 32,
        INVALID_OFFSET //16
    };
    return s_arrPixelFormatBits[iFormat];
}

UInt PixelFormatChannelCount( PixelFormat iFormat )
{
    static UInt s_arrPixelFormatChannelCount[PIXEL_FMT_COUNT] = {
        0,
        4, 4, 4, 4,
        3, 3, 3, 3,
        4, 4, 4, 4, 4, 4,
        2, 2, 2, 2,
        4, 4, 4, 4, 4, 4,
        4, 4, 4,
        3, 3, 3,
        4, 4, 4,
        2, 2, 2, 2, 2, 2,
        1, 1, 1, 1,
        4,
        3,
        2, 2, 2, 2, 2,
        1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1,
        1,
        1,
        2,
        2,
        1,
        1,
        2, 1, 1,
        2, 1, 1,
        4,
        4,
        4,
        3,
        4,
        1, 2,
        4,
        4, 4, 4,
        4, 4, 4,
        4, 4, 4,
        1, 1, 1,
        2, 2, 2,
        3, 3, 3,
        4, 4, 4,
        INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, //3, 3, 3, 3, 3, 3,
        INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, //3, 3, 3, 3, 3, 3,
        INVALID_OFFSET //3
    };
    return s_arrPixelFormatChannelCount[iFormat];
}
UInt PixelFormatChannelBits( PixelFormat iFormat, UInt iChannel )
{
    static UInt s_arrPixelFormatChannelBits[PIXEL_FMT_COUNT][4] = {
        { 0, 0, 0, 0 },
        { 32, 32, 32, 32 }, { 32, 32, 32, 32 }, { 32, 32, 32, 32 }, { 32, 32, 32, 32 },
        { 32, 32, 32,  0 }, { 32, 32, 32,  0 }, { 32, 32, 32,  0 }, { 32, 32, 32,  0 },
        { 16, 16, 16, 16 }, { 16, 16, 16, 16 }, { 16, 16, 16, 16 }, { 16, 16, 16, 16 }, { 16, 16, 16, 16 }, { 16, 16, 16, 16 },
        { 32, 32,  0,  0 }, { 32, 32,  0,  0 }, { 32, 32,  0,  0 }, { 32, 32,  0,  0 },
        {  8,  8,  8,  8 }, {  8,  8,  8,  8 }, {  8,  8,  8,  8 }, {  8,  8,  8,  8 }, {  8,  8,  8,  8 }, {  8,  8,  8,  8 },
        {  8,  8,  8,  8 }, {  8,  8,  8,  8 }, {  8,  8,  8,  8 },
        {  8,  8,  8,  0 }, {  8,  8,  8,  0 }, {  8,  8,  8,  0 },
        { 10, 10, 10,  2 }, { 10, 10, 10,  2 }, { 10, 10, 10,  2 },
        { 16, 16,  0,  0 }, { 16, 16,  0,  0 }, { 16, 16,  0,  0 }, { 16, 16,  0,  0 }, { 16, 16,  0,  0 }, { 16, 16,  0,  0 },
        { 32,  0,  0,  0 }, { 32,  0,  0,  0 }, { 32,  0,  0,  0 }, { 32,  0,  0,  0 },
        {  5,  5,  5,  1 },
        {  5,  6,  5,  0 },
        {  8,  8,  0,  0 }, {  8,  8,  0,  0 }, {  8,  8,  0,  0 }, {  8,  8,  0,  0 }, {  8,  8,  0,  0 },
        { 16,  0,  0,  0 }, { 16,  0,  0,  0 }, { 16,  0,  0,  0 }, { 16,  0,  0,  0 }, { 16,  0,  0,  0 }, { 16,  0,  0,  0 },
        {  8,  0,  0,  0 }, {  8,  0,  0,  0 }, {  8,  0,  0,  0 }, {  8,  0,  0,  0 }, {  8,  0,  0,  0 },
        {  8,  0,  0,  0 },
        {  1,  0,  0,  0 },
        { 32,  8,  0,  0 },
        { 24,  8,  0,  0 },
        { 32,  0,  0,  0 },
        { 16,  0,  0,  0 },
        { 32,  8,  0,  0 }, { 32,  0,  0,  0 }, {  8,  0,  0,  0 },
        { 24,  8,  0,  0 }, { 24,  0,  0,  0 }, {  8,  0,  0,  0 },
        {  8,  8,  8,  8 },
        {  8,  8,  8,  8 },
        { 10, 10, 10,  2 },
        { 11, 11, 10,  0 },
        {  9,  9,  9,  5 },
        {  8,  0,  0,  0 }, {  8,  8,  0,  0 },
        {  4,  4,  4,  4 },
        //PIXEL_FMT_BC1,  PIXEL_FMT_BC1UN,     PIXEL_FMT_BC1UN_SRGB,
        { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET }, { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET },
        { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET },
        //PIXEL_FMT_BC2,  PIXEL_FMT_BC2UN,     PIXEL_FMT_BC2UN_SRGB,
        { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET }, { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET },
        { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET },
        //PIXEL_FMT_BC3,  PIXEL_FMT_BC3UN,     PIXEL_FMT_BC3UN_SRGB,
        { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET }, { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET },
        { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET },
        //PIXEL_FMT_BC4,  PIXEL_FMT_BC4UN,     PIXEL_FMT_BC4SN,
        { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET }, { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET },
        { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET },
        //PIXEL_FMT_BC5,  PIXEL_FMT_BC5UN,     PIXEL_FMT_BC5SN,
        { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET }, { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET },
        { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET },
        //PIXEL_FMT_BC6H, PIXEL_FMT_BC6H_UF16, PIXEL_FMT_BC6H_SF16,
        { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET }, { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET },
        { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET },
        //PIXEL_FMT_BC7,  PIXEL_FMT_BC7UN,     PIXEL_FMT_BC7UN_SRGB,
        { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET }, { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET },
        { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET },
        //PIXEL_FMT_AYUV, PIXEL_FMT_Y410, PIXEL_FMT_Y416, PIXEL_FMT_NV12, PIXEL_FMT_P010, PIXEL_FMT_P016,
        { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET }, { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET },
        { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET }, { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET },
        { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET }, { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET },
        //PIXEL_FMT_YUY2, PIXEL_FMT_Y210, PIXEL_FMT_Y216, PIXEL_FMT_NV11, PIXEL_FMT_AI44, PIXEL_FMT_IA44,
        { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET }, { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET },
        { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET }, { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET },
        { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET }, { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET },
        //PIXEL_FMT_420_OPAQUE,
        { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET }
    };
    return s_arrPixelFormatChannelBits[iFormat][iChannel];
}

UInt PixelFormatChannelShift( PixelFormat iFormat, UInt iChannel )
{
    static UInt s_arrPixelFormatChannelShift[PIXEL_FMT_COUNT][4] = {
        { 0, 0, 0, 0 },
        { 0, 32, 64, 96 }, { 0, 32, 64, 96 }, { 0, 32, 64, 96 }, { 0, 32, 64, 96 },
        { 0, 32, 64,  0 }, { 0, 32, 64,  0 }, { 0, 32, 64,  0 }, { 0, 32, 64,  0 },
        { 0, 16, 32, 48 }, { 0, 16, 32, 48 }, { 0, 16, 32, 48 }, { 0, 16, 32, 48 }, { 0, 16, 32, 48 }, { 0, 16, 32, 48 },
        { 0, 32,  0,  0 }, { 0, 32,  0,  0 }, { 0, 32,  0,  0 }, { 0, 32,  0,  0 },
        { 0,  8, 16, 24 }, { 0,  8, 16, 24 }, { 0,  8, 16, 24 }, { 0,  8, 16, 24 }, { 0,  8, 16, 24 }, { 0,  8, 16, 24 },
        { 0,  8, 16, 24 }, { 0,  8, 16, 24 }, { 0,  8, 16, 24 },
        { 0,  8, 16,  0 }, { 0,  8, 16,  0 }, { 0,  8, 16,  0 },
        { 0, 10, 20, 30 }, { 0, 10, 20, 30 }, { 0, 10, 20, 30 },
        { 0, 16,  0,  0 }, { 0, 16,  0,  0 }, { 0, 16,  0,  0 }, { 0, 16,  0,  0 }, { 0, 16,  0,  0 }, { 0, 16,  0,  0 },
        { 0,  0,  0,  0 }, { 0,  0,  0,  0 }, { 0,  0,  0,  0 }, { 0,  0,  0,  0 },
        { 0,  5, 10, 15 },
        { 0,  5, 11,  0 },
        { 0,  8,  0,  0 }, { 0,  8,  0,  0 }, { 0,  8,  0,  0 }, { 0,  8,  0,  0 }, { 0,  8,  0,  0 },
        { 0,  0,  0,  0 }, { 0,  0,  0,  0 }, { 0,  0,  0,  0 }, { 0,  0,  0,  0 }, { 0,  0,  0,  0 }, { 0,  0,  0,  0 },
        { 0,  0,  0,  0 }, { 0,  0,  0,  0 }, { 0,  0,  0,  0 }, { 0,  0,  0,  0 }, { 0,  0,  0,  0 },
        { 0,  0,  0,  0 },
        { 0,  0,  0,  0 },
        { 0, 32,  0,  0 },
        { 0, 24,  0,  0 },
        { 0,  0,  0,  0 },
        { 0,  0,  0,  0 },
        { 0, 32,  0,  0 }, { 0,  0,  0,  0 }, { 32, 0,  0,  0 },
        { 0, 24,  0,  0 }, { 0,  0,  0,  0 }, { 24, 0,  0,  0 },
        { 0,  8, 16, 24 },
        { 0,  8, 16, 24 },
        { 0, 10, 20, 30 },
        { 0, 11, 22,  0 },
        { 0,  9, 18, 27 },
        { 0,  0,  0,  0 }, { 0,  8,  0,  0 },
        { 0,  4,  8, 12 },
        //PIXEL_FMT_BC1,  PIXEL_FMT_BC1UN,     PIXEL_FMT_BC1UN_SRGB,
        { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET }, { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET },
        { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET },
        //PIXEL_FMT_BC2,  PIXEL_FMT_BC2UN,     PIXEL_FMT_BC2UN_SRGB,
        { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET }, { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET },
        { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET },
        //PIXEL_FMT_BC3,  PIXEL_FMT_BC3UN,     PIXEL_FMT_BC3UN_SRGB,
        { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET }, { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET },
        { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET },
        //PIXEL_FMT_BC4,  PIXEL_FMT_BC4UN,     PIXEL_FMT_BC4SN,
        { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET }, { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET },
        { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET },
        //PIXEL_FMT_BC5,  PIXEL_FMT_BC5UN,     PIXEL_FMT_BC5SN,
        { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET }, { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET },
        { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET },
        //PIXEL_FMT_BC6H, PIXEL_FMT_BC6H_UF16, PIXEL_FMT_BC6H_SF16,
        { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET }, { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET },
        { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET },
        //PIXEL_FMT_BC7,  PIXEL_FMT_BC7UN,     PIXEL_FMT_BC7UN_SRGB,
        { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET }, { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET },
        { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET },
        //PIXEL_FMT_AYUV, PIXEL_FMT_Y410, PIXEL_FMT_Y416, PIXEL_FMT_NV12, PIXEL_FMT_P010, PIXEL_FMT_P016,
        { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET }, { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET },
        { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET }, { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET },
        { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET }, { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET },
        //PIXEL_FMT_YUY2, PIXEL_FMT_Y210, PIXEL_FMT_Y216, PIXEL_FMT_NV11, PIXEL_FMT_AI44, PIXEL_FMT_IA44,
        { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET }, { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET },
        { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET }, { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET },
        { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET }, { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET },
        //PIXEL_FMT_420_OPAQUE,
        { INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET, INVALID_OFFSET }
    };
    return s_arrPixelFormatChannelShift[iFormat][iChannel];
}

Bool PixelFormatIsMipMapable( PixelFormat iFormat )
{
    static Bool s_arrPixelFormatMipMapable[PIXEL_FMT_COUNT] = {
        false,
        false, false, true, true,
        false, false, true, true,
        false, false, true, true, true, true,
        false, false, true, true,
        false, true, true, true, true, true,
        false, true, true,
        false, true, true,
        false, true, true,
        false, false, true, true, true, true,
        false, false, true, true,
        true,
        true,
        false, true, true, true, true,
        false, false, true, true, true, true,
        false, true, true, true, true,
        true,
        true,
        false,
        false,
        false,
        false,
        false, false, true,
        false, true, true,
        true,
        true,
        true,
        false,
        false,
        false, false,
        true,
        false, false, false,
        false, false, false,
        false, false, false,
        false, false, false,
        false, false, false,
        false, false, false,
        false, false, false,
        false, false, false, false, false, false,
        false, false, false, false, false, false,
        false
    };
    return s_arrPixelFormatMipMapable[iFormat];
}
Bool PixelFormatIsCompressed( PixelFormat iFormat )
{
    static Bool s_arrPixelFormatCompressed[PIXEL_FMT_COUNT] = {
        false,
        false, false, false, false,
        false, false, false, false,
        false, false, false, false, false, false,
        false, false, false, false,
        false, false, false, false, false, false,
        false, false, false,
        false, false, false,
        false, false, false,
        false, false, false, false, false, false,
        false, false, false, false,
        false,
        false,
        false, false, false, false, false,
        false, false, false, false, false, false,
        false, false, false, false, false,
        false,
        false,
        false,
        false,
        false,
        false,
        false, false, false,
        false, false, false,
        false,
        false,
        false,
        false,
        false,
        false, false,
        false,
        true, true, true,
        true, true, true,
        true, true, true,
        true, true, true,
        true, true, true,
        true, true, true,
        true, true, true,
        false, false, false, false, false, false,
        false, false, false, false, false, false,
        false
    };
    return s_arrPixelFormatCompressed[iFormat];
}

Void PixelFormatExtractChannels( DWord outDstChannels[4], const Byte * pSrcColor, PixelFormat iSrcFormat )
{
    UInt iChannelCount = PixelFormatChannelCount( iSrcFormat );

    UInt iChannel = 0;
    UInt iChannelBits = PixelFormatChannelBits( iSrcFormat, iChannel );
    UInt iExtractedChannelBits = 0;
    outDstChannels[iChannel] = 0;

    Byte iExtractedByte = *pSrcColor++;
    Byte iExtractMask = 0x80;

    while( true ) {
        // Extract a bit
        if ( iExtractedByte & iExtractMask )
            outDstChannels[iChannel] |= 0x01;

        // Check if we filled that channel
        ++iExtractedChannelBits;
        if ( iExtractedChannelBits >= iChannelBits ) {
            // Check if we filled all channels
            ++iChannel;
            if ( iChannel >= iChannelCount )
                return; // Done

            // Next channel
            iChannelBits = PixelFormatChannelBits( iSrcFormat, iChannel );
            iExtractedChannelBits = 0;
            outDstChannels[iChannel] = 0;
        } else {
            // Prepare channel to receive next bit
            outDstChannels[iChannel] <<= 1;
        }

        // Next bit
        iExtractMask >>= 1;
        if ( iExtractMask == 0 ) {
            iExtractedByte = *pSrcColor++;
            iExtractMask = 0x80;
        }
    }
}
Void PixelFormatMergeChannels( Byte * outDstColor, const DWord pSrcChannels[4], PixelFormat iDstFormat )
{
    UInt iChannelCount = PixelFormatChannelCount( iDstFormat );

    Byte iMergedByte = 0;
    Byte iMergeMask = 0x80;

    UInt iChannel = 0;
    UInt iChannelBits = PixelFormatChannelBits( iDstFormat, iChannel );
    DWord iChannelMask = ( 1 << (iChannelBits - 1) );

    while( true ) {
        // Extract a bit
        if ( pSrcChannels[iChannel] & iChannelMask )
            iMergedByte |= iMergeMask;

        // Next bit
        iMergeMask >>= 1;
        if ( iMergeMask == 0 ) {
            *outDstColor++ = iMergedByte;
            iMergedByte = 0;
            iMergeMask = 0x80;
        }

        // Check if we exhausted that channel
        iChannelMask >>= 1;
        if ( iChannelMask == 0 ) {
            // Check if we exhausted all channels
            ++iChannel;
            if ( iChannel >= iChannelCount )
                return; // Done

            iChannelBits = PixelFormatChannelBits( iDstFormat, iChannel );
            iChannelMask = ( 1 << (iChannelBits - 1) );
        }
    }
}

typedef Void (*_PixelFormatConvertFrom)( Float[4], const Byte * );
typedef Void (*_PixelFormatConvertTo)( Byte *, const Float[4] );

static const Float _fInvBYTEMAX  = ( 1.0f / (Float)BYTE_MAX );
static const Float _fInvWORDMAX  = ( 1.0f / (Float)WORD_MAX );
static const Float _fInvDWORDMAX = ( 1.0f / (Float)DWORD_MAX );

static const Byte _1BitsMAX      = 0x01;
static const Float _fInv1BitsMAX = ( 1.0f / (Float)_1BitsMAX );

static const Byte _2BitsMAX      = 0x03;
static const Float _fInv2BitsMAX = ( 1.0f / (Float)_2BitsMAX );

static const Byte _4BitsMAX      = 0x0f;
static const Float _fInv4BitsMAX = ( 1.0f / (Float)_4BitsMAX );

static const Byte _5BitsMAX      = 0x1f;
static const Float _fInv5BitsMAX = ( 1.0f / (Float)_5BitsMAX );

static const Byte _6BitsMAX      = 0x3f;
static const Float _fInv6BitsMAX = ( 1.0f / (Float)_6BitsMAX );

static const Word _10BitsMAX      = 0x03ff;
static const Float _fInv10BitsMAX = ( 1.0f / (Float)_10BitsMAX );

static const DWord _24BitsMAX      = 0x00ffffff;
static const Float _fInv24BitsMAX = ( 1.0f / (Float)_24BitsMAX );

Void _ConvertFrom_RGBA32_F( Float outDstColor[4], const Byte * pSrcColor )
{
    const Float * pSrc = (const Float *)pSrcColor;
    outDstColor[0] = pSrc[0];
    outDstColor[1] = pSrc[1];
    outDstColor[2] = pSrc[2];
    outDstColor[3] = pSrc[3];
}
Void _ConvertFrom_RGBA32_U( Float outDstColor[4], const Byte * pSrcColor )
{
    const DWord * pSrc = (const DWord *)pSrcColor;
    outDstColor[0] = ( _fInvDWORDMAX * (Float)(pSrc[0]) );
    outDstColor[1] = ( _fInvDWORDMAX * (Float)(pSrc[1]) );
    outDstColor[2] = ( _fInvDWORDMAX * (Float)(pSrc[2]) );
    outDstColor[3] = ( _fInvDWORDMAX * (Float)(pSrc[3]) );
}
Void _ConvertFrom_RGBA32_S( Float outDstColor[4], const Byte * pSrcColor )
{
    const Long * pSrc = (const Long *)pSrcColor;
    outDstColor[0] = ( _fInvDWORDMAX * (Float)((Int64)(pSrc[0]) + (Int64)SLONG_MAX + 1i64) );
    outDstColor[1] = ( _fInvDWORDMAX * (Float)((Int64)(pSrc[1]) + (Int64)SLONG_MAX + 1i64) );
    outDstColor[2] = ( _fInvDWORDMAX * (Float)((Int64)(pSrc[2]) + (Int64)SLONG_MAX + 1i64) );
    outDstColor[3] = ( _fInvDWORDMAX * (Float)((Int64)(pSrc[3]) + (Int64)SLONG_MAX + 1i64) );
}

Void _ConvertFrom_RGB32_F( Float outDstColor[4], const Byte * pSrcColor )
{
    const Float * pSrc = (const Float *)pSrcColor;
    outDstColor[0] = pSrc[0];
    outDstColor[1] = pSrc[1];
    outDstColor[2] = pSrc[2];
    outDstColor[3] = 0.0f;
}
Void _ConvertFrom_RGB32_U( Float outDstColor[4], const Byte * pSrcColor )
{
    const DWord * pSrc = (const DWord *)pSrcColor;
    outDstColor[0] = ( _fInvDWORDMAX * (Float)(pSrc[0]) );
    outDstColor[1] = ( _fInvDWORDMAX * (Float)(pSrc[1]) );
    outDstColor[2] = ( _fInvDWORDMAX * (Float)(pSrc[2]) );
    outDstColor[3] = 0.0f;
}
Void _ConvertFrom_RGB32_S( Float outDstColor[4], const Byte * pSrcColor )
{
    const Long * pSrc = (const Long *)pSrcColor;
    outDstColor[0] = ( _fInvDWORDMAX * (Float)((Int64)(pSrc[0]) + (Int64)SLONG_MAX + 1i64) );
    outDstColor[1] = ( _fInvDWORDMAX * (Float)((Int64)(pSrc[1]) + (Int64)SLONG_MAX + 1i64) );
    outDstColor[2] = ( _fInvDWORDMAX * (Float)((Int64)(pSrc[2]) + (Int64)SLONG_MAX + 1i64) );
    outDstColor[3] = 0.0f;
}

Void _ConvertFrom_RGBA16_F( Float outDstColor[4], const Byte * pSrcColor )
{
    DebugAssert( false );
    outDstColor[0] = 0.0f;
    outDstColor[1] = 0.0f;
    outDstColor[2] = 0.0f;
    outDstColor[3] = 0.0f;
}
Void _ConvertFrom_RGBA16_U( Float outDstColor[4], const Byte * pSrcColor )
{
    const Word * pSrc = (const Word *)pSrcColor;
    outDstColor[0] = ( _fInvWORDMAX * (Float)(pSrc[0]) );
    outDstColor[1] = ( _fInvWORDMAX * (Float)(pSrc[1]) );
    outDstColor[2] = ( _fInvWORDMAX * (Float)(pSrc[2]) );
    outDstColor[3] = ( _fInvWORDMAX * (Float)(pSrc[3]) );
}
Void _ConvertFrom_RGBA16_S( Float outDstColor[4], const Byte * pSrcColor )
{
    const Short * pSrc = (const Short *)pSrcColor;
    outDstColor[0] = ( _fInvWORDMAX * (Float)((Long)(pSrc[0]) + (Long)SSHORT_MAX + 1l) );
    outDstColor[1] = ( _fInvWORDMAX * (Float)((Long)(pSrc[1]) + (Long)SSHORT_MAX + 1l) );
    outDstColor[2] = ( _fInvWORDMAX * (Float)((Long)(pSrc[2]) + (Long)SSHORT_MAX + 1l) );
    outDstColor[3] = ( _fInvWORDMAX * (Float)((Long)(pSrc[3]) + (Long)SSHORT_MAX + 1l) );
}

Void _ConvertFrom_RG32_F( Float outDstColor[4], const Byte * pSrcColor )
{
    const Float * pSrc = (const Float *)pSrcColor;
    outDstColor[0] = pSrc[0];
    outDstColor[1] = pSrc[1];
    outDstColor[2] = 0.0f;
    outDstColor[3] = 0.0f;
}
Void _ConvertFrom_RG32_U( Float outDstColor[4], const Byte * pSrcColor )
{
    const DWord * pSrc = (const DWord *)pSrcColor;
    outDstColor[0] = ( _fInvDWORDMAX * (Float)(pSrc[0]) );
    outDstColor[1] = ( _fInvDWORDMAX * (Float)(pSrc[1]) );
    outDstColor[2] = 0.0f;
    outDstColor[3] = 0.0f;
}
Void _ConvertFrom_RG32_S( Float outDstColor[4], const Byte * pSrcColor )
{
    const Long * pSrc = (const Long *)pSrcColor;
    outDstColor[0] = ( _fInvDWORDMAX * (Float)((Int64)(pSrc[0]) + (Int64)SLONG_MAX + 1i64) );
    outDstColor[1] = ( _fInvDWORDMAX * (Float)((Int64)(pSrc[1]) + (Int64)SLONG_MAX + 1i64) );
    outDstColor[2] = 0.0f;
    outDstColor[3] = 0.0f;
}

Void _ConvertFrom_RGBA8_U( Float outDstColor[4], const Byte * pSrcColor )
{
    outDstColor[0] = ( _fInvBYTEMAX * (Float)(pSrcColor[0]) );
    outDstColor[1] = ( _fInvBYTEMAX * (Float)(pSrcColor[1]) );
    outDstColor[2] = ( _fInvBYTEMAX * (Float)(pSrcColor[2]) );
    outDstColor[3] = ( _fInvBYTEMAX * (Float)(pSrcColor[3]) );
}
Void _ConvertFrom_RGBA8_S( Float outDstColor[4], const Byte * pSrcColor )
{
    const Char * pSrc = (const Char *)pSrcColor;
    outDstColor[0] = ( _fInvBYTEMAX * (Float)((Short)(pSrc[0]) + (Short)SCHAR_MAX + 1) );
    outDstColor[1] = ( _fInvBYTEMAX * (Float)((Short)(pSrc[1]) + (Short)SCHAR_MAX + 1) );
    outDstColor[2] = ( _fInvBYTEMAX * (Float)((Short)(pSrc[2]) + (Short)SCHAR_MAX + 1) );
    outDstColor[3] = ( _fInvBYTEMAX * (Float)((Short)(pSrc[3]) + (Short)SCHAR_MAX + 1) );
}
Void _ConvertFrom_RGBA8_SRGB( Float outDstColor[4], const Byte * pSrcColor )
{
    DebugAssert( false );
    outDstColor[0] = 0.0f;
    outDstColor[1] = 0.0f;
    outDstColor[2] = 0.0f;
    outDstColor[3] = 0.0f;
}

Void _ConvertFrom_BGRA8_U( Float outDstColor[4], const Byte * pSrcColor )
{
    outDstColor[0] = ( _fInvBYTEMAX * (Float)(pSrcColor[2]) );
    outDstColor[1] = ( _fInvBYTEMAX * (Float)(pSrcColor[1]) );
    outDstColor[2] = ( _fInvBYTEMAX * (Float)(pSrcColor[0]) );
    outDstColor[3] = ( _fInvBYTEMAX * (Float)(pSrcColor[3]) );
}
Void _ConvertFrom_BGRA8_SRGB( Float outDstColor[4], const Byte * pSrcColor )
{
    DebugAssert( false );
    outDstColor[0] = 0.0f;
    outDstColor[1] = 0.0f;
    outDstColor[2] = 0.0f;
    outDstColor[3] = 0.0f;
}

Void _ConvertFrom_BGRX8_U( Float outDstColor[4], const Byte * pSrcColor )
{
    outDstColor[0] = ( _fInvBYTEMAX * (Float)(pSrcColor[2]) );
    outDstColor[1] = ( _fInvBYTEMAX * (Float)(pSrcColor[1]) );
    outDstColor[2] = ( _fInvBYTEMAX * (Float)(pSrcColor[0]) );
    outDstColor[3] = ( _fInvBYTEMAX * (Float)(pSrcColor[3]) );
}
Void _ConvertFrom_BGRX8_SRGB( Float outDstColor[4], const Byte * pSrcColor )
{
    DebugAssert( false );
    outDstColor[0] = 0.0f;
    outDstColor[1] = 0.0f;
    outDstColor[2] = 0.0f;
    outDstColor[3] = 0.0f;
}

Void _ConvertFrom_RGB10A2_U( Float outDstColor[4], const Byte * pSrcColor )
{
    const DWord * pSrc = (const DWord *)pSrcColor;
    outDstColor[0] = ( _fInv10BitsMAX * (Float)( ((*pSrc) & 0xffc00000) >> 22 ) );
    outDstColor[1] = ( _fInv10BitsMAX * (Float)( ((*pSrc) & 0x003ff000) >> 12 ) );
    outDstColor[2] = ( _fInv10BitsMAX * (Float)( ((*pSrc) & 0x00000ffc) >>  2 ) );
    outDstColor[3] = ( _fInv2BitsMAX  * (Float)( ((*pSrc) & 0x00000003) ) );
}

Void _ConvertFrom_RG16_F( Float outDstColor[4], const Byte * pSrcColor )
{
    DebugAssert( false );
    outDstColor[0] = 0.0f;
    outDstColor[1] = 0.0f;
    outDstColor[2] = 0.0f;
    outDstColor[3] = 0.0f;
}
Void _ConvertFrom_RG16_U( Float outDstColor[4], const Byte * pSrcColor )
{
    const Word * pSrc = (const Word *)pSrcColor;
    outDstColor[0] = ( _fInvWORDMAX * (Float)(pSrc[0]) );
    outDstColor[1] = ( _fInvWORDMAX * (Float)(pSrc[1]) );
    outDstColor[2] = 0.0f;
    outDstColor[3] = 0.0f;
}
Void _ConvertFrom_RG16_S( Float outDstColor[4], const Byte * pSrcColor )
{
    const Short * pSrc = (const Short *)pSrcColor;
    outDstColor[0] = ( _fInvWORDMAX * (Float)((Long)(pSrc[0]) + (Long)SSHORT_MAX + 1l) );
    outDstColor[1] = ( _fInvWORDMAX * (Float)((Long)(pSrc[1]) + (Long)SSHORT_MAX + 1l) );
    outDstColor[2] = 0.0f;
    outDstColor[3] = 0.0f;
}

Void _ConvertFrom_R32_F( Float outDstColor[4], const Byte * pSrcColor )
{
    const Float * pSrc = (const Float *)pSrcColor;
    outDstColor[0] = *pSrc;
    outDstColor[1] = 0.0f;
    outDstColor[2] = 0.0f;
    outDstColor[3] = 0.0f;
}
Void _ConvertFrom_R32_U( Float outDstColor[4], const Byte * pSrcColor )
{
    const DWord * pSrc = (const DWord *)pSrcColor;
    outDstColor[0] = ( _fInvDWORDMAX * (Float)(*pSrc) );
    outDstColor[1] = 0.0f;
    outDstColor[2] = 0.0f;
    outDstColor[3] = 0.0f;
}
Void _ConvertFrom_R32_S( Float outDstColor[4], const Byte * pSrcColor )
{
    const Long * pSrc = (const Long *)pSrcColor;
    outDstColor[0] = ( _fInvDWORDMAX * (Float)((Int64)(*pSrc) + (Int64)SLONG_MAX + 1i64) );
    outDstColor[1] = 0.0f;
    outDstColor[2] = 0.0f;
    outDstColor[3] = 0.0f;
}

Void _ConvertFrom_BGR5A1_U( Float outDstColor[4], const Byte * pSrcColor )
{
    const Word * pSrc = (const Word *)pSrcColor;
    outDstColor[0] = ( _fInv5BitsMAX * (Float)( ((*pSrc) & 0x003e) >>  1 ) );
    outDstColor[1] = ( _fInv5BitsMAX * (Float)( ((*pSrc) & 0x07c0) >>  6 ) );
    outDstColor[2] = ( _fInv5BitsMAX * (Float)( ((*pSrc) & 0xf800) >> 11 ) );
    outDstColor[3] = ( _fInv1BitsMAX * (Float)( ((*pSrc) & 0x0001) ) );
}

Void _ConvertFrom_B5G6R5_U( Float outDstColor[4], const Byte * pSrcColor )
{
    const Word * pSrc = (const Word *)pSrcColor;
    outDstColor[0] = ( _fInv5BitsMAX * (Float)( ((*pSrc) & 0x001f) ) );
    outDstColor[1] = ( _fInv6BitsMAX * (Float)( ((*pSrc) & 0x07e0) >>  5 ) );
    outDstColor[2] = ( _fInv5BitsMAX * (Float)( ((*pSrc) & 0xf800) >> 11 ) );
    outDstColor[3] = 0.0f;
}

Void _ConvertFrom_RG8_U( Float outDstColor[4], const Byte * pSrcColor )
{
    outDstColor[0] = ( _fInvBYTEMAX * (Float)(pSrcColor[0]) );
    outDstColor[1] = ( _fInvBYTEMAX * (Float)(pSrcColor[1]) );
    outDstColor[2] = 0.0f;
    outDstColor[3] = 0.0f;
}
Void _ConvertFrom_RG8_S( Float outDstColor[4], const Byte * pSrcColor )
{
    const Char * pSrc = (const Char *)pSrcColor;
    outDstColor[0] = ( _fInvBYTEMAX * (Float)((Short)(pSrc[0]) + (Short)SCHAR_MAX + 1) );
    outDstColor[1] = ( _fInvBYTEMAX * (Float)((Short)(pSrc[1]) + (Short)SCHAR_MAX + 1) );
    outDstColor[2] = 0.0f;
    outDstColor[3] = 0.0f;
}

Void _ConvertFrom_R16_F( Float outDstColor[4], const Byte * pSrcColor )
{
    DebugAssert( false );
    outDstColor[0] = 0.0f;
    outDstColor[1] = 0.0f;
    outDstColor[2] = 0.0f;
    outDstColor[3] = 0.0f;
}
Void _ConvertFrom_R16_U( Float outDstColor[4], const Byte * pSrcColor )
{
    const Word * pSrc = (const Word *)pSrcColor;
    outDstColor[0] = ( _fInvWORDMAX * (Float)(*pSrc) );
    outDstColor[1] = 0.0f;
    outDstColor[2] = 0.0f;
    outDstColor[3] = 0.0f;
}
Void _ConvertFrom_R16_S( Float outDstColor[4], const Byte * pSrcColor )
{
    const Short * pSrc = (const Short *)pSrcColor;
    outDstColor[0] = ( _fInvWORDMAX * (Float)((Long)(*pSrc) + (Long)SSHORT_MAX + 1l) );
    outDstColor[1] = 0.0f;
    outDstColor[2] = 0.0f;
    outDstColor[3] = 0.0f;
}

Void _ConvertFrom_R8_U( Float outDstColor[4], const Byte * pSrcColor )
{
    outDstColor[0] = ( _fInvBYTEMAX * (Float)(*pSrcColor) );
    outDstColor[1] = 0.0f;
    outDstColor[2] = 0.0f;
    outDstColor[3] = 0.0f;
}
Void _ConvertFrom_R8_S( Float outDstColor[4], const Byte * pSrcColor )
{
    const Char * pSrc = (const Char *)pSrcColor;
    outDstColor[0] = ( _fInvBYTEMAX * (Float)((Short)(*pSrc) + (Short)SCHAR_MAX + 1) );
    outDstColor[1] = 0.0f;
    outDstColor[2] = 0.0f;
    outDstColor[3] = 0.0f;
}

Void _ConvertFrom_A8_U( Float outDstColor[4], const Byte * pSrcColor )
{
    outDstColor[0] = 0.0f;
    outDstColor[1] = 0.0f;
    outDstColor[2] = 0.0f;
    outDstColor[3] = ( _fInvBYTEMAX * (Float)(*pSrcColor) );
}

Void _ConvertFrom_D32S8X24_FU( Float outDstColor[4], const Byte * pSrcColor )
{
    const Float * pSrc0 = (const Float *)pSrcColor;
    const Byte * pSrc1 = ( pSrcColor + 4 );
    outDstColor[0] = *pSrc0;
    outDstColor[1] = (Float)(*pSrc1); // Never scale stencil
    outDstColor[2] = 0.0f;
    outDstColor[3] = 0.0f;
}
Void _ConvertFrom_D24S8_U( Float outDstColor[4], const Byte * pSrcColor )
{
    const DWord * pSrc = (const DWord *)pSrcColor;
    outDstColor[0] = ( _fInv24BitsMAX * (Float)( ((*pSrc) & 0xffffff00) >> 8 ) );
    outDstColor[1] = (Float)( ((*pSrc) & 0x000000ff) ); // Never scale stencil
    outDstColor[2] = 0.0f;
    outDstColor[3] = 0.0f;
}
Void _ConvertFrom_D32_F( Float outDstColor[4], const Byte * pSrcColor )
{
    const Float * pSrc = (const Float *)pSrcColor;
    outDstColor[0] = *pSrc;
    outDstColor[1] = 0.0f;
    outDstColor[2] = 0.0f;
    outDstColor[3] = 0.0f;
}
Void _ConvertFrom_D16_U( Float outDstColor[4], const Byte * pSrcColor )
{
    const Word * pSrc = (const Word *)pSrcColor;
    outDstColor[0] = ( _fInvWORDMAX * (Float)(*pSrc) );
    outDstColor[1] = 0.0f;
    outDstColor[2] = 0.0f;
    outDstColor[3] = 0.0f;
}

Void _ConvertFrom_R32X8X24_F( Float outDstColor[4], const Byte * pSrcColor )
{
    const Float * pSrc = (const Float *)pSrcColor;
    outDstColor[0] = *pSrc;
    outDstColor[1] = 0.0f;
    outDstColor[2] = 0.0f;
    outDstColor[3] = 0.0f;
}
Void _ConvertFrom_X32G8X24_U( Float outDstColor[4], const Byte * pSrcColor )
{
    const Byte * pSrc = ( pSrcColor + 4 );
    outDstColor[0] = 0.0f;
    outDstColor[1] = ( _fInvBYTEMAX * (Float)(*pSrc) );
    outDstColor[2] = 0.0f;
    outDstColor[3] = 0.0f;
}
Void _ConvertFrom_R24X8_U( Float outDstColor[4], const Byte * pSrcColor )
{
    const DWord * pSrc = (const DWord *)pSrcColor;
    outDstColor[0] = ( _fInv24BitsMAX * (Float)( ((*pSrc) & 0xffffff00) >> 8 ) );
    outDstColor[1] = 0.0f;
    outDstColor[2] = 0.0f;
    outDstColor[3] = 0.0f;
}
Void _ConvertFrom_X24G8_U( Float outDstColor[4], const Byte * pSrcColor )
{
    const Byte * pSrc = ( pSrcColor + 3 );
    outDstColor[0] = 0.0f;
    outDstColor[1] = ( _fInvBYTEMAX * (Float)(*pSrc) );
    outDstColor[2] = 0.0f;
    outDstColor[3] = 0.0f;
}

Void _ConvertFrom_RG8BG8_U( Float outDstColor[4], const Byte * pSrcColor )
{
    outDstColor[0] = ( _fInvBYTEMAX * (Float)(pSrcColor[0]) );
    outDstColor[1] = ( _fInvBYTEMAX * (Float)(pSrcColor[1]) );
    outDstColor[2] = ( _fInvBYTEMAX * (Float)(pSrcColor[2]) );
    outDstColor[3] = ( _fInvBYTEMAX * (Float)(pSrcColor[3]) );
}
Void _ConvertFrom_GR8GB8_U( Float outDstColor[4], const Byte * pSrcColor )
{
    outDstColor[0] = ( _fInvBYTEMAX * (Float)(pSrcColor[1]) );
    outDstColor[1] = ( _fInvBYTEMAX * (Float)(pSrcColor[0]) );
    outDstColor[2] = ( _fInvBYTEMAX * (Float)(pSrcColor[3]) );
    outDstColor[3] = ( _fInvBYTEMAX * (Float)(pSrcColor[2]) );
}

Void _ConvertFrom_RGB10XRBIAS_A2_U( Float outDstColor[4], const Byte * pSrcColor )
{
    static const Float s_fInv510 = ( 1.0f / 510.0f );
    const DWord * pSrc = (const DWord *)pSrcColor;
    outDstColor[0] = ( s_fInv510     * (Float)( (((*pSrc) & 0xffc00000) >> 22) - 0x180 ) );
    outDstColor[1] = ( s_fInv510     * (Float)( (((*pSrc) & 0x003ff000) >> 12) - 0x180 ) );
    outDstColor[2] = ( s_fInv510     * (Float)( (((*pSrc) & 0x00000ffc) >>  2) - 0x180 ) );
    outDstColor[3] = ( _fInv2BitsMAX * (Float)(  ((*pSrc) & 0x00000003) ) );
}

Void _ConvertFrom_RG11B10_F( Float outDstColor[4], const Byte * pSrcColor )
{
    DebugAssert( false );
    outDstColor[0] = 0.0f;
    outDstColor[1] = 0.0f;
    outDstColor[2] = 0.0f;
    outDstColor[3] = 0.0f;
}
Void _ConvertFrom_RGB9EXP5_F( Float outDstColor[4], const Byte * pSrcColor )
{
    DebugAssert( false );
    outDstColor[0] = 0.0f;
    outDstColor[1] = 0.0f;
    outDstColor[2] = 0.0f;
    outDstColor[3] = 0.0f;
}

Void _ConvertFrom_P8_U( Float outDstColor[4], const Byte * pSrcColor )
{
    outDstColor[0] = ( _fInvBYTEMAX * (Float)(*pSrcColor) );
    outDstColor[1] = 0.0f;
    outDstColor[2] = 0.0f;
    outDstColor[3] = 0.0f;
}
Void _ConvertFrom_AP8_U( Float outDstColor[4], const Byte * pSrcColor )
{
    outDstColor[0] = ( _fInvBYTEMAX * (Float)(pSrcColor[1]) );
    outDstColor[1] = 0.0f;
    outDstColor[2] = 0.0f;
    outDstColor[3] = ( _fInvBYTEMAX * (Float)(pSrcColor[0]) );
}

Void _ConvertFrom_BGRA4_U( Float outDstColor[4], const Byte * pSrcColor )
{
    const Word * pSrc = (const Word *)pSrcColor;
    outDstColor[0] = ( _fInv4BitsMAX * (Float)( ((*pSrc) & 0x00f0) >>  4 ) );
    outDstColor[1] = ( _fInv4BitsMAX * (Float)( ((*pSrc) & 0x0f00) >>  8 ) );
    outDstColor[2] = ( _fInv4BitsMAX * (Float)( ((*pSrc) & 0xf000) >> 12 ) );
    outDstColor[3] = ( _fInv4BitsMAX * (Float)( ((*pSrc) & 0x000f) ) );
}

Void PixelFormatConvertFrom( Float outDstColor[4], const Byte * pSrcColor, PixelFormat iSrcFormat, UInt iCount )
{
    static _PixelFormatConvertFrom sm_arrConvertFrom[PIXEL_FMT_COUNT] = {
        NULL,
        NULL, _ConvertFrom_RGBA32_F, _ConvertFrom_RGBA32_U, _ConvertFrom_RGBA32_S,
        NULL, _ConvertFrom_RGB32_F, _ConvertFrom_RGB32_U, _ConvertFrom_RGB32_S,
        NULL, _ConvertFrom_RGBA16_F, _ConvertFrom_RGBA16_U, _ConvertFrom_RGBA16_U, _ConvertFrom_RGBA16_S, _ConvertFrom_RGBA16_S,
        NULL, _ConvertFrom_RG32_F, _ConvertFrom_RG32_U, _ConvertFrom_RG32_S,
        NULL, _ConvertFrom_RGBA8_U, _ConvertFrom_RGBA8_SRGB, _ConvertFrom_RGBA8_U, _ConvertFrom_RGBA8_S, _ConvertFrom_RGBA8_S,
        NULL, _ConvertFrom_BGRA8_U, _ConvertFrom_BGRA8_SRGB,
        NULL, _ConvertFrom_BGRX8_U, _ConvertFrom_BGRX8_SRGB,
        NULL, _ConvertFrom_RGB10A2_U, _ConvertFrom_RGB10A2_U,
        NULL, _ConvertFrom_RG16_F, _ConvertFrom_RG16_U, _ConvertFrom_RG16_U, _ConvertFrom_RG16_S, _ConvertFrom_RG16_S,
        NULL, _ConvertFrom_R32_F, _ConvertFrom_R32_U, _ConvertFrom_R32_S,
        _ConvertFrom_BGR5A1_U,
        _ConvertFrom_B5G6R5_U,
        NULL, _ConvertFrom_RG8_U, _ConvertFrom_RG8_U, _ConvertFrom_RG8_S, _ConvertFrom_RG8_S,
        NULL, _ConvertFrom_R16_F, _ConvertFrom_R16_U, _ConvertFrom_R16_U, _ConvertFrom_R16_S, _ConvertFrom_R16_S,
        NULL, _ConvertFrom_R8_U, _ConvertFrom_R8_U, _ConvertFrom_R8_S, _ConvertFrom_R8_S,
        _ConvertFrom_A8_U,
        NULL,
        _ConvertFrom_D32S8X24_FU,
        _ConvertFrom_D24S8_U,
        _ConvertFrom_D32_F,
        _ConvertFrom_D16_U,
        NULL, _ConvertFrom_R32X8X24_F, _ConvertFrom_X32G8X24_U,
        NULL, _ConvertFrom_R24X8_U, _ConvertFrom_X24G8_U,
        _ConvertFrom_RG8BG8_U,
        _ConvertFrom_GR8GB8_U,
        _ConvertFrom_RGB10XRBIAS_A2_U,
        _ConvertFrom_RG11B10_F,
        _ConvertFrom_RGB9EXP5_F,
        _ConvertFrom_P8_U, _ConvertFrom_AP8_U,
        _ConvertFrom_BGRA4_U,
        NULL, NULL, NULL,
        NULL, NULL, NULL,
        NULL, NULL, NULL,
        NULL, NULL, NULL,
        NULL, NULL, NULL,
        NULL, NULL, NULL,
        NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL,
        NULL
    };
    UInt iChannelCount = PixelFormatChannelCount( iSrcFormat );
    UInt iByteCount = PixelFormatBytes( iSrcFormat );

    _PixelFormatConvertFrom pfConvertFrom = sm_arrConvertFrom[iSrcFormat];
    DebugAssert( pfConvertFrom != NULL );

    for( UInt i = 0; i < iCount; ++i ) {
        pfConvertFrom( outDstColor, pSrcColor );
        outDstColor += iChannelCount;
        pSrcColor += iByteCount;
    }
}

Void _ConvertTo_RGBA32_F( Byte * outDstColor, const Float pSrcColor[4] )
{
    Float * pDst = (Float*)outDstColor;
    pDst[0] = pSrcColor[0];
    pDst[1] = pSrcColor[1];
    pDst[2] = pSrcColor[2];
    pDst[3] = pSrcColor[3];
}
Void _ConvertTo_RGBA32_U( Byte * outDstColor, const Float pSrcColor[4] )
{
    DWord * pDst = (DWord*)outDstColor;
    pDst[0] = (DWord)( pSrcColor[0] * (Float)DWORD_MAX );
    pDst[1] = (DWord)( pSrcColor[1] * (Float)DWORD_MAX );
    pDst[2] = (DWord)( pSrcColor[2] * (Float)DWORD_MAX );
    pDst[3] = (DWord)( pSrcColor[3] * (Float)DWORD_MAX );
}
Void _ConvertTo_RGBA32_S( Byte * outDstColor, const Float pSrcColor[4] )
{
    Long * pDst = (Long*)outDstColor;
    pDst[0] = (Long)( (pSrcColor[0] * (Float)DWORD_MAX) + (Float)SLONG_MIN );
    pDst[1] = (Long)( (pSrcColor[1] * (Float)DWORD_MAX) + (Float)SLONG_MIN );
    pDst[2] = (Long)( (pSrcColor[2] * (Float)DWORD_MAX) + (Float)SLONG_MIN );
    pDst[3] = (Long)( (pSrcColor[3] * (Float)DWORD_MAX) + (Float)SLONG_MIN );
}

Void _ConvertTo_RGB32_F( Byte * outDstColor, const Float pSrcColor[4] )
{
    Float * pDst = (Float*)outDstColor;
    pDst[0] = pSrcColor[0];
    pDst[1] = pSrcColor[1];
    pDst[2] = pSrcColor[2];
}
Void _ConvertTo_RGB32_U( Byte * outDstColor, const Float pSrcColor[4] )
{
    DWord * pDst = (DWord*)outDstColor;
    pDst[0] = (DWord)( pSrcColor[0] * (Float)DWORD_MAX );
    pDst[1] = (DWord)( pSrcColor[1] * (Float)DWORD_MAX );
    pDst[2] = (DWord)( pSrcColor[2] * (Float)DWORD_MAX );
}
Void _ConvertTo_RGB32_S( Byte * outDstColor, const Float pSrcColor[4] )
{
    Long * pDst = (Long*)outDstColor;
    pDst[0] = (Long)( (pSrcColor[0] * (Float)DWORD_MAX) + (Float)SLONG_MIN );
    pDst[1] = (Long)( (pSrcColor[1] * (Float)DWORD_MAX) + (Float)SLONG_MIN );
    pDst[2] = (Long)( (pSrcColor[2] * (Float)DWORD_MAX) + (Float)SLONG_MIN );
}

Void _ConvertTo_RGBA16_F( Byte * outDstColor, const Float pSrcColor[4] )
{
    DebugAssert( false );
    Word * pDst = (Word*)outDstColor;
    pDst[0] = 0;
    pDst[1] = 0;
    pDst[2] = 0;
    pDst[3] = 0;
}
Void _ConvertTo_RGBA16_U( Byte * outDstColor, const Float pSrcColor[4] )
{
    Word * pDst = (Word*)outDstColor;
    pDst[0] = (Word)( pSrcColor[0] * (Float)WORD_MAX );
    pDst[1] = (Word)( pSrcColor[1] * (Float)WORD_MAX );
    pDst[2] = (Word)( pSrcColor[2] * (Float)WORD_MAX );
    pDst[3] = (Word)( pSrcColor[3] * (Float)WORD_MAX );
}
Void _ConvertTo_RGBA16_S( Byte * outDstColor, const Float pSrcColor[4] )
{
    Short * pDst = (Short*)outDstColor;
    pDst[0] = (Short)( (pSrcColor[0] * (Float)WORD_MAX) + (Float)SSHORT_MIN );
    pDst[1] = (Short)( (pSrcColor[1] * (Float)WORD_MAX) + (Float)SSHORT_MIN );
    pDst[2] = (Short)( (pSrcColor[2] * (Float)WORD_MAX) + (Float)SSHORT_MIN );
    pDst[3] = (Short)( (pSrcColor[3] * (Float)WORD_MAX) + (Float)SSHORT_MIN );
}

Void _ConvertTo_RG32_F( Byte * outDstColor, const Float pSrcColor[4] )
{
    Float * pDst = (Float*)outDstColor;
    pDst[0] = pSrcColor[0];
    pDst[1] = pSrcColor[1];
}
Void _ConvertTo_RG32_U( Byte * outDstColor, const Float pSrcColor[4] )
{
    DWord * pDst = (DWord*)outDstColor;
    pDst[0] = (DWord)( pSrcColor[0] * (Float)DWORD_MAX );
    pDst[1] = (DWord)( pSrcColor[1] * (Float)DWORD_MAX );
}
Void _ConvertTo_RG32_S( Byte * outDstColor, const Float pSrcColor[4] )
{
    Long * pDst = (Long*)outDstColor;
    pDst[0] = (Long)( (pSrcColor[0] * (Float)DWORD_MAX) + (Float)SLONG_MIN );
    pDst[1] = (Long)( (pSrcColor[1] * (Float)DWORD_MAX) + (Float)SLONG_MIN );
}

Void _ConvertTo_RGBA8_U( Byte * outDstColor, const Float pSrcColor[4] )
{
    outDstColor[0] = (Byte)( pSrcColor[0] * (Float)BYTE_MAX );
    outDstColor[1] = (Byte)( pSrcColor[1] * (Float)BYTE_MAX );
    outDstColor[2] = (Byte)( pSrcColor[2] * (Float)BYTE_MAX );
    outDstColor[3] = (Byte)( pSrcColor[3] * (Float)BYTE_MAX );
}
Void _ConvertTo_RGBA8_S( Byte * outDstColor, const Float pSrcColor[4] )
{
    Char * pDst = (Char*)outDstColor;
    pDst[0] = (Char)( (pSrcColor[0] * (Float)BYTE_MAX) + (Float)SCHAR_MIN );
    pDst[1] = (Char)( (pSrcColor[1] * (Float)BYTE_MAX) + (Float)SCHAR_MIN );
    pDst[2] = (Char)( (pSrcColor[2] * (Float)BYTE_MAX) + (Float)SCHAR_MIN );
    pDst[3] = (Char)( (pSrcColor[3] * (Float)BYTE_MAX) + (Float)SCHAR_MIN );
}
Void _ConvertTo_RGBA8_SRGB( Byte * outDstColor, const Float pSrcColor[4] )
{
    DebugAssert( false );
    outDstColor[0] = 0;
    outDstColor[1] = 0;
    outDstColor[2] = 0;
    outDstColor[3] = 0;
}

Void _ConvertTo_BGRA8_U( Byte * outDstColor, const Float pSrcColor[4] )
{
    outDstColor[0] = (Byte)( pSrcColor[2] * (Float)BYTE_MAX );
    outDstColor[1] = (Byte)( pSrcColor[1] * (Float)BYTE_MAX );
    outDstColor[2] = (Byte)( pSrcColor[0] * (Float)BYTE_MAX );
    outDstColor[3] = (Byte)( pSrcColor[3] * (Float)BYTE_MAX );
}
Void _ConvertTo_BGRA8_SRGB( Byte * outDstColor, const Float pSrcColor[4] )
{
    DebugAssert( false );
    outDstColor[0] = 0;
    outDstColor[1] = 0;
    outDstColor[2] = 0;
    outDstColor[3] = 0;
}

Void _ConvertTo_BGRX8_U( Byte * outDstColor, const Float pSrcColor[4] )
{
    outDstColor[0] = (Byte)( pSrcColor[2] * (Float)BYTE_MAX );
    outDstColor[1] = (Byte)( pSrcColor[1] * (Float)BYTE_MAX );
    outDstColor[2] = (Byte)( pSrcColor[0] * (Float)BYTE_MAX );
    outDstColor[3] = 0;
}
Void _ConvertTo_BGRX8_SRGB( Byte * outDstColor, const Float pSrcColor[4] )
{
    DebugAssert( false );
    outDstColor[0] = 0;
    outDstColor[1] = 0;
    outDstColor[2] = 0;
    outDstColor[3] = 0;
}

Void _ConvertTo_RGB10A2_U( Byte * outDstColor, const Float pSrcColor[4] )
{
    DWord * pDst = (DWord*)outDstColor;
    *pDst = (DWord)( pSrcColor[0] * (Float)_10BitsMAX );
    *pDst <<= 10;
    *pDst |= (DWord)( pSrcColor[1] * (Float)_10BitsMAX );
    *pDst <<= 10;
    *pDst |= (DWord)( pSrcColor[2] * (Float)_10BitsMAX );
    *pDst <<= 2;
    *pDst |= (DWord)( pSrcColor[3] * (Float)_2BitsMAX );
}

Void _ConvertTo_RG16_F( Byte * outDstColor, const Float pSrcColor[4] )
{
    DebugAssert( false );
    Word * pDst = (Word*)outDstColor;
    pDst[0] = 0;
    pDst[1] = 0;
}
Void _ConvertTo_RG16_U( Byte * outDstColor, const Float pSrcColor[4] )
{
    Word * pDst = (Word*)outDstColor;
    pDst[0] = (Word)( pSrcColor[0] * (Float)WORD_MAX );
    pDst[1] = (Word)( pSrcColor[1] * (Float)WORD_MAX );
}
Void _ConvertTo_RG16_S( Byte * outDstColor, const Float pSrcColor[4] )
{
    Short * pDst = (Short*)outDstColor;
    pDst[0] = (Short)( (pSrcColor[0] * (Float)WORD_MAX) + (Float)SSHORT_MIN );
    pDst[1] = (Short)( (pSrcColor[1] * (Float)WORD_MAX) + (Float)SSHORT_MIN );
}

Void _ConvertTo_R32_F( Byte * outDstColor, const Float pSrcColor[4] )
{
    Float * pDst = (Float*)outDstColor;
    *pDst = pSrcColor[0];
}
Void _ConvertTo_R32_U( Byte * outDstColor, const Float pSrcColor[4] )
{
    DWord * pDst = (DWord*)outDstColor;
    *pDst = (DWord)( pSrcColor[0] * (Float)DWORD_MAX );
}
Void _ConvertTo_R32_S( Byte * outDstColor, const Float pSrcColor[4] )
{
    Long * pDst = (Long*)outDstColor;
    *pDst = (Long)( (pSrcColor[0] * (Float)DWORD_MAX) + (Float)SLONG_MIN );
}

Void _ConvertTo_BGR5A1_U( Byte * outDstColor, const Float pSrcColor[4] )
{
    Word * pDst = (Word*)outDstColor;
    *pDst = (Word)( pSrcColor[2] * (Float)_5BitsMAX );
    *pDst <<= 5;
    *pDst |= (Word)( pSrcColor[1] * (Float)_5BitsMAX );
    *pDst <<= 5;
    *pDst |= (Word)( pSrcColor[0] * (Float)_5BitsMAX );
    *pDst <<= 1;
    *pDst |= (Word)( pSrcColor[3] * (Float)_1BitsMAX );
}

Void _ConvertTo_B5G6R5_U( Byte * outDstColor, const Float pSrcColor[4] )
{
    Word * pDst = (Word*)outDstColor;
    *pDst = (Word)( pSrcColor[2] * (Float)_5BitsMAX );
    *pDst <<= 6;
    *pDst |= (Word)( pSrcColor[1] * (Float)_6BitsMAX );
    *pDst <<= 5;
    *pDst |= (Word)( pSrcColor[0] * (Float)_5BitsMAX );
}

Void _ConvertTo_RG8_U( Byte * outDstColor, const Float pSrcColor[4] )
{
    outDstColor[0] = (Byte)( pSrcColor[0] * (Float)BYTE_MAX );
    outDstColor[1] = (Byte)( pSrcColor[1] * (Float)BYTE_MAX );
}
Void _ConvertTo_RG8_S( Byte * outDstColor, const Float pSrcColor[4] )
{
    Char * pDst = (Char*)outDstColor;
    pDst[0] = (Char)( (pSrcColor[0] * (Float)BYTE_MAX) + (Float)SCHAR_MIN );
    pDst[1] = (Char)( (pSrcColor[1] * (Float)BYTE_MAX) + (Float)SCHAR_MIN );
}

Void _ConvertTo_R16_F( Byte * outDstColor, const Float pSrcColor[4] )
{
    DebugAssert( false );
    Word * pDst = (Word*)outDstColor;
    *pDst = 0;
}
Void _ConvertTo_R16_U( Byte * outDstColor, const Float pSrcColor[4] )
{
    Word * pDst = (Word*)outDstColor;
    *pDst = (Word)( pSrcColor[0] * (Float)WORD_MAX );
}
Void _ConvertTo_R16_S( Byte * outDstColor, const Float pSrcColor[4] )
{
    Short * pDst = (Short*)outDstColor;
    *pDst = (Short)( (pSrcColor[0] * (Float)WORD_MAX) + (Float)SSHORT_MIN );
}

Void _ConvertTo_R8_U( Byte * outDstColor, const Float pSrcColor[4] )
{
    *outDstColor = (Byte)( pSrcColor[0] * (Float)BYTE_MAX );
}
Void _ConvertTo_R8_S( Byte * outDstColor, const Float pSrcColor[4] )
{
    Char * pDst = (Char*)outDstColor;
    *pDst = (Char)( (pSrcColor[0] * (Float)BYTE_MAX) + (Float)SCHAR_MIN );
}

Void _ConvertTo_A8_U( Byte * outDstColor, const Float pSrcColor[4] )
{
    *outDstColor = (Byte)( pSrcColor[3] * (Float)BYTE_MAX );
}

Void _ConvertTo_D32S8X24_FU( Byte * outDstColor, const Float pSrcColor[4] )
{
    Float * pDst0 = (Float*)outDstColor;
    Byte * pDst1 = ( outDstColor + 4 );
    *pDst0 = pSrcColor[0];
    *pDst1 = (Byte)( pSrcColor[1] ); // Never scale stencil
    outDstColor[5] = 0;
    outDstColor[6] = 0;
    outDstColor[7] = 0;
}
Void _ConvertTo_D24S8_U( Byte * outDstColor, const Float pSrcColor[4] )
{
    DWord * pDst = (DWord*)outDstColor;
    *pDst = (DWord)( pSrcColor[0] * (Float)_24BitsMAX );
    *pDst <<= 8;
    *pDst |= (DWord)( (Byte)(pSrcColor[1]) ); // Never scale stencil
}
Void _ConvertTo_D32_F( Byte * outDstColor, const Float pSrcColor[4] )
{
    Float * pDst = (Float*)outDstColor;
    *pDst = pSrcColor[0];
}
Void _ConvertTo_D16_U( Byte * outDstColor, const Float pSrcColor[4] )
{
    Word * pDst = (Word*)outDstColor;
    *pDst = (Word)( pSrcColor[0] * (Float)WORD_MAX );
}

Void _ConvertTo_R32X8X24_F( Byte * outDstColor, const Float pSrcColor[4] )
{
    Float * pDst = (Float*)outDstColor;
    *pDst = pSrcColor[0];
    outDstColor[4] = 0;
    outDstColor[5] = 0;
    outDstColor[6] = 0;
    outDstColor[7] = 0;
}
Void _ConvertTo_X32G8X24_U( Byte * outDstColor, const Float pSrcColor[4] )
{
    DWord * pDst0 = (DWord*)outDstColor;
    Byte * pDst1 = (Byte*)( outDstColor + 4 );
    *pDst0 = 0;
    *pDst1 = (Byte)( pSrcColor[1] * (Float)BYTE_MAX );
    outDstColor[5] = 0;
    outDstColor[6] = 0;
    outDstColor[7] = 0;
}
Void _ConvertTo_R24X8_U( Byte * outDstColor, const Float pSrcColor[4] )
{
    DWord * pDst = (DWord*)outDstColor;
    *pDst = (DWord)( pSrcColor[0] * (Float)_24BitsMAX );
    *pDst <<= 8;
}
Void _ConvertTo_X24G8_U( Byte * outDstColor, const Float pSrcColor[4] )
{
    Byte * pDst = (Byte*)( outDstColor + 3 );
    *pDst = (Byte)( pSrcColor[1] * (Float)BYTE_MAX );
}

Void _ConvertTo_RG8BG8_U( Byte * outDstColor, const Float pSrcColor[4] )
{
    outDstColor[0] = (Byte)( pSrcColor[0] * (Float)BYTE_MAX );
    outDstColor[1] = (Byte)( pSrcColor[1] * (Float)BYTE_MAX );
    outDstColor[2] = (Byte)( pSrcColor[2] * (Float)BYTE_MAX );
    outDstColor[3] = (Byte)( pSrcColor[3] * (Float)BYTE_MAX );
}
Void _ConvertTo_GR8GB8_U( Byte * outDstColor, const Float pSrcColor[4] )
{
    outDstColor[0] = (Byte)( pSrcColor[1] * (Float)BYTE_MAX );
    outDstColor[1] = (Byte)( pSrcColor[0] * (Float)BYTE_MAX );
    outDstColor[2] = (Byte)( pSrcColor[3] * (Float)BYTE_MAX );
    outDstColor[3] = (Byte)( pSrcColor[2] * (Float)BYTE_MAX );
}

Void _ConvertTo_RGB10XRBIAS_A2_U( Byte * outDstColor, const Float pSrcColor[4] )
{
    DWord * pDst = (DWord*)outDstColor;
    *pDst = ( 0x180ul + (DWord)(pSrcColor[0] * 510.0f) ) & 0x3fful;
    *pDst <<= 10;
    *pDst |= ( 0x180ul + (DWord)(pSrcColor[1] * 510.0f) ) & 0x3fful;
    *pDst <<= 10;
    *pDst |= ( 0x180ul + (DWord)(pSrcColor[2] * 510.0f) ) & 0x3fful;
    *pDst <<= 2;
    *pDst |= (DWord)( pSrcColor[3] * (Float)_2BitsMAX );
}

Void _ConvertTo_RG11B10_F( Byte * outDstColor, const Float pSrcColor[4] )
{
    DebugAssert( false );
    outDstColor[0] = 0;
    outDstColor[1] = 0;
    outDstColor[2] = 0;
    outDstColor[3] = 0;
}
Void _ConvertTo_RGB9EXP5_F( Byte * outDstColor, const Float pSrcColor[4] )
{
    DebugAssert( false );
    outDstColor[0] = 0;
    outDstColor[1] = 0;
    outDstColor[2] = 0;
    outDstColor[3] = 0;
}

Void _ConvertTo_P8_U( Byte * outDstColor, const Float pSrcColor[4] )
{
    *outDstColor = (Byte)( pSrcColor[0] * (Float)BYTE_MAX );
}
Void _ConvertTo_AP8_U( Byte * outDstColor, const Float pSrcColor[4] )
{
    outDstColor[0] = (Byte)( pSrcColor[3] * (Float)BYTE_MAX );
    outDstColor[1] = (Byte)( pSrcColor[0] * (Float)BYTE_MAX );
}

Void _ConvertTo_BGRA4_U( Byte * outDstColor, const Float pSrcColor[4] )
{
    Word * pDst = (Word*)outDstColor;
    *pDst = (Word)( pSrcColor[2] * (Float)_4BitsMAX );
    *pDst <<= 4;
    *pDst |= (Word)( pSrcColor[1] * (Float)_4BitsMAX );
    *pDst <<= 4;
    *pDst |= (Word)( pSrcColor[0] * (Float)_4BitsMAX );
    *pDst <<= 4;
    *pDst |= (Word)( pSrcColor[3] * (Float)_4BitsMAX );
}

Void PixelFormatConvertTo( Byte * outDstColor, const Float pSrcColor[4], PixelFormat iDstFormat, UInt iCount )
{
    static _PixelFormatConvertTo sm_arrConvertTo[PIXEL_FMT_COUNT] = {
        NULL,
        NULL, _ConvertTo_RGBA32_F, _ConvertTo_RGBA32_U, _ConvertTo_RGBA32_S,
        NULL, _ConvertTo_RGB32_F, _ConvertTo_RGB32_U, _ConvertTo_RGB32_S,
        NULL, _ConvertTo_RGBA16_F, _ConvertTo_RGBA16_U, _ConvertTo_RGBA16_U, _ConvertTo_RGBA16_S, _ConvertTo_RGBA16_S,
        NULL, _ConvertTo_RG32_F, _ConvertTo_RG32_U, _ConvertTo_RG32_S,
        NULL, _ConvertTo_RGBA8_U, _ConvertTo_RGBA8_SRGB, _ConvertTo_RGBA8_U, _ConvertTo_RGBA8_S, _ConvertTo_RGBA8_S,
        NULL, _ConvertTo_BGRA8_U, _ConvertTo_BGRA8_SRGB,
        NULL, _ConvertTo_BGRX8_U, _ConvertTo_BGRX8_SRGB,
        NULL, _ConvertTo_RGB10A2_U, _ConvertTo_RGB10A2_U,
        NULL, _ConvertTo_RG16_F, _ConvertTo_RG16_U, _ConvertTo_RG16_U, _ConvertTo_RG16_S, _ConvertTo_RG16_S,
        NULL, _ConvertTo_R32_F, _ConvertTo_R32_U, _ConvertTo_R32_S,
        _ConvertTo_BGR5A1_U,
        _ConvertTo_B5G6R5_U,
        NULL, _ConvertTo_RG8_U, _ConvertTo_RG8_U, _ConvertTo_RG8_S, _ConvertTo_RG8_S,
        NULL, _ConvertTo_R16_F, _ConvertTo_R16_U, _ConvertTo_R16_U, _ConvertTo_R16_S, _ConvertTo_R16_S,
        NULL, _ConvertTo_R8_U, _ConvertTo_R8_U, _ConvertTo_R8_S, _ConvertTo_R8_S,
        _ConvertTo_A8_U,
        NULL,
        _ConvertTo_D32S8X24_FU,
        _ConvertTo_D24S8_U,
        _ConvertTo_D32_F,
        _ConvertTo_D16_U,
        NULL, _ConvertTo_R32X8X24_F, _ConvertTo_X32G8X24_U,
        NULL, _ConvertTo_R24X8_U, _ConvertTo_X24G8_U,
        _ConvertTo_RG8BG8_U,
        _ConvertTo_GR8GB8_U,
        _ConvertTo_RGB10XRBIAS_A2_U,
        _ConvertTo_RG11B10_F,
        _ConvertTo_RGB9EXP5_F,
        _ConvertTo_P8_U, _ConvertTo_AP8_U,
        _ConvertTo_BGRA4_U,
        NULL, NULL, NULL,
        NULL, NULL, NULL,
        NULL, NULL, NULL,
        NULL, NULL, NULL,
        NULL, NULL, NULL,
        NULL, NULL, NULL,
        NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL,
        NULL
    };
    UInt iChannelCount = PixelFormatChannelCount( iDstFormat );
    UInt iByteCount = PixelFormatBytes( iDstFormat );

    _PixelFormatConvertTo pfConvertTo = sm_arrConvertTo[iDstFormat];
    DebugAssert( pfConvertTo != NULL );

    for( UInt i = 0; i < iCount; ++i ) {
        pfConvertTo( outDstColor, pSrcColor );
        outDstColor += iByteCount;
        pSrcColor += iChannelCount;
    }
}

Void PixelFormatLERP( Byte * outDstColor, const Byte ** arrColors, UInt iCount, PixelFormat iInFormat, PixelFormat iOutFormat )
{
    DebugAssert( iCount > 0 );

    UInt k, iPixelFormatChannels = PixelFormatChannelCount( iInFormat );
    Float tmpConv[4], tmpLERP[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

    for( UInt i = 0; i < iCount; ++i ) {
        PixelFormatConvertFrom( tmpConv, arrColors[i], iInFormat, 1 );

        for( k = 0; k < iPixelFormatChannels; ++k )
            tmpLERP[k] += tmpConv[k];
    }

    Float fScale = ( 1.0f / (Float)iCount );
    for( k = 0; k < iPixelFormatChannels; ++k )
        tmpLERP[k] *= fScale;

    if ( iOutFormat == PIXEL_FMT_UNKNOWN )
        iOutFormat = iInFormat;
    PixelFormatConvertTo( outDstColor, tmpLERP, iOutFormat, 1 );
}
