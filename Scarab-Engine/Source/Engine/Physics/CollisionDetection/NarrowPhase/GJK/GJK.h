/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/CollisionDetection/NarrowPhase/GJK/GJK.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Narrow-Phase : Convex, Gilbert-Johnson-Keerti algorithm coupled
//               with Expanding-Polytope-Algorithm or Minkowski-Portal-Refinement
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : GJK-EPA is hard to make stable but is probably the best algorithm
//              regarding performance and consistence.
//              MPR is also a good one ...
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_NARROWPHASE_SAT_SAT_H
#define SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_NARROWPHASE_SAT_SAT_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Shape/TransformedShape.h"

#include "../NarrowPhase.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
    // GJK
#define GJK_MAX_ITERATIONS 128
#define GJK_ACCURACY       1.0e-4f
#define GJK_MIN_DISTANCE   1.0e-4f
#define GJK_EPSILON_DUP    1.0e-4f
#define GJK_EPSILON_S2     0.0f
#define GJK_EPSILON_S3     0.0f
#define GJK_EPSILON_S4     0.0f

enum GJKState {
    GJKSTATE_VALID = 0,
    GJKSTATE_INSIDE,
    GJKSTATE_FAILED
};

    // EPA
#define EPA_MAX_ITERATIONS 255
#define EPA_MAX_VERTICES   64
#define EPA_MAX_FACETS     (EPA_MAX_VERTICES << 1)
#define EPA_ACCURACY       1.0e-4f
#define EPA_EPSILON        1.0e-5f

enum EPAState {
    EPASTATE_VALID = 0,
    EPASTATE_TOUCHING,
    EPASTATE_DEGENERATED,
    EPASTATE_NON_CONVEX,
    EPASTATE_INVALID_HULL,
    EPASTATE_ACCURACY_REACHED,
    EPASTATE_FALLBACK,
    EPASTATE_FAILED
};

    // MPR
#define MPR_MAX_ITERATIONS 1000
#define MPR_ACCURACY       1.0e-6f

/////////////////////////////////////////////////////////////////////////////////
// The GJK class
class GJK : public NarrowPhaseConvex
{
public:
    GJK( TransformedShapeConvex * pConvexA, TransformedShapeConvex * pConvexB );
    virtual ~GJK();

    // Narrow Collision algorithm
    virtual Void Initialize();
    virtual Bool DetectCollision( Bool bGenerateContactPoints );

private:
    // SV/SF allocators
    template<typename T, UInt SIZE> struct _PoolAllocator {
        inline _PoolAllocator();
        inline Void _Reset();

        inline T * _Allocate();
        inline Void _Free( T * pPtr );

        UInt iNextFree, iPoolCount;
        T arrPool[SIZE];
    };

    // Support vertices
    typedef struct _support_vertex {
        inline struct _support_vertex & operator=( const struct _support_vertex & rhs );

        Vector3 vDir, vSupport;
    } _SupportVertex;
    
    inline Void _GetSupportA( Vector3 * outSupport, const Vector3 & vDir ) const;
    inline Void _GetSupportB( Vector3 * outSupport, const Vector3 & vDir ) const;
    inline Void _GetSupportVertex( _SupportVertex * outSV, const Vector3 & vDir ) const;

    typedef struct _mpr_support_vertex {
        inline struct _mpr_support_vertex & operator=( const struct _mpr_support_vertex & rhs );

        Vector3 vPointM, vPointA, vPointB;
    } _MPRSupportVertex;

    inline Bool _MPR_CanContainOrigin( const _MPRSupportVertex * pSupport, const Vector3 & vDir ) const;
    inline Void _MPR_GetOrigin( _MPRSupportVertex * outOrigin ) const;
    inline Void _MPR_GetSupport( _MPRSupportVertex * outSupport, const Vector3 & vDir ) const;

    // Simplices
    typedef struct _simplex {
        inline Void _Initialize();

        UInt iRank;
        _SupportVertex * arrVertices[4];
        Scalar arrWeights[4];
    } _Simplex;

    inline Void _PushVertex( _Simplex * pSimplex, const Vector3 & vDir );
    inline Void _PopVertex( _Simplex * pSimplex );

    typedef struct _mpr_simplex {
        inline Void _Initialize();

        inline Void SwapSupports( UInt iIndex1, UInt iIndex2 );

        inline Void GetPortalDir( Vector3 * outPortalDir ) const;
        inline Bool ContainsOrigin( const Vector3 & vPortalDir ) const;
        inline Bool TestTolerance( const _MPRSupportVertex * pSupport, const Vector3 & vPortalDir ) const;

        UInt iRank;
        _MPRSupportVertex * arrVertices[4];
    } _MPRSimplex;

    inline Void _MPR_ExpandPortal( _MPRSimplex * pSimplex, _MPRSupportVertex * pSupport );

