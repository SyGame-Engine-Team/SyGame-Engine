/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Graphics/GraphicsManager.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Graphics : GraphicsManager
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

inline Effect * GraphicsManager::GetEffect( EffectID idEffect ) const {
    Assert( idEffect < EFFECTID_COUNT );
    return m_arrEffects[idEffect];
}

inline Void GraphicsManager::DestroyEffectInstance( EffectInstance * pEffectInstance ) const {
    Effect::DestroyInstance( pEffectInstance );
}

inline Material * GraphicsManager::GetMaterial( const GChar * strName ) const {
    _MaterialMap::Iterator itMaterial = m_hMaterialMap.Get( strName );
    if ( itMaterial.IsNull() )
        return NULL;
    return itMaterial.GetItemPtr();
}
inline Light * GraphicsManager::GetLight( const GChar * strName ) const {
    _LightMap::Iterator itLight = m_hLightMap.Get( strName );
    if ( itLight.IsNull() )
        return NULL;
    return itLight.GetItemPtr();
}

inline Material * GraphicsManager::CreateMaterial( const GChar * strName ) {
    _MaterialMap::Iterator itMaterial;
    Bool bInserted = m_hMaterialMap.Insert( strName, Material(), &itMaterial );
    Assert( bInserted && !(itMaterial.IsNull()) );
    return itMaterial.GetItemPtr();
}
inline Light * GraphicsManager::CreateLight( const GChar * strName, LightType iType ) {
    _LightMap::Iterator itLight;
    Bool bInserted = m_hLightMap.Insert( strName, Light(iType), &itLight );
    Assert( bInserted && !(itLight.IsNull()) );
    return itLight.GetItemPtr();
}
inline Void GraphicsManager::DestroyMaterial( const GChar * strName ) {
    Bool bRemoved = m_hMaterialMap.Remove( strName );
    Assert( bRemoved );
}
inline Void GraphicsManager::DestroyLight( const GChar * strName ) {
    Bool bRemoved = m_hLightMap.Remove( strName );
    Assert( bRemoved );
}

inline GPU2DBitmap * GraphicsManager::GetBitmap( const GChar * strName ) const {
    _BitmapMap::Iterator itBitmap = m_hBitmapMap.Get( strName );
    if ( itBitmap.IsNull() )
        return NULL;
    return itBitmap.GetItem();
}

inline GPUTexture * GraphicsManager::GetTexture( const GChar * strName ) const {
    _TextureMap::Iterator itTexture = m_hTextureMap.Get( strName );
    if ( itTexture.IsNull() )
        return NULL;
    return itTexture.GetItem();
}

inline const GeometryInstance * GraphicsManager::GetGeometryInstance( const GChar * strName ) const {
    _GeometryInstanceMap::Iterator itGeometryInstance = m_hGeometryInstanceMap.Get( strName );
    if ( itGeometryInstance.IsNull() )
        return NULL;
    return itGeometryInstance.GetItemPtr();
}

/////////////////////////////////////////////////////////////////////////////////

inline Int GraphicsManager::_Compare_Strings( const GChar * const & rLeft, const GChar * const & rRight, Void * /*pUserData*/ ) {
    return StringFn->Cmp( rLeft, rRight );
}
