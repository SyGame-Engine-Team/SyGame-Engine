/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Gameplay/Items.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Gameplay : Items
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

inline Bool Item::IsUsable() const {
    return m_bUsable;
}
inline Bool Item::IsConsumedOnUse() const {
    return m_bConsumedOnUse;
}

/////////////////////////////////////////////////////////////////////////////////
// ItemInstance implementation
inline ItemInstance & ItemInstance::operator=( const ItemInstance & rhs ) {
    m_pItem = rhs.m_pItem;
    m_pOwner = rhs.m_pOwner;
    m_pTarget = rhs.m_pTarget;
    m_pBound = rhs.m_pBound;
    m_iStackCount = rhs.m_iStackCount;
    return (*this);
}

inline Item * ItemInstance::GetItem() const {
    return m_pItem;
}
inline BaseCharacter * ItemInstance::GetOwner() const {
    return m_pOwner;
}
inline BaseCharacter * ItemInstance::GetTarget() const {
    return m_pTarget;
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

inline ItemBindType ItemInstance::GetBindType() const {
    return m_pItem->GetBindType();
}
inline Bool ItemInstance::IsBound() const {
    return ( m_pBound != NULL );
}
inline BaseCharacter * ItemInstance::GetBind() const {
    return m_pBound;
}

inline Bool ItemInstance::IsEmpty() const {
    return ( m_iStackCount == 0 );
}
inline Bool ItemInstance::IsStackable() const {
    return m_pItem->IsStackable();
}
inline UInt ItemInstance::GetStackCount() const {
    return m_iStackCount;
}
inline UInt ItemInstance::GetStackMax() const {
    return m_pItem->GetStackMax();
}

inline Bool ItemInstance::IsUsable() const {
    return m_pItem->IsUsable();
}
inline Bool ItemInstance::IsConsumedOnUse() const {
    return m_pItem->IsConsumedOnUse();
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

inline Void ItemInstance::Bind() {
    if ( m_pBound == NULL )
        m_pBound = m_pOwner;
}
inline Void ItemInstance::Use( BaseCharacter * pTarget ) {
    if ( m_pItem->IsUsable() && m_iStackCount > 0 ) {
        m_pTarget = pTarget;
        m_pItem->OnUse( this );
        if ( m_pItem->IsConsumedOnUse() )
            --m_iStackCount;
        m_pTarget = NULL;
    }
}

/////////////////////////////////////////////////////////////////////////////////
// Inventory implementation
inline ItemInstance * Inventory::GetItem( InventoryBagID idBag, UInt iSlot ) {
    Assert( idBag < INVENTORYBAGID_COUNT && iSlot < INVENTORY_BAG_SIZE );
    return &( m_arrBags[idBag].arrSlots[iSlot] );
}
inline ItemInstance * Inventory::GetItem( EquipmentSoltID idEquipSlot ) {
    Assert( idEquipSlot < EQUIPSLOTID_COUNT );
    return &( m_arrEquipmentSlots[idEquipSlot] );
}

