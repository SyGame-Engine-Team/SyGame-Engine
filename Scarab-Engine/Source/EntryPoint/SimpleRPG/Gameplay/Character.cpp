/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Gameplay/Character.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Gameplay : Character
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
#include "Character.h"

#include "GameplayManager.h"

/////////////////////////////////////////////////////////////////////////////////
// BaseCharacter implementation
BaseCharacter::BaseCharacter( const GChar * strName, CharClass iClass, UInt iLevel, const Vertex3 & vPosition ):
    m_hStatistics( this ), m_hStatusEffectBuffer( this ), m_hSkillBook( this ), m_hTalentTree(), m_hInventory( this ), m_hFactionRegistry(),
    m_hCastingAnimation()
{
    // Name & class
    StringFn->NCopy( m_strName, strName, 64 );
    m_iClass = iClass;
    m_iLevel = iLevel;

    // XP & leveling
    m_iCurrentXP = 0;
    m_iMaxXP = _ComputeMaxXP( m_iLevel );

    // Statistics
    m_hStatistics.OnLevelChange();

    // StatusEffects

    // Skills

    // Talents
    //TalentTree * pTalentTree = TalentTree::GetInstance( m_iClass )
    GameplayFn->GetTalentTree( m_iClass )->GenerateInstance( &m_hTalentTree, this, 0 );

    // Inventory

    // Factions

    // Target selection
    m_pSelectedTarget = NULL;
    m_pFocusedTarget = NULL;

    m_fTargetListLastRefresh = 0.0f;
    m_iCurrentTarget = INVALID_OFFSET;

    m_iTargetCount = 0;
    for( UInt i = 0; i < CHARACTER_TARGETLIST_MAX_SIZE; ++i )
        m_arrTargetList[i] = NULL;

    // Continuous attack
    m_bAttacking = false;
    m_bContinuous = false;
    m_fLastAttackElapsedTimeRH = 0.0f;
    m_pAttackingWeaponRH = NULL;
    m_fLastAttackElapsedTimeLH = 0.0f;
    m_pAttackingWeaponLH = NULL;
    m_fLastAttackElapsedTimeT = 0.0f;
    m_pAttackingWeaponT = NULL;
    m_fLastAttackElapsedTimeF = 0.0f;
    m_pAttackingWeaponF = NULL;
    m_pAttackingTarget = NULL;

    // Casting sequence
    m_idCastingSkill = SKILLID_UNDEFINED;
    m_fCastingTime = 0.0f;
    m_pCastingTarget = NULL;

    // Global cooldown
    m_fGlobalCooldown = 0.0f;

    // Character state
    m_fLastRegenTickElapsedTime = 0.0f;
    m_iCurrentHP = m_hStatistics.GetMaxHP();
    m_iCurrentMP = m_hStatistics.GetMaxMP();

    m_idCombat = INVALID_OFFSET;

    // Entities (last)
    m_pEntity = EntityFn->CreateCharEntity( this, vPosition );
    m_pSelectionMarker = NULL;

    m_pEntity->GetController()->SetMovementSpeed( m_hStatistics.GetMovementSpeed() );
}
BaseCharacter::~BaseCharacter()
{
    // Entities (first)
    if ( m_pSelectionMarker != NULL ) {
        EntityFn->DestroyEntity( m_pSelectionMarker->GetName() );
        m_pSelectionMarker = NULL;
    }
    EntityFn->DestroyEntity( m_pEntity->GetName() );
    m_pEntity = NULL;
}

Void BaseCharacter::AddXP( UInt iXP )
{
    m_iCurrentXP += iXP;
    while ( m_iCurrentXP >= m_iMaxXP ) {
        iXP = ( m_iCurrentXP - m_iMaxXP );
        LevelUp();
        m_iCurrentXP = iXP;
    }
}
Void BaseCharacter::RemoveXP( UInt iXP )
{
    while ( m_iCurrentXP < iXP ) {
        iXP -= m_iCurrentXP;
        LevelDown();
        m_iCurrentXP = m_iMaxXP - 1;
    }
    m_iCurrentXP -= iXP;
}
Void BaseCharacter::SetLevel( UInt iLevel )
{
    if ( iLevel > CHARACTER_LEVEL_CAP )
        iLevel = CHARACTER_LEVEL_CAP;
    if ( iLevel == m_iLevel )
        return;

    m_iLevel = iLevel;

    m_iCurrentXP = 0;
    m_iMaxXP = _ComputeMaxXP( m_iLevel );

    m_hStatistics.OnLevelChange();
    m_pEntity->GetController()->SetMovementSpeed( m_hStatistics.GetMovementSpeed() );

    m_iCurrentHP = m_hStatistics.GetMaxHP();
    m_iCurrentMP = m_hStatistics.GetMaxMP();
}

Void BaseCharacter::Teleport( const Vertex3 & vPosition, Bool bFar, Bool bKeepInertia )
{
    InterruptAttack();
    InterruptCast();
    if ( bFar ) {
        SelectTarget( NULL );
        FocusTarget( NULL );
        GameplayFn->LeaveCombat( m_idCombat, this );
        m_idCombat = INVALID_OFFSET;
    }
    m_pEntity->Teleport( vPosition, bKeepInertia );
}

