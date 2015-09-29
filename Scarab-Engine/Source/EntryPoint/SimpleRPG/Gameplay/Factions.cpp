/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Gameplay/Factions.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Gameplay : Factions
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
#include "Factions.h"

/////////////////////////////////////////////////////////////////////////////////
// FactionRegistry implementation
Bool FactionRegistry::sm_mapHostility[FACTIONID_COUNT * FACTIONID_COUNT] =
{
    // UNDEFINED | RED(P) | GREEN(P) | BLUE(P) | YELLOW(P) | HOSTILE(M) |
       false,      false,   false,     false,    false,      false,     // UNDEFINED
       false,      false,   true,      true,     true,       true,      // RED(P)
       false,      true,    false,     true,     true,       true,      // GREEN(P)
       false,      true,    true,      false,    true,       true,      // BLUE(P)
       false,      true,    true,      true,     false,      true,      // YELLOW(P)
       false,      true,    true,      true,     true,       true       // HOSTILE(M)
};

FactionRegistry::FactionRegistry()
{
    m_idParentFaction = FACTIONID_UNDEFINED;
}
FactionRegistry::~FactionRegistry()
{
    // nothing to do
}

Bool FactionRegistry::IsHostile( FactionID idFaction ) const
{
    Assert( m_idParentFaction < FACTIONID_COUNT );
    Assert( idFaction < FACTIONID_COUNT );
    return sm_mapHostility[ (m_idParentFaction * FACTIONID_COUNT) + idFaction ];
}

