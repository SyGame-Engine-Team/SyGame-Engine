/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Tools/GUIDragNDrop.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Drag & Drop state management
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "GUIDragNDrop.h"

/////////////////////////////////////////////////////////////////////////////////
// GUIDragNDrop implementation
GUIDragNDrop::GUIDragNDrop()
{
    for( UInt i = 0; i < GUIDRAG_CHANNELS; ++i ) {
        m_arrDragData[i].bPrepared = false;
        m_arrDragData[i].bConfirmed = false;
        m_arrDragData[i].pSource = NULL;
        m_arrDragData[i].pDest = NULL;
        m_arrDragData[i].pUserData = NULL;
    }
}
GUIDragNDrop::~GUIDragNDrop()
{
    // nothing to do
}

Void GUIDragNDrop::Prepare( GUIDragChannel iChannel, GUIEventProcessor * pMousedOver, const Point2 & ptScreenPos )
{
    GUIDragData * pDragData = ( m_arrDragData + iChannel );
    Assert( !(pDragData->bPrepared) );

    pDragData->pSource = pMousedOver;
    pDragData->ptSourcePos = ptScreenPos;

    pDragData->bPrepared = true;
}
Bool GUIDragNDrop::Update( GUIDragChannel iChannel, GUIEventProcessor * pMousedOver, const Point2 & ptScreenPos )
{
    GUIDragData * pDragData = ( m_arrDragData + iChannel );
    Assert( pDragData->bPrepared );

    // Update case
    if ( pDragData->bConfirmed ) {
        pDragData->pDest = pMousedOver;
        pDragData->ptDestPos = ptScreenPos;
        return true;
    }

    // MousedOver has changed
    if ( pMousedOver != pDragData->pSource ) {
        pDragData->pDest = pMousedOver;
        pDragData->ptDestPos = ptScreenPos;
        pDragData->bConfirmed = true;
        return true;
    }

    // Moved far enough
    if ( _TestProximity( pDragData->ptSourcePos, ptScreenPos ) ) {
        pDragData->pDest = pMousedOver;
        pDragData->ptDestPos = ptScreenPos;
        pDragData->bConfirmed = true;
        return true;
    }

    return false;
}
Void GUIDragNDrop::Abort( GUIDragChannel iChannel )
{
    GUIDragData * pDragData = ( m_arrDragData + iChannel );
    Assert( pDragData->bPrepared );

    pDragData->bPrepared = false;
    pDragData->bConfirmed = false;
    pDragData->pSource = NULL;
    pDragData->pDest = NULL;
    pDragData->pUserData = NULL;
}
Void GUIDragNDrop::Complete( GUIDragChannel iChannel )
{
    GUIDragData * pDragData = ( m_arrDragData + iChannel );
    Assert( pDragData->bConfirmed );

    pDragData->bPrepared = false;
    pDragData->bConfirmed = false;
    pDragData->pSource = NULL;
    pDragData->pDest = NULL;
    pDragData->pUserData = NULL;
}

/////////////////////////////////////////////////////////////////////////////////

Bool GUIDragNDrop::_TestProximity( const Point2 & ptReferencePos, const Point2 & ptTestPos )
{
    Int iDX = ( ptTestPos.X - ptReferencePos.X );
    if ( iDX < 0 )
        iDX = -iDX;
    if ( iDX > GUIDRAG_THRESHOLD )
        return true;

    Int iDY = ( ptTestPos.Y - ptReferencePos.Y );
    if ( iDY < 0 )
        iDY = -iDY;
    if ( iDY > GUIDRAG_THRESHOLD )
        return true;

    return false;
}
