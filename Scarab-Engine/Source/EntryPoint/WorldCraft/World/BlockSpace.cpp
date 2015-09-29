/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/World/BlockSpace.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World hierarchy : BlockSpace
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
#include "BlockSpace.h"

#include "BlockWorld.h"

/////////////////////////////////////////////////////////////////////////////////
// BlockSpace implementation
BlockSpace::BlockSpace( Area * pParentArea, const BlockSpaceCoords & vBlockSpaceCoords ):
    m_vBlockSpaceCoords( vBlockSpaceCoords )
{
    // Parent linkage
    m_pParentArea = pParentArea;

    // Create Block containment
    m_pBlock = NULL;

    m_bHasMicroBlockSpace = false;
    m_iMicroBlockSpaceDepth = 0;
    m_iMicroBlockSpaceSlice = 0;
    m_fMicroBlockUnit = 0.0f;

    m_iMicroBlockCount = 0;
    m_arrMicroBlocks = NULL;
}
BlockSpace::~BlockSpace()
{
    // Destroy Block containment
    DestroyMicroBlockSpace();

    if ( m_pBlock != NULL ) {
        BlockWorldFn->SelectMemory( TEXT("BlockInstances") );
        Delete( m_pBlock );
        m_pBlock = NULL;
        BlockWorldFn->UnSelectMemory();
    }
}

Void BlockSpace::MakeUID( BlockSpaceUID * outUID ) const
{
    m_pParentArea->MakeUID( &(outUID->hAreaUID) );
    outUID->vBlockSpaceCoords = m_vBlockSpaceCoords;
}

Region * BlockSpace::GetParentRegion() const
{
    return m_pParentArea->GetParentRegion();
}
RegionLayer BlockSpace::GetRegionLayer( MoonID * outMoonID ) const
{
    return m_pParentArea->GetRegionLayer( outMoonID );
}

Planet * BlockSpace::GetParentPlanet() const
{
    return m_pParentArea->GetParentPlanet();
}
VoidSpace * BlockSpace::GetParentVoidSpace() const
{
    return m_pParentArea->GetParentVoidSpace();
}

StarSystem * BlockSpace::GetParentStarSystem() const
{
    return m_pParentArea->GetParentStarSystem();
}
Galaxy * BlockSpace::GetParentGalaxy() const
{
    return m_pParentArea->GetParentGalaxy();
}
Dimension * BlockSpace::GetParentDimension() const
{
    return m_pParentArea->GetParentDimension();
}

BlockSpace * BlockSpace::GetAdjacentSpace( BlockSpaceAdjacency iAdjacentSpace ) const
{
    Area * pAdjacentArea = m_pParentArea;
    BlockSpaceCoords vAdjacentCoords = m_vBlockSpaceCoords;

    if ( iAdjacentSpace & BLOCKSPACE_ADJACENCY_WEST ) {
        if ( vAdjacentCoords.X == 0 ) {
            vAdjacentCoords.X = ( AREA_SIZE - 1 );
            pAdjacentArea = pAdjacentArea->GetAdjacentArea( AREA_ADJACENCY_WEST );
        } else
            vAdjacentCoords.X -= 1;
    } else if ( iAdjacentSpace & BLOCKSPACE_ADJACENCY_EAST ) {
        if ( vAdjacentCoords.X == AREA_SIZE - 1 ) {
            vAdjacentCoords.X = 0;
            pAdjacentArea = pAdjacentArea->GetAdjacentArea( AREA_ADJACENCY_EAST );
        } else
            vAdjacentCoords.X += 1;
    }

    if ( iAdjacentSpace & BLOCKSPACE_ADJACENCY_SOUTH ) {
        if ( vAdjacentCoords.Y == 0 ) {
            vAdjacentCoords.Y = ( AREA_SIZE - 1 );
            pAdjacentArea = pAdjacentArea->GetAdjacentArea( AREA_ADJACENCY_SOUTH );
        } else
            vAdjacentCoords.Y -= 1;
    } else if ( iAdjacentSpace & BLOCKSPACE_ADJACENCY_NORTH ) {
        if ( vAdjacentCoords.Y == AREA_SIZE - 1 ) {
            vAdjacentCoords.Y = 0;
            pAdjacentArea = pAdjacentArea->GetAdjacentArea( AREA_ADJACENCY_NORTH );
        } else
            vAdjacentCoords.Y += 1;
    }

    if ( iAdjacentSpace & BLOCKSPACE_ADJACENCY_DOWN ) {
        if ( vAdjacentCoords.Z == 0 ) {
            vAdjacentCoords.Z = ( AREA_HEIGHT - 1 );
            pAdjacentArea = pAdjacentArea->GetAdjacentArea( AREA_ADJACENCY_DOWN );
        } else
            vAdjacentCoords.Z -= 1;
    } else if ( iAdjacentSpace & BLOCKSPACE_ADJACENCY_UP ) {
        if ( vAdjacentCoords.Z == AREA_HEIGHT - 1 ) {
            vAdjacentCoords.Z = 0;
            pAdjacentArea = pAdjacentArea->GetAdjacentArea( AREA_ADJACENCY_UP );
        } else
            vAdjacentCoords.Z += 1;
    }

    if ( pAdjacentArea == NULL )
        return NULL;
    return pAdjacentArea->GetBlockSpace( vAdjacentCoords );
}

Void BlockSpace::GetBlockPosition( Point3 * outPosition ) const
{
    // Get region coordinates
    Region * pParentRegion = m_pParentArea->GetParentRegion();
    const RegionCoords & vRegionCoords = pParentRegion->GetRegionCoords();

    outPosition->X = vRegionCoords.X;
    outPosition->Y = vRegionCoords.Y;
    outPosition->Z = 0;

    // Convert to area coordinates
    const AreaCoords & vAreaCoords = m_pParentArea->GetAreaCoords();
    
    outPosition->X += ( AREA_SIZE * vAreaCoords.X );
    outPosition->Y += ( AREA_SIZE * vAreaCoords.Y );

    // Convert to blockspace coordinates
    outPosition->X += m_vBlockSpaceCoords.X;
    outPosition->Y += m_vBlockSpaceCoords.Y;
    outPosition->Z += m_vBlockSpaceCoords.Z;
}

Void BlockSpace::CreateMicroBlockSpace( UInt iDepth )
{
    if ( m_bHasMicroBlockSpace ) {
        if ( m_iMicroBlockSpaceDepth == iDepth )
            return;
        DestroyMicroBlockSpace();
    }

    m_bHasMicroBlockSpace = true;
    m_iMicroBlockSpaceDepth = iDepth;
    m_iMicroBlockSpaceSlice = iDepth * iDepth;
    m_fMicroBlockUnit = ( BLOCK_UNIT / (Scalar)iDepth );

    m_iMicroBlockCount = iDepth * m_iMicroBlockSpaceSlice;

    BlockWorldFn->SelectMemory( TEXT("Scratch") );
    m_arrMicroBlocks = New BlockInstance*[m_iMicroBlockCount];
    BlockWorldFn->UnSelectMemory();

    for( UInt i = 0; i < m_iMicroBlockCount; ++i )
        m_arrMicroBlocks[i] = NULL;
}
Void BlockSpace::DestroyMicroBlockSpace()
{
    if ( !m_bHasMicroBlockSpace )
        return;

    ClearMicroBlocks();

    BlockWorldFn->SelectMemory( TEXT("Scratch") );
    DeleteA( m_arrMicroBlocks );
    BlockWorldFn->UnSelectMemory();

    m_bHasMicroBlockSpace = false;
    m_iMicroBlockSpaceDepth = 0;
    m_iMicroBlockSpaceSlice = 0;
    m_fMicroBlockUnit = 0.0f;

    m_iMicroBlockCount = 0;
    m_arrMicroBlocks = NULL;
}

