/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/Container/WorldSwitchNode.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Container : Switch nodes
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
#ifndef SCARAB_ENGINE_WORLD_CONTAINER_WORLDSWITCHNODE_H
#define SCARAB_ENGINE_WORLD_CONTAINER_WORLDSWITCHNODE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Nodes/WorldNode.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The WorldSwitchNode class
class WorldSwitchNode : public WorldNode
{
public:
    WorldSwitchNode( const GChar * strName = NULL );
    virtual ~WorldSwitchNode();

    // Type
    inline virtual WorldNodeType GetType() const;

    // Switch access
    inline const GChar * GetActiveChild() const;
    inline Void SetActiveChild( const GChar * strName );
    inline Void Disable();

protected:
    // Culling support
    virtual Void _OnVisibleGroup( WorldCuller & wCuller, Bool bNoCull );

private:
    const GChar * m_strActiveChild;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "WorldSwitchNode.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_WORLD_CONTAINER_WORLDSWITCHNODE_H

