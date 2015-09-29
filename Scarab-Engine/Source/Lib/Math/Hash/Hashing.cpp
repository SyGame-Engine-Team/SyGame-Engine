/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Hash/Hashing.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Simple, fast & specialized hashing algorithms ...
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
#include "Hashing.h"

/////////////////////////////////////////////////////////////////////////////////
// Hashing implementation
Hashing::Hashing()
{
    _CRC32_GenerateTable(0xedb88320); // Polynomial
    _CRCGeneric_GenerateTable();
}
Hashing::~Hashing()
{
}

DWord Hashing::MixedBits(DWord dwValue) const
{
    dwValue += ~(dwValue << 15);
    dwValue ^=  (dwValue >> 10);
    dwValue +=  (dwValue << 3);
    dwValue ^=  (dwValue >> 6);
    dwValue += ~(dwValue << 11);
    dwValue ^=  (dwValue >> 16);
    return dwValue;
}
DWord Hashing::ThomasWang(DWord dwValue) const
{
    dwValue = ~dwValue + (dwValue << 15);
    dwValue ^= (dwValue >> 12);
    dwValue += (dwValue << 2);
    dwValue ^= (dwValue >> 4);
    dwValue *= 2057;
    dwValue ^= (dwValue >> 16);
    return dwValue;
}

/////////////////////////////////////////////////////////////////////////////////

DWord Hashing::BKDR( const Byte * str, UInt iLength ) const
{
    DWord dwHash = 0, dwSeed = 131; // 31 131 1313 13131 etc ...
    const Byte * pEnd = str + iLength;
    while(str < pEnd) {
        dwHash = (dwHash * dwSeed) + *str;
        ++str;
    }
    return dwHash;
}
DWord Hashing::BKDR( const GChar * str ) const
{
    DWord dwHash = 0, dwSeed = 131; // 31 131 1313 13131 etc ...
    while(*str != NULLBYTE) {
        dwHash = (dwHash * dwSeed) + (*str & 0xff);
        ++str;
    }
    return dwHash;
}
DWord Hashing::Bernstein( const Byte * str, UInt iLength ) const
{
    DWord dwHash = 5381;
    const Byte * pEnd = str + iLength;
    while(str < pEnd) {
        dwHash = ((dwHash << 5) + dwHash) + *str; // *33
        ++str;
    }
    return dwHash;
}
DWord Hashing::Bernstein( const GChar * str ) const
{
    DWord dwHash = 5381;
    while(*str != NULLBYTE) {
        dwHash = ((dwHash << 5) + dwHash) + (*str & 0xff); // *33
        ++str;
    }
    return dwHash;
}
DWord Hashing::AlphaNum( const Byte * str, UInt iLength ) const
{
    DWord dwHash = 0;
    const Byte * pEnd = str + iLength;
    while(str < pEnd) {
        dwHash = ( (dwHash << 5) + (dwHash * 5) + *str );
        ++str;
    }
    return dwHash;
}
DWord Hashing::AlphaNum( const GChar * str ) const
{
    DWord dwHash = 0;
    while(*str != NULLBYTE) {
        dwHash = ( (dwHash << 5) + (dwHash * 5) + (*str & 0xff) );
        ++str;
    }
    return dwHash;
}
DWord Hashing::SDBM( const Byte * str, UInt iLength ) const
{
    DWord dwHash = 0;
    const Byte * pEnd = str + iLength;
    while(str < pEnd) {
        dwHash = ( (dwHash << 6) + (dwHash << 16) - dwHash + *str );
        ++str;
    }
    return dwHash;
}
DWord Hashing::SDBM( const GChar * str ) const
{
    DWord dwHash = 0;
    while(*str != NULLBYTE) {
        dwHash = ( (dwHash << 6) + (dwHash << 16) - dwHash + (*str & 0xff) );
        ++str;
    }
    return dwHash;
}
DWord Hashing::RS( const Byte * str, UInt iLength ) const
{
    UInt iA = 63689, iB = 378551;
    DWord dwHash = 0;
    const Byte * pEnd = str + iLength;
    while(str < pEnd) {
        dwHash = dwHash * iA + *str;
        iA *= iB;
        ++str;
    }
    return dwHash;
}
DWord Hashing::RS( const GChar * str ) const
{
    UInt iA = 63689, iB = 378551;
    DWord dwHash = 0;
    while(*str != NULLBYTE) {
        dwHash = dwHash * iA + (*str & 0xff);
        iA *= iB;
        ++str;
    }
    return dwHash;
}
DWord Hashing::ELF( const Byte * str, UInt iLength ) const
{
    DWord dwHash = 0, X;
    const Byte * pEnd = str + iLength;
    while(str < pEnd) {
        dwHash = (dwHash << 4) + *str;
        X = dwHash & 0xf0000000;
        if (X != 0) {
            dwHash ^= (X >> 24);
            dwHash &= (~X);
        }
        ++str;
    }
    return dwHash;
}
DWord Hashing::ELF( const GChar * str ) const
{
    DWord dwHash = 0, X;
    while(*str != NULLBYTE) {
        dwHash = (dwHash << 4) + (*str & 0xff);
        X = dwHash & 0xf0000000;
        if (X != 0) {
            dwHash ^= (X >> 24);
            dwHash &= (~X);
        }
        ++str;
    }
    return dwHash;
}
DWord Hashing::FNV( const Byte * str, UInt iLength ) const
{
    DWord dwHash = 2166136261;
    const Byte * pEnd = str + iLength;
    while(str < pEnd) {
        dwHash = (dwHash * 16777619) ^ *str;
        ++str;
    }
    return dwHash;
}
DWord Hashing::FNV( const GChar * str ) const
{
    DWord dwHash = 2166136261;
    while(*str != NULLBYTE) {
        dwHash = (dwHash * 16777619) ^ (*str & 0xff);
        ++str;
    }
    return dwHash;
}
DWord Hashing::JS( const Byte * str, UInt iLength ) const
{
    DWord dwHash = 1315423911;
    const Byte * pEnd = str + iLength;
    while(str < pEnd) {
        dwHash ^= ( (dwHash << 5) + (dwHash >> 2) + *str );
        ++str;
    }
    return dwHash;
}
DWord Hashing::JS( const GChar * str ) const
{
    DWord dwHash = 1315423911;
    while(*str != NULLBYTE) {
        dwHash ^= ( (dwHash << 5) + (dwHash >> 2) + (*str & 0xff) );
        ++str;
    }
    return dwHash;
}
DWord Hashing::AP( const Byte * str, UInt iLength ) const
{
    DWord dwHash = 0;
    Bool bBias = false;
    const Byte * pEnd = str + iLength;
    while(str < pEnd) {
        if (bBias)
            dwHash ^= ~( (dwHash << 11) ^ (dwHash >> 5) ^ *str );
        else
            dwHash ^=  ( (dwHash << 7) ^ (dwHash >> 3) ^ *str );
        bBias = !bBias;
        ++str;
    }
    return dwHash;
}
DWord Hashing::AP( const GChar * str ) const
{
    DWord dwHash = 0;
    Bool bBias = false;
    while(*str != NULLBYTE) {
        if (bBias)
            dwHash ^= ~( (dwHash << 11) ^ (dwHash >> 5) ^ (*str & 0xff) );
        else
            dwHash ^=  ( (dwHash << 7) ^ (dwHash >> 3) ^ (*str & 0xff) );
        bBias = !bBias;
        ++str;
    }
    return dwHash;
}

