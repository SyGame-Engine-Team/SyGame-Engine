/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Tools/GUISkin.h
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
// Header prelude
#ifndef SCARAB_ENGINE_GUI_TOOLS_GUISKIN_H
#define SCARAB_ENGINE_GUI_TOOLS_GUISKIN_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Math/Types/Discrete/Rectangle2.h"
#include "../../../Lib/Math/Types/Color/Color4.h"

#include "../../Rendering/2D/Resources/GPU2DBitmap.h"
#include "../../Rendering/2D/Resources/GPU2DText.h"

#include "../GUIEventProcessor.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Predefined skins
#define GUISKIN_DEFAULT TEXT("Default")

// Skin elements dimensions
#define GUISKIN_SIZE_TILE       32
#define GUISKIN_SIZE_ICON       32
#define GUISKIN_SIZE_ICON_SMALL 16
#define GUISKIN_SIZE_ICON_TINY   8
#define GUISKIN_SIZE_CURSOR     16

#define GUISKIN_MARGINS          2

#define GUISKIN_HSPACING(_width)  ( (_width) + (GUISKIN_MARGINS << 1) )
#define GUISKIN_HSPACING_ICON     GUISKIN_SIZE_ICON_SMALL

#define GUISKIN_VSPACING(_height) ( (_height) + (GUISKIN_MARGINS << 1) )
#define GUISKIN_VSPACING_ICON     GUISKIN_SIZE_ICON_SMALL

// Skin texture locations
#define GUISKIN_TEXTURE_SIZE 128

#define GUISKIN_LOCATION_WINDOW_BACKGROUNDTITLE Rectangle2( Point2(   0,   0 ), 32, 32 )
#define GUISKIN_LOCATION_WINDOW_BORDER          Rectangle2( Point2(  64,  64 ), 16, 16 )
#define GUISKIN_LOCATION_WINDOW_CORNER          Rectangle2( Point2(  80,  64 ), 16, 16 )
#define GUISKIN_LOCATION_MENU_BACKGROUND        Rectangle2( Point2(  32,   0 ), 32, 32 )
#define GUISKIN_LOCATION_MENU_ARROW             Rectangle2( Point2(  96,  64 ), 16, 16 )
//#define GUISKIN_LOCATION_MENU_SELECTION         Rectangle2( Point2(  64,   0 ), 32, 32 )
#define GUISKIN_LOCATION_SECTION_BACKGROUND     Rectangle2( Point2(  96,   0 ), 32, 32 )
#define GUISKIN_LOCATION_BUTTON_DOWN            Rectangle2( Point2(   0,  96 ), 32, 16 )
#define GUISKIN_LOCATION_BUTTON_UP              Rectangle2( Point2(   0, 112 ), 32, 16 )
#define GUISKIN_LOCATION_CHECKBOX_CLEARED       Rectangle2( Point2( 112,  64 ), 16, 16 )
#define GUISKIN_LOCATION_CHECKBOX_TICKED        Rectangle2( Point2(  64,  80 ), 16, 16 )
#define GUISKIN_LOCATION_CHECKBOX_NEUTRAL       Rectangle2( Point2(  80,  80 ), 16, 16 )
#define GUISKIN_LOCATION_RADIOBUTTON_CLEARED    Rectangle2( Point2(  96,  80 ), 16, 16 )
#define GUISKIN_LOCATION_RADIOBUTTON_TICKED     Rectangle2( Point2( 112,  80 ), 16, 16 )
#define GUISKIN_LOCATION_TEXTINPUT_BACKGROUND   Rectangle2( Point2(   0,  32 ), 32, 32 )
#define GUISKIN_LOCATION_TEXTLOG_BACKGROUND     Rectangle2( Point2(  32,  32 ), 32, 32 )
#define GUISKIN_LOCATION_LIST_BACKGROUND        Rectangle2( Point2(  64,  32 ), 32, 32 )
#define GUISKIN_LOCATION_LIST_ARROWTOP          Rectangle2( Point2(  48,  96 ),  8,  8 )
#define GUISKIN_LOCATION_LIST_ARROWBOTTOM       Rectangle2( Point2(  56,  96 ),  8,  8 )
#define GUISKIN_LOCATION_TREE_BACKGROUND        Rectangle2( Point2(  96,  32 ), 32, 32 )
#define GUISKIN_LOCATION_TREE_EXPANDBOX         Rectangle2( Point2(  32,  96 ),  8,  8 )
#define GUISKIN_LOCATION_TREE_COLLAPSEBOX       Rectangle2( Point2(  40,  96 ),  8,  8 )
#define GUISKIN_LOCATION_ICON_UNDEFINED         Rectangle2( Point2(   0,  64 ), 32, 32 )
#define GUISKIN_LOCATION_CURSOR_UNDEFINED       Rectangle2( Point2(  32,  64 ), 32, 32 )

// Skin images
enum GUISkinImage
{
    GUISKIN_IMAGE_WINDOW_BACKGROUNDTITLE = 0,
    GUISKIN_IMAGE_WINDOW_BORDER,
    GUISKIN_IMAGE_WINDOW_CORNER,

