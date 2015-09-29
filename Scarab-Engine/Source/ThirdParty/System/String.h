/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/System/String.h
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
// Header prelude
#ifndef SCARAB_THIRDPARTY_SYSTEM_STRING_H
#define SCARAB_THIRDPARTY_SYSTEM_STRING_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Platform.h"

#include "Hardware/FPU.h" // Power10 needed

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define StringFn String::GetInstancePtr()

typedef Word CharacterClass;
#define CHARACTER_CLASS_UNKNOWN     0x0000
#define CHARACTER_CLASS_CONTROL     0x0001 // Not printables
#define CHARACTER_CLASS_ALPHA       0x0002
#define CHARACTER_CLASS_UPPERCASE   0x0004
#define CHARACTER_CLASS_LOWERCASE   0x0008
#define CHARACTER_CLASS_DIGIT       0x0010
#define CHARACTER_CLASS_HEXDIGIT    0x0020
#define CHARACTER_CLASS_PUNCTUATION 0x0040
#define CHARACTER_CLASS_BLANK       0x0080 // space, tabulations, cr/lf ?
#define CHARACTER_CLASS_SPACE       0x0100 // space, tabulations, cr/lf ?
#define CHARACTER_CLASS_SPECIAL     0x0200

enum Radix
{
    RADIX_BINARY = 2,
    RADIX_OCTAL = 8,
    RADIX_DECIMAL = 10,
    RADIX_HEXADECIMAL = 16
};

    // Standard locales
#define LOCALENAME_MAX_LENGTH 85 // language(8),region(8),script(64),separators(2),prefix(2),null(1)

#define LOCALENAME_EN_GB   TEXT("en-gb") // English, Great Britain
#define LOCALENAME_EN_US   TEXT("en-us") // English, United States
#define LOCALENAME_EN_CA   TEXT("en-ca") // English, Canada
#define LOCALENAME_FR_FR   TEXT("fr-fr") // French, France
#define LOCALENAME_FR_BE   TEXT("fr-be") // French, Belgium
#define LOCALENAME_FR_CH   TEXT("fr-ch") // French, Switzerland
#define LOCALENAME_FR_CA   TEXT("fr-ca") // French, Canada
#define LOCALENAME_ES_ES   TEXT("es-es") // Spanish, Spain
#define LOCALENAME_PT_PT   TEXT("pt-pt") // Portuguese, Portugal
#define LOCALENAME_DE_DE   TEXT("de-de") // German, Germany
#define LOCALENAME_DE_AT   TEXT("de-at") // German, Austria
#define LOCALENAME_DE_CH   TEXT("de-ch") // German, Switzerland
#define LOCALENAME_NL_NL   TEXT("nl-nl") // Dutch, Netherlands
#define LOCALENAME_NL_BE   TEXT("nl-be") // Dutch, Belgium
#define LOCALENAME_IT_IT   TEXT("it-it") // Italian, Italy
#define LOCALENAME_IT_CH   TEXT("it-ch") // Italian, Switzerland
#define LOCALENAME_RU_RU   TEXT("ru-ru") // Russian, Russia
#define LOCALENAME_JA_JP   TEXT("ja-jp") // Japanese, Japan
#define LOCALENAME_KO_KR   TEXT("ko-kr") // Korean, Korea
#define LOCALENAME_ZH_HANS TEXT("zh-hans") // Chinese, simplified
#define LOCALENAME_ZH_HANT TEXT("zh-hant") // Chinese, traditional
// Chinese, mandarin
// Indi, Tamul, Arab

/////////////////////////////////////////////////////////////////////////////////
// The String class
class String
{
    // Discrete singleton interface
public:
    inline static String * GetInstancePtr();

private:
    String();
    ~String();

public:
    // Encoding operations
    inline MBChar * AsciiToMultiByte( MBChar * outStr, const AChar * inStr, UInt iLength ) const;
    inline WChar * AsciiToWideChar( WChar * outStr, const AChar * inStr, UInt iLength ) const;
    WChar * MultiByteToWideChar( WChar * outStr, const MBChar * inStr, UInt iLength ) const;
    MBChar * WideCharToMultiByte( MBChar * outStr, const WChar * inStr, UInt iLength ) const;

    const WChar * NextCharW( const WChar * inStr ) const;
    const MBChar * NextCharMB( const MBChar * inStr ) const;
    inline const AChar * NextCharA( const AChar * inStr ) const;
    inline const GChar * NextChar( const GChar * inStr ) const;

    const WChar * PrevCharW( const WChar * inStart, const WChar * inStr ) const;
    const MBChar * PrevCharMB( const MBChar * inStart, const MBChar * inStr ) const;
    inline const AChar * PrevCharA( const AChar * inStart, const AChar * inStr ) const;
    inline const GChar * PrevChar( const GChar * inStart, const GChar * inStr ) const;

    // Character classification
    CharacterClass GetClassW( WChar ch ) const;
    CharacterClass GetClassMB( MBChar ch ) const;
    inline CharacterClass GetClassA( AChar ch ) const;
    inline CharacterClass GetClass( GChar ch ) const;

    inline Bool IsAsciiW( WChar ch ) const;
    inline Bool IsAsciiMB( MBChar ch ) const;
    inline Bool IsAsciiA( AChar ch ) const;
    inline Bool IsAscii( GChar ch ) const;

    inline Bool IsExtAsciiW( WChar ch ) const;
    inline Bool IsExtAsciiMB( MBChar ch ) const;
    inline Bool IsExtAsciiA( AChar ch ) const;
    inline Bool IsExtAscii( GChar ch ) const;

    inline Bool IsPrintableW( WChar ch ) const;
    inline Bool IsPrintableMB( MBChar ch ) const;
    inline Bool IsPrintableA( AChar ch ) const;
    inline Bool IsPrintable( GChar ch ) const;

    inline Bool IsAlphaW( WChar ch ) const;
    inline Bool IsAlphaMB( MBChar ch ) const;
    inline Bool IsAlphaA( AChar ch ) const;
    inline Bool IsAlpha( GChar ch ) const;

    inline Bool IsAlphaNumW( WChar ch ) const;
    inline Bool IsAlphaNumMB( MBChar ch ) const;
    inline Bool IsAlphaNumA( AChar ch ) const;
    inline Bool IsAlphaNum( GChar ch ) const;

    inline Bool IsLowerCaseW( WChar ch ) const;
    inline Bool IsLowerCaseMB( MBChar ch ) const;
    inline Bool IsLowerCaseA( AChar ch ) const;
    inline Bool IsLowerCase( GChar ch ) const;

    inline Bool IsUpperCaseW( WChar ch ) const;
    inline Bool IsUpperCaseMB( MBChar ch ) const;
    inline Bool IsUpperCaseA( AChar ch ) const;
    inline Bool IsUpperCase( GChar ch ) const;

    inline Bool IsBinDigitW( WChar ch ) const;
    inline Bool IsBinDigitMB( MBChar ch ) const;
    inline Bool IsBinDigitA( AChar ch ) const;
    inline Bool IsBinDigit( GChar ch ) const;

    inline Bool IsOctDigitW( WChar ch ) const;
    inline Bool IsOctDigitMB( MBChar ch ) const;
    inline Bool IsOctDigitA( AChar ch ) const;
    inline Bool IsOctDigit( GChar ch ) const;

    inline Bool IsDecDigitW( WChar ch ) const;
    inline Bool IsDecDigitMB( MBChar ch ) const;
    inline Bool IsDecDigitA( AChar ch ) const;
    inline Bool IsDecDigit( GChar ch ) const;

    inline Bool IsHexDigitW( WChar ch ) const;
    inline Bool IsHexDigitMB( MBChar ch ) const;
    inline Bool IsHexDigitA( AChar ch ) const;
    inline Bool IsHexDigit( GChar ch ) const;

    inline Bool IsBlankW( WChar ch ) const;
    inline Bool IsBlankMB( MBChar ch ) const;
    inline Bool IsBlankA( AChar ch ) const;
    inline Bool IsBlank( GChar ch ) const;

    inline Bool IsSpaceW( WChar ch ) const;
    inline Bool IsSpaceMB( MBChar ch ) const;
    inline Bool IsSpaceA( AChar ch ) const;
    inline Bool IsSpace( GChar ch ) const;

    inline Bool IsPunctuationW( WChar ch ) const;
    inline Bool IsPunctuationMB( MBChar ch ) const;
    inline Bool IsPunctuationA( AChar ch ) const;
    inline Bool IsPunctuation( GChar ch ) const;

    // Character operations
    WChar ToLowerCaseW( WChar ch ) const;
    MBChar ToLowerCaseMB( MBChar ch ) const;
    inline AChar ToLowerCaseA( AChar ch ) const;
    inline GChar ToLowerCase( GChar ch ) const;

    WChar ToUpperCaseW( WChar ch ) const;
    MBChar ToUpperCaseMB( MBChar ch ) const;
    inline AChar ToUpperCaseA( AChar ch ) const;
    inline GChar ToUpperCase( GChar ch ) const;

    inline UInt CharToBinW( WChar ch ) const;
    inline UInt CharToBinMB( MBChar ch ) const;
    inline UInt CharToBinA( AChar ch ) const;
    inline UInt CharToBin( GChar ch ) const;

    inline UInt CharToOctW( WChar ch ) const;
    inline UInt CharToOctMB( MBChar ch ) const;
    inline UInt CharToOctA( AChar ch ) const;
    inline UInt CharToOct( GChar ch ) const;

    inline UInt CharToDecW( WChar ch ) const;
    inline UInt CharToDecMB( MBChar ch ) const;
    inline UInt CharToDecA( AChar ch ) const;
    inline UInt CharToDec( GChar ch ) const;

    inline UInt CharToHexW( WChar ch ) const;
    inline UInt CharToHexMB( MBChar ch ) const;
    inline UInt CharToHexA( AChar ch ) const;
    inline UInt CharToHex( GChar ch ) const;
    
    inline WChar BinToCharW( UInt iValue ) const;
    inline MBChar BinToCharMB( UInt iValue ) const;
    inline AChar BinToCharA( UInt iValue ) const;
    inline GChar BinToChar( UInt iValue ) const;

    inline WChar OctToCharW( UInt iValue ) const;
    inline MBChar OctToCharMB( UInt iValue ) const;
    inline AChar OctToCharA( UInt iValue ) const;
    inline GChar OctToChar( UInt iValue ) const;

    inline WChar DecToCharW( UInt iValue ) const;
    inline MBChar DecToCharMB( UInt iValue ) const;
    inline AChar DecToCharA( UInt iValue ) const;
    inline GChar DecToChar( UInt iValue ) const;

    inline WChar HexToCharW( UInt iValue ) const;
    inline MBChar HexToCharMB( UInt iValue ) const;
    inline AChar HexToCharA( UInt iValue ) const;
    inline GChar HexToChar( UInt iValue ) const;

    // String operations
    UInt LengthW( const WChar * inStr ) const;
    UInt LengthMB( const MBChar * inStr ) const;
    inline UInt LengthA( const AChar * inStr ) const;
    inline UInt Length( const GChar * inStr ) const;

    WChar * CopyW( WChar * strTo, const WChar * strFrom ) const;
    MBChar * CopyMB( MBChar * strTo, const MBChar * strFrom ) const;
    inline AChar * CopyA( AChar * strTo, const AChar * strFrom ) const;
    inline GChar * Copy( GChar * strTo, const GChar * strFrom ) const;

