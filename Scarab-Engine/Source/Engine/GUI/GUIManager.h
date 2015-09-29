/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/GUIManager.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Core-Class for graphic user interface.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : TODO, Add a skin manager ...
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_GUI_GUIMANAGER_H
#define SCARAB_ENGINE_GUI_GUIMANAGER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Lib/Object/Patterns/Manager.h"

#include "../Input/InputManager.h"
#include "../Resource/ResourceManager.h"
#include "../Rendering/RenderingManager.h"

#include "GUIWidget.h"
#include "GUIWindow.h"
#include "GUIBackboard.h"
#include "GUIOverlay.h"

#include "Tools/GUIDragNDrop.h"
#include "Tools/GUIMoveResize.h"
#include "Tools/GUIMenuLoop.h"
#include "Tools/GUITimerEvents.h"
#include "Tools/GUISkin.h"
#include "Tools/GUIMessageBox.h"

// Overlay collection
#include "Overlays/GUIMenu.h"

// Widget collection
#include "Widgets/Common/GUIContainer.h"
#include "Widgets/Common/GUISection.h"
#include "Widgets/Common/GUILabel.h"
#include "Widgets/Common/GUIButton.h"
#include "Widgets/Common/GUICheckBox.h"
#include "Widgets/Common/GUIRadioButton.h"
#include "Widgets/Common/GUIComboList.h"
#include "Widgets/Common/GUITextInput.h"

#include "Widgets/Extended/GUISwitchTabs.h"
#include "Widgets/Extended/GUITextLog.h"
#include "Widgets/Extended/GUIList.h"
#include "Widgets/Extended/GUITree.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define GUIFn GUIManager::GetInstancePtr()

/////////////////////////////////////////////////////////////////////////////////
// The GUIManager class
class GUIManager : public Manager<GUIManager>
{
public:
    inline static const GChar * GetName();
    inline static const GChar * GetMemoryBudget();

protected:
    friend class Manager<GUIManager>;
    GUIManager();
    virtual ~GUIManager();

public:
    Void Initialize();
    Void Cleanup();

    // Screen interface
    inline GPU2DContext * GetScreenContext() const;
    inline Void GetScreenSize( UInt * outWidth, UInt * outHeight ) const;

    // Backboard interface
    inline GUIBackboard * GetBackboard() const;
    inline Void SetBackboardModel( GUIBackboardModel * pModel );

    // Window interface
    GUIWindow * CreateWindow( const GChar * strName, const Rectangle2 & rectWindow, GUIWindowStyle iWinStyle );
    Void DestroyWindow( GUIWindow * pWindow );

    GUIWidget * CreateWidget( GUIWidgetType iType, GUIWidgetModel * pModel,
                              GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName );

    Void RegisterWindow( GUIWindow * pWindow );
    Void UnregisterWindow( GUIWindow * pWindow );

    inline GUIWindow * GetWindow( const GChar * strName ) const;
    inline GUIWindow * GetFocusWindow() const;
    inline GUIWindow * GetMouseOverWindow() const;
    Void SetFocusWindow( GUIWindow * pWindow );

    inline Void EnumWindows() const;
    inline GUIWindow * EnumNextWindow() const;

    // Overlay interface
    Void RegisterOverlay( GUIOverlay * pOverlay );
    Void UnregisterOverlay( GUIOverlay * pOverlay );

    inline GUIOverlay * GetOverlay( const GChar * strName ) const;
    inline GUIOverlay * GetFocusOverlay() const;
    inline GUIOverlay * GetMouseOverOverlay() const;

    // Message-Box interface
    inline Bool HasMessageBox() const;
    inline Bool MessageBox( GUIMessageBoxType iType, const GChar * strMessage, GUIMessageBoxButtons iButtons,
                            GUIMsgBoxCallback pfCallback = NULL, Void * pUserData = NULL );
    inline Bool DialogBox( const GUIMessageBoxDialogDeclaration * arrDialogFormItems, UInt iDialogFormItemCount,
                           GUIMsgBoxDialogCallback pfCallback = NULL, Void * pUserData = NULL );

    // Skin interface
    inline const GUISkin * GetSkin() const;

    // Picking interface
    GUIWindow * PickWindow( const Point2 & ptScreenPos ) const;
    GUIOverlay * PickOverlay( const Point2 & ptScreenPos ) const;

    // Event-Handling interface
    Void OnKeyPress( KeyCode iKey );
    Void OnKeyRelease( KeyCode iKey );

