/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Formats/Integer.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Integer provides exact integer arithmetic support
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Integer implementation
template<UInt INT_BYTES>
const Integer<INT_BYTES> Integer<INT_BYTES>::Zero = Integer<INT_BYTES>(0ul);
template<UInt INT_BYTES>
const Integer<INT_BYTES> Integer<INT_BYTES>::One = Integer<INT_BYTES>(1ul);
template<UInt INT_BYTES>
const Integer<INT_BYTES> Integer<INT_BYTES>::Infinity = Integer<INT_BYTES>(FLOAT_NAN);

template<UInt INT_BYTES>
Integer<INT_BYTES>::Integer()
{
    Assert( INT_BYTES >= 8 );
    // nothing to do
}
template<UInt INT_BYTES>
Integer<INT_BYTES>::Integer(const Integer<INT_BYTES> & rhs)
{
    Assert( INT_BYTES >= 8 );
    for( UInt i = 0; i < INT_DIGIT_COUNT; ++i )
        m_arrDigits[i] = rhs.m_arrDigits[i];
}
template<UInt INT_BYTES>
Integer<INT_BYTES>::Integer(UChar iValue)
{
    Assert( INT_BYTES >= 8 );
    MemZero( m_arrDigits, INT_BYTES );
    m_arrDigits[0] = (UShort)( iValue );
}
template<UInt INT_BYTES>
Integer<INT_BYTES>::Integer(SChar iValue)
{
    Assert( INT_BYTES >= 8 );
    MemZero( m_arrDigits, INT_BYTES );
    m_arrDigits[0] = (UShort)( iValue & 0x7f );
    if ( iValue < 0 )
        m_arrDigits[INT_DIGIT_LAST] |= 0x8000;
}
template<UInt INT_BYTES>
Integer<INT_BYTES>::Integer(UShort iValue)
{
    Assert( INT_BYTES >= 8 );
    MemZero( m_arrDigits, INT_BYTES );
    m_arrDigits[0] = iValue;
}
template<UInt INT_BYTES>
Integer<INT_BYTES>::Integer(SShort iValue)
{
    Assert( INT_BYTES >= 8 );
    MemZero( m_arrDigits, INT_BYTES );
    m_arrDigits[0] = (UShort)( iValue & 0x7fff );
    if ( iValue < 0 )
        m_arrDigits[INT_DIGIT_LAST] |= 0x8000;
}
template<UInt INT_BYTES>
Integer<INT_BYTES>::Integer(ULong iValue)
{
    Assert( INT_BYTES >= 8 );
    MemZero( m_arrDigits, INT_BYTES );
    m_arrDigits[0] = (UShort)( iValue & 0x0000ffff );
    m_arrDigits[1] = (UShort)( (iValue & 0xffff0000) >> 16 );
}
template<UInt INT_BYTES>
Integer<INT_BYTES>::Integer(SLong iValue)
{
    Assert( INT_BYTES >= 8 );
    MemZero( m_arrDigits, INT_BYTES );
    m_arrDigits[0] = (UShort)( iValue & 0x0000ffff );
    m_arrDigits[1] = (UShort)( (iValue & 0x7fff0000) >> 16 );
    if ( iValue < 0 )
        m_arrDigits[INT_DIGIT_LAST] |= 0x8000;
}
template<UInt INT_BYTES>
Integer<INT_BYTES>::Integer(UInt iValue)
{
    Assert( INT_BYTES >= 8 );
    MemZero( m_arrDigits, INT_BYTES );
    m_arrDigits[0] = (UShort)( iValue & 0x0000ffff );
    m_arrDigits[1] = (UShort)( (iValue & 0xffff0000) >> 16 );
}
template<UInt INT_BYTES>
Integer<INT_BYTES>::Integer(SInt iValue)
{
    Assert( INT_BYTES >= 8 );
    MemZero( m_arrDigits, INT_BYTES );
    m_arrDigits[0] = (UShort)( iValue & 0x0000ffff );
    m_arrDigits[1] = (UShort)( (iValue & 0x7fff0000) >> 16 );
    if ( iValue < 0 )
        m_arrDigits[INT_DIGIT_LAST] |= 0x8000;
}
template<UInt INT_BYTES>
Integer<INT_BYTES>::Integer(UInt64 iValue)
{
    Assert( INT_BYTES >= 8 );
    MemZero( m_arrDigits, INT_BYTES );
    m_arrDigits[0] = (UShort)( iValue & 0x000000000000ffffi64 );
    m_arrDigits[1] = (UShort)( (iValue & 0x00000000ffff0000i64) >> 16 );
    m_arrDigits[2] = (UShort)( (iValue & 0x0000ffff00000000i64) >> 32 );
    m_arrDigits[3] = (UShort)( (iValue & 0xffff000000000000i64) >> 48 );
}
template<UInt INT_BYTES>
Integer<INT_BYTES>::Integer(SInt64 iValue)
{
    Assert( INT_BYTES >= 8 );
    MemZero( m_arrDigits, INT_BYTES );
    m_arrDigits[0] = (UShort)( iValue & 0x000000000000ffffi64 );
    m_arrDigits[1] = (UShort)( (iValue & 0x00000000ffff0000i64) >> 16 );
    m_arrDigits[2] = (UShort)( (iValue & 0x0000ffff00000000i64) >> 32 );
    m_arrDigits[3] = (UShort)( (iValue & 0x7fff000000000000i64) >> 48 );
    if ( iValue < 0 )
        m_arrDigits[INT_DIGIT_LAST] |= 0x8000;
}
template<UInt INT_BYTES>
Integer<INT_BYTES>::Integer(Float fValue)
{
    Assert( INT_BYTES >= 8 );

    if ( fValue == 0.0f ) {
        MemZero( m_arrDigits, INT_BYTES );
        return;
    }

    FloatConverter fConv;
    fConv.f = fValue;
    UInt iSign = ( fConv.i & FLOAT_SIGN_MASK );
    Int iExp = ( (fConv.i & FLOAT_EXP_MASK) >> FLOAT_MANT_BITS );
    UInt iMant = ( fConv.i & FLOAT_MANT_MASK );

    // Inf / NaN case, treat as +-inf
    if ( iExp == 255 ) {
        MemSet( m_arrDigits, 0xff, INT_BYTES - sizeof(Short) );
        if ( iSign != 0 )
            m_arrDigits[INT_DIGIT_LAST] = 0xffff;
        else
            m_arrDigits[INT_DIGIT_LAST] = 0x7fff;
        return;
    }

    // Denormalized case, | (-1)^s * 2^(-126) * 0.m | < 1 => treat as +0
    if ( iExp == 0 ) {
        MemZero( m_arrDigits, INT_BYTES );
        return;
    }

    // Normalized case, (-1)^s * 2^(e-127) * 1.m = (-1)^s * 2^(e-127) * (2^23 + m) * 2^-23
    //                                           = (-1)^s * 2^(e-150) * (2^23 + m)
    UInt iValue = ( (1 << FLOAT_MANT_BITS) + iMant );
    Int iPower2 = ( iExp - 150 );
    if ( iPower2 < -FLOAT_MANT_BITS ) {
        MemZero( m_arrDigits, INT_BYTES );
        return;
    }
    while( iPower2 < 0 || (iValue & 1) == 0 ) {
        iValue >>= 1;
        ++iPower2;
    }

    if ( iPower2 == 0 ) {
        MemZero( m_arrDigits, INT_BYTES );
        m_arrDigits[0] = (UShort)( iValue & 0x0000ffff );
        m_arrDigits[1] = (UShort)( (iValue & 0xffff0000) >> 16 );
        if ( iSign != 0 )
            m_arrDigits[INT_DIGIT_LAST] |= 0x8000;
        return;
    }

    // ( iPower2 > 0 )
    UInt iDigit = ( iPower2 >> 4 ); // /16
    Assert( iDigit + 2 <= INT_DIGIT_LAST );
    iPower2 &= 0x0000000f; // %16
    UInt64 iBigValue = iValue;
    iBigValue <<= iPower2; // max 24 + 15 = 39 bits
    MemZero( m_arrDigits, INT_BYTES );
    m_arrDigits[iDigit]     = (UShort)( iBigValue & 0x000000000000ffffi64 );
    m_arrDigits[iDigit + 1] = (UShort)( (iBigValue & 0x00000000ffff0000i64) >> 16 );
    m_arrDigits[iDigit + 2] = (UShort)( (iBigValue & 0x0000007f00000000i64) >> 32 );
    if ( iSign != 0 )
        m_arrDigits[INT_DIGIT_LAST] |= 0x8000;
}
template<UInt INT_BYTES>
Integer<INT_BYTES>::Integer(Double fValue)
{
    Assert( INT_BYTES >= 8 );

    if ( fValue == 0.0 ) {
        MemZero( m_arrDigits, INT_BYTES );
        return;
    }

    DoubleConverter fConv;
    fConv.f = fValue;
    UInt64 iSign = ( fConv.i & DOUBLE_SIGN_MASK );
    Int64 iExp = ( (fConv.i & DOUBLE_EXP_MASK) >> DOUBLE_MANT_BITS );
    UInt64 iMant = ( fConv.i & DOUBLE_MANT_MASK );

    // Inf / NaN case, treat as +-inf
    if ( iExp == 2047 ) {
        MemSet( m_arrDigits, 0xff, INT_BYTES - sizeof(Short) );
        if ( iSign != 0 )
            m_arrDigits[INT_DIGIT_LAST] = 0xffff;
        else
            m_arrDigits[INT_DIGIT_LAST] = 0x7fff;
        return;
    }

    // Denormalized case, | (-1)^s * 2^(-1022) * 0.m | < 1 => treat as +0
    if ( iExp == 0 ) {
        MemZero( m_arrDigits, INT_BYTES );
        return;
    }

    // Normalized case, (-1)^s * 2^(e-1023) * 1.m = (-1)^s * 2^(e-1023) * (2^52 + m) * 2^-52
    //                                            = (-1)^s * 2^(e-1075) * (2^52 + m)
    UInt64 iValue = ( (1ui64 << DOUBLE_MANT_BITS) + iMant );
    Int iPower2 = ( iExp - 1075 );
    if ( iPower2 < -DOUBLE_MANT_BITS ) {
        MemZero( m_arrDigits, INT_BYTES );
        return;
    }
    while( iPower2 < 0 || (iValue & 1) == 0 ) {
        iValue >>= 1;
        ++iPower2;
    }

    if ( iPower2 == 0 ) {
        MemZero( m_arrDigits, INT_BYTES );
        m_arrDigits[0] = (UShort)( iValue & 0x000000000000ffffi64 );
        m_arrDigits[1] = (UShort)( (iValue & 0x00000000ffff0000i64) >> 16 );
        m_arrDigits[2] = (UShort)( (iValue & 0x0000ffff00000000i64) >> 32 );
        m_arrDigits[3] = (UShort)( (iValue & 0xffff000000000000i64) >> 48 );
        if ( iSign != 0 )
            m_arrDigits[INT_DIGIT_LAST] |= 0x8000;
        return;
    }

    // ( iPower2 > 0 )
    UInt iDigit = ( iPower2 >> 4 ); // /16
    Assert( iDigit + 4 <= INT_DIGIT_LAST );
    iPower2 &= 0x0000000f; // %16
    UInt iOverlapBits = (32 - iPower2);
    UInt64 iOverlapMask = ( 1ui64 << iOverlapBits ) - 1;
    UInt64 iValueLow = ( iValue & 0x00000000ffffffffi64 ) << iPower2;
    UInt64 iValueHigh = ( iValue & 0xffffffff00000000i64 ) >> 32;
    iValueLow |= ( (iValueHigh & iOverlapMask) << (32 + iPower2) )
    iValueHigh >>= iOverlapBits; // max 53 + 15 - 64 = 4 bits
    MemZero( m_arrDigits, INT_BYTES );
    m_arrDigits[iDigit]     = (UShort)( iValueLow & 0x000000000000ffffi64 );
    m_arrDigits[iDigit + 1] = (UShort)( (iValueLow & 0x00000000ffff0000i64) >> 16 );
    m_arrDigits[iDigit + 2] = (UShort)( (iValueLow & 0x0000ffff00000000i64) >> 32 );
    m_arrDigits[iDigit + 3] = (UShort)( (iValueLow & 0xffff000000000000i64) >> 48 );
    m_arrDigits[iDigit + 4] = (UShort)( iValueHigh & 0x000000000000000fi64 );
    if ( iSign != 0 )
        m_arrDigits[INT_DIGIT_LAST] |= 0x8000;
}
template<UInt INT_BYTES>
Integer<INT_BYTES>::~Integer()
{
    // nothing to do
}

