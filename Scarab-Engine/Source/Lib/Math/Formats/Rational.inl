/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Formats/Rational.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Rational provides exact rational arithmetic support
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Rational implementation
template<UInt INT_BYTES>
Rational<INT_BYTES>::Rational():
    m_iNumerator(0u), m_iDenominator(1u)
{
    // nothing to do
}
template<UInt INT_BYTES>
Rational<INT_BYTES>::Rational( const Rational & rhs ):
    m_iNumerator(rhs.m_iNumerator), m_iDenominator(rhs.m_iDenominator)
{
    // nothing to do
}
template<UInt INT_BYTES>
Rational<INT_BYTES>::Rational( const Integer<INT_BYTES> & intNumerator ):
    m_iNumerator(intNumerator), m_iDenominator(1u)
{
    // nothing to do
}
template<UInt INT_BYTES>
Rational<INT_BYTES>::Rational( const Integer<INT_BYTES> & intNumerator, const Integer<INT_BYTES> & intDenominator ):
    m_iNumerator(intNumerator), m_iDenominator(intDenominator)
{
    // nothing to do
}
template<UInt INT_BYTES>
Rational<INT_BYTES>::Rational( Float fValue ):
    m_iNumerator(0u), m_iDenominator(1u)
{
    if ( fValue == 0.0f )
        return;

    FloatConverter fConv;
    fConv.f = fValue;
    UInt iSign = ( fConv.i & FLOAT_SIGN_MASK );
    Int iExp = ( (fConv.i & FLOAT_EXP_MASK) >> FLOAT_MANT_BITS );
    UInt iMant = ( fConv.i & FLOAT_MANT_MASK );

    // Inf / NaN case, treat as +-inf
    if ( iExp == 255 ) {
        MemSet( m_iNumerator.m_arrDigits, 0xff, INT_BYTES );
        m_iNumerator._SetSign( iSign != 0 );
        return;
    }

    // Denormalized case, (-1)^s * 2^(-126) * 0.m = (-1)^s * 2^(e-149) * m
    if ( iExp == 0 ) {
        m_iNumerator = iMant;
        m_iDenominator <<= 149;
        m_iNumerator._SetSign( iSign != 0 );
        return;
    }

    // Normalized case, (-1)^s * 2^(e-127) * 1.m = (-1)^s * 2^(e-127) * (2^23 + m) * 2^-23
    //                                           = (-1)^s * 2^(e-150) * (2^23 + m)
    m_iNumerator = ( (1 << FLOAT_MANT_BITS) + iMant );
    Int iPower2 = ( iExp - 150 );
    if ( iPower2 > 0 )
        m_iNumerator <<= iPower2;
    else if ( iPower2 < 0 )
        m_iDenominator <<= (-iPower2);
    m_iNumerator._SetSign( iSign != 0 );
}
template<UInt INT_BYTES>
Rational<INT_BYTES>::Rational( Double fValue ):
    m_iNumerator(0u), m_iDenominator(1u)
{
    if ( fValue == 0.0 )
        return;

    DoubleConverter fConv;
    fConv.f = fValue;
    UInt64 iSign = ( fConv.i & DOUBLE_SIGN_MASK );
    Int64 iExp = ( (fConv.i & DOUBLE_EXP_MASK) >> DOUBLE_MANT_BITS );
    UInt64 iMant = ( fConv.i & DOUBLE_MANT_MASK );

    // Inf / NaN case, treat as +-inf
    if ( iExp == 2047 ) {
        MemSet( m_iNumerator.m_arrDigits, 0xff, INT_BYTES );
        m_iNumerator._SetSign( iSign != 0 );
        return;
    }

    // Denormalized case, (-1)^s * 2^(-1022) * 0.m = (-1)^s * 2^(e-1074) * m
    if ( iExp == 0 ) {
        m_iNumerator = iMant;
        m_iDenominator <<= 1074;
        m_iNumerator._SetSign( iSign != 0 );
        return;
    }

    // Normalized case, (-1)^s * 2^(e-1023) * 1.m = (-1)^s * 2^(e-1023) * (2^52 + m) * 2^-52
    //                                            = (-1)^s * 2^(e-1075) * (2^52 + m)
    m_iNumerator = ( (1ui64 << DOUBLE_MANT_BITS) + iMant );
    Int iPower2 = ( iExp - 1075 );
    if ( iPower2 > 0 )
        m_iNumerator <<= iPower2;
    else if ( iPower2 < 0 )
        m_iDenominator <<= (-iPower2);
    m_iNumerator._SetSign( iSign != 0 );
}
template<UInt INT_BYTES>
Rational<INT_BYTES>::~Rational()
{
    // nothing to do
}

