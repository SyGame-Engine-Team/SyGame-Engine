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
// Includes
#include "XML.h"

#include "../ResourceManager.h"

/////////////////////////////////////////////////////////////////////////////////
// XMLAttribute implementation
XMLAttribute::XMLAttribute( const GChar * strName, const GChar * strValue ):
    m_strValue()
{
    m_bIdentifier = false;

    StringFn->NCopy( m_strName, strName, XML_NAME_SIZE - 1 );

    m_strValue.UseMemoryContext( ResourceFn->GetMemoryContext(), TEXT("XML") );
    m_strValue.Create();
    if ( strValue != NULL ) {
        while( *strValue != NULLBYTE )
            m_strValue.Push( *strValue++ );
    }
    m_strValue.Push( NULLBYTE );
}
XMLAttribute::~XMLAttribute()
{
    m_strValue.Destroy();
}

Void XMLAttribute::SetValue( const GChar * strValue ) 
{
    m_strValue.Clear();
    if ( strValue != NULL ) {
        while( *strValue != NULLBYTE )
            m_strValue.Push( *strValue++ );
    }
    m_strValue.Push( NULLBYTE );
}

/////////////////////////////////////////////////////////////////////////////////
// XMLNode implementation
XMLNode::XMLNode( XMLNodeType iType, const GChar * strTagName ):
    m_mapAttributes(), m_itEnumerate(), m_arrChildren(), m_mapChildrenID()
{
    // Type & Depth
    m_iType = iType;
    m_iDepth = 0;

    // Path & Name
    StringFn->NCopy( m_strTagName, strTagName, XML_NAME_SIZE - 1 );
    m_strPathName[0] = TEXT('/');
    StringFn->NCopy( m_strPathName + 1, m_strTagName, XML_URI_SIZE - 2 );

    // Attributes
    m_mapAttributes.SetComparator( _Compare_Strings, NULL );
    m_mapAttributes.UseMemoryContext( ResourceFn->GetMemoryContext(), TEXT("XML") );
    m_mapAttributes.Create();

    m_strIdentifierAttribute = NULL;

    // Linkage
    m_pParentDocument = NULL;
    m_pParent = NULL;
    m_iChildIndex = INVALID_OFFSET;

    m_pPrevSibbling = NULL;
    m_pNextSibbling = NULL;

    m_bIsNode = false;
}
XMLNode::~XMLNode()
{
    // Here we already are in XML memory context

    if ( m_bIsNode ) {
        // Recurse
        for( UInt i = 0; i < m_arrChildren.Count(); ++i )
            Delete( m_arrChildren[i] );

        // Linkage
        m_mapChildrenID.Destroy();
        m_arrChildren.Destroy();
    }

    // Attributes
    m_mapAttributes.Destroy();
}

XMLAttribute * XMLNode::CreateAttribute( const GChar * strName, const GChar * strValue )
{
    Assert( m_iType <= XML_DOCUMENT );

    XMLAttribute * pAttribute = NULL;

    _AttributeMap::Iterator itAttribute = m_mapAttributes.Get( strName );
    if ( itAttribute.IsNull() ) {
        ResourceFn->SelectMemory( TEXT("XML") );
        pAttribute = New XMLAttribute( strName, strValue );
        ResourceFn->UnSelectMemory();

        Bool bInserted = m_mapAttributes.Insert( pAttribute->GetName(), pAttribute );
        Assert( bInserted );
    } else
        pAttribute = itAttribute.GetItem();

    pAttribute->SetValue( strValue );
    return pAttribute;
}
Void XMLNode::DestroyAttribute( const GChar * strName )
{
    _AttributeMap::Iterator itAttribute = m_mapAttributes.Get( strName );
    if ( itAttribute.IsNull() )
        return;
    Bool bRemoved = m_mapAttributes.Remove( strName );
    Assert( bRemoved );
}

Void XMLNode::SetIdentifierAttribute( const GChar * strName )
{
    _AttributeMap::Iterator itAttribute;
    
    if ( m_strIdentifierAttribute != NULL ) {
        itAttribute = m_mapAttributes.Get( m_strIdentifierAttribute );
        Assert( !(itAttribute.IsNull()) );
        itAttribute.GetItem()->m_bIdentifier = false;
    }
    
    if ( strName != NULL ) {
        itAttribute = m_mapAttributes.Get( strName );
        Assert( !(itAttribute.IsNull()) );
        itAttribute.GetItem()->m_bIdentifier = true;
    }

    m_strIdentifierAttribute = strName;
}

XMLNode * XMLNode::GetChildByTag( const GChar * strTagName, UInt iOffset, UInt * outIndex ) const
{
    Assert( m_bIsNode );
    Assert( iOffset < m_arrChildren.Count() );

    for( UInt i = iOffset; i < m_arrChildren.Count(); ++i ) {
        if ( _Compare_Strings(strTagName,m_arrChildren[i]->m_strTagName,NULL) == 0 ) {
            if ( outIndex != NULL )
                *outIndex = i;
            return m_arrChildren[i];
        }
    }

    if ( outIndex != NULL )
        *outIndex = INVALID_OFFSET;
    return NULL;
}
XMLNode * XMLNode::GetChildNByTag( const GChar * strTagName, UInt iOccurence ) const
{
    Assert( m_bIsNode );
    UInt iCount = ( iOccurence + 1 );
    UInt iOffset = 0, iIndex = 0;
    XMLNode * pNode = NULL;

    for( UInt i = 0; i < iCount; ++i ) {
        pNode = GetChildByTag( strTagName, iOffset, &iIndex );
        if ( pNode == NULL )
            break;
        iOffset = iIndex + 1;
    }
    return pNode;
}
XMLNode * XMLNode::GetChildByTagPath( const GChar * strTagPath ) const
{
    Assert( m_bIsNode );
    GChar strTag[XML_NAME_SIZE];
    GChar strOccurence[8];
    GChar * pStr;

    // Skip heading slash
    if ( *strTagPath == TEXT('/') )
        ++strTagPath;

    // Empty path case
    if ( *strTagPath == NULLBYTE )
        return NULL;

    // Extract tag
    strTag[0] = NULLBYTE;
    strOccurence[0] = NULLBYTE;
    pStr = strTag;

    while( true ) {
        if ( *strTagPath == TEXT('/') || *strTagPath == NULLBYTE ) {
            *pStr = NULLBYTE;
            break;
        }
        if ( *strTagPath == TEXT('#') ) {
            *pStr = NULLBYTE;
            pStr = strOccurence;
            ++strTagPath;
            continue;
        }
        *pStr++ = *strTagPath++;
    }

    // Get Child
    UInt iOccurence = (UInt)( StringFn->ToUInt(strOccurence) );
    XMLNode * pChild = GetChildNByTag( strTag, iOccurence );
        
    // Child not found case
    if ( pChild == NULL )
        return NULL;

    // Leaf child case
    if ( pChild->IsLeaf() )
        return pChild;

    // End of path case
    if ( *strTagPath == NULLBYTE )
        return pChild;

    // Trailing slash case
    if ( *(strTagPath+1) == NULLBYTE )
        return pChild;

    // Recurse
    return pChild->GetChildByTagPath( strTagPath );
}

