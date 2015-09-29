/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Gameplay/StatusEffects.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Gameplay : StatusEffects
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
#include "StatusEffects.h"

#include "GameplayManager.h"

/////////////////////////////////////////////////////////////////////////////////
// StatusEffect implementation
StatusEffect::StatusEffect()
{
    m_iType = STATUSEFFECT_UNDEFINED;
    m_idStatusEffect = STATUSEFFECTID_UNDEFINED;
    m_strName[0] = NULLBYTE;

    m_iMaxRank = 0;

    m_bPositive = false;
    m_bMovementLimiter = false;
    m_bActionLimiter = false;
    m_bStatModifier = false;

    m_fDuration = 0.0f;
    m_fTickPeriod = 0.0f;
    m_fTickFrequency = 0.0f;

    m_iMaxStacks = 0;

    m_pAnimation = NULL;
}
StatusEffect::~StatusEffect()
{
    // nothing to do
}

StatusEffect * StatusEffect::GetInstance( StatusEffectID idStatusEffect )
{
    return GameplayFn->GetStatusEffect( idStatusEffect );
}

Void StatusEffect::GenerateInstance( StatusEffectInstance * outStatusEffectInstance, BaseCharacter * pOwner, BaseCharacter * pSource, UInt iRank, UInt iStacks )
{
    Assert( iRank > 0 && iStacks > 0 );

    outStatusEffectInstance->m_pStatusEffect = this;
    outStatusEffectInstance->m_pOwner = pOwner;
    outStatusEffectInstance->m_pSource = pSource;

    outStatusEffectInstance->m_iRank = iRank;

    outStatusEffectInstance->m_fElapsedTime = 0.0f;
    outStatusEffectInstance->m_fLastTickElapsedTime = 0.0f;

    outStatusEffectInstance->m_iStacks = iStacks;

    m_pAnimation->GenerateInstance( &(outStatusEffectInstance->m_hAnimationInstance), m_fDuration, pOwner, pOwner );

    OnApply( outStatusEffectInstance );
}

/////////////////////////////////////////////////////////////////////////////////
// StatusEffectInstance implementation
StatusEffectInstance::StatusEffectInstance():
    m_hAnimationInstance()
{
    m_pStatusEffect = NULL;
    m_pOwner = NULL;
    m_pSource = NULL;

    m_iRank = 0;

    m_fElapsedTime = 0.0f;
    m_fLastTickElapsedTime = 0.0f;

    m_iStacks = 0;
}
StatusEffectInstance::~StatusEffectInstance()
{
    if ( m_hAnimationInstance.GetAnimation() != NULL )
        m_hAnimationInstance.Interrupt();
}

Bool StatusEffectInstance::Update( Scalar fTimeStep )
{
    Assert( m_pStatusEffect != NULL );

    // Zombie effects (should not happen)
    Scalar fDuration = m_pStatusEffect->GetDuration();
    if ( m_fElapsedTime >= fDuration )
        return false;

    Assert( m_hAnimationInstance.GetAnimation() != NULL );

    // Update status effect animation & compute overtime
    Scalar fOverTime = 0.0f;
    m_fElapsedTime += fTimeStep;
    Bool bContinue = m_hAnimationInstance.Update( fTimeStep );

    if ( !bContinue && m_fElapsedTime >= fDuration ) {
        fOverTime = ( m_fElapsedTime - fDuration );
        m_fElapsedTime = fDuration;
        m_hAnimationInstance = AnimationInstance();
    }

    // Update ticks
    Scalar fTickPeriod = m_pStatusEffect->GetTickPeriod();
    m_fLastTickElapsedTime += ( fTimeStep - fOverTime );
    while( m_fLastTickElapsedTime > fTickPeriod ) {
        m_fLastTickElapsedTime -= fTickPeriod;
        m_pStatusEffect->OnTick( this );
    }

    // Fade
    if ( !bContinue ) {
        m_fLastTickElapsedTime = 0.0f;
        m_pStatusEffect->OnFade( this );
    }

    return bContinue;
}

/////////////////////////////////////////////////////////////////////////////////
// StatusEffectBuffer implementation
StatusEffectBuffer::StatusEffectBuffer( BaseCharacter * pOwner ):
    m_mapStatusEffects(), m_itEnumerate()
{
    m_pOwner = pOwner;

    m_mapStatusEffects.SetComparator( _Compare_StatusEffectIDs, NULL );
    m_mapStatusEffects.UseMemoryContext( GameplayFn->GetMemoryContext(), TEXT("Scratch") );
    m_mapStatusEffects.Create();

    m_iEnumerate = INVALID_OFFSET;
}
StatusEffectBuffer::~StatusEffectBuffer()
{
    m_mapStatusEffects.Destroy();
}

