/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/MeshGraph.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Base classes for adjacency & topology information.
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_MESHGRAPH_H
#define SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_MESHGRAPH_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Features/Vertices/MeshVertexManifold2.h"
#include "Features/Vertices/MeshVertexManifoldN.h"
#include "Features/Edges/MeshEdgeManifold2.h"
#include "Features/Edges/MeshEdgeManifold3.h"
#include "Features/Edges/MeshEdgeManifoldN.h"
#include "Features/Facets/MeshTriangleManifold3.h"
#include "Features/Facets/MeshTriangleManifoldN.h"
#include "Features/Facets/MeshQuadManifold3.h"
#include "Features/Facets/MeshQuadManifoldN.h"
#include "Features/Facets/MeshPolygonManifold3.h"
#include "Features/Facets/MeshPolygonManifoldN.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
enum MeshGraphType
{
    MESHGRAPH_MANIFOLD2 = 0,
    MESHGRAPH_MANIFOLD3,
    MESHGRAPH_MANIFOLDN
};

/////////////////////////////////////////////////////////////////////////////////
// The MeshGraph class
class MeshGraph : public Datastruct
{
public:
    MeshGraph();
    virtual ~MeshGraph();

    // Type
    virtual MeshGraphType GetType() const = 0;

protected:
    // Linkage operations
    typedef DWord _LinkageResult;
    static const _LinkageResult _LINKAGE_DONE         = 0;
    static const _LinkageResult _LINKAGE_GRADE_A      = 0x00000001;
    static const _LinkageResult _LINKAGE_GRADE_B      = 0x00000002;
    static const _LinkageResult _LINKAGE_FACETSIZE_A  = 0x00000010; // Facets Only
    static const _LinkageResult _LINKAGE_FULLEDGE     = 0xfffffffe;
    static const _LinkageResult _LINKAGE_NOLINK       = 0xffffffff;

    static _LinkageResult _Connect( MeshFacet * pFacetA,   MeshFacet * pFacetB );
    static _LinkageResult _Connect( MeshFacet * pFacetA,   MeshEdge * pEdgeB );
    static _LinkageResult _Connect( MeshFacet * pFacetA,   MeshVertex * pVertexB );
    static _LinkageResult _Connect( MeshEdge * pEdgeA,     MeshEdge * pEdgeB );
    static _LinkageResult _Connect( MeshEdge * pEdgeA,     MeshVertex * pVertexB );
    static _LinkageResult _Connect( MeshVertex * pVertexA, MeshVertex * pVertexB );

    static _LinkageResult _Disconnect( MeshFacet * pFacetA,   MeshFacet * pFacetB );
    static _LinkageResult _Disconnect( MeshFacet * pFacetA,   MeshEdge * pEdgeB );
    static _LinkageResult _Disconnect( MeshFacet * pFacetA,   MeshVertex * pVertexB );
    static _LinkageResult _Disconnect( MeshEdge * pEdgeA,     MeshEdge * pEdgeB );
    static _LinkageResult _Disconnect( MeshEdge * pEdgeA,     MeshVertex * pVertexB );
    static _LinkageResult _Disconnect( MeshVertex * pVertexA, MeshVertex * pVertexB );

    // Upgrade/Downgrade operations
    //enum _GradeChange { _DOWNGRADE = 0, _UPGRADE = 1 };
    //static Void _ChangeGrade( MeshVertex ** ppVertex, _GradeChange iGradeChange );
    //static Void _ChangeGrade( MeshEdge ** ppEdge, _GradeChange iGradeChange );
    //static Void _ChangeGrade( MeshFacet ** ppFacet,  _GradeChange iGradeChange );

    // Grow/Shrink operations
    //static Void _Grow( MeshFacet ** ppFacet, UInt iEdge, const MeshEdgeKey & edgeKey );
    //static Void _Shrink( MeshFacet ** ppFacet, UInt iEdge, UInt iVertex );
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "MeshGraph.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_MESHGRAPH_H

