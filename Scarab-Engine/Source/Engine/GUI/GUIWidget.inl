/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/GUIWidget.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Base class for all window-interface elements ...
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// GUIWidgetModel implementation
inline Bool GUIWidgetModel::HasContextMenu() const {
    return ( m_pContextMenuModel != NULL );
}
inline GUIMenuModel * GUIWidgetModel::GetContextMenu() const {
    return m_pContextMenuModel;
}

/////////////////////////////////////////////////////////////////////////////////

inline GUIWidget * GUIWidgetModel::_GetWidget() const {
    return (GUIWidget*)m_pEventProcessor;
}

/////////////////////////////////////////////////////////////////////////////////
// GUIWidget implementation
inline GUIWidgetType GUIWidget::GetType() const {
    return GUIWIDGET_UNDEFINED;
}

inline UInt GUIWidget::GetLevel() const {
    return m_iLevel;
}
inline const Rectangle2 & GUIWidget::GetClientRect() const {
    return m_rectClient;
}

inline Bool GUIWidget::IsRoot() const {
    return ( m_pParent == NULL );
}
inline Bool GUIWidget::IsLeaf() const {
    return m_mapChildren.IsEmpty();
}

inline Bool GUIWidget::IsVisible() const {
    return m_bIsVisible;
}

inline GUIWindow * GUIWidget::GetParentWindow() const {
    return m_pParentWindow;
}
inline GUIWidget * GUIWidget::GetParent() const {
    return m_pParent;
}

inline UInt GUIWidget::GetChildCount() const {
    return m_mapChildren.Count();
}
inline Bool GUIWidget::HasChild( const GChar * strName ) const {
    return m_mapChildren.Contains(strName);
}
inline GUIWidget * GUIWidget::GetChild( const GChar * strName ) const {
    WidgetMap::Iterator itChild = m_mapChildren.Get( strName );
    if ( itChild.IsNull() )
        return NULL;
    return itChild.GetItem();
}

inline Void GUIWidget::EnumChildren() const {
    m_itEnumerate = m_mapChildren.Begin();
}
inline GUIWidget * GUIWidget::EnumNextChild() const {
    if ( m_itEnumerate.IsNull() )
        return NULL;
    GUIWidget * pNode = m_itEnumerate.GetItem();
    ++m_itEnumerate;
    return pNode;
}

inline Void GUIWidget::ScreenToLocal( Point2 * outLocalPos, const Point2 & ptScreenPos ) const {
    outLocalPos->X = ( ptScreenPos.X - m_rectClient.TopLeft.X );
    outLocalPos->Y = ( ptScreenPos.Y - m_rectClient.TopLeft.Y );
}
inline Void GUIWidget::LocalToScreen( Point2 * outScreenPos, const Point2 & ptLocalPos ) const {
    outScreenPos->X = ( m_rectClient.TopLeft.X + ptLocalPos.X );
    outScreenPos->Y = ( m_rectClient.TopLeft.Y + ptLocalPos.Y );
}

/////////////////////////////////////////////////////////////////////////////////

inline GUIWidgetModel * GUIWidget::_GetWidgetModel() const {
    return (GUIWidgetModel*)m_pModel;
}

inline Void GUIWidget::_SetVisibleOverride( Bool bVisible ) {
    m_bIsVisible = bVisible;
}

/////////////////////////////////////////////////////////////////////////////////

inline Int GUIWidget::_Name_Comparator( const GChar* const & rLeft, const GChar* const & rRight, Void * /*pUserData*/ ) {
    return StringFn->NCmp( rLeft, rRight, GUI_NAME_LENGTH - 1 );
}

inline Void GUIWidget::_ToLocalCoords( Point2 * outLocalPos, const Point2 & ptScreenPos ) const {
    ScreenToLocal( outLocalPos, ptScreenPos );
}