Bool StatusEffectBuffer::AddEffect( StatusEffectID idStatusEffect, BaseCharacter * pSource, UInt iRank, UInt iStacks )
{
    StatusEffect * pStatusEffect = GameplayFn->GetStatusEffect( idStatusEffect );
    _StatusEffectMap::Iterator itStatusEffect = m_mapStatusEffects.Get( idStatusEffect );

    // New effect case
    if ( itStatusEffect.IsNull() ) {
        _StatusEffectData hData;
        hData.iInstanceCount = 1;
        StatusEffectInstance * pInstance = hData.arrInstances;

        pStatusEffect->GenerateInstance( pInstance, m_pOwner, pSource, iRank, iStacks );

        Bool bInserted = m_mapStatusEffects.Insert( idStatusEffect, hData );
        Assert( bInserted );

        return true;
    }

    _StatusEffectData * pData = itStatusEffect.GetItemPtr();

    // Check for the same source
    for( UInt i = 0; i < pData->iInstanceCount; ++i ) {
        StatusEffectInstance * pInstance = ( pData->arrInstances + i );

        if ( pInstance->GetSource() == pSource ) {
            // Stacking/Refresh case
            if ( pInstance->GetRank() == iRank ) {
                pInstance->AddStacks( iStacks );
                return true;
            }

            // Replace case
            if ( pInstance->GetRank() < iRank ) {
                pStatusEffect->GenerateInstance( pInstance, m_pOwner, pSource, iRank, iStacks );
                return true;
            }

            // Wasted case
            return false;
        }
    }

    // Out of room case
    if ( pData->iInstanceCount >= STATUSEFFECT_MAX_INSTANCES )
        return false;

    // Append case
    StatusEffectInstance * pInstance = ( pData->arrInstances + pData->iInstanceCount );
    ++(pData->iInstanceCount);
    pStatusEffect->GenerateInstance( pInstance, m_pOwner, pSource, iRank, iStacks );
    return true;
}
Bool StatusEffectBuffer::RemoveEffect( StatusEffectID idStatusEffect )
{
    _StatusEffectData hData;
    Bool bRemoved = m_mapStatusEffects.Remove( idStatusEffect, &hData );
    if ( !bRemoved )
        return false;

    for( UInt i = 0; i < hData.iInstanceCount; ++i )
        hData.arrInstances[i].Fade();

    return true;
}

