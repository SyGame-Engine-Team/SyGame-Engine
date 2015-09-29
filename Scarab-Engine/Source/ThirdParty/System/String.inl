/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/System/String.inl
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
// String implementation
inline String * String::GetInstancePtr() {
    static String s_Instance;
    return &s_Instance;
}

inline MBChar * String::AsciiToMultiByte( MBChar * outStr, const AChar * inStr, UInt iLength ) const {
    return NCopyMB( outStr, inStr, iLength );
}
inline WChar * String::AsciiToWideChar( WChar * outStr, const AChar * inStr, UInt iLength ) const {
    return MultiByteToWideChar( outStr, inStr, iLength );
}

inline const AChar * String::NextCharA( const AChar * inStr ) const {
    return NextCharMB( inStr );
}
inline const GChar * String::NextChar( const GChar * inStr ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return NextCharW( inStr );
#else
    return NextCharMB( inStr );
#endif
}

inline const AChar * String::PrevCharA( const AChar * inStart, const AChar * inStr ) const {
    return PrevCharMB( inStart, inStr );
}
inline const GChar * String::PrevChar( const GChar * inStart, const GChar * inStr ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return PrevCharW( inStart, inStr );
#else
    return PrevCharMB( inStart, inStr );
#endif
}

inline CharacterClass String::GetClassA( AChar ch ) const {
    return GetClassMB( ch );
}
inline CharacterClass String::GetClass( GChar ch ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return GetClassW( ch );
#else
    return GetClassMB( ch );
#endif
}

inline Bool String::IsAsciiW( WChar ch ) const {
    return ( ch <= 0x7f );
}
inline Bool String::IsAsciiMB( MBChar ch ) const {
    return ( ch <= 0x7f );
}
inline Bool String::IsAsciiA( AChar ch ) const {
    return IsAsciiMB( ch );
}
inline Bool String::IsAscii( GChar ch ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return IsAsciiW( ch );
#else
    return IsAsciiMB( ch );
#endif
}

inline Bool String::IsExtAsciiW( WChar ch ) const {
    return ( ch >= 0x80 && ch <= 0xff );
}
inline Bool String::IsExtAsciiMB( MBChar ch ) const {
    return ( ch >= 0x80 );
}
inline Bool String::IsExtAsciiA( AChar ch ) const {
    return IsExtAsciiMB( ch );
}
inline Bool String::IsExtAscii( GChar ch ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return IsExtAsciiW( ch );
#else
    return IsExtAsciiMB( ch );
#endif
}

inline Bool String::IsPrintableW( WChar ch ) const {
    CharacterClass chClass = GetClassW( ch );
    return ( (chClass & CHARACTER_CLASS_CONTROL) == 0 );
}
inline Bool String::IsPrintableMB( MBChar ch ) const {
    CharacterClass chClass = GetClassMB( ch );
    return ( (chClass & CHARACTER_CLASS_CONTROL) == 0 );
}
inline Bool String::IsPrintableA( AChar ch ) const {
    return IsPrintableMB( ch );
}
inline Bool String::IsPrintable( GChar ch ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return IsPrintableW( ch );
#else
    return IsPrintableMB( ch );
#endif
}

inline Bool String::IsAlphaW( WChar ch ) const {
    CharacterClass chClass = GetClassW( ch );
    return ( (chClass & CHARACTER_CLASS_ALPHA) != 0 );
}
inline Bool String::IsAlphaMB( MBChar ch ) const {
    CharacterClass chClass = GetClassMB( ch );
    return ( (chClass & CHARACTER_CLASS_ALPHA) != 0 );
}
inline Bool String::IsAlphaA( AChar ch ) const {
    return IsAlphaMB( ch );
}
inline Bool String::IsAlpha( GChar ch ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return IsAlphaW( ch );
#else
    return IsAlphaMB( ch );
#endif
}

