/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/Gameplay/Items/ItemClasses/EquipmentItems/ToolItems.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : WorldCraft, Gameplay, EquipmentItems : ToolItems
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
#ifndef SCARAB_ENTRYPOINT_WORLDCRAFT_GAMEPLAY_ITEMS_ITEMCLASSES_EQUIPMENTITEMS_TOOLITEMS_H
#define SCARAB_ENTRYPOINT_WORLDCRAFT_GAMEPLAY_ITEMS_ITEMCLASSES_EQUIPMENTITEMS_TOOLITEMS_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../ItemTypes.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The ToolItem class
class ToolItem : public EquipmentItem
{
public:
    ToolItem();
    virtual ~ToolItem();

    // Getters
    inline Scalar GetUsageRange() const;

    inline Scalar GetUsageSpeed() const;
    inline Scalar GetUsagePeriod() const;

    // Methods
    virtual UInt GetUsageModeCount() const = 0;

    virtual Bool HasPrimaryUsage( UInt iMode ) const = 0;
    virtual Scalar GetPrimaryUsageCooldown( UInt iMode ) const = 0;
    virtual Void PrimaryUse( UInt iMode, ItemInstance * pInstance ) const = 0;

    virtual Bool HasSecondaryUsage( UInt iMode ) const = 0;
    virtual Scalar GetSecondaryUsageCooldown( UInt iMode ) const = 0;
    virtual Void SecondaryUse( UInt iMode, ItemInstance * pInstance ) const = 0;

    virtual Bool HasSpecialUsage() const = 0;
    virtual Scalar GetSpecialUsageCooldown() const = 0;
    virtual Void SpecialUse( ItemInstance * pInstance ) const = 0;

protected:
    Scalar m_fUsageRange;
    
    Scalar m_fUsageSpeed;
    Scalar m_fUsagePeriod;
};

/////////////////////////////////////////////////////////////////////////////////
// The ToolRightItem class
class ToolRightItem : public ToolItem
{
public:
    ToolRightItem();
    virtual ~ToolRightItem();

    // Getters
    inline Bool IsTwoHanded() const;

protected:
    Bool m_bTwoHanded;
};

/////////////////////////////////////////////////////////////////////////////////
// The ToolLeftItem class
class ToolLeftItem : public ToolItem
{
public:
    ToolLeftItem();
    virtual ~ToolLeftItem();

protected:

};

/////////////////////////////////////////////////////////////////////////////////
// The ToolDistanceItem class
class ToolDistanceItem : public ToolItem
{
public:
    ToolDistanceItem();
    virtual ~ToolDistanceItem();

protected:

};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "ToolItems.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDCRAFT_GAMEPLAY_ITEMS_ITEMCLASSES_EQUIPMENTITEMS_TOOLITEMS_H

