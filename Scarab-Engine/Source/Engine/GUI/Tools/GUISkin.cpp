/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Tools/GUISkin.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Container for all resources needed for GUI rendering
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
#include "GUISkin.h"

#include "../GUIManager.h"

/////////////////////////////////////////////////////////////////////////////////
// GUISkin implementation
GUISkin::GUISkin( const GChar * strName ):
    m_hColorBrush( GUIFn->GetScreenContext() ), m_hTextBrush( GUIFn->GetScreenContext() ), m_hTextFormat()
{
    StringFn->NCopy( m_strName, strName, GUI_NAME_LENGTH - 1 );

    // Bitmap
    m_pSkinBitmap = GUIFn->GetScreenContext()->CreateBitmap( PIXEL_FMT_RGBA8UN, 10, 10, NULL, false );

    MemZero( m_arrLocations, GUISKIN_IMAGE_COUNT * sizeof(Rectangle2) );

    // Colors
    m_hColorBrush.Bind();

    MemZero( m_arrColors, GUISKIN_COLOR_COUNT * sizeof(Color4) );

    // Fonts
    m_hTextBrush.Bind();
    m_hTextFormat.Bind( LOCALENAME_EN_US, GPU2DFONTFAMILY_ARIAL, GPU2DFONT_WEIGHT_NORMAL, GPU2DFONT_STYLE_NORMAL, GPU2DFONT_STRETCH_NORMAL, 10.0f );
    m_hTextFormat.SetWordWrapping( GPU2DTEXT_WORDWRAPPING_NONE );

    MemZero( m_arrTextFormats, GUISKIN_FONT_COUNT * sizeof(GPU2DTextFormat*) );

    // Load skin
    _LoadSkin();
}
GUISkin::~GUISkin()
{
    // Fonts
    m_hTextFormat.UnBind();
    m_hTextBrush.UnBind();

    // Colors
    m_hColorBrush.UnBind();

    // Bitmap
    m_pSkinBitmap->UnBind();
    RenderingFn->GetScreen2DContext()->DestroyBitmap( m_pSkinBitmap );
}

/////////////////////////////////////////////////////////////////////////////////

