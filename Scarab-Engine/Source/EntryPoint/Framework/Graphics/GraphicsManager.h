/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/Framework/Graphics/GraphicsManager.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Framework, Graphics : GraphicsManager
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
#ifndef SCARAB_ENTRYPOINT_FRAMEWORK_GRAPHICS_GRAPHICSMANAGER_H
#define SCARAB_ENTRYPOINT_FRAMEWORK_GRAPHICS_GRAPHICSMANAGER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Engine/Application/ApplicationManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define GraphicsFn GraphicsManager::GetInstancePtr()

// Identifiers
typedef UInt LightID;
typedef UInt MaterialID;
typedef UInt TextureID;
typedef UInt EffectID;
typedef UInt GeometryID;
typedef UInt BitmapID;

// Instanced geometry content
typedef struct _geometry_instance {
    Mesh * pMesh;
    MeshGraph * pGraph;
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

    // Lights
    inline Light * GetLight( LightID iLightID ) const;

    inline Light * CreateLight( LightID iLightID, LightType iType );
    inline Void DestroyLight( LightID iLightID );

    // Materials
    inline Material * GetMaterial( MaterialID iMaterialID ) const;
    
    inline Material * CreateMaterial( MaterialID iMaterialID );
    inline Void DestroyMaterial( MaterialID iMaterialID );

    // Textures
    inline GPUTexture * GetTexture( TextureID iTextureID ) const;

    inline GPUTexture1D * CreateTexture1D( TextureID iTextureID, GPUResourceUsage iUsage, PixelFormat iFormat, UInt iWidth, UInt iMipLevelCount );
    inline GPUTexture2D * CreateTexture2D( TextureID iTextureID, GPUResourceUsage iUsage, PixelFormat iFormat, UInt iWidth, UInt iHeight, UInt iMipLevelCount );
    inline GPUTexture3D * CreateTexture3D( TextureID iTextureID, GPUResourceUsage iUsage, PixelFormat iFormat, UInt iWidth, UInt iHeight, UInt iDepth, UInt iMipLevelCount );
    inline GPUTextureCube * CreateTextureCube( TextureID iTextureID, GPUResourceUsage iUsage, PixelFormat iFormat, UInt iEdgeLength, UInt iMipLevelCount );
    inline Void DestroyTexture( TextureID iTextureID );

    // Effects
    inline Effect * GetEffect( EffectID iEffectID ) const;

    template<typename T>
    inline T * CreateEffect( EffectID iEffectID );
    inline Void DestroyEffect( EffectID iEffectID );

    // Instanced geometry
    inline const GeometryInstance * GetGeometryInstance( GeometryID iGeometryID ) const;

    const GeometryInstance * CreateGeometryInstance( GeometryID iGeometryID, Mesh * pMesh, MeshGraph * pGraph = NULL, Shape * pShape = NULL );
    Void DestroyGeometryInstance( GeometryID iGeometryID );

    // Bitmaps
    inline GPU2DBitmap * GetBitmap( BitmapID iBitmapID ) const;

    inline GPU2DBitmap * CreateBitmap( BitmapID iBitmapID, PixelFormat iFormat, UInt iWidth, UInt iHeight );
    inline Void DestroyBitmap( BitmapID iBitmapID );

private:
    // Helpers
    inline static Int _Compare_IDs( const UInt & rLeft, const UInt & rRight, Void * pUserData );

    // Lights
    typedef TreeMap<LightID,Light> _LightMap;
    _LightMap m_mapLights;

    // Materials
    typedef TreeMap<MaterialID,Material> _MaterialMap;
    _MaterialMap m_mapMaterials;

    // Textures
    typedef TreeMap<TextureID,GPUTexture*> _TextureMap;
    _TextureMap m_mapTextures;

    // Effects
    typedef TreeMap<EffectID,Effect*> _EffectMap;
    _EffectMap m_mapEffects;

    // Instanced geometry
    typedef TreeMap<GeometryID,GeometryInstance> _GeometryMap;
    _GeometryMap m_mapGeometryInstances;

    // Bitmaps
    typedef TreeMap<BitmapID,GPU2DBitmap*> _BitmapMap;
    _BitmapMap m_mapBitmaps;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GraphicsManager.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_FRAMEWORK_GRAPHICS_GRAPHICSMANAGER_H


