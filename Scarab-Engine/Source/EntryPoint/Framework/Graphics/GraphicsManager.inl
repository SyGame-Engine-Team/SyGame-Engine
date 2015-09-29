/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/Framework/Graphics/GraphicsManager.inl
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
// GraphicsManager implementation
inline const GChar * GraphicsManager::GetName() {
    return TEXT("GraphicsManager");
}
inline const GChar * GraphicsManager::GetMemoryBudget() {
    return TEXT (
"Heap:Scratch(4194304);"
    );
}

inline Light * GraphicsManager::GetLight( LightID iLightID ) const {
    _LightMap::Iterator itLight = m_mapLights.Get( iLightID );
    if ( itLight.IsNull() )
        return NULL;
    return itLight.GetItemPtr();
}

inline Light * GraphicsManager::CreateLight( LightID iLightID, LightType iType ) {
    _LightMap::Iterator itLight;
    Bool bInserted = m_mapLights.Insert( iLightID, Light(iType), &itLight );
    Assert( bInserted && !(itLight.IsNull()) );
    return itLight.GetItemPtr();
}
inline Void GraphicsManager::DestroyLight( LightID iLightID ) {
    Bool bRemoved = m_mapLights.Remove( iLightID );
    Assert( bRemoved );
}

inline Material * GraphicsManager::GetMaterial( MaterialID iMaterialID ) const {
    _MaterialMap::Iterator itMaterial = m_mapMaterials.Get( iMaterialID );
    if ( itMaterial.IsNull() )
        return NULL;
    return itMaterial.GetItemPtr();
}

inline Material * GraphicsManager::CreateMaterial( MaterialID iMaterialID ) {
    _MaterialMap::Iterator itMaterial;
    Bool bInserted = m_mapMaterials.Insert( iMaterialID, Material(), &itMaterial );
    Assert( bInserted && !(itMaterial.IsNull()) );
    return itMaterial.GetItemPtr();
}
inline Void GraphicsManager::DestroyMaterial( MaterialID iMaterialID ) {
    Bool bRemoved = m_mapMaterials.Remove( iMaterialID );
    Assert( bRemoved );
}

inline GPUTexture * GraphicsManager::GetTexture( TextureID iTextureID ) const {
    _TextureMap::Iterator itTexture = m_mapTextures.Get( iTextureID );
    if ( itTexture.IsNull() )
        return NULL;
    return itTexture.GetItem();
}

inline GPUTexture1D * GraphicsManager::CreateTexture1D( TextureID iTextureID, GPUResourceUsage iUsage, PixelFormat iFormat, UInt iWidth, UInt iMipLevelCount ) {
    GPUTexture1D * pTexture = RenderingFn->CreateTexture1D( iFormat, iWidth, iMipLevelCount, 1, NULL, true );
    pTexture->SetUsage( iUsage );
    Bool bInserted = m_mapTextures.Insert( iTextureID, pTexture );
    Assert( bInserted );
    return pTexture;
}
inline GPUTexture2D * GraphicsManager::CreateTexture2D( TextureID iTextureID, GPUResourceUsage iUsage, PixelFormat iFormat, UInt iWidth, UInt iHeight, UInt iMipLevelCount ) {
    GPUTexture2D * pTexture = RenderingFn->CreateTexture2D( iFormat, iWidth, iHeight, iMipLevelCount, 1, 1, NULL, true );
    pTexture->SetUsage( iUsage );
    Bool bInserted = m_mapTextures.Insert( iTextureID, pTexture );
    Assert( bInserted );
    return pTexture;
}
inline GPUTexture3D * GraphicsManager::CreateTexture3D( TextureID iTextureID, GPUResourceUsage iUsage, PixelFormat iFormat, UInt iWidth, UInt iHeight, UInt iDepth, UInt iMipLevelCount ) {
    GPUTexture3D * pTexture = RenderingFn->CreateTexture3D( iFormat, iWidth, iHeight, iDepth, iMipLevelCount, NULL, true );
    pTexture->SetUsage( iUsage );
    Bool bInserted = m_mapTextures.Insert( iTextureID, pTexture );
    Assert( bInserted );
    return pTexture;
}
inline GPUTextureCube * GraphicsManager::CreateTextureCube( TextureID iTextureID, GPUResourceUsage iUsage, PixelFormat iFormat, UInt iEdgeLength, UInt iMipLevelCount ) {
    GPUTextureCube * pTexture = RenderingFn->CreateTextureCube( iFormat, iEdgeLength, iMipLevelCount, 1, NULL, true );
    pTexture->SetUsage( iUsage );
    Bool bInserted = m_mapTextures.Insert( iTextureID, pTexture );
    Assert( bInserted );
    return pTexture;
}
inline Void GraphicsManager::DestroyTexture( TextureID iTextureID ) {
    GPUTexture * pTexture = NULL;
    Bool bRemoved = m_mapTextures.Remove( iTextureID, &pTexture );
    Assert( bRemoved && pTexture != NULL );

    RenderingFn->DestroyResource( pTexture );
}

