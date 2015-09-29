/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/WorldPathFinder.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Graph : PathFinding operations
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : Triple-Tiered incremental implementation of the A* algorithm
//              using mapped heaps, integer encoding and parallel searches ...
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_WORLD_WORLDPATHFINDER_H
#define SCARAB_ENGINE_WORLD_WORLDPATHFINDER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Lib/Datastruct/Array/Array.h"
#include "../../Lib/Datastruct/Table/TreeMap.h"

#include "../../Lib/Math/Functions/MathFunction.h"
#include "../../Lib/Math/Types/Vector/Vector3.h"
#include "../../Lib/Math/Types/Vertex/Vertex3.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Adjacency limits
#define WORLD_PATHFINDER_MAX_ADJACENCY_MACRO 26 // 3D Englobing Box, full adjacency
#define WORLD_PATHFINDER_MAX_ADJACENCY_MICRO 26 // 3D Englobing Box, full adjacency
#define WORLD_PATHFINDER_MAX_ADJACENCY_NANO  26 // 3D Englobing Box, full adjacency

// Waypoint costs / Heuristics
#define WORLD_PATHFINDER_COST_INFINITE SCALAR_INFINITE

enum PathFindHeuristic {
    PATHFIND_HEURISTIC_DISTANCE = 0,
    PATHFIND_HEURISTIC_DISTANCE_SQR,
    PATHFIND_HEURISTIC_DISTANCE_MANHATTAN,

    PATHFIND_HEURISTIC_COUNT
};

// Parallelization
#define WORLD_PATHFINDER_SEARCH_WARMUP 32 // Iterations allowed for searches upon start. Gives some insight, avoiding
                                          // computing path that could get obsolete and allowing early movement.

#define WORLD_PATHFINDER_SUBSEARCH_DELAY  32 // Only begin path's sub-searches after walking that much nodes.
                                             // You should setup initial iterations such that this can be covered
                                             // in a single initial update so you can start moving directly ...
                                             // You need to cover the delay buffer for at least the lowest
                                             // hierarchic level you use.
#define WORLD_PATHFINDER_MAX_SUB_SEARCHES 16 // Max parallel sub-searches to run for a single path

// Waypoint states
class WorldPathWaypoint;

typedef struct _waypoint_state {
    inline struct _waypoint_state & operator=( const struct _waypoint_state & rhs );

    WorldPathWaypoint * pWaypoint;

    // List tracking
    UInt iHeapNode;
    Bool bInOpenList;
    Bool bInClosedList;

    // Path tracking
    struct _waypoint_state * pPathParent;

    // Computed Costs
    Scalar fCurrentCost;   // = pParent->fCurrentCost + this->Cost
    Scalar fEstimatedCost; // = m_pfHeuristicCost( this, pTarget )
    Scalar fPathCost;      // = fCurrentCost + fEstimatedCost
} WaypointState;

// Search instances
typedef UInt SearchInstanceID;

enum PathFindType {
    PATHFIND_TYPE_SIMPLE       = 0x00000000,
    PATHFIND_TYPE_HIERARCHIC_2 = 0x80000000,
    PATHFIND_TYPE_HIERARCHIC_3 = 0x40000000
};

// Prototypes
class WorldPathMacroNode;
class WorldPathMicroNode;
class WorldPathNanoNode;

/////////////////////////////////////////////////////////////////////////////////
// The WorldPathWaypoint class
class WorldPathWaypoint
{
public:
    WorldPathWaypoint();
    virtual ~WorldPathWaypoint();

    // Getters
    virtual Bool IsMacroNode() const = 0;
    virtual Bool IsMicroNode() const = 0;
    virtual Bool IsNanoNode() const = 0;

    inline const Vertex3 & GetPosition() const;
    inline Void SetPosition( const Vertex3 & vPosition );

    inline Scalar GetCost() const;
    inline Void SetCost( Scalar fCost );

