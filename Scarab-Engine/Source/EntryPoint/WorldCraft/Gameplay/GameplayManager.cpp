/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Gameplay/GameplayManager.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Gameplay : GameplayManager
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
#include "GameplayManager.h"

/////////////////////////////////////////////////////////////////////////////////
// GameplayManager implementation
const Scalar GameplayManager::_INV_RANDMAX = ( 1.0f / (Scalar)( RandFn->XorShiftMax() ) );

GameplayManager::GameplayManager():
    Manager<GameplayManager>(),
    m_mapCharacters(), m_mapCombats(), m_itEnumerate()
{
    // nothing to do
}
GameplayManager::~GameplayManager()
{
    // nothing to do
}

Void GameplayManager::Initialize()
{
    SelectMemory( TEXT("Scratch") );

    // StatusEffect database
    m_arrStatusEffects[STATUSEFFECTID_UNDEFINED] = NULL;

    m_arrStatusEffects[STATUSEFFECTID_CHILL] = NULL;

    m_arrStatusEffects[STATUSEFFECTID_DOT] = NULL;
    m_arrStatusEffects[STATUSEFFECTID_HOT] = NULL;
    
    m_arrStatusEffects[STATUSEFFECTID_SLOW] = NULL;
    m_arrStatusEffects[STATUSEFFECTID_ROOT] = NULL;
    m_arrStatusEffects[STATUSEFFECTID_STUN] = NULL;
    m_arrStatusEffects[STATUSEFFECTID_FEAR] = NULL;
    m_arrStatusEffects[STATUSEFFECTID_JAIL] = NULL;

    // Skill database
    m_arrSkills[SKILLID_UNDEFINED] = NULL;

    m_arrSkills[SKILLID_BASICATTACK] = NULL;
    m_arrSkills[SKILLID_BASICCAST] = NULL;

    m_arrSkills[SKILLID_WARPHOME] = NULL;
    m_arrSkills[SKILLID_REST] = NULL;

    m_arrSkills[SKILLID_HUMAN_SCIENCE] = NULL;
    m_arrSkills[SKILLID_HUMAN_CHARISM] = NULL;

    m_arrSkills[SKILLID_MAGE_FIREBOLT] = New MageSkillFireBolt();
    m_arrSkills[SKILLID_MAGE_ICEARROW] = NULL;
    m_arrSkills[SKILLID_MAGE_BLAZE] = NULL;
    m_arrSkills[SKILLID_MAGE_ICENOVA] = New MageSkillIceNova();
    m_arrSkills[SKILLID_MAGE_MAGICBLAST] = NULL;
    m_arrSkills[SKILLID_MAGE_EARTHQUAKE] = NULL;
    m_arrSkills[SKILLID_MAGE_BLIZZARD] = NULL;
    m_arrSkills[SKILLID_MAGE_BLINK] = NULL;

    // Talent database
    m_arrTalentTrees[CHARCLASS_WARRIOR] = NULL;
    m_arrTalentTrees[CHARCLASS_THIEF] = NULL;
    m_arrTalentTrees[CHARCLASS_HUNTER] = NULL;
    m_arrTalentTrees[CHARCLASS_MAGE] = NULL;
    m_arrTalentTrees[CHARCLASS_WARLOCK] = NULL;
    m_arrTalentTrees[CHARCLASS_PRIEST] = NULL;
    m_arrTalentTrees[CHARCLASS_PALADIN] = NULL;
    m_arrTalentTrees[CHARCLASS_DRUID] = NULL;

    // Item database
    m_arrItems[ITEMID_UNDEFINED] = NULL;
    
    m_arrItems[ITEMID_WEAPON_FISTS] = New WeaponEmptyFists();

    UnSelectMemory();

    // Characters
    m_mapCharacters.SetComparator( _Compare_Strings, NULL );
    m_mapCharacters.UseMemoryContext( GetMemoryContext(), TEXT("Scratch") );
    m_mapCharacters.Create();

    // Combats
    m_mapCombats.SetComparator( _Compare_CombatIDs, NULL );
    m_mapCombats.UseMemoryContext( GetMemoryContext(), TEXT("Scratch") );
    m_mapCombats.Create();

    ////////////////////////////////////////////////////////////////////
    Vertex3 vPlayerSpawnPosition( 272.0f, 272.0f, 10.0f );
    Vertex3 vDummySpawnPosition( 300.0f, 300.0f, 10.0f );

        // Player
    PCharacter * pPlayer = (PCharacter*)( CreateCharacter( TEXT("Shiki"), CHARCLASS_MAGE, 1, false, vPlayerSpawnPosition ) );

        // Dummy
    NPCharacter * pDummy = (NPCharacter*)( CreateCharacter( TEXT("Dummy"), CHARCLASS_MAGE, 1, true, vDummySpawnPosition ) );
    ////////////////////////////////////////////////////////////////////
}
Void GameplayManager::Cleanup()
{
    SelectMemory( TEXT("Scratch") );

    // Combats
    m_mapCombats.Destroy();

    // Characters
    for( _CharacterMap::Iterator itCharacter = m_mapCharacters.Begin(); !(itCharacter.IsNull()); ++itCharacter )
        Delete( itCharacter.GetItem() );
    m_mapCharacters.Destroy();

    // Item database
    for( UInt i = 0; i < ITEMID_COUNT; ++i ) {
        if ( m_arrItems[i] != NULL ) {
            Delete( m_arrItems[i] );
            m_arrItems[i] = NULL;
        }
    }

    // Talent database
    for( UInt i = 0; i < CHARCLASS_COUNT; ++i ) {
        if ( m_arrTalentTrees[i] != NULL ) {
            Delete( m_arrTalentTrees[i] );
            m_arrTalentTrees[i] = NULL;
        }
    }

    // Skill database
    for( UInt i = 0; i < SKILLID_COUNT; ++i ) {
        if ( m_arrSkills[i] != NULL ) {
            Delete( m_arrSkills[i] );
            m_arrSkills[i] = NULL;
        }
    }

    // StatusEffect database
    for( UInt i = 0; i < STATUSEFFECTID_COUNT; ++i ) {
        if ( m_arrStatusEffects[i] != NULL ) {
            Delete( m_arrStatusEffects[i] );
            m_arrStatusEffects[i] = NULL;
        }
    }

    UnSelectMemory();
}

