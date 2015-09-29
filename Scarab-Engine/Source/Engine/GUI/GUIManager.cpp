/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/GUIManager.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Core-Class for graphic user interface.
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
#include "GUIManager.h"

/////////////////////////////////////////////////////////////////////////////////
// GUIManager implementation
GUIManager::GUIManager():
    Manager<GUIManager>(), m_mapWindows(), m_hWindowDepthMap(), m_itEnumerate(), m_mapOverlays(),
    m_guiDragNDrop(), m_guiMoveResize(), m_guiMenuLoop(), m_guiTimerEvents()
{
    m_pGUIContext = NULL;

    m_pBackboardDefaultModel = NULL;
    m_pBackboard = NULL;

    m_pFocus = NULL;
    m_pMousedOver = NULL;
    m_pOverlayFocus = NULL;
    m_pOverlayMousedOver = NULL;

    m_pSkin = NULL;

    m_pMessageBox = NULL;
}
GUIManager::~GUIManager()
{
    // nothing to do
}

Void GUIManager::Initialize()
{
    // Rendering Context
    m_pGUIContext = RenderingFn->GetScreen2DContext();

    // Backboard
    SelectMemory( TEXT("LayoutData") );
    m_pBackboardDefaultModel = New GUIBackboardModel();
    m_pBackboard = New GUIBackboard( m_pBackboardDefaultModel );
    UnSelectMemory();

    // Window map
    m_mapWindows.UseMemoryContext( GetMemoryContext(), TEXT("Scratch") );
    m_mapWindows.SetComparator( _Name_Comparator, NULL );
    m_mapWindows.Create();

    m_hWindowDepthMap.UseMemoryContext( GetMemoryContext(), TEXT("Scratch") );
    m_hWindowDepthMap.SetComparator( _Depth_Comparator, NULL );
    m_hWindowDepthMap.Create();

    // Overlay map
    m_mapOverlays.UseMemoryContext( GetMemoryContext(), TEXT("Scratch") );
    m_mapOverlays.SetComparator( _Name_Comparator, NULL );
    m_mapOverlays.Create();

    m_hOverlayDepthMap.UseMemoryContext( GetMemoryContext(), TEXT("Scratch") );
    m_hOverlayDepthMap.SetComparator( _Depth_Comparator, NULL );
    m_hOverlayDepthMap.Create();

    // Skin
    SelectMemory( TEXT("SkinData") );
    m_pSkin = New GUISkin( GUISKIN_DEFAULT );
    UnSelectMemory();

    // Message-Box
    SelectMemory( TEXT("Scratch") );
    m_pMessageBox = New GUIMessageBox();
    UnSelectMemory();
}
Void GUIManager::Cleanup()
{
    // Message-Box
    SelectMemory( TEXT("Scratch") );
    Delete( m_pMessageBox );
    m_pMessageBox = NULL;
    UnSelectMemory();

    // Skin
    SelectMemory( TEXT("SkinData") );
    Delete( m_pSkin );
    m_pSkin = NULL;
    UnSelectMemory();

    // Overlay map
    m_hOverlayDepthMap.Destroy();
    m_mapOverlays.Destroy();

    // Window map
    m_hWindowDepthMap.Destroy();
    m_mapWindows.Destroy();

    // Backboard
    SelectMemory( TEXT("LayoutData") );
    Delete( m_pBackboard );
    m_pBackboard = NULL;
    Delete( m_pBackboardDefaultModel );
    m_pBackboardDefaultModel = NULL;
    UnSelectMemory();

    // Rendering Context
    m_pGUIContext = NULL;
}

GUIWindow * GUIManager::CreateWindow( const GChar * strName, const Rectangle2 & rectWindow, GUIWindowStyle iWinStyle )
{
    Assert( rectWindow.Width() <= RenderingFn->GetSwapChainDesc()->iWidth );
    Assert( rectWindow.Height() <= RenderingFn->GetSwapChainDesc()->iHeight );

    SelectMemory( TEXT("LayoutData") );
    GUIWindow * pWindow = New GUIWindow( strName, rectWindow, iWinStyle );
    UnSelectMemory();

    return pWindow;
}
Void GUIManager::DestroyWindow( GUIWindow * pWindow )
{
    Assert( pWindow != NULL );

    SelectMemory( TEXT("LayoutData") );
    Delete( pWindow );
    UnSelectMemory();
}

