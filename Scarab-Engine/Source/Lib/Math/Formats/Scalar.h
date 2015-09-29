/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Formats/Scalar.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Scalar provides floating point arithmetic support
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
#ifndef SCARAB_LIB_MATH_FORMATS_SCALAR_H
#define SCARAB_LIB_MATH_FORMATS_SCALAR_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../ThirdParty/System/Platform.h"

/////////////////////////////////////////////////////////////////////////////////
// Conditional Compilation Settings
#define MATH_SCALAR_SINGLE_PRECISION
//#define MATH_SCALAR_DOUBLE_PRECISION // rarely used in real-time engines

/////////////////////////////////////////////////////////////////////////////////
// Scalar definitions
#if defined(MATH_SCALAR_SINGLE_PRECISION)
	typedef Float			   Scalar;
	typedef FloatConverter	   ScalarConverter;
    inline DWord IntRepr(Scalar f);
	inline Scalar ScalarRepr(DWord i);

    #define SCALAR_DIGITS      FLOAT_DIGITS
    #define SCALAR_ERROR	   0.000001f

    #define SCALAR_SIGN_BITS   FLOAT_SIGN_BITS
    #define SCALAR_EXP_BITS    FLOAT_EXP_BITS
    #define SCALAR_MANT_BITS   FLOAT_MANT_BITS

    #define SCALAR_SIGN_MASK   FLOAT_SIGN_MASK
    #define SCALAR_EXP_MASK    FLOAT_EXP_MASK
    #define SCALAR_MANT_MASK   FLOAT_MANT_MASK

    #define SCALAR_EXP_BIAS    FLOAT_EXP_BIAS

    #define SCALAR_NAN         FLOAT_NAN

	#define SCALAR_EPSILON     FLOAT_EPSILON
	#define SCALAR_MIN         FLOAT_MIN
	#define SCALAR_MAX         FLOAT_MAX
	#define SCALAR_INFINITE    FLOAT_INFINITE

    #define SCALAR_EXP_MIN_BIN FLOAT_EXP_MIN_BIN
    #define SCALAR_EXP_MAX_BIN FLOAT_EXP_MAX_BIN
    #define SCALAR_EXP_MIN     FLOAT_EXP_MIN
    #define SCALAR_EXP_MAX     FLOAT_EXP_MAX

#elif defined(MATH_SCALAR_DOUBLE_PRECISION)
    typedef Double			   Scalar;
	typedef DoubleConverter	   ScalarConverter;
    inline QWord IntRepr(Scalar f);
	inline Scalar ScalarRepr(QWord i);

    #define SCALAR_DIGITS      DOUBLE_DIGITS
    #define SCALAR_ERROR	   0.00000000001

    #define SCALAR_SIGN_BITS   DOUBLE_SIGN_BITS
    #define SCALAR_EXP_BITS    DOUBLE_EXP_BITS
    #define SCALAR_MANT_BITS   DOUBLE_MANT_BITS

    #define SCALAR_SIGN_MASK   DOUBLE_SIGN_MASK
    #define SCALAR_EXP_MASK    DOUBLE_EXP_MASK
    #define SCALAR_MANT_MASK   DOUBLE_MANT_MASK

    #define SCALAR_EXP_BIAS    DOUBLE_EXP_BIAS

    #define SCALAR_NAN         DOUBLE_NAN

	#define SCALAR_EPSILON     DOUBLE_EPSILON
	#define SCALAR_MIN         DOUBLE_MIN
	#define SCALAR_MAX         DOUBLE_MAX
	#define SCALAR_INFINITE    DOUBLE_INFINITE

    #define SCALAR_EXP_MIN_BIN DOUBLE_EXP_MIN_BIN
    #define SCALAR_EXP_MAX_BIN DOUBLE_EXP_MAX_BIN
    #define SCALAR_EXP_MIN     DOUBLE_EXP_MIN
    #define SCALAR_EXP_MAX     DOUBLE_EXP_MAX

#endif

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#if defined(MATH_SCALAR_SINGLE_PRECISION)
    #define SCALAR_E          2.7182818285f
	#define SCALAR_1_E        0.367879441171f
	#define SCALAR_LN2        0.693147180560f
	#define SCALAR_LN10       2.30258509299f
	#define SCALAR_1_LN2      1.44269504089f
	#define SCALAR_1_LN10     0.434294481903f
	#define SCALAR_PI_6       0.523598775598f
	#define SCALAR_PI_4       0.785398163397f
	#define SCALAR_PI_3       1.0471975512f
	#define SCALAR_PI_2       1.57079632679f
	#define SCALAR_2PI_3      2.09439510239f
	#define SCALAR_3PI_4      2.35619449019f
	#define SCALAR_5PI_6      2.61799387799f
	#define SCALAR_PI         3.14159265359f
	#define SCALAR_5PI_4      3.92699081699f
	#define SCALAR_4PI_3      4.18879020479f
	#define SCALAR_3PI_2      4.71238898038f
	#define SCALAR_7PI_4      5.49778714378f
	#define SCALAR_2PI        6.28318530718f
	#define SCALAR_1_PI       0.318309886184f
	#define SCALAR_2_PI       0.636619772368f
	#define SCALAR_2_SQRTPI   1.12837916710f
	#define SCALAR_SQRT2      1.41421356237f
	#define SCALAR_SQRT2_2    0.707106781187f
	#define SCALAR_SQRT3      1.73205080757f
	#define SCALAR_SQRT3_2    0.866025403784f
	#define SCALAR_1_SQRT2    0.707106781187f
	#define SCALAR_1_SQRT3    0.57735026919f
	#define SCALAR_SQRT_SQRT2 1.189207115f

#elif defined(MATH_SCALAR_DOUBLE_PRECISION)
    #define SCALAR_E          2.71828182845904523536
	#define SCALAR_1_E        0.367879441171442321595
	#define SCALAR_LN2        0.693147180559945309417
	#define SCALAR_LN10       2.30258509299404568402
	#define SCALAR_1_LN2      1.44269504088896340736
	#define SCALAR_1_LN10     0.434294481903251827651
	#define SCALAR_PI_6       0.523598775598298873077
	#define SCALAR_PI_4       0.785398163397448309616
	#define SCALAR_PI_3       1.04719755119659774615
	#define SCALAR_PI_2       1.57079632679489661923
	#define SCALAR_2PI_3      2.09439510239319549231
	#define SCALAR_3PI_4      2.35619449019234492885
	#define SCALAR_5PI_6      2.61799387799149436539
	#define SCALAR_PI         3.14159265358979323846
	#define SCALAR_5PI_4      3.92699081698724154808
	#define SCALAR_4PI_3      4.18879020478639098462
	#define SCALAR_3PI_2      4.71238898038468985769
	#define SCALAR_7PI_4      5.49778714378213816731
	#define SCALAR_2PI        6.28318530717958647692
	#define SCALAR_1_PI       0.318309886183790671538
	#define SCALAR_2_PI       0.636619772367581343076
	#define SCALAR_2_SQRTPI   1.12837916709551257390
	#define SCALAR_SQRT2      1.41421356237309504880
	#define SCALAR_SQRT2_2    0.707106781186547524401
	#define SCALAR_SQRT3      1.73205080756887729352
	#define SCALAR_SQRT3_2    0.866025403784438646763
	#define SCALAR_1_SQRT2    0.707106781186547524401
	#define SCALAR_1_SQRT3    0.577350269189625764509
	#define SCALAR_SQRT_SQRT2 1.18920711500272106671

#endif

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Scalar.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_FORMATS_SCALAR_H
