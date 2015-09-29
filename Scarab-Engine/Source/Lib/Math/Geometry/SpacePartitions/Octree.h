/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/SpacePartitions/Octree.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Octrees
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : TODO : Add dynamic capacities ...
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_LIB_MATH_GEOMETRY_SPACEPARTITIONS_OCTREE_H
#define SCARAB_LIB_MATH_GEOMETRY_SPACEPARTITIONS_OCTREE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Datastruct/Datastruct.h"

#include "../BoundingShapes/3D/BVAABox.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

    // Octree leaves capacity
#define OCTREE_LEAF_CAPACITY 8

    // Octree children's axes
#define OCTREE_CHILD_X 0x01 //
#define OCTREE_CHILD_Y 0x02 // 0 => negative side
#define OCTREE_CHILD_Z 0x04 // 1 => positive side

/////////////////////////////////////////////////////////////////////////////////
// The TOctreeBaseNode class
template<typename Real>
class TOctreeBaseNode
{
public:
    TOctreeBaseNode( const TBVAABox<Real> & bvNodeAABox );
    virtual ~TOctreeBaseNode();

    virtual Bool IsNode() const = 0;
    virtual Bool IsLeaf() const = 0;

    TBVAABox<Real> bvAABox;
};

// Explicit instanciation
typedef TOctreeBaseNode<Float> OctreeBaseNodef;
typedef TOctreeBaseNode<Double> OctreeBaseNoded;
typedef TOctreeBaseNode<Scalar> OctreeBaseNode;

/////////////////////////////////////////////////////////////////////////////////
// The TOctreeNode class
template<typename Real>
class TOctreeNode : public TOctreeBaseNode<Real>
{
public:
    TOctreeNode( const TBVAABox<Real> & bvNodeAABox );
    virtual ~TOctreeNode();

    inline virtual Bool IsNode() const;
    inline virtual Bool IsLeaf() const;

    // Node data
    TOctreeBaseNode<Real> * pChild[8]; // node case
};

// Explicit instanciation
typedef TOctreeNode<Float> OctreeNodef;
typedef TOctreeNode<Double> OctreeNoded;
typedef TOctreeNode<Scalar> OctreeNode;

/////////////////////////////////////////////////////////////////////////////////
// The TOctreeLeaf class
template<typename Real>
class TOctreeLeaf : public TOctreeBaseNode<Real>
{
public:
    TOctreeLeaf( const TBVAABox<Real> & bvNodeAABox );
    virtual ~TOctreeLeaf();

    inline virtual Bool IsNode() const;
    inline virtual Bool IsLeaf() const;

    // Leaf data
    UInt iDataCount;
    TBVAABox<Real> arrAABoxes[OCTREE_LEAF_CAPACITY];
    Void * arrDatas[OCTREE_LEAF_CAPACITY];
};

// Explicit instanciation
typedef TOctreeLeaf<Float> OctreeLeaff;
typedef TOctreeLeaf<Double> OctreeLeafd;
typedef TOctreeLeaf<Scalar> OctreeLeaf;

/////////////////////////////////////////////////////////////////////////////////
// The TOctree class
template<typename Real>
class TOctree : public Datastruct
{
public: // Handlers (Nested callbacks)
    class HEnumerator { public:
        HEnumerator()          { }
        virtual ~HEnumerator() { }

        virtual Void EnumerateNode( const TOctreeNode<Real> * pNode ) { }
        virtual Void EnumerateLeaf( const TOctreeLeaf<Real> * pLeaf ) { }
    };
    class HCollider { public:
        HCollider()          { }
        virtual ~HCollider() { }

        virtual Void Process( const TOctreeLeaf<Real> * pLeaf ) { }
        virtual Void Process( const TOctreeLeaf<Real> * pLeafA, const TOctreeLeaf<Real> * pLeafB ) { }
    };

public:
    TOctree();
    virtual ~TOctree();

    // Deferred construction
    inline Void SetRootBox( const TBVAABox<Real> & bvAABox );

    // Common minimal methods
    virtual Void Create();
    virtual Void Destroy();

    inline virtual UInt MemorySize() const;
    inline virtual UInt Count() const;

    virtual Void Clear();

    // Insertion
    Void Insert( const TBVAABox<Real> & bvAABox, Void * pData );

    // Removal
    //////////////////////

    // Updating
    //////////////////////

    // Optimization
    //////////////////////

    // Enumerate support
    inline Void Enumerate( HEnumerator * pEnumerator ) const;

    // Collide support
    inline Void CollideRay( const TLine3<Real> & vRay, Real fMinDistance, Real fMaxDistance, HCollider * pCollider ) const;
    inline Void CollideAABox( const TBVAABox<Real> & bvAABox, HCollider * pCollider ) const;
    inline Void CollideTree( const TOctree<Real> * pTree, HCollider * pCollider ) const;

private:
    // Helpers
    static UInt _SelectChildren( const TOctreeNode<Real> * pNode, const TBVAABox<Real> & bvAABox );

    // Recursive helpers
    Void _rec_SplitLeafToNode( TOctreeNode<Real> ** ppNode, const TOctreeLeaf<Real> * pLeaf );
    Void _rec_Insert( TOctreeBaseNode<Real> ** ppNode, const TBVAABox<Real> & bvAABox, Void * pData );

    Void _rec_DestroyTree( TOctreeBaseNode<Real> * pNode );

    static Void _rec_Enumerate( const TOctreeBaseNode<Real> * pNode, HEnumerator * pEnumerator );

    static Void _rec_CollideRay( const TOctreeBaseNode<Real> * pNode,
                                 const TLine3<Real> & vRay, Real fMinDistance, Real fMaxDistance,
                                 HCollider * pCollider );
    static Void _rec_CollideAABox( const TOctreeBaseNode<Real> * pNode, const TBVAABox<Real> & bvAABox,
                                   HCollider * pCollider );
    static Void _rec_CollideTree( const TOctreeBaseNode<Real> * pNodeA, const TOctreeBaseNode<Real> * pNodeB,
                                  HCollider * pCollider );

    // Octree data
    TBVAABox<Real> m_bvRootBox;

    TOctreeBaseNode<Real> * m_pRoot;
    UInt m_iNodeCount;
    UInt m_iLeafCount;
};

// Explicit instanciation
typedef TOctree<Float> Octreef;
typedef TOctree<Double> Octreed;
typedef TOctree<Scalar> Octree;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Octree.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_SPACEPARTITIONS_OCTREE_H

