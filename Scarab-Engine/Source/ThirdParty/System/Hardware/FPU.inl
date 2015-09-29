/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/System/Hardware/FPU.inl
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
// FPU implementation
inline FPU * FPU::GetInstancePtr() {
    static FPU s_Instance;
    return &s_Instance;
}

inline Int _fastcall FPU::Floor( Float fValue ) {
    Int iRes;
	__asm { fld		fValue
			fsub	sm_fHalfF
			fistp	iRes }
	return iRes;
}
inline Int _fastcall FPU::Floor( Double fValue ) {
    Int iRes;
	__asm { fld		fValue
			fsub	sm_fHalfD
			fistp	iRes }
	return iRes;
}

inline Int _fastcall FPU::Ceil( Float fValue ) {
    Int iRes;
	__asm { fld		fValue
			fadd	sm_fHalfF
			fistp	iRes }
	return iRes;
}
inline Int _fastcall FPU::Ceil( Double fValue ) {
    Int iRes;
	__asm { fld		fValue
			fadd	sm_fHalfD
			fistp	iRes }
	return iRes;
}

inline Int _fastcall FPU::Round( Float fValue ) {
    Int iRes;
	__asm { fld		fValue
			fistp	iRes }
	return iRes;
}
inline Int _fastcall FPU::Round( Double fValue ) {
    Int iRes;
	__asm { fld		fValue
			fistp	iRes }
	return iRes;
}

inline Float _fastcall FPU::Abs( Float fValue ) {
    __asm { fld fValue
            fabs
            fstp fValue }
    return fValue;
}
inline Double _fastcall FPU::Abs( Double fValue ) {
    __asm { fld fValue
            fabs
            fstp fValue }
    return fValue;
}

inline Float _fastcall FPU::Mod( Float fValue, Float fMod ) {
    __asm {   fld     fMod         // st(0) = fMod
              fld     fValue       // st(1) = fMod, st(0) = fValue
_Loop_FPREM1: fprem1               // Use IEEE-754 version of partial remainder
              fstsw   ax           // store status word in ax
              test    ah, 100b     // test C2
              jnz     _Loop_FPREM1 // repeat if set.
              fstp    fValue       // get result
              fstp    st(0) }      // st empty
    return fValue;
}
inline Double _fastcall FPU::Mod( Double fValue, Double fMod ) {
    __asm {   fld     fMod         // st(0) = fMod
              fld     fValue       // st(1) = fMod, st(0) = fValue
_Loop_FPREM1: fprem1               // Use IEEE-754 version of partial remainder
              fstsw   ax           // store status word in ax
              test    ah, 100b     // test C2
              jnz     _Loop_FPREM1 // repeat if set.
              fstp    fValue       // get result
              fstp    st(0) }      // st empty
    return fValue;
}

inline Float _fastcall FPU::Sqrt( Float fValue ) {
    __asm { fld fValue	  // st(0) = fValue
			fsqrt		  // st(0) = sqrt(fValue)
			fstp fValue } // fValue = sqrt(fValue), st empty
	return fValue;
}
inline Double _fastcall FPU::Sqrt( Double fValue ) {
    __asm { fld fValue	  // st(0) = fValue
			fsqrt		  // st(0) = sqrt(fValue)
			fstp fValue } // fValue = sqrt(fValue), st empty
	return fValue;
}

inline Float _fastcall FPU::Ln( Float fValue ) {
    __asm { fldln2	      // st(0) = ln(2)
			fld fValue	  // st(1) = ln(2), st(0) = fValue
			fyl2x         // st(0) = ln(2) * log2(fValue) = ln(fValue)
			fstp fValue } // fValue = ln(fValue), st empty
	return fValue;
}
inline Double _fastcall FPU::Ln( Double fValue ) {
    __asm { fldln2        // st(0) = ln(2)
			fld fValue    // st(1) = ln(2), st(0) = fValue
			fyl2x		  // st(0) = ln(2) * log2(fValue) = ln(fValue)
			fstp fValue } // fValue = ln(fValue), st empty
	return fValue;
}