inline Bool String::IsAlphaNumW( WChar ch ) const {
    CharacterClass chClass = GetClassW( ch );
    return ( (chClass & CHARACTER_CLASS_ALPHA) != 0 || (chClass & CHARACTER_CLASS_DIGIT) != 0 );
}
inline Bool String::IsAlphaNumMB( MBChar ch ) const {
    CharacterClass chClass = GetClassMB( ch );
    return ( (chClass & CHARACTER_CLASS_ALPHA) != 0 || (chClass & CHARACTER_CLASS_DIGIT) != 0 );
}
inline Bool String::IsAlphaNumA( AChar ch ) const {
    return IsAlphaNumMB( ch );
}
inline Bool String::IsAlphaNum( GChar ch ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return IsAlphaNumW( ch );
#else
    return IsAlphaNumMB( ch );
#endif
}

inline Bool String::IsLowerCaseW( WChar ch ) const {
    CharacterClass chClass = GetClassW( ch );
    return ( (chClass & CHARACTER_CLASS_LOWERCASE) != 0 );
}
inline Bool String::IsLowerCaseMB( MBChar ch ) const {
    CharacterClass chClass = GetClassMB( ch );
    return ( (chClass & CHARACTER_CLASS_LOWERCASE) != 0 );
}
inline Bool String::IsLowerCaseA( AChar ch ) const {
    return IsLowerCaseMB( ch );
}
inline Bool String::IsLowerCase( GChar ch ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return IsLowerCaseW( ch );
#else
    return IsLowerCaseMB( ch );
#endif
}

inline Bool String::IsUpperCaseW( WChar ch ) const {
    CharacterClass chClass = GetClassW( ch );
    return ( (chClass & CHARACTER_CLASS_UPPERCASE) != 0 );
}
inline Bool String::IsUpperCaseMB( MBChar ch ) const {
    CharacterClass chClass = GetClassMB( ch );
    return ( (chClass & CHARACTER_CLASS_UPPERCASE) != 0 );
}
inline Bool String::IsUpperCaseA( AChar ch ) const {
    return IsUpperCaseMB( ch );
}
inline Bool String::IsUpperCase( GChar ch ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return IsUpperCaseW( ch );
#else
    return IsUpperCaseMB( ch );
#endif
}

inline Bool String::IsBinDigitW( WChar ch ) const {
    return ( WTEXT('0') <= ch && ch <= WTEXT('1') );
}
inline Bool String::IsBinDigitMB( MBChar ch ) const {
    return ( MBTEXT('0') <= ch && ch <= MBTEXT('1') );
}
inline Bool String::IsBinDigitA( AChar ch ) const {
    return IsBinDigitMB( ch );
}
inline Bool String::IsBinDigit( GChar ch ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return IsBinDigitW( ch );
#else
    return IsBinDigitMB( ch );
#endif
}

inline Bool String::IsOctDigitW( WChar ch ) const {
    return ( WTEXT('0') <= ch && ch <= WTEXT('7') );
}
inline Bool String::IsOctDigitMB( MBChar ch ) const {
    return ( MBTEXT('0') <= ch && ch <= MBTEXT('7') );
}
inline Bool String::IsOctDigitA( AChar ch ) const {
    return IsOctDigitMB( ch );
}
inline Bool String::IsOctDigit( GChar ch ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return IsOctDigitW( ch );
#else
    return IsOctDigitMB( ch );
#endif
}

inline Bool String::IsDecDigitW( WChar ch ) const {
    return ( WTEXT('0') <= ch && ch <= WTEXT('9') );
}
inline Bool String::IsDecDigitMB( MBChar ch ) const {
    return ( MBTEXT('0') <= ch && ch <= MBTEXT('9') );
}
inline Bool String::IsDecDigitA( AChar ch ) const {
    return IsDecDigitMB( ch );
}
inline Bool String::IsDecDigit( GChar ch ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return IsDecDigitW( ch );
#else
    return IsDecDigitMB( ch );
#endif
}

