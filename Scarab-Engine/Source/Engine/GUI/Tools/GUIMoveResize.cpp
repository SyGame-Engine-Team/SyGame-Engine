/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Tools/GUIMoveResize.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Window Moving & Sizing state management
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
#include "GUIMoveResize.h"

#include "../GUIManager.h"

/////////////////////////////////////////////////////////////////////////////////
// GUIMoveResize implementation
GUIMoveResize::GUIMoveResize():
    m_offMoveDragPoint(), m_rectOldWinRect()
{
    m_pWindow = NULL;

    m_bIsMoving = false;
    m_offMoveDragPoint.X = 0;
    m_offMoveDragPoint.Y = 0;

    m_bIsResizing = false;
    m_iResizeFlags = 0;
    m_rectOldWinRect.TopLeft.X = 0;
    m_rectOldWinRect.TopLeft.Y = 0;
    m_rectOldWinRect.DownRight.X = 0;
    m_rectOldWinRect.DownRight.Y = 0;
}
GUIMoveResize::~GUIMoveResize()
{
    // nothing to do
}

Void GUIMoveResize::StartMoving( GUIWindow * pWindow, const Point2 & ptScreenDragPoint )
{
    Assert( m_pWindow == NULL );

    m_pWindow = pWindow;

    // Purge pending events
    Bool bProcessed = m_pWindow->_ProcessNextEvent();
    while( bProcessed )
        bProcessed = m_pWindow->_ProcessNextEvent();

    // Compute drag point offset
    m_offMoveDragPoint = ( ptScreenDragPoint - m_pWindow->GetWindowRect().TopLeft );

    // Start moving
    m_bIsMoving = true;
}
Void GUIMoveResize::UpdateMove( const Point2 & ptScreenPos )
{
    Assert( m_bIsMoving );

    Point2 ptNewPos = ( ptScreenPos - m_offMoveDragPoint );

    // Move
    m_pWindow->_Move( ptNewPos );
}
Void GUIMoveResize::EndMoving()
{
    Assert( m_bIsMoving );

    // Reset
    m_offMoveDragPoint.X = 0;
    m_offMoveDragPoint.Y = 0;

    // End moving
    m_pWindow = NULL;
    m_bIsMoving = false;
}

Void GUIMoveResize::StartResizing( GUIWindow * pWindow, GUIResizeFlag iFlags )
{
    Assert( m_pWindow == NULL );

    m_pWindow = pWindow;

    // Purge pending events
    Bool bProcessed = m_pWindow->_ProcessNextEvent();
    while( bProcessed )
        bProcessed = m_pWindow->_ProcessNextEvent();

    // Save state
    m_iResizeFlags = iFlags;
    m_rectOldWinRect = m_pWindow->GetWindowRect();

    // Start resizing
    m_bIsResizing = true;
}
Void GUIMoveResize::UpdateResize( const Point2 & ptScreenPos )
{
    Assert( m_bIsResizing );

    UInt iWidth, iHeight;
    GUIFn->GetScreenSize( &iWidth, &iHeight );

    Rectangle2 rectNewWinRect = m_rectOldWinRect;
    if ( m_iResizeFlags & GUIRESIZE_TOP )
        rectNewWinRect.TopLeft.Y = Clamp<Int>( (ptScreenPos.Y - 1), 0, (rectNewWinRect.DownRight.Y - GUIWINDOW_MIN_HEIGHT) );
    if ( m_iResizeFlags & GUIRESIZE_LEFT )
        rectNewWinRect.TopLeft.X = Clamp<Int>( (ptScreenPos.X - 1), 0, (rectNewWinRect.DownRight.X - GUIWINDOW_MIN_WIDTH) );
    if ( m_iResizeFlags & GUIRESIZE_BOTTOM )
        rectNewWinRect.DownRight.Y = Clamp<Int>( (ptScreenPos.Y + 1), (rectNewWinRect.TopLeft.Y + GUIWINDOW_MIN_HEIGHT), (iHeight - 1) );
    if ( m_iResizeFlags & GUIRESIZE_RIGHT )
        rectNewWinRect.DownRight.X = Clamp<Int>( (ptScreenPos.X + 1), (rectNewWinRect.TopLeft.X + GUIWINDOW_MIN_WIDTH), (iWidth - 1) );

    // Move & Resize
    m_pWindow->_MoveResize( rectNewWinRect.TopLeft, rectNewWinRect.Width(), rectNewWinRect.Height() );
    m_pWindow->_PostEvent_Resize();
}
Void GUIMoveResize::EndResizing()
{
    Assert( m_bIsResizing );

    // Reset
    m_iResizeFlags = 0;
    m_rectOldWinRect.TopLeft.X = 0;
    m_rectOldWinRect.TopLeft.Y = 0;
    m_rectOldWinRect.DownRight.X = 0;
    m_rectOldWinRect.DownRight.Y = 0;

    // End resizing
    m_pWindow = NULL;
    m_bIsResizing = false;
}

