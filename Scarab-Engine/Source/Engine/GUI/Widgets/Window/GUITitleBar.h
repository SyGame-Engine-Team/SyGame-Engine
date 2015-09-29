/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Widgets/Window/GUITitleBar.h
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
// Header prelude
#ifndef SCARAB_ENGINE_GUI_WIDGETS_WINDOW_GUITITLEBAR_H
#define SCARAB_ENGINE_GUI_WIDGETS_WINDOW_GUITITLEBAR_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../GUIWidget.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Prototypes
class GUIMenuModel;

/////////////////////////////////////////////////////////////////////////////////
// The GUITitleBarModel class
class GUITitleBarModel : public GUIWidgetModel
{
public:
    GUITitleBarModel( const GChar * strTitle );
    virtual ~GUITitleBarModel();

    // Model
    inline const GChar * GetTitle( UInt * outLength = NULL ) const;
    Void SetTitle( const GChar * strTitle );

    //virtual Bool HasMinimizeButton() const;
    //virtual const ImageSurface * GetMinimizeIcon() const;

    //virtual Bool HasCloseButton() const;
    //virtual const ImageSurface * GetCloseIcon() const;

    //virtual Void OnTitleMenu() = 0;
    //virtual Void OnMinimize() = 0;
    //virtual Void OnMaximize() = 0;
    //virtual Void OnClose() = 0;

    // Controller
    virtual Void OnStartDrag( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags );

protected:
    UInt m_iTitleLength;
    GChar m_strTitle[GUI_NAME_LENGTH];
};

/////////////////////////////////////////////////////////////////////////////////
// The GUITitleBar class
class GUITitleBar : public GUIWidget
{
public:
    GUITitleBar( GUITitleBarModel * pModel, GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName );
    virtual ~GUITitleBar();

    // Type
	inline virtual GUIWidgetType GetType() const;

protected:
    // Drawing interface
    virtual Void _Draw( const Rectangle2 & rectClient );
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GUITitleBar.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_GUI_WIDGETS_WINDOW_GUITITLEBAR_H

