/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Application/Instance/BaseApplication.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Abstract base class for all applications
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
#ifndef SCARAB_ENGINE_APPLICATION_INSTANCE_BASEAPPLICATION_H
#define SCARAB_ENGINE_APPLICATION_INSTANCE_BASEAPPLICATION_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Tools/CommandLine.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
    // Callbacks
typedef Void (*CreateAppCallback)();
typedef Void (*DestroyAppCallback)();
typedef Int (*RunCallback)( const CommandLine & );

    // Macro interface
        // Header, Inside class body
#define DeclareApplication \
    public:  static Bool _RegisterApplication(); \
             static Void _Create(); \
             static Void _Destroy(); \
    private: static Bool sm_bAppRegistered;

        // Header, After class body (outside)
#define RegisterApplication( _class ) \
    static Bool sg_bAppRegistered_##_class = _class::_RegisterApplication();

        // Source, for ConsoleApplication apps
#define DefineConsoleApplication( _class ) \
    Bool _class::sm_bAppRegistered = false; \
    Bool _class::_RegisterApplication() { \
        if ( !sm_bAppRegistered ) { \
            BaseApplication::_SetCreateDestroy( &_Create, &_Destroy ); \
            sm_bAppRegistered = true; \
        } \
        return sm_bAppRegistered; \
    } \
    Void _class::_Create() { \
        BaseApplication::_SetRunCallback( &(ConsoleApplication::Run) ); \
        BaseApplication::_SetAppInstance( (BaseApplication*)( New _class() ) ); \
    } \
    Void _class::_Destroy() { \
        BaseApplication::_DeleteAppInstance(); \
    }

        // Source, for WindowApplication apps
#define DefineWindowApplication( _class ) \
    Bool _class::sm_bAppRegistered = false; \
    Bool _class::_RegisterApplication() { \
        if ( !sm_bAppRegistered ) { \
            BaseApplication::_SetCreateDestroy( &_Create, &_Destroy ); \
            sm_bAppRegistered = true; \
        } \
        return sm_bAppRegistered; \
    } \
    Void _class::_Create() { \
        BaseApplication::_SetRunCallback( &(WindowApplication::Run) ); \
        BaseApplication::_SetAppInstance( (BaseApplication*)( New _class() ) ); \
    } \
    Void _class::_Destroy() { \
        BaseApplication::_DeleteAppInstance(); \
    }

/////////////////////////////////////////////////////////////////////////////////
// The BaseApplication class
class BaseApplication
{
protected:
    BaseApplication();
    virtual ~BaseApplication();

    // Application instanciation system
    static Void _SetCreateDestroy( CreateAppCallback pfCreate, DestroyAppCallback pfDestroy );

    static Void _SetRunCallback( RunCallback pfRun );
    static Void _SetAppInstance( BaseApplication * pAppInstance );

    static Void _DeleteAppInstance();
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "BaseApplication.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_APPLICATION_INSTANCE_BASEAPPLICATION_H

