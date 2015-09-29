/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/World/WorldVisibility.h
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
// Header prelude
#ifndef SCARAB_ENTRYPOINT_WORLDCRAFT_WORLD_WORLDVISIBILITY_H
#define SCARAB_ENTRYPOINT_WORLDCRAFT_WORLD_WORLDVISIBILITY_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Engine/World/WorldManager.h"

#include "BlockWorld.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The BlockLeaf class
class BlockLeaf : public WorldLeaf
{
public:
    BlockLeaf( BlockInstance * pBlockInstance, const BlockSpaceCoords & vBlockSpaceCoords );
    virtual ~BlockLeaf();

private:
};

/////////////////////////////////////////////////////////////////////////////////
// The MicroBlockLeaf class
class MicroBlockLeaf : public WorldLeaf
{
public:
    MicroBlockLeaf( BlockInstance * pBlockInstance, const BlockSpaceCoords & vBlockSpaceCoords, const MicroBlockCoords & vMicroBlockCoords );
    virtual ~MicroBlockLeaf();

private:
};

/////////////////////////////////////////////////////////////////////////////////
// The BlockSpaceNode class
class BlockSpaceNode : public WorldNode
{
public:
    BlockSpaceNode( BlockSpace * pBlockSpace, const BlockSpaceCoords & vBlockSpaceCoords );
    virtual ~BlockSpaceNode();

private:
};

/////////////////////////////////////////////////////////////////////////////////
// The BlockWorldArea class
class BlockWorldArea : public WorldOutdoorArea
{
public:
    BlockWorldArea( const Vertex2 & vOrigin, const GChar * strName = NULL );
    virtual ~BlockWorldArea();

    // Height support
    virtual Scalar GetHeight( const Vertex2 & vPosition ) const;

    // Update methods
	virtual Void OnAreaUpdate( const Point2 & vAreaCoords );

private:
    // Helpers
    Area * _QueryArea( const AreaUID & hAreaUID ) const;
    Void _BuildBSPTree( WorldNode ** arrLayer, Axis iAxis, UInt iSpanX, UInt iSpanY, UInt iSpanZ );

    // Area
    Area * m_pVisibleArea;
};

/////////////////////////////////////////////////////////////////////////////////
// The BlockWorldRegion class
class BlockWorldRegion : public WorldOutdoor
{
public:
    BlockWorldRegion( const RegionUID & hRegionUID, UInt iViewDistance, const GChar * strName = NULL );
    virtual ~BlockWorldRegion();

    // Getters
    inline const RegionUID & GetRegionUID() const;

protected:
    // Custom regions support
    virtual WorldOutdoorArea * _CreateArea( const Vertex2 & vOrigin, const Vector2 & vSize, const GChar * strName = NULL );

private:
    // Helpers
    inline static Vertex2 _GetOrigin( const RegionCoords & vRegionCoords );

    RegionUID m_hRegionUID;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "WorldVisibility.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDCRAFT_WORLD_WORLDVISIBILITY_H

