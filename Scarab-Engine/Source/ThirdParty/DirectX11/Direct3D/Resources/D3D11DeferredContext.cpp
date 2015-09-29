/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/Resources/D3D11DeferredContext.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU resources : Deferred Contexts.
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
#include <d3d11.h>

#undef DebugAssert

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "D3D11DeferredContext.h"

#include "../D3D11Renderer.h"

/////////////////////////////////////////////////////////////////////////////////
// D3D11CommandList implementation
D3D11CommandList::D3D11CommandList()
{
    m_pCommandList = NULL;
}
D3D11CommandList::~D3D11CommandList()
{
    if ( m_pCommandList != NULL ) {
        m_pCommandList->Release();
        m_pCommandList = NULL;
    }
}

Void D3D11CommandList::Clear()
{
    if ( m_pCommandList != NULL ) {
        m_pCommandList->Release();
        m_pCommandList = NULL;
    }
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11DeferredContext implementation
D3D11DeferredContext::D3D11DeferredContext( D3D11Renderer * pRenderer )
{
    m_pRenderer = pRenderer;

    m_pDeferredContext = NULL;

    m_bTemporaryDestroyed = false;
}
D3D11DeferredContext::~D3D11DeferredContext()
{
    if ( IsCreated() )
        Destroy();
}

Void D3D11DeferredContext::Create()
{
    DebugAssert( !(IsCreated()) );

    _NakedCreate();
}
Void D3D11DeferredContext::Destroy()
{
    DebugAssert( IsCreated() );

    if ( m_bTemporaryDestroyed )
        m_bTemporaryDestroyed = false;
    else
        _NakedDestroy();
}

Void D3D11DeferredContext::OnDestroyDevice()
{
    DebugAssert( !m_bTemporaryDestroyed );

    if ( m_pDeferredContext != NULL ) {
        _NakedDestroy();
        m_bTemporaryDestroyed = true;
    }
}
Void D3D11DeferredContext::OnRestoreDevice()
{
    DebugAssert( m_pDeferredContext == NULL );

    if ( m_bTemporaryDestroyed ) {
        _NakedCreate();
        m_bTemporaryDestroyed = false;
    }
}

Void D3D11DeferredContext::MakeCommandList( D3D11CommandList * outCommandList, Bool bRestoreDeferredContextState )
{
    DebugAssert( IsCreated() );

    ID3D11CommandList * pCommandList = NULL;
    HRESULT hRes = m_pDeferredContext->FinishCommandList( (bRestoreDeferredContextState) ? TRUE : FALSE, &pCommandList );
    DebugAssert( hRes == S_OK && pCommandList != NULL );

    outCommandList->Clear();
    outCommandList->m_pCommandList = pCommandList;
}

Void D3D11DeferredContext::Clear()
{
    DebugAssert( IsCreated() );

    m_pDeferredContext->ClearState();
}
Void D3D11DeferredContext::Reset()
{
    DebugAssert( IsCreated() );

    ID3D11CommandList * pCommandList = NULL;
    HRESULT hRes = m_pDeferredContext->FinishCommandList( FALSE, &pCommandList );
    DebugAssert( hRes == S_OK && pCommandList != NULL );

    pCommandList->Release();
    pCommandList = NULL;

    m_pDeferredContext->ClearState();
}

/////////////////////////////////////////////////////////////////////////////////

Void D3D11DeferredContext::_NakedCreate()
{
    UInt iFlags = 0; // Future use for now ...

    m_pDeferredContext = NULL;
    HRESULT hRes = m_pRenderer->m_pDevice->CreateDeferredContext( iFlags, &m_pDeferredContext );
    DebugAssert( hRes == S_OK && m_pDeferredContext != NULL );
}
Void D3D11DeferredContext::_NakedDestroy()
{
    m_pDeferredContext->Release();
    m_pDeferredContext = NULL;
}

