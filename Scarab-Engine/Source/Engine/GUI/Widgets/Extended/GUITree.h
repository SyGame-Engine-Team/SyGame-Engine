/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Widgets/Extended/GUITree.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Widget : Extended controls, Tree view
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
#ifndef SCARAB_ENGINE_GUI_WIDGETS_EXTENDED_GUITREE_H
#define SCARAB_ENGINE_GUI_WIDGETS_EXTENDED_GUITREE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../Lib/Datastruct/Array/Array.h"

#include "../../Tools/GUISKin.h"

#include "../../GUIWidget.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The GUITreeNode class
class GUITreeNode
{
public:
    // Getters
    inline Bool IsRoot() const;
    inline Bool IsLeaf() const;
    inline Bool IsLastChild() const;
    inline UInt GetDepth() const;

    inline Bool IsExpanded() const;
    inline Bool IsSelected() const;

    inline UInt GetParentIndex() const;
    inline GUITreeNode * GetParent() const;
    inline UInt GetChildCount() const;
    inline GUITreeNode * GetChild( UInt iChild ) const;

    inline const GChar * GetName( UInt * outLength = NULL ) const;
    inline GUISkinImage GetIcon() const;

    // Node interface
    inline UInt GetNodeCount() const;
    GUITreeNode * GetNode( UInt iNode ) const;
    GUITreeNode * GetNextNode() const;

    GUITreeNode * GetNode( const GChar * strPath ) const;
    Void GetPath( GChar * outPath, UInt * outLength ) const;

private:
    // Delegate of GUITreeModel
    friend class GUITreeModel;
    GUITreeNode();
    GUITreeNode( GUITreeNode * pParent, Bool bIsLeaf, const GChar * strName, GUISkinImage idIcon );
    ~GUITreeNode();

    // Dynamic operations needing update
    Void _DestroyChild( UInt iChild );
    Void _Expand();
    Void _Collapse();

    // State
    Bool m_bIsLastChild;
    UInt m_iDepth;

    Bool m_bPopulated;
    UInt m_iNodeCount;

    Bool m_bExpanded;
    Bool m_bSelected;

    // Parent link
    UInt m_iParentIndex;
    GUITreeNode * m_pParent;

    // Child links
    Array<GUITreeNode*> m_arrChildren;

    // User Data
    Bool m_bIsLeaf;
    GUISkinImage m_idIcon;
    UInt m_iNameLength;
    GChar m_strName[GUI_NAME_LENGTH + 1];
};

/////////////////////////////////////////////////////////////////////////////////
// The GUITreeModel class
class GUITreeModel : public GUIWidgetModel
{
public:
    GUITreeModel( UInt iNodeHeight );
    virtual ~GUITreeModel();

    // Model
    inline UInt GetDisplayTop() const;
    inline UInt GetDisplayCount() const;

    inline UInt GetNodeHeight() const;

    inline UInt GetNodeCount() const;
    inline GUITreeNode * GetNode( UInt iNode ) const;

    inline GUITreeNode * GetNode( const GChar * strPath ) const;

    GUITreeNode * CreateNode( GUITreeNode * pParent, Bool bIsLeaf, const GChar * strName, GUISkinImage idIcon );
    Void DestroyNode( GUITreeNode * pNode );

    Void Expand( GUITreeNode * pNode );
    Void Collapse( GUITreeNode * pNode );

    Void SetScroll( UInt iScrollPos );
    Void ScrollUp( UInt iAmount = 1 );
    Void ScrollDown( UInt iAmount = 1 );

    inline GUITreeNode * GetLastSelectedNode() const;

	Void SelectAll();
	Void UnselectAll();
	Void Select( UInt iNode, Bool bSelect, GUITreeNode * pNode = NULL );
	Void ToggleSelect( UInt iNode, GUITreeNode * pNode = NULL );

    // Controller
    virtual Void OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags );

    virtual Void OnScroll( const Point2 & ptLocalPos, Int iWheelDelta, GUIEventFlag iFlags );

    virtual Void OnResize();

    virtual Void OnPopulateNode( GUITreeNode * pNode );
    virtual Void OnSelectionChange();

private:
    UInt m_iDisplayTop;
    UInt m_iDisplayCount;

    UInt m_iNodeHeight;

    UInt m_iLastSelected;
    GUITreeNode * m_pLastSelected;

    GUITreeNode * m_pRoot;
};

/////////////////////////////////////////////////////////////////////////////////
// The GUITree class
class GUITree : public GUIWidget
{
public:
    GUITree( GUITreeModel * pModel, GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName );
    virtual ~GUITree();

    // Type
	inline virtual GUIWidgetType GetType() const;

protected:
    // Drawing interface
    virtual Void _Draw( const Rectangle2 & rectClient );
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GUITree.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_GUI_WIDGETS_EXTENDED_GUITREE_H

