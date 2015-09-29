/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/System/Hardware/FPU.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : FPU low level abstraction layer
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
#include "FPU.h"

/////////////////////////////////////////////////////////////////////////////////
// FPU implementation
const Float FPU::sm_fHalfF = 0.5f;
const Double FPU::sm_fHalfD = 0.5;

FPU::FPU()
{
    // nothing to do
}
FPU::~FPU()
{
    // nothing to do
}

Void FPU::SetInternalPrecision( FPUPrecisionMode iPrecisionMode )
{
    Word w, *pW = &w;
	__asm { fnstcw pW }	// save control word
	w = (w & 0xfcff) | ( (iPrecisionMode & 0x3) << 8 );
	__asm { fldcw pW }	// load new value
}
Void FPU::SetRoundingMode( FPURoundingMode iRoundingMode )
{
    Word w, *pW = &w;
	__asm { fnstcw pW }	// save control word
	w = (w & 0xf3ff) | ( (iRoundingMode & 0x3) << 10 );
	__asm { fldcw pW }	// load new value
}

FPUClass FPU::Inspect( Float fValue, Bool * outIsNegative )
{
    Word wSign, wClass;
	__asm {
		fld fValue    // Push f on fpu stack
		fxam		  // Examine f
		fstsw ax	  // Look in status word for the result

		shr ax, 8	  // Skip bits 0-7
		mov cx, ax	  // Copy to cx
		and cx, 0x01  // Keep bit 8 only (C0)

		shr ax, 1	  // Skip bit 8
		mov bx, ax	  // Copy to bx
		and bx, 0x01  // Keep bit 9 only (C1)
		mov wSign, bx // Save as sign

		shr ax, 1	  // Skip bit 9
		mov bx, ax	  // Copy to bx
		and bx, 0x01  // Keep bit 10 only (C2)
		shl bx, 1	  // Put bit in place
		or cx, bx     // Add bit to cx

		shr ax, 4	  // Skip bit 10-13
		and ax, 0x01  // Keep bit 14 only (C3)
		shl ax, 2	  // Put bit in place
		or cx, ax     // Add bit to cx
		mov wClass, cx // Save as class
	}
	if ( outIsNegative != NULL )
		*outIsNegative = ( wSign != 0 );
	return wClass;
}
FPUClass FPU::Inspect( Double fValue, Bool * outIsNegative )
{
    Word wSign, wClass;
	__asm {
		fld fValue    // Push f on fpu stack
		fxam		  // Examine f
		fstsw ax	  // Look in status word for the result

		shr ax, 8	  // Skip bits 0-7
		mov cx, ax	  // Copy to cx
		and cx, 0x01  // Keep bit 8 only (C0)

		shr ax, 1	  // Skip bit 8
		mov bx, ax	  // Copy to bx
		and bx, 0x01  // Keep bit 9 only (C1)
		mov wSign, bx // Save as sign

		shr ax, 1	  // Skip bit 9
		mov bx, ax	  // Copy to bx
		and bx, 0x01  // Keep bit 10 only (C2)
		shl bx, 1	  // Put bit in place
		or cx, bx     // Add bit to cx

		shr ax, 4	  // Skip bit 10-13
		and ax, 0x01  // Keep bit 14 only (C3)
		shl ax, 2	  // Put bit in place
		or cx, ax     // Add bit to cx
		mov wClass, cx // Save as class
	}
	if ( outIsNegative != NULL )
		*outIsNegative = ( wSign != 0 );
	return wClass;
}
