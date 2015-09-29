/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Widgets/Window/GUITitleBar.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Widget : Window-specific, Title Bar
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
#include "GUITitleBar.h"

#include "../../GUIManager.h"

/////////////////////////////////////////////////////////////////////////////////
// GUITitleBarModel implementation
GUITitleBarModel::GUITitleBarModel( const GChar * strTitle ):
    GUIWidgetModel()
{
    GChar * pEnd = StringFn->NCopy( m_strTitle, strTitle, GUI_NAME_LENGTH - 1 );
    m_iTitleLength = ( pEnd - m_strTitle );
}
GUITitleBarModel::~GUITitleBarModel()
{
    // nothing to do
}

Void GUITitleBarModel::SetTitle( const GChar * strTitle )
{
    // Set new title
    GChar * pEnd = StringFn->NCopy( m_strTitle, strTitle, GUI_NAME_LENGTH - 1 );
    m_iTitleLength = ( pEnd - m_strTitle );
}

Void GUITitleBarModel::OnStartDrag( const Point2 & /*ptLocalPos*/, KeyCode iKey, GUIEventFlag /*iFlags*/ )
{
    if ( iKey != KEYCODE_MOUSELEFT )
        return;

    // Widget
    GUITitleBar * pWidget = (GUITitleBar*)_GetWidget();

    // Parent window
    GUIWindow * pWindow = pWidget->GetParentWindow();

    // Start moving
    GUIFn->EnterDragMove( pWindow );
}

/////////////////////////////////////////////////////////////////////////////////
// GUITitleBar implementation
GUITitleBar::GUITitleBar( GUITitleBarModel * pModel, GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName ):
    GUIWidget( pModel, pParent, guiLayout, strName )
{
    // nothing to do
}
GUITitleBar::~GUITitleBar()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

Void GUITitleBar::_Draw( const Rectangle2 & rectClient )
{
    GPU2DContext * pGUIContext = GUIFn->GetScreenContext();

    // Model
    GUITitleBarModel * pModel = (GUITitleBarModel*)_GetWidgetModel();

    UInt iTitleLength = 0;
    const GChar * strTitle = pModel->GetTitle( &iTitleLength );

    // Skin
    const GUISkin * pSkin = GUIFn->GetSkin();

    GPU2DBitmap * pSkinBitmap = pSkin->GetSkinBitmap();
    const Rectangle2 & locBackground = pSkin->GetLocation( GUISKIN_IMAGE_WINDOW_BACKGROUNDTITLE );

    GPU2DColor hTextColor = pSkin->GetColor( GUISKIN_COLOR_WINDOW_TITLE );

    GPU2DSolidColorBrush * pTextBrush = pSkin->GetSkinTextBrush();
    GPU2DTextFormat * pTextFormat = pSkin->GetSkinTextFormat( GUISKIN_FONT_WINDOWTITLE );

    // Background
    GPU2DRectangle hDstRect = rectClient;
    GPU2DRectangle hSrcRect = locBackground;

    pGUIContext->DrawBitmap( pSkinBitmap, &hDstRect, &hSrcRect );

    // Title text
    pTextFormat->SetTextAlignment( GPU2DTEXT_ALIGNMENT_CENTER );
    pTextFormat->SetParagraphAlignment( GPU2DTEXT_PARAGRAPHALIGNMENT_CENTER );

    pTextBrush->SetColor( &hTextColor );
    pGUIContext->DrawText( strTitle, iTitleLength, pTextFormat, &hDstRect, pTextBrush, GPU2DTEXT_DRAWOPTION_CLIP );
}

