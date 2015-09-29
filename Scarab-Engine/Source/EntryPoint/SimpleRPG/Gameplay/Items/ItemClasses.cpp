/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Gameplay/Items/ItemClasses.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Gameplay, Items : ItemClasses
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
#include "ItemClasses.h"

#include "../GameplayManager.h"

/////////////////////////////////////////////////////////////////////////////////
// TrashItem implementation
TrashItem::TrashItem():
    Item()
{
    m_iType = ITEMTYPE_TRASH;
    m_iClass = ITEMCLASS_TRASH;
    m_iBindType = ITEMBIND_NONE;
}
TrashItem::~TrashItem()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// ConsummableItem implementation
ConsummableItem::ConsummableItem():
    Item()
{
    m_iType = ITEMTYPE_CONSUMMABLE;
    m_iBindType = ITEMBIND_NONE;

    m_bUsable = true;
    m_bConsumedOnUse = true;
}
ConsummableItem::~ConsummableItem()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// FoodItem implementation
FoodItem::FoodItem():
    ConsummableItem()
{
    m_iClass = ITEMCLASS_FOOD;
}
FoodItem::~FoodItem()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// DrinkItem implementation
DrinkItem::DrinkItem():
    ConsummableItem()
{
    m_iClass = ITEMCLASS_DRINK;
}
DrinkItem::~DrinkItem()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// PotionItem implementation
PotionItem::PotionItem():
    ConsummableItem()
{
    m_iClass = ITEMCLASS_POTION;
}
PotionItem::~PotionItem()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// ThrowableItem implementation
ThrowableItem::ThrowableItem():
    ConsummableItem()
{
    m_iClass = ITEMCLASS_THROWABLE;
}
ThrowableItem::~ThrowableItem()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// EquipmentItem implementation
EquipmentItem::EquipmentItem():
    Item()
{
    m_iType = ITEMTYPE_EQUIPMENT;
    m_iBindType = ITEMBIND_ON_EQUIP;

    for( UInt i = 0; i < CHARSTAT_COUNT; ++i )
        m_arrStatBonus[i] = 0.0f;
}
EquipmentItem::~EquipmentItem()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// ArmorItem implementation
ArmorItem::ArmorItem():
    EquipmentItem()
{
    m_pOnHitEffect = NULL;
}
ArmorItem::~ArmorItem()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// HeadItem implementation
HeadItem::HeadItem():
    ArmorItem()
{
    m_iClass = ITEMCLASS_HEAD;
}
HeadItem::~HeadItem()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// ChestItem implementation
ChestItem::ChestItem():
    ArmorItem()
{
    m_iClass = ITEMCLASS_CHEST;
}
ChestItem::~ChestItem()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// ShouldersItem implementation
ShouldersItem::ShouldersItem():
    ArmorItem()
{
    m_iClass = ITEMCLASS_SHOULDERS;
}
ShouldersItem::~ShouldersItem()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// ArmsItem implementation
ArmsItem::ArmsItem():
    ArmorItem()
{
    m_iClass = ITEMCLASS_ARMS;
}
ArmsItem::~ArmsItem()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// WristsItem implementation
WristsItem::WristsItem():
    ArmorItem()
{
    m_iClass = ITEMCLASS_WRISTS;
}
WristsItem::~WristsItem()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// HandsItem implementation
HandsItem::HandsItem():
    ArmorItem()
{
    m_iClass = ITEMCLASS_HANDS;
}
HandsItem::~HandsItem()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// LegsItem implementation
LegsItem::LegsItem():
    ArmorItem()
{
    m_iClass = ITEMCLASS_LEGS;
}
LegsItem::~LegsItem()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// FeetItem implementation
FeetItem::FeetItem():
    ArmorItem()
{
    m_iClass = ITEMCLASS_FEET;
}
FeetItem::~FeetItem()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// JewelItem implementation
JewelItem::JewelItem():
    EquipmentItem()
{
    // nothing to do
}
JewelItem::~JewelItem()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// NeckItem implementation
NeckItem::NeckItem():
    JewelItem()
{
    m_iClass = ITEMCLASS_NECK;
}
NeckItem::~NeckItem()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// BeltItem implementation
BeltItem::BeltItem():
    JewelItem()
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
    JewelItem()
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
    JewelItem()
{
    m_iClass = ITEMCLASS_TRINKET;

    m_bUsable = true;
    m_bConsumedOnUse = false;
}
TrinketItem::~TrinketItem()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// WeaponItem implementation
WeaponItem::WeaponItem():
    EquipmentItem()
{
    m_fAttackRange = 1.0f;

    m_fAttackSpeed = 1.0f;
    m_fAttackPeriod = 1.0f;

    m_iDamageMin = 1;
    m_iDamageMax = 2;

    m_idOnHitEffect = STATUSEFFECTID_UNDEFINED;
    m_fOnHitEffectScore = 0.0f;
    m_iOnHitEffectRank = 0;
    m_iOnHitEffectStacks = 0;

    m_idOnCriticalEffect = STATUSEFFECTID_UNDEFINED;
    m_fOnCriticalEffectScore = 0.0f;
    m_iOnCriticalEffectRank = 0;
    m_iOnCriticalEffectStacks = 0;
}
WeaponItem::~WeaponItem()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// WeaponRightItem implementation
WeaponRightItem::WeaponRightItem():
    WeaponItem()
{
    m_iClass = ITEMCLASS_WEAPON_RIGHT;

    m_bTwoHanded = false;
}
WeaponRightItem::~WeaponRightItem()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// WeaponLeftItem implementation
WeaponLeftItem::WeaponLeftItem():
    WeaponItem()
{
    m_iClass = ITEMCLASS_WEAPON_LEFT;
}
WeaponLeftItem::~WeaponLeftItem()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// WeaponThrowItem implementation
WeaponThrowItem::WeaponThrowItem():
    WeaponItem()
{
    m_iClass = ITEMCLASS_WEAPON_THROW;
}
WeaponThrowItem::~WeaponThrowItem()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// QuestItem implementation
QuestItem::QuestItem():
    Item()
{
    m_iType = ITEMTYPE_QUEST;
    m_iClass = ITEMCLASS_QUEST;
    m_iBindType = ITEMBIND_ON_PICK;

    //m_idParentQuest = QUESTID_UNDEFINED;
}
QuestItem::~QuestItem()
{
    // nothing to do
}

