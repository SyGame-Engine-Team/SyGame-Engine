/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/Gameplay/Blocks/Block.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Block base interface
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
#include "Block.h"

#include "BlockManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Block implementation
Block::Block( const BlockID & hBlockID, const GChar * strName ):
    m_hID(), m_vMicroBlockSize()
{
    m_hID.dwID = hBlockID.dwID;
    m_hID.dwSpecifier = hBlockID.dwSpecifier;
    StringFn->NCopy( m_strName, strName, BLOCK_NAME_LENGTH );

    // Default properties (air-like)
    m_bPassThrough = true;
    m_bBreakable = false;
    m_fHardness = SCALAR_INFINITE;
    m_bHasGravity = false;
    m_fMass = 0.0f;
    m_bIsLiquid = false;
    m_bIsGas = false;
    m_fViscosity = SCALAR_INFINITE;

    // Drop table
    //m_iDropTableSize = 0;
    //for( UInt i = 0; i < BLOCK_MAX_DROPS; ++i ) {
    //    m_arrDropTable[i].pItem = NULL;
    //    m_arrDropTable[i].fDropRate = 0.0f;
    //    m_arrDropTable[i].iMinAmount = 0;
    //    m_arrDropTable[i].iMaxAmount = 0;
    //}

    // MicroBlocks
    m_bIsMicroBlock = false;
    m_iMinMicroBlockDepth = 0;
    m_vMicroBlockSize = Point3::Null;

    // MultiBlocks
    //m_pMultiBlockPattern = NULL;

    // I/O
    //m_bHasIOInterfaces = false;
    //for( UInt i = 0; i < 6; ++i )
    //    m_arrFaceInterfaces[i] = NULL;

    // Rendering
    m_bHasVisual = false;
    m_pIconSmall = NULL;
    m_pIconLarge = NULL;
    for( UInt i = 0; i < 6; ++i ) {
        m_arrMaterials[i].Ambient.A = 0.0f;
        m_arrMaterials[i].Emissive.A = 0.0f;
        m_arrMaterials[i].Diffuse.A = 0.0f;
        m_arrMaterials[i].Specular.A = 0.0f;
        m_arrTextures[i] = NULL;
        //m_arrShadowMaps[i] = NULL;
        //m_arrBumpMaps[i] = NULL;
    }
}
Block::~Block()
{
    // Rendering
    if ( m_bHasVisual ) {
        for( UInt i = 0; i < 6; ++i ) {
            if ( m_arrTextures[i] != NULL ) {
                m_arrTextures[i]->UnBind();
                RenderingFn->DestroyResource( m_arrTextures[i] );
            }
            //if ( m_arrShadowMaps[i] != NULL ) {
            //    m_arrShadowMaps[i]->UnBind();
            //    RenderingFn->DestroyResource( m_arrShadowMaps[i] );
            //}
            //if ( m_arrBumpMaps[i] != NULL ) {
            //    m_arrBumpMaps[i]->UnBind();
            //    RenderingFn->DestroyResource( m_arrBumpMaps[i] );
            //}
        }
        if ( m_pIconLarge != NULL ) {
            m_pIconLarge->UnBind();
            RenderingFn->GetScreen2DContext()->DestroyBitmap( m_pIconLarge );
        }
        if ( m_pIconSmall != NULL ) {
            m_pIconSmall->UnBind();
            RenderingFn->GetScreen2DContext()->DestroyBitmap( m_pIconSmall );
        }
    }

    // I/O
    //if ( m_bHasIOInterfaces ) {
    //    BlockFn->SelectMemory( TEXT("IOInterfaces") );
    //    for( UInt i = 0; i < 6; ++i ) {
    //        if ( m_arrFaceInterfaces[i] != NULL )
    //            Delete( m_arrFaceInterfaces[i] );
    //    }
    //    BlockFn->UnSelectMemory();
    //}

    // MultiBlocks
    // We don't own the pattern !
}

/////////////////////////////////////////////////////////////////////////////////
// BlockInstance implementation
BlockInstance::BlockInstance( const Block * pBlock, BlockOrientation iOrientation )
{
    m_pBlock = pBlock;
    m_iOrientation = iOrientation;
}
BlockInstance::~BlockInstance()
{
    // nothing to do
}