XMLNode * XMLNode::GetChildByAttribute( const GChar * strName, const GChar * strValue, UInt iOffset, UInt * outIndex ) const
{
    Assert( m_bIsNode );
    Assert( iOffset < m_arrChildren.Count() );

    for( UInt i = iOffset; i < m_arrChildren.Count(); ++i ) {
        XMLAttribute * pAttribute = m_arrChildren[i]->GetAttribute( strName );
        if ( pAttribute != NULL ) {
            if ( _Compare_Strings(strValue,pAttribute->GetValue(),NULL) == 0 ) {
                if ( outIndex != NULL )
                    *outIndex = i;
                return m_arrChildren[i];
            }
        }
    }

    if ( outIndex != NULL )
        *outIndex = INVALID_OFFSET;
    return NULL;
}
XMLNode * XMLNode::GetChildNByAttribute( const GChar * strName, const GChar * strValue, UInt iOccurence ) const
{
    Assert( m_bIsNode );
    UInt iCount = ( iOccurence + 1 );
    UInt iOffset = 0, iIndex = 0;
    XMLNode * pNode = NULL;

    for( UInt i = 0; i < iCount; ++i ) {
        pNode = GetChildByAttribute( strName, strValue, iOffset, &iIndex );
        if ( pNode == NULL )
            break;
        iOffset = iIndex + 1;
    }
    return pNode;
}
XMLNode * XMLNode::GetChildByAttributePath( const GChar * strAttributePath ) const
{
    Assert( m_bIsNode );
    GChar strName[XML_NAME_SIZE];
    GChar strValue[XML_NAME_SIZE];
    GChar strOccurence[8];
    GChar * pStr;

    // Skip heading slash
    if ( *strAttributePath == TEXT('/') )
        ++strAttributePath;

    // Empty path case
    if ( *strAttributePath == NULLBYTE )
        return NULL;

    // Extract attribute
    strName[0] = NULLBYTE;
    strValue[0] = NULLBYTE;
    strOccurence[0] = NULLBYTE;
    pStr = strName;

    while( true ) {
        if ( *strAttributePath == TEXT('/') || *strAttributePath == NULLBYTE ) {
            *pStr = NULLBYTE;
            break;
        }
        if ( *strAttributePath == TEXT(':') ) {
            *pStr = NULLBYTE;
            pStr = strValue;
            ++strAttributePath;
            continue;
        }
        if ( *strAttributePath == TEXT('#') ) {
            *pStr = NULLBYTE;
            pStr = strOccurence;
            ++strAttributePath;
            continue;
        }
        *pStr++ = *strAttributePath++;
    }

    // Get Child
    UInt iOccurence = (UInt)( StringFn->ToUInt(strOccurence) );
    XMLNode * pChild = GetChildNByAttribute( strName, strValue, iOccurence );
        
    // Child not found case
    if ( pChild == NULL )
        return NULL;

    // Leaf child case
    if ( pChild->IsLeaf() )
        return pChild;

    // End of path case
    if ( *strAttributePath == NULLBYTE )
        return pChild;

    // Trailing slash case
    if ( *(strAttributePath+1) == NULLBYTE )
        return pChild;

    // Recurse
    return pChild->GetChildByAttributePath( strAttributePath );
}

XMLNode * XMLNode::GetChildByIDPath( const GChar * strIDPath ) const
{
    Assert( m_bIsNode );
    GChar strID[XML_NAME_SIZE];
    GChar * pStr;

    // Skip heading slash
    if ( *strIDPath == TEXT('/') )
        ++strIDPath;

    // Empty path case
    if ( *strIDPath == NULLBYTE )
        return NULL;

    // Extract ID
    strID[0] = NULLBYTE;
    pStr = strID;

    while( true ) {
        if ( *strIDPath == TEXT('/') || *strIDPath == NULLBYTE ) {
            *pStr = NULLBYTE;
            break;
        }
        *pStr++ = *strIDPath++;
    }

    // Get Child
    XMLNode * pChild = GetChildByID( strID );
        
    // Child not found case
    if ( pChild == NULL )
        return NULL;

    // Leaf child case
    if ( pChild->IsLeaf() )
        return pChild;

    // End of path case
    if ( *strIDPath == NULLBYTE )
        return pChild;

    // Trailing slash case
    if ( *(strIDPath+1) == NULLBYTE )
        return pChild;

    // Recurse
    return pChild->GetChildByIDPath( strIDPath );
}

Void XMLNode::GetChildText( Array<GChar> * outBuffer ) const
{
    Assert( m_bIsNode );

    outBuffer->Clear();
    for( UInt i = 0; i < m_arrChildren.Count(); ++i ) {
        if ( m_arrChildren[i]->m_iType == XML_TEXT ) {
            XMLText * pText = (XMLText*)( m_arrChildren[i] );
            const Array<GChar> * strText = pText->EditText();
            outBuffer->Push( *strText );
        }
    }
    outBuffer->Push( NULLBYTE );
}
Void XMLNode::GetChildText( GChar * outBuffer, UInt iSize ) const
{
    Assert( m_bIsNode );

    UInt iLength = 0;
    for( UInt i = 0; i < m_arrChildren.Count(); ++i ) {
        if ( m_arrChildren[i]->m_iType == XML_TEXT ) {
            XMLText * pText = (XMLText*)( m_arrChildren[i] );
            UInt iTextLength = 0;
            const GChar * strText = pText->GetText( &iTextLength );

            iLength += iTextLength;
            if ( (iLength + 1) > iSize )
                break;

            outBuffer = StringFn->NCopy( outBuffer, strText, iSize - (iLength + 1) );
        }
    }
    *outBuffer = NULLBYTE;
}

