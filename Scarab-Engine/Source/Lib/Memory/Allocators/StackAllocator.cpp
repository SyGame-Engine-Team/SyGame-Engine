/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Memory/Allocators/StackAllocator.cpp
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

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "StackAllocator.h"

///////////////////////////////////////////////////////////////////////////////
// StackAllocator implementation
StackAllocator::StackAllocator( UInt iContextID, const GChar * strContextName,
                                UInt iAllocatorID, const GChar * strAllocatorName ):
    MemoryAllocator( iContextID, strContextName, iAllocatorID, strAllocatorName )
{
    m_pBuffer = NULL;
    m_iTotalSize = 0;
    m_iStackTop = 0;
    m_iStackBase = 0;
    m_iFrameLevel = 0;
}
StackAllocator::~StackAllocator()
{
    if ( m_pBuffer != NULL )
        SystemFn->MemFree( m_pBuffer );
}

Void StackAllocator::Initialize( UInt iStackSize )
{
    Assert( m_pBuffer == NULL );

    m_iTotalSize = iStackSize;
    m_pBuffer = (Byte*)SystemFn->MemAlloc( m_iTotalSize );

    m_iStackTop = 0;
    m_iStackBase = 0;
    m_iFrameLevel = 0;
}
Void StackAllocator::Cleanup()
{
    Assert( m_pBuffer != NULL );
    
    SystemFn->MemFree( m_pBuffer );

    m_pBuffer = NULL;
    m_iTotalSize = 0;
    m_iStackTop = 0;
    m_iStackBase = 0;
    m_iFrameLevel = 0;
}

Byte * StackAllocator::Allocate( UInt iSize )
{
    Assert( m_pBuffer != NULL );
    if ( (m_iStackTop + iSize) > m_iTotalSize )
        return NULL;

    Byte * pPtr = m_pBuffer + m_iStackTop;
    m_iStackTop += iSize;
    return pPtr;
}
Void StackAllocator::Free( Byte * pMemory )
{
    Assert( m_pBuffer != NULL );
    Assert( pMemory >= ( m_pBuffer + m_iStackBase ) );
    Assert( pMemory < ( m_pBuffer + m_iStackTop ) );

    m_iStackTop = (UInt)(pMemory - m_pBuffer);
}

Void StackAllocator::Free( UInt iSize )
{
    Assert( m_pBuffer != NULL );

    if ( iSize > (m_iStackTop - m_iStackBase) )
        m_iStackTop = m_iStackBase;
    else
        m_iStackTop -= iSize;
}
Void StackAllocator::Free()
{
    Assert( m_pBuffer != NULL );

    m_iStackTop = m_iStackBase;
}

UInt StackAllocator::BeginFrame()
{
    Assert( m_pBuffer != NULL );
    if ( (m_iStackTop + sizeof(UInt)) > m_iTotalSize )
        return INVALID_OFFSET;

    *( (UInt*)(m_pBuffer + m_iStackTop) ) = m_iStackBase;
    m_iStackTop += sizeof(UInt);
    m_iStackBase = m_iStackTop;
    ++m_iFrameLevel;
    return m_iFrameLevel;
}
Void StackAllocator::EndFrame()
{
    Assert( m_pBuffer != NULL );
    Assert( (m_iFrameLevel > 0) && (m_iStackTop > 0) );

    m_iStackTop = m_iStackBase;
    m_iStackTop -= sizeof(UInt);
    m_iStackBase = *( (UInt*)(m_pBuffer + m_iStackTop) );
    --m_iFrameLevel;
}
Void StackAllocator::UnrollFrames( UInt iTargetFrame )
{
    Assert( m_pBuffer != NULL );
    Assert( m_iFrameLevel > iTargetFrame );

    while( m_iFrameLevel > iTargetFrame )
        EndFrame();
}

