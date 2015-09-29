/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/SpacePartitions/GaussTree.inl
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
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TGaussTree implementation
template<typename Real>
TGaussTree<Real>::TGaussTree()
{
    m_arrNodes = NULL;
    m_iNodeCount = 0;
}
template<typename Real>
TGaussTree<Real>::~TGaussTree()
{
    // nothing to do
    // Call Destroy explicitly
}

template<typename Real>
Void TGaussTree<Real>::Create()
{
    Assert( m_arrNodes == NULL );
}
template<typename Real>
Void TGaussTree<Real>::Destroy()
{
    Clear();
}

template<typename Real>
inline UInt TGaussTree<Real>::MemorySize() const {
    return ( m_iNodeCount * sizeof(SphericalArc) );
}
template<typename Real>
inline UInt TGaussTree<Real>::Count() const {
    return m_iNodeCount;
}

template<typename Real>
Void TGaussTree<Real>::Clear()
{
    if ( m_arrNodes != NULL ) {
        _SelectMemory();
        DeleteA( m_arrNodes );
        _UnSelectMemory();
    }

    m_arrNodes = NULL;
    m_iNodeCount = 0;
}

template<typename Real>
Void TGaussTree<Real>::Build( const MeshVoronoiGraph * pGraph )
{
    Assert( m_arrNodes == NULL );

    _SelectMemory();

    // Create temp storage for arcs
    RedBlackTree<SphericalArc> sortedArcs;
    sortedArcs.SetComparator( _Compare_Arcs );
    sortedArcs.Create();

    // Build Arcs & Bisectors
    _Build_SphericalArcs( sortedArcs, pGraph );
    _Build_SphericalBisectors( sortedArcs, pGraph );

    // Create temp storage for nodes
    Array<SphericalArc> arrNodes;
    arrNodes.Create();

    // Build BSP Tree
    RedBlackTree<SphericalArc>::Iterator itArc;
    for( itArc = sortedArcs.End(); !(itArc.IsNull()); --itArc )
        _rec_Build_Insert( 0, arrNodes, itArc.Get() );

    m_iNodeCount = arrNodes.Count();

    // Destroy temp storage for arcs
    sortedArcs.Destroy();

    // Finalize
    m_arrNodes = New SphericalArc[m_iNodeCount];

    MemCopy( m_arrNodes, (const SphericalArc *)arrNodes, m_iNodeCount * sizeof(SphericalArc) );

    // Destroy temp storage for nodes
    arrNodes.Destroy();

    _UnSelectMemory();
}

template<typename Real>
inline Void TGaussTree<Real>::ExtremalQuery( const TVector3<Real> & vDirection, HQuery * pQuery ) const {
    Assert( m_arrNodes != NULL );
    _rec_Query( 0, vDirection, pQuery );
}

/////////////////////////////////////////////////////////////////////////////////

template<typename Real>
Int TGaussTree<Real>::_Compare_Arcs( const SphericalArc & rLeft, const SphericalArc & rRight, Void * /*pUserData*/ )
{
    // Sort by path length
    if ( rLeft.iPathLength < rRight.iPathLength )
        return +1;
    if ( rRight.iPathLength < rLeft.iPathLength )
        return -1;

    // Use facet pointers as identifier
    if ( rLeft.pFacet[0] < rRight.pFacet[0] )
        return +1;
    if ( rRight.pFacet[0] < rLeft.pFacet[0] )
        return -1;

    if ( rLeft.pFacet[1] < rRight.pFacet[1] )
        return +1;
    if ( rRight.pFacet[1] < rLeft.pFacet[1] )
        return -1;

    return 0;
}

