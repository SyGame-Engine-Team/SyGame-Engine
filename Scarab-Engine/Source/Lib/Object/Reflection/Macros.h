/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Object/Reflection/Macros.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Macro interface declaration.
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
#ifndef SCARAB_LIB_OBJECT_REFLECTION_MACROS_H
#define SCARAB_LIB_OBJECT_REFLECTION_MACROS_H

/////////////////////////////////////////////////////////////////////////////////
// Includes

/////////////////////////////////////////////////////////////////////////////////
// Class Master Declaration
#define ReflectDeclareClass \
	public: \
        virtual Void * _Reflect_VirtualCast(const _Reflection::ClassDescriptor *); \
        inline static  _Reflection::ClassDescriptor * _Reflect_StaticGet() { return &_Reflect_Class; } \
		inline virtual _Reflection::ClassDescriptor * _Reflect_VirtualGet() const { return &_Reflect_Class; } \
    protected: \
        static Void * _Reflect_StaticCast(Void*, UInt); \
        static Void * _Reflect_Construct(); \
        static _Reflection::ClassDescriptor _Reflect_Class;

/////////////////////////////////////////////////////////////////////////////////
// Class Global definitions
#define ReflectDefineClass_0( _class, _typemod, _classmod, _alloc_method ) \
    Void * _class::_Reflect_VirtualCast(const _Reflection::ClassDescriptor * pType) { \
		if (pType == &_Reflect_Class) return this; \
		return NULL; \
	} \
    Void * _class::_Reflect_StaticCast(Void * pInstance, UInt) { \
		_class * pCasted = (_class*)pInstance; \
        return pCasted; \
	} \
    Void * _class::_Reflect_Construct() { \
        return _alloc_method _class; \
    } \
    static _Reflection::ClassDescriptor * _Reflect_Class_arrParents_ ## _class [] = { \
        NULL \
    }; \
    _Reflection::ClassDescriptor _class::_Reflect_Class ( \
        _Reflection::_Builder_Type ( \
            TEXT(#_class), SPECIFIER_TYPE_CLASS, _typemod, SPECIFIER_SCOPE_GLOBAL, SPECIFIER_ACCESS_DEFAULT, \
            sizeof(_class), _class::_Reflect_StaticCast, _class::_Reflect_Construct, \
            NULL \
        ), \
        _classmod, _Reflect_Class_arrParents_ ## _class \
    );

#define ReflectDefineClass_1( _class, _typemod, _classmod, _alloc_method, _parent0 ) \
    Void * _class::_Reflect_VirtualCast(const _Reflection::ClassDescriptor * pType) { \
		if (pType == &_Reflect_Class) return this; \
        Void * pPtr = _parent0::_Reflect_VirtualCast(pType); \
		if (pPtr != NULL) return pPtr; \
		return NULL; \
	} \
    Void * _class::_Reflect_StaticCast(Void * pInstance, UInt iParent) { \
		_class * pCasted = (_class*)pInstance; \
        if (iParent == 0) return (_parent0*)pCasted; \
        return pCasted; \
	} \
    Void * _class::_Reflect_Construct() { \
        return _alloc_method _class; \
    } \
    static _Reflection::ClassDescriptor * _Reflect_Class_arrParents_ ## _class [] = { \
        _parent0::_Reflect_StaticGet(), NULL \
    }; \
    _Reflection::ClassDescriptor _class::_Reflect_Class ( \
        _Reflection::_Builder_Type ( \
            TEXT(#_class), SPECIFIER_TYPE_CLASS, _typemod, SPECIFIER_SCOPE_GLOBAL, SPECIFIER_ACCESS_DEFAULT, \
            sizeof(_class), _class::_Reflect_StaticCast, _class::_Reflect_Construct, \
            NULL \
        ), \
        _classmod, _Reflect_Class_arrParents_ ## _class \
    );

/////////////////////////////////////////////////////////////////////////////////
// Class Nested definitions
#define ReflectDefineNestedClass_0( _nestclass, _class, _typemod, _access, _classmod, _alloc_method ) \
    Void * _nestclass::_class::_Reflect_VirtualCast(const _Reflection::ClassDescriptor * pType) { \
		if (pType == &_Reflect_Class) return this; \
		return NULL; \
	} \
    Void * _nestclass::_class::_Reflect_StaticCast(Void * pInstance, UInt) { \
		_class * pCasted = (_class*)pInstance; \
        return pCasted; \
	} \
    Void * _nestclass::_class::_Reflect_Construct() { \
        return _alloc_method _class; \
    } \
    static _Reflection::ClassDescriptor * _Reflect_Class_arrParents_ ## _class [] = { NULL }; \
    _Reflection::ClassDescriptor _nestclass::_class::_Reflect_Class ( \
        _Reflection::_Builder_Type ( \
            TEXT(#_class), SPECIFIER_TYPE_CLASS, _typemod, SPECIFIER_SCOPE_NESTED, _access, \
            sizeof(_class), _nestclass::_class::_Reflect_StaticCast, _nestclass::_class::_Reflect_Construct, \
            _nestclass::_Reflect_StaticGet() \
        ), \
        _classmod, _Reflect_Class_arrParents_ ## _class \
    );

#define ReflectDefineNestedClass_1( _nestclass, _class, _typemod, _access, _classmod, _alloc_method, _parent0 ) \
    Void * _nestclass::_class::_Reflect_VirtualCast(const _Reflection::ClassDescriptor * pType) { \
		if (pType == &_Reflect_Class) return this; \
        Void * pPtr = _parent0::_Reflect_VirtualCast(pType); \
		if (pPtr != NULL) return pPtr; \
		return NULL; \
	} \
    Void * _nestclass::_class::_Reflect_StaticCast(Void * pInstance, UInt iParent) { \
		_class * pCasted = (_class*)pInstance; \
        if (iParent == 0) return (_parent0*)pCasted; \
        return pCasted; \
	} \
    Void * _nestclass::_class::_Reflect_Construct() { \
        return _alloc_method _class; \
    } \
    static _Reflection::ClassDescriptor * _Reflect_Class_arrParents_ ## _class [] = { \
        _parent0::_Reflect_StaticGet(), NULL \
    }; \
    _Reflection::ClassDescriptor _nestclass::_class::_Reflect_Class ( \
        _Reflection::_Builder_Type ( \
            TEXT(#_class), SPECIFIER_TYPE_CLASS, _typemod, SPECIFIER_SCOPE_NESTED, _access, \
            sizeof(_class), _nestclass::_class::_Reflect_StaticCast, _nestclass::_class::_Reflect_Construct, \
            _nestclass::_Reflect_StaticGet() \
        ), \
        _classmod, _Reflect_Class_arrParents_ ## _class \
    );

/////////////////////////////////////////////////////////////////////////////////
// Member interface
#define ReflectDeclareMember( _member ) \
    protected: \
        static Void * _Reflect_Member_Getter_ ## _member( Void * pInstance ); \
        static Void _Reflect_Member_Setter_ ## _member( Void * pInstance, Void * pValue ); \
        static _Reflection::MemberDescriptor _Reflect_Member_ ## _member;

#define ReflectDefineMember( _class, _member, _type, _access ) \
    Void * _class::_Reflect_Member_Getter_ ## _member( Void * pInstance ) { \
        static _Reflection::Holder<_type> s_hRetVal; \
	    s_hRetVal = ((_class*)pInstance)->_member; \
        return &s_hRetVal; \
    } \
    Void _class::_Reflect_Member_Setter_ ## _member( Void * pInstance, Void * pValue ) { \
        _Reflection::Holder<_type> * pArg = (_Reflection::Holder<_type>*)pValue; \
	    ((_class*)pInstance)->_member = *pArg; \
    } \
    _Reflection::MemberDescriptor _class::_Reflect_Member_ ## _member ( \
        _Reflection::_Builder_Member ( \
            TEXT(#_member), SPECIFIER_SCOPE_NESTED, _access, SPECIFIER_STORAGE_DEFAULT, \
            sizeof(_type), _class::_Reflect_Member_Getter_ ## _member, _class::_Reflect_Member_Setter_ ## _member, \
            _class::_Reflect_StaticGet(), TEXT(#_type) \
        ) \
    );

#define ReflectDefineMemberStatic( _class, _member, _type, _access ) \
    Void * _class::_Reflect_Member_Getter_ ## _member( Void * pInstance ) { \
        static _Reflection::Holder<_type> s_hRetVal; \
        s_hRetVal = _class::_member; \
        return &s_hRetVal; \
    } \
    Void _class::_Reflect_Member_Setter_ ## _member( Void * pInstance, Void * pValue ) { \
        _Reflection::Holder<_type> * pArg = (_Reflection::Holder<_type>*)pValue; \
	    _class::_member = *pArg; \
    } \
    _Reflection::MemberDescriptor _class::_Reflect_Member_ ## _member ( \
        _Reflection::_Builder_Member ( \
            TEXT(#_member), SPECIFIER_SCOPE_NESTED, _access, SPECIFIER_STORAGE_STATIC, \
            sizeof(_type), _class::_Reflect_Member_Getter_ ## _member, _class::_Reflect_Member_Setter_ ## _member, \
            _class::_Reflect_StaticGet(), TEXT(#_type) \
        ) \
    );

/////////////////////////////////////////////////////////////////////////////////
// Method interface
#define ReflectDeclareMethod( _method ) \
    protected: \
        static Void * _Reflect_Method_Invoke_ ## _method( Void * pInstance, Void ** arrArgV, UInt iArgC ); \
        static _Reflection::FunctionDescriptor _Reflect_Function_ ## _method; \
        static _Reflection::MethodDescriptor _Reflect_Method_ ## _method;

#define ReflectDefineMethod_0( _class, _method, _funcmod, _methodtype, _methodtypemod, _access, _storage ) \
    Void * _class::_Reflect_Method_Invoke_ ## _method( Void * pInstance, Void **, UInt ) { \
        ((_class*)pInstance)->_method(); \
        return NULL; \
	} \
    static _Reflection::ModTypeDescriptor _Reflect_Method_arrReturnArguments_ ## _class ## _ ## _method [] = { \
        _Reflection::ModTypeDescriptor(), \
        _Reflection::ModTypeDescriptor() \
    }; \
    _Reflection::FunctionDescriptor _class::_Reflect_Function_ ## _method ( \
        _Reflection::_Builder_Type ( \
            TEXT(#_method), SPECIFIER_TYPE_FUNCTION, SPECIFIER_TYPEMOD_NONE, SPECIFIER_SCOPE_NESTED, \
            SPECIFIER_ACCESS_PROTECTED, 4, NULL, NULL, _class::_Reflect_StaticGet() \
        ), _funcmod, \
        _Reflect_Method_arrReturnArguments_ ## _class ## _ ## _method \
    ); \
    _Reflection::MethodDescriptor _class::_Reflect_Method_ ## _method ( \
        _Reflection::_Builder_Method ( \
            TEXT(#_method), _methodtype, _methodtypemod, SPECIFIER_SCOPE_NESTED, _access, _storage, \
            _class::_Reflect_Method_Invoke_ ## _method, \
            _class::_Reflect_StaticGet(), &(_class::_Reflect_Function_ ## _method) \
        ) \
    );

#define ReflectDefineMethod_1( _class, _method, _funcmod, _methodtype, _methodtypemod, _access, _storage, \
                               _arg0 ) \
    Void * _class::_Reflect_Method_Invoke_ ## _method( Void * pInstance, Void ** arrArgV, UInt ) { \
        _Reflection::Holder<_arg0> * pArg0 = (_Reflection::Holder<_arg0>*)(arrArgV[0]); \
        ((_class*)pInstance)->_method( *pArg0 ); \
        return NULL; \
	} \
    static _Reflection::ModTypeDescriptor _Reflect_Method_arrReturnArguments_ ## _class ## _ ## _method [] = { \
        _Reflection::ModTypeDescriptor(), _Reflection::ModTypeDescriptor(TEXT(#_arg0)), \
        _Reflection::ModTypeDescriptor() \
    }; \
    _Reflection::FunctionDescriptor _class::_Reflect_Function_ ## _method ( \
        _Reflection::_Builder_Type ( \
            TEXT(#_method), SPECIFIER_TYPE_FUNCTION, SPECIFIER_TYPEMOD_NONE, SPECIFIER_SCOPE_NESTED, \
            SPECIFIER_ACCESS_PROTECTED, 4, NULL, NULL, _class::_Reflect_StaticGet() \
        ), _funcmod, \
        _Reflect_Method_arrReturnArguments_ ## _class ## _ ## _method \
    ); \
    _Reflection::MethodDescriptor _class::_Reflect_Method_ ## _method ( \
        _Reflection::_Builder_Method ( \
            TEXT(#_method), _methodtype, _methodtypemod, SPECIFIER_SCOPE_NESTED, _access, _storage, \
            _class::_Reflect_Method_Invoke_ ## _method, \
            _class::_Reflect_StaticGet(), &(_class::_Reflect_Function_ ## _method) \
        ) \
    );

/////////////////////////////////////////////////////////////////////////////////

#define ReflectDefineMethod_Ret_0( _class, _method, _funcmod, _methodtype, _methodtypemod, _access, _storage, \
                               _return ) \
    Void * _class::_Reflect_Method_Invoke_ ## _method( Void * pInstance, Void **, UInt ) { \
        static _Reflection::Holder<_return> s_hRetVal; \
        s_hRetVal = ((_class*)pInstance)->_method(); \
        return &s_hRetVal; \
	} \
    static _Reflection::ModTypeDescriptor _Reflect_Method_arrReturnArguments_ ## _class ## _ ## _method [] = { \
        _Reflection::ModTypeDescriptor(TEXT(#_return)), \
        _Reflection::ModTypeDescriptor() \
    }; \
    _Reflection::FunctionDescriptor _class::_Reflect_Function_ ## _method ( \
        _Reflection::_Builder_Type ( \
            TEXT(#_method), SPECIFIER_TYPE_FUNCTION, SPECIFIER_TYPEMOD_NONE, SPECIFIER_SCOPE_NESTED, \
            SPECIFIER_ACCESS_PROTECTED, 4, NULL, NULL, _class::_Reflect_StaticGet() \
        ), _funcmod, \
        _Reflect_Method_arrReturnArguments_ ## _class ## _ ## _method \
    ); \
    _Reflection::MethodDescriptor _class::_Reflect_Method_ ## _method ( \
        _Reflection::_Builder_Method ( \
            TEXT(#_method), _methodtype, _methodtypemod, SPECIFIER_SCOPE_NESTED, _access, _storage, \
            _class::_Reflect_Method_Invoke_ ## _method, \
            _class::_Reflect_StaticGet(), &(_class::_Reflect_Function_ ## _method) \
        ) \
    );

#define ReflectDefineMethod_Ret_1( _class, _method, _funcmod, _methodtype, _methodtypemod, _access, _storage, \
                               _return, _arg0 ) \
    Void * _class::_Reflect_Method_Invoke_ ## _method( Void * pInstance, Void ** arrArgV, UInt ) { \
        static _Reflection::Holder<_return> s_hRetVal; \
        _Reflection::Holder<_arg0> * pArg0 = (_Reflection::Holder<_arg0>*)(arrArgV[0]); \
        s_hRetVal = ((_class*)pInstance)->_method( *pArg0 ); \
        return &s_hRetVal; \
	} \
    static _Reflection::ModTypeDescriptor _Reflect_Method_arrReturnArguments_ ## _class ## _ ## _method [] = { \
        _Reflection::ModTypeDescriptor(TEXT(#_return)), _Reflection::ModTypeDescriptor(TEXT(#_arg0)), \
        _Reflection::ModTypeDescriptor() \
    }; \
    _Reflection::FunctionDescriptor _class::_Reflect_Function_ ## _method ( \
        _Reflection::_Builder_Type ( \
            TEXT(#_method), SPECIFIER_TYPE_FUNCTION, SPECIFIER_TYPEMOD_NONE, SPECIFIER_SCOPE_NESTED, \
            SPECIFIER_ACCESS_PROTECTED, 4, NULL, NULL, _class::_Reflect_StaticGet() \
        ), _funcmod, \
        _Reflect_Method_arrReturnArguments_ ## _class ## _ ## _method \
    ); \
    _Reflection::MethodDescriptor _class::_Reflect_Method_ ## _method ( \
        _Reflection::_Builder_Method ( \
            TEXT(#_method), _methodtype, _methodtypemod, SPECIFIER_SCOPE_NESTED, _access, _storage, \
            _class::_Reflect_Method_Invoke_ ## _method, \
            _class::_Reflect_StaticGet(), &(_class::_Reflect_Function_ ## _method) \
        ) \
    );

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
//#include "Macros.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_OBJECT_REFLECTION_MACROS_H
