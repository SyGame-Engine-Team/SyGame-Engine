/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Object/Reflection/Member.inl
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
// BEGIN : _Reflection implementation
namespace _Reflection {

/////////////////////////////////////////////////////////////////////////////////
// _Reflection::MemberDescriptor implementation
inline const GChar * MemberDescriptor::Name() const {
    return m_strRelativeName;
}
inline const GChar * MemberDescriptor::FullName() const {
    return m_strAbsoluteName;
}

inline SpecifierMember MemberDescriptor::SpecMember() const {
    return m_ModType.SpecKind();
}
inline SpecifierMemberMod MemberDescriptor::SpecMemberMod() const {
    return m_ModType.SpecModifier();
}
inline SpecifierScope MemberDescriptor::SpecScope() const {
    return m_specScope;
}
inline SpecifierAccess MemberDescriptor::SpecAccess() const {
    return m_specAccess;
}
inline SpecifierStorage MemberDescriptor::SpecStorage() const {
    return m_specStorage;
}
inline UInt MemberDescriptor::Size() const {
    return m_iBytes;
}
inline UInt MemberDescriptor::IndirectionLevel() const {
    return m_ModType.IndirectionLevel();
}

inline const TypeDescriptor * MemberDescriptor::Type() const {
    return m_ModType.Type();
}

template<typename T>
inline T MemberDescriptor::Get( Void * pInstance ) const {
    Holder<T> * pValue = (Holder<T>*)m_pfGet(pInstance);
    return *pValue;
}
template<typename T>
inline Void MemberDescriptor::Set( Void * pInstance, T value ) const {
    Holder<T> * pValue = (Holder<T>*)( &value );
    m_pfSet( pInstance, pValue );
}

/////////////////////////////////////////////////////////////////////////////////
// END : _Reflection implementation
};

