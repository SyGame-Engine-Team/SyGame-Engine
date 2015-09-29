/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/GlobalEffects/EffectPlanarShadows.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Global Effect : Shadows casting on planes
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
#include "EffectPlanarShadows.h"

#include "../WorldManager.h"

///////////////////////////////////////////////////////////////////////////////////
//// EffectPlanarShadows implementation
//ShadowPlane::ShadowPlane( MeshQuad * pPlaneMesh, EffectInstance * pPlaneEffect, const GChar * strName ):
//    WorldLeaf( strName )
//{
//    m_vPlane = pPlaneMesh->GetGeometry().GetPlane();
//    m_colShadowColor = Color4::Black;
//
//    SetMesh( pPlaneMesh );
//    SetEffectInstance( pPlaneEffect );
//
//    CullMode = WORLD_CULL_ALLWAYS;
//}
//ShadowPlane::~ShadowPlane()
//{
//    SetEffectInstance( NULL );
//    SetMesh( NULL );
//}
//
//Bool ShadowPlane::ComputeProjection( Matrix4 * outProjection, const Light * pProjector, const WorldLeaf * pShadowCaster ) const
//{
//    // Conservative test, can cause wrong results with large casters that
//    // intersects the shadow plane but we hardly ever compute shadows
//    // in such situations ...
//    if ( pShadowCaster->WorldBV.WhichSide(m_vPlane) < 0 )
//        return false;
//
//    // Compute projection matrix
//    Vector3 vNormal = m_vPlane.Normal;
//    Scalar fDot;
//
//    if ( pProjector->GetType() == LIGHT_DIRECTIONAL ) {
//        fDot = ( vNormal * pProjector->Direction );
//        if ( fDot < 0.0f ) {
//            outProjection->MakeObliqueProjection( vNormal, m_vPlane.Position, pProjector->Direction );
//            return true;
//        }
//        return false;
//    }
//
//    Assert( pProjector->GetType() == LIGHT_OMNI || pProjector->GetType() == LIGHT_POINT ||
//            pProjector->GetType() == LIGHT_SPOT );
//
//    fDot = ( vNormal * pProjector->Position.ToVector() );
//    if ( fDot > 0.0f ) {
//        outProjection->MakePerspectiveProjection( vNormal, m_vPlane.Position, pProjector->Position );
//        return true;
//    }
//    return false;
//}
//
///////////////////////////////////////////////////////////////////////////////////
//// EffectPlanarShadows implementation
//EffectPlanarShadows::EffectPlanarShadows( UInt iPlaneCount, UInt iProjectorCount ):
//    GlobalEffect(),
//    m_DepthState(), m_StencilState(), m_BlendingState(),
//    m_Material(), m_MaterialEffect()
//{
//    WorldFn->SelectMemory( TEXT("Scratch") );
//
//    m_iPlaneCount = iPlaneCount;
//    m_arrPlanes = New ShadowPlane*[m_iPlaneCount];
//
//    m_iProjectorCount = iProjectorCount;
//    m_arrProjectors = New Light*[m_iProjectorCount];
//
//    m_pMaterialEffectInstance = m_MaterialEffect.CreateInstance( &m_Material );
//
//    WorldFn->UnSelectMemory();
//
//    UInt i;
//    for( i = 0; i < m_iPlaneCount; ++i )
//        m_arrPlanes[i] = NULL;
//    for( i = 0; i < m_iProjectorCount; ++i )
//        m_arrProjectors[i] = NULL;
//}
//EffectPlanarShadows::~EffectPlanarShadows()
//{
//    WorldFn->SelectMemory( TEXT("Scratch") );
//
//    Delete( m_pMaterialEffectInstance );
//    DeleteA( m_arrProjectors );
//    DeleteA( m_arrPlanes );
// 
//    WorldFn->UnSelectMemory();
//}
//
//Void EffectPlanarShadows::Render( WorldLeaf * pShadowCaster )
//{
//    // Get camera
//    Camera * pCamera = RenderingFn->GetCamera();
//
//    // Backup states
//    const DepthState * pSavedDepthState = RenderingFn->GetOverrideDepthState();
//    const StencilState * pSavedStencilState = RenderingFn->GetOverrideStencilState();
//    const BlendingState * pSavedBlendingState = RenderingFn->GetOverrideBlendingState();
//    EffectInstance * pSavedEffect;
//
//    // Main loop
//    ShadowPlane * pShadowPlane;
//    Light * pProjector;
//    Matrix4 matProjection;
//    Bool bValidProjection;
//
//    // Override Depth/Stencil states
//    RenderingFn->SetOverrideDepthState( &m_DepthState );
//    RenderingFn->SetOverrideStencilState( &m_StencilState );
//
//    for( UInt iPlane = 0; iPlane < m_iPlaneCount; ++iPlane ) {
//        pShadowPlane = m_arrPlanes[iPlane];
//        if ( pShadowPlane == NULL )
//            continue;
//        for( UInt iProjector = 0; iProjector < m_iProjectorCount; ++iProjector ) {
//            pProjector = m_arrProjectors[iProjector];
//            if ( pProjector == NULL )
//                continue;
//
//            // Compute projection matrix
//            bValidProjection = pShadowPlane->ComputeProjection( &matProjection, pProjector, pShadowCaster );
//            if ( !bValidProjection )
//                continue;
//
//            // Render the plane, marking visible pixels in the stencil buffer.
//            m_StencilState.StencilEnabled = true;
//            m_StencilState.StencilTest = STENCIL_TEST_ALLWAYS;
//            m_StencilState.Reference = (iPlane + 1);
//            m_StencilState.OnStencilFail = STENCIL_OP_KEEP;
//            m_StencilState.OnDepthFail = STENCIL_OP_KEEP;
//            m_StencilState.OnDepthSuccess = STENCIL_OP_REPLACE;
//            m_DepthState.DepthEnabled = true;
//            m_DepthState.DepthWritable = true;
//            m_DepthState.DepthTest = DEPTH_LESSER_EQUAL;
//
//            pShadowPlane->Render();
//
//            // Override Blending state
//            RenderingFn->SetOverrideBlendingState( &m_BlendingState );
//
//            // Blend shadow color with pixels on the projection plane
//            m_BlendingState.BlendEnabled = true;
//            m_BlendingState.BlendSrc = BLENDING_SRC_SRC_ALPHA;
//            m_BlendingState.BlendDst = BLENDING_DST_SRC_ALPHA_INVERT;
//            m_Material.Diffuse = pShadowPlane->GetShadowColor();
//
//            // Disable depth testing and let the stencil buffer select
//            // the pixels where the plane was drawn
//            m_StencilState.StencilEnabled = true;
//            m_StencilState.StencilTest = STENCIL_TEST_EQUAL;
//            m_StencilState.Reference = (iPlane + 1);
//            m_StencilState.OnStencilFail = STENCIL_OP_KEEP;
//            m_StencilState.OnDepthFail = STENCIL_OP_KEEP;
//            m_StencilState.OnDepthSuccess = STENCIL_OP_ZERO;
//            m_DepthState.DepthEnabled = false;
//
//            // Add projection
//            pCamera->SetPreViewMatrix( matProjection );
//
//            // Render the caster, using shadow color material effect
//            pSavedEffect = pShadowCaster->GetEffectInstance();
//            pShadowCaster->SetEffectInstance( m_pMaterialEffectInstance );
//            pShadowCaster->Render();
//            pShadowCaster->SetEffectInstance( pSavedEffect );
//
//            // Remove projection
//            pCamera->DisablePreView();
//
//            // Restore Blending state
//            RenderingFn->SetOverrideBlendingState( pSavedBlendingState );
//
//        }
//    }
//
//    // Restore Depth/Stencil states
//    RenderingFn->SetOverrideDepthState( pSavedDepthState );
//    RenderingFn->SetOverrideStencilState( pSavedStencilState );
//}

