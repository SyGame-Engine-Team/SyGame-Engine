/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/GUIManager.inl
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
// GUIManager implementation
inline const GChar * GUIManager::GetName() {
    return TEXT("GUIManager");
}
inline const GChar * GUIManager::GetMemoryBudget() {
    return TEXT (
"Heap:Scratch(4194304);Heap:Resources(4194304);\
Heap:ScreenData(4194304);\
Heap:LayoutData(4194304);Heap:SkinData(4194304);"
    );
}

inline GPU2DContext * GUIManager::GetScreenContext() const {
    return m_pGUIContext;
}
inline Void GUIManager::GetScreenSize( UInt * outWidth, UInt * outHeight ) const {
    m_pGUIContext->GetPixelSize( outWidth, outHeight );
}

inline GUIBackboard * GUIManager::GetBackboard() const {
    return m_pBackboard;
}
inline Void GUIManager::SetBackboardModel( GUIBackboardModel * pModel ) {
    if ( pModel == NULL )
        m_pBackboard->SetModel( m_pBackboardDefaultModel );
    else
        m_pBackboard->SetModel( pModel );
}

inline GUIWindow * GUIManager::GetWindow( const GChar * strName ) const {
    WindowMap::Iterator itWindow = m_mapWindows.Get( strName );
    if ( itWindow.IsNull() )
        return NULL;
    return itWindow.GetItem();
}
inline GUIWindow * GUIManager::GetFocusWindow() const {
    return m_pFocus;
}
inline GUIWindow * GUIManager::GetMouseOverWindow() const {
    return m_pMousedOver;
}

inline Void GUIManager::EnumWindows() const {
    m_itEnumerate = m_mapWindows.Begin();
}
inline GUIWindow * GUIManager::EnumNextWindow() const {
    if ( m_itEnumerate.IsNull() )
        return NULL;
    GUIWindow * pWindow = m_itEnumerate.GetItem();
    ++m_itEnumerate;
    return pWindow;
}

inline GUIOverlay * GUIManager::GetOverlay( const GChar * strName ) const {
    OverlayMap::Iterator itOverlay = m_mapOverlays.Get( strName );
    if ( itOverlay.IsNull() )
        return NULL;
    return itOverlay.GetItem();
}
inline GUIOverlay * GUIManager::GetFocusOverlay() const {
    return m_pOverlayFocus;
}
inline GUIOverlay * GUIManager::GetMouseOverOverlay() const {
    return m_pOverlayMousedOver;
}

inline Bool GUIManager::HasMessageBox() const {
    return m_pMessageBox->IsOpened();
}
inline Bool GUIManager::MessageBox( GUIMessageBoxType iType, const GChar * strMessage, GUIMessageBoxButtons iButtons, GUIMsgBoxCallback pfCallback, Void * pUserData ) {
    return m_pMessageBox->Open( iType, strMessage, iButtons, pfCallback, pUserData );
}
inline Bool GUIManager::DialogBox( const GUIMessageBoxDialogDeclaration * arrDialogFormItems, UInt iDialogFormItemCount, GUIMsgBoxDialogCallback pfCallback, Void * pUserData ) {
    return m_pMessageBox->OpenDialog( arrDialogFormItems, iDialogFormItemCount, pfCallback, pUserData );
}

inline const GUISkin * GUIManager::GetSkin() const {
    return m_pSkin;
}

inline UInt GUIManager::CreateTimer( GUIEventProcessor * pTarget, Scalar fIntervalMS ) {
    return m_guiTimerEvents.CreateTimer( pTarget, fIntervalMS );
}
inline Void GUIManager::DestroyTimer( UInt iTimerID ) {
    m_guiTimerEvents.DestroyTimer( iTimerID );
}

inline const GUITimer * GUIManager::GetTimer( UInt iTimerID ) const {
    return m_guiTimerEvents.GetTimer(iTimerID);
}

inline Bool GUIManager::IsDragPrepared( GUIDragChannel iChannel ) const {
    return m_guiDragNDrop.IsPrepared( iChannel );
}
inline Bool GUIManager::IsDragConfirmed( GUIDragChannel iChannel ) const {
    return m_guiDragNDrop.IsConfirmed( iChannel );
}

