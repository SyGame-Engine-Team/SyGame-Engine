/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Resource/XML/XML.h
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
// Header prelude
#ifndef SCARAB_ENGINE_RESOURCE_XML_XML_H
#define SCARAB_ENGINE_RESOURCE_XML_XML_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../ThirdParty/System/System.h"

#include "../../../Lib/String/PatternMatching.h"
#include "../../../Lib/Datastruct/Array/Array.h"
#include "../../../Lib/Datastruct/Table/TreeMap.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// XML specifications
#define XML_VERSION          TEXT("1.0")

#define XML_ENCODING_ASCII   TEXT("us-ascii")
#define XML_ENCODING_UTF8    TEXT("utf-8")
#define XML_ENCODING_UTF16   TEXT("utf-16")
#define XML_ENCODING_ISO     TEXT("iso-8859-1")

#define XML_ENCODING_DEFAULT XML_ENCODING_UTF8

#define XML_NAME_SIZE 256
#define XML_URI_SIZE  1024

enum XMLNodeType {
    XML_NODE = 0,
    XML_DOCUMENT,

    XML_COMMENT,
    XML_TEXT,
    //XML_TEXT_UNPARSED,

    XML_COUNT
};

// Lexical analysis
enum XMLToken {
    XMLTOKEN_UNDEFINED = 0,

    XMLTOKEN_SEPARATOR, // [ |\n|\t]*
    XMLTOKEN_NAME,      // [A-Z|a-z|_][A-Z|a-z|0-9|_]*
    XMLTOKEN_TEXT,      // *

    XMLTOKEN_HEADER_START, // <?xml
    XMLTOKEN_HEADER_END,   // ?>

    XMLTOKEN_COMMENT_START, // <!--
    XMLTOKEN_COMMENT_END,   // -->

    XMLTOKEN_TAGSTART,           // <
    XMLTOKEN_TAGSTART_CLOSENODE, // </
    XMLTOKEN_TAGEND,             // >
    XMLTOKEN_TAGEND_LEAF,        // />

    XMLTOKEN_ATTRIB_AFFECT, // =
    XMLTOKEN_ATTRIB_STRING, // "

    XMLTOKEN_COUNT
};

// XML Grammar
enum XMLExpression {
    XMLEXPRESSION_UNDEFINED = 0,

    XMLEXPRESSION_DOCUMENT,  // XMLEXPRESSION_HEADER XMLEXPRESSION_NODE

    XMLEXPRESSION_HEADER,    // <?xml XMLEXPRESSION_ATTRIB_LIST ?>

    XMLEXPRESSION_NODE_CONTENT, // XMLEXPRESSION_NODE XMLEXPRESSION_NODE_CONTENT
                                // | XMLEXPRESSION_NODE
                                // | XMLEXPRESSION_LEAF XMLEXPRESSION_NODE_CONTENT
                                // | XMLEXPRESSION_LEAF
                                // | XMLEXPRESSION_COMMENT XMLEXPRESSION_NODE_CONTENT
                                // | XMLEXPRESSION_COMMENT
                                // | XMLTOKEN_TEXT
                                // | <NOTHING>

    XMLEXPRESSION_NODE, // < XMLTOKEN_NAME XMLEXPRESSION_ATTRIB_LIST >
                        //     XMLEXPRESSION_NODE_CONTENT
                        // </ XMLTOKEN_NAME >

    XMLEXPRESSION_LEAF, // < XMLTOKEN_NAME XMLEXPRESSION_ATTRIB_LIST />

    XMLEXPRESSION_COMMENT, // <!-- XMLTOKEN_TEXT -->

    XMLEXPRESSION_ATTRIB_LIST, // XMLEXPRESSION_ATTRIB XMLEXPRESSION_ATTRIB_LIST
                               // | XMLEXPRESSION_ATTRIB
                               // | <NOTHING>

    XMLEXPRESSION_ATTRIB, // XMLTOKEN_SEPARATOR XMLTOKEN_NAME = " XMLTOKEN_TEXT "