    // Simplex facets
    typedef struct _simplex_facet {
        inline Void _Initialize();

        inline struct _simplex_facet & operator=( const struct _simplex_facet & rhs );

        inline Void _Link( UInt iIndexA, struct _simplex_facet * pFacetB, UInt iIndexB );

        Byte iPass;
        Vector3 vNormal;
        Scalar fDistance;
        _SupportVertex * arrVertices[3];
        Byte arrEdgeLinks[3];
        struct _simplex_facet * arrFacetLinks[3];
        struct _simplex_facet * arrListLinks[2];
    } _SimplexFacet;

    typedef struct _simplex_facetlist {
        inline Void _Initialize();

        inline Void _Append( _SimplexFacet * pFacet );
        inline Void _Remove( _SimplexFacet * pFacet );

        UInt iCount;
        _SimplexFacet * pHead;
    } _SimplexFacetList;

    // Simplex horizon
    typedef struct _simplex_horizon {
        inline Void _Initialize();

        UInt iCount;
        _SimplexFacet * pFacetC;
        _SimplexFacet * pFacetF;
    } _SimplexHorizon;

    // GJK/EPA/MPR solvers
    typedef struct _gjk_solver {
        GJKState iState;
        Vector3 vDirection;
        Scalar fDistance;
        UInt iCurrentSimplex;
        _Simplex arrSimplices[2];
        _PoolAllocator<_SupportVertex,4> hSVAllocator;
    } _GJKSolver;

    typedef struct _epa_solver {
        EPAState iState;
        Vector3 vNormal;
        Scalar fDepth;
        _SimplexFacetList hHull;
        _PoolAllocator<_SupportVertex,EPA_MAX_VERTICES> hSVAllocator;
        _PoolAllocator<_SimplexFacet,EPA_MAX_FACETS> hSFAllocator;
    } _EPASolver;

    typedef struct _mpr_solver {
        _MPRSimplex hPortal;
        _PoolAllocator<_MPRSupportVertex,5> hSVAllocator;
    } _MPRSolver;

    // GJK algorithm
        // Internal routines
    inline static Scalar _GJK_Det( const Vector3 & vA, const Vector3 & vB, const Vector3 & vC );

    static Scalar _GJK_ProjectOrigin( const Vector3 & vA, const Vector3 & vB, Scalar * arrWeights, UInt * pMask );
    static Scalar _GJK_ProjectOrigin( const Vector3 & vA, const Vector3 & vB, const Vector3 & vC, Scalar * arrWeights, UInt * pMask );
    static Scalar _GJK_ProjectOrigin( const Vector3 & vA, const Vector3 & vB, const Vector3 & vC, const Vector3 & vD, Scalar * arrWeights, UInt * pMask );

        // Main routines
    Void _GJK_Initialize();
    GJKState _GJK_Evaluate( const Vector3 & vGuess );
    Bool _GJK_ContainsOrigin();

    // EPA algorithm
        // Internal routines
    static Bool _EPA_GetEdgeDistance( Scalar * outDistance, const _SimplexFacet * pFacet, const _SupportVertex * pA, const _SupportVertex * pB );
    _SimplexFacet * _EPA_CreateFacet( _SupportVertex * pA, _SupportVertex * pB, _SupportVertex * pC, Bool bCheckConvexity );

    _SimplexFacet * _EPA_FindBestFacet() const;
    Bool _EPA_Expand( _SimplexHorizon * outHorizon, Byte iPass, _SupportVertex * pSV, _SimplexFacet * pFacet, Byte iEdge );

        // Main routines
    Void _EPA_Initialize();
    EPAState _EPA_Evaluate( const Vector3 & vGuess );

    // MPR algorithm
        // Internal routines
    inline static Bool _MPR_TestEquality( Scalar fA, Scalar fB );
    inline static Bool _MPR_TestEquality( const Vector3 & vA, const Vector3 & vB );
    static Scalar _MPR_SegmentDistanceSqr( Vector3 * outNormal, const Vector3 & vA, const Vector3 & vB );
    static Scalar _MPR_TriangleDistanceSqr( Vector3 * outNormal, const Vector3 & vA, const Vector3 & vB, const Vector3 & vC );

    Int _MPR_DiscoverPortal();
    Int _MPR_RefinePortal();

    Void _MPR_FindPosition( Vector3 * outPosition ) const;
    Void _MPR_FindPenetration( Vector3 * outPosition, Vector3 * outNormal, Scalar * outPenetration );

        // Main routines
    Void _MPR_Initialize();
    Bool _MPR_Evaluate( Vector3 * outPosition, Vector3 * outNormal, Scalar * outPenetration );

    // GJK-EPA-MPR solver data
    Vector3 m_vGuess;
    _Simplex m_hSimplex;

    _GJKSolver m_hGJKSolver;
    _EPASolver m_hEPASolver;
    _MPRSolver m_hMPRSolver;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GJK.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_NARROWPHASE_SAT_SAT_H

