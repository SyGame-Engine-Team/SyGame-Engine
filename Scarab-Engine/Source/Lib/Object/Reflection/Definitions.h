/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Object/Reflection/Definitions.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Reflection library shared definitions.
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
#ifndef SCARAB_LIB_OBJECT_REFLECTION_DEFINITIONS_H
#define SCARAB_LIB_OBJECT_REFLECTION_DEFINITIONS_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../ThirdParty/System/Platform.h"
#include "../../../ThirdParty/System/String.h"

#include "../../Math/Hash/Hashing.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define REFLECTION_STRINGSIZE 256

#define REFLECTION_LOOKUP_SIZE 4096

#define REFLECTION_MAX_CHILDS 32 // acceptable for now
#define REFLECTION_MAX_NESTED 8
#define REFLECTION_MAX_METHODS 128
#define REFLECTION_MAX_MEMBERS 128

// Type specifiers
#define _SPECIFIER_TYPE_MASK    0x0f
#define _SPECIFIER_TYPE_SHIFT   4
#define SPECIFIER_TYPE_NULL     0x00
#define SPECIFIER_TYPE_VOID     0x01
#define SPECIFIER_TYPE_BUILTIN  0x02
#define SPECIFIER_TYPE_FUNCTION 0x03
#define SPECIFIER_TYPE_TYPEDEF  0x04
#define SPECIFIER_TYPE_ENUM     0x05
#define SPECIFIER_TYPE_UNION    0x06
#define SPECIFIER_TYPE_STRUCT   0x07
#define SPECIFIER_TYPE_CLASS    0x08

// Type mod specifiers
#define _SPECIFIER_TYPEMOD_MASK     0x03
#define _SPECIFIER_TYPEMOD_SHIFT    2
#define SPECIFIER_TYPEMOD_NONE      0x00
#define SPECIFIER_TYPEMOD_INTERNAL  0x01 // |-flag (private at module scope)
#define SPECIFIER_TYPEMOD_TEMPLATE  0x02 // |-flag, function/union/struct/class only

// Type function-mod specifiers
#define _SPECIFIER_FUNCTIONMOD_MASK     0x3f
#define _SPECIFIER_FUNCTIONMOD_SHIFT    6
#define SPECIFIER_FUNCTIONMOD_CDECL     0x00 // calling convention
#define SPECIFIER_FUNCTIONMOD_STDCALL   0x01 // calling convention
#define SPECIFIER_FUNCTIONMOD_FASTCALL  0x02 // calling convention
#define SPECIFIER_FUNCTIONMOD_THISCALL  0x03 // calling convention
#define SPECIFIER_FUNCTIONMOD_INLINE    0x04 // |-flag
#define SPECIFIER_FUNCTIONMOD_CONST     0x08 // |-flag
#define SPECIFIER_FUNCTIONMOD_RECURSIVE 0x10 // |-flag
#define SPECIFIER_FUNCTIONMOD_VARGLIST  0x20 // |-flag

// Type class-mod specifiers
#define _SPECIFIER_CLASSMOD_MASK     0x03
#define _SPECIFIER_CLASSMOD_SHIFT    2
#define SPECIFIER_CLASSMOD_NONE      0x00
#define SPECIFIER_CLASSMOD_ABSTRACT  0x01 // class only (must be derived)
#define SPECIFIER_CLASSMOD_INTERFACE 0x02 // class only (must be fully derived)
#define SPECIFIER_CLASSMOD_SEALED    0x03 // class only (cannot be derived)

// Member specifiers
#define _SPECIFIER_MEMBER_MASK     0x07
#define _SPECIFIER_MEMBER_SHIFT    3
#define SPECIFIER_MEMBER_VALUE     0x00
#define SPECIFIER_MEMBER_REFERENCE 0x01
#define SPECIFIER_MEMBER_POINTER   0x02
#define SPECIFIER_MEMBER_ARRAY     0x03
#define SPECIFIER_MEMBER_FUNCPTR   0x04
#define SPECIFIER_MEMBER_MEMBERPTR 0x05
#define SPECIFIER_MEMBER_METHODPTR 0x06

// Member mod specifiers
#define _SPECIFIER_MEMBERMOD_MASK     0x07
#define _SPECIFIER_MEMBERMOD_SHIFT    3
#define SPECIFIER_MEMBERMOD_NONE      0x00
#define SPECIFIER_MEMBERMOD_CONST     0x01
#define SPECIFIER_MEMBERMOD_VOLATILE  0x02
#define SPECIFIER_MEMBERMOD_MUTABLE   0x03 // can't be static (writable by const methods)
#define SPECIFIER_MEMBERMOD_TRANSIENT 0x04 // |-flag (ignore for serialization)

// Method specifiers
#define _SPECIFIER_METHOD_MASK      0x07
#define _SPECIFIER_METHOD_SHIFT     3
#define SPECIFIER_METHOD_NONE       0x00
#define SPECIFIER_METHOD_CONSTR     0x01
#define SPECIFIER_METHOD_CONSTRCOPY 0x02
#define SPECIFIER_METHOD_DESTR      0x03
#define SPECIFIER_METHOD_OPERATOR   0x04
#define SPECIFIER_METHOD_TYPECAST   0x05
#define SPECIFIER_METHOD_GETTER     0x06
#define SPECIFIER_METHOD_SETTER     0x07

