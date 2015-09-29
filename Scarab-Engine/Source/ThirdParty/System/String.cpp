/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/System/String.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : String API providing Legacy & Unicode support
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
#include <windows.h>

#undef DebugAssert

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "String.h"

/////////////////////////////////////////////////////////////////////////////////
// String implementation
String::String()
{
    // nothing to do
}
String::~String()
{
    // nothing to do
}

WChar * String::MultiByteToWideChar( WChar * outStr, const MBChar * inStr, UInt iLength ) const
{
    Int iCount = ::MultiByteToWideChar( CP_UTF8, 0, inStr, -1, outStr, iLength );
    return ( outStr + (iCount - 1) );
}
MBChar * String::WideCharToMultiByte( MBChar * outStr, const WChar * inStr, UInt iLength ) const
{
    Int iCount = ::WideCharToMultiByte( CP_UTF8, 0, inStr, -1, outStr, iLength, NULL, NULL );
    return ( outStr + (iCount - 1) );
}

const WChar * String::NextCharW( const WChar * inStr ) const
{
    return CharNextW( inStr );
}
const MBChar * String::NextCharMB( const MBChar * inStr ) const
{
    return CharNextA( inStr );
}

const WChar * String::PrevCharW( const WChar * inStart, const WChar * inStr ) const
{
    return CharPrevW( inStart, inStr );
}
const MBChar * String::PrevCharMB( const MBChar * inStart, const MBChar * inStr ) const
{
    return CharPrevA( inStart, inStr );
}

CharacterClass String::GetClassW( WChar ch ) const
{
    WORD wType = 0;
    BOOL bRes = GetStringTypeExW( LOCALE_SYSTEM_DEFAULT, CT_CTYPE1, &ch, 1, &wType );
    DebugAssert( bRes != FALSE );

    CharacterClass chClass = CHARACTER_CLASS_UNKNOWN;
    if ( wType & C1_CNTRL )
        chClass |= CHARACTER_CLASS_CONTROL;
    if ( wType & C1_ALPHA )
        chClass |= CHARACTER_CLASS_ALPHA;
    if ( wType & C1_UPPER )
        chClass |= CHARACTER_CLASS_UPPERCASE;
    if ( wType & C1_LOWER )
        chClass |= CHARACTER_CLASS_LOWERCASE;
    if ( wType & C1_DIGIT )
        chClass |= CHARACTER_CLASS_DIGIT;
    if ( wType & C1_XDIGIT )
        chClass |= CHARACTER_CLASS_HEXDIGIT;
    if ( wType & C1_PUNCT )
        chClass |= CHARACTER_CLASS_PUNCTUATION;
    if ( wType & C1_BLANK )
        chClass |= CHARACTER_CLASS_BLANK;
    if ( wType & C1_SPACE )
        chClass |= CHARACTER_CLASS_SPACE;
    if ( wType & C1_DEFINED )
        chClass |= CHARACTER_CLASS_SPECIAL;

    return chClass;
}
CharacterClass String::GetClassMB( MBChar ch ) const
{
    WORD wType = 0;
    BOOL bRes = GetStringTypeExA( LOCALE_SYSTEM_DEFAULT, CT_CTYPE1, &ch, 1, &wType );
    DebugAssert( bRes != FALSE );

    CharacterClass chClass = CHARACTER_CLASS_UNKNOWN;
    if ( wType & C1_UPPER )
        chClass |= CHARACTER_CLASS_UPPERCASE;
    if ( wType & C1_LOWER )
        chClass |= CHARACTER_CLASS_LOWERCASE;
    if ( wType & C1_DIGIT )
        chClass |= CHARACTER_CLASS_DIGIT;
    if ( wType & C1_XDIGIT )
        chClass |= CHARACTER_CLASS_HEXDIGIT;
    if ( wType & C1_PUNCT )
        chClass |= CHARACTER_CLASS_PUNCTUATION;
    if ( wType & C1_BLANK )
        chClass |= CHARACTER_CLASS_BLANK;
    if ( wType & C1_SPACE )
        chClass |= CHARACTER_CLASS_SPACE;
    if ( wType & C1_CNTRL )
        chClass |= CHARACTER_CLASS_CONTROL;

    if ( wType & C1_ALPHA )
        chClass |= CHARACTER_CLASS_ALPHA;
    if ( wType & C1_DEFINED )
        chClass |= CHARACTER_CLASS_SPECIAL;

    return chClass;
}

WChar String::ToLowerCaseW( WChar ch ) const
{
    return (WChar)CharLowerW( (LPWSTR)ch );
}
MBChar String::ToLowerCaseMB( MBChar ch ) const
{
    return (MBChar)CharLowerA( (LPSTR)ch );
}

WChar String::ToUpperCaseW( WChar ch ) const
{
    return (WChar)CharUpperW( (LPWSTR)ch );
}
MBChar String::ToUpperCaseMB( MBChar ch ) const
{
    return (MBChar)CharUpperA( (LPSTR)ch );
}

UInt String::LengthW( const WChar * inStr ) const
{
    UInt iLength = 0;
    while( *inStr++ != WNULLBYTE )
        ++iLength;
    return iLength;
}
UInt String::LengthMB( const MBChar * inStr ) const
{
    UInt iLength = 0;
    while( *inStr++ != MBNULLBYTE )
        ++iLength;
    return iLength;
}

WChar * String::CopyW( WChar * strTo, const WChar * strFrom ) const
{
    while( *strFrom != WNULLBYTE )
        *strTo++ = *strFrom++;
    *strTo = WNULLBYTE;
    return strTo;
}
MBChar * String::CopyMB( MBChar * strTo, const MBChar * strFrom ) const
{
    while( *strFrom != MBNULLBYTE )
        *strTo++ = *strFrom++;
    *strTo = MBNULLBYTE;
    return strTo;
}

WChar * String::NCopyW( WChar * strTo, const WChar * strFrom, UInt iLength ) const
{
    UInt iCount = 0;
    while( iCount < iLength && *strFrom != WNULLBYTE ) {
        *strTo++ = *strFrom++;
        ++iCount;
    }
    *strTo = WNULLBYTE;
    return strTo;
}
MBChar * String::NCopyMB( MBChar * strTo, const MBChar * strFrom, UInt iLength ) const
{
    UInt iCount = 0;
    while( iCount < iLength && *strFrom != MBNULLBYTE ) {
        *strTo++ = *strFrom++;
        ++iCount;
    }
    *strTo = MBNULLBYTE;
    return strTo;
}

Int String::CmpW( const WChar * strLeft, const WChar * strRight ) const
{
    while( *strLeft == *strRight ) {
        if ( *strLeft == WNULLBYTE )
            return 0;
        ++strLeft; ++strRight;
    }
    return ( *strLeft < *strRight ) ? +1 : -1;
}
Int String::CmpMB( const MBChar * strLeft, const MBChar * strRight ) const
{
    while( *strLeft == *strRight ) {
        if ( *strLeft == MBNULLBYTE )
            return 0;
        ++strLeft; ++strRight;
    }
    return ( *strLeft < *strRight ) ? +1 : -1;
}

