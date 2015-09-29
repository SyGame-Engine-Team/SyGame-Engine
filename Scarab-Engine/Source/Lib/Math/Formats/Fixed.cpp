/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Formats/Fixed.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Fixed provides fixed point arithmetic support
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
#include "Fixed.h"

/////////////////////////////////////////////////////////////////////////////////
// Fixed implementation
const Fixed Fixed::Zero = Fixed(0.0f);
const Fixed Fixed::One = Fixed(1.0f);
const Fixed Fixed::Infinity = Fixed(FIXED_INFINITE);

const Int Fixed::RAW = 7777777; // dumb value
const Int Fixed::FP = FIXED_FRACT_BITS;
const Int Fixed::FP_POW = (1 << FIXED_FRACT_BITS);
const Double Fixed::FP_INVPOW = ( 1.0 / ( (Double)(1 << FIXED_FRACT_BITS) ) );
const Int Fixed::FP_DBL = (FIXED_FRACT_BITS << 1); // *2
const Int Fixed::FP_HALF = (FIXED_FRACT_BITS >> 1); // /2

Fixed Fixed::Sqrt(const Fixed & fpValue)
{
    Int64 iRoot = 0;
    Int64 iLeft = ( ((Int64)fpValue.m_fpValue) << FP );
    Int64 iMask = ( 1i64 << ((sizeof(Int64) << 3) - 2) );
    while( iMask != 0 ) {
        if ( (iLeft & (-iMask)) > iRoot ) {
            iRoot += iMask;
            iLeft -= iRoot;
            iRoot += iMask;
        }
        iRoot >>= 1;
        iMask >>= 2;
    }
    return Fixed( RAW, (Int)iRoot );
}
Fixed Fixed::ArcTan2(const Fixed & fNum, const Fixed & fDenom)
{
	Fixed fpAbsY = Abs(fNum) + FIXED_EPSILON;
	Fixed fpR, fpTheta;
	if ( fDenom >= Zero ) {
		fpR = (fDenom - fpAbsY) / (fDenom + fpAbsY);
		fpTheta = Fixed(SCALAR_PI_4);
	} else {
		fpR = (fpAbsY + fDenom) / (fpAbsY - fDenom);
		fpTheta = Fixed(SCALAR_3PI_4);
	}
	fpTheta += ( Fixed(0.1963f) * (fpR*fpR*fpR) - Fixed(0.9817f) * fpR );
	return (fNum < Zero) ? -fpTheta : fpTheta;
}