template<UInt INT_BYTES>
Rational<INT_BYTES>::operator Float() const
{
    if ( m_iNumerator == 0 )
        return 0.0f;

    Int iSignN = m_iNumerator._GetSign();
    Int iSignD = m_iDenominator._GetSign();
    Integer<INT_BYTES> intAbsN(m_iNumerator);
    Integer<INT_BYTES> intAbsD(m_iDenominator);
    intAbsN._SetSign(false);
    intAbsD._SetSign(false);

    UInt iLeadBitN = intAbsN._LeadingBit();
    UInt iLeadBitD = intAbsD._LeadingBit();
    Int iPower2 = ( iLeadBitN - iLeadBitD );
    if ( iPower2 > 0 )
        intAbsD <<= iPower2;
    else if ( iPower2 < 0 )
        intAbsN <<= (-iPower2);
    if ( intAbsN < intAbsD ) {
        intAbsN <<= 1;
        --iPower2;
    }

    FloatConverter fConv;

    // Infinite case
    if ( iPower2 > FLOAT_EXP_BIAS ) {
        fConv.i = 0x7f7fffff;
        if ( iSignN * iSignD < 0 )
            fConv.i |= 0x80000000;
        return fConv.f;
    }

    // Epsilon case
    if ( iPower2 < -(FLOAT_EXP_BIAS + FLOAT_MANT_BITS - 1) ) {
        fConv.i = 0;
        if ( iSignN * iSignD < 0 )
            fConv.i |= 0x80000000;
        return fConv.f;
    }

    // Normal / Denormal cases
    UInt iExp, iMantissa, iMantissaBit;
    Int iShift;
    if ( iPower2 >= -(FLOAT_EXP_BIAS - 1) ) {
        iExp = iPower2 + FLOAT_EXP_BIAS;
        iMantissaBit = 1;
        iShift = 0;
    } else {
        iExp = 0;
        iMantissaBit = 0;
        intAbsD <<= 1;
        iShift = -(iPower2 + FLOAT_EXP_BIAS);
    }
    iMantissa = 0;
    UInt iMask = ( ( 1 << (FLOAT_MANT_BITS - 1) ) >> iShift );
    while( iMask > 0 ) {
        if ( iMantissaBit == 1 )
            intAbsN -= intAbsD;
        intAbsN <<= 1;
        if ( intAbsN >= intAbsD ) {
            iMantissaBit = 1;
            iMantissa |= iMask;
        } else
            iMantissaBit = 0;
        iMask >>= 1;
    }
    fConv.i = ( (iExp << FLOAT_MANT_BITS) | iMantissa );
    if ( iSignN * iSignD < 0 )
        fConv.i |= 0x80000000;
    return fConv.f;
}
template<UInt INT_BYTES>
Rational<INT_BYTES>::operator Double() const
{
    if ( m_iNumerator == 0 )
        return 0.0;

    Int iSignN = m_iNumerator._GetSign();
    Int iSignD = m_iDenominator._GetSign();
    Integer<INT_BYTES> intAbsN(m_iNumerator);
    Integer<INT_BYTES> intAbsD(m_iDenominator);
    intAbsN._SetSign(false);
    intAbsD._SetSign(false);

    UInt iLeadBitN = intAbsN._LeadingBit();
    UInt iLeadBitD = intAbsD._LeadingBit();
    Int iPower2 = ( iLeadBitN - iLeadBitD );
    if ( iPower2 > 0 )
        intAbsD <<= iPower2;
    else if ( iPower2 < 0 )
        intAbsN <<= (-iPower2);
    if ( intAbsN < intAbsD ) {
        intAbsN <<= 1;
        --iPower2;
    }

    DoubleConverter fConv;

    // Infinite case
    if ( iPower2 > DOUBLE_EXP_BIAS ) {
        fConv.i = 0x7fefffffffffffffui64;
        if ( iSignN * iSignD < 0 )
            fConv.i |= 0x8000000000000000ui64;
        return fConv.f;
    }

    // Epsilon case
    if ( iPower2 < -(DOUBLE_EXP_BIAS + DOUBLE_MANT_BITS - 1) ) {
        fConv.i = 0ui64;
        if ( iSignN * iSignD < 0 )
            fConv.i |= 0x8000000000000000ui64;
        return fConv.f;
    }

    // Normal / Denormal cases
    UInt64 iExp, iMantissa;
    UInt iMantissaBit;
    Int iShift;
    if ( iPower2 >= -(DOUBLE_EXP_BIAS - 1) ) {
        iExp = iPower2 + DOUBLE_EXP_BIAS;
        iMantissaBit = 1;
        iShift = 0;
    } else {
        iExp = 0ui64;
        iMantissaBit = 0;
        intAbsD <<= 1;
        iShift = -(iPower2 + DOUBLE_EXP_BIAS);
    }
    iMantissa = 0ui64;
    UInt64 iMask = ( ( 1ui64 << (DOUBLE_MANT_BITS - 1) ) >> iShift );
    while( iMask > 0ui64 ) {
        if ( iMantissaBit == 1 )
            intAbsN -= intAbsD;
        intAbsN <<= 1;
        if ( intAbsN >= intAbsD ) {
            iMantissaBit = 1;
            iMantissa |= iMask;
        } else
            iMantissaBit = 0;
        iMask >>= 1;
    }
    fConv.i = ( (iExp << DOUBLE_MANT_BITS) | iMantissa );
    if ( iSignN * iSignD < 0 )
        fConv.i |= 0x8000000000000000ui64;
    return fConv.f;
}

