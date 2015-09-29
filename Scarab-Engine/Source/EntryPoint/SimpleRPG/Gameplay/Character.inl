/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Gameplay/Character.inl
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
// BaseCharacter implementation
inline CharEntity * BaseCharacter::GetEntity() const {
    return m_pEntity;
}

inline const GChar * BaseCharacter::GetName() const {
    return m_strName;
}
inline CharClass BaseCharacter::GetClass() const {
    return m_iClass;
}

inline UInt BaseCharacter::GetLevel() const {
    return m_iLevel;
}
inline UInt BaseCharacter::GetCurrentXP() const {
    return m_iCurrentXP;
}
inline UInt BaseCharacter::GetMaxXP() const {
    return m_iMaxXP;
}

inline Void BaseCharacter::LevelUp() {
    SetLevel( m_iLevel + 1 );
}
inline Void BaseCharacter::LevelDown() {
    if ( m_iLevel == 1 )
        return;
    SetLevel( m_iLevel - 1 );
}

inline Scalar BaseCharacter::GetBaseStat( CharStatistic iStat ) const {
    return m_hStatistics.GetBaseStat( iStat );
}
inline Scalar BaseCharacter::GetBaseStatModifier( CharStatistic iStat ) const {
    return m_hStatistics.GetBaseStatModifier( iStat );
}
inline Scalar BaseCharacter::GetModdedStat( CharStatistic iStat ) const {
    return m_hStatistics.GetModdedStat( iStat );
}
inline Scalar BaseCharacter::GetPassiveStatModifier( CharStatistic iStat ) const {
    return m_hStatistics.GetPassiveStatModifier( iStat );
}
inline Scalar BaseCharacter::GetItemStatModifier( CharStatistic iStat ) const {
    return m_hStatistics.GetItemStatModifier( iStat );
}
inline Scalar BaseCharacter::GetFinalStat( CharStatistic iStat ) const {
    return m_hStatistics.GetFinalStat( iStat );
}
inline Scalar BaseCharacter::GetActiveStatModifier( CharStatistic iStat ) const {
    return m_hStatistics.GetActiveStatModifier( iStat );
}
inline Scalar BaseCharacter::GetEffectiveStat( CharStatistic iStat ) const {
    return m_hStatistics.GetEffectiveStat( iStat );
}

inline UInt BaseCharacter::GetMaxHP() const {
    return m_hStatistics.GetMaxHP();
}
inline UInt BaseCharacter::GetMaxMP() const {
    return m_hStatistics.GetMaxMP();
}
inline Scalar BaseCharacter::GetMovementSpeed() const {
    return m_hStatistics.GetMovementSpeed();
}

inline UInt BaseCharacter::GetStatusEffectCount() const {
    return m_hStatusEffectBuffer.GetStatusEffectCount();
}

inline Bool BaseCharacter::HasStatusEffect( StatusEffectID idStatusEffect ) const {
    return m_hStatusEffectBuffer.HasStatusEffect( idStatusEffect );
}

inline UInt BaseCharacter::GetStatusEffectInstanceCount( StatusEffectID idStatusEffect ) const {
    return m_hStatusEffectBuffer.GetStatusEffectInstanceCount( idStatusEffect );
}
inline StatusEffectInstance * BaseCharacter::GetStatusEffectInstance( StatusEffectID idStatusEffect, UInt iIndex ) const {
    return m_hStatusEffectBuffer.GetStatusEffectInstance( idStatusEffect, iIndex );
}

inline Void BaseCharacter::EnumStatusEffects() const {
    m_hStatusEffectBuffer.EnumStatusEffects();
}
inline StatusEffectID BaseCharacter::EnumNextStatusEffect() const {
    return m_hStatusEffectBuffer.EnumNextStatusEffect();
}

inline Void BaseCharacter::EnumStatusEffectInstances() const {
    m_hStatusEffectBuffer.EnumStatusEffectInstances();
}
inline StatusEffectInstance * BaseCharacter::EnumNextStatusEffectInstance() const {
    return m_hStatusEffectBuffer.EnumNextStatusEffectInstance();
}

inline Bool BaseCharacter::AddEffect( StatusEffectID idStatusEffect, BaseCharacter * pSource, UInt iRank, UInt iStacks ) {
    Bool bAdded = m_hStatusEffectBuffer.AddEffect( idStatusEffect, pSource, iRank, iStacks );
    if ( bAdded ) {
        m_hStatistics.OnActiveChange();
        m_pEntity->GetController()->SetMovementSpeed( m_hStatistics.GetMovementSpeed() );
    }
    return bAdded;
}
inline Bool BaseCharacter::RemoveEffect( StatusEffectID idStatusEffect ) {
    Bool bRemoved = m_hStatusEffectBuffer.RemoveEffect( idStatusEffect );
    if ( bRemoved ) {
        m_hStatistics.OnActiveChange();
        m_pEntity->GetController()->SetMovementSpeed( m_hStatistics.GetMovementSpeed() );
    }
    return bRemoved;
}

