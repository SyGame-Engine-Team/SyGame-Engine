/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/2D/Resources/D2D1RenderState.h
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
// Header prelude
#ifndef SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_2D_RESOURCES_D2D1RENDERSTATE_H
#define SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_2D_RESOURCES_D2D1RENDERSTATE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../D2D1Mappings.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Prototypes
class D2D1RenderingContext;
class D2D1FontFace;

class D2D1RenderState;
class D2D1TextRenderState;

/////////////////////////////////////////////////////////////////////////////////
// The D2D1RenderState class
class D2D1RenderState
{
public:
    D2D1RenderState();
    ~D2D1RenderState();

    // Deferred construction
    inline Bool IsCreated() const;

    Void Create( const D2D1RenderStateDesc * pDesc, D2D1TextRenderState * pTextRenderState = NULL );
    Void Destroy();

    // Methods
    Void GetDesc( D2D1RenderStateDesc * outDesc ) const;
    Void SetDesc( const D2D1RenderStateDesc * pDesc );

    Void GetTextRenderState( D2D1TextRenderState * outTextRenderState ) const;
    Void SetTextRenderState( D2D1TextRenderState * pTextRenderState = NULL );

private:
    friend class D2D1RenderingContext;

    ID2D1DrawingStateBlock * m_pStateBlock;
};

/////////////////////////////////////////////////////////////////////////////////
// The D2D1TextRenderState class
class D2D1TextRenderState
{
public:
    D2D1TextRenderState();
    ~D2D1TextRenderState();

    // Deferred construction
    inline Bool IsCreated() const;

    Void Create( Void * pMonitor = NULL );
    Void Create( const D2D1TextRenderStateDesc * pDesc );
    Void Destroy();

    // Methods
    Float GetGamma() const;
    Float GetEnhancedContrast() const;
    Float GetClearTypeLevel() const;
    D2D1TextPixelGeometry GetPixelGeometry() const;
    D2D1TextRenderingMode GetRenderingMode() const;

    Void GetDesc( D2D1TextRenderStateDesc * outDesc ) const;

private:
    friend class D2D1FontFace;
    friend class D2D1RenderState;
    friend class D2D1RenderingContext;

    IDWriteRenderingParams * m_pTextRenderingParams;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "D2D1RenderState.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_2D_RESOURCES_D2D1RENDERSTATE_H

