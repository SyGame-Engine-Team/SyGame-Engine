/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/GlobalEffects/GlobalEffect.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Base class for global effects
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_WORLD_GLOBALEFFECTS_GLOBALEFFECT_H
#define SCARAB_ENGINE_WORLD_GLOBALEFFECTS_GLOBALEFFECT_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../WorldCuller.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The GlobalEffect class
class GlobalEffect
{
public:
    GlobalEffect();
    virtual ~GlobalEffect();

    // Rendering support
    virtual Void Render( WorldLeaf * pSubject ) = 0;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GlobalEffect.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_WORLD_GLOBALEFFECTS_GLOBALEFFECT_H