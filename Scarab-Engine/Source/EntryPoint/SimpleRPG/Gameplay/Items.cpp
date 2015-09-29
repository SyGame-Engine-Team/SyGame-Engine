/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Gameplay/Items.cpp
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
// Includes
#include "Items.h"

#include "GameplayManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Item implementation
Item::Item()
{
    m_iType = ITEMTYPE_UNDEFINED;
    m_iClass = ITEMCLASS_UNDEFINED;
    m_iItemID = ITEMID_UNDEFINED;
    m_strName[0] = NULLBYTE;

    m_iBindType = ITEMBIND_NONE;
    m_iRating = 0;

    m_iStackMax = 1;

    m_bUsable = false;
    m_bConsumedOnUse = false;
}
Item::~Item()
{
    // nothing to do
}

Item * Item::GetInstance( ItemID idItem )
{
    return GameplayFn->GetItem( idItem );
}

Void Item::GenerateInstance( ItemInstance * outItemInstance, BaseCharacter * pOwner, UInt iStackCount )
{
    Assert( iStackCount > 0 );

    outItemInstance->m_pItem = this;
    outItemInstance->m_pOwner = pOwner;
    outItemInstance->m_pTarget = NULL;

    outItemInstance->m_pBound = NULL;

    outItemInstance->m_iStackCount = iStackCount;
}

/////////////////////////////////////////////////////////////////////////////////
// ItemInstance implementation
ItemInstance::ItemInstance()
{
    m_pItem = NULL;
    m_pBound = NULL;
    m_pTarget = NULL;

    m_pOwner = NULL;

    m_iStackCount = 0;
}
ItemInstance::~ItemInstance()
{
    // nothing to do
}

