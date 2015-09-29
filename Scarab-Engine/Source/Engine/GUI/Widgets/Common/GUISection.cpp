/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Widgets/Common/GUISection.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Widget : Common controls, Section
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
#include "GUISection.h"

#include "../../GUIManager.h"

/////////////////////////////////////////////////////////////////////////////////
// GUISectionModel implementation
GUISectionModel::GUISectionModel():
    GUIWidgetModel()
{
    // nothing to do
}
GUISectionModel::~GUISectionModel()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// GUISection implementation
GUISection::GUISection( GUISectionModel * pModel, GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName ):
    GUIWidget( pModel, pParent, guiLayout, strName )
{
    // nothing to do
}
GUISection::~GUISection()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

Void GUISection::_Draw( const Rectangle2 & rectClient )
{
    GPU2DContext * pGUIContext = GUIFn->GetScreenContext();

    // Model
    //GUISectionModel * pModel = (GUISectionModel*)_GetWidgetModel();

    // Skin
    const GUISkin * pSkin = GUIFn->GetSkin();

    GPU2DBitmap * pSkinBitmap = pSkin->GetSkinBitmap();
    const Rectangle2 & locBackground = pSkin->GetLocation( GUISKIN_IMAGE_SECTION_BACKGROUND );

    // Background
    GPU2DRectangle hDstRect = rectClient;
    GPU2DRectangle hSrcRect = locBackground;

    pGUIContext->DrawBitmap( pSkinBitmap, &hDstRect, &hSrcRect );
}