inline GUIEventProcessor * GUIManager::GetDragSource( GUIDragChannel iChannel ) const {
    return m_guiDragNDrop.GetSource( iChannel );
}
inline const Point2 & GUIManager::GetDragSourcePos( GUIDragChannel iChannel ) const {
    return m_guiDragNDrop.GetSourcePos( iChannel );
}

inline GUIEventProcessor * GUIManager::GetDragDest( GUIDragChannel iChannel ) const {
    return m_guiDragNDrop.GetDest( iChannel );
}
inline const Point2 & GUIManager::GetDragDestPos( GUIDragChannel iChannel ) const {
    return m_guiDragNDrop.GetDestPos( iChannel );
}

inline Void * GUIManager::GetDragData( GUIDragChannel iChannel ) const {
    return m_guiDragNDrop.GetUserData( iChannel );
}
inline Void GUIManager::SetDragData( GUIDragChannel iChannel, Void * pDragData ) {
    m_guiDragNDrop.SetUserData( iChannel, pDragData );
}

inline Void GUIManager::EnterDragMove( GUIWindow * pWindow ) {
    Assert( m_guiDragNDrop.IsConfirmed(GUIDRAG_LEFT) );
    if ( (pWindow->GetStyle() & GUIWINDOW_STYLE_MOVE) == 0 )
        return; // Window doesn't allow moving
    m_guiMoveResize.StartMoving( pWindow, m_guiDragNDrop.GetSourcePos(GUIDRAG_LEFT) );
}
inline Void GUIManager::EnterDragResize( GUIWindow * pWindow, GUIResizeFlag iFlags ) {
    Assert( m_guiDragNDrop.IsConfirmed(GUIDRAG_LEFT) );
    if ( (pWindow->GetStyle() & GUIWINDOW_STYLE_RESIZE) == 0 )
        return; // Window doesn't allow resizing
    m_guiMoveResize.StartResizing( pWindow, iFlags );
}
inline Void GUIManager::LeaveDragMoveResize() {
    Assert( m_guiDragNDrop.IsConfirmed(GUIDRAG_LEFT) );
    if ( m_guiMoveResize.IsMoving() )
        m_guiMoveResize.EndMoving();
    if ( m_guiMoveResize.IsResizing() )
        m_guiMoveResize.EndResizing();
    m_guiDragNDrop.Complete( GUIDRAG_LEFT );
}

inline Bool GUIManager::IsInMenuLoop() const {
    return m_guiMenuLoop.IsActive();
}
inline const GUIMenuModel * GUIManager::GetMenuLoopModel() const {
    if ( m_guiMenuLoop.IsActive() || m_guiMenuLoop.IsZombie() )
        return (const GUIMenuModel *)( m_guiMenuLoop.GetRootMenu()->GetModel() );
    return NULL;
}

inline Void GUIManager::EnterMenuLoop( GUIMenuModel * pModel, const GChar * strName, const Point2 & ptScreenPos ) {
    m_guiMenuLoop.Start( pModel, strName, ptScreenPos );
}
inline Void GUIManager::LeaveMenuLoop( Bool bAborting ) {
    m_guiMenuLoop.Stop( bAborting );
}

/////////////////////////////////////////////////////////////////////////////////

inline Int GUIManager::_Name_Comparator( const GChar* const & rLeft, const GChar* const & rRight, Void * /*pUserData*/ ) {
    return StringFn->NCmp( rLeft, rRight, GUI_NAME_LENGTH - 1 );
}
inline Int GUIManager::_Depth_Comparator( GUIWindow * const & rLeft, GUIWindow * const & rRight, Void * /*pUserData*/ ) {
    if ( rLeft->m_iDepth > rRight->m_iDepth )
        return +1;
    if ( rLeft->m_iDepth < rRight->m_iDepth )
        return -1;
    if ( rLeft < rRight )
        return +1;
    if ( rLeft > rRight )
        return -1;
    return 0;
}
inline Int GUIManager::_Depth_Comparator( GUIOverlay * const & rLeft, GUIOverlay * const & rRight, Void * /*pUserData*/ ) {
    if ( rLeft->m_iDepth > rRight->m_iDepth )
        return +1;
    if ( rLeft->m_iDepth < rRight->m_iDepth )
        return -1;
    if ( rLeft < rRight )
        return +1;
    if ( rLeft > rRight )
        return -1;
    return 0;
}

inline GUIDragNDrop * GUIManager::_GetDragNDrop() {
    return &m_guiDragNDrop;
}