Void BaseCharacter::SelectNextNearestTarget()
{
    Scalar fElapsedTime = 10.0f;
    TimeMeasure fTime;
    if ( m_fTargetListLastRefresh != 0.0f )
        fElapsedTime = (Scalar)( SystemFn->TimeRelative( m_fTargetListLastRefresh, TIMEUNIT_SECONDS, &fTime ) );
    else
        fTime = (Scalar)( SystemFn->TimeAbsolute( TIMEUNIT_SECONDS ) );

    // It's been a while, recompute the list
    if ( fElapsedTime > 1.0f ) {
        m_fTargetListLastRefresh = (Scalar)fTime;
        m_iCurrentTarget = 0;

        m_iTargetCount = 0;

        // Get view direction from camera
        const Camera * pRenderCamera = RenderingFn->GetCamera();
        Vector3 vAim = pRenderCamera->GetViewDir();
        vAim.Z = 0.0f;
        vAim.Normalize();

        const Array<SimpleEntity*> & arrNearestEntities = m_pEntity->GetEntitiesInRange( ENTITYTYPE_CHARACTER, CHARACTER_SELECTION_RANGE, true );
        for( UInt i = 0; i < arrNearestEntities.Count(); ++i ) {
            BaseCharacter * pCharacter = ((CharEntity*)arrNearestEntities[i])->GetCharacter();
            if ( !(IsHostile(pCharacter)) )
                continue;

            // Cull out of sight characters
            Vector3 vW = ( pCharacter->m_pEntity->GetPosition() - m_pEntity->GetPosition() );
            if ( (vW * vAim) > 0.0f ) {
                m_arrTargetList[m_iTargetCount++] = pCharacter;
                if ( m_iTargetCount >= CHARACTER_TARGETLIST_MAX_SIZE )
                    break;
            }
        }
    } else {
        if ( m_iTargetCount > 1 )
            m_iCurrentTarget = (m_iCurrentTarget + 1) % m_iTargetCount;
    }

    // Cycle through the list
    if ( m_iTargetCount == 0 )
        SelectTarget( NULL );
    else
        SelectTarget( m_arrTargetList[m_iCurrentTarget] );
}

