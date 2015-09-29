/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Object/Reflection/UnionStruct.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : UnionStruct descriptor.
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
#include "UnionStruct.h"

#include "Member.h"
#include "Class.h"

/////////////////////////////////////////////////////////////////////////////////
// BEGIN : _Reflection implementation
namespace _Reflection {

/////////////////////////////////////////////////////////////////////////////////
// _Reflection::UnionDescriptor implementation
UnionDescriptor::UnionDescriptor( const TypeCreationParams * pParams ):
    TypeDescriptor( pParams )
{
    // nothing to do
}
UnionDescriptor::~UnionDescriptor()
{
    // nothing to do
}

const TypeDescriptor * UnionDescriptor::NestedChild(const GChar * strName) const
{
    UInt i;
    for(i = 0; i < m_iNestedChildsCount; ++i) {
        if ( StringFn->NCmp( m_arrNestedChilds[i]->Name(), strName, REFLECTION_STRINGSIZE - 1 ) == 0 )
            return m_arrNestedChilds[i];
    }
    const TypeDescriptor * pNestedChild = NULL;
    for(i = 0; i < m_iNestedChildsCount; ++i) {
        if ( m_arrNestedChilds[i]->SpecType() == SPECIFIER_TYPE_UNION )
            pNestedChild = ( (UnionDescriptor*)(m_arrNestedChilds[i]) )->NestedChild(strName);
        else if ( m_arrNestedChilds[i]->SpecType() == SPECIFIER_TYPE_STRUCT )
            pNestedChild = ( (StructDescriptor*)(m_arrNestedChilds[i]) )->NestedChild(strName);
        else if ( m_arrNestedChilds[i]->SpecType() == SPECIFIER_TYPE_CLASS )
            pNestedChild = ( (ClassDescriptor*)(m_arrNestedChilds[i]) )->NestedChild(strName);
        if ( pNestedChild != NULL )
            return pNestedChild;
    }
    return NULL;
}
const MemberDescriptor * UnionDescriptor::Member(const GChar * strName) const
{
    UInt i;
    for(i = 0; i < m_iMembersCount; ++i) {
        if ( StringFn->NCmp( m_arrMembers[i]->Name(), strName, REFLECTION_STRINGSIZE - 1 ) == 0 )
            return m_arrMembers[i];
    }
    return NULL;
}

/////////////////////////////////////////////////////////////////////////////////

Void UnionDescriptor::_RemoveNestedChild(TypeDescriptor * pNestedChild)
{
    TypeDescriptor ** pCurNested = m_arrNestedChilds;
    TypeDescriptor ** pEnd = m_arrNestedChilds + m_iNestedChildsCount;
    while(pCurNested < pEnd) {
        if ( *pCurNested == pNestedChild )
            break;
        ++pCurNested;
    }
    if ( pCurNested < pEnd ) {
        --pEnd;
        while(pCurNested < pEnd) {
            *pCurNested = *(pCurNested + 1);
            ++pCurNested;
        }
        --m_iNestedChildsCount;
    }
}
Void UnionDescriptor::_RemoveMember(MemberDescriptor * pMember)
{
    MemberDescriptor ** pCurMember = m_arrMembers;
    MemberDescriptor ** pEnd = m_arrMembers + m_iMembersCount;
    while(pCurMember < pEnd) {
        if ( *pCurMember == pMember )
            break;
        ++pCurMember;
    }
    if ( pCurMember < pEnd ) {
        --pEnd;
        while(pCurMember < pEnd) {
            *pCurMember = *(pCurMember + 1);
            ++pCurMember;
        }
        --m_iMembersCount;
    }
}

/////////////////////////////////////////////////////////////////////////////////
// _Reflection::StructDescriptor implementation
StructDescriptor::StructDescriptor( const TypeCreationParams * pParams ):
    TypeDescriptor( pParams )
{
    // nothing to do
}
StructDescriptor::~StructDescriptor()
{
    // nothing to do
}

const TypeDescriptor * StructDescriptor::NestedChild(const GChar * strName) const
{
    UInt i;
    for(i = 0; i < m_iNestedChildsCount; ++i) {
        if ( StringFn->NCmp( m_arrNestedChilds[i]->Name(), strName, REFLECTION_STRINGSIZE - 1 ) == 0 )
            return m_arrNestedChilds[i];
    }
    const TypeDescriptor * pNestedChild = NULL;
    for(i = 0; i < m_iNestedChildsCount; ++i) {
        if ( m_arrNestedChilds[i]->SpecType() == SPECIFIER_TYPE_UNION )
            pNestedChild = ( (UnionDescriptor*)(m_arrNestedChilds[i]) )->NestedChild(strName);
        else if ( m_arrNestedChilds[i]->SpecType() == SPECIFIER_TYPE_STRUCT )
            pNestedChild = ( (StructDescriptor*)(m_arrNestedChilds[i]) )->NestedChild(strName);
        else if ( m_arrNestedChilds[i]->SpecType() == SPECIFIER_TYPE_CLASS )
            pNestedChild = ( (ClassDescriptor*)(m_arrNestedChilds[i]) )->NestedChild(strName);
        if ( pNestedChild != NULL )
            return pNestedChild;
    }
    return NULL;
}
const MemberDescriptor * StructDescriptor::Member(const GChar * strName) const
{
    UInt i;
    for(i = 0; i < m_iMembersCount; ++i) {
        if ( StringFn->NCmp( m_arrMembers[i]->Name(), strName, REFLECTION_STRINGSIZE - 1 ) == 0 )
            return m_arrMembers[i];
    }
    return NULL;
}

/////////////////////////////////////////////////////////////////////////////////

Void StructDescriptor::_RemoveNestedChild(TypeDescriptor * pNestedChild)
{
    TypeDescriptor ** pCurNested = m_arrNestedChilds;
    TypeDescriptor ** pEnd = m_arrNestedChilds + m_iNestedChildsCount;
    while(pCurNested < pEnd) {
        if ( *pCurNested == pNestedChild )
            break;
        ++pCurNested;
    }
    if ( pCurNested < pEnd ) {
        --pEnd;
        while(pCurNested < pEnd) {
            *pCurNested = *(pCurNested + 1);
            ++pCurNested;
        }
        --m_iNestedChildsCount;
    }
}
Void StructDescriptor::_RemoveMember(MemberDescriptor * pMember)
{
    MemberDescriptor ** pCurMember = m_arrMembers;
    MemberDescriptor ** pEnd = m_arrMembers + m_iMembersCount;
    while(pCurMember < pEnd) {
        if ( *pCurMember == pMember )
            break;
        ++pCurMember;
    }
    if ( pCurMember < pEnd ) {
        --pEnd;
        while(pCurMember < pEnd) {
            *pCurMember = *(pCurMember + 1);
            ++pCurMember;
        }
        --m_iMembersCount;
    }
}

/////////////////////////////////////////////////////////////////////////////////
// END : _Reflection implementation
};