template<UInt INT_BYTES>
inline Integer<INT_BYTES>::operator UChar() const {
    return (UChar)( m_arrDigits[0] & 0x00ff );
}
template<UInt INT_BYTES>
inline Integer<INT_BYTES>::operator SChar() const {
    SChar iRes = (SChar)( m_arrDigits[0] & 0x007f );
    if ( m_arrDigits[INT_DIGIT_LAST] & 0x8000 != 0 )
        iRes |= 0x80;
    return iRes;
}
template<UInt INT_BYTES>
inline Integer<INT_BYTES>::operator UShort() const {
    return m_arrDigits[0];
}
template<UInt INT_BYTES>
inline Integer<INT_BYTES>::operator SShort() const {
    SShort iRes = (SShort)( m_arrDigits[0] & 0x7fff );
    if ( m_arrDigits[INT_DIGIT_LAST] & 0x8000 != 0 )
        iRes |= 0x8000;
    return iRes;
}
template<UInt INT_BYTES>
inline Integer<INT_BYTES>::operator ULong() const {
    return ( ( ((ULong)m_arrDigits[1]) << 16 ) | (ULong)m_arrDigits[0] );
}
template<UInt INT_BYTES>
inline Integer<INT_BYTES>::operator SLong() const {
    SLong iRes = ( (SLong)(m_arrDigits[1] & 0x7fff) ) << 16;
    iRes |= (SLong)m_arrDigits[0];
    if ( m_arrDigits[INT_DIGIT_LAST] & 0x8000 != 0 )
        iRes |= 0x80000000;
    return iRes;
}
template<UInt INT_BYTES>
inline Integer<INT_BYTES>::operator UInt() const {
    return ( ( ((UInt)m_arrDigits[1]) << 16 ) | (UInt)m_arrDigits[0] );
}
template<UInt INT_BYTES>
inline Integer<INT_BYTES>::operator SInt() const {
    SInt iRes = ( (SInt)(m_arrDigits[1] & 0x7fff) ) << 16;
    iRes |= (SInt)m_arrDigits[0];
    if ( m_arrDigits[INT_DIGIT_LAST] & 0x8000 != 0 )
        iRes |= 0x80000000;
    return iRes;
}
template<UInt INT_BYTES>
inline Integer<INT_BYTES>::operator UInt64() const {
    return ( ( ((UInt64)m_arrDigits[3]) << 48 ) | ( ((UInt64)m_arrDigits[2]) << 32 ) |
             ( ((UInt64)m_arrDigits[1]) << 16 ) | (UInt64)m_arrDigits[0] );
}
template<UInt INT_BYTES>
inline Integer<INT_BYTES>::operator SInt64() const {
    SInt64 iRes = ( (SInt64)(m_arrDigits[3] & 0x7fff) ) << 48;
    iRes |= ( (SInt64)m_arrDigits[2] ) << 32;
    iRes |= ( (SInt64)m_arrDigits[1] ) << 16;
    iRes |= (SInt64)m_arrDigits[0];
    if ( m_arrDigits[INT_DIGIT_LAST] & 0x8000 != 0 )
        iRes |= 0x8000000000000000i64;
    return iRes;
}
template<UInt INT_BYTES>
Integer<INT_BYTES>::operator Float() const
{
    Float fRes = 0.0f, fBasis = 65536.0f;
    UInt iLead = _LeadingDigit();
    if ( iLead == INVALID_OFFSET )
        return 0.0f;
    Int iDigit = (Int)iLead;
    while( iDigit >= 0 ) {
        fRes *= fBasis;
        fRes += (Float)( _GetDigit(iDigit) );
        --iDigit;
    }
    if ( m_arrDigits[INT_DIGIT_LAST] & 0x8000 != 0 )
        fRes = -fRes;
    return fRes;
}
template<UInt INT_BYTES>
Integer<INT_BYTES>::operator Double() const
{
    Double fRes = 0.0, fBasis = 65536.0;
    UInt iLead = _LeadingDigit();
    if ( iLead == INVALID_OFFSET )
        return 0.0;
    Int iDigit = (Int)iLead;
    while( iDigit >= 0 ) {
        fRes *= fBasis;
        fRes += (Double)( _GetDigit(iDigit) );
        --iDigit;
    }
    if ( m_arrDigits[INT_DIGIT_LAST] & 0x8000 != 0 )
        fRes = -fRes;
    return fRes;
}