    WChar * NCopyW( WChar * strTo, const WChar * strFrom, UInt iLength ) const;
    MBChar * NCopyMB( MBChar * strTo, const MBChar * strFrom, UInt iLength ) const;
    inline AChar * NCopyA( AChar * strTo, const AChar * strFrom, UInt iLength ) const;
    inline GChar * NCopy( GChar * strTo, const GChar * strFrom, UInt iLength ) const;

    Int CmpW( const WChar * strLeft, const WChar * strRight ) const;
    Int CmpMB( const MBChar * strLeft, const MBChar * strRight ) const;
    inline Int CmpA( const AChar * strLeft, const AChar * strRight ) const;
    inline Int Cmp( const GChar * strLeft, const GChar * strRight ) const;

    Int NCmpW( const WChar * strLeft, const WChar * strRight, UInt iLength ) const;
    Int NCmpMB( const MBChar * strLeft, const MBChar * strRight, UInt iLength ) const;
    inline Int NCmpA( const AChar * strLeft, const AChar * strRight, UInt iLength ) const;
    inline Int NCmp( const GChar * strLeft, const GChar * strRight, UInt iLength ) const;

    const WChar * FindW( const WChar * inStr, WChar ch ) const;
    const MBChar * FindMB( const MBChar * inStr, MBChar ch ) const;
    inline const AChar * FindA( const AChar * inStr, AChar ch ) const;
    inline const GChar * Find( const GChar * inStr, GChar ch ) const;

    Void ReverseW( WChar * pStr, UInt iLength ) const;
    Void ReverseMB( MBChar * pStr, UInt iLength ) const;
    inline Void ReverseA( AChar * pStr, UInt iLength ) const;
    inline Void Reverse( GChar * pStr, UInt iLength ) const;

    UInt64 ToUIntW( const WChar * inStr, const WChar ** outStr = NULL ) const;
    UInt64 ToUIntMB( const MBChar * inStr, const MBChar ** outStr = NULL ) const;
    inline UInt64 ToUIntA( const AChar * inStr, const AChar ** outStr = NULL ) const;
    inline UInt64 ToUInt( const GChar * inStr, const GChar ** outStr = NULL ) const;

    Int64 ToIntW( const WChar * inStr, const WChar ** outStr = NULL ) const;
    Int64 ToIntMB( const MBChar * inStr, const MBChar ** outStr = NULL ) const;
    inline Int64 ToIntA( const AChar * inStr, const AChar ** outStr = NULL ) const;
    inline Int64 ToInt( const GChar * inStr, const GChar ** outStr = NULL ) const;

    Float ToFloatW( const WChar * inStr, const WChar ** outStr = NULL ) const;
    Float ToFloatMB( const MBChar * inStr, const MBChar ** outStr = NULL ) const;
    inline Float ToFloatA( const AChar * inStr, const AChar ** outStr = NULL ) const;
    inline Float ToFloat( const GChar * inStr, const GChar ** outStr = NULL ) const;

    Double ToDoubleW( const WChar * inStr, const WChar ** outStr = NULL ) const;
    Double ToDoubleMB( const MBChar * inStr, const MBChar ** outStr = NULL ) const;
    inline Double ToDoubleA( const AChar * inStr, const AChar ** outStr = NULL ) const;
    inline Double ToDouble( const GChar * inStr, const GChar ** outStr = NULL ) const;

    WChar * FromUIntW( WChar * outStr, UInt64 iValue, Radix iRadix = RADIX_DECIMAL, UInt iMinDigits = 1 ) const;
    MBChar * FromUIntMB( MBChar * outStr, UInt64 iValue, Radix iRadix = RADIX_DECIMAL, UInt iMinDigits = 1 ) const;
    inline AChar * FromUIntA( AChar * outStr, UInt64 iValue, Radix iRadix = RADIX_DECIMAL, UInt iMinDigits = 1 ) const;
    inline GChar * FromUInt( GChar * outStr, UInt64 iValue, Radix iRadix = RADIX_DECIMAL, UInt iMinDigits = 1 ) const;

