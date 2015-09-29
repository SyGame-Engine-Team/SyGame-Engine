/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Effects/EffectTechnique.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Multi-pass chaining for the effect system.
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
#include "EffectTechnique.h"

/////////////////////////////////////////////////////////////////////////////////
// EffectTechnique implementation
EffectTechnique::EffectTechnique()
{
    m_iPassCount = 0;
    for( UInt i = 0; i < EFFECT_TECHNIQUE_MAX_PASSES; ++i )
        m_arrPasses[i] = NULL;
}
EffectTechnique::~EffectTechnique()
{
    // nothing to do
}

Void EffectTechnique::AddPass( EffectPass * pPass )
{
    Assert( m_iPassCount < EFFECT_TECHNIQUE_MAX_PASSES );
    Assert( pPass != NULL );

    m_arrPasses[m_iPassCount] = pPass;
    ++m_iPassCount;
}
EffectPass * EffectTechnique::RemovePass( UInt iPass )
{
    Assert( iPass < m_iPassCount );

    EffectPass * pOldPass = m_arrPasses[iPass];

    --m_iPassCount;
    for( UInt i = iPass; i < m_iPassCount; ++i )
        m_arrPasses[i] = m_arrPasses[i+1];
    m_arrPasses[m_iPassCount] = NULL;

    return pOldPass;
}
EffectPass * EffectTechnique::SetPass( UInt iPass, EffectPass * pPass )
{
    Assert( iPass < m_iPassCount );
    Assert( pPass != NULL );

    EffectPass * pOldPass = m_arrPasses[iPass];

    m_arrPasses[iPass] = pPass;

    return pOldPass;
}
