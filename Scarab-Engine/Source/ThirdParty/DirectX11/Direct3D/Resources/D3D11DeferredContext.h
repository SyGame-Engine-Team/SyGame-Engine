/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/Resources/D3D11DeferredContext.h
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
// Header prelude
#ifndef SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_RESOURCES_D3D11DEFERREDCONTEXT_H
#define SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_RESOURCES_D3D11DEFERREDCONTEXT_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../D3D11Mappings.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Prototypes
class D3D11Renderer;

class D3D11Buffer;
class D3D11Texture;
class D3D11Texture1D;
class D3D11Texture2D;
class D3D11Texture3D;
class D3D11TextureCube;

class D3D11QueryCommandProcessing;
class D3D11QueryOcclusion;
class D3D11QueryTimeStampFrequency;
class D3D11QueryTimeStamp;
class D3D11QueryStatsPipeline;
class D3D11QueryStatsStreamOutput;
class D3D11PredicateOcclusion;
class D3D11PredicateStreamOutputOverflow;
class D3D11Counter;

class D3D11CommandList;
class D3D11DeferredContext;

/////////////////////////////////////////////////////////////////////////////////
// The D3D11CommandList class
class D3D11CommandList
{
public:
    D3D11CommandList();
    ~D3D11CommandList();

    Void Clear();

private:
    friend class D3D11Renderer;
    friend class D3D11DeferredContext;

    ID3D11CommandList * m_pCommandList;
};

/////////////////////////////////////////////////////////////////////////////////
// The D3D11DeferredContext class
class D3D11DeferredContext
{
public:
    D3D11DeferredContext( D3D11Renderer * pRenderer );
    ~D3D11DeferredContext();

    // Deferred construction
    inline Bool IsCreated() const;

    Void Create();
    Void Destroy();

    Void OnDestroyDevice();
    Void OnRestoreDevice();

    // Methods
    Void MakeCommandList( D3D11CommandList * outCommandList, Bool bRestoreDeferredContextState );

    Void Clear(); // Disables everything in this context
    Void Reset(); // Reset command list then Clear

private:
    friend class D3D11Renderer;
    D3D11Renderer * m_pRenderer;

    // Access to resources
    friend class D3D11Buffer;
    friend class D3D11Texture;
    friend class D3D11Texture1D;
    friend class D3D11Texture2D;
    friend class D3D11Texture3D;
    friend class D3D11TextureCube;

    // Access to asynchronous queries
    friend class D3D11QueryCommandProcessing;
    friend class D3D11QueryOcclusion;
    friend class D3D11QueryTimeStampFrequency;
    friend class D3D11QueryTimeStamp;
    friend class D3D11QueryStatsPipeline;
    friend class D3D11QueryStatsStreamOutput;
    friend class D3D11PredicateOcclusion;
    friend class D3D11PredicateStreamOutputOverflow;
    friend class D3D11Counter;

    ID3D11DeviceContext * m_pDeferredContext;

    // Auto-Regen system
    Void _NakedCreate();
    Void _NakedDestroy();

    Bool m_bTemporaryDestroyed;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "D3D11DeferredContext.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_RESOURCES_D3D11DEFERREDCONTEXT_H