    XMLEXPRESSION_COUNT
};

// Prototypes
class XMLNode;
class XMLDocument;

/////////////////////////////////////////////////////////////////////////////////
// The XMLAttribute class
class XMLAttribute
{
protected:
    friend class XMLNode;
    XMLAttribute( const GChar * strName, const GChar * strValue = NULL );
public:
    ~XMLAttribute();

    inline Bool IsIdentifier() const;

    inline const GChar * GetName() const;
    inline const GChar * GetValue( UInt * outLength = NULL ) const;
    inline Array<GChar> * EditValue();
    Void SetValue( const GChar * strValue );

private:
    Bool m_bIdentifier;

    GChar m_strName[XML_NAME_SIZE];
    Array<GChar> m_strValue;
};

/////////////////////////////////////////////////////////////////////////////////
// The XMLNode class
class XMLNode
{
protected:
    friend class XMLDocument;
    XMLNode( XMLNodeType iType, const GChar * strTagName );
public:
    virtual ~XMLNode();

    // Type & Depth
    inline XMLNodeType GetType() const;

    inline Bool IsNode() const;
    inline Bool IsLeaf() const;
    inline Bool IsRoot() const;
    inline UInt GetDepth() const;

    // Getters
    inline const GChar * GetTagName() const;
    inline const GChar * GetPathName() const;

    // Attributes
    inline Bool HasAttributes() const;
    inline UInt GetAttributeCount() const;
    inline Bool HasAttribute( const GChar * strName ) const;
    inline XMLAttribute * GetAttribute( const GChar * strName ) const;

    inline Void EnumAttributes() const;
    inline XMLAttribute * EnumNextAttribute() const;

    XMLAttribute * CreateAttribute( const GChar * strName, const GChar * strValue = NULL );
    Void DestroyAttribute( const GChar * strName );

    inline const GChar * GetIdentifierAttribute() const;
    Void SetIdentifierAttribute( const GChar * strName );

    // Linkage
    inline XMLDocument * GetParentDocument() const;
    inline XMLNode * GetParent() const;
    inline UInt GetChildIndex() const;

    inline XMLNode * GetPrevSibbling() const;
    inline XMLNode * GetNextSibbling() const;

    inline Bool HasChildren() const;
    inline UInt GetChildCount() const;
    inline XMLNode * GetChild( UInt iIndex ) const;

    // Tree operations
    XMLNode * GetChildByTag( const GChar * strTagName, UInt iOffset, UInt * outIndex = NULL ) const;
    XMLNode * GetChildNByTag( const GChar * strTagName, UInt iOccurence ) const;
    XMLNode * GetChildByTagPath( const GChar * strTagPath ) const;

    XMLNode * GetChildByAttribute( const GChar * strName, const GChar * strValue, UInt iOffset, UInt * outIndex = NULL ) const;
    XMLNode * GetChildNByAttribute( const GChar * strName, const GChar * strValue, UInt iOccurence ) const;
    XMLNode * GetChildByAttributePath( const GChar * strAttributePath ) const;

    inline XMLNode * GetChildByID( const GChar * strID, UInt * outIndex = NULL ) const;
    XMLNode * GetChildByIDPath( const GChar * strIDPath ) const;

    Void GetChildText( Array<GChar> * outBuffer ) const;
    Void GetChildText( GChar * outBuffer, UInt iSize ) const;

    UInt AppendChild( XMLNode * pNode );
	Void InsertChild( XMLNode * pNode, UInt iIndex );
	XMLNode * ReplaceChild( XMLNode * pNode, UInt iIndex );
	XMLNode * RemoveChild( UInt iIndex );

	Void InsertBefore( XMLNode * pNode );
	Void InsertAfter( XMLNode * pNode );

    // XML methods
	inline XMLNode * Clone( Bool bRecursive = true ) const;

    Bool Render( const GChar * strFile ) const;
    Bool RenderXML( Array<GChar> * outXML ) const;

