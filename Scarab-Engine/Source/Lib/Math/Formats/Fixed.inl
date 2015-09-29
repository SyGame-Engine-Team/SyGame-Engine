/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Formats/Fixed.inl
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
// Fixed implementation
inline Fixed::Fixed(Int /*dummyarg*/, Int fpValue) { m_fpValue = fpValue; }

inline Fixed::Fixed()                  {}
inline Fixed::Fixed(const Fixed & rhs) { m_fpValue = rhs.m_fpValue; }
inline Fixed::Fixed(UChar iValue)      { m_fpValue = ( ((SInt)iValue) << FP ); }
inline Fixed::Fixed(SChar iValue)      { m_fpValue = ( ((SInt)iValue) << FP ); }
inline Fixed::Fixed(UShort iValue)     { m_fpValue = ( ((SInt)iValue) << FP ); }
inline Fixed::Fixed(SShort iValue)     { m_fpValue = ( ((SInt)iValue) << FP ); }
inline Fixed::Fixed(ULong iValue)      { m_fpValue = ( ((SInt)iValue) << FP ); }
inline Fixed::Fixed(SLong iValue)      { m_fpValue = ( ((SInt)iValue) << FP ); }
inline Fixed::Fixed(UInt iValue)       { m_fpValue = ( ((signed)iValue) << FP ); }
inline Fixed::Fixed(SInt iValue)       { m_fpValue = ( iValue << FP ); }
inline Fixed::Fixed(Float fValue)      { m_fpValue = (Int)(fValue * (Float)FP_POW); }
inline Fixed::Fixed(Double fValue)     { m_fpValue = (Int)(fValue * (Double)FP_POW); }

inline Fixed::operator UChar() const  { return (UChar)(m_fpValue >> FP); }
inline Fixed::operator SChar() const  { return (SChar)(m_fpValue >> FP); }
inline Fixed::operator UShort() const { return (UShort)(m_fpValue >> FP); }
inline Fixed::operator SShort() const { return (SShort)(m_fpValue >> FP); }
inline Fixed::operator ULong() const  { return (ULong)(m_fpValue >> FP); }
inline Fixed::operator SLong() const  { return (SLong)(m_fpValue >> FP); }
inline Fixed::operator UInt() const   { return (unsigned)(m_fpValue >> FP); }
inline Fixed::operator SInt() const   { return (m_fpValue >> FP); }
inline Fixed::operator Float() const  { return (m_fpValue * (Float)FP_INVPOW); }
inline Fixed::operator Double() const { return (m_fpValue * FP_INVPOW); }

inline Fixed & Fixed::operator=(const Fixed & rhs)	{ m_fpValue = rhs.m_fpValue; return (*this); }

inline Fixed Fixed::operator>>(const Int rhs) const { return Fixed( RAW, m_fpValue >> rhs ); }
inline Fixed Fixed::operator<<(const Int rhs) const { return Fixed( RAW, m_fpValue << rhs ); }
inline Fixed & Fixed::operator>>=(const Int rhs)    { m_fpValue >>= rhs; return (*this); }
inline Fixed & Fixed::operator<<=(const Int rhs)    { m_fpValue <<= rhs; return (*this); }

inline Fixed Fixed::operator+() const { return Fixed( RAW, m_fpValue ); }
inline Fixed Fixed::operator-() const { return Fixed( RAW, -m_fpValue ); }

inline Fixed Fixed::operator+(const Fixed & rhs) const	{ return Fixed( RAW, m_fpValue + rhs.m_fpValue ); }
inline Fixed Fixed::operator-(const Fixed & rhs) const	{ return Fixed( RAW, m_fpValue - rhs.m_fpValue ); }
inline Fixed Fixed::operator*(const Fixed & rhs) const {
	return Fixed( RAW, (m_fpValue >> FP_HALF) * (rhs.m_fpValue >> FP_HALF) );
}
inline Fixed Fixed::operator/(const Fixed & rhs) const {
	return Fixed( RAW, (Int)( (((Int64)m_fpValue) << FP) / (Int64)(rhs.m_fpValue) ) );
}

inline Fixed & Fixed::operator+=(const Fixed & rhs)	{ m_fpValue += rhs.m_fpValue; return (*this); }
inline Fixed & Fixed::operator-=(const Fixed & rhs)	{ m_fpValue -= rhs.m_fpValue; return (*this); }
inline Fixed & Fixed::operator*=(const Fixed & rhs)	{ *this = (*this) * rhs; return (*this); }
inline Fixed & Fixed::operator/=(const Fixed & rhs)	{ *this = (*this) / rhs; return (*this); }

inline Bool Fixed::operator==(const Fixed & rhs) const	{ return (m_fpValue == rhs.m_fpValue); }
inline Bool Fixed::operator!=(const Fixed & rhs) const	{ return (m_fpValue != rhs.m_fpValue); }
inline Bool Fixed::operator<=(const Fixed & rhs) const	{ return (m_fpValue <= rhs.m_fpValue); }
inline Bool Fixed::operator>=(const Fixed & rhs) const	{ return (m_fpValue >= rhs.m_fpValue); }
inline Bool Fixed::operator<(const Fixed & rhs) const	{ return (m_fpValue < rhs.m_fpValue); }
inline Bool Fixed::operator>(const Fixed & rhs) const	{ return (m_fpValue > rhs.m_fpValue); }

inline Fixed Fixed::Abs(const Fixed & fpValue) {
    return (fpValue < Zero) ? -fpValue : fpValue;
}

