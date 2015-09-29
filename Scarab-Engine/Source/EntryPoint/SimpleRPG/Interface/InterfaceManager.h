/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Interface/InterfaceManager.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Interface : InterfaceManager
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
#ifndef SCARAB_ENTRYPOINT_SIMPLERPG_INTERFACE_INTERFACEMANAGER_H
#define SCARAB_ENTRYPOINT_SIMPLERPG_INTERFACE_INTERFACEMANAGER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Gameplay/GameplayManager.h"

#include "SelfBar.h"
#include "TargetBar.h"
#include "SkillBar.h"
#include "CastBar.h"
#include "StatusEffectBar.h"

#include "CharacterView.h"
#include "SkillBookView.h"
#include "InventoryView.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define InterfaceFn InterfaceManager::GetInstancePtr()

#define INTERFACE_HUD_ICON_SIZE 32

// HUD Skin
enum HUDImage {
    HUD_IMAGE_BACKGROUND_HPBAR = 0,
    HUD_IMAGE_FOREGROUND_HPBAR,
    HUD_IMAGE_BACKGROUND_MPBAR,
    HUD_IMAGE_FOREGROUND_MPBAR,

    HUD_IMAGE_BACKGROUND_STATUSEFFECTBAR,
    HUD_IMAGE_FOREGROUND_STATUSEFFECTBAR,

    HUD_IMAGE_BACKGROUND_CASTBAR,
    HUD_IMAGE_FOREGROUND_CASTBAR,

    HUD_IMAGE_BACKGROUND_SKILLBAR,
    HUD_IMAGE_FOREGROUND_SKILLBAR_GCD,
    HUD_IMAGE_FOREGROUND_SKILLBAR_CD,

    HUD_IMAGE_BACKGROUND_CHARACTERVIEW,

    HUD_IMAGE_BACKGROUND_SKILLBOOKVIEW,

    HUD_IMAGE_BACKGROUND_INVENTORYVIEW,
    HUD_IMAGE_FOREGROUND_INVENTORYVIEW,
    HUD_IMAGE_FOREGROUND_INVENTORYVIEW_BAGBUTTON_TRASH,
    HUD_IMAGE_FOREGROUND_INVENTORYVIEW_BAGBUTTON_CONSUMMABLE,
    HUD_IMAGE_FOREGROUND_INVENTORYVIEW_BAGBUTTON_EQUIPMENT,
    HUD_IMAGE_FOREGROUND_INVENTORYVIEW_BAGBUTTON_QUEST,
    HUD_IMAGE_FOREGROUND_INVENTORYVIEW_BAGBUTTON_BUFFER,
    HUD_IMAGE_FOREGROUND_INVENTORYVIEW_ACTIVEBAGBUTTON,

    HUD_IMAGE_COUNT
};

enum HUDColor {
    HUD_COLOR_SELFBAR_TEXT = 0,
    
    HUD_COLOR_TARGETBAR_TEXT,
    
    HUD_COLOR_CASTBAR_TEXT,

    HUD_COLOR_CHARACTERVIEW_TITLE,
    HUD_COLOR_CHARACTERVIEW_VALUE,

    HUD_COLOR_SKILLBOOK_TEXT,

    HUD_COLOR_COUNT
};

enum HUDFont {
    HUD_FONT_SELFBAR_TEXT = 0,

    HUD_FONT_TARGET_TEXT,

    HUD_FONT_CASTBAR_TEXT,

    HUD_FONT_CHARACTERVIEW_TITLE,
    HUD_FONT_CHARACTERVIEW_VALUE,

    HUD_FONT_SKILLBOOK_TEXT,

    HUD_FONT_COUNT
};

// HUD skin texture locations
#define HUD_TEXTURE_SIZE 128

#define HUD_LOCATION_BACKGROUND_HPBAR Rectangle2( Point2(  0, 0 ), 32, 32 )
#define HUD_LOCATION_FOREGROUND_HPBAR Rectangle2( Point2( 32, 0 ), 32, 32 )
#define HUD_LOCATION_BACKGROUND_MPBAR Rectangle2( Point2( 64, 0 ), 32, 32 )
#define HUD_LOCATION_FOREGROUND_MPBAR Rectangle2( Point2( 96, 0 ), 32, 32 )

