/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/WorldCaster.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Graph : Casting operations
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
#ifndef SCARAB_ENGINE_WORLD_WORLDCASTER_H
#define SCARAB_ENGINE_WORLD_WORLDCASTER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Lib/Datastruct/Array/Array.h"

#include "../../Lib/Math/Geometry/Intersection/3D/Planar/IntersectTriangle3Line3.h"

#include "Nodes/WorldLeaf.h"
#include "Container/WorldSwitchNode.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define WORLDCASTER_RESULTS_PAGESIZE 64

typedef struct _ray_cast_result
{
    WorldLeaf * pHit;
    Scalar fDistance;
    UInt iTriangleIndex;
    Vertex3 vTriangleParameters; // barycentric coordinates
                                 // relative to the world
    // Sorting
    inline Bool operator==( const struct _ray_cast_result & rhs ) const;
    inline Bool operator!=( const struct _ray_cast_result & rhs ) const;
    inline Bool operator<( const struct _ray_cast_result & rhs ) const;
    inline Bool operator<=( const struct _ray_cast_result & rhs ) const;
    inline Bool operator>( const struct _ray_cast_result & rhs ) const;
    inline Bool operator>=( const struct _ray_cast_result & rhs ) const;
} RayCastResult;
typedef struct _sphere_cast_result
{
    WorldLeaf * pHit;
    Scalar fDistance;

    // Sorting
    inline Bool operator==( const struct _sphere_cast_result & rhs ) const;
    inline Bool operator!=( const struct _sphere_cast_result & rhs ) const;
    inline Bool operator<( const struct _sphere_cast_result & rhs ) const;
    inline Bool operator<=( const struct _sphere_cast_result & rhs ) const;
    inline Bool operator>( const struct _sphere_cast_result & rhs ) const;
    inline Bool operator>=( const struct _sphere_cast_result & rhs ) const;
} SphereCastResult;

/////////////////////////////////////////////////////////////////////////////////
// The WorldCaster class
class WorldCaster
{
public:
    WorldCaster();
    ~WorldCaster();

    // Ray-Casting
    Void RayCast( WorldBaseNode * pWorldGraph, const Line3 & vRay, Scalar fMinDistance, Scalar fMaxDistance );

    inline UInt GetRayCastResultCount() const;
    inline const RayCastResult * GetRayCastResult( UInt iResult ) const;

    const RayCastResult * GetClosestRayCastResult();
    const RayCastResult * GetClosestPositiveRayCastResult();
    const RayCastResult * GetClosestNegativeRayCastResult();
    Void SortRayCastResults();

    // Sphere-Casting
    Void SphereCast( WorldBaseNode * pWorldGraph, const Sphere & vSphere );

    inline UInt GetSphereCastResultCount() const;
    inline const SphereCastResult * GetSphereCastResult( UInt iResult ) const;

    const SphereCastResult * GetClosestSphereCastResult();
    Void SortSphereCastResults();

private:
    // Helpers
    inline static Int _ResultOrdering_RayCast( const RayCastResult & rLeft, const RayCastResult & rRight );
    inline static Int _ResultOrdering_SphereCast( const SphereCastResult & rLeft, const SphereCastResult & rRight );

    Void _Rec_RayCast( WorldBaseNode * pNode );
    Void _Rec_SphereCast( WorldBaseNode * pNode );

    // Ray-Casting
    Line3 m_vRay;
    Scalar m_fRayMinDistance;
    Scalar m_fRayMaxDistance;

    UInt m_iClosestRayCastResult;
    UInt m_iClosestPositiveRayCastResult, m_iClosestNegativeRayCastResult;
    Bool m_bSortedRayCastResults;
    Array<RayCastResult> m_arrRayCastResults;

    // Sphere-Casting
    Sphere m_vSphere;

    UInt m_iClosestSphereCastResult;
    Bool m_bSortedSphereCastResults;
    Array<SphereCastResult> m_arrSphereCastResults;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "WorldCaster.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_WORLD_WORLDCASTER_H

