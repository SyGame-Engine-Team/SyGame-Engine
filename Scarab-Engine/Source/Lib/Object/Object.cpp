/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Object/Object.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : The Object fundamental type, all classes herit from this, at
//               least indirectly ...
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Object.h"

/////////////////////////////////////////////////////////////////////////////////
// Object implementation
ReflectDefineClass_0( Object, SPECIFIER_TYPEMOD_NONE, SPECIFIER_CLASSMOD_NONE, New );
ReflectDefineMethod_Ret_0( Object, GetHash, SPECIFIER_FUNCTIONMOD_THISCALL | SPECIFIER_FUNCTIONMOD_CONST,
                           SPECIFIER_METHOD_GETTER, SPECIFIER_METHODMOD_NONE,
                           SPECIFIER_ACCESS_PUBLIC, SPECIFIER_STORAGE_VIRTUAL,
                           DWord );
ReflectDefineMethod_1( Object, Serialize, SPECIFIER_FUNCTIONMOD_THISCALL | SPECIFIER_FUNCTIONMOD_CONST,
                       SPECIFIER_METHOD_NONE, SPECIFIER_METHODMOD_NONE,
                       SPECIFIER_ACCESS_PUBLIC, SPECIFIER_STORAGE_VIRTUAL,
                       const Object & );

Object::Object()
{
    // do nothing
}
Object::Object( const Object & /*rhs*/ )
{
    // do nothing
}
Object::~Object()
{
    // do nothing
}
