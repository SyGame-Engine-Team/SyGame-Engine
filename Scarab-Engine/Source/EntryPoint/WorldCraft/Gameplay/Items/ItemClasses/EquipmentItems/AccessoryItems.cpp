/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/Gameplay/Items/ItemClasses/EquipmentItems/AccessoryItems.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : WorldCraft, Gameplay, EquipmentItems : AccessoryItems
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
#include "AccessoryItems.h"

/////////////////////////////////////////////////////////////////////////////////
// AccessoryItem implementation
AccessoryItem::AccessoryItem():
    EquipmentItem()
{
    // nothing to do
}
AccessoryItem::~AccessoryItem()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// NeckItem implementation
NeckItem::NeckItem():
    AccessoryItem()
{
    m_iClass = ITEMCLASS_NECK;
}
NeckItem::~NeckItem()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// BackItem implementation
BackItem::BackItem():
    AccessoryItem()
{
    m_iClass = ITEMCLASS_BACK;
}
BackItem::~BackItem()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// BeltItem implementation
BeltItem::BeltItem():
    AccessoryItem()
{
    m_iClass = ITEMCLASS_BELT;
}
BeltItem::~BeltItem()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// RingItem implementation
RingItem::RingItem():
    AccessoryItem()
{
    m_iClass = ITEMCLASS_RING;
}
RingItem::~RingItem()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// TrinketItem implementation
TrinketItem::TrinketItem():
    AccessoryItem()
{
    m_iClass = ITEMCLASS_TRINKET;
}
TrinketItem::~TrinketItem()
{
    // nothing to do
}
