/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Console/ConsoleEnvironment.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Delegate of Console manager, variables & execution state ...
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : Note = I could go a lot further using reflection but that would
//                     be overkill for performance (reflecting so much objects !)
//                     Plus having control over fixed commands is safer than
//                     allowing random method invokations and member lookups ...
//              TODO = Script conditional jumps & loops ...
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_CONSOLE_CONSOLEENVIRONMENT_H
#define SCARAB_ENGINE_CONSOLE_CONSOLEENVIRONMENT_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Lib/Datastruct/Table/TreeMap.h"

#include "../Resource/ResourceManager.h"

#include "ConsoleCommand.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

    // Variable types
enum ConsoleVariableType
{
    // Make sure you match these with ResourceType values in RDBFileStructure.h !
    CONSOLEVAR_TYPE_UNDEFINED = 0,

    // Resource types (same values)
        // Rendering types
    CONSOLEVAR_TYPE_MESH,
    CONSOLEVAR_TYPE_TEXTURE,
    //CONSOLEVAR_TYPE_LIGHT,
    //CONSOLEVAR_TYPE_MATERIAL,
    //CONSOLEVAR_TYPE_EFFECT,
    //CONSOLEVAR_TYPE_CAMERA,
        // Physics types
    //CONSOLEVAR_TYPE_SHAPE,
    //CONSOLEVAR_TYPE_BODY,
    //CONSOLEVAR_TYPE_JOINT,
    //CONSOLEVAR_TYPE_CONTROLLER,
        // World types
    //CONSOLEVAR_TYPE_WORLDCAMERA,
    //CONSOLEVAR_TYPE_WORLDLEAF,
    //CONSOLEVAR_TYPE_WORLDNODE,

    // Primitive types
    //CONSOLEVAR_TYPE_INTEGER,
    //CONSOLEVAR_TYPE_SCALAR,
    //CONSOLEVAR_TYPE_STRING,

    // Math types
    //CONSOLEVAR_TYPE_VECTOR2,
    //CONSOLEVAR_TYPE_VECTOR3,
    //CONSOLEVAR_TYPE_VECTOR4,
    //CONSOLEVAR_TYPE_MATRIX2,
    //CONSOLEVAR_TYPE_MATRIX3,
    //CONSOLEVAR_TYPE_MATRIX4,
    //CONSOLEVAR_TYPE_QUATERNION,
    //CONSOLEVAR_TYPE_TRANSFORM2,
    //CONSOLEVAR_TYPE_TRANSFORM3,

    // Total types
    CONSOLEVAR_TYPE_COUNT
};

    // Variable names
#define CONSOLEVAR_NAME_LENGTH 32

/////////////////////////////////////////////////////////////////////////////////
// The ConsoleVariable class
class ConsoleVariable
{
public:
    static const ConsoleVariable Null;

    ConsoleVariable( const GChar * strName );
    ConsoleVariable( const GChar * strName, ConsoleVariableType iType, Void * pValue );
    ~ConsoleVariable();

    // Typename support
    static const GChar * GetTypeName( ConsoleVariableType iTypeValue );
    static ConsoleVariableType GetTypeValue( const GChar * strTypeName );

    // Name access
    inline const GChar * GetName() const;

    // Type & value access
    inline ConsoleVariableType GetType() const;
    inline Void * GetValue() const;

    Void Affect( ConsoleVariableType iType, Void * pValue );
    Void Affect( ConsoleVariable * pValue );

protected:
    GChar m_strName[CONSOLEVAR_NAME_LENGTH];

    ConsoleVariableType m_iType;
    Void * m_pValue;
};

/////////////////////////////////////////////////////////////////////////////////
// The ConsoleEnvironment class
class ConsoleEnvironment
{
public:
    ConsoleEnvironment();
    ~ConsoleEnvironment();

    // Deferred creation & destruction
    Void Initialize();
    Void Cleanup();

    // Environment variables
    ConsoleVariable * CreateVariable( const GChar * strName ); // untyped var
    ConsoleVariable * CreateVariable( const GChar * strName, ConsoleVariableType iType, Void * pValue );
    Bool DestroyVariable( const GChar * strName );

    inline UInt GetVariableCount() const;
    inline ConsoleVariable * GetVariable( const GChar * strName ) const;

    inline Void EnumVariables() const;
    inline ConsoleVariable * EnumNextVariable() const;

    Bool AffectVariable( const GChar * strName, ConsoleVariableType iType, Void * pValue );
    Bool AffectVariable( const GChar * strDstName, const GChar * strSrcName );

    // Resource state data
    inline ResourceDB * GetActiveRDB() const;
    inline Void SetActiveRDB( ResourceDB * pRDB );

    inline Bool HasOpenedResource() const;
    inline RDB_ResourceHandle * GetOpenedResource();
    inline const GChar * GetOpenedResourceName() const;

    Bool OpenResource( const GChar * strResPath );
    Void CloseResource();

private:
    // Environment variables
    inline static Int _Name_Comparator( const GChar* const & rLeft, const GChar* const & rRight, Void * pUserData );
    typedef TreeMap<const GChar*, ConsoleVariable*> VariableMap;

    VariableMap m_mapVariables;
    mutable VariableMap::Iterator m_itEnumerate;

    // Resource state data
    ResourceDB * m_pActiveDB;

    Bool m_bOpenedResource;
    RDB_ResourceHandle m_hOpenedResource;
    GChar m_strOpenedResourceName[RDB_NAME_LENGTH];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "ConsoleEnvironment.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_CONSOLE_CONSOLEENVIRONMENT_H