Void GUISkin::_LoadSkin()
{
    GChar strResPath[RDB_PATH_LENGTH + 1];
    GChar * strResName = StringFn->Format( strResPath, TEXT("%s%s/%s/"), RDB_PACKAGE_NULL, RDB_LOCATION_GUI_SKINS, m_strName );

    // Skin texture
    StringFn->Copy( strResName, TEXT("SkinBitmap.bmap") );
    m_pSkinBitmap->Load( RDB_MASTER, strResPath );
    m_pSkinBitmap->Bind();

    m_arrLocations[GUISKIN_IMAGE_WINDOW_BACKGROUNDTITLE] = GUISKIN_LOCATION_WINDOW_BACKGROUNDTITLE;
    m_arrLocations[GUISKIN_IMAGE_WINDOW_BORDER] = GUISKIN_LOCATION_WINDOW_BORDER;
    m_arrLocations[GUISKIN_IMAGE_WINDOW_CORNER] = GUISKIN_LOCATION_WINDOW_CORNER;
    m_arrLocations[GUISKIN_IMAGE_MENU_BACKGROUND] = GUISKIN_LOCATION_MENU_BACKGROUND;
    m_arrLocations[GUISKIN_IMAGE_MENU_ARROW] = GUISKIN_LOCATION_MENU_ARROW;
    m_arrLocations[GUISKIN_IMAGE_SECTION_BACKGROUND] = GUISKIN_LOCATION_SECTION_BACKGROUND;
    m_arrLocations[GUISKIN_IMAGE_BUTTON_DOWN] = GUISKIN_LOCATION_BUTTON_DOWN;
    m_arrLocations[GUISKIN_IMAGE_BUTTON_UP] = GUISKIN_LOCATION_BUTTON_UP;
    m_arrLocations[GUISKIN_IMAGE_CHECKBOX_CLEARED] = GUISKIN_LOCATION_CHECKBOX_CLEARED;
    m_arrLocations[GUISKIN_IMAGE_CHECKBOX_TICKED] = GUISKIN_LOCATION_CHECKBOX_TICKED;
    m_arrLocations[GUISKIN_IMAGE_CHECKBOX_NEUTRAL] = GUISKIN_LOCATION_CHECKBOX_NEUTRAL;
    m_arrLocations[GUISKIN_IMAGE_RADIOBUTTON_CLEARED] = GUISKIN_LOCATION_RADIOBUTTON_CLEARED;
    m_arrLocations[GUISKIN_IMAGE_RADIOBUTTON_TICKED] = GUISKIN_LOCATION_RADIOBUTTON_TICKED;
    m_arrLocations[GUISKIN_IMAGE_TEXTINPUT_BACKGROUND] = GUISKIN_LOCATION_TEXTINPUT_BACKGROUND;
    m_arrLocations[GUISKIN_IMAGE_TEXTLOG_BACKGROUND] = GUISKIN_LOCATION_TEXTLOG_BACKGROUND;
    m_arrLocations[GUISKIN_IMAGE_LIST_BACKGROUND] = GUISKIN_LOCATION_LIST_BACKGROUND;
    m_arrLocations[GUISKIN_IMAGE_LIST_ARROWTOP] = GUISKIN_LOCATION_LIST_ARROWTOP;
    m_arrLocations[GUISKIN_IMAGE_LIST_ARROWBOTTOM] = GUISKIN_LOCATION_LIST_ARROWBOTTOM;
    m_arrLocations[GUISKIN_IMAGE_TREE_BACKGROUND] = GUISKIN_LOCATION_TREE_BACKGROUND;
    m_arrLocations[GUISKIN_IMAGE_TREE_EXPANDBOX] = GUISKIN_LOCATION_TREE_EXPANDBOX;
    m_arrLocations[GUISKIN_IMAGE_TREE_COLLAPSEBOX] = GUISKIN_LOCATION_TREE_COLLAPSEBOX;

    m_arrLocations[GUISKIN_IMAGE_ICON_UNDEFINED] = GUISKIN_LOCATION_ICON_UNDEFINED;
    m_arrLocations[GUISKIN_IMAGE_ICON_HELP] = Rectangle2( Point2(0,0), 1, 1 );
    m_arrLocations[GUISKIN_IMAGE_ICON_MINIMIZE] = Rectangle2( Point2(0,0), 1, 1 );
    m_arrLocations[GUISKIN_IMAGE_ICON_CLOSE] = Rectangle2( Point2(0,0), 1, 1 );

    m_arrLocations[GUISKIN_IMAGE_CURSOR_UNDEFINED] = GUISKIN_LOCATION_CURSOR_UNDEFINED;
    m_arrLocations[GUISKIN_IMAGE_CURSOR_DEFAULT] = Rectangle2( Point2(0,0), 1, 1 );

    // Colors
    m_arrColors[GUISKIN_COLOR_WINDOW_TITLE] = Color4::White;
    m_arrColors[GUISKIN_COLOR_MENU_TEXTACTIVE] = Color4::Black;
    m_arrColors[GUISKIN_COLOR_MENU_TEXTINACTIVE] = Color4::LightGray;
    m_arrColors[GUISKIN_COLOR_MENU_SELECTION] = Color4::Blue;
    m_arrColors[GUISKIN_COLOR_LABEL_TEXT] = Color4::Black;
    m_arrColors[GUISKIN_COLOR_BUTTON_TEXTUP] = Color4::Black;
    m_arrColors[GUISKIN_COLOR_BUTTON_TEXTDOWN] = Color4::White;
    m_arrColors[GUISKIN_COLOR_TEXT] = Color4::Black;
    m_arrColors[GUISKIN_COLOR_TEXTINPUT_TEXT] = Color4::Black;
    m_arrColors[GUISKIN_COLOR_TEXTINPUT_TEXTSELECTED] = Color4::White;
    m_arrColors[GUISKIN_COLOR_TEXTINPUT_SELECTION] = Color4::Blue;
    m_arrColors[GUISKIN_COLOR_TEXTINPUT_CONTRAST] = Color4::Red;
    m_arrColors[GUISKIN_COLOR_TEXTLOG_TEXT] = Color4::Black;
    m_arrColors[GUISKIN_COLOR_LIST_TEXT] = Color4::Black;
    m_arrColors[GUISKIN_COLOR_LIST_TEXTSELECTED] = Color4::White;
    m_arrColors[GUISKIN_COLOR_LIST_SELECTION] = Color4::Blue;
    m_arrColors[GUISKIN_COLOR_TREE_TEXT] = Color4::Black;
    m_arrColors[GUISKIN_COLOR_TREE_TEXTSELECTED] = Color4::White;
    m_arrColors[GUISKIN_COLOR_TREE_SELECTION] = Color4::Blue;
    m_arrColors[GUISKIN_COLOR_TREE_LINKS] = Color4::Red;

    // Fonts
    m_arrTextFormats[GUISKIN_FONT_WINDOWTITLE] = &m_hTextFormat;
    m_arrTextFormats[GUISKIN_FONT_MENU] = &m_hTextFormat;
    m_arrTextFormats[GUISKIN_FONT_LABEL] = &m_hTextFormat;
    m_arrTextFormats[GUISKIN_FONT_BUTTON] = &m_hTextFormat;
    m_arrTextFormats[GUISKIN_FONT_TEXT] = &m_hTextFormat;
    m_arrTextFormats[GUISKIN_FONT_TEXTINPUT] = &m_hTextFormat;
    m_arrTextFormats[GUISKIN_FONT_TEXTLOG] = &m_hTextFormat;
    m_arrTextFormats[GUISKIN_FONT_LIST] = &m_hTextFormat;
    m_arrTextFormats[GUISKIN_FONT_COMBOLIST] = &m_hTextFormat;
    m_arrTextFormats[GUISKIN_FONT_TREE] = &m_hTextFormat;
}