template<UInt INT_BYTES>
inline Rational<INT_BYTES> & Rational<INT_BYTES>::operator=( const Rational<INT_BYTES> & rhs ) {
    m_iNumerator = rhs.m_iNumerator;
    m_iDenominator = rhs.m_iDenominator;
    return (*this);
}
template<UInt INT_BYTES>
inline Rational<INT_BYTES> & Rational<INT_BYTES>::operator=( const Integer<INT_BYTES> & rhs ) {
    m_iNumerator = rhs;
    m_iDenominator = 1u;
    return (*this);
}

template<UInt INT_BYTES>
inline Rational<INT_BYTES> Rational<INT_BYTES>::operator+() const { return (*this); }
template<UInt INT_BYTES>
inline Rational<INT_BYTES> Rational<INT_BYTES>::operator-() const { return Rational<INT_BYTES>(-m_iNumerator, m_iDenominator); }

template<UInt INT_BYTES>
Rational<INT_BYTES> Rational<INT_BYTES>::operator+(const Rational<INT_BYTES> & rhs) const
{
    Rational<INT_BYTES> iRes;
    iRes.m_iNumerator = ( (m_iNumerator * rhs.m_iDenominator) + (m_iDenominator * rhs.m_iNumerator) );
    iRes.m_iDenominator = ( m_iDenominator * rhs.m_iDenominator );
    iRes._FractionReduction();
    return iRes;
}
template<UInt INT_BYTES>
Rational<INT_BYTES> Rational<INT_BYTES>::operator+(const Integer<INT_BYTES> & rhs) const
{
    Rational<INT_BYTES> iRes;
    iRes.m_iNumerator = ( m_iNumerator + (m_iDenominator * rhs) );
    iRes.m_iDenominator = m_iDenominator;
    iRes._FractionReduction();
    return iRes;
}

