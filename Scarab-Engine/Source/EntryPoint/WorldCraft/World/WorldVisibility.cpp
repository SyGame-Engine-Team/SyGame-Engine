/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/World/WorldVisibility.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World integration with outdoor culling system
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
#include "WorldVisibility.h"

/////////////////////////////////////////////////////////////////////////////////
// BlockSpaceNode implementation
BlockSpaceNode::BlockSpaceNode( BlockSpace * pBlockSpace, const BlockSpaceCoords & vBlockSpaceCoords )
{
}
BlockSpaceNode::~BlockSpaceNode()
{
}

/////////////////////////////////////////////////////////////////////////////////
// BlockWorldArea implementation
BlockWorldArea::BlockWorldArea( const Vertex2 & vOrigin, const GChar * strName ):
    WorldOutdoorArea( vOrigin, Vector2(AREA_UNIT,AREA_UNIT), AREA_SIZE, AREA_SIZE, strName )
{
    m_pVisibleArea = NULL;
}
BlockWorldArea::~BlockWorldArea()
{
    // nothing to do
}

Scalar BlockWorldArea::GetHeight( const Vertex2 & vPosition ) const
{
    Assert( m_pVisibleArea != NULL );

    // Get block position (X,Y)
    UInt iX, iY;
    Bool bClipped = GetGridPosition( &iX, &iY, vPosition );
    Assert( bClipped );

    // Search for the highest non-empty blockspace on that column
    BlockSpaceCoords vBlockSpaceCoords( iX, iY, 255 );
    Scalar fHeight = ( BLOCK_UNIT * 256.0f );
    while( vBlockSpaceCoords.Z >= 0 ) {
        BlockSpace * pBlockSpace = m_pVisibleArea->GetBlockSpace( vBlockSpaceCoords );
        if ( pBlockSpace->GetBlock() != NULL )
            return fHeight;

        if ( pBlockSpace->HasMicroBlockSpace() ) {
            UInt iDepth = pBlockSpace->GetMicroBlockSpaceDepth();
            Scalar fInvDepth = MathFn->Invert( (Scalar)iDepth );

            // Get micro block position (X,Y)
            Vertex3 vBlockOrigin;
            pBlockSpace->GetBlockOrigin( &vBlockOrigin );

            Vector2 vLocalPos = ( vPosition - (Vertex2)vBlockOrigin );
            iX = (UInt)( MathFn->Floor( vLocalPos.X * fInvDepth ) );
            iY = (UInt)( MathFn->Floor( vLocalPos.Y * fInvDepth ) );

            MicroBlockCoords vMicroBlockCoords( iX, iY, iDepth - 1 );
            Scalar fMicroHeight = fHeight;
            while( vMicroBlockCoords.Z >= 0 ) {
                if ( pBlockSpace->GetMicroBlock(vMicroBlockCoords) != NULL )
                    return fMicroHeight;                
                --(vMicroBlockCoords.Z);
                fMicroHeight -= ( BLOCK_UNIT * fInvDepth );
            }
        }

        --(vBlockSpaceCoords.Z);
        fHeight -= BLOCK_UNIT;
    }

    Assert( false ); // Did you break my bedrock ?!!
    return -1.0f;    // Should never happen since you'll be warped to the layer below before that case can occur
}