Bool BaseCharacter::Attack( BaseCharacter * pTarget, Bool bContinuous, Bool bUseThrow )
{
    if ( pTarget == NULL )
        return false;

    // Check if dead
    if ( IsDead() )
        return false;

    // Check valid target
    if ( pTarget->IsDead() || !(IsHostile(pTarget)) )
        return false;

    // Check weapons
    if ( bUseThrow ) {
        m_pAttackingWeaponRH = NULL;
        m_pAttackingWeaponLH = NULL;
        ItemInstance * pItemInstance = GetItem( EQUIPSLOTID_WEAPON_THROW );
        m_pAttackingWeaponT = ( pItemInstance == NULL ) ? NULL : (WeaponThrowItem*)(pItemInstance->GetItem());
        m_pAttackingWeaponF = NULL;
        if ( m_pAttackingWeaponT == NULL )
            return false;
    } else {
        ItemInstance * pItemInstance = GetItem( EQUIPSLOTID_WEAPON_RIGHT );
        m_pAttackingWeaponRH = ( pItemInstance == NULL ) ? NULL : (WeaponRightItem*)( pItemInstance->GetItem() );
        pItemInstance = GetItem( EQUIPSLOTID_WEAPON_LEFT );
        m_pAttackingWeaponLH = ( pItemInstance == NULL ) ? NULL : (WeaponLeftItem*)( pItemInstance->GetItem() );
        m_pAttackingWeaponT = NULL;
        m_pAttackingWeaponF = NULL;
        if ( m_pAttackingWeaponRH == NULL && m_pAttackingWeaponLH == NULL )
            m_pAttackingWeaponF = (WeaponEmptyFists*)( Item::GetInstance(ITEMID_WEAPON_FISTS) );
    }

    // Check range
    Scalar fMaxRange = 0.0f;
    if ( bUseThrow )
        fMaxRange = m_pAttackingWeaponT->GetAttackRange();
    else {
        if ( m_pAttackingWeaponRH != NULL )
            fMaxRange = m_pAttackingWeaponRH->GetAttackRange();
        if ( m_pAttackingWeaponLH != NULL && fMaxRange < m_pAttackingWeaponLH->GetAttackRange() )
            fMaxRange = m_pAttackingWeaponLH->GetAttackRange();
        if ( m_pAttackingWeaponF != NULL && fMaxRange < m_pAttackingWeaponF->GetAttackRange() )
            fMaxRange = m_pAttackingWeaponF->GetAttackRange();
    }
    Scalar fDistance = m_pEntity->GetDistance( pTarget->m_pEntity );
    if ( fDistance > fMaxRange ) {
        m_pAttackingWeaponRH = NULL;
        m_pAttackingWeaponLH = NULL;
        m_pAttackingWeaponT = NULL;
        m_pAttackingWeaponF = NULL;
        return false;
    }

    // Check line of sight
    if ( !(m_pEntity->HasLineOfSight(pTarget->m_pEntity)) ) {
        m_pAttackingWeaponRH = NULL;
        m_pAttackingWeaponLH = NULL;
        m_pAttackingWeaponT = NULL;
        m_pAttackingWeaponF = NULL;
        return false;
    }

    // Start attack
    m_bAttacking = true;
    m_bContinuous = bContinuous;
    m_fLastAttackElapsedTimeRH = 0.0f;
    m_fLastAttackElapsedTimeLH = 0.0f;
    m_fLastAttackElapsedTimeT = 0.0f;
    m_fLastAttackElapsedTimeF = 0.0f;
    m_pAttackingTarget = pTarget;

    return true;
}
Bool BaseCharacter::InterruptAttack()
{
    // Check if attacking
    if ( !m_bAttacking )
        return false;

    // End attack
    m_bAttacking = false;
    m_bContinuous = false;
    m_fLastAttackElapsedTimeRH = 0.0f;
    m_pAttackingWeaponRH = NULL;
    m_fLastAttackElapsedTimeLH = 0.0f;
    m_pAttackingWeaponLH = NULL;
    m_fLastAttackElapsedTimeT = 0.0f;
    m_pAttackingWeaponT = NULL;
    m_fLastAttackElapsedTimeF = 0.0f;
    m_pAttackingWeaponF = NULL;
    m_pAttackingTarget = NULL;

    return true;
}
Void BaseCharacter::WeaponAttack( ItemInstance * pWeaponInstance, BaseCharacter * pTarget )
{
    WeaponItem * pWeapon = NULL;
    if ( pWeaponInstance == NULL )
        pWeapon = (WeaponItem*)( Item::GetInstance( ITEMID_WEAPON_FISTS ) );
    else
        pWeapon = (WeaponItem*)( pWeaponInstance->GetItem() );

    // Hit check
    Scalar fHitScore = m_hStatistics.GetEffectiveStat( CHARSTAT_HIT );
    Bool bHit = ( GameplayFn->RandUnit() < fHitScore );
    if ( !bHit ) {
        // Combat log output
        DebugPrint( TEXT("\nWeapon Attack : %s missed %s !"), m_strName, pTarget->m_strName );
        return;
    }

    // Dodge check
    Scalar fDodgeScore = pTarget->m_hStatistics.GetEffectiveStat( CHARSTAT_DODGE );
    Bool bDodged = ( GameplayFn->RandUnit() < fDodgeScore );
    if ( bDodged ) {
        // Combat log output
        DebugPrint( TEXT("\nWeapon Attack : %s dodged %s's attack !"), pTarget->m_strName, m_strName );
        return;
    }

    // On-Hit effects
    StatusEffectID idOnHitEffect = pWeapon->GetOnHitEffect();
    Bool bOnHitEffect = false;
    if ( idOnHitEffect != STATUSEFFECTID_UNDEFINED ) {
        Scalar fOnHitEffectScore = pWeapon->GetOnHitEffectScore();
        bOnHitEffect = ( GameplayFn->RandUnit() < fOnHitEffectScore );
        if ( bOnHitEffect )
            pTarget->AddEffect( idOnHitEffect, this, pWeapon->GetOnHitEffectRank(), pWeapon->GetOnHitEffectStacks() );
    }

    // Block check
    Scalar fBlockScore = pTarget->m_hStatistics.GetEffectiveStat( CHARSTAT_BLOCK );
    Bool bBlocked = ( GameplayFn->RandUnit() < fBlockScore );
    if ( bBlocked ) {
        // Combat log output
        DebugPrint( TEXT("\nWeapon Attack : %s blocked %s's attack !"), pTarget->m_strName, m_strName );
        return;
    }

    // Compute input damage : Strength/Dexterity/Power
    UInt iBaseDamage = GameplayFn->RandRange( pWeapon->GetDamageMin(), pWeapon->GetDamageMax() );
    ////////////////////////////////////////
    UInt iBonusDamage = 1;
    ////////////////////////////////////////
    UInt iInputDamage = ( iBaseDamage + iBonusDamage );

    // Critical check
    Scalar fCriticalHitScore = m_hStatistics.GetEffectiveStat( CHARSTAT_CRITICALHIT );
    Bool bCritical = ( GameplayFn->RandUnit() < fCriticalHitScore );
    StatusEffectID idOnCriticalEffect = STATUSEFFECTID_UNDEFINED;
    Bool bOnCriticalEffect = false;
    if ( bCritical ) {
        // Update input damage : CriticalPower
        Scalar fCriticalPowerScore = m_hStatistics.GetEffectiveStat( CHARSTAT_CRITICALPOWER );
        iInputDamage = (UInt)( MathFn->Floor( fCriticalPowerScore * (Scalar)iInputDamage ) );

        // On-Critical effects
        idOnCriticalEffect = pWeapon->GetOnCriticalEffect();
        if ( idOnCriticalEffect != STATUSEFFECTID_UNDEFINED ) {
            Scalar fOnCriticalEffectScore = pWeapon->GetOnCriticalEffectScore();
            bOnCriticalEffect = ( GameplayFn->RandUnit() < fOnCriticalEffectScore );
            if ( bOnCriticalEffect )
                pTarget->AddEffect( idOnCriticalEffect, this, pWeapon->GetOnCriticalEffectRank(), pWeapon->GetOnCriticalEffectStacks() );
        }
    }

    // Damage reduction : Armor/Defense
    ////////////////////////////////////////
    UInt iDamageReduction = 1;
    ////////////////////////////////////////

    UInt iEffectiveDamage = 0;
    if ( iInputDamage > iDamageReduction )
        iEffectiveDamage = ( iInputDamage - iDamageReduction );

    // Apply damages to target
    if ( iEffectiveDamage > 0 )
        pTarget->Damage( this, iEffectiveDamage );

    // Combat log output
    if ( bCritical ) {
        if ( iEffectiveDamage == 0 )
            DebugPrint( TEXT("\nWeapon Attack : %s absorbed %s's attack (critical) !"), pTarget->m_strName, m_strName );
        else
            DebugPrint( TEXT("\nWeapon Attack : %s deals %d damages to %s (critical) !"), m_strName, iEffectiveDamage, pTarget->m_strName );
    } else {
        if ( iEffectiveDamage == 0 )
            DebugPrint( TEXT("\nWeapon Attack : %s absorbed %s's attack !"), pTarget->m_strName, m_strName );
        else
            DebugPrint( TEXT("\nWeapon Attack : %s deals %d damages to %s !"), m_strName, iEffectiveDamage, pTarget->m_strName );
    }
    if ( iDamageReduction > 0 )
        DebugPrint( TEXT(" -(Absorbed=%d)"), iDamageReduction );
    if ( bOnHitEffect )
        DebugPrint( TEXT(" +(OnHit=%s)"), StatusEffect::GetInstance(idOnHitEffect)->GetName() );
    if ( bOnCriticalEffect )
        DebugPrint( TEXT(" +(OnCritical=%s)"), StatusEffect::GetInstance(idOnCriticalEffect)->GetName() );
}

