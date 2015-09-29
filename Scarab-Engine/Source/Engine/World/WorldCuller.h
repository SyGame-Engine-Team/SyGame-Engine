/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/WorldCuller.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Graph : Culling operations
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
#ifndef SCARAB_ENGINE_WORLD_WORLDCULLER_H
#define SCARAB_ENGINE_WORLD_WORLDCULLER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Lib/Datastruct/Array/Array.h"

#include "../Rendering/Resources/Camera.h"

#include "Nodes/WorldBaseNode.h"
#include "Nodes/WorldLeaf.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define VISIBLEGROUP_PAGESIZE 128

#define WORLDCULLER_MAX_USERPLANES (32 - FRUSTRUM_COUNT)

/////////////////////////////////////////////////////////////////////////////////
// The VisibleGroup class
class VisibleGroup
{
public:
    VisibleGroup();
    ~VisibleGroup();

    inline UInt GetCount() const;
    inline WorldLeaf * GetLeaf( UInt iIndex ) const;

    inline Void Insert( WorldLeaf * pLeaf );
    inline Void Clear();

private:
    Array<WorldLeaf*> m_arrLeaves;
};

/////////////////////////////////////////////////////////////////////////////////
// The WorldCuller class
class WorldCuller
{
public:
    WorldCuller();
    virtual ~WorldCuller();

    // Camera access
    inline const Camera * GetCamera() const;
    Void SetCamera( const Camera * pCamera, Bool bSetFrustrum );

    Void UpdateCamera();

    // Frustrum access
    Void GetFrustrumState( Scalar * outFrustrum ) const;
    Void SetFrustrumState( const Scalar * pFrustrum );

    inline const Plane & GetFrustrumPlane( UInt iFrustrumPlane ) const;
    inline const Vertex3 & GetFrustrumVertex( UInt iFrustrumVertex ) const;
    inline const Vertex3 & GetFrustrumVertex( Bool bFar, Bool bTop, Bool bLeft ) const;

    // User planes access
    inline UInt GetUserPlaneCount() const;
    inline const Plane & GetUserPlane( UInt iUserPlane ) const;

    inline Void PushUserPlane( const Plane & newPlane );
    inline Void PopUserPlane();

    // Plane state access
    inline DWord GetPlaneState() const;
    inline Void SetPlaneState( DWord dwPlaneState );

    // Culling methods
    Bool IsVisible( const BoundingVolume & testBV );
    Bool IsVisible( const Vertex3 * arrPolygon, UInt iPolygonSize, Bool bIgnoreNearPlane = false ) const;
    Int WhichSide( const Plane & testPlane ) const;

    Void ComputeVisibleGroup( WorldBaseNode * pWorldGraph );

    inline const VisibleGroup & GetVisibleGroup() const;
    virtual Void OnVisibleNode( WorldLeaf * pVisibleLeaf );

private:
    // Camera & frustrum
    const Camera * m_pCamera;
    Scalar m_arrFrustrumState[FRUSTRUM_COUNT];
    Plane m_arrFrustrumPlanes[FRUSTRUM_COUNT];
    Vertex3 m_arrFrustrumVertices[8];

    // User planes
    UInt m_iUserPlaneCount;
    Plane m_arrUserPlanes[WORLDCULLER_MAX_USERPLANES];

    // Plane state
    DWord m_dwPlaneState;

    // Current visible group being computed
    VisibleGroup m_curVisibleGroup;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "WorldCuller.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_WORLD_WORLDCULLER_H

