/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Widgets/Extended/GUITree.inl
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
// GUITreeNode implementation
inline Bool GUITreeNode::IsRoot() const {
    return ( m_pParent == NULL );
}
inline Bool GUITreeNode::IsLeaf() const {
    return m_bIsLeaf;
}
inline Bool GUITreeNode::IsLastChild() const {
    return m_bIsLastChild;
}
inline UInt GUITreeNode::GetDepth() const {
    return m_iDepth;
}

inline Bool GUITreeNode::IsExpanded() const {
    return m_bExpanded;
}
inline Bool GUITreeNode::IsSelected() const {
    return m_bSelected;
}

inline UInt GUITreeNode::GetParentIndex() const {
    return m_iParentIndex;
}
inline GUITreeNode * GUITreeNode::GetParent() const {
    return m_pParent;
}
inline UInt GUITreeNode::GetChildCount() const {
    return m_arrChildren.Count();
}
inline GUITreeNode * GUITreeNode::GetChild( UInt iChild ) const {
    Assert( iChild < m_arrChildren.Count() );
    return m_arrChildren[iChild];
}

inline const GChar * GUITreeNode::GetName( UInt * outLength ) const {
    if ( outLength != NULL )
        *outLength = m_iNameLength;
    return m_strName;
}
inline GUISkinImage GUITreeNode::GetIcon() const {
    return m_idIcon;
}

inline UInt GUITreeNode::GetNodeCount() const {
    return m_iNodeCount;
}

/////////////////////////////////////////////////////////////////////////////////
// GUITreeModel implementation
inline UInt GUITreeModel::GetDisplayTop() const {
    return m_iDisplayTop;
}
inline UInt GUITreeModel::GetDisplayCount() const {
    return m_iDisplayCount;
}

inline UInt GUITreeModel::GetNodeHeight() const {
    return m_iNodeHeight;
}

inline UInt GUITreeModel::GetNodeCount() const {
    return m_pRoot->GetNodeCount();
}
inline GUITreeNode * GUITreeModel::GetNode( UInt iNode ) const {
    return m_pRoot->GetNode( iNode );
}

inline GUITreeNode * GUITreeModel::GetNode( const GChar * strPath ) const {
    return m_pRoot->GetNode( strPath );
}

inline GUITreeNode * GUITreeModel::GetLastSelectedNode() const {
    return m_pLastSelected;
}

/////////////////////////////////////////////////////////////////////////////////
// GUITree implementation
inline GUIWidgetType GUITree::GetType() const {
    return GUIWIDGET_TREE;
}