inline Float _fastcall FPU::Log2( Float fValue ) {
    __asm { fld1          // st(0) = 1.0
			fld fValue    // st(1) = 1.0, st(0) = fValue
			fyl2x         // st(0) = 1.0 * log2(fValue)
			fstp fValue } // fValue = log2(fValue), st empty
	return fValue;
}
inline Double _fastcall FPU::Log2( Double fValue ) {
    __asm { fld1          // st(0) = 1.0
			fld fValue    // st(1) = 1.0, st(0) = fValue
			fyl2x         // st(0) = 1.0 * log2(fValue)
			fstp fValue } // fValue = log2(fValue), st empty
	return fValue;
}

inline Float _fastcall FPU::Log10( Float fValue ) {
    __asm { fldlg2        // st(0) = log10(2)
			fld fValue    // st(1) = log10(2), st(0) = fValue
			fyl2x         // st(0) = log10(2) * log2(fValue) = log10(fValue)
			fstp fValue } // fValue = log10(fValue), st empty
	return fValue;
}
inline Double _fastcall FPU::Log10( Double fValue ) {
    __asm { fldlg2        // st(0) = log10(2)
			fld fValue    // st(1) = log10(2), st(0) = fValue
			fyl2x         // st(0) = log10(2) * log2(fValue) = log10(fValue)
			fstp fValue } // fValue = log10(fValue), st empty
	return fValue;
}

inline Float _fastcall FPU::LogN( Float fBase, Float fValue ) {
    __asm { fld1                // st(0) = 1.0
			fld fBase           // st(1) = 1.0, st(0) = fBase
			fyl2x               // st(0) = 1.0 * log2(fBase)
            fld1                // st(1) = log2(fBase), st(0) = 1.0
            fdivrp st(1), st(0) // st(0) = 1.0 / log2(fBase) = logn(2)
			fld fValue          // st(1) = logn(2), st(0) = fValue
			fyl2x               // st(0) = logn(2) * log2(fValue) = logn(fValue)
			fstp fValue }       // fValue = logn(fValue), st empty
	return fValue;
}
inline Double _fastcall FPU::LogN( Double fBase, Double fValue ) {
    __asm { fld1                // st(0) = 1.0
			fld fBase           // st(1) = 1.0, st(0) = fBase
			fyl2x               // st(0) = 1.0 * log2(fBase)
            fld1                // st(1) = log2(fBase), st(0) = 1.0
            fdivrp st(1), st(0) // st(0) = 1.0 / log2(fBase) = logn(2)
			fld fValue          // st(1) = logn(2), st(0) = fValue
			fyl2x               // st(0) = logn(2) * log2(fValue) = logn(fValue)
			fstp fValue }       // fValue = logn(fValue), st empty
	return fValue;
}

inline Float _fastcall FPU::Exp( Float fValue ) {
    __asm { fldl2e              // st(0) = log2(e)
            fld fValue          // st(1) = log2(e), st(0) = fValue
            fmulp st(1), st(0)  // st(0) = fValue * log2(e)
            fld st(0)           // st(1) = fValue * log2(e), st(0) = fValue * log2(e)
			frndint             // st(1) = fValue * log2(e), st(0) = round0(fValue * log2(e)) = ipart
			fsub st(1), st(0)   // st(1) = (fValue * log2(e) - ipart) = fpart, st(0) = ipart
			fxch                // st(1) = ipart, st(0) = fpart
			f2xm1               // st(1) = ipart, st(0) = 2 ^ fpart - 1
			fld1                // st(2) = ipart, st(1) = 2 ^ fpart - 1, st(0) = 1.0
			faddp  st(1), st(0) // st(1) = ipart, st(0) = 2 ^ fpart
			fscale              // st(1) = ipart, st(0) = 2 ^ fpart * 2 ^ ipart = 2 ^ (fValue * log2(e)) = e ^ fValue
			fstp fValue         // fValue = e ^ fValue, st(0) = ipart
			fstp st(0) }        // st empty
	return fValue;
}
inline Double _fastcall FPU::Exp( Double fValue ) {
    __asm { fldl2e              // st(0) = log2(e)
            fld fValue          // st(1) = log2(e), st(0) = fValue
            fmulp st(1), st(0)  // st(0) = fValue * log2(e)
            fld st(0)           // st(1) = fValue * log2(e), st(0) = fValue * log2(e)
			frndint             // st(1) = fValue * log2(e), st(0) = round0(fValue * log2(e)) = ipart
			fsub st(1), st(0)   // st(1) = (fValue * log2(e) - ipart) = fpart, st(0) = ipart
			fxch                // st(1) = ipart, st(0) = fpart
			f2xm1               // st(1) = ipart, st(0) = 2 ^ fpart - 1
			fld1                // st(2) = ipart, st(1) = 2 ^ fpart - 1, st(0) = 1.0
			faddp  st(1), st(0) // st(1) = ipart, st(0) = 2 ^ fpart
			fscale              // st(1) = ipart, st(0) = 2 ^ fpart * 2 ^ ipart = 2 ^ (fValue * log2(e)) = e ^ fValue
			fstp fValue         // fValue = e ^ fValue, st(0) = ipart
			fstp st(0) }        // st empty
	return fValue;
}

