/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Object/Reflection/Member.h
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
// Header prelude
#ifndef SCARAB_LIB_OBJECT_REFLECTION_MEMBER_H
#define SCARAB_LIB_OBJECT_REFLECTION_MEMBER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Definitions.h"
#include "Type.h"

/////////////////////////////////////////////////////////////////////////////////
// _Reflection namespace (private space)
namespace _Reflection
{
/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
    struct MemberCreationParams {
        const GChar * strName;
        SpecifierScope iScope;
        SpecifierAccess iAccess;
        SpecifierStorage iStorage;
        UInt iByteSize;
        GetHandler pfGetFunc;
        SetHandler pfSetFunc;
        TypeDescriptor * pNestedParent;
        const GChar * strTypeExpr;
    };
    const MemberCreationParams * _Builder_Member( const GChar * strName,
                                                  SpecifierScope iScope, SpecifierAccess iAccess, SpecifierStorage iStorage,
                                                  UInt iBytes, GetHandler pfGetFunc, SetHandler pfSetFunc,
                                                  TypeDescriptor * pNestedParent,
                                                  const GChar * strTypeExpr );

/////////////////////////////////////////////////////////////////////////////////
// MemberDescriptor
    class MemberDescriptor
    {
    public:
        MemberDescriptor( const MemberCreationParams * pParams );
        ~MemberDescriptor();

        inline const GChar * Name() const;
        inline const GChar * FullName() const;

        inline SpecifierMember SpecMember() const;
        inline SpecifierMemberMod SpecMemberMod() const;
        inline SpecifierScope SpecScope() const;
        inline SpecifierAccess SpecAccess() const;
        inline SpecifierStorage SpecStorage() const;
        inline UInt Size() const;
        inline UInt IndirectionLevel() const;

        inline const TypeDescriptor * Type() const;
        const TypeDescriptor * NestedParent(const GChar * strName) const;

        Bool IsEqual(const MemberDescriptor * pMember) const;

        template<typename T>
        inline T Get( Void * pInstance ) const;
	    template<typename T>
        inline Void Set( Void * pInstance, T value ) const;

    private:
        // Names
        GChar m_strRelativeName[REFLECTION_STRINGSIZE];
        UInt m_iRelativeNameLength;
        GChar m_strAbsoluteName[REFLECTION_STRINGSIZE];
        UInt m_iAbsoluteNameLength;

        // Specifiers
        SpecifierScope m_specScope;
        SpecifierAccess m_specAccess;
        SpecifierStorage m_specStorage;
        UInt m_iBytes;

        // Handlers
        GetHandler m_pfGet;
        SetHandler m_pfSet;

        // Type link
        ModTypeDescriptor m_ModType;

        // Nesting links
        TypeDescriptor * m_pNestedParent;
    };
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Member.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_OBJECT_REFLECTION_MEMBER_H
