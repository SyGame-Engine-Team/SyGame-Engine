/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Functions/MathFunction.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Basic functions
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
#include "MathFunction.h"

/////////////////////////////////////////////////////////////////////////////////
// MathFunction implementation

/////////////////////////////////////////////////////////////////////////////////

template<>
Float MathFunction<Float>::_Tricked_Invert(Float f) const
{
    static const DWord s_Float1_0 = 0x3f800000ul;

    m_FloatConverter.f = f;
	m_FloatConverter.i = ( (s_Float1_0 << 1) - m_FloatConverter.i );
	f = m_FloatConverter.f * ( 2.0f - (f * m_FloatConverter.f) );

    return f;
}
template<>
Double MathFunction<Double>::_Tricked_Invert(Double f) const
{
    static const QWord s_Double1_0 = 0x3ff0000000000000ui64;

    m_DoubleConverter.f = f;
	m_DoubleConverter.i = ( (s_Double1_0 << 1) - m_DoubleConverter.i );
	f = m_DoubleConverter.f * ( 2.0 - (f * m_DoubleConverter.f) );

    return f;
}

template<>
Float MathFunction<Float>::_Tricked_InvSqrt(Float f) const
{
	// Nvidia trick
    //m_FloatConverter.f = f;
    //m_FloatConverter.i = ( (FLOAT_1_0 << 1) + FLOAT_1_0 - m_FloatConverter.i ) >> 1;
    //f = m_FloatConverter.f * ( 1.47f - (0.47f * f * m_FloatConverter.f * m_FloatConverter.f) );

	// A little variation
	m_FloatConverter.f = f;
	m_FloatConverter.i = 0x5f3759df - ( m_FloatConverter.i >> 1 );
	f = m_FloatConverter.f * ( 1.5f - (0.5f * f * m_FloatConverter.f * m_FloatConverter.f) );

	return f;
}
template<>
Double MathFunction<Double>::_Tricked_InvSqrt(Double f) const
{
    // Nvidia trick
    //m_DoubleConverter.f = f;
    //m_DoubleConverter.i = ( (DOUBLE_1_0 << 1) + DOUBLE_1_0 - m_DoubleConverter.i ) >> 1;
    //f = m_DoubleConverter.f * ( 1.47 - (0.47 * f * m_DoubleConverter.f * m_DoubleConverter.f) );

    // A little variation
    m_DoubleConverter.f = f;
	m_DoubleConverter.i = 0x5fe6ec85e7de30da - ( m_DoubleConverter.i >> 1 );
	f = m_DoubleConverter.f * ( 1.5 - (0.5 * f * m_DoubleConverter.f * m_DoubleConverter.f) );

	return f;
}

