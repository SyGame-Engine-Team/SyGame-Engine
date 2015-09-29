/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Object/Reflection/Member.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Member descriptor.
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
#include "Member.h"

#include "UnionStruct.h"
#include "Class.h"

/////////////////////////////////////////////////////////////////////////////////
// BEGIN : _Reflection implementation
namespace _Reflection {

const MemberCreationParams * _Builder_Member( const GChar * strName,
                                              SpecifierScope iScope, SpecifierAccess iAccess, SpecifierStorage iStorage,
                                              UInt iBytes, GetHandler pfGetFunc, SetHandler pfSetFunc,
                                              TypeDescriptor * pNestedParent,
                                              const GChar * strTypeExpr )
{
    static MemberCreationParams s_memberParams;
    s_memberParams.strName = strName;
    s_memberParams.iScope = iScope;
    s_memberParams.iAccess = iAccess;
    s_memberParams.iStorage = iStorage;
    s_memberParams.iByteSize = iBytes;
    s_memberParams.pfGetFunc = pfGetFunc;
    s_memberParams.pfSetFunc = pfSetFunc;
    s_memberParams.pNestedParent = pNestedParent;
    s_memberParams.strTypeExpr = strTypeExpr;
    return &s_memberParams;
}

/////////////////////////////////////////////////////////////////////////////////
// _Reflection::MemberDescriptor implementation
MemberDescriptor::MemberDescriptor( const MemberCreationParams * pParams ):
    m_ModType(pParams->strTypeExpr)
{
    GChar * strTmp = StringFn->NCopy( m_strRelativeName, pParams->strName, REFLECTION_STRINGSIZE - 1 );
    m_iRelativeNameLength = ( strTmp - m_strRelativeName );
    if ( pParams->pNestedParent != NULL ) {
        strTmp = StringFn->NCopy( m_strAbsoluteName, pParams->pNestedParent->FullName(), REFLECTION_STRINGSIZE - 1 );
        strTmp = StringFn->NCopy( strTmp, TEXT("::"), REFLECTION_STRINGSIZE - 1 - ( strTmp - m_strAbsoluteName ) );
        strTmp = StringFn->NCopy( strTmp, pParams->strName, REFLECTION_STRINGSIZE - 1 - ( strTmp - m_strAbsoluteName ) );
    } else
        strTmp = StringFn->NCopy( m_strAbsoluteName, pParams->strName, REFLECTION_STRINGSIZE - 1 );
    m_iAbsoluteNameLength = ( strTmp - m_strAbsoluteName );

    m_specScope = pParams->iScope;
    m_specAccess = pParams->iAccess;
    m_specStorage = pParams->iStorage;
    m_iBytes = pParams->iByteSize;

    m_pfGet = pParams->pfGetFunc;
    m_pfSet = pParams->pfSetFunc;

    m_pNestedParent = pParams->pNestedParent;
    if (m_pNestedParent != NULL) {
        if ( m_pNestedParent->SpecType() == SPECIFIER_TYPE_UNION )
            ((UnionDescriptor*)m_pNestedParent)->_AddMember(this);
        else if ( m_pNestedParent->SpecType() == SPECIFIER_TYPE_STRUCT )
            ((StructDescriptor*)m_pNestedParent)->_AddMember(this);
        else if ( m_pNestedParent->SpecType() == SPECIFIER_TYPE_CLASS )
            ((ClassDescriptor*)m_pNestedParent)->_AddMember(this);
    }
}
MemberDescriptor::~MemberDescriptor()
{
    if (m_pNestedParent != NULL) {
        if ( m_pNestedParent->SpecType() == SPECIFIER_TYPE_UNION )
            ((UnionDescriptor*)m_pNestedParent)->_RemoveMember(this);
        else if ( m_pNestedParent->SpecType() == SPECIFIER_TYPE_STRUCT )
            ((StructDescriptor*)m_pNestedParent)->_RemoveMember(this);
        else if ( m_pNestedParent->SpecType() == SPECIFIER_TYPE_CLASS )
            ((ClassDescriptor*)m_pNestedParent)->_RemoveMember(this);
    }
}

const TypeDescriptor * MemberDescriptor::NestedParent(const GChar * strName) const
{
    if (strName == NULL)
        return m_pNestedParent;
    if (m_pNestedParent == NULL)
        return NULL;
    if ( StringFn->NCmp( m_pNestedParent->Name(), strName, REFLECTION_STRINGSIZE - 1 ) == 0 )
        return m_pNestedParent;
    return m_pNestedParent->NestedParent(strName);
}

Bool MemberDescriptor::IsEqual(const MemberDescriptor * pMember) const
{
    if ( this == pMember )
        return true;
    if ( StringFn->NCmp( m_strRelativeName, pMember->m_strRelativeName, REFLECTION_STRINGSIZE - 1 ) != 0 )
        return false;
    if ( !(m_ModType.IsEqual(pMember->m_ModType)) )
        return false;
    if ( m_pNestedParent == NULL )
        return (pMember->m_pNestedParent == NULL);
    if (pMember->m_pNestedParent == NULL)
        return false;
    return ( m_pNestedParent->IsEqual(pMember->m_pNestedParent) );
}

/////////////////////////////////////////////////////////////////////////////////
// END : _Reflection implementation
};