inline Bool String::IsHexDigitW( WChar ch ) const {
    return ( ( WTEXT('0') <= ch && ch <= WTEXT('9') ) ||
             ( WTEXT('a') <= ch && ch <= WTEXT('f') ) ||
             ( WTEXT('A') <= ch && ch <= WTEXT('F') ) );
}
inline Bool String::IsHexDigitMB( MBChar ch ) const {
    return ( ( MBTEXT('0') <= ch && ch <= MBTEXT('9') ) ||
             ( MBTEXT('a') <= ch && ch <= MBTEXT('f') ) ||
             ( MBTEXT('A') <= ch && ch <= MBTEXT('F') ) );
}
inline Bool String::IsHexDigitA( AChar ch ) const {
    return IsHexDigitMB( ch );
}
inline Bool String::IsHexDigit( GChar ch ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return IsHexDigitW( ch );
#else
    return IsHexDigitMB( ch );
#endif
}

inline Bool String::IsBlankW( WChar ch ) const {
    CharacterClass chClass = GetClassW( ch );
    return ( (chClass & CHARACTER_CLASS_BLANK) != 0 );
}
inline Bool String::IsBlankMB( MBChar ch ) const {
    CharacterClass chClass = GetClassMB( ch );
    return ( (chClass & CHARACTER_CLASS_BLANK) != 0 );
}
inline Bool String::IsBlankA( AChar ch ) const {
    return IsBlankMB( ch );
}
inline Bool String::IsBlank( GChar ch ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return IsBlankW( ch );
#else
    return IsBlankMB( ch );
#endif
}

inline Bool String::IsSpaceW( WChar ch ) const {
    CharacterClass chClass = GetClassW( ch );
    return ( (chClass & CHARACTER_CLASS_SPACE) != 0 );
}
inline Bool String::IsSpaceMB( MBChar ch ) const {
    CharacterClass chClass = GetClassMB( ch );
    return ( (chClass & CHARACTER_CLASS_SPACE) != 0 );
}
inline Bool String::IsSpaceA( AChar ch ) const {
    return IsSpaceMB( ch );
}
inline Bool String::IsSpace( GChar ch ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return IsSpaceW( ch );
#else
    return IsSpaceMB( ch );
#endif
}

inline Bool String::IsPunctuationW( WChar ch ) const {
    CharacterClass chClass = GetClassW( ch );
    return ( (chClass & CHARACTER_CLASS_PUNCTUATION) != 0 );
}
inline Bool String::IsPunctuationMB( MBChar ch ) const {
    CharacterClass chClass = GetClassMB( ch );
    return ( (chClass & CHARACTER_CLASS_PUNCTUATION) != 0 );
}
inline Bool String::IsPunctuationA( AChar ch ) const {
    return IsPunctuationMB( ch );
}
inline Bool String::IsPunctuation( GChar ch ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return IsPunctuationW( ch );
#else
    return IsPunctuationMB( ch );
#endif
}

inline AChar String::ToLowerCaseA( AChar ch ) const {
    return ToLowerCaseMB( ch );
}
inline GChar String::ToLowerCase( GChar ch ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return ToLowerCaseW( ch );
#else
    return ToLowerCaseMB( ch );
#endif
}

inline AChar String::ToUpperCaseA( AChar ch ) const {
    return ToUpperCaseMB( ch );
}
inline GChar String::ToUpperCase( GChar ch ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return ToUpperCaseW( ch );
#else
    return ToUpperCaseMB( ch );
#endif
}

inline UInt String::CharToBinW( WChar ch ) const {
    return (UInt)( ch - WTEXT('0') );
}
inline UInt String::CharToBinMB( MBChar ch ) const {
    return (UInt)( ch - MBTEXT('0') );
}
inline UInt String::CharToBinA( AChar ch ) const {
    return CharToBinMB( ch );
}
inline UInt String::CharToBin( GChar ch ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return CharToBinW( ch );
#else
    return CharToBinMB( ch );
#endif
}

inline UInt String::CharToOctW( WChar ch ) const {
    return (UInt)( ch - WTEXT('0') );
}
inline UInt String::CharToOctMB( MBChar ch ) const {
    return (UInt)( ch - MBTEXT('0') );
}
inline UInt String::CharToOctA( AChar ch ) const {
    return CharToOctMB( ch );
}
inline UInt String::CharToOct( GChar ch ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return CharToOctW( ch );
#else
    return CharToOctMB( ch );
#endif
}

