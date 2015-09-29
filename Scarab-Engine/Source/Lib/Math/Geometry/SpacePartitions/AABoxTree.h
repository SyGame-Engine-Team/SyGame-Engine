/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/SpacePartitions/AABoxTree.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : AABox Trees
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_SPACEPARTITIONS_AABOXTREE_H
#define SCARAB_LIB_MATH_GEOMETRY_SPACEPARTITIONS_AABOXTREE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Datastruct/Datastruct.h"

#include "../BoundingShapes/3D/BVAABox.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TAABoxNode class
template<typename Real>
class TAABoxNode
{
public:
    TAABoxNode( const TBVAABox<Real> & bvNodeAABox, TAABoxNode<Real> * pParentNode = NULL );
    TAABoxNode( const TBVAABox<Real> & bvNodeAABox, TAABoxNode<Real> * pParentNode, Void * pData );
    TAABoxNode( const TBVAABox<Real> & bvNodeAABox, TAABoxNode<Real> * pParentNode,
                TAABoxNode<Real> * pChildA, TAABoxNode<Real> * pChildB );
    TAABoxNode( const TAABoxNode<Real> & rhs );
    ~TAABoxNode();

    inline Bool IsNode() const;
    inline Bool IsLeaf() const;

    // Node data
    TBVAABox<Real> bvAABox;

    TAABoxNode<Real> * pParent;
    union {
        TAABoxNode<Real> * pChild[2]; // node case
        Void * pLeafData;             // leaf case
    };
};

// Explicit instanciation
typedef TAABoxNode<Float> AABoxNodef;
typedef TAABoxNode<Double> AABoxNoded;
typedef TAABoxNode<Scalar> AABoxNode;

/////////////////////////////////////////////////////////////////////////////////
// The TAABoxTree class
template<typename Real>
class TAABoxTree : public Datastruct
{
public: // Handlers (Nested callbacks)
    class HEnumerator { public:
        HEnumerator()          { }
        virtual ~HEnumerator() { }

        virtual Void EnumerateNode( const TAABoxNode<Real> * /*pNode*/ ) { }
        virtual Void EnumerateLeaf( const TAABoxNode<Real> * /*pLeaf*/ ) { }
    };
    class HCloner { public:
        HCloner()          { }
        virtual ~HCloner() { }

        virtual Void CloneLeaf( const TAABoxNode<Real> * /*pLeaf*/ ) { }
    };
    class HCollider { public:
        HCollider()          { }
        virtual ~HCollider() { }

        virtual Bool Descent( const TAABoxNode<Real> * /*pNode*/ ) { return true; }
        virtual Bool AllLeaves( const TAABoxNode<Real> * /*pNode*/ ) { return true; }

        virtual Void Process( const TAABoxNode<Real> * /*pLeaf*/ ) { }
        virtual Void Process( const TAABoxNode<Real> * /*pLeaf*/, Real /*fProjection*/ ) { }
        virtual Void Process( const TAABoxNode<Real> * /*pLeafA*/, const TAABoxNode<Real> * /*pLeafB*/ ) { }
    };

public:
    TAABoxTree();
    virtual ~TAABoxTree();

    // Common minimal methods
    virtual Void Create();
    virtual Void Destroy();

    inline virtual UInt MemorySize() const;
    inline virtual UInt Count() const;

    virtual Void Clear();

    // Getters
    inline TAABoxNode<Real> * GetRoot() const;

    // Insertion
    TAABoxNode<Real> * Insert( const TBVAABox<Real> & bvAABox, Void * pLeafData );

    // Removal
    Void Remove( TAABoxNode<Real> * pLeaf );

    // Updating
    Void Update( TAABoxNode<Real> * pLeaf, UInt iLookAhead = INVALID_OFFSET );
    Void Update( TAABoxNode<Real> * pLeaf, const TBVAABox<Real> & bvAABox, UInt iLookAhead = INVALID_OFFSET );

    // Optimization
    Void OptimizeBottomUp();
    Void OptimizeTopDown( UInt iThreshold = 128 );
    Void OptimizeIncremental( UInt iPasses = INVALID_OFFSET );

    // Leaf extraction
    inline UInt GetMaxDepth() const;
    inline UInt GetLeafCount() const;
    inline UInt ExtractLeaves( TAABoxNode<Real> ** outLeaves, Bool bDeleteNodes = false );