template<UInt INT_BYTES>
Integer<INT_BYTES> & Integer<INT_BYTES>::operator=(const Integer<INT_BYTES> & rhs)
{
    for( UInt i = 0; i < INT_DIGIT_COUNT; ++i )
        m_arrDigits[i] = rhs.m_arrDigits[i];
    return (*this);
}

template<UInt INT_BYTES>
Integer<INT_BYTES> Integer<INT_BYTES>::operator>>(const Int rhs) const
{
    if ( rhs == 0 )
        return (*this);
    if ( rhs < 0 )
        return operator<<(-rhs);

    // Digits to shift
    UInt iDigitShift = ( rhs >> 4 ); // /16
    if ( iDigitShift > INT_DIGIT_LAST )
        return Integer<INT_BYTES>(0u);

    // Digit shift
    Integer<INT_BYTES> intRes(0u);
    UInt i, j;
    if ( iDigitShift > 0 ) {
        i = 0;
        for( j = iDigitShift; j <= INT_DIGIT_LAST; ++j )
            intRes._SetDigit( i++, _GetDigit(j) );
    }

    // Remaining bitshifts
    UInt iBitShift = ( rhs & 0x0000000f ); // %16
    UInt iValue;
    if ( iBitShift > 0 ) {
        i = 0;
        for( j = i + 1; j <= INT_DIGIT_LAST; ++j ) {
            iValue = _GetDigit( i, j );
            iValue >>= iBitShift;
            intRes._SetDigit( i, iValue );
            ++i;
        }
        iValue = _GetDigit( INT_DIGIT_LAST );
        iValue >>= iBitShift;
        intRes._SetDigit( INT_DIGIT_LAST, iValue );
    }

    // Transmit Sign
    if ( m_arrDigits[INT_DIGIT_LAST] & 0x8000 != 0 )
        intRes.m_arrDigits[INT_DIGIT_LAST] |= 0x8000;

    return intRes;
}
template<UInt INT_BYTES>
Integer<INT_BYTES> Integer<INT_BYTES>::operator<<(const Int rhs) const
{
    if ( rhs == 0 )
        return (*this);
    if ( rhs < 0 )
        return operator>>(-rhs);

    // Digits to shift
    UInt iDigitShift = ( rhs >> 4 ); // /16
    if ( iDigitShift > INT_DIGIT_LAST )
        return Integer<INT_BYTES>(0u);

    // Digit shift
    Integer<INT_BYTES> intRes(0u);
    Int i, j;
    if ( iDigitShift > 0 ) {
        i = INT_DIGIT_LAST;
        for( j = INT_DIGIT_LAST - iDigitShift; j >= 0; --j )
            intRes._SetDigit( i--, _GetDigit(j) );
    }

    // Remaining bitshifts
    UInt iBitShift = ( rhs & 0x0000000f ); // %16
    UInt iValue;
    if ( iBitShift > 0 ) {
        i = INT_DIGIT_LAST;
        for( j = i - 1; j >= 0; --j ) {
            iValue = _GetDigit( j, i );
            iValue <<= iBitShift;
            intRes._SetDigit( i, (iValue & 0xffff0000) >> 16 );
            --i;
        }
        iValue = _GetDigit( 0 );
        iValue <<= iBitShift;
        intRes._SetDigit( 0, iValue );
    }

    // Transmit Sign
    if ( m_arrDigits[INT_DIGIT_LAST] & 0x8000 != 0 )
        intRes.m_arrDigits[INT_DIGIT_LAST] |= 0x8000;

    return intRes;
}
template<UInt INT_BYTES>
Integer<INT_BYTES> & Integer<INT_BYTES>::operator>>=(const Int rhs)
{
    if ( rhs == 0 )
        return (*this);
    if ( rhs < 0 )
        return operator<<=(-rhs);

    // Digits to shift
    UInt iDigitShift = ( rhs >> 4 ); // /16
    if ( iDigitShift > INT_DIGIT_LAST ) {
        MemZero( m_arrDigits, INT_BYTES );
        return (*this);
    }

    // Digit shift
    UInt i, j;
    if ( iDigitShift > 0 ) {
        i = 0;
        for( j = iDigitShift; j <= INT_DIGIT_LAST; ++j ) {
            _SetDigit( i++, _GetDigit(j) );
            _SetDigit( j, 0 );
        }
    }

    // Remaining bitshifts
    UInt iBitShift = ( rhs & 0x0000000f ); // %16
    UInt iValue;
    if ( iBitShift > 0 ) {
        i = 0;
        for( j = i + 1; j <= INT_DIGIT_LAST; ++j ) {
            iValue = _GetDigit( i, j );
            iValue >>= iBitShift;
            _SetDigit( i, iValue );
            ++i;
        }
        iValue = _GetDigit( INT_DIGIT_LAST );
        iValue >>= iBitShift;
        _SetDigit( INT_DIGIT_LAST, iValue );
    }

    return (*this);
}
template<UInt INT_BYTES>
Integer<INT_BYTES> & Integer<INT_BYTES>::operator<<=(const Int rhs)
{
    if ( rhs == 0 )
        return (*this);
    if ( rhs < 0 )
        return operator>>=(-rhs);

    // Digits to shift
    UInt iDigitShift = ( rhs >> 4 ); // /16
    if ( iDigitShift > INT_DIGIT_LAST ) {
        MemZero( m_arrDigits, INT_BYTES );
        return (*this);
    }

    // Digit shift
    Int i, j;
    if ( iDigitShift > 0 ) {
        i = INT_DIGIT_LAST;
        for( j = INT_DIGIT_LAST - iDigitShift; j >= 0; --j ) {
            _SetDigit( i--, _GetDigit(j) );
            _SetDigit( j, 0 );
        }
    }

    // Remaining bitshifts
    UInt iBitShift = ( rhs & 0x0000000f ); // %16
    UInt iValue;
    if ( iBitShift > 0 ) {
        i = INT_DIGIT_LAST;
        for( j = i - 1; j >= 0; --j ) {
            iValue = _GetDigit( j, i );
            iValue <<= iBitShift;
            _SetDigit( i, (iValue & 0xffff0000) >> 16 );
            --i;
        }
        iValue = _GetDigit( 0 );
        iValue <<= iBitShift;
        _SetDigit( 0, (iValue & 0x0000ffff) );
    }

    return (*this);
}