Int String::NCmpW( const WChar * strLeft, const WChar * strRight, UInt iLength ) const
{
    if ( iLength == 0 )
        return 0;
    UInt iCount = 0;
    while( *strLeft == *strRight ) {
        ++iCount;
        if ( iCount >= iLength || *strLeft == WNULLBYTE )
            return 0;
        ++strLeft; ++strRight;
    }
    return ( *strLeft < *strRight ) ? +1 : -1;
}
Int String::NCmpMB( const MBChar * strLeft, const MBChar * strRight, UInt iLength ) const
{
    if ( iLength == 0 )
        return 0;
    UInt iCount = 0;
    while( *strLeft == *strRight ) {
        ++iCount;
        if ( iCount >= iLength || *strLeft == MBNULLBYTE )
            return 0;
        ++strLeft; ++strRight;
    }
    return ( *strLeft < *strRight ) ? +1 : -1;
}

const WChar * String::FindW( const WChar * inStr, WChar ch ) const
{
    while( *inStr != WNULLBYTE ) {
        if ( *inStr == ch )
            return inStr;
        ++inStr;
    }
    return NULL;
}
const MBChar * String::FindMB( const MBChar * inStr, MBChar ch ) const
{
    while( *inStr != MBNULLBYTE ) {
        if ( *inStr == ch )
            return inStr;
        ++inStr;
    }
    return NULL;
}

Void String::ReverseW( WChar * pStr, UInt iLength ) const
{
    WChar ch;
    WChar * strEnd = pStr + iLength - 1;
    while( pStr < strEnd ) {
        ch = *pStr;
        *pStr = *strEnd;
        *strEnd = ch;
        ++pStr; --strEnd;
    }
}
Void String::ReverseMB( MBChar * pStr, UInt iLength ) const
{
    MBChar ch;
    MBChar * strEnd = pStr + iLength - 1;
    while( pStr < strEnd ) {
        ch = *pStr;
        *pStr = *strEnd;
        *strEnd = ch;
        ++pStr; --strEnd;
    }
}

UInt64 String::ToUIntW( const WChar * inStr, const WChar ** outStr ) const
{
    // Parse prefix
    Int64 iRadix = 10;
    if ( *inStr == WTEXT('0') ) {
        ++inStr;
        switch( *inStr ) {
            case WTEXT('b'): case WTEXT('B'): iRadix = 2; ++inStr; break;
            case WTEXT('o'): case WTEXT('O'): iRadix = 8; ++inStr; break;
            case WTEXT('d'): case WTEXT('D'): iRadix = 10; ++inStr; break;
            case WTEXT('x'): case WTEXT('X'): iRadix = 16; ++inStr; break;
            default: break;
        }
        while( *inStr == WTEXT('0') )
            ++inStr;
    }

    // Parse value
    Int64 iValue = 0;
    while( *inStr != WNULLBYTE ) {
        if ( iRadix ==  2 ) {
            if ( !IsBinDigitW(*inStr) )
                break;
            iValue = (iValue << 1) + (Int64)CharToBinW(*inStr);
        } else if ( iRadix ==  8 ) {
            if ( !IsOctDigitW(*inStr) )
                break;
            iValue = (iValue << 3) + (Int64)CharToOctW(*inStr);
        } else if ( iRadix ==  10 ) {
            if ( !IsDecDigitW(*inStr) )
                break;
            iValue = (iValue << 3) + (iValue << 1) + (Int64)CharToDecW(*inStr);
        } else if ( iRadix ==  16 ) {
            if ( !IsHexDigitW(*inStr) )
                break;
            iValue = (iValue << 4) + (Int64)CharToHexW(*inStr);
        } else
            DebugAssert( false );
        ++inStr;
    }

    if ( outStr != NULL )
        *outStr = inStr;
    return iValue;
}
UInt64 String::ToUIntMB( const MBChar * inStr, const MBChar ** outStr ) const
{
    // Parse prefix
    Int64 iRadix = 10;
    if ( *inStr == MBTEXT('0') ) {
        ++inStr;
        switch( *inStr ) {
            case MBTEXT('b'): case MBTEXT('B'): iRadix = 2; ++inStr; break;
            case MBTEXT('o'): case MBTEXT('O'): iRadix = 8; ++inStr; break;
            case MBTEXT('d'): case MBTEXT('D'): iRadix = 10; ++inStr; break;
            case MBTEXT('x'): case MBTEXT('X'): iRadix = 16; ++inStr; break;
            default: break;
        }
        while( *inStr == MBTEXT('0') )
            ++inStr;
    }

    // Parse value
    Int64 iValue = 0;
    while( *inStr != MBNULLBYTE ) {
        if ( iRadix ==  2 ) {
            if ( !IsBinDigitMB(*inStr) )
                break;
            iValue = (iValue << 1) + (Int64)CharToBinMB(*inStr);
        } else if ( iRadix ==  8 ) {
            if ( !IsOctDigitMB(*inStr) )
                break;
            iValue = (iValue << 3) + (Int64)CharToOctMB(*inStr);
        } else if ( iRadix ==  10 ) {
            if ( !IsDecDigitMB(*inStr) )
                break;
            iValue = (iValue << 3) + (iValue << 1) + (Int64)CharToDecMB(*inStr);
        } else if ( iRadix ==  16 ) {
            if ( !IsHexDigitMB(*inStr) )
                break;
            iValue = (iValue << 4) + (Int64)CharToHexMB(*inStr);
        } else
            DebugAssert( false );
        ++inStr;
    }

    if ( outStr != NULL )
        *outStr = inStr;
    return iValue;
}

Int64 String::ToIntW( const WChar * inStr, const WChar ** outStr ) const
{
    // Parse sign
    Bool bNegative = false;
    if ( *inStr == WTEXT('-') ) {
        bNegative = true;
        ++inStr;
    } else if ( *inStr == WTEXT('+') )
        ++inStr;

    Int64 iValue = (Int64)ToUIntW( inStr, outStr );
    return bNegative ? -iValue : iValue;
}
Int64 String::ToIntMB( const MBChar * inStr, const MBChar ** outStr ) const
{
    // Parse sign
    Bool bNegative = false;
    if ( *inStr == MBTEXT('-') ) {
        bNegative = true;
        ++inStr;
    } else if ( *inStr == MBTEXT('+') )
        ++inStr;

    Int64 iValue = (Int64)ToUIntMB( inStr, outStr );
    return bNegative ? -iValue : iValue;
}

