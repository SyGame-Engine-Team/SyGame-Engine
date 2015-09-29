/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/World/BlockWorld.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World base interface
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
#ifndef SCARAB_ENTRYPOINT_WORLDCRAFT_WORLD_BLOCKWORLD_H
#define SCARAB_ENTRYPOINT_WORLDCRAFT_WORLD_BLOCKWORLD_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Engine/Resource/XML/XML.h"

#include "../Gameplay/Blocks/BlockManager.h"

#include "Dimension.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define BlockWorldFn BlockWorldManager::GetInstancePtr()

#define BLOCKWORLD_SAVE_PATH TEXT("SavedData/Worlds")

// Region UIDs
struct RegionUID {
    RegionUID();
    RegionUID( const RegionUID & rhs );

    inline RegionUID & operator=( const RegionUID & rhs );

    inline Bool operator==( const RegionUID & rhs ) const;
    inline Bool operator!=( const RegionUID & rhs ) const;

    DimensionID iDimendionID;
    GalaxyID iGalaxyID;
    StarSystemID iStarSystemID;

    UInt iOrbit;

    RegionLayer iRegionLayer;
    MoonID iMoonID; // only for layer REGION_LAYER_MOONS

    RegionCoords vRegionCoords;
};

// Area UIDs
struct AreaUID {
    AreaUID();
    AreaUID( const AreaUID & rhs );

    inline AreaUID & operator=( const AreaUID & rhs );

    inline Bool operator==( const AreaUID & rhs ) const;
    inline Bool operator!=( const AreaUID & rhs ) const;

    RegionUID hRegionUID;
    AreaCoords vAreaCoords;
};

// BlockSpace UIDs
struct BlockSpaceUID {
    BlockSpaceUID();
    BlockSpaceUID( const BlockSpaceUID & rhs );

    inline BlockSpaceUID & operator=( const BlockSpaceUID & rhs );

    inline Bool operator==( const BlockSpaceUID & rhs ) const;
    inline Bool operator!=( const BlockSpaceUID & rhs ) const;

    AreaUID hAreaUID;
    BlockSpaceCoords vBlockSpaceCoords;
};

/////////////////////////////////////////////////////////////////////////////////
// The BlockWorldManager class
class BlockWorldManager : public Manager<BlockWorldManager>
{
public:
    inline static const GChar * GetName();
    inline static const GChar * GetMemoryBudget();

protected:
    friend class Manager<BlockWorldManager>;
    BlockWorldManager();
    virtual ~BlockWorldManager();

public:
    Void Initialize();
    Void Cleanup();

    // Dimensions data
    inline UInt GetDimensionCount() const;
    inline Dimension * GetDimension( DimensionID iDimensionID ) const;

    inline Void EnumDimensions();
    inline Dimension * EnumNextDimension();

    Dimension * CreateDimension( DimensionID iDimensionID );
    Void DestroyDimension( DimensionID iDimensionID );

    Void DestroyWorld();

    // Direct access
    Region * GetRegion( const RegionUID & hRegionUID ) const;
    inline Area * GetArea( const AreaUID & hAreaUID ) const;
    inline BlockSpace * GetBlockSpace( const BlockSpaceUID & hBlockSpaceUID ) const;

    Region * CreateRegion( const RegionUID & hRegionUID );
    inline Area * CreateArea( const AreaUID & hAreaUID );

    // Persistence support
    inline Void SetWorldName( const GChar * strName );

    Bool LoadArea( const AreaUID & hAreaUID );
    Bool LoadRegion( const RegionUID & hRegionUID );
    Bool LoadWorld();

    Bool SaveArea( const AreaUID & hAreaUID );
    Bool SaveRegion( const RegionUID & hRegionUID );
    Bool SaveWorld(); // First-time saves only, prefer synchronization afterward (much faster)

    inline Bool NeedSynchonization( const AreaUID & hAreaUID ) const;
    inline Bool NeedSynchonization( const RegionUID & hRegionUID ) const;
    inline Bool NeedSynchonization() const;

    inline Bool SynchronizeArea( const AreaUID & hAreaUID ); // <=> SaveArea, we don't go as far as block granularity
    Bool SynchronizeRegion( const RegionUID & hRegionUID );
    Bool SynchronizeWorld();

    inline Void RollbackArea( const AreaUID & hAreaUID );
    inline Void RollbackRegion( const RegionUID & hRegionUID );
    inline Void RollbackWorld();

    // Block operations
    //Void PlaceBlock( BlockInstance * pBlockInstance, const BlockSpaceUID & hBlockSpaceUID );
    //Void PlaceMicroBlock( BlockInstance * pBlockInstance, const BlockSpaceUID & hBlockSpaceUID, const MicroBlockCoords & vMicroBlockCoords );
    //BlockInstance * RemoveBlock( const BlockSpaceUID & hBlockSpaceUID );
    //BlockInstance * RemoveMicroBlock( const BlockSpaceUID & hBlockSpaceUID, const MicroBlockCoords & vMicroBlockCoords );

    //BlockOrientation RotateBlock( const BlockSpaceUID & hBlockSpaceUID );
    //BlockOrientation RotateMicroBlock( const BlockSpaceUID & hBlockSpaceUID, const MicroBlockCoords & vMicroBlockCoords );