Bool BaseCharacter::Cast( SkillID idSkill, BaseCharacter * pTarget )
{
    // Check if dead
    if ( IsDead() )
        return false;

    // Check movement
    if ( IsMoving() )
        return false;

    // Check if already casting
    if ( m_idCastingSkill != SKILLID_UNDEFINED )
        return false;

    // Check global cooldown
    if ( m_fGlobalCooldown > 0.0f )
        return false;

    // Check skill cooldown
    SkillInstance * pSkill = m_hSkillBook.GetSkill( idSkill );
    if ( pSkill->IsOnCooldown() )
        return false;

    // Check mana cost
    if ( m_iCurrentMP < pSkill->GetManaCost() )
        return false;

    if ( pTarget != NULL ) {
        // Check valid target
        if ( pTarget->IsDead() || !(IsHostile(pTarget)) )
            return false;

        // Check range
        Scalar fDistance = m_pEntity->GetDistance( pTarget->m_pEntity );
        if ( fDistance > pSkill->GetRange() )
            return false;

        // Check line of sight
        if ( !(m_pEntity->HasLineOfSight(pTarget->m_pEntity)) )
            return false;
    } else if ( pSkill->RequiresTarget() ) {
        if ( GetSelectedTarget() == NULL )
            return false;
    }

    // Start global cooldown
    m_fGlobalCooldown = GAMEPLAY_GLOBAL_COOLDOWN;

    // Start skill cooldown
    pSkill->StartCooldown();

    // Instant
    if ( pSkill->IsInstant() ) {
        pSkill->Cast( pTarget );
        return true;
    }

    // Start cast
    m_idCastingSkill = idSkill;
    m_fCastingTime = pSkill->GetCastingTime(); // Add haste influence ////////////////
    m_pCastingTarget = pTarget;

    Animation * pAnimation = Animation::GetInstance( ANIMID_SKILL_CASTAURA );
    pAnimation->GenerateInstance( &m_hCastingAnimation, m_fCastingTime, this, m_pCastingTarget );

    return true;
}
Bool BaseCharacter::InterruptCast()
{
    // Check if casting
    if ( m_idCastingSkill == SKILLID_UNDEFINED )
        return false;

    // End cast
    m_hCastingAnimation.Interrupt();
    m_hCastingAnimation = AnimationInstance();

    m_idCastingSkill = SKILLID_UNDEFINED;
    m_fCastingTime = 0.0f;
    m_pCastingTarget = NULL;

    return true;
}
Void BaseCharacter::SkillAttack( SkillInstance * pSkillInstance, BaseCharacter * pTarget )
{
    Assert( pSkillInstance != NULL );
    ActiveSkill * pSkill = (ActiveSkill*)( pSkillInstance->GetSkill() );

    // Hit check
    Scalar fHitScore = m_hStatistics.GetEffectiveStat( pSkill->IsMagical() ? CHARSTAT_HIT_MAGIC : CHARSTAT_HIT );
    Bool bHit = ( GameplayFn->RandUnit() < fHitScore );
    if ( !bHit ) {
        // Combat log output
        DebugPrint( TEXT("\n%s : %s missed %s !"), pSkill->GetName(), m_strName, pTarget->m_strName );
        return;
    }

    // Dodge check
    if ( pSkill->IsPhysical() ) {
        Scalar fDodgeScore = pTarget->m_hStatistics.GetEffectiveStat( CHARSTAT_DODGE );
        Bool bDodged = ( GameplayFn->RandUnit() < fDodgeScore );
        if ( bDodged ) {
            // Combat log output
            DebugPrint( TEXT("\n%s : %s dodged %s's attack !"), pSkill->GetName(), pTarget->m_strName, m_strName );
            return;
        }
    }

    // On-Hit effects
    StatusEffectID idOnHitEffect = pSkill->GetOnHitEffect();
    Bool bOnHitEffect = false;
    if ( idOnHitEffect != STATUSEFFECTID_UNDEFINED ) {
        Scalar fOnHitEffectScore = pSkill->GetOnHitEffectScore();
        bOnHitEffect = ( GameplayFn->RandUnit() < fOnHitEffectScore );
        if ( bOnHitEffect )
            pTarget->AddEffect( idOnHitEffect, this, pSkill->GetOnHitEffectRank(), pSkill->GetOnHitEffectStacks() );
    }

    // Block check
    if ( pSkill->IsPhysical() ) {
        Scalar fBlockScore = pTarget->m_hStatistics.GetEffectiveStat( CHARSTAT_BLOCK );
        Bool bBlocked = ( GameplayFn->RandUnit() < fBlockScore );
        if ( bBlocked ) {
            // Combat log output
            DebugPrint( TEXT("\n%s : %s blocked %s's attack !"), pSkill->GetName(), pTarget->m_strName, m_strName );
            return;
        }
    }

    // Compute input damage : Strength/Dexterity/Power
    UInt iBaseDamage = GameplayFn->RandRange( pSkill->GetDamageMin(), pSkill->GetDamageMax() );
    ////////////////////////////////////////
    UInt iBonusDamage = 1;
    ////////////////////////////////////////
    UInt iInputDamage = ( iBaseDamage + iBonusDamage );

    // Critical check
    Scalar fCriticalHitScore = m_hStatistics.GetEffectiveStat( pSkill->IsMagical() ? CHARSTAT_CRITICALHIT_MAGIC : CHARSTAT_CRITICALHIT );
    Bool bCritical = ( GameplayFn->RandUnit() < fCriticalHitScore );
    StatusEffectID idOnCriticalEffect = STATUSEFFECTID_UNDEFINED;
    Bool bOnCriticalEffect = false;
    if ( bCritical ) {
        // Update input damage : CriticalPower
        Scalar fCriticalPowerScore = m_hStatistics.GetEffectiveStat( pSkill->IsMagical() ? CHARSTAT_CRITICALPOWER_MAGIC : CHARSTAT_CRITICALPOWER );
        iInputDamage = (UInt)( MathFn->Floor( fCriticalPowerScore * (Scalar)iInputDamage ) );

        // On-Critical effects
        idOnCriticalEffect = pSkill->GetOnCriticalEffect();
        if ( idOnCriticalEffect != STATUSEFFECTID_UNDEFINED ) {
            Scalar fOnCriticalEffectScore = pSkill->GetOnCriticalEffectScore();
            bOnCriticalEffect = ( GameplayFn->RandUnit() < fOnCriticalEffectScore );
            if ( bOnCriticalEffect )
                pTarget->AddEffect( idOnCriticalEffect, this, pSkill->GetOnCriticalEffectRank(), pSkill->GetOnCriticalEffectStacks() );
        }
    }

    // Damage reduction : Armor/Defense
    ////////////////////////////////////////
    UInt iDamageReduction = 1;
    ////////////////////////////////////////

    UInt iEffectiveDamage = 0;
    if ( iInputDamage > iDamageReduction )
        iEffectiveDamage = ( iInputDamage - iDamageReduction );

    // Apply damages to target
    if ( iEffectiveDamage > 0 )
        pTarget->Damage( this, iEffectiveDamage );

    // Combat log output
    if ( bCritical ) {
        if ( iEffectiveDamage == 0 )
            DebugPrint( TEXT("\n%s : %s absorbed %s's attack (critical) !"), pSkill->GetName(), pTarget->m_strName, m_strName );
        else
            DebugPrint( TEXT("\n%s : %s deals %d damages to %s (critical) !"), pSkill->GetName(), m_strName, iEffectiveDamage, pTarget->m_strName );
    } else {
        if ( iEffectiveDamage == 0 )
            DebugPrint( TEXT("\n%s : %s absorbed %s's attack !"), pSkill->GetName(), pTarget->m_strName, m_strName );
        else
            DebugPrint( TEXT("\n%s : %s deals %d damages to %s !"), pSkill->GetName(), m_strName, iEffectiveDamage, pTarget->m_strName );
    }
    if ( iDamageReduction > 0 )
        DebugPrint( TEXT(" -(Absorbed=%d)"), iDamageReduction );
    if ( bOnHitEffect )
        DebugPrint( TEXT(" +(OnHit=%s)"), StatusEffect::GetInstance(idOnHitEffect)->GetName() );
    if ( bOnCriticalEffect )
        DebugPrint( TEXT(" +(OnCritical=%s)"), StatusEffect::GetInstance(idOnCriticalEffect)->GetName() );
}