    Bool Parse( const GChar * strFile );
    Bool ParseXML( const GChar * strXML );

protected:
    // Helpers
    inline static Int _Compare_Strings( const GChar * const & strLeft, const GChar * const & strRight, Void * pUserData );

    Void _MakeLeaf();
    Void _MakeNode();

    Void _rec_SetDepth( UInt iDepth );
    Void _rec_SetParentDocument( XMLDocument * pDocument );

    typedef Bool (*_XMLReadCallback)( GChar * outCh, Void * pUserData );
    typedef Bool (*_XMLWriteCallback)( const GChar * str, Void * pUserData );

    static Bool _ReadCallback_File( GChar * outCh, Void * pUserData );
    static Bool _ReadCallback_XML( GChar * outCh, Void * pUserData );

    static Bool _WriteCallback_File( const GChar * str, Void * pUserData );
    static Bool _WriteCallback_XML( const GChar * str, Void * pUserData );

    // Cloning
    virtual XMLNode * _Clone( Bool bRecursive ) const;

    // Rendering
    Void _GetIndentString( GChar * outIndentString ) const;

    virtual Bool _Render( _XMLWriteCallback pfCallback, Void * pUserData ) const;

    // Parsing
    inline static Bool _IsBlank( GChar ch );
    inline static Bool _IsNameChar( GChar ch );
    static XMLToken _Parse_NextToken( GChar * outTokenData, GChar * pOverflowChar, _XMLReadCallback pfCallback, Void * pUserData );

    Bool _Parse_AttributeList( XMLToken * pOverflowToken, GChar * pOverflowChar, _XMLReadCallback pfCallback, Void * pUserData );
    Bool _Parse_NodeContent( XMLToken * pOverflowToken, GChar * pOverflowChar, _XMLReadCallback pfCallback, Void * pUserData );

    virtual Bool _Parse( XMLToken * pOverflowToken, GChar * pOverflowChar, _XMLReadCallback pfCallback, Void * pUserData );

    // Type & Depth
    XMLNodeType m_iType;
    UInt m_iDepth;

    // Path & Name
    GChar m_strTagName[XML_NAME_SIZE];
    GChar m_strPathName[XML_URI_SIZE];

    // Attributes
    typedef TreeMap<const GChar *,XMLAttribute*> _AttributeMap;
    _AttributeMap m_mapAttributes;

    mutable _AttributeMap::Iterator m_itEnumerate;

    const GChar * m_strIdentifierAttribute;

    // Linkage
    XMLDocument * m_pParentDocument;
    XMLNode * m_pParent;
    UInt m_iChildIndex; // in parent
    
    XMLNode * m_pPrevSibbling;
    XMLNode * m_pNextSibbling;

    Bool m_bIsNode;
    Array<XMLNode*> m_arrChildren;

    typedef TreeMap<const GChar *,XMLNode*> _ChildrenIDMap;
    _ChildrenIDMap m_mapChildrenID;
};

/////////////////////////////////////////////////////////////////////////////////
// The XMLComment class
class XMLComment : public XMLNode
{
protected:
    friend class XMLNode;
    friend class XMLDocument;
    XMLComment( const GChar * strComment = NULL );
public:
    virtual ~XMLComment();

    // Comment access
    inline const GChar * GetComment( UInt * outLength = NULL ) const;
    inline Array<GChar> * EditComment();
    Void SetComment( const GChar * strComment );

    Void AppendComment( const GChar * strComment );
    Void InsertComment( const GChar * strComment, UInt iIndex );
    Void ReplaceComment( const GChar * strComment, UInt iIndex, UInt iLength );
    Void DeleteComment( UInt iIndex, UInt iLength );

protected:
    // Cloning
	virtual XMLNode * _Clone( Bool bRecursive ) const;

    // Rendering
    virtual Bool _Render( _XMLWriteCallback pfCallback, Void * pUserData ) const;

