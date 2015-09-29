/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Widgets/Common/GUILabel.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Widget : Common controls, Text Label
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
#include "GUILabel.h"

#include "../../GUIManager.h"

/////////////////////////////////////////////////////////////////////////////////
// GUILabelModel implementation
GUILabelModel::GUILabelModel( const GChar * strText ):
    GUIWidgetModel()
{
    GChar * pEnd = StringFn->NCopy( m_strText, strText, GUILABEL_MAX_LENGTH - 1 );
    m_iLabelLength = ( pEnd - m_strText );
}
GUILabelModel::~GUILabelModel()
{
    // nothing to do
}

Void GUILabelModel::SetText( const GChar * strText )
{
    // Set new text
    GChar * pEnd = StringFn->NCopy( m_strText, strText, GUILABEL_MAX_LENGTH - 1 );
    m_iLabelLength = ( pEnd - m_strText );

    // Raise callback
    OnTextChange();
}

Void GUILabelModel::OnTextChange()
{
    // stub method, do nothing
}

/////////////////////////////////////////////////////////////////////////////////
// GUILabel implementation
GUILabel::GUILabel( GUILabelModel * pModel, GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName ):
    GUIWidget( pModel, pParent, guiLayout, strName )
{
    // nothing to do
}
GUILabel::~GUILabel()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

Void GUILabel::_Draw( const Rectangle2 & rectClient )
{
    GPU2DContext * pGUIContext = GUIFn->GetScreenContext();

    // Model
    GUILabelModel * pModel = (GUILabelModel*)_GetWidgetModel();

    UInt iTextLength = 0;
    const GChar * strText = pModel->GetText( &iTextLength );

    // Skin
    const GUISkin * pSkin = GUIFn->GetSkin();

    GPU2DColor hTextColor = pSkin->GetColor( GUISKIN_COLOR_LABEL_TEXT );

    GPU2DSolidColorBrush * pTextBrush = pSkin->GetSkinTextBrush();
    GPU2DTextFormat * pTextFormat = pSkin->GetSkinTextFormat( GUISKIN_FONT_LABEL );

    // Text
    GPU2DRectangle hDstRect = rectClient;

    pTextFormat->SetTextAlignment( GPU2DTEXT_ALIGNMENT_CENTER );
    pTextFormat->SetParagraphAlignment( GPU2DTEXT_PARAGRAPHALIGNMENT_CENTER );

    pTextBrush->SetColor( &hTextColor );
    pGUIContext->DrawText( strText, iTextLength, pTextFormat, &hDstRect, pTextBrush, GPU2DTEXT_DRAWOPTION_CLIP );
}