Bool ItemInstance::Update( Scalar /*fTimeStep*/ )
{
    Assert( m_pItem != NULL );

    // nothing to do for now ...

    return ( m_iStackCount > 0 );
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

Void Inventory::SwapItems( InventoryBagID idBagA, UInt iSlotA, InventoryBagID idBagB, UInt iSlotB )
{
    ItemInstance * pItemInstanceA = GetItem( idBagA, iSlotA );
    ItemInstance * pItemInstanceB = GetItem( idBagB, iSlotB );
    if ( pItemInstanceA->IsEmpty() && pItemInstanceB->IsEmpty() )
        return;

    Swap<ItemInstance>( pItemInstanceA, pItemInstanceB );
}

UInt Inventory::PickItem( Item * pItem, UInt iAmount )
{
    Assert( iAmount > 0 );

    if ( m_iTotalFreeSlots == 0 )
        return INVALID_OFFSET;

    if ( m_arrBags[INVENTORYBAGID_BUFFER].iFreeSlots == 0 )
        SortBufferBagItems();

    UInt iFreeSlot = _Bag_GetFreeSlot( INVENTORYBAGID_BUFFER );
    ItemInstance * pItemInstance = GetItem( INVENTORYBAGID_BUFFER, iFreeSlot );
    pItem->GenerateInstance( pItemInstance, m_pOwner, iAmount );

    if ( pItem->GetBindType() == ITEMBIND_ON_PICK )
        pItemInstance->Bind();

    --(m_arrBags[INVENTORYBAGID_BUFFER].iFreeSlots);
    --m_iTotalFreeSlots;

    return iFreeSlot;
}
Void Inventory::DropItem( InventoryBagID idBag, UInt iSlot )
{
    ItemInstance * pItemInstance = GetItem( idBag, iSlot );
    if ( pItemInstance->IsEmpty() )
        return;

    *pItemInstance = ItemInstance();

    ++(m_arrBags[idBag].iFreeSlots);
    ++m_iTotalFreeSlots;
}
ItemInstance * Inventory::EquipItem( EquipmentSoltID idEquipSlot, InventoryBagID idBag, UInt iSlot )
{
    Assert( idEquipSlot < EQUIPSLOTID_COUNT );

    ItemInstance * pItemInstance = GetItem( idBag, iSlot );
    if ( pItemInstance->IsEmpty() || pItemInstance->GetType() != ITEMTYPE_EQUIPMENT )
        return NULL;
    switch( idEquipSlot ) {
        case EQUIPSLOTID_HEAD:         if ( pItemInstance->GetClass() != ITEMCLASS_HEAD ) return NULL; break;
        case EQUIPSLOTID_CHEST:        if ( pItemInstance->GetClass() != ITEMCLASS_CHEST ) return NULL; break;
        case EQUIPSLOTID_SHOULDERS:    if ( pItemInstance->GetClass() != ITEMCLASS_SHOULDERS ) return NULL; break;
        case EQUIPSLOTID_ARMS:         if ( pItemInstance->GetClass() != ITEMCLASS_ARMS ) return NULL; break;
        case EQUIPSLOTID_WRISTS:       if ( pItemInstance->GetClass() != ITEMCLASS_WRISTS ) return NULL; break;
        case EQUIPSLOTID_HANDS:        if ( pItemInstance->GetClass() != ITEMCLASS_HANDS ) return NULL; break;
        case EQUIPSLOTID_LEGS:         if ( pItemInstance->GetClass() != ITEMCLASS_LEGS ) return NULL; break;
        case EQUIPSLOTID_FEET:         if ( pItemInstance->GetClass() != ITEMCLASS_FEET ) return NULL; break;
        case EQUIPSLOTID_NECK:         if ( pItemInstance->GetClass() != ITEMCLASS_NECK ) return NULL; break;
        case EQUIPSLOTID_BELT:         if ( pItemInstance->GetClass() != ITEMCLASS_BELT ) return NULL; break;
        case EQUIPSLOTID_RING_1:
        case EQUIPSLOTID_RING_2:       if ( pItemInstance->GetClass() != ITEMCLASS_RING ) return NULL; break;
        case EQUIPSLOTID_TRINKET_1:
        case EQUIPSLOTID_TRINKET_2:    if ( pItemInstance->GetClass() != ITEMCLASS_TRINKET ) return NULL; break;
        case EQUIPSLOTID_WEAPON_RIGHT: if ( pItemInstance->GetClass() != ITEMCLASS_WEAPON_RIGHT ) return NULL; break;
        case EQUIPSLOTID_WEAPON_LEFT:  if ( pItemInstance->GetClass() != ITEMCLASS_WEAPON_LEFT ) return NULL; break;
        case EQUIPSLOTID_WEAPON_THROW: if ( pItemInstance->GetClass() != ITEMCLASS_WEAPON_THROW ) return NULL; break;
        default: Assert(false); break;
    }

    if ( pItemInstance->GetBindType() == ITEMBIND_ON_EQUIP )
        pItemInstance->Bind();

    ItemInstance * pEquipItemInstance = GetItem( idEquipSlot );
    if ( pEquipItemInstance->IsEmpty() ) {
        *pEquipItemInstance = *pItemInstance;
        *pItemInstance = ItemInstance();

        ++(m_arrBags[idBag].iFreeSlots);
        ++m_iTotalFreeSlots;

        return pEquipItemInstance;
    }
    
    ItemInstance hSwap = *pEquipItemInstance;
    *pEquipItemInstance = *pItemInstance;
    *pItemInstance = hSwap;

    return pEquipItemInstance;
}
ItemInstance * Inventory::UnEquipItem( EquipmentSoltID idEquipSlot )
{
    ItemInstance * pEquipItemInstance = GetItem( idEquipSlot );
    if ( pEquipItemInstance->IsEmpty() )
        return NULL;
    if ( m_iTotalFreeSlots == 0 || m_arrBags[INVENTORYBAGID_EQUIPMENT].iFreeSlots == 0 )
        return NULL;

    UInt iFreeSlot = _Bag_GetFreeSlot( INVENTORYBAGID_EQUIPMENT );
    ItemInstance * pItemInstance = GetItem( INVENTORYBAGID_EQUIPMENT, iFreeSlot );

    *pItemInstance = *pEquipItemInstance;
    *pEquipItemInstance = ItemInstance();

    --(m_arrBags[INVENTORYBAGID_EQUIPMENT].iFreeSlots);
    --m_iTotalFreeSlots;

    return pItemInstance;
}

ItemInstance * Inventory::UseItem( BaseCharacter * pTarget, InventoryBagID idBag, UInt iSlot )
{
    ItemInstance * pItem = GetItem( idBag, iSlot );
    if ( pItem->IsEmpty() )
        return NULL;

    pItem->Use( pTarget );

    if ( pItem->IsEmpty() ) {
        DropItem( idBag, iSlot );
        return NULL;
    }

    return pItem;
}
ItemInstance * Inventory::UseItem( BaseCharacter * pTarget, EquipmentSoltID idEquipSlot )
{
    ItemInstance * pItem = GetItem( idEquipSlot );
    if ( pItem->IsEmpty() )
        return NULL;

    pItem->Use( pTarget );

    return pItem;
}

Void Inventory::SortBag( InventoryBagID idBag )
{
    Assert( idBag < INVENTORYBAGID_COUNT );
    _InventoryBag * pBag = ( m_arrBags + idBag );

    UInt iItemCount = ( INVENTORY_BAG_SIZE - pBag->iFreeSlots );
    if ( iItemCount == 0 )
        return;

    UInt iTmpCount = 0;
    ItemInstance arrTmp[INVENTORY_BAG_SIZE];

    while( iItemCount > 0 ) {
        UInt iMinIndex = INVALID_OFFSET;
        ItemID iMinID = ITEMID_UNDEFINED;
        for( UInt i = 0; i < INVENTORY_BAG_SIZE; ++i ) {
            if ( !(pBag->arrSlots[i].IsEmpty()) ) {
                if ( pBag->arrSlots[i].GetID() < iMinID ) {
                    iMinIndex = i;
                    iMinID = pBag->arrSlots[i].GetID();
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
        pBag->arrSlots[i] = ItemInstance();
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
        ItemInstance * pItemInstance = GetItem( INVENTORYBAGID_BUFFER, i );
        if ( pItemInstance->IsEmpty() )
            continue;

        InventoryBagID idTargetBag = INVENTORYBAGID_COUNT;
        switch( pItemInstance->GetType() ) {
            case ITEMTYPE_TRASH:       idTargetBag = INVENTORYBAGID_TRASH;       break;
            case ITEMTYPE_CONSUMMABLE: idTargetBag = INVENTORYBAGID_CONSUMMABLE; break;
            case ITEMTYPE_EQUIPMENT:   idTargetBag = INVENTORYBAGID_EQUIPMENT;   break;
            case ITEMTYPE_QUEST:       idTargetBag = INVENTORYBAGID_QUEST;       break;
            default: Assert(false); break;
        }

        if ( m_arrBags[idTargetBag].iFreeSlots > 0 ) {
            UInt iFreeSlot = _Bag_GetFreeSlot( idTargetBag );
            ItemInstance * pTargetItemInstance = GetItem( idTargetBag, iFreeSlot );

            *pTargetItemInstance = *pItemInstance;
            *pItemInstance = ItemInstance();

            ++(m_arrBags[INVENTORYBAGID_BUFFER].iFreeSlots);
            --(m_arrBags[idTargetBag].iFreeSlots);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////

UInt Inventory::_Bag_GetFreeSlot( InventoryBagID idBag ) const
{
    Assert( idBag < INVENTORYBAGID_COUNT );
    const _InventoryBag * pBag = ( m_arrBags + idBag );
    if ( pBag->iFreeSlots == 0 )
        return INVALID_OFFSET;

    for( UInt i = 0; i < INVENTORY_BAG_SIZE; ++i ) {
        if ( pBag->arrSlots[i].IsEmpty() )
            return i;
    }

    Assert(false); // Should never happen
    return INVALID_OFFSET;
}

/////////////////////////////////////////////////////////////////////////////////
// LootStash implementation
LootStash::LootStash()
{
}
LootStash::~LootStash()
{
}

