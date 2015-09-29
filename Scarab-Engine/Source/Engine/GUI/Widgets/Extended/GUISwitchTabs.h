/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Widgets/Extended/GUISwitchTabs.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Widget : Extended controls, Switching Tabbed containers
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : TODO = Movable Tabs using button area's drag&drop events ...
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_GUI_WIDGETS_EXTENDED_GUISWITCHTABS_H
#define SCARAB_ENGINE_GUI_WIDGETS_EXTENDED_GUISWITCHTABS_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Common/GUISection.h"
#include "../Common/GUIButton.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define GUISWITCHTABS_MAX_TABS         16
#define GUISWITCHTABS_BUTTONBAR_HEIGHT 20
#define GUISWITCHTABS_BUTTON_WIDTH     100 // Use INVALID_OFFSET to fill button bar
                                           // with equal size buttons

    // Prototypes
class GUISwitchTabsButtonModel;

class GUISwitchTabsModel;
class GUISwitchTabs;

/////////////////////////////////////////////////////////////////////////////////
// The GUISwitchTabsButtonModel class
class GUISwitchTabsButtonModel : public GUIButtonModel
{
public:
    GUISwitchTabsButtonModel( GUISwitchTabsModel * pSwitchTabsModel, UInt iTabIndex, const GChar * strText );
    virtual ~GUISwitchTabsButtonModel();

    virtual Void OnPress();

private:
    GUISwitchTabsModel * m_pSwitchTabsModel;
    UInt m_iTabIndex;
};

/////////////////////////////////////////////////////////////////////////////////
// The GUISwitchTabsModel class
class GUISwitchTabsModel : public GUIWidgetModel
{
public:
    GUISwitchTabsModel( GUISectionModel ** arrTabs, const GChar ** arrTabNames, UInt iTabCount );
    virtual ~GUISwitchTabsModel();

    // Model
    inline UInt GetTabCount() const;
    inline GUISectionModel * GetTab( UInt iTab ) const;
    inline const GChar * GetTabName( UInt iTab ) const;

    inline GUISectionModel * GetButtonBar() const;
    inline GUISwitchTabsButtonModel * GetButton( UInt iTab ) const;

    inline UInt GetSelectedTab() const;
    Void SetSelectedTab( UInt iTab );

    virtual UInt GetButtonBarHeight() const;
    virtual UInt GetButtonWidth() const;

    // Controller
    virtual Void OnSwitch( UInt iOldTab, UInt iNewTab );

protected:
    UInt m_iTabCount;
    GUISectionModel * m_arrTabModels[GUISWITCHTABS_MAX_TABS];

    GUISectionModel * m_pButtonBarModel;
    GUISwitchTabsButtonModel * m_arrButtonModels[GUISWITCHTABS_MAX_TABS];

    UInt m_iSelectedTab;
};

/////////////////////////////////////////////////////////////////////////////////
// The GUISwitchTabs class
class GUISwitchTabs : public GUIWidget
{
public:
    GUISwitchTabs( GUISwitchTabsModel * pModel, GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName );
    virtual ~GUISwitchTabs();

    // Type
	inline virtual GUIWidgetType GetType() const;

    // Tabs access
    inline UInt GetTabCount() const;
    inline GUISection * GetTab( UInt iTab ) const;

    // Visibility control
    virtual Void SetVisible( Bool bVisible );

protected:
    // Drawing interface
    virtual Void _Draw( const Rectangle2 & rectClient );

private:
    friend class GUISwitchTabsModel;
    Void _UpdateAfterSwitch( UInt iOldTab, UInt iNewTab );

    UInt m_iTabCount;
    GUISection * m_arrTabs[GUISWITCHTABS_MAX_TABS];

    GUISection * m_pButtonBar;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GUISwitchTabs.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_GUI_WIDGETS_EXTENDED_GUISWITCHTABS_H

