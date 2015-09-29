/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Console/ConsoleCommand.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Base class for commands & queries abstraction layer ...
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
#ifndef SCARAB_ENGINE_CONSOLE_CONSOLECOMMAND_H
#define SCARAB_ENGINE_CONSOLE_CONSOLECOMMAND_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Lib/Error/ErrorManager.h"

#include "../../Lib/Math/Formats/Scalar.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

    // Magic char
#define CONSOLECOMMAND_MAGIC_CHAR TEXT('/')

    // Command strings lengths
#define CONSOLECOMMAND_NAME_LENGTH 16

#define CONSOLECOMMAND_INPUT_LENGTH  GUITEXTINPUT_MAX_LENGTH
#define CONSOLECOMMAND_OUTPUT_LENGTH GUITEXTLOG_LINE_LENGTH

    // Command categories
enum ConsoleCommandCategory
{
    CONSOLECOMMAND_GLOBAL = 0,

    CONSOLECOMMAND_RESOURCE,

    CONSOLECOMMAND_RENDERING,
    CONSOLECOMMAND_MODELING,
    CONSOLECOMMAND_PHYSICS,
    CONSOLECOMMAND_WORLD,

    CONSOLECOMMAND_INPUT,
    CONSOLECOMMAND_GUI,

    CONSOLECOMMAND_CATEGORY_COUNT
};

    // Command security levels
enum ConsoleCommandLevel
{
    // Must be sorted by decreasing access
    CONSOLECOMMAND_LEVEL_DEV = 0,
    CONSOLECOMMAND_LEVEL_ADMIN,
    CONSOLECOMMAND_LEVEL_GM,
    CONSOLECOMMAND_LEVEL_PLAYER,

    CONSOLECOMMAND_LEVEL_COUNT
};

    // Command arguments
#define CONSOLECOMMAND_MAX_ARGUMENTS  8

typedef DWord ConsoleCommandArgType;
#define CONSOLECOMMAND_ARG_INTEGER    0x00000001
#define CONSOLECOMMAND_ARG_SCALAR     0x00000002
#define CONSOLECOMMAND_ARG_STRING     0x00000004
#define CONSOLECOMMAND_ARG_TYPE_COUNT 3

typedef DWord ConsoleCommandArgFlag;
#define CONSOLECOMMAND_ARG_OPTIONAL 0x80000000
#define CONSOLECOMMAND_ARG_MUTEX    0x00000100

typedef struct _consolecommand_argument
{
    inline Void AddType( ConsoleCommandArgType iType );
    inline Void RemoveType( ConsoleCommandArgType iType );

    inline Bool IsOptional() const;
    inline Void SetOptional( Bool bOptional );

    inline Bool IsMutex() const;
    inline UInt GetMutexCount() const;
    inline Void SetMutex( UInt iCount = INVALID_OFFSET );

    ConsoleCommandArgType iTypes;
    ConsoleCommandArgFlag iFlags;
} ConsoleCommandArgument;

/////////////////////////////////////////////////////////////////////////////////
// The ConsoleCommand class
class ConsoleCommand
{
public:
    ConsoleCommand( ConsoleCommandCategory iCategory, const GChar * strName, ConsoleCommandLevel iLevel );
    virtual ~ConsoleCommand();

    // Typename support
    static const GChar * GetCategoryName( ConsoleCommandCategory iCategory );
    static ConsoleCommandCategory ParseCategory( const GChar * strCategoryName );

    // Descriptor
    inline ConsoleCommandCategory GetCategory() const;
    inline const GChar * GetName() const;

    inline ConsoleCommandLevel GetLevel() const;

    inline UInt GetMinArgCount() const;
    inline UInt GetMaxArgCount() const;

    inline UInt GetArgCount() const;
    inline const ConsoleCommandArgument * GetArgument( UInt iArgument ) const;

    // Informations
    virtual GChar * GetHelp( GChar * strOutput ) const = 0;

    // Execution
    virtual Bool Execute( GChar * strOutput, const GChar * strArgList ) = 0;

protected:
    // Helpers
    static const GChar * _ExtractArgument_Integer( Int * outInteger, const GChar * strArgList );
    static const GChar * _ExtractArgument_Scalar( Scalar * outScalar, const GChar * strArgList );
    static const GChar * _ExtractArgument_String( GChar * outString, const GChar * strArgList );

    // Command descriptor
    ConsoleCommandCategory m_iCategory;
    GChar m_strName[CONSOLECOMMAND_NAME_LENGTH];

    ConsoleCommandLevel m_iLevel;

    UInt m_iMinArgCount, m_iMaxArgCount;
    UInt m_iArgCount;
    ConsoleCommandArgument m_arrArguments[CONSOLECOMMAND_MAX_ARGUMENTS];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "ConsoleCommand.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_CONSOLE_CONSOLECOMMAND_H

