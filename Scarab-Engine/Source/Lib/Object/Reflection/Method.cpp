/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Object/Reflection/Method.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Method descriptor.
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
#include "Method.h"

#include "Class.h"

/////////////////////////////////////////////////////////////////////////////////
// BEGIN : _Reflection implementation
namespace _Reflection {

const MethodCreationParams * _Builder_Method( const GChar * strName, SpecifierMember iMethod, SpecifierMemberMod iMethodMod,
                                              SpecifierScope iScope, SpecifierAccess iAccess, SpecifierStorage iStorage,
                                              InvokeHandler pfInvokeFunc,
                                              TypeDescriptor * pNestedParent,
                                              FunctionDescriptor * pType )
{
    static MethodCreationParams s_methodParams;
    s_methodParams.strName = strName;
    s_methodParams.iMethod = iMethod;
    s_methodParams.iMethodMod = iMethodMod;
    s_methodParams.iScope = iScope;
    s_methodParams.iAccess = iAccess;
    s_methodParams.iStorage = iStorage;
    s_methodParams.pfInvokeFunc = pfInvokeFunc;
    s_methodParams.pNestedParent = pNestedParent;
    s_methodParams.pType = pType;
    return &s_methodParams;
}

/////////////////////////////////////////////////////////////////////////////////
// _Reflection::MethodDescriptor implementation
MethodDescriptor::MethodDescriptor( const MethodCreationParams * pParams )
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

    m_specMethod = pParams->iMethod;
    m_specMethodMod = pParams->iMethodMod;
    m_specScope = pParams->iScope;
    m_specAccess = pParams->iAccess;
    m_specStorage = pParams->iStorage;

    m_pfInvoke = pParams->pfInvokeFunc;

    m_pType = pParams->pType;

    m_pNestedParent = pParams->pNestedParent;
    if (m_pNestedParent != NULL) {
        if ( m_pNestedParent->SpecType() == SPECIFIER_TYPE_CLASS )
            ((ClassDescriptor*)m_pNestedParent)->_AddMethod(this);
    }
}
MethodDescriptor::~MethodDescriptor()
{
    if (m_pNestedParent != NULL) {
        if ( m_pNestedParent->SpecType() == SPECIFIER_TYPE_CLASS )
            ((ClassDescriptor*)m_pNestedParent)->_RemoveMethod(this);
    }
}

const TypeDescriptor * MethodDescriptor::NestedParent(const GChar * strName) const
{
    if ( strName == NULL )
        return m_pNestedParent;
    if ( m_pNestedParent == NULL )
        return NULL;
    if ( StringFn->NCmp( m_pNestedParent->Name(), strName, REFLECTION_STRINGSIZE - 1 ) == 0 )
        return m_pNestedParent;
    return m_pNestedParent->NestedParent(strName);
}

Bool MethodDescriptor::IsEqual(const MethodDescriptor * pMethod) const
{
    if ( this == pMethod )
        return true;
    if ( StringFn->NCmp( m_strRelativeName, pMethod->m_strRelativeName, REFLECTION_STRINGSIZE - 1 ) != 0 )
        return false;
    if ( !(m_pType->IsEqual(pMethod->m_pType)) )
        return false;
    if ( m_specMethod != pMethod->m_specMethod )
        return false;
    if ( m_pNestedParent == NULL )
        return (pMethod->m_pNestedParent == NULL);
    if ( pMethod->m_pNestedParent == NULL )
        return false;
    return ( m_pNestedParent->IsEqual(pMethod->m_pNestedParent) );
}

/////////////////////////////////////////////////////////////////////////////////
// END : _Reflection implementation
};
