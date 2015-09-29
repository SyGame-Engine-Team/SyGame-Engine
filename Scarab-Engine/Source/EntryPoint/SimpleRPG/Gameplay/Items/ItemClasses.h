/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Gameplay/Items/ItemClasses.h
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
// Header prelude
#ifndef SCARAB_ENTRYPOINT_SIMPLERPG_GAMEPLAY_ITEMS_ITEMCLASSES_H
#define SCARAB_ENTRYPOINT_SIMPLERPG_GAMEPLAY_ITEMS_ITEMCLASSES_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Statistics.h"
#include "../StatusEffects.h"
#include "../Items.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TrashItem class
class TrashItem : public Item
{
public:
    TrashItem();
    virtual ~TrashItem();

protected:

};

/////////////////////////////////////////////////////////////////////////////////
// The ConsummableItem class
class ConsummableItem : public Item
{
public:
    ConsummableItem();
    virtual ~ConsummableItem();

protected:

};

/////////////////////////////////////////////////////////////////////////////////
// The FoodItem class
class FoodItem : public ConsummableItem
{
public:
    FoodItem();
    virtual ~FoodItem();

protected:

};

/////////////////////////////////////////////////////////////////////////////////
// The DrinkItem class
class DrinkItem : public ConsummableItem
{
public:
    DrinkItem();
    virtual ~DrinkItem();

protected:

};

/////////////////////////////////////////////////////////////////////////////////
// The PotionItem class
class PotionItem : public ConsummableItem
{
public:
    PotionItem();
    virtual ~PotionItem();

protected:

};

/////////////////////////////////////////////////////////////////////////////////
// The ThrowableItem class
class ThrowableItem : public ConsummableItem
{
public:
    ThrowableItem();
    virtual ~ThrowableItem();

protected:

};

/////////////////////////////////////////////////////////////////////////////////
// The EquipmentItem class
class EquipmentItem : public Item
{
public:
    EquipmentItem();
    virtual ~EquipmentItem();

    // Getters
    virtual Bool IsArmor() const = 0;
    virtual Bool IsJewel() const = 0;
    virtual Bool IsWeapon() const = 0;

    inline const Scalar * GetStatBonus() const;

protected:
    Scalar m_arrStatBonus[CHARSTAT_COUNT];
};

/////////////////////////////////////////////////////////////////////////////////
// The ArmorItem class
class ArmorItem : public EquipmentItem
{
public:
    ArmorItem();
    virtual ~ArmorItem();

    // Getters
    inline virtual Bool IsArmor() const;
    inline virtual Bool IsJewel() const;
    inline virtual Bool IsWeapon() const;

    inline StatusEffect * GetOnHitEffect() const;

protected:
    StatusEffect * m_pOnHitEffect;
};

/////////////////////////////////////////////////////////////////////////////////
// The HeadItem class
class HeadItem : public ArmorItem
{
public:
    HeadItem();
    virtual ~HeadItem();

protected:

};

/////////////////////////////////////////////////////////////////////////////////
// The ChestItem class
class ChestItem : public ArmorItem
{
public:
    ChestItem();
    virtual ~ChestItem();

protected:

};

/////////////////////////////////////////////////////////////////////////////////
// The ShouldersItem class
class ShouldersItem : public ArmorItem
{
public:
    ShouldersItem();
    virtual ~ShouldersItem();

protected:

};

/////////////////////////////////////////////////////////////////////////////////
// The ArmsItem class
class ArmsItem : public ArmorItem
{
public:
    ArmsItem();
    virtual ~ArmsItem();

protected:

};

/////////////////////////////////////////////////////////////////////////////////
// The WristsItem class
class WristsItem : public ArmorItem
{
public:
    WristsItem();
    virtual ~WristsItem();

protected:

};

/////////////////////////////////////////////////////////////////////////////////
// The HandsItem class
class HandsItem : public ArmorItem
{
public:
    HandsItem();
    virtual ~HandsItem();

protected:

};

/////////////////////////////////////////////////////////////////////////////////
// The LegsItem class
class LegsItem : public ArmorItem
{
public:
    LegsItem();
    virtual ~LegsItem();

protected:

};

/////////////////////////////////////////////////////////////////////////////////
// The FeetItem class
class FeetItem : public ArmorItem
{
public:
    FeetItem();
    virtual ~FeetItem();

protected:

};

/////////////////////////////////////////////////////////////////////////////////
// The JewelItem class
class JewelItem : public EquipmentItem
{
public:
    JewelItem();
    virtual ~JewelItem();

    // Getters
    inline virtual Bool IsArmor() const;
    inline virtual Bool IsJewel() const;
    inline virtual Bool IsWeapon() const;

protected:

};

/////////////////////////////////////////////////////////////////////////////////
// The NeckItem class
class NeckItem : public JewelItem
{
public:
    NeckItem();
    virtual ~NeckItem();

protected:

};

/////////////////////////////////////////////////////////////////////////////////
// The BeltItem class
class BeltItem : public JewelItem
{
public:
    BeltItem();
    virtual ~BeltItem();

protected:

};

/////////////////////////////////////////////////////////////////////////////////
// The RingItem class
class RingItem : public JewelItem
{
public:
    RingItem();
    virtual ~RingItem();

protected:

};

/////////////////////////////////////////////////////////////////////////////////
// The TrinketItem class
class TrinketItem : public JewelItem
{
public:
    TrinketItem();
    virtual ~TrinketItem();

protected:

};

/////////////////////////////////////////////////////////////////////////////////
// The WeaponItem class
class WeaponItem : public EquipmentItem
{
public:
    WeaponItem();
    virtual ~WeaponItem();

    // Getters
    inline virtual Bool IsArmor() const;
    inline virtual Bool IsJewel() const;
    inline virtual Bool IsWeapon() const;

    inline Scalar GetAttackRange() const;

    inline Scalar GetAttackSpeed() const;
    inline Scalar GetAttackPeriod() const;

    inline UInt GetDamageMin() const;
    inline UInt GetDamageMax() const;

    inline StatusEffectID GetOnHitEffect() const;
    inline Scalar GetOnHitEffectScore() const;
    inline UInt GetOnHitEffectRank() const;
    inline UInt GetOnHitEffectStacks() const;

    inline StatusEffectID GetOnCriticalEffect() const;
    inline Scalar GetOnCriticalEffectScore() const;
    inline UInt GetOnCriticalEffectRank() const;
    inline UInt GetOnCriticalEffectStacks() const;

protected:
    Scalar m_fAttackRange;
    
    Scalar m_fAttackSpeed;
    Scalar m_fAttackPeriod;

    UInt m_iDamageMin;
    UInt m_iDamageMax;

    StatusEffectID m_idOnHitEffect;
    Scalar m_fOnHitEffectScore;
    UInt m_iOnHitEffectRank;
    UInt m_iOnHitEffectStacks;

    StatusEffectID m_idOnCriticalEffect;
    Scalar m_fOnCriticalEffectScore;
    UInt m_iOnCriticalEffectRank;
    UInt m_iOnCriticalEffectStacks;
};

/////////////////////////////////////////////////////////////////////////////////
// The WeaponRightItem class
class WeaponRightItem : public WeaponItem
{
public:
    WeaponRightItem();
    virtual ~WeaponRightItem();

    // Getters
    inline Bool IsTwoHanded() const;

protected:
    Bool m_bTwoHanded;
};

/////////////////////////////////////////////////////////////////////////////////
// The WeaponLeftItem class
class WeaponLeftItem : public WeaponItem
{
public:
    WeaponLeftItem();
    virtual ~WeaponLeftItem();

protected:

};

/////////////////////////////////////////////////////////////////////////////////
// The WeaponThrowItem class
class WeaponThrowItem : public WeaponItem
{
public:
    WeaponThrowItem();
    virtual ~WeaponThrowItem();

protected:

};

/////////////////////////////////////////////////////////////////////////////////
// The QuestItem class
class QuestItem : public Item
{
public:
    QuestItem();
    virtual ~QuestItem();

    // Getters
    //inline QuestID GetParentQuest() const;

protected:
    //QuestID m_idParentQuest;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "ItemClasses.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_SIMPLERPG_GAMEPLAY_ITEMS_ITEMCLASSES_H

