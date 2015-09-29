/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Overlays/GUIMenu.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Overlay : Generic menu interface
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
#ifndef SCARAB_ENGINE_GUI_OVERLAYS_GUIMENU_H
#define SCARAB_ENGINE_GUI_OVERLAYS_GUIMENU_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Tools/GUISKin.h"

#include "../GUIOverlay.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define GUIMENU_TEXTWIDTH 128

// Prototypes
class GUIMenuModel;

/////////////////////////////////////////////////////////////////////////////////
// The GUIMenuItem class
class GUIMenuItem
{
private:
    // Delegate of GUIMenuModel
    friend class GUIMenuModel;
    GUIMenuItem();
    GUIMenuItem( const GChar * strName, GUISkinImage idIcon, Void * pUserData, GUIMenuModel * pSubMenu );
    ~GUIMenuItem();

public:
    // Getters
    inline Bool IsActive() const;

    inline Bool HasSubMenu() const;
    inline GUIMenuModel * GetSubMenu() const;

    inline const GChar * GetName( UInt * outLentgh = NULL ) const;
    inline GUISkinImage GetIcon() const;
    inline Void * GetUserData() const;

private:
    // State
    Bool m_bActive;

    // SubMenu
    GUIMenuModel * m_pSubMenu;

    // User Data
    GUISkinImage m_idIcon;
    UInt m_iNameLength;
    GChar m_strName[GUI_NAME_LENGTH + 1];
    Void * m_pUserData;
};

/////////////////////////////////////////////////////////////////////////////////
// The GUIMenuModel class
class GUIMenuModel : public GUIOverlayModel
{
public:
    GUIMenuModel( UInt iItemHeight, UInt iItemCount, const GChar * arrNames[], GUISkinImage arrIcons[], Void * arrUserDatas[] = NULL, GUIMenuModel * arrSubMenus[] = NULL );
	virtual ~GUIMenuModel();

    // Model
    inline UInt GetItemHeight() const;

    inline UInt GetItemCount() const;
    inline const GUIMenuItem * GetItem( UInt iItem ) const;

    inline Void SetActive( UInt iItem, Bool bActive );

    inline UInt GetSelectedItem() const;
    inline Void SelectItem( UInt iItem );

    // Controller
    virtual Void OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags );

    virtual Void OnRollOver( const Point2 & ptLocalPos, GUIEventFlag iFlags );

    virtual Void OnValidate( UInt iItem );
    virtual Void OnAbort();

protected:
    UInt m_iItemHeight;

    UInt m_iSelectedItem;

    UInt m_iItemCount;
    GUIMenuItem * m_arrItems;
};

/////////////////////////////////////////////////////////////////////////////////
// The GUIMenu class
class GUIMenu : public GUIOverlay
{
public:
    GUIMenu( GUIMenuModel * pModel, const GChar * strName, const Point2 & ptScreenPos );
	virtual ~GUIMenu();

    // Getters
    inline virtual GUIOverlayType GetType() const;

    inline UInt GetItemCount() const;
    inline GUIMenu * GetSubMenu( UInt iItem ) const;

    // Helpers
    Void Expand();
    Void Collapse();

    GUIMenu * GetSelectedMenu();

protected:
    // Drawing interface
    virtual Void _Draw( const Rectangle2 & rectClient );

private:
    static const Rectangle2 & _ComputePlacement( const GUIMenuModel * pModel, const Point2 & ptScreenPos );

    // Menu data
    UInt m_iItemCount;
    GUIMenu ** m_arrSubMenus;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GUIMenu.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_GUI_OVERLAYS_GUIMENU_H
