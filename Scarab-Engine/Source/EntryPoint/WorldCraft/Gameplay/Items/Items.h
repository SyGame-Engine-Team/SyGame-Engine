/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/Gameplay/Items/Items.h
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
// Header prelude
#ifndef SCARAB_ENTRYPOINT_WORLDCRAFT_GAMEPLAY_ITEMS_ITEMS_H
#define SCARAB_ENTRYPOINT_WORLDCRAFT_GAMEPLAY_ITEMS_ITEMS_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Graphics/GraphicsManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define ITEM_NAME_LENGTH 64
#define ITEM_RATING_MAX  1024

// Item classification
enum ItemType {
    ITEMTYPE_UNDEFINED = 0,

    ITEMTYPE_BLOCK,
    ITEMTYPE_EQUIPMENT,
    ITEMTYPE_CONSUMABLE,
    ITEMTYPE_QUEST,

    ITEMTYPE_COUNT
};
enum ItemClass {
    ITEMCLASS_UNDEFINED = 0,

    // Block type
    ITEMCLASS_SOLID,
    ITEMCLASS_LIQUID,
    ITEMCLASS_GAS,
    ITEMCLASS_PLASMA,

    // Equipment type
        // Armor subtype
    ITEMCLASS_HEAD,
    ITEMCLASS_CHEST,
    ITEMCLASS_SHOULDERS,
    ITEMCLASS_ARMS,
    ITEMCLASS_WRISTS,
    ITEMCLASS_HANDS,
    ITEMCLASS_LEGS,
    ITEMCLASS_FEET,
        // Accessory subtype
    ITEMCLASS_NECK,
    ITEMCLASS_BACK,
    ITEMCLASS_BELT,
    ITEMCLASS_RING,
    ITEMCLASS_TRINKET,
        // Tool subtype
    ITEMCLASS_TOOL_RIGHT,
    ITEMCLASS_TOOL_LEFT,
    ITEMCLASS_TOOL_DISTANCE,
        // Weapon subtype
    ITEMCLASS_WEAPON_RIGHT,
    ITEMCLASS_WEAPON_LEFT,
    ITEMCLASS_WEAPON_DISTANCE,

    // Consummable type
    ITEMCLASS_FOOD,
    ITEMCLASS_DRINK,
    ITEMCLASS_POTION,
    ITEMCLASS_PROJECTILE,

    // Quest type
    ITEMCLASS_QUEST,

    ITEMCLASS_COUNT
};

// Item identifiers
typedef DWord ItemID;
#define ITEMID_UNDEFINED 0

//ITEMID_BLOCK_STONE,
//ITEMID_LIQUID_WATER,
//
//ITEMID_DEBUG_HELM,
//ITEMID_DEBUG_CHESTPLATE,
//ITEMID_DEBUG_SHOULDERS,
//ITEMID_DEBUG_ELBOWS,
//ITEMID_DEBUG_BRACELETS,
//ITEMID_DEBUG_GLOVES,
//ITEMID_DEBUG_PANTS,
//ITEMID_DEBUG_SHOES,
//
//ITEMID_DEBUG_NECKLACE,
//ITEMID_DEBUG_CAPE,
//ITEMID_DEBUG_BELT,
//ITEMID_DEBUG_RING_1,
//ITEMID_DEBUG_RING_2,
//ITEMID_DEBUG_TRINKET_1,
//ITEMID_DEBUG_TRINKET_2,
//
//ITEMID_DEBUG_TOOL_RIGHT,
//ITEMID_DEBUG_TOOL_LEFT,
//ITEMID_DEBUG_TOOL_DISTANCE,
//ITEMID_DEBUG_WEAPON_RIGHT,
//ITEMID_DEBUG_WEAPON_LEFT,
//ITEMID_DEBUG_WEAPON_DISTANCE,
//
//ITEMID_DEBUG_FOOD,
//ITEMID_DEBUG_DRINK,
//ITEMID_DEBUG_POTION,
//ITEMID_DEBUG_SHURIKEN,

//ITEMID_DEBUG_QUESTSTART,
//ITEMID_DEBUG_QUESTOBJECTIVE,
//ITEMID_DEBUG_QUESTGOAL,

// Item properties
enum ItemBindType {
    ITEMBIND_NONE = 0,
    ITEMBIND_ON_USE,
    ITEMBIND_ON_EQUIP,
    ITEMBIND_ON_PICK,

    ITEMBIND_COUNT
};

// Prototypes
class BaseCharacter;

class Item;
class ItemInstance;

/////////////////////////////////////////////////////////////////////////////////
// The Item class
class Item
{
public:
    Item();
    virtual ~Item();

    // Item IDs
    inline static DWord GetTypeBaseID( ItemType iType );
    inline static DWord GetTypeDomainCapacity( ItemType iType );
    inline static DWord GetClassBaseID( ItemClass iClass );
    inline static DWord GetClassDomainCapacity( ItemClass iClass );

    inline static ItemID MakeID( ItemClass iClass, DWord dwLocalID );

    static ItemType GetTypeFromID( ItemID iItemID );
    static ItemClass GetClassFromID( ItemID iItemID );
    static DWord GetLocalID( ItemID iItemID, UInt * outClassDomainCapacity = NULL );

    // Getters
    inline ItemType GetType() const;
    inline ItemClass GetClass() const;
    inline ItemID GetID() const;

    inline const GChar * GetName() const;
    inline ItemBindType GetBindType() const;
    inline UInt GetRating() const;

    inline Bool IsStackable() const;
    inline UInt GetStackMax() const;

    // Instanciation
    ItemInstance * CreateInstance( BaseCharacter * pOwner, UInt iStackCount ) const;

protected:
    // Item IDs
    typedef struct _itemid_domain {
        DWord dwBaseID;
        DWord dwCapacity;
    } _ItemIDDomain;
    static _ItemIDDomain sm_arrTypeIDDomains[ITEMTYPE_COUNT];
    static _ItemIDDomain sm_arrClassIDDomains[ITEMCLASS_COUNT];

    // Item data
    ItemType m_iType;
    ItemClass m_iClass;
    ItemID m_iItemID;

    GChar m_strName[ITEM_NAME_LENGTH];
    ItemBindType m_iBindType;
    UInt m_iRating;

    UInt m_iStackMax;
};

/////////////////////////////////////////////////////////////////////////////////
// The ItemInstance class
class ItemInstance
{
public:
    ItemInstance( const Item * pItem, BaseCharacter * pOwner, UInt iStackCount );
    ItemInstance( const ItemInstance & rhs );
    virtual ~ItemInstance();

    inline ItemInstance & operator=( const ItemInstance & rhs );

    // Getters
    inline const Item * GetItem() const;

    inline ItemType GetType() const;
    inline ItemClass GetClass() const;
    inline ItemID GetID() const;

    inline const GChar * GetName() const;
    inline UInt GetRating() const;

    inline BaseCharacter * GetOwner() const;

    inline ItemBindType GetBindType() const;
    inline Bool IsBound() const;
    inline BaseCharacter * GetBind() const;
    inline Void Bind();

    inline Bool IsStackable() const;
    inline UInt GetStackMax() const;
    inline Bool IsEmpty() const;
    inline UInt GetStackCount() const;
    inline Bool IncreaseStack();
    inline Bool DecreaseStack();
    
    // Update routine
    Bool Update( Scalar fTimeStep );

protected:
    const Item * m_pItem;
    BaseCharacter * m_pOwner;
    BaseCharacter * m_pBound;

    // Stacking
    UInt m_iStackCount;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Items.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDCRAFT_GAMEPLAY_ITEMS_ITEMS_H