Float String::ToFloatW( const WChar * inStr, const WChar ** outStr ) const
{
    // Parse sign
    Bool bNegative = false;
    if ( *inStr == WTEXT('-') ) {
        bNegative = true;
        ++inStr;
    } else if ( *inStr == WTEXT('+') )
        ++inStr;

    // Parse integral part
    Float fValue = 0.0f;
    while( IsDecDigitW(*inStr) )
        fValue = (fValue * 10.0f) + (Float)CharToDecW(*inStr++);

    // Parse decimal part
    if ( *inStr == WTEXT('.') ) {
        ++inStr;
        Float fDecimal = 0.1f;
        while( IsDecDigitW(*inStr) ) {
            fValue += fDecimal * (Float)CharToDecW(*inStr++);
            fDecimal *= 0.1f;
        }
    }

    // Parse exponent part
    if ( *inStr == WTEXT('e') || *inStr == WTEXT('E') ) {
        ++inStr;
        Bool bNegativeExponent = false;
        if ( *inStr == WTEXT('-') ) {
            bNegativeExponent = true;
            ++inStr;
        } else if ( *inStr == WTEXT('+') )
            ++inStr;
        Int iExponent = 0;
        while( IsDecDigitW(*inStr) )
            iExponent = (iExponent << 3) + (iExponent << 1) + (Int)CharToDecW(*inStr++);
        if ( bNegativeExponent )
            iExponent = -iExponent;
        fValue *= FPUFn->Power10f( iExponent );
    }

    if ( outStr != NULL )
        *outStr = inStr;
    return bNegative ? -fValue : fValue;
}
Float String::ToFloatMB( const MBChar * inStr, const MBChar ** outStr ) const
{
    // Parse sign
    Bool bNegative = false;
    if ( *inStr == MBTEXT('-') ) {
        bNegative = true;
        ++inStr;
    } else if ( *inStr == MBTEXT('+') )
        ++inStr;

    // Parse integral part
    Float fValue = 0.0f;
    while( IsDecDigitMB(*inStr) )
        fValue = (fValue * 10.0f) + (Float)CharToDecMB(*inStr++);

    // Parse decimal part
    if ( *inStr == MBTEXT('.') ) {
        ++inStr;
        Float fDecimal = 0.1f;
        while( IsDecDigitMB(*inStr) ) {
            fValue += fDecimal * (Float)CharToDecMB(*inStr++);
            fDecimal *= 0.1f;
        }
    }

    // Parse exponent part
    if ( *inStr == MBTEXT('e') || *inStr == MBTEXT('E') ) {
        ++inStr;
        Bool bNegativeExponent = false;
        if ( *inStr == MBTEXT('-') ) {
            bNegativeExponent = true;
            ++inStr;
        } else if ( *inStr == MBTEXT('+') )
            ++inStr;
        Int iExponent = 0;
        while( IsDecDigitMB(*inStr) )
            iExponent = (iExponent << 3) + (iExponent << 1) + (Int)CharToDecMB(*inStr++);
        if ( bNegativeExponent )
            iExponent = -iExponent;
        fValue *= FPUFn->Power10f( iExponent );
    }

    if ( outStr != NULL )
        *outStr = inStr;
    return bNegative ? -fValue : fValue;
}

Double String::ToDoubleW( const WChar * inStr, const WChar ** outStr ) const
{
    // Parse sign
    Bool bNegative = false;
    if ( *inStr == WTEXT('-') ) {
        bNegative = true;
        ++inStr;
    } else if ( *inStr == WTEXT('+') )
        ++inStr;

    // Parse integral part
    Double fValue = 0.0;
    while( IsDecDigitW(*inStr) )
        fValue = (fValue * 10.0) + (Double)CharToDecW(*inStr++);

    // Parse decimal part
    if ( *inStr == WTEXT('.') ) {
        ++inStr;
        Double fDecimal = 0.1;
        while( IsDecDigitW(*inStr) ) {
            fValue += fDecimal * (Double)CharToDecW(*inStr++);
            fDecimal *= 0.1;
        }
    }

    // Parse exponent part
    if ( *inStr == WTEXT('e') || *inStr == WTEXT('E') ) {
        ++inStr;
        Bool bNegativeExponent = false;
        if ( *inStr == WTEXT('-') ) {
            bNegativeExponent = true;
            ++inStr;
        } else if ( *inStr == WTEXT('+') )
            ++inStr;
        Int iExponent = 0;
        while( IsDecDigitW(*inStr) )
            iExponent = (iExponent << 3) + (iExponent << 1) + (Int)CharToDecW(*inStr++);
        if ( bNegativeExponent )
            iExponent = -iExponent;
        fValue *= FPUFn->Power10d( iExponent );
    }

    if ( outStr != NULL )
        *outStr = inStr;
    return bNegative ? -fValue : fValue;
}
Double String::ToDoubleMB( const MBChar * inStr, const MBChar ** outStr ) const
{
    // Parse sign
    Bool bNegative = false;
    if ( *inStr == MBTEXT('-') ) {
        bNegative = true;
        ++inStr;
    } else if ( *inStr == MBTEXT('+') )
        ++inStr;

    // Parse integral part
    Double fValue = 0.0;
    while( IsDecDigitMB(*inStr) )
        fValue = (fValue * 10.0) + (Double)CharToDecMB(*inStr++);

    // Parse decimal part
    if ( *inStr == MBTEXT('.') ) {
        ++inStr;
        Double fDecimal = 0.1;
        while( IsDecDigitMB(*inStr) ) {
            fValue += fDecimal * (Double)CharToDecMB(*inStr++);
            fDecimal *= 0.1;
        }
    }

    // Parse exponent part
    if ( *inStr == MBTEXT('e') || *inStr == MBTEXT('E') ) {
        ++inStr;
        Bool bNegativeExponent = false;
        if ( *inStr == MBTEXT('-') ) {
            bNegativeExponent = true;
            ++inStr;
        } else if ( *inStr == MBTEXT('+') )
            ++inStr;
        Int iExponent = 0;
        while( IsDecDigitMB(*inStr) )
            iExponent = (iExponent << 3) + (iExponent << 1) + (Int)CharToDecMB(*inStr++);
        if ( bNegativeExponent )
            iExponent = -iExponent;
        fValue *= FPUFn->Power10d( iExponent );
    }

    if ( outStr != NULL )
        *outStr = inStr;
    return bNegative ? -fValue : fValue;
}

WChar * String::FromUIntW( WChar * outStr, UInt64 iValue, Radix iRadix, UInt iMinDigits ) const
{
    // Print prefix
    switch( iRadix ) {
        case RADIX_BINARY:      *outStr++ = WTEXT('0'); *outStr++ = WTEXT('b'); break;
        case RADIX_OCTAL:       *outStr++ = WTEXT('0'); *outStr++ = WTEXT('o'); break;
        case RADIX_HEXADECIMAL: *outStr++ = WTEXT('0'); *outStr++ = WTEXT('x'); break;
        case RADIX_DECIMAL:     break; // no prefix
        default: DebugAssert( false ); break;
    }

    WChar * pMin = outStr + iMinDigits;

    // Zero case
    if ( iValue == 0 ) {
        *outStr++ = WTEXT('0');
        while( outStr < pMin )
            *outStr++ = WTEXT('0');
        *outStr = WNULLBYTE;
        return outStr;
    }

    // Print digits
    WChar * pFirst = outStr;
    UInt64 iBase = (UInt64)iRadix;
    UInt64 iDigit = 0;
    while( iValue != 0 ) {
        iDigit = iValue % iBase;
        if ( iRadix == RADIX_HEXADECIMAL && iDigit >= 10 )
            *outStr++ = ( WTEXT('a') + (WChar)(iDigit - 10) );
        else
            *outStr++ = ( WTEXT('0') + (WChar)iDigit );
        iValue -= iDigit;
        iValue /= iBase;
    }
    while( outStr < pMin )
        *outStr++ = WTEXT('0');
    ReverseW( pFirst, (outStr - pFirst) );

    *outStr = WNULLBYTE;
    return outStr;
}
MBChar * String::FromUIntMB( MBChar * outStr, UInt64 iValue, Radix iRadix, UInt iMinDigits ) const
{
    // Print prefix
    switch( iRadix ) {
        case RADIX_BINARY:      *outStr++ = MBTEXT('0'); *outStr++ = MBTEXT('b'); break;
        case RADIX_OCTAL:       *outStr++ = MBTEXT('0'); *outStr++ = MBTEXT('o'); break;
        case RADIX_HEXADECIMAL: *outStr++ = MBTEXT('0'); *outStr++ = MBTEXT('x'); break;
        case RADIX_DECIMAL:     break; // no prefix
        default: DebugAssert( false ); break;
    }

    MBChar * pMin = outStr + iMinDigits;

    // Zero case
    if ( iValue == 0 ) {
        *outStr++ = MBTEXT('0');
        while( outStr < pMin )
            *outStr++ = MBTEXT('0');
        *outStr = MBNULLBYTE;
        return outStr;
    }

    // Print digits
    MBChar * pFirst = outStr;
    UInt64 iBase = (UInt64)iRadix;
    UInt64 iDigit = 0;
    while( iValue != 0 ) {
        iDigit = iValue % iBase;
        if ( iRadix == RADIX_HEXADECIMAL && iDigit >= 10 )
            *outStr++ = ( MBTEXT('a') + (MBChar)(iDigit - 10) );
        else
            *outStr++ = ( MBTEXT('0') + (MBChar)iDigit );
        iValue -= iDigit;
        iValue /= iBase;
    }
    while( outStr < pMin )
        *outStr++ = MBTEXT('0');
    ReverseMB( pFirst, (outStr - pFirst) );

    *outStr = MBNULLBYTE;
    return outStr;
}