UInt XMLNode::AppendChild( XMLNode * pNode )
{
    Assert( m_bIsNode );
    Assert( pNode->m_iType != XML_DOCUMENT );
    UInt iIndex = m_arrChildren.Count();

    // Depth
    pNode->_rec_SetDepth( m_iDepth + 1 );

    // Path names
    StringFn->Format( pNode->m_strPathName, TEXT("%s/%s"), m_strPathName, pNode->m_strTagName );

    // Parent linkage
    pNode->_rec_SetParentDocument( m_pParentDocument );
    pNode->m_pParent = this;
    pNode->m_iChildIndex = iIndex;

    // Child linkage
    m_arrChildren.Push( pNode );

    if ( m_strIdentifierAttribute != NULL ) {
        XMLAttribute * pIDAttribute = pNode->GetAttribute( m_strIdentifierAttribute );
        if ( pIDAttribute != NULL ) {
            pNode->SetIdentifierAttribute( m_strIdentifierAttribute );
            Bool bInserted = m_mapChildrenID.Insert( pIDAttribute->GetValue(), pNode );
            Assert( bInserted );
        }
    }

    // Sibbling linkage
    pNode->m_pPrevSibbling = NULL;
    pNode->m_pNextSibbling = NULL;
    if ( iIndex > 0 ) {
        XMLNode * pPrevNode = m_arrChildren[iIndex - 1];
        pPrevNode->m_pNextSibbling = pNode;
        pNode->m_pPrevSibbling = pPrevNode;
    }

    return iIndex;
}
Void XMLNode::InsertChild( XMLNode * pNode, UInt iIndex )
{
    Assert( m_bIsNode );
    Assert( pNode->m_iType != XML_DOCUMENT );
    Assert( iIndex <= m_arrChildren.Count() );

    // Depth
    pNode->_rec_SetDepth( m_iDepth + 1 );

    // Path names
    StringFn->Format( pNode->m_strPathName, TEXT("%s/%s"), m_strPathName, pNode->m_strTagName );

    // Parent linkage
    pNode->_rec_SetParentDocument( m_pParentDocument );
    pNode->m_pParent = this;
    pNode->m_iChildIndex = iIndex;

    // Child linkage
    m_arrChildren.Insert( iIndex, pNode );
    for( UInt i = iIndex + 1; i < m_arrChildren.Count(); ++i )
        ++(m_arrChildren[i]->m_iChildIndex);

    if ( m_strIdentifierAttribute != NULL ) {
        XMLAttribute * pIDAttribute = pNode->GetAttribute( m_strIdentifierAttribute );
        if ( pIDAttribute != NULL ) {
            pNode->SetIdentifierAttribute( m_strIdentifierAttribute );
            Bool bInserted = m_mapChildrenID.Insert( pIDAttribute->GetValue(), pNode );
            Assert( bInserted );
        }
    }

    // Sibbling linkage
    pNode->m_pPrevSibbling = NULL;
    pNode->m_pNextSibbling = NULL;
    if ( iIndex > 0 ) {
        XMLNode * pPrevNode = m_arrChildren[iIndex - 1];
        pPrevNode->m_pNextSibbling = pNode;
        pNode->m_pPrevSibbling = pPrevNode;
    }
    if ( iIndex < m_arrChildren.Count() - 1 ) {
        XMLNode * pNextNode = m_arrChildren[iIndex + 1];
        pNextNode->m_pPrevSibbling = pNode;
        pNode->m_pNextSibbling = pNextNode;
    }
}
XMLNode * XMLNode::ReplaceChild( XMLNode * pNode, UInt iIndex )
{
    Assert( m_bIsNode );
    Assert( pNode->m_iType != XML_DOCUMENT );
    Assert( iIndex < m_arrChildren.Count() );
    XMLNode * pReplacedNode = m_arrChildren[iIndex];

    // Depth
    pNode->_rec_SetDepth( m_iDepth + 1 );

    pReplacedNode->_rec_SetDepth( 0 );

    // Path names
    StringFn->Format( pNode->m_strPathName, TEXT("%s/%s"), m_strPathName, pNode->m_strTagName );

    pReplacedNode->m_strPathName[0] = TEXT('/');
    StringFn->NCopy( pReplacedNode->m_strPathName + 1, pReplacedNode->m_strTagName, XML_URI_SIZE - 2 );

    // Parent linkage
    pNode->_rec_SetParentDocument( m_pParentDocument );
    pNode->m_pParent = this;
    pNode->m_iChildIndex = iIndex;

    pReplacedNode->_rec_SetParentDocument( NULL );
    pReplacedNode->m_pParent = NULL;
    pReplacedNode->m_iChildIndex = INVALID_OFFSET;

    // Child linkage
    m_arrChildren[iIndex] = pNode;

    if ( m_strIdentifierAttribute != NULL ) {
        XMLAttribute * pIDAttribute = pReplacedNode->GetAttribute( m_strIdentifierAttribute );
        if ( pIDAttribute != NULL ) {
            pReplacedNode->SetIdentifierAttribute( NULL );
            Bool bRemoved = m_mapChildrenID.Remove( pIDAttribute->GetValue() );
            Assert( bRemoved );
        }
        pIDAttribute = pNode->GetAttribute( m_strIdentifierAttribute );
        if ( pIDAttribute != NULL ) {
            pNode->SetIdentifierAttribute( m_strIdentifierAttribute );
            Bool bInserted = m_mapChildrenID.Insert( pIDAttribute->GetValue(), pNode );
            Assert( bInserted );
        }
    }

    // Sibbling linkage
    pNode->m_pPrevSibbling = pReplacedNode->m_pPrevSibbling;
    pNode->m_pNextSibbling = pReplacedNode->m_pNextSibbling;
    if ( pNode->m_pPrevSibbling != NULL )
        pNode->m_pPrevSibbling->m_pNextSibbling = pNode;
    if ( pNode->m_pNextSibbling != NULL )
        pNode->m_pNextSibbling->m_pPrevSibbling = pNode;

    pReplacedNode->m_pNextSibbling = NULL;
    pReplacedNode->m_pPrevSibbling = NULL;

    // Done
    return pReplacedNode;
}
XMLNode * XMLNode::RemoveChild( UInt iIndex )
{
    Assert( m_bIsNode );
    Assert( iIndex < m_arrChildren.Count() );
    XMLNode * pRemovedNode = m_arrChildren[iIndex];

    // Depth
    pRemovedNode->_rec_SetDepth( 0 );

    // Path names
    pRemovedNode->m_strPathName[0] = TEXT('/');
    StringFn->NCopy( pRemovedNode->m_strPathName + 1, pRemovedNode->m_strTagName, XML_URI_SIZE - 2 );

    // Parent linkage
    pRemovedNode->_rec_SetParentDocument( NULL );
    pRemovedNode->m_pParent = NULL;
    pRemovedNode->m_iChildIndex = INVALID_OFFSET;

    // Child linkage
    m_arrChildren.Remove( iIndex, NULL, 1 );
    for( UInt i = iIndex; i < m_arrChildren.Count(); ++i )
        --(m_arrChildren[i]->m_iChildIndex);
    
    if ( m_strIdentifierAttribute != NULL ) {
        XMLAttribute * pIDAttribute = pRemovedNode->GetAttribute( m_strIdentifierAttribute );
        if ( pIDAttribute != NULL ) {
            pRemovedNode->SetIdentifierAttribute( NULL );
            Bool bRemoved = m_mapChildrenID.Remove( pIDAttribute->GetValue() );
            Assert( bRemoved );
        }
    }

    // Sibbling linkage
    if ( pRemovedNode->m_pPrevSibbling != NULL )
        pRemovedNode->m_pPrevSibbling->m_pNextSibbling = pRemovedNode->m_pNextSibbling;
    if ( pRemovedNode->m_pNextSibbling != NULL )
        pRemovedNode->m_pNextSibbling->m_pPrevSibbling = pRemovedNode->m_pPrevSibbling;
    pRemovedNode->m_pPrevSibbling = NULL;
    pRemovedNode->m_pNextSibbling = NULL;

    // Done
    return pRemovedNode;
}

Void XMLNode::InsertBefore( XMLNode * pNode )
{
    Assert( m_iType != XML_DOCUMENT );
    Assert( pNode->m_iType != XML_DOCUMENT );

    // Depth
    pNode->_rec_SetDepth( m_iDepth );

    // Path names
    if ( m_pParent != NULL )
        StringFn->Format( pNode->m_strPathName, TEXT("%s/%s"), m_pParent->m_strPathName, pNode->m_strTagName );

    // Parent linkage
    pNode->_rec_SetParentDocument( m_pParentDocument );
    pNode->m_pParent = m_pParent;
    pNode->m_iChildIndex = INVALID_OFFSET;

    // Child linkage
    if ( m_pParent != NULL ) {
        pNode->m_iChildIndex = m_iChildIndex;
        m_pParent->m_arrChildren.Insert( pNode->m_iChildIndex, pNode );
        for( UInt i = pNode->m_iChildIndex + 1; i < m_pParent->m_arrChildren.Count(); ++i )
            ++(m_pParent->m_arrChildren[i]->m_iChildIndex);

        if ( m_pParent->m_strIdentifierAttribute != NULL ) {
            XMLAttribute * pIDAttribute = pNode->GetAttribute( m_pParent->m_strIdentifierAttribute );
            if ( pIDAttribute != NULL ) {
                Bool bInserted = m_pParent->m_mapChildrenID.Insert( pIDAttribute->GetValue(), pNode );
                Assert( bInserted );
            }
        }
    }

    // Sibbling linkage
    pNode->m_pPrevSibbling = m_pPrevSibbling;
    pNode->m_pNextSibbling = this;
    if ( m_pPrevSibbling != NULL )
        m_pPrevSibbling->m_pNextSibbling = pNode;
    m_pPrevSibbling = pNode;
}
Void XMLNode::InsertAfter( XMLNode * pNode )
{
    Assert( m_iType != XML_DOCUMENT );
    Assert( pNode->m_iType != XML_DOCUMENT );

    // Depth
    pNode->_rec_SetDepth( m_iDepth );

    // Path names
    if ( m_pParent != NULL )
        StringFn->Format( pNode->m_strPathName, TEXT("%s/%s"), m_pParent->m_strPathName, pNode->m_strTagName );

    // Parent linkage
    pNode->_rec_SetParentDocument( m_pParentDocument );
    pNode->m_pParent = m_pParent;
    pNode->m_iChildIndex = INVALID_OFFSET;

    // Child linkage
    if ( m_pParent != NULL ) {
        pNode->m_iChildIndex = m_iChildIndex + 1;
        m_pParent->m_arrChildren.Insert( pNode->m_iChildIndex, pNode );
        for( UInt i = pNode->m_iChildIndex + 1; i < m_pParent->m_arrChildren.Count(); ++i )
            ++(m_pParent->m_arrChildren[i]->m_iChildIndex);

        if ( m_pParent->m_strIdentifierAttribute != NULL ) {
            XMLAttribute * pIDAttribute = pNode->GetAttribute( m_pParent->m_strIdentifierAttribute );
            if ( pIDAttribute != NULL ) {
                Bool bInserted = m_pParent->m_mapChildrenID.Insert( pIDAttribute->GetValue(), pNode );
                Assert( bInserted );
            }
        }
    }

    // Sibbling linkage
    pNode->m_pPrevSibbling = this;
    pNode->m_pNextSibbling = m_pNextSibbling;
    if ( m_pNextSibbling != NULL )
        m_pNextSibbling->m_pPrevSibbling = pNode;
    m_pNextSibbling = pNode;
}

Bool XMLNode::Parse( const GChar * strFile )
{
    HFile hFile = SystemFn->OpenFile( strFile, FILE_READ );
    if ( !(hFile.IsValid()) )
        return false;

    XMLToken iOverflowToken = XMLTOKEN_UNDEFINED;
    GChar chOverflowChar = NULLBYTE;
    Bool bValid = _Parse( &iOverflowToken, &chOverflowChar, _ReadCallback_File, &hFile );
    Assert( iOverflowToken == XMLTOKEN_UNDEFINED );
    Assert( chOverflowChar == NULLBYTE );

    hFile.Close();
    return bValid;
}
Bool XMLNode::ParseXML( const GChar * strXML )
{
    XMLToken iOverflowToken = XMLTOKEN_UNDEFINED;
    GChar chOverflowChar = NULLBYTE;
    Bool bValid = _Parse( &iOverflowToken, &chOverflowChar, _ReadCallback_XML, &strXML );
    Assert( iOverflowToken == XMLTOKEN_UNDEFINED );
    Assert( chOverflowChar == NULLBYTE );

    return bValid;
}