template<UInt INT_BYTES>
Rational<INT_BYTES> Rational<INT_BYTES>::operator-(const Rational<INT_BYTES> & rhs) const
{
    Rational<INT_BYTES> iRes;
    iRes.m_iNumerator = ( (m_iNumerator * rhs.m_iDenominator) - (m_iDenominator * rhs.m_iNumerator) );
    iRes.m_iDenominator = ( m_iDenominator * rhs.m_iDenominator );
    iRes._FractionReduction();
    return iRes;
}
template<UInt INT_BYTES>
Rational<INT_BYTES> Rational<INT_BYTES>::operator-(const Integer<INT_BYTES> & rhs) const
{
    Rational<INT_BYTES> iRes;
    iRes.m_iNumerator = ( m_iNumerator - (m_iDenominator * rhs) );
    iRes.m_iDenominator = m_iDenominator;
    iRes._FractionReduction();
    return iRes;
}

template<UInt INT_BYTES>
Rational<INT_BYTES> Rational<INT_BYTES>::operator*(const Rational<INT_BYTES> & rhs) const
{
    Rational<INT_BYTES> iRes;
    iRes.m_iNumerator = ( m_iNumerator * rhs.m_iNumerator );
    iRes.m_iDenominator = ( m_iDenominator * rhs.m_iDenominator );
    iRes._FractionReduction();
    return iRes;
}
template<UInt INT_BYTES>
Rational<INT_BYTES> Rational<INT_BYTES>::operator*(const Integer<INT_BYTES> & rhs) const
{
    Rational<INT_BYTES> iRes;
    iRes.m_iNumerator = ( m_iNumerator * rhs );
    iRes.m_iDenominator = m_iDenominator;
    iRes._FractionReduction();
    return iRes;
}

template<UInt INT_BYTES>
Rational<INT_BYTES> Rational<INT_BYTES>::operator/(const Rational<INT_BYTES> & rhs) const
{
    Rational<INT_BYTES> iRes;
    iRes.m_iNumerator = ( m_iNumerator * rhs.m_iDenominator );
    iRes.m_iDenominator = ( m_iDenominator * rhs.m_iNumerator );
    iRes._FractionReduction();
    return iRes;
}
template<UInt INT_BYTES>
Rational<INT_BYTES> Rational<INT_BYTES>::operator/(const Integer<INT_BYTES> & rhs) const
{
    Rational<INT_BYTES> iRes;
    iRes.m_iNumerator = m_iNumerator;
    iRes.m_iDenominator = ( m_iDenominator * rhs );
    iRes._FractionReduction();
    return iRes;
}

template<UInt INT_BYTES>
Rational<INT_BYTES> & Rational<INT_BYTES>::operator+=(const Rational<INT_BYTES> & rhs)
{
    m_iNumerator *= rhs.m_iDenominator;
    m_iNumerator += ( m_iDenominator * rhs.m_iNumerator );
    m_iDenominator *= rhs.m_iDenominator;
    _FractionReduction();
    return (*this);
}
template<UInt INT_BYTES>
Rational<INT_BYTES> & Rational<INT_BYTES>::operator+=(const Integer<INT_BYTES> & rhs)
{
    m_iNumerator += ( m_iDenominator * rhs );
    _FractionReduction();
    return (*this);
}

template<UInt INT_BYTES>
Rational<INT_BYTES> & Rational<INT_BYTES>::operator-=(const Rational<INT_BYTES> & rhs)
{
    m_iNumerator *= rhs.m_iDenominator;
    m_iNumerator -= ( m_iDenominator * rhs.m_iNumerator );
    m_iDenominator *= rhs.m_iDenominator;
    _FractionReduction();
    return (*this);
}
template<UInt INT_BYTES>
Rational<INT_BYTES> & Rational<INT_BYTES>::operator-=(const Integer<INT_BYTES> & rhs)
{
    m_iNumerator -= ( m_iDenominator * rhs );
    _FractionReduction();
    return (*this);
}

template<UInt INT_BYTES>
Rational<INT_BYTES> & Rational<INT_BYTES>::operator*=(const Rational<INT_BYTES> & rhs)
{
    m_iNumerator *= rhs.m_iNumerator;
    m_iDenominator *= rhs.m_iDenominator;
    _FractionReduction();
    return (*this);
}
template<UInt INT_BYTES>
Rational<INT_BYTES> & Rational<INT_BYTES>::operator*=(const Integer<INT_BYTES> & rhs)
{
    m_iNumerator *= rhs;
    _FractionReduction();
    return (*this);
}

