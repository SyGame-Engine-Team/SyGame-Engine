/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/GUISpatialTree.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Spatial partition for accelerated point-location queries ...
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-Noncommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "GUISpatialTree.h"

/////////////////////////////////////////////////////////////////////////////////
// GUISpatialTree implementation
GUISpatialTree::GUISpatialTree( Bool bAllowOverlaps )
{
    m_pRoot = NULL;

    m_bAllowOverlaps = bAllowOverlaps;
}
GUISpatialTree::~GUISpatialTree()
{
}

Void * GUISpatialTree::SpatialSearch( const Point2 & ptCoords ) const
{
    Assert( m_pRoot != NULL );

    Bool bValidSearch = m_pRoot->rectRegion.Contains( ptCoords );
    if ( !bValidSearch )
        return NULL;

    GUISpatialNode * pNode = _SearchNode( m_pRoot, ptCoords );
    Assert( pNode != NULL );

    return _SearchItem( pNode, ptCoords );
}

/////////////////////////////////////////////////////////////////////////////////

GUISpatialNode * GUISpatialTree::_SearchNode( GUISpatialNode * pNode, const Point2 & ptCoords ) const
{
    Assert( pNode != NULL );
    Assert( pNode->rectRegion.Contains(ptCoords) );

    // Leaf case
    if ( pNode->iSplitAxis == GUIQUADSPLIT_NONE )
        return pNode;

    // Node cases
    Int iAbsoluteSplit;
    GUIQuadNode * pChild = NULL;
    if ( pNode->iSplitAxis == GUIQUADSPLIT_X ) {
        iAbsoluteSplit = pNode->rectRegion.TopLeft.X;
        iAbsoluteSplit += MathFn->Floor( fRelativeSplit * (Scalar)( pNode->rectRegion.Width() - 1 ) );
        if ( ptCoords.X == iAbsoluteSplit )
            return pNode;
        if ( ptCoords.X < iAbsoluteSplit )
            pChild = pNode->pLesser;
        else // ( ptCoords.X > iAbsoluteSplit )
            pChild = pNode->pGreater;
    } else { // ( pNode->iSplitAxis == GUIQUADSPLIT_Y )
        iAbsoluteSplit = pNode->rectRegion.TopLeft.Y;
        iAbsoluteSplit += MathFn->Floor( fRelativeSplit * (Scalar)( pNode->rectRegion.Height() - 1 ) );
        if ( ptCoords.Y == iAbsoluteSplit )
            return pNode;
        if ( ptCoords.Y < iAbsoluteSplit )
            pChild = pNode->pLesser;
        else // ( ptCoords.Y > iAbsoluteSplit )
            pChild = pNode->pGreater;
    }
    return _SearchNode( pChild, ptCoords );
}
Void * GUISpatialTree::_SearchItem( GUISpatialNode * pNode, const Point2 & ptCoords ) const
{
    Assert( pNode != NULL );

    List<GUISpatialNodeItem>::Iterator it;

    // Non-Overlapping case
    if ( !m_bAllowOverlaps ) {
        for( it = pNode->lstNodeItems.Begin(); !(it.IsNull()); ++it ) {
            if ( it.GetData().rectRegion.Contains(ptCoords) )
                return it.GetData().pItem;
        }
        return NULL;
    }

    // Overlapping case
    UInt iTopMostDepth = 0xffffffff;
    Void * pItem = NULL;
    for( it = pNode->lstNodeItems.Begin(); !(it.IsNull()); ++it ) {
        if ( it.GetData().rectRegion.Contains(ptCoords) ) {
            if ( it.GetData().iDepth < iTopMostDepth ) {
                iTopMostDepth = it.GetData().iDepth;
                pItem = it.GetData().pItem;
            }
        }
    }
    return pItem;
}
