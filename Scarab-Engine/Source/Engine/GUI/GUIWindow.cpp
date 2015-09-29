/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/GUIWindow.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GUIWindow is the root dynamic container for GUI layouts.
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
#include "GUIWindow.h"

#include "GUIManager.h"

/////////////////////////////////////////////////////////////////////////////////
// GUIWindow implementation
GUIWindow::GUIWindow( const GChar * strName, const Rectangle2 & rectWindow, GUIWindowStyle iWinStyle ):
    m_rectWindow( rectWindow )
{
    Assert( rectWindow.Width() >= GUIWINDOW_MIN_WIDTH );
    Assert( rectWindow.Height() >= GUIWINDOW_MIN_HEIGHT );

    // Name
    StringFn->NCopy( m_strName, strName, GUI_NAME_LENGTH - 1 );

    // Depth
    m_iDepth = GUIWINDOW_MIN_DEPTH;

    // Style
    m_iWinStyle = iWinStyle;

    // States
    m_bIsVisible = false;
    m_bHasFocus = false;
    m_pFocus = NULL;
    m_bHasMouseOver = false;
    m_pMousedOver = NULL;

    m_bEventBlocking = true;

    m_pWindowRoot = NULL;
    m_pClientRoot = NULL;

    UInt i;
    for( i = 0; i < GUIBORDER_COUNT; ++i )
        m_arrBorderModels[i] = NULL;
    for( i = 0; i < GUICORNER_COUNT; ++i )
        m_arrCornerModels[i] = NULL;
    m_pTitleBarModel = NULL;
    //m_pMenuBarModel = NULL;
    //m_pStatusBarModel = NULL;

    _BuildOverlappedWindow();
}
GUIWindow::~GUIWindow()
{
    // Destroy widget tree
    if ( m_pWindowRoot != NULL )
        Delete( m_pWindowRoot );

    // Destroy window models
    UInt i;
    //if ( m_pStatusBarModel != NULL )
    //    Delete( m_pStatusBarModel );
    //if ( m_pMenuBarModel != NULL )
    //    Delete( m_pMenuBarModel );
    if ( m_pTitleBarModel != NULL )
        Delete( m_pTitleBarModel );
    for( i = 0; i < GUICORNER_COUNT; ++i ) {
        if ( m_arrCornerModels[i] != NULL )
            Delete( m_arrCornerModels[i] );
    }
    for( i = 0; i < GUIBORDER_COUNT; ++i ) {
        if ( m_arrBorderModels[i] != NULL )
            Delete( m_arrBorderModels[i] );
    }
}

Void GUIWindow::SetVisible( Bool bVisible )
{
    if ( m_bIsVisible != bVisible ) {
        if ( bVisible )
            GUIFn->SetFocusWindow( this );
        else if ( m_bHasFocus )
            GUIFn->SetFocusWindow( NULL );
    }

    m_bIsVisible = bVisible;
    m_pWindowRoot->SetVisible( bVisible );
}

/////////////////////////////////////////////////////////////////////////////////