BaseCharacter * GameplayManager::CreateCharacter( const GChar * strName, CharClass iClass, UInt iLevel, Bool bNPC, const Vertex3 & vPosition )
{
    if ( m_mapCharacters.Contains(strName) )
        return NULL;

    BaseCharacter * pCharacter = NULL;

    SelectMemory( TEXT("Scratch") );
    if ( bNPC )
        pCharacter = New NPCharacter( strName, iClass, iLevel, vPosition );
    else
        pCharacter = New PCharacter( strName, iClass, iLevel, vPosition );
    UnSelectMemory();

    Bool bInserted = m_mapCharacters.Insert( pCharacter->GetName(), pCharacter );
    Assert( bInserted );

    return pCharacter;
}
Void GameplayManager::DestroyCharacter( const GChar * strName )
{
    BaseCharacter * pCharacter = NULL;
    Bool bRemoved = m_mapCharacters.Remove( strName, &pCharacter );
    Assert( bRemoved && pCharacter != NULL );

    SelectMemory( TEXT("Scratch") );
    Delete( pCharacter );
    UnSelectMemory();
}

CombatID GameplayManager::CreateCombat( BaseCharacter * pSource, BaseCharacter * pTarget )
{
    CombatID idCombat = _GenerateCombatID();

    _CombatData hCombatData;
    hCombatData.iCharacterCount = 2;
    hCombatData.arrCharacters[0] = pSource;
    hCombatData.arrCharacters[1] = pTarget;

    Bool bInserted = m_mapCombats.Insert( idCombat, hCombatData );
    Assert( bInserted );

    return idCombat;
}
CombatID GameplayManager::MergeCombats( CombatID idSourceCombat, CombatID idTargetCombat )
{
    if ( idSourceCombat == idTargetCombat )
        return idSourceCombat;

    _CombatMap::Iterator itSourceCombat = m_mapCombats.Get( idSourceCombat );
    _CombatMap::Iterator itTargetCombat = m_mapCombats.Get( idTargetCombat );
    if ( itSourceCombat.IsNull() || itTargetCombat.IsNull() )
        return COMBATID_INVALID;
    _CombatData * pSourceCombatData = itSourceCombat.GetItemPtr();
    _CombatData * pTargetCombatData = itTargetCombat.GetItemPtr();
    Assert( (pSourceCombatData->iCharacterCount + pTargetCombatData->iCharacterCount) <= COMBAT_MAX_CHARACTERS );

    for( UInt i = 0; i < pTargetCombatData->iCharacterCount; ++i ) {
        pSourceCombatData->arrCharacters[pSourceCombatData->iCharacterCount] = pTargetCombatData->arrCharacters[i];
        ++(pSourceCombatData->iCharacterCount);
    }
    pTargetCombatData->iCharacterCount = 0;

    Bool bRemoved = m_mapCombats.Remove( idTargetCombat );
    Assert( bRemoved );

    return idSourceCombat;
}
Void GameplayManager::JoinCombat( CombatID idCombat, BaseCharacter * pTarget )
{
    _CombatMap::Iterator itCombat = m_mapCombats.Get( idCombat );
    if ( itCombat.IsNull() )
        return;
    _CombatData * pCombatData = itCombat.GetItemPtr();
    Assert( (pCombatData->iCharacterCount + 1) <= COMBAT_MAX_CHARACTERS );

    pCombatData->arrCharacters[pCombatData->iCharacterCount] = pTarget;
    ++(pCombatData->iCharacterCount);
}
Void GameplayManager::LeaveCombat( CombatID idCombat, BaseCharacter * pTarget )
{
    _CombatMap::Iterator itCombat = m_mapCombats.Get( idCombat );
    if ( itCombat.IsNull() )
        return;
    _CombatData * pCombatData = itCombat.GetItemPtr();
    if ( pCombatData->iCharacterCount == 0 )
        return;

    for( UInt i = 0; i < pCombatData->iCharacterCount; ++i ) {
        if ( pCombatData->arrCharacters[i] == pTarget ) {
            if ( i == (pCombatData->iCharacterCount - 1) )
                pCombatData->arrCharacters[i] = NULL;
            else {
                pCombatData->arrCharacters[i] = pCombatData->arrCharacters[pCombatData->iCharacterCount - 1];
                pCombatData->arrCharacters[pCombatData->iCharacterCount - 1] = NULL;
            }
            --(pCombatData->iCharacterCount);
            break;
        }
    }
}