Void BaseCharacter::Damage( BaseCharacter * pSource, UInt iDamage )
{
    // Enter/Merge Combat(s)
    if ( m_idCombat == COMBATID_INVALID ) {
        if ( pSource->m_idCombat == COMBATID_INVALID )
            pSource->m_idCombat = GameplayFn->CreateCombat( pSource, this );
        else
            GameplayFn->JoinCombat( pSource->m_idCombat, this );
         m_idCombat = pSource->m_idCombat;
    } else {
        if ( pSource->m_idCombat == COMBATID_INVALID )
            GameplayFn->JoinCombat( m_idCombat, pSource );
        else
            m_idCombat = GameplayFn->MergeCombats( pSource->m_idCombat, m_idCombat );
        pSource->m_idCombat = m_idCombat;
    }

    // Generate threat
    /////////////////////////////////////
    // Generate threat on target

    // Check for death
    if ( m_iCurrentHP <= iDamage ) {
        RemoveAllEffects();

        SetStanding();
        InterruptAttack();
        InterruptCast();

        m_fGlobalCooldown = 0.0f;

        m_iCurrentHP = 0;
        m_iCurrentMP = 0;

        GameplayFn->LeaveCombat( m_idCombat, this );
        m_idCombat = INVALID_OFFSET;

        // Debug
        DebugPrint( TEXT("\nDamaged %s for %d, Dead !"), m_strName, iDamage );

        return;
    }

    // Damage
    m_iCurrentHP -= iDamage;

    // Debug
    DebugPrint( TEXT("\nDamaged %s for %d, HP = %d/%d !"), m_strName, iDamage, m_iCurrentHP, m_hStatistics.GetMaxHP() );
    DebugPrint( TEXT("\nSourceCombatID = %d"), pSource->m_idCombat );
    DebugPrint( TEXT("\nTargetCombatID = %d"), m_idCombat );
}
Void BaseCharacter::Heal( BaseCharacter * pSource, UInt iHeal )
{
    // Enter/Merge Combat(s)
    if ( m_idCombat != COMBATID_INVALID ) {
        if ( pSource->m_idCombat == COMBATID_INVALID )
            GameplayFn->JoinCombat( m_idCombat, pSource );
        else
            m_idCombat = GameplayFn->MergeCombats( pSource->m_idCombat, m_idCombat );
        pSource->m_idCombat = m_idCombat;
    }

    // Generate threat
    /////////////////////////////////////
    // Generate threat on hostiles to source/target in the combat
    // Higher threat for target's attackers

    // Heal
    m_iCurrentHP += iHeal;
    if ( m_iCurrentHP > m_hStatistics.GetMaxHP() )
        m_iCurrentHP = m_hStatistics.GetMaxHP();

    // Debug
    DebugPrint( TEXT("\nHealed %s for %d, HP = %d/%d !"), m_strName, iHeal, m_iCurrentHP, m_hStatistics.GetMaxHP() );
    DebugPrint( TEXT("\nSourceCombatID = %d"), pSource->m_idCombat );
    DebugPrint( TEXT("\nTargetCombatID = %d"), m_idCombat );
}

