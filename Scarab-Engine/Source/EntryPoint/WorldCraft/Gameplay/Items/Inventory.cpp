/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/Gameplay/Items/Inventory.cpp
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
// Includes
#include "Inventory.h"

/////////////////////////////////////////////////////////////////////////////////
// LootStash implementation
LootStash::LootStash()
{
}
LootStash::~LootStash()
{
}

/////////////////////////////////////////////////////////////////////////////////
// Inventory implementation
Inventory::Inventory( BaseCharacter * pOwner )
{
    m_pOwner = pOwner;

    // Empty bags
    m_iTotalFreeSlots = ( INVENTORY_BAG_SIZE * INVENTORYBAGID_COUNT );
    for( UInt i = 0; i < INVENTORYBAGID_COUNT; ++i )
        m_arrBags[i].iFreeSlots = INVENTORY_BAG_SIZE;
}
Inventory::~Inventory()
{
    // nothing to do
}

Bool Inventory::PickItem( ItemInstance * pItemInstance )
{
    Assert( pItemInstance != NULL );

    if ( m_iTotalFreeSlots == 0 )
        return false;

    if ( m_arrBags[INVENTORYBAGID_BUFFER].iFreeSlots == 0 )
        SortBufferBagItems();

    UInt iFreeSlot = _Bag_GetFreeSlot( INVENTORYBAGID_BUFFER );
    Assert( m_arrBags[INVENTORYBAGID_BUFFER].arrSlots[iFreeSlot] == NULL );

    if ( pItemInstance->GetBindType() == ITEMBIND_ON_PICK )
        pItemInstance->Bind();

    m_arrBags[INVENTORYBAGID_BUFFER].arrSlots[iFreeSlot] = pItemInstance;
    --(m_arrBags[INVENTORYBAGID_BUFFER].iFreeSlots);
    --m_iTotalFreeSlots;

    return true;
}
ItemInstance * Inventory::DropItem( InventoryBagID idBag, UInt iSlot )
{
    Assert( idBag < INVENTORYBAGID_COUNT && iSlot < INVENTORY_BAG_SIZE );

    ItemInstance * pItemInstance = m_arrBags[idBag].arrSlots[iSlot];

    m_arrBags[idBag].arrSlots[iSlot] = NULL;
    ++(m_arrBags[idBag].iFreeSlots);
    ++m_iTotalFreeSlots;

    return pItemInstance;
}
ItemInstance * Inventory::EquipItem( EquipmentSoltID idEquipSlot, InventoryBagID idBag, UInt iSlot )
{
    Assert( idBag < INVENTORYBAGID_COUNT && iSlot < INVENTORY_BAG_SIZE );

    ItemInstance * pSwap = m_arrBags[idBag].arrSlots[iSlot];
    if ( pSwap == NULL )
        return NULL;

    switch( idEquipSlot ) {
        case EQUIPSLOTID_HEAD:            if ( pSwap->GetClass() != ITEMCLASS_HEAD ) return NULL; break;
        case EQUIPSLOTID_CHEST:           if ( pSwap->GetClass() != ITEMCLASS_CHEST ) return NULL; break;
        case EQUIPSLOTID_SHOULDERS:       if ( pSwap->GetClass() != ITEMCLASS_SHOULDERS ) return NULL; break;
        case EQUIPSLOTID_ARMS:            if ( pSwap->GetClass() != ITEMCLASS_ARMS ) return NULL; break;
        case EQUIPSLOTID_WRISTS:          if ( pSwap->GetClass() != ITEMCLASS_WRISTS ) return NULL; break;
        case EQUIPSLOTID_HANDS:           if ( pSwap->GetClass() != ITEMCLASS_HANDS ) return NULL; break;
        case EQUIPSLOTID_LEGS:            if ( pSwap->GetClass() != ITEMCLASS_LEGS ) return NULL; break;
        case EQUIPSLOTID_FEET:            if ( pSwap->GetClass() != ITEMCLASS_FEET ) return NULL; break;
        case EQUIPSLOTID_NECK:            if ( pSwap->GetClass() != ITEMCLASS_NECK ) return NULL; break;
        case EQUIPSLOTID_BELT:            if ( pSwap->GetClass() != ITEMCLASS_BELT ) return NULL; break;
        case EQUIPSLOTID_RING_1:
        case EQUIPSLOTID_RING_2:          if ( pSwap->GetClass() != ITEMCLASS_RING ) return NULL; break;
        case EQUIPSLOTID_TRINKET_1:
        case EQUIPSLOTID_TRINKET_2:       if ( pSwap->GetClass() != ITEMCLASS_TRINKET ) return NULL; break;
        case EQUIPSLOTID_TOOL_RIGHT:      if ( pSwap->GetClass() != ITEMCLASS_TOOL_RIGHT ) return NULL; break;
        case EQUIPSLOTID_TOOL_LEFT:       if ( pSwap->GetClass() != ITEMCLASS_TOOL_LEFT ) return NULL; break;
        case EQUIPSLOTID_TOOL_DISTANCE:   if ( pSwap->GetClass() != ITEMCLASS_TOOL_DISTANCE ) return NULL; break;
        case EQUIPSLOTID_WEAPON_RIGHT:    if ( pSwap->GetClass() != ITEMCLASS_WEAPON_RIGHT ) return NULL; break;
        case EQUIPSLOTID_WEAPON_LEFT:     if ( pSwap->GetClass() != ITEMCLASS_WEAPON_LEFT ) return NULL; break;
        case EQUIPSLOTID_WEAPON_DISTANCE: if ( pSwap->GetClass() != ITEMCLASS_WEAPON_DISTANCE ) return NULL; break;
        default: Assert(false); break;
    }

    if ( pSwap->GetBindType() == ITEMBIND_ON_EQUIP )
        pSwap->Bind();

    m_arrBags[idBag].arrSlots[iSlot] = m_arrEquipmentSlots[idEquipSlot];
    m_arrEquipmentSlots[idEquipSlot] = pSwap;

    return pSwap;
}
ItemInstance * Inventory::UnEquipItem( EquipmentSoltID idEquipSlot )
{
    Assert( idEquipSlot < EQUIPSLOTID_COUNT );
    
    ItemInstance * pItemInstance = m_arrEquipmentSlots[idEquipSlot];
    if ( pItemInstance == NULL )
        return NULL;

    if ( m_iTotalFreeSlots == 0 )
        return NULL;

    InventoryBagID idBag = INVENTORYBAGID_EQUIPMENT;
    if ( m_arrBags[idBag].iFreeSlots == 0  ) {
        idBag = INVENTORYBAGID_BUFFER;
        if ( m_arrBags[idBag].iFreeSlots == 0 )
            SortBufferBagItems();
    }

    UInt iFreeSlot = _Bag_GetFreeSlot( idBag );
    Assert( m_arrBags[idBag].arrSlots[iFreeSlot] == NULL );

    m_arrBags[idBag].arrSlots[iFreeSlot] = pItemInstance;
    --(m_arrBags[idBag].iFreeSlots);
    --m_iTotalFreeSlots;

    return pItemInstance;
}

