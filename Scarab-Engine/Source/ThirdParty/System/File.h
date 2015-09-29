/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/System/File.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : File IO standard API
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
#ifndef SCARAB_THIRDPARTY_SYSTEM_FILE_H
#define SCARAB_THIRDPARTY_SYSTEM_FILE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Platform.h"

#include "String.h"

/////////////////////////////////////////////////////////////////////////////////
// Forward declarations for Win32
typedef void * HANDLE;

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define FILENAME_LENGTH 256
#define PATHNAME_LENGTH 1024

#define FILE_ACCESS_RETRIES 5

// Modes
typedef DWord FileMode;
#define FILE_READ   0x00000001 // Allows read operations
#define FILE_WRITE  0x00000002 // Allows write operations

// Flags
typedef DWord FileFlags;
#define FILE_FLAG_RANDOM         0x00000010 // Hint signaling random access will be used
#define FILE_FLAG_SEQUENTIAL     0x00000020 // Hint signaling sequential access will be used
#define FILE_FLAG_NO_WRITE_CACHE 0x00000100 // Disable Caching for write operations
#define FILE_FLAG_NO_CACHE       0x00000200 // Disable Caching for read & write operations
//#define FILE_FLAG_ASYNCHRONOUS   0x80000000 // Enable asynchronous I/O interface (Don't disable system cache is this case !)

// Attributes
typedef DWord FileAttributes;
#define FILE_ATTRIB_READONLY  0x00000001
#define FILE_ATTRIB_TEMPORARY 0x00000002
#define FILE_ATTRIB_ARCHIVE   0x00000004
#define FILE_ATTRIB_HIDDEN    0x00000008

// Seeking
enum FileSeek
{
    FILE_SEEK_BEGIN = 0,
    FILE_SEEK_CURRENT,
    FILE_SEEK_END
};

// Locking
//enum FileLock
//{
//    FILE_LOCK_SHARED = 0,
//    FILE_LOCK_EXCLUSIVE
//};

/////////////////////////////////////////////////////////////////////////////////
// The HFile class
class HFile
{
public:
    HFile();
	~HFile();

    //Bool ReOpen( FileMode iMode, FileFlags iFlags = 0 );
    Void Close();	

    inline Bool IsValid() const;
    inline Bool CanRead() const;
    inline Bool CanWrite() const;

    inline const GChar * GetPathName() const;
    inline FileMode GetMode() const;
    inline FileFlags GetFlags() const;
    inline FileAttributes GetAttributes() const;

    UInt64 GetSize() const;
    Bool SetSize( UInt64 iByteSize ) const;

    Bool Tell( UInt64 * outAbsolute ) const;
    Bool Seek( FileSeek iBase, Int64 iRelative ) const;
    inline Bool Seek( UInt64 iAbsolute ) const;
    inline Bool Rewind() const;

    Bool ReadBytes( Byte * pOutBuffer, UInt iByteSize ) const;
    Bool WriteBytes( const Byte * pInBuffer, UInt iByteSize ) const;

    Bool ReadChar( GChar * outCh ) const;
    Bool WriteChar( GChar ch ) const;

    Bool ReadString( GChar * outString, UInt iMaxLength ) const;
    Bool WriteString( const GChar * strString ) const;

    //Bool LockBytes( FileLock iLock, UInt64 iByteOffset, UInt64 iByteSize, Bool bWait = true ) const;
    //Bool UnLockBytes( UInt64 iByteOffset, UInt64 iByteSize ) const;

private:
    // Delegate of system
    friend class System;
    HFile( const GChar * strPathName, FileMode iMode, FileFlags iFlags, Bool bCreateElseOpen );

    // Helpers
    static FileAttributes _ConvertAttributes( DWord dwAttribs );
    static DWord _RevertAttributes( FileAttributes iAttributes );

	GChar m_strPathName[PATHNAME_LENGTH];
    FileMode m_iMode;
    FileFlags m_iFlags;
    FileAttributes m_iAttributes;

    // Handle
    HANDLE m_hFile;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "File.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_THIRDPARTY_SYSTEM_FILE_H
