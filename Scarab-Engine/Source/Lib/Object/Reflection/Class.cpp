/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Object/Reflection/Class.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Class descriptor.
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
#include "Class.h"

#include "Member.h"
#include "Method.h"
#include "UnionStruct.h"

/////////////////////////////////////////////////////////////////////////////////
// BEGIN : _Reflection implementation
namespace _Reflection {

/////////////////////////////////////////////////////////////////////////////////
// _Reflection::ClassDescriptor implementation
ClassDescriptor::ClassDescriptor( const TypeCreationParams * pParams, SpecifierClassMod iClassMod,
                                  ClassDescriptor ** arrParents ):
    TypeDescriptor( pParams )
{
    m_specClassMod = iClassMod;
    m_arrParents = arrParents;
    m_iParentsCount = 0;
    while(m_arrParents[m_iParentsCount] != NULL) {
        m_arrParents[m_iParentsCount]->_AddChild(this);
        ++m_iParentsCount;
    }
}
ClassDescriptor::~ClassDescriptor()
{
    for(UInt i = 0; i < m_iParentsCount; ++i)
        m_arrParents[i]->_RemoveChild(this);
}

const ClassDescriptor * ClassDescriptor::Parent(const GChar * strName) const
{
    UInt i;
    for(i = 0; i < m_iParentsCount; ++i) {
        if ( StringFn->NCmp( m_arrParents[i]->m_strRelativeName, strName, REFLECTION_STRINGSIZE - 1 ) == 0 )
            return m_arrParents[i];
    }
    const ClassDescriptor * pParent = NULL;
    for(i = 0; i < m_iParentsCount; ++i) {
        pParent = m_arrParents[i]->Parent(strName);
        if ( pParent != NULL )
            return pParent;
    }
    return NULL;
}
const ClassDescriptor * ClassDescriptor::Child(const GChar * strName) const
{
    UInt i;
    for(i = 0; i < m_iChildsCount; ++i) {
        if ( StringFn->NCmp( m_arrChilds[i]->m_strRelativeName, strName, REFLECTION_STRINGSIZE - 1 ) == 0 )
            return m_arrChilds[i];
    }
    const ClassDescriptor * pChild = NULL;
    for(i = 0; i < m_iChildsCount; ++i) {
        pChild = m_arrChilds[i]->Child(strName);
        if ( pChild != NULL )
            return pChild;
    }
    return NULL;
}
const TypeDescriptor * ClassDescriptor::NestedChild(const GChar * strName) const
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
const MemberDescriptor * ClassDescriptor::Member(const GChar * strName) const
{
    UInt i;
    for(i = 0; i < m_iMembersCount; ++i) {
        if ( StringFn->NCmp( m_arrMembers[i]->Name(), strName, REFLECTION_STRINGSIZE - 1 ) == 0 )
            return m_arrMembers[i];
    }
    const MemberDescriptor * pMember;
    for(i = 0; i < m_iParentsCount; ++i) {
        pMember = m_arrParents[i]->Member(strName);
        if ( pMember != NULL )
            return pMember;
    }
    return NULL;
}
const MethodDescriptor * ClassDescriptor::Method(const GChar * strName) const
{
    UInt i;
    for(i = 0; i < m_iMethodsCount; ++i) {
        if ( StringFn->NCmp( m_arrMethods[i]->Name(), strName, REFLECTION_STRINGSIZE - 1 ) == 0 )
            return m_arrMethods[i];
    }
    const MethodDescriptor * pMethod;
    for(i = 0; i < m_iParentsCount; ++i) {
        pMethod = m_arrParents[i]->Method(strName);
        if ( pMethod != NULL )
            return pMethod;
    }
    return NULL;
}

Bool ClassDescriptor::HeritsFrom(const ClassDescriptor * pType) const
{
    UInt i;
    for(i=0; i < m_iParentsCount; ++i) {
		if ( m_arrParents[i]->IsEqual(pType) )
            return true;
	}
    for(i=0; i < m_iParentsCount; ++i) {
		if ( m_arrParents[i]->HeritsFrom(pType) )
            return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////////

Void ClassDescriptor::_RemoveChild(ClassDescriptor * pChild)
{
    ClassDescriptor ** pCurChild = m_arrChilds;
    ClassDescriptor ** pEnd = m_arrChilds + m_iChildsCount;
    while(pCurChild < pEnd) {
        if ( *pCurChild == pChild )
            break;
        ++pCurChild;
    }
    if ( pCurChild < pEnd ) {
        --pEnd;
        while(pCurChild < pEnd) {
            *pCurChild = *(pCurChild + 1);
            ++pCurChild;
        }
        --m_iChildsCount;
    }
}
Void ClassDescriptor::_RemoveNestedChild(TypeDescriptor * pNestedChild)
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
Void ClassDescriptor::_RemoveMember(MemberDescriptor * pMember)
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
Void ClassDescriptor::_RemoveMethod(MethodDescriptor * pMethod)
{
    MethodDescriptor ** pCurMethod = m_arrMethods;
    MethodDescriptor ** pEnd = m_arrMethods + m_iMethodsCount;
    while(pCurMethod < pEnd) {
        if ( *pCurMethod == pMethod )
            break;
        ++pCurMethod;
    }
    if ( pCurMethod < pEnd ) {
        --pEnd;
        while(pCurMethod < pEnd) {
            *pCurMethod = *(pCurMethod + 1);
            ++pCurMethod;
        }
        --m_iMethodsCount;
    }
}

/////////////////////////////////////////////////////////////////////////////////
// END : _Reflection implementation
};