// Method mod specifiers
#define _SPECIFIER_METHODMOD_MASK    0x07
#define _SPECIFIER_METHODMOD_SHIFT   3
#define SPECIFIER_METHODMOD_NONE     0x00
#define SPECIFIER_METHODMOD_IMPLICIT 0x01 // |-flag, constr/typecast only (use implicit typecasts)
#define SPECIFIER_METHODMOD_EXPLICIT 0x02 // |-flag, constr/typecast only (force explicit typecasts)
#define SPECIFIER_METHODMOD_OVERRIDE 0x04 // |-flag

// Scope specifiers
#define _SPECIFIER_SCOPE_MASK     0x03
#define _SPECIFIER_SCOPE_SHIFT    2
#define SPECIFIER_SCOPE_GLOBAL    0x00
#define SPECIFIER_SCOPE_NESTED    0x01
#define SPECIFIER_SCOPE_ARGUMENT  0x02
#define SPECIFIER_SCOPE_TEMPLATE  0x03

// Access specifiers
#define _SPECIFIER_ACCESS_MASK     0x03
#define _SPECIFIER_ACCESS_SHIFT    2
#define SPECIFIER_ACCESS_DEFAULT   0x00
#define SPECIFIER_ACCESS_PUBLIC    0x01
#define SPECIFIER_ACCESS_PROTECTED 0x02
#define SPECIFIER_ACCESS_PRIVATE   0x03

// Storage specifiers
#define _SPECIFIER_STORAGE_MASK        0x07
#define _SPECIFIER_STORAGE_SHIFT       3
#define SPECIFIER_STORAGE_DEFAULT      0x00
#define SPECIFIER_STORAGE_STATIC       0x01
#define SPECIFIER_STORAGE_VIRTUAL      0x02
#define SPECIFIER_STORAGE_VIRTUAL_PURE 0x03
#define SPECIFIER_STORAGE_EXTERN       0x04
#define SPECIFIER_STORAGE_REGISTER     0x05

/////////////////////////////////////////////////////////////////////////////////
// _Reflection namespace (private space)
namespace _Reflection
{
    // Specifiers
    typedef Byte SpecifierType;
    typedef Byte SpecifierTypeMod;
    typedef Byte SpecifierFunctionMod;
    typedef Byte SpecifierClassMod;
    typedef Byte SpecifierMember;
    typedef Byte SpecifierMemberMod;
    typedef Byte SpecifierMethod;
    typedef Byte SpecifierMethodMod;
    typedef Byte SpecifierScope;
    typedef Byte SpecifierAccess;
    typedef Byte SpecifierStorage;

    // Handlers
    typedef Void * (*CastHandler)( Void * pInstance, UInt iParent );
    typedef Void * (*NewHandler)();
	typedef Void * (*GetHandler)( Void * pInstance );
	typedef Void   (*SetHandler)( Void * pInstance, Void * pValue );
    typedef Void * (*InvokeHandler)( Void * pInstance, Void ** arrArgV, UInt iArgC );

    // Prototypes
    struct TypeCreationParams;
    struct MemberCreationParams;
    struct MethodCreationParams;
    class TypeDescriptor;
    class ModTypeDescriptor;
    class BuiltinDescriptor;
    class FunctionDescriptor;
    class TypedefDescriptor;
    class EnumDescriptor;
    class UnionDescriptor;
    class StructDescriptor;
    class ClassDescriptor;
    class MemberDescriptor;
    class MethodDescriptor;

/////////////////////////////////////////////////////////////////////////////////
// Holder template
    template<typename T>
	class Holder {
	    public:  Holder():m_Value()                                  { }
		         Holder(T value):m_Value(value)                      { }
		         Holder(const Holder<T> & rhs):m_Value(rhs.m_Value)  { }
		         ~Holder()                                           { }
		         inline operator T() const                           { return m_Value; }
		         inline Holder<T> & operator=(T rhs) {
                     if (&m_Value == &rhs) return (*this); m_Value = rhs; return (*this);
                 }
		         inline Holder<T> & operator=(const Holder<T> & rhs) {
                     if (this == &rhs) return (*this); m_Value = rhs.m_Value; return (*this);
                 }
	    private: T m_Value;
	};

/////////////////////////////////////////////////////////////////////////////////
// LookupTable
    template<typename T, UInt SIZE>
    class LookupTable
    {
    public:
        LookupTable();
        ~LookupTable();

        inline T * Get( const GChar * strName ) const;
        const GChar * Insert( T * pItem );
        Void Remove( const GChar * strName );

    private:
        inline DWord _Hash( const GChar * strName ) const;
        UInt _Index(DWord dwHash) const;

        typedef struct _lookup_item
        {
            DWord dwHash;
            const GChar * strName; // Full Names
            T * pItem;
        } LookupItem;

        LookupItem m_arrItems[SIZE];
    };

};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Definitions.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_OBJECT_REFLECTION_DEFINITIONS_H
