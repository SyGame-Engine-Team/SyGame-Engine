/////////////////////////////////////////////////////////////////////////////////
// File : Lib/UID/IDAllocator.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : An ID allocator as it is allways very convenient to have ;)
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
#include "IDAllocator.h"

/////////////////////////////////////////////////////////////////////////////////
// IDAllocator implementation
IDAllocator::IDAllocator()
{
	MemZero( m_pBitmap, IDALLOCATOR_SIZE );
	m_iNextFreeIndex = 0;
	m_iCount = 0;
}
IDAllocator::~IDAllocator()
{
	// nothing to do
}

UInt IDAllocator::Allocate()
{
    if ( m_iCount >= IDALLOCATOR_MAX_IDS )
        return INVALID_OFFSET;

	UInt iRes = m_iNextFreeIndex;
	_SetAllocated(iRes, true);
	++m_iCount;

	// Woot I've finally placed it !!! The 'do while' loop !!! /Gratz to me ...
	do {
		++m_iNextFreeIndex;
		if ( m_iNextFreeIndex >= IDALLOCATOR_MAX_IDS )
			m_iNextFreeIndex = 0;
	} while( _IsAllocated(m_iNextFreeIndex) );

	return iRes;
}
Void IDAllocator::Free(UInt iID)
{
    if ( iID >= IDALLOCATOR_MAX_IDS )
		return;

	if ( !_IsAllocated(iID) )
		return;
	_SetAllocated(iID, false);
	--m_iCount;

	if (iID < m_iNextFreeIndex)
		m_iNextFreeIndex = iID;
}

Bool IDAllocator::_IsAllocated(UInt iID) const
{
	UInt iByte = (iID >> 3);
	UInt iMask = ( 0x80 >> (iID & 0x00000007) );
	return ( (m_pBitmap[iByte] & iMask) != 0 );
}
Void IDAllocator::_SetAllocated(UInt iID, Bool bAllocated)
{
	UInt iByte = (iID >> 3);
	UInt iMask = ( 0x80 >> (iID & 0x00000007) );
	if (bAllocated)
		m_pBitmap[iByte] |= iMask;
	else
		m_pBitmap[iByte] &= (~iMask);
}
