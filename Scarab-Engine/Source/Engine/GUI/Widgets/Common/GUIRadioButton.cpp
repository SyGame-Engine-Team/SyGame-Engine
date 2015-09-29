/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Widgets/Common/GUIRadioButton.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Widget : Common controls, Radio buttons
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
#include "GUIRadioButton.h"

#include "../../GUIManager.h"

/////////////////////////////////////////////////////////////////////////////////
// GUIRadioButtonGroup implementation
GUIRadioButtonGroup::GUIRadioButtonGroup():
    m_arrModels( 8 )
{
    m_iTicked = INVALID_OFFSET;

    m_arrModels.UseMemoryContext( GUIFn->GetMemoryContext(), TEXT("LayoutData") );
    m_arrModels.Create();
}
GUIRadioButtonGroup::~GUIRadioButtonGroup()
{
    m_arrModels.Destroy();
}

/////////////////////////////////////////////////////////////////////////////////
// GUIRadioButtonModel implementation
GUIRadioButtonModel::GUIRadioButtonModel( GUIRadioButtonGroup * pGroup ):
    GUIWidgetModel()
{
    m_bTicked = false;
    m_pGroup = pGroup;

    m_iGroupIndex = m_pGroup->m_arrModels.Count();
    m_pGroup->m_arrModels.Push( this );
    if ( m_pGroup->m_iTicked == INVALID_OFFSET ) {
        m_bTicked = true;
        m_pGroup->m_iTicked = m_iGroupIndex;
    }
}
GUIRadioButtonModel::~GUIRadioButtonModel()
{
    // nothing to do
}

Void GUIRadioButtonModel::SetTicked()
{
    if ( m_bTicked )
        return;

    // Ticked Widget
    GUIRadioButtonModel * pTickedModel = m_pGroup->GetTickedModel();

    // Set as ticked
    pTickedModel->m_bTicked = false;
    m_bTicked = true;
    m_pGroup->m_iTicked = m_iGroupIndex;

    // Raise callback
    OnStateChange();
}

Void GUIRadioButtonModel::OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags )
{
    // Handle context menu
    if ( iKey == KEYCODE_MOUSERIGHT ) {
        GUIWidgetModel::OnClick( ptLocalPos, iKey, iFlags );
        return;
    }

    if ( iKey != KEYCODE_MOUSELEFT )
        return;

    // Deal event
    SetTicked();
}

Void GUIRadioButtonModel::OnStateChange()
{
    // stub method, do nothing
}

/////////////////////////////////////////////////////////////////////////////////
// GUIRadioButton implementation
GUIRadioButton::GUIRadioButton( GUIRadioButtonModel * pModel, GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName ):
    GUIWidget( pModel, pParent, guiLayout, strName )
{
    // nothing to do
}
GUIRadioButton::~GUIRadioButton()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

Void GUIRadioButton::_Draw( const Rectangle2 & rectClient )
{
    GPU2DContext * pGUIContext = GUIFn->GetScreenContext();

    // Model
    GUIRadioButtonModel * pModel = (GUIRadioButtonModel*)_GetWidgetModel();

    GUISkinImage iBackground = pModel->IsTicked() ? GUISKIN_IMAGE_RADIOBUTTON_TICKED : GUISKIN_IMAGE_RADIOBUTTON_CLEARED;

    // Skin
    const GUISkin * pSkin = GUIFn->GetSkin();

    GPU2DBitmap * pSkinBitmap = pSkin->GetSkinBitmap();
    const Rectangle2 & locBackground = pSkin->GetLocation( iBackground );

    // Background
    GPU2DRectangle hDstRect = rectClient;
    GPU2DRectangle hSrcRect = locBackground;

    pGUIContext->DrawBitmap( pSkinBitmap, &hDstRect, &hSrcRect );
}

