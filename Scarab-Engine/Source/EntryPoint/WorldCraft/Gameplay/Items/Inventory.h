/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/Gameplay/Items/Inventory.h
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
// Header prelude
#ifndef SCARAB_ENTRYPOINT_WORLDCRAFT_GAMEPLAY_ITEMS_INVENTORY_H
#define SCARAB_ENTRYPOINT_WORLDCRAFT_GAMEPLAY_ITEMS_INVENTORY_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Items.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Bag inventory
#define INVENTORY_BAG_SIZE 64

enum InventoryBagID {
    INVENTORYBAGID_BLOCK = 0,
    INVENTORYBAGID_EQUIPMENT,
    INVENTORYBAGID_CONSUMABLE,
    INVENTORYBAGID_QUEST,
    INVENTORYBAGID_BUFFER,

    INVENTORYBAGID_COUNT
};

// Equipped inventory
enum EquipmentSoltID {
    EQUIPSLOTID_HEAD = 0,
    EQUIPSLOTID_CHEST,
    EQUIPSLOTID_SHOULDERS,
    EQUIPSLOTID_ARMS,
    EQUIPSLOTID_WRISTS,
    EQUIPSLOTID_HANDS,
    EQUIPSLOTID_LEGS,
    EQUIPSLOTID_FEET,

    EQUIPSLOTID_NECK,
    EQUIPSLOTID_BACK,
    EQUIPSLOTID_BELT,
    EQUIPSLOTID_RING_1,
    EQUIPSLOTID_RING_2,
    EQUIPSLOTID_TRINKET_1,
    EQUIPSLOTID_TRINKET_2,

    EQUIPSLOTID_TOOL_RIGHT,
    EQUIPSLOTID_TOOL_LEFT,
    EQUIPSLOTID_TOOL_DISTANCE,

    EQUIPSLOTID_WEAPON_RIGHT,
    EQUIPSLOTID_WEAPON_LEFT,
    EQUIPSLOTID_WEAPON_DISTANCE,

    EQUIPSLOTID_COUNT
};

/////////////////////////////////////////////////////////////////////////////////
// The LootStash class
class LootStash
{
public:
    LootStash();
    virtual ~LootStash();

protected:
};

/////////////////////////////////////////////////////////////////////////////////
// The Inventory class
class Inventory
{
public:
    Inventory( BaseCharacter * pOwner );
    virtual ~Inventory();

    // Getters
    inline ItemInstance * GetItem( InventoryBagID idBag, UInt iSlot );
    inline ItemInstance * GetItem( EquipmentSoltID idEquipSlot );

    // Methods
    inline Void SwapItems( InventoryBagID idBagA, UInt iSlotA, InventoryBagID idBagB, UInt iSlotB );

    ItemInstance * SetItem( InventoryBagID idBag, UInt iSlot, ItemInstance * pItemInstance );

    Bool PickItem( ItemInstance * pItemInstance );
    ItemInstance * DropItem( InventoryBagID idBag, UInt iSlot );
    ItemInstance * EquipItem( EquipmentSoltID idEquipSlot, InventoryBagID idBag, UInt iSlot );
    ItemInstance * UnEquipItem( EquipmentSoltID idEquipSlot );

    Void SortBag( InventoryBagID idBag );
    Void SortAllBags();
    Void SortBufferBagItems();

protected:
    BaseCharacter * m_pOwner;

    typedef struct _inventory_bag {
        UInt iFreeSlots;
        ItemInstance * arrSlots[INVENTORY_BAG_SIZE];
    } _InventoryBag;

    UInt _Bag_GetFreeSlot( InventoryBagID idBag ) const;

    UInt m_iTotalFreeSlots;
    _InventoryBag m_arrBags[INVENTORYBAGID_COUNT];
    ItemInstance * m_arrEquipmentSlots[EQUIPSLOTID_COUNT];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Inventory.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDCRAFT_GAMEPLAY_ITEMS_INVENTORY_H


