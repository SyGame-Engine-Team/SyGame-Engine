/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/Gameplay/Items/Inventory.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : WorldCraft, Gameplay : Inventory
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// LootStash implementation

/////////////////////////////////////////////////////////////////////////////////
// Inventory implementation
inline ItemInstance * Inventory::GetItem( InventoryBagID idBag, UInt iSlot ) {
    Assert( idBag < INVENTORYBAGID_COUNT && iSlot < INVENTORY_BAG_SIZE );
    return m_arrBags[idBag].arrSlots[iSlot];
}
inline ItemInstance * Inventory::GetItem( EquipmentSoltID idEquipSlot ) {
    Assert( idEquipSlot < EQUIPSLOTID_COUNT );
    return m_arrEquipmentSlots[idEquipSlot];
}

inline Void Inventory::SwapItems( InventoryBagID idBagA, UInt iSlotA, InventoryBagID idBagB, UInt iSlotB ) {
    Assert( idBagA < INVENTORYBAGID_COUNT && iSlotA < INVENTORY_BAG_SIZE );
    Assert( idBagB < INVENTORYBAGID_COUNT && iSlotB < INVENTORY_BAG_SIZE );
    ItemInstance * pSwap = m_arrBags[idBagA].arrSlots[iSlotA];
    m_arrBags[idBagA].arrSlots[iSlotA] = m_arrBags[idBagB].arrSlots[iSlotB];
    m_arrBags[idBagB].arrSlots[iSlotB] = pSwap;
}