    GUISKIN_IMAGE_MENU_BACKGROUND,
    GUISKIN_IMAGE_MENU_ARROW,

    GUISKIN_IMAGE_SECTION_BACKGROUND,

    GUISKIN_IMAGE_BUTTON_DOWN,
    GUISKIN_IMAGE_BUTTON_UP,

    GUISKIN_IMAGE_CHECKBOX_CLEARED,
    GUISKIN_IMAGE_CHECKBOX_TICKED,
    GUISKIN_IMAGE_CHECKBOX_NEUTRAL,

    GUISKIN_IMAGE_RADIOBUTTON_CLEARED,
    GUISKIN_IMAGE_RADIOBUTTON_TICKED,

    GUISKIN_IMAGE_TEXTINPUT_BACKGROUND,

    GUISKIN_IMAGE_TEXTLOG_BACKGROUND,

    GUISKIN_IMAGE_LIST_BACKGROUND,
    GUISKIN_IMAGE_LIST_ARROWTOP,
    GUISKIN_IMAGE_LIST_ARROWBOTTOM,

    GUISKIN_IMAGE_TREE_BACKGROUND,
    GUISKIN_IMAGE_TREE_EXPANDBOX,
    GUISKIN_IMAGE_TREE_COLLAPSEBOX,

    GUISKIN_IMAGE_ICON_UNDEFINED,
    GUISKIN_IMAGE_ICON_HELP,
    GUISKIN_IMAGE_ICON_MINIMIZE,
    GUISKIN_IMAGE_ICON_CLOSE,

    GUISKIN_IMAGE_CURSOR_UNDEFINED,
    GUISKIN_IMAGE_CURSOR_DEFAULT,

    GUISKIN_IMAGE_COUNT
};

// Skin colors
enum GUISkinColor
{
    GUISKIN_COLOR_WINDOW_TITLE = 0,

    GUISKIN_COLOR_MENU_TEXTACTIVE,
    GUISKIN_COLOR_MENU_TEXTINACTIVE,
    GUISKIN_COLOR_MENU_SELECTION,

    GUISKIN_COLOR_LABEL_TEXT,

    GUISKIN_COLOR_BUTTON_TEXTUP,
    GUISKIN_COLOR_BUTTON_TEXTDOWN,

    GUISKIN_COLOR_TEXT,

    GUISKIN_COLOR_TEXTINPUT_TEXT,
    GUISKIN_COLOR_TEXTINPUT_TEXTSELECTED,
    GUISKIN_COLOR_TEXTINPUT_SELECTION,
    GUISKIN_COLOR_TEXTINPUT_CONTRAST,

    GUISKIN_COLOR_TEXTLOG_TEXT,

    GUISKIN_COLOR_LIST_TEXT,
    GUISKIN_COLOR_LIST_TEXTSELECTED,
    GUISKIN_COLOR_LIST_SELECTION,

    GUISKIN_COLOR_TREE_TEXT,
    GUISKIN_COLOR_TREE_TEXTSELECTED,
    GUISKIN_COLOR_TREE_SELECTION,
    GUISKIN_COLOR_TREE_LINKS,

    GUISKIN_COLOR_COUNT
};

// Skin fonts
#define GUISKIN_FONT_SIZE 16

enum GUISkinFont
{
    GUISKIN_FONT_WINDOWTITLE = 0,

    GUISKIN_FONT_MENU,

    GUISKIN_FONT_LABEL,

    GUISKIN_FONT_BUTTON,

    GUISKIN_FONT_TEXT,

    GUISKIN_FONT_TEXTINPUT,

    GUISKIN_FONT_TEXTLOG,

    GUISKIN_FONT_LIST,
    GUISKIN_FONT_COMBOLIST,

    GUISKIN_FONT_TREE,

    GUISKIN_FONT_COUNT
};

/////////////////////////////////////////////////////////////////////////////////
// The GUISkin class
class GUISkin
{
public:
    GUISkin( const GChar * strName );
    ~GUISkin();

    // Resources access
    inline GPU2DBitmap * GetSkinBitmap() const;
    inline const Rectangle2 & GetLocation( GUISkinImage iImageID ) const;

    inline GPU2DSolidColorBrush * GetSkinColorBrush() const;
    inline const Color4 & GetColor( GUISkinColor iColorID ) const;

    inline GPU2DSolidColorBrush * GetSkinTextBrush() const;
    inline GPU2DTextFormat * GetSkinTextFormat( GUISkinFont iFontID ) const;

private:
    // Helpers
    Void _LoadSkin();

    // Skin data
    GChar m_strName[GUI_NAME_LENGTH];

    GPU2DBitmap * m_pSkinBitmap;
    Rectangle2 m_arrLocations[GUISKIN_IMAGE_COUNT];

    mutable GPU2DSolidColorBrush m_hColorBrush;
    Color4 m_arrColors[GUISKIN_COLOR_COUNT];

    mutable GPU2DSolidColorBrush m_hTextBrush;
    GPU2DTextFormat m_hTextFormat;
    GPU2DTextFormat * m_arrTextFormats[GUISKIN_FONT_COUNT];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GUISkin.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_GUI_TOOLS_GUISKIN_H