template<UInt INT_BYTES>
inline Integer<INT_BYTES> Integer<INT_BYTES>::operator+() const { return (*this); }
template<UInt INT_BYTES>
inline Integer<INT_BYTES> Integer<INT_BYTES>::operator-() const {
    Integer<INT_BYTES> intRes(*this);
    if ( ( intRes.m_arrDigits[INT_DIGIT_LAST] & 0x8000 ) != 0 )
        intRes.m_arrDigits[INT_DIGIT_LAST] &= 0x7fff;
    else
        intRes.m_arrDigits[INT_DIGIT_LAST] |= 0x8000;
    return intRes;
}

template<UInt INT_BYTES>
Integer<INT_BYTES> Integer<INT_BYTES>::operator+(const Integer<INT_BYTES> & rhs) const
{
    Int iSign = _GetSign();
    if ( iSign != rhs._GetSign() )
        return (iSign > 0) ? operator-(-rhs) : rhs.operator-(-(*this));

    Integer intRes(0u);

    UInt iSum, iCarry = 0;
    for( UInt i = 0; i < INT_DIGIT_LAST; ++i ) {
        iSum = _GetDigit(i) + rhs._GetDigit(i) + iCarry;
        intRes._SetDigit( i, iSum );
        iCarry = ( (iSum & 0x00010000) >> 16 );
    }
    iSum = _GetDigit(INT_DIGIT_LAST) + rhs._GetDigit(INT_DIGIT_LAST) + iCarry;
    intRes._SetDigit( INT_DIGIT_LAST, iSum );
    iCarry = ( (iSum & 0x00008000) >> 16 );
    Assert( iCarry == 0 ); // Overflow

    intRes._SetSign( iSign < 0 );

    return intRes;
}
template<UInt INT_BYTES>
Integer<INT_BYTES> Integer<INT_BYTES>::operator-(const Integer<INT_BYTES> & rhs) const
{
    if ( rhs._GetSign() < 0 )
        return operator+(-rhs);
    if ( _GetSign() < 0 )
        return -( rhs.operator+(-(*this)) );
    if ( (*this) < rhs )
        return -( rhs.operator-(*this) );

    Integer intRes(0u);

    Int iDiff, iCarry = 0;
    for( UInt i = 0; i <= INT_DIGIT_LAST; ++i ) {
        iDiff = (Int)(_GetDigit(i)) - ( (Int)(rhs._GetDigit(i)) + iCarry );
        iCarry = 0;
        if ( iDiff < 0 ) {
            iDiff += 0x00010000;
            iCarry = 1;
        }
        intRes._SetDigit( i, iDiff );
    }
    Assert( iCarry == 0 ); // Overflow

    return intRes;
}
template<UInt INT_BYTES>
Integer<INT_BYTES> Integer<INT_BYTES>::operator*(const Integer<INT_BYTES> & rhs) const
{
    Int iSignL = _GetSign();
    Int iSignR = rhs._GetSign();
    Integer<INT_BYTES> intL(*this);
    Integer<INT_BYTES> intR(rhs);
    intL._SetSign(false);
    intR._SetSign(false);

    UShort arrProduct[INT_DIGIT_COUNT << 1];
    UShort * pProduct = arrProduct;
    UShort arrResult[INT_DIGIT_COUNT << 1];
    UShort * pRes = arrResult;
    MemZero( arrResult, INT_BYTES << 1 );

    UInt iL, iR, iValL, iValR, iProd, iCarry;
    UInt iSum, iTerm0, iTerm1;
    UShort *pBufferP, *pBufferR;
    UInt iSize = ( INT_DIGIT_COUNT << 1 );
    for( iL = 0; iL < INT_DIGIT_COUNT; ++iL ) {
        iValL = intL._GetDigit(iL);
        if ( iValL > 0 ) {
            pBufferP = pProduct;
            iCarry = 0;
            for( iR = 0; iR < INT_DIGIT_COUNT; ++iR ) {
                iValR = intR._GetDigit(iR);
                iProd = ( iValL * iValR ) + iCarry;
                *pBufferP++ = (UShort)( iProd & 0x0000ffff );
                iCarry = ( iProd & 0xffff0000 ) >> 16;
            }
            *pBufferP = (UShort)( iCarry & 0x0000ffff );

            pBufferP = pProduct;
            pBufferR = pRes;
            iCarry = 0;
            for( iR = 0; iR <= INT_DIGIT_COUNT; ++iR ) {
                iTerm0 = (UInt)( *pBufferP++ );
                iTerm1 = (UInt)( *pBufferR );
                iSum = iTerm0 + iTerm1 + iCarry;
                *pBufferR++ = (UShort)( iSum & 0x0000ffff );
                iCarry = ( iSum & 0x00010000 ) >> 16;
            }
            for( /**/; iR < iSize && iCarry > 0; ++iR ) {
                iTerm0 = (UInt)( *pBufferR );
                iSum = iTerm0 + iCarry;
                *pBufferR++ = (UShort)( iSum & 0x0000ffff );
                iCarry = ( iSum & 0x00010000 ) >> 16;
            }
        }
        ++pProduct;
        ++pRes;
        --iSize;
    }

    // Overflow
    for( iL = (INT_DIGIT_COUNT << 1) - 1; iL >= INT_DIGIT_COUNT; --iL )
        Assert( arrResult[iL] == 0 );
    Assert( ( arrResult[INT_DIGIT_LAST] & 0x8000 ) == 0 );

    // Result
    Integer<INT_BYTES> intRes;
    for( iL = 0; iL <= INT_DIGIT_LAST; ++iL )
        intRes.m_arrDigits[iL] = arrResult[iL];
    intRes._SetSign( (iSignL * iSignR) < 0 );
    return intRes;
}
template<UInt INT_BYTES>
Integer<INT_BYTES> Integer<INT_BYTES>::operator/(const Integer<INT_BYTES> & rhs) const
{
    Integer intQ, intR;
    Bool bOk = _DivMod( (*this), rhs, intQ, intR );
    if ( !bOk ) {
        // Div by 0 => signed infinity
        MemSet( intQ.m_arrDigits, 0xff, INT_BYTES );
        intQ._SetSign( _GetSign() * rhs._GetSign() < 0 );
    }
    return intQ;
}
template<UInt INT_BYTES>
Integer<INT_BYTES> Integer<INT_BYTES>::operator%(const Integer<INT_BYTES> & rhs) const
{
    Integer intQ, intR;
    Bool bOk = _DivMod( (*this), rhs, intQ, intR );
    if ( !bOk ) {
        // Div by 0 => signed infinity (Never use numerator, may seem valid but is NOT !)
        MemSet( intR.m_arrDigits, 0xff, INT_BYTES );
        intR._SetSign( _GetSign() * rhs._GetSign() < 0 );
    }
    return intR;
}

