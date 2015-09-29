/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Widgets/Extended/GUIList.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Widget : Extended controls, List view
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
#ifndef SCARAB_ENGINE_GUI_WIDGETS_EXTENDED_GUILIST_H
#define SCARAB_ENGINE_GUI_WIDGETS_EXTENDED_GUILIST_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../Lib/Datastruct/Array/Array.h"

#include "../../Tools/GUISKin.h"

#include "../../GUIWidget.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The GUIListNode class
class GUIListNode
{
public:
    GUIListNode();
    GUIListNode( const GChar * strName, GUISkinImage idIcon = GUISKIN_IMAGE_ICON_UNDEFINED );
    ~GUIListNode();

    // Getters
    inline Bool IsSelected() const;

    inline const GChar * GetName( UInt * outLength = NULL ) const;
    inline GUISkinImage GetIcon() const;

private:
    friend class GUIListModel;

    // State
    Bool m_bSelected;

    // User Data
    UInt m_iNameLength;
    GChar m_strName[GUI_NAME_LENGTH];
    GUISkinImage m_idIcon;
};

/////////////////////////////////////////////////////////////////////////////////
// The GUIListModel class
class GUIListModel : public GUIWidgetModel
{
public:
    GUIListModel( UInt iNodeHeight, UInt iNodeCount, const GChar * arrTexts[], GUISkinImage arrIcons[] );
    virtual ~GUIListModel();

    // Model
    inline UInt GetDisplayTop() const;
    inline UInt GetDisplayCount() const;

    inline UInt GetNodeHeight() const;

    inline UInt GetNodeCount() const;
    inline GUIListNode * GetNode( UInt iNode ) const;

    Void Push( const GChar * strName, GUISkinImage idIcon );
    Void Unshift( const GChar * strName, GUISkinImage idIcon );

    Void Pop();
    Void Shift();

    Void Insert( UInt iNode, const GChar * strName, GUISkinImage idIcon = GUISKIN_IMAGE_ICON_UNDEFINED );
    Void Replace( UInt iNode, const GChar * strName, GUISkinImage idIcon = GUISKIN_IMAGE_ICON_UNDEFINED );
	Void Remove( UInt iNode );

    Void SetScroll( UInt iScrollPos );
    Void ScrollUp( UInt iAmount = 1 );
    Void ScrollDown( UInt iAmount = 1 );

	Void SelectAll();
	Void UnselectAll();
	Void Select( UInt iItem, Bool bSelect );
	Void ToggleSelect( UInt iItem );

    // Controller
    virtual Void OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags );

    virtual Void OnScroll( const Point2 & ptLocalPos, Int iWheelDelta, GUIEventFlag iFlags );

    virtual Void OnResize();

    virtual Void OnInsert( UInt iItem );
    virtual Void OnRemove( UInt iItem );
    virtual Void OnSelectionChange();

protected:
    UInt m_iDisplayTop;
    UInt m_iDisplayCount;

    UInt m_iNodeHeight;

    UInt m_iLastSelected;

    Array<GUIListNode> m_arrNodes;
};

/////////////////////////////////////////////////////////////////////////////////
// The GUIList class
class GUIList : public GUIWidget
{
public:
    GUIList( GUIListModel * pModel, GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName );
    virtual ~GUIList();

    // Type
	inline virtual GUIWidgetType GetType() const;

protected:
    // Drawing interface
    virtual Void _Draw( const Rectangle2 & rectClient );
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GUIList.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_GUI_WIDGETS_EXTENDED_GUILIST_H

