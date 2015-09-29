/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Effects/Effect.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : The fundamental type for the effect system.
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
#include "Effect.h"

#include "../RenderingManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Effect implementation
Effect::Effect()
{
    m_iTechniqueCount = 0;
    for( UInt i = 0; i < EFFECT_MAX_TECHNIQUES; ++i )
        m_arrTechniques[i] = NULL;
}
Effect::~Effect()
{
    // nothing to do
}

Void Effect::AddTechnique( EffectTechnique * pTechnique )
{
    Assert( m_iTechniqueCount < EFFECT_MAX_TECHNIQUES );
    Assert( pTechnique != NULL );

    m_arrTechniques[m_iTechniqueCount] = pTechnique;
    ++m_iTechniqueCount;
}
EffectTechnique * Effect::RemoveTechnique( UInt iTechnique )
{
    Assert( iTechnique < m_iTechniqueCount );

    EffectTechnique * pOldTechnique = m_arrTechniques[iTechnique];

    --m_iTechniqueCount;
    for( UInt i = iTechnique; i < m_iTechniqueCount; ++i )
        m_arrTechniques[i] = m_arrTechniques[i+1];
    m_arrTechniques[m_iTechniqueCount] = NULL;

    return pOldTechnique;
}
EffectTechnique * Effect::SetTechnique( UInt iTechnique, EffectTechnique * pTechnique )
{
    Assert( iTechnique < m_iTechniqueCount );
    Assert( pTechnique != NULL );

    EffectTechnique * pOldTechnique = m_arrTechniques[iTechnique];

    m_arrTechniques[iTechnique] = pTechnique;

    return pOldTechnique;
}

Void Effect::DestroyInstance( EffectInstance * pInstance )
{
    RenderingFn->SelectMemory( TEXT("Effects") );
    Delete( pInstance );
	RenderingFn->UnSelectMemory();
}