inline Effect * GraphicsManager::GetEffect( EffectID iEffectID ) const {
    _EffectMap::Iterator itEffect = m_mapEffects.Get( iEffectID );
    if ( itEffect.IsNull() )
        return NULL;
    return itEffect.GetItem();
}

template<typename T>
inline T * GraphicsManager::CreateEffect( EffectID iEffectID ) {
    SelectMemory( TEXT("Scratch") );
    T * pEffect = New T();
    UnSelectMemory();

    Bool bInserted = m_mapEffects.Insert( iEffectID, pEffect );
    Assert( bInserted );
    return pEffect;
}
inline Void GraphicsManager::DestroyEffect( EffectID iEffectID ) {
    Effect * pEffect = NULL;
    Bool bRemoved = m_mapEffects.Remove( iEffectID, &pEffect );
    Assert( bRemoved && pEffect != NULL );

    SelectMemory( TEXT("Scratch") );
    Delete( pEffect );
    UnSelectMemory();
}

inline const GeometryInstance * GraphicsManager::GetGeometryInstance( GeometryID iGeometryID ) const {
    _GeometryMap::Iterator itGeometryInstance = m_mapGeometryInstances.Get( iGeometryID );
    if ( itGeometryInstance.IsNull() )
        return NULL;
    return itGeometryInstance.GetItemPtr();
}

inline GPU2DBitmap * GraphicsManager::GetBitmap( BitmapID iBitmapID ) const {
    _BitmapMap::Iterator itBitmap = m_mapBitmaps.Get( iBitmapID );
    if ( itBitmap.IsNull() )
        return NULL;
    return itBitmap.GetItem();
}

inline GPU2DBitmap * GraphicsManager::CreateBitmap( BitmapID iBitmapID, PixelFormat iFormat, UInt iWidth, UInt iHeight ) {
    GPU2DBitmap * pBitmap = RenderingFn->GetScreen2DContext()->CreateBitmap( iFormat, iWidth, iHeight, NULL, true );

    Bool bInserted = m_mapBitmaps.Insert( iBitmapID, pBitmap );
    Assert( bInserted );
    return pBitmap;
}
inline Void GraphicsManager::DestroyBitmap( BitmapID iBitmapID ) {
    GPU2DBitmap * pBitmap = NULL;
    Bool bRemoved = m_mapBitmaps.Remove( iBitmapID, &pBitmap );
    Assert( bRemoved && pBitmap != NULL );

    RenderingFn->GetScreen2DContext()->DestroyBitmap( pBitmap );
}

/////////////////////////////////////////////////////////////////////////////////

inline Int GraphicsManager::_Compare_IDs( const UInt & rLeft, const UInt & rRight, Void * /*pUserData*/ ) {
    if ( rLeft < rRight )
        return +1;
    if ( rLeft > rRight )
        return -1;
    return 0;
}
