/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Widgets/Extended/GUISwitchTabs.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Widget : Extended controls, Switching Tabbed containers
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
#include "GUISwitchTabs.h"

#include "../../GUIManager.h"

/////////////////////////////////////////////////////////////////////////////////
// GUISwitchTabsButtonModel implementation
GUISwitchTabsButtonModel::GUISwitchTabsButtonModel( GUISwitchTabsModel * pSwitchTabsModel, UInt iTabIndex, const GChar * strText ):
    GUIButtonModel( strText )
{
    m_pSwitchTabsModel = pSwitchTabsModel;
    m_iTabIndex = iTabIndex;
}
GUISwitchTabsButtonModel::~GUISwitchTabsButtonModel()
{
    // nothing to do
}

Void GUISwitchTabsButtonModel::OnPress()
{
    // Default behaviour
    m_pSwitchTabsModel->SetSelectedTab( m_iTabIndex );
}

/////////////////////////////////////////////////////////////////////////////////
// GUISwitchTabsModel implementation
GUISwitchTabsModel::GUISwitchTabsModel( GUISectionModel ** arrTabs, const GChar ** arrTabNames, UInt iTabCount ):
    GUIWidgetModel()
{
    Assert( iTabCount <= GUISWITCHTABS_MAX_TABS );

    UInt i;

    m_iTabCount = iTabCount;
    for( i = 0; i < m_iTabCount; ++i )
        m_arrTabModels[i] = arrTabs[i];

    m_pButtonBarModel = New GUISectionModel();
    for( i = 0; i < m_iTabCount; ++i )
        m_arrButtonModels[i] = New GUISwitchTabsButtonModel( this, i, arrTabNames[i] );

    m_iSelectedTab = 0;
}
GUISwitchTabsModel::~GUISwitchTabsModel()
{
    UInt i;
    for( i = 0; i < m_iTabCount; ++i )
        Delete( m_arrTabModels[i] );

    Delete( m_pButtonBarModel );
    for( i = 0; i < m_iTabCount; ++i )
        Delete( m_arrButtonModels[i] );
}

Void GUISwitchTabsModel::SetSelectedTab( UInt iTab )
{
    if ( m_iSelectedTab == iTab )
        return;

    // Widget
    GUISwitchTabs * pWidget = (GUISwitchTabs*)_GetWidget();

    // Update selected tab
    UInt iOldTab = m_iSelectedTab;
    m_iSelectedTab = iTab;

    // Raise callback
    OnSwitch( iOldTab, m_iSelectedTab );

    // Update
    pWidget->_UpdateAfterSwitch( iOldTab, m_iSelectedTab );
}

UInt GUISwitchTabsModel::GetButtonBarHeight() const
{
    // Default value
    return GUISWITCHTABS_BUTTONBAR_HEIGHT;
}
UInt GUISwitchTabsModel::GetButtonWidth() const
{
    // Default value
    return GUISWITCHTABS_BUTTON_WIDTH;
}

Void GUISwitchTabsModel::OnSwitch( UInt /*iOldTab*/, UInt /*iNewTab*/ )
{
    // stub method, do nothing
}

/////////////////////////////////////////////////////////////////////////////////
// GUISwitchTabs implementation
GUISwitchTabs::GUISwitchTabs( GUISwitchTabsModel * pModel, GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName ):
    GUIWidget( pModel, pParent, guiLayout, strName )
{
    const Rectangle2 & rectClient = GetClientRect();

    m_iTabCount = pModel->GetTabCount();
    UInt i;

    GChar strTabName[GUI_NAME_LENGTH];

    UInt iButtonBarHeight = pModel->GetButtonBarHeight();
    UInt iButtonWidth = pModel->GetButtonWidth();
    if ( iButtonWidth == INVALID_OFFSET )
        iButtonWidth = ( rectClient.Width() / m_iTabCount );

    // Layout
    GUIGridLayout switchLayout;
    switchLayout.FillX = GUILAYOUT_FILL_FIXED;
    switchLayout.FillY = GUILAYOUT_FILL_FIXED;
    switchLayout.Anchor = GUILAYOUT_ANCHOR_FIXED;

    switchLayout.FixedPos.X = 0;
    switchLayout.FixedPos.Y = 0;
    switchLayout.FixedSize.X = rectClient.Width();
    switchLayout.FixedSize.Y = iButtonBarHeight;
    m_pButtonBar = (GUISection*)( GUIFn->CreateWidget( GUIWIDGET_SECTION, pModel->GetButtonBar(), this,
                                                       switchLayout, TEXT("<SwitchTabs_ButtonBar>") ) );

    switchLayout.FixedPos.X = 0;
    switchLayout.FixedPos.Y = 0;
    switchLayout.FixedSize.X = iButtonWidth;
    switchLayout.FixedSize.Y = iButtonBarHeight;
    for( i = 0; i < m_iTabCount; ++i ) {
        StringFn->Format( strTabName, TEXT("<SwitchTabs_Button_%d>"), i );
        GUIFn->CreateWidget( GUIWIDGET_BUTTON, pModel->GetButton(i), m_pButtonBar, switchLayout, strTabName );
        switchLayout.FixedPos.X += iButtonWidth;
    }

    switchLayout.FixedPos.X = 0;
    switchLayout.FixedPos.Y = iButtonBarHeight;
    switchLayout.FixedSize.X = rectClient.Width();
    switchLayout.FixedSize.Y = ( rectClient.Height() - iButtonBarHeight );
    for( i = 0; i < m_iTabCount; ++i ) {
        StringFn->Format( strTabName, TEXT("<SwitchTabs_Tab_%d>"), i );
        m_arrTabs[i] = (GUISection*)( GUIFn->CreateWidget( GUIWIDGET_SECTION, pModel->GetTab(i), this, switchLayout, strTabName ) );
    }
}
GUISwitchTabs::~GUISwitchTabs()
{
    // nothing to do
}

Void GUISwitchTabs::SetVisible( Bool bVisible )
{
    // Model
    GUISwitchTabsModel * pModel = (GUISwitchTabsModel*)_GetWidgetModel();

    _SetVisibleOverride( bVisible );

    m_pButtonBar->SetVisible( bVisible );
    m_arrTabs[pModel->GetSelectedTab()]->SetVisible( bVisible );
}

/////////////////////////////////////////////////////////////////////////////////

Void GUISwitchTabs::_Draw( const Rectangle2 & /*rectClient*/ )
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

Void GUISwitchTabs::_UpdateAfterSwitch( UInt iOldTab, UInt iNewTab )
{
    m_arrTabs[iOldTab]->SetVisible( false );
    m_arrTabs[iNewTab]->SetVisible( IsVisible() );
}