GUIWidget * GUIManager::CreateWidget( GUIWidgetType iType, GUIWidgetModel * pModel,
                                      GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName )
{
    SelectMemory( TEXT("LayoutData") );

    GUIWidget * pWidget = NULL;
    switch( iType ) {
        case GUIWIDGET_CONTAINER:   pWidget = New GUIContainer   (                               pParent, guiLayout, strName ); break;
        case GUIWIDGET_SECTION:     pWidget = New GUISection     ( (GUISectionModel*)    pModel, pParent, guiLayout, strName ); break;
        case GUIWIDGET_SWITCHTABS:  pWidget = New GUISwitchTabs  ( (GUISwitchTabsModel*) pModel, pParent, guiLayout, strName ); break;
        case GUIWIDGET_LABEL:       pWidget = New GUILabel       ( (GUILabelModel*)      pModel, pParent, guiLayout, strName ); break;
        case GUIWIDGET_BUTTON:      pWidget = New GUIButton      ( (GUIButtonModel*)     pModel, pParent, guiLayout, strName ); break;
        case GUIWIDGET_CHECKBOX:    pWidget = New GUICheckBox    ( (GUICheckBoxModel*)   pModel, pParent, guiLayout, strName ); break;
        case GUIWIDGET_RADIOBUTTON: pWidget = New GUIRadioButton ( (GUIRadioButtonModel*)pModel, pParent, guiLayout, strName ); break;
        case GUIWIDGET_COMBOLIST:   pWidget = New GUIComboList   ( (GUIComboListModel*)  pModel, pParent, guiLayout, strName ); break;
        case GUIWIDGET_TEXTINPUT:   pWidget = New GUITextInput   ( (GUITextInputModel*)  pModel, pParent, guiLayout, strName ); break;
        case GUIWIDGET_TEXTLOG:     pWidget = New GUITextLog     ( (GUITextLogModel*)    pModel, pParent, guiLayout, strName ); break;
        case GUIWIDGET_LIST:        pWidget = New GUIList        ( (GUIListModel*)       pModel, pParent, guiLayout, strName ); break;
        case GUIWIDGET_TREE:        pWidget = New GUITree        ( (GUITreeModel*)       pModel, pParent, guiLayout, strName ); break;
        default: Assert(false); break;
    }

    UnSelectMemory();

    return pWidget;
}

Void GUIManager::RegisterWindow( GUIWindow * pWindow )
{
    Assert( pWindow != NULL );

    if ( m_mapWindows.Contains(pWindow->GetName()) )
        return;

    Bool bInserted = m_mapWindows.Insert( pWindow->GetName(), pWindow );
    Assert( bInserted );
    bInserted = m_hWindowDepthMap.Insert( pWindow );
    Assert( bInserted );
}
Void GUIManager::UnregisterWindow( GUIWindow * pWindow )
{
    Assert( pWindow != NULL );

    if ( m_pFocus == pWindow ) {
        m_pFocus = NULL;
        pWindow->_PostEvent_FocusLoss();
    }
    if ( m_pMousedOver == pWindow ) {
        m_pMousedOver = NULL;
        pWindow->_PostEvent_RollOut();
    }

    Bool bRemoved = m_hWindowDepthMap.Remove( pWindow );
    Assert( bRemoved );
    bRemoved = m_mapWindows.Remove( pWindow->GetName() );
    Assert( bRemoved );
}

Void GUIManager::SetFocusWindow( GUIWindow * pWindow )
{
    if ( m_pFocus != pWindow ) {
        if ( m_pFocus != NULL )
            m_pFocus->_PostEvent_FocusLoss();
        else
            m_pBackboard->_PostEvent_FocusLoss();
        if ( pWindow != NULL )
            pWindow->_PostEvent_FocusGain();
        else
            m_pBackboard->_PostEvent_FocusGain();
        m_pFocus = pWindow;
    }
}

