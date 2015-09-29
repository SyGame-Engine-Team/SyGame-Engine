/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Hash/SHA1.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Cryptographic-level SHA1 algorithm ...
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
#include "SHA1.h"

/////////////////////////////////////////////////////////////////////////////////
// SHA1Hasher implementation
SHA1Hasher::SHA1Hasher()
{
    _Initialize();
}
SHA1Hasher::~SHA1Hasher()
{
}

Void SHA1Hasher::Update(const Void * pData, DWord Size)
{
	if (pData == NULL)
		return;
	
	const Byte * pBData = (const Byte*)pData;
	for(DWord i = 0; i < Size; ++i)
	{
		m_arrMsgBuffer[m_iMsgSize++] = pBData[i];
		if (m_iMsgSize == 64)
		{
			_SHA1Transform();
			++m_iBlokCount;
			m_iMsgSize = 0;
		}
	}
}
Void SHA1Hasher::GetHash(HashSHA1 pOut)
{
	if (pOut == NULL)
		return;
	
		// save state (BlokCount is unchanged)
    Byte SvgBuf[128];
	DWord SvgBlok[5];
	DWord SvgSize = m_iMsgSize;
    MemCopy( SvgBuf, m_arrMsgBuffer, 128*sizeof(Byte) );
    MemCopy( SvgBlok, m_arrBlok, 5*sizeof(DWord) );

	_MakePadding();
	if (m_iMsgSize == 128)
	{
		_SHA1Transform();
		MemCopy( m_arrMsgBuffer, &(m_arrMsgBuffer[64]), 64*sizeof(Byte) );
		_SHA1Transform();
	}
	else /* 64 */
		_SHA1Transform();

	Byte k = 0;
	for(Byte i = 0; i < 5; ++i)
	{
		pOut[k++] = (Byte)(m_arrBlok[i] >> 24);
		pOut[k++] = (Byte)(m_arrBlok[i] >> 16);
		pOut[k++] = (Byte)(m_arrBlok[i] >> 8);
		pOut[k++] = (Byte)(m_arrBlok[i]);
	}

		// restore state
    MemCopy( m_arrMsgBuffer, SvgBuf, 128*sizeof(Byte) );
    MemCopy( m_arrBlok, SvgBlok, 5*sizeof(DWord) );
	m_iMsgSize = SvgSize;
}

/////////////////////////////////////////////////////////////////////////////////

Void SHA1Hasher::_Initialize()
{
	/* init */
	m_iMsgSize = 0;
	m_iBlokCount = 0;
	m_arrBlok[0] = 0x67452301;
	m_arrBlok[1] = 0xefcdab89;
	m_arrBlok[2] = 0x98badcfe;
	m_arrBlok[3] = 0x10325476;
	m_arrBlok[4] = 0xc3d2e1f0;
}
Void SHA1Hasher::_MakePadding()
{
	DWord Size = m_iMsgSize;
	m_arrMsgBuffer[Size++] = 0x80;
	while( (Size & 0x3f) != 56 )  // %64
		m_arrMsgBuffer[Size++] = 0x00;
	UInt64 BitSize = ( ((UInt64)((m_iBlokCount << 6) + m_iMsgSize)) << 3 );
	for(Byte i = 0; i < 8; ++i)
		m_arrMsgBuffer[Size++] = (Byte)(BitSize >> (56 - 8*i));
	m_iMsgSize = Size;
}
DWord SHA1Hasher::_Operation_F(Byte t, DWord B, DWord C, DWord D)
{
	if (t <= 19)
		return ( (B & C) | ((~B) & D) );
	else if (t <= 39)
		return (B ^ C ^ D);
	else if (t <= 59)
		return ( (B & C) | (B & D) | (C & D) );
	else if (t <= 79)
		return (B ^ C ^ D);
	else
		return 0; /* never happen */
}
DWord SHA1Hasher::_Operation_K(Byte t)
{
	if (t <= 19)
		return 0x5a827999;
	else if (t <= 39)
		return 0x6ed9eba1;
	else if (t <= 59)
		return 0x8f1bbcdc;
	else if (t <= 79)
		return 0xca62c1d6;
	else
		return 0; /* never happen */
}
Void SHA1Hasher::_SHA1Transform()
{
	Byte t;
	DWord SeqW[80];
	DWord ABCDE[5], Temp;

	for(t = 0; t < 16; ++t)
	{
		SeqW[t] = ((DWord)m_arrMsgBuffer[(t << 2)]) << 24;
		SeqW[t] |= ((DWord)m_arrMsgBuffer[(t << 2) + 1]) << 16;
		SeqW[t] |= ((DWord)m_arrMsgBuffer[(t << 2) + 2]) << 8;
		SeqW[t] |= ((DWord)m_arrMsgBuffer[(t << 2) + 3]);
	}
	for(t = 16; t < 80; ++t)
		SeqW[t] = BitRotL( (SeqW[t-3] ^ SeqW[t-8] ^ SeqW[t-14] ^ SeqW[t-16]), 1 );

	ABCDE[0] = m_arrBlok[0];
	ABCDE[1] = m_arrBlok[1];
	ABCDE[2] = m_arrBlok[2];
	ABCDE[3] = m_arrBlok[3];
	ABCDE[4] = m_arrBlok[4];
	for(t = 0; t < 80; ++t)
	{
		Temp = ( BitRotL(ABCDE[0], 5) + _Operation_F(t,ABCDE[1],ABCDE[2],ABCDE[3]) + ABCDE[4]
					+ SeqW[t] + _Operation_K(t) );
		ABCDE[4] = ABCDE[3];
		ABCDE[3] = ABCDE[2];
		ABCDE[2] = BitRotL(ABCDE[1], 30);
		ABCDE[1] = ABCDE[0];
		ABCDE[0] = Temp;
	}
	m_arrBlok[0] += ABCDE[0];
	m_arrBlok[1] += ABCDE[1];
	m_arrBlok[2] += ABCDE[2];
	m_arrBlok[3] += ABCDE[3];
	m_arrBlok[4] += ABCDE[4];
}

