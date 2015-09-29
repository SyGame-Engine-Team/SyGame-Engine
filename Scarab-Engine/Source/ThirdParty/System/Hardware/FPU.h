/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/System/Hardware/FPU.h
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
// Known Bugs : TODO = morph this to FPUStack, like SSE ...
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_THIRDPARTY_SYSTEM_HARDWARE_FPU_H
#define SCARAB_THIRDPARTY_SYSTEM_HARDWARE_FPU_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Platform.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define FPUFn FPU::GetInstancePtr()

    // IEEE 754 specifications
typedef DWord FPUClass;
#define FPU_CLASS_UNSUPPORTED 0x00
#define FPU_CLASS_NAN         0x01
#define FPU_CLASS_NORMAL      0x02
#define FPU_CLASS_INFINITY    0x03
#define FPU_CLASS_ZERO        0x04
#define FPU_CLASS_EMPTY       0x05
#define FPU_CLASS_DENORMAL    0x06

typedef Word FPUPrecisionMode;
#define FPU_PRECISION_SINGLE   0 // 24 bits
#define FPU_PRECISION_DOUBLE   2 // 53 bits
#define FPU_PRECISION_EXTENDED 3 // 64 bits

typedef Word FPURoundingMode;
#define FPU_ROUND_NEAREST  0 // floor if equally close
#define FPU_ROUND_FLOOR    1 // toward -inf
#define FPU_ROUND_CEIL     2 // toward +inf
#define FPU_ROUND_TRUNCATE 3 // toward 0 (abs round)

/////////////////////////////////////////////////////////////////////////////////
// The FPU class
class FPU
{
    // Discrete singleton interface
public:
    inline static FPU * GetInstancePtr();

private:
    FPU();
    ~FPU();

public:
    static Void SetInternalPrecision( FPUPrecisionMode iPrecisionMode = FPU_PRECISION_EXTENDED );
    static Void SetRoundingMode( FPURoundingMode iRoundingMode = FPU_ROUND_NEAREST );

    static FPUClass Inspect( Float fValue, Bool * outIsNegative = NULL );
    static FPUClass Inspect( Double fValue, Bool * outIsNegative = NULL );

    inline static Int _fastcall Floor( Float fValue );
    inline static Int _fastcall Floor( Double fValue );

    inline static Int _fastcall Ceil( Float fValue );
    inline static Int _fastcall Ceil( Double fValue );

    inline static Int _fastcall Round( Float fValue );
    inline static Int _fastcall Round( Double fValue );

    inline static Float _fastcall Abs( Float fValue );
    inline static Double _fastcall Abs( Double fValue );

    inline static Float _fastcall Mod( Float fValue, Float fMod );
    inline static Double _fastcall Mod( Double fValue, Double fMod );

    inline static Float _fastcall Sqrt( Float fValue );
    inline static Double _fastcall Sqrt( Double fValue );

    inline static Float _fastcall Ln( Float fValue );
    inline static Double _fastcall Ln( Double fValue );

    inline static Float _fastcall Log2( Float fValue );
    inline static Double _fastcall Log2( Double fValue );

    inline static Float _fastcall Log10( Float fValue );
    inline static Double _fastcall Log10( Double fValue );

    inline static Float _fastcall LogN( Float fBase, Float fValue );
    inline static Double _fastcall LogN( Double fBase, Double fValue );

    inline static Float _fastcall Exp( Float fValue );
    inline static Double _fastcall Exp( Double fValue );

    inline static Float _fastcall Exp2( Float fValue );
    inline static Double _fastcall Exp2( Double fValue );

    inline static Float _fastcall Exp10( Float fValue );
    inline static Double _fastcall Exp10( Double fValue );

    inline static Float _fastcall ExpN( Float fBase, Float fValue );
    inline static Double _fastcall ExpN( Double fBase, Double fValue );

    inline static Float _fastcall Power2f( Int iExponent );
    inline static Double _fastcall Power2d( Int iExponent );

    inline static Float _fastcall Power10f( Int iExponent );
    inline static Double _fastcall Power10d( Int iExponent );

    inline static Float _fastcall PowerN( Float fBase, Int iExponent );
    inline static Double _fastcall PowerN( Double fBase, Int iExponent );

    inline static Float _fastcall Sin( Float fValue );
    inline static Double _fastcall Sin( Double fValue );

    inline static Float _fastcall Cos( Float fValue );
    inline static Double _fastcall Cos( Double fValue );

    inline static Void _fastcall SinCos( Float * outSin, Float * outCos, Float fValue );
    inline static Void _fastcall SinCos( Double * outSin, Double * outCos, Double fValue );

    inline static Float _fastcall Tan( Float fValue );
    inline static Double _fastcall Tan( Double fValue );

    inline static Float _fastcall ArcSin( Float fValue ); // returns in [-pi;pi], sign of fValue
    inline static Double _fastcall ArcSin( Double fValue );

    inline static Float _fastcall ArcCos( Float fValue ); // returns in [-pi;pi], sign of fValue
    inline static Double _fastcall ArcCos( Double fValue );

    inline static Float _fastcall ArcTan( Float fValue ); // = arctan2( f, 1.0f ) in [-pi/2;pi/2], sign of fValue
    inline static Double _fastcall ArcTan( Double fValue );

    inline static Float _fastcall ArcTan2( Float fNum, Float fDenom ); // returns in [-pi;pi], sign of fNum
    inline static Double _fastcall ArcTan2( Double fNum, Double fDenom );

private:
    static const Float sm_fHalfF;
    static const Double sm_fHalfD;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "FPU.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_THIRDPARTY_SYSTEM_HARDWARE_FPU_H

