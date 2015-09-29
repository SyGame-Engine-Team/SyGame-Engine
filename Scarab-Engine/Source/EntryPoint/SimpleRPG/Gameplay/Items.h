/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Gameplay/Items.h
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
// Header prelude
#ifndef SCARAB_ENTRYPOINT_SIMPLERPG_GAMEPLAY_ITEMS_H
#define SCARAB_ENTRYPOINT_SIMPLERPG_GAMEPLAY_ITEMS_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Engine/Application/ApplicationManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define ITEM_RATING_MAX 1024

// Item classification
enum ItemType {
    ITEMTYPE_UNDEFINED = 0,

    ITEMTYPE_TRASH,
    ITEMTYPE_CONSUMMABLE,
    ITEMTYPE_EQUIPMENT,
    ITEMTYPE_QUEST,

    ITEMTYPE_COUNT
};

enum ItemClass {
    ITEMCLASS_UNDEFINED = 0,

    ITEMCLASS_TRASH,

    ITEMCLASS_FOOD,
    ITEMCLASS_DRINK,
    ITEMCLASS_POTION,
    ITEMCLASS_THROWABLE,

    ITEMCLASS_HEAD,
    ITEMCLASS_CHEST,
    ITEMCLASS_SHOULDERS,
    ITEMCLASS_ARMS,
    ITEMCLASS_WRISTS,
    ITEMCLASS_HANDS,
    ITEMCLASS_LEGS,
    ITEMCLASS_FEET,

    ITEMCLASS_NECK,
    ITEMCLASS_BELT,
    ITEMCLASS_RING,
    ITEMCLASS_TRINKET,

    ITEMCLASS_WEAPON_RIGHT,
    ITEMCLASS_WEAPON_LEFT,
    ITEMCLASS_WEAPON_THROW,

    ITEMCLASS_QUEST,

    ITEMCLASS_COUNT
};

// Item list
enum ItemID {
    ITEMID_UNDEFINED = 0,

    ITEMID_WEAPON_FISTS,

    ITEMID_COUNT
};

// Item binding
enum ItemBindType {
    ITEMBIND_NONE = 0,
    ITEMBIND_ON_EQUIP,
    ITEMBIND_ON_PICK,

    ITEMBIND_COUNT
};

// Bag inventory
#define INVENTORY_BAG_SIZE 64

enum InventoryBagID {
    INVENTORYBAGID_TRASH = 0,
    INVENTORYBAGID_CONSUMMABLE,
    INVENTORYBAGID_EQUIPMENT,
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
    EQUIPSLOTID_BELT,
    EQUIPSLOTID_RING_1,
    EQUIPSLOTID_RING_2,
    EQUIPSLOTID_TRINKET_1,
    EQUIPSLOTID_TRINKET_2,

    EQUIPSLOTID_WEAPON_RIGHT,
    EQUIPSLOTID_WEAPON_LEFT,
    EQUIPSLOTID_WEAPON_THROW,

    EQUIPSLOTID_COUNT
};


// Prototypes
class BaseCharacter;

class ItemInstance;

/////////////////////////////////////////////////////////////////////////////////
// The Item class
class Item
{
public:
    Item();
    virtual ~Item();

    // Getters
    static Item * GetInstance( ItemID idItem );

    inline ItemType GetType() const;
    inline ItemClass GetClass() const;
    inline ItemID GetID() const;
    inline const GChar * GetName() const;

    inline UInt GetRating() const;

    inline ItemBindType GetBindType() const;

    inline Bool IsStackable() const;
    inline UInt GetStackMax() const;

    inline Bool IsUsable() const;
    inline Bool IsConsumedOnUse() const;

    // Methods
    Void GenerateInstance( ItemInstance * outItemInstance, BaseCharacter * pOwner, UInt iStackCount );

    virtual Void OnUse( ItemInstance * pInstance ) const = 0;

protected:
    ItemType m_iType;
    ItemClass m_iClass;
    ItemID m_iItemID;
    GChar m_strName[64];

    UInt m_iRating;

    // Bounding
    ItemBindType m_iBindType;

    // Stacking
    UInt m_iStackMax;

    // Using
    Bool m_bUsable;
    Bool m_bConsumedOnUse;

    //Scalar m_fUsageCooldown;
};

/////////////////////////////////////////////////////////////////////////////////
// The ItemInstance class
class ItemInstance
{
public:
    ItemInstance();
    virtual ~ItemInstance();

    inline ItemInstance & operator=( const ItemInstance & rhs );

    // Getters
    inline Item * GetItem() const;
    inline BaseCharacter * GetOwner() const;
    inline BaseCharacter * GetTarget() const;

    inline ItemType GetType() const;
    inline ItemClass GetClass() const;
    inline ItemID GetID() const;
    inline const GChar * GetName() const;

    inline UInt GetRating() const;

    inline ItemBindType GetBindType() const;
    inline Bool IsBound() const;
    inline BaseCharacter * GetBind() const;

    inline Bool IsEmpty() const;
    inline Bool IsStackable() const;
    inline UInt GetStackCount() const;
    inline UInt GetStackMax() const;

    inline Bool IsUsable() const;
    inline Bool IsConsumedOnUse() const;

    // Methods
    inline Bool IncreaseStack();
    inline Bool DecreaseStack();

    inline Void Bind();
    inline Void Use( BaseCharacter * pTarget );

    // Update routine
    Bool Update( Scalar fTimeStep );

protected:
    friend class Item;

    Item * m_pItem;
    BaseCharacter * m_pOwner;
    BaseCharacter * m_pTarget;

    // Bounding
    BaseCharacter * m_pBound;

    // Stacking
    UInt m_iStackCount;
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
    Void SwapItems( InventoryBagID idBagA, UInt iSlotA, InventoryBagID idBagB, UInt iSlotB );

    UInt PickItem( Item * pItem, UInt iAmount = 1 );
    Void DropItem( InventoryBagID idBag, UInt iSlot );
    ItemInstance * EquipItem( EquipmentSoltID idEquipSlot, InventoryBagID idBag, UInt iSlot );
    ItemInstance * UnEquipItem( EquipmentSoltID idEquipSlot );

    ItemInstance * UseItem( BaseCharacter * pTarget, InventoryBagID idBag, UInt iSlot );
    ItemInstance * UseItem( BaseCharacter * pTarget, EquipmentSoltID idEquipSlot );

    Void SortBag( InventoryBagID idBag );
    Void SortAllBags();
    Void SortBufferBagItems();

protected:
    BaseCharacter * m_pOwner;

    typedef struct _inventory_bag {
        UInt iFreeSlots;
        ItemInstance arrSlots[INVENTORY_BAG_SIZE];
    } _InventoryBag;

    UInt _Bag_GetFreeSlot( InventoryBagID idBag ) const;

    UInt m_iTotalFreeSlots;
    _InventoryBag m_arrBags[INVENTORYBAGID_COUNT];
    ItemInstance m_arrEquipmentSlots[EQUIPSLOTID_COUNT];
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
// Backward Includes (Inlines & Templates)
#include "Items.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_SIMPLERPG_GAMEPLAY_ITEMS_H