Void GUIManager::RegisterOverlay( GUIOverlay * pOverlay )
{
    Assert( pOverlay != NULL );

    if ( m_mapOverlays.Contains(pOverlay->GetName()) )
        return;

    Bool bInserted = m_mapOverlays.Insert( pOverlay->GetName(), pOverlay );
    Assert( bInserted );
    bInserted = m_hOverlayDepthMap.Insert( pOverlay );
    Assert( bInserted );
}
Void GUIManager::UnregisterOverlay( GUIOverlay * pOverlay )
{
    Assert( pOverlay != NULL );

    if ( m_pOverlayFocus == pOverlay ) {
        m_pOverlayFocus = NULL;
        pOverlay->_PostEvent_FocusLoss();
    }
    if ( m_pOverlayMousedOver == pOverlay ) {
        m_pOverlayMousedOver = NULL;
        pOverlay->_PostEvent_RollOut();
    }

    Bool bRemoved = m_hOverlayDepthMap.Remove( pOverlay );
    Assert( bRemoved );
    bRemoved = m_mapOverlays.Remove( pOverlay->GetName() );
    Assert( bRemoved );
}

GUIWindow * GUIManager::PickWindow( const Point2 & ptScreenPos ) const
{
    UInt iTopMostDepth = GUIWINDOW_MAX_DEPTH;
    GUIWindow * pTopMostWindow = NULL;

    WindowMap::Iterator it;
    GUIWindow * pCurWindow;
    Rectangle2 rectWindow;
    UInt iCurDepth;

    for( it = m_mapWindows.Begin(); !(it.IsNull()); ++it ) {
        pCurWindow = it.GetItem();
        if ( !(pCurWindow->IsVisible()) )
            continue;
        rectWindow = pCurWindow->GetWindowRect();
        if ( !(rectWindow.Contains(ptScreenPos)) )
            continue;
        iCurDepth = pCurWindow->GetDepth();
        if ( iCurDepth < iTopMostDepth ) {
            iTopMostDepth = iCurDepth;
            pTopMostWindow = pCurWindow;
            if ( iTopMostDepth == GUIWINDOW_MIN_DEPTH )
                break; // Must be the good one
        }
    }

    return pTopMostWindow;
}
GUIOverlay * GUIManager::PickOverlay( const Point2 & ptScreenPos ) const
{
    UInt iTopMostDepth = GUIOVERLAY_MAX_DEPTH;
    GUIOverlay * pTopMostOverlay = NULL;

    OverlayMap::Iterator it;
    GUIOverlay * pCurOverlay;
    Rectangle2 rectOverlay;
    UInt iCurDepth;

    for( it = m_mapOverlays.Begin(); !(it.IsNull()); ++it ) {
        pCurOverlay = it.GetItem();
        if ( !(pCurOverlay->IsVisible()) )
            continue;
        rectOverlay = pCurOverlay->GetOverlayRect();
        if ( !(rectOverlay.Contains(ptScreenPos)) )
            continue;
        iCurDepth = pCurOverlay->GetDepth();
        if ( iCurDepth < iTopMostDepth ) {
            iTopMostDepth = iCurDepth;
            pTopMostOverlay = pCurOverlay;
            if ( iTopMostDepth == GUIOVERLAY_MIN_DEPTH )
                break; // Must be the good one
        }
    }

    return pTopMostOverlay;
}

