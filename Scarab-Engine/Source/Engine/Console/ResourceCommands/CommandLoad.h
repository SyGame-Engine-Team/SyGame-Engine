/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Console/ResourceCommands/CommandLoad.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Commands : Resource, Load
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
#ifndef SCARAB_ENGINE_CONSOLE_RESOURCECOMMANDS_COMMANDLOAD_H
#define SCARAB_ENGINE_CONSOLE_RESOURCECOMMANDS_COMMANDLOAD_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Resource/ResourceManager.h"

#include "../ConsoleCommand.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The CommandLoad class
class CommandLoad : public ConsoleCommand
{
public:
    CommandLoad();
    virtual ~CommandLoad();

    // Informations
    virtual GChar * GetHelp( GChar * strOutput ) const;

    // Execution
    virtual Bool Execute( GChar * strOutput, const GChar * strArgList );
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "CommandLoad.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_CONSOLE_RESOURCECOMMANDS_COMMANDLOAD_H

