/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Widgets/Common/GUIButton.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Widget : Common controls, Button
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
#include "GUIButton.h"

#include "../../GUIManager.h"

/////////////////////////////////////////////////////////////////////////////////
// GUIButtonModel implementation
GUIButtonModel::GUIButtonModel( const GChar * strText ):
    GUIWidgetModel()
{
    m_bPressed = false;

    GChar * pEnd = StringFn->NCopy( m_strText, strText, GUIBUTTON_MAX_LENGTH - 1 );
    m_iTextLength = ( pEnd - m_strText );
}
GUIButtonModel::~GUIButtonModel()
{
    // nothing to do
}

Void GUIButtonModel::SetText( const GChar * strText )
{
    // Set new text
    GChar * pEnd = StringFn->NCopy( m_strText, strText, GUIBUTTON_MAX_LENGTH - 1 );
    m_iTextLength = ( pEnd - m_strText );

    // Raise callback
    OnTextChange();
}

Void GUIButtonModel::OnMousePress( const Point2 & /*ptLocalPos*/, KeyCode iKey, GUIEventFlag /*iFlags*/ )
{
    if ( iKey != KEYCODE_MOUSELEFT )
        return;
    if ( m_bPressed )
        return;

    // Deal event
    m_bPressed = true;
}
Void GUIButtonModel::OnMouseRelease( const Point2 & /*ptLocalPos*/, KeyCode iKey, GUIEventFlag /*iFlags*/ )
{
    if ( iKey != KEYCODE_MOUSELEFT )
        return;
    if ( !m_bPressed )
        return;

    // Deal event
    m_bPressed = false;
}

Void GUIButtonModel::OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags )
{
    // Handle context menu
    if ( iKey == KEYCODE_MOUSERIGHT ) {
        GUIWidgetModel::OnClick( ptLocalPos, iKey, iFlags );
        return;
    }

    if ( iKey != KEYCODE_MOUSELEFT )
        return;

    // Deal event
    OnPress();
}

Void GUIButtonModel::OnPress()
{
    // stub method, do nothing
}
Void GUIButtonModel::OnTextChange()
{
    // stub method, do nothing
}

/////////////////////////////////////////////////////////////////////////////////
// GUIButton implementation
GUIButton::GUIButton( GUIButtonModel * pModel, GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName ):
    GUIWidget( pModel, pParent, guiLayout, strName )
{
    // nothing to do
}
GUIButton::~GUIButton()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

Void GUIButton::_Draw( const Rectangle2 & rectClient )
{
    GPU2DContext * pGUIContext = GUIFn->GetScreenContext();

    // Model
    GUIButtonModel * pModel = (GUIButtonModel*)_GetWidgetModel();

    GUISkinImage iBackground = pModel->IsPressed() ? GUISKIN_IMAGE_BUTTON_DOWN : GUISKIN_IMAGE_BUTTON_UP;

    GUISkinColor iTextColor = pModel->IsPressed() ? GUISKIN_COLOR_BUTTON_TEXTDOWN : GUISKIN_COLOR_BUTTON_TEXTUP;

    UInt iTextLength = 0;
    const GChar * strText = pModel->GetText( &iTextLength );

    // Skin
    const GUISkin * pSkin = GUIFn->GetSkin();

    GPU2DBitmap * pSkinBitmap = pSkin->GetSkinBitmap();
    const Rectangle2 & locBackground = pSkin->GetLocation( iBackground );

    GPU2DColor hTextColor = pSkin->GetColor( iTextColor );

    GPU2DSolidColorBrush * pTextBrush = pSkin->GetSkinTextBrush();
    GPU2DTextFormat * pTextFormat = pSkin->GetSkinTextFormat( GUISKIN_FONT_BUTTON );

    // Background
    GPU2DRectangle hDstRect = rectClient;
    GPU2DRectangle hSrcRect = locBackground;

    pGUIContext->DrawBitmap( pSkinBitmap, &hDstRect, &hSrcRect );

    // Text
    pTextFormat->SetTextAlignment( GPU2DTEXT_ALIGNMENT_CENTER );
    pTextFormat->SetParagraphAlignment( GPU2DTEXT_PARAGRAPHALIGNMENT_CENTER );

    pTextBrush->SetColor( &hTextColor );
    pGUIContext->DrawText( strText, iTextLength, pTextFormat, &hDstRect, pTextBrush, GPU2DTEXT_DRAWOPTION_CLIP );
}

