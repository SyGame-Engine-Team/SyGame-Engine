/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/Nodes/WorldBaseNode.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Base Node class for the WorldGraph manager
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : Very rare & subtle situation here using protected methods of
//              WorldBaseNode in a derived class like WorldNode.
//              Calling protected methods of a WorldBaseNode object from WorldNode
//              is not legal.
//              As an example, if a WorldBaseNode * pNode object calls _SetParent
//              inside a WorldNode's method, WorldNode has protected access but
//              pNode may not be a WorldNode ... say a WorldLeaf. In this case,
//              the real class of pNode is WorldLeaf which is a foreign class from
//              WorldNode's point of view.
//              In particular, if WorldLeaf was using private derivation, a violation
//              would occur. Typecasting only doesn't work here since the compiler
//              doesn't even investigate WorldLeaf to check protected access validity.
//              Calling protected from a WorldNode on a WorldLeaf object will allways
//              be forbidden. The solutions are reduced to using public access or
//              abusing friend declarations ...
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_WORLD_NODES_WORLDBASENODE_H
#define SCARAB_ENGINE_WORLD_NODES_WORLDBASENODE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Math/Geometry/BoundingShapes/3D/BVAABox.h"

//#include "../../Physics/.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
    // Node names
#define WORLDGRAPH_NAME_LENGTH 64

    // Bounding volume for world hierarchy
typedef BVAABox WorldGraphBV;

    // Node types
enum WorldNodeType
{
    WORLDNODE_UNDEFINED         = 0xffffffff,

    // Node & Leaf types
    WORLDNODE_LEAF              = 0x00000000,
    WORLDNODE_NODE              = 0x80000000,

    // Container nodes
    WORLDNODE_BSP               = 0x80000001,
    WORLDNODE_SWITCH            = 0x80000002,
    WORLDNODE_LIGHT             = 0x00000003,

    // Outdoor nodes
    WORLDNODE_OUTDOOR_AREA      = 0x80000004,
    WORLDNODE_OUTDOOR           = 0x80000005,

    // Indoor nodes
    WORLDNODE_INDOOR_CONVEXROOM = 0x80000006,
    WORLDNODE_INDOOR_AREA       = 0x80000007,

    WORLDNODE_COUNT
};

    // Culling flags
enum WorldCulling
{
    WORLD_CULL_ALLWAYS = 0,
    WORLD_CULL_NEVER,
    WORLD_CULL_DYNAMIC
};

    // Prototypes
class WorldCuller;

class WorldPathWaypoint;

/////////////////////////////////////////////////////////////////////////////////
// The WorldBaseNode interface
class WorldBaseNode
{
public:
    WorldBaseNode( const GChar * strName = NULL );
    virtual ~WorldBaseNode();

    // Type
    virtual WorldNodeType GetType() const = 0;
    inline Bool IsLeaf() const;
    inline Bool IsNode() const;

    // Name
    inline const GChar * GetName() const;

    // Parent Node access
    inline WorldBaseNode * GetParent() const;

    // World Transform
    Transform3 WorldTransform;
    Bool UpdateWorldTransform;

    // World BV
    WorldGraphBV WorldBV;
    Bool UpdateWorldBV;

    // Culling mode
    WorldCulling CullMode;

    // Debug draw support
    inline DWord GetDebugFlags() const;
    inline Void SetDebugFlags( DWord dwDebugFlags );

    // Animation support
    Void Update( Scalar fTimeStep );

protected:
    // Needed, see notice above
    friend class WorldNode;
    friend class WorldSwitchNode;
    friend class WorldBSPNode;
    friend class WorldIndoorArea;

    // Update support
    virtual Void _UpdateWorldTransform( Scalar fTimeStep ) = 0;
    virtual Void _UpdateWorldBV() = 0;

    // Culling support
    friend class WorldCuller;
    Void _QueryVisibleGroup( WorldCuller & wCuller, Bool bNoCull );
    virtual Void _OnVisibleGroup( WorldCuller & wCuller, Bool bNoCull ) = 0;    

    // Parent link
    WorldBaseNode * m_pParent;

private:
    // Name support
    static UInt sm_iNoNameCount;
    GChar m_strName[WORLDGRAPH_NAME_LENGTH];

    // Debug draw support
    DWord m_dwDebugFlags;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "WorldBaseNode.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_WORLD_NODES_WORLDBASENODE_H