inline Float _fastcall FPU::Exp2( Float fValue ) {
    __asm { fld fValue          // st(0) = fValue
			fld st(0)           // st(1) = fValue, st(0) = fValue
			frndint             // st(1) = fValue, st(0) = round0(fValue) = ipart
			fsub st(1), st(0)   // st(1) = (fValue - ipart) = fpart, st(0) = ipart
			fxch                // st(1) = ipart, st(0) = fpart
			f2xm1               // st(1) = ipart, st(0) = 2 ^ fpart - 1
			fld1                // st(2) = ipart, st(1) = 2 ^ fpart - 1, st(0) = 1.0
			faddp st(1), st(0)  // st(1) = ipart, st(0) = 2 ^ fpart
			fscale              // st(1) = ipart, st(0) = 2 ^ fpart * 2 ^ ipart = 2 ^ fValue
			fstp fValue         // fValue = 2 ^ fValue, st(0) = ipart
			fstp st(0) }        // st empty
	return fValue;
}
inline Double _fastcall FPU::Exp2( Double fValue ) {
    __asm { fld fValue          // st(0) = fValue
			fld st(0)           // st(1) = fValue, st(0) = fValue
			frndint             // st(1) = fValue, st(0) = round0(fValue) = ipart
			fsub st(1), st(0)   // st(1) = (fValue - ipart) = fpart, st(0) = ipart
			fxch                // st(1) = ipart, st(0) = fpart
			f2xm1               // st(1) = ipart, st(0) = 2 ^ fpart - 1
			fld1                // st(2) = ipart, st(1) = 2 ^ fpart - 1, st(0) = 1.0
			faddp st(1), st(0)  // st(1) = ipart, st(0) = 2 ^ fpart
			fscale              // st(1) = ipart, st(0) = 2 ^ fpart * 2 ^ ipart = 2 ^ fValue
			fstp fValue         // fValue = 2 ^ fValue, st(0) = ipart
			fstp st(0) }        // st empty
	return fValue;
}

inline Float _fastcall FPU::Exp10( Float fValue ) {
    __asm { fldl2t              // st(0) = log2(10)
			fld fValue          // st(1) = log2(10), st(0) = fValue
            fmulp st(1), st(0)  // st(0) = fValue * log2(10)
			fld st(0)           // st(1) = fValue * log2(10), st(0) = fValue * log2(10)
			frndint             // st(1) = fValue * log2(10), st(0) = round0(fValue * log2(10)) = ipart
			fsub st(1), st(0)   // st(1) = (fValue * log2(10) - ipart) = fpart, st(0) = ipart
			fxch                // st(1) = ipart, st(0) = fpart
			f2xm1               // st(1) = ipart, st(0) = 2 ^ fpart - 1
			fld1                // st(2) = ipart, st(1) = 2 ^ fpart - 1, st(0) = 1.0
			faddp st(1), st(0)  // st(1) = ipart, st(0) = 2 ^ fpart
			fscale              // st(1) = ipart, st(0) = 2 ^ fpart * 2 ^ ipart = 2 ^ (fValue * log2(10)) = 10 ^ fValue
			fstp fValue         // fValue = 10 ^ fValue, st(0) = ipart
			fstp st(0) }        // st empty
	return fValue;
}
inline Double _fastcall FPU::Exp10( Double fValue ) {
    __asm { fldl2t              // st(0) = log2(10)
			fld fValue          // st(1) = log2(10), st(0) = fValue
            fmulp st(1), st(0)  // st(0) = fValue * log2(10)
			fld st(0)           // st(1) = fValue * log2(10), st(0) = fValue * log2(10)
			frndint             // st(1) = fValue * log2(10), st(0) = round0(fValue * log2(10)) = ipart
			fsub st(1), st(0)   // st(1) = (fValue * log2(10) - ipart) = fpart, st(0) = ipart
			fxch                // st(1) = ipart, st(0) = fpart
			f2xm1               // st(1) = ipart, st(0) = 2 ^ fpart - 1
			fld1                // st(2) = ipart, st(1) = 2 ^ fpart - 1, st(0) = 1.0
			faddp st(1), st(0)  // st(1) = ipart, st(0) = 2 ^ fpart
			fscale              // st(1) = ipart, st(0) = 2 ^ fpart * 2 ^ ipart = 2 ^ (fValue * log2(10)) = 10 ^ fValue
			fstp fValue         // fValue = 10 ^ fValue, st(0) = ipart
			fstp st(0) }        // st empty
	return fValue;
}

