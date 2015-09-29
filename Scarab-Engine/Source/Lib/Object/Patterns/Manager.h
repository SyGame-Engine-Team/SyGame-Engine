/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Object/Patterns/Manager.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Generic singleton abstractor for the Manager pattern.
//               Integrated memory management.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : Use : class MyManager : public Manager<MyManager> {};
//              A manager MUST provide 2 static functions :
//              // Returns the manager unique name
//              static const GChar * GetName();
//              // Returns a null-terminated string holding a list of allocator
//              // declarations in the form :
//              // "Decl0; Decl1; ...; DeclN;"
//              // Decl = Type:Name(Arg0, Arg1, ..., ArgN)
//              // Type = ( 'Stack', 'Heap', 'Pool', 'Break' )
//              // Args = Stack => 1, stack size
//              //        Heap => 1, heap size
//              //        Pool => 2, chunk size, chunk count
//              //        Break => 2, block size, range size   (All UInts)
//              static const GChar * GetMemoryBudget();
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_LIB_OBJECT_PATTERNS_MANAGER_H
#define SCARAB_LIB_OBJECT_PATTERNS_MANAGER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Memory/MemoryManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define MANAGER_MAX_ALLOCATORS 256

/////////////////////////////////////////////////////////////////////////////////
// The Manager class
template<class T>
class Manager
{
public:
    static Void Create();
    static Void Destroy();

    inline static T & GetInstance();
    inline static T * GetInstancePtr();

    inline Void SelectMemory( const GChar * strName ) const;
    inline Void UnSelectMemory() const;

    inline static MemoryContextID GetMemoryContext();

protected:
    Manager();
    virtual ~Manager();

private:
    static T * sm_pInstance;

    static MemoryContextID sm_idMemoryContext;

    // Memory Budget
    Void _CreateMemoryBudget( const GChar * strMemoryBudget );
    Void _ParseBudgetDeclaration( const GChar * strDecl );
    Void _DestroyMemoryBudget();

    UInt m_iAllocatorCount;
    MemoryAllocatorID m_arrAllocators[MANAGER_MAX_ALLOCATORS];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Manager.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_OBJECT_PATTERNS_MANAGER_H

