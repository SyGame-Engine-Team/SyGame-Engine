/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Tools/GUIEvent.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Messaging support for the GUI system.
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
#include "GUIEvent.h"

/////////////////////////////////////////////////////////////////////////////////
// GUIEvent implementation
GUIEvent::GUIEvent()
{
    m_iType = GUIEVENT_NOP;
	m_iFlags = 0;

    m_iParamTotalSize = 0;
    m_iParamCount = 0;
}
GUIEvent::GUIEvent( GUIEventType iType, GUIEventFlag iFlags )
{
	m_iType = iType;
	m_iFlags = iFlags;

    m_iParamTotalSize = 0;
    m_iParamCount = 0;
}
GUIEvent::GUIEvent( const GUIEvent & rhs )
{
    m_iType = rhs.m_iType;
	m_iFlags = rhs.m_iFlags;

    m_iParamTotalSize = rhs.m_iParamTotalSize;
    m_iParamCount = rhs.m_iParamCount;

    MemCopy( m_arrParameters, rhs.m_arrParameters, m_iParamTotalSize );
    MemCopy( m_arrParamOffsets, rhs.m_arrParamOffsets, m_iParamCount * sizeof(UInt) );
}
GUIEvent::~GUIEvent()
{
	// nothing to do
}

GUIEvent & GUIEvent::operator=( const GUIEvent & rhs )
{
    m_iType = rhs.m_iType;
	m_iFlags = rhs.m_iFlags;

    m_iParamTotalSize = rhs.m_iParamTotalSize;
    m_iParamCount = rhs.m_iParamCount;

    MemCopy( m_arrParameters, rhs.m_arrParameters, m_iParamTotalSize );
    MemCopy( m_arrParamOffsets, rhs.m_arrParamOffsets, m_iParamCount * sizeof(UInt) );

    return (*this);
}

Void GUIEvent::PushParameter( const Byte * pParameter, UInt iSize )
{
    Assert( m_iParamCount < GUIEVENT_MAX_PARAMETERS );
    Assert( m_iParamTotalSize + iSize <= GUIEVENT_MAX_PARAMETERS_SIZE );

    m_arrParamOffsets[m_iParamCount] = m_iParamTotalSize;
	MemCopy( m_arrParameters + m_iParamTotalSize, pParameter, iSize );
    m_iParamTotalSize += iSize;
    ++m_iParamCount;
}

