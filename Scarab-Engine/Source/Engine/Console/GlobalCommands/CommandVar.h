/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Console/GlobalCommands/CommandVar.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Commands : Global, Var
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
#ifndef SCARAB_ENGINE_CONSOLE_GLOBALCOMMANDS_COMMANDVAR_H
#define SCARAB_ENGINE_CONSOLE_GLOBALCOMMANDS_COMMANDVAR_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../ConsoleCommand.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The CommandVar class
class CommandVar : public ConsoleCommand
{
public:
    CommandVar();
    virtual ~CommandVar();

    // Informations
    virtual GChar * GetHelp( GChar * strOutput ) const;

    // Execution
    virtual Bool Execute( GChar * strOutput, const GChar * strArgList );
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "CommandVar.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_CONSOLE_GLOBALCOMMANDS_COMMANDVAR_H

