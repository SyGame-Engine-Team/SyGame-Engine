/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Object/Patterns/VirtualHolder.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Generic interface for statically allocated abstract classes
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : Most apreciated way to bypass limitations and pseudo-instanciate
//              abstract interfaces and pure-virtual methods dereferencing ...
//              WITHOUT using dynamic memory ! Specifying the biggest child
//              class should never be a problem where this is needed ... ;)
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_LIB_OBJECT_PATTERNS_VIRTUALHOLDER_H
#define SCARAB_LIB_OBJECT_PATTERNS_VIRTUALHOLDER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Error/ErrorManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The VirtualHolder class
template<typename T, UInt VH_MAX_SIZE>
class VirtualHolder
{
protected:
    VirtualHolder();
    virtual ~VirtualHolder();

    template<typename DerivedT>
    inline Void Instanciate();
    template<typename DerivedT>
    inline Void Instanciate( const DerivedT & rhs );

    inline T & GetInstance();
    inline const T & GetInstance() const;

    template<typename DerivedT>
    inline DerivedT & GetDerivedInstance();
    template<typename DerivedT>
    inline const DerivedT & GetDerivedInstance() const;

private:
    T * m_pInstance;
    Byte m_arrInstanceBytes[sizeof(T) + VH_MAX_SIZE];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "VirtualHolder.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_OBJECT_PATTERNS_VIRTUALHOLDER_H