Bool XMLNode::Render( const GChar * strFile ) const
{
    HFile hFile = SystemFn->CreateFile( strFile, FILE_WRITE );
    if ( !(hFile.IsValid()) )
        return false;

    Bool bFinished = _Render( _WriteCallback_File, &hFile );

    hFile.Close();
    return bFinished;
}
Bool XMLNode::RenderXML( Array<GChar> * outXML ) const
{
    return _Render( _WriteCallback_XML, outXML );
}

/////////////////////////////////////////////////////////////////////////////////

Void XMLNode::_MakeLeaf()
{
    Assert( !m_bIsNode );

    // nothing to do
}
Void XMLNode::_MakeNode()
{
    if ( m_bIsNode )
        return;

    // Create children storage
    m_arrChildren.UseMemoryContext( ResourceFn->GetMemoryContext(), TEXT("XML") );
    m_arrChildren.Create();

    m_mapChildrenID.SetComparator( _Compare_Strings, NULL );
    m_mapChildrenID.UseMemoryContext( ResourceFn->GetMemoryContext(), TEXT("XML") );
    m_mapChildrenID.Create();

    m_bIsNode = true;
}

Void XMLNode::_rec_SetDepth( UInt iDepth )
{
    // Set depth
    m_iDepth = iDepth;

    // Recurse
    if ( m_bIsNode ) {
        for( UInt i = 0; i < m_arrChildren.Count(); ++i )
            m_arrChildren[i]->_rec_SetDepth( iDepth + 1 );
    }
}
Void XMLNode::_rec_SetParentDocument( XMLDocument * pDocument )
{
    // Set parent document
    m_pParentDocument = pDocument;

    // Recurse
    if ( m_bIsNode ) {
        for( UInt i = 0; i < m_arrChildren.Count(); ++i )
            m_arrChildren[i]->_rec_SetParentDocument( pDocument );
    }
}

Bool XMLNode::_ReadCallback_File( GChar * outCh, Void * pUserData )
{
    HFile * pFile = (HFile*)pUserData;
    return pFile->ReadChar( outCh );
}
Bool XMLNode::_ReadCallback_XML( GChar * outCh, Void * pUserData )
{
    const GChar ** pXML = (const GChar **)pUserData;
    *outCh = **pXML;
    ++(*pXML);
    return true;
}

Bool XMLNode::_WriteCallback_File( const GChar * str, Void * pUserData )
{
    HFile * pFile = (HFile*)pUserData;
    return pFile->WriteString( str );
}
Bool XMLNode::_WriteCallback_XML( const GChar * str, Void * pUserData )
{
    Array<GChar> * pXML = (Array<GChar>*)pUserData;
    while( *str != NULLBYTE )
        pXML->Push( *str++ );
    return true;
}

XMLNode * XMLNode::_Clone( Bool bRecursive ) const
{
    // Create clone
    ResourceFn->SelectMemory( TEXT("XML") );
    XMLNode * pClone = New XMLNode( XML_NODE, m_strTagName );
    ResourceFn->UnSelectMemory();

    if ( m_bIsNode )
        pClone->_MakeNode();
    else
        pClone->_MakeLeaf();

    // Copy attributes
    _AttributeMap::Iterator itAttribute = m_mapAttributes.Begin();
    while( !(itAttribute.IsNull()) ) {
        pClone->CreateAttribute( itAttribute.GetKey(), itAttribute.GetItem()->GetValue() );
        ++itAttribute;
    }
    pClone->m_strIdentifierAttribute = m_strIdentifierAttribute;

    // Recurse
    if ( bRecursive && m_bIsNode ) {
        for( UInt i = 0; i < m_arrChildren.Count(); ++i ) {
            XMLNode * pChildClone = m_arrChildren[i]->Clone( true );
            pClone->AppendChild( pChildClone );
        }
    }

    // Done
    return pClone;
}

Void XMLNode::_GetIndentString( GChar * outIndentString ) const
{
    Assert( (m_iDepth+2) < 256 );
    outIndentString[0] = TEXT('\r');
    outIndentString[1] = TEXT('\n');
    for( UInt i = 0; i < m_iDepth; ++i )
        outIndentString[i+2] = TEXT('\t');
    outIndentString[m_iDepth+2] = NULLBYTE;
}

Bool XMLNode::_Render( _XMLWriteCallback pfCallback, Void * pUserData ) const
{
    Bool bContinue;
    GChar strIndent[256];
    _GetIndentString( strIndent );

    // Indentation
    bContinue = pfCallback( strIndent, pUserData );
    if ( !bContinue )
        return false;

    // Start opening tag
    bContinue = pfCallback( TEXT("<"), pUserData );
    if ( !bContinue )
        return false;

    // Tag Name
    bContinue = pfCallback( m_strTagName, pUserData );
    if ( !bContinue )
        return false;

    // Attributes
    _AttributeMap::Iterator itAttribute = m_mapAttributes.Begin();
    while( !(itAttribute.IsNull()) ) {
        XMLAttribute * pAttribute = itAttribute.GetItem();
        bContinue = pfCallback( TEXT(" "), pUserData );
        if ( !bContinue )
            return false;
        bContinue = pfCallback( pAttribute->GetName(), pUserData );
        if ( !bContinue )
            return false;
        bContinue = pfCallback( TEXT("=\""), pUserData );
        if ( !bContinue )
            return false;
        bContinue = pfCallback( pAttribute->GetValue(), pUserData );
        if ( !bContinue )
            return false;
        bContinue = pfCallback( TEXT("\""), pUserData );
        if ( !bContinue )
            return false;
        ++itAttribute;
    }

    // Leaf case
    if ( !m_bIsNode ) {
        // End opening tag
        bContinue = pfCallback( TEXT("/>"), pUserData );
        if ( !bContinue )
            return false;

        return true;
    }

    // End opening tag
    bContinue = pfCallback( TEXT(">"), pUserData );
    if ( !bContinue )
        return false;

    // Recurse
    for( UInt i = 0; i < m_arrChildren.Count(); ++i ) {
        bContinue = m_arrChildren[i]->_Render( pfCallback, pUserData );
        if ( !bContinue )
            return false;
    }

    // Indentation
    if ( m_arrChildren.Count() > 0 ) {
        bContinue = pfCallback( strIndent, pUserData );
        if ( !bContinue )
            return false;
    }

    // Start closing tag
    bContinue = pfCallback( TEXT("</"), pUserData );
    if ( !bContinue )
        return false;

    // Tag Name
    bContinue = pfCallback( m_strTagName, pUserData );
    if ( !bContinue )
        return false;

    // End closing tag
    bContinue = pfCallback( TEXT(">"), pUserData );
    if ( !bContinue )
        return false;

    return true;
}

