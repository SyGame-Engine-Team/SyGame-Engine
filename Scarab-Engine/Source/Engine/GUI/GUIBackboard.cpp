/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/GUIBackboard.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Virtual background representing the "desktop" region,
//               behind all windows ... (mostly for event tracking)
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
#include "GUIBackboard.h"

#include "GUIManager.h"

/////////////////////////////////////////////////////////////////////////////////
// GUIBackboardModel implementation
GUIBackboardModel::GUIBackboardModel():
    GUIEventProcessorModel()
{
    m_pContextMenuModel = NULL;
}
GUIBackboardModel::~GUIBackboardModel()
{
    // nothing to do
}

Void GUIBackboardModel::SetContextMenu( GUIMenuModel * pContextMenuModel )
{
    // Ensure old menu is closed
    if ( m_pContextMenuModel != NULL ) {
        if ( GUIFn->GetMenuLoopModel() == m_pContextMenuModel )
            GUIFn->LeaveMenuLoop( true );
    }

    // Switch model
    m_pContextMenuModel = pContextMenuModel;
}

Void GUIBackboardModel::OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag /*iFlags*/ )
{
    if ( iKey != KEYCODE_MOUSERIGHT )
        return;

    if ( m_pContextMenuModel == NULL )
        return;

    // Enter context menu (screen = local here)
    GUIFn->EnterMenuLoop( m_pContextMenuModel, TEXT("<ContextMenu>"), ptLocalPos );
}

/////////////////////////////////////////////////////////////////////////////////
// GUIBackboard implementation
GUIBackboard::GUIBackboard( GUIBackboardModel * pModel ):
    GUIEventProcessor( pModel, TEXT("<Backboard>") ), m_rectScreen()
{
    Assert( pModel != NULL );

    // Screen region
    UInt iWidth, iHeight;
    GUIFn->GetScreenSize( &iWidth, &iHeight );
    m_rectScreen.TopLeft.X = 0;
    m_rectScreen.TopLeft.Y = 0;
    m_rectScreen.DownRight.X = iWidth - 1;
    m_rectScreen.DownRight.Y = iHeight - 1;
}
GUIBackboard::~GUIBackboard()
{
    // nothing to do
}

Void GUIBackboard::SetModel( GUIBackboardModel * pModel )
{
    Assert( pModel != NULL );

    // Purge pending events
    Bool bProcessed = _ProcessNextEvent();
    while( bProcessed )
        bProcessed = _ProcessNextEvent();

    // Switch model
    ((GUIBackboardModel*)m_pModel)->_SetBackboard( NULL );
    m_pModel = pModel;
    pModel->_SetBackboard( this );
}
