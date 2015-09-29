/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Widgets/Extended/GUIList.inl
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
// GUIListNode implementation
inline Bool GUIListNode::IsSelected() const {
    return m_bSelected;
}

inline const GChar * GUIListNode::GetName( UInt * outLength ) const {
    if ( outLength != NULL )
        *outLength = m_iNameLength;
    return m_strName;
}
inline GUISkinImage GUIListNode::GetIcon() const {
    return m_idIcon;
}

/////////////////////////////////////////////////////////////////////////////////
// GUIListModel implementation
inline UInt GUIListModel::GetDisplayTop() const {
    return m_iDisplayTop;
}
inline UInt GUIListModel::GetDisplayCount() const {
    return m_iDisplayCount;
}

inline UInt GUIListModel::GetNodeHeight() const {
    return m_iNodeHeight;
}

inline UInt GUIListModel::GetNodeCount() const {
    return m_arrNodes.Count();
}
inline GUIListNode * GUIListModel::GetNode( UInt iNode ) const {
    Assert( iNode < m_arrNodes.Count() );
    return (GUIListNode*)( &(m_arrNodes[iNode]) );
}

/////////////////////////////////////////////////////////////////////////////////
// GUIList implementation
inline GUIWidgetType GUIList::GetType() const {
    return GUIWIDGET_LIST;
}

