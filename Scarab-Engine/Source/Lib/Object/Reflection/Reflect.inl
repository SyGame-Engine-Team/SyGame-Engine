/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Object/Reflection/Reflect.inl
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
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// ReflectionInterface implementation
inline ReflectionInterface * ReflectionInterface::GetInstancePtr() {
    static ReflectionInterface s_Instance;
    return &s_Instance;
}

template<typename T>
inline Type * ReflectionInterface::GetType() const {
    return T::_Reflect_StaticGet();
}
template<typename T>
inline Type * ReflectionInterface::GetType(const T * pObject) const {
    return pObject->_Reflect_VirtualGet();
}
template<typename T>
inline Type * ReflectionInterface::GetType(const T & rObject) const {
    return rObject._Reflect_VirtualGet();
}

inline Type * ReflectionInterface::GetType(const GChar * strName) const {
    return Type::GetByName(strName);
}

template<typename TO, typename FROM>
inline TO * ReflectionInterface::Cast( FROM * pObject ) const {
    return (TO*)( pObject->_Reflect_VirtualCast(TO::_Reflect_StaticGet()) );
}
template<typename TO, typename FROM>
inline TO & ReflectionInterface::Cast( FROM & rObject ) const {
    return (TO&)( *( (TO*)( rObject._Reflect_VirtualCast(TO::_Reflect_StaticGet()) ) ) );
}

template<typename FROM>
inline Void * ReflectionInterface::Cast( const Type * pTarget, FROM * pObject ) const {
    return pObject->_Reflect_VirtualCast( pTarget );
}
