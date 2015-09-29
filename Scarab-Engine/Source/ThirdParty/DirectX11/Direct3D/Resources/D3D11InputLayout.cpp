/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/Resources/D3D11InputLayout.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU resources : Input Layouts.
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
#include "D3D11InputLayout.h"

#include "../D3D11Renderer.h"

/////////////////////////////////////////////////////////////////////////////////
// D3D11InputLayout implementation
D3D11InputLayout::D3D11InputLayout( D3D11Renderer * pRenderer )
{
    m_pRenderer = pRenderer;

    m_iFieldCount = 0;

    m_bFinalized = false;
    m_pInputLayout = NULL;

    m_bTemporaryDestroyed = false;
    m_hCreationParameters.pCompiledShader = NULL;
}
D3D11InputLayout::~D3D11InputLayout()
{
    if ( IsCreated() )
        Destroy();
}

Void D3D11InputLayout::PushField( const D3D11InputFieldDesc * pFieldDesc )
{
    DebugAssert( !m_bFinalized );
    DebugAssert( m_iFieldCount < D3D11INPUTLAYOUT_MAX_FIELDS );

    if ( StringFn->CmpA(pFieldDesc->strSemantic, D3D11INPUTFIELD_SEMANTIC_COLOR) == 0 ) {
        DebugAssert( pFieldDesc->iSemanticIndex < D3D11INPUTLAYOUT_MAX_COLORS );
    } else if ( StringFn->CmpA(pFieldDesc->strSemantic, D3D11INPUTFIELD_SEMANTIC_TEXCOORD) == 0 ) {
        DebugAssert( pFieldDesc->iSemanticIndex < D3D11INPUTLAYOUT_MAX_TEXCOORDS );
    }
    DebugAssert( pFieldDesc->iSlot < 16 );

    pFieldDesc->ConvertTo( m_arrFields + m_iFieldCount );

    ++m_iFieldCount;
}
Void D3D11InputLayout::Finalize()
{
    DebugAssert( !m_bFinalized );

    m_bFinalized = true;
}

Void D3D11InputLayout::Create( const D3D11ShaderCompiler * pCompiledShader )
{
    DebugAssert( m_bFinalized );
    DebugAssert( !(IsCreated()) );

    m_hCreationParameters.pCompiledShader = pCompiledShader;

    _NakedCreate();
}
Void D3D11InputLayout::Destroy()
{
    DebugAssert( IsCreated() );

    if ( m_bTemporaryDestroyed )
        m_bTemporaryDestroyed = false;
    else
        _NakedDestroy();

    m_hCreationParameters.pCompiledShader = NULL;
}

Void D3D11InputLayout::OnDestroyDevice()
{
    DebugAssert( !m_bTemporaryDestroyed );

    if ( m_pInputLayout != NULL ) {
        _NakedDestroy();
        m_bTemporaryDestroyed = true;
    }
}
Void D3D11InputLayout::OnRestoreDevice()
{
    DebugAssert( m_pInputLayout == NULL );

    if ( m_bTemporaryDestroyed ) {
        _NakedCreate();
        m_bTemporaryDestroyed = false;
    }
}

/////////////////////////////////////////////////////////////////////////////////

Void D3D11InputLayout::_NakedCreate()
{
    m_pInputLayout = NULL;
    HRESULT hRes = m_pRenderer->m_pDevice->CreateInputLayout( (D3D11_INPUT_ELEMENT_DESC*)m_arrFields, m_iFieldCount, m_hCreationParameters.pCompiledShader->GetByteCode(),
                                                              m_hCreationParameters.pCompiledShader->GetByteCodeLength(), &m_pInputLayout );
    DebugAssert( hRes == S_OK && m_pInputLayout != NULL );
}
Void D3D11InputLayout::_NakedDestroy()
{
    m_pInputLayout->Release();
    m_pInputLayout = NULL;
}