inline Float _fastcall FPU::ExpN( Float fBase, Float fValue ) {
    __asm { fld1                // st(0) = 1.0
			fld fBase           // st(1) = 1.0, st(0) = fBase
			fyl2x               // st(0) = 1.0 * log2(fBase)
			fld fValue          // st(1) = log2(fBase), st(0) = fValue
            fmulp st(1), st(0)  // st(0) = fValue * log2(fBase)
			fld st(0)           // st(1) = fValue * log2(fBase), st(0) = fValue * log2(fBase)
			frndint             // st(1) = fValue * log2(fBase), st(0) = round0(fValue * log2(fBase)) = ipart
			fsub st(1), st(0)   // st(1) = (fValue * log2(fBase) - ipart) = fpart, st(0) = ipart
			fxch                // st(1) = ipart, st(0) = fpart
			f2xm1               // st(1) = ipart, st(0) = 2 ^ fpart - 1
			fld1                // st(2) = ipart, st(1) = 2 ^ fpart - 1, st(0) = 1.0
			faddp st(1), st(0)  // st(1) = ipart, st(0) = 2 ^ fpart
			fscale              // st(1) = ipart, st(0) = 2 ^ fpart * 2 ^ ipart = 2 ^ (fValue * log2(fBase)) = fBase ^ fValue
			fstp fValue         // fValue = fBase ^ fValue, st(0) = ipart
			fstp st(0) }        // st empty
	return fValue;
}
inline Double _fastcall FPU::ExpN( Double fBase, Double fValue ) {
    __asm { fld1                // st(0) = 1.0
			fld fBase           // st(1) = 1.0, st(0) = fBase
			fyl2x               // st(0) = 1.0 * log2(fBase)
			fld fValue          // st(1) = log2(fBase), st(0) = fValue
            fmulp st(1), st(0)  // st(0) = fValue * log2(fBase)
			fld st(0)           // st(1) = fValue * log2(fBase), st(0) = fValue * log2(fBase)
			frndint             // st(1) = fValue * log2(fBase), st(0) = round0(fValue * log2(fBase)) = ipart
			fsub st(1), st(0)   // st(1) = (fValue * log2(fBase) - ipart) = fpart, st(0) = ipart
			fxch                // st(1) = ipart, st(0) = fpart
			f2xm1               // st(1) = ipart, st(0) = 2 ^ fpart - 1
			fld1                // st(2) = ipart, st(1) = 2 ^ fpart - 1, st(0) = 1.0
			faddp st(1), st(0)  // st(1) = ipart, st(0) = 2 ^ fpart
			fscale              // st(1) = ipart, st(0) = 2 ^ fpart * 2 ^ ipart = 2 ^ (fValue * log2(fBase)) = fBase ^ fValue
			fstp fValue         // fValue = fBase ^ fValue, st(0) = ipart
			fstp st(0) }        // st empty
	return fValue;
}

inline Float _fastcall FPU::Power2f( Int iExponent ) {
    Float fRes;
	__asm { fild iExponent	// st(0) = Exp
			fld1			// st(1) = Exp, st(0) = 1.0
			fscale			// st(1) = Exp, st(0) = 1.0 * 2 ^ Exp = 2 ^ Exp
			fstp fRes		// fRes = 2 ^ Exp, st(0) = Exp
			fstp st(0) }	// st empty
	return fRes;
}
inline Double _fastcall FPU::Power2d( Int iExponent ) {
    Double fRes;
	__asm { fild iExponent	// st(0) = Exp
			fld1			// st(1) = Exp, st(0) = 1.0
			fscale			// st(1) = Exp, st(0) = 1.0 * 2 ^ Exp = 2 ^ Exp
			fstp fRes		// fRes = 2 ^ Exp, st(0) = Exp
			fstp st(0) }	// st empty
	return fRes;
}

