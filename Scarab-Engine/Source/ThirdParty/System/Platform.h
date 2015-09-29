/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/System/Platform.h
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
// Known Bugs : TODO : Add 128-bits built-in type support ...
//                     Don't forget to correct MemAlign128 args & ret type !
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// These are the define values used to detect various things
// WIN32 / _WIN32     :  Microsoft Windows OS
// __linux__	      :  GNU Linux OS
// __cplusplus        :  Using C++ compiler
// UNICODE / _UNICODE : Unicode UTF-16 charset

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_THIRDPARTY_SYSTEM_PLATFORM_H
#define SCARAB_THIRDPARTY_SYSTEM_PLATFORM_H

/////////////////////////////////////////////////////////////////////////////////
// Includes

/////////////////////////////////////////////////////////////////////////////////
// Compiler directives
#pragma warning(disable:4005) // Macro redefinitions
#pragma warning(disable:4127) // Conditional expression is constant
#pragma warning(disable:4201) // Non-Standard extension : unnammed struct/union
#pragma warning(disable:4239) // Non-Standard extension : implicit conversion to reference
//#pragma warning(disable:4310) // Cast truncates constant value
#pragma warning(disable:4345) // Behaviour change with built-in types constructed with
                              // pre-allocated memory (placement new) => default-initialized
#pragma warning(disable:4512) // Could not generate assignment operator
#pragma warning(disable:4725) // fprem1 lacking precision on very old CPUs

/////////////////////////////////////////////////////////////////////////////////
// NULL pointer definition
#ifndef NULL
	#ifdef __cplusplus
		#define NULL 0
	#else
		#define NULL ((void*)0)
	#endif
#endif // !NULL

/////////////////////////////////////////////////////////////////////////////////
// Endianess
static const int _ENDIAN_TEST = 1;
#define SCARAB_BIG_ENDIAN    ( *( (char*)&_ENDIAN_TEST ) == 0 )
#define SCARAB_LITTLE_ENDIAN ( *( (char*)&_ENDIAN_TEST ) != 0 )

/////////////////////////////////////////////////////////////////////////////////
// Architecture (support x86 only)
#if defined(__x86_64__) || defined(_M_X64)
    #define SCARAB_ARCHITECTURE_X86_64
#elif defined(__i386) || defined(_M_IX86)
	#define SCARAB_ARCHITECTURE_X86_32
#else
    #define SCARAB_ARCHITECTURE_UNKNOWN
#endif

/////////////////////////////////////////////////////////////////////////////////
// Operating system (Windows / Linux only)
#if defined(_WIN32)
    #define SCARAB_OS_WINDOWS
#elif defined(__linux__)
    #define SCARAB_OS_LINUX
#else
    #define SCARAB_OS_UNKNOWN
#endif

/////////////////////////////////////////////////////////////////////////////////
// Built-in type definitions
    // Void
typedef void              Void;

    // Bool type definition (if needed)
#ifndef __cplusplus
    enum bool { false = 0, true = 1 };
#endif
typedef	bool              Bool;

    // Integer 8 bits
typedef	signed char       SChar;
typedef	unsigned char     UChar;
typedef	char              Char;

    // Integer 16 bits
typedef	signed short      SShort;
typedef	unsigned short    UShort;
typedef	short             Short;

    // Integer 32 bits
typedef	signed long       SLong;
typedef	unsigned long     ULong;
typedef	long              Long;

	// Integer 64 bits
#if defined(SCARAB_OS_WINDOWS)
    typedef unsigned __int64        UInt64;
    typedef signed __int64          SInt64;
    typedef __int64                 Int64;
#elif defined(SCARAB_OS_LINUX)
    #if (__WORDSIZE == 64)
	    typedef unsigned long int   UInt64;
        typedef signed long int     SInt64;
        typedef long int            Int64;
    #else
	    typedef unsigned long long  UInt64;
        typedef signed long long    SInt64;
        typedef long long           Int64;
    #endif
#endif

    // Integer machine word size (32 bits at time of writting)
typedef	signed int        SInt;
typedef	unsigned int      UInt;
typedef	int               Int;

    // Assembly types equivalences
typedef	UChar             Byte;
typedef	UShort            Word;
typedef	ULong             DWord;
typedef	UInt64            QWord;

/////////////////////////////////////////////////////////////////////////////////
// Built-in types limits
    // Byte
#define BYTE_BITCOUNT 8 // we assume octet size to be constant for ages

    // Integer 8 bits
#define SCHAR_MIN     (-(Char)128)
#define SCHAR_MAX     ((Char)127)
#define UCHAR_MAX     ((Byte)255)

    // Integer 16 bits
#define SSHORT_MIN    (-(Short)32768)
#define SSHORT_MAX    ((Short)32767)
#define USHORT_MAX    ((UShort)0xffff)

    // Integer 32 bits
#define SLONG_MIN     (-(Long)2147483648)
#define SLONG_MAX     ((Long)2147483647)
#define ULONG_MAX     ((ULong)0xffffffff)

    // Integer 64 bits
#define SINT64_MIN    (-(Int64)9223372036854775808)
#define SINT64_MAX    ((Int64)9223372036854775807)
#define UINT64_MAX    ((UInt64)0xffffffffffffffff)

    // Integer machine word size (32 bits at time of writting)
#define SINT_MIN      (-(Int)2147483648)
#define SINT_MAX      ((Int)2147483647)
#define UINT_MAX      ((UInt)0xffffffff)

    // Assembly types equivalences
