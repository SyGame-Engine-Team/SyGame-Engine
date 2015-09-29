/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Object/Reflection/Type.h
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
// Known Bugs : Unimplemented derived types : BuiltinDescriptor,
//                                            TypedefDescriptor,
//                                            EnumDescriptor
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_LIB_OBJECT_REFLECTION_TYPE_H
#define SCARAB_LIB_OBJECT_REFLECTION_TYPE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Definitions.h"

/////////////////////////////////////////////////////////////////////////////////
// _Reflection namespace (private space)
namespace _Reflection
{
/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
    struct TypeCreationParams {
        const GChar * strName;
        SpecifierType iType;
        SpecifierTypeMod iTypeMod;
        SpecifierScope iScope;
        SpecifierAccess iAccess;
        UInt iByteSize;
        CastHandler pfCastFunc;
        NewHandler pfNewFunc;
        TypeDescriptor * pNestedParent;
    };
    const TypeCreationParams * _Builder_Type( const GChar * strName, SpecifierType iType, SpecifierTypeMod iTypeMod,
                                              SpecifierScope iScope, SpecifierAccess iAccess,
                                              UInt iBytes, CastHandler pfCastFunc, NewHandler pfNewFunc,
                                              TypeDescriptor * pNestedParent );

/////////////////////////////////////////////////////////////////////////////////
// TypeDescriptor
    class TypeDescriptor
    {
    public:
        TypeDescriptor( const TypeCreationParams * pParams );
        ~TypeDescriptor();

        inline static TypeDescriptor * GetByName(const GChar * strName);

        inline const GChar * Name() const;
        inline const GChar * FullName() const;

        inline SpecifierType SpecType() const;
        inline SpecifierTypeMod SpecTypeMod() const;
        inline SpecifierScope SpecScope() const;
        inline SpecifierAccess SpecAccess() const;
        inline UInt Size() const;

        const TypeDescriptor * NestedParent(const GChar * strName = NULL) const;

        virtual Bool IsEqual(const TypeDescriptor * pType) const;

        template<typename T>
        inline T * Cast( Void * pInstance, UInt iParent = INVALID_OFFSET ) const;
        template<typename T>
        inline T * Instanciate() const;

    protected:
        // Names
        GChar m_strRelativeName[REFLECTION_STRINGSIZE];
        UInt m_iRelativeNameLength;
        GChar m_strAbsoluteName[REFLECTION_STRINGSIZE];
        UInt m_iAbsoluteNameLength;

        // Specifiers
        SpecifierType m_specType;
        SpecifierTypeMod m_specTypeMod;
        SpecifierScope m_specScope;
        SpecifierAccess m_specAccess;
        UInt m_iBytes;

        // Handlers
        CastHandler m_pfCast;
        NewHandler m_pfNew;

        // Nesting links
        TypeDescriptor * m_pNestedParent;

        // Types table
        static LookupTable<TypeDescriptor,REFLECTION_LOOKUP_SIZE> sm_htTypes;
    };

/////////////////////////////////////////////////////////////////////////////////
// ModTypeDescriptor
    class ModTypeDescriptor
    {
    public:
        ModTypeDescriptor();
        ModTypeDescriptor( const GChar * strExpr );
        ModTypeDescriptor( const ModTypeDescriptor & rhs );
        ~ModTypeDescriptor();

        inline ModTypeDescriptor & operator=(const ModTypeDescriptor & rhs);

        inline SpecifierMember SpecKind() const;
        inline SpecifierMemberMod SpecModifier() const;
        inline UInt IndirectionLevel() const;
        inline const TypeDescriptor * Type() const;

        Bool IsEqual(const ModTypeDescriptor & rModType) const;

    private:
        Void _ParseTypeExpr( const GChar * strExpr );

        SpecifierMember m_iMember;
        SpecifierMemberMod m_iMemberMod;
        UInt m_iIndirectionLevel;
        TypeDescriptor * m_pType;
    };

};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Type.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_OBJECT_REFLECTION_TYPE_H
