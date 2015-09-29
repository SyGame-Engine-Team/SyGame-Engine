/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/2D/Resources/D2D1Layer.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : D2D1 Dev-Dep Resource : Layers.
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

#undef DebugAssert

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "D2D1Layer.h"

#include "../D2D1RenderingContext.h"

/////////////////////////////////////////////////////////////////////////////////
// D2D1Layer implementation
D2D1Layer::D2D1Layer( D2D1RenderingContext * pContext2D )
{
    m_pContext2D = pContext2D;

    m_hInitialDIPSize.width = 0.0f;
    m_hInitialDIPSize.height = 0.0f;

    m_pLayer = NULL;

    m_bTemporaryDestroyed = false;
}
D2D1Layer::~D2D1Layer()
{
    if ( IsCreated() )
        Destroy();
}

Void D2D1Layer::Create( Float fInitialDIPWidth, Float fInitialDIPHeight )
{
    DebugAssert( !(IsCreated()) );

    m_hInitialDIPSize.width = fInitialDIPWidth;
    m_hInitialDIPSize.height = fInitialDIPHeight;

    _NakedCreate();
}
Void D2D1Layer::Destroy()
{
    DebugAssert( IsCreated() );

    if ( m_bTemporaryDestroyed )
        m_bTemporaryDestroyed = false;
    else
        _NakedDestroy();

    m_hInitialDIPSize.width = 0.0f;
    m_hInitialDIPSize.height = 0.0f;
}

Void D2D1Layer::OnDestroyDevice()
{
    DebugAssert( !m_bTemporaryDestroyed );

    if ( m_pLayer != NULL ) {
        _NakedDestroy();
        m_bTemporaryDestroyed = true;
    }
}
Void D2D1Layer::OnRestoreDevice()
{
    DebugAssert( m_pLayer == NULL );

    if ( m_bTemporaryDestroyed ) {
        _NakedCreate();
        m_bTemporaryDestroyed = false;
    }
}

Void D2D1Layer::GetCurrentDIPSize( Float * outWidth, Float * outHeight ) const
{
    DebugAssert( IsCreated() );

    D2D1_SIZE_F hDIPSize = m_pLayer->GetSize();
    if ( outWidth != NULL )
        *outWidth = hDIPSize.width;
    if ( outHeight != NULL )
        *outHeight = hDIPSize.height;
}
Void D2D1Layer::UpdateInitialDIPSize()
{
    DebugAssert( IsCreated() );

    *( (D2D1_SIZE_F*)&m_hInitialDIPSize ) = m_pLayer->GetSize();
}

/////////////////////////////////////////////////////////////////////////////////

Void D2D1Layer::_NakedCreate()
{
    m_pLayer = NULL;
    HRESULT hRes = m_pContext2D->m_pD2D1RenderingContext->CreateLayer( (const D2D1_SIZE_F *)&m_hInitialDIPSize, &m_pLayer );
    DebugAssert( hRes == S_OK && m_pLayer != NULL );
}
Void D2D1Layer::_NakedDestroy()
{
    m_pLayer->Release();
    m_pLayer = NULL;
}

