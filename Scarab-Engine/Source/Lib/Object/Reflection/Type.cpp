/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Object/Reflection/Type.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Generic type descriptor.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Type.h"

#include "UnionStruct.h"
#include "Class.h"

/////////////////////////////////////////////////////////////////////////////////
// BEGIN : _Reflection implementation
namespace _Reflection {

const TypeCreationParams * _Builder_Type( const GChar * strName, SpecifierType iType, SpecifierTypeMod iTypeMod,
                                          SpecifierScope iScope, SpecifierAccess iAccess,
                                          UInt iBytes, CastHandler pfCastFunc, NewHandler pfNewFunc,
                                          TypeDescriptor * pNestedParent )
{
    static TypeCreationParams s_typeParams;
    s_typeParams.strName = strName;
    s_typeParams.iType = iType;
    s_typeParams.iTypeMod = iTypeMod;
    s_typeParams.iScope = iScope;
    s_typeParams.iAccess = iAccess;
    s_typeParams.iByteSize = iBytes;
    s_typeParams.pfCastFunc = pfCastFunc;
    s_typeParams.pfNewFunc = pfNewFunc;
    s_typeParams.pNestedParent = pNestedParent;
    return &s_typeParams;
}

/////////////////////////////////////////////////////////////////////////////////
// _Reflection::TypeDescriptor implementation
LookupTable<TypeDescriptor,REFLECTION_LOOKUP_SIZE> TypeDescriptor::sm_htTypes =
    LookupTable<TypeDescriptor,REFLECTION_LOOKUP_SIZE>();

TypeDescriptor::TypeDescriptor( const TypeCreationParams * pParams )
{
    GChar * strTmp = StringFn->NCopy( m_strRelativeName, pParams->strName, REFLECTION_STRINGSIZE - 1 );
    m_iRelativeNameLength = ( strTmp - m_strRelativeName );
    if ( pParams->pNestedParent != NULL ) {
        strTmp = StringFn->NCopy( m_strAbsoluteName, pParams->pNestedParent->m_strAbsoluteName, REFLECTION_STRINGSIZE - 1 );
        strTmp = StringFn->NCopy( strTmp, TEXT("::"), REFLECTION_STRINGSIZE - 1 - ( strTmp - m_strAbsoluteName ) );
        strTmp = StringFn->NCopy( strTmp, pParams->strName, REFLECTION_STRINGSIZE - 1 - ( strTmp - m_strAbsoluteName ) );
    } else
        strTmp = StringFn->NCopy( m_strAbsoluteName, pParams->strName, REFLECTION_STRINGSIZE - 1 );
    m_iAbsoluteNameLength = ( strTmp - m_strAbsoluteName );

    m_specType = pParams->iType;
    m_specTypeMod = pParams->iTypeMod;
    m_specScope = pParams->iScope;
    m_specAccess = pParams->iAccess;
    m_iBytes = pParams->iByteSize;

    m_pfCast = pParams->pfCastFunc;
    m_pfNew = pParams->pfNewFunc;

    m_pNestedParent = pParams->pNestedParent;
    if (m_pNestedParent != NULL) {
        if ( m_pNestedParent->m_specType == SPECIFIER_TYPE_UNION )
            ((UnionDescriptor*)m_pNestedParent)->_AddNestedChild(this);
        else if ( m_pNestedParent->m_specType == SPECIFIER_TYPE_STRUCT )
            ((StructDescriptor*)m_pNestedParent)->_AddNestedChild(this);
        else if ( m_pNestedParent->m_specType == SPECIFIER_TYPE_CLASS )
            ((ClassDescriptor*)m_pNestedParent)->_AddNestedChild(this);
    }

    sm_htTypes.Insert(this);
}
TypeDescriptor::~TypeDescriptor()
{
    if (m_pNestedParent != NULL) {
        if ( m_pNestedParent->m_specType == SPECIFIER_TYPE_UNION )
            ((UnionDescriptor*)m_pNestedParent)->_RemoveNestedChild(this);
        else if ( m_pNestedParent->m_specType == SPECIFIER_TYPE_STRUCT )
            ((StructDescriptor*)m_pNestedParent)->_RemoveNestedChild(this);
        else if ( m_pNestedParent->m_specType == SPECIFIER_TYPE_CLASS )
            ((ClassDescriptor*)m_pNestedParent)->_RemoveNestedChild(this);
    }

    sm_htTypes.Remove(m_strAbsoluteName);
}

const TypeDescriptor * TypeDescriptor::NestedParent(const GChar * strName) const
{
    if (strName == NULL)
        return m_pNestedParent;
    if (m_pNestedParent == NULL)
        return NULL;
    if ( StringFn->NCmp( m_pNestedParent->m_strRelativeName, strName, REFLECTION_STRINGSIZE - 1 ) == 0 )
        return m_pNestedParent;
    return m_pNestedParent->NestedParent(strName);
}

Bool TypeDescriptor::IsEqual(const TypeDescriptor * pType) const
{
    if ( this == pType )
        return true;
    if ( StringFn->NCmp( m_strRelativeName, pType->m_strRelativeName, REFLECTION_STRINGSIZE - 1 ) != 0 )
        return false;
    if ( m_specType != pType->m_specType )
        return false;
    if ( m_specScope != pType->m_specScope )
        return false;
    if ( m_pNestedParent == NULL )
        return (pType->m_pNestedParent == NULL);
    if (pType->m_pNestedParent == NULL)
        return false;
    return ( m_pNestedParent->IsEqual(pType->m_pNestedParent) );
}

/////////////////////////////////////////////////////////////////////////////////
// _Reflection::ModTypeDescriptor implementation
ModTypeDescriptor::ModTypeDescriptor()
{
    m_iMember = SPECIFIER_MEMBER_VALUE;
    m_iMemberMod = SPECIFIER_MEMBERMOD_NONE;
    m_iIndirectionLevel = 0;
    m_pType = NULL;
}
ModTypeDescriptor::ModTypeDescriptor( const GChar * strExpr )
{
    _ParseTypeExpr(strExpr);
}
ModTypeDescriptor::ModTypeDescriptor( const ModTypeDescriptor & rhs )
{
    m_iMember = rhs.m_iMember;
    m_iMemberMod = rhs.m_iMemberMod;
    m_iIndirectionLevel = rhs.m_iIndirectionLevel;
    m_pType = rhs.m_pType;
}
ModTypeDescriptor::~ModTypeDescriptor()
{
    // nothing to do
}

Bool ModTypeDescriptor::IsEqual(const ModTypeDescriptor & rModType) const
{
    if (m_iMember != rModType.m_iMember)
        return false;
    if (m_iMemberMod != rModType.m_iMemberMod)
        return false;
    if (m_iIndirectionLevel != rModType.m_iIndirectionLevel)
        return false;
    return m_pType->IsEqual(rModType.m_pType);
}

/////////////////////////////////////////////////////////////////////////////////

Void ModTypeDescriptor::_ParseTypeExpr( const GChar * strExpr )
{
    m_iMember = SPECIFIER_MEMBER_VALUE;
    m_iMemberMod = SPECIFIER_MEMBERMOD_NONE;
    m_iIndirectionLevel = 0;
    m_pType = NULL;

    // Parsing ...
    // TypeExpr = <transient>? <const/volatile/mutable>? <typename> ('*')* ('&')? ( '['']' )?
    // FuncTypeExpr = <transient>? TypeExpr ( '(' '*' <funcptrname> ')' <const>? '(' (TypeExpr ',')* ')' )?
    GChar strBuffer[REFLECTION_STRINGSIZE];
    GChar * pBuff;

    // Eat leading spaces
    while(*strExpr == ' ') ++strExpr;

    // State machine
    UInt iState = 0;
    while(true) {
        switch(iState) {
            case 0:
                pBuff = strBuffer;
                while( *strExpr != ' ' && *strExpr != '*' && *strExpr != '&' && *strExpr != '[' ) {
                    *pBuff = *strExpr;
                    ++pBuff; ++strExpr;
                }
                *pBuff = NULLBYTE;
                while(*strExpr == ' ') ++strExpr;

                if ( StringFn->Cmp(strBuffer, TEXT("transient")) == 0 ) {
                    m_iMemberMod = SPECIFIER_MEMBERMOD_TRANSIENT;
                    iState = 1;
                } else if ( StringFn->Cmp(strBuffer, TEXT("const")) == 0 ) {
                    m_iMemberMod = SPECIFIER_MEMBERMOD_CONST;
                    iState = 2;
                } else if ( StringFn->Cmp(strBuffer, TEXT("volatile")) == 0 ) {
                    m_iMemberMod = SPECIFIER_MEMBERMOD_VOLATILE;
                    iState = 2;
                } else if ( StringFn->Cmp(strBuffer, TEXT("mutable")) == 0 ) {
                    m_iMemberMod = SPECIFIER_MEMBERMOD_MUTABLE;
                    iState = 2;
                } else {
                    m_pType = TypeDescriptor::GetByName(strBuffer);
                    m_iMember = SPECIFIER_MEMBER_VALUE;
                    iState = 3;
                }
                break;
            case 1:
                pBuff = strBuffer;
                while( *strExpr != ' ' && *strExpr != '*' && *strExpr != '&' && *strExpr != '[' ) {
                    *pBuff = *strExpr;
                    ++pBuff; ++strExpr;
                }
                *pBuff = NULLBYTE;
                while(*strExpr == ' ') ++strExpr;

                if ( StringFn->Cmp(strBuffer, TEXT("const")) == 0 ) {
                    m_iMemberMod |= SPECIFIER_MEMBERMOD_CONST;
                    iState = 2;
                } else if ( StringFn->Cmp(strBuffer, TEXT("volatile")) == 0 ) {
                    m_iMemberMod |= SPECIFIER_MEMBERMOD_VOLATILE;
                    iState = 2;
                } else if ( StringFn->Cmp(strBuffer, TEXT("mutable")) == 0 ) {
                    m_iMemberMod |= SPECIFIER_MEMBERMOD_MUTABLE;
                    iState = 2;
                } else {
                    m_pType = TypeDescriptor::GetByName(strBuffer);
                    m_iMember = SPECIFIER_MEMBER_VALUE;
                    iState = 3;
                }
                break;
            case 2:
                pBuff = strBuffer;
                while( *strExpr != ' ' && *strExpr != '*' && *strExpr != '&' && *strExpr != '[' ) {
                    *pBuff = *strExpr;
                    ++pBuff; ++strExpr;
                }
                *pBuff = NULLBYTE;
                while(*strExpr == ' ') ++strExpr;

                m_pType = TypeDescriptor::GetByName(strBuffer);
                m_iMember = SPECIFIER_MEMBER_VALUE;
                iState = 3;
                break;
            case 3:
                if ( *strExpr == '*' ) {
                    ++strExpr;
                    while(*strExpr == ' ') ++strExpr;

                    m_iMember = SPECIFIER_MEMBER_POINTER;
                    ++m_iIndirectionLevel;
                    iState = 3;
                } else if ( *strExpr == '&' ) {
                    ++strExpr;
                    while(*strExpr == ' ') ++strExpr;

                    m_iMember = SPECIFIER_MEMBER_REFERENCE;
                    ++m_iIndirectionLevel;
                    iState = 4;
                } else if ( *strExpr == '[' ) {
                    ++strExpr;
                    while(*strExpr == ' ') ++strExpr;

                    if ( *strExpr != ']' )
                        return;
                    ++strExpr;

                    m_iMember = SPECIFIER_MEMBER_ARRAY;
                    ++m_iIndirectionLevel;
                    iState = 0xff;
                } else
                    iState = 0xff;
                break;
            case 4:
                if ( *strExpr == '[' ) {
                    ++strExpr;
                    while(*strExpr == ' ') ++strExpr;

                    if ( *strExpr != ']' )
                        return;
                    ++strExpr;

                    m_iMember = SPECIFIER_MEMBER_ARRAY;
                    ++m_iIndirectionLevel;
                    iState = 0xff;
                } else {
                    iState = 0xff;
                }
                break;
            case 0xff:
                return;
                break;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////
// END : _Reflection implementation
};
