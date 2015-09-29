/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Graphics/GraphicsManager.cpp
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
// Includes
#include "GraphicsManager.h"

/////////////////////////////////////////////////////////////////////////////////
// GraphicsManager implementation
GraphicsManager::GraphicsManager():
    Manager<GraphicsManager>(),
    m_hBitmapMap(), m_hTextureMap(), m_hGeometryInstanceMap()
{
    m_bWireframe = false;
}
GraphicsManager::~GraphicsManager()
{
    // nothing to do
}

Void GraphicsManager::Initialize()
{
    SelectMemory( TEXT("Scratch") );

    // Effect manager
    m_arrEffects[EFFECTID_UNDEFINED] = New EffectDefault();

    m_arrEffects[EFFECTID_COLOR]   = New EffectColor();
    m_arrEffects[EFFECTID_TEXTURE] = New EffectTexture();

    m_arrEffects[EFFECTID_MATERIAL]               = New EffectMaterial();
    m_arrEffects[EFFECTID_MATERIAL_TEXTURE]       = New EffectMaterialTexture();
    m_arrEffects[EFFECTID_LIGHT_MATERIAL]         = New EffectLightMaterial();
    m_arrEffects[EFFECTID_LIGHT_MATERIAL_TEXTURE] = New EffectLightMaterialTexture();

    // Materials & Lights manager
    m_hMaterialMap.SetComparator( _Compare_Strings, NULL );
    m_hMaterialMap.UseMemoryContext( GetMemoryContext(), TEXT("Scratch") );
    m_hMaterialMap.Create();

    m_hLightMap.SetComparator( _Compare_Strings, NULL );
    m_hLightMap.UseMemoryContext( GetMemoryContext(), TEXT("Scratch") );
    m_hLightMap.Create();

    // Bitmap manager
    m_hBitmapMap.SetComparator( _Compare_Strings, NULL );
    m_hBitmapMap.UseMemoryContext( GetMemoryContext(), TEXT("Scratch") );
    m_hBitmapMap.Create();

    // Texture manager
    m_hTextureMap.SetComparator( _Compare_Strings, NULL );
    m_hTextureMap.UseMemoryContext( GetMemoryContext(), TEXT("Scratch") );
    m_hTextureMap.Create();

    // Instanced geometry
    m_hGeometryInstanceMap.SetComparator( _Compare_Strings, NULL );
    m_hGeometryInstanceMap.UseMemoryContext( GetMemoryContext(), TEXT("Scratch") );
    m_hGeometryInstanceMap.Create();

    UnSelectMemory();
}
Void GraphicsManager::Cleanup()
{
    SelectMemory( TEXT("Scratch") );

    // Instanced geometry
    for( _GeometryInstanceMap::Iterator itGeometryInstance = m_hGeometryInstanceMap.Begin(); !(itGeometryInstance.IsNull()); ++itGeometryInstance ) {
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
    m_hGeometryInstanceMap.Destroy();

    // Texture manager
    for( _TextureMap::Iterator itTexture = m_hTextureMap.Begin(); !itTexture.IsNull(); ++itTexture ) {
        itTexture.GetItem()->UnBind();
        RenderingFn->DestroyResource( itTexture.GetItem() );
    }
    m_hTextureMap.Destroy();

    // Bitmap manager
    for( _BitmapMap::Iterator itBitmap = m_hBitmapMap.Begin(); !itBitmap.IsNull(); ++itBitmap ) {
        itBitmap.GetItem()->UnBind();
        RenderingFn->GetScreen2DContext()->DestroyBitmap( itBitmap.GetItem() );
    }
    m_hBitmapMap.Destroy();

    // Materials & Lights manager
    m_hLightMap.Destroy();
    m_hMaterialMap.Destroy();

    // Effect manager
    for( UInt i = 0; i < EFFECTID_COUNT; ++i ) {
        Delete( m_arrEffects[i] );
        m_arrEffects[i] = NULL;
    }

    UnSelectMemory();
}

EffectInstance * GraphicsManager::CreateEffectInstance( EffectID idEffect, Void ** arrParameters ) const
{
    EffectInstance * pEffectInstance = NULL;

    switch( idEffect ) {
        case EFFECTID_UNDEFINED:
            pEffectInstance = ( (EffectDefault*)(m_arrEffects[idEffect]) )->CreateInstance();
            break;
        case EFFECTID_COLOR:
            pEffectInstance = ( (EffectColor*)(m_arrEffects[idEffect]) )->CreateInstance();
            break;
        case EFFECTID_TEXTURE:
            pEffectInstance = ( (EffectTexture*)(m_arrEffects[idEffect]) )->CreateInstance( (GPUTexture2D*)(arrParameters[0]) );
            break;
        case EFFECTID_MATERIAL:
            pEffectInstance = ( (EffectMaterial*)(m_arrEffects[idEffect]) )->CreateInstance( (Material*)(arrParameters[0]) );
            break;
        case EFFECTID_MATERIAL_TEXTURE:
            pEffectInstance = ( (EffectMaterialTexture*)(m_arrEffects[idEffect]) )->CreateInstance( (Material*)(arrParameters[0]), (GPUTexture2D*)(arrParameters[1]) );
            break;
        case EFFECTID_LIGHT_MATERIAL:
            pEffectInstance = ( (EffectLightMaterial*)(m_arrEffects[idEffect]) )->CreateInstance( EFFECTLIGHT_TECHNIQUE_POINT_PHONG, (Light*)(arrParameters[0]), (Material*)(arrParameters[1]) );
            break;
        case EFFECTID_LIGHT_MATERIAL_TEXTURE:
            pEffectInstance = ( (EffectLightMaterialTexture*)(m_arrEffects[idEffect]) )->CreateInstance( EFFECTLIGHT_TECHNIQUE_POINT_PHONG, (Light*)(arrParameters[0]), (Material*)(arrParameters[1]), (GPUTexture2D*)(arrParameters[2]) );
            break;
        default: Assert(false); break;
    }

    return pEffectInstance;
}

GPU2DBitmap * GraphicsManager::CreateBitmap( const GChar * strName, PixelFormat iFormat, UInt iWidth, UInt iHeight )
{
    GPU2DBitmap * pBitmap = RenderingFn->GetScreen2DContext()->CreateBitmap( iFormat, iWidth, iHeight, NULL, true );

    _BitmapMap::Iterator itBitmap;
    Bool bInserted = m_hBitmapMap.Insert( strName, pBitmap, &itBitmap );
    Assert( bInserted && !(itBitmap.IsNull()) );

    return itBitmap.GetItem();
}
Void GraphicsManager::DestroyBitmap( const GChar * strName )
{
    GPU2DBitmap * pBitmap = NULL;
    Bool bRemoved = m_hBitmapMap.Remove( strName, &pBitmap );
    Assert( bRemoved && pBitmap != NULL );

    RenderingFn->GetScreen2DContext()->DestroyBitmap( pBitmap );
}

GPUTexture * GraphicsManager::CreateTexture( const GChar * strName, GPUResourceType iType, GPUResourceUsage iUsage, PixelFormat iFormat, UInt iMipLevelCount, UInt * arrDimensions )
{
    GPUTexture * pTexture = NULL;

    switch( iType ) {
        case GPURESOURCE_TEXTURE_1D:
            pTexture = RenderingFn->CreateTexture1D( iFormat, arrDimensions[0], iMipLevelCount, 1, NULL, true );
            pTexture->SetUsage( iUsage );
            break;
        case GPURESOURCE_TEXTURE_2D:
            pTexture = RenderingFn->CreateTexture2D( iFormat, arrDimensions[0], arrDimensions[1], iMipLevelCount, 1, 1, NULL, true );
            pTexture->SetUsage( iUsage );
            break;
        case GPURESOURCE_TEXTURE_3D:
            pTexture = RenderingFn->CreateTexture3D( iFormat, arrDimensions[0], arrDimensions[1], arrDimensions[2], iMipLevelCount, NULL, true );
            pTexture->SetUsage( iUsage );
            break;
        case GPURESOURCE_TEXTURE_CUBE:
            pTexture = RenderingFn->CreateTextureCube( iFormat, arrDimensions[0], iMipLevelCount, 1, NULL, true );
            pTexture->SetUsage( iUsage );
            break;
        default: Assert(false); break;
    }

    _TextureMap::Iterator itTexture;
    Bool bInserted = m_hTextureMap.Insert( strName, pTexture, &itTexture );
    Assert( bInserted && !(itTexture.IsNull()) );

    return itTexture.GetItem();
}
Void GraphicsManager::DestroyTexture( const GChar * strName )
{
    GPUTexture * pTexture = NULL;
    Bool bRemoved = m_hTextureMap.Remove( strName, &pTexture );
    Assert( bRemoved && pTexture != NULL );

    RenderingFn->DestroyResource( pTexture );
}

const GeometryInstance * GraphicsManager::CreateGeometryInstance( const GChar * strName, GeometryID idGeometry, Bool bCreateShape, EffectID idEffect, Void ** arrParameters )
{
    GPUInputLayout * pIL = GetEffect( idEffect )->GetInputLayout( EFFECTLIGHT_TECHNIQUE_POINT_PHONG, 0 );

    SelectMemory( TEXT("Scratch") );

    GeometryInstance hGeometryInstance;
    hGeometryInstance.idGeometry = idGeometry;
    hGeometryInstance.pMesh = NULL;
    hGeometryInstance.pGraph = NULL;
    hGeometryInstance.pShape = NULL;

    switch( idGeometry ) {
        case GEOMETRYID_SEGMENT:
            hGeometryInstance.pMesh = New MeshSegment( *(Segment3*)(arrParameters[0]), pIL );
            //if ( bCreateShape )
            //    hGeometryInstance.pShape = PhysicsFn->CreateShapeSegment( (MeshSegment*)(hGeometryInstance.pMesh) );
            break;
        case GEOMETRYID_TRIANGLE:
            hGeometryInstance.pMesh = New MeshTriangle( *(Triangle3*)(arrParameters[0]), pIL );
            //if ( bCreateShape )
            //    hGeometryInstance.pShape = PhysicsFn->CreateShapeTriangle( (MeshTriangle*)(hGeometryInstance.pMesh) );
            break;
        case GEOMETRYID_QUAD:
            hGeometryInstance.pMesh = New MeshQuad( *(Quad3*)(arrParameters[0]), pIL );
            //if ( bCreateShape )
            //    hGeometryInstance.pShape = PhysicsFn->CreateShapeQuad( (MeshQuad*)(hGeometryInstance.pMesh) );
            break;
        case GEOMETRYID_PLANE:
            hGeometryInstance.pMesh = New MeshGridPlane( *(Plane*)(arrParameters[0]), *(Scalar*)(arrParameters[1]), *(Scalar*)(arrParameters[2]),
                                                         *(UInt*)(arrParameters[3]), *(UInt*)(arrParameters[4]), pIL );
            //if ( bCreateShape )
            //    hGeometryInstance.pShape = PhysicsFn->CreateShapePlane( (MeshGridPlane*)(hGeometryInstance.pMesh) );
            break;
        case GEOMETRYID_DISK:
            hGeometryInstance.pMesh = New MeshDisk( *(Circle3*)(arrParameters[0]), *(UInt*)(arrParameters[1]), *(UInt*)(arrParameters[2]), pIL );
            //if ( bCreateShape )
            //    hGeometryInstance.pShape = PhysicsFn->CreateShapeDisk( (MeshDisk*)(hGeometryInstance.pMesh) );
            break;
        case GEOMETRYID_BOX:
            hGeometryInstance.pMesh = New MeshBox( *(Box*)(arrParameters[0]), pIL, *(Bool*)(arrParameters[1]) );
            if ( bCreateShape )
                hGeometryInstance.pShape = PhysicsFn->CreateShapeBox( (MeshBox*)(hGeometryInstance.pMesh) );
            break;
        case GEOMETRYID_SPHERE:
            hGeometryInstance.pMesh = New MeshSphere( *(Sphere*)(arrParameters[0]), *(UInt*)(arrParameters[1]), *(UInt*)(arrParameters[2]), pIL, *(Bool*)(arrParameters[3]) );
            if ( bCreateShape )
                hGeometryInstance.pShape = PhysicsFn->CreateShapeSphere( (MeshSphere*)(hGeometryInstance.pMesh) );
            break;
        case GEOMETRYID_TORUS:
            hGeometryInstance.pMesh = New MeshTorus( *(Torus*)(arrParameters[0]), *(UInt*)(arrParameters[1]), *(UInt*)(arrParameters[2]), pIL, *(Bool*)(arrParameters[3]) );
            //if ( bCreateShape )
            //    hGeometryInstance.pShape = PhysicsFn->CreateShapeTorus( (MeshTorus*)(hGeometryInstance.pMesh) );
            break;
        case GEOMETRYID_CYLINDER:
            hGeometryInstance.pMesh = New MeshCylinder( *(Cylinder*)(arrParameters[0]), *(UInt*)(arrParameters[1]), *(UInt*)(arrParameters[2]), pIL, *(Bool*)(arrParameters[3]) );
            if ( bCreateShape ) {
                hGeometryInstance.pGraph = New MeshVoronoiGraph();
                hGeometryInstance.pGraph->UseMemoryContext( GetMemoryContext(), TEXT("Scratch") );
                hGeometryInstance.pGraph->Create();
                ((MeshCylinder*)hGeometryInstance.pMesh)->GenerateGraph( (MeshVoronoiGraph*)(hGeometryInstance.pGraph), *(UInt*)(arrParameters[1]), *(UInt*)(arrParameters[2]), *(Bool*)(arrParameters[3]) );

                UInt iOffset, iSize;
                pIL->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_POSITION, 0 );
                Byte * arrPositions = hGeometryInstance.pMesh->GetVB()->GetData() + iOffset;

                ((MeshVoronoiGraph*)hGeometryInstance.pGraph)->ComputeFeatureData( arrPositions, hGeometryInstance.pMesh->GetVertexSize() );
                hGeometryInstance.pShape = PhysicsFn->CreateShapeConvexMesh( (MeshVoronoiGraph*)(hGeometryInstance.pGraph), (MeshCylinder*)(hGeometryInstance.pMesh) );
            }
            break;
        default: Assert(false); break;
    }

    UnSelectMemory();

    if ( hGeometryInstance.pMesh == NULL )
        return NULL;

    _GeometryInstanceMap::Iterator itGeometryInstance;
    Bool bInserted = m_hGeometryInstanceMap.Insert( strName, hGeometryInstance, &itGeometryInstance );
    Assert( bInserted && !(itGeometryInstance.IsNull()) );

    return itGeometryInstance.GetItemPtr();
}
Void GraphicsManager::DestroyGeometryInstance( const GChar * strName )
{
    GeometryInstance hGeometryInstance;
    hGeometryInstance.idGeometry = GEOMETRYID_COUNT;
    Bool bRemoved = m_hGeometryInstanceMap.Remove( strName, &hGeometryInstance );
    Assert( bRemoved && hGeometryInstance.idGeometry != GEOMETRYID_COUNT );

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

//Bool GraphicsManager::ToggleWireframe()
//{
//    m_bWireframe = !m_bWireframe;
//
//    for( UInt i = 0; i < EFFECTID_COUNT; ++i ) {
//        GPURasterizerState * pRasterState = m_arrEffects[i]->GetRasterizerState(0,0);
//
//        GPURasterizerStateDesc hDesc;
//        pRasterState->GetDesc( &hDesc );
//        pRasterState->UnBind();
//
//        hDesc.iFillMode = (m_bWireframe) ? GPURASTERIZER_FILL_WIREFRAME : GPURASTERIZER_FILL_SOLID;
//        pRasterState->Bind( &hDesc );
//    }
//
//    return m_bWireframe;
//}