template<UInt INT_BYTES>
inline Integer<INT_BYTES> & Integer<INT_BYTES>::operator+=(const Integer<INT_BYTES> & rhs) { *this = (*this) + rhs; return (*this); }
template<UInt INT_BYTES>
inline Integer<INT_BYTES> & Integer<INT_BYTES>::operator-=(const Integer<INT_BYTES> & rhs) { *this = (*this) - rhs; return (*this); }
template<UInt INT_BYTES>
inline Integer<INT_BYTES> & Integer<INT_BYTES>::operator*=(const Integer<INT_BYTES> & rhs) { *this = (*this) * rhs; return (*this); }
template<UInt INT_BYTES>
inline Integer<INT_BYTES> & Integer<INT_BYTES>::operator/=(const Integer<INT_BYTES> & rhs) { *this = (*this) / rhs; return (*this); }
template<UInt INT_BYTES>
inline Integer<INT_BYTES> & Integer<INT_BYTES>::operator%=(const Integer<INT_BYTES> & rhs) { *this = (*this) % rhs; return (*this); }

template<UInt INT_BYTES>
inline Bool Integer<INT_BYTES>::operator==(const Integer<INT_BYTES> & rhs) const { return ( _Compare(*this, rhs) == 0 ); }
template<UInt INT_BYTES>
inline Bool Integer<INT_BYTES>::operator!=(const Integer<INT_BYTES> & rhs) const { return ( _Compare(*this, rhs) != 0 ); }
template<UInt INT_BYTES>
inline Bool Integer<INT_BYTES>::operator<=(const Integer<INT_BYTES> & rhs) const { return ( _Compare(*this, rhs) <= 0 ); }
template<UInt INT_BYTES>
inline Bool Integer<INT_BYTES>::operator>=(const Integer<INT_BYTES> & rhs) const { return ( _Compare(*this, rhs) >= 0 ); }
template<UInt INT_BYTES>
inline Bool Integer<INT_BYTES>::operator<(const Integer<INT_BYTES> & rhs) const { return ( _Compare(*this, rhs) < 0 ); }
template<UInt INT_BYTES>
inline Bool Integer<INT_BYTES>::operator>(const Integer<INT_BYTES> & rhs) const { return ( _Compare(*this, rhs) > 0 ); }

