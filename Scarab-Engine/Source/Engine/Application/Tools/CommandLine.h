/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Application/Tools/CommandLine.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Simple container to manage command line arguments.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : A Command Line is a space-separated string of the form :
//              "progname option0 option1 option2 ... optionN"
//              optionN = "-optionname arg0 arg1 arg2 ... argN"
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_APPLICATION_TOOLS_COMMANDLINE_H
#define SCARAB_ENGINE_APPLICATION_TOOLS_COMMANDLINE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Error/ErrorManager.h"

#include "../../../Lib/Math/Formats/Scalar.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define CMDLINE_MAX_ARGUMENTS       128
#define CMDLINE_MAX_ARGUMENT_LENGTH 256
#define CMDLINE_MAX_OPTIONS          16
#define CMDLINE_MAX_OPTION_ARGUMENTS 8

/////////////////////////////////////////////////////////////////////////////////
// The CommandLine class
class CommandLine
{
public:
    CommandLine( UInt iArgCount, const AChar ** arrArguments );
    ~CommandLine();

    inline const GChar * GetProgramName() const;

    inline UInt GetOptionCount() const;
    UInt GetOption( const GChar * strName ) const;

    inline const GChar * GetName( UInt iOption ) const;
    inline UInt GetArgCount( UInt iOption ) const;

    Bool GetBool( UInt iOption, UInt iArg ) const;
    Int GetInteger( UInt iOption, UInt iArg ) const;
    Float GetFloat( UInt iOption, UInt iArg ) const;
    const GChar * GetString( UInt iOption, UInt iArg ) const;

private:
    typedef struct _cmdline_option
    {
        const GChar * strName;
        UInt iArgCount;
        const GChar * arrArguments[CMDLINE_MAX_OPTION_ARGUMENTS];
    } CmdLineOption;

    UInt m_iArgCount;
    typedef GChar ArgumentString[CMDLINE_MAX_ARGUMENT_LENGTH];
    ArgumentString m_arrArguments[CMDLINE_MAX_ARGUMENTS];

    const GChar * m_strProgName;

    UInt m_iOptionCount;
    CmdLineOption m_arrOptions[CMDLINE_MAX_OPTIONS];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "CommandLine.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_APPLICATION_TOOLS_COMMANDLINE_H

