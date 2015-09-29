/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/World/Landscape.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, World : Landscape
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
#include "Landscape.h"

#include "../Entities/EntityManager.h"

/////////////////////////////////////////////////////////////////////////////////
// LandscapeRegion implementation
LandscapeRegion::LandscapeRegion( Landscape * pParentLandscape, const Vertex2 & vOrigin, const Vector2 & vSize, Scalar fMinHeight, Scalar fMaxHeight,
			                      UInt iSampleCountX, UInt iSampleCountY, const GChar * strName ):
    WorldOutdoorRegion( vOrigin, vSize, fMinHeight, fMaxHeight, iSampleCountX, iSampleCountY, strName )
{
    m_pParentLandscape = pParentLandscape;

    // Create Landscape
    m_pLandscapeEffect = (EffectColor*)( GraphicsFn->GetEffect( EFFECTID_COLOR ) );
    m_pLandscapeEffectInstance = (EffectColorInstance*)( GraphicsFn->CreateEffectInstance( EFFECTID_COLOR ) );

    GPUInputLayout * pIL = m_pLandscapeEffectInstance->GetTechnique()->GetInputLayout(0);

    WorldFn->SelectMemory( TEXT("Outdoor") );
    m_pLandscapeMesh = New MeshHeightField( m_arrHeightField, m_vOrigin, m_vSize, m_fMinHeight, m_fMaxHeight, m_iSampleCountX, m_iSampleCountY, pIL );
    WorldFn->UnSelectMemory();

    m_pLandscapeShape = PhysicsFn->CreateShapeHeightField( m_pLandscapeMesh );
    m_pLandscapeBody = PhysicsFn->CreateRigidBody( true, m_pLandscapeShape, 1.0f, Vertex3::Null, Quaternion::Identity, Vector3::Null, Vector3::Null );

    WorldFn->SelectMemory( TEXT("Outdoor") );
    m_pLandscape = New WorldLeaf( TEXT("Landscape") );
    WorldFn->UnSelectMemory();

    m_pLandscape->SetMesh( m_pLandscapeMesh );
    m_pLandscape->SetEffectInstance( m_pLandscapeEffectInstance );
    m_pLandscape->SetBody( m_pLandscapeBody );

    AddChild( m_pLandscape );

    // Debug ...
    m_pLandscape->SetDebugFlags( WORLD_DEBUGDRAW_CONTACTS | WORLD_DEBUGDRAW_CONTACTPOINTS );
    SetDebugFlags( WORLD_DEBUGDRAW_PATHLINKS );
}
LandscapeRegion::~LandscapeRegion()
{
    RemoveAllChildren();

    m_pLandscape->SetBody( NULL );
    m_pLandscape->SetEffectInstance( NULL );
    m_pLandscape->SetMesh( NULL );

    WorldFn->SelectMemory( TEXT("Outdoor") );
    Delete( m_pLandscape );
    m_pLandscape = NULL;
    WorldFn->UnSelectMemory();

    PhysicsFn->DestroyBody( m_pLandscapeBody );
    m_pLandscapeBody = NULL;
    PhysicsFn->DestroyShape( m_pLandscapeShape );
    m_pLandscapeShape = NULL;

    WorldFn->SelectMemory( TEXT("Outdoor") );
    Delete( m_pLandscapeMesh );
    m_pLandscapeMesh = NULL;
    WorldFn->UnSelectMemory();

    GraphicsFn->DestroyEffectInstance( m_pLandscapeEffectInstance );
    m_pLandscapeEffectInstance = NULL;
}

