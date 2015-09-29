/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Object/Patterns/VirtualHolder.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Generic interface for statically allocated abstract classes
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// VirtualHolder implementation
template<typename T, UInt VH_MAX_SIZE>
VirtualHolder<T,VH_MAX_SIZE>::VirtualHolder()
{
    m_pInstance = NULL;
}
template<typename T, UInt VH_MAX_SIZE>
VirtualHolder<T,VH_MAX_SIZE>::~VirtualHolder()
{
    // nothing to do
}

template<typename T, UInt VH_MAX_SIZE>
    template<typename DerivedT>
inline Void VirtualHolder<T,VH_MAX_SIZE>::Instanciate() {
    Assert( sizeof(DerivedT) <= VH_MAX_SIZE );
    m_pInstance = (T*)( new(m_arrInstanceBytes) DerivedT() );
}
template<typename T, UInt VH_MAX_SIZE>
    template<typename DerivedT>
inline Void VirtualHolder<T,VH_MAX_SIZE>::Instanciate( const DerivedT & rhs ) {
    Assert( sizeof(DerivedT) <= VH_MAX_SIZE );
    m_pInstance = (T*)( new(m_arrInstanceBytes) DerivedT(rhs) );
}

template<typename T, UInt VH_MAX_SIZE>
inline T & VirtualHolder<T,VH_MAX_SIZE>::GetInstance() {
    return *m_pInstance;
}
template<typename T, UInt VH_MAX_SIZE>
inline const T & VirtualHolder<T,VH_MAX_SIZE>::GetInstance() const {
    return *m_pInstance;
}

template<typename T, UInt VH_MAX_SIZE>
    template<typename DerivedT>
inline DerivedT & VirtualHolder<T,VH_MAX_SIZE>::GetDerivedInstance() {
    Assert(m_pInstance != NULL);
    return *( (DerivedT*)m_pInstance );
}
template<typename T, UInt VH_MAX_SIZE>
    template<typename DerivedT>
inline const DerivedT & VirtualHolder<T,VH_MAX_SIZE>::GetDerivedInstance() const {
    Assert(m_pInstance != NULL);
    return *( (DerivedT*)m_pInstance );
}
