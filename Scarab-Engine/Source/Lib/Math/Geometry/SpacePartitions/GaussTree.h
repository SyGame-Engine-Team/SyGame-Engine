/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/SpacePartitions/GaussTree.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GaussTrees (BSP-Tree of a Gauss-Map using spherical arcs)
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : TODO : Build from mesh, no graph ! Add dynamic capacities ...
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_LIB_MATH_GEOMETRY_SPACEPARTITIONS_GAUSSTREE_H
#define SCARAB_LIB_MATH_GEOMETRY_SPACEPARTITIONS_GAUSSTREE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Datastruct/Array/Array.h"
#include "../../../Datastruct/Tree/RedBlackTree.h"

#include "../../Types/Vertex/Vertex3.h"
#include "../../Types/Vector/Vector3.h"

#include "../MeshGraphs/Graphs/MeshVoronoiGraph.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TGaussTree class
template<typename Real>
class TGaussTree : public Datastruct
{
public: // Handlers (Nested callbacks)
    class HQuery { public:
        HQuery()          { }
        virtual ~HQuery() { }

        virtual Void Result( UInt /*iVertex*/ ) { }
    };

public:
    TGaussTree();
    virtual ~TGaussTree();

    // Common minimal methods
    virtual Void Create();
    virtual Void Destroy();

    inline virtual UInt MemorySize() const;
    inline virtual UInt Count() const;

    virtual Void Clear();

    // Building
    Void Build( const MeshVoronoiGraph * pGraph );

    // Extremal query support
    inline Void ExtremalQuery( const TVector3<Real> & vDirection, HQuery * pQuery ) const;

private:
    // Building helpers
    typedef struct _index_pair {
        UInt iFirst, iLast;
    } IndexPair;
    typedef struct _spherical_arc {
        MeshFacet * pFacet[2];          // first & last facets at arc's endpoints
        UInt iChild[2];                 // 0 = negative / 1 = positive in BSP Tree
        MeshVertex * pVertex[2];        // 0 = negative / 1 = positive relative to this arc
        TVector3<Real> vNormal;         // Normal of this arc (cross product of facet normals)
        UInt iPathLength;               // Number of arcs from facet0's normal to facet1's normal
    } SphericalArc;

    static Int _Compare_Arcs( const SphericalArc & rLeft, const SphericalArc & rRight, Void * pUserData );

    static Void _Build_SphericalArcs( RedBlackTree<SphericalArc> & sortedArcs, const MeshVoronoiGraph * pGraph );
    static Void _Build_SphericalBisectors( RedBlackTree<SphericalArc> & sortedArcs, const MeshVoronoiGraph * pGraph );

    Void _rec_Build_Insert( UInt iNode, Array<SphericalArc> & arrNodes, const SphericalArc & rArc );

    Void _rec_Query( UInt iNode, const TVector3<Real> & vDirection, HQuery * pQuery ) const;

    // GaussTree data
    SphericalArc * m_arrNodes;
    UInt m_iNodeCount;
};

// Explicit instanciation
typedef TGaussTree<Float> GaussTreef;
typedef TGaussTree<Double> GaussTreed;
typedef TGaussTree<Scalar> GaussTree;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GaussTree.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_SPACEPARTITIONS_GAUSSTREE_H

