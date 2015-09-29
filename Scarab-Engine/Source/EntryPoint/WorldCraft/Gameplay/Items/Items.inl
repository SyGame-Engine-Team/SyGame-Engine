/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/Gameplay/Items/Items.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : WorldCraft, Gameplay : Items
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Item implementation
inline DWord Item::GetTypeBaseID( ItemType iType ) {
    Assert( iType < ITEMTYPE_COUNT );
    return sm_arrTypeIDDomains[iType].dwBaseID;
}
inline DWord Item::GetTypeDomainCapacity( ItemType iType ) {
    Assert( iType < ITEMTYPE_COUNT );
    return sm_arrTypeIDDomains[iType].dwCapacity;
}
inline DWord Item::GetClassBaseID( ItemClass iClass ) {
    Assert( iClass < ITEMCLASS_COUNT );
    return sm_arrClassIDDomains[iClass].dwBaseID;
}
inline DWord Item::GetClassDomainCapacity( ItemClass iClass ) {
    Assert( iClass < ITEMCLASS_COUNT );
    return sm_arrClassIDDomains[iClass].dwCapacity;
}

inline ItemID Item::MakeID( ItemClass iClass, DWord dwLocalID ) {
    Assert( iClass < ITEMCLASS_COUNT );
    DWord dwBaseID = sm_arrClassIDDomains[iClass].dwBaseID;
    Assert( dwLocalID < sm_arrClassIDDomains[iClass].dwCapacity );
    return (ItemID)( dwBaseID | dwLocalID );
}

inline ItemType Item::GetType() const {
    return m_iType;
}
inline ItemClass Item::GetClass() const {
    return m_iClass;
}
inline ItemID Item::GetID() const {
    return m_iItemID;
}

inline const GChar * Item::GetName() const {
    return m_strName;
}
inline ItemBindType Item::GetBindType() const {
    return m_iBindType;
}
inline UInt Item::GetRating() const {
    return m_iRating;
}

inline Bool Item::IsStackable() const {
    return ( m_iStackMax > 1 );
}
inline UInt Item::GetStackMax() const {
    return m_iStackMax;
}

/////////////////////////////////////////////////////////////////////////////////
// ItemInstance implementation
inline ItemInstance & ItemInstance::operator=( const ItemInstance & rhs ) {
    m_pItem = rhs.m_pItem;
    m_pOwner = rhs.m_pOwner;
    m_pBound = rhs.m_pBound;
    m_iStackCount = rhs.m_iStackCount;
    return (*this);
}

inline const Item * ItemInstance::GetItem() const {
    return m_pItem;
}

inline ItemType ItemInstance::GetType() const {
    return m_pItem->GetType();
}
inline ItemClass ItemInstance::GetClass() const {
    return m_pItem->GetClass();
}
inline ItemID ItemInstance::GetID() const {
    return m_pItem->GetID();
}

inline const GChar * ItemInstance::GetName() const {
    return m_pItem->GetName();
}
inline UInt ItemInstance::GetRating() const {
    return m_pItem->GetRating();
}

inline BaseCharacter * ItemInstance::GetOwner() const {
    return m_pOwner;
}

inline ItemBindType ItemInstance::GetBindType() const {
    return m_pItem->GetBindType();
}
inline Bool ItemInstance::IsBound() const {
    return ( m_pBound != NULL );
}
inline BaseCharacter * ItemInstance::GetBind() const {
    return m_pBound;
}
inline Void ItemInstance::Bind() {
    if ( m_pBound == NULL )
        m_pBound = m_pOwner;
}

inline Bool ItemInstance::IsStackable() const {
    return m_pItem->IsStackable();
}
inline UInt ItemInstance::GetStackMax() const {
    return m_pItem->GetStackMax();
}
inline Bool ItemInstance::IsEmpty() const {
    return ( m_iStackCount == 0 );
}
inline UInt ItemInstance::GetStackCount() const {
    return m_iStackCount;
}
inline Bool ItemInstance::IncreaseStack() {
    if ( m_iStackCount >= m_pItem->GetStackMax() )
        return false;
    ++m_iStackCount;
    return true;
}
inline Bool ItemInstance::DecreaseStack() {
    if ( m_iStackCount == 0 )
        return false;
    --m_iStackCount;
    return true;
}