template<UInt INT_BYTES>
Rational<INT_BYTES> & Rational<INT_BYTES>::operator/=(const Rational<INT_BYTES> & rhs)
{
    m_iNumerator *= rhs.m_iDenominator;
    m_iDenominator *= rhs.m_iNumerator;
    _FractionReduction();
    return (*this);
}
template<UInt INT_BYTES>
Rational<INT_BYTES> & Rational<INT_BYTES>::operator/=(const Integer<INT_BYTES> & rhs)
{
    m_iDenominator *= rhs;
    _FractionReduction();
    return (*this);
}

template<UInt INT_BYTES>
inline Bool Rational<INT_BYTES>::operator==(const Rational<INT_BYTES> & rhs) const {
    return ( m_iNumerator * rhs.m_iDenominator == m_iDenominator * rhs.m_iNumerator );
}
template<UInt INT_BYTES>
inline Bool Rational<INT_BYTES>::operator==(const Integer<INT_BYTES> & rhs) const {
    return ( m_iNumerator == m_iDenominator * rhs );
}

template<UInt INT_BYTES>
inline Bool Rational<INT_BYTES>::operator!=(const Rational<INT_BYTES> & rhs) const {
    return ( m_iNumerator * rhs.m_iDenominator != m_iDenominator * rhs.m_iNumerator );
}
template<UInt INT_BYTES>
inline Bool Rational<INT_BYTES>::operator!=(const Integer<INT_BYTES> & rhs) const {
    return ( m_iNumerator != m_iDenominator * rhs );
}

template<UInt INT_BYTES>
Bool Rational<INT_BYTES>::operator<=(const Rational<INT_BYTES> & rhs) const
{
    Integer<INT_BYTES> intProd0 = m_iNumerator * rhs.m_iDenominator;
    Integer<INT_BYTES> intProd1 = m_iDenominator * rhs.m_iNumerator;
    if ( m_iDenominator > Integer<INT_BYTES>::Zero )
        return ( rhs.m_iDenominator > Integer<INT_BYTES>::Zero ) ? (intProd0 <= intProd1) : (intProd1 <= intProd0);
    else
        return ( rhs.m_iDenominator > Integer<INT_BYTES>::Zero ) ? (intProd1 <= intProd0) : (intProd0 <= intProd1);
}
template<UInt INT_BYTES>
Bool Rational<INT_BYTES>::operator<=(const Integer<INT_BYTES> & rhs) const
{
    Integer<INT_BYTES> intProd = m_iDenominator * rhs;
    if ( m_iDenominator > Integer<INT_BYTES>::Zero )
        return ( m_iNumerator <= intProd );
    else
        return ( intProd <= m_iNumerator );
}

template<UInt INT_BYTES>
Bool Rational<INT_BYTES>::operator>=(const Rational<INT_BYTES> & rhs) const
{
    Integer<INT_BYTES> intProd0 = m_iNumerator * rhs.m_iDenominator;
    Integer<INT_BYTES> intProd1 = m_iDenominator * rhs.m_iNumerator;
    if ( m_iDenominator > Integer<INT_BYTES>::Zero )
        return ( rhs.m_iDenominator > Integer<INT_BYTES>::Zero ) ? (intProd0 >= intProd1) : (intProd1 >= intProd0);
    else
        return ( rhs.m_iDenominator > Integer<INT_BYTES>::Zero ) ? (intProd1 >= intProd0) : (intProd0 >= intProd1);
}
template<UInt INT_BYTES>
Bool Rational<INT_BYTES>::operator>=(const Integer<INT_BYTES> & rhs) const
{
    Integer<INT_BYTES> intProd = m_iDenominator * rhs;
    if ( m_iDenominator > Integer<INT_BYTES>::Zero )
        return ( m_iNumerator >= intProd );
    else
        return ( intProd >= m_iNumerator );
}

