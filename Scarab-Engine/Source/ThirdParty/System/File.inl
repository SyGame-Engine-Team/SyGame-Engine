/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/System/File.inl
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
// HFile implementation
inline Bool HFile::IsValid() const {
    return ( m_hFile != NULL );
}
inline Bool HFile::CanRead() const {
    return ( (m_iMode & FILE_READ) != 0 );
}
inline Bool HFile::CanWrite() const {
    return ( (m_iMode & FILE_WRITE) != 0 );
}

inline const GChar * HFile::GetPathName() const {
    return m_strPathName;
}
inline FileMode HFile::GetMode() const {
    return m_iMode;
}
inline FileFlags HFile::GetFlags() const {
    return m_iFlags;
}
inline FileAttributes HFile::GetAttributes() const {
    return m_iAttributes;
}

inline Bool HFile::Seek( UInt64 iAbsolute ) const {
    return Seek( FILE_SEEK_BEGIN, (Int64)iAbsolute );
}
inline Bool HFile::Rewind() const {
    return Seek( FILE_SEEK_BEGIN, 0 );
}