Void GUIManager::OnKeyPress( KeyCode iKey )
{
    // Filter interface keys
    switch( iKey ) {
        case KEYCODE_PRNTSCR: {
                ///////////////////////////
                return;
            } break;
        default: break;
    }

    // Special State : moving / resizing
    if ( m_guiMoveResize.IsActive() ) {
        // Ignore event emission
        return;
    }

    // Special State : menu loop
    if ( m_guiMenuLoop.IsZombie() )
        m_guiMenuLoop.Reset();
    if ( m_guiMenuLoop.IsActive() ) {
        GUIMenu * pSelectedMenu = m_guiMenuLoop.GetRootMenu()->GetSelectedMenu();
        if ( pSelectedMenu != NULL )
            pSelectedMenu->_PostEvent_KeyPress( iKey );
        return;
    }

    // Handle overlays
    if ( m_pOverlayFocus != NULL ) {
        m_pOverlayFocus->_PostEvent_KeyPress( iKey );
        return;
    }

    // Handle windows
    if ( m_pFocus != NULL )
        m_pFocus->_PostEvent_KeyPress( iKey );
    else
        m_pBackboard->_PostEvent_KeyPress( iKey );
}
Void GUIManager::OnKeyRelease( KeyCode iKey )
{
    // Filter interface keys
    switch( iKey ) {
        case KEYCODE_PRNTSCR: {
                ///////////////////////////
                return;
            } break;
        default: break;
    }

    // Special State : moving / resizing
    if ( m_guiMoveResize.IsActive() ) {
        // Ignore event emission
        return;
    }

    // Special State : menu loop
    if ( m_guiMenuLoop.IsZombie() )
        m_guiMenuLoop.Reset();
    if ( m_guiMenuLoop.IsActive() ) {
        GUIMenu * pSelectedMenu = m_guiMenuLoop.GetRootMenu()->GetSelectedMenu();
        if ( pSelectedMenu != NULL )
            pSelectedMenu->_PostEvent_KeyRelease( iKey );
        return;
    }

    // Handle overlays
    if ( m_pOverlayFocus != NULL ) {
        m_pOverlayFocus->_PostEvent_KeyRelease( iKey );
        return;
    }

    // Handle windows
    if ( m_pFocus != NULL )
        m_pFocus->_PostEvent_KeyRelease( iKey );
    else
        m_pBackboard->_PostEvent_KeyRelease( iKey );
}

