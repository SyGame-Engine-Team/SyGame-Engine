/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/Nodes/WorldNode.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Node : Internal nodes
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_WORLD_NODES_WORLDNODE_H
#define SCARAB_ENGINE_WORLD_NODES_WORLDNODE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Datastruct/Table/TreeMap.h"

#include "WorldBaseNode.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The WorldNode class
class WorldNode : public WorldBaseNode
{
public:
    WorldNode( const GChar * strName = NULL );
    virtual ~WorldNode();

    // Type
    inline virtual WorldNodeType GetType() const;

    // Children nodes access
    inline UInt GetChildCount() const;
    inline Bool HasChild( const GChar * strName ) const;
    inline WorldBaseNode * GetChild( const GChar * strName ) const;

    inline Void EnumChildren() const;
    inline WorldBaseNode * EnumNextChild() const;

    Bool AddChild( WorldBaseNode * pNode );
    WorldBaseNode * RemoveChild( const GChar * strName );
    Void RemoveAllChildren();

protected:
    // Update support
    virtual Void _UpdateWorldTransform( Scalar fTimeStep );
    virtual Void _UpdateWorldBV();

    // Culling support
    virtual Void _OnVisibleGroup( WorldCuller & wCuller, Bool bNoCull );

private:
    typedef TreeMap<const GChar *, WorldBaseNode*> ChildMap;
    inline static Int _Name_Comparator( const GChar* const & rLeft, const GChar* const & rRight, Void * pUserData );

    ChildMap m_mapChildren;
    mutable ChildMap::Iterator m_itEnumerate;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "WorldNode.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_WORLD_NODES_WORLDNODE_H