inline Float _fastcall FPU::Power10f( Int iExponent ) {
    Float fRes;
    __asm { fldl2t				// st(0) = log2(10)
			fild iExponent      // st(1) = log2(10), st(0) = Exp
            fmulp st(1), st(0)  // st(0) = Exp * log2(10)
			fld st(0)           // st(1) = Exp * log2(10), st(0) = Exp * log2(10)
			frndint				// st(1) = Exp * log2(10), st(0) = round0(Exp * log2(10)) = ipart
			fsub st(1), st(0)	// st(1) = (Exp * log2(10) - ipart) = fpart, st(0) = ipart
			fxch				// st(1) = ipart, st(0) = fpart
			f2xm1				// st(1) = ipart, st(0) = 2 ^ fpart - 1
			fld1				// st(2) = ipart, st(1) = 2 ^ fpart - 1, st(0) = 1.0
			faddp st(1), st(0)  // st(1) = ipart, st(0) = 2 ^ fpart
			fscale				// st(1) = ipart, st(0) = 2 ^ fpart * 2 ^ ipart = 2 ^ (Exp * log2(10)) = 10 ^ Exp
			fstp fRes           // fRes = 10 ^ Exp, st(0) = ipart
			fstp st(0) }		// st empty
    return fRes;
}
inline Double _fastcall FPU::Power10d( Int iExponent ) {
    Double fRes;
    __asm { fldl2t				// st(0) = log2(10)
			fild iExponent      // st(1) = log2(10), st(0) = Exp
            fmulp st(1), st(0)  // st(0) = Exp * log2(10)
			fld st(0)           // st(1) = Exp * log2(10), st(0) = Exp * log2(10)
			frndint				// st(1) = Exp * log2(10), st(0) = round0(Exp * log2(10)) = ipart
			fsub st(1), st(0)	// st(1) = (Exp * log2(10) - ipart) = fpart, st(0) = ipart
			fxch				// st(1) = ipart, st(0) = fpart
			f2xm1				// st(1) = ipart, st(0) = 2 ^ fpart - 1
			fld1				// st(2) = ipart, st(1) = 2 ^ fpart - 1, st(0) = 1.0
			faddp st(1), st(0)  // st(1) = ipart, st(0) = 2 ^ fpart
			fscale				// st(1) = ipart, st(0) = 2 ^ fpart * 2 ^ ipart = 2 ^ (Exp * log2(10)) = 10 ^ Exp
			fstp fRes           // fRes = 10 ^ Exp, st(0) = ipart
			fstp st(0) }		// st empty
    return fRes;
}

inline Float _fastcall FPU::PowerN( Float fBase, Int iExponent ) {
    __asm { fld1				// st(0) = 1.0
			fld fBase           // st(1) = 1.0, st(0) = fBase
			fyl2x				// st(0) = 1.0 * log2(fBase)
			fild iExponent		// st(1) = log2(fBase), st(0) = Exp
            fmulp st(1), st(0)  // st(0) = Exp * log2(fBase)
			fld st(0)           // st(1) = Exp * log2(fBase), st(0) = Exp * log2(fBase)
			frndint				// st(1) = Exp * log2(fBase), st(0) = round0(Exp * log2(fBase)) = ipart
			fsub st(1), st(0)	// st(1) = (Exp * log2(fBase) - ipart) = fpart, st(0) = ipart
			fxch				// st(1) = ipart, st(0) = fpart
			f2xm1				// st(1) = ipart, st(0) = 2 ^ fpart - 1
			fld1				// st(2) = ipart, st(1) = 2 ^ fpart - 1, st(0) = 1.0
			faddp st(1), st(0)  // st(1) = ipart, st(0) = 2 ^ fpart
			fscale				// st(1) = ipart, st(0) = 2 ^ fpart * 2 ^ ipart = 2 ^ (Exp * log2(fBase)) = fBase ^ Exp
			fstp fBase          // fBase = fBase ^ Exp, st(0) = ipart
			fstp st(0) }        // st empty
	return fBase;
}
inline Double _fastcall FPU::PowerN( Double fBase, Int iExponent ) {
    __asm { fld1				// st(0) = 1.0
			fld fBase           // st(1) = 1.0, st(0) = fBase
			fyl2x				// st(0) = 1.0 * log2(fBase)
			fild iExponent		// st(1) = log2(fBase), st(0) = Exp
            fmulp st(1), st(0)  // st(0) = Exp * log2(fBase)
			fld st(0)           // st(1) = Exp * log2(fBase), st(0) = Exp * log2(fBase)
			frndint				// st(1) = Exp * log2(fBase), st(0) = round0(Exp * log2(fBase)) = ipart
			fsub st(1), st(0)	// st(1) = (Exp * log2(fBase) - ipart) = fpart, st(0) = ipart
			fxch				// st(1) = ipart, st(0) = fpart
			f2xm1				// st(1) = ipart, st(0) = 2 ^ fpart - 1
			fld1				// st(2) = ipart, st(1) = 2 ^ fpart - 1, st(0) = 1.0
			faddp st(1), st(0)  // st(1) = ipart, st(0) = 2 ^ fpart
			fscale				// st(1) = ipart, st(0) = 2 ^ fpart * 2 ^ ipart = 2 ^ (Exp * log2(fBase)) = fBase ^ Exp
			fstp fBase          // fBase = fBase ^ Exp, st(0) = ipart
			fstp st(0) }        // st empty
	return fBase;
}