template<UInt INT_BYTES>
Bool Rational<INT_BYTES>::operator<(const Rational<INT_BYTES> & rhs) const
{
    Integer<INT_BYTES> intProd0 = m_iNumerator * rhs.m_iDenominator;
    Integer<INT_BYTES> intProd1 = m_iDenominator * rhs.m_iNumerator;
    if ( m_iDenominator > Integer<INT_BYTES>::Zero )
        return ( rhs.m_iDenominator > Integer<INT_BYTES>::Zero ) ? (intProd0 < intProd1) : (intProd1 < intProd0);
    else
        return ( rhs.m_iDenominator > Integer<INT_BYTES>::Zero ) ? (intProd1 < intProd0) : (intProd0 < intProd1);
}
template<UInt INT_BYTES>
Bool Rational<INT_BYTES>::operator<(const Integer<INT_BYTES> & rhs) const
{
    Integer<INT_BYTES> intProd = m_iDenominator * rhs;
    if ( m_iDenominator > Integer<INT_BYTES>::Zero )
        return ( m_iNumerator < intProd );
    else
        return ( intProd < m_iNumerator );
}

template<UInt INT_BYTES>
Bool Rational<INT_BYTES>::operator>(const Rational<INT_BYTES> & rhs) const
{
    Integer<INT_BYTES> intProd0 = m_iNumerator * rhs.m_iDenominator;
    Integer<INT_BYTES> intProd1 = m_iDenominator * rhs.m_iNumerator;
    if ( m_iDenominator > Integer<INT_BYTES>::Zero )
        return ( rhs.m_iDenominator > Integer<INT_BYTES>::Zero ) ? (intProd0 > intProd1) : (intProd1 > intProd0);
    else
        return ( rhs.m_iDenominator > Integer<INT_BYTES>::Zero ) ? (intProd1 > intProd0) : (intProd0 > intProd1);
}
template<UInt INT_BYTES>
Bool Rational<INT_BYTES>::operator>(const Integer<INT_BYTES> & rhs) const
{
    Integer<INT_BYTES> intProd = m_iDenominator * rhs;
    if ( m_iDenominator > Integer<INT_BYTES>::Zero )
        return ( m_iNumerator > intProd );
    else
        return ( intProd > m_iNumerator );
}

template<UInt INT_BYTES>
inline Integer<INT_BYTES> & Rational<INT_BYTES>::Numerator()   { return m_iNumerator; }
template<UInt INT_BYTES>
inline Integer<INT_BYTES> & Rational<INT_BYTES>::Denominator() { return m_iDenominator; }
template<UInt INT_BYTES>
inline const Integer<INT_BYTES> & Rational<INT_BYTES>::Numerator() const   { return m_iNumerator; }
template<UInt INT_BYTES>
inline const Integer<INT_BYTES> & Rational<INT_BYTES>::Denominator() const { return m_iDenominator; }

template<UInt INT_BYTES>
inline Rational<INT_BYTES> Rational<INT_BYTES>::Abs() const {
    return ( (*this) >= Integer<INT_BYTES>::Zero ) ? (*this) : -(*this);
}

/////////////////////////////////////////////////////////////////////////////////

template<UInt INT_BYTES>
Void Rational<INT_BYTES>::_FractionReduction()
{
    if ( (m_iNumerator.m_arrDigits[0] & 1) != 0 ||
         (m_iDenominator.m_arrDigits[0] & 1) != 0 )
        return;

    UInt iTrailingDigitN = m_iNumerator._TrailingDigit();
    if ( iTrailingDigitN == INVALID_OFFSET ) {
        m_iDenominator = Integer<INT_BYTES>::One;
        return;
    }

    UInt iTrailingDigitD = m_iDenominator._TrailingDigit();
    Assert( iTrailingDigitD != INVALID_OFFSET );
    UInt iMinTrailing = Min<UInt>( iTrailingDigitN, iTrailingDigitD );
    UInt iBitN = m_iNumerator._TrailingBit( iTrailingDigitN );
    UInt iBitD = m_iDenominator._TrailingBit( iTrailingDigitD );
    UInt iMinBit = Min<UInt>( iBitN, iBitD );
    UInt iShift = ( iMinTrailing << 4 ) + iMinBit;

    m_iNumerator >>= iShift;
    m_iDenominator >>= iShift;
}

