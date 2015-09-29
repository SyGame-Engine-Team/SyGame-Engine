/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Object/Reflection/Method.h
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
// Header prelude
#ifndef SCARAB_LIB_OBJECT_REFLECTION_METHOD_H
#define SCARAB_LIB_OBJECT_REFLECTION_METHOD_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Definitions.h"
#include "Type.h"
#include "Function.h"

/////////////////////////////////////////////////////////////////////////////////
// _Reflection namespace (private space)
namespace _Reflection
{
/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
    struct MethodCreationParams {
        const GChar * strName;
        SpecifierMethod iMethod;
        SpecifierMethodMod iMethodMod;
        SpecifierScope iScope;
        SpecifierAccess iAccess;
        SpecifierStorage iStorage;
        InvokeHandler pfInvokeFunc;
        TypeDescriptor * pNestedParent;
        FunctionDescriptor * pType;
    };
    const MethodCreationParams * _Builder_Method( const GChar * strName, SpecifierMember iMethod, SpecifierMemberMod iMethodMod,
                                                  SpecifierScope iScope, SpecifierAccess iAccess, SpecifierStorage iStorage,
                                                  InvokeHandler pfInvokeFunc,
                                                  TypeDescriptor * pNestedParent,
                                                  FunctionDescriptor * pType );

/////////////////////////////////////////////////////////////////////////////////
// MethodDescriptor
    class MethodDescriptor
    {
    public:
        MethodDescriptor( const MethodCreationParams * pParams );
        ~MethodDescriptor();

        inline const GChar * Name() const;
        inline const GChar * FullName() const;

        inline SpecifierMethod SpecMethod() const;
        inline SpecifierMethodMod SpecMethodMod() const;
        inline SpecifierScope SpecScope() const;
        inline SpecifierAccess SpecAccess() const;
        inline SpecifierStorage SpecStorage() const;

        inline const FunctionDescriptor * Type() const;
        const TypeDescriptor * NestedParent(const GChar * strName = NULL) const;

        Bool IsEqual(const MethodDescriptor * pMethod) const;

        inline Void Invoke( Void * pInstance ) const;
        template<typename T>
        inline T Invoke( Void * pInstance ) const;
        template<UInt ARGCOUNT>
    	inline Void Invoke( Void * pInstance, ... ) const;
        template<typename T, UInt ARGCOUNT>
    	inline T Invoke( Void * pInstance, ... ) const;

    private:
        // Names
        GChar m_strRelativeName[REFLECTION_STRINGSIZE];
        UInt m_iRelativeNameLength;
        GChar m_strAbsoluteName[REFLECTION_STRINGSIZE];
        UInt m_iAbsoluteNameLength;

        // Specifiers
        SpecifierMethod m_specMethod;
        SpecifierMethodMod m_specMethodMod;
        SpecifierScope m_specScope;
        SpecifierAccess m_specAccess;
        SpecifierStorage m_specStorage;

        // Handlers
        InvokeHandler m_pfInvoke;

        // Type link
        FunctionDescriptor * m_pType;

        // Nesting links
        TypeDescriptor * m_pNestedParent;
    };
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Method.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_OBJECT_REFLECTION_METHOD_H