Void BlockSpace::GetMicroBlockLayout( MicroBlockCoords * outMicroBlockOrigin, MicroBlockCoords * outMicroBlockSize, const MicroBlockCoords & vMicroBlockCoords ) const
{
    Assert( m_bHasMicroBlockSpace );
    Assert( vMicroBlockCoords.X >= 0 && vMicroBlockCoords.X < (Int)m_iMicroBlockSpaceDepth );
    Assert( vMicroBlockCoords.Y >= 0 && vMicroBlockCoords.Y < (Int)m_iMicroBlockSpaceDepth );
    Assert( vMicroBlockCoords.Z >= 0 && vMicroBlockCoords.Z < (Int)m_iMicroBlockSpaceDepth );

    UInt iRefIndex = ( (vMicroBlockCoords.Z * m_iMicroBlockSpaceSlice) + (vMicroBlockCoords.Y * m_iMicroBlockSpaceDepth) + vMicroBlockCoords.X );
    BlockInstance * pRefMicroBlock = m_arrMicroBlocks[iRefIndex];

    *outMicroBlockOrigin = vMicroBlockCoords;
    *outMicroBlockSize = vMicroBlockCoords;
    UInt iNextIndex, iCurIndex = iRefIndex;

    // Expand along Z axis
    while( outMicroBlockOrigin->Z > 0 ) {
        iNextIndex = ( iCurIndex - m_iMicroBlockSpaceSlice );
        if ( m_arrMicroBlocks[iNextIndex] != pRefMicroBlock )
            break;

        --(outMicroBlockOrigin->Z);
        iCurIndex = iNextIndex;
    }
    while( outMicroBlockSize->Z < (Int)m_iMicroBlockSpaceDepth - 1 ) {
        iNextIndex = ( iCurIndex + m_iMicroBlockSpaceSlice );
        if ( m_arrMicroBlocks[iNextIndex] != pRefMicroBlock )
            break;

        ++(outMicroBlockSize->Z);
        iCurIndex = iNextIndex;
    }

    // Expand along Y axis
    while( outMicroBlockOrigin->Y > 0 ) {
        iNextIndex = ( iCurIndex - m_iMicroBlockSpaceDepth );
        if ( m_arrMicroBlocks[iNextIndex] != pRefMicroBlock )
            break;

        --(outMicroBlockOrigin->Y);
        iCurIndex = iNextIndex;
    }
    while( outMicroBlockSize->Y < (Int)m_iMicroBlockSpaceDepth - 1 ) {
        iNextIndex = ( iCurIndex + m_iMicroBlockSpaceDepth );
        if ( m_arrMicroBlocks[iNextIndex] != pRefMicroBlock )
            break;

        ++(outMicroBlockSize->Y);
        iCurIndex = iNextIndex;
    }

    // Expand along X axis
    while( outMicroBlockOrigin->X > 0 ) {
        iNextIndex = ( iCurIndex - 1 );
        if ( m_arrMicroBlocks[iNextIndex] != pRefMicroBlock )
            break;

        --(outMicroBlockOrigin->X);
        iCurIndex = iNextIndex;
    }
    while( outMicroBlockSize->X < (Int)m_iMicroBlockSpaceDepth - 1 ) {
        iNextIndex = ( iCurIndex + 1 );
        if ( m_arrMicroBlocks[iNextIndex] != pRefMicroBlock )
            break;

        ++(outMicroBlockSize->X);
        iCurIndex = iNextIndex;
    }

    // Convert to size
    outMicroBlockSize->X += ( 1 - outMicroBlockOrigin->X );
    outMicroBlockSize->Y += ( 1 - outMicroBlockOrigin->Y );
    outMicroBlockSize->Z += ( 1 - outMicroBlockOrigin->Z );
}