Void LandscapeRegion::OnRegionUpdate( UInt iRegionX, UInt iRegionY )
{
    // Update region
    m_vOrigin = m_pParentLandscape->GetOrigin();
    m_vSize = m_pParentLandscape->GetRegionSize();
    m_vOrigin.X += ( m_vSize.X * (Scalar)iRegionX );
    m_vOrigin.Y += ( m_vSize.Y * (Scalar)iRegionY );

    // Update mesh
    //m_arrHeightField = m_pLandscapeMesh->UpdateGeometryBegin( &m_iSampleCountX, &m_iSampleCountY );

    UInt iMinX, iMinY, iMaxX, iMaxY;
    m_pParentLandscape->GetMinEyeRegion( &iMinX, &iMinY );
    m_pParentLandscape->GetMaxEyeRegion( &iMaxX, &iMaxY );
    Color4 fColor = Color4( 0.0f, 0.5f, 0.0f, 1.0f );
    if ( iRegionX < iMinX || iRegionY < iMinY || iRegionX > iMaxX || iRegionY > iMaxY )
        fColor = Color4( 0.5f, 0.0f, 0.0f, 1.0f );

    UInt iVertexSize = m_pLandscapeMesh->GetVertexSize();
    UInt iVertexCount = m_pLandscapeMesh->GetVertexCount();

    UInt iOffset, iSize;
    m_pLandscapeMesh->GetIL()->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_COLOR, 0 );
    Byte * arrColors = m_pLandscapeMesh->GetVB()->GetData() + iOffset;

    Color4 * pColor;

    for( UInt i = 0; i < iVertexCount; ++i ) {
        pColor = (Color4*)arrColors;
        *pColor = fColor;
        arrColors += iVertexSize;
    }

    for( UInt iY = 0; iY < m_iSampleCountY; ++iY ) {
        for( UInt iX = 0; iX < m_iSampleCountX; ++iX ) {
            m_arrHeightField[(iY * m_iSampleCountX) + iX] = 0.0f;
        }
    }
    m_fMinHeight = -SHAPE_MARGIN;
    m_fMaxHeight = +SHAPE_MARGIN;

    m_pLandscapeMesh->UpdateGeometryEnd( m_vOrigin, m_vSize, m_fMinHeight, m_fMaxHeight, NULL );

    // Update cost field
    for( UInt iY = 0; iY < m_iGridHeight; ++iY ) {
        for( UInt iX = 0; iX < m_iGridWidth; ++iX ) {
            UInt iIndex = ( (iY * (m_iGridWidth << 1)) + (iX << 1) );
            m_arrCostGrid[iIndex] = 1.0f;
            m_arrCostGrid[iIndex + 1] = 1.0f;
        }
    }

    // Update path-finding data
    _PathFindingGraph_Update();
}

/////////////////////////////////////////////////////////////////////////////////
// RPGLandscape implementation
Landscape::Landscape():
    WorldOutdoorArea( Vertex2::Null, RPGLANDSCAPE_MAX_REGIONS, RPGLANDSCAPE_MAX_REGIONS, RPGLANDSCAPE_VIEW_DISTANCE,
                      Vector2(RPGLANDSCAPE_REGION_SIZE,RPGLANDSCAPE_REGION_SIZE), RPGLANDSCAPE_REGION_HEIGHT_MIN, RPGLANDSCAPE_REGION_HEIGHT_MAX, TEXT("LandscapeArea") )
{
    _InitializeRegions( RPGLANDSCAPE_REGION_SAMPLES, RPGLANDSCAPE_REGION_SAMPLES );

    m_pEyeEntity = NULL;
}
Landscape::~Landscape()
{
	// nothing to do
}

Void Landscape::SetEyeEntity( CharEntity * pEyeEntity )
{
    m_pEyeEntity = pEyeEntity;

    //const Vertex2 & vOrigin = GetOrigin();
    //Vertex3 vMin(
    //    vOrigin.X + 1.0f,
    //    vOrigin.Y + 1.0f,
    //    GetMinHeight() + 1.0f );
    //Vertex3 vMax(
    //    vOrigin.X + (RPGLANDSCAPE_REGION_SIZE * (Scalar)RPGLANDSCAPE_MAX_REGIONS) - 1.0f,
    //    vOrigin.Y + (RPGLANDSCAPE_REGION_SIZE * (Scalar)RPGLANDSCAPE_MAX_REGIONS) - 1.0f,
    //    GetMaxHeight() - 1.0f
    //);
    m_pEyeEntity->GetController()->SetBounded( true, GetMinEyeBound(), GetMaxEyeBound() );
}

/////////////////////////////////////////////////////////////////////////////////

WorldOutdoorRegion * Landscape::_CreateRegion( const Vertex2 & vOrigin, const Vector2 & vSize, Scalar fMinHeight, Scalar fMaxHeight,
                                               UInt iSampleCountX, UInt iSampleCountY, const GChar * strName )
{
    return New LandscapeRegion( this, vOrigin, vSize, fMinHeight, fMaxHeight, iSampleCountX, iSampleCountY, strName );
}
Void Landscape::_OnEyeUpdate( Vertex2 * outEye ) const
{
    // Override with our entity's eye
    if ( m_pEyeEntity != NULL )
        *outEye = m_pEyeEntity->GetPosition();
}