/////////////////////////////////////////////////////////////////////////////////

DWord Hashing::CRC32( const Byte * str, UInt iLength ) const
{
    DWord dwCRC = iLength; // Init value
    const Byte * pEnd = str + iLength;
    while(str < pEnd) {
        dwCRC = (dwCRC >> 8) ^ m_arrTableCRC32[ (dwCRC & 0xff) ^ *str ];
        ++str;
    }
    return dwCRC;
}
DWord Hashing::CRC32( const GChar * str ) const
{
    DWord dwCRC = StringFn->Length( str ); // Init value
    while(*str != NULLBYTE) {
        dwCRC = (dwCRC >> 8) ^ m_arrTableCRC32[ (dwCRC & 0xff) ^ (*str & 0xff) ];
        ++str;
    }
    return dwCRC;
}
DWord Hashing::CRCGeneric( const Byte * str, UInt iLength ) const
{
    DWord dwCRC = iLength; // Init value
    const Byte * pEnd = str + iLength;
    while(str < pEnd) {
        dwCRC = (dwCRC >> 8) ^ m_arrTableCRCGeneric[ (dwCRC & 0xff) ^ *str ];
        ++str;
    }
    return dwCRC;
}
DWord Hashing::CRCGeneric( const GChar * str ) const
{
    DWord dwCRC = StringFn->Length( str ); // Init value
    while(*str != NULLBYTE) {
        dwCRC = (dwCRC >> 8) ^ m_arrTableCRCGeneric[ (dwCRC & 0xff) ^ (*str & 0xff) ];
        ++str;
    }
    return dwCRC;
}