inline UInt String::CharToDecW( WChar ch ) const {
    return (UInt)( ch - WTEXT('0') );
}
inline UInt String::CharToDecMB( MBChar ch ) const {
    return (UInt)( ch - MBTEXT('0') );
}
inline UInt String::CharToDecA( AChar ch ) const {
    return CharToDecMB( ch );
}
inline UInt String::CharToDec( GChar ch ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return CharToDecW( ch );
#else
    return CharToDecMB( ch );
#endif
}

inline UInt String::CharToHexW( WChar ch ) const {
    if ( WTEXT('0') <= ch && ch <= WTEXT('9') )
        return (UInt)( ch - WTEXT('0') );
    if ( WTEXT('a') <= ch && ch <= WTEXT('f') )
        return 10 + (UInt)( ch - WTEXT('a') );
    return 10 + (UInt)( ch - WTEXT('A') );
}
inline UInt String::CharToHexMB( MBChar ch ) const {
    if ( MBTEXT('0') <= ch && ch <= MBTEXT('9') )
        return (UInt)( ch - MBTEXT('0') );
    if ( MBTEXT('a') <= ch && ch <= MBTEXT('f') )
        return 10 + (UInt)( ch - MBTEXT('a') );
    return 10 + (UInt)( ch - MBTEXT('A') );
}
inline UInt String::CharToHexA( AChar ch ) const {
    return CharToHexMB( ch );
}
inline UInt String::CharToHex( GChar ch ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return CharToHexW( ch );
#else
    return CharToHexMB( ch );
#endif
}

inline WChar String::BinToCharW( UInt iValue ) const {
    return ( WTEXT('0') + (WChar)iValue );
}
inline MBChar String::BinToCharMB( UInt iValue ) const {
    return ( MBTEXT('0') + (MBChar)iValue );
}
inline AChar String::BinToCharA( UInt iValue ) const {
    return BinToCharMB( iValue );
}
inline GChar String::BinToChar( UInt iValue ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return BinToCharW( iValue );
#else
    return BinToCharMB( iValue );
#endif
}

inline WChar String::OctToCharW( UInt iValue ) const {
    return ( WTEXT('0') + (WChar)iValue );
}
inline MBChar String::OctToCharMB( UInt iValue ) const {
    return ( MBTEXT('0') + (MBChar)iValue );
}
inline AChar String::OctToCharA( UInt iValue ) const {
    return OctToCharMB( iValue );
}
inline GChar String::OctToChar( UInt iValue ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return OctToCharW( iValue );
#else
    return OctToCharMB( iValue );
#endif
}

inline WChar String::DecToCharW( UInt iValue ) const {
    return ( WTEXT('0') + (WChar)iValue );
}
inline MBChar String::DecToCharMB( UInt iValue ) const {
    return ( MBTEXT('0') + (MBChar)iValue );
}
inline AChar String::DecToCharA( UInt iValue ) const {
    return DecToCharMB( iValue );
}
inline GChar String::DecToChar( UInt iValue ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return DecToCharW( iValue );
#else
    return DecToCharMB( iValue );
#endif
}

inline WChar String::HexToCharW( UInt iValue ) const {
    if ( iValue < 10 )
        return ( WTEXT('0') + (WChar)iValue );
    return ( WTEXT('a') + (WChar)(iValue - 10) );
}
inline MBChar String::HexToCharMB( UInt iValue ) const {
    if ( iValue < 10 )
        return ( MBTEXT('0') + (MBChar)iValue );
    return ( MBTEXT('a') + (MBChar)(iValue - 10) );
}
inline AChar String::HexToCharA( UInt iValue ) const {
    return HexToCharMB( iValue );
}
inline GChar String::HexToChar( UInt iValue ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return HexToCharW( iValue );
#else
    return HexToCharMB( iValue );
#endif
}

inline UInt String::LengthA( const AChar * inStr ) const {
    return LengthMB( inStr );
}
inline UInt String::Length( const GChar * inStr ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return LengthW( inStr );
#else
    return LengthMB( inStr );
#endif
}

inline AChar * String::CopyA( AChar * strTo, const AChar * strFrom ) const {
    return CopyMB( strTo, strFrom );
}
inline GChar * String::Copy( GChar * strTo, const GChar * strFrom ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return CopyW( strTo, strFrom );
#else
    return CopyMB( strTo, strFrom );
#endif
}