Void StackAllocator::GenerateReport( AllocatorReport * outReport ) const
{
    static UInt s_ScratchMemory1[STACKREPORT_MAX_FRAMES];
    static UInt s_ScratchMemory2[STACKREPORT_MAX_FRAMES];
    Assert( m_iFrameLevel < STACKREPORT_MAX_FRAMES );

    Assert( outReport != NULL );
    StackReport * outStackReport = (StackReport*)outReport;
    outStackReport->idMemoryContext = m_idContext;
    outStackReport->strContextName = m_strContextName;
    outStackReport->idMemoryAllocator = m_idAllocator;
    outStackReport->strAllocatorName = m_strAllocatorName;
    outStackReport->pBaseAddress = m_pBuffer;
    outStackReport->iTotalSize = m_iTotalSize;
    outStackReport->iAllocatedSize = m_iStackTop;
    outStackReport->iFreeSize = (m_iTotalSize - m_iStackTop);
    outStackReport->iFrameLevel = m_iFrameLevel;
    outStackReport->iFrameCount = (m_iFrameLevel + 1);
    outStackReport->arrFrameBases = s_ScratchMemory1;
    outStackReport->arrFrameSizes = s_ScratchMemory2;

    // Extract frame layout
    Int iCurFrame = m_iFrameLevel;
    UInt iCurTop = m_iStackTop;
    UInt iCurBase = m_iStackBase;
    while( true ) {
        outStackReport->arrFrameBases[iCurFrame] = iCurBase;
        outStackReport->arrFrameSizes[iCurFrame] = ( iCurTop - iCurBase );
        if ( iCurFrame == 0 )
            break;
        iCurTop = iCurBase - sizeof(UInt);
        iCurBase = *( (UInt*)(m_pBuffer + iCurTop) );
        --iCurFrame;
    }
}
Void StackAllocator::LogReport( const AllocatorReport * pReport ) const
{
    Assert( pReport != NULL );
    const StackReport * pStackReport = (const StackReport*)pReport;

    HFile logFile = SystemFn->OpenFile( STACKREPORT_LOGFILE, FILE_WRITE );
    Assert( logFile.IsValid() );
    Bool bOk = logFile.Seek( FILE_SEEK_END, 0 );
    Assert( bOk );

    ErrorFn->Log( logFile, TEXT("Stack Report :") ); // Add Timestamps ? maybe usefull later ...

    ErrorFn->Log( logFile, TEXT("\n => Memory Context ID       : %ud"),  pStackReport->idMemoryContext );
    ErrorFn->Log( logFile, TEXT("\n => Memory Context Name     : %s"),   pStackReport->strContextName );
    ErrorFn->Log( logFile, TEXT("\n => Memory Allocator ID     : %ud"),  pStackReport->idMemoryAllocator );
    ErrorFn->Log( logFile, TEXT("\n => Memory Allocator Name   : %s"),   pStackReport->strAllocatorName );
    ErrorFn->Log( logFile, TEXT("\n => Base Address            : %u8x"), pStackReport->pBaseAddress );
    ErrorFn->Log( logFile, TEXT("\n => Total size              : %ud"),  pStackReport->iTotalSize );
    ErrorFn->Log( logFile, TEXT("\n => Allocated size          : %ud"),  pStackReport->iAllocatedSize );
    ErrorFn->Log( logFile, TEXT("\n => Free size               : %ud"),  pStackReport->iFreeSize );
    ErrorFn->Log( logFile, TEXT("\n => Frame Level             : %ud"),  pStackReport->iFrameLevel );
    ErrorFn->Log( logFile, TEXT("\n => Frame Count             : %ud"),  pStackReport->iFrameCount );

    ErrorFn->Log( logFile, TEXT("\n => FrameLayout (Base,Size) :") );
    for( UInt i = 0; i < pStackReport->iFrameCount; ++i )
        ErrorFn->Log( logFile, TEXT(" (%ud,%ud)"), pStackReport->arrFrameBases[i], pStackReport->arrFrameSizes[i] );

    ErrorFn->Log( logFile, TEXT("\n\n") );

    logFile.Close();
}
