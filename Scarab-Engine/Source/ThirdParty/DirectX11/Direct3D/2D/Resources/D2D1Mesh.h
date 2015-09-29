/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/2D/Resources/D2D1Mesh.h
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
// Header prelude
#ifndef SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_2D_RESOURCES_D2D1MESH_H
#define SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_2D_RESOURCES_D2D1MESH_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../D2D1Mappings.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Prototypes
class D2D1RenderingContext;
class D2D1Geometry;

/////////////////////////////////////////////////////////////////////////////////
// The D2D1Mesh class
class D2D1Mesh
{
public:
    D2D1Mesh( D2D1RenderingContext * pContext2D );
    virtual ~D2D1Mesh();

    // Deferred construction
    inline Bool IsCreated() const;

    Void Create();
    Void Destroy();

    Void OnDestroyDevice();
    Void OnRestoreDevice();

    // Methods
    inline Bool IsOpened() const;

    Void Open();
    Void Close();

    Void AddTriangles( const D2D1Triangle * arrTriangles, UInt iTriangleCount );

protected:
    friend class D2D1Geometry;
    friend class D2D1RenderingContext;
    D2D1RenderingContext * m_pContext2D;

    ID2D1Mesh * m_pMesh;

    Bool m_bOpened;
    ID2D1TessellationSink * m_pTessellationSink;

    // Auto-Regen system
    Void _NakedCreate();
    Void _NakedDestroy();
    Void _NakedOpen();
    Void _NakedClose();

    Bool m_bTemporaryDestroyed;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "D2D1Mesh.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_2D_RESOURCES_D2D1MESH_H