inline AChar * String::NCopyA( AChar * strTo, const AChar * strFrom, UInt iLength ) const {
    return NCopyMB( strTo, strFrom, iLength );
}
inline GChar * String::NCopy( GChar * strTo, const GChar * strFrom, UInt iLength ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return NCopyW( strTo, strFrom, iLength );
#else
    return NCopyMB( strTo, strFrom, iLength );
#endif
}

inline Int String::CmpA( const AChar * strLeft, const AChar * strRight ) const {
    return CmpMB( strLeft, strRight );
}
inline Int String::Cmp( const GChar * strLeft, const GChar * strRight ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return CmpW( strLeft, strRight );
#else
    return CmpMB( strLeft, strRight );
#endif
}

inline Int String::NCmpA( const AChar * strLeft, const AChar * strRight, UInt iLength ) const {
    return NCmpMB( strLeft, strRight, iLength );
}
inline Int String::NCmp( const GChar * strLeft, const GChar * strRight, UInt iLength ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return NCmpW( strLeft, strRight, iLength );
#else
    return NCmpMB( strLeft, strRight, iLength );
#endif
}

inline const AChar * String::FindA( const AChar * inStr, AChar ch ) const {
    return FindMB( inStr, ch );
}
inline const GChar * String::Find( const GChar * inStr, GChar ch ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return FindW( inStr, ch );
#else
    return FindMB( inStr, ch );
#endif
}

inline Void String::ReverseA( AChar * pStr, UInt iLength ) const {
    ReverseMB( pStr, iLength );
}
inline Void String::Reverse( GChar * pStr, UInt iLength ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    ReverseW( pStr, iLength );
#else
    ReverseMB( pStr, iLength );
#endif
}

inline UInt64 String::ToUIntA( const AChar * inStr, const AChar ** outStr ) const {
    return ToUIntMB( inStr, outStr );
}
inline UInt64 String::ToUInt( const GChar * inStr, const GChar ** outStr ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return ToUIntW( inStr, outStr );
#else
    return ToUIntMB( inStr, outStr );
#endif
}

inline Int64 String::ToIntA( const AChar * inStr, const AChar ** outStr ) const {
    return ToIntMB( inStr, outStr );
}
inline Int64 String::ToInt( const GChar * inStr, const GChar ** outStr ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return ToIntW( inStr, outStr );
#else
    return ToIntMB( inStr, outStr );
#endif
}

inline Float String::ToFloatA( const AChar * inStr, const AChar ** outStr ) const {
    return ToFloatMB( inStr, outStr );
}
inline Float String::ToFloat( const GChar * inStr, const GChar ** outStr ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return ToFloatW( inStr, outStr );
#else
    return ToFloatMB( inStr, outStr );
#endif
}

inline Double String::ToDoubleA( const AChar * inStr, const AChar ** outStr ) const {
    return ToDoubleMB( inStr, outStr );
}
inline Double String::ToDouble( const GChar * inStr, const GChar ** outStr ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return ToDoubleW( inStr, outStr );
#else
    return ToDoubleMB( inStr, outStr );
#endif
}

inline AChar * String::FromUIntA( AChar * outStr, UInt64 iValue, Radix iRadix, UInt iMinDigits ) const {
    return FromUIntMB( outStr, iValue, iRadix, iMinDigits );
}
inline GChar * String::FromUInt( GChar * outStr, UInt64 iValue, Radix iRadix, UInt iMinDigits ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return FromUIntW( outStr, iValue, iRadix, iMinDigits );
#else
    return FromUIntMB( outStr, iValue, iRadix, iMinDigits );
#endif
}

inline AChar * String::FromIntA( AChar * outStr, Int64 iValue, Radix iRadix, UInt iMinDigits ) const {
    return FromIntMB( outStr, iValue, iRadix, iMinDigits );
}
inline GChar * String::FromInt( GChar * outStr, Int64 iValue, Radix iRadix, UInt iMinDigits ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return FromIntW( outStr, iValue, iRadix, iMinDigits );
#else
    return FromIntMB( outStr, iValue, iRadix, iMinDigits );
#endif
}