Void BlockWorldArea::OnAreaUpdate( const Point2 & vAreaCoords )
{
    // Get parent region
    BlockWorldRegion * pParent = (BlockWorldRegion*)( GetParent() );

    // Get area from block world
    AreaUID hAreaUID;
    hAreaUID.hRegionUID = pParent->GetRegionUID();
    hAreaUID.vAreaCoords = vAreaCoords;
    m_pVisibleArea = _QueryArea( hAreaUID );

    // Build BSPTree for that area
    WorldFn->SelectMemory( TEXT("Nodes") );

        // Leaf layer : Pack by 4*4*4 sets, 16*16*256 => 4*4*64
    WorldNode * arrLayer[1024];
    GChar strName[64];
    UInt iIndex = 0;

    BlockSpaceCoords vBlockSpaceCoords;
    for( vBlockSpaceCoords.Z = 0; vBlockSpaceCoords.Z < AREA_HEIGHT; vBlockSpaceCoords.Z += 4 ) {
        for( vBlockSpaceCoords.Y = 0; vBlockSpaceCoords.Y < AREA_SIZE; vBlockSpaceCoords.Y += 4 ) {
            for( vBlockSpaceCoords.X = 0; vBlockSpaceCoords.X < AREA_SIZE; vBlockSpaceCoords.X += 4 ) {
                // Collect all blockspaces for that leaf
                StringFn->Format( strName, TEXT("BSPLeaf_%d_%d_%d"), vBlockSpaceCoords.X, vBlockSpaceCoords.Y, vBlockSpaceCoords.Z );
                arrLayer[iIndex] = New WorldNode( strName );

                for( Int iZ = 0; iZ < 4; ++iZ ) {
                    for( Int iY = 0; iY < 4; ++iY ) {
                        for( Int iX = 0; iX < 4; ++iX ) {
                            BlockSpaceCoords vTmp = ( vBlockSpaceCoords + Offset3(iX, iY, iZ) );
                            BlockSpace * pBlockSpace = m_pVisibleArea->GetBlockSpace( vTmp );
                            arrLayer[iIndex]->AddChild( New BlockSpaceNode(pBlockSpace, vTmp) );
                        }
                    }
                }

                ++iIndex;
            }
        }
    }
    UInt iSpanX = ( AREA_SIZE >> 2 );
    UInt iSpanY = ( AREA_SIZE >> 2 );
    UInt iSpanZ = ( AREA_HEIGHT >> 2 );

        // Node layers
    _BuildBSPTree( arrLayer, AXIS_Z, iSpanX, iSpanY, iSpanZ ); // 4*4*64 => 4*4*32
    iSpanZ >>= 1;
    _BuildBSPTree( arrLayer, AXIS_Y, iSpanX, iSpanY, iSpanZ ); // 4*4*32 => 4*2*32
    iSpanY >>= 1;
    _BuildBSPTree( arrLayer, AXIS_X, iSpanX, iSpanY, iSpanZ ); // 4*2*32 => 2*2*32
    iSpanX >>= 1;
    _BuildBSPTree( arrLayer, AXIS_Z, iSpanX, iSpanY, iSpanZ ); // 2*2*32 => 2*2*16
    iSpanZ >>= 1;
    _BuildBSPTree( arrLayer, AXIS_Y, iSpanX, iSpanY, iSpanZ ); // 2*2*16 => 2*1*16
    iSpanY >>= 1;
    _BuildBSPTree( arrLayer, AXIS_X, iSpanX, iSpanY, iSpanZ ); // 2*1*16 => 1*1*16
    iSpanX >>= 1;
    _BuildBSPTree( arrLayer, AXIS_Z, iSpanX, iSpanY, iSpanZ ); // 1*1*16 => 1*1*8
    iSpanZ >>= 1;
    _BuildBSPTree( arrLayer, AXIS_Z, iSpanX, iSpanY, iSpanZ ); // 1*1*8 => 1*1*4
    iSpanZ >>= 1;
    _BuildBSPTree( arrLayer, AXIS_Z, iSpanX, iSpanY, iSpanZ ); // 1*1*4 => 1*1*2
    iSpanZ >>= 1;
    Assert( iSpanX == 1 );
    Assert( iSpanY == 1 );
    Assert( iSpanZ == 2 );

        // Root layer
    WorldBSPNode * pRoot = New WorldBSPNode( TEXT("Root") );
    pRoot->SetNegativeChild( arrLayer[0] );
    pRoot->SetPositiveChild( arrLayer[1] );

    WorldFn->UnSelectMemory();

    // Done
    AddChild( pRoot );
}

/////////////////////////////////////////////////////////////////////////////////