Void GameplayManager::Update( Scalar fTimeStep )
{
    // Update characters
    for( _CharacterMap::Iterator itCharacter = m_mapCharacters.Begin(); !(itCharacter.IsNull()); ++itCharacter )
        itCharacter.GetItem()->Update( fTimeStep );

    // Fade combats
    _CombatMap::Iterator itCombat = m_mapCombats.Begin();
    while( !(itCombat.IsNull()) ) {
        _CombatData * pCombatData = itCombat.GetItemPtr();
        if ( pCombatData->iCharacterCount >= 2 ) {
            ++itCombat;
            continue;
        }

        _CombatMap::Iterator itTmp = itCombat.Previous();
        if ( itTmp.IsNull() ) {
            itTmp = itCombat.Next();
            if ( itTmp.IsNull() ) {
                Bool bRemoved = m_mapCombats.Remove( itCombat.GetKey() );
                Assert( bRemoved );
                break;
            }
            Bool bRemoved = m_mapCombats.Remove( itCombat.GetKey() );
            Assert( bRemoved );
            itCombat = itTmp;
            continue;
        }
        Bool bRemoved = m_mapCombats.Remove( itCombat.GetKey() );
        Assert( bRemoved );
        itCombat = itTmp;
        ++itCombat;
    }
}

/////////////////////////////////////////////////////////////////////////////////
GameplayManager::_CombatData::_CombatData()
{
    iCharacterCount = 0;
}
GameplayManager::_CombatData::~_CombatData()
{
    // nothing to do
}

GameplayManager::_CombatData & GameplayManager::_CombatData::operator=( const _CombatData & rhs )
{
    iCharacterCount = rhs.iCharacterCount;
    for( UInt i = 0; i < iCharacterCount; ++i )
        arrCharacters[i] = rhs.arrCharacters[i];
    return (*this);
}