WChar * String::FromIntW( WChar * outStr, Int64 iValue, Radix iRadix, UInt iMinDigits ) const
{
    // Print sign
    UInt64 iAbsValue = 0;
    if ( iValue < 0 ) {
        iAbsValue = (UInt64)(-iValue);
        *outStr++ = WTEXT('-');
    } else
        iAbsValue = (UInt64)iValue;

    return FromUIntW( outStr, iAbsValue, iRadix, iMinDigits );
}
MBChar * String::FromIntMB( MBChar * outStr, Int64 iValue, Radix iRadix, UInt iMinDigits ) const
{
    // Print sign
    UInt64 iAbsValue = 0;
    if ( iValue < 0 ) {
        iAbsValue = (UInt64)(-iValue);
        *outStr++ = MBTEXT('-');
    } else
        iAbsValue = (UInt64)iValue;

    return FromUIntMB( outStr, iAbsValue, iRadix, iMinDigits );
}

WChar * String::FromFloatW( WChar * outStr, Float fValue, UInt iMinDigits, UInt iMaxDigits ) const
{
    // Clamp digits range
    DebugAssert( iMinDigits <= iMaxDigits );
    if ( iMinDigits < 1 )  iMinDigits = 1;
	if ( iMaxDigits > 16 ) iMaxDigits = 16;

    // Zero case
    if ( fValue == 0.0f || fValue == -0.0f ) {
        *outStr++ = WTEXT('0');
        *outStr++ = WTEXT('.');
        *outStr++ = WTEXT('0');
        while( iMinDigits > 2 ) {
            *outStr++ = WTEXT('0');
            --iMinDigits;
        }
        *outStr = WNULLBYTE;
        return outStr;
    }

    // Print sign
    if ( fValue < 0.0f ) {
        fValue = -fValue;
        *outStr++ = WTEXT('-');
    }

    // Extract exponent
    Int iExponent = 0;
    while( fValue >= 1.0f ) {
        fValue *= 0.1f;
        ++iExponent;
    }
    while( fValue < 0.1f ) {
        fValue *= 10.0f;
        --iExponent;
    }
    while( iMinDigits > 0 ) {
        fValue *= 10.0f;
        --iExponent;
        --iMinDigits;
    }

    // Print integer part
    Int64 iIntPart = (Int64)fValue;
    outStr = FromIntW( outStr, iIntPart );

    // Print dot
    *outStr++ = WTEXT('.');

    // Print fraction part
    Float fFractPart = ( fValue - (Float)iIntPart );
	UInt iRemaining = ( iMaxDigits - iMinDigits );
    while( iRemaining > 0 ) {
        fFractPart *= 10.0f;
        --iRemaining;
    }
    outStr = FromIntW( outStr, (Int64)fFractPart );

    // Remove trailing 0s
    --outStr;
    while( *outStr == WTEXT('0') )
        --outStr;
    if ( *outStr == WTEXT('.') ) {
        ++outStr;
        *outStr++ = WTEXT('0');
    } else
        ++outStr;

    // Print exponent
    if ( iExponent != 0 ) {
        *outStr++ = WTEXT('e');
        outStr = FromIntW( outStr, iExponent );
    }

    *outStr = WNULLBYTE;
    return outStr;
}
MBChar * String::FromFloatMB( MBChar * outStr, Float fValue, UInt iMinDigits, UInt iMaxDigits ) const
{
    // Clamp digits range
    DebugAssert( iMinDigits < iMaxDigits );
    if ( iMinDigits < 1 )  iMinDigits = 1;
	if ( iMaxDigits > 16 ) iMaxDigits = 16;

    // Zero case
    if ( fValue == 0.0f || fValue == -0.0f ) {
        *outStr++ = MBTEXT('0');
        *outStr++ = MBTEXT('.');
        *outStr++ = MBTEXT('0');
        while( iMinDigits > 2 ) {
            *outStr++ = MBTEXT('0');
            --iMinDigits;
        }
        *outStr = MBNULLBYTE;
        return outStr;
    }

    // Print sign
    if ( fValue < 0.0f ) {
        fValue = -fValue;
        *outStr++ = MBTEXT('-');
    }

    // Extract exponent
    Int iExponent = 0;
    while( fValue >= 1.0f ) {
        fValue *= 0.1f;
        ++iExponent;
    }
    while( fValue < 0.1f ) {
        fValue *= 10.0f;
        --iExponent;
    }
    while( iMinDigits > 0 ) {
        fValue *= 10.0f;
        --iExponent;
        --iMinDigits;
    }

    // Print integer part
    Int64 iIntPart = (Int64)fValue;
    outStr = FromIntMB( outStr, iIntPart );

    // Print dot
    *outStr++ = MBTEXT('.');

    // Print fraction part
    Float fFractPart = ( fValue - (Float)iIntPart );
	UInt iRemaining = ( iMaxDigits - iMinDigits );
    while( iRemaining > 0 ) {
        fFractPart *= 10.0f;
        --iRemaining;
    }
    outStr = FromIntMB( outStr, (Int64)fFractPart );

    // Remove trailing 0s
    --outStr;
    while( *outStr == MBTEXT('0') )
        --outStr;
    if ( *outStr == MBTEXT('.') ) {
        ++outStr;
        *outStr++ = MBTEXT('0');
    } else
        ++outStr;

    // Print exponent
    if ( iExponent != 0 ) {
        *outStr++ = MBTEXT('e');
        outStr = FromIntMB( outStr, iExponent );
    }

    *outStr = MBNULLBYTE;
    return outStr;
}