/////////////////////////////////////////////////////////////////////////////////

DWord Hashing::OneAtATime( const Byte * str, UInt iLength ) const
{
    DWord dwHash = 0;
    const Byte * pEnd = str + iLength;
    while(str < pEnd) {
        dwHash += *str;
        dwHash += (dwHash << 10);
        dwHash ^= (dwHash >>  6);
        ++str;
    }
    dwHash += (dwHash <<  3);
    dwHash ^= (dwHash >> 11);
    dwHash += (dwHash << 15);
    return dwHash;
}
DWord Hashing::OneAtATime( const GChar * str ) const
{
    DWord dwHash = 0;
    while(*str != NULLBYTE) {
        dwHash += (*str & 0xff);
        dwHash += (dwHash << 10);
        dwHash ^= (dwHash >>  6);
        ++str;
    }
    dwHash += (dwHash <<  3);
    dwHash ^= (dwHash >> 11);
    dwHash += (dwHash << 15);
    return dwHash;
}
DWord Hashing::BobJenkins( const Byte * str, UInt iLength ) const
{
    // Internal state
    DWord dwA = 0x9e3779b9; // The magical
    DWord dwB = 0x9e3779b9; // golden ratio
    DWord dwC = 0; // Init value (0 to begin)

    // Main Loop
    while(iLength >= 12) {
        dwA += _Get32Bits(str); str += 4;
        dwB += _Get32Bits(str); str += 4;
        dwC += _Get32Bits(str); str += 4;
        _BobJenkins_Mix(dwA, dwB, dwC);
        iLength -= 12;
    }

    // End Cases
    dwC += iLength;
    switch(iLength) {
        case 11: dwC += ( (DWord)(str[10]) ) << 24;
        case 10: dwC += ( (DWord)(str[9]) ) << 16;
        case  9: dwC += ( (DWord)(str[8]) ) << 8;
        // first byte of C contains length
        case  8: dwB += ( (DWord)(str[7]) ) << 24;
        case  7: dwB += ( (DWord)(str[6]) ) << 16;
        case  6: dwB += ( (DWord)(str[5]) ) << 8;
        case  5: dwB += ( (DWord)(str[4]) );
        case  4: dwA += ( (DWord)(str[3]) ) << 24;
        case  3: dwA += ( (DWord)(str[2]) ) << 16;
        case  2: dwA += ( (DWord)(str[1]) ) << 8;
        case  1: dwA += ( (DWord)(str[0]) );
        case  0: ;// nothing to do
    }
    _BobJenkins_Mix(dwA, dwB, dwC);

    return dwC;
}
DWord Hashing::BobJenkins( const GChar * str ) const
{
    // Internal state
    DWord dwA = 0x9e3779b9; // The magical
    DWord dwB = 0x9e3779b9; // golden ratio
    DWord dwC = 0; // Init value (0 to begin)

    UInt iLength = 0;
    const GChar * pEnd = str;
    while( *pEnd != NULLBYTE && iLength < 12 ) {
        ++pEnd; ++iLength;
    }

    // Main Loop
    while(iLength >= 12) {
        dwA += _Get32Bits(str); str += 4;
        dwB += _Get32Bits(str); str += 4;
        dwC += _Get32Bits(str); str += 4;
        _BobJenkins_Mix(dwA, dwB, dwC);
        iLength -= 12;
        while( *pEnd != NULLBYTE && iLength < 12 ) {
            ++pEnd; ++iLength;
        }
    }

    // End Cases
    dwC += iLength;
    switch(iLength) {
        case 11: dwC += ( (DWord)(str[10] & 0xff) ) << 24;
        case 10: dwC += ( (DWord)(str[9]  & 0xff) ) << 16;
        case  9: dwC += ( (DWord)(str[8]  & 0xff) ) << 8;
        // first byte of C contains length
        case  8: dwB += ( (DWord)(str[7]  & 0xff) ) << 24;
        case  7: dwB += ( (DWord)(str[6]  & 0xff) ) << 16;
        case  6: dwB += ( (DWord)(str[5]  & 0xff) ) << 8;
        case  5: dwB += ( (DWord)(str[4]  & 0xff) );
        case  4: dwA += ( (DWord)(str[3]  & 0xff) ) << 24;
        case  3: dwA += ( (DWord)(str[2]  & 0xff) ) << 16;
        case  2: dwA += ( (DWord)(str[1]  & 0xff) ) << 8;
        case  1: dwA += ( (DWord)(str[0]  & 0xff) );
        case  0: ;// nothing to do
    }
    _BobJenkins_Mix(dwA, dwB, dwC);

    return dwC;
}
DWord Hashing::PaulHsieh( const Byte * str, UInt iLength ) const
{
    DWord dwHash = iLength, dwTemp;
    Int iRem = (iLength & 0x03);
    iLength >>= 2;

    // Main Loop
    while(iLength > 0) {
        dwHash += _Get16Bits(str);                  str += 2;
        dwTemp  = (_Get16Bits(str) << 11) ^ dwHash; str += 2;
        dwHash  = (dwHash << 16) ^ dwTemp;
        dwHash += (dwHash >> 11);
        --iLength;
    }

    // End Cases
    if (iRem == 3) {
        dwHash += _Get16Bits(str); str += 2;
        dwHash ^= (dwHash << 16);
        dwHash ^= (  *str << 18);
        dwHash += (dwHash >> 11);
    } else if (iRem == 2) {
        dwHash += _Get16Bits(str);
        dwHash ^= (dwHash << 11);
        dwHash += (dwHash >> 17);
    } else if (iRem == 1) {
        dwHash += *str;
        dwHash ^= (dwHash << 10);
        dwHash += (dwHash >> 1);
    }

    // Force avalanching for last 127 bits
    dwHash ^= dwHash << 3;
    dwHash += dwHash >> 5;
    dwHash ^= dwHash << 4;
    dwHash += dwHash >> 17;
    dwHash ^= dwHash << 25;
    dwHash += dwHash >> 6;

    return dwHash;
}
DWord Hashing::PaulHsieh( const GChar * str ) const
{
    UInt iLength = StringFn->Length( str );

    DWord dwHash = iLength, dwTemp;
    Int iRem = (iLength & 0x03);
    iLength >>= 2;

    // Main Loop
    while(iLength > 0) {
        dwHash += _Get16Bits(str);                  str += 2;
        dwTemp  = (_Get16Bits(str) << 11) ^ dwHash; str += 2;
        dwHash  = (dwHash << 16) ^ dwTemp;
        dwHash += (dwHash >> 11);
        --iLength;
    }

    // End Cases
    if (iRem == 3) {
        dwHash += _Get16Bits(str); str += 2;
        dwHash ^= (dwHash << 16);
        dwHash ^= ( ( (DWord)(*str & 0xff) ) << 18 );
        dwHash += (dwHash >> 11);
    } else if (iRem == 2) {
        dwHash += _Get16Bits(str);
        dwHash ^= (dwHash << 11);
        dwHash += (dwHash >> 17);
    } else if (iRem == 1) {
        dwHash += (*str & 0xff);
        dwHash ^= (dwHash << 10);
        dwHash += (dwHash >> 1);
    }

    // Force avalanching for last 127 bits
    dwHash ^= dwHash << 3;
    dwHash += dwHash >> 5;
    dwHash ^= dwHash << 4;
    dwHash += dwHash >> 17;
    dwHash ^= dwHash << 25;
    dwHash += dwHash >> 6;

    return dwHash;
}
DWord Hashing::BobJenkinsRot( const Byte * str, UInt iLength ) const
{
    // Internal state
    DWord dwA = 0xdeadbeef + iLength + 0; // Init value (0 to begin)
    DWord dwB = dwA;
    DWord dwC = dwA;

    // Main Loop
    while(iLength > 12) {
        dwA += _Get32Bits(str); str += 4;
        dwB += _Get32Bits(str); str += 4;
        dwC += _Get32Bits(str); str += 4;
        _BobJenkins_MixRot(dwA, dwB, dwC);
        iLength -= 12;
    }

    // End Cases
    switch(iLength) {
        case 12: dwC += ( (DWord)(str[11]) ) << 24;
        case 11: dwC += ( (DWord)(str[10]) ) << 16;
        case 10: dwC += ( (DWord)(str[9]) ) << 8;
        case  9: dwC += ( (DWord)(str[8]) );
        case  8: dwB += ( (DWord)(str[7]) ) << 24;
        case  7: dwB += ( (DWord)(str[6]) ) << 16;
        case  6: dwB += ( (DWord)(str[5]) ) << 8;
        case  5: dwB += ( (DWord)(str[4]) );
        case  4: dwA += ( (DWord)(str[3]) ) << 24;
        case  3: dwA += ( (DWord)(str[2]) ) << 16;
        case  2: dwA += ( (DWord)(str[1]) ) << 8;
        case  1: dwA += ( (DWord)(str[0]) ); break;
        case  0: return dwC;
    }
    _BobJenkins_Final(dwA, dwB, dwC);

    return dwC;
}
DWord Hashing::BobJenkinsRot( const GChar * str ) const
{
    // Internal state
    DWord dwA = 0xdeadbeef + (*str & 0xff) + 0; // Init value (0 to begin)
    DWord dwB = dwA;
    DWord dwC = dwA;

    UInt iLength = 0;
    const GChar * pEnd = str;
    while( *pEnd != NULLBYTE && iLength <= 12 ) {
        ++pEnd; ++iLength;
    }

    // Main Loop
    while(iLength > 12) {
        dwA += _Get32Bits(str); str += 4;
        dwB += _Get32Bits(str); str += 4;
        dwC += _Get32Bits(str); str += 4;
        _BobJenkins_MixRot(dwA, dwB, dwC);
        iLength -= 12;
        while( *pEnd != NULLBYTE && iLength <= 12 ) {
            ++pEnd; ++iLength;
        }
    }

    // End Cases
    switch(iLength) {
        case 12: dwC += ( (DWord)(str[11] & 0xff) ) << 24;
        case 11: dwC += ( (DWord)(str[10] & 0xff) ) << 16;
        case 10: dwC += ( (DWord)(str[9]  & 0xff) ) << 8;
        case  9: dwC += ( (DWord)(str[8]  & 0xff) );
        case  8: dwB += ( (DWord)(str[7]  & 0xff) ) << 24;
        case  7: dwB += ( (DWord)(str[6]  & 0xff) ) << 16;
        case  6: dwB += ( (DWord)(str[5]  & 0xff) ) << 8;
        case  5: dwB += ( (DWord)(str[4]  & 0xff) );
        case  4: dwA += ( (DWord)(str[3]  & 0xff) ) << 24;
        case  3: dwA += ( (DWord)(str[2]  & 0xff) ) << 16;
        case  2: dwA += ( (DWord)(str[1]  & 0xff) ) << 8;
        case  1: dwA += ( (DWord)(str[0]  & 0xff) ); break;
        case  0: return dwC;
    }
    _BobJenkins_Final(dwA, dwB, dwC);

    return dwC;
}