#define HUD_LOCATION_BACKGROUND_CASTBAR Rectangle2( Point2( 128, 0 ), 32, 32 )
#define HUD_LOCATION_FOREGROUND_CASTBAR Rectangle2( Point2( 160, 0 ), 32, 32 )

#define HUD_LOCATION_BACKGROUND_STATUSEFFECTBAR Rectangle2( Point2( 192, 0 ), 32, 32 )
#define HUD_LOCATION_FOREGROUND_STATUSEFFECTBAR Rectangle2( Point2( 224, 0 ), 32, 32 )

#define HUD_LOCATION_BACKGROUND_SKILLBAR     Rectangle2( Point2(  0, 32 ), 32, 32 )
#define HUD_LOCATION_FOREGROUND_SKILLBAR_GCD Rectangle2( Point2( 32, 32 ), 32, 32 )
#define HUD_LOCATION_FOREGROUND_SKILLBAR_CD  Rectangle2( Point2( 64, 32 ), 32, 32 )

#define HUD_LOCATION_BACKGROUND_CHARACTERVIEW Rectangle2( Point2( 96, 32 ), 32, 32 )

#define HUD_LOCATION_BACKGROUND_SKILLBOOKVIEW Rectangle2( Point2( 128, 32 ), 32, 32 )

#define HUD_LOCATION_BACKGROUND_INVENTORYVIEW                       Rectangle2( Point2( 160, 32 ), 32, 32 )
#define HUD_LOCATION_FOREGROUND_INVENTORYVIEW                       Rectangle2( Point2( 192, 32 ), 32, 32 )
#define HUD_LOCATION_FOREGROUND_INVENTORYVIEW_BAGBUTTON_TRASH       Rectangle2( Point2( 224, 32 ), 32, 32 )
#define HUD_LOCATION_FOREGROUND_INVENTORYVIEW_BAGBUTTON_CONSUMMABLE Rectangle2( Point2(   0, 64 ), 32, 32 )
#define HUD_LOCATION_FOREGROUND_INVENTORYVIEW_BAGBUTTON_EQUIPMENT   Rectangle2( Point2(  32, 64 ), 32, 32 )
#define HUD_LOCATION_FOREGROUND_INVENTORYVIEW_BAGBUTTON_QUEST       Rectangle2( Point2(  64, 64 ), 32, 32 )
#define HUD_LOCATION_FOREGROUND_INVENTORYVIEW_BAGBUTTON_BUFFER      Rectangle2( Point2(  96, 64 ), 32, 32 )
#define HUD_LOCATION_FOREGROUND_INVENTORYVIEW_ACTIVEBAGBUTTON       Rectangle2( Point2( 128, 64 ), 32, 32 )

// HUD status effects icons texture locations
#define HUDICON_LOCATION_STATUSEFFECT_CHILL Rectangle2( Point2( 0, 0 ), 32, 32 )

// HUD skill icons texture locations
#define HUDICON_LOCATION_SKILL_MAGE_FIREBOLT Rectangle2( Point2(  0, 0 ), 32, 32 )
#define HUDICON_LOCATION_SKILL_MAGE_ICENOVA  Rectangle2( Point2( 32, 0 ), 32, 32 )

// HUD item icons texture locations
#define HUDICON_LOCATION_ITEM_WEAPON_FISTS Rectangle2( Point2( 0, 0 ), 32, 32 )

// HUD Drag & Drop
enum HUDDragType{
    HUD_DRAGTYPE_UNDEFINED = 0,

    HUD_DRAGTYPE_SKILL,
    HUD_DRAGTYPE_ITEM,

    HUD_DRAGTYPE_COUNT
};

typedef struct _hud_dragndrop_data {
    HUDDragType iType;
    UInt iIdentifier;
} HUDDragNDropData;

/////////////////////////////////////////////////////////////////////////////////
// The HUDBackboardModel class
class HUDBackboardModel : public GUIBackboardModel
{
public:
    HUDBackboardModel();
    virtual ~HUDBackboardModel();

    virtual Void OnKeyPress( KeyCode iKey, GUIEventFlag iFlags );
	virtual Void OnKeyRelease( KeyCode iKey, GUIEventFlag iFlags );

	virtual Void OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags );

    virtual Void OnStartDrag( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags );
	virtual Void OnDragRight( const Point2 & ptLocalPos, GUIEventFlag iFlags );
	virtual Void OnStopDrag( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags );

    virtual Void OnScroll( const Point2 & ptLocalPos, Int iWheelDelta, GUIEventFlag iFlags );

