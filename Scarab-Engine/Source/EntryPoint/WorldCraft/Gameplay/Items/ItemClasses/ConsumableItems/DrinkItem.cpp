/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/Gameplay/Items/ItemClasses/ConsumableItems/DrinkItem.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : WorldCraft, Gameplay, ConsumableItems : DrinkItem
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
#include "DrinkItem.h"

/////////////////////////////////////////////////////////////////////////////////
// DrinkItem implementation
DrinkItem::DrinkItem():
    ConsumableItem()
{
    m_iClass = ITEMCLASS_DRINK;
}
DrinkItem::~DrinkItem()
{
    // nothing to do
}