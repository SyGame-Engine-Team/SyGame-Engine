/////////////////////////////////////////////////////////////////////////////////
// File : Lib/UID/IDAllocator.h
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
// Header prelude
#ifndef SCARAB_LIB_UID_IDALLOCATOR_H
#define SCARAB_LIB_UID_IDALLOCATOR_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../ThirdParty/System/Platform.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define IDALLOCATOR_MAX_IDS 8192
#define IDALLOCATOR_SIZE    (IDALLOCATOR_MAX_IDS >> 3)

/////////////////////////////////////////////////////////////////////////////////
// The IDAllocator class
class IDAllocator
{
public:
	IDAllocator();
	~IDAllocator();

	UInt Allocate();
	Void Free(UInt iID);

private:
	Bool _IsAllocated(UInt iID) const;
	Void _SetAllocated(UInt iID, Bool bAllocated);

	Byte m_pBitmap[IDALLOCATOR_SIZE];
	UInt m_iNextFreeIndex;
	UInt m_iCount;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "IDAllocator.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_UID_IDALLOCATOR_H