/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Object/Reflection/UnionStruct.h
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
// Header prelude
#ifndef SCARAB_LIB_OBJECT_REFLECTION_UNIONSTRUCT_H
#define SCARAB_LIB_OBJECT_REFLECTION_UNIONSTRUCT_H

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
    // Union descriptor
    class UnionDescriptor : public TypeDescriptor
    {
    public:
        UnionDescriptor( const TypeCreationParams * pParams );
        ~UnionDescriptor();

        const TypeDescriptor * NestedChild(const GChar * strName) const;
        const MemberDescriptor * Member(const GChar * strName) const;

    private:
        friend class TypeDescriptor;
        friend class MemberDescriptor;

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
    };

    // Struct descriptor
    class StructDescriptor : public TypeDescriptor
    {
    public:
        StructDescriptor( const TypeCreationParams * pParams );
        ~StructDescriptor();

        const TypeDescriptor * NestedChild(const GChar * strName) const;
        const MemberDescriptor * Member(const GChar * strName) const;

    private:
        friend class TypeDescriptor;
        friend class MemberDescriptor;

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
    };

};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "UnionStruct.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_OBJECT_REFLECTION_UNIONSTRUCT_H
