/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/GUISpatialTree.h
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
// Header prelude
#ifndef KOALA_ENGINE_GUI_GUISPATIALTREE_H
#define KOALA_ENGINE_GUI_GUISPATIALTREE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Lib/Core/Math/Types/Discrete/Rectangle2.h"

#include "../../Lib/Core/Datastruct/List/List.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
enum GUISpatialSplit
{
    GUISPATIAL_SPLIT_NONE = 0,
    GUISPATIAL_SPLIT_X,
    GUISPATIAL_SPLIT_Y
};

typedef struct _gui_spacialnode_item
{
    Rectangle2 rectRegion;
    UInt iDepth;
    Void * pItem;
} GUISpatialNodeItem;

typedef struct _gui_spatialnode
{
    Rectangle2 rectRegion;
    List<GUISpatialNodeItem> lstNodeItems;

    GUIQuadSplit iSplitAxis;
    Scalar fRelativeSplit;
    struct _gui_spatialnode * pLesser;
    struct _gui_spatialnode * pGreater;
} GUISpatialNode;

/////////////////////////////////////////////////////////////////////////////////
// The GUISpatialTree class
class GUISpatialTree
{
public:
    GUISpatialTree( Bool bAllowOverlaps );
    ~GUISpatialTree();

    Void * SpatialSearch( const Point2 & ptCoords ) const;
    Void Insert( const Rectangle2 & rectRegion, UInt iDepth, Void * pItem )
    {
        // For each node from root to leaf {
        //     if node is leaf {
        //         if node is full {
        //             split node;
        //             insert in leaf child;
        //         } else {
        //             insert in this node;
        //         }
        //         break;
        //     }
        //     if item intersects with split line {
        //         insert in this node;
        //         break;
        //     }
        //     recurse in child;
        // }
    }
    Bool Remove( const Rectangle2 & rectRegion, UInt iDepth, Void * pItem )
    {
    }

private:
    // Search routines
    GUISpatialNode * _SearchNode( GUISpatialNode * pNode, const Point2 & ptCoords ) const;
    Void * _SearchItem( GUISpatialNode * pNode, const Point2 & ptCoords ) const;

    // SplitAxis fitting routines
    Bool _SplitTestX( GUISpatialNode * pLeaf, Scalar * outRelativeSplit, Int * outBalanceFactor )
    {
        GUIFn->SelectMemory( TEXT("Scratch") );
        GUISpatialNodeItem * arrSortedItems = KNew() GUISpatialNodeItem[pLeaf->lstNodeItems.Count()];

        UInt iOrigin = (UInt)( pLeaf->rectRegion.TopLeft.X );
        UInt iAbsoluteSplit = ( iOrigin + (pLeaf->rectRegion.Width() >> 1) ); // /2
        UInt iMinSplit = (UInt)( pLeaf->rectRegion.TopLeft.X );
        UInt iMaxSplit = (UInt)( pLeaf->rectRegion.DownRight.X );

        KDeleteA( arrSortedItems );
        GUIFn->UnSelectMemory();
    }

    // Tree data
    GUISpatialNode * m_pRoot;

    Bool m_bAllowOverlaps;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GUISpatialTree.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // KOALA_ENGINE_GUI_GUISPATIALTREE_H
