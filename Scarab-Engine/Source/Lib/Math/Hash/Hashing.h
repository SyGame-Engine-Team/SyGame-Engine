/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Hash/Hashing.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Simple, fast & specialized hashing algorithms ...
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : TODO : Recode PaulHsieh for string ... (boring)
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_LIB_MATH_HASH_HASHING_H
#define SCARAB_LIB_MATH_HASH_HASHING_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../ThirdParty/System/String.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define HashFn Hashing::GetInstancePtr()

/////////////////////////////////////////////////////////////////////////////////
// The Hashing class
class Hashing
{
    // Discrete singleton interface
public:
    inline static Hashing * GetInstancePtr();

private:
    Hashing();
    ~Hashing();

public:
    // Okay, we list algorithm ordered by method, complexity & efficiency
    // Here is a little comparative :
    // Min-Size = Minimum reasonable table size greater than 1000.
    // Complexity = Overall complexity in instructions
    // Streamed-Complexity = Complexity when streamed, gaining prelude/conclude code
    // Funnel-15 = Largest set of input-bits affecting the smallest set of internal-bits when mapping 15-byte keys into a 1-byte result.
    // Funnel-100 = Largest set of input-bits affecting the smallest set of internal-bits when mapping 100-byte keys into a 4-byte result.
    // Collide-32 = Number of collisions found when a dictionary of 38,470 English words was hashed into a 32-bit result.
    // Collide-1000 = Chi^2 measure of how well the hash did at mapping the dictionary
    //    ( Chi^2 measure means : "better than random < -3 < random fluctuations < +3 < worse than random" )
    //               Min-Size   Complexity 	Streamed-Complexity 	Funnel-15 	Funnel-100 	Collide-32 	Collide-1000
    // Bernstein         1024 	      7n+3 	               3n+2 	   3 => 2 	    3 => 2 	         4 	       +1.69
    // CRC32 	         1024         9n+3 	               5n+2 	   2 => 1 	  11 => 10 	         0 	       +0.07
    // CRCGeneric	     1024         9n+3 	               5n+2 	     none 	      none 	         0 	       -1.83
    // One-at-a-Time     1024 	      9n+9 	               5n+8 	     none 	      none 	         0 	       -0.05
    // BobJenkins        1024 	     6n+35 	                N/A 	     none 	      none 	         0 	       +0.33
    // Paul Hsieh        1024 	     5n+17 	                N/A 	   3 => 2 	    3 => 2 	         1 	       +1.12
    // BobJenkinsRot     1024 	     5n+20 	                N/A 	     none 	      none 	         0 	       -0.08
    //
    // IMPORTANT : None of those function can do better than 2^32 collision probability,
    // which is NOT suitable for cryptographic-level security, use higher-order hashers
    // like MD5 or SHA1 if you need cryptographic-level hashing ...
        // 32-bits hashers
    DWord MixedBits(DWord dwValue) const;
    DWord ThomasWang(DWord dwValue) const;
        // Simple hashers
    DWord BKDR( const Byte * str, UInt iLength ) const;
    DWord BKDR( const GChar * str ) const;
    DWord Bernstein( const Byte * str, UInt iLength ) const;
    DWord Bernstein( const GChar * str ) const;
    DWord AlphaNum( const Byte * str, UInt iLength ) const;
    DWord AlphaNum( const GChar * str ) const;
    DWord SDBM( const Byte * str, UInt iLength ) const;
    DWord SDBM( const GChar * str ) const;
    DWord RS( const Byte * str, UInt iLength ) const;
    DWord RS( const GChar * str ) const;
    DWord ELF( const Byte * str, UInt iLength ) const;
    DWord ELF( const GChar * str ) const;
    DWord FNV( const Byte * str, UInt iLength ) const;
    DWord FNV( const GChar * str ) const;
    DWord JS( const Byte * str, UInt iLength ) const;
    DWord JS( const GChar * str ) const;
    DWord AP( const Byte * str, UInt iLength ) const;
    DWord AP( const GChar * str ) const;
        // Cyclic Redundancy code hashers
    DWord CRC32( const Byte * str, UInt iLength ) const;
    DWord CRC32( const GChar * str ) const;
    DWord CRCGeneric( const Byte * str, UInt iLength ) const;
    DWord CRCGeneric( const GChar * str ) const;
        // Advanced hashers
    DWord OneAtATime( const Byte * str, UInt iLength ) const;
    DWord OneAtATime( const GChar * str ) const;
    DWord BobJenkins( const Byte * str, UInt iLength ) const;
    DWord BobJenkins( const GChar * str ) const;
    DWord PaulHsieh( const Byte * str, UInt iLength ) const;
    DWord PaulHsieh( const GChar * str ) const;
    DWord BobJenkinsRot( const Byte * str, UInt iLength ) const; // <= The best one so far ...
    DWord BobJenkinsRot( const GChar * str ) const;              //

private:
    // internal helpers
    inline Word _Get16Bits(const Byte * str) const;
    inline Word _Get16Bits(const AChar * str) const;
    inline Word _Get16Bits(const WChar * str) const;
    inline DWord _Get32Bits(const Byte * str) const;
    inline DWord _Get32Bits(const AChar * str) const;
    inline DWord _Get32Bits(const WChar * str) const;

    Void _CRC32_GenerateTable(DWord dwPolynomial);
    DWord m_arrTableCRC32[256];

    Void _CRCGeneric_GenerateTable();
    DWord m_arrTableCRCGeneric[256];

    inline Void _BobJenkins_Mix( DWord & a, DWord & b, DWord & c ) const;
    inline Void _BobJenkins_MixRot( DWord & a, DWord & b, DWord & c ) const;
    inline Void _BobJenkins_Final( DWord & a, DWord & b, DWord & c ) const;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Hashing.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_HASH_HASHING_H