WChar * String::FromDoubleW( WChar * outStr, Double fValue, UInt iMinDigits, UInt iMaxDigits ) const
{
    // Clamp digits range
    DebugAssert( iMinDigits < iMaxDigits );
    if ( iMinDigits < 1 )  iMinDigits = 1;
	if ( iMaxDigits > 16 ) iMaxDigits = 16;
	UInt iRemaining = ( iMaxDigits - iMinDigits );

    // Zero case
    if ( fValue == 0.0 || fValue == -0.0 ) {
        *outStr++ = WTEXT('0');
        *outStr++ = WTEXT('.');
        *outStr++ = WTEXT('0');
        while( iMinDigits > 2 ) {
            *outStr++ = WTEXT('0');
            --iMinDigits;
        }
        *outStr = WNULLBYTE;
        return outStr;
    }

    // Print sign
    if ( fValue < 0.0 ) {
        fValue = -fValue;
        *outStr++ = WTEXT('-');
    }

    // Extract exponent
    Int iExponent = 0;
    while( fValue >= 1.0 ) {
        fValue *= 0.1;
        ++iExponent;
    }
    while( fValue < 0.1 ) {
        fValue *= 10.0;
        --iExponent;
    }
    while( iMinDigits > 0 ) {
        fValue *= 10.0;
        --iExponent;
        --iMinDigits;
    }

    // Print integer part
    Int64 iIntPart = (Int64)fValue;
    outStr = FromIntW( outStr, iIntPart );

    // Print dot
    *outStr++ = WTEXT('.');

    // Print fraction part
    Double fFractPart = ( fValue - (Double)iIntPart );
    while( iRemaining > 0 ) {
        fFractPart *= 10.0;
        --iRemaining;
    }
    outStr = FromIntW( outStr, (Int64)fFractPart );

    // Remove trailing 0s
    --outStr;
    while( *outStr == WTEXT('0') )
        --outStr;
    if ( *outStr == WTEXT('.') ) {
        ++outStr;
        *outStr++ = WTEXT('0');
    } else
        ++outStr;

    // Print exponent
    if ( iExponent != 0 ) {
        *outStr++ = WTEXT('e');
        outStr = FromIntW( outStr, iExponent );
    }

    *outStr = WNULLBYTE;
    return outStr;
}
MBChar * String::FromDoubleMB( MBChar * outStr, Double fValue, UInt iMinDigits, UInt iMaxDigits ) const
{
    // Clamp digits range
    DebugAssert( iMinDigits < iMaxDigits );
    if ( iMinDigits < 1 )  iMinDigits = 1;
	if ( iMaxDigits > 16 ) iMaxDigits = 16;
	UInt iRemaining = ( iMaxDigits - iMinDigits );

    // Zero case
    if ( fValue == 0.0 || fValue == -0.0 ) {
        *outStr++ = MBTEXT('0');
        *outStr++ = MBTEXT('.');
        *outStr++ = MBTEXT('0');
        while( iMinDigits > 2 ) {
            *outStr++ = MBTEXT('0');
            --iMinDigits;
        }
        *outStr = MBNULLBYTE;
        return outStr;
    }

    // Print sign
    if ( fValue < 0.0 ) {
        fValue = -fValue;
        *outStr++ = MBTEXT('-');
    }

    // Extract exponent
    Int iExponent = 0;
    while( fValue >= 1.0 ) {
        fValue *= 0.1;
        ++iExponent;
    }
    while( fValue < 0.1 ) {
        fValue *= 10.0;
        --iExponent;
    }
    while( iMinDigits > 0 ) {
        fValue *= 10.0;
        --iExponent;
        --iMinDigits;
    }

    // Print integer part
    Int64 iIntPart = (Int64)fValue;
    outStr = FromIntMB( outStr, iIntPart );

    // Print dot
    *outStr++ = MBTEXT('.');

    // Print fraction part
    Double fFractPart = ( fValue - (Double)iIntPart );
    while( iRemaining > 0 ) {
        fFractPart *= 10.0;
        --iRemaining;
    }
    outStr = FromIntMB( outStr, (Int64)fFractPart );

    // Remove trailing 0s
    --outStr;
    while( *outStr == MBTEXT('0') )
        --outStr;
    if ( *outStr == MBTEXT('.') ) {
        ++outStr;
        *outStr++ = MBTEXT('0');
    } else
        ++outStr;

    // Print exponent
    if ( iExponent != 0 ) {
        *outStr++ = MBTEXT('e');
        outStr = FromIntMB( outStr, iExponent );
    }

    *outStr = MBNULLBYTE;
    return outStr;
}

const WChar * String::ScanVAW( const WChar * inStr, const WChar * strFormat, VArgPtr pArgList ) const
{
    Bool bUnsigned;

    while( *strFormat != WNULLBYTE ) {
        if ( *strFormat != WTEXT('%') ) {
            if ( *inStr != *strFormat )
                break;
            ++inStr; ++strFormat;
            continue;
        }
        ++strFormat;

        if ( *strFormat == WTEXT('%') ) {
            if ( *inStr != *strFormat )
                break;
            ++inStr; ++strFormat;
            continue;
        }

        bUnsigned = false;
        if ( *strFormat == WTEXT('u') || *strFormat == WTEXT('U') ) {
            bUnsigned = true;
            ++strFormat;
        }

        switch( *strFormat ) {
            case WTEXT('s'): case WTEXT('S'): {
                    WChar * outString = VArg_Get( pArgList, WChar* );
                    UInt iLength = VArg_Get( pArgList, UInt );
                    NCopyW( outString, inStr, iLength );
                    inStr += iLength;
                } break;
            case WTEXT('f'): {
                    Float * outFloat = VArg_Get( pArgList, Float* );
                    *outFloat = ToFloatW( inStr, &inStr );
                } break;
            case WTEXT('F'): {
                    Double * outDouble = VArg_Get( pArgList, Double* );
                    *outDouble = ToDoubleW( inStr, &inStr );
                } break;
            case WTEXT('n'): {
                    if ( bUnsigned ) {
                        UInt * outUInt = VArg_Get( pArgList, UInt* );
                        *outUInt = (UInt)ToUIntW( inStr, &inStr );
                    } else {
                        Int * outInt = VArg_Get( pArgList, Int* );
                        *outInt = (Int)ToIntW( inStr, &inStr );
                    }
                } break;
            case WTEXT('N'): {
                    if ( bUnsigned ) {
                        UInt64 * outUInt64 = VArg_Get( pArgList, UInt64* );
                        *outUInt64 = ToUIntW( inStr, &inStr );
                    } else {
                        Int64 * outInt64 = VArg_Get( pArgList, Int64* );
                        *outInt64 = ToIntW( inStr, &inStr );
                    }
                } break;
            default: DebugAssert( false ); break;
        }
        ++strFormat;
    }
    return inStr;
}
const MBChar * String::ScanVAMB( const MBChar * inStr, const MBChar * strFormat, VArgPtr pArgList ) const
{
    Bool bUnsigned;

    while( *strFormat != MBNULLBYTE ) {
        if ( *strFormat != MBTEXT('%') ) {
            if ( *inStr != *strFormat )
                break;
            ++inStr; ++strFormat;
            continue;
        }
        ++strFormat;

        if ( *strFormat == MBTEXT('%') ) {
            if ( *inStr != *strFormat )
                break;
            ++inStr; ++strFormat;
            continue;
        }

        bUnsigned = false;
        if ( *strFormat == MBTEXT('u') || *strFormat == MBTEXT('U') ) {
            bUnsigned = true;
            ++strFormat;
        }

        switch( *strFormat ) {
            case MBTEXT('s'): case MBTEXT('S'): {
                    MBChar * outString = VArg_Get( pArgList, MBChar* );
                    UInt iLength = VArg_Get( pArgList, UInt );
                    NCopyMB( outString, inStr, iLength );
                    inStr += iLength;
                } break;
            case MBTEXT('f'): {
                    Float * outFloat = VArg_Get( pArgList, Float* );
                    *outFloat = ToFloatMB( inStr, &inStr );
                } break;
            case MBTEXT('F'): {
                    Double * outDouble = VArg_Get( pArgList, Double* );
                    *outDouble = ToDoubleMB( inStr, &inStr );
                } break;
            case MBTEXT('n'): {
                    if ( bUnsigned ) {
                        UInt * outUInt = VArg_Get( pArgList, UInt* );
                        *outUInt = (UInt)ToUIntMB( inStr, &inStr );
                    } else {
                        Int * outInt = VArg_Get( pArgList, Int* );
                        *outInt = (Int)ToIntMB( inStr, &inStr );
                    }
                } break;
            case MBTEXT('N'): {
                    if ( bUnsigned ) {
                        UInt64 * outUInt64 = VArg_Get( pArgList, UInt64* );
                        *outUInt64 = ToUIntMB( inStr, &inStr );
                    } else {
                        Int64 * outInt64 = VArg_Get( pArgList, Int64* );
                        *outInt64 = ToIntMB( inStr, &inStr );
                    }
                } break;
            default: DebugAssert( false ); break;
        }
        ++strFormat;
    }
    return inStr;
}