    // Enumerate support
    inline Void Enumerate( HEnumerator * pEnumerator ) const;

    // Clone support
    inline Void Clone( TAABoxTree<Real> * pTarget, HCloner * pCloner ) const;

    // Collide support
    inline Void CollideRay( const TLine3<Real> & vRay, Real fMinDistance, Real fMaxDistance, HCollider * pCollider ) const;
    inline Void CollideAABox( const TBVAABox<Real> & bvAABox, HCollider * pCollider ) const;
    inline Void CollideTree( const TAABoxTree<Real> * pTree, HCollider * pCollider ) const;
    inline Void CollidePlanes( TPlane<Real> * arrPlanes, UInt iPlaneCount, HCollider * pCollider ) const;
    inline Void CollideOcclusion( TPlane<Real> * arrPlanes, UInt iPlaneCount, const TVector3<Real> & vSortAxis,
                                  HCollider * pCollider ) const;
    inline Void CollideUser( HCollider * pCollider ) const;

private:
    // Helpers
    static Real _ProjectMinimum( const TBVAABox<Real> & bvAABox, const TVector3<Real> & vAxis );
    static TAABoxNode<Real> * _SelectChild( const TAABoxNode<Real> * pNode, const TBVAABox<Real> & dbvReference );

    TAABoxNode<Real> * _SortNodes( TAABoxNode<Real> * pNode );

    // Recursive helpers
    Void _InsertLeaf( TAABoxNode<Real> * pNode, TAABoxNode<Real> * pLeaf );
    TAABoxNode<Real> * _RemoveLeaf( TAABoxNode<Real> * pLeaf );

    Void _rec_DestroyTree( TAABoxNode<Real> * pNode );

    TAABoxNode<Real> * _BottomUp( TAABoxNode<Real> ** arrLeaves, UInt iLeafCount );
    TAABoxNode<Real> * _rec_TopDown( TAABoxNode<Real> ** arrLeaves, UInt iLeafCount, UInt iThreshold );

    static UInt _rec_GetMaxDepth( const TAABoxNode<Real> * pNode );
    static UInt _rec_GetLeafCount( const TAABoxNode<Real> * pNode );
    UInt _rec_ExtractLeaves( TAABoxNode<Real> * pNode, TAABoxNode<Real> ** outLeaves, UInt iLeafCount,
                             Bool bDeleteNodes = false );

    static Void _rec_Enumerate( const TAABoxNode<Real> * pNode, HEnumerator * pEnumerator );

    static Void _rec_Clone( const TAABoxNode<Real> * pNode,
                            TAABoxTree<Real> * pTarget, TAABoxNode<Real> * pTargetParent, UInt iTargetParentIndex,
                            HCloner * pCloner );

    static Void _rec_CollideRay( const TAABoxNode<Real> * pNode,
                                 const TLine3<Real> & vRay, Real fMinDistance, Real fMaxDistance,
                                 HCollider * pCollider );
    static Void _rec_CollideAABox( const TAABoxNode<Real> * pNode, const TBVAABox<Real> & bvAABox,
                                     HCollider * pCollider );
    static Void _rec_CollideTree( const TAABoxNode<Real> * pNodeA, const TAABoxNode<Real> * pNodeB,
                                  HCollider * pCollider );
    static Void _rec_CollidePlanes( const TAABoxNode<Real> * pNode, UInt iMask,
                                    TPlane<Real> * arrPlanes, UInt iPlaneCount,
                                    HCollider * pCollider );
    static Void _rec_CollideOcclusion( const TAABoxNode<Real> * pNode, UInt iMask, Real fProjection,
                                       TPlane<Real> * arrPlanes, UInt iPlaneCount, const TVector3<Real> & vSortAxis,
                                       HCollider * pCollider );
    static Void _rec_CollideUser( const TAABoxNode<Real> * pNode, HCollider * pCollider );

    static Void _rec_CollidePlanes_AllLeaves( const TAABoxNode<Real> * pNode, HCollider * pCollider );

    // Dynamic BVTree data
    TAABoxNode<Real> * m_pRoot;
    UInt m_iNodeCount;
};

// Explicit instanciation
typedef TAABoxTree<Float> AABoxTreef;
typedef TAABoxTree<Double> AABoxTreed;
typedef TAABoxTree<Scalar> AABoxTree;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "AABoxTree.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_SPACEPARTITIONS_AABOXTREE_H

