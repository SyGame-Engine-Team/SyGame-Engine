/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/SpacePartitions/KDTree.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : K-Dimensional Trees
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_SPACEPARTITIONS_KDTREE_H
#define SCARAB_LIB_MATH_GEOMETRY_SPACEPARTITIONS_KDTREE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Datastruct/Datastruct.h"

#include "../../Types/Vertex/Vertex3.h"
#include "../../Types/Vector/Vector3.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TKDTree class
//template<typename Real>
//class TKDTree : public Datastruct
//{
//public:
//    class SpatialItem { public:
//        SpatialItem();
//        virtual ~SpatialItem();
//
//        virtual const TVertex3<Real> & GetCenter() = 0;
//        virtual Real GetRadius() = 0;
//
//        virtual const TVector3<Real> & GetMinBound() = 0;
//        virtual const TVector3<Real> & GetMaxBound() = 0;
//
//        virtual Void OnVisitNearest() = 0;
//
//        SpatialItem * Next;
//    };
//    typedef SpatialItem * (*ItemBuilder)();
//
//public:
//    TKDTree( ItemBuilder pfItemBuilder, UInt iK );
//    virtual ~TKDTree();
//
//    // Common minimal methods
//    virtual Void Create();
//    virtual Void Destroy();
//
//    inline virtual UInt MemorySize() const;
//    inline virtual UInt Count() const;
//
//    virtual Void Clear();
//
//	// Methods
//
//private:
//    typedef struct _kdnode
//    {
//        struct _kdnode * pLeft;
//        struct _kdnode * pRight;
//        Byte iAxis;
//        Real fSplit;
//
//        SpatialItem * pItemList;
//    } KDNode;
//
//    // Traversals
//    Void _NearestNeightboors( KDNode * pNode, const TVertex3<Real> & vPoint, Real fRange, TVertex3<Real> & vNear ) const
//    {
//        if ( pNode == NULL )
//            return;
//
//        // Visit node
//        SpatialItem * pItem = pNode->pItemList;
//        while( pItem != NULL ) {
//            pItem->OnVisitNearest();
//            pItem = pItem->Next;
//        }
//
//        // Visit Near sub-tree
//        KDNode * pNear = ( vPoint[pNode->iAxis] < pNode->fSplit ) ? pNode->pLeft : pNode->pRight;
//        _NearestNeightboors( pNear, vPoint, fRange, vNear );
//
//        // Visit Far sub-tree
//        Real fOldNear = vNear[pNode->iAxis];
//        vNear[pNode->iAxis] = pNode->fSplit;
//        if ( (vNear - vPoint).NormSqr() < (fRange * fRange) )
//            _NearestNeightboors( pFar, vPoint, fRange, vNear );
//        vNear[pNode->iAxis] = fOldNear;
//    }
//
//    ItemBuilder m_pfItemBuilder;
//
//    KDNode * m_pRoot;
//};
//
//// Explicit instanciation
//typedef TKDTree<Float> KDTreef;
//typedef TKDTree<Double> KDTreed;
//typedef TKDTree<Scalar> KDTree;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "KDTree.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_SPACEPARTITIONS_KDTREE_H