Void BaseCharacter::Kill()
{
    /////////////////////////////////////
}
Void BaseCharacter::Resurrect( UInt /*iHP*/ )
{
    /////////////////////////////////////
}

Void BaseCharacter::Update( Scalar fTimeStep )
{
    // Update selection marker
    if ( m_pSelectionMarker != NULL ) {
        if ( m_pSelectedTarget != NULL )
            m_pSelectionMarker->TrackTarget( m_pSelectedTarget );
    }

    // Update status effects
    Bool bHasChanged = m_hStatusEffectBuffer.Update( fTimeStep );
    if ( bHasChanged ) {
        m_hStatistics.OnActiveChange();
        m_pEntity->GetController()->SetMovementSpeed( m_hStatistics.GetMovementSpeed() );
    }

    // Update skills
    m_hSkillBook.Update( fTimeStep );

    // Update global cooldown
    if ( m_fGlobalCooldown > 0.0f ) {
        m_fGlobalCooldown -= fTimeStep;
        if ( m_fGlobalCooldown < 0.0f )
            m_fGlobalCooldown = 0.0f;
    }

    // Update continuous attack
    _UpdateAttacking( fTimeStep );

    // Update casting sequence
    _UpdateCasting( fTimeStep );

    // Update state
        // HP/MP regen
    m_fLastRegenTickElapsedTime += fTimeStep;
    while( m_fLastRegenTickElapsedTime > 1.0f ) {
        m_iCurrentHP += (UInt)( MathFn->Floor( m_hStatistics.GetEffectiveStat(CHARSTAT_REGEN_HEALTH) ) );
        if ( m_iCurrentHP > m_hStatistics.GetMaxHP() )
            m_iCurrentHP =  m_hStatistics.GetMaxHP();
        m_iCurrentMP += (UInt)( MathFn->Floor( m_hStatistics.GetEffectiveStat(CHARSTAT_REGEN_MANA) ) );
        if ( m_iCurrentMP > m_hStatistics.GetMaxMP() )
            m_iCurrentMP =  m_hStatistics.GetMaxMP();

        m_fLastRegenTickElapsedTime -= 1.0f;
    }

        // Combat
    /////////////////////////////////////////////

    // Update Movement tracking (last)
    m_pEntity->UpdatePathTracking();
}

/////////////////////////////////////////////////////////////////////////////////

