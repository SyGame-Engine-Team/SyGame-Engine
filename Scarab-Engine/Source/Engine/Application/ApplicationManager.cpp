/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Application/ApplicationManager.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Core-Class for the Application abstraction system
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
#include "ApplicationManager.h"

/////////////////////////////////////////////////////////////////////////////////
// ApplicationManager implementation
CreateAppCallback ApplicationManager::CreateApp = NULL;
DestroyAppCallback ApplicationManager::DestroyApp = NULL;

ApplicationManager::ApplicationManager():
    Manager<ApplicationManager>()
{
    // nothing to do
}
ApplicationManager::~ApplicationManager()
{
    // nothing to do
}

Void ApplicationManager::Initialize()
{
    // Begin memory selection stack
    SelectMemory( TEXT("Scratch") );

    // Create App Instance
    CreateApp();
}
Void ApplicationManager::Cleanup()
{
    // Destroy App Instance
    DestroyApp();

    // End memory selection stack
    UnSelectMemory();
}

Int ApplicationManager::Main( Int iArgC, Char ** ppArgV )
{
    // Initialize random numbers
    RandFn->SeedAll( LowWord(SystemFn->TicksAbsolute()), 10 );

    // Create Memory Manager (no init)
    MemoryManager::Create();

    // Create & Init Application Manager
    Create();
    ApplicationManager * pThis = GetInstancePtr();
    pThis->Initialize();

    // Static Initialize
    InitCleanup::Initialize();

    // Parse command line
    CommandLine * pCmdLine = New CommandLine( (UInt)iArgC, (const AChar **)ppArgV );

    // Run Application
    Int iRetCode = pThis->RunFunction( *pCmdLine );

    // Destroy command line
    Delete ( pCmdLine );
    pCmdLine = NULL;

    // Static Cleanup
    InitCleanup::Cleanup();

    // Cleanup & Destroy Application Manager
    pThis->Cleanup();
    pThis = NULL;
    Destroy();

    // Destroy Memory Manager (no cleanup)
    MemoryManager::Destroy();

    return iRetCode;
}