XMLToken XMLNode::_Parse_NextToken( GChar * outTokenData, GChar * pOverflowChar, _XMLReadCallback pfCallback, Void * pUserData )
{
    Bool bContinue;
    GChar * strOut = outTokenData;

    // Perform lexical analysis
    GChar ch;
    UInt iState = 0;
    while( true ) {
        switch( iState ) {
            case 0: { // Initial state
                if ( *pOverflowChar == NULLBYTE )
                    bContinue = pfCallback( &ch, pUserData );
                else {
                    ch = *pOverflowChar;
                    *pOverflowChar = NULLBYTE;
                    bContinue = true;
                }
                if ( !bContinue ) {
                    *outTokenData = NULLBYTE;
                    return XMLTOKEN_UNDEFINED;
                }

                if ( _IsBlank(ch) ) {
                    *strOut++ = ch;
                    iState = 1;
                    break;
                }
                if ( ch == TEXT('<') ) {
                    *strOut++ = ch;
                    iState = 2;
                    break;
                }
                if ( ch == TEXT('>') ) {
                    *strOut++ = ch;
                    *strOut = NULLBYTE;
                    return XMLTOKEN_TAGEND;
                }
                if ( ch == TEXT('/') ) {
                    *strOut++ = ch;
                    iState = 3;
                    break;
                }
                if ( ch == TEXT('?') ) {
                    *strOut++ = ch;
                    iState = 4;
                    break;
                }
                if ( ch == TEXT('-') ) {
                    *strOut++ = ch;
                    iState = 5;
                    break;
                }
                if ( ch == TEXT('=') ) {
                    *strOut++ = ch;
                    *strOut = NULLBYTE;
                    return XMLTOKEN_ATTRIB_AFFECT;
                }
                if ( ch == TEXT('"') ) {
                    *strOut++ = ch;
                    *strOut = NULLBYTE;
                    return XMLTOKEN_ATTRIB_STRING;
                }
                if ( _IsNameChar(ch) ) {
                    *strOut++ = ch;
                    iState = 6;
                    break;
                }
                *strOut++ = ch;
                *strOut = NULLBYTE;
                return XMLTOKEN_TEXT;
            } break;
            case 1: { // Separator state
                bContinue = pfCallback( &ch, pUserData );
                if ( !bContinue ) {
                    *outTokenData = NULLBYTE;
                    return XMLTOKEN_UNDEFINED;
                }

                if ( !(_IsBlank(ch)) ) {
                    *strOut = NULLBYTE;
                    *pOverflowChar = ch;
                    return XMLTOKEN_SEPARATOR;
                }
                *strOut++ = ch;
            } break;
            case 2: { // Tag start state
                bContinue = pfCallback( &ch, pUserData );
                if ( !bContinue ) {
                    *outTokenData = NULLBYTE;
                    return XMLTOKEN_UNDEFINED;
                }

                if ( ch == TEXT('/') ) {
                    *strOut++ = ch;
                    *strOut = NULLBYTE;
                    return XMLTOKEN_TAGSTART_CLOSENODE;
                }
                if ( ch == TEXT('?') ) {
                    *strOut++ = ch;
                    iState = 7;
                    break;
                }
                if ( ch == TEXT('!') ) {
                    *strOut++ = ch;
                    iState = 8;
                    break;
                }
                if ( _IsNameChar(ch) ) {
                    *strOut = NULLBYTE;
                    *pOverflowChar = ch;
                    return XMLTOKEN_TAGSTART;
                }
                *strOut++ = ch;
                *strOut = NULLBYTE;
                return XMLTOKEN_TEXT;
            } break;
            case 3: { // Tag end (leaf) state
                bContinue = pfCallback( &ch, pUserData );
                if ( !bContinue ) {
                    *outTokenData = NULLBYTE;
                    return XMLTOKEN_UNDEFINED;
                }

                if ( ch == TEXT('>') ) {
                    *strOut++ = ch;
                    *strOut = NULLBYTE;
                    return XMLTOKEN_TAGEND_LEAF;
                }
                *strOut++ = ch;
                *strOut = NULLBYTE;
                return XMLTOKEN_TEXT;
            } break;
            case 4: { // Header end state
                bContinue = pfCallback( &ch, pUserData );
                if ( !bContinue ) {
                    *outTokenData = NULLBYTE;
                    return XMLTOKEN_UNDEFINED;
                }

                if ( ch == TEXT('>') ) {
                    *strOut++ = ch;
                    *strOut = NULLBYTE;
                    return XMLTOKEN_HEADER_END;
                }
                *strOut++ = ch;
                *strOut = NULLBYTE;
                return XMLTOKEN_TEXT;
            } break;
            case 5: { // Comment end state 1
                bContinue = pfCallback( &ch, pUserData );
                if ( !bContinue ) {
                    *outTokenData = NULLBYTE;
                    return XMLTOKEN_UNDEFINED;
                }

                if ( ch == TEXT('-') ) {
                    *strOut++ = ch;
                    iState = 9;
                    break;
                }
                *strOut++ = ch;
                *strOut = NULLBYTE;
                return XMLTOKEN_TEXT;
            } break;
            case 6: { // Name state
                bContinue = pfCallback( &ch, pUserData );
                if ( !bContinue ) {
                    *outTokenData = NULLBYTE;
                    return XMLTOKEN_UNDEFINED;
                }

                if ( !(_IsNameChar(ch)) ) {
                    *strOut = NULLBYTE;
                    *pOverflowChar = ch;
                    return XMLTOKEN_NAME;
                }
                *strOut++ = ch;
            } break;
            case 7: { // Header start state 1
                bContinue = pfCallback( &ch, pUserData );
                if ( !bContinue ) {
                    *outTokenData = NULLBYTE;
                    return XMLTOKEN_UNDEFINED;
                }

                if ( ch == TEXT('x') ) {
                    *strOut++ = ch;
                    iState = 10;
                    break;
                }
                *strOut++ = ch;
                *strOut = NULLBYTE;
                return XMLTOKEN_TEXT;
            } break;
            case 8: { // Comment start state 1
                bContinue = pfCallback( &ch, pUserData );
                if ( !bContinue ) {
                    *outTokenData = NULLBYTE;
                    return XMLTOKEN_UNDEFINED;
                }

                if ( ch == TEXT('-') ) {
                    *strOut++ = ch;
                    iState = 11;
                    break;
                }
                *strOut++ = ch;
                *strOut = NULLBYTE;
                return XMLTOKEN_TEXT;
            } break;
            case 9: { // Comment end state 2
                bContinue = pfCallback( &ch, pUserData );
                if ( !bContinue ) {
                    *outTokenData = NULLBYTE;
                    return XMLTOKEN_UNDEFINED;
                }

                if ( ch == TEXT('>') ) {
                    *strOut++ = ch;
                    *strOut = NULLBYTE;
                    return XMLTOKEN_COMMENT_END;
                }
                *strOut++ = ch;
                *strOut = NULLBYTE;
                return XMLTOKEN_TEXT;
            } break;
            case 10: { // Header start state 2
                bContinue = pfCallback( &ch, pUserData );
                if ( !bContinue ) {
                    *outTokenData = NULLBYTE;
                    return XMLTOKEN_UNDEFINED;
                }

                if ( ch == TEXT('m') ) {
                    *strOut++ = ch;
                    iState = 12;
                    break;
                }
                *strOut++ = ch;
                *strOut = NULLBYTE;
                return XMLTOKEN_TEXT;
            } break;
            case 11: { // Comment start state 2
                bContinue = pfCallback( &ch, pUserData );
                if ( !bContinue ) {
                    *outTokenData = NULLBYTE;
                    return XMLTOKEN_UNDEFINED;
                }

                if ( ch == TEXT('-') ) {
                    *strOut++ = ch;
                    *strOut = NULLBYTE;
                    return XMLTOKEN_COMMENT_START;
                }
                *strOut++ = ch;
                *strOut = NULLBYTE;
                return XMLTOKEN_TEXT;
            } break;
            case 12: { // Header start state 3
                bContinue = pfCallback( &ch, pUserData );
                if ( !bContinue ) {
                    *outTokenData = NULLBYTE;
                    return XMLTOKEN_UNDEFINED;
                }

                if ( ch == TEXT('l') ) {
                    *strOut++ = ch;
                    *strOut = NULLBYTE;
                    return XMLTOKEN_HEADER_START;
                }
                *strOut++ = ch;
                *strOut = NULLBYTE;
                return XMLTOKEN_TEXT;
            } break;
            default: Assert(false); *outTokenData = NULLBYTE; return XMLTOKEN_UNDEFINED;
        }
    }
}

