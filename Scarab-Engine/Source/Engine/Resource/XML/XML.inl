/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Resource/XML/XML.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : XML documents parser & writer
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// XMLAttribute implementation
inline Bool XMLAttribute::IsIdentifier() const {
    return m_bIdentifier;
}

inline const GChar * XMLAttribute::GetName() const {
    return m_strName;
}
inline const GChar * XMLAttribute::GetValue( UInt * outLength ) const {
    if ( outLength != NULL )
        *outLength = ( m_strValue.Count() - 1 );
    return (const GChar *)m_strValue;
}
inline Array<GChar> * XMLAttribute::EditValue() {
    return &m_strValue;
}

/////////////////////////////////////////////////////////////////////////////////
// XMLNode implementation
inline XMLNodeType XMLNode::GetType() const {
    return m_iType;
}

inline Bool XMLNode::IsNode() const {
    return m_bIsNode;
}
inline Bool XMLNode::IsLeaf() const {
    return !m_bIsNode;
}
inline Bool XMLNode::IsRoot() const {
    return ( m_pParent == NULL );
}
inline UInt XMLNode::GetDepth() const {
    return m_iDepth;
}

inline const GChar * XMLNode::GetTagName() const {
    return m_strTagName;
}
inline const GChar * XMLNode::GetPathName() const {
    return m_strPathName;
}

inline Bool XMLNode::HasAttributes() const {
    return ( m_mapAttributes.Count() > 0 );
}
inline UInt XMLNode::GetAttributeCount() const {
    return m_mapAttributes.Count();
}
inline Bool XMLNode::HasAttribute( const GChar * strName ) const {
    return m_mapAttributes.Contains( strName );
}
inline XMLAttribute * XMLNode::GetAttribute( const GChar * strName ) const {
    _AttributeMap::Iterator itAttribute = m_mapAttributes.Get( strName );
    if ( itAttribute.IsNull() )
        return NULL;
    return itAttribute.GetItem();
}

inline Void XMLNode::EnumAttributes() const {
    m_itEnumerate = m_mapAttributes.Begin();
}
inline XMLAttribute * XMLNode::EnumNextAttribute() const {
    if ( m_itEnumerate.IsNull() )
        return NULL;
    XMLAttribute * pAttribute = m_itEnumerate.GetItem();
    ++m_itEnumerate;
    return pAttribute;
}

inline const GChar * XMLNode::GetIdentifierAttribute() const {
    return m_strIdentifierAttribute;
}

inline XMLDocument * XMLNode::GetParentDocument() const {
    return m_pParentDocument;
}
inline XMLNode * XMLNode::GetParent() const {
    return m_pParent;
}
inline UInt XMLNode::GetChildIndex() const {
    return m_iChildIndex;
}

inline XMLNode * XMLNode::GetPrevSibbling() const {
    return m_pPrevSibbling;
}
inline XMLNode * XMLNode::GetNextSibbling() const {
    return m_pNextSibbling;
}

inline Bool XMLNode::HasChildren() const {
    Assert( m_bIsNode );
    return ( m_arrChildren.Count() > 0 );
}
inline UInt XMLNode::GetChildCount() const {
    Assert( m_bIsNode );
    return m_arrChildren.Count();
}
inline XMLNode * XMLNode::GetChild( UInt iIndex ) const {
    Assert( m_bIsNode );
    Assert( iIndex < m_arrChildren.Count() );
    return m_arrChildren[iIndex];
}

inline XMLNode * XMLNode::GetChildByID( const GChar * strID, UInt * outIndex ) const {
    Assert( m_bIsNode );
    _ChildrenIDMap::Iterator itChild = m_mapChildrenID.Get( strID );
    if ( itChild.IsNull() ) {
        if ( outIndex != NULL )
            *outIndex = INVALID_OFFSET;
        return NULL;
    }
    if ( outIndex != NULL )
        *outIndex = itChild.GetItem()->m_iChildIndex;
    return itChild.GetItem();
}

inline XMLNode * XMLNode::Clone( Bool bRecursive ) const {
    return _Clone( bRecursive );
}

/////////////////////////////////////////////////////////////////////////////////

inline Int XMLNode::_Compare_Strings( const GChar * const & strLeft, const GChar * const & strRight, Void * /*pUserData*/ ) {
    return StringFn->NCmp( strLeft, strRight, XML_NAME_SIZE - 1 );
}

inline Bool XMLNode::_IsBlank( GChar ch ) {
    return ( ch == TEXT(' ') || ch == TEXT('\n') || ch == TEXT('\r') || ch == TEXT('\t') );
}
inline Bool XMLNode::_IsNameChar( GChar ch ) {
    return ( ( TEXT('A') <= ch && ch <= TEXT('Z') )
                || ( TEXT('a') <= ch && ch <= TEXT('z') )
                || ch == TEXT('_') );
}

/////////////////////////////////////////////////////////////////////////////////
// XMLComment implementation
inline const GChar * XMLComment::GetComment( UInt * outLength ) const {
    if ( outLength != NULL )
        *outLength = ( m_strComment.Count() - 1 );
    return (const GChar *)m_strComment;
}
inline Array<GChar> * XMLComment::EditComment() {
    return &m_strComment;
}

/////////////////////////////////////////////////////////////////////////////////
// XMLText implementation
inline const GChar * XMLText::GetText( UInt * outLength ) const {
    if ( outLength != NULL )
        *outLength = ( m_strText.Count() - 1 );
    return (const GChar *)m_strText;
}
inline Array<GChar> * XMLText::EditText() {
    return &m_strText;
}

/////////////////////////////////////////////////////////////////////////////////
// XMLUnparsedText implementation

/////////////////////////////////////////////////////////////////////////////////
// XMLDocument implementation
inline const GChar * XMLDocument::GetVersion() const {
    return m_strVersion;
}
inline const GChar * XMLDocument::GetEncoding() const {
    return m_strEncoding;
}

inline Void XMLDocument::SetVersion( const GChar * strVersion ) {
    StringFn->NCopy( m_strVersion, strVersion, 63 );
}
inline Void XMLDocument::SetEncoding( const GChar * strEncoding ) {
    StringFn->NCopy( m_strEncoding, strEncoding, 63 );
}

