/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/GUIWidget.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Base class for all window-interface elements ...
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
#include "GUIWidget.h"

#include "GUIManager.h"

/////////////////////////////////////////////////////////////////////////////////
// GUIWidgetModel implementation
GUIWidgetModel::GUIWidgetModel():
    GUIEventProcessorModel()
{
    m_pContextMenuModel = NULL;
}
GUIWidgetModel::~GUIWidgetModel()
{
    // nothing to do
}

Void GUIWidgetModel::SetContextMenu( GUIMenuModel * pContextMenuModel )
{
    // Ensure old menu is closed
    if ( m_pContextMenuModel != NULL ) {
        if ( GUIFn->GetMenuLoopModel() == m_pContextMenuModel )
            GUIFn->LeaveMenuLoop( true );
    }

    // Switch model
    m_pContextMenuModel = pContextMenuModel;
}

Void GUIWidgetModel::OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag /*iFlags*/ )
{
    if ( iKey != KEYCODE_MOUSERIGHT )
        return;

    if ( m_pContextMenuModel == NULL )
        return;

    // Widget
    GUIWidget * pWidget = _GetWidget();

    // Enter context menu
    Point2 ptScreenPos;
    pWidget->LocalToScreen( &ptScreenPos, ptLocalPos );

    GUIFn->EnterMenuLoop( m_pContextMenuModel, TEXT("<ContextMenu>"), ptScreenPos );
}

/////////////////////////////////////////////////////////////////////////////////
// GUIWidget implementation
GUIWidget::GUIWidget( GUIWindow * pParentWindow, const GChar * strName ):
	GUIEventProcessor( NULL, strName ), m_rectClient(), m_mapChildren(), m_itEnumerate()
{
    Assert( pParentWindow != NULL );

    // Depth in the container tree
    m_iLevel = 0;

    // Layout
    m_pLayout = NULL;
    m_rectClient = pParentWindow->GetWindowRect();

    // States
    m_bIsVisible = false;

    // Hierarchic links
    m_pParentWindow = pParentWindow;
    m_pParent = NULL;

    m_mapChildren.UseMemoryContext( GUIFn->GetMemoryContext(), TEXT("LayoutData") );
    m_mapChildren.SetComparator( _Name_Comparator, NULL );
    m_mapChildren.Create();
}
GUIWidget::GUIWidget( GUIWidgetModel * pModel, GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName ):
	GUIEventProcessor( pModel, strName ), m_rectClient(), m_mapChildren(), m_itEnumerate()
{
    Assert( pParent != NULL );

    // Depth in the container tree
    m_iLevel = ( pParent->m_iLevel + 1 );

    // Layout
    GUIFn->SelectMemory( TEXT("LayoutData") );
    switch( guiLayout.Kind() ) {
        //case GUILAYOUT_BOX: m_pLayout = New GUIBoxLayout( (const GUIBoxLayout &)guiLayout ); break;
        case GUILAYOUT_GRID: m_pLayout = New GUIGridLayout( (const GUIGridLayout &)guiLayout ); break;
        default: Assert(false); break;
    }
    GUIFn->UnSelectMemory();

    m_pLayout->ComputeLayout( &m_rectClient, pParent->m_rectClient );

    // State
    m_bIsVisible = false;

    // Hierarchic links
	m_pParentWindow = pParent->m_pParentWindow;
    m_pParent = pParent;
    Bool bInserted = pParent->m_mapChildren.Insert( GetName(), this );
    Assert( bInserted );

    m_mapChildren.UseMemoryContext( GUIFn->GetMemoryContext(), TEXT("LayoutData") );
    m_mapChildren.SetComparator( _Name_Comparator, NULL );
    m_mapChildren.Create();
}
GUIWidget::~GUIWidget()
{
    // Allways called from GUIWindow's destructor,
    // so we're already in LayoutData memory context

    // Recurse on children
    WidgetMap::Iterator it;
    for( it = m_mapChildren.Begin(); !(it.IsNull()); ++it )
        Delete( it.GetItem() );

    m_mapChildren.Destroy();

    // Layout
    if ( m_pLayout != NULL )
		Delete( m_pLayout );
}

