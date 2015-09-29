/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/System/Platform.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Platform independance standard definitions
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Bit-wise functions
inline Byte LowWord(Word value)   {
    return (Byte) ( value & 0x00ffu );
}
inline Word LowWord(DWord value)  {
    return (Word) ( value & 0x0000fffful );
}
inline DWord LowWord(QWord value) {
    return (DWord)( value & 0x00000000ffffffffui64 );
}
inline Byte HighWord(Word value)   {
    return (Byte) ( (value & 0xff00u) >> 8 );
}
inline Word HighWord(DWord value)  {
    return (Word) ( (value & 0xffff0000ul) >> 16 );
}
inline DWord HighWord(QWord value) {
    return (DWord)( (value & 0xffffffff00000000ui64) >> 32 );
}

inline Byte BitRotL(Byte value, UInt shift) {
    return ( (value << shift) | (value >> (bit_sizeof(Byte)-shift)) );
}
inline Word BitRotL(Word value, UInt shift) {
    return ( (value << shift) | (value >> (bit_sizeof(Word)-shift)) );
}
inline DWord BitRotL(DWord value, UInt shift) {
    return ( (value << shift) | (value >> (bit_sizeof(DWord)-shift)) );
}
inline QWord BitRotL(QWord value, UInt shift) {
    return ( (value << shift) | (value >> (bit_sizeof(QWord)-shift)) );
}
inline Byte BitRotR(Byte value, UInt shift) {
    return ( (value >> shift) | (value << (bit_sizeof(Byte)-shift)) );
}
inline Word BitRotR(Word value, UInt shift) {
    return ( (value >> shift) | (value << (bit_sizeof(Word)-shift)) );
}
inline DWord BitRotR(DWord value, UInt shift) {
    return ( (value >> shift) | (value << (bit_sizeof(DWord)-shift)) );
}
inline QWord BitRotR(QWord value, UInt shift) {
    return ( (value >> shift) | (value << (bit_sizeof(QWord)-shift)) );
}

extern const Byte _sg_arrByteMirrors[256];
inline Byte BitMirror(Byte value) {
    return _sg_arrByteMirrors[value];
}
inline Word BitMirror(Word value) {
    return (
        ( (Word)BitMirror(LowWord(value)) << bit_sizeof(Byte) ) |
		( (Word)BitMirror(HighWord(value)) )
	);
}
inline DWord BitMirror(DWord value) {
    return (
        ( (DWord)BitMirror(LowWord(value)) << bit_sizeof(Word) ) |
		( (DWord)BitMirror(HighWord(value)) )
	);
}
inline QWord BitMirror(QWord value) {
    return (
        ( (QWord)BitMirror(LowWord(value)) << bit_sizeof(DWord) ) |
		( (QWord)BitMirror(HighWord(value)) )
	);
}

inline Byte ByteMirror(Byte value) {
    return value;
}
inline Word ByteMirror(Word value) {
    return (
		( (value & 0x00ffu) << 8 ) |
        ( (value & 0xff00u) >> 8 )
	);
}
inline DWord ByteMirror(DWord value) {
    return (
		( (value & 0x000000fful) << 24 ) |
		( (value & 0x0000ff00ul) <<  8 ) |
		( (value & 0x00ff0000ul) >>  8 ) |
		( (value & 0xff000000ul) >> 24 )
	);
}
inline QWord ByteMirror(QWord value) {
    return (
		( (value & 0x00000000000000ffui64) << 56 ) |
		( (value & 0x000000000000ff00ui64) << 40 ) |
		( (value & 0x0000000000ff0000ui64) << 24 ) |
		( (value & 0x00000000ff000000ui64) <<  8 ) |
		( (value & 0x000000ff00000000ui64) >>  8 ) |
		( (value & 0x0000ff0000000000ui64) >> 24 ) |
		( (value & 0x00ff000000000000ui64) >> 40 ) |
		( (value & 0xff00000000000000ui64) >> 56 )
	);
}

