/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Console/GlobalCommands/CommandVarSet.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Commands : Global, VarSet
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
#ifndef SCARAB_ENGINE_CONSOLE_GLOBALCOMMANDS_COMMANDVARSET_H
#define SCARAB_ENGINE_CONSOLE_GLOBALCOMMANDS_COMMANDVARSET_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../ConsoleCommand.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The CommandVarSet class
class CommandVarSet : public ConsoleCommand
{
public:
    CommandVarSet();
    virtual ~CommandVarSet();

    // Informations
    virtual GChar * GetHelp( GChar * strOutput ) const;

    // Execution
    virtual Bool Execute( GChar * strOutput, const GChar * strArgList );
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "CommandVarSet.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_CONSOLE_GLOBALCOMMANDS_COMMANDVARSET_H