/////////////////////////////////////////////////////////////////////////////////

template<UInt INT_BYTES>
Int Integer<INT_BYTES>::_Compare( const Integer<INT_BYTES> & lhs, const Integer<INT_BYTES> & rhs )
{
    Int iSign = lhs._GetSign();
    if ( iSign != rhs._GetSign() )
        return (iSign > 0) ? +1 : -1;

    UInt iValL, iValR;
    for( Int i = INT_DIGIT_LAST; i >= 0; --i ) {
        iValL = lhs._GetDigit(i);
        iValR = rhs._GetDigit(i);
        if ( iValL > iValR )
            return +iSign;
        if ( iValR > iValL )
            return -iSign;
    }
    return 0;
}

template<UInt INT_BYTES>
Bool Integer<INT_BYTES>::_DivMod( const Integer<INT_BYTES> & iNumerator, const Integer<INT_BYTES> & iDenominator,
                       Integer<INT_BYTES> & outQuotient, Integer<INT_BYTES> & outRemainder )
{
    if ( iDenominator == 0 ) {
        outQuotient = 0;
        outRemainder = 0;
        Assert(false);
        return false;
    }
    if ( iNumerator == 0 ) {
        outQuotient = 0;
        outRemainder = 0;
        return true;
    }

    Int iSignN = iNumerator._GetSign();
    Int iSignD = iDenominator._GetSign();
    Integer<INT_BYTES> absN = iNumerator;
    Integer<INT_BYTES> absD = iDenominator;
    absN._SetSign(false);
    absD._SetSign(false);

    Int iCmp = _Compare( absN, absD );
    if ( iCmp < 0 ) {
        outQuotient = 0;
        outRemainder = iNumerator;
        return true;
    }
    if ( iCmp == 0 ) {
        outQuotient = 1;
        outRemainder = 0;
        return true;
    }

    // Divide
    if ( absD > 0x0000ffff )
        _DivMultiple( absN, absD, outQuotient, outRemainder );
    else
        _DivSingle( absN, absD._GetDigit(0), outQuotient, outRemainder );

    // Signs
    outQuotient._SetSign( iSignN*iSignD < 0 );
    outRemainder._SetSign( iSignN < 0 );

    return true;
}
template<UInt INT_BYTES>
Void Integer<INT_BYTES>::_DivSingle( const Integer<INT_BYTES> & iNumerator, UInt iDenominator,
                          Integer<INT_BYTES> & outQuotient, Integer<INT_BYTES> & outRemainder )
{
    // Numerator
    UInt iLeadN = iNumerator._LeadingDigit();
    const UShort * pBlockN = ( iNumerator.m_arrDigits + iLeadN );

    // Quotient
    outQuotient = 0;
    UShort * pBlockQ = ( outQuotient.m_arrDigits + iLeadN );

    // Divide
    UInt iDigitR = 0, iDigitN, iN, iQ;
    UInt iLast = INVALID_OFFSET;
    for( Int i = (Int)iLeadN; i >= 0; --i ) {
        iDigitN = ( (UInt)(*pBlockN) & 0x0000ffff ); // Pick next digit
        iN = ( (iDigitR << 16) | iDigitN ); // Add remainder digit
        iQ = iN / iDenominator; // step quotient
        iDigitR = iN - (iQ * iDenominator); // step remainder
        *pBlockQ = (UShort)( iQ & 0x0000ffff );
        if ( iLast == INVALID_OFFSET && iQ > 0 ) {
            iLast = (UInt)i;
        }
        --pBlockN;
        --pBlockQ;
    }
    Assert( iLast != INVALID_OFFSET );

    // Remainder
    outRemainder = 0;
    outRemainder.m_arrDigits[0] = (UShort)( iDigitR & 0x0000ffff );
    outRemainder.m_arrDigits[1] = (UShort)( (iDigitR & 0xffff0000) >> 16 );
}
template<UInt INT_BYTES>
Void Integer<INT_BYTES>::_DivMultiple( const Integer<INT_BYTES> & iNumerator, const Integer<INT_BYTES> & iDenominator,
                                       Integer<INT_BYTES> & outQuotient, Integer<INT_BYTES> & outRemainder )
{
    // Normalization can cause overflow when computing the product adjust * numerator,
    // an assertion is triggered to indicate a bigger byte size is required.
    // Ideally, the overflow would be allowed and the overflow digit would go as
    // the first digit of the numerator in the division algorithm ...
    // Making Integer<N> dependant from Integer<N+1> creates an infinite recursive
    // dependancy and is NOT acceptable ... Nesting a special class would be even
    // worse in code size and performances (including cache misses) ...
    UInt iLeadD = iDenominator._LeadingDigit();
    UInt iLeadDigitD = iDenominator._GetDigit(iLeadD);
    UInt iAdjust = 0x00010000 / (iLeadDigitD + 1);
    Integer<INT_BYTES> intAdjNum = ( iNumerator * iAdjust );
    Integer<INT_BYTES> intAdjDenom = ( iDenominator * iAdjust );
    Assert( intAdjDenom._LeadingDigit() == iLeadD );

    // Init results
    outQuotient = 0;
    outRemainder = 0;

    // Compute steps required
    UInt iLeadN = intAdjNum._LeadingDigit();
    UInt iLeadQ, iHatR;
    Assert( iLeadN >= iLeadD );
    if ( iLeadN != iLeadD ) {
        iLeadQ = ( iLeadN - iLeadD - 1 );
        iHatR = 1;
    } else {
        iLeadQ = 0;
        iHatR = 0;
    }

    // Divide
    UInt iDigitN0, iDigitN1, iDigitN2;
    UInt iDigitD0 = intAdjDenom._GetDigit(iLeadD);
    UInt iDigitD1 = intAdjDenom._GetDigit(iLeadD - 1);
    Int iDigitN = (Int)iLeadN;
    Int iDigitQ = (Int)iLeadQ;
    UInt iHatQ, iProd, iTmp;
    while( iDigitQ >= 0 ) {
        if ( iHatR > 0 ) {
            iDigitN0 = intAdjNum._GetDigit( iDigitN-- );
            iDigitN1 = intAdjNum._GetDigit( iDigitN-- );
            iDigitN2 = intAdjNum._GetDigit( iDigitN );
        } else {
            iDigitN0 = 0;
            iDigitN1 = intAdjNum._GetDigit( iDigitN-- );
            iDigitN2 = intAdjNum._GetDigit( iDigitN );
        }

        // Quotient
        iTmp = ( iDigitN0 << 16 ) | iDigitN1;
        iHatQ = (iDigitN0 != iDigitD0) ? (iTmp / iDigitD0) : 0x0000ffff;
        iProd = iHatQ * iDigitD0;
        Assert( iTmp >= iProd );
        iHatR = iTmp - iProd;
        if ( iDigitD1 * iHatQ > (iHatR << 16) + iDigitN2 ) {
            --iHatQ;
            iHatR += iDigitD0;
            if ( iDigitD1 * iHatQ > (iHatR << 16) + iDigitN2 ) {
                --iHatQ;
                iHatR += iDigitD0;
            }
        }
        Integer<INT_BYTES> intQ(0u);
        intQ._SetDigit( iDigitQ, iHatQ );

        // Remainder
        Integer<INT_BYTES> intProd = intQ * intAdjDenom;
        intAdjNum -= intProd;
        if ( intAdjNum < 0 ) {
            --iHatQ;
            intAdjNum += intAdjDenom;
            Assert( intAdjNum >= 0 );
        }

        // Update
        outQuotient._SetDigit( iDigitQ, iHatQ );
        if ( intAdjNum < intAdjDenom )
            break;
        iDigitN = intAdjNum._LeadingDigit();
        --iDigitQ;
    }

    // UnNormalize
    if ( intAdjNum > 0 ) {
        Integer intShouldBeZero;
        _DivSingle( intAdjNum, (UInt)( iAdjust & 0x0000FFFF ), outRemainder, intShouldBeZero );
        Assert( intShouldBeZero == 0 );
    } else
        outRemainder = 0;
}