Int BaseCharacter::_Compare_CharDistance( BaseCharacter * const & rLeft, BaseCharacter * const & rRight, Void * pUserData )
{
    BaseCharacter * pThis = (BaseCharacter*)pUserData;
    Vertex3 vPosition = pThis->GetEntity()->GetPosition();
    Vertex3 vLeft = rLeft->GetEntity()->GetPosition();
    Vertex3 vRight = rRight->GetEntity()->GetPosition();
    Scalar fDistL = ( vLeft - vPosition ).NormSqr();
    Scalar fDistR = ( vRight - vPosition ).NormSqr();
    if ( fDistL < fDistR )
        return +1;
    if ( fDistL > fDistR )
        return -1;
    return 0;
}

Void BaseCharacter::_UpdateAttacking( Scalar fTimeStep )
{
    if ( !m_bAttacking )
        return;

    // Check valid target
    if ( m_pAttackingTarget->IsDead() || !(IsHostile(m_pAttackingTarget)) ) {
        InterruptAttack();
        return;
    }

    Bool bAttacked = false;

    // Switch between active weapons
    if ( m_pAttackingWeaponT != NULL ) {
        // Check range
        Scalar fRange = m_pAttackingWeaponT->GetAttackRange();
        Scalar fDistance = m_pEntity->GetDistance( m_pAttackingTarget->m_pEntity );
        if ( fDistance <= fRange ) {
            // Check line of sight
            if ( m_pEntity->HasLineOfSight(m_pAttackingTarget->m_pEntity) ) {
                // Throw attacks
                ItemInstance * pWeaponInstance = GetItem( EQUIPSLOTID_WEAPON_THROW );
                Scalar fPeriod = m_pAttackingWeaponT->GetAttackPeriod(); // Add haste influence ////////////////
                m_fLastAttackElapsedTimeT += fTimeStep;
                while( m_fLastAttackElapsedTimeT > fPeriod ) {
                    WeaponAttack( pWeaponInstance, m_pAttackingTarget );
                    m_fLastAttackElapsedTimeT -= fPeriod;
                    bAttacked = true;
                }
            }
        }
    } else {
        Bool bHasLineOfSight = m_pEntity->HasLineOfSight( m_pAttackingTarget->m_pEntity );

        if ( m_pAttackingWeaponRH != NULL ) {
            // Check range
            Scalar fRange = m_pAttackingWeaponRH->GetAttackRange();
            Scalar fDistance = m_pEntity->GetDistance( m_pAttackingTarget->m_pEntity );
            if ( fDistance <= fRange ) {
                // Check line of sight
                if ( bHasLineOfSight ) {
                    // Melee attacks
                    ItemInstance * pWeaponInstance = GetItem( EQUIPSLOTID_WEAPON_RIGHT );
                    Scalar fPeriod = m_pAttackingWeaponRH->GetAttackPeriod(); // Add haste influence ////////////////
                    m_fLastAttackElapsedTimeRH += fTimeStep;
                    while( m_fLastAttackElapsedTimeRH > fPeriod ) {
                        WeaponAttack( pWeaponInstance, m_pAttackingTarget );
                        m_fLastAttackElapsedTimeRH -= fPeriod;
                        bAttacked = true;
                    }
                }
            }
        }
        if ( m_pAttackingWeaponLH != NULL ) {
            // Check range
            Scalar fRange = m_pAttackingWeaponLH->GetAttackRange();
            Scalar fDistance = m_pEntity->GetDistance( m_pAttackingTarget->m_pEntity );
            if ( fDistance <= fRange ) {
                // Check line of sight
                if ( bHasLineOfSight ) {
                    // Melee attacks
                    ItemInstance * pWeaponInstance = GetItem( EQUIPSLOTID_WEAPON_LEFT );
                    Scalar fPeriod = m_pAttackingWeaponLH->GetAttackPeriod(); // Add haste influence ////////////////
                    m_fLastAttackElapsedTimeLH += fTimeStep;
                    while( m_fLastAttackElapsedTimeLH > fPeriod ) {
                        WeaponAttack( pWeaponInstance, m_pAttackingTarget );
                        m_fLastAttackElapsedTimeLH -= fPeriod;
                        bAttacked = true;
                    }
                }
            }
        }
        if ( m_pAttackingWeaponF != NULL ) {
            // Check range
            Scalar fRange = m_pAttackingWeaponF->GetAttackRange();
            Scalar fDistance = m_pEntity->GetDistance( m_pAttackingTarget->m_pEntity );
            if ( fDistance <= fRange ) {
                // Check line of sight
                if ( bHasLineOfSight ) {
                    // Melee attacks
                    Scalar fPeriod = m_pAttackingWeaponF->GetAttackPeriod(); // Add haste influence ////////////////
                    m_fLastAttackElapsedTimeF += fTimeStep;
                    while( m_fLastAttackElapsedTimeF > fPeriod ) {
                        WeaponAttack( NULL, m_pAttackingTarget );
                        m_fLastAttackElapsedTimeF -= fPeriod;
                        bAttacked = true;
                    }
                }
            }
        }
    }

    // One-Attack only case
    if ( bAttacked && !m_bContinuous )
        InterruptAttack();
}