Bool BlockSpace::AddMicroBlock( const MicroBlockCoords & vMicroBlockCoords, const MicroBlockCoords & vMicroBlockSize, BlockInstance * pBlockInstance )
{
    Assert( m_bHasMicroBlockSpace );
    Assert( vMicroBlockCoords.X >= 0 && vMicroBlockCoords.X < (Int)m_iMicroBlockSpaceDepth );
    Assert( vMicroBlockCoords.Y >= 0 && vMicroBlockCoords.Y < (Int)m_iMicroBlockSpaceDepth );
    Assert( vMicroBlockCoords.Z >= 0 && vMicroBlockCoords.Z < (Int)m_iMicroBlockSpaceDepth );
    Assert( (vMicroBlockCoords.X + vMicroBlockSize.X) <= (Int)m_iMicroBlockSpaceDepth );
    Assert( (vMicroBlockCoords.Y + vMicroBlockSize.Y) <= (Int)m_iMicroBlockSpaceDepth );
    Assert( (vMicroBlockCoords.Z + vMicroBlockSize.Z) <= (Int)m_iMicroBlockSpaceDepth );
    
    UInt iIndex = ( (vMicroBlockCoords.Z * (Int)m_iMicroBlockSpaceSlice) + (vMicroBlockCoords.Y * (Int)m_iMicroBlockSpaceDepth) + vMicroBlockCoords.X );

    MicroBlockCoords vCur;
    for( vCur.Z = 0; vCur.Z < vMicroBlockSize.Z; ++(vCur.Z) ) {
        for( vCur.Y = 0; vCur.Y < vMicroBlockSize.Y; ++(vCur.Y) ) {
            for( vCur.X = 0; vCur.X < vMicroBlockSize.X; ++(vCur.X) ) {
                UInt iCurIndex = iIndex + ( (vCur.Z * (Int)m_iMicroBlockSpaceSlice) + (vCur.Y * (Int)m_iMicroBlockSpaceDepth) + vCur.X );
                if ( m_arrMicroBlocks[iCurIndex] != NULL )
                    return false;
            }
        }
    }
    for( vCur.Z = 0; vCur.Z < vMicroBlockSize.Z; ++(vCur.Z) ) {
        for( vCur.Y = 0; vCur.Y < vMicroBlockSize.Y; ++(vCur.Y) ) {
            for( vCur.X = 0; vCur.X < vMicroBlockSize.X; ++(vCur.X) ) {
                UInt iCurIndex = iIndex + ( (vCur.Z * (Int)m_iMicroBlockSpaceSlice) + (vCur.Y * (Int)m_iMicroBlockSpaceDepth) + vCur.X );
                m_arrMicroBlocks[iCurIndex] = pBlockInstance;
            }
        }
    }
    return true;
}
BlockInstance * BlockSpace::RemoveMicroBlock( const MicroBlockCoords & vMicroBlockCoords, MicroBlockCoords * outLayoutOrigin, MicroBlockCoords * outLayoutSize )
{
    Assert( m_bHasMicroBlockSpace );
    Assert( vMicroBlockCoords.X >= 0 && vMicroBlockCoords.X < (Int)m_iMicroBlockSpaceDepth );
    Assert( vMicroBlockCoords.Y >= 0 && vMicroBlockCoords.Y < (Int)m_iMicroBlockSpaceDepth );
    Assert( vMicroBlockCoords.Z >= 0 && vMicroBlockCoords.Z < (Int)m_iMicroBlockSpaceDepth );

    UInt iIndex = ( (vMicroBlockCoords.Z * (Int)m_iMicroBlockSpaceSlice) + (vMicroBlockCoords.Y * (Int)m_iMicroBlockSpaceDepth) + vMicroBlockCoords.X );
    if ( m_arrMicroBlocks[iIndex] == NULL )
        return NULL;

    BlockInstance * pRemoved = m_arrMicroBlocks[iIndex];

    MicroBlockCoords vOrigin, vSize;
    GetMicroBlockLayout( &vOrigin, &vSize, vMicroBlockCoords );

    iIndex = ( (vOrigin.Z * (Int)m_iMicroBlockSpaceSlice) + (vOrigin.Y * (Int)m_iMicroBlockSpaceDepth) + vOrigin.X );

    MicroBlockCoords vCur;
    for( vCur.Z = 0; vCur.Z < vSize.Z; ++(vCur.Z) ) {
        for( vCur.Y = 0; vCur.Y < vSize.Y; ++(vCur.Y) ) {
            for( vCur.X = 0; vCur.X < vSize.X; ++(vCur.X) ) {
                UInt iCurIndex = iIndex + ( (vCur.Z * (Int)m_iMicroBlockSpaceSlice) + (vCur.Y * (Int)m_iMicroBlockSpaceDepth) + vCur.X );
                Assert( m_arrMicroBlocks[iCurIndex] == pRemoved );
                m_arrMicroBlocks[iCurIndex] = NULL;
            }
        }
    }

    if ( outLayoutOrigin != NULL )
        *outLayoutOrigin = vOrigin;
    if ( outLayoutSize != NULL )
        *outLayoutSize = vSize;

    return pRemoved;
}
Void BlockSpace::ClearMicroBlocks()
{
    Assert( m_bHasMicroBlockSpace );

    BlockWorldFn->SelectMemory( TEXT("BlockInstances") );

    MicroBlockCoords vMicroBlockCoords;
    for( vMicroBlockCoords.Z = 0; vMicroBlockCoords.Z < (Int)m_iMicroBlockSpaceDepth; ++(vMicroBlockCoords.Z) ) {
        for( vMicroBlockCoords.Y = 0; vMicroBlockCoords.Y < (Int)m_iMicroBlockSpaceDepth; ++(vMicroBlockCoords.Y) ) {
            for( vMicroBlockCoords.X = 0; vMicroBlockCoords.X < (Int)m_iMicroBlockSpaceDepth; ++(vMicroBlockCoords.X) ) {
                BlockInstance * pMicroBlock = RemoveMicroBlock( vMicroBlockCoords );
                if ( pMicroBlock != NULL ) {
                    Delete( pMicroBlock );
                    pMicroBlock = NULL;
                }
            }
        }
    }

    BlockWorldFn->UnSelectMemory();
}

