/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Object/Reflection/Definitions.inl
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
// BEGIN : _Reflection implementation
namespace _Reflection {

/////////////////////////////////////////////////////////////////////////////////
// _Reflection::LookupTable implementation
template<typename T, UInt SIZE>
LookupTable<T,SIZE>::LookupTable()
{
    for(UInt i = 0; i < SIZE; ++i) {
        m_arrItems[i].dwHash = 0;
        m_arrItems[i].strName = NULL;
        m_arrItems[i].pItem = NULL;
    }
}
template<typename T, UInt SIZE>
LookupTable<T,SIZE>::~LookupTable()
{
    // nothing to do
}

template<typename T, UInt SIZE>
inline T * LookupTable<T,SIZE>::Get( const GChar * strName ) const {
    return m_arrItems[ _Index( _Hash( strName ) ) ].pItem;
}
template<typename T, UInt SIZE>
const GChar * LookupTable<T,SIZE>::Insert( T * pItem )
{
    const GChar * strName = pItem->FullName();
    DWord dwHash = _Hash( strName );
    UInt iIndex = _Index( dwHash );
    m_arrItems[iIndex].dwHash = dwHash;
    m_arrItems[iIndex].strName = strName;
    m_arrItems[iIndex].pItem = pItem;
    return strName;
}
template<typename T, UInt SIZE>
Void LookupTable<T,SIZE>::Remove( const GChar * strName )
{
    UInt iIndex = _Index( _Hash( strName ) );
    m_arrItems[iIndex].dwHash = INVALID_OFFSET;
    m_arrItems[iIndex].strName = NULL;
    m_arrItems[iIndex].pItem = NULL;
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T, UInt SIZE>
inline DWord LookupTable<T,SIZE>::_Hash( const GChar * strName ) const {
    return HashFn->BobJenkinsRot( strName );
}
template<typename T, UInt SIZE>
UInt LookupTable<T,SIZE>::_Index(DWord dwHash) const
{
    UInt iHash = dwHash & (SIZE - 1);
    UInt iIndex;

    // collisions, if any
    UInt i = 0;
    Int iBias = -1;
    while(true) {
        iIndex = ( (iHash + (4*iBias*i*i)) & (SIZE - 1) );
        if ( m_arrItems[iIndex].dwHash == 0 )
            return iIndex;
        if ( m_arrItems[iIndex].dwHash == dwHash )
            return iIndex;
        iBias = -iBias;
        if (iBias > 0)
            ++i;
    }
}

/////////////////////////////////////////////////////////////////////////////////
// END : _Reflection implementation
};