inline Float _fastcall FPU::Sin( Float fValue ) {
    __asm { fld fValue      // st(0) = fValue
			fsin            // st(0) = sin(fValue)
			fstp fValue }   // fValue = sin(fValue), st empty
	return fValue;
}
inline Double _fastcall FPU::Sin( Double fValue ) {
    __asm { fld fValue      // st(0) = fValue
			fsin            // st(0) = sin(fValue)
			fstp fValue }   // fValue = sin(fValue), st empty
	return fValue;
}

inline Float _fastcall FPU::Cos( Float fValue ) {
    __asm { fld fValue      // st(0) = fValue
			fcos            // st(0) = cos(fValue)
			fstp fValue }   // fValue = cos(fValue), st empty
	return fValue;
}
inline Double _fastcall FPU::Cos( Double fValue ) {
    __asm { fld fValue      // st(0) = fValue
			fcos            // st(0) = cos(fValue)
			fstp fValue }   // fValue = cos(fValue), st empty
	return fValue;
}

inline Void _fastcall FPU::SinCos( Float * outSin, Float * outCos, Float fValue ) {
    Float fTmp;
    __asm { fld fValue      // st(0) = fValue
			fsincos         // st(1) = sin(fValue), st(0) = cos(fValue)
			fstp fTmp       // fTmp = cos(fValue), st(0) = sin(fValue)
			fstp fValue }   // fValue = sin(fValue), st empty
    *outSin = fValue;
	*outCos = fTmp;
}
inline Void _fastcall FPU::SinCos( Double * outSin, Double * outCos, Double fValue ) {
    Double fTmp;
    __asm { fld fValue      // st(0) = fValue
			fsincos         // st(1) = sin(fValue), st(0) = cos(fValue)
			fstp fTmp       // fTmp = cos(fValue), st(0) = sin(fValue)
			fstp fValue }   // fValue = sin(fValue), st empty
    *outSin = fValue;
	*outCos = fTmp;
}

inline Float _fastcall FPU::Tan( Float fValue ) {
    __asm { fld fValue      // st(0) = fValue
			fptan           // st(1) = tan(fValue), st(0) = 1
			fstp fValue     // fValue = 1, st(0) = tan(fValue)
			fstp fValue }   // fValue = tan(fValue), st empty
	return fValue;
}
inline Double _fastcall FPU::Tan( Double fValue ) {
    __asm { fld fValue      // st(0) = fValue
			fptan           // st(1) = tan(fValue), st(0) = 1
			fstp fValue     // fValue = 1, st(0) = tan(fValue)
			fstp fValue }   // fValue = tan(fValue), st empty
	return fValue;
}