inline Bool BaseCharacter::RemoveEffects( StatusEffectType iType ) {
    Bool bRemoved = m_hStatusEffectBuffer.RemoveEffects( iType );
    if ( bRemoved ) {
        m_hStatistics.OnActiveChange();
        m_pEntity->GetController()->SetMovementSpeed( m_hStatistics.GetMovementSpeed() );
    }
    return bRemoved;
}
inline Bool BaseCharacter::RemovePositiveEffects() {
    Bool bRemoved = m_hStatusEffectBuffer.RemovePositiveEffects();
    if ( bRemoved ) {
        m_hStatistics.OnActiveChange();
        m_pEntity->GetController()->SetMovementSpeed( m_hStatistics.GetMovementSpeed() );
    }
    return bRemoved;
}
inline Bool BaseCharacter::RemoveNegativeEffects() {
    Bool bRemoved = m_hStatusEffectBuffer.RemoveNegativeEffects();
    if ( bRemoved ) {
        m_hStatistics.OnActiveChange();
        m_pEntity->GetController()->SetMovementSpeed( m_hStatistics.GetMovementSpeed() );
    }
    return bRemoved;
}
inline Bool BaseCharacter::RemoveMovementLimiterEffects() {
    Bool bRemoved = m_hStatusEffectBuffer.RemoveMovementLimiterEffects();
    if ( bRemoved ) {
        m_hStatistics.OnActiveChange();
        m_pEntity->GetController()->SetMovementSpeed( m_hStatistics.GetMovementSpeed() );
    }
    return bRemoved;
}
inline Bool BaseCharacter::RemoveActionLimiterEffects() {
    Bool bRemoved = m_hStatusEffectBuffer.RemoveActionLimiterEffects();
    if ( bRemoved ) {
        m_hStatistics.OnActiveChange();
        m_pEntity->GetController()->SetMovementSpeed( m_hStatistics.GetMovementSpeed() );
    }
    return bRemoved;
}
inline Bool BaseCharacter::RemoveAllEffects() {
    Bool bRemoved = m_hStatusEffectBuffer.RemoveAllEffects();
    if ( bRemoved ) {
        m_hStatistics.OnActiveChange();
        m_pEntity->GetController()->SetMovementSpeed( m_hStatistics.GetMovementSpeed() );
    }
    return bRemoved;
}

inline Bool BaseCharacter::HasSkill( SkillID idSkill ) const {
    return m_hSkillBook.HasSkill( idSkill );
}
inline SkillInstance * BaseCharacter::GetSkill( SkillID idSkill ) const {
    return m_hSkillBook.GetSkill( idSkill );
}

inline Void BaseCharacter::EnumSkills() const {
    m_hSkillBook.EnumSkills();
}
inline SkillInstance * BaseCharacter::EnumNextSkill() const {
    return m_hSkillBook.EnumNextSkill();
}

inline Bool BaseCharacter::AddSkill( SkillID idSkill ) {
    Bool bAdded = m_hSkillBook.AddSkill( idSkill );
    if ( bAdded && Skill::GetInstance(idSkill)->IsPassive() ) {
        m_hStatistics.OnPassiveChange();
        m_pEntity->GetController()->SetMovementSpeed( m_hStatistics.GetMovementSpeed() );
    }
    return bAdded;
}
inline Bool BaseCharacter::RemoveSkill( SkillID idSkill ) {
    Bool bRemoved = m_hSkillBook.RemoveSkill( idSkill );
    if ( bRemoved && Skill::GetInstance(idSkill)->IsPassive() ) {
        m_hStatistics.OnPassiveChange();
        m_pEntity->GetController()->SetMovementSpeed( m_hStatistics.GetMovementSpeed() );
    }
    return bRemoved;
}

inline Bool BaseCharacter::IsOnCooldown( SkillID idSkill ) const {
    SkillInstance * pSkillInstance = m_hSkillBook.GetSkill( idSkill );
    if ( pSkillInstance == NULL )
        return false;
    return pSkillInstance->IsOnCooldown();
}
inline Scalar BaseCharacter::GetCooldown( SkillID idSkill ) const {
    SkillInstance * pSkillInstance = m_hSkillBook.GetSkill( idSkill );
    if ( pSkillInstance == NULL )
        return 0.0f;
    return pSkillInstance->GetCooldown();
}