Void GUIWindow::_BuildOverlappedWindow()
{
    // WindowRoot
    m_pWindowRoot = New GUIWidget( this, TEXT("<Root>") );
    if ( m_iWinStyle == GUIWINDOW_STYLE_NAKED ) {
        m_pClientRoot = m_pWindowRoot;
        return;
    }

    // Layout
    GUIGridLayout winLayout;
    winLayout.FillX = GUILAYOUT_FILL_FIXED;
    winLayout.FillY = GUILAYOUT_FILL_FIXED;
    winLayout.Anchor = GUILAYOUT_ANCHOR_FIXED;

    Rectangle2 rectClient( Point2::Null, m_rectWindow.Width(), m_rectWindow.Height() );

    // Border
    if ( m_iWinStyle & GUIWINDOW_STYLE_BORDER ) {
        // Models
        UInt i;
        for( i = 0; i < GUIBORDER_COUNT; ++i )
            m_arrBorderModels[i] = New GUIBorderModel( (GUIBorderType)i );
        for( i = 0; i < GUICORNER_COUNT; ++i )
            m_arrCornerModels[i] = New GUICornerModel( (GUICornerType)i );

        // Layout
            // Top
        winLayout.FixedSize.X = rectClient.Width() - 6;
        winLayout.FixedSize.Y = 3;
        winLayout.FixedPos = rectClient.TopLeft;
        winLayout.FixedPos.X += 3;
        New GUIBorder( m_arrBorderModels[GUIBORDER_TOP], m_pWindowRoot, winLayout, TEXT("<Border_Top>") );
            // Left
        winLayout.FixedSize.X = 3;
        winLayout.FixedSize.Y = rectClient.Height() - 6;
        winLayout.FixedPos = rectClient.TopLeft;
        winLayout.FixedPos.Y += 3;
        New GUIBorder( m_arrBorderModels[GUIBORDER_LEFT], m_pWindowRoot, winLayout, TEXT("<Border_Left>") );
            // Bottom
        winLayout.FixedSize.X = rectClient.Width() - 6;
        winLayout.FixedSize.Y = 3;
        winLayout.FixedPos = rectClient.TopLeft;
        winLayout.FixedPos.X += 3;
        winLayout.FixedPos.Y += (rectClient.Height() - 3);
        New GUIBorder( m_arrBorderModels[GUIBORDER_BOTTOM], m_pWindowRoot, winLayout, TEXT("<Border_Bottom>") );
            // Right
        winLayout.FixedSize.X = 3;
        winLayout.FixedSize.Y = rectClient.Height() - 6;
        winLayout.FixedPos = rectClient.TopLeft;
        winLayout.FixedPos.X += (rectClient.Width() - 3);
        winLayout.FixedPos.Y += 3;
        New GUIBorder( m_arrBorderModels[GUIBORDER_RIGHT], m_pWindowRoot, winLayout, TEXT("<Border_Right>") );
            // TopLeft
        winLayout.FixedSize.X = 3;
        winLayout.FixedSize.Y = 3;
        winLayout.FixedPos = rectClient.TopLeft;
        New GUICorner( m_arrCornerModels[GUICORNER_TOPLEFT], m_pWindowRoot, winLayout, TEXT("<Corner_TopLeft>") );
            // BottomLeft
        winLayout.FixedSize.X = 3;
        winLayout.FixedSize.Y = 3;
        winLayout.FixedPos = rectClient.TopLeft;
        winLayout.FixedPos.Y += (rectClient.Height() - 3);
        New GUICorner( m_arrCornerModels[GUICORNER_BOTTOMLEFT], m_pWindowRoot, winLayout, TEXT("<Corner_BottomLeft>") );
            // BottomRight
        winLayout.FixedSize.X = 3;
        winLayout.FixedSize.Y = 3;
        winLayout.FixedPos = rectClient.TopLeft;
        winLayout.FixedPos.X += (rectClient.Width() - 3);
        winLayout.FixedPos.Y += (rectClient.Height() - 3);
        New GUICorner( m_arrCornerModels[GUICORNER_BOTTOMRIGHT], m_pWindowRoot, winLayout, TEXT("<Corner_BottomRight>") );
            // TopRight
        winLayout.FixedSize.X = 3;
        winLayout.FixedSize.Y = 3;
        winLayout.FixedPos = rectClient.TopLeft;
        winLayout.FixedPos.X += (rectClient.Width() - 3);
        New GUICorner( m_arrCornerModels[GUICORNER_TOPRIGHT], m_pWindowRoot, winLayout, TEXT("<Corner_TopRight>") );

        // Update client rect
        rectClient.TopLeft.X += 3;
        rectClient.TopLeft.Y += 3;
        rectClient.DownRight.X -= 3;
        rectClient.DownRight.Y -= 3;
    }

    // TitleBar
    if ( m_iWinStyle & GUIWINDOW_STYLE_TITLEBAR ) {
        // Model
        m_pTitleBarModel = New GUITitleBarModel( m_strName );

        // Layout
        winLayout.FixedSize.X = rectClient.Width();
        winLayout.FixedSize.Y = 20;
        winLayout.FixedPos = rectClient.TopLeft;
        New GUITitleBar( m_pTitleBarModel, m_pWindowRoot, winLayout, TEXT("<TitleBar>") );

        // Update client rect
        rectClient.TopLeft.Y += 20;
    }

    // MenuBar
    /////////////////////////////////////////

    // StatusBar
    /////////////////////////////////////////

    // ClientRoot
    winLayout.FixedSize.X = rectClient.Width();
    winLayout.FixedSize.Y = rectClient.Height();
    winLayout.FixedPos = rectClient.TopLeft;
    m_pClientRoot = New GUIWidget( NULL, m_pWindowRoot, winLayout, TEXT("<ClientRoot>") );
}

