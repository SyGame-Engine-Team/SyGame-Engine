/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Widgets/Window/GUIBorders.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Widget : Window-specific, Border & Corner overlaps
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
#include "GUIBorders.h"

#include "../../GUIManager.h"

/////////////////////////////////////////////////////////////////////////////////
// GUIBorderModel implementation
GUIBorderModel::GUIBorderModel( GUIBorderType iType ):
    GUIWidgetModel()
{
    m_iBorderType = iType;
}
GUIBorderModel::~GUIBorderModel()
{
    // nothing to do
}

Void GUIBorderModel::OnStartDrag( const Point2 & /*ptLocalPos*/, KeyCode iKey, GUIEventFlag /*iFlags*/ )
{
    if ( iKey != KEYCODE_MOUSELEFT )
        return;

    // Widget
    GUIBorder * pWidget = (GUIBorder*)_GetWidget();

    // Parent window
    GUIWindow * pWindow = pWidget->GetParentWindow();

    // Start dragging
    GUIResizeFlag iResizeFlags = 0;
    switch( m_iBorderType ) {
        case GUIBORDER_TOP:    iResizeFlags = GUIRESIZE_TOP; break;
        case GUIBORDER_LEFT:   iResizeFlags = GUIRESIZE_LEFT; break;
        case GUIBORDER_BOTTOM: iResizeFlags = GUIRESIZE_BOTTOM; break;
        case GUIBORDER_RIGHT:  iResizeFlags = GUIRESIZE_RIGHT; break;
        default: Assert(false); break;
    }
    GUIFn->EnterDragResize( pWindow, iResizeFlags );
}

/////////////////////////////////////////////////////////////////////////////////
// GUICornerModel implementation
GUICornerModel::GUICornerModel( GUICornerType iType ):
    GUIWidgetModel()
{
    m_iCornerType = iType;
}
GUICornerModel::~GUICornerModel()
{
    // nothing to do
}

Void GUICornerModel::OnStartDrag( const Point2 & /*ptLocalPos*/, KeyCode iKey, GUIEventFlag /*iFlags*/ )
{
    if ( iKey != KEYCODE_MOUSELEFT )
        return;

    // Widget
    GUICorner * pWidget = (GUICorner*)_GetWidget();

    // Parent window
    GUIWindow * pWindow = pWidget->GetParentWindow();

    // Start dragging
    GUIResizeFlag iResizeFlags = 0;
    switch( m_iCornerType ) {
        case GUICORNER_TOPLEFT:     iResizeFlags = (GUIRESIZE_TOP | GUIRESIZE_LEFT); break;
        case GUICORNER_BOTTOMLEFT:  iResizeFlags = (GUIRESIZE_BOTTOM | GUIRESIZE_LEFT); break;
        case GUICORNER_BOTTOMRIGHT: iResizeFlags = (GUIRESIZE_BOTTOM | GUIRESIZE_RIGHT); break;
        case GUICORNER_TOPRIGHT:    iResizeFlags = (GUIRESIZE_TOP | GUIRESIZE_RIGHT); break;
        default: Assert(false); break;
    }
    GUIFn->EnterDragResize( pWindow, iResizeFlags );
}

/////////////////////////////////////////////////////////////////////////////////
// GUIBorder implementation
GUIBorder::GUIBorder( GUIBorderModel * pModel, GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName ):
    GUIWidget( pModel, pParent, guiLayout, strName )
{
    // nothing to do
}
GUIBorder::~GUIBorder()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