WChar * String::FormatVAW( WChar * outStr, const WChar * strFormat, VArgPtr pArgList ) const
{
    Bool bUnsigned;
    UInt iMinDigits, iMaxDigits;

    while( *strFormat != WNULLBYTE ) {
        if ( *strFormat != WTEXT('%') ) {
            *outStr++ = *strFormat++;
            continue;
        }
        ++strFormat;

        if ( *strFormat == WTEXT('%') ) {
            *outStr++ = *strFormat++;
            continue;
        }

        bUnsigned = false;
        if ( *strFormat == WTEXT('u') || *strFormat == WTEXT('U') ) {
            bUnsigned = true;
            ++strFormat;
        }

        iMinDigits = 0;
        iMaxDigits = 0;
        if ( IsDecDigitW(*strFormat) ) {
            iMinDigits = (UInt)ToUIntW( strFormat, &strFormat );
            if ( *strFormat == WTEXT('.') ) {
                ++strFormat;
                if ( IsDecDigitW(*strFormat) )
                    iMaxDigits = (UInt)ToUIntW( strFormat, &strFormat );
            }
        }

        switch( *strFormat ) {
            case WTEXT('s'): case WTEXT('S'): {
                    const WChar * inString = VArg_Get( pArgList, const WChar * );
                    outStr = CopyW( outStr, inString );
                } break;
            case WTEXT('f'): {
                    if ( iMinDigits == 0 ) iMinDigits = 2;
                    if ( iMaxDigits == 0 ) iMaxDigits = 8;
                    Float inFloat = (Float)( VArg_Get( pArgList, Double ) ); // Float are passed as doubles ....
                    outStr = FromFloatW( outStr, inFloat, iMinDigits, iMaxDigits );
                } break;
            case WTEXT('F'): {
                    if ( iMinDigits == 0 ) iMinDigits = 2;
                    if ( iMaxDigits == 0 ) iMaxDigits = 8;
                    Double inDouble = VArg_Get( pArgList, Double );
                    outStr = FromDoubleW( outStr, inDouble, iMinDigits, iMaxDigits );
                } break;
            default: {
                    if ( iMinDigits == 0 ) iMinDigits = 1;
                    Bool bBig = false;
                    Radix iRadix;
                    switch( *strFormat ) {
                        case WTEXT('B'): bBig = true; case WTEXT('b'): iRadix = RADIX_BINARY; break;
                        case WTEXT('O'): bBig = true; case WTEXT('o'): iRadix = RADIX_OCTAL; break;
                        case WTEXT('D'): bBig = true; case WTEXT('d'): iRadix = RADIX_DECIMAL; break;
                        case WTEXT('X'): bBig = true; case WTEXT('x'): iRadix = RADIX_HEXADECIMAL; break;
                        default: DebugAssert( false ); break;
                    }
                    if ( bBig ) {
                        if ( bUnsigned ) {
                            UInt64 inUInt64 = VArg_Get( pArgList, UInt64 );
                            outStr = FromUIntW( outStr, inUInt64, iRadix, iMinDigits );
                        } else {
                            Int64 inInt64 = VArg_Get( pArgList, Int64 );
                            outStr = FromIntW( outStr, inInt64, iRadix, iMinDigits );
                        }
                    } else {
                        if ( bUnsigned ) {
                            UInt inUInt = VArg_Get( pArgList, UInt );
                            outStr = FromUIntW( outStr, (UInt64)inUInt, iRadix, iMinDigits );
                        } else {
                            Int inInt = VArg_Get( pArgList, Int );
                            outStr = FromIntW( outStr, (Int64)inInt, iRadix, iMinDigits );
                        }
                    }
                } break;
        }
        ++strFormat;
    }
    *outStr = WNULLBYTE;
    return outStr;
}
MBChar * String::FormatVAMB( MBChar * outStr, const MBChar * strFormat, VArgPtr pArgList ) const
{
    Bool bUnsigned;
    UInt iMinDigits, iMaxDigits;

    while( *strFormat != MBNULLBYTE ) {
        if ( *strFormat != MBTEXT('%') ) {
            *outStr++ = *strFormat++;
            continue;
        }
        ++strFormat;

        if ( *strFormat == MBTEXT('%') ) {
            *outStr++ = *strFormat++;
            continue;
        }

        bUnsigned = false;
        if ( *strFormat == MBTEXT('u') || *strFormat == MBTEXT('U') ) {
            bUnsigned = true;
            ++strFormat;
        }

        iMinDigits = 0;
        iMaxDigits = 0;
        if ( IsDecDigitMB(*strFormat) ) {
            iMinDigits = (UInt)ToUIntMB( strFormat, &strFormat );
            if ( *strFormat == MBTEXT('.') ) {
                ++strFormat;
                if ( IsDecDigitMB(*strFormat) )
                    iMaxDigits = (UInt)ToUIntMB( strFormat, &strFormat );
            }
        }

        switch( *strFormat ) {
            case MBTEXT('s'): case MBTEXT('S'): {
                    const MBChar * inString = VArg_Get( pArgList, const MBChar * );
                    outStr = CopyMB( outStr, inString );
                } break;
            case MBTEXT('f'): {
                    if ( iMinDigits == 0 ) iMinDigits = 2;
                    if ( iMaxDigits == 0 ) iMaxDigits = 8;
                    Float inFloat = (Float)( VArg_Get( pArgList, Double ) ); // Float are passed as doubles ....
                    outStr = FromFloatMB( outStr, inFloat, iMinDigits, iMaxDigits );
                } break;
            case MBTEXT('F'): {
                    if ( iMinDigits == 0 ) iMinDigits = 2;
                    if ( iMaxDigits == 0 ) iMaxDigits = 8;
                    Double inDouble = VArg_Get( pArgList, Double );
                    outStr = FromDoubleMB( outStr, inDouble, iMinDigits, iMaxDigits );
                } break;
            default: {
                    if ( iMinDigits == 0 ) iMinDigits = 1;
                    Bool bBig = false;
                    Radix iRadix;
                    switch( *strFormat ) {
                        case MBTEXT('B'): bBig = true; case MBTEXT('b'): iRadix = RADIX_BINARY; break;
                        case MBTEXT('O'): bBig = true; case MBTEXT('o'): iRadix = RADIX_OCTAL; break;
                        case MBTEXT('D'): bBig = true; case MBTEXT('d'): iRadix = RADIX_DECIMAL; break;
                        case MBTEXT('X'): bBig = true; case MBTEXT('x'): iRadix = RADIX_HEXADECIMAL; break;
                        default: DebugAssert( false ); break;
                    }
                    if ( bBig ) {
                        if ( bUnsigned ) {
                            UInt64 inUInt64 = VArg_Get( pArgList, UInt64 );
                            outStr = FromUIntMB( outStr, inUInt64, iRadix, iMinDigits );
                        } else {
                            Int64 inInt64 = VArg_Get( pArgList, Int64 );
                            outStr = FromIntMB( outStr, inInt64, iRadix, iMinDigits );
                        }
                    } else {
                        if ( bUnsigned ) {
                            UInt inUInt = VArg_Get( pArgList, UInt );
                            outStr = FromUIntMB( outStr, (UInt64)inUInt, iRadix, iMinDigits );
                        } else {
                            Int inInt = VArg_Get( pArgList, Int );
                            outStr = FromIntMB( outStr, (Int64)inInt, iRadix, iMinDigits );
                        }
                    }
                } break;
        }
        ++strFormat;
    }
    *outStr = MBNULLBYTE;
    return outStr;
}