Void GUIManager::OnMouseMove( const Point2 & ptScreenPos )
{
    // Special State : moving / resizing
    if ( m_guiMoveResize.IsActive() ) {
        // Update
        if ( m_guiMoveResize.IsMoving() )
            m_guiMoveResize.UpdateMove( ptScreenPos );
        if ( m_guiMoveResize.IsResizing() )
            m_guiMoveResize.UpdateResize( ptScreenPos );
        // Ignore event emission
        return;
    }

    // Special State : menu loop
    if ( m_guiMenuLoop.IsZombie() )
        m_guiMenuLoop.Reset();

    // Handle overlays
    GUIOverlay * pTargetOverlay = PickOverlay( ptScreenPos );

    // Track mouse-over Overlay
    if ( m_pOverlayMousedOver != pTargetOverlay ) {
        if ( m_pOverlayMousedOver != NULL )
            m_pOverlayMousedOver->_PostEvent_RollOut();
        if ( pTargetOverlay != NULL )
            pTargetOverlay->_PostEvent_RollIn();
        m_pOverlayMousedOver = pTargetOverlay;
    }

    if ( pTargetOverlay != NULL ) {
        pTargetOverlay->_PostEvent_MouseMove( ptScreenPos );
        return;
    }

    // Handle windows
    GUIWindow * pTargetWindow = PickWindow( ptScreenPos );

    // Track mouse-over Window
    if ( m_pMousedOver != pTargetWindow ) {
        if ( m_pMousedOver != NULL )
            m_pMousedOver->_PostEvent_RollOut();
        else
            m_pBackboard->_PostEvent_RollOut();
        if ( pTargetWindow != NULL )
            pTargetWindow->_PostEvent_RollIn();
        else
            m_pBackboard->_PostEvent_RollIn();
        m_pMousedOver = pTargetWindow;
    }

    if ( pTargetWindow != NULL )
        pTargetWindow->_PostEvent_MouseMove( ptScreenPos );
    else
        m_pBackboard->_PostEvent_MouseMove( ptScreenPos );
}
Void GUIManager::OnMousePress( const Point2 & ptScreenPos, KeyCode iKey )
{
    // Special State : moving / resizing
    if ( m_guiMoveResize.IsActive() ) {
        // Ignore event emission
        return;
    }

    // Special State : menu loop
    if ( m_guiMenuLoop.IsZombie() )
        m_guiMenuLoop.Reset();

    // Handle overlays
    GUIOverlay * pTargetOverlay = PickOverlay( ptScreenPos );

    // Track focus Overlay
    if ( m_pOverlayFocus != pTargetOverlay ) {
        if ( m_pOverlayFocus != NULL )
            m_pOverlayFocus->_PostEvent_FocusLoss();
        if ( pTargetOverlay != NULL )
            pTargetOverlay->_PostEvent_FocusGain();
        m_pOverlayFocus = pTargetOverlay;
    }

    // Special State : menu loop
    if ( m_guiMenuLoop.IsActive() ) {
        if ( (pTargetOverlay == NULL) || (pTargetOverlay->GetType() != GUIOVERLAY_MENU) ) {
            m_guiMenuLoop.Stop( true );
            m_guiMenuLoop.Reset();
        }
    }

    if ( pTargetOverlay != NULL ) {
        pTargetOverlay->_PostEvent_MousePress( ptScreenPos, iKey );
        return;
    }

    // Handle windows
	GUIWindow * pTargetWindow = PickWindow( ptScreenPos );

    // Track focus Window
    if ( m_pFocus != pTargetWindow ) {
        if ( m_pFocus != NULL )
            m_pFocus->_PostEvent_FocusLoss();
        else
            m_pBackboard->_PostEvent_FocusLoss();
        if ( pTargetWindow != NULL )
            pTargetWindow->_PostEvent_FocusGain();
        else
            m_pBackboard->_PostEvent_FocusGain();
        m_pFocus = pTargetWindow;
    }

    if ( pTargetWindow != NULL )
        pTargetWindow->_PostEvent_MousePress( ptScreenPos, iKey );
    else
        m_pBackboard->_PostEvent_MousePress( ptScreenPos, iKey );
}
Void GUIManager::OnMouseRelease( const Point2 & ptScreenPos, KeyCode iKey )
{
    // Special State : moving / resizing
    if ( m_guiMoveResize.IsActive() ) {
        // Finalize
        if ( iKey == KEYCODE_MOUSELEFT )
            LeaveDragMoveResize();
        // Ignore event emission
        return;
    }

    // Special State : menu loop
    if ( m_guiMenuLoop.IsZombie() )
        m_guiMenuLoop.Reset();

    // Handle overlays
    GUIOverlay * pTargetOverlay = PickOverlay( ptScreenPos );

    // Track focus Overlay
    if ( m_pOverlayFocus != pTargetOverlay ) {
        if ( m_pOverlayFocus != NULL )
            m_pOverlayFocus->_PostEvent_FocusLoss();
        if ( pTargetOverlay != NULL )
            pTargetOverlay->_PostEvent_FocusGain();
        m_pOverlayFocus = pTargetOverlay;
    }

    // Special State : menu loop
    if ( m_guiMenuLoop.IsActive() ) {
        if ( (pTargetOverlay == NULL) || (pTargetOverlay->GetType() != GUIOVERLAY_MENU) ) {
            m_guiMenuLoop.Stop( true );
            m_guiMenuLoop.Reset();
        }
    }

    if ( pTargetOverlay != NULL ) {
        pTargetOverlay->_PostEvent_MouseRelease( ptScreenPos, iKey );
        return;
    }

    // Handle windows
    GUIWindow * pTargetWindow = PickWindow( ptScreenPos );

    // Track focus Window
    if ( m_pFocus != pTargetWindow ) {
        if ( m_pFocus != NULL )
            m_pFocus->_PostEvent_FocusLoss();
        else
            m_pBackboard->_PostEvent_FocusLoss();
        if ( pTargetWindow != NULL )
            pTargetWindow->_PostEvent_FocusGain();
        else
            m_pBackboard->_PostEvent_FocusGain();
        m_pFocus = pTargetWindow;
    }

    if ( pTargetWindow != NULL )
        pTargetWindow->_PostEvent_MouseRelease( ptScreenPos, iKey );
    else
        m_pBackboard->_PostEvent_MouseRelease( ptScreenPos, iKey );
}
Void GUIManager::OnMouseDblClick( const Point2 & ptScreenPos, KeyCode iKey )
{
    // Special State : moving / resizing
    if ( m_guiMoveResize.IsActive() ) {
        // Ignore event emission
        return;
    }

    // Special State : menu loop
    if ( m_guiMenuLoop.IsZombie() )
        m_guiMenuLoop.Reset();

    // Handle overlays
    GUIOverlay * pTargetOverlay = PickOverlay( ptScreenPos );

    // Track focus Overlay
    if ( m_pOverlayFocus != pTargetOverlay ) {
        if ( m_pOverlayFocus != NULL )
            m_pOverlayFocus->_PostEvent_FocusLoss();
        if ( pTargetOverlay != NULL )
            pTargetOverlay->_PostEvent_FocusGain();
        m_pOverlayFocus = pTargetOverlay;
    }

    // Special State : menu loop
    if ( m_guiMenuLoop.IsActive() ) {
        if ( (pTargetOverlay == NULL) || (pTargetOverlay->GetType() != GUIOVERLAY_MENU) ) {
            m_guiMenuLoop.Stop( true );
            m_guiMenuLoop.Reset();
        }
    }

    if ( pTargetOverlay != NULL ) {
        pTargetOverlay->_PostEvent_MouseDblClick( ptScreenPos, iKey );
        return;
    }

    // Handle windows
	GUIWindow * pTargetWindow = PickWindow( ptScreenPos );

    // Track focus Window
    if ( m_pFocus != pTargetWindow ) {
        if ( m_pFocus != NULL )
            m_pFocus->_PostEvent_FocusLoss();
        else
            m_pBackboard->_PostEvent_FocusLoss();
        if ( pTargetWindow != NULL )
            pTargetWindow->_PostEvent_FocusGain();
        else
            m_pBackboard->_PostEvent_FocusGain();
        m_pFocus = pTargetWindow;
    }

	if ( pTargetWindow != NULL )
        pTargetWindow->_PostEvent_MouseDblClick( ptScreenPos, iKey );
    else
        m_pBackboard->_PostEvent_MouseDblClick( ptScreenPos, iKey );
}
Void GUIManager::OnMouseWheel( const Point2 & ptScreenPos, Int iWheelDelta )
{
    // Special State : moving / resizing
    if ( m_guiMoveResize.IsActive() ) {
        // Ignore event emission
        return;
    }

    // Special State : menu loop
    if ( m_guiMenuLoop.IsZombie() )
        m_guiMenuLoop.Reset();
    if ( m_guiMenuLoop.IsActive() ) {
        GUIMenu * pSelectedMenu = m_guiMenuLoop.GetRootMenu()->GetSelectedMenu();
        if ( pSelectedMenu != NULL )
            pSelectedMenu->_PostEvent_MouseWheel( ptScreenPos, iWheelDelta );
        return;
    }

    // Handle overlays
    if ( m_pOverlayMousedOver != NULL ) {
        m_pOverlayMousedOver->_PostEvent_MouseWheel( ptScreenPos, iWheelDelta );
        return;
    }

    // Handle windows
    if ( m_pMousedOver != NULL )
        m_pMousedOver->_PostEvent_MouseWheel( ptScreenPos, iWheelDelta );
    else
        m_pBackboard->_PostEvent_MouseWheel( ptScreenPos, iWheelDelta );
}

