/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Console/GlobalCommands/CommandVar.cpp
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
// Includes
#include "CommandVar.h"

#include "../ConsoleManager.h"

/////////////////////////////////////////////////////////////////////////////////
// CommandVar implementation
CommandVar::CommandVar():
    ConsoleCommand( CONSOLECOMMAND_GLOBAL, TEXT("var"), CONSOLECOMMAND_LEVEL_DEV )
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
CommandVar::~CommandVar()
{
    // nothing to do
}

GChar * CommandVar::GetHelp( GChar * strOutput ) const
{
    // Prototype & arguments
    strOutput = StringFn->Copy( strOutput, TEXT("Prototype = /var (varname)\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  varname = (optional) Variable name\n") );

    // Description
    strOutput = StringFn->Copy( strOutput, TEXT("Description = Prints varname infos or variable list\n") );

    return strOutput;
}

Bool CommandVar::Execute( GChar * strOutput, const GChar * strArgList )
{
    // Get console environment
    ConsoleEnvironment * pEnv = ConsoleFn->GetEnvironment();

    // Parse arguments
    GChar strArg0[CONSOLECOMMAND_OUTPUT_LENGTH];

    // Pick first argument
    strArgList = _ExtractArgument_String( strArg0, strArgList );

    // No argument case
    if ( strArgList == NULL ) {
        strOutput = StringFn->Format( strOutput, TEXT("Variable list (%d) :\n"), pEnv->GetVariableCount() );
        pEnv->EnumVariables();
        const ConsoleVariable * pVar = pEnv->EnumNextVariable();
        while( pVar != NULL ) {
            strOutput = StringFn->Format( strOutput, TEXT("(%s) %s = %8x\n"), ConsoleVariable::GetTypeName(pVar->GetType()),
                                                                      pVar->GetName(), pVar->GetValue() );
            pVar = pEnv->EnumNextVariable();
        }

        return true;
    }

    // One argument case
    const ConsoleVariable * pVar = pEnv->GetVariable( strArg0 );
    if ( pVar == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Bad parameter 0 : Invalid varname ...") );
        return false;
    }

    ConsoleVariableType iType = pVar->GetType();
    Void * pValue = pVar->GetValue();

    strOutput = StringFn->Format( strOutput, TEXT("Inspecting variable %s :\n"), pVar->GetName() );
    strOutput = StringFn->Format( strOutput, TEXT(" -> Type = %s\n"), ConsoleVariable::GetTypeName(iType) );
    if ( pValue == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT(" -> Value = NULL") );
        return true;
    }
    strOutput = StringFn->Format( strOutput, TEXT(" -> Value = %8x\n"), pValue );
    if ( iType == CONSOLEVAR_TYPE_UNDEFINED )
        return true;

    switch( iType ) {
        case CONSOLEVAR_TYPE_TEXTURE: {
                const GPUTexture * pTexture = (const GPUTexture *)pValue;
                strOutput = StringFn->Format( strOutput, TEXT(" -> Texture.Type       = %d\n"), pTexture->GetType() );
                strOutput = StringFn->Format( strOutput, TEXT(" -> Texture.Format     = %d\n"), pTexture->GetFormat() );
                strOutput = StringFn->Format( strOutput, TEXT(" -> Texture.Usage      = %d\n"), pTexture->GetUsage() );
                strOutput = StringFn->Format( strOutput, TEXT(" -> Texture.MipMaps    = %d\n"), pTexture->GetMipLevelCount() );
                strOutput = StringFn->Format( strOutput, TEXT(" -> Texture.TotalBytes = %d\n"), pTexture->GetSize() );
                strOutput = StringFn->Format( strOutput, TEXT(" -> Texture.Stride     = %d\n"), pTexture->GetStride() );
                strOutput = StringFn->Format( strOutput, TEXT(" -> Texture.Width      = %d\n"), pTexture->GetMipLevelBound(0,0) );
                if ( pTexture->GetType() != GPURESOURCE_TEXTURE_1D )
                    strOutput = StringFn->Format( strOutput, TEXT(" -> Texture.Height     = %d\n"), pTexture->GetMipLevelBound(0,1) );
                if ( pTexture->GetType() == GPURESOURCE_TEXTURE_3D )
                    strOutput = StringFn->Format( strOutput, TEXT(" -> Texture.Depth      = %d\n"), pTexture->GetMipLevelBound(0,2) );
            } break;
        default: Assert(false); return false;
    }

    return true;
}