Void Inventory::SortBag( InventoryBagID idBag )
{
    Assert( idBag < INVENTORYBAGID_COUNT );
    _InventoryBag * pBag = ( m_arrBags + idBag );

    UInt iItemCount = ( INVENTORY_BAG_SIZE - pBag->iFreeSlots );
    if ( iItemCount == 0 )
        return;

    UInt iTmpCount = 0;
    ItemInstance * arrTmp[INVENTORY_BAG_SIZE];

    while( iItemCount > 0 ) {
        UInt iMinIndex = INVALID_OFFSET;
        ItemID iMinID = ITEMID_UNDEFINED;
        for( UInt i = 0; i < INVENTORY_BAG_SIZE; ++i ) {
            if ( pBag->arrSlots[i] != NULL ) {
                if ( pBag->arrSlots[i]->GetID() < iMinID ) {
                    iMinIndex = i;
                    iMinID = pBag->arrSlots[i]->GetID();
                }
            }
        }
        Assert( iMinIndex != INVALID_OFFSET );
        arrTmp[iTmpCount++] = pBag->arrSlots[iMinIndex];
        --iItemCount;
    }
    for( UInt i = 0; i < iTmpCount; ++i )
        pBag->arrSlots[i] = arrTmp[i];
    for( UInt i = iTmpCount; i < INVENTORY_BAG_SIZE; ++i )
        pBag->arrSlots[i] = NULL;
}
Void Inventory::SortAllBags()
{
    for( UInt i = 0; i < INVENTORYBAGID_COUNT; ++i )
        SortBag( (InventoryBagID)i );
}
Void Inventory::SortBufferBagItems()
{
    if ( m_arrBags[INVENTORYBAGID_BUFFER].iFreeSlots == INVENTORY_BAG_SIZE )
        return;

    for( UInt i = 0; i < INVENTORY_BAG_SIZE; ++i ) {
        ItemInstance * pItemInstance = m_arrBags[INVENTORYBAGID_BUFFER].arrSlots[i];
        if ( pItemInstance == NULL )
            continue;

        InventoryBagID idTargetBag = INVENTORYBAGID_COUNT;
        switch( pItemInstance->GetType() ) {
            case ITEMTYPE_BLOCK:      idTargetBag = INVENTORYBAGID_BLOCK;       break;
            case ITEMTYPE_EQUIPMENT:  idTargetBag = INVENTORYBAGID_EQUIPMENT;   break;
            case ITEMTYPE_CONSUMABLE: idTargetBag = INVENTORYBAGID_CONSUMABLE; break;
            case ITEMTYPE_QUEST:      idTargetBag = INVENTORYBAGID_QUEST;       break;
            default: Assert(false); break;
        }
        if ( m_arrBags[idTargetBag].iFreeSlots == 0 )
            continue;

        UInt iFreeSlot = _Bag_GetFreeSlot( idTargetBag );
        Assert( m_arrBags[idTargetBag].arrSlots[iFreeSlot] == NULL );

        m_arrBags[INVENTORYBAGID_BUFFER].arrSlots[i] = NULL;
        m_arrBags[idTargetBag].arrSlots[iFreeSlot] = pItemInstance;

        ++(m_arrBags[INVENTORYBAGID_BUFFER].iFreeSlots);
        --(m_arrBags[idTargetBag].iFreeSlots);
    }
    SortBag( INVENTORYBAGID_BUFFER );
}

/////////////////////////////////////////////////////////////////////////////////

UInt Inventory::_Bag_GetFreeSlot( InventoryBagID idBag ) const
{
    Assert( idBag < INVENTORYBAGID_COUNT );

    const _InventoryBag * pBag = ( m_arrBags + idBag );
    Assert( pBag->iFreeSlots > 0 );

    for( UInt i = 0; i < INVENTORY_BAG_SIZE; ++i ) {
        if ( pBag->arrSlots[i] == NULL )
            return i;
    }

    Assert(false); // Should never happen
    return INVALID_OFFSET;
}

