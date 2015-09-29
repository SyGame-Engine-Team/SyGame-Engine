/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Object/Reflection/Type.inl
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
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// BEGIN : _Reflection implementation
namespace _Reflection {

/////////////////////////////////////////////////////////////////////////////////
// _Reflection::TypeDescriptor implementation
inline TypeDescriptor * TypeDescriptor::GetByName(const GChar * strName) {
    return sm_htTypes.Get(strName);
}

inline const GChar * TypeDescriptor::Name() const {
    return m_strRelativeName;
}
inline const GChar * TypeDescriptor::FullName() const {
    return m_strAbsoluteName;
}

inline SpecifierType TypeDescriptor::SpecType() const {
    return m_specType;
}
inline SpecifierTypeMod TypeDescriptor::SpecTypeMod() const {
    return m_specTypeMod;
}
inline SpecifierScope TypeDescriptor::SpecScope() const {
    return m_specScope;
}
inline SpecifierAccess TypeDescriptor::SpecAccess() const {
    return m_specAccess;
}
inline UInt TypeDescriptor::Size() const {
    return m_iBytes;
}

template<typename T>
inline T * TypeDescriptor::Cast( Void * pInstance, UInt iParent ) const {
    return (T*)( m_pfCast( pInstance, iParent ) );
}
template<typename T>
inline T * TypeDescriptor::Instanciate() const {
    return (T*)( m_pfNew() );
}

/////////////////////////////////////////////////////////////////////////////////
// _Reflection::ModTypeDescriptor implementation
inline ModTypeDescriptor & ModTypeDescriptor::operator=(const ModTypeDescriptor & rhs) {
    if (this == &rhs)
        return (*this);
    m_iMember = rhs.m_iMember;
    m_iMemberMod = rhs.m_iMemberMod;
    m_iIndirectionLevel = rhs.m_iIndirectionLevel;
    m_pType = rhs.m_pType;
    return (*this);
}

inline SpecifierMember ModTypeDescriptor::SpecKind() const {
    return m_iMember;
}
inline SpecifierMemberMod ModTypeDescriptor::SpecModifier() const {
    return m_iMemberMod;
}
inline UInt ModTypeDescriptor::IndirectionLevel() const {
    return m_iIndirectionLevel;
}
inline const TypeDescriptor * ModTypeDescriptor::Type() const {
    return m_pType;
}

/////////////////////////////////////////////////////////////////////////////////
// END : _Reflection implementation
};