template<UInt INT_BYTES>
inline Int Integer<INT_BYTES>::_GetSign() const {
    return ( ( m_arrDigits[INT_DIGIT_LAST] & 0x8000 ) != 0 ) ? -1 : +1;
}
template<UInt INT_BYTES>
inline Void Integer<INT_BYTES>::_SetSign( Bool bBitValue ) {
    if (bBitValue)
        m_arrDigits[INT_DIGIT_LAST] |= 0x8000;
    else
        m_arrDigits[INT_DIGIT_LAST] &= 0x7fff;
}
template<UInt INT_BYTES>
inline UInt Integer<INT_BYTES>::_GetDigit( UInt iDigit ) const {
    Assert( iDigit < INT_DIGIT_COUNT );
    if ( iDigit == INT_DIGIT_LAST )
        return ( (UInt)(m_arrDigits[iDigit] & 0x7fff) & 0x0000ffff );
    return ( (UInt)(m_arrDigits[iDigit]) & 0x0000ffff );
}
template<UInt INT_BYTES>
inline UInt Integer<INT_BYTES>::_GetDigit( UInt iLowDigit, UInt iHighDigit ) const {
    return ( _GetDigit(iLowDigit) | (_GetDigit(iHighDigit) << 16) );
}
template<UInt INT_BYTES>
inline Void Integer<INT_BYTES>::_SetDigit( UInt iDigit, UInt iValue ) {
    Assert( iDigit < INT_DIGIT_COUNT );
    if ( iDigit == INT_DIGIT_LAST ) {
        m_arrDigits[iDigit] &= 0x8000;
        m_arrDigits[iDigit] |= (UShort)( iValue & 0x00007fff );
    } else
        m_arrDigits[iDigit] = (UShort)( iValue & 0x0000ffff );
}
template<UInt INT_BYTES>
inline Bool Integer<INT_BYTES>::_GetBit( UInt iBit ) const {
    Assert( iBit < INT_BITS );
    UInt iDigit = ( iBit >> 4 ); // /16
    UInt iIndex = ( iBit & 0x0f ); // %16
    return ( (m_arrDigits[iDigit] & (1 << iIndex)) != 0 );
}
template<UInt INT_BYTES>
inline Void Integer<INT_BYTES>::_SetBit( UInt iBit, Bool bBitValue ) {
    Assert( iBit < INT_BITS );
    UInt iDigit = ( iBit >> 4 ); // /16
    UInt iIndex = ( iBit & 0x0f ); // %16
    if (bBitValue)
        m_arrDigits[iDigit] |= (1 << iIndex);
    else
        m_arrDigits[iDigit] &= ~(1 << iIndex);
}

