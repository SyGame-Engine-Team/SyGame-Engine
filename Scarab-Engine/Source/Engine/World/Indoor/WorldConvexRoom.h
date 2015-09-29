/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/Indoor/WorldConvexRoom.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Graph : Interior, Convex Room
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : none
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_WORLD_INDOOR_WORLDCONVEXROOM_H
#define SCARAB_ENGINE_WORLD_INDOOR_WORLDCONVEXROOM_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Nodes/WorldNode.h"

#include "WorldPortal.h"
#include "WorldPortalCuller.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The WorldConvexRoom class
class WorldConvexRoom : public WorldNode
{
public:
    WorldConvexRoom( UInt iPortalCount, const GChar * strName = NULL );
    virtual ~WorldConvexRoom();

    // Type
    inline virtual WorldNodeType GetType() const;

    // Portals access
    inline UInt GetPortalCount() const;
    inline WorldPortal * GetPortal( UInt iPortal ) const;

    WorldPortal * CreatePortal( UInt iPortal, const Vertex3 * arrPolygon, UInt iPolygonSize );
    Void DestroyPortal( UInt iPortal );

    // Containment test
    virtual Bool Contains( const Vertex3 & vPoint ) const;

protected:
    // Give access to WorldPortal
    friend class WorldPortal;

    // Culling support
    virtual Void _OnVisibleGroup( WorldCuller & wCuller, Bool bNoCull );

private:
    // Traversal support
    Bool m_bVisited;

    // Portals
    UInt m_iPortalCount;
    WorldPortal ** m_arrPortals;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "WorldConvexRoom.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_WORLD_INDOOR_WORLDCONVEXROOM_H

