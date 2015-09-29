/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Tools/GUITimerEvents.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Non-buffered clock-tick events.
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
#include "GUITimerEvents.h"

#include "../GUIManager.h"

/////////////////////////////////////////////////////////////////////////////////
// GUITimerEvents implementation
GUITimerEvents::GUITimerEvents()
{
    m_iTimerCount = 0;
    for( UInt i = 0; i < GUITIMEREVENTS_MAX; ++i ) {
        m_arrTimers[i].pTarget = NULL;
        m_arrTimers[i].fIntervalMS = 0.0;
        m_arrTimers[i].fLastTickMS = 0.0;
    }
}
GUITimerEvents::~GUITimerEvents()
{
    // nothing to do
}

UInt GUITimerEvents::CreateTimer( GUIEventProcessor * pTarget, Double fIntervalMS )
{
    if ( m_iTimerCount >= GUITIMEREVENTS_MAX )
        return INVALID_OFFSET;

    UInt iTimerID = m_iTimerCount;
    m_arrTimers[iTimerID].pTarget = pTarget;
    m_arrTimers[iTimerID].fIntervalMS = fIntervalMS;
    m_arrTimers[iTimerID].fLastTickMS = -1.0;
    ++m_iTimerCount;
    return iTimerID;
}
Void GUITimerEvents::DestroyTimer( UInt iTimerID )
{
    Assert( m_iTimerCount > 0 );
    Assert( iTimerID < m_iTimerCount );
    --m_iTimerCount;
    for( UInt i = iTimerID; i < m_iTimerCount; ++i ) {
        m_arrTimers[i].pTarget = m_arrTimers[i+1].pTarget;
        m_arrTimers[i].fIntervalMS = m_arrTimers[i+1].fIntervalMS;
        m_arrTimers[i].fLastTickMS = m_arrTimers[i+1].fLastTickMS;
    }
}