inline ItemInstance * BaseCharacter::GetItem( InventoryBagID idBag, UInt iSlot ) {
    return m_hInventory.GetItem( idBag, iSlot );
}
inline ItemInstance * BaseCharacter::GetItem( EquipmentSoltID idEquipSlot ) {
    return m_hInventory.GetItem( idEquipSlot );
}

inline UInt BaseCharacter::PickItem( Item * pItem, UInt iAmount ) {
    return m_hInventory.PickItem( pItem, iAmount );
}
inline Void BaseCharacter::DropItem( InventoryBagID idBag, UInt iSlot ) {
    m_hInventory.DropItem( idBag, iSlot );
}
inline ItemInstance * BaseCharacter::EquipItem( EquipmentSoltID idEquipSlot, InventoryBagID idBag, UInt iSlot ) {
    ItemInstance * pItemInstance = m_hInventory.EquipItem( idEquipSlot, idBag, iSlot );
    if ( pItemInstance != NULL ) {
        m_hStatistics.OnItemChange();
        m_pEntity->GetController()->SetMovementSpeed( m_hStatistics.GetMovementSpeed() );
    }
    return pItemInstance;
}
inline ItemInstance * BaseCharacter::UnEquipItem( EquipmentSoltID idEquipSlot ) {
    ItemInstance * pItemInstance = m_hInventory.UnEquipItem( idEquipSlot );
    if ( pItemInstance != NULL ) {
        m_hStatistics.OnItemChange();
        m_pEntity->GetController()->SetMovementSpeed( m_hStatistics.GetMovementSpeed() );
    }
    return pItemInstance;
}

inline ItemInstance * BaseCharacter::UseItem( BaseCharacter * pTarget, InventoryBagID idBag, UInt iSlot ) {
    return m_hInventory.UseItem( pTarget, idBag, iSlot );
}
inline ItemInstance * BaseCharacter::UseItem( BaseCharacter * pTarget, EquipmentSoltID idEquipSlot ) {
    return m_hInventory.UseItem( pTarget, idEquipSlot );
}

inline Void BaseCharacter::SortBag( InventoryBagID idBag ) {
    m_hInventory.SortBag( idBag );
}
inline Void BaseCharacter::SortAllBags() {
    m_hInventory.SortAllBags();
}
inline Void BaseCharacter::SortBufferBagItems() {
    m_hInventory.SortBufferBagItems();
}

inline Bool BaseCharacter::IsHostile( BaseCharacter * pTarget ) const {
    if ( pTarget == this )
        return false;
    return m_hFactionRegistry.IsHostile( pTarget->m_hFactionRegistry.GetParentFaction() );
}

inline Scalar BaseCharacter::GetDistance( BaseCharacter * pTarget ) const {
    return m_pEntity->GetDistance( pTarget->m_pEntity );
}
inline Bool BaseCharacter::HasLineOfSight( BaseCharacter * pTarget ) const {
    return m_pEntity->HasLineOfSight( pTarget->m_pEntity );
}
inline const Array<SimpleEntity*> & BaseCharacter::GetEntitiesInRange( EntityType iType, Scalar fRange, Bool bSort ) const {
    return m_pEntity->GetEntitiesInRange( iType, fRange, bSort );
}

inline Vertex3 BaseCharacter::GetPosition() const {
    return m_pEntity->GetPosition();
}

inline Bool BaseCharacter::IsMovingForward() const {
    return m_pEntity->IsMovingForward();
}
inline Bool BaseCharacter::IsMovingBackward() const {
    return m_pEntity->IsMovingBackward();
}
inline Bool BaseCharacter::IsMovingLeft() const {
    return m_pEntity->IsMovingLeft();
}
inline Bool BaseCharacter::IsMovingRight() const {
    return m_pEntity->IsMovingRight();
}
inline Bool BaseCharacter::IsMovingUp() const {
    return m_pEntity->IsMovingUp();
}
inline Bool BaseCharacter::IsMovingDown() const {
    return m_pEntity->IsMovingDown();
}
inline Bool BaseCharacter::IsMoving() const {
    return m_pEntity->IsMoving();
}

inline Void BaseCharacter::SetMovingForward( Bool bMoving ) {
    m_pEntity->SetMovingForward( bMoving );
}
inline Void BaseCharacter::SetMovingBackward( Bool bMoving ) {
    m_pEntity->SetMovingBackward( bMoving );
}
inline Void BaseCharacter::SetMovingLeft( Bool bMoving ) {
    m_pEntity->SetMovingLeft( bMoving );
}
inline Void BaseCharacter::SetMovingRight( Bool bMoving ) {
    m_pEntity->SetMovingRight( bMoving );
}
inline Void BaseCharacter::SetMovingUp( Bool bMoving ) {
    m_pEntity->SetMovingUp( bMoving );
}
inline Void BaseCharacter::SetMovingDown( Bool bMoving ) {
    m_pEntity->SetMovingDown( bMoving );
}
inline Void BaseCharacter::SetStanding() {
    m_pEntity->SetStanding();
}