inline AChar * String::FromFloatA( AChar * outStr, Float fValue, UInt iMinDigits, UInt iMaxDigits ) const {
    return FromFloatMB( outStr, fValue, iMinDigits, iMaxDigits );
}
inline GChar * String::FromFloat( GChar * outStr, Float fValue, UInt iMinDigits, UInt iMaxDigits ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return FromFloatW( outStr, fValue, iMinDigits, iMaxDigits );
#else
    return FromFloatMB( outStr, fValue, iMinDigits, iMaxDigits );
#endif
}

inline AChar * String::FromDoubleA( AChar * outStr, Double fValue, UInt iMinDigits, UInt iMaxDigits ) const {
    return FromDoubleMB( outStr, fValue, iMinDigits, iMaxDigits );
}
inline GChar * String::FromDouble( GChar * outStr, Double fValue, UInt iMinDigits, UInt iMaxDigits ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return FromDoubleW( outStr, fValue, iMinDigits, iMaxDigits );
#else
    return FromDoubleMB( outStr, fValue, iMinDigits, iMaxDigits );
#endif
}

inline const AChar * String::ScanVAA( const AChar * inStr, const AChar * strFormat, VArgPtr pArgList ) const {
    return ScanVAMB( inStr, strFormat, pArgList );
}
inline const GChar * String::ScanVA( const GChar * inStr, const GChar * strFormat, VArgPtr pArgList ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return ScanVAW( inStr, strFormat, pArgList );
#else
    return ScanVAMB( inStr, strFormat, pArgList );
#endif
}

inline const WChar * String::ScanW( const WChar * inStr, const WChar * strFormat, ... ) const {
    VArgPtr pArgList;
    VArg_Init( pArgList, strFormat );
    return ScanVAW( inStr, strFormat, pArgList );
}
inline const MBChar * String::ScanMB( const MBChar * inStr, const MBChar * strFormat, ... ) const {
    VArgPtr pArgList;
    VArg_Init( pArgList, strFormat );
    return ScanVAMB( inStr, strFormat, pArgList );
}
inline const AChar * String::ScanA( const AChar * inStr, const AChar * strFormat, ... ) const {
    VArgPtr pArgList;
    VArg_Init( pArgList, strFormat );
    return ScanVAA( inStr, strFormat, pArgList );
}
inline const GChar * String::Scan( const GChar * inStr, const GChar * strFormat, ... ) const {
    VArgPtr pArgList;
    VArg_Init( pArgList, strFormat );
    return ScanVA( inStr, strFormat, pArgList );
}

inline AChar * String::FormatVAA( AChar * outStr, const AChar * strFormat, VArgPtr pArgList ) const {
    return FormatVAMB( outStr, strFormat, pArgList );
}
inline GChar * String::FormatVA( GChar * outStr, const GChar * strFormat, VArgPtr pArgList ) const {
#if ( defined(UNICODE) || defined(_UNICODE) )
    return FormatVAW( outStr, strFormat, pArgList );
#else
    return FormatVAMB( outStr, strFormat, pArgList );
#endif
}

inline WChar * String::FormatW( WChar * outStr, const WChar * strFormat, ... ) const {
    VArgPtr pArgList;
    VArg_Init( pArgList, strFormat );
    return FormatVAW( outStr, strFormat, pArgList );
}
inline MBChar * String::FormatMB( MBChar * outStr, const MBChar * strFormat, ... ) const {
    VArgPtr pArgList;
    VArg_Init( pArgList, strFormat );
    return FormatVAMB( outStr, strFormat, pArgList );
}
inline AChar * String::FormatA( AChar * outStr, const AChar * strFormat, ... ) const {
    VArgPtr pArgList;
    VArg_Init( pArgList, strFormat );
    return FormatVAA( outStr, strFormat, pArgList );
}
inline GChar * String::Format( GChar * outStr, const GChar * strFormat, ... ) const {
    VArgPtr pArgList;
    VArg_Init( pArgList, strFormat );
    return FormatVA( outStr, strFormat, pArgList );
}
