/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/2D/Resources/D2D1Mesh.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : D2D1 Dev-Dep Resource : Meshes.
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
#include "D2D1Mesh.h"

#include "../D2D1RenderingContext.h"

/////////////////////////////////////////////////////////////////////////////////
// D2D1Mesh implementation
D2D1Mesh::D2D1Mesh( D2D1RenderingContext * pContext2D )
{
    m_pContext2D = pContext2D;

    m_pMesh = NULL;

    m_bOpened = false;
    m_pTessellationSink = NULL;

    m_bTemporaryDestroyed = false;
}
D2D1Mesh::~D2D1Mesh()
{
    if ( IsCreated() ) {
        if ( IsOpened() )
            Close();
        Destroy();
    }
}

Void D2D1Mesh::Create()
{
    DebugAssert( !(IsCreated()) );

    _NakedCreate();
}
Void D2D1Mesh::Destroy()
{
    DebugAssert( IsCreated() );

    if ( IsOpened() )
        Close();

    if ( m_bTemporaryDestroyed )
        m_bTemporaryDestroyed = false;
    else
        _NakedDestroy();
}

Void D2D1Mesh::OnDestroyDevice()
{
    DebugAssert( !m_bTemporaryDestroyed );

    if ( m_pMesh != NULL ) {
        _NakedDestroy();
        m_bTemporaryDestroyed = true;
    }
}
Void D2D1Mesh::OnRestoreDevice()
{
    DebugAssert( m_pMesh == NULL );

    if ( m_bTemporaryDestroyed ) {
        _NakedCreate();
        m_bTemporaryDestroyed = false;
    }
}

Void D2D1Mesh::Open()
{
    DebugAssert( IsCreated() );
    DebugAssert( !(IsOpened()) );

    _NakedOpen();

    m_bOpened = true;
}
Void D2D1Mesh::Close()
{
    DebugAssert( IsCreated() );
    DebugAssert( IsOpened() );

    _NakedClose();

    m_bOpened = false;
}

Void D2D1Mesh::AddTriangles( const D2D1Triangle * arrTriangles, UInt iTriangleCount )
{
    DebugAssert( IsCreated() );
    DebugAssert( IsOpened() );

    m_pTessellationSink->AddTriangles( (const D2D1_TRIANGLE *)arrTriangles, iTriangleCount );
}

/////////////////////////////////////////////////////////////////////////////////

Void D2D1Mesh::_NakedCreate()
{
    m_pMesh = NULL;
    HRESULT hRes = m_pContext2D->m_pD2D1RenderingContext->CreateMesh( &m_pMesh );
    DebugAssert( hRes == S_OK && m_pMesh != NULL );

    if ( m_bOpened )
        _NakedOpen();
}
Void D2D1Mesh::_NakedDestroy()
{
    if ( m_bOpened )
        _NakedClose();

    m_pMesh->Release();
    m_pMesh = NULL;
}
Void D2D1Mesh::_NakedOpen()
{
    m_pTessellationSink = NULL;
    HRESULT hRes = m_pMesh->Open( &m_pTessellationSink );
    DebugAssert( hRes == S_OK && m_pTessellationSink != NULL );
}
Void D2D1Mesh::_NakedClose()
{
    HRESULT hRes = m_pTessellationSink->Close();
    DebugAssert( hRes == S_OK );

    m_pTessellationSink->Release();
    m_pTessellationSink = NULL;
}

