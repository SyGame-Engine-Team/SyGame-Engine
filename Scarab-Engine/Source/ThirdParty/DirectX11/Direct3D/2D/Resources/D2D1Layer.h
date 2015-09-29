/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/2D/Resources/D2D1Layer.h
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
// Header prelude
#ifndef SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_2D_RESOURCES_D2D1LAYER_H
#define SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_2D_RESOURCES_D2D1LAYER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "D2D1Brush.h"
#include "D2D1Geometry.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// D2D1Layer definitions
typedef struct _d2d1_layer_parameters {    
    D2D1Rectangle rectContentBounds;            // Rectangular clip applied to layer (affected by world transform)

    D2D1Geometry * pMaskGeometry;               // Geometry of the layer's mask (optional)
    D2D1AntialiasingMode iMaskAntialiasingMode; // Antialiasing mode of the mask (optional)
    D2D1Matrix32 matMaskTransform;              // Additional transform applied to mask (optional)

    D2D1Brush * pOpacityBrush;                  // Additional blush to apply for alpha channel only (optional)

    Float fOpacity;                             // Opacity of the layer when blending back to render target

    D2D1LayerOptions iLayerOptions;             // Hint for layers using cleartype content, ...
} D2D1LayerParameters;

// Prototypes
class D2D1RenderingContext;

/////////////////////////////////////////////////////////////////////////////////
// The D2D1Layer class
class D2D1Layer
{
public:
    D2D1Layer( D2D1RenderingContext * pContext2D );
    ~D2D1Layer();

    // Deferred construction
    inline Bool IsCreated() const;

    Void Create( Float fInitialDIPWidth = 0.0f, Float fInitialDIPHeight = 0.0f );
    Void Destroy();

    Void OnDestroyDevice();
    Void OnRestoreDevice();

    // Methods
    inline Float GetInitialDIPWidth() const;
    inline Float GetInitialDIPHeight() const;

    Void GetCurrentDIPSize( Float * outWidth, Float * outHeight ) const;
    Void UpdateInitialDIPSize();

private:
    friend class D2D1RenderingContext;
    D2D1RenderingContext * m_pContext2D;

    __D2D1_SIZE_F m_hInitialDIPSize;
    ID2D1Layer * m_pLayer;

    // Auto-Regen system
    Void _NakedCreate();
    Void _NakedDestroy();

    Bool m_bTemporaryDestroyed;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "D2D1Layer.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_2D_RESOURCES_D2D1LAYER_H