template<UInt INT_BYTES>
UInt Integer<INT_BYTES>::_LeadingDigit() const
{
    if ( m_arrDigits[INT_DIGIT_LAST] != 0 && m_arrDigits[INT_DIGIT_LAST] != 0x8000 )
        return INT_DIGIT_LAST;
    for( Int i = INT_DIGIT_LAST - 1; i >= 0; --i ) {
        if ( m_arrDigits[i] != 0 )
            return (UInt)i;
    }
    return INVALID_OFFSET;
}
template<UInt INT_BYTES>
UInt Integer<INT_BYTES>::_TrailingDigit() const
{
    for( UInt i = 0; i < INT_DIGIT_LAST; ++i ) {
        if ( m_arrDigits[i] != 0 )
            return i;
    }
    if ( m_arrDigits[INT_DIGIT_LAST] != 0 && m_arrDigits[INT_DIGIT_LAST] != 0x8000 )
        return INT_DIGIT_LAST;
    return INVALID_OFFSET;
}
template<UInt INT_BYTES>
UInt Integer<INT_BYTES>::_LeadingBit( UInt iDigit ) const
{
    Assert( iDigit < INT_DIGIT_COUNT );
    UShort iValue = m_arrDigits[iDigit];
    if ( iDigit == INT_DIGIT_LAST )
        iValue &= 0x7fff;
    Assert( iValue != 0 );

    // Binary search for MSB
    if ( (iValue & 0xff00) != 0 ) {
        if ( (iValue & 0xf000) != 0 ) {
            if ( (iValue & 0xc000) != 0 ) {
                if ( (iValue & 0x8000) != 0 )
                    return 15;
                else // ( (iValue & 0x4000) != 0 )
                    return 14;
            } else { // ( (iValue & 0x3000) != 0 )
                if ( (iValue & 0x2000) != 0 )
                    return 13;
                else // ( (iValue & 0x1000) != 0 )
                    return 12;
            }
        } else { // ( (iValue & 0x0f00) != 0 )
            if ( (iValue & 0x0c00) != 0 ) {
                if ( (iValue & 0x0800) != 0 )
                    return 11;
                else // ( (iValue & 0x0400) != 0 )
                    return 10;
            } else { // ( (iValue & 0x0300) != 0 )
                if ( (iValue & 0x0200) != 0 )
                    return 9;
                else // ( (iValue & 0x0100) != 0 )
                    return 8;
            }
        }
    } else { // ( (iValue & 0x00ff) != 0 )
        if ( (iValue & 0x00f0) != 0 ) {
            if ( (iValue & 0x00c0) != 0 ) {
                if ( (iValue & 0x0080) != 0 )
                    return 7;
                else // ( (iValue & 0x0040) != 0 )
                    return 6;
            } else { // ( (iValue & 0x0030) != 0 )
                if ( (iValue & 0x0020) != 0 )
                    return 5;
                else // ( (iValue & 0x0010) != 0 )
                    return 4;
            }
        } else { // ( iValue & 0x000f != 0 )
            if ( (iValue & 0x000c) != 0 ) {
                if ( (iValue & 0x0008) != 0 )
                    return 3;
                else // ( (iValue & 0x0004) != 0 )
                    return 2;
            } else { // ( (iValue & 0x0003) != 0 )
                if ( (iValue & 0x0002) != 0 )
                    return 1;
                else // ( (iValue & 0x0001) != 0 )
                    return 0;
            }
        }
    }
}
template<UInt INT_BYTES>
UInt Integer<INT_BYTES>::_TrailingBit( UInt iDigit ) const
{
    Assert( iDigit < INT_DIGIT_COUNT );
    UShort iValue = m_arrDigits[iDigit];
    if ( iDigit == INT_DIGIT_LAST )
        iValue &= 0x7fff;
    Assert( iValue != 0 );

    // Binary search for LSB
    if ( (iValue & 0x00ff) != 0 ) {
        if ( (iValue & 0x000f) != 0 ) {
            if ( (iValue & 0x0003) != 0 ) {
                if ( (iValue & 0x0001) != 0 )
                    return 0;
                else // ( (iValue & 0x0002) != 0 )
                    return 1;
            } else { // ( (iValue & 0x000c) != 0 )
                if ( (iValue & 0x0004) != 0 )
                    return 2;
                else // ( (iValue & 0x0008) != 0 )
                    return 3;
            }
        } else { // ( (iValue & 0x00f0) != 0 )
            if ( (iValue & 0x0030) != 0 ) {
                if ( (iValue & 0x0010) != 0 )
                    return 4;
                else // ( (iValue & 0x0020) != 0 )
                    return 5;
            } else { // ( (iValue & 0x00c0) != 0 )
                if ( (iValue & 0x0040) != 0 )
                    return 6;
                else // ( (iValue & 0x0080) != 0 )
                    return 7;
            }
        }
    } else { // ( (iValue & 0xff00) != 0 )
        if ( (iValue & 0x0f00) != 0 ) {
            if ( (iValue & 0x0300) != 0 ) {
                if ( (iValue & 0x0100) != 0 )
                    return 8;
                else // ( (iValue & 0x0200) != 0 )
                    return 9;
            } else { // ( (iValue & 0x0c00) != 0 )
                if ( (iValue & 0x0400) != 0 )
                    return 10;
                else // ( (iValue & 0x0800) != 0 )
                    return 11;
            }
        } else { // ( (iValue & 0xf000) != 0 )
            if ( (iValue & 0x3000) != 0 ) {
                if ( (iValue & 0x1000) != 0 )
                    return 12;
                else // ( (iValue & 0x2000) != 0 )
                    return 13;
            } else { // ( (iValue & 0xc000) != 0 )
                if ( (iValue & 0x4000) != 0 )
                    return 14;
                else // ( (iValue & 0x8000) != 0 )
                    return 15;
            }
        }
    }
}
template<UInt INT_BYTES>
inline UInt Integer<INT_BYTES>::_LeadingBit() const {
    UInt iDigit = _LeadingDigit();
    if ( iDigit != INVALID_OFFSET )
        return _LeadingBit(iDigit); + ( iDigit << 4 ); // *16
    return INVALID_OFFSET;
}
template<UInt INT_BYTES>
inline UInt Integer<INT_BYTES>::_TrailingBit() const {
    UInt iDigit = _TrailingDigit();
    if ( iDigit != INVALID_OFFSET )
        return _TrailingBit(iDigit); + ( iDigit << 4 ); // *16
    return INVALID_OFFSET;
}