    // Adjacency access
    virtual UInt GetAdjacencyCount() const = 0;
    virtual WorldPathWaypoint * GetAdjacentWaypoint( UInt iIndex ) const = 0;

    virtual Bool HasAdjacentWaypoint( WorldPathWaypoint * pAdjacentWaypoint ) const = 0;
    virtual Void AddAdjacentWaypoint( WorldPathWaypoint * pAdjacentWaypoint ) = 0;
    virtual Void ClearAdjacency() = 0;

    // Hierarchic access
    virtual WorldPathWaypoint * GetGridParent() const = 0;
    virtual WorldPathWaypoint * GetGridRoot() const = 0;

protected:
    // Position
    Vertex3 m_vPosition;

    // Cost
    Scalar m_fCost;
};

/////////////////////////////////////////////////////////////////////////////////
// The WorldPathMacroNode class
class WorldPathMacroNode : public WorldPathWaypoint
{
public:
    WorldPathMacroNode();
    virtual ~WorldPathMacroNode();

    // Getters
    inline virtual Bool IsMacroNode() const;
    inline virtual Bool IsMicroNode() const;
    inline virtual Bool IsNanoNode() const;

    // Adjacency access
    inline virtual UInt GetAdjacencyCount() const;
    inline virtual WorldPathWaypoint * GetAdjacentWaypoint( UInt iIndex ) const;

    virtual Bool HasAdjacentWaypoint( WorldPathWaypoint * pAdjacentWaypoint ) const;
    inline virtual Void AddAdjacentWaypoint( WorldPathWaypoint * pAdjacentWaypoint );
    inline virtual Void ClearAdjacency();

    // Hierarchic access
    inline virtual WorldPathWaypoint * GetGridParent() const;
    inline virtual WorldPathWaypoint * GetGridRoot() const;

    inline Void SetGridRoot( WorldPathMicroNode * pGridRoot );

protected:
    // Adjacency
    UInt m_iAdjacencyCount;
    WorldPathMacroNode * m_arrAdjacentWaypoints[WORLD_PATHFINDER_MAX_ADJACENCY_MACRO];

    // Lower-Tier Grid
    WorldPathMicroNode * m_pGridRoot;
};

/////////////////////////////////////////////////////////////////////////////////
// The WorldPathMicroNode class
class WorldPathMicroNode : public WorldPathWaypoint
{
public:
    WorldPathMicroNode();
    virtual ~WorldPathMicroNode();

    // Getters
    inline virtual Bool IsMacroNode() const;
    inline virtual Bool IsMicroNode() const;
    inline virtual Bool IsNanoNode() const;

    // Adjacency access
    inline virtual UInt GetAdjacencyCount() const;
    inline virtual WorldPathWaypoint * GetAdjacentWaypoint( UInt iIndex ) const;

    virtual Bool HasAdjacentWaypoint( WorldPathWaypoint * pAdjacentWaypoint ) const;
    inline virtual Void AddAdjacentWaypoint( WorldPathWaypoint * pAdjacentWaypoint );
    inline virtual Void ClearAdjacency();

    // Hierarchic access
    inline virtual WorldPathWaypoint * GetGridParent() const;
    inline virtual WorldPathWaypoint * GetGridRoot() const;

    inline Void SetGridParent( WorldPathMacroNode * pGridParent );
    inline Void SetGridRoot( WorldPathNanoNode * pGridRoot );

protected:
    // Adjacency
    UInt m_iAdjacencyCount;
    WorldPathMicroNode * m_arrAdjacentWaypoints[WORLD_PATHFINDER_MAX_ADJACENCY_MICRO];

    // Upper-Tier Parent
    WorldPathMacroNode * m_pGridParent;

    // Lower-Tier Grid
    WorldPathNanoNode * m_pGridRoot;
};

