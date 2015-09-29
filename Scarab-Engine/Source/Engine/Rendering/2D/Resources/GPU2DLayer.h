/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/2D/Resources/GPU2DLayer.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU 2D-Resource : Layers
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
#ifndef SCARAB_ENGINE_RENDERING_2D_RESOURCES_GPU2DLAYER_H
#define SCARAB_ENGINE_RENDERING_2D_RESOURCES_GPU2DLAYER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "GPU2DBrush.h"
#include "GPU2DGeometry.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
enum GPU2DLayerOptions {
    GPU2DLAYER_OPTION_NONE           = DEVICE2DLAYER_OPTION_NONE,
    GPU2DLAYER_OPTION_CLEARTYPE_INIT = DEVICE2DLAYER_OPTION_CLEARTYPE_INIT
};

typedef struct _gpu2d_layer_parameters {    
    GPU2DRectangle rectContentBounds;            // Rectangular clip applied to layer (affected by world transform)

    GPU2DGeometry * pMaskGeometry;               // Geometry of the layer's mask (optional)
    GPU2DAntialiasingMode iMaskAntialiasingMode; // Antialiasing mode of the mask (optional)
    GPU2DMatrix32 matMaskTransform;              // Additional transform applied to mask (optional)

    GPU2DBrush * pOpacityBrush;                  // Additional brush to apply for alpha channel only (optional)

    Float fOpacity;                              // Opacity of the layer when blending back to render target

    GPU2DLayerOptions iLayerOptions;             // Options for layers ...
} GPU2DLayerParameters;

// Prototypes
class GPU2DContext;

/////////////////////////////////////////////////////////////////////////////////
// The GPU2DLayer class
class GPU2DLayer
{
public:
    GPU2DLayer( GPU2DContext * pContext2D );
    ~GPU2DLayer();

    // Binding
    inline Bool IsBound() const;

    Void Bind( Float fInitialDIPWidth = 0.0f, Float fInitialDIPHeight = 0.0f );
    Void UnBind();

    // Methods
    inline Float GetInitialDIPWidth() const;
    inline Float GetInitialDIPHeight() const;

    inline Void GetCurrentDIPSize( Float * outWidth, Float * outHeight ) const;
    inline Void UpdateInitialDIPSize();

private:
    friend class GPU2DContext;
    GPU2DContext * m_pContext2D;

    // Device data
    Device2DLayer m_hDevice2DLayer;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GPU2DLayer.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RENDERING_2D_RESOURCES_GPU2DLAYER_H
