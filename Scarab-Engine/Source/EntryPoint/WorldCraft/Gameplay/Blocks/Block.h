/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/Gameplay/Blocks/Block.h
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
// Header prelude
#ifndef SCARAB_ENTRYPOINT_WORLDCRAFT_GAMEPLAY_BLOCKS_BLOCK_H
#define SCARAB_ENTRYPOINT_WORLDCRAFT_GAMEPLAY_BLOCKS_BLOCK_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Math/Types/Discrete/Box3.h"

#include "../Graphics/GraphicsManager.h"

//#include "../Items/Item.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define BLOCK_UNIT      1.0f // Size of blocks in canonic rendering system
#define BLOCK_INV_UNIT  1.0f
#define BLOCK_HALF_UNIT 0.5f

#define BLOCK_NAME_LENGTH 32
#define BLOCK_MAX_DROPS   4

typedef struct _block_id {
    DWord dwID;
    DWord dwSpecifier;
} BlockID;

enum BlockFace {
    BLOCK_FACE_FRONT = 0,
    BLOCK_FACE_BACK,
    BLOCK_FACE_LEFT,
    BLOCK_FACE_RIGHT,
    BLOCK_FACE_TOP,
    BLOCK_FACE_BOTTOM
};

enum BlockOrientation { // Facing of the front face
    BLOCK_ORIENTATION_NORTH = 0,
    BLOCK_ORIENTATION_SOUTH,
    BLOCK_ORIENTATION_EAST,
    BLOCK_ORIENTATION_WEST,
    BLOCK_ORIENTATION_UP,
    BLOCK_ORIENTATION_DOWN
};

// Prototypes
class Block;
class BlockInstance;

/////////////////////////////////////////////////////////////////////////////////
// The Block class
class Block
{
public:
    Block( const BlockID & hBlockID, const GChar * strName );
    virtual ~Block();

    // Deferred construction

    // ID & Name access
    inline const BlockID & GetID() const;
    inline const GChar * GetName() const;

    // Basic properties
    inline Bool CanPassThrough() const;
    inline Bool IsBreakable() const;
    inline Scalar GetHardness() const;
    inline Bool HasGravity() const;
    inline Scalar GetMass() const;
    inline Bool IsSolid() const;
    inline Bool IsLiquid() const;
    inline Bool IsGas() const;
    inline Bool IsFluid() const;
    inline Scalar GetViscosity() const;
    
    // Item drop properties
    //inline UInt GetDropTableSize() const;
    //inline Item * GetDropItem( UInt iIndex ) const;
    //inline Scalar GetDropRate( UInt iIndex ) const;
    //inline UInt GetDropMinAmount( UInt iIndex ) const;
    //inline UInt GetDropMaxAmount( UInt iIndex ) const;

    // MicroBlocks support
    inline Bool IsMicroBlock() const;
    inline UInt GetMinMicroBlockDepth() const;
    inline const Point3 & GetMicroBlockSize() const;

    // MultiBlocks support
    ///////////////////////////////////////

    // I/O support
    ///////////////////////////////////////

    // Rendering data
    inline GPU2DBitmap * GetIconSmall() const;
    inline GPU2DBitmap * GetIconLarge() const;
    inline const Material * GetMaterial( BlockFace iFace ) const;
    inline GPUTexture2D * GetTexture( BlockFace iFace ) const;
    //inline GPUTexture2D * GetShadowMap( BlockFace iFace ) const;
    //inline GPUTexture2D * GetBumpMap( BlockFace iFace ) const;

    // Instanciation
    virtual BlockInstance * CreateInstance( BlockOrientation iOrientation ) const = 0;

protected:
    // Block data
    BlockID m_hID;
    GChar m_strName[BLOCK_NAME_LENGTH + 1];

    // Basic properties
    Bool m_bPassThrough; // Can we walk/fly through that block ?
    Bool m_bBreakable;   // Can we break that block ?
    Scalar m_fHardness;  // How hard is it to break ?
    Bool m_bHasGravity;  // Is it submitted to gravity ?
    Scalar m_fMass;      // How does it react to gravity ?
    Bool m_bIsLiquid;    // Is it a fluid we
    Bool m_bIsGas;       //   should spread ?
    Scalar m_fViscosity; // How does that fluid spread ?

    // Item drop properties
    //typedef struct _droptable_entry {
    //    Item * pItem;
    //    Scalar fDropRate;
    //    UInt iMinAmount;
    //    UInt iMaxAmount;
    //} _DropTableEntry;

    //UInt m_iDropTableSize;
    //_DropTableEntry m_arrDropTable[BLOCK_MAX_DROPS];

    // MicroBlocks support
    Bool m_bIsMicroBlock;       // Is it a microblock ?
    UInt m_iMinMicroBlockDepth; // How much depth do we need to represent it ?
    Point3 m_vMicroBlockSize;   // How large is it in the minimal microblock space ?

    // MultiBlocks support
    //MultiBlockPattern * m_pMultiBlockPattern; // Null if not a multiblock part !

    // I/O support
    //Bool m_bHasIOInterfaces; // Does that block supports I/O ?
    //IOInterface * m_arrFaceInterfaces[6];

    // Rendering data
    Bool m_bHasVisual; // Do we have anything to render ?
    GPU2DBitmap * m_pIconSmall;
    GPU2DBitmap * m_pIconLarge;
    Material m_arrMaterials[6];
    GPUTexture2D * m_arrTextures[6];
    //GPUTexture2D * m_arrShadowMaps[6];
    //GPUTexture2D * m_arrBumpMaps[6];
};

/////////////////////////////////////////////////////////////////////////////////
// The BlockInstance class
class BlockInstance
{
public:
    BlockInstance( const Block * pBlock, BlockOrientation iOrientation );
    virtual ~BlockInstance();

    // Block access
    inline const Block * GetBlock() const;

    // Orientation access
    inline BlockOrientation GetOrientation() const;
    inline BlockOrientation CycleOrientation();

protected:
    // Persistence support
    friend class BlockWorldManager;
    virtual Void _LoadState( const XMLNode * pState ) = 0;
    virtual Void _SaveState( XMLNode * pState ) const = 0;

    // Block data
    const Block * m_pBlock;

    // Instance data
    BlockOrientation m_iOrientation;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Block.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDCRAFT_GAMEPLAY_BLOCKS_BLOCK_H