    //Void ReadBlockData( Void * outBlockData, const BlockSpaceUID & hBlockSpaceUID ) const;
    //Void ReadMicroBlockData( Void * outMicroBlockData, const BlockSpaceUID & hBlockSpaceUID, const MicroBlockCoords & vMicroBlockCoords ) const;
    //Void WriteBlockData( const BlockSpaceUID & hBlockSpaceUID, Void * pBlockData );
    //Void WriteMicroBlockData( const BlockSpaceUID & hBlockSpaceUID, const MicroBlockCoords & vMicroBlockCoords, Void * pMicroBlockData );

    //Void SendBlockCommand( const BlockSpaceUID & hBlockSpaceUID, BlockCommandID iBlockCommandID, Void * pCommandData );
    //Void SendMicroBlockCommand( const BlockSpaceUID & hBlockSpaceUID, const MicroBlockCoords & vMicroBlockCoords, BlockCommandID iBlockCommandID, Void * pCommandData );

    // Update support
    Void RegisterUpdateArea( const AreaUID & hAreaUID );
    Void UnRegisterUpdateArea( const AreaUID & hAreaUID );

    Void UpdateWorld();

private:
    // Helpers
    inline static Int _Compare_DimensionID( const DimensionID & rLeft, const DimensionID & rRight, Void * pUserData );
    inline static Int _Compare_AreaCoords( const AreaCoords & rLeft, const AreaCoords & rRight, Void * pUserData );
    inline static Int _Compare_RegionUID( const RegionUID & rLeft, const RegionUID & rRight, Void * pUserData );

    // World structure
    typedef TreeMap<DimensionID,Dimension*> DimensionMap;
    DimensionMap m_mapDimensions;
    DimensionMap::Iterator m_itEnumerateDimension;

    // Persistence support
    inline GChar * _GetWorldPath( GChar * outPath ) const;
    inline GChar * _GetDimensionPath( GChar * outPath, DimensionID iDimensionID ) const;
    inline GChar * _GetGalaxyPath( GChar * outPath, GalaxyID iGalaxyID ) const;
    inline GChar * _GetStarSystemPath( GChar * outPath, StarSystemID iStarSystemID ) const;
    inline GChar * _GetPlanetPath( GChar * outPath, UInt iOrbit ) const;
    GChar * _GetRegionLayerPath( GChar * outPath, RegionLayer iRegionLayer, MoonID iMoonID ) const;
    GChar * _GetRegionLayerPath( GChar * outPath, const RegionUID & hRegionUID ) const;
    inline GChar * _GetRegionPath( GChar * outPath, const RegionCoords & vRegionCoords ) const;
    inline GChar * _GetRegionPath( GChar * outPath, const RegionUID & hRegionUID ) const;
    inline GChar * _GetAreaPath( GChar * outPath, const AreaCoords & vAreaCoords ) const;
    inline GChar * _GetAreaPath( GChar * outPath, const AreaUID & hAreaUID ) const;

    Bool _LoadDimension( Dimension * pDimension, GChar * strAbsolutePath, GChar * strPathPtr );
    Bool _LoadGalaxy( Galaxy * pGalaxy, GChar * strAbsolutePath, GChar * strPathPtr );
    Bool _LoadStarSystem( StarSystem * pStarSystem, GChar * strAbsolutePath, GChar * strPathPtr );
    Bool _LoadPlanet( Planet * pPlanet, GChar * strAbsolutePath, GChar * strPathPtr );
    Bool _LoadVoidSpace( VoidSpace * pVoidSpace, GChar * strAbsolutePath, GChar * strPathPtr );
    Bool _LoadRegionLayer( Planet * pPlanet, RegionLayer iRegionLayer, MoonID iMoonID, GChar * strAbsolutePath, GChar * strPathPtr );
    Bool _LoadRegion( Region * pRegion, GChar * strAbsolutePath, GChar * strPathPtr );
    Bool _LoadArea( Area * pArea, GChar * strAbsolutePath );

    Bool _SaveDimension( Dimension * pDimension, GChar * strAbsolutePath, GChar * strPathPtr );
    Bool _SaveGalaxy( Galaxy * pGalaxy, GChar * strAbsolutePath, GChar * strPathPtr );
    Bool _SaveStarSystem( StarSystem * pStarSystem, GChar * strAbsolutePath, GChar * strPathPtr );
    Bool _SavePlanet( Planet * pPlanet, GChar * strAbsolutePath, GChar * strPathPtr );
    Bool _SaveVoidSpace( VoidSpace * pVoidSpace, GChar * strAbsolutePath, GChar * strPathPtr );
    Bool _SaveRegionLayer( Planet * pPlanet, RegionLayer iRegionLayer, MoonID iMoonID, GChar * strAbsolutePath, GChar * strPathPtr );
    Bool _SaveRegion( Region * pRegion, GChar * strAbsolutePath, GChar * strPathPtr );
    Bool _SaveArea( Area * pArea, GChar * strAbsolutePath );

    Void _MarkDirtyArea( const AreaUID & hAreaUID );
    Void _ClearDirtyArea( const AreaUID & hAreaUID );
    Void _ClearDirtyRegion( const RegionUID & hRegionUID );
    Void _ClearDirtyRegions();

    typedef RedBlackTree<AreaCoords> DirtyAreaMap;
    typedef TreeMap<RegionUID,DirtyAreaMap*> DirtyRegionMap;
    DirtyRegionMap m_mapDirtyRegions;

    GChar m_strWorldName[FILENAME_LENGTH + 1];

    // Update support
    Array<BlockInstance*> m_arrUpdateList;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "BlockWorld.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDCRAFT_WORLD_BLOCKWORLD_H

