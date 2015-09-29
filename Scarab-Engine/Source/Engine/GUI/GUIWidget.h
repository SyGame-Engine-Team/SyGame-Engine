/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/GUIWidget.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Base class for all window-interface elements ...
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : TODO = GUIComboBox (needs GUIDropList) ...
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_GUI_GUIWIDGET_H
#define SCARAB_ENGINE_GUI_GUIWIDGET_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Lib/Datastruct/Table/TreeMap.h"

#include "GUIEventProcessor.h"

#include "Tools/GUILayout.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
enum GUIWidgetType
{
	GUIWIDGET_UNDEFINED = 0,

	GUIWIDGET_CONTAINER,
	GUIWIDGET_SECTION,
	GUIWIDGET_SWITCHTABS,

	GUIWIDGET_LABEL,

	GUIWIDGET_BUTTON,
	GUIWIDGET_CHECKBOX,
	GUIWIDGET_RADIOBUTTON,
	GUIWIDGET_COMBOLIST,

	GUIWIDGET_TEXTINPUT,
	GUIWIDGET_TEXTLOG,

	GUIWIDGET_LIST,
	GUIWIDGET_TREE,

	GUIWIDGET_CUSTOM,

    // Reserved (window assets)
    GUIWIDGET_WINDOW_BORDER,
    GUIWIDGET_WINDOW_CORNER,
    GUIWIDGET_WINDOW_TITLEBAR,
	GUIWIDGET_WINDOW_MENUBAR,
	GUIWIDGET_WINDOW_STATUSBAR,

	GUIWIDGET_COUNT
};

// Prototypes
class GUIWindow;

class GUIMenuModel;

class GUIWidgetModel;
class GUIWidget;

/////////////////////////////////////////////////////////////////////////////////
// The GUIWidgetModel class
class GUIWidgetModel : public GUIEventProcessorModel
{
public:
    GUIWidgetModel();
	virtual ~GUIWidgetModel();

    // Model & Controller
    /////////////////////////////////////////////

    // Context menu support
    inline Bool HasContextMenu() const;
    inline GUIMenuModel * GetContextMenu() const;

    Void SetContextMenu( GUIMenuModel * pContextMenuModel );

	virtual Void OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags );

protected:
    inline GUIWidget * _GetWidget() const;

    // Context menu support
    GUIMenuModel * m_pContextMenuModel;
};

/////////////////////////////////////////////////////////////////////////////////
// The GUIWidget class
class GUIWidget : public GUIEventProcessor
{
private:
    // Give access to GUIWindow
    friend class GUIWindow;
    GUIWidget( GUIWindow * pParentWindow, const GChar * strName );
public:
	GUIWidget( GUIWidgetModel * pModel, GUIWidget * Parent, const GUILayout & guiLayout, const GChar * strName );
	virtual ~GUIWidget();

    // Getters
    inline virtual GUIWidgetType GetType() const;

    inline UInt GetLevel() const;
    inline const Rectangle2 & GetClientRect() const;

    inline Bool IsRoot() const;
    inline Bool IsLeaf() const;

    inline Bool IsVisible() const;    
    virtual Void SetVisible( Bool bVisible );

    inline GUIWindow * GetParentWindow() const;
    inline GUIWidget * GetParent() const;

    inline UInt GetChildCount() const;
    inline Bool HasChild( const GChar * strName ) const;
    inline GUIWidget * GetChild( const GChar * strName ) const;

    inline Void EnumChildren() const;
    inline GUIWidget * EnumNextChild() const;

    // Widget local coordinates
    inline Void ScreenToLocal( Point2 * outLocalPos, const Point2 & ptScreenPos ) const;
    inline Void LocalToScreen( Point2 * outScreenPos, const Point2 & ptLocalPos ) const;

    // Picking interface
    GUIWidget * PickChild( const Point2 & ptScreenPos );

    // Child destruction
    Void DestroyChild( const GChar * strName );
    Void DestroyChildren();

protected:
    inline GUIWidgetModel * _GetWidgetModel() const;

    // Needed when overloading SetVisible
    inline Void _SetVisibleOverride( Bool bVisible );

    // Drawing interface
    virtual Void _Draw( const Rectangle2 & rectClient );

private:
    // Helpers
    typedef TreeMap<const GChar*, GUIWidget*> WidgetMap;
    inline static Int _Name_Comparator( const GChar* const & rLeft, const GChar* const & rRight, Void * pUserData );

    // Event-Handling interface
    inline virtual Void _ToLocalCoords( Point2 * outLocalPos, const Point2 & ptScreenPos ) const;

    Void _FloodResizeEvent();
    Void _FloodExitEvent();

    virtual Bool _ProcessNextEvent();

    // Updating interface
    Void _UpdateClientRect();

    // Rendering interface
    Void _Render();

    // Container data
    UInt m_iLevel;
    GUILayout * m_pLayout;
    Rectangle2 m_rectClient;

    Bool m_bIsVisible;

    GUIWindow * m_pParentWindow;
    GUIWidget * m_pParent;
    WidgetMap m_mapChildren;
    mutable WidgetMap::Iterator m_itEnumerate;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GUIWidget.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_GUI_GUIWIDGET_H