private:
    static Void _MsgBox_ConfirmExit( GUIMessageBoxResult iResult, Void * pUserData );

    GUIMenuModel * m_pMenuModel;

    Bool m_bMovingX, m_bMovingY, m_bMovingZ;
    Vector3 m_vMoveX, m_vMoveY, m_vMoveZ;

    Point2 m_ptLastMouseDragPos;
};

/////////////////////////////////////////////////////////////////////////////////
// The InterfaceManager class
class InterfaceManager : public Manager<InterfaceManager>
{
public:
    inline static const GChar * GetName();
    inline static const GChar * GetMemoryBudget();

protected:
    friend class Manager<InterfaceManager>;
    InterfaceManager();
    virtual ~InterfaceManager();

public:
    Void Initialize();
    Void Cleanup();

    // HUD access
    inline GPU2DContext * GetHUDContext() const;

    inline GPU2DBitmap * GetHUDSkinBitmap() const;
    inline GPU2DBitmap * GetHUDStatusEffectBitmap() const;
    inline GPU2DBitmap * GetHUDSkillsBitmap() const;
    inline GPU2DBitmap * GetHUDItemsBitmap() const;

    inline const Rectangle2 & GetHUDLocation( HUDImage iHUDImage ) const;
    inline const Rectangle2 & GetHUDLocation( StatusEffectID idStatusEffect ) const;
    inline const Rectangle2 & GetHUDLocation( SkillID idSkill ) const;
    inline const Rectangle2 & GetHUDLocation( ItemID idItem ) const;

    inline GPU2DSolidColorBrush * GetHUDColorBrush() const;
    inline const Color4 & GetHUDColor( HUDColor iHUDColor ) const;

    inline GPU2DSolidColorBrush * GetHUDTextBrush() const;
    inline GPU2DTextFormat * GetHUDTextFormat( HUDFont iHUDFont ) const;

    // HUD views
    inline Void ToggleCharacterView();
    inline Void ToggleSkillBookView();
    inline Void UpdateSkillBookView();
    inline Void ToggleInventoryView();

    // HUD drag & drop data
    inline HUDDragNDropData * GetDragNDropData();

    // Main routines
    Void Update();

    Void Render();

private:
    // HUD Skin
    GPU2DBitmap * m_pSkinBitmap;
    GPU2DBitmap * m_pStatusEffectsBitmap;
    GPU2DBitmap * m_pSkillsBitmap;
    GPU2DBitmap * m_pItemsBitmap;

    Rectangle2 m_arrHUDLocations[HUD_IMAGE_COUNT];
    Rectangle2 m_arrStatusEffectIcons[STATUSEFFECTID_COUNT];
    Rectangle2 m_arrSkillIcons[SKILLID_COUNT];
    Rectangle2 m_arrItemIcons[ITEMID_COUNT];

    mutable GPU2DSolidColorBrush m_hColorBrush;
    Color4 m_arrHUDColors[HUD_COLOR_COUNT];

    mutable GPU2DSolidColorBrush m_hTextBrush;
    GPU2DTextFormat m_hTextFormat;
    GPU2DTextFormat * m_arrTextFormats[HUD_FONT_COUNT];

    // HUD Interface
    Void _CreateHeadUpDisplay();
    Void _DestroyHeadUpDisplay();

    HUDBackboardModel * m_pBackboardModel;

    SelfBarModel * m_pSelfBarModel;
    TargetBarModel * m_pTargetBarModel;
    TargetBarModel * m_pFocusBarModel;

    StatusEffectBarModel * m_pStatusEffectBarModel;
    SkillBarModel * m_pSkillBarModel;

    CastBarModel * m_pCastBarModel;

    CharacterViewModel * m_pCharacterViewModel;
    CharacterView * m_pCharacterView;
    SkillBookViewModel * m_pSkillBookViewModel;
    SkillBookView * m_pSkillBookView;
    InventoryViewModel * m_pInventoryViewModel;
    InventoryView * m_pInventoryView;

    GUIWindow * m_pHUDWindow;

    HUDDragNDropData m_hDragNDrop;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "InterfaceManager.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_SIMPLERPG_INTERFACE_INTERFACEMANAGER_H