Void GUIBorder::_Draw( const Rectangle2 & rectClient )
{
    GPU2DContext * pGUIContext = GUIFn->GetScreenContext();

    // Model
    GUIBorderModel * pModel = (GUIBorderModel*)_GetWidgetModel();

    // Skin
    const GUISkin * pSkin = GUIFn->GetSkin();

    GPU2DBitmap * pSkinBitmap = pSkin->GetSkinBitmap();
    const Rectangle2 & locBackground = pSkin->GetLocation( GUISKIN_IMAGE_WINDOW_BORDER );

    // Background
    GPU2DRectangle hDstRect = rectClient;

    GPU2DRectangle hSrcRect;
    switch( pModel->GetBorderType() ) {
        case GUIBORDER_TOP:
            hSrcRect.fLeft = (Float)( locBackground.TopLeft.X );
            hSrcRect.fTop = (Float)( locBackground.TopLeft.Y );
            hSrcRect.fRight = hSrcRect.fLeft + 7.0f;
            hSrcRect.fBottom = hSrcRect.fTop + 2.0f;
            break;
        case GUIBORDER_LEFT:
            hSrcRect.fLeft = (Float)( locBackground.TopLeft.X );
            hSrcRect.fTop = (Float)( locBackground.TopLeft.Y + 8 );
            hSrcRect.fRight = hSrcRect.fLeft + 2.0f;
            hSrcRect.fBottom = hSrcRect.fTop + 7.0f;
            break;
        case GUIBORDER_BOTTOM:
            hSrcRect.fLeft = (Float)( locBackground.TopLeft.X + 8 );
            hSrcRect.fTop = (Float)( locBackground.TopLeft.Y + 13 );
            hSrcRect.fRight = hSrcRect.fLeft + 7.0f;
            hSrcRect.fBottom = hSrcRect.fTop + 2.0f;
            break;
        case GUIBORDER_RIGHT:
            hSrcRect.fLeft = (Float)( locBackground.TopLeft.X + 13 );
            hSrcRect.fTop = (Float)( locBackground.TopLeft.Y );
            hSrcRect.fRight = hSrcRect.fLeft + 2.0f;
            hSrcRect.fBottom = hSrcRect.fTop + 7.0f;
            break;
        default: Assert(false); break;
    }

    pGUIContext->DrawBitmap( pSkinBitmap, &hDstRect, &hSrcRect );
}

/////////////////////////////////////////////////////////////////////////////////
// GUICorner implementation
GUICorner::GUICorner( GUICornerModel * pModel, GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName ):
    GUIWidget( pModel, pParent, guiLayout, strName )
{
    // nothing to do
}
GUICorner::~GUICorner()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

Void GUICorner::_Draw( const Rectangle2 & rectClient )
{
    GPU2DContext * pGUIContext = GUIFn->GetScreenContext();

    // Model
    GUICornerModel * pModel = (GUICornerModel*)_GetWidgetModel();

    // Skin
    const GUISkin * pSkin = GUIFn->GetSkin();

    GPU2DBitmap * pSkinBitmap = pSkin->GetSkinBitmap();
    const Rectangle2 & locBackground = pSkin->GetLocation( GUISKIN_IMAGE_WINDOW_CORNER );

    // Background
    GPU2DRectangle hDstRect = rectClient;

    GPU2DRectangle hSrcRect;
    switch( pModel->GetCornerType() ) {
        case GUICORNER_TOPLEFT:
            hSrcRect.fLeft = (Float)( locBackground.TopLeft.X );
            hSrcRect.fTop = (Float)( locBackground.TopLeft.Y );
            break;
        case GUICORNER_BOTTOMLEFT:
            hSrcRect.fLeft = (Float)( locBackground.TopLeft.X );
            hSrcRect.fTop = (Float)( locBackground.TopLeft.Y + 13 );
            break;
        case GUICORNER_BOTTOMRIGHT:
            hSrcRect.fLeft = (Float)( locBackground.TopLeft.X + 13 );
            hSrcRect.fTop = (Float)( locBackground.TopLeft.Y + 13 );
            break;
        case GUICORNER_TOPRIGHT:
            hSrcRect.fLeft = (Float)( locBackground.TopLeft.X + 13 );
            hSrcRect.fTop = (Float)( locBackground.TopLeft.Y );
            break;
        default: Assert(false); break;
    }
    hSrcRect.fRight = hSrcRect.fLeft + 2.0f;
    hSrcRect.fBottom = hSrcRect.fTop + 2.0f;

    pGUIContext->DrawBitmap( pSkinBitmap, &hDstRect, &hSrcRect );
}