template<typename Real>
Void TGaussTree<Real>::_Build_SphericalArcs( RedBlackTree<SphericalArc> & sortedArcs, const MeshVoronoiGraph * pGraph )
{
    // Walk through all edges
    SphericalArc newArc;

    MeshVoronoiGraph::Edge * pEdge;
    TVector3<Real> vFacetNormal[2];
    Bool bInserted;

    pGraph->EnumEdges();
    pEdge = (MeshVoronoiGraph::Edge*)( pGraph->EnumNextEdge() );
    while( pEdge != NULL ) {
        newArc.pFacet[0] = pEdge->Facets[0];
        newArc.pFacet[1] = pEdge->Facets[1];

        newArc.iChild[0] = INVALID_OFFSET;
        newArc.iChild[1] = INVALID_OFFSET;
        newArc.pVertex[0] = pEdge->Vertices[0]; // Rely on SortEdgeLinkage
        newArc.pVertex[1] = pEdge->Vertices[1]; // to deduce signs ...

        newArc.iPathLength = 1;

        // Compute Arc's normal
        switch( newArc.pFacet[0]->GetFacetType() ) {
            case MESH_FACET_TRIANGLE: vFacetNormal[0] = ( (MeshVoronoiGraph::Triangle*)(newArc.pFacet[0]) )->Normal; break;
            case MESH_FACET_QUAD:     vFacetNormal[0] = (     (MeshVoronoiGraph::Quad*)(newArc.pFacet[0]) )->Normal; break;
            case MESH_FACET_POLYGON:  vFacetNormal[0] = (  (MeshVoronoiGraph::Polygon*)(newArc.pFacet[0]) )->Normal; break;
            default: Assert(false); break;
        }
        switch( newArc.pFacet[1]->GetFacetType() ) {
            case MESH_FACET_TRIANGLE: vFacetNormal[1] = ( (MeshVoronoiGraph::Triangle*)(newArc.pFacet[1]) )->Normal; break;
            case MESH_FACET_QUAD:     vFacetNormal[1] = (     (MeshVoronoiGraph::Quad*)(newArc.pFacet[1]) )->Normal; break;
            case MESH_FACET_POLYGON:  vFacetNormal[1] = (  (MeshVoronoiGraph::Polygon*)(newArc.pFacet[1]) )->Normal; break;
            default: Assert(false); break;
        }
        newArc.vNormal = ( vFacetNormal[0] ^ vFacetNormal[1] );
        newArc.vNormal.Normalize();

        // Add new arc
        bInserted = sortedArcs.Insert( newArc );
        //Assert( bInserted );

        // Next edge
        pEdge = (MeshVoronoiGraph::Edge*)( pGraph->EnumNextEdge() );
    }
}
template<typename Real>
Void TGaussTree<Real>::_Build_SphericalBisectors( RedBlackTree<SphericalArc> & sortedArcs, const MeshVoronoiGraph * pGraph )
{
    // Create temp queue
    Array<IndexPair> arrQueue;
    arrQueue.Create();

    UInt iEdgeCount;
    UInt iFacetCount;
    IndexPair idxPair, idxPairTemp;

    // Walk through all vertices
    SphericalArc newArc;

    MeshVoronoiGraph::Vertex * pVertex;
    MeshEdge *pEdge, *pNextEdge;
    MeshFacet * pFacet;
    TVector3<Real> vFacetNormal[2];
    Bool bInserted;

    pGraph->EnumVertices();
    pVertex = (MeshVoronoiGraph::Vertex*)( pGraph->EnumNextVertex() );
    while( pVertex != NULL ) {
        iEdgeCount = pVertex->GetEdgeCount();
        iFacetCount = iEdgeCount;
        idxPair.iFirst = 0;
        idxPair.iLast = iFacetCount;
        arrQueue.Push( idxPair );

        while( !(arrQueue.IsEmpty()) ) {
            arrQueue.Shift( idxPair );
            UInt iPathLength = ( idxPair.iLast - idxPair.iFirst );

            // End of subdivision cases
            if ( iPathLength <= 1 )
                continue;
            if ( iPathLength == (iFacetCount - 1) )
                continue;

            // Spherical polygon subdivision
            if ( idxPair.iLast < iFacetCount ) {
                pEdge = pVertex->Edges[idxPair.iFirst];
                pNextEdge = pVertex->Edges[(idxPair.iFirst + 1) % iEdgeCount];
                pFacet = pEdge->GetFacet(0);
                if ( pFacet->FindEdge(pNextEdge) == INVALID_OFFSET )
                    pFacet = pEdge->GetFacet(1);
                newArc.pFacet[0] = pFacet;

                pEdge = pVertex->Edges[idxPair.iLast];
                pNextEdge = pVertex->Edges[(idxPair.iLast + 1) % iEdgeCount];
                pFacet = pEdge->GetFacet(0);
                if ( pFacet->FindEdge(pNextEdge) == INVALID_OFFSET )
                    pFacet = pEdge->GetFacet(1);
                newArc.pFacet[1] = pFacet;

                newArc.iChild[0] = INVALID_OFFSET;
                newArc.iChild[1] = INVALID_OFFSET;
                newArc.pVertex[0] = pVertex;
                newArc.pVertex[1] = pVertex;

                newArc.iPathLength = iPathLength;

                // Compute Arc's normal
                switch( newArc.pFacet[0]->GetFacetType() ) {
                    case MESH_FACET_TRIANGLE: vFacetNormal[0] = ( (MeshVoronoiGraph::Triangle*)(newArc.pFacet[0]) )->Normal; break;
                    case MESH_FACET_QUAD:     vFacetNormal[0] = (     (MeshVoronoiGraph::Quad*)(newArc.pFacet[0]) )->Normal; break;
                    case MESH_FACET_POLYGON:  vFacetNormal[0] = (  (MeshVoronoiGraph::Polygon*)(newArc.pFacet[0]) )->Normal; break;
                    default: Assert(false); break;
                }
                switch( newArc.pFacet[1]->GetFacetType() ) {
                    case MESH_FACET_TRIANGLE: vFacetNormal[1] = ( (MeshVoronoiGraph::Triangle*)(newArc.pFacet[1]) )->Normal; break;
                    case MESH_FACET_QUAD:     vFacetNormal[1] = (     (MeshVoronoiGraph::Quad*)(newArc.pFacet[1]) )->Normal; break;
                    case MESH_FACET_POLYGON:  vFacetNormal[1] = (  (MeshVoronoiGraph::Polygon*)(newArc.pFacet[1]) )->Normal; break;
                    default: Assert(false); break;
                }
                newArc.vNormal = ( vFacetNormal[0] ^ vFacetNormal[1] );
                newArc.vNormal.Normalize();

                // Done
                bInserted = sortedArcs.Insert( newArc );
                Assert( bInserted );
            }

            // Recurse
            UInt iSplit = ( (idxPair.iFirst + idxPair.iLast + 1) >> 1 );
            if ( iSplit < idxPair.iLast ) {
                idxPairTemp.iFirst = idxPair.iFirst;
                idxPairTemp.iLast = iSplit;
                arrQueue.Push( idxPairTemp );
                idxPairTemp.iFirst = iSplit;
                idxPairTemp.iLast = idxPair.iLast;
                arrQueue.Push( idxPairTemp );
            }
        }

        // Next vertex
        pVertex = (MeshVoronoiGraph::Vertex*)( pGraph->EnumNextVertex() );
    }

    // Destroy temp queue
    arrQueue.Destroy();
}
template<typename Real>
Void TGaussTree<Real>::_rec_Build_Insert( UInt iNode, Array<SphericalArc> & arrNodes, const SphericalArc & rArc )
{
    // Root case
    if ( arrNodes.IsEmpty() ) {
        arrNodes.Push( rArc );
        return;
    }

    SphericalArc * pNode = &( arrNodes[iNode] );
    Int iSign0, iSign1;
    UInt iSwitch;

    // Arc Endpoint A
    if ( rArc.pFacet[0] == pNode->pFacet[0] || rArc.pFacet[0] == pNode->pFacet[1] )
        iSign0 = 0;
    else {
        TVector3<Real> vFacetNormal;
        switch( pNode->pFacet[0]->GetFacetType() ) {
            case MESH_FACET_TRIANGLE: vFacetNormal = ( (MeshVoronoiGraph::Triangle*)(pNode->pFacet[0]) )->Normal; break;
            case MESH_FACET_QUAD:     vFacetNormal = (     (MeshVoronoiGraph::Quad*)(pNode->pFacet[0]) )->Normal; break;
            case MESH_FACET_POLYGON:  vFacetNormal = (  (MeshVoronoiGraph::Polygon*)(pNode->pFacet[0]) )->Normal; break;
            default: Assert(false); break;
        }
        iSign0 = MathFn->SignI( vFacetNormal * pNode->vNormal );
    }

    // Arc Endpoint B
    if ( rArc.pFacet[1] == pNode->pFacet[0] || rArc.pFacet[1] == pNode->pFacet[1] )
        iSign1 = 0;
    else {
        TVector3<Real> vFacetNormal;
        switch( pNode->pFacet[1]->GetFacetType() ) {
            case MESH_FACET_TRIANGLE: vFacetNormal = ( (MeshVoronoiGraph::Triangle*)(pNode->pFacet[1]) )->Normal; break;
            case MESH_FACET_QUAD:     vFacetNormal = (     (MeshVoronoiGraph::Quad*)(pNode->pFacet[1]) )->Normal; break;
            case MESH_FACET_POLYGON:  vFacetNormal = (  (MeshVoronoiGraph::Polygon*)(pNode->pFacet[1]) )->Normal; break;
            default: Assert(false); break;
        }
        iSign1 = MathFn->SignI( vFacetNormal * pNode->vNormal );
    }

    // Cull against the node's arc
    iSwitch = 0;
    if ( (iSign0 * iSign1) < 0 ) {
        // Crossing arcs, propagate to both subtrees
        iSwitch = 3;
    } else if ( iSign0 < 0 || iSign1 < 0 ) {
        // Negative subtree
        iSwitch = 1;
    } else if ( iSign0 > 0 || iSign1 > 0 ) {
        // Positive subtree
        iSwitch = 2;
    } // else no propagation needed

    // Recurse / Insert
    if ( iSwitch & 1 ) {
        if ( pNode->iChild[0] != INVALID_OFFSET )
            _rec_Build_Insert( pNode->iChild[0], arrNodes, rArc );
        else {
            pNode->iChild[0] = arrNodes.Count();
            arrNodes.Push( rArc );
            pNode = &( arrNodes[iNode] );
        }
    }
    if ( iSwitch & 2 ) {
        if ( pNode->iChild[1] != INVALID_OFFSET )
            _rec_Build_Insert( pNode->iChild[1], arrNodes, rArc );
        else {
            pNode->iChild[1] = arrNodes.Count();
            arrNodes.Push( rArc );
        }
    }
}

template<typename Real>
Void TGaussTree<Real>::_rec_Query( UInt iNode, const TVector3<Real> & vDirection, HQuery * pQuery ) const
{
    Assert( iNode < m_iNodeCount );
    const SphericalArc * pNode = ( m_arrNodes + iNode );

    Int iSign = MathFn->SignI( vDirection * pNode->vNormal );
    UInt iSide = ( (iSign < 0) ? 0 : 1 );

    // Leaf case
    if ( pNode->iChild[iSide] == INVALID_OFFSET ) {
        pQuery->Result( pNode->pVertex[iSide]->Key.V );
        return;
    }

    // Node case
    _rec_Query( pNode->iChild[iSide], vDirection, pQuery );
}