Bool XMLNode::_Parse_AttributeList( XMLToken * pOverflowToken, GChar * pOverflowChar, _XMLReadCallback pfCallback, Void * pUserData )
{
    GChar strTokenData[XML_NAME_SIZE];
    GChar strName[XML_NAME_SIZE];

    // Separator 
    XMLToken iToken = _Parse_NextToken( strTokenData, pOverflowChar, pfCallback, pUserData );
    if ( iToken != XMLTOKEN_SEPARATOR ) {
        *pOverflowToken = iToken;
        return true;
    }

    // Name
    iToken = _Parse_NextToken( strName, pOverflowChar, pfCallback, pUserData );
    if ( iToken != XMLTOKEN_NAME ) {
        *pOverflowToken = iToken;
        return true;
    }

    // Affectation sign
    iToken = _Parse_NextToken( strTokenData, pOverflowChar, pfCallback, pUserData );
    if ( iToken == XMLTOKEN_SEPARATOR )
        iToken = _Parse_NextToken( strTokenData, pOverflowChar, pfCallback, pUserData );
    if ( iToken != XMLTOKEN_ATTRIB_AFFECT ) {
        *pOverflowToken = XMLTOKEN_UNDEFINED;
        return false;
    }

    // Value start
    iToken = _Parse_NextToken( strTokenData, pOverflowChar, pfCallback, pUserData );
    if ( iToken == XMLTOKEN_SEPARATOR )
        iToken = _Parse_NextToken( strTokenData, pOverflowChar, pfCallback, pUserData );
    if ( iToken != XMLTOKEN_ATTRIB_STRING ) {
        *pOverflowToken = XMLTOKEN_UNDEFINED;
        return false;
    }

    // Create attribute
    XMLAttribute * pAttribute = CreateAttribute( strName );
    Array<GChar> * pValue = pAttribute->EditValue();
    pValue->Clear();

    // Extract value
    Assert( *pOverflowChar == NULLBYTE );
    GChar ch;
    Bool bContinue = pfCallback( &ch, pUserData );
    if ( !bContinue ) {
        pValue->Push( NULLBYTE );
        *pOverflowToken = XMLTOKEN_UNDEFINED;
        return false;
    }
    while( ch != TEXT('"') ) {
        // Escape string
        if ( ch == TEXT('\\') ) {
            Bool bContinue = pfCallback( &ch, pUserData );
            if ( !bContinue ) {
                pValue->Push( NULLBYTE );
                *pOverflowToken = XMLTOKEN_UNDEFINED;
                return false;
            }
            if ( ch == TEXT('\\') )
                pValue->Push( TEXT('\\') );
            else if ( ch == TEXT('n') )
                pValue->Push( TEXT('\n') );
            else if ( ch == TEXT('t') )
                pValue->Push( TEXT('\t') );
            else if ( ch == TEXT('"') )
                pValue->Push( TEXT('"') );
            else {
                pValue->Push( NULLBYTE );
                *pOverflowToken = XMLTOKEN_UNDEFINED;
                return false;
            }
        } else
            pValue->Push( ch );

        Bool bContinue = pfCallback( &ch, pUserData );
        if ( !bContinue ) {
            pValue->Push( NULLBYTE );
            *pOverflowToken = XMLTOKEN_UNDEFINED;
            return false;
        }
    }
    pValue->Push( NULLBYTE );

    // Recurse
    return _Parse_AttributeList( pOverflowToken, pOverflowChar, pfCallback, pUserData );
}
Bool XMLNode::_Parse_NodeContent( XMLToken * pOverflowToken, GChar * pOverflowChar, _XMLReadCallback pfCallback, Void * pUserData )
{
    GChar strTokenData[XML_NAME_SIZE];

    // Parse next token
    XMLToken iToken;
    if ( *pOverflowToken == XMLTOKEN_UNDEFINED )
        iToken = _Parse_NextToken( strTokenData, pOverflowChar, pfCallback, pUserData );
    else {
        iToken = *pOverflowToken;
        *pOverflowToken = XMLTOKEN_UNDEFINED;
    }
    if ( iToken == XMLTOKEN_SEPARATOR )
        iToken = _Parse_NextToken( strTokenData, pOverflowChar, pfCallback, pUserData );

    // Closing case
    if ( iToken == XMLTOKEN_TAGSTART_CLOSENODE ) {
        *pOverflowToken = iToken;
        return true;
    }

    // Node / Leaf case
    if ( iToken == XMLTOKEN_TAGSTART ) {
        *pOverflowToken = iToken;

        // Create child
        ResourceFn->SelectMemory( TEXT("XML") );
        XMLNode * pChild = New XMLNode( XML_NODE, TEXT("_xmlnew") );
        ResourceFn->UnSelectMemory();

        // Recurse on child
        Bool bContinue = pChild->_Parse( pOverflowToken, pOverflowChar, pfCallback, pUserData );
        if ( !bContinue )
            return false;

        // Add child
        AppendChild( pChild );

        // Recurse
        return _Parse_NodeContent( pOverflowToken, pOverflowChar, pfCallback, pUserData );
    }

    // Comment case
    if ( iToken == XMLTOKEN_COMMENT_START ) {
        // Create child
        ResourceFn->SelectMemory( TEXT("XML") );
        XMLComment * pChild = New XMLComment();
        ResourceFn->UnSelectMemory();

        // Extract Comment
        Array<GChar> * pComment = pChild->EditComment();
        pComment->Clear();

        Assert( *pOverflowChar == NULLBYTE );
        GChar ch;
        Bool bContinue = pfCallback( &ch, pUserData );
        if ( !bContinue ) {
            pComment->Push( NULLBYTE );
            return false;
        }
        while( true ) {
            if ( ch == TEXT('-') ) {
                Bool bContinue = pfCallback( &ch, pUserData );
                if ( !bContinue ) {
                    pComment->Push( NULLBYTE );
                    return false;
                }

                if ( ch == TEXT('-') ) {
                    Bool bContinue = pfCallback( &ch, pUserData );
                    if ( !bContinue ) {
                        pComment->Push( NULLBYTE );
                        return false;
                    }

                    if ( ch == TEXT('>') )
                        break;
                    else {
                        pComment->Push( TEXT('-') );
                        pComment->Push( TEXT('-') );
                    }
                } else
                    pComment->Push( TEXT('-') );
            }
            pComment->Push( ch );

            Bool bContinue = pfCallback( &ch, pUserData );
            if ( !bContinue ) {
                pComment->Push( NULLBYTE );
                return false;
            }
        }
        pComment->Push( NULLBYTE );

        // Add child
        AppendChild( pChild );

        // Recurse
        return _Parse_NodeContent( pOverflowToken, pOverflowChar, pfCallback, pUserData );
    }

    // Text case
        // Create child
    ResourceFn->SelectMemory( TEXT("XML") );
    XMLText * pChild = New XMLText();
    ResourceFn->UnSelectMemory();

        // Extract text
    Array<GChar> * pText = pChild->EditText();
    pText->Clear();
    pText->Push( strTokenData, StringFn->Length(strTokenData) );

    Bool bContinue;
    GChar ch;
    if ( *pOverflowChar == NULLBYTE ) {
        bContinue = pfCallback( &ch, pUserData );
        if ( !bContinue ) {
            pText->Push( NULLBYTE );
            return false;
        }
    } else {
        ch = *pOverflowChar;
        *pOverflowChar = NULLBYTE;
    }
    while( ch != TEXT('<') ) {
        pText->Push( ch );

        Bool bContinue = pfCallback( &ch, pUserData );
        if ( !bContinue ) {
            pText->Push( NULLBYTE );
            return false;
        }
    }
    pText->Push( NULLBYTE );
    *pOverflowChar = ch;

        // Add child
    AppendChild( pChild );

        // Recurse
    return _Parse_NodeContent( pOverflowToken, pOverflowChar, pfCallback, pUserData );
}