    WChar * FromIntW( WChar * outStr, Int64 iValue, Radix iRadix = RADIX_DECIMAL, UInt iMinDigits = 1 ) const;
    MBChar * FromIntMB( MBChar * outStr, Int64 iValue, Radix iRadix = RADIX_DECIMAL, UInt iMinDigits = 1 ) const;
    inline AChar * FromIntA( AChar * outStr, Int64 iValue, Radix iRadix = RADIX_DECIMAL, UInt iMinDigits = 1 ) const;
    inline GChar * FromInt( GChar * outStr, Int64 iValue, Radix iRadix = RADIX_DECIMAL, UInt iMinDigits = 1 ) const;

    WChar * FromFloatW( WChar * outStr, Float fValue, UInt iMinDigits = 2, UInt iMaxDigits = 8 ) const;
    MBChar * FromFloatMB( MBChar * outStr, Float fValue, UInt iMinDigits = 2, UInt iMaxDigits = 8 ) const;
    inline AChar * FromFloatA( AChar * outStr, Float fValue, UInt iMinDigits = 2, UInt iMaxDigits = 8 ) const;
    inline GChar * FromFloat( GChar * outStr, Float fValue, UInt iMinDigits = 2, UInt iMaxDigits = 8 ) const;

    WChar * FromDoubleW( WChar * outStr, Double fValue, UInt iMinDigits = 2, UInt iMaxDigits = 8 ) const;
    MBChar * FromDoubleMB( MBChar * outStr, Double fValue, UInt iMinDigits = 2, UInt iMaxDigits = 8 ) const;
    inline AChar * FromDoubleA( AChar * outStr, Double fValue, UInt iMinDigits = 2, UInt iMaxDigits = 8 ) const;
    inline GChar * FromDouble( GChar * outStr, Double fValue, UInt iMinDigits = 2, UInt iMaxDigits = 8 ) const;

    const WChar * ScanVAW( const WChar * inStr, const WChar * strFormat, VArgPtr pArgList ) const;
    const MBChar * ScanVAMB( const MBChar * inStr, const MBChar * strFormat, VArgPtr pArgList ) const;
    inline const AChar * ScanVAA( const AChar * inStr, const AChar * strFormat, VArgPtr pArgList ) const;
    inline const GChar * ScanVA( const GChar * inStr, const GChar * strFormat, VArgPtr pArgList ) const;

    inline const WChar * ScanW( const WChar * inStr, const WChar * strFormat, ... ) const;
    inline const MBChar * ScanMB( const MBChar * inStr, const MBChar * strFormat, ... ) const;
    inline const AChar * ScanA( const AChar * inStr, const AChar * strFormat, ... ) const;
    inline const GChar * Scan( const GChar * inStr, const GChar * strFormat, ... ) const;

    WChar * FormatVAW( WChar * outStr, const WChar * strFormat, VArgPtr pArgList ) const;
    MBChar * FormatVAMB( MBChar * outStr, const MBChar * strFormat, VArgPtr pArgList ) const;
    inline AChar * FormatVAA( AChar * outStr, const AChar * strFormat, VArgPtr pArgList ) const;
    inline GChar * FormatVA( GChar * outStr, const GChar * strFormat, VArgPtr pArgList ) const;

    inline WChar * FormatW( WChar * outStr, const WChar * strFormat, ... ) const;
    inline MBChar * FormatMB( MBChar * outStr, const MBChar * strFormat, ... ) const;
    inline AChar * FormatA( AChar * outStr, const AChar * strFormat, ... ) const;
    inline GChar * Format( GChar * outStr, const GChar * strFormat, ... ) const;

    // Key to Char convertion
    GChar ConvertKeyCode( KeyCode iKey, Bool bShift, Bool bCtrl, Bool bAlt, Bool bNumLock, Bool bCapsLock, Bool bQWERTY = false ) const;

private:

};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "String.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_THIRDPARTY_SYSTEM_STRING_H
