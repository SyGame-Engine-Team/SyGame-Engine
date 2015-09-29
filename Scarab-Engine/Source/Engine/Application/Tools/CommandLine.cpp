/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Application/Tools/CommandLine.cpp
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
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "CommandLine.h"

/////////////////////////////////////////////////////////////////////////////////
// CommandLine implementation
CommandLine::CommandLine( UInt iArgCount, const AChar ** arrArguments )
{
    Assert( iArgCount <= CMDLINE_MAX_ARGUMENTS );

    UInt iArg;
    GChar * strArg;

    // Argument list
    m_iArgCount = iArgCount;
    for( iArg = 0; iArg < m_iArgCount; ++iArg )
        StringFn->AsciiToWideChar( m_arrArguments[iArg], arrArguments[iArg], CMDLINE_MAX_ARGUMENT_LENGTH - 1 );

    // Program Name
    m_strProgName = m_arrArguments[0];

    // Option Count
    m_iOptionCount = 0;
    for( iArg = 1; iArg < m_iArgCount; ++iArg ) {
        strArg = m_arrArguments[iArg];
        if ( strArg[0] == TEXT('-') )
            ++m_iOptionCount;
    }
    Assert( m_iOptionCount <= CMDLINE_MAX_OPTIONS );

    // Option parameters
    iArg = 1;
    UInt iOpt = 0;
    UInt iState = 0;
    while( iState != INVALID_OFFSET ) {
        switch(iState) {
            case 0:
                if ( iArg < m_iArgCount ) {
                    strArg = m_arrArguments[iArg];
                    Assert( strArg[0] == TEXT('-') );
                    m_arrOptions[iOpt].strName = (strArg + 1);
                    m_arrOptions[iOpt].iArgCount = 0;
                    m_arrOptions[iOpt].arrArguments[0] = NULL;
                    ++iArg;
                    iState = 1;
                } else {
                    Assert( iOpt == m_iOptionCount );
                    iState = INVALID_OFFSET;
                }
                break;
            case 1:
                if ( iArg < m_iArgCount ) {
                    strArg = m_arrArguments[iArg];
                    if ( strArg[0] == TEXT('-') ) {
                        iArgCount = m_arrOptions[iOpt].iArgCount;
                        if ( iArgCount > 0 ) {
                            Assert( iArgCount <= CMDLINE_MAX_OPTION_ARGUMENTS );
                            for( UInt i = 0; i < iArgCount; ++i )
                                m_arrOptions[iOpt].arrArguments[i] = m_arrArguments[iArg - iArgCount + i];
                        }
                        ++iOpt;
                        iState = 0;
                    } else {
                        ++(m_arrOptions[iOpt].iArgCount);
                        ++iArg;
                        iState = 1;
                    }
                } else {
                    iArgCount = m_arrOptions[iOpt].iArgCount;
                    if ( iArgCount > 0 ) {
                        Assert( iArgCount <= CMDLINE_MAX_OPTION_ARGUMENTS );
                        for( UInt i = 0; i < iArgCount; ++i )
                            m_arrOptions[iOpt].arrArguments[i] = m_arrArguments[iArg - iArgCount + i];
                    }
                    ++iOpt;
                    Assert( iOpt == m_iOptionCount );
                    iState = INVALID_OFFSET;
                }
                break;
        }
    }
}
CommandLine::~CommandLine()
{
    // nothing to do
}

UInt CommandLine::GetOption( const GChar * strName ) const
{
    for( UInt i = 0; i < m_iOptionCount; ++i ) {
        if ( StringFn->Cmp(m_arrOptions[i].strName, strName) == 0 )
            return i;
    }
    return INVALID_OFFSET;
}

Bool CommandLine::GetBool( UInt iOption, UInt iArg ) const
{
    Assert( iOption < m_iOptionCount );
    Assert( iArg < m_arrOptions[iOption].iArgCount );
    const GChar * strArg = m_arrOptions[iOption].arrArguments[iArg];

    if ( StringFn->Cmp(strArg, TEXT("true")) == 0 )
        return true;
    else if ( StringFn->Cmp(strArg, TEXT("false")) == 0 )
        return false;
    else {
        Assert(false);
        return false;
    }
}
Int CommandLine::GetInteger( UInt iOption, UInt iArg ) const
{
    Assert( iOption < m_iOptionCount );
    Assert( iArg < m_arrOptions[iOption].iArgCount );
    const GChar * strArg = m_arrOptions[iOption].arrArguments[iArg];

    return (Int)( StringFn->ToInt(strArg) );
}
Float CommandLine::GetFloat( UInt iOption, UInt iArg ) const
{
    Assert( iOption < m_iOptionCount );
    Assert( iArg < m_arrOptions[iOption].iArgCount );
    const GChar * strArg = m_arrOptions[iOption].arrArguments[iArg];

    return (Scalar)( StringFn->ToFloat(strArg) );
}
const GChar * CommandLine::GetString( UInt iOption, UInt iArg ) const
{
    Assert( iOption < m_iOptionCount );
    Assert( iArg < m_arrOptions[iOption].iArgCount );
    return m_arrOptions[iOption].arrArguments[iArg];
}

