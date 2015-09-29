/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/Graphics/GraphicsManager.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : WorldCraft, Graphics : GraphicsManager
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
#ifndef SCARAB_ENTRYPOINT_WORLDCRAFT_GRAPHICS_GRAPHICSMANAGER_H
#define SCARAB_ENTRYPOINT_WORLDCRAFT_GRAPHICS_GRAPHICSMANAGER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Engine/Application/ApplicationManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define GraphicsFn GraphicsManager::GetInstancePtr()

// Declared effects
enum EffectID {
    EFFECTID_DEFAULT = 0, // LightMaterialTexture

    EFFECTID_COUNT
};

// Declared geometry
enum GeometryID {
    GEOMETRYID_BOX = 0, // Our dear blocks ...

    GEOMETRYID_COUNT
};

typedef struct _geometry_instance {
    GeometryID idGeometry;
    Mesh * pMesh;
    Shape * pShape;
} GeometryInstance;

/////////////////////////////////////////////////////////////////////////////////
// The GraphicsManager class
class GraphicsManager : public Manager<GraphicsManager>
{
public:
    inline static const GChar * GetName();
    inline static const GChar * GetMemoryBudget();

protected:
    friend class Manager<GraphicsManager>;
    GraphicsManager();
    virtual ~GraphicsManager();

public:
    Void Initialize();
    Void Cleanup();

    // Effect management
    inline Effect * GetEffect( EffectID idEffect ) const;

    EffectInstance * CreateEffectInstance( EffectID idEffect, Void ** arrParameters ) const;
    inline Void DestroyEffectInstance( EffectInstance * pEffectInstance ) const;

    // Materials & Lights management
    inline Material * GetMaterial( const GChar * strName ) const;
    inline Light * GetLight( const GChar * strName ) const;

    inline Material * CreateMaterial( const GChar * strName );
    inline Light * CreateLight( const GChar * strName, LightType iType );
    inline Void DestroyMaterial( const GChar * strName );
    inline Void DestroyLight( const GChar * strName );

    // Bitmap management
    inline GPU2DBitmap * GetBitmap( const GChar * strName ) const;

    GPU2DBitmap * CreateBitmap( const GChar * strName, PixelFormat iFormat, UInt iWidth, UInt iHeight );
    Void DestroyBitmap( const GChar * strName );

    // Texture management
    inline GPUTexture * GetTexture( const GChar * strName ) const;

    GPUTexture * CreateTexture( const GChar * strName, GPUResourceType iType, GPUResourceUsage iUsage, PixelFormat iFormat, UInt iMipLevelCount, UInt * arrDimensions );
    Void DestroyTexture( const GChar * strName );

    // Instanced geometry
    inline const GeometryInstance * GetGeometryInstance( const GChar * strName ) const;

    const GeometryInstance * CreateGeometryInstance( const GChar * strName, GeometryID idGeometry, Bool bCreateShape, EffectID idEffect, Void ** arrParameters = NULL );
    Void DestroyGeometryInstance( const GChar * strName );

    // RenderStates
    Bool ToggleWireframe();

private:
    // Helpers
    inline static Int _Compare_Strings( const GChar * const & rLeft, const GChar * const & rRight, Void * pUserData );

    // Effect manager
    Effect * m_arrEffects[EFFECTID_COUNT];

    // Materials & Lights manager
    typedef TreeMap<const GChar*,Material> _MaterialMap;
    typedef TreeMap<const GChar*,Light> _LightMap;
    _MaterialMap m_hMaterialMap;
    _LightMap m_hLightMap;

    // Bitmap manager
    typedef TreeMap<const GChar*,GPU2DBitmap*> _BitmapMap;
    _BitmapMap m_hBitmapMap;

    // Texture manager
    typedef TreeMap<const GChar*,GPUTexture*> _TextureMap;
    _TextureMap m_hTextureMap;

    // Instanced geometry
    typedef TreeMap<const GChar*,GeometryInstance> _GeometryInstanceMap;
    _GeometryInstanceMap m_hGeometryInstanceMap;

    // RenderStates
    Bool m_bWireframe;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GraphicsManager.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDCRAFT_GRAPHICS_GRAPHICSMANAGER_H


