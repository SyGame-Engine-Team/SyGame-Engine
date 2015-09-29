/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Application/Instance/ConsoleApplication.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Base class for console applications.
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
#ifndef SCARAB_ENGINE_APPLICATION_INSTANCE_CONSOLEAPPLICATION_H
#define SCARAB_ENGINE_APPLICATION_INSTANCE_CONSOLEAPPLICATION_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "BaseApplication.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The ConsoleApplication class
class ConsoleApplication : public BaseApplication
{
protected:
    ConsoleApplication( const GChar * strTitle );
    virtual ~ConsoleApplication();

    static Int Run( const CommandLine & cmdLine );
    virtual Int Main( const CommandLine & cmdLine ) = 0;

    const GChar * m_strTitle;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "ConsoleApplication.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_APPLICATION_INSTANCE_CONSOLEAPPLICATION_H