#define BYTE_MAX      UCHAR_MAX
#define WORD_MAX      USHORT_MAX
#define DWORD_MAX     ULONG_MAX
#define QWORD_MAX     UINT64_MAX

/////////////////////////////////////////////////////////////////////////////////
// INVALID values
#define INVALID_OFFSET ((UInt)0xffffffff)
#define INVALID_SIZE   ((UInt)0xffffffff)

/////////////////////////////////////////////////////////////////////////////////
// IEEE 754 type definitions
    // Float 32 bits
typedef float Float;
typedef union _float_converter { Float f; DWord i; } FloatConverter;

#define FLOAT_DIGITS        6 // Precision # of digits

#define FLOAT_SIGN_BITS		1
#define FLOAT_EXP_BITS		8
#define FLOAT_MANT_BITS		23

#define FLOAT_SIGN_MASK		0x80000000
#define FLOAT_EXP_MASK		0x7f800000
#define FLOAT_MANT_MASK		0x007fffff

#define FLOAT_EXP_BIAS		127

static const ULong _FLOAT_NAN_REPR = 0x7fc00000;
#define FLOAT_NAN ( *((Float*)(&_FLOAT_NAN_REPR)) )

    // Double 64 bits
typedef double Double;
typedef union _double_converter { Double f; QWord i; } DoubleConverter;

#define DOUBLE_DIGITS       15 // Precision # of digits

#define DOUBLE_SIGN_BITS	1
#define DOUBLE_EXP_BITS	    11
#define DOUBLE_MANT_BITS	52

#define DOUBLE_SIGN_MASK	0x8000000000000000    
#define DOUBLE_EXP_MASK	    0x7ff0000000000000
#define DOUBLE_MANT_MASK	0x000fffffffffffff

#define DOUBLE_EXP_BIAS	    1023

static const UInt64 _DOUBLE_NAN_REPR = 0x7ff8000000000000;
#define DOUBLE_NAN ( *((Double*)(&_DOUBLE_NAN_REPR)) )

/////////////////////////////////////////////////////////////////////////////////
// IEEE 754 type limits
    // Float 32 bits
#define FLOAT_EPSILON   1.192092896e-07f // Min positive value such that (1.0 + Eps) != 1.0
#define FLOAT_MIN       1.175494351e-38f // Minimum positive value
#define FLOAT_MAX       3.402823466e+38f // Maximum positive value
#define FLOAT_INFINITE  FLOAT_MAX

#define FLOAT_EXP_MIN_BIN   (-126)
#define FLOAT_EXP_MAX_BIN   127
#define FLOAT_EXP_MIN       (-37) // Minimum decimal exponent
#define FLOAT_EXP_MAX       38    // Maximum decimal exponent

    // Double 64 bits
#define DOUBLE_EPSILON  2.2204460492503131e-016 // Min positive value such that (1.0 + Eps) != 1.0
#define DOUBLE_MIN      2.2250738585072014e-308 // Minimum positive value
#define DOUBLE_MAX      1.7976931348623158e+308 // Maximum positive value
#define DOUBLE_INFINITE DOUBLE_MAX

#define DOUBLE_EXP_MIN_BIN  (-1022)
#define DOUBLE_EXP_MAX_BIN  1023
#define DOUBLE_EXP_MIN      (-307) // Minimum decimal exponent
#define DOUBLE_EXP_MAX      308    // Maximum decimal exponent

/////////////////////////////////////////////////////////////////////////////////
// Variable Arguments support
// Be carefull with calling conventions, this is _cdecl / _stdcall version, NEVER use with _thiscall or _fastcall
// (noobs : _cdecl/_stdcall have arguments ordered with growing adresses ...)
// For obvious portability reasons, you should make minimal use of this ...
typedef Byte * VArgPtr;
#define _VArg_SizeOf( _n )              ( (sizeof(_n) + sizeof(Int) - 1) & (~(sizeof(Int) - 1)) )
#define VArg_Init( _vargptr, _lastarg ) ( (_vargptr) = (VArgPtr)(&(_lastarg)) + _VArg_SizeOf(_lastarg) )
#define VArg_Get( _vargptr, _argtype )  ( *(_argtype*)( ((_vargptr)+=_VArg_SizeOf(_argtype)) - _VArg_SizeOf(_argtype) ) )
#define VArg_Destroy( _vargptr )        ( (_vargptr) = NULL )

/////////////////////////////////////////////////////////////////////////////////
// Debug basic support
void _DebugAssert_OutputDebugString( const char * strExpr, const char * strFilename, UInt iLine );