inline const Vector3 & BaseCharacter::GetMovementDirection() const {
    return m_pEntity->GetMovementDirection();
}

inline Bool BaseCharacter::IsPathTracking() const {
    return m_pEntity->IsPathTracking();
}

inline Void BaseCharacter::StartPathTracking( const Vertex3 & vTarget ) {
    return m_pEntity->StartPathTracking( vTarget );
}
inline Void BaseCharacter::EndPathTracking() {
    return m_pEntity->EndPathTracking();
}

inline BaseCharacter * BaseCharacter::GetSelectedTarget() const {
    return m_pSelectedTarget;
}
inline BaseCharacter * BaseCharacter::GetFocusedTarget() const {
    return m_pFocusedTarget;
}

inline Void BaseCharacter::SelectTarget( BaseCharacter * pTarget ) {
    m_pSelectedTarget = pTarget;
    if ( m_pSelectionMarker != NULL )
        m_pSelectionMarker->SetVisible( m_pSelectedTarget != NULL );
}
inline Void BaseCharacter::FocusTarget( BaseCharacter * pTarget ) {
    m_pFocusedTarget = pTarget;
}
inline Void BaseCharacter::SwapFocusTarget() {
    BaseCharacter * pSwap = m_pSelectedTarget;
    m_pSelectedTarget = m_pFocusedTarget;
    m_pFocusedTarget = pSwap;
    if ( m_pSelectionMarker != NULL )
        m_pSelectionMarker->SetVisible( m_pSelectedTarget != NULL );
}

inline Bool BaseCharacter::IsAttacking() const {
    return m_bAttacking;
}
inline BaseCharacter * BaseCharacter::GetAttackingTarget() const {
    return m_pAttackingTarget;
}

inline Bool BaseCharacter::IsCasting() const {
    return ( m_idCastingSkill != SKILLID_UNDEFINED );
}
inline SkillID BaseCharacter::GetCastingSkill() const {
    return m_idCastingSkill;
}
inline Scalar BaseCharacter::GetCastingTime() const {
    return m_fCastingTime;
}
inline BaseCharacter * BaseCharacter::GetCastingTarget() const {
    return m_pCastingTarget;
}

inline Bool BaseCharacter::IsOnGlobalCooldown() const {
    return ( m_fGlobalCooldown > 0.0f );
}
inline Scalar BaseCharacter::GetGlobalCooldown() const {
    return m_fGlobalCooldown;
}

inline UInt BaseCharacter::GetCurrentHP() const {
    return m_iCurrentHP;
}
inline UInt BaseCharacter::GetCurrentMP() const {
    return m_iCurrentMP;
}

inline Bool BaseCharacter::IsInCombat() const {
    return ( m_idCombat != INVALID_OFFSET );
}
inline CombatID BaseCharacter::GetCombatID() const {
    return m_idCombat;
}

inline Bool BaseCharacter::IsDead() const {
    return ( m_iCurrentHP == 0 );
}

/////////////////////////////////////////////////////////////////////////////////

inline UInt BaseCharacter::_ComputeMaxXP( UInt iLevel ) {
    return ( iLevel * 100 );
}

/////////////////////////////////////////////////////////////////////////////////
// PCharacter implementation
inline Bool PCharacter::IsPC() const {
    return true;
}
inline Bool PCharacter::IsNPC() const {
    return false;
}

/////////////////////////////////////////////////////////////////////////////////
// NPCharacter implementation
inline Bool NPCharacter::IsPC() const {
    return false;
}
inline Bool NPCharacter::IsNPC() const {
    return true;
}

inline Bool NPCharacter::HasThreat( BaseCharacter * pTarget ) const {
    return m_hThreatTable.HasThreat( pTarget );
}
inline UInt NPCharacter::GetThreat( BaseCharacter * pTarget ) const {
    return m_hThreatTable.GetThreat( pTarget );
}

inline BaseCharacter * NPCharacter::GetBiggestThreat( UInt * outThreat ) const {
    return m_hThreatTable.GetBiggestThreat( outThreat );
}

inline UInt NPCharacter::AddThreat( BaseCharacter * pTarget, UInt iThreat ) {
    return m_hThreatTable.AddThreat( pTarget, iThreat );
}
inline Void NPCharacter::ResetThreat() {
    m_hThreatTable.ResetThreat();
}