GChar String::ConvertKeyCode( KeyCode iKey, Bool bShift, Bool bCtrl, Bool bAlt, Bool bNumLock, Bool bCapsLock, Bool bQWERTY ) const
{
    if ( KEYCODE_FAMILY(iKey) != KEYCODE_FAMILY_PRINTABLES )
        return NULLBYTE;
    DWord dwSubFamily = KEYCODE_SUBFAMILY(iKey);
    DWord dwKeyIndex = KEYCODE_INDEX(iKey);
    GChar ch = NULLBYTE;

    Bool bUseCaps = ( bShift != bCapsLock );

    // Character case
    if ( dwSubFamily == KEYCODE_SUBFAMILY_CHARS ) {
        if ( dwKeyIndex == KEYCODE_INDEX(KEYCODE_SPACE) )
            ch = TEXT(' ');
        else {
            ch = bUseCaps ? TEXT('A') : TEXT('a');
            ch += (GChar)( dwKeyIndex - KEYCODE_INDEX(KEYCODE_A) );
        }
        return ch;
    }

    // Numeric case
    if ( dwSubFamily == KEYCODE_SUBFAMILY_NUMBERS ) {
        // numpad case
        DWord dwBaseIndex = KEYCODE_INDEX(KEYCODE_NUMPAD_0);
        if ( dwKeyIndex >= dwBaseIndex ) {
            if ( bNumLock ) {
                ch = TEXT('0');
                ch += (GChar)( dwKeyIndex - dwBaseIndex );
                return ch;
            }
            return NULLBYTE; // non-printable
        }

        // regular case
        dwBaseIndex = KEYCODE_INDEX(KEYCODE_0);
        if ( bQWERTY ) {
            if ( !bUseCaps ) {
                ch = TEXT('0');
                ch += (GChar)( dwKeyIndex - dwBaseIndex );
                return ch;
            }
            switch( dwKeyIndex ) {
	            case KEYCODE_INDEX(KEYCODE_0): return TEXT(')');
	            case KEYCODE_INDEX(KEYCODE_1): return TEXT('!');
	            case KEYCODE_INDEX(KEYCODE_2): return TEXT('@');
                case KEYCODE_INDEX(KEYCODE_3): return TEXT('#');
	            case KEYCODE_INDEX(KEYCODE_4): return TEXT('$');
	            case KEYCODE_INDEX(KEYCODE_5): return TEXT('%');
	            case KEYCODE_INDEX(KEYCODE_6): return TEXT('^');
	            case KEYCODE_INDEX(KEYCODE_7): return TEXT('&');
	            case KEYCODE_INDEX(KEYCODE_8): return TEXT('*');
	            case KEYCODE_INDEX(KEYCODE_9): return TEXT('(');
	            default: return NULLBYTE; // never happen
            }
        } else { // AZERTY
            if ( bCtrl && bAlt ) {
                switch( dwKeyIndex ) {
				    case KEYCODE_INDEX(KEYCODE_0): return TEXT('@');
				    case KEYCODE_INDEX(KEYCODE_1): break; // ignore this case & continue
				    case KEYCODE_INDEX(KEYCODE_2): return TEXT('~');
				    case KEYCODE_INDEX(KEYCODE_3): return TEXT('#');
                    case KEYCODE_INDEX(KEYCODE_4): return TEXT('{');
				    case KEYCODE_INDEX(KEYCODE_5): return TEXT('[');
				    case KEYCODE_INDEX(KEYCODE_6): return TEXT('|');
				    case KEYCODE_INDEX(KEYCODE_7): return TEXT('`');
				    case KEYCODE_INDEX(KEYCODE_8): return TEXT('\\');
				    case KEYCODE_INDEX(KEYCODE_9): return TEXT('^');
				    default: return NULLBYTE; // never happen
			    }
            }
            if ( bUseCaps ) {
                ch = TEXT('0');
                ch += (GChar)( dwKeyIndex - dwBaseIndex );
                return ch;
            }
            switch( dwKeyIndex ) {
			    case KEYCODE_INDEX(KEYCODE_0): return TEXT('à');
			    case KEYCODE_INDEX(KEYCODE_1): return TEXT('&');
			    case KEYCODE_INDEX(KEYCODE_2): return TEXT('é');
			    case KEYCODE_INDEX(KEYCODE_3): return TEXT('"');
			    case KEYCODE_INDEX(KEYCODE_4): return TEXT('\'');
			    case KEYCODE_INDEX(KEYCODE_5): return TEXT('(');
			    case KEYCODE_INDEX(KEYCODE_6): return TEXT('-');
			    case KEYCODE_INDEX(KEYCODE_7): return TEXT('è');
			    case KEYCODE_INDEX(KEYCODE_8): return TEXT('_');
			    case KEYCODE_INDEX(KEYCODE_9): return TEXT('ç');
			    default: return NULLBYTE; // never happen
		    }
        }
    }

    // Symbol case
    if ( dwSubFamily == KEYCODE_SUBFAMILY_SYMBOLS ) {
        // numpad case
        if ( dwKeyIndex < KEYCODE_INDEX(KEYCODE_OEM_1) ) {
            switch( dwKeyIndex ) {
			    case KEYCODE_INDEX(KEYCODE_NUMPAD_ADD): return TEXT('+');
			    case KEYCODE_INDEX(KEYCODE_NUMPAD_SUB): return TEXT('-');
			    case KEYCODE_INDEX(KEYCODE_NUMPAD_MUL): return TEXT('*');
			    case KEYCODE_INDEX(KEYCODE_NUMPAD_DIV): return TEXT('/');
                case KEYCODE_INDEX(KEYCODE_NUMPAD_DOT): return (bNumLock) ? TEXT('.') : NULLBYTE;
			    default: return NULLBYTE; // never happen
		    }
        }

        // OEM case
        if ( bQWERTY ) {
            if ( bUseCaps ) {
                switch( dwKeyIndex ) {
                    case KEYCODE_INDEX(KEYCODE_OEM_1):      return TEXT(':');
				    case KEYCODE_INDEX(KEYCODE_OEM_PLUS):   return TEXT('+');
				    case KEYCODE_INDEX(KEYCODE_OEM_COMMA):  return TEXT('<');
				    case KEYCODE_INDEX(KEYCODE_OEM_MINUS):  return TEXT('_');
				    case KEYCODE_INDEX(KEYCODE_OEM_PERIOD): return TEXT('>');
				    case KEYCODE_INDEX(KEYCODE_OEM_2):      return TEXT('?');
				    case KEYCODE_INDEX(KEYCODE_OEM_3):      return TEXT('~');
				    case KEYCODE_INDEX(KEYCODE_OEM_4):      return TEXT('{');
				    case KEYCODE_INDEX(KEYCODE_OEM_5):      return TEXT('|');
                    case KEYCODE_INDEX(KEYCODE_OEM_6):      return TEXT('}');
				    case KEYCODE_INDEX(KEYCODE_OEM_7):      return TEXT('"');
				    case KEYCODE_INDEX(KEYCODE_OEM_8):      return NULLBYTE; // non-existing in QWERTY
				    case KEYCODE_INDEX(KEYCODE_OEM_102):    return TEXT('|');
				    default: return NULLBYTE; // never happen
			    }
            }
            switch( dwKeyIndex ) {
		        case KEYCODE_INDEX(KEYCODE_OEM_1):      return TEXT(';');
		        case KEYCODE_INDEX(KEYCODE_OEM_PLUS):   return TEXT('=');
		        case KEYCODE_INDEX(KEYCODE_OEM_COMMA):  return TEXT(',');
		        case KEYCODE_INDEX(KEYCODE_OEM_MINUS):  return TEXT('-');
		        case KEYCODE_INDEX(KEYCODE_OEM_PERIOD): return TEXT('.');
		        case KEYCODE_INDEX(KEYCODE_OEM_2):      return TEXT('/');
		        case KEYCODE_INDEX(KEYCODE_OEM_3):      return TEXT('`');
		        case KEYCODE_INDEX(KEYCODE_OEM_4):      return TEXT('[');
		        case KEYCODE_INDEX(KEYCODE_OEM_5):      return TEXT('\\');
		        case KEYCODE_INDEX(KEYCODE_OEM_6):      return TEXT(']');
		        case KEYCODE_INDEX(KEYCODE_OEM_7):      return TEXT('\'');
		        case KEYCODE_INDEX(KEYCODE_OEM_8):      return NULLBYTE; // non-existing in QWERTY
		        case KEYCODE_INDEX(KEYCODE_OEM_102):    return TEXT('\\');
		        default: return NULLBYTE; // never happen
	        }
        } else { // AZERTY
            if ( bCtrl && bAlt ) {
                switch( dwKeyIndex ) {
				    case KEYCODE_INDEX(KEYCODE_OEM_1):    return TEXT('¤');
                    case KEYCODE_INDEX(KEYCODE_OEM_PLUS): return TEXT('}');
				    case KEYCODE_INDEX(KEYCODE_OEM_4):    return TEXT(']');
				    case KEYCODE_INDEX(KEYCODE_OEM_COMMA):
				    case KEYCODE_INDEX(KEYCODE_OEM_PERIOD):
				    case KEYCODE_INDEX(KEYCODE_OEM_2):
				    case KEYCODE_INDEX(KEYCODE_OEM_3):
				    case KEYCODE_INDEX(KEYCODE_OEM_5):
				    case KEYCODE_INDEX(KEYCODE_OEM_6):
				    case KEYCODE_INDEX(KEYCODE_OEM_7):
				    case KEYCODE_INDEX(KEYCODE_OEM_8):
				    case KEYCODE_INDEX(KEYCODE_OEM_102):   break; // ignore those cases & continue
				    case KEYCODE_INDEX(KEYCODE_OEM_MINUS): return NULLBYTE; // non-existing in AZERTY
				    default: return NULLBYTE; // never happen
			    }
            }
            if ( bUseCaps ) {
                switch( dwKeyIndex ) {
				    case KEYCODE_INDEX(KEYCODE_OEM_1):      return TEXT('£');
				    case KEYCODE_INDEX(KEYCODE_OEM_PLUS):   return TEXT('+');
				    case KEYCODE_INDEX(KEYCODE_OEM_COMMA):  return TEXT('?');
				    case KEYCODE_INDEX(KEYCODE_OEM_MINUS):  return NULLBYTE; // non-existing in AZERTY
				    case KEYCODE_INDEX(KEYCODE_OEM_PERIOD): return TEXT('.');
				    case KEYCODE_INDEX(KEYCODE_OEM_2):      return TEXT('/');
				    case KEYCODE_INDEX(KEYCODE_OEM_3):      return TEXT('%');
				    case KEYCODE_INDEX(KEYCODE_OEM_4):      return TEXT('°');
				    case KEYCODE_INDEX(KEYCODE_OEM_5):      return TEXT('µ');
				    case KEYCODE_INDEX(KEYCODE_OEM_6):      return TEXT('¨');
				    case KEYCODE_INDEX(KEYCODE_OEM_7):      break; // ignore this case & continue
				    case KEYCODE_INDEX(KEYCODE_OEM_8):      return TEXT('§');
				    case KEYCODE_INDEX(KEYCODE_OEM_102):    return TEXT('>');
				    default: return NULLBYTE; // never happen
			    }
            }
            switch( dwKeyIndex ) {
		        case KEYCODE_INDEX(KEYCODE_OEM_1):      return TEXT('$');
		        case KEYCODE_INDEX(KEYCODE_OEM_PLUS):   return TEXT('=');
		        case KEYCODE_INDEX(KEYCODE_OEM_COMMA):  return TEXT(',');
		        case KEYCODE_INDEX(KEYCODE_OEM_MINUS):  return NULLBYTE; // non-existing in AZERTY
		        case KEYCODE_INDEX(KEYCODE_OEM_PERIOD): return TEXT(';');
                case KEYCODE_INDEX(KEYCODE_OEM_2):      return TEXT(':');
		        case KEYCODE_INDEX(KEYCODE_OEM_3):      return TEXT('ù');
		        case KEYCODE_INDEX(KEYCODE_OEM_4):      return TEXT(')');
		        case KEYCODE_INDEX(KEYCODE_OEM_5):      return TEXT('*');
		        case KEYCODE_INDEX(KEYCODE_OEM_6):      return TEXT('^');
		        case KEYCODE_INDEX(KEYCODE_OEM_7):      return TEXT('²');
		        case KEYCODE_INDEX(KEYCODE_OEM_8):      return TEXT('!');
		        case KEYCODE_INDEX(KEYCODE_OEM_102):    return TEXT('<');
		        default: return NULLBYTE; // never happen
	        }
        }
    }

    return NULLBYTE;
}