inline Float _fastcall FPU::ArcSin( Float fValue ) {
    __asm { fld fValue          // st(0) = fValue
			fld st(0)           // st(1) = fValue, st(0) = fValue
			fmul st(0), st(1)   // st(1) = fValue, st(0) = (fValue*fValue)
			fld1                // st(2) = fValue, st(1) = (fValue*fValue), st(0) = 1
			fsubrp st(1), st(0) // st(1) = fValue, st(0) = (1-fValue*fValue)
			fsqrt               // st(1) = fValue, st(0) = sqrt(1-fValue*fValue)
            fpatan              // st(0) = arctan( fValue / sqrt(1-fValue*fValue) ) = arcsin(fValue)
            fstp fValue }       // fValue = arcsin(fValue), st empty
    return fValue;
}
inline Double _fastcall FPU::ArcSin( Double fValue ) {
    __asm { fld fValue          // st(0) = fValue
			fld st(0)           // st(1) = fValue, st(0) = fValue
			fmul st(0), st(1)   // st(1) = fValue, st(0) = (fValue*fValue)
			fld1                // st(2) = fValue, st(1) = (fValue*fValue), st(0) = 1
			fsubrp st(1), st(0) // st(1) = fValue, st(0) = (1-fValue*fValue)
			fsqrt               // st(1) = fValue, st(0) = sqrt(1-fValue*fValue)
            fpatan              // st(0) = arctan( fValue / sqrt(1-fValue*fValue) ) = arcsin(fValue)
            fstp fValue }       // fValue = arcsin(fValue), st empty
    return fValue;
}

inline Float _fastcall FPU::ArcCos( Float fValue ) {
    __asm { fld fValue          // st(0) = fValue
			fld st(0)           // st(1) = fValue, st(0) = fValue
			fmul st(0), st(1)   // st(1) = fValue, st(0) = (fValue*fValue)
			fld1                // st(2) = fValue, st(1) = (fValue*fValue), st(0) = 1
			fsubrp st(1), st(0) // st(1) = fValue, st(0) = (1-fValue*fValue)
			fsqrt               // st(1) = fValue, st(0) = sqrt(1-fValue*fValue)
            fxch                // st(1) = sqrt(1-fValue*fValue), st(0) = fValue
            fpatan              // st(0) = arctan( sqrt(1-fValue*fValue) / fValue ) = arccos(fValue)
            fstp fValue }       // fValue = arccos(fValue), st empty
    return fValue;
}
inline Double _fastcall FPU::ArcCos( Double fValue ) {
    __asm { fld fValue          // st(0) = fValue
			fld st(0)           // st(1) = fValue, st(0) = fValue
			fmul st(0), st(1)   // st(1) = fValue, st(0) = (fValue*fValue)
			fld1                // st(2) = fValue, st(1) = (fValue*fValue), st(0) = 1
			fsubrp st(1), st(0) // st(1) = fValue, st(0) = (1-fValue*fValue)
			fsqrt               // st(1) = fValue, st(0) = sqrt(1-fValue*fValue)
            fxch                // st(1) = sqrt(1-fValue*fValue), st(0) = fValue
            fpatan              // st(0) = arctan( sqrt(1-fValue*fValue) / fValue ) = arccos(fValue)
            fstp fValue }       // fValue = arccos(fValue), st empty
    return fValue;
}

inline Float _fastcall FPU::ArcTan( Float fValue ) {
    __asm { fld fValue      // st(0) = fValue
			fld1            // st(1) = fValue, st(0) = 1
			fpatan          // st(0) = arctan( fValue / 1 ) = arctan(fValue)
			fstp fValue }   // fValue = arctan(fValue), st empty
	return fValue;
}
inline Double _fastcall FPU::ArcTan( Double fValue ) {
    __asm { fld fValue      // st(0) = fValue
			fld1            // st(1) = fValue, st(0) = 1
			fpatan          // st(0) = arctan( fValue / 1 ) = arctan(fValue)
			fstp fValue }   // fValue = arctan(fValue), st empty
	return fValue;
}

inline Float _fastcall FPU::ArcTan2( Float fNum, Float fDenom ) {
    __asm { fld fNum    // st(0) = fNum
			fld fDenom  // st(1) = fNum, st(0) = fDenom
			fpatan      // st(0) = arctan( fNum / fDenom )
			fstp fNum } // fNum = arctan( fNum / fDenom ), st empty
	return fNum;
}
inline Double _fastcall FPU::ArcTan2( Double fNum, Double fDenom ) {
    __asm { fld fNum    // st(0) = fNum
			fld fDenom  // st(1) = fNum, st(0) = fDenom
			fpatan      // st(0) = arctan( fNum / fDenom )
			fstp fNum } // fNum = arctan( fNum / fDenom ), st empty
	return fNum;
}

