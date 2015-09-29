/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Widgets/Common/GUICheckBox.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Widget : Common controls, Check-Box
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
#include "GUICheckBox.h"

#include "../../GUIManager.h"

/////////////////////////////////////////////////////////////////////////////////
// GUICheckBoxModel implementation
GUICheckBoxModel::GUICheckBoxModel( Bool bUseThirdState ):
    GUIWidgetModel()
{
    m_bUseThirdState = bUseThirdState;
    m_iState = GUICHECKBOX_CLEARED;
}
GUICheckBoxModel::~GUICheckBoxModel()
{
    // nothing to do
}

Void GUICheckBoxModel::SetCleared()
{
    // Set new state
    m_iState = GUICHECKBOX_CLEARED;

    // Raise callback
    OnStateChange();
}
Void GUICheckBoxModel::SetTicked()
{
    // Set new state
    m_iState = GUICHECKBOX_TICKED;

    // Raise callback
    OnStateChange();
}
Void GUICheckBoxModel::SetNeutral()
{
    Assert( m_bUseThirdState );

    // Set new state
    m_iState = GUICHECKBOX_NEUTRAL;

    // Raise callback
    OnStateChange();
}

Void GUICheckBoxModel::Toggle()
{
    // Set new state
    if ( m_bUseThirdState )
        m_iState = (GUICheckBoxState)( ((UInt)m_iState + 1) % 3 );
    else
        m_iState = (GUICheckBoxState)( ((UInt)m_iState + 1) % 2 );

    // Raise callback
    OnStateChange();
}

Void GUICheckBoxModel::OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags )
{
    // Handle context menu
    if ( iKey == KEYCODE_MOUSERIGHT ) {
        GUIWidgetModel::OnClick( ptLocalPos, iKey, iFlags );
        return;
    }

    if ( iKey != KEYCODE_MOUSELEFT )
        return;

    // Deal event
    Toggle();
}

Void GUICheckBoxModel::OnStateChange()
{
    // stub method, do nothing
}

/////////////////////////////////////////////////////////////////////////////////
// GUICheckBox implementation
GUICheckBox::GUICheckBox( GUICheckBoxModel * pModel, GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName ):
    GUIWidget( pModel, pParent, guiLayout, strName )
{
    // nothing to do
}
GUICheckBox::~GUICheckBox()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

Void GUICheckBox::_Draw( const Rectangle2 & rectClient )
{
    GPU2DContext * pGUIContext = GUIFn->GetScreenContext();

    // Model
    GUICheckBoxModel * pModel = (GUICheckBoxModel*)_GetWidgetModel();

    GUISkinImage iBackground = GUISKIN_IMAGE_COUNT;
    if ( pModel->IsCleared() )
        iBackground = GUISKIN_IMAGE_CHECKBOX_CLEARED;
    else if ( pModel->IsTicked() )
        iBackground = GUISKIN_IMAGE_CHECKBOX_TICKED;
    else {
        Assert( pModel->IsNeutral() );
        iBackground = GUISKIN_IMAGE_CHECKBOX_NEUTRAL;
    }

    // Skin
    const GUISkin * pSkin = GUIFn->GetSkin();

    GPU2DBitmap * pSkinBitmap = pSkin->GetSkinBitmap();
    const Rectangle2 & locBackground = pSkin->GetLocation( iBackground );

    // Background
    GPU2DRectangle hDstRect = rectClient;
    GPU2DRectangle hSrcRect = locBackground;

    pGUIContext->DrawBitmap( pSkinBitmap, &hDstRect, &hSrcRect );
}


