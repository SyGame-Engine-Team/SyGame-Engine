/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Object/Reflection/Class.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Class descriptor.
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
// _Reflection::ClassDescriptor implementation
inline SpecifierClassMod ClassDescriptor::SpecClassMod() const {
    return m_specClassMod;
}

inline Bool ClassDescriptor::CanCastFrom(const ClassDescriptor * pType) const {
    return ( IsEqual(pType) || pType->HeritsFrom(this) );
}

template<typename T>
T * ClassDescriptor::Instanciate(const ClassDescriptor * pTarget, const GChar * strName) const
{
    Void * pPtr = NULL;
    UInt i;
    if ( StrCmpN(m_strRelativeName, strName, REFLECTION_STRINGSIZE) == 0 )
        pPtr = New<T>();
    else {
        for(i = 0; i < m_iChildsCount; ++i) {
            pPtr = m_arrChilds[i]->Instanciate<T>( this, strName );
            if (pPtr != NULL)
                break;
        }
    }
    if (pPtr == NULL)
        return NULL;
    if (this == pTarget)
        i = INVALID_OFFSET;
    else {
        for(i = 0; i < m_iParentsCount; ++i) {
            if (m_arrParents[i] == pTarget)
                break;
        }
    }
    return Cast<T>(pPtr, i);
}

/////////////////////////////////////////////////////////////////////////////////

inline Void ClassDescriptor::_AddChild(ClassDescriptor * pChild) {
    m_arrChilds[m_iChildsCount++] = pChild;
}
inline Void ClassDescriptor::_AddNestedChild(TypeDescriptor * pNestedChild) {
    m_arrNestedChilds[m_iNestedChildsCount++] = pNestedChild;
}
inline Void ClassDescriptor::_AddMember(MemberDescriptor * pMember) {
    m_arrMembers[m_iMembersCount++] = pMember;
}
inline Void ClassDescriptor::_AddMethod(MethodDescriptor * pMethod) {
    m_arrMethods[m_iMethodsCount++] = pMethod;
}

/////////////////////////////////////////////////////////////////////////////////
// END : _Reflection implementation
};

