/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Application/Tools/InitCleanup.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Initialization & Cleanup callbacks.
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
#include "InitCleanup.h"

/////////////////////////////////////////////////////////////////////////////////
// InitCleanup implementation
UInt InitCleanup::sm_InitializerCount = 0;
Initializer InitCleanup::sm_arrInitializers[INITCLEANUP_MAX_CALLBACKS];

UInt InitCleanup::sm_CleanerCount = 0;
Cleaner InitCleanup::sm_arrCleaners[INITCLEANUP_MAX_CALLBACKS];

InitCleanup::InitCleanup()
{
    // nothing to do
}
InitCleanup::~InitCleanup()
{
    // nothing to do
}

Void InitCleanup::AddInitializer( Initializer pfCallback )
{
    Assert(sm_InitializerCount < INITCLEANUP_MAX_CALLBACKS);
    sm_arrInitializers[sm_InitializerCount++] = pfCallback;
}
Void InitCleanup::AddCleaner( Cleaner pfCallback )
{
    Assert(sm_CleanerCount < INITCLEANUP_MAX_CALLBACKS);
    sm_arrCleaners[sm_CleanerCount++] = pfCallback;
}

Void InitCleanup::Initialize()
{
    for( UInt i = 0; i < sm_InitializerCount; ++i )
        sm_arrInitializers[i]();
}
Void InitCleanup::Cleanup()
{
    for( UInt i = 0; i < sm_CleanerCount; ++i )
        sm_arrCleaners[i]();
}

