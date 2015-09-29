/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/Gameplay/Items/ItemClasses/ConsumableItems/PotionItem.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : WorldCraft, Gameplay, ConsumableItems : PotionItem
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
#ifndef SCARAB_ENTRYPOINT_WORLDCRAFT_GAMEPLAY_ITEMS_ITEMCLASSES_CONSUMABLEITEMS_POTIONITEM_H
#define SCARAB_ENTRYPOINT_WORLDCRAFT_GAMEPLAY_ITEMS_ITEMCLASSES_CONSUMABLEITEMS_POTIONITEM_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../ItemTypes.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The PotionItem class
class PotionItem : public ConsumableItem
{
public:
    PotionItem();
    virtual ~PotionItem();

protected:

};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "PotionItem.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDCRAFT_GAMEPLAY_ITEMS_ITEMCLASSES_CONSUMABLEITEMS_POTIONITEM_H
