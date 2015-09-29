/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Application/Tools/InitCleanup.h
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
// Header prelude
#ifndef SCARAB_ENGINE_APPLICATION_TOOLS_INITCLEANUP_H
#define SCARAB_ENGINE_APPLICATION_TOOLS_INITCLEANUP_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Error/ErrorManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define INITCLEANUP_MAX_CALLBACKS 256

typedef Void (*Initializer)();
typedef Void (*Cleaner)();

// Macro interface
#define DECLARE_INITIALIZER \
    public:  static Bool SetInitializer(); \
             static Void Initialize(); \
    private: static Bool sm_bInitializerSet;
#define DECLARE_CLEANER \
    public:  static Bool SetCleaner(); \
             static Void Cleanup(); \
    private: static Bool sm_bCleanerSet;

#define DEFINE_INITIALIZER(_class) \
    Bool _class::sm_bInitializerSet = false; \
    Bool _class::SetInitializer() { \
        if ( !sm_bInitializerSet ) { \
            InitCleanup::AddInitializer(_class::Initialize); \
            sm_bInitializerSet = true; \
        } \
        return sm_bInitializerSet; \
    }
#define DEFINE_CLEANER(_class) \
    Bool _class::sm_bCleanerSet = false; \
    Bool _class::SetCleaner() { \
        if ( !sm_bCleanerSet ) { \
            InitCleanup::AddCleaner(_class::Cleanup); \
            sm_bCleanerSet = true; \
        } \
        return sm_bCleanerSet; \
    }

#define ENABLE_INITIALIZER(_class) \
    static Bool sg_bSetInitializer_##_class = _class::SetInitializer();
#define ENABLE_CLEANER(_class) \
    static Bool sg_bSetCleaner_##_class = _class::SetCleaner();

/////////////////////////////////////////////////////////////////////////////////
// The InitCleanup class
class InitCleanup
{
public:
    static Void AddInitializer( Initializer pfCallback );
    static Void AddCleaner( Cleaner pfCallback );

    static Void Initialize();
    static Void Cleanup();

private:
    InitCleanup();
    ~InitCleanup();

    static UInt sm_InitializerCount;
    static Initializer sm_arrInitializers[INITCLEANUP_MAX_CALLBACKS];

    static UInt sm_CleanerCount;
    static Cleaner sm_arrCleaners[INITCLEANUP_MAX_CALLBACKS];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "InitCleanup.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_APPLICATION_TOOLS_INITCLEANUP_H

