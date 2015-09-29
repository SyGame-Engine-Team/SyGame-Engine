/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Tools/GUIMenuLoop.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Menu state management
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
#include "GUIMenuLoop.h"

#include "../GUIManager.h"

/////////////////////////////////////////////////////////////////////////////////
// GUIMenuLoop implementation
GUIMenuLoop::GUIMenuLoop()
{
    m_iLoopState = GUIMENU_STATE_NULL;
    m_pRootMenu = NULL;
}
GUIMenuLoop::~GUIMenuLoop()
{
    Assert( m_iLoopState == GUIMENU_STATE_NULL );
    // nothing to do
}

Void GUIMenuLoop::Start( GUIMenuModel * pModel, const GChar * strName, const Point2 & ptScreenPos )
{
    Assert( m_iLoopState == GUIMENU_STATE_NULL );

    GUIFn->SelectMemory( TEXT("LayoutData") );
    m_pRootMenu = New GUIMenu( pModel, strName, ptScreenPos );
    GUIFn->UnSelectMemory();

    _RegisterMenu( m_pRootMenu );

    m_pRootMenu->SetVisible( true );

    m_iLoopState = GUIMENU_STATE_LOOP;
}
Void GUIMenuLoop::Stop( Bool bAborting )
{
    Assert( m_iLoopState == GUIMENU_STATE_LOOP );

    if ( bAborting )
        ( (GUIMenuModel*)(m_pRootMenu->GetModel()) )->OnAbort();

    m_pRootMenu->Collapse();
    m_pRootMenu->SetVisible( false );

    m_iLoopState = GUIMENU_STATE_ZOMBIE;
}
Void GUIMenuLoop::Reset()
{
    Assert( m_iLoopState == GUIMENU_STATE_ZOMBIE );

    _UnregisterMenu( m_pRootMenu );

    GUIFn->SelectMemory( TEXT("LayoutData") );
    Delete( m_pRootMenu );
    GUIFn->UnSelectMemory();

    m_pRootMenu = NULL;

    m_iLoopState = GUIMENU_STATE_NULL;
}

/////////////////////////////////////////////////////////////////////////////////

Void GUIMenuLoop::_RegisterMenu( GUIMenu * pMenu )
{
    // Recurse
    GUIMenu * pSubMenu;
    UInt iItemCount = pMenu->GetItemCount();
    for( UInt i = 0; i < iItemCount; ++i ) {
        pSubMenu = pMenu->GetSubMenu(i);
        if ( pSubMenu != NULL )
            _RegisterMenu( pSubMenu );
    }

    // Register
    GUIFn->RegisterOverlay( pMenu );
}
Void GUIMenuLoop::_UnregisterMenu( GUIMenu * pMenu )
{
    // Recurse
    GUIMenu * pSubMenu;
    UInt iItemCount = pMenu->GetItemCount();
    for( UInt i = 0; i < iItemCount; ++i ) {
        pSubMenu = pMenu->GetSubMenu(i);
        if ( pSubMenu != NULL )
            _UnregisterMenu( pSubMenu );
    }

    // Unregister
    GUIFn->UnregisterOverlay( pMenu );
}
