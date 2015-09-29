/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Object/Reflection/Reflect.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Reflection interface.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs :
//             UNSUPPORTED : - const pointers / various ind levels
//                           - no function-pointer, use typedefs ...
//                           - only public non-virtual inheritance
//                           - extern & register storages
//                           - using references gets very tricky ...
//             TODO : Make child arrays sorted & use dichotomic searches
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_LIB_OBJECT_REFLECTION_REFLECT_H
#define SCARAB_LIB_OBJECT_REFLECTION_REFLECT_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Definitions.h"
#include "Type.h"
#include "Function.h"
#include "UnionStruct.h"
#include "Class.h"
#include "Member.h"
#include "Method.h"
#include "Macros.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define ReflectFn ReflectionInterface::GetInstancePtr()

typedef _Reflection::TypeDescriptor Type;
//typedef _Reflection::BuiltinDescriptor Builtin;
//typedef _Reflection::EnumDescriptor Enum;
//typedef _Reflection::TypedefDescriptor Typedef;
typedef _Reflection::FunctionDescriptor Function;
typedef _Reflection::UnionDescriptor Union;
typedef _Reflection::StructDescriptor Struct;
typedef _Reflection::ClassDescriptor Class;
typedef _Reflection::MemberDescriptor Member;
typedef _Reflection::MethodDescriptor Method;

/////////////////////////////////////////////////////////////////////////////////
// The ReflectionInterface class
class ReflectionInterface
{
    // Discrete singleton interface
public:
    inline static ReflectionInterface * GetInstancePtr();

private:
    ReflectionInterface();
    ~ReflectionInterface();

public:
    // Reflection object getters
    template<typename T>
    inline Type * GetType() const;
    template<typename T>
    inline Type * GetType(const T * pObject) const;
    template<typename T>
    inline Type * GetType(const T & rObject) const;

    inline Type * GetType(const GChar * strName) const;

    // Type casting
    template<typename TO, typename FROM>
    inline TO * Cast( FROM * pObject ) const;
    template<typename TO, typename FROM>
    inline TO & Cast( FROM & rObject ) const;

    template<typename FROM>
    inline Void * Cast( const Type * pTarget, FROM * pObject ) const;

private:

};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Reflect.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_OBJECT_REFLECTION_REFLECT_H
