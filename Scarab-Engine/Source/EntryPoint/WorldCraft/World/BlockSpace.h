/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/World/BlockSpace.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World hierarchy : BlockSpace (elementary volume)
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
#ifndef SCARAB_ENTRYPOINT_WORLDCRAFT_WORLD_BLOCKSPACE_H
#define SCARAB_ENTRYPOINT_WORLDCRAFT_WORLD_BLOCKSPACE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Blocks/Block.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// BlockSpace coordinates (relative to parent area)
typedef Point3 BlockSpaceCoords;

// MicroBlock coordinates (relative to parent blockspace)
typedef Point3 MicroBlockCoords;

// Adjacency
enum BlockSpaceAdjacency {
    BLOCKSPACE_ADJACENCY_WEST  = 0x01, // -X
    BLOCKSPACE_ADJACENCY_EAST  = 0x02, // +X
    BLOCKSPACE_ADJACENCY_SOUTH = 0x04, // -Y
    BLOCKSPACE_ADJACENCY_NORTH = 0x08, // +Y
    BLOCKSPACE_ADJACENCY_DOWN  = 0x10, // -Z
    BLOCKSPACE_ADJACENCY_UP    = 0x20  // +Z
};

// Layers
enum RegionLayer {
    REGION_LAYER_UNDERGROUND = 0,
    REGION_LAYER_SURFACE,
    REGION_LAYER_ATMOSPHERE,
    REGION_LAYER_OUTERSPACE,    // Space outside of a planet (below moons)
    REGION_LAYER_MOONS,
    REGION_LAYER_FARSPACE,      // Space between planets in a star system
    REGION_LAYER_DEEPSPACE,     // Space between star systems in a galaxy
    REGION_LAYER_OBLIVIONSPACE, // Space between galaxies in a dimension

    REGION_LAYER_INVALID
};
typedef DWord MoonID; // REGION_LAYER_MOONS only

// Prototypes
class Area;
class Region;
class Planet;
class VoidSpace;
class StarSystem;
class Galaxy;
class Dimension;
struct BlockSpaceUID;
struct AreaUID;
struct RegionUID;

/////////////////////////////////////////////////////////////////////////////////
// The BlockSpace class
class BlockSpace
{
public:
    BlockSpace( Area * pParentArea, const BlockSpaceCoords & vBlockSpaceCoords );
    ~BlockSpace();

    // BlockSpace data
    inline const BlockSpaceCoords & GetBlockSpaceCoords() const;

    Void MakeUID( BlockSpaceUID * outUID ) const;

    // Parent linkage
    inline Area * GetParentArea() const;

    Region * GetParentRegion() const;
    RegionLayer GetRegionLayer( MoonID * outMoonID = NULL ) const;

    Planet * GetParentPlanet() const;
    VoidSpace * GetParentVoidSpace() const;

    StarSystem * GetParentStarSystem() const;
    Galaxy * GetParentGalaxy() const;
    Dimension * GetParentDimension() const;

    // Adjacency linkage
    BlockSpace * GetAdjacentSpace( BlockSpaceAdjacency iAdjacentSpace ) const;

    // Child linkage : Block containment
    inline BlockInstance * GetBlock() const;
    inline Void SetBlock( BlockInstance * pBlockInstance );

    Void GetBlockPosition( Point3 * outPosition ) const;
    inline Void GetBlockOrigin( Vertex3 * outOrigin ) const;
    inline Void GetBlockCenter( Vertex3 * outCenter ) const;

    inline Bool HasMicroBlockSpace() const;
    inline UInt GetMicroBlockSpaceDepth() const;
    inline Scalar GetMicroBlockUnit() const;

    Void CreateMicroBlockSpace( UInt iDepth );
    Void DestroyMicroBlockSpace();

    inline Void GetMicroBlockOrigin( Vertex3 * outOrigin, const MicroBlockCoords & vMicroBlockCoords ) const;
    inline Void GetMicroBlockCenter( Vertex3 * outCenter, const MicroBlockCoords & vMicroBlockCoords ) const;

    inline BlockInstance * GetMicroBlock( const MicroBlockCoords & vMicroBlockCoords ) const;
    inline Void SetMicroBlock( const MicroBlockCoords & vMicroBlockCoords, BlockInstance * pBlockInstance );
    Void GetMicroBlockLayout( MicroBlockCoords * outMicroBlockOrigin, MicroBlockCoords * outMicroBlockSize, const MicroBlockCoords & vMicroBlockCoords ) const;

    Bool AddMicroBlock( const MicroBlockCoords & vMicroBlockCoords, const MicroBlockCoords & vMicroBlockSize, BlockInstance * pBlockInstance );
    BlockInstance * RemoveMicroBlock( const MicroBlockCoords & vMicroBlockCoords, MicroBlockCoords * outLayoutOrigin = NULL, MicroBlockCoords * outLayoutSize = NULL );
    Void ClearMicroBlocks();

private:
    // BlockSpace data
    BlockSpaceCoords m_vBlockSpaceCoords;

    // Parent data : Area
    Area * m_pParentArea;

    // Child linkage : Block containment
    BlockInstance * m_pBlock;

    Bool m_bHasMicroBlockSpace;
    UInt m_iMicroBlockSpaceDepth;
    UInt m_iMicroBlockSpaceSlice;
    Scalar m_fMicroBlockUnit;

    UInt m_iMicroBlockCount;
    BlockInstance ** m_arrMicroBlocks;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "BlockSpace.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDCRAFT_WORLD_BLOCKSPACE_H
