/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Gameplay/ThreatTable.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Gameplay : ThreatTable
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENTRYPOINT_SIMPLERPG_GAMEPLAY_THREATTABLE_H
#define SCARAB_ENTRYPOINT_SIMPLERPG_GAMEPLAY_THREATTABLE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Engine/Application/ApplicationManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define THREAT_GROW_VALUE   200
#define THREAT_SHRINK_VALUE 100

#define THREATTABLE_MAX_RECORDS 256
#define THREATTABLE_ROOT 1

// Prototypes
class BaseCharacter;

/////////////////////////////////////////////////////////////////////////////////
// The ThreatTable class
class ThreatTable
{
public:
    ThreatTable();
    ~ThreatTable();

    inline UInt GetThreatRecordCount() const;
    inline BaseCharacter * GetThreatRecord( UInt iIndex, UInt * outThreat = NULL, Bool * outMark = NULL ) const;

    inline Bool HasThreat( BaseCharacter * pCharacter ) const;
    inline UInt GetThreat( BaseCharacter * pCharacter ) const;

    inline BaseCharacter * GetBiggestThreat( UInt * outThreat = NULL ) const;

    UInt AddThreat( BaseCharacter * pCharacter, UInt iThreat, Bool bMark = false );
    UInt RemoveThreat( BaseCharacter * pCharacter, UInt iThreat, Bool bMark = false );

    inline Void ResetThreat();

    inline Bool HasMark( BaseCharacter * pCharacter ) const;
    Void ResetMarks();

private:
    typedef struct _threat_record {
        inline struct _threat_record & operator=( const struct _threat_record & rhs );

        BaseCharacter * pTarget;
        UInt iThreat;
        UInt iNode;
        Bool bMark;
    } _ThreatRecord;
    typedef TreeMap<BaseCharacter*,_ThreatRecord> _ThreatMap;

    inline static Int _Compare_BaseChars( BaseCharacter * const & rLeft, BaseCharacter * const & rRight, Void * pUserData );

    Void _Heap_Insert( _ThreatRecord * pThreatRecord );
    Void _Heap_Remove( UInt iNode );
    Void _Heap_UpdateUp( UInt iNode );
    Void _Heap_UpdateDown( UInt iNode );

    UInt m_iRecordCount;
    _ThreatRecord * m_arrRecords[THREATTABLE_MAX_RECORDS + 1];
    _ThreatMap m_hThreatMap;
};


/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "ThreatTable.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_SIMPLERPG_GAMEPLAY_THREATTABLE_H

