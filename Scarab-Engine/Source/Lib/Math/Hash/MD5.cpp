/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Hash/MD5.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Cryptographic-level MD5 algorithm ...
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
#include "MD5.h"

/////////////////////////////////////////////////////////////////////////////////
// MD5Hasher implementation
const DWord MD5Hasher::sm_arrSinTable[65] = {
    0, // dummy
	// T[i] = (int)( 4294967296 * abs(sin(i)) ), i in radians
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

MD5Hasher::MD5Hasher()
{
    _Initialize();
}
MD5Hasher::~MD5Hasher()
{
}

Void MD5Hasher::Update(const Void * pData, DWord Size)
{
	if (pData == NULL)
		return;
	
	const Byte * pBData = (const Byte*)pData;
	for(DWord i = 0; i < Size; ++i)
	{
		m_arrMsgBuffer[m_iMsgSize++] = pBData[i];
		if (m_iMsgSize == 64)
		{
			_MD5Transform();
			++m_iBlokCount;
			m_iMsgSize = 0;
		}
	}
}
Void MD5Hasher::GetHash(HashMD5 pOut)
{
	if (pOut == NULL)
		return;
	
		// save state (BlokCount is unchanged)
    Byte SvgBuf[128];
	DWord SvgBlok[4];
	DWord SvgSize = m_iMsgSize;
    MemCopy( SvgBuf, m_arrMsgBuffer, 128*sizeof(Byte) );
    MemCopy( SvgBlok, m_arrBlok, 4*sizeof(DWord) );

	_MakePadding();
	if (m_iMsgSize == 128)
	{
		_MD5Transform();
		MemCopy( m_arrMsgBuffer, &(m_arrMsgBuffer[64]), 64*sizeof(Byte) );
		_MD5Transform();
	}
	else /* 64 */
		_MD5Transform();

	*( (DWord*)(&(pOut[0])) ) = m_arrBlok[0];
	*( (DWord*)(&(pOut[4])) ) = m_arrBlok[1];
	*( (DWord*)(&(pOut[8])) ) = m_arrBlok[2];
	*( (DWord*)(&(pOut[12])) ) = m_arrBlok[3];

		// restore state
    MemCopy( m_arrMsgBuffer, SvgBuf, 128*sizeof(Byte) );
    MemCopy( m_arrBlok, SvgBlok, 4*sizeof(DWord) );
	m_iMsgSize = SvgSize;
}

/////////////////////////////////////////////////////////////////////////////////

Void MD5Hasher::_Initialize()
{
	m_iMsgSize = 0;
	m_iBlokCount = 0;
	m_arrBlok[0] = 0x67452301; /* A */
	m_arrBlok[1] = 0xefcdab89; /* B */
	m_arrBlok[2] = 0x98badcfe; /* C */
	m_arrBlok[3] = 0x10325476; /* D */
}
Void MD5Hasher::_MakePadding()
{
	DWord Size = m_iMsgSize;
	m_arrMsgBuffer[Size++] = 0x80;
	while( (Size & 0x3f) != 56 ) // %64
		m_arrMsgBuffer[Size++] = 0x00;
	*( (UInt64*)(&(m_arrMsgBuffer[Size])) ) = ( ((UInt64)((m_iBlokCount << 6) + m_iMsgSize)) << 3 );
	Size += 8;
	m_iMsgSize = Size;
}
Void MD5Hasher::_Operation_F(DWord * pA, DWord B, DWord C, DWord D, DWord X, Byte Rot, Byte I)
{
	DWord Fval = ( (B & C) | ((~B) & D) );
	DWord Tmp = (*pA + Fval + X + sm_arrSinTable[I]);
	Tmp = BitRotL(Tmp,Rot);
	*pA = B + Tmp;
}
Void MD5Hasher::_Operation_G(DWord * pA, DWord B, DWord C, DWord D, DWord X, Byte Rot, Byte I)
{
	DWord Gval = ( (B & D) | (C & (~D)) );
	DWord Tmp = (*pA + Gval + X + sm_arrSinTable[I]);
	Tmp = BitRotL(Tmp,Rot);
	*pA = B + Tmp;
}
Void MD5Hasher::_Operation_H(DWord * pA, DWord B, DWord C, DWord D, DWord X, Byte Rot, Byte I)
{
	DWord Hval = ( B ^ C ^ D );
	DWord Tmp = (*pA + Hval + X + sm_arrSinTable[I]);
	Tmp = BitRotL(Tmp,Rot);
	*pA = B + Tmp;
}
Void MD5Hasher::_Operation_I(DWord * pA, DWord B, DWord C, DWord D, DWord X, Byte Rot, Byte I)
{
	DWord Ival = ( C ^ (B | (~D)) );
	DWord Tmp = (*pA + Ival + X + sm_arrSinTable[I]);
	Tmp = BitRotL(Tmp,Rot);
	*pA = B + Tmp;
}
Void MD5Hasher::_MD5Transform()
{
    Byte j;
	DWord * pMsg = (DWord*)m_arrMsgBuffer;
	DWord X[16];
	for(j = 0; j < 16; ++j)
		X[j] = pMsg[j];

	DWord svgA = m_arrBlok[0];
	DWord svgB = m_arrBlok[1];
	DWord svgC = m_arrBlok[2];
	DWord svgD = m_arrBlok[3];
	j = 1;
	
	/* round 1 */
	_Operation_F(&(m_arrBlok[0]), m_arrBlok[1], m_arrBlok[2], m_arrBlok[3], X[0], 7, j++);
	_Operation_F(&(m_arrBlok[3]), m_arrBlok[0], m_arrBlok[1], m_arrBlok[2], X[1], 12, j++);
	_Operation_F(&(m_arrBlok[2]), m_arrBlok[3], m_arrBlok[0], m_arrBlok[1], X[2], 17, j++);
	_Operation_F(&(m_arrBlok[1]), m_arrBlok[2], m_arrBlok[3], m_arrBlok[0], X[3], 22, j++);

	_Operation_F(&(m_arrBlok[0]), m_arrBlok[1], m_arrBlok[2], m_arrBlok[3], X[4], 7, j++);
	_Operation_F(&(m_arrBlok[3]), m_arrBlok[0], m_arrBlok[1], m_arrBlok[2], X[5], 12, j++);
	_Operation_F(&(m_arrBlok[2]), m_arrBlok[3], m_arrBlok[0], m_arrBlok[1], X[6], 17, j++);
	_Operation_F(&(m_arrBlok[1]), m_arrBlok[2], m_arrBlok[3], m_arrBlok[0], X[7], 22, j++);

	_Operation_F(&(m_arrBlok[0]), m_arrBlok[1], m_arrBlok[2], m_arrBlok[3], X[8], 7, j++);
	_Operation_F(&(m_arrBlok[3]), m_arrBlok[0], m_arrBlok[1], m_arrBlok[2], X[9], 12, j++);
	_Operation_F(&(m_arrBlok[2]), m_arrBlok[3], m_arrBlok[0], m_arrBlok[1], X[10], 17, j++);
	_Operation_F(&(m_arrBlok[1]), m_arrBlok[2], m_arrBlok[3], m_arrBlok[0], X[11], 22, j++);

	_Operation_F(&(m_arrBlok[0]), m_arrBlok[1], m_arrBlok[2], m_arrBlok[3], X[12], 7, j++);
	_Operation_F(&(m_arrBlok[3]), m_arrBlok[0], m_arrBlok[1], m_arrBlok[2], X[13], 12, j++);
	_Operation_F(&(m_arrBlok[2]), m_arrBlok[3], m_arrBlok[0], m_arrBlok[1], X[14], 17, j++);
	_Operation_F(&(m_arrBlok[1]), m_arrBlok[2], m_arrBlok[3], m_arrBlok[0], X[15], 22, j++);

	/* round 2 */
	_Operation_G(&(m_arrBlok[0]), m_arrBlok[1], m_arrBlok[2], m_arrBlok[3], X[1], 5, j++);
	_Operation_G(&(m_arrBlok[3]), m_arrBlok[0], m_arrBlok[1], m_arrBlok[2], X[6], 9, j++);
	_Operation_G(&(m_arrBlok[2]), m_arrBlok[3], m_arrBlok[0], m_arrBlok[1], X[11], 14, j++);
	_Operation_G(&(m_arrBlok[1]), m_arrBlok[2], m_arrBlok[3], m_arrBlok[0], X[0], 20, j++);

	_Operation_G(&(m_arrBlok[0]), m_arrBlok[1], m_arrBlok[2], m_arrBlok[3], X[5], 5, j++);
	_Operation_G(&(m_arrBlok[3]), m_arrBlok[0], m_arrBlok[1], m_arrBlok[2], X[10], 9, j++);
	_Operation_G(&(m_arrBlok[2]), m_arrBlok[3], m_arrBlok[0], m_arrBlok[1], X[15], 14, j++);
	_Operation_G(&(m_arrBlok[1]), m_arrBlok[2], m_arrBlok[3], m_arrBlok[0], X[4], 20, j++);

	_Operation_G(&(m_arrBlok[0]), m_arrBlok[1], m_arrBlok[2], m_arrBlok[3], X[9], 5, j++);
	_Operation_G(&(m_arrBlok[3]), m_arrBlok[0], m_arrBlok[1], m_arrBlok[2], X[14], 9, j++);
	_Operation_G(&(m_arrBlok[2]), m_arrBlok[3], m_arrBlok[0], m_arrBlok[1], X[3], 14, j++);
	_Operation_G(&(m_arrBlok[1]), m_arrBlok[2], m_arrBlok[3], m_arrBlok[0], X[8], 20, j++);

	_Operation_G(&(m_arrBlok[0]), m_arrBlok[1], m_arrBlok[2], m_arrBlok[3], X[13], 5, j++);
	_Operation_G(&(m_arrBlok[3]), m_arrBlok[0], m_arrBlok[1], m_arrBlok[2], X[2], 9, j++);
	_Operation_G(&(m_arrBlok[2]), m_arrBlok[3], m_arrBlok[0], m_arrBlok[1], X[7], 14, j++);
	_Operation_G(&(m_arrBlok[1]), m_arrBlok[2], m_arrBlok[3], m_arrBlok[0], X[12], 20, j++);

	/* round 3 */
	_Operation_H(&(m_arrBlok[0]), m_arrBlok[1], m_arrBlok[2], m_arrBlok[3], X[5], 4, j++);
	_Operation_H(&(m_arrBlok[3]), m_arrBlok[0], m_arrBlok[1], m_arrBlok[2], X[8], 11, j++);
	_Operation_H(&(m_arrBlok[2]), m_arrBlok[3], m_arrBlok[0], m_arrBlok[1], X[11], 16, j++);
	_Operation_H(&(m_arrBlok[1]), m_arrBlok[2], m_arrBlok[3], m_arrBlok[0], X[14], 23, j++);

	_Operation_H(&(m_arrBlok[0]), m_arrBlok[1], m_arrBlok[2], m_arrBlok[3], X[1], 4, j++);
	_Operation_H(&(m_arrBlok[3]), m_arrBlok[0], m_arrBlok[1], m_arrBlok[2], X[4], 11, j++);
	_Operation_H(&(m_arrBlok[2]), m_arrBlok[3], m_arrBlok[0], m_arrBlok[1], X[7], 16, j++);
	_Operation_H(&(m_arrBlok[1]), m_arrBlok[2], m_arrBlok[3], m_arrBlok[0], X[10], 23, j++);

	_Operation_H(&(m_arrBlok[0]), m_arrBlok[1], m_arrBlok[2], m_arrBlok[3], X[13], 4, j++);
	_Operation_H(&(m_arrBlok[3]), m_arrBlok[0], m_arrBlok[1], m_arrBlok[2], X[0], 11, j++);
	_Operation_H(&(m_arrBlok[2]), m_arrBlok[3], m_arrBlok[0], m_arrBlok[1], X[3], 16, j++);
	_Operation_H(&(m_arrBlok[1]), m_arrBlok[2], m_arrBlok[3], m_arrBlok[0], X[6], 23, j++);

	_Operation_H(&(m_arrBlok[0]), m_arrBlok[1], m_arrBlok[2], m_arrBlok[3], X[9], 4, j++);
	_Operation_H(&(m_arrBlok[3]), m_arrBlok[0], m_arrBlok[1], m_arrBlok[2], X[12], 11, j++);
	_Operation_H(&(m_arrBlok[2]), m_arrBlok[3], m_arrBlok[0], m_arrBlok[1], X[15], 16, j++);
	_Operation_H(&(m_arrBlok[1]), m_arrBlok[2], m_arrBlok[3], m_arrBlok[0], X[2], 23, j++);

	/* round 4 */
	_Operation_I(&(m_arrBlok[0]), m_arrBlok[1], m_arrBlok[2], m_arrBlok[3], X[0], 6, j++);
	_Operation_I(&(m_arrBlok[3]), m_arrBlok[0], m_arrBlok[1], m_arrBlok[2], X[7], 10, j++);
	_Operation_I(&(m_arrBlok[2]), m_arrBlok[3], m_arrBlok[0], m_arrBlok[1], X[14], 15, j++);
	_Operation_I(&(m_arrBlok[1]), m_arrBlok[2], m_arrBlok[3], m_arrBlok[0], X[5], 21, j++);

	_Operation_I(&(m_arrBlok[0]), m_arrBlok[1], m_arrBlok[2], m_arrBlok[3], X[12], 6, j++);
	_Operation_I(&(m_arrBlok[3]), m_arrBlok[0], m_arrBlok[1], m_arrBlok[2], X[3], 10, j++);
	_Operation_I(&(m_arrBlok[2]), m_arrBlok[3], m_arrBlok[0], m_arrBlok[1], X[10], 15, j++);
	_Operation_I(&(m_arrBlok[1]), m_arrBlok[2], m_arrBlok[3], m_arrBlok[0], X[1], 21, j++);

	_Operation_I(&(m_arrBlok[0]), m_arrBlok[1], m_arrBlok[2], m_arrBlok[3], X[8], 6, j++);
	_Operation_I(&(m_arrBlok[3]), m_arrBlok[0], m_arrBlok[1], m_arrBlok[2], X[15], 10, j++);
	_Operation_I(&(m_arrBlok[2]), m_arrBlok[3], m_arrBlok[0], m_arrBlok[1], X[6], 15, j++);
	_Operation_I(&(m_arrBlok[1]), m_arrBlok[2], m_arrBlok[3], m_arrBlok[0], X[13], 21, j++);

	_Operation_I(&(m_arrBlok[0]), m_arrBlok[1], m_arrBlok[2], m_arrBlok[3], X[4], 6, j++);
	_Operation_I(&(m_arrBlok[3]), m_arrBlok[0], m_arrBlok[1], m_arrBlok[2], X[11], 10, j++);
	_Operation_I(&(m_arrBlok[2]), m_arrBlok[3], m_arrBlok[0], m_arrBlok[1], X[2], 15, j++);
	_Operation_I(&(m_arrBlok[1]), m_arrBlok[2], m_arrBlok[3], m_arrBlok[0], X[9], 21, j++);

	m_arrBlok[0] += svgA;
	m_arrBlok[1] += svgB;
	m_arrBlok[2] += svgC;
	m_arrBlok[3] += svgD;
}

