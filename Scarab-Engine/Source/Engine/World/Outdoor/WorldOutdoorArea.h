/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/Outdoor/WorldOutdoorArea.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Node : Outdoor, Area
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
#ifndef SCARAB_ENGINE_WORLD_OUTDOOR_WORLDOUTDOORAREA_H
#define SCARAB_ENGINE_WORLD_OUTDOOR_WORLDOUTDOORAREA_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Math/Types/Discrete/Point2.h"

#include "../Nodes/WorldNode.h"
#include "../Nodes/WorldLeaf.h"

#include "../WorldPathFinder.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define WORLD_OUTDOOR_SLOPE_UNWALKABLE 0.75f
#define WORLD_OUTDOOR_SLOPE_COST       10.0f

/////////////////////////////////////////////////////////////////////////////////
// The WorldOutdoorArea class
class WorldOutdoorArea : public WorldNode
{
public:
    WorldOutdoorArea( const Vertex2 & vOrigin, const Vector2 & vSize, UInt iGridWidth, UInt iGridHeight, const GChar * strName = NULL );
    virtual ~WorldOutdoorArea();

    // Type
    inline virtual WorldNodeType GetType() const;

    // Getters
	inline const Vertex2 & GetOrigin() const;
    inline const Vector2 & GetSize() const;

    // Height support
    virtual Scalar GetHeight( const Vertex2 & vPosition ) const = 0;

	// Update support
	virtual Void OnAreaUpdate( const Point2 & vAreaCoords ) = 0;

    // Path-Finding support
    inline WorldPathMicroNode * GetAttachedWaypoint();

    inline UInt GetGridWidth() const;
    inline UInt GetGridHeight() const;
    inline const Vector2 & GetGridCellSize() const;
    Bool GetGridPosition( UInt * outGridX, UInt * outGridY, const Vertex2 & vPosition ) const;

    inline Scalar GetGridCost( UInt iGridX, UInt iGridY ) const;
    inline Scalar GetGridCost( const Vertex2 & vPosition ) const;
    inline WorldPathNanoNode * GetGridWaypoint( UInt iGridX, UInt iGridY ) const;
    inline WorldPathNanoNode * GetGridWaypoint( const Vertex2 & vPosition ) const;

protected:
	Vertex2 m_vOrigin;
	Vector2 m_vSize;
    
    // Path-Finding support
    Void _PathFindingGraph_Initialize();
    Void _PathFindingGraph_Update(); // Call this last in OnAreaUpdate()

    WorldPathMicroNode m_hAttachedWaypoint;

    UInt m_iGridWidth;
    UInt m_iGridHeight;
    UInt m_iGridSize;
    Vector2 m_vGridCellSize;
    Scalar * m_arrCostGrid;
    WorldPathNanoNode * m_arrWaypointGrid;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "WorldOutdoorArea.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_WORLD_OUTDOOR_WORLDOUTDOORAREA_H

