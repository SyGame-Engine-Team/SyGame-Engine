/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/Gameplay/Items/Items.cpp
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
// Includes
#include "Items.h"

#include "../GameplayManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Item implementation
Item::_ItemIDDomain Item::sm_arrTypeIDDomains[ITEMTYPE_COUNT] = {
    { 0, 0 },                   // ITEMTYPE_UNDEFINED
    { 0x00000000, 0x40000000 }, // ITEMTYPE_BLOCK
    { 0x40000000, 0x40000000 }, // ITEMTYPE_EQUIPMENT
    { 0x80000000, 0x40000000 }, // ITEMTYPE_CONSUMABLE
    { 0xc0000000, 0x40000000 }  // ITEMTYPE_QUEST
};
Item::_ItemIDDomain Item::sm_arrClassIDDomains[ITEMCLASS_COUNT] = {
    { 0, 0 }, // ITEMCLASS_UNDEFINED
    // Block type
    { 0x00000000, 0x10000000 }, // ITEMCLASS_SOLID
    { 0x10000000, 0x10000000 }, // ITEMCLASS_LIQUID
    { 0x20000000, 0x10000000 }, // ITEMCLASS_GAS
    { 0x30000000, 0x10000000 }, // ITEMCLASS_PLASMA
    // Equipment type
        // Armor subtype
    { 0x40000000, 0x02000000 }, // ITEMCLASS_HEAD
    { 0x42000000, 0x02000000 }, // ITEMCLASS_CHEST
    { 0x44000000, 0x02000000 }, // ITEMCLASS_SHOULDERS
    { 0x46000000, 0x02000000 }, // ITEMCLASS_ARMS
    { 0x48000000, 0x02000000 }, // ITEMCLASS_WRISTS
    { 0x4a000000, 0x02000000 }, // ITEMCLASS_HANDS
    { 0x4c000000, 0x02000000 }, // ITEMCLASS_LEGS
    { 0x4e000000, 0x02000000 }, // ITEMCLASS_FEET
        // Accessory subtype
    { 0x50000000, 0x03000000 }, // ITEMCLASS_NECK
    { 0x53000000, 0x02000000 }, // ITEMCLASS_BACK
    { 0x55000000, 0x03000000 }, // ITEMCLASS_BELT
    { 0x58000000, 0x04000000 }, // ITEMCLASS_RING
    { 0x5c000000, 0x04000000 }, // ITEMCLASS_TRINKET
        // Tool subtype
    { 0x60000000, 0x08000000 }, // ITEMCLASS_TOOL_RIGHT
    { 0x68000000, 0x04000000 }, // ITEMCLASS_TOOL_LEFT
    { 0x6c000000, 0x04000000 }, // ITEMCLASS_TOOL_DISTANCE
        // Weapon subtype
    { 0x70000000, 0x08000000 }, // ITEMCLASS_WEAPON_RIGHT
    { 0x78000000, 0x04000000 }, // ITEMCLASS_WEAPON_LEFT
    { 0x7c000000, 0x04000000 }, // ITEMCLASS_WEAPON_DISTANCE
    // Consummable type
    { 0x80000000, 0x10000000 }, // ITEMCLASS_FOOD
    { 0x90000000, 0x10000000 }, // ITEMCLASS_DRINK
    { 0xa0000000, 0x10000000 }, // ITEMCLASS_POTION
    { 0xb0000000, 0x10000000 }, // ITEMCLASS_PROJECTILE
    // Quest type
    { 0xc0000000, 0x40000000 }, // ITEMCLASS_QUEST
};

Item::Item()
{
    m_iType = ITEMTYPE_UNDEFINED;
    m_iClass = ITEMCLASS_UNDEFINED;
    m_iItemID = ITEMID_UNDEFINED;

    m_strName[0] = NULLBYTE;
    m_iBindType = ITEMBIND_NONE;
    m_iRating = 0;

    m_iStackMax = 1;
}
Item::~Item()
{
    // nothing to do
}

ItemType Item::GetTypeFromID( ItemID iItemID )
{
    for( UInt i = 1; i < ITEMTYPE_COUNT; ++i ) {
        if ( iItemID < sm_arrTypeIDDomains[i].dwBaseID )
            return (ItemType)( i - 1 );
    }

    return (ItemType)( ITEMTYPE_COUNT - 1 );
}
ItemClass Item::GetClassFromID( ItemID iItemID )
{
    for( UInt i = 1; i < ITEMCLASS_COUNT; ++i ) {
        if ( iItemID < sm_arrClassIDDomains[i].dwBaseID )
            return (ItemClass)( i - 1 );
    }

    return (ItemClass)( ITEMCLASS_COUNT - 1 );
}
DWord Item::GetLocalID( ItemID iItemID, UInt * outClassDomainCapacity )
{
    for( UInt i = 1; i < ITEMCLASS_COUNT; ++i ) {
        if ( iItemID < sm_arrClassIDDomains[i].dwBaseID ) {
            if ( outClassDomainCapacity != NULL )
                *outClassDomainCapacity = sm_arrClassIDDomains[i-1].dwCapacity;
            return ( iItemID - sm_arrClassIDDomains[i-1].dwBaseID );
        }
    }

    if ( outClassDomainCapacity != NULL )
        *outClassDomainCapacity = sm_arrClassIDDomains[ITEMCLASS_COUNT-1].dwCapacity;
    return ( iItemID - sm_arrClassIDDomains[ITEMCLASS_COUNT-1].dwBaseID );
}

ItemInstance * Item::CreateInstance( BaseCharacter * pOwner, UInt iStackCount ) const
{
    Assert( iStackCount > 0 && iStackCount < m_iStackMax );

    GameplayFn->SelectMemory( TEXT("ItemInstances") );
    ItemInstance * pItemInstance = New ItemInstance( this, pOwner, iStackCount );
    GameplayFn->UnSelectMemory();

    return pItemInstance;
}

/////////////////////////////////////////////////////////////////////////////////
// ItemInstance implementation
ItemInstance::ItemInstance( const Item * pItem, BaseCharacter * pOwner, UInt iStackCount )
{
    m_pItem = pItem;
    m_pOwner = pOwner;

    m_pBound = NULL;

    m_iStackCount = iStackCount;
}
ItemInstance::ItemInstance( const ItemInstance & rhs )
{
    m_pItem = rhs.m_pItem;
    m_pOwner = rhs.m_pOwner;

    m_pBound = rhs.m_pBound;

    m_iStackCount = rhs.m_iStackCount;
}
ItemInstance::~ItemInstance()
{
    // nothing to do
}

Bool ItemInstance::Update( Scalar /*fTimeStep*/ )
{
    Assert( m_pItem != NULL );

    // nothing to do for now ...
    // Might be useful for stuff like radioactive decay for nuclear-waste ...

    return ( m_iStackCount > 0 );
}


