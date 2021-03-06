/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Console/ResourceCommands/CommandCreate.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Commands : Resource, Create
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
#ifndef SCARAB_ENGINE_CONSOLE_RESOURCECOMMANDS_COMMANDCREATE_H
#define SCARAB_ENGINE_CONSOLE_RESOURCECOMMANDS_COMMANDCREATE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Resource/ResourceManager.h"

#include "../ConsoleCommand.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The CommandCreate class
class CommandCreate : public ConsoleCommand
{
public:
    CommandCreate();
    virtual ~CommandCreate();

    // Informations
    virtual GChar * GetHelp( GChar * strOutput ) const;

    // Execution
    virtual Bool Execute( GChar * strOutput, const GChar * strArgList );
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "CommandCreate.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_CONSOLE_RESOURCECOMMANDS_COMMANDCREATE_H

