/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/Gameplay/Items/ItemClasses/EquipmentItems/WeaponItems.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : WorldCraft, Gameplay, EquipmentItems : WeaponItems
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
#ifndef SCARAB_ENTRYPOINT_WORLDCRAFT_GAMEPLAY_ITEMS_ITEMCLASSES_EQUIPMENTITEMS_WEAPONITEMS_H
#define SCARAB_ENTRYPOINT_WORLDCRAFT_GAMEPLAY_ITEMS_ITEMCLASSES_EQUIPMENTITEMS_WEAPONITEMS_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../ItemTypes.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The WeaponItem class
class WeaponItem : public EquipmentItem
{
public:
    WeaponItem();
    virtual ~WeaponItem();

    // Getters
    inline Scalar GetAttackRange() const;

    inline Scalar GetAttackSpeed() const;
    inline Scalar GetAttackPeriod() const;

    inline UInt GetDamageMin() const;
    inline UInt GetDamageMax() const;

    // Methods
    virtual UInt GetAttackModeCount() const = 0;

    virtual Bool HasPrimaryAttack( UInt iMode ) const = 0;
    virtual Scalar GetPrimaryAttackCooldown( UInt iMode ) const = 0;
    virtual Void PrimaryAttack( UInt iMode, ItemInstance * pInstance ) const = 0;

    virtual Bool HasSecondaryAttack( UInt iMode ) const = 0;
    virtual Scalar GetSecondaryAttackCooldown( UInt iMode ) const = 0;
    virtual Void SecondaryAttack( UInt iMode, ItemInstance * pInstance ) const = 0;

    virtual Bool HasSpecialAttack() const = 0;
    virtual Scalar GetSpecialAttackCooldown() const = 0;
    virtual Void SpecialAttack( ItemInstance * pInstance ) const = 0;

protected:
    Scalar m_fAttackRange;
    
    Scalar m_fAttackSpeed;
    Scalar m_fAttackPeriod;

    UInt m_iDamageMin;
    UInt m_iDamageMax;
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
// The WeaponDistanceItem class
class WeaponDistanceItem : public WeaponItem
{
public:
    WeaponDistanceItem();
    virtual ~WeaponDistanceItem();

protected:

};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "WeaponItems.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDCRAFT_GAMEPLAY_ITEMS_ITEMCLASSES_EQUIPMENTITEMS_WEAPONITEMS_H