/////////////////////////////////////////////////////////////////////////////////
// The WorldPathNanoNode class
class WorldPathNanoNode : public WorldPathWaypoint
{
public:
    WorldPathNanoNode();
    virtual ~WorldPathNanoNode();

    // Getters
    inline virtual Bool IsMacroNode() const;
    inline virtual Bool IsMicroNode() const;
    inline virtual Bool IsNanoNode() const;

    // Adjacency access
    inline virtual UInt GetAdjacencyCount() const;
    inline virtual WorldPathWaypoint * GetAdjacentWaypoint( UInt iIndex ) const;

    virtual Bool HasAdjacentWaypoint( WorldPathWaypoint * pAdjacentWaypoint ) const;
    inline virtual Void AddAdjacentWaypoint( WorldPathWaypoint * pAdjacentWaypoint );
    inline virtual Void ClearAdjacency();

    // Hierarchic access
    inline virtual WorldPathWaypoint * GetGridParent() const;
    inline virtual WorldPathWaypoint * GetGridRoot() const;

    inline Void SetGridParent( WorldPathMicroNode * pGridParent );

protected:
    // Adjacency
    UInt m_iAdjacencyCount;
    WorldPathNanoNode * m_arrAdjacentWaypoints[WORLD_PATHFINDER_MAX_ADJACENCY_NANO];

    // Upper-Tier Parent
    WorldPathMicroNode * m_pGridParent;
};

/////////////////////////////////////////////////////////////////////////////////
// The WaypointHeap class
class WaypointHeap
{
public:
    WaypointHeap();
    ~WaypointHeap();

    // Getters
    inline Bool IsEmpty() const;
    inline UInt Count() const;

    // Methods
    Void Create();
    Void Destroy();

    Void Merge( WaypointState * pWaypointState );
    WaypointState * Extract( Bool bRemove );
    Void UpdateUp( WaypointState * pWaypointState );

private:
    // Helpers
    inline static Int _Compare( const WaypointState * pLeft, const WaypointState * pRight );

    // Array-based heap
    Void _Heap_UpdateUp( UInt iNode );
    Void _Heap_UpdateDown( UInt iNode );

    Array<WaypointState*> m_arrNodes;
};

/////////////////////////////////////////////////////////////////////////////////
// The WorldPathFinder class
class WorldPathFinder
{
public:
    WorldPathFinder();
    ~WorldPathFinder();

    // Getters
    Bool IsFinished( SearchInstanceID idSearch ) const;
    Bool IsPathFound( SearchInstanceID idSearch ) const;
    UInt GetIterationCount( SearchInstanceID idSearch ) const;
    
    WorldPathWaypoint * GetStart( SearchInstanceID idSearch ) const;
    WorldPathWaypoint * GetTarget( SearchInstanceID idSearch ) const;

    //  Heuristic cost functions
    inline Void SetHeuristicCostFunction( PathFindHeuristic idHeuristic );

    // Main routines
    SearchInstanceID StartSearch( WorldPathWaypoint * pStart, WorldPathWaypoint * pTarget, PathFindType iType );
    Void EndSearch( SearchInstanceID idSearch );
    Bool UpdateSearch( SearchInstanceID idSearch, UInt iIterationCount );
    Bool GetPath( SearchInstanceID idSearch, const Array<WorldPathWaypoint*> ** outPath );

private:
    // Hierarchic parallel searching system
    SearchInstanceID _SimpleSearch_Start( WorldPathWaypoint * pStart, WorldPathWaypoint * pTarget );
    Void _SimpleSearch_End( SearchInstanceID idSearch );
    Bool _SimpleSearch_Update( SearchInstanceID idSearch, UInt iIterationCount );
    Void _SimpleSearch_BuildPath( SearchInstanceID idSearch );