Area * BlockWorldArea::_QueryArea( const AreaUID & hAreaUID ) const
{
    return BlockWorldFn->GetArea( hAreaUID );
}
Void BlockWorldArea::_BuildBSPTree( WorldNode ** arrLayer, Axis iAxis, UInt iSpanX, UInt iSpanY, UInt iSpanZ )
{
    // We have 16*16*256 block spaces, we aim for leaves with 4*4*4 block spaces : split accordingly
    WorldBSPNode * pNode;
    GChar strName[64];
    UInt iX, iY, iZ, iIndex;

    UInt iPitch = iSpanX;
    UInt iSlice = iSpanY * iPitch;

    switch( iAxis ) {
        case AXIS_Z:
            for( iZ = 0; iZ < iSpanZ; iZ += 2 ) {
                for( iY = 0; iY < iSpanY; ++iY ) {
                    for( iX = 0; iX < iSpanX; ++iX ) {
                        StringFn->Format( strName, TEXT("BSPNode_Z_%d_%d_%d"), iX, iY, iZ );
                        pNode = New WorldBSPNode( strName );

                        iIndex = ( iZ * iSlice + iY * iPitch + iX );
                        pNode->SetNegativeChild( arrLayer[iIndex] );
                        pNode->SetPositiveChild( arrLayer[iIndex + iSlice] );

                        arrLayer[iIndex - ((iZ>>1) * iSlice)] = pNode; // Compact array on-the-fly
                        arrLayer[iIndex + iSlice] = NULL;
                    }
                }
            }
            break;
        case AXIS_Y:
            for( iZ = 0; iZ < iSpanZ; ++iZ ) {
                for( iY = 0; iY < iSpanY; iY += 2 ) {
                    for( iX = 0; iX < iSpanX; ++iX ) {
                        StringFn->Format( strName, TEXT("BSPNode_Y_%d_%d_%d"), iX, iY, iZ );
                        WorldBSPNode * pNode = New WorldBSPNode( strName );

                        iIndex = ( iZ * iSlice + iY * iPitch + iX );
                        pNode->SetNegativeChild( arrLayer[iIndex] );
                        pNode->SetPositiveChild( arrLayer[iIndex + iPitch] );

                        arrLayer[iIndex - ((iY>>1) * iPitch)] = pNode; // Compact array on-the-fly
                        arrLayer[iIndex + iPitch] = NULL;
                    }
                }
            }
            break;
        case AXIS_X:
            for( iZ = 0; iZ < iSpanZ; ++iZ ) {
                for( iY = 0; iY < iSpanY; ++iY ) {
                    for( iX = 0; iX < iSpanX; iX += 2 ) {
                        StringFn->Format( strName, TEXT("BSPNode_X_%d_%d_%d"), iX, iY, iZ );
                        WorldBSPNode * pNode = New WorldBSPNode( strName );

                        iIndex = ( iZ * iSlice + iY * iPitch + iX );
                        pNode->SetNegativeChild( arrLayer[iIndex] );
                        pNode->SetPositiveChild( arrLayer[iIndex + 1] );

                        arrLayer[iIndex - (iX>>1)] = pNode; // Compact array on-the-fly
                        arrLayer[iIndex + 1] = NULL;
                    }
                }
            }
            break;
        default: Assert( false ); break;
    }
}

/////////////////////////////////////////////////////////////////////////////////
// BlockWorldRegion implementation
BlockWorldRegion::BlockWorldRegion( const RegionUID & hRegionUID, UInt iViewDistance, const GChar * strName ):
    WorldOutdoor( _GetOrigin(hRegionUID.vRegionCoords), Vector2(AREA_UNIT,AREA_UNIT), iViewDistance, strName ),
    m_hRegionUID( hRegionUID )
{
    // WorldOutdoor initialization
    _Initialize();
}
BlockWorldRegion::~BlockWorldRegion()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

WorldOutdoorArea * BlockWorldRegion::_CreateArea( const Vertex2 & vOrigin, const Vector2 & /*vSize*/, const GChar * strName )
{
    return New BlockWorldArea( vOrigin, strName );
}