Bool XMLNode::_Parse( XMLToken * pOverflowToken, GChar * pOverflowChar, _XMLReadCallback pfCallback, Void * pUserData )
{
    GChar strTokenData[XML_NAME_SIZE];

    // Parse open tag
    XMLToken iToken;
    if ( *pOverflowToken == XMLTOKEN_UNDEFINED )
        iToken = _Parse_NextToken( strTokenData, pOverflowChar, pfCallback, pUserData );
    else {
        iToken = *pOverflowToken;
        *pOverflowToken = XMLTOKEN_UNDEFINED;
    }
    if ( iToken == XMLTOKEN_SEPARATOR )
        iToken = _Parse_NextToken( strTokenData, pOverflowChar, pfCallback, pUserData );
    if ( iToken != XMLTOKEN_TAGSTART )
        return false;

        // Name & path name
    iToken = _Parse_NextToken( strTokenData, pOverflowChar, pfCallback, pUserData );
    if ( iToken != XMLTOKEN_NAME )
        return false;

    StringFn->NCopy( m_strTagName, strTokenData, XML_NAME_SIZE );
    if ( m_pParent != NULL )
        StringFn->Format( m_strPathName, TEXT("%s/%s"), m_pParent->m_strPathName, m_strTagName );
    else {
        m_strPathName[0] = TEXT('/');
        StringFn->NCopy( m_strPathName + 1, m_strTagName, XML_URI_SIZE - 2 );
    }

        // Attributes
    Bool bContinue = _Parse_AttributeList( &iToken, pOverflowChar, pfCallback, pUserData );
    if ( !bContinue )
        return false;

    // Leaf case
    if ( iToken == XMLTOKEN_TAGEND_LEAF ) {
        _MakeLeaf();

        Assert( *pOverflowToken == XMLTOKEN_UNDEFINED );
        Assert( *pOverflowChar == NULLBYTE );
        return true;
    }

    // Node case
    if ( iToken != XMLTOKEN_TAGEND )
        return false;
    _MakeNode();

    // Recurse
    bContinue = _Parse_NodeContent( pOverflowToken, pOverflowChar, pfCallback, pUserData );
    if ( !bContinue )
        return false;
    iToken = *pOverflowToken;
    *pOverflowToken = XMLTOKEN_UNDEFINED;

    // Parse close tag
    if ( iToken != XMLTOKEN_TAGSTART_CLOSENODE )
        return false;

    iToken = _Parse_NextToken( strTokenData, pOverflowChar, pfCallback, pUserData );
    if ( iToken != XMLTOKEN_NAME )
        return false;

    if ( _Compare_Strings(m_strTagName,strTokenData,NULL) != 0 )
        return false;

    iToken = _Parse_NextToken( strTokenData, pOverflowChar, pfCallback, pUserData );
    if ( iToken != XMLTOKEN_TAGEND )
        return false;

    // Done
    Assert( *pOverflowToken == XMLTOKEN_UNDEFINED );
    Assert( *pOverflowChar == NULLBYTE );
    return true;
}

/////////////////////////////////////////////////////////////////////////////////
// XMLComment implementation
XMLComment::XMLComment( const GChar * strComment ):
    XMLNode( XML_COMMENT, TEXT("_xml_comment") )
{
    _MakeLeaf();

    m_strComment.UseMemoryContext( ResourceFn->GetMemoryContext(), TEXT("XML") );
    m_strComment.Create();
    if ( strComment != NULL ) {
        while( *strComment != NULLBYTE )
            m_strComment.Push( *strComment++ );
    }
    m_strComment.Push( NULLBYTE );
}
XMLComment::~XMLComment()
{
    m_strComment.Destroy();
}

Void XMLComment::SetComment( const GChar * strComment )
{
    m_strComment.Clear();
    if ( strComment != NULL ) {
        while( *strComment != NULLBYTE )
            m_strComment.Push( *strComment++ );
    }
    m_strComment.Push( NULLBYTE );
}

Void XMLComment::AppendComment( const GChar * strComment )
{
    while( *strComment != NULLBYTE )
        m_strComment.Push( *strComment++ );
}
Void XMLComment::InsertComment( const GChar * strComment, UInt iIndex )
{
    Assert( iIndex < m_strComment.Count() );
    UInt iLength = StringFn->Length( strComment );
    m_strComment.Insert( iIndex, strComment, iLength );
}
Void XMLComment::ReplaceComment( const GChar * strComment, UInt iIndex, UInt iLength )
{
    Assert( (iIndex + iLength) < m_strComment.Count() );
    UInt iNewLength = StringFn->Length( strComment );
    m_strComment.Remove( iIndex, NULL, iLength );
    m_strComment.Insert( iIndex, strComment, iNewLength );
}
Void XMLComment::DeleteComment( UInt iIndex, UInt iLength )
{
    Assert( (iIndex + iLength) < m_strComment.Count() );
    m_strComment.Remove( iIndex, NULL, iLength );
}

/////////////////////////////////////////////////////////////////////////////////

XMLNode * XMLComment::_Clone( Bool /*bRecursive*/ ) const
{
    // Create clone
    ResourceFn->SelectMemory( TEXT("XML") );
    XMLComment * pClone = New XMLComment( (const GChar *)m_strComment );
    ResourceFn->UnSelectMemory();

    // Done
    return pClone;
}

Bool XMLComment::_Render( _XMLWriteCallback pfCallback, Void * pUserData ) const
{
    Bool bContinue;
    GChar strIndent[256];
    _GetIndentString( strIndent );

    // Indentation
    bContinue = pfCallback( strIndent, pUserData );
    if ( !bContinue )
        return false;

    // Start comment tag
    bContinue = pfCallback( TEXT("<!-- "), pUserData );
    if ( !bContinue )
        return false;

    // Comment text
    bContinue = pfCallback( (const GChar *)m_strComment, pUserData );
    if ( !bContinue )
        return false;
    
    // Indentation
    if ( m_strComment.Count() > 1 ) {
        bContinue = pfCallback( strIndent, pUserData );
        if ( !bContinue )
            return false;
    }

    // End comment tag
    bContinue = pfCallback( TEXT("-->"), pUserData );
    if ( !bContinue )
        return false;

    return true;
}

Bool XMLComment::_Parse( XMLToken * /*pOverflowToken*/, GChar * /*pOverflowChar*/, _XMLReadCallback /*pfCallback*/, Void * /*pUserData*/ )
{
    // nothing to do
    return true;
}

/////////////////////////////////////////////////////////////////////////////////
// XMLText implementation
XMLText::XMLText( const GChar * strText ):
    XMLNode( XML_TEXT, TEXT("_xml_text") )
{
    _MakeLeaf();

    m_strText.UseMemoryContext( ResourceFn->GetMemoryContext(), TEXT("XML") );
    m_strText.Create();
    if ( strText != NULL ) {
        while( *strText != NULLBYTE )
            m_strText.Push( *strText++ );
    }
    m_strText.Push( NULLBYTE );
}
XMLText::~XMLText()
{
    m_strText.Destroy();
}

Void XMLText::SetText( const GChar * strText )
{
    m_strText.Clear();
    if ( strText != NULL ) {
        while( *strText != NULLBYTE )
            m_strText.Push( *strText++ );
    }
    m_strText.Push( NULLBYTE );
}

Void XMLText::AppendText( const GChar * strText )
{
    while( *strText != NULLBYTE )
        m_strText.Push( *strText++ );
}
Void XMLText::InsertText( const GChar * strText, UInt iIndex )
{
    Assert( iIndex < m_strText.Count() );
    UInt iLength = StringFn->Length( strText );
    m_strText.Insert( iIndex, strText, iLength );
}
Void XMLText::ReplaceText( const GChar * strText, UInt iIndex, UInt iLength )
{
    Assert( (iIndex + iLength) < m_strText.Count() );
    UInt iNewLength = StringFn->Length( strText );
    m_strText.Remove( iIndex, NULL, iLength );
    m_strText.Insert( iIndex, strText, iNewLength );
}
Void XMLText::DeleteText( UInt iIndex, UInt iLength )
{
    Assert( (iIndex + iLength) < m_strText.Count() );
    m_strText.Remove( iIndex, NULL, iLength );
}

/////////////////////////////////////////////////////////////////////////////////

XMLNode * XMLText::_Clone( Bool /*bRecursive*/ ) const
{
    // Create clone
    ResourceFn->SelectMemory( TEXT("XML") );
    XMLText * pClone = New XMLText( (const GChar *)m_strText );
    ResourceFn->UnSelectMemory();

    // Done
    return pClone;
}

Bool XMLText::_Render( _XMLWriteCallback pfCallback, Void * pUserData ) const
{
    Bool bContinue;
    GChar strIndent[256];
    _GetIndentString( strIndent );

    // Indentation
    bContinue = pfCallback( strIndent, pUserData );
    if ( !bContinue )
        return false;

    // Text
    bContinue = pfCallback( (const GChar *)m_strText, pUserData );
    if ( !bContinue )
        return false;
    
    return true;
}

Bool XMLText::_Parse( XMLToken * /*pOverflowToken*/, GChar * /*pOverflowChar*/, _XMLReadCallback /*pfCallback*/, Void * /*pUserData*/ )
{
    // nothing to do
    return true;
}

/////////////////////////////////////////////////////////////////////////////////
// XMLUnparsedText implementation

/////////////////////////////////////////////////////////////////////////////////
// XMLDocument implementation
XMLDocument::XMLDocument( const GChar * strTagName, const GChar * strVersion, const GChar * strEncoding ):
    XMLNode( XML_DOCUMENT, strTagName )
{
    _MakeNode();

    // Initialize parent document propagation
    m_pParentDocument = this;

    // Version & Encoding
    StringFn->NCopy( m_strVersion, strVersion, 63 );
    StringFn->NCopy( m_strEncoding, strEncoding, 63 );
}
XMLDocument::~XMLDocument()
{
    // nothing to do
}

