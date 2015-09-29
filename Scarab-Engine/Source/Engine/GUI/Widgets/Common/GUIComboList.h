/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Widgets/Common/GUIComboList.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Widget : Common controls, ComboList
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
#ifndef SCARAB_ENGINE_GUI_WIDGETS_COMMON_GUICOMBOLIST_H
#define SCARAB_ENGINE_GUI_WIDGETS_COMMON_GUICOMBOLIST_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../GUIWidget.h"
#include "../../Overlays/GUIMenu.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Prototypes
class GUIComboListMenuModel;
class GUIComboListModel;
class GUIComboList;

/////////////////////////////////////////////////////////////////////////////////
// The GUIComboListMenuModel class
class GUIComboListMenuModel : public GUIMenuModel
{
public:
    GUIComboListMenuModel( GUIComboListModel * pModel, UInt iItemCount, const GChar ** arrItemNames, GUISkinImage * arrItemIcons, Void ** arrItemUserDatas = NULL );
    virtual ~GUIComboListMenuModel();

    // Controller
    virtual Void OnValidate( UInt iItem );
    virtual Void OnAbort();

protected:
    // Target model
    GUIComboListModel * m_pModel;
};

/////////////////////////////////////////////////////////////////////////////////
// The GUIComboListModel class
class GUIComboListModel : public GUIWidgetModel
{
public:
    GUIComboListModel( UInt iItemCount, const GChar ** arrItemNames, GUISkinImage * arrItemIcons, Void ** arrItemUserDatas = NULL );
    virtual ~GUIComboListModel();

    // Model
    inline UInt GetItemCount() const;
    inline const GChar * GetItemName( UInt iItem, UInt * outLength = NULL ) const;
    inline GUISkinImage GetItemIcon( UInt iItem ) const;
    inline Void * GetItemUserData( UInt iItem ) const;

    inline UInt GetSelectedItem() const;
    inline const GChar * GetSelectedItemName( UInt * outLength = NULL ) const;
    inline GUISkinImage GetSelectedItemIcon() const;
    inline Void * GetSelectedItemUserData() const;

    inline Void SelectItem( UInt iItem );

    inline Bool IsOpened() const;
    inline Bool IsClosed() const;

    // Controller
    virtual Void OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags );

    virtual Void OnChoicePick( Void * pUserData );

protected:
    friend class GUIComboListMenuModel;

    UInt m_iSelectedItem;
    Bool m_bOpened;

    GUIComboListMenuModel * m_pComboListMenuModel;
};

/////////////////////////////////////////////////////////////////////////////////
// The GUIComboList class
class GUIComboList : public GUIWidget
{
public:
    GUIComboList( GUIComboListModel * pModel, GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName );
    virtual ~GUIComboList();

    // Type
	inline virtual GUIWidgetType GetType() const;

protected:
    // Drawing interface
    virtual Void _Draw( const Rectangle2 & rectClient );
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GUIComboList.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_GUI_WIDGETS_COMMON_GUICOMBOLIST_H

