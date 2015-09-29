/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/Framework/Graphics/GraphicsManager.cpp
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
// Includes
#include "GraphicsManager.h"

/////////////////////////////////////////////////////////////////////////////////
// GraphicsManager implementation
GraphicsManager::GraphicsManager():
    Manager<GraphicsManager>(),
    m_mapLights(), m_mapMaterials(), m_mapTextures(), m_mapEffects(), m_mapGeometryInstances(), m_mapBitmaps()
{
    // nothing to do
}
GraphicsManager::~GraphicsManager()
{
    // nothing to do
}

Void GraphicsManager::Initialize()
{
    m_mapLights.UseMemoryContext( GetMemoryContext(), TEXT("Scratch") );
    m_mapLights.SetComparator( _Compare_IDs, NULL );
    m_mapLights.Create();

    m_mapMaterials.UseMemoryContext( GetMemoryContext(), TEXT("Scratch") );
    m_mapMaterials.SetComparator( _Compare_IDs, NULL );
    m_mapMaterials.Create();

    m_mapTextures.UseMemoryContext( GetMemoryContext(), TEXT("Scratch") );
    m_mapTextures.SetComparator( _Compare_IDs, NULL );
    m_mapTextures.Create();

    m_mapEffects.UseMemoryContext( GetMemoryContext(), TEXT("Scratch") );
    m_mapEffects.SetComparator( _Compare_IDs, NULL );
    m_mapEffects.Create();

    m_mapGeometryInstances.UseMemoryContext( GetMemoryContext(), TEXT("Scratch") );
    m_mapGeometryInstances.SetComparator( _Compare_IDs, NULL );
    m_mapGeometryInstances.Create();

    m_mapBitmaps.UseMemoryContext( GetMemoryContext(), TEXT("Scratch") );
    m_mapBitmaps.SetComparator( _Compare_IDs, NULL );
    m_mapBitmaps.Create();
}
Void GraphicsManager::Cleanup()
{
    SelectMemory( TEXT("Scratch") );

    for( _BitmapMap::Iterator itBitmap = m_mapBitmaps.Begin(); !itBitmap.IsNull(); ++itBitmap ) {
        GPU2DBitmap * pBitmap = itBitmap.GetItem();
        pBitmap->UnBind();
        RenderingFn->GetScreen2DContext()->DestroyBitmap( pBitmap );
    }
    m_mapBitmaps.Destroy();

    for( _GeometryMap::Iterator itGeometryInstance = m_mapGeometryInstances.Begin(); !(itGeometryInstance.IsNull()); ++itGeometryInstance ) {
        GeometryInstance * pGeometryInstance = itGeometryInstance.GetItemPtr();
        if ( pGeometryInstance->pShape != NULL )
            PhysicsFn->DestroyShape( pGeometryInstance->pShape );
        if ( pGeometryInstance->pGraph != NULL ) {
            pGeometryInstance->pGraph->Destroy();
            Delete( pGeometryInstance->pGraph );
        }
        if ( pGeometryInstance->pMesh != NULL )
            Delete( pGeometryInstance->pMesh );
    }
    m_mapGeometryInstances.Destroy();

    for( _EffectMap::Iterator itEffect = m_mapEffects.Begin(); !itEffect.IsNull(); ++itEffect ) {
        Effect * pEffect = itEffect.GetItem();
        Delete( pEffect );
    }
    m_mapEffects.Destroy();

    for( _TextureMap::Iterator itTexture = m_mapTextures.Begin(); !itTexture.IsNull(); ++itTexture ) {
        GPUTexture * pTexture = itTexture.GetItem();
        pTexture->UnBind();
        RenderingFn->DestroyResource( pTexture );
    }
    m_mapTextures.Destroy();

    m_mapMaterials.Destroy();

    m_mapLights.Destroy();

    UnSelectMemory();
}

const GeometryInstance * GraphicsManager::CreateGeometryInstance( GeometryID iGeometryID, Mesh * pMesh, MeshGraph * pGraph, Shape * pShape )
{
    Assert( pMesh != NULL );

    _GeometryMap::Iterator itGeometryInstance;
    Bool bInserted = m_mapGeometryInstances.Insert( iGeometryID, GeometryInstance(), &itGeometryInstance );
    Assert( bInserted && !(itGeometryInstance.IsNull()) );

    GeometryInstance * pGeometryInstance = itGeometryInstance.GetItemPtr();
    pGeometryInstance->pMesh = pMesh;
    pGeometryInstance->pGraph = pGraph;
    pGeometryInstance->pShape = pShape;

    return pGeometryInstance;
}
Void GraphicsManager::DestroyGeometryInstance( GeometryID iGeometryID )
{
    GeometryInstance hGeometryInstance;
    hGeometryInstance.pMesh = NULL;
    Bool bRemoved = m_mapGeometryInstances.Remove( iGeometryID, &hGeometryInstance );
    Assert( bRemoved && hGeometryInstance.pMesh != NULL );

    SelectMemory( TEXT("Scratch") );

    if ( hGeometryInstance.pShape != NULL )
        PhysicsFn->DestroyShape( hGeometryInstance.pShape );
    if ( hGeometryInstance.pGraph != NULL ) {
        hGeometryInstance.pGraph->Destroy();
        Delete( hGeometryInstance.pGraph );
    }
    if ( hGeometryInstance.pMesh != NULL )
        Delete( hGeometryInstance.pMesh );
    
    UnSelectMemory();
}