/////////////////////////////////////////////////////////////////////////////////

Void Hashing::_CRC32_GenerateTable(DWord dwPolynomial)
{
    DWord dwCRC;
    for(UInt i = 0; i < 256; ++i) {
        dwCRC = i;
        dwCRC = (dwCRC >> 1) ^ ( (dwCRC & 0x01) ? dwPolynomial : 0 );
        dwCRC = (dwCRC >> 1) ^ ( (dwCRC & 0x01) ? dwPolynomial : 0 );
        dwCRC = (dwCRC >> 1) ^ ( (dwCRC & 0x01) ? dwPolynomial : 0 );
        dwCRC = (dwCRC >> 1) ^ ( (dwCRC & 0x01) ? dwPolynomial : 0 );
        dwCRC = (dwCRC >> 1) ^ ( (dwCRC & 0x01) ? dwPolynomial : 0 );
        dwCRC = (dwCRC >> 1) ^ ( (dwCRC & 0x01) ? dwPolynomial : 0 );
        dwCRC = (dwCRC >> 1) ^ ( (dwCRC & 0x01) ? dwPolynomial : 0 );
        dwCRC = (dwCRC >> 1) ^ ( (dwCRC & 0x01) ? dwPolynomial : 0 );
        m_arrTableCRC32[i] = dwCRC;
    }
}
Void Hashing::_CRCGeneric_GenerateTable()
{
    Byte X;
    UInt i,j;
    for(i = 0; i < 256; ++i) {
        X = (Byte)i;
        for(j=0; j<5; ++j) {
            X += 1; X += (X << 1); X ^= (X >> 1);
        }
        m_arrTableCRCGeneric[i] = X;
        for(j=0; j<5; ++j) {
            X += 2; X += (X << 1); X ^= (X >> 1);
        }
        m_arrTableCRCGeneric[i] ^= ( ((DWord)X) << 8 );
        for(j=0; j<5; ++j) {
            X += 3; X += (X << 1); X ^= (X >> 1);
        }
        m_arrTableCRCGeneric[i] ^= ( ((DWord)X) << 16 );
        for(j=0; j<5; ++j) {
            X += 4; X += (X << 1); X ^= (X >> 1);
        }
        m_arrTableCRCGeneric[i] ^= ( ((DWord)X) << 24 );
    }
}
