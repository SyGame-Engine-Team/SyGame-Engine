/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Console/ConsoleCommand.cpp
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
// Includes
#include "ConsoleCommand.h"

#include "ConsoleManager.h"

/////////////////////////////////////////////////////////////////////////////////
// ConsoleCommand implementation
ConsoleCommand::ConsoleCommand( ConsoleCommandCategory iCategory, const GChar * strName, ConsoleCommandLevel iLevel )
{
    m_iCategory = iCategory;
    StringFn->NCopy( m_strName, strName, CONSOLECOMMAND_NAME_LENGTH - 1 );

    m_iLevel = iLevel;

    m_iMinArgCount = 0;
    m_iMaxArgCount = 0;
    m_iArgCount = 0;
}
ConsoleCommand::~ConsoleCommand()
{
    // nothing to do
}

const GChar * ConsoleCommand::GetCategoryName( ConsoleCommandCategory iCategory )
{
    switch( iCategory ) {
        case CONSOLECOMMAND_GLOBAL:    return TEXT("<global>");
        case CONSOLECOMMAND_RESOURCE:  return TEXT("resource");
        case CONSOLECOMMAND_RENDERING: return TEXT("rendering");
        case CONSOLECOMMAND_MODELING:  return TEXT("modeling");
        case CONSOLECOMMAND_PHYSICS:   return TEXT("physics");
        case CONSOLECOMMAND_WORLD:     return TEXT("world");
        case CONSOLECOMMAND_INPUT:     return TEXT("input");
        case CONSOLECOMMAND_GUI:       return TEXT("gui");
        default: Assert(false); return NULL;
    }
}
ConsoleCommandCategory ConsoleCommand::ParseCategory( const GChar * strCategoryName )
{
    // Simple search
    ConsoleCommandCategory iCategory;
    for( UInt i = 1; i < CONSOLECOMMAND_CATEGORY_COUNT; ++i ) {
        iCategory = (ConsoleCommandCategory)i;
        if ( StringFn->Cmp(GetCategoryName(iCategory), strCategoryName) == 0 )
            return iCategory;
    }

    // Not found
    return CONSOLECOMMAND_GLOBAL;
}

/////////////////////////////////////////////////////////////////////////////////

const GChar * ConsoleCommand::_ExtractArgument_Integer( Int * outInteger, const GChar * strArgList )
{
    if ( strArgList == NULL || *strArgList == NULLBYTE )
        return NULL;

    const GChar * strArgEnd = NULL;
    *outInteger = (Int)( StringFn->ToInt(strArgList, &strArgEnd) );
    if ( strArgEnd == strArgList )
        return NULL;
    return strArgEnd;
}
const GChar * ConsoleCommand::_ExtractArgument_Scalar( Scalar * outScalar, const GChar * strArgList )
{
    if ( strArgList == NULL || *strArgList == NULLBYTE )
        return NULL;

    const GChar * strArgEnd = NULL;
    *outScalar = (Scalar)( StringFn->ToFloat(strArgList, &strArgEnd) );
    if ( strArgEnd == strArgList )
        return NULL;
    return strArgEnd;
}
const GChar * ConsoleCommand::_ExtractArgument_String( GChar * outString, const GChar * strArgList )
{
    if ( strArgList == NULL || *strArgList == NULLBYTE )
        return NULL;

    GChar chEndingChar = TEXT(' '); 
    if ( *strArgList == TEXT('"') ) {
        chEndingChar = TEXT('"'); 
        ++strArgList;
    }

    if ( *strArgList == NULLBYTE || *strArgList == chEndingChar )
        return NULL;

    const GChar * strArgEnd = strArgList;
    while( *strArgEnd != NULLBYTE ) {
        if ( *strArgEnd == chEndingChar )
            break;
        ++strArgEnd;
    }

    UInt iLength = Min<UInt>( (strArgEnd - strArgList), CONSOLECOMMAND_OUTPUT_LENGTH - 1 );
    StringFn->NCopy( outString, strArgList, iLength );

    if ( *strArgEnd != NULLBYTE )
        ++strArgEnd;
    return strArgEnd;
}

