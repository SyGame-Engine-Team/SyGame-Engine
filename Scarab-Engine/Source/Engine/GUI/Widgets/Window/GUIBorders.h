/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Widgets/Window/GUIBorders.h
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
// Header prelude
#ifndef SCARAB_ENGINE_GUI_WIDGETS_WINDOW_GUIBORDERS_H
#define SCARAB_ENGINE_GUI_WIDGETS_WINDOW_GUIBORDERS_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../GUIWidget.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
enum GUIBorderType
{
    GUIBORDER_TOP = 0,
    GUIBORDER_LEFT,
    GUIBORDER_BOTTOM,
    GUIBORDER_RIGHT,

    GUIBORDER_COUNT
};

enum GUICornerType
{
    GUICORNER_TOPLEFT = 0,
    GUICORNER_BOTTOMLEFT,
    GUICORNER_BOTTOMRIGHT,
    GUICORNER_TOPRIGHT,

    GUICORNER_COUNT
};

/////////////////////////////////////////////////////////////////////////////////
// The GUIBorderModel class
class GUIBorderModel : public GUIWidgetModel
{
public:
    GUIBorderModel( GUIBorderType iType );
    virtual ~GUIBorderModel();

    // Model
    inline GUIBorderType GetBorderType() const;

    // Controller
    virtual Void OnStartDrag( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags );

protected:
    GUIBorderType m_iBorderType;
};

/////////////////////////////////////////////////////////////////////////////////
// The GUICornerModel class
class GUICornerModel : public GUIWidgetModel
{
public:
    GUICornerModel( GUICornerType iType );
    virtual ~GUICornerModel();

    // Model
    inline GUICornerType GetCornerType() const;

    // Controller
    virtual Void OnStartDrag( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags );

protected:
    GUICornerType m_iCornerType;
};

/////////////////////////////////////////////////////////////////////////////////
// The GUIBorder class
class GUIBorder : public GUIWidget
{
public:
    GUIBorder( GUIBorderModel * pModel, GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName );
    virtual ~GUIBorder();

    // Type
	inline virtual GUIWidgetType GetType() const;

protected:
    // Drawing interface
    virtual Void _Draw( const Rectangle2 & rectClient );
};

/////////////////////////////////////////////////////////////////////////////////
// The GUICorner class
class GUICorner : public GUIWidget
{
public:
    GUICorner( GUICornerModel * pModel, GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName );
    virtual ~GUICorner();

    // Type
	inline virtual GUIWidgetType GetType() const;

protected:
    // Drawing interface
    virtual Void _Draw( const Rectangle2 & rectClient );
};


/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GUIBorders.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_GUI_WIDGETS_WINDOW_GUIBORDERS_H

