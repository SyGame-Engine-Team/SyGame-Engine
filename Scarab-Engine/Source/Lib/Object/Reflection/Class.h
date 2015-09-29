/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Object/Reflection/Class.h
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
// Header prelude
#ifndef SCARAB_LIB_OBJECT_REFLECTION_CLASS_H
#define SCARAB_LIB_OBJECT_REFLECTION_CLASS_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Definitions.h"
#include "Type.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// _Reflection namespace (private space)
namespace _Reflection
{
    // Class descriptor
    class ClassDescriptor : public TypeDescriptor
    {
    public:
        ClassDescriptor( const TypeCreationParams * pParams, SpecifierClassMod iClassMod,
                         ClassDescriptor ** arrParents );
        ~ClassDescriptor();

        inline SpecifierClassMod SpecClassMod() const;

        // Getters
        const ClassDescriptor * Parent(const GChar * strName) const;
        const ClassDescriptor * Child(const GChar * strName) const;
        const TypeDescriptor * NestedChild(const GChar * strName) const;
        const MemberDescriptor * Member(const GChar * strName) const;
        const MethodDescriptor * Method(const GChar * strName) const;

        // Inheritance methods
        Bool HeritsFrom(const ClassDescriptor * pType) const;
        inline Bool CanCastFrom(const ClassDescriptor * pType) const;

        template<typename T>
        T * Instanciate(const ClassDescriptor * pTarget, const GChar * strName) const;

    private:
        friend class TypeDescriptor;
        friend class MemberDescriptor;
        friend class MethodDescriptor;

        // Specifiers
        SpecifierClassMod m_specClassMod;

        // Inheritance links
        inline Void _AddChild(ClassDescriptor * pChild);
        Void _RemoveChild(ClassDescriptor * pChild);
        ClassDescriptor ** m_arrParents; // NULL-ended array
        UInt m_iParentsCount;
        ClassDescriptor * m_arrChilds[REFLECTION_MAX_CHILDS];
        UInt m_iChildsCount;

        // Nesting links
        inline Void _AddNestedChild(TypeDescriptor * pNestedChild);
        Void _RemoveNestedChild(TypeDescriptor * pNestedChild);
        TypeDescriptor * m_arrNestedChilds[REFLECTION_MAX_NESTED];
        UInt m_iNestedChildsCount;

        // Member links
        inline Void _AddMember(MemberDescriptor * pMember);
        Void _RemoveMember(MemberDescriptor * pMember);
        MemberDescriptor * m_arrMembers[REFLECTION_MAX_MEMBERS];
        UInt m_iMembersCount;

        // Method links
        inline Void _AddMethod(MethodDescriptor * pMethod);
        Void _RemoveMethod(MethodDescriptor * pMethod);
        MethodDescriptor * m_arrMethods[REFLECTION_MAX_METHODS];
        UInt m_iMethodsCount;
    };

};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Class.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_OBJECT_REFLECTION_CLASS_H