Void GUIWidget::SetVisible( Bool bVisible )
{
    m_bIsVisible = bVisible;

    // Recurse
    WidgetMap::Iterator it;
    for( it = m_mapChildren.Begin(); !(it.IsNull()); ++it )
        it.GetItem()->SetVisible( bVisible );
}

GUIWidget * GUIWidget::PickChild( const Point2 & ptScreenPos )
{
    // Check visibility
    if ( !m_bIsVisible )
        return NULL;

    // Check parent region
    Bool bContainTest = m_rectClient.Contains( ptScreenPos );
    if ( !bContainTest )
        return NULL;

    // Check children regions
    WidgetMap::Iterator it;
    GUIWidget * pChild;
    for( it = m_mapChildren.Begin(); !(it.IsNull()); ++it ) {
        pChild = it.GetItem()->PickChild( ptScreenPos );
        if ( pChild != NULL )
            return pChild;
    }

    // Not in a child's region => parent's region
    return this;
}

Void GUIWidget::DestroyChild( const GChar * strName )
{
    GUIWidget * pChild = NULL;
    Bool bRemoved = m_mapChildren.Remove( strName, &pChild );
    Assert( bRemoved && pChild != NULL );
    
    GUIFn->SelectMemory( TEXT("LayoutData") );
    Delete( pChild );
    GUIFn->UnSelectMemory();
}
Void GUIWidget::DestroyChildren()
{
    WidgetMap::Iterator it;
    GUIWidget * pChild;

    GUIFn->SelectMemory( TEXT("LayoutData") );

    for( it = m_mapChildren.Begin(); !(it.IsNull()); ++it ) {
        pChild = it.GetItem();
        Assert( pChild != NULL );
        Delete( pChild );
    }
    m_mapChildren.Clear();

    GUIFn->UnSelectMemory();
}

/////////////////////////////////////////////////////////////////////////////////

Void GUIWidget::_Draw( const Rectangle2 & /*rectClient*/ )
{
    // Stub method : do nothing
}

/////////////////////////////////////////////////////////////////////////////////

Void GUIWidget::_FloodResizeEvent()
{
    _PostEvent_Resize();

    // Recurse
    WidgetMap::Iterator it;
    for( it = m_mapChildren.Begin(); !(it.IsNull()); ++it )
        it.GetItem()->_FloodResizeEvent();
}
Void GUIWidget::_FloodExitEvent()
{
    _PostEvent_Exit();

    // Recurse
    WidgetMap::Iterator it;
    for( it = m_mapChildren.Begin(); !(it.IsNull()); ++it )
        it.GetItem()->_FloodExitEvent();
}

Bool GUIWidget::_ProcessNextEvent()
{
    // Down-Top Recurse
    WidgetMap::Iterator it;
    for( it = m_mapChildren.Begin(); !(it.IsNull()); ++it ) {
        if ( it.GetItem()->_ProcessNextEvent() )
            return true;
    }

    // Handle an event
    return GUIEventProcessor::_ProcessNextEvent();
}

Void GUIWidget::_UpdateClientRect()
{
    // Update from parent
    if ( m_pParent == NULL )
        m_rectClient = m_pParentWindow->GetWindowRect();
    else
        m_pLayout->ComputeLayout( &m_rectClient, m_pParent->m_rectClient );

    // Transmit to children
    WidgetMap::Iterator it;
    for( it = m_mapChildren.Begin(); !(it.IsNull()); ++it )
        it.GetItem()->_UpdateClientRect();
}
Void GUIWidget::_Render()
{
    if ( !m_bIsVisible )
        return;

    // Draw the widget
    _Draw( m_rectClient );

    // Top-Down Recurse
    WidgetMap::Iterator it;
    for( it = m_mapChildren.Begin(); !(it.IsNull()); ++it )
        it.GetItem()->_Render();
}