    // Parsing
    virtual Bool _Parse( XMLToken * pOverflowToken, GChar * pOverflowChar, _XMLReadCallback pfCallback, Void * pUserData );

    // Comment
    Array<GChar> m_strComment;
};

/////////////////////////////////////////////////////////////////////////////////
// The XMLText class
class XMLText : public XMLNode
{
protected:
    friend class XMLNode;
    friend class XMLDocument;
    XMLText( const GChar * strText = NULL );
public:
    virtual ~XMLText();

    // Text access
    inline const GChar * GetText( UInt * outLength = NULL ) const;
    inline Array<GChar> * EditText();
    Void SetText( const GChar * strText );

    Void AppendText( const GChar * strText );
    Void InsertText( const GChar * strText, UInt iIndex );
    Void ReplaceText( const GChar * strText, UInt iIndex, UInt iLength );
    Void DeleteText( UInt iIndex, UInt iLength );

protected:
    // Cloning
	virtual XMLNode * _Clone( Bool bRecursive ) const;

    // Rendering
    virtual Bool _Render( _XMLWriteCallback pfCallback, Void * pUserData ) const;

    // Parsing
    virtual Bool _Parse( XMLToken * pOverflowToken, GChar * pOverflowChar, _XMLReadCallback pfCallback, Void * pUserData );

    // Text
    Array<GChar> m_strText;
};

/////////////////////////////////////////////////////////////////////////////////
// The XMLUnparsedText class

/////////////////////////////////////////////////////////////////////////////////
// The XMLDocument class
class XMLDocument : public XMLNode
{
protected:
    XMLDocument( const GChar * strTagName, const GChar * strVersion, const GChar * strEncoding = XML_ENCODING_DEFAULT );
    virtual ~XMLDocument();
public:

    // Document creation
    static XMLDocument * CreateDocument( const GChar * strTagName, const GChar * strVersion, const GChar * strEncoding = XML_ENCODING_DEFAULT );
    static XMLDocument * CreateDocument( const GChar * strFile );
    static XMLDocument * CreateDocumentXML( const GChar * strXML );

    static Void DestroyDocument( XMLDocument * pDocument );

    // Version & Encoding
    inline const GChar * GetVersion() const;
    inline const GChar * GetEncoding() const;

    inline Void SetVersion( const GChar * strVersion );
    inline Void SetEncoding( const GChar * strEncoding );

    // Node management
    static XMLNode * CreateNode( const GChar * strTagName, Bool bLeaf );
    static XMLComment * CreateComment( const GChar * strComment = NULL );
    static XMLText * CreateText( const GChar * strText = NULL );

    static XMLNode * CreateNode( const GChar * strFile );
    static XMLNode * CreateNodeXML( const GChar * strXML );

    static Void DestroyNode( XMLNode * pNode );

    // Includes & Imports
    //Void ResolveIncludes(); // Lookup all include nodes and replace them with their content
                              // This is recursive of course ...

    // Validation & normalization
    //Bool ValidateDTD() const;

    //Bool ValidateSchema( XMLReadCallback pfCallback, Void * pUserData );

    //Bool ValidateSchema( const GChar * strFile ) const;
    //Bool ValidateSchemaXML( const GChar * strXML ) const;

    //Void Normalize();

protected:
    // Cloning
	virtual XMLNode * _Clone( Bool bRecursive ) const;

    // Rendering
    virtual Bool _Render( _XMLWriteCallback pfCallback, Void * pUserData ) const;

    // Parsing
    Bool _Parse_Header( XMLToken * pOverflowToken, GChar * pOverflowChar, _XMLReadCallback pfCallback, Void * pUserData );

    virtual Bool _Parse( XMLToken * pOverflowToken, GChar * pOverflowChar, _XMLReadCallback pfCallback, Void * pUserData );

    // Version & Encoding
    GChar m_strVersion[64];
    GChar m_strEncoding[64];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "XML.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RESOURCE_XML_XML_H