#define DebugBreak { __asm int 3 }
#define DebugAssert( _expression ) \
    if ( _expression ) {} else { \
        _DebugAssert_OutputDebugString( #_expression, __FILE__, __LINE__ ); \
        DebugBreak; \
    }

/////////////////////////////////////////////////////////////////////////////////
// Bit-wise functions
#define bit_sizeof(x) (sizeof(x) << 3) // *8

    // masking
inline Byte LowWord(Word value);
inline Word LowWord(DWord value);
inline DWord LowWord(QWord value);
inline Byte HighWord(Word value);
inline Word HighWord(DWord value);
inline DWord HighWord(QWord value);

    // circular permutation
inline Byte BitRotL(Byte value, UInt shift);
inline Word BitRotL(Word value, UInt shift);
inline DWord BitRotL(DWord value, UInt shift);
inline QWord BitRotL(QWord value, UInt shift);
inline Byte BitRotR(Byte value, UInt shift);
inline Word BitRotR(Word value, UInt shift);
inline DWord BitRotR(DWord value, UInt shift);
inline QWord BitRotR(QWord value, UInt shift);

    // mirror
inline Byte BitMirror(Byte value);
inline Word BitMirror(Word value);
inline DWord BitMirror(DWord value);
inline QWord BitMirror(QWord value);

inline Byte ByteMirror(Byte value);
inline Word ByteMirror(Word value);
inline DWord ByteMirror(DWord value);
inline QWord ByteMirror(QWord value);

/////////////////////////////////////////////////////////////////////////////////
// All purpose functions
template<typename T> inline T         DivUp(const T & lhs, const T & rhs);
template<typename T> inline const T & Min(const T & lhs, const T & rhs);
template<typename T> inline const T & Max(const T & lhs, const T & rhs);
template<typename T> inline T         Clamp(const T & val, const T & low, const T & high);
template<typename T> inline Void      Swap(T * lhs, T * rhs);

template<typename T>
Bool Contains( const T * arrItems, UInt iSize, const T & rItem );

template<typename T>
Void QuickSort( T * arrItems, UInt iSize, Int (*pfCompare)(const T &, const T &, Void*), Void * pUserData = NULL );

/////////////////////////////////////////////////////////////////////////////////
// Memory functions
inline UChar  MemAlign8(UChar bitSize);
inline UShort MemAlign16(UShort byteSize);
inline ULong  MemAlign32(ULong byteSize);
inline UInt64 MemAlign64(UInt64 byteSize);
inline UInt64 MemAlign128(UInt64 byteSize); // SSE will need this

Void MemCopy8(Void * pTo, const Void * pFrom, UChar size);
Void MemCopy16(Void * pTo, const Void * pFrom, UShort size);
Void MemCopy32(Void * pTo, const Void * pFrom, ULong size);
Void MemCopy64(Void * pTo, const Void * pFrom, UInt64 size);

Void MemCopyBackward8(Void * pTo, const Void * pFrom, UChar size);
Void MemCopyBackward16(Void * pTo, const Void * pFrom, UShort size);
Void MemCopyBackward32(Void * pTo, const Void * pFrom, ULong size);
Void MemCopyBackward64(Void * pTo, const Void * pFrom, UInt64 size);

Void MemSet8(Void * ptr, Byte value, UChar size);
Void MemSet16(Void * ptr, Byte value, UShort size);
Void MemSet32(Void * ptr, Byte value, ULong size);
Void MemSet64(Void * ptr, Byte value, UInt64 size);

Void MemZero8(Void * ptr, UChar size);
Void MemZero16(Void * ptr, UShort size);
Void MemZero32(Void * ptr, ULong size);
Void MemZero64(Void * ptr, UInt64 size);

Int MemCmp8(const Void * pLeft, const Void * pRight, UChar size);
Int MemCmp16(const Void * pLeft, const Void * pRight, UShort size);
Int MemCmp32(const Void * pLeft, const Void * pRight, ULong size);
Int MemCmp64(const Void * pLeft, const Void * pRight, UInt64 size);

#if defined(_PLATFORM_64)
    #define MemAlign         MemAlign64
    #define MemCopy          MemCopy64
    #define MemCopyBackward  MemCopyBackward64
    #define MemSet           MemSet64
    #define MemZero          MemZero64
    #define MemCmp           MemCmp64
#elif defined(_PLATFORM_32) || 1
    #define MemAlign         MemAlign32
    #define MemCopy          MemCopy32
    #define MemCopyBackward  MemCopyBackward32
    #define MemSet           MemSet32
    #define MemZero          MemZero32
    #define MemCmp           MemCmp32
#elif defined(_PLATFORM_16)
    #define MemAlign         MemAlign16
    #define MemCopy          MemCopy16
    #define MemCopyBackward  MemCopyBackward16
    #define MemSet           MemSet16
    #define MemZero          MemZero16
    #define MemCmp           MemCmp16
#elif defined(_PLATFORM_8)
    #define MemAlign         MemAlign8
    #define MemCopy          MemCopy8
    #define MemCopyBackward  MemCopyBackward8
    #define MemSet           MemSet8
    #define MemZero          MemZero8
    #define MemCmp           MemCmp8
#endif

/////////////////////////////////////////////////////////////////////////////////
// Characters & Strings
// Length on all strings is ALLWAYS in character count, even for UTF-8,
// regardless of lead/trail bytes, to ensure consistency.
// Ending NULLBYTE is ALLWAYS present/enforced and lengths NEVER count it.
// This way length is ALLWAYS equal to memory size minus one.

    // Unicode UTF-16
typedef wchar_t       WideChar;
typedef WideChar      WChar;
typedef WChar *       WString;
typedef const WChar * WCString;
#define WNULLBYTE     ( (WChar)0 )
#define WTEXT(_str)   ( L##_str )

    // Legacy UTF-8 (single byte chars maps to ASCII)
typedef char           MultiByteChar;
typedef MultiByteChar  MBChar;
typedef MBChar *       MBString;
typedef const MBChar * MBCString;
#define MBNULLBYTE     ( (MBChar)0 )
#define MBTEXT(_str)   ( _str )

    // Legacy ASCII
typedef char          AsciiChar;
typedef AsciiChar     AChar;
typedef AChar *       AString;
typedef const AChar * ACString;
#define ANULLBYTE     ( (AChar)0 )
#define ATEXT(_str)   ( _str )

    // Generic
#if ( defined(UNICODE) || defined(_UNICODE) )
    // Unicode, UTF-16 encoding
    #define STRING_USE_UTF16

    typedef WChar GChar;
    typedef WString GString;
    typedef WCString GCString;
    #define NULLBYTE WNULLBYTE
    #define TEXT(_str) WTEXT(_str)
#else
    // Legacy, UTF-8 encoding
    #define STRING_USE_UTF8

    typedef MBChar GChar;
    typedef MBString GString;
    typedef MBCString GCString;
    #define NULLBYTE MBNULLBYTE
    #define TEXT(_str) MBTEXT(_str)
#endif

/////////////////////////////////////////////////////////////////////////////////
// Security remainder ...
// Beware of Memory, String operations and the likes, here is a very simple case
// of buffer overflow as a proof of concept, or how a memcpy-op can become a random
// code executer ... 
//Void EvilFunc()
//{
//    // After overflowing EIP, with &EvilFunc, the CPU
//    // takes us there while believing he's returning
//    // from WeakFunc to main ...
//    printf("\nBuffer Overflowed !");
//}
//Void WeakFunc( const Char * str, UInt iLength )
//{
//    // Something very evil !!!
//    Char Buffer[8];
//    for(UInt i = 0; i < iLength; ++i)
//        Buffer[i] = str[i];
//}
//int main(int argc, char ** argv)
//{
//    // Here we reproduce stack EBP correct value so that the OS can't notice,
//    // and replace the saved EIP with the address of our code.
//    // EvilFunc is at 0x0042caf0, 0x0013ff68 is our stack pointer and closely
//    // related to &Buffer in WeakFunc, by call/ret cleverly we can force the
//    // CPU to push &WeakFunc and &Buffer dynamically, think about it for fun ;)
//    WeakFunc( "01234567\xCC\xCC\xCC\xCC\x68\xFF\x13\x00\xF0\xCA\x42\x00", 20 );
//
//    // We never get there ...
//    return 0;
//}
// Okay that's a trivial stack overflow, real cases are MUCH more complex
// nowadays ... but there's allways a way to exploit !
// (one-byte overflow, heap overflow, seh, dll-injection, ...)
// DEP & BufferCheck technologies also have their flaws, notice DEP is useless
// when, just like above, the exploit knows a code address to put his code and
// redirect to. Most of time this is not the case and data has to be executed
// to inject itself (shellcode) but, again, there's allways a way ...
/////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////
// KeyCode definitions
typedef DWord KeyCode;

#define KEYCODE_NULL 0

#define KEYCODE_FAMILY(_keycode)    ( (_keycode) & 0xff000000 )
#define KEYCODE_SUBFAMILY(_keycode) ( (_keycode) & 0xffff0000 )
#define KEYCODE_INDEX(_keycode)     ( (_keycode) & 0x000000ff ) // index in the keystate array

// Families
#define KEYCODE_FAMILY_NULL           0x00000000
#define KEYCODE_FAMILY_PRINTABLES     0x01000000
#define KEYCODE_FAMILY_DISPLACEMENTS  0x02000000
#define KEYCODE_FAMILY_FUNCTIONS      0x03000000
#define KEYCODE_FAMILY_SPECIALS       0x04000000
#define KEYCODE_FAMILY_TOGGLES        0x05000000
#define KEYCODE_FAMILY_MOUSE          0x06000000

// Sub-Families
#define KEYCODE_SUBFAMILY_NULL    0x00000000
#define KEYCODE_SUBFAMILY_CHARS   0x01010000 // characters A-Z & space
#define KEYCODE_SUBFAMILY_NUMBERS 0x01020000 // Figures 0-9 & numpad versions
#define KEYCODE_SUBFAMILY_SYMBOLS 0x01030000 // mostly OEMs
#define KEYCODE_SUBFAMILY_STDFUNC 0x03010000 // F1 - F12
#define KEYCODE_SUBFAMILY_EXTFUNC 0x03020000 // empty for now
#define KEYCODE_SUBFAMILY_SHIFTS  0x05010000 // Ctrl Alt Shift
#define KEYCODE_SUBFAMILY_LOCKS   0x05020000 // NumLock CapsLock ScrollLock

// Key-Codes : Characters
#define KEYCODE_SPACE ( KEYCODE_SUBFAMILY_CHARS | 0x01 )
#define KEYCODE_A     ( KEYCODE_SUBFAMILY_CHARS | 0x02 )
#define KEYCODE_B     ( KEYCODE_SUBFAMILY_CHARS | 0x03 )
#define KEYCODE_C     ( KEYCODE_SUBFAMILY_CHARS | 0x04 )
#define KEYCODE_D     ( KEYCODE_SUBFAMILY_CHARS | 0x05 )
#define KEYCODE_E     ( KEYCODE_SUBFAMILY_CHARS | 0x06 )
#define KEYCODE_F     ( KEYCODE_SUBFAMILY_CHARS | 0x07 )
#define KEYCODE_G     ( KEYCODE_SUBFAMILY_CHARS | 0x08 )
#define KEYCODE_H     ( KEYCODE_SUBFAMILY_CHARS | 0x09 )
#define KEYCODE_I     ( KEYCODE_SUBFAMILY_CHARS | 0x0a )
#define KEYCODE_J     ( KEYCODE_SUBFAMILY_CHARS | 0x0b )
#define KEYCODE_K     ( KEYCODE_SUBFAMILY_CHARS | 0x0c )
#define KEYCODE_L     ( KEYCODE_SUBFAMILY_CHARS | 0x0d )
#define KEYCODE_M     ( KEYCODE_SUBFAMILY_CHARS | 0x0e )
#define KEYCODE_N     ( KEYCODE_SUBFAMILY_CHARS | 0x0f )
#define KEYCODE_O     ( KEYCODE_SUBFAMILY_CHARS | 0x10 )
#define KEYCODE_P     ( KEYCODE_SUBFAMILY_CHARS | 0x11 )
#define KEYCODE_Q     ( KEYCODE_SUBFAMILY_CHARS | 0x12 )
#define KEYCODE_R     ( KEYCODE_SUBFAMILY_CHARS | 0x13 )
#define KEYCODE_S     ( KEYCODE_SUBFAMILY_CHARS | 0x14 )
#define KEYCODE_T     ( KEYCODE_SUBFAMILY_CHARS | 0x15 )
#define KEYCODE_U     ( KEYCODE_SUBFAMILY_CHARS | 0x16 )
#define KEYCODE_V     ( KEYCODE_SUBFAMILY_CHARS | 0x17 )
#define KEYCODE_W     ( KEYCODE_SUBFAMILY_CHARS | 0x18 )
#define KEYCODE_X     ( KEYCODE_SUBFAMILY_CHARS | 0x19 )
#define KEYCODE_Y     ( KEYCODE_SUBFAMILY_CHARS | 0x1a )
#define KEYCODE_Z     ( KEYCODE_SUBFAMILY_CHARS | 0x1b )

// Key-Codes : Numbers
#define KEYCODE_0        ( KEYCODE_SUBFAMILY_NUMBERS | 0x1c )
#define KEYCODE_1        ( KEYCODE_SUBFAMILY_NUMBERS | 0x1d )
#define KEYCODE_2        ( KEYCODE_SUBFAMILY_NUMBERS | 0x1e )
#define KEYCODE_3        ( KEYCODE_SUBFAMILY_NUMBERS | 0x1f )
#define KEYCODE_4        ( KEYCODE_SUBFAMILY_NUMBERS | 0x20 )
#define KEYCODE_5        ( KEYCODE_SUBFAMILY_NUMBERS | 0x21 )
#define KEYCODE_6        ( KEYCODE_SUBFAMILY_NUMBERS | 0x22 )
#define KEYCODE_7        ( KEYCODE_SUBFAMILY_NUMBERS | 0x23 )
#define KEYCODE_8        ( KEYCODE_SUBFAMILY_NUMBERS | 0x24 )
#define KEYCODE_9        ( KEYCODE_SUBFAMILY_NUMBERS | 0x25 )
#define KEYCODE_NUMPAD_0 ( KEYCODE_SUBFAMILY_NUMBERS | 0x26 )
#define KEYCODE_NUMPAD_1 ( KEYCODE_SUBFAMILY_NUMBERS | 0x27 )
#define KEYCODE_NUMPAD_2 ( KEYCODE_SUBFAMILY_NUMBERS | 0x28 )
#define KEYCODE_NUMPAD_3 ( KEYCODE_SUBFAMILY_NUMBERS | 0x29 )
#define KEYCODE_NUMPAD_4 ( KEYCODE_SUBFAMILY_NUMBERS | 0x2a )
#define KEYCODE_NUMPAD_5 ( KEYCODE_SUBFAMILY_NUMBERS | 0x2b )
#define KEYCODE_NUMPAD_6 ( KEYCODE_SUBFAMILY_NUMBERS | 0x2c )
#define KEYCODE_NUMPAD_7 ( KEYCODE_SUBFAMILY_NUMBERS | 0x2d )
#define KEYCODE_NUMPAD_8 ( KEYCODE_SUBFAMILY_NUMBERS | 0x2e )
#define KEYCODE_NUMPAD_9 ( KEYCODE_SUBFAMILY_NUMBERS | 0x2f )

// Key-Codes : Symbols
#define KEYCODE_NUMPAD_ADD ( KEYCODE_SUBFAMILY_SYMBOLS | 0x30 )
#define KEYCODE_NUMPAD_SUB ( KEYCODE_SUBFAMILY_SYMBOLS | 0x31 )
#define KEYCODE_NUMPAD_MUL ( KEYCODE_SUBFAMILY_SYMBOLS | 0x32 )
#define KEYCODE_NUMPAD_DIV ( KEYCODE_SUBFAMILY_SYMBOLS | 0x33 )
#define KEYCODE_NUMPAD_DOT ( KEYCODE_SUBFAMILY_SYMBOLS | 0x34 )
#define KEYCODE_OEM_1      ( KEYCODE_SUBFAMILY_SYMBOLS | 0x35 ) // QWERTY = ';:' / AZERTY = '$£¤'
#define KEYCODE_OEM_PLUS   ( KEYCODE_SUBFAMILY_SYMBOLS | 0x36 ) // QWERTY = '=+' / AZERTY = '=+}'
#define KEYCODE_OEM_COMMA  ( KEYCODE_SUBFAMILY_SYMBOLS | 0x37 ) // QWERTY = ',<' / AZERTY = ',?'
#define KEYCODE_OEM_MINUS  ( KEYCODE_SUBFAMILY_SYMBOLS | 0x38 ) // QWERTY = '-_' / AZERTY = void
#define KEYCODE_OEM_PERIOD ( KEYCODE_SUBFAMILY_SYMBOLS | 0x39 ) // QWERTY = '.>' / AZERTY = ';.'
#define KEYCODE_OEM_2      ( KEYCODE_SUBFAMILY_SYMBOLS | 0x3a ) // QWERTY = '/?' / AZERTY = ':/'
#define KEYCODE_OEM_3      ( KEYCODE_SUBFAMILY_SYMBOLS | 0x3b ) // QWERTY = '`~' / AZERTY = 'ù%'
#define KEYCODE_OEM_4      ( KEYCODE_SUBFAMILY_SYMBOLS | 0x3c ) // QWERTY = '[{' / AZERTY = ')°]'
#define KEYCODE_OEM_5      ( KEYCODE_SUBFAMILY_SYMBOLS | 0x3d ) // QWERTY = '\|' / AZERTY = '*µ'
#define KEYCODE_OEM_6      ( KEYCODE_SUBFAMILY_SYMBOLS | 0x3e ) // QWERTY = ']}' / AZERTY = '^¨'
#define KEYCODE_OEM_7      ( KEYCODE_SUBFAMILY_SYMBOLS | 0x3f ) // QWERTY = ''"' / AZERTY = '²'
#define KEYCODE_OEM_8      ( KEYCODE_SUBFAMILY_SYMBOLS | 0x40 ) // QWERTY = void / AZERTY = '!§'
#define KEYCODE_OEM_102    ( KEYCODE_SUBFAMILY_SYMBOLS | 0x41 ) // QWERTY = '\|' / AZERTY = '<>' (RT 102-keys keyb)

// Key-Codes : Displacements
#define KEYCODE_UP       ( KEYCODE_FAMILY_DISPLACEMENTS | 0x42 )
#define KEYCODE_DOWN     ( KEYCODE_FAMILY_DISPLACEMENTS | 0x43 )
#define KEYCODE_LEFT     ( KEYCODE_FAMILY_DISPLACEMENTS | 0x44 )
#define KEYCODE_RIGHT    ( KEYCODE_FAMILY_DISPLACEMENTS | 0x45 )
#define KEYCODE_PAGEUP   ( KEYCODE_FAMILY_DISPLACEMENTS | 0x46 )
#define KEYCODE_PAGEDOWN ( KEYCODE_FAMILY_DISPLACEMENTS | 0x47 )
#define KEYCODE_HOME     ( KEYCODE_FAMILY_DISPLACEMENTS | 0x48 )
#define KEYCODE_END      ( KEYCODE_FAMILY_DISPLACEMENTS | 0x49 )

// Key-Codes : Standard functions
#define KEYCODE_F1  ( KEYCODE_SUBFAMILY_STDFUNC | 0x4a )
#define KEYCODE_F2  ( KEYCODE_SUBFAMILY_STDFUNC | 0x4b )
#define KEYCODE_F3  ( KEYCODE_SUBFAMILY_STDFUNC | 0x4c )
#define KEYCODE_F4  ( KEYCODE_SUBFAMILY_STDFUNC | 0x4d )
#define KEYCODE_F5  ( KEYCODE_SUBFAMILY_STDFUNC | 0x4e )
#define KEYCODE_F6  ( KEYCODE_SUBFAMILY_STDFUNC | 0x4f )
#define KEYCODE_F7  ( KEYCODE_SUBFAMILY_STDFUNC | 0x50 )
#define KEYCODE_F8  ( KEYCODE_SUBFAMILY_STDFUNC | 0x51 )
#define KEYCODE_F9  ( KEYCODE_SUBFAMILY_STDFUNC | 0x52 )
#define KEYCODE_F10 ( KEYCODE_SUBFAMILY_STDFUNC | 0x53 )
#define KEYCODE_F11 ( KEYCODE_SUBFAMILY_STDFUNC | 0x54 )
#define KEYCODE_F12 ( KEYCODE_SUBFAMILY_STDFUNC | 0x55 )

// Key-Codes : Extended functions
#define KEYCODE_F13 ( KEYCODE_SUBFAMILY_EXTFUNC | 0x56 )
#define KEYCODE_F14 ( KEYCODE_SUBFAMILY_EXTFUNC | 0x57 )
#define KEYCODE_F15 ( KEYCODE_SUBFAMILY_EXTFUNC | 0x58 )
#define KEYCODE_F16 ( KEYCODE_SUBFAMILY_EXTFUNC | 0x59 )
#define KEYCODE_F17 ( KEYCODE_SUBFAMILY_EXTFUNC | 0x5a )
#define KEYCODE_F18 ( KEYCODE_SUBFAMILY_EXTFUNC | 0x5b )
#define KEYCODE_F19 ( KEYCODE_SUBFAMILY_EXTFUNC | 0x5c )
#define KEYCODE_F20 ( KEYCODE_SUBFAMILY_EXTFUNC | 0x5d )
#define KEYCODE_F21 ( KEYCODE_SUBFAMILY_EXTFUNC | 0x5e )
#define KEYCODE_F22 ( KEYCODE_SUBFAMILY_EXTFUNC | 0x5f )
#define KEYCODE_F23 ( KEYCODE_SUBFAMILY_EXTFUNC | 0x60 )
#define KEYCODE_F24 ( KEYCODE_SUBFAMILY_EXTFUNC | 0x61 )

// Key-Codes : Specials
#define KEYCODE_ESCAPE    ( KEYCODE_FAMILY_SPECIALS | 0x62 )
#define KEYCODE_ENTER     ( KEYCODE_FAMILY_SPECIALS | 0x63 )
#define KEYCODE_TAB       ( KEYCODE_FAMILY_SPECIALS | 0x64 )
#define KEYCODE_BACKSPACE ( KEYCODE_FAMILY_SPECIALS | 0x65 )
#define KEYCODE_DELETE    ( KEYCODE_FAMILY_SPECIALS | 0x66 )
#define KEYCODE_INSERT    ( KEYCODE_FAMILY_SPECIALS | 0x67 )
#define KEYCODE_PRNTSCR   ( KEYCODE_FAMILY_SPECIALS | 0x68 )
#define KEYCODE_PAUSE     ( KEYCODE_FAMILY_SPECIALS | 0x69 )
#define KEYCODE_CLRSCR    ( KEYCODE_FAMILY_SPECIALS | 0x6a )

// Key-Codes : Shift toggles
#define KEYCODE_SHIFT  ( KEYCODE_SUBFAMILY_SHIFTS | 0x6b )
#define KEYCODE_LSHIFT ( KEYCODE_SUBFAMILY_SHIFTS | 0x6c )
#define KEYCODE_RSHIFT ( KEYCODE_SUBFAMILY_SHIFTS | 0x6d )
#define KEYCODE_CTRL   ( KEYCODE_SUBFAMILY_SHIFTS | 0x6e )
#define KEYCODE_LCTRL  ( KEYCODE_SUBFAMILY_SHIFTS | 0x6f )
#define KEYCODE_RCTRL  ( KEYCODE_SUBFAMILY_SHIFTS | 0x70 )
#define KEYCODE_ALT    ( KEYCODE_SUBFAMILY_SHIFTS | 0x71 )
#define KEYCODE_LALT   ( KEYCODE_SUBFAMILY_SHIFTS | 0x72 )
#define KEYCODE_RALT   ( KEYCODE_SUBFAMILY_SHIFTS | 0x73 )

// Key-Codes : Lock toggles
#define KEYCODE_NUMLOCK    ( KEYCODE_SUBFAMILY_LOCKS | 0x74 )
#define KEYCODE_CAPSLOCK   ( KEYCODE_SUBFAMILY_LOCKS | 0x75 )
#define KEYCODE_SCROLLLOCK ( KEYCODE_SUBFAMILY_LOCKS | 0x76 )

// Key-Codes : Mouse
#define KEYCODE_MOUSELEFT      ( KEYCODE_FAMILY_MOUSE | 0x01 )
#define KEYCODE_MOUSERIGHT     ( KEYCODE_FAMILY_MOUSE | 0x02 )
#define KEYCODE_MOUSEMIDDLE    ( KEYCODE_FAMILY_MOUSE | 0x03 )
#define KEYCODE_MOUSEX1        ( KEYCODE_FAMILY_MOUSE | 0x04 )
#define KEYCODE_MOUSEX2        ( KEYCODE_FAMILY_MOUSE | 0x05 )
#define KEYCODE_MOUSEX3        ( KEYCODE_FAMILY_MOUSE | 0x06 )
#define KEYCODE_MOUSEX4        ( KEYCODE_FAMILY_MOUSE | 0x07 )
#define KEYCODE_MOUSEX5        ( KEYCODE_FAMILY_MOUSE | 0x08 )
#define KEYCODE_MOUSEX6        ( KEYCODE_FAMILY_MOUSE | 0x09 )
#define KEYCODE_MOUSEX7        ( KEYCODE_FAMILY_MOUSE | 0x0a )
#define KEYCODE_MOUSEX8        ( KEYCODE_FAMILY_MOUSE | 0x0b )
#define KEYCODE_MOUSEWHEELUP   ( KEYCODE_FAMILY_MOUSE | 0xf0 )
#define KEYCODE_MOUSEWHEELDOWN ( KEYCODE_FAMILY_MOUSE | 0xf1 )

/////////////////////////////////////////////////////////////////////////////////
// PixelFormat definitions
enum PixelFormat {
    // Suffixes = typing
    // <nothing> = untyped
    // F = Float
    // UN = Unsigned int, normalized
    // UI = Unsigned int
    // SN = Signed int, normalized
    // SI = Signed int
    // UN_SRGB = Same as UN, with sRGB format
    // XRBIAS = 2.8 fixed-point biased format
    
    PIXEL_FMT_UNKNOWN = 0,

        // 128-bits formats
    PIXEL_FMT_RGBA32, PIXEL_FMT_RGBA32F, PIXEL_FMT_RGBA32UI, PIXEL_FMT_RGBA32SI,

        // 96-bits formats
    PIXEL_FMT_RGB32, PIXEL_FMT_RGB32F, PIXEL_FMT_RGB32UI, PIXEL_FMT_RGB32SI,

        // 64-bits formats
    PIXEL_FMT_RGBA16, PIXEL_FMT_RGBA16F, PIXEL_FMT_RGBA16UN, PIXEL_FMT_RGBA16UI, PIXEL_FMT_RGBA16SN, PIXEL_FMT_RGBA16SI,
    PIXEL_FMT_RG32,   PIXEL_FMT_RG32F,   PIXEL_FMT_RG32UI,   PIXEL_FMT_RG32SI,

        // 32-bits formats
    PIXEL_FMT_RGBA8,   PIXEL_FMT_RGBA8UN,   PIXEL_FMT_RGBA8UN_SRGB, PIXEL_FMT_RGBA8UI, PIXEL_FMT_RGBA8SN, PIXEL_FMT_RGBA8SI,
    PIXEL_FMT_BGRA8,   PIXEL_FMT_BGRA8UN,   PIXEL_FMT_BGRA8UN_SRGB,
    PIXEL_FMT_BGRX8,   PIXEL_FMT_BGRX8UN,   PIXEL_FMT_BGRX8UN_SRGB,
    PIXEL_FMT_RGB10A2, PIXEL_FMT_RGB10A2UN, PIXEL_FMT_RGB10A2UI,
    PIXEL_FMT_RG16,    PIXEL_FMT_RG16F,     PIXEL_FMT_RG16UN,       PIXEL_FMT_RG16UI,  PIXEL_FMT_RG16SN,  PIXEL_FMT_RG16SI,
    PIXEL_FMT_R32,     PIXEL_FMT_R32F,      PIXEL_FMT_R32UI,        PIXEL_FMT_R32SI,

        // 16-bits formats
    PIXEL_FMT_BGR5A1UN,
    PIXEL_FMT_B5G6R5UN,
    PIXEL_FMT_RG8, PIXEL_FMT_RG8UN, PIXEL_FMT_RG8UI, PIXEL_FMT_RG8SN, PIXEL_FMT_RG8SI,
    PIXEL_FMT_R16, PIXEL_FMT_R16F,  PIXEL_FMT_R16UN, PIXEL_FMT_R16UI, PIXEL_FMT_R16SN, PIXEL_FMT_R16SI,

        // 8-bits formats
    PIXEL_FMT_R8, PIXEL_FMT_R8UN, PIXEL_FMT_R8UI, PIXEL_FMT_R8SN, PIXEL_FMT_R8SI,
    PIXEL_FMT_A8UN,

        // 1-bit format
    PIXEL_FMT_R1UN,

        // Depth-Stencil formats
    PIXEL_FMT_D32F_S8X24UI,
    PIXEL_FMT_D24UN_S8UI,
    PIXEL_FMT_D32F,
    PIXEL_FMT_D16UN,

        // Misc formats
    PIXEL_FMT_R32G8X24, PIXEL_FMT_R32F_X8X24, PIXEL_FMT_X32_G8X24UI,
    PIXEL_FMT_R24G8,    PIXEL_FMT_R24UN_X8,   PIXEL_FMT_X24_G8UI,

    PIXEL_FMT_RG8UN_BG8UN,
    PIXEL_FMT_GR8UN_GB8UN,

    PIXEL_FMT_RGB10XRBIAS_A2UN,
    PIXEL_FMT_RG11B10F,         // 3 floats with 5-bits exponents and 6/5-bits mantissas
    PIXEL_FMT_RGB9E5_SHAREDEXP, // 3 floats with three 9-bits mantissas sharing one 5-bits exponent

        // Palettized formats
    PIXEL_FMT_P8, PIXEL_FMT_AP8,
    PIXEL_FMT_BGRA4UN,

        // Block-Compressed formats
    PIXEL_FMT_BC1,  PIXEL_FMT_BC1UN,     PIXEL_FMT_BC1UN_SRGB,
    PIXEL_FMT_BC2,  PIXEL_FMT_BC2UN,     PIXEL_FMT_BC2UN_SRGB,
    PIXEL_FMT_BC3,  PIXEL_FMT_BC3UN,     PIXEL_FMT_BC3UN_SRGB,
    PIXEL_FMT_BC4,  PIXEL_FMT_BC4UN,     PIXEL_FMT_BC4SN,
    PIXEL_FMT_BC5,  PIXEL_FMT_BC5UN,     PIXEL_FMT_BC5SN,
    PIXEL_FMT_BC6H, PIXEL_FMT_BC6H_UF16, PIXEL_FMT_BC6H_SF16, // HDR colors
    PIXEL_FMT_BC7,  PIXEL_FMT_BC7UN,     PIXEL_FMT_BC7UN_SRGB,

        // Video-specific YUV formats
    PIXEL_FMT_AYUV, PIXEL_FMT_Y410, PIXEL_FMT_Y416, PIXEL_FMT_NV12, PIXEL_FMT_P010, PIXEL_FMT_P016,
    PIXEL_FMT_YUY2, PIXEL_FMT_Y210, PIXEL_FMT_Y216, PIXEL_FMT_NV11, PIXEL_FMT_AI44, PIXEL_FMT_IA44,
    PIXEL_FMT_420_OPAQUE,

    PIXEL_FMT_COUNT
};

UInt PixelFormatBits( PixelFormat iFormat );
inline UInt PixelFormatBytes( PixelFormat iFormat );

UInt PixelFormatChannelCount( PixelFormat iFormat );
UInt PixelFormatChannelBits( PixelFormat iFormat, UInt iChannel );
inline UInt PixelFormatChannelBytes( PixelFormat iFormat, UInt iChannel );

UInt PixelFormatChannelShift( PixelFormat iFormat, UInt iChannel );
inline UInt PixelFormatChannelMask( PixelFormat iFormat, UInt iChannel );

Bool PixelFormatIsMipMapable( PixelFormat iFormat );
Bool PixelFormatIsCompressed( PixelFormat iFormat );

Void PixelFormatExtractChannels( DWord outDstChannels[4], const Byte * pSrcColor, PixelFormat iSrcFormat );
Void PixelFormatMergeChannels( Byte * outDstColor, const DWord pSrcChannels[4], PixelFormat iDstFormat );

Void PixelFormatConvertFrom( Float outDstColor[4], const Byte * pSrcColor, PixelFormat iSrcFormat, UInt iCount = 1 );
Void PixelFormatConvertTo( Byte * outDstColor, const Float pSrcColor[4], PixelFormat iDstFormat, UInt iCount = 1 );

Void PixelFormatLERP( Byte * outDstColor, const Byte ** arrColors, UInt iCount, PixelFormat iInFormat, PixelFormat iOutFormat = PIXEL_FMT_UNKNOWN );

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Platform.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_THIRDPARTY_SYSTEM_PLATFORM_H
