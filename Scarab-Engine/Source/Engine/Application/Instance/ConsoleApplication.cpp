/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Application/Instance/ConsoleApplication.cpp
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
// Includes
#include "ConsoleApplication.h"

#include "../ApplicationManager.h"

/////////////////////////////////////////////////////////////////////////////////
// ConsoleApplication implementation
ConsoleApplication::ConsoleApplication( const GChar * strTitle ):
    BaseApplication()
{
    m_strTitle = strTitle;
}
ConsoleApplication::~ConsoleApplication()
{
    // nothing to do
}

Int ConsoleApplication::Run( const CommandLine & cmdLine )
{
    ConsoleApplication * pAppInst = (ConsoleApplication*)( ApplicationFn->GetInstance() );
    return pAppInst->Main( cmdLine );
}