    SearchInstanceID _Hierarchic2Search_Start( WorldPathWaypoint * pStart, WorldPathWaypoint * pTarget );
    Void _Hierarchic2Search_End( SearchInstanceID idSearch );
    Bool _Hierarchic2Search_Update( SearchInstanceID idSearch, UInt iIterationCount );
    Void _Hierarchic2Search_BuildPath( SearchInstanceID idSearch );

    SearchInstanceID _Hierarchic3Search_Start( WorldPathWaypoint * pStart, WorldPathWaypoint * pTarget );
    Void _Hierarchic3Search_End( SearchInstanceID idSearch );
    Bool _Hierarchic3Search_Update( SearchInstanceID idSearch, UInt iIterationCount );
    Void _Hierarchic3Search_BuildPath( SearchInstanceID idSearch );

    // Waypoint mapped heaps
    inline static Int _Compare_Waypoints( WorldPathWaypoint * const & rLeft, WorldPathWaypoint * const & rRight, Void * pUserData );

    typedef TreeMap<WorldPathWaypoint*,WaypointState> _WaypointMap;

    // Search instances
    typedef struct _search_instance {
        inline struct _search_instance & operator=( const struct _search_instance & rhs );

        SearchInstanceID idSearch;
        UInt iIterationCount;
        Bool bFinished, bPathFound;

        WorldPathWaypoint * pStart;
        WorldPathWaypoint * pTarget;

        _WaypointMap hExploreMap;
        WaypointHeap hOpenList;
        WaypointHeap hClosedList;

        Array<WorldPathWaypoint*> arrPath;
    } _SearchInstance;

    typedef struct _hierarchicsearch_instance {
        inline struct _hierarchicsearch_instance & operator=( const struct _hierarchicsearch_instance & rhs );

        PathFindType iType;

        SearchInstanceID idSearch;
        UInt iIterationCount;
        Bool bFinished, bPathFound;
        Bool bBiasFirst, bBiasLast;

        WorldPathWaypoint * pStart;
        WorldPathWaypoint * pTarget;

        SearchInstanceID idParentSearch;
        Array<SearchInstanceID> arrSubSearches; // index i => path between nodes (i-1) and (i)

        Array<WorldPathWaypoint*> arrPath;
    } _HierarchicSearchInstance;

    inline Bool _SearchInstance_IsAllocated( SearchInstanceID idSearch ) const;

    Void _SearchInstance_Init();
    SearchInstanceID _SearchInstance_Alloc();
    Void _SearchInstance_Free( SearchInstanceID idSearch );

    inline Bool _HierarchicSearchInstance_IsAllocated( SearchInstanceID idSearch ) const;

    Void _HierarchicSearchInstance_Init();
    SearchInstanceID _HierarchicSearchInstance_Alloc();
    Void _HierarchicSearchInstance_Free( SearchInstanceID idSearch );

    UInt m_iSearchInstanceCount;
    UInt m_iNextFreeSearchInstance;
    Array<_SearchInstance> m_arrSearchInstances;

    UInt m_iHierarchicSearchInstanceCount;
    UInt m_iNextFreeHierarchicSearchInstance;
    Array<_HierarchicSearchInstance> m_arrHierarchicSearchInstances;

    // Heuristic cost function
    inline static Scalar _Heuristic_Distance( WorldPathWaypoint * pCurrent, WorldPathWaypoint * pTarget );
    inline static Scalar _Heuristic_DistanceSquared( WorldPathWaypoint * pCurrent, WorldPathWaypoint * pTarget );
    inline static Scalar _Heuristic_DistanceManhattan( WorldPathWaypoint * pCurrent, WorldPathWaypoint * pTarget );

    typedef Scalar (*PathFinderHeuristicFunction)( WorldPathWaypoint *, WorldPathWaypoint * );
    static PathFinderHeuristicFunction m_arrHeuristics[PATHFIND_HEURISTIC_COUNT];

    PathFinderHeuristicFunction m_pfHeuristicCost;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "WorldPathFinder.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_WORLD_WORLDPATHFINDER_H