/////////////////////////////////////////////////////////////////////////////////
// All purpose functions
template<typename T>
inline T DivUp(const T & lhs, const T & rhs) {
    return ( (lhs + rhs - 1) / rhs );
}
template<typename T>
inline const T & Min(const T & lhs, const T & rhs) {
    return (lhs < rhs) ? lhs : rhs;
}
template<typename T>
inline const T & Max(const T & lhs, const T & rhs) {
    return (lhs < rhs) ? rhs : lhs;
}
template<typename T>
inline T Clamp(const T & val, const T & low, const T & high) {
    return Max<T>( low, Min<T>(val, high) );
}
template<typename T>
inline void Swap(T * lhs, T * rhs) {
    T swap = *lhs;
    *lhs = *rhs;
    *rhs = swap;
}

template<typename T>
Bool Contains( const T * arrItems, UInt iSize, const T & rItem )
{
    for( UInt i = 0; i < iSize; ++i ) {
        if ( arrItems[i] == rItem )
            return true;
    }
    return false;
}

template<typename T>
Void QuickSort( T * arrItems, UInt iSize, Int (*pfCompare)(const T &, const T &, Void*), Void * pUserData )
{
    static Bool s_bSendLeft = true;
    if (iSize <= 1)
        return;

    T * pPivot = arrItems;
    T * pBack = arrItems + iSize - 1;
    Int iCmp;
    while(pPivot < pBack) {
        iCmp = pfCompare( *pPivot, *(pPivot + 1), pUserData );
        if ( iCmp < 0 ) {
            Swap<T>( pPivot, pPivot + 1 );
            ++pPivot;
        } else if ( iCmp > 0 ) {
            if ( pPivot + 1 != pBack )
		        Swap<T>( pPivot + 1, pBack );
		    --pBack;
        } else {
            // balance distribution of equal cases to temper some annoying
            // issues like arrays filled with the same value ...
            if (s_bSendLeft) {
                Swap<T>( pPivot, pPivot + 1 );
			    ++pPivot;
		    } else {
			    if ( pPivot + 1 != pBack )
		            Swap<T>( pPivot + 1, pBack );
			    --pBack;
		    }
		    s_bSendLeft = !s_bSendLeft;
        }
    }

    UInt iLeft = ( pPivot - arrItems );
    UInt iRight = ( iSize - iLeft - 1 );
    if (iLeft > 1)
	    QuickSort<T>( arrItems, iLeft, pfCompare, pUserData );
    if (iRight > 1)
	    QuickSort<T>( pPivot + 1, iRight, pfCompare, pUserData );
}

/////////////////////////////////////////////////////////////////////////////////
// Memory functions
inline UChar MemAlign8(UChar bitSize) {
    UChar mask = (BYTE_BITCOUNT - 1u);
	if (bitSize & mask)
		return ( (bitSize & (~mask)) + BYTE_BITCOUNT );
	return bitSize;
}
inline UShort MemAlign16(UShort byteSize) {
    UShort mask = (sizeof(Word) - 1u);
	if (byteSize & mask)
		return ( (byteSize & (~mask)) + sizeof(Word) );
	return byteSize;
}
inline ULong MemAlign32(ULong byteSize) {
    ULong mask = (sizeof(DWord) - 1ul);
	if (byteSize & mask)
		return ( (byteSize & (~mask)) + sizeof(DWord) );
	return byteSize;
}
inline UInt64 MemAlign64(UInt64 byteSize) {
    UInt64 mask = (sizeof(QWord) - 1ui64);
	if (byteSize & mask)
		return ( (byteSize & (~mask)) + sizeof(QWord) );
	return byteSize;
}
inline UInt64 MemAlign128(UInt64 byteSize) {
    UInt64 size = (sizeof(QWord) << 1); // *2
    UInt64 mask = (size - 1ui64);
	if (byteSize & mask)
		return ( (byteSize & (~mask)) + size );
	return byteSize;
}

/////////////////////////////////////////////////////////////////////////////////
// PixelFormat definitions
inline UInt PixelFormatBytes( PixelFormat iFormat ) {
    return ( PixelFormatBits(iFormat) >> 3 );
}

inline UInt PixelFormatChannelBytes( PixelFormat iFormat, UInt iChannel ) {
    return ( PixelFormatChannelBits(iFormat, iChannel) >> 3 );
}

inline UInt PixelFormatChannelMask( PixelFormat iFormat, UInt iChannel ) {
    return ( (1u << PixelFormatChannelBits(iFormat, iChannel)) - 1u ); // Underflow is intended !
}
