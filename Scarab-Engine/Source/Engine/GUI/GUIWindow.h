/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/GUIWindow.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GUIWindow is the root dynamic container for GUI layouts.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : TODO = MenuBar, StatusBar, Minimization, ...
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_GUI_GUIWINDOW_H
#define SCARAB_ENGINE_GUI_GUIWINDOW_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "GUIWidget.h"

// Widgets dedicated to windows
#include "Widgets/Window/GUIBorders.h"
#include "Widgets/Window/GUITitleBar.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define GUIWINDOW_MIN_WIDTH  16
#define GUIWINDOW_MIN_HEIGHT 16

#define GUIWINDOW_MIN_DEPTH 32
#define GUIWINDOW_MAX_DEPTH 255

// Window style flags
typedef DWord GUIWindowStyle;
#define GUIWINDOW_STYLE_NAKED     0x00000000

#define GUIWINDOW_STYLE_BORDER    0x00000001
#define GUIWINDOW_STYLE_TITLEBAR  0x00000002
#define GUIWINDOW_STYLE_MENUBAR   0x00000004
#define GUIWINDOW_STYLE_STATUSBAR 0x00000008

#define GUIWINDOW_STYLE_MOVE      0x00010000
#define GUIWINDOW_STYLE_RESIZE    0x00020000
#define GUIWINDOW_STYLE_DYNAMIC   ( GUIWINDOW_STYLE_MOVE | GUIWINDOW_STYLE_RESIZE )

#define GUIWINDOW_STYLE_DEFAULT   ( GUIWINDOW_STYLE_BORDER | GUIWINDOW_STYLE_TITLEBAR | GUIWINDOW_STYLE_MOVE )
#define GUIWINDOW_STYLE_DEFAULTEX ( GUIWINDOW_STYLE_DEFAULT | GUIWINDOW_STYLE_RESIZE )
#define GUIWINDOW_STYLE_FULL      ( GUIWINDOW_STYLE_DEFAULTEX | GUIWINDOW_STYLE_MENUBAR | GUIWINDOW_STYLE_STATUSBAR )

// Prototypes
class GUIMoveResize;
class GUIManager;

/////////////////////////////////////////////////////////////////////////////////
// The GUIWindow class
class GUIWindow
{
public:
    GUIWindow( const GChar * strName, const Rectangle2 & rectWindow, GUIWindowStyle iWinStyle );
    ~GUIWindow();

    // Getters
    inline const GChar * GetName() const;
    inline const Rectangle2 & GetWindowRect() const;
    inline UInt GetDepth() const;
    inline GUIWindowStyle GetStyle() const;

    inline Bool IsVisible() const;
    Void SetVisible( Bool bVisible );

    inline Bool HasFocus() const;
    inline GUIWidget * GetFocus() const;
    inline Bool HasMouseOver() const;
    inline GUIWidget * GetMouseOver() const;

    inline Bool IsEventBlocking() const;
    inline Void SetEventBlocking( Bool bEventBlocking );

    // Widget tree access
    inline GUIWidget * GetClientRoot() const;

    // Window local coordinates
    inline Void ScreenToWindow( Point2 * outWindowPos, const Point2 & ptScreenPos ) const;
    inline Void WindowToScreen( Point2 * outScreenPos, const Point2 & ptWindowPos ) const;

    // Picking interface
    inline GUIWidget * PickContainer( const Point2 & ptScreenPos ) const;

    // Context menu support
    inline Bool HasContextMenu() const;
    inline GUIMenuModel * GetContextMenu() const;
    inline Void SetContextMenu( GUIMenuModel * pContextMenuModel );

private:
    // Give access to GUIMoveResize & GUIManager
    friend class GUIMoveResize;
    friend class GUIManager;

    // Helpers
    Void _BuildOverlappedWindow();

    // Event-Handling interface
    Void _PostEvent_KeyPress( KeyCode iKey );
    Void _PostEvent_KeyRelease( KeyCode iKey );

    Void _PostEvent_MouseMove( const Point2 & ptScreenPos );
    Void _PostEvent_MousePress( const Point2 & ptScreenPos, KeyCode iKey );
    Void _PostEvent_MouseRelease( const Point2 & ptScreenPos, KeyCode iKey );
    Void _PostEvent_MouseDblClick( const Point2 & ptScreenPos, KeyCode iKey );
    Void _PostEvent_MouseWheel( const Point2 & ptScreenPos, Int iWheelDelta );

    Void _PostEvent_FocusGain();
    Void _PostEvent_FocusLoss();

    Void _PostEvent_RollIn();
    Void _PostEvent_RollOut();

    inline Void _PostEvent_Resize();
    inline Void _PostEvent_Exit();

    inline Bool _ProcessNextEvent();

    // Move-Resize interface
    Void _Move( const Point2 & ptScreenPos );
    Void _Resize( UInt iWidth, UInt iHeight );
    Void _MoveResize( const Point2 & ptScreenPos, UInt iWidth, UInt iHeight );

    // Rendering interface
    Void _Render();

    // Window data
    GChar m_strName[GUI_NAME_LENGTH];
    Rectangle2 m_rectWindow;
    UInt m_iDepth;
    GUIWindowStyle m_iWinStyle;

    Bool m_bIsVisible;
    Bool m_bHasFocus;
    GUIWidget * m_pFocus;
    Bool m_bHasMouseOver;
    GUIWidget * m_pMousedOver;

    Bool m_bEventBlocking;

    GUIWidget * m_pWindowRoot;
    GUIWidget * m_pClientRoot;

    GUIBorderModel * m_arrBorderModels[GUIBORDER_COUNT];
    GUICornerModel * m_arrCornerModels[GUICORNER_COUNT];
    GUITitleBarModel * m_pTitleBarModel;
    //GUIMenuBarModel * m_pMenuBarModel;
    //GUIStateBarModel * m_pStateBarModel;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GUIWindow.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_GUI_GUIWINDOW_H
