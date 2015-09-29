/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Console/GUICommands/CommandShowTree.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Commands : GUI, ShowTree
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
#include "CommandShowTree.h"

#include "../ConsoleManager.h"

/////////////////////////////////////////////////////////////////////////////////
// CommandShowTree implementation
CommandShowTree::CommandShowTree():
    ConsoleCommand( CONSOLECOMMAND_GUI, TEXT("showtree"), CONSOLECOMMAND_LEVEL_DEV )
{
    // Build argument list
    m_iMinArgCount = 0;
    m_iMaxArgCount = 1;
    m_iArgCount = 1;

    m_arrArguments[0].iTypes = 0;
    m_arrArguments[0].iFlags = 0;
    m_arrArguments[0].AddType( CONSOLECOMMAND_ARG_STRING );
    m_arrArguments[0].SetOptional( true );
}
CommandShowTree::~CommandShowTree()
{
    // nothing to do
}

GChar * CommandShowTree::GetHelp( GChar * strOutput ) const
{
    // Prototype & arguments
    strOutput = StringFn->Copy( strOutput, TEXT("Prototype = /gui showtree (windowname)\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  windowname = (optional) Window name\n") );

    // Description
    strOutput = StringFn->Copy( strOutput, TEXT("Description = Prints widget tree structure of given window or\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  gives a list of window names when no argument is provided\n") );

    return strOutput;
}

Bool CommandShowTree::Execute( GChar * strOutput, const GChar * strArgList )
{
    // Parse arguments
    GChar strArg0[CONSOLECOMMAND_INPUT_LENGTH];

    // Pick first argument
    strArgList = _ExtractArgument_String( strArg0, strArgList );

    // No argument case
    if ( strArgList == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Window list :\n") );
        GUIFn->EnumWindows();
        const GUIWindow * pWindow = GUIFn->EnumNextWindow();
        while( pWindow != NULL ) {
            strOutput = StringFn->Format( strOutput, TEXT("%s\n"), pWindow->GetName() );
            pWindow = GUIFn->EnumNextWindow();
        }
        return true;
    }

    // One argument case
    const GUIWindow * pWindow = GUIFn->GetWindow( strArg0 );
    if ( pWindow == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Bad parameter 0 : Invalid window name ...") );
        return false;
    }

    strOutput = StringFn->Format( strOutput, TEXT("State of %s :\n"), pWindow->GetName() );
    strOutput = StringFn->Format( strOutput, TEXT("  Window rect = (%d,%d,%d,%d)\n"), pWindow->GetWindowRect().TopLeft.X,
                                                                                      pWindow->GetWindowRect().TopLeft.Y,
                                                                                      pWindow->GetWindowRect().DownRight.X,
                                                                                      pWindow->GetWindowRect().DownRight.Y );
    strOutput = StringFn->Format( strOutput, TEXT("  Depth = %d\n"), pWindow->GetDepth() );
    strOutput = StringFn->Format( strOutput, TEXT("  Style = %x\n"), (UInt)(pWindow->GetStyle()) );
    strOutput = StringFn->Format( strOutput, TEXT("  Visible = %s\n"), (pWindow->IsVisible() ? TEXT("true") : TEXT("false")) );
    strOutput = StringFn->Format( strOutput, TEXT("  Focus = %s\n"), (pWindow->HasFocus() ? pWindow->GetFocus()->GetName() : TEXT("NULL")) );
    strOutput = StringFn->Format( strOutput, TEXT("  MouseOver = %s\n"), pWindow->HasMouseOver() ? pWindow->GetMouseOver()->GetName()
                                                                                                 : TEXT("NULL") );

    strOutput = StringFn->Copy( strOutput, TEXT("Widget tree :\n") );
    strOutput = _rec_OutputWidgetTree( strOutput, pWindow->GetClientRoot(), 0 );

    return true;
}

/////////////////////////////////////////////////////////////////////////////////

GChar * CommandShowTree::_rec_OutputWidgetTree( GChar * strOutput, const GUIWidget * pWidget, UInt iDepth ) const
{
    // Print that widget name
    for( UInt i = 0; i < iDepth; ++i )
        *(strOutput++) = TEXT('|');
    *(strOutput++) = TEXT('-');
    *(strOutput++) = TEXT(' ');
    strOutput = StringFn->Copy( strOutput, pWidget->GetName() );
    strOutput = StringFn->Format( strOutput, TEXT(" / Client rect = (%d,%d,%d,%d)\n"), pWidget->GetClientRect().TopLeft.X,
                                                                                       pWidget->GetClientRect().TopLeft.Y,
                                                                                       pWidget->GetClientRect().DownRight.X,
                                                                                       pWidget->GetClientRect().DownRight.Y );

    // Recurse on children
    pWidget->EnumChildren();
    const GUIWidget * pChild = pWidget->EnumNextChild();
    while( pChild != NULL ) {
        strOutput = _rec_OutputWidgetTree( strOutput, pChild, iDepth + 1 );
        pChild = pWidget->EnumNextChild();
    }

    return strOutput;
}

