/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/GlobalEffects/EffectPlanarReflection.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Global Effect : Reflection of silhouettes on planes
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
#include "EffectPlanarReflection.h"

/////////////////////////////////////////////////////////////////////////////////
// EffectPlanarReflection implementation
//EffectPlanarReflection::EffectPlanarReflection( UInt iPlaneCount ):
//    GlobalEffect(),
//    m_CullingState(), m_DepthState(), m_StencilState(), m_BlendingState()
//{
//    m_iPlaneCount = iPlaneCount;
//
//    RenderingFn->SelectMemory( TEXT("Scratch") );
//    m_arrPlanes = New WorldTriangleMesh*[m_iPlaneCount];
//    m_arrReflectances = New Scalar[m_iPlaneCount];
//    RenderingFn->UnSelectMemory();
//}
//EffectPlanarReflection::~EffectPlanarReflection()
//{
//    RenderingFn->SelectMemory( TEXT("Scratch") );
//    DeleteA( m_arrPlanes );
//    DeleteA( m_arrReflectances );
//    RenderingFn->UnSelectMemory();
//}
//
//Void EffectPlanarReflection::Draw( const VisibleGroup & visibleNodes )
//{
//    WorldLeaf * pVisible;
//
//    // Backup states
//    const CullingState * pSavedCullState = RenderingFn->GetOverrideCullingState();
//    if ( pSavedCullState != NULL )
//        MemCopy( &m_CullingState, pSavedCullState, sizeof(CullingState) );
//    else
//        MemCopy( &m_CullingState, RenderingFn->GetCullingState(), sizeof(CullingState) );
//    const DepthState * pSavedDepthState = RenderingFn->GetOverrideDepthState();
//    const StencilState * pSavedStencilState = RenderingFn->GetOverrideStencilState();
//    const BlendingState * pSavedBlendingState = RenderingFn->GetOverrideBlendingState();
//    Scalar fSavedMinDepth, fSavedMaxDepth;
//    RenderingFn->GetDepthRange( fSavedMinDepth, fSavedMaxDepth );
//
//    // Setup Depth/Stencil states
//    RenderingFn->SetOverrideDepthState( &m_DepthState );
//    RenderingFn->SetOverrideStencilState( &m_StencilState );
//
//    // Get camera
//    Camera * pCamera = RenderingFn->GetCamera();
//
//    // Main loop
//    Matrix4 matReflection;
//    Plane modelPlane;
//    UInt i, j;
//    const UInt iVisiblesCount = visibleNodes.GetCount();
//    for( i = 0; i < m_iPlaneCount; ++i ) {
//        // Render the mirror plane into the stencil buffer.
//        // Visible mirror pixels will take the plane's stencil value,
//        // disable depth/color writting, keep depth testing though.
//        m_StencilState.StencilEnabled = true;
//        m_StencilState.StencilTest = STENCIL_TEST_ALLWAYS;
//        m_StencilState.Reference = (i + 1);
//        m_StencilState.OnStencilFail = STENCIL_OP_KEEP;
//        m_StencilState.OnDepthFail = STENCIL_OP_KEEP;
//        m_StencilState.OnDepthSuccess = STENCIL_OP_REPLACE;
//        m_DepthState.DepthEnabled = true;
//        m_DepthState.DepthWritable = false;
//        m_DepthState.DepthTest = DEPTH_LESSER_EQUAL;
//
//        RenderingFn->SetColorMask( false, false, false, false );
//        RenderingFn->DrawPrimitive( m_arrPlanes[i] );
//
//        // Render the mirror again, processing only marked pixels.
//        // Keep stencil buffer unchanged, disable depth testing only
//        // for marked pixels.
//        m_StencilState.StencilEnabled = true;
//        m_StencilState.StencilTest = STENCIL_TEST_EQUAL;
//        m_StencilState.Reference = (i + 1);
//        m_StencilState.OnStencilFail = STENCIL_OP_KEEP;
//        m_StencilState.OnDepthFail = STENCIL_OP_KEEP;
//        m_StencilState.OnDepthSuccess = STENCIL_OP_KEEP;
//        m_DepthState.DepthEnabled = true;
//        m_DepthState.DepthWritable = true;
//        m_DepthState.DepthTest = DEPTH_ALLWAYS;
//
//        RenderingFn->SetDepthRange( 1.0f, 1.0f );
//        RenderingFn->SetColorMask( true, true, true, true );
//        RenderingFn->DrawPrimitive( m_arrPlanes[i] );
//
//        // Restore depth test & depth range
//        m_DepthState.DepthTest = DEPTH_LESSER_EQUAL;
//        RenderingFn->SetDepthRange( fSavedMinDepth, fSavedMaxDepth );
//
//        // TODO : Should add the world plane as a clipping plane,
//        //        using user clip planes support in ClippingState,
//        //        right here ...
//
//        // Pick reflection matrix and insert it between world and
//        // view transforms in the pipeline
//        _GetReflectionData( i, matReflection, modelPlane );
//        pCamera->SetPreViewMatrix( matReflection );
//
//        // Reverse Culling direction
//        RenderingFn->SetOverrideCullingState( &m_CullingState );
//        m_CullingState.CullConv = (CullingConvention)( 1 - (UInt)(m_CullingState.CullConv) );
//
//        // Render reflected objects
//        for( j = 0; j < iVisiblesCount; ++j ) {
//            pVisible = (WorldLeaf*)( visibleNodes.GetNode(j) );
//            RenderingFn->DrawPrimitive( pVisible );
//        }
//
//        // Restore Culling direction
//        RenderingFn->SetOverrideCullingState( pSavedCullState );
//
//        // Remove reflection
//        pCamera->SetPreViewMatrix( Matrix4::Identity );
//
//        // TODO : ... and disable it right there.
//
//        // Setup Blending state
//        RenderingFn->SetOverrideBlendingState( &m_BlendingState );
//        m_BlendingState.BlendEnabled = true;
//        m_BlendingState.BlendSrc = BLENDING_SRC_CONST_ALPHA_INVERT;
//        m_BlendingState.BlendDst = BLENDING_DST_CONST_ALPHA;
//        m_BlendingState.ConstantColor = Color4(0.0f, 0.0f, 0.0f, m_arrReflectances[i]);
//
//        // Now we render the reflected plane again for marked pixels,
//        // clearing them on the fly.
//        m_StencilState.StencilEnabled = true;
//        m_StencilState.StencilTest = STENCIL_TEST_EQUAL;
//        m_StencilState.Reference = (i + 1);
//        m_StencilState.OnStencilFail = STENCIL_OP_KEEP;
//        m_StencilState.OnDepthFail = STENCIL_OP_KEEP;
//        m_StencilState.OnDepthSuccess = STENCIL_OP_INVERT;
//
//        RenderingFn->DrawPrimitive( m_arrPlanes[i] );
//
//        // Restore Blending state
//        RenderingFn->SetOverrideBlendingState( pSavedBlendingState );
//    }
//
//    // Restore states
//    RenderingFn->SetOverrideDepthState( pSavedDepthState );
//    RenderingFn->SetOverrideStencilState( pSavedStencilState );
//
//    // Render everything as usual
//    for( j = 0; j < iVisiblesCount; ++j ) {
//        pVisible = (WorldLeaf*)( visibleNodes.GetNode(i) );
//        RenderingFn->DrawPrimitive( pVisible );
//    }
//}
//
///////////////////////////////////////////////////////////////////////////////////
//
//Void EffectPlanarReflection::_GetReflectionData( UInt iPlane, Matrix4 & outReflection, Plane & outModelPlane ) const
//{
//    // Reflection matrix
//    Vertex3 arrVertices[3];
//    m_arrPlanes[iPlane]->GetWorldTriangle( 0, arrVertices );
//    Triangle3 tmpTriangle( arrVertices );
//    Plane worldPlane = tmpTriangle.GetPlane();
//    outReflection.MakeReflection( worldPlane.Normal, worldPlane.Position );
//
//    // modelPlane
//    m_arrPlanes[iPlane]->GetModelTriangle( 0, arrVertices );
//    tmpTriangle = Triangle3( arrVertices );
//    outModelPlane = tmpTriangle.GetPlane();
//}