XMLDocument * XMLDocument::CreateDocument( const GChar * strTagName, const GChar * strVersion, const GChar * strEncoding )
{
    ResourceFn->SelectMemory( TEXT("XML") );
    XMLDocument * pDocument = New XMLDocument( strTagName, strVersion, strEncoding );
    ResourceFn->UnSelectMemory();

    return pDocument;
}
XMLDocument * XMLDocument::CreateDocument( const GChar * strFile )
{
    ResourceFn->SelectMemory( TEXT("XML") );
    XMLDocument * pDocument = New XMLDocument( TEXT("_xmlnew"), XML_VERSION );
    ResourceFn->UnSelectMemory();

    Bool bValid = pDocument->Parse( strFile );
    Assert( bValid );

    return pDocument;
}
XMLDocument * XMLDocument::CreateDocumentXML( const GChar * strXML )
{
    ResourceFn->SelectMemory( TEXT("XML") );
    XMLDocument * pDocument = New XMLDocument( TEXT("_xmlnew"), XML_VERSION );
    ResourceFn->UnSelectMemory();

    Bool bValid = pDocument->ParseXML( strXML );
    Assert( bValid );

    return pDocument;
}

Void XMLDocument::DestroyDocument( XMLDocument * pDocument )
{
    ResourceFn->SelectMemory( TEXT("XML") );
    Delete( pDocument );
    ResourceFn->UnSelectMemory();
}

XMLNode * XMLDocument::CreateNode( const GChar * strTagName, Bool bLeaf )
{
    ResourceFn->SelectMemory( TEXT("XML") );
    XMLNode * pNode = New XMLNode( XML_NODE, strTagName );
    ResourceFn->UnSelectMemory();

    if ( bLeaf )
        pNode->_MakeLeaf();
    else
        pNode->_MakeNode();

    return pNode;
}
XMLComment * XMLDocument::CreateComment( const GChar * strComment )
{
    ResourceFn->SelectMemory( TEXT("XML") );
    XMLComment * pComment = New XMLComment( strComment );
    ResourceFn->UnSelectMemory();

    return pComment;
}
XMLText * XMLDocument::CreateText( const GChar * strText )
{
    ResourceFn->SelectMemory( TEXT("XML") );
    XMLText * pText = New XMLText( strText );
    ResourceFn->UnSelectMemory();

    return pText;
}

XMLNode * XMLDocument::CreateNode( const GChar * strFile )
{
    ResourceFn->SelectMemory( TEXT("XML") );
    XMLNode * pNode = New XMLNode( XML_NODE, TEXT("_xmlnew") );
    ResourceFn->UnSelectMemory();

    Bool bValid = pNode->Parse( strFile );
    Assert( bValid );

    return pNode;
}
XMLNode * XMLDocument::CreateNodeXML( const GChar * strXML )
{
    ResourceFn->SelectMemory( TEXT("XML") );
    XMLNode * pNode = New XMLNode( XML_NODE, TEXT("_xmlnew") );
    ResourceFn->UnSelectMemory();

    Bool bValid = pNode->ParseXML( strXML );
    Assert( bValid );

    return pNode;
}

Void XMLDocument::DestroyNode( XMLNode * pNode )
{
    Assert( pNode->IsRoot() );

    ResourceFn->SelectMemory( TEXT("XML") );
    Delete( pNode );
    ResourceFn->UnSelectMemory();
}

/////////////////////////////////////////////////////////////////////////////////

XMLNode * XMLDocument::_Clone( Bool bRecursive ) const
{
    // Create clone
    ResourceFn->SelectMemory( TEXT("XML") );
    XMLDocument * pClone = New XMLDocument( m_strTagName, m_strVersion, m_strEncoding );
    ResourceFn->UnSelectMemory();

    // Copy attributes
    _AttributeMap::Iterator itAttribute = m_mapAttributes.Begin();
    while( !(itAttribute.IsNull()) ) {
        pClone->CreateAttribute( itAttribute.GetKey(), itAttribute.GetItem()->GetValue() );
        ++itAttribute;
    }
    pClone->m_strIdentifierAttribute = m_strIdentifierAttribute;

    // Recurse
    if ( bRecursive ) {
        for( UInt i = 0; i < m_arrChildren.Count(); ++i ) {
            XMLNode * pChildClone = m_arrChildren[i]->Clone( true );
            pClone->AppendChild( pChildClone );
        }
    }

    // Done
    return pClone;
}

Bool XMLDocument::_Render( _XMLWriteCallback pfCallback, Void * pUserData ) const
{
    Assert( m_pParentDocument == this );
    Assert( m_pParent == NULL );
    Assert( m_iDepth == 0 );

    // Document header
    GChar strHeader[256];
    StringFn->Format( strHeader, TEXT("<?xml version=\"%s\" encoding=\"%s\"?>"), m_strVersion, m_strEncoding );
    Bool bContinue = pfCallback( strHeader, pUserData );
    if ( !bContinue )
        return false;

    // XML tree
    return XMLNode::_Render( pfCallback, pUserData );
}

Bool XMLDocument::_Parse_Header( XMLToken * pOverflowToken, GChar * pOverflowChar, _XMLReadCallback pfCallback, Void * pUserData )
{
    GChar strTokenData[XML_NAME_SIZE];

    // Header start
    XMLToken iToken;
    if ( *pOverflowToken == XMLTOKEN_UNDEFINED )
        iToken = _Parse_NextToken( strTokenData, pOverflowChar, pfCallback, pUserData );
    else {
        iToken = *pOverflowToken;
        *pOverflowToken = XMLTOKEN_UNDEFINED;
    }
    if ( iToken == XMLTOKEN_SEPARATOR )
        iToken = _Parse_NextToken( strTokenData, pOverflowChar, pfCallback, pUserData );
    if ( iToken != XMLTOKEN_HEADER_START )
        return false;

    // Attribute list
    Bool bContinue = _Parse_AttributeList( &iToken, pOverflowChar, pfCallback, pUserData );
    if ( !bContinue )
        return false;

    // Header end
    if ( iToken != XMLTOKEN_HEADER_END )
        return false;

    // Extract version & encoding from attributes
    XMLAttribute * pAttribute = GetAttribute( TEXT("version") );
    if ( pAttribute == NULL )
        return false;
    StringFn->NCopy( m_strVersion, pAttribute->GetValue(), 63 );
    DestroyAttribute( TEXT("version") );

    pAttribute = GetAttribute( TEXT("encoding") );
    if ( pAttribute == NULL )
        return false;
    StringFn->NCopy( m_strEncoding, pAttribute->GetValue(), 63 );
    DestroyAttribute( TEXT("encoding") );

    // Done
    Assert( *pOverflowToken == XMLTOKEN_UNDEFINED );
    Assert( *pOverflowChar == NULLBYTE );
    return true;
}

Bool XMLDocument::_Parse( XMLToken * pOverflowToken, GChar * pOverflowChar, _XMLReadCallback pfCallback, Void * pUserData )
{
    // Cleanup
    m_iDepth = 0;

    m_strTagName[0] = NULLBYTE;
    m_strPathName[0] = NULLBYTE;

    m_mapAttributes.Clear();
    m_strIdentifierAttribute = NULL;

    m_pParentDocument = this;
    m_pParent = NULL;
    m_iChildIndex = INVALID_OFFSET;

    m_pPrevSibbling = NULL;
    m_pNextSibbling = NULL;

    ResourceFn->SelectMemory( TEXT("XML") );
    for( UInt i = 0; i < m_arrChildren.Count(); ++i )
        Delete( m_arrChildren[i] );
    ResourceFn->UnSelectMemory();
    m_arrChildren.Clear();
    m_mapChildrenID.Clear();

    m_strVersion[0] = NULLBYTE;
    m_strEncoding[0] = NULLBYTE;

    // Document header
    Bool bContinue = _Parse_Header( pOverflowToken, pOverflowChar, pfCallback, pUserData );
    if ( !bContinue )
        return false;

    // XML tree
    return XMLNode::_Parse( pOverflowToken, pOverflowChar, pfCallback, pUserData );
}