Void BaseCharacter::_UpdateCasting( Scalar fTimeStep )
{
    if ( m_idCastingSkill == SKILLID_UNDEFINED )
        return;

    // Check movement
    if ( IsMoving() ) {
        InterruptCast();
        return;
    }

    SkillInstance * pSkill = m_hSkillBook.GetSkill( m_idCastingSkill );

    // Check mana cost
    UInt iManaCost = pSkill->GetManaCost();
    if ( m_iCurrentMP < iManaCost ) {
        InterruptCast();
        return;
    }

    if ( m_pCastingTarget != NULL ) {
        // Check valid target
        if ( m_pCastingTarget->IsDead() || !(IsHostile(m_pCastingTarget)) ) {
            InterruptCast();
            return;
        }

        // Check range
        Scalar fDistance = m_pEntity->GetDistance( m_pCastingTarget->m_pEntity );
        if ( fDistance > pSkill->GetRange() ) {
            InterruptCast();
            return;
        }

        // Check line of sight
        if ( !(m_pEntity->HasLineOfSight(m_pCastingTarget->m_pEntity)) ) {
            InterruptCast();
            return;
        }
    }

    // Update casting animation
    m_fCastingTime -= fTimeStep;
    Bool bContinue = m_hCastingAnimation.Update( fTimeStep );
    if ( bContinue && m_fCastingTime > 0.0f )
        return;

    // Cast skill
    m_iCurrentMP -= iManaCost;
    pSkill->Cast( m_pCastingTarget );

    // End of cast
    m_idCastingSkill = SKILLID_UNDEFINED;
    m_fCastingTime = 0.0f;
    m_pCastingTarget = NULL;
    m_hCastingAnimation = AnimationInstance();
}

/////////////////////////////////////////////////////////////////////////////////
// PCharacter implementation
PCharacter::PCharacter( const GChar * strName, CharClass iClass, UInt iLevel, const Vertex3 & vPosition ):
    BaseCharacter( strName, iClass, iLevel, vPosition )
{
    m_hFactionRegistry.SetParentFaction( FACTIONID_PLAYER_BLUE );

    // Entities (last)
    m_pSelectionMarker = EntityFn->CreateSelectionMarker( this );
    m_pSelectionMarker->SetVisible( false );
}
PCharacter::~PCharacter()
{
    // nothing to do
}

Void PCharacter::Update( Scalar fTimeStep )
{
    // nothing to do

    // Default update
    BaseCharacter::Update( fTimeStep );
}

/////////////////////////////////////////////////////////////////////////////////
// NPCharacter implementation
NPCharacter::NPCharacter( const GChar * strName, CharClass iClass, UInt iLevel, const Vertex3 & vPosition ):
    BaseCharacter( strName, iClass, iLevel, vPosition ),
    m_hThreatTable()
{
    m_hFactionRegistry.SetParentFaction( FACTIONID_MONSTER_HOSTILE );

    m_fThreatDistance = 30.0f;
    for( UInt i = 0; i < CHARACTER_MAX_NPC_LINKAGE; ++i )
        m_arrLinkedNPCs[i] = NULL;

    m_iBehaviour = NPCBEHAVIOUR_INITIALIZE;
}
NPCharacter::~NPCharacter()
{
    // nothing to do
}

Void NPCharacter::Update( Scalar fTimeStep )
{
    // Update threat table
    m_hThreatTable.ResetMarks();
    const Array<SimpleEntity*> & arrInRangeCharacters = m_pEntity->GetEntitiesInRange( ENTITYTYPE_CHARACTER, m_fThreatDistance, false );
        // Increase threat to hostiles in range / Add them
    UInt iCharCount = arrInRangeCharacters.Count();
    for( UInt i = 0; i < iCharCount; ++i ) {
        if ( m_pEntity->HasLineOfSight(arrInRangeCharacters[i]) ) {
            BaseCharacter * pCharacterInRange = ((CharEntity*)(arrInRangeCharacters[i]))->GetCharacter();
            if ( IsHostile(pCharacterInRange) )
                m_hThreatTable.AddThreat( pCharacterInRange, THREAT_GROW_VALUE, true );
        }
    }
        // Decrease threat of out of range units
    UInt iThreatCount = m_hThreatTable.GetThreatRecordCount();
    for( UInt i = 0; i < iThreatCount; ++i ) {
        Bool bMark = false;
        BaseCharacter * pCharacter = m_hThreatTable.GetThreatRecord( i, NULL, &bMark );
        if ( !bMark )
            m_hThreatTable.RemoveThreat( pCharacter, THREAT_SHRINK_VALUE );
    }

    // Update IA behaviour
    _BehaviourScript( fTimeStep );

    // Default update
    BaseCharacter::Update( fTimeStep );
}

/////////////////////////////////////////////////////////////////////////////////

Void NPCharacter::_BehaviourScript( Scalar /*fTimeStep*/ )
{
    // Default dummy behaviour
    switch( m_iBehaviour ) {
        case NPCBEHAVIOUR_INITIALIZE:
            m_iBehaviour = NPCBEHAVIOUR_STANDING;
            break;
        case NPCBEHAVIOUR_STANDING:
            // nothing to do
            break;
        case NPCBEHAVIOUR_WANDERING:
        case NPCBEHAVIOUR_PATROLLING:
        case NPCBEHAVIOUR_ATTACKING:
        case NPCBEHAVIOUR_DEFENDING:
        case NPCBEHAVIOUR_RUNNING:
        case NPCBEHAVIOUR_SPECIAL:
        default: Assert(false);  break;
    }
}