    Void OnMouseMove( const Point2 & ptScreenPos );
    Void OnMousePress( const Point2 & ptScreenPos, KeyCode iKey );
    Void OnMouseRelease( const Point2 & ptScreenPos, KeyCode iKey );
    Void OnMouseDblClick( const Point2 & ptScreenPos, KeyCode iKey );
    Void OnMouseWheel( const Point2 & ptScreenPos, Int iWheelDelta );

    Void OnExit();

    inline UInt CreateTimer( GUIEventProcessor * pTarget, Scalar fIntervalMS );
    inline Void DestroyTimer( UInt iTimerID );

    inline const GUITimer * GetTimer( UInt iTimerID ) const;

    // Drag'N'Drop interface
    inline Bool IsDragPrepared( GUIDragChannel iChannel ) const;
    inline Bool IsDragConfirmed( GUIDragChannel iChannel ) const;

    inline GUIEventProcessor * GetDragSource( GUIDragChannel iChannel ) const;
    inline const Point2 & GetDragSourcePos( GUIDragChannel iChannel ) const;

    inline GUIEventProcessor * GetDragDest( GUIDragChannel iChannel ) const;
    inline const Point2 & GetDragDestPos( GUIDragChannel iChannel ) const;

    inline Void * GetDragData( GUIDragChannel iChannel ) const;
    inline Void SetDragData( GUIDragChannel iChannel, Void * pDragData );

    // Move-Resize interface
    inline Void EnterDragMove( GUIWindow * pWindow );
    inline Void EnterDragResize( GUIWindow * pWindow, GUIResizeFlag iFlags );
    inline Void LeaveDragMoveResize();
    Void MoveWindow( GUIWindow * pWindow, const Point2 & ptScreenPos );
    Void ResizeWindow( GUIWindow * pWindow, UInt iWidth, UInt iHeight );

    // Menu interface
    inline Bool IsInMenuLoop() const;
    inline const GUIMenuModel * GetMenuLoopModel() const;

    inline Void EnterMenuLoop( GUIMenuModel * pModel, const GChar * strName, const Point2 & ptScreenPos );
    inline Void LeaveMenuLoop( Bool bAborting );

    // Updating interface
    Void Update();

    // Rendering interface
    Void Render();

private:
    // Helpers
    typedef TreeMap<const GChar*, GUIWindow*> WindowMap;
    typedef TreeMap<const GChar*, GUIOverlay*> OverlayMap;
    inline static Int _Name_Comparator( const GChar* const & rLeft, const GChar* const & rRight, Void * pUserData );

    typedef RedBlackTree<GUIWindow*> WindowDepthMap;
    typedef RedBlackTree<GUIOverlay*> OverlayDepthMap;
    inline static Int _Depth_Comparator( GUIWindow * const & rLeft, GUIWindow * const & rRight, Void * pUserData );
    inline static Int _Depth_Comparator( GUIOverlay * const & rLeft, GUIOverlay * const & rRight, Void * pUserData );

    friend class GUIWindow;
    friend class GUIOverlay;
    Void _UpdateWindowDepth( GUIWindow * pWindow, UInt iNewDepth );
    Void _UpdateOverlayDepth( GUIOverlay * pOverlay, UInt iNewDepth );

    // Drag'N'Drop access
    friend class GUIEventProcessor;
    inline GUIDragNDrop * _GetDragNDrop();

    // Rendering Context
    GPU2DContext * m_pGUIContext;

    // Backboard
    GUIBackboardModel * m_pBackboardDefaultModel;
    GUIBackboard * m_pBackboard;

    // Windows map
    WindowMap m_mapWindows;
    WindowDepthMap m_hWindowDepthMap;

    mutable WindowMap::Iterator m_itEnumerate;

    // Overlays map
    OverlayMap m_mapOverlays;
    OverlayDepthMap m_hOverlayDepthMap;

    // GUI States
    GUIWindow * m_pFocus;
    GUIWindow * m_pMousedOver;
    GUIOverlay * m_pOverlayFocus;
    GUIOverlay * m_pOverlayMousedOver;

    GUIDragNDrop m_guiDragNDrop;
    GUIMoveResize m_guiMoveResize;
    GUIMenuLoop m_guiMenuLoop;
    GUITimerEvents m_guiTimerEvents;

    // GUI Skin
    GUISkin * m_pSkin;

    // GUI Message-Box
    GUIMessageBox * m_pMessageBox;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GUIManager.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_GUI_GUIMANAGER_H
