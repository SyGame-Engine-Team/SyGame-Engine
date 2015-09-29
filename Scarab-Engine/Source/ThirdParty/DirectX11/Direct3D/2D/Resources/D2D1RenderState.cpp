/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/2D/Resources/D2D1RenderState.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : D2D1 Dev-Ind Resource : Render States.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Third-Party Includes
#pragma warning(disable:4005)

#define WIN32_LEAN_AND_MEAN
#include <d2d1.h>
#include <dwrite.h>

#undef DebugAssert

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "D2D1RenderState.h"

#include "../D2D1RenderingContext.h"

/////////////////////////////////////////////////////////////////////////////////
// D2D1RenderState implementation
D2D1RenderState::D2D1RenderState()
{
    m_pStateBlock = NULL;
}
D2D1RenderState::~D2D1RenderState()
{
    if ( IsCreated() )
        Destroy();
}

Void D2D1RenderState::Create( const D2D1RenderStateDesc * pDesc, D2D1TextRenderState * pTextRenderState )
{
    DebugAssert( !(IsCreated()) );
    DebugAssert( pTextRenderState == NULL || pTextRenderState->IsCreated() );

    D2D1_DRAWING_STATE_DESCRIPTION hDesc;
    pDesc->ConvertTo( &hDesc );

    m_pStateBlock = NULL;
    HRESULT hRes = D2D1RenderingContext::sm_pD2D1Factory->CreateDrawingStateBlock( &hDesc, (pTextRenderState != NULL) ? pTextRenderState->m_pTextRenderingParams : NULL, &m_pStateBlock );
    DebugAssert( hRes == S_OK && m_pStateBlock != NULL );
}
Void D2D1RenderState::Destroy()
{
    DebugAssert( IsCreated() );

    m_pStateBlock->Release();
    m_pStateBlock = NULL;
}

Void D2D1RenderState::GetDesc( D2D1RenderStateDesc * outDesc ) const
{
    DebugAssert( IsCreated() );

    D2D1_DRAWING_STATE_DESCRIPTION hDesc;
    m_pStateBlock->GetDescription( &hDesc );

    outDesc->ConvertFrom( &hDesc );
}
Void D2D1RenderState::SetDesc( const D2D1RenderStateDesc * pDesc )
{
    DebugAssert( IsCreated() );

    D2D1_DRAWING_STATE_DESCRIPTION hDesc;
    pDesc->ConvertTo( &hDesc );

    m_pStateBlock->SetDescription( &hDesc );
}

Void D2D1RenderState::GetTextRenderState( D2D1TextRenderState * outTextRenderState ) const
{
    DebugAssert( IsCreated() );
    DebugAssert( !(outTextRenderState->IsCreated()) );

    m_pStateBlock->GetTextRenderingParams( &(outTextRenderState->m_pTextRenderingParams) );
}
Void D2D1RenderState::SetTextRenderState( D2D1TextRenderState * pTextRenderState )
{
    DebugAssert( IsCreated() );
    DebugAssert( pTextRenderState == NULL || pTextRenderState->IsCreated() );

    m_pStateBlock->SetTextRenderingParams( (pTextRenderState != NULL) ? pTextRenderState->m_pTextRenderingParams : NULL );
}

/////////////////////////////////////////////////////////////////////////////////
// D2D1TextRenderState implementation
D2D1TextRenderState::D2D1TextRenderState()
{
    m_pTextRenderingParams = NULL;
}
D2D1TextRenderState::~D2D1TextRenderState()
{
    if ( IsCreated() )
        Destroy();
}

Void D2D1TextRenderState::Create( Void * pMonitor )
{
    DebugAssert( !(IsCreated()) );

    m_pTextRenderingParams = NULL;
    if ( pMonitor != NULL ) {
        HRESULT hRes = D2D1RenderingContext::sm_pDWFactory->CreateMonitorRenderingParams( (HMONITOR)pMonitor, &m_pTextRenderingParams );
        DebugAssert( hRes == S_OK && m_pTextRenderingParams != NULL );
    } else {
        HRESULT hRes = D2D1RenderingContext::sm_pDWFactory->CreateRenderingParams( &m_pTextRenderingParams );
        DebugAssert( hRes == S_OK && m_pTextRenderingParams != NULL );
    }
}
Void D2D1TextRenderState::Create( const D2D1TextRenderStateDesc * pDesc )
{
    DebugAssert( !(IsCreated()) );

    m_pTextRenderingParams = NULL;
    HRESULT hRes = D2D1RenderingContext::sm_pDWFactory->CreateCustomRenderingParams( pDesc->fGamma, pDesc->fEnhancedContrast, pDesc->fClearTypeLevel,
                                                                                     D2D1TextPixelGeometryToD2D1[pDesc->iPixelGeometry],
                                                                                     D2D1TextRenderingModeToD2D1[pDesc->iRenderingMode], &m_pTextRenderingParams );
    DebugAssert( hRes == S_OK && m_pTextRenderingParams != NULL );
}
Void D2D1TextRenderState::Destroy()
{
    DebugAssert( IsCreated() );

    m_pTextRenderingParams->Release();
    m_pTextRenderingParams = NULL;
}

Float D2D1TextRenderState::GetGamma() const
{
    DebugAssert( IsCreated() );

    return m_pTextRenderingParams->GetGamma();
}
Float D2D1TextRenderState::GetEnhancedContrast() const
{
    DebugAssert( IsCreated() );

    return m_pTextRenderingParams->GetEnhancedContrast();
}
Float D2D1TextRenderState::GetClearTypeLevel() const
{
    DebugAssert( IsCreated() );

    return m_pTextRenderingParams->GetClearTypeLevel();
}
D2D1TextPixelGeometry D2D1TextRenderState::GetPixelGeometry() const
{
    DebugAssert( IsCreated() );

    return D2D1TextPixelGeometryFromD2D1[m_pTextRenderingParams->GetPixelGeometry()];
}
D2D1TextRenderingMode D2D1TextRenderState::GetRenderingMode() const
{
    DebugAssert( IsCreated() );

    return D2D1TextRenderingModeFromD2D1[m_pTextRenderingParams->GetRenderingMode()];
}

Void D2D1TextRenderState::GetDesc( D2D1TextRenderStateDesc * outDesc ) const
{
    DebugAssert( IsCreated() );

    outDesc->fGamma = m_pTextRenderingParams->GetGamma();
    outDesc->fEnhancedContrast = m_pTextRenderingParams->GetEnhancedContrast();
    outDesc->fClearTypeLevel = m_pTextRenderingParams->GetClearTypeLevel();
    outDesc->iPixelGeometry = D2D1TextPixelGeometryFromD2D1[m_pTextRenderingParams->GetPixelGeometry()];
    outDesc->iRenderingMode = D2D1TextRenderingModeFromD2D1[m_pTextRenderingParams->GetRenderingMode()];
}
