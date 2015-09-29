/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Object/Reflection/Method.inl
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
// BEGIN : _Reflection implementation
namespace _Reflection {

/////////////////////////////////////////////////////////////////////////////////
// _Reflection::MethodDescriptor implementation
inline const GChar * MethodDescriptor::Name() const {
    return m_strRelativeName;
}
inline const GChar * MethodDescriptor::FullName() const {
    return m_strAbsoluteName;
}

inline SpecifierMethod MethodDescriptor::SpecMethod() const {
    return m_specMethod;
}
inline SpecifierMethodMod MethodDescriptor::SpecMethodMod() const {
    return m_specMethodMod;
}
inline SpecifierScope MethodDescriptor::SpecScope() const {
    return m_specScope;
}
inline SpecifierAccess MethodDescriptor::SpecAccess() const {
    return m_specAccess;
}
inline SpecifierStorage MethodDescriptor::SpecStorage() const {
    return m_specStorage;
}

inline const FunctionDescriptor * MethodDescriptor::Type() const {
    return m_pType;
}

inline Void MethodDescriptor::Invoke( Void * pInstance ) const {
    m_pfInvoke(pInstance, NULL, 0);
}
template<typename T>
inline T MethodDescriptor::Invoke( Void * pInstance ) const {
    Holder<T> * pRetValue = (Holder<T>*)( m_pfInvoke(pInstance, NULL, 0) );
    return *pRetValue;
}
template<UInt ARGCOUNT>
inline Void MethodDescriptor::Invoke( Void * pInstance, ... ) const {
    static Void * s_arrArgument[ARGCOUNT];
    VArgPtr pArgs;
    VArg_Init(pArgs, pInstance);
    for(UInt i = 0; i < ARGCOUNT; ++i)
        s_arrArgument[i] = VArg_Get(pArgs, Void*);
    VArg_Destroy(pArgs);

    m_pfInvoke(pInstance, s_arrArgument, ARGCOUNT);
}

template<typename T, UInt ARGCOUNT>
inline T MethodDescriptor::Invoke( Void * pInstance, ... ) const {
    static Void * s_arrArgument[ARGCOUNT];
    VArgPtr pArgs;
    VArg_Init(pArgs, pInstance);
    for(UInt i = 0; i < ARGCOUNT; ++i)
        s_arrArgument[i] = VArg_Get(pArgs, Void*);
    VArg_Destroy(pArgs);

    Holder<T> * pRetValue = (Holder<T>*)( m_pfInvoke(pInstance, s_arrArgument, ARGCOUNT) );
    return *pRetValue;
}

/////////////////////////////////////////////////////////////////////////////////
// END : _Reflection implementation
};

