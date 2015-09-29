/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Memory/Allocators/StackAllocator.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Very simple, but efficient, LIFO allocator.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// StackAllocator implementation
inline AllocatorType StackAllocator::GetType() const {
    return ALLOCATOR_STACK;
}
inline UInt StackAllocator::GetBlockSize( Byte * pMemory ) const {
    Assert( m_pBuffer != NULL );
    Assert( pMemory >= ( m_pBuffer + m_iStackBase ) );
    Assert( pMemory < ( m_pBuffer + m_iStackTop ) );
    return ( m_iStackTop - (UInt)(pMemory - m_pBuffer) );
}

inline UInt StackAllocator::FrameLevel() const {
    return m_iFrameLevel;
}
inline UInt StackAllocator::FrameSize() const {
    return (m_iStackTop - m_iStackBase);
}
inline UInt StackAllocator::AllocatedSize() const {
    return m_iStackTop;
}
inline UInt StackAllocator::TotalSize() const {
    return m_iTotalSize;
}