Void GUIManager::OnExit()
{
    // Special State : moving / resizing
    if ( m_guiMoveResize.IsActive() )
        LeaveDragMoveResize();

    // Special State : menu loop
    if ( m_guiMenuLoop.IsActive() )
        m_guiMenuLoop.Stop( true );
    if ( m_guiMenuLoop.IsZombie() )
        m_guiMenuLoop.Reset();

    // Flood with exit events
    OverlayMap::Iterator itOverlay;
    for( itOverlay = m_mapOverlays.Begin(); !(itOverlay.IsNull()); ++itOverlay )
        itOverlay.GetItem()->_PostEvent_Exit();

    WindowMap::Iterator itWindow;
    for( itWindow = m_mapWindows.Begin(); !(itWindow.IsNull()); ++itWindow )
        itWindow.GetItem()->_PostEvent_Exit();

    m_pBackboard->_PostEvent_Exit();
}

Void GUIManager::MoveWindow( GUIWindow * pWindow, const Point2 & ptScreenPos )
{
    // Ignore while moving / resizing
    if ( m_guiMoveResize.IsActive() )
        return;

    // Purge pending events
    Bool bProcessed = pWindow->_ProcessNextEvent();
    while( bProcessed )
        bProcessed = pWindow->_ProcessNextEvent();

    // Instant move
    pWindow->_Move( ptScreenPos );
}
Void GUIManager::ResizeWindow( GUIWindow * pWindow, UInt iWidth, UInt iHeight )
{
    // Ignore while moving / resizing
    if ( m_guiMoveResize.IsActive() )
        return;

    // Purge pending events
    Bool bProcessed = pWindow->_ProcessNextEvent();
    while( bProcessed )
        bProcessed = pWindow->_ProcessNextEvent();    

    // Instant resize
    pWindow->_Resize( iWidth, iHeight );
}

Void GUIManager::Update()
{
    Bool bProcessed = false;

    // Special State : menu loop
    if ( m_guiMenuLoop.IsZombie() )
        m_guiMenuLoop.Reset();

    // Process overlay events
    OverlayMap::Iterator itOverlay;
    for( itOverlay = m_mapOverlays.Begin(); !(itOverlay.IsNull()); ++itOverlay ) {
        bProcessed = itOverlay.GetItem()->_ProcessNextEvent();
        while ( bProcessed )
            bProcessed = itOverlay.GetItem()->_ProcessNextEvent();
    }

    // Process window events
    WindowMap::Iterator itWindow;
    for( itWindow = m_mapWindows.Begin(); !(itWindow.IsNull()); ++itWindow ) {
        bProcessed = itWindow.GetItem()->_ProcessNextEvent();
        while ( bProcessed )
            bProcessed = itWindow.GetItem()->_ProcessNextEvent();
    }

    // Process backboard events
    bProcessed = m_pBackboard->_ProcessNextEvent();
    while( bProcessed )
        bProcessed = m_pBackboard->_ProcessNextEvent();

    // Dispatch Timer events (no buffering)
    UInt iTimerCount = m_guiTimerEvents.GetTimerCount();
    GUITimer * pTimer;
    Scalar fTimeMS = (Scalar)( SystemFn->TimeAbsolute(TIMEUNIT_MILLISECONDS) );
    for( UInt i = 0; i < iTimerCount; ++i ) {
        pTimer = m_guiTimerEvents.GetTimer( i );
        if ( pTimer->pTarget != NULL ) {
            if ( ( pTimer->fLastTickMS == -1.0 ) ||
                 ( (fTimeMS - pTimer->fLastTickMS) >= pTimer->fIntervalMS ) ) {
                pTimer->pTarget->_PostEvent_Tick( i );
                pTimer->fLastTickMS = fTimeMS;
            }
        }
    }
}

Void GUIManager::Render()
{
    m_pGUIContext->Begin();
    
    WindowDepthMap::Iterator itWindow;
    for( itWindow = m_hWindowDepthMap.Begin(); !(itWindow.IsNull()); ++itWindow )
        itWindow.Get()->_Render();

    OverlayDepthMap::Iterator itOverlay;
    for( itOverlay = m_hOverlayDepthMap.Begin(); !(itOverlay.IsNull()); ++itOverlay )
        itOverlay.Get()->_Render();

    m_pGUIContext->End();
}

/////////////////////////////////////////////////////////////////////////////////

Void GUIManager::_UpdateWindowDepth( GUIWindow * pWindow, UInt iNewDepth )
{
    Bool bOk = m_hWindowDepthMap.Remove( pWindow );
    Assert( bOk );

    pWindow->m_iDepth = iNewDepth;

    bOk = m_hWindowDepthMap.Insert( pWindow );
    Assert( bOk );
}
Void GUIManager::_UpdateOverlayDepth( GUIOverlay * pOverlay, UInt iNewDepth )
{
    Bool bOk = m_hOverlayDepthMap.Remove( pOverlay );
    Assert( bOk );

    pOverlay->m_iDepth = iNewDepth;

    bOk = m_hOverlayDepthMap.Insert( pOverlay );
    Assert( bOk );
}