Void GUIWindow::_PostEvent_KeyPress( KeyCode iKey )
{
    // Filter window keys
    switch( iKey ) {
        case KEYCODE_PRNTSCR: {
                // Ignore
                return;
            } break;
        default: break;
    }

    // Transmit any other
    if ( m_bEventBlocking ) {
        if ( m_pFocus != NULL )
            m_pFocus->_PostEvent_KeyPress( iKey );
    } else {
        if ( m_pFocus != NULL && m_pFocus != m_pClientRoot )
            m_pFocus->_PostEvent_KeyPress( iKey );
        else
            GUIFn->GetBackboard()->_PostEvent_KeyPress( iKey );
        GUIFn->SetFocusWindow( NULL );
    }
}
Void GUIWindow::_PostEvent_KeyRelease( KeyCode iKey )
{
    // Filter window keys
    switch( iKey ) {
        case KEYCODE_PRNTSCR: {
                // Ignore
                return;
            } break;
        default: break;
    }

    // Transmit any other
    if ( m_bEventBlocking ) {
        if ( m_pFocus != NULL )
            m_pFocus->_PostEvent_KeyRelease( iKey );
    } else {
        if ( m_pFocus != NULL && m_pFocus != m_pClientRoot )
            m_pFocus->_PostEvent_KeyRelease( iKey );
        else
            GUIFn->GetBackboard()->_PostEvent_KeyRelease( iKey );
        GUIFn->SetFocusWindow( NULL );
    }
}

Void GUIWindow::_PostEvent_MouseMove( const Point2 & ptScreenPos )
{
    GUIWidget * pTarget = PickContainer( ptScreenPos );

    // Track mouse-over Widget
    if ( m_pMousedOver != pTarget ) {
        if ( m_pMousedOver != NULL )
            m_pMousedOver->_PostEvent_RollOut();
        if ( pTarget != NULL )
            pTarget->_PostEvent_RollIn();
        m_pMousedOver = pTarget;
    }

    if ( m_bEventBlocking ) {
        if ( m_pMousedOver != NULL )
            m_pMousedOver->_PostEvent_MouseMove( ptScreenPos );
    } else {
        if ( m_pMousedOver != NULL && m_pMousedOver != m_pClientRoot )
            m_pMousedOver->_PostEvent_MouseMove( ptScreenPos );
        else
            GUIFn->GetBackboard()->_PostEvent_MouseMove( ptScreenPos );
    }
}
Void GUIWindow::_PostEvent_MousePress( const Point2 & ptScreenPos, KeyCode iKey )
{
    GUIWidget * pTarget = PickContainer( ptScreenPos );

    // Track focus Widget
    if ( m_pFocus != pTarget ) {
        if ( m_pFocus != NULL )
            m_pFocus->_PostEvent_FocusLoss();
        if ( pTarget != NULL )
            pTarget->_PostEvent_FocusGain();
        m_pFocus = pTarget;
    }

    if ( m_bEventBlocking ) {
        if ( m_pFocus != NULL )
            m_pFocus->_PostEvent_MousePress( ptScreenPos, iKey );
    } else {
        if ( m_pFocus != NULL && m_pFocus != m_pClientRoot )
            m_pFocus->_PostEvent_MousePress( ptScreenPos, iKey );
        else
            GUIFn->GetBackboard()->_PostEvent_MousePress( ptScreenPos, iKey );
        GUIFn->SetFocusWindow( NULL );
    }
}
Void GUIWindow::_PostEvent_MouseRelease( const Point2 & ptScreenPos, KeyCode iKey )
{
    GUIWidget * pTarget = PickContainer( ptScreenPos );

    // Track focus Widget
    if ( m_pFocus != pTarget ) {
        if ( m_pFocus != NULL )
            m_pFocus->_PostEvent_FocusLoss();
        if ( pTarget != NULL )
            pTarget->_PostEvent_FocusGain();
        m_pFocus = pTarget;
    }

    if ( m_bEventBlocking ) {
        if ( m_pFocus != NULL )
            m_pFocus->_PostEvent_MouseRelease( ptScreenPos, iKey );
    } else {
        if ( m_pFocus != NULL && m_pFocus != m_pClientRoot )
            m_pFocus->_PostEvent_MouseRelease( ptScreenPos, iKey );
        else
            GUIFn->GetBackboard()->_PostEvent_MouseRelease( ptScreenPos, iKey );
        GUIFn->SetFocusWindow( NULL );
    }
}
Void GUIWindow::_PostEvent_MouseDblClick( const Point2 & ptScreenPos, KeyCode iKey )
{
    GUIWidget * pTarget = PickContainer( ptScreenPos );

    // Track focus Widget
    if ( m_pFocus != pTarget ) {
        if ( m_pFocus != NULL )
            m_pFocus->_PostEvent_FocusLoss();
        if ( pTarget != NULL )
            pTarget->_PostEvent_FocusGain();
        m_pFocus = pTarget;
    }

    if ( m_bEventBlocking ) {
        if ( m_pFocus != NULL )
            m_pFocus->_PostEvent_MouseDblClick( ptScreenPos, iKey );
    } else {
        if ( m_pFocus != NULL && m_pFocus != m_pClientRoot )
            m_pFocus->_PostEvent_MouseDblClick( ptScreenPos, iKey );
        else
            GUIFn->GetBackboard()->_PostEvent_MouseDblClick( ptScreenPos, iKey );
        GUIFn->SetFocusWindow( NULL );
    }
}
Void GUIWindow::_PostEvent_MouseWheel( const Point2 & ptScreenPos, Int iWheelDelta )
{
    if ( m_bEventBlocking ) {
        if ( m_pMousedOver != NULL )
            m_pMousedOver->_PostEvent_MouseWheel( ptScreenPos, iWheelDelta );
    } else {
        if ( m_pMousedOver != NULL && m_pMousedOver != m_pClientRoot )
            m_pMousedOver->_PostEvent_MouseWheel( ptScreenPos, iWheelDelta );
        else
            GUIFn->GetBackboard()->_PostEvent_MouseWheel( ptScreenPos, iWheelDelta );
        GUIFn->SetFocusWindow( NULL );
    }

    //if ( m_bEventBlocking ) {
    //    if ( m_pFocus != NULL )
    //        m_pFocus->_PostEvent_MouseWheel( ptScreenPos, iWheelDelta );
    //} else {
    //    if ( m_pFocus != NULL && m_pFocus != m_pClientRoot )
    //        m_pFocus->_PostEvent_MouseWheel( ptScreenPos, iWheelDelta );
    //    else
    //        GUIFn->GetBackboard()->_PostEvent_MouseWheel( ptScreenPos, iWheelDelta );
    //    GUIFn->SetFocusWindow( NULL );
    //}
}

Void GUIWindow::_PostEvent_FocusGain()
{
    m_bHasFocus = true;

    GUIFn->_UpdateWindowDepth( this, GUIWINDOW_MIN_DEPTH );
}
Void GUIWindow::_PostEvent_FocusLoss()
{
    Assert( m_iDepth < GUIWINDOW_MAX_DEPTH );

    m_bHasFocus = false;

    GUIFn->_UpdateWindowDepth( this, m_iDepth + 1 );
}

Void GUIWindow::_PostEvent_RollIn()
{
    m_bHasMouseOver = true;
}
Void GUIWindow::_PostEvent_RollOut()
{
    m_bHasMouseOver = false;
}

Void GUIWindow::_Move( const Point2 & ptScreenPos )
{
    // Clamp to screen
    UInt iScreenWidth, iScreenHeight;
    GUIFn->GetScreenSize( &iScreenWidth, &iScreenHeight );
    UInt iWinWidth = m_rectWindow.Width();
    UInt iWinHeight = m_rectWindow.Height();

    UInt iX = ( ptScreenPos.X < 0 ) ? 0 : (UInt)( ptScreenPos.X );
    UInt iY = ( ptScreenPos.Y < 0 ) ? 0 : (UInt)( ptScreenPos.Y );
    if ( iX + iWinWidth > iScreenWidth )
        iX = iScreenWidth - iWinWidth;
    if ( iY + iWinHeight > iScreenHeight )
        iY = iScreenHeight - iWinHeight;

    // Move
    m_rectWindow.TopLeft.X = iX;
    m_rectWindow.TopLeft.Y = iY;
    m_rectWindow.DownRight.X = iX + ( iWinWidth - 1 );
    m_rectWindow.DownRight.Y = iY + ( iWinHeight - 1 );

    // Update
    m_pWindowRoot->_UpdateClientRect();
}
Void GUIWindow::_Resize( UInt iWidth, UInt iHeight )
{
    // Clamp to screen
    UInt iScreenWidth, iScreenHeight;
    GUIFn->GetScreenSize( &iScreenWidth, &iScreenHeight );
    UInt iWinX = (UInt)( m_rectWindow.TopLeft.X );
    UInt iWinY = (UInt)( m_rectWindow.TopLeft.Y );

    if ( iWidth < GUIWINDOW_MIN_WIDTH )
        iWidth = GUIWINDOW_MIN_WIDTH;
    if ( iHeight < GUIWINDOW_MIN_HEIGHT )
        iHeight = GUIWINDOW_MIN_HEIGHT;
    if ( iWinX + iWidth > iScreenWidth )
        iWidth = iScreenWidth - iWinX;
    if ( iWinY + iHeight > iScreenHeight )
        iHeight = iScreenHeight - iWinY;

    // Resize
    m_rectWindow.DownRight.X = iWinX + ( iWidth - 1 );
    m_rectWindow.DownRight.Y = iWinY + ( iHeight - 1 );

    // Update
    m_pWindowRoot->_UpdateClientRect();
}
Void GUIWindow::_MoveResize( const Point2 & ptScreenPos, UInt iWidth, UInt iHeight )
{
    // Clamp to screen
    UInt iScreenWidth, iScreenHeight;
    GUIFn->GetScreenSize( &iScreenWidth, &iScreenHeight );

    UInt iX = ( ptScreenPos.X < 0 ) ? 0 : (UInt)( ptScreenPos.X );
    UInt iY = ( ptScreenPos.Y < 0 ) ? 0 : (UInt)( ptScreenPos.Y );
    if ( iX + iWidth > iScreenWidth )
        iWidth = iScreenWidth - iX;
    if ( iY + iHeight > iScreenHeight )
        iHeight = iScreenHeight - iY;

    // Move & Resize
    m_rectWindow.TopLeft.X = iX;
    m_rectWindow.TopLeft.Y = iY;
    m_rectWindow.DownRight.X = iX + ( iWidth - 1 );
    m_rectWindow.DownRight.Y = iY + ( iHeight - 1 );

    // Update
    m_pWindowRoot->_UpdateClientRect();
}

Void GUIWindow::_Render()
{
    if ( !m_bIsVisible )
        return;

    m_pWindowRoot->_Render();
}

