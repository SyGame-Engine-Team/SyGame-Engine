/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Application/ApplicationManager.h
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
// Header prelude
#ifndef SCARAB_ENGINE_APPLICATION_APPLICATIONMANAGER_H
#define SCARAB_ENGINE_APPLICATION_APPLICATIONMANAGER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Lib/Object/Patterns/Manager.h"

#include "Tools/InitCleanup.h"
#include "Tools/CommandLine.h"

#include "Instance/ConsoleApplication.h"
#include "Instance/WindowApplication.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define ApplicationFn ApplicationManager::GetInstancePtr()

/////////////////////////////////////////////////////////////////////////////////
// The ApplicationManager class
class ApplicationManager : public Manager<ApplicationManager>
{
public:
    inline static const GChar * GetName();
    inline static const GChar * GetMemoryBudget();

protected:
    friend class Manager<ApplicationManager>;
    ApplicationManager();
    virtual ~ApplicationManager();

public:
    Void Initialize();
    Void Cleanup();

    inline BaseApplication * GetInstance() const;

    // Entry point
    static Int Main( Int iArgC, Char ** ppArgV );

private:
    friend class BaseApplication;
    static CreateAppCallback CreateApp;
    static DestroyAppCallback DestroyApp;

    // Entry point control-passing
    RunCallback RunFunction;

    // Application instance
    BaseApplication * m_pAppInstance;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "ApplicationManager.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_APPLICATION_APPLICATIONMANAGER_H

