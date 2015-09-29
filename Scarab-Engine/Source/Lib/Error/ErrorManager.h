/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Error/ErrorManager.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : A general purpose manager for error tracking
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : Independant managers = None
//              Dependant managers = Everything
//              No error report here, error handling system MUST be failsafe !
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_LIB_ERROR_ERRORMANAGER_H
#define SCARAB_LIB_ERROR_ERRORMANAGER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../ThirdParty/System/System.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define ErrorFn ErrorManager::GetInstancePtr()

// Macro interface
#define ErrorContextDeclare( _identifier ) \
    extern const ErrorContextID _identifier
#define ErrorContextDefine( _identifier, _name ) \
    const ErrorContextID _identifier = ErrorFn->CreateContext( _name )

#define ErrorDeclare( _identifier ) \
    extern const ErrorCode _identifier
#define ErrorDefine( _context, _identifier, _msg ) \
    const ErrorCode _identifier = ErrorFn->CreateErrorCode( _context, _msg )

// DebugPrint wrapper
#define DebugPrint( _string, ... ) \
    ErrorFn->_DebugPrint( _string, __VA_ARGS__ )

// Throw wrapper
#define Throw( _errorcode ) \
    ErrorFn->_Throw( _errorcode, TEXT(__FILE__), __LINE__ )

// Assert wrapper
#define ASSERT_ENABLED // Comment for release builds
#define ASSERT_BREAK   // Comment for release builds
#define ASSERT_PRINT   // Comment for release builds

#ifdef ASSERT_ENABLED
    #ifdef ASSERT_BREAK
        #ifdef ASSERT_PRINT
            #define Assert( _expr ) \
                if ( _expr ) {} else { \
                    ErrorFn->_DebugPrint( TEXT("Assertion failed => %s \n"), TEXT(#_expr) ); \
                    ErrorFn->_DebugPrint( TEXT("  \t in file : %s \n"), TEXT(__FILE__) ); \
                    ErrorFn->_DebugPrint( TEXT("  \t at line : %d \n\n"), __LINE__ ); \
                    ErrorFn->_Assert( TEXT(#_expr), TEXT(__FILE__), __LINE__ ); \
                    DebugBreak; \
                }
        #else
            #define Assert( _expr ) \
                if ( _expr ) {} else { \
                    ErrorFn->_Assert( TEXT(#_expr), TEXT(__FILE__), __LINE__ ); \
                    DebugBreak; \
                }
        #endif
    #else
        #ifdef ASSERT_PRINT
            #define Assert( _expr ) \
                if ( _expr ) {} else { \
                    ErrorFn->_DebugPrint( TEXT("Assertion failed => %s \n"), TEXT(#_expr) ); \
                    ErrorFn->_DebugPrint( TEXT("  \t in file : %s \n"), TEXT(__FILE__) ); \
                    ErrorFn->_DebugPrint( TEXT("  \t at line : %d \n\n"), __LINE__ ); \
                    ErrorFn->_Assert( TEXT(#_expr), TEXT(__FILE__), __LINE__ ); \
                }
        #else
            #define Assert( _expr ) \
                if ( _expr ) {} else { \
                    ErrorFn->_Assert( TEXT(#_expr), TEXT(__FILE__), __LINE__ ); \
                }
        #endif
    #endif
#else
    #define Assert( _expr )
#endif

// Error manager definitions
typedef DWord ErrorCode;

#define ERROR_MAX_CONTEXTS   64
#define ERROR_CONTEXT_SHARED (ERROR_MAX_CONTEXTS + 1) // arbitrary value > ERROR_MAX_CONTEXTS

typedef DWord ErrorContextID;
#define ERROR_CONTEXT_MAX_NAMELENGTH    64
#define ERROR_CONTEXT_MAX_ERRORCODES    128
#define ERROR_CONTEXT_MAX_MESSAGELENGTH 128 // should be enough for anything ...

typedef GChar ErrorMessage[ERROR_CONTEXT_MAX_MESSAGELENGTH];

typedef struct _error_context
{
    ErrorContextID idContext;
    GChar strName[ERROR_CONTEXT_MAX_NAMELENGTH];

    ErrorMessage arrMessages[ERROR_CONTEXT_MAX_ERRORCODES];
    UInt iNextFreeCode;
} ErrorContext;

// Logging definitions
#define ERROR_LOGFILE_THROWS  TEXT("Logs/Errors.log")
#define ERROR_LOGFILE_ASSERTS TEXT("Logs/Asserts.log")

// Standard shared codes
ErrorDeclare( ERROR_OK );
ErrorDeclare( ERROR_UNKNOWN );
ErrorDeclare( ERROR_BAD_ARGUMENT );
ErrorDeclare( ERROR_BAD_RESULT );
ErrorDeclare( ERROR_OUT_OF_MEMORY );

/////////////////////////////////////////////////////////////////////////////////
// The ErrorManager class
class ErrorManager
{
    // Discrete singleton interface
public:
    inline static ErrorManager * GetInstancePtr();

private:
    ErrorManager();
    ~ErrorManager();

public:
    // Context declarator
    ErrorContextID CreateContext( const GChar * strName );

    // Error codes declarator
    ErrorCode CreateErrorCode( ErrorContextID idContext, const GChar * strErrorMsg );

    // Error message retriever
    const GChar * GetContextName( ErrorCode iErrorCode );
    const GChar * GetErrorMessage( ErrorCode iErrorCode );

    // Logging support
    Void _DebugPrint( const GChar * strDebugMessage, ... );                             // don't use without macro
    Void _Throw( ErrorCode iErrorCode, const GChar * strFileName, UInt iLineNumber );      // don't use without macro
    Void _Assert( const GChar * strExpr, const GChar * strFileName, UInt iLineNumber ); // don't use without macro
    Void Log( const GChar * strLogFile, const GChar * strFormat, ... );
    Void Log( const HFile & hLogFile, const GChar * strFormat, ... );

private:
    // Little helpers
    inline ErrorContext * _GetContext( ErrorContextID idContext );

    // Shared errors
    ErrorContext m_errorSharedContext;

    // Contexts
    ErrorContext m_arrContexts[ERROR_MAX_CONTEXTS];
    ErrorContextID m_iNextFreeContext;

    // Logging buffer
    static GChar sm_strLogBuffer[4096];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "ErrorManager.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_ERROR_ERRORMANAGER_H