Bool StatusEffectBuffer::RemoveEffects( StatusEffectType iType )
{
    Bool bHasChanged = false;

    _StatusEffectMap::Iterator itStatusEffect = m_mapStatusEffects.Begin();
    while( !(itStatusEffect.IsNull()) ) {
        if ( StatusEffect::GetInstance(itStatusEffect.GetKey())->GetType() != iType ) {
            ++itStatusEffect;
            continue;
        }

        bHasChanged = true;

        _StatusEffectData * pData = itStatusEffect.GetItemPtr();
        for( UInt i = 0; i < pData->iInstanceCount; ++i )
            pData->arrInstances[i].Fade();

        _StatusEffectMap::Iterator itTmp = itStatusEffect;
        ++itStatusEffect;
        Bool bRemoved = m_mapStatusEffects.Remove( itTmp.GetKey() );
        Assert( bRemoved );
    }

    return bHasChanged;
}
Bool StatusEffectBuffer::RemovePositiveEffects()
{
    Bool bHasChanged = false;

    _StatusEffectMap::Iterator itStatusEffect = m_mapStatusEffects.Begin();
    while( !(itStatusEffect.IsNull()) ) {
        if ( !(StatusEffect::GetInstance(itStatusEffect.GetKey())->IsPositive()) ) {
            ++itStatusEffect;
            continue;
        }

        bHasChanged = true;

        _StatusEffectData * pData = itStatusEffect.GetItemPtr();
        for( UInt i = 0; i < pData->iInstanceCount; ++i )
            pData->arrInstances[i].Fade();

        _StatusEffectMap::Iterator itTmp = itStatusEffect;
        ++itStatusEffect;
        Bool bRemoved = m_mapStatusEffects.Remove( itTmp.GetKey() );
        Assert( bRemoved );
    }

    return bHasChanged;
}
Bool StatusEffectBuffer::RemoveNegativeEffects()
{
    Bool bHasChanged = false;

    _StatusEffectMap::Iterator itStatusEffect = m_mapStatusEffects.Begin();
    while( !(itStatusEffect.IsNull()) ) {
        if ( !(StatusEffect::GetInstance(itStatusEffect.GetKey())->IsNegative()) ) {
            ++itStatusEffect;
            continue;
        }

        bHasChanged = true;

        _StatusEffectData * pData = itStatusEffect.GetItemPtr();
        for( UInt i = 0; i < pData->iInstanceCount; ++i )
            pData->arrInstances[i].Fade();

        _StatusEffectMap::Iterator itTmp = itStatusEffect;
        ++itStatusEffect;
        Bool bRemoved = m_mapStatusEffects.Remove( itTmp.GetKey() );
        Assert( bRemoved );
    }

    return bHasChanged;
}
Bool StatusEffectBuffer::RemoveMovementLimiterEffects()
{
    Bool bHasChanged = false;

    _StatusEffectMap::Iterator itStatusEffect = m_mapStatusEffects.Begin();
    while( !(itStatusEffect.IsNull()) ) {
        if ( !(StatusEffect::GetInstance(itStatusEffect.GetKey())->IsMovementLimiter()) ) {
            ++itStatusEffect;
            continue;
        }

        bHasChanged = true;

        _StatusEffectData * pData = itStatusEffect.GetItemPtr();
        for( UInt i = 0; i < pData->iInstanceCount; ++i )
            pData->arrInstances[i].Fade();

        _StatusEffectMap::Iterator itTmp = itStatusEffect;
        ++itStatusEffect;
        Bool bRemoved = m_mapStatusEffects.Remove( itTmp.GetKey() );
        Assert( bRemoved );
    }

    return bHasChanged;
}
Bool StatusEffectBuffer::RemoveActionLimiterEffects()
{
    Bool bHasChanged = false;

    _StatusEffectMap::Iterator itStatusEffect = m_mapStatusEffects.Begin();
    while( !(itStatusEffect.IsNull()) ) {
        if ( !(StatusEffect::GetInstance(itStatusEffect.GetKey())->IsActionLimiter()) ) {
            ++itStatusEffect;
            continue;
        }

        bHasChanged = true;

        _StatusEffectData * pData = itStatusEffect.GetItemPtr();
        for( UInt i = 0; i < pData->iInstanceCount; ++i )
            pData->arrInstances[i].Fade();

        _StatusEffectMap::Iterator itTmp = itStatusEffect;
        ++itStatusEffect;
        Bool bRemoved = m_mapStatusEffects.Remove( itTmp.GetKey() );
        Assert( bRemoved );
    }

    return bHasChanged;
}
Bool StatusEffectBuffer::RemoveAllEffects()
{
    Bool bHasChanged = ( m_mapStatusEffects.Count() > 0 );

    for( _StatusEffectMap::Iterator itStatusEffect = m_mapStatusEffects.Begin(); !(itStatusEffect.IsNull()); ++itStatusEffect ) {
        _StatusEffectData * pData = itStatusEffect.GetItemPtr();
        for( UInt i = 0; i < pData->iInstanceCount; ++i )
            pData->arrInstances[i].Fade();
    }

    m_mapStatusEffects.Clear();

    return bHasChanged;
}

Bool StatusEffectBuffer::Update( Scalar fTimeStep )
{
    Bool bHasChanged = false;

    _StatusEffectMap::Iterator itStatusEffect = m_mapStatusEffects.Begin();
    while( !(itStatusEffect.IsNull()) ) {
        _StatusEffectData * pData = itStatusEffect.GetItemPtr();

        UInt i = 0;
        while( i < pData->iInstanceCount ) {
            Bool bContinue = pData->arrInstances[i].Update( fTimeStep );
            if ( bContinue ) {
                ++i;
                continue;
            }

            bHasChanged = true;

            // Remove faded instances
            if ( pData->iInstanceCount == 1 ) {
                pData->arrInstances[0] = StatusEffectInstance();
                pData->iInstanceCount = 0;
                break;
            }
            if ( i == (pData->iInstanceCount - 1) ) {
                pData->arrInstances[i] = StatusEffectInstance();
                --(pData->iInstanceCount);
                break;
            }

            pData->arrInstances[i] = pData->arrInstances[pData->iInstanceCount - 1];
            pData->arrInstances[pData->iInstanceCount - 1] = StatusEffectInstance();
            --(pData->iInstanceCount);
        }


        // Remove when all instances have faded
        if ( pData->iInstanceCount == 0 ) {
            _StatusEffectMap::Iterator itTmp = itStatusEffect;
            ++itStatusEffect;
            Bool bRemoved = m_mapStatusEffects.Remove( itTmp.GetKey() );
            Assert( bRemoved );
        } else
            ++itStatusEffect;
    }

    return bHasChanged;
}

/////////////////////////////////////////////////////////////////////////////////

Int StatusEffectBuffer::_Compare_StatusEffectIDs( const StatusEffectID & rLeft, const StatusEffectID & rRight, Void * /*pUserData*/ )
{
    if ( rLeft < rRight )
        return +1;
    if ( rLeft > rRight )
        return -1;
    return 0;
}
