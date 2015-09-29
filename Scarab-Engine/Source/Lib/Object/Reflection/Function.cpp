/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Object/Reflection/Function.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Function type descriptor.
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
#include "Function.h"

/////////////////////////////////////////////////////////////////////////////////
// BEGIN : _Reflection implementation
namespace _Reflection {

/////////////////////////////////////////////////////////////////////////////////
// _Reflection::FunctionDescriptor implementation
FunctionDescriptor::FunctionDescriptor( const TypeCreationParams * pParams, SpecifierFunctionMod iFuncMod,
                                        ModTypeDescriptor * arrReturnArguments ):
    TypeDescriptor( pParams )
{
    m_specFuncMod = iFuncMod;
    m_arrReturnArguments = arrReturnArguments;
    m_iArgumentsCount = 0;
    ModTypeDescriptor * pArg = m_arrReturnArguments + 1;
    while( pArg->Type() != NULL ) {
        ++m_iArgumentsCount;
        ++pArg;
    }
}
FunctionDescriptor::~FunctionDescriptor()
{
    // nothing to do
}

Bool FunctionDescriptor::IsEqual(const TypeDescriptor * pType) const
{
    if ( pType->SpecType() != SPECIFIER_TYPE_FUNCTION )
        return false;
    FunctionDescriptor * pFunction = (FunctionDescriptor*)pType;
    if ( this == pFunction )
        return true;
    if ( StringFn->NCmp( m_strRelativeName, pFunction->m_strRelativeName, REFLECTION_STRINGSIZE - 1 ) != 0 )
        return false;
    if ( m_specScope != pFunction->m_specScope )
        return false;
    if ( m_iArgumentsCount != pFunction->m_iArgumentsCount )
        return false;
    for(UInt i = 0; i < m_iArgumentsCount + 1; ++i) {
        if ( !( m_arrReturnArguments[i].IsEqual(pFunction->m_arrReturnArguments[i]) ) )
            return false;
    }
    if ( m_pNestedParent == NULL )
        return (pFunction->m_pNestedParent == NULL);
    if ( pFunction->m_pNestedParent == NULL )
        return false;
    return ( m_pNestedParent->IsEqual(pFunction->m_pNestedParent) );
}

/////////////////////////////////////////////////////////////////////////////////
// END : _Reflection implementation
};
