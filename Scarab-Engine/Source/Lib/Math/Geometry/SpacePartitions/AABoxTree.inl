/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/SpacePartitions/AABoxTree.inl
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
// TAABoxNode implementation
template<typename Real>
TAABoxNode<Real>::TAABoxNode( const TBVAABox<Real> & bvNodeAABox, TAABoxNode<Real> * pParentNode ):
    bvAABox( bvNodeAABox )
{
    pParent = pParentNode;
    pChild[0] = NULL;
    pChild[1] = NULL;
}
template<typename Real>
TAABoxNode<Real>::TAABoxNode( const TBVAABox<Real> & bvNodeAABox, TAABoxNode<Real> * pParentNode, Void * pData ):
    bvAABox( bvNodeAABox )
{
    pParent = pParentNode;
    pLeafData = pData;
    pChild[1] = NULL;
}
template<typename Real>
TAABoxNode<Real>::TAABoxNode( const TBVAABox<Real> & bvNodeAABox, TAABoxNode<Real> * pParentNode,
                              TAABoxNode<Real> * pChildA, TAABoxNode<Real> * pChildB ):
    bvAABox( bvNodeAABox )
{
    pParent = pParentNode;
    pChild[0] = pChildA;
    pChild[1] = pChildB;
}
template<typename Real>
TAABoxNode<Real>::TAABoxNode( const TAABoxNode<Real> & rhs ):
    bvAABox( rhs.bvAABox )
{
    pParent = rhs.pParent;
    pChild[0] = rhs.pChild[0];
    pChild[1] = rhs.pChild[1];
}
template<typename Real>
TAABoxNode<Real>::~TAABoxNode()
{
    // nothing to do
}

template<typename Real>
inline Bool TAABoxNode<Real>::IsNode() const {
    return ( pChild[1] != NULL );
}
template<typename Real>
inline Bool TAABoxNode<Real>::IsLeaf() const {
    return ( pChild[1] == NULL );
}

/////////////////////////////////////////////////////////////////////////////////
// TAABoxTree implementation
template<typename Real>
TAABoxTree<Real>::TAABoxTree():
    Datastruct()
{
    m_pRoot = NULL;
    m_iNodeCount = 0;
}
template<typename Real>
TAABoxTree<Real>::~TAABoxTree()
{
    // nothing to do
    // Call Destroy explicitly
}

template<typename Real>
Void TAABoxTree<Real>::Create()
{
    Assert( m_pRoot == NULL );
}
template<typename Real>
Void TAABoxTree<Real>::Destroy()
{
    Clear();
}

template<typename Real>
inline UInt TAABoxTree<Real>::MemorySize() const {
    return ( m_iNodeCount * sizeof(TAABoxNode<Real>) );
}
template<typename Real>
inline UInt TAABoxTree<Real>::Count() const {
    return m_iNodeCount;
}

template<typename Real>
Void TAABoxTree<Real>::Clear()
{
    if ( m_pRoot != NULL )
        _rec_DestroyTree( m_pRoot );
    m_pRoot = NULL;
    m_iNodeCount = 0;
}

template<typename Real>
inline TAABoxNode<Real> * TAABoxTree<Real>::GetRoot() const {
    return m_pRoot;
}

template<typename Real>
TAABoxNode<Real> * TAABoxTree<Real>::Insert( const TBVAABox<Real> & bvAABox, Void * pLeafData )
{
    _SelectMemory();
    TAABoxNode<Real> * pLeaf = New TAABoxNode<Real>( bvAABox, NULL, pLeafData );
    _UnSelectMemory();
    ++m_iNodeCount;

    _InsertLeaf( m_pRoot, pLeaf );

    return pLeaf;
}

template<typename Real>
Void TAABoxTree<Real>::Remove( TAABoxNode<Real> * pLeaf )
{
    _RemoveLeaf( pLeaf );

    _SelectMemory();
    Delete( pLeaf );
    _UnSelectMemory();
    --m_iNodeCount;
}

template<typename Real>
Void TAABoxTree<Real>::Update( TAABoxNode<Real> * pLeaf, UInt iLookAhead )
{
    TAABoxNode<Real> * pRoot = _RemoveLeaf( pLeaf );
    if ( pRoot != NULL ) {
        if ( iLookAhead != INVALID_OFFSET ) {
            for( UInt i = 0; i < iLookAhead; ++i ) {
                if ( pRoot->pParent == NULL )
                    break;
                pRoot = pRoot->pParent;
            }
        } else
            pRoot = m_pRoot;
    }
    _InsertLeaf( pRoot, pLeaf );
}
template<typename Real>
Void TAABoxTree<Real>::Update( TAABoxNode<Real> * pLeaf, const TBVAABox<Real> & bvAABox, UInt iLookAhead )
{
    TAABoxNode<Real> * pRoot = _RemoveLeaf( pLeaf );
    if ( pRoot != NULL ) {
        if ( iLookAhead != INVALID_OFFSET ) {
            for( UInt i = 0; i < iLookAhead; ++i ) {
                if ( pRoot->pParent == NULL )
                    break;
                pRoot = pRoot->pParent;
            }
        } else
            pRoot = m_pRoot;
    }
    pLeaf->bvAABox = bvAABox;
    _InsertLeaf( pRoot, pLeaf );
}

template<typename Real>
Void TAABoxTree<Real>::OptimizeBottomUp()
{
    if ( m_pRoot == NULL )
        return;

    UInt iLeafCount = GetLeafCount();

    // Create leaf array
    _SelectMemory();
    TAABoxNode<Real> ** arrLeaves = New TAABoxNode<Real>*[iLeafCount];
    _UnSelectMemory();

    ExtractLeaves( arrLeaves, true );

    // Bottom-Up builder
    m_pRoot = _BottomUp( arrLeaves, iLeafCount );

    // Destroy leaf array
    _SelectMemory();
    DeleteA( arrLeaves );
    _UnSelectMemory();
}
template<typename Real>
Void TAABoxTree<Real>::OptimizeTopDown( UInt iThreshold )
{
    if ( m_pRoot == NULL )
        return;

    UInt iLeafCount = GetLeafCount();

    // Create leaf array
    _SelectMemory();
    TAABoxNode<Real> ** arrLeaves = New TAABoxNode<Real>*[iLeafCount];
    _UnSelectMemory();

    ExtractLeaves( arrLeaves, true );

    // Top-Down builder
    m_pRoot = _rec_TopDown( arrLeaves, iLeafCount, iThreshold );

    // Destroy leaf array
    _SelectMemory();
    DeleteA( arrLeaves );
    _UnSelectMemory();
}
template<typename Real>
Void TAABoxTree<Real>::OptimizeIncremental( UInt iPasses )
{
    static const UInt iMask = ( (sizeof(UInt) << 3) - 1 );

    if ( iPasses == INVALID_OFFSET )
        iPasses = GetLeafCount();

    if ( m_pRoot == NULL )
        return;
    if ( iPasses == 0 )
        return;

    TAABoxNode<Real> * pNode;
    UInt iPath = 0, iBit; // iPath should be a persistent member ?

    while( iPasses > 0 ) {
        pNode = m_pRoot;
        iBit = 0;
        while( pNode->IsNode() ) {
            pNode = _SortNodes( pNode );
            pNode = pNode->pChild[(iPath >> iBit) & 1];
            iBit = ( (iBit + 1) & iMask );
        }
        Update( pNode );
        ++iPath;
        --iPasses;
    }
}

template<typename Real>
inline UInt TAABoxTree<Real>::GetMaxDepth() const {
    return _rec_GetMaxDepth( m_pRoot );
}
template<typename Real>
inline UInt TAABoxTree<Real>::GetLeafCount() const {
    return _rec_GetLeafCount( m_pRoot );
}
template<typename Real>
inline UInt TAABoxTree<Real>::ExtractLeaves( TAABoxNode<Real> ** outLeaves, Bool bDeleteNodes ) {
    UInt iLeafCount = _rec_ExtractLeaves( m_pRoot, outLeaves, 0, bDeleteNodes );
    if ( bDeleteNodes ) {
        m_pRoot = NULL;
        m_iNodeCount = 0;
    }
    return iLeafCount;
}

template<typename Real>
inline Void TAABoxTree<Real>::Enumerate( HEnumerator * pEnumerator ) const {
    _rec_Enumerate( m_pRoot, pEnumerator );
}

template<typename Real>
inline Void TAABoxTree<Real>::Clone( TAABoxTree<Real> * pTarget, HCloner * pCloner ) const {
    _rec_Clone( m_pRoot, pTarget, NULL, 0, pCloner );
}

template<typename Real>
inline Void TAABoxTree<Real>::CollideRay( const TLine3<Real> & vRay, Real fMinDistance, Real fMaxDistance, HCollider * pCollider ) const {
    _rec_CollideRay( m_pRoot, vRay, fMinDistance, fMaxDistance, pCollider );
}
template<typename Real>
inline Void TAABoxTree<Real>::CollideAABox( const TBVAABox<Real> & bvAABox, HCollider * pCollider ) const {
    _rec_CollideAABox( m_pRoot, bvAABox, pCollider );
}
template<typename Real>
inline Void TAABoxTree<Real>::CollideTree( const TAABoxTree<Real> * pTree, HCollider * pCollider ) const {
    _rec_CollideTree( m_pRoot, pTree->m_pRoot, pCollider );
}
template<typename Real>
inline Void TAABoxTree<Real>::CollidePlanes( TPlane<Real> * arrPlanes, UInt iPlaneCount, HCollider * pCollider ) const {
    _rec_CollidePlanes( m_pRoot, 0, arrPlanes, iPlaneCount, pCollider );
}
template<typename Real>
inline Void TAABoxTree<Real>::CollideOcclusion( TPlane<Real> * arrPlanes, UInt iPlaneCount, const TVector3<Real> & vSortAxis,
                                                HCollider * pCollider ) const {
    _rec_CollideOcclusion( m_pRoot, 0, _ProjectMinimum(m_pRoot->bvAABox, vSortAxis),
                           arrPlanes, iPlaneCount, vSortAxis, pCollider );
}
template<typename Real>
inline Void TAABoxTree<Real>::CollideUser( HCollider * pCollider ) const {
    _rec_CollideUser( m_pRoot, pCollider );
}

/////////////////////////////////////////////////////////////////////////////////

template<typename Real>
Real TAABoxTree<Real>::_ProjectMinimum( const TBVAABox<Real> & bvAABox, const TVector3<Real> & vAxis )
{
    TVertex3<Real> vMinMax[2] = { bvAABox.GetMax(), bvAABox.GetMin() };
    TVector3<Real> vProj (
        vMinMax[(vAxis.X >= 0) ? 1 : 0].X,
        vMinMax[(vAxis.Y >= 0) ? 1 : 0].Y,
        vMinMax[(vAxis.Z >= 0) ? 1 : 0].Z
    );
    return ( vProj * vAxis );
}
template<typename Real>
TAABoxNode<Real> * TAABoxTree<Real>::_SelectChild( const TAABoxNode<Real> * pNode, const TBVAABox<Real> & dbvReference ) {
    TVector3<Real> vD = ( dbvReference.GetCenter() - pNode->pChild[0]->bvAABox.GetCenter() );
    Real fDistA = ( MathRealFn->Abs(vD.X) + MathRealFn->Abs(vD.Y) + MathRealFn->Abs(vD.Z) );

    vD = ( dbvReference.GetCenter() - pNode->pChild[1]->bvAABox.GetCenter() );
    Real fDistB = ( MathRealFn->Abs(vD.X) + MathRealFn->Abs(vD.Y) + MathRealFn->Abs(vD.Z) );

    return ( (fDistA < fDistB) ? pNode->pChild[0] : pNode->pChild[1] );
}

template<typename Real>
TAABoxNode<Real> * TAABoxTree<Real>::_SortNodes( TAABoxNode<Real> * pNode )
{
    Assert( pNode->IsNode() );

    TAABoxNode<Real> * pParent = pNode->pParent;
    if ( pParent <= pNode )
        return pNode;

    UInt iNodeIndex = ( pParent->pChild[0] == pNode ) ? 0 : 1;
    UInt iBrotherIndex = ( 1 - iNodeIndex );

    TAABoxNode<Real> * pBrother = pParent->pChild[iBrotherIndex];
    TAABoxNode<Real> * pGrandParent = pParent->pParent;

    UInt iParentIndex = ( pGrandParent->pChild[0] == pParent ) ? 0 : 1;

    if ( pGrandParent != NULL )
        pGrandParent->pChild[iParentIndex] = pNode;
    else
        m_pRoot = pNode;

    pBrother->pParent = pNode;
    pParent->pParent = pNode;
    pNode->pParent = pGrandParent;
    pParent->pChild[0] = pNode->pChild[0];
    pParent->pChild[1] = pNode->pChild[1];
    pNode->pChild[0]->pParent = pParent;
    pNode->pChild[1]->pParent = pParent;
    pNode->pChild[iNodeIndex] = pParent;
    pNode->pChild[iBrotherIndex] = pBrother;

    Swap< TBVAABox<Real> >( &(pParent->bvAABox), &(pNode->bvAABox) );

    return pParent;
}

template<typename Real>
Void TAABoxTree<Real>::_InsertLeaf( TAABoxNode<Real> * pNode, TAABoxNode<Real> * pLeaf )
{
    if ( m_pRoot == NULL ) {
        pLeaf->pParent = NULL;
        m_pRoot = pLeaf;
        return;
    }
    Assert( pNode != NULL );

    while( pNode->IsNode() )
        pNode = _SelectChild( pNode, pLeaf->bvAABox );

    TAABoxNode<Real> * pParent = pNode->pParent;

    TBVAABox<Real> dbvMergedShape = pLeaf->bvAABox;
    dbvMergedShape.Grow( pNode->bvAABox );

    _SelectMemory();
    TAABoxNode<Real> * pNewNode = New TAABoxNode<Real>( dbvMergedShape, pParent, NULL, NULL );
    _UnSelectMemory();
    ++m_iNodeCount;

    if ( pParent != NULL ) {
        if ( pParent->pChild[0] == pNode )
            pParent->pChild[0] = pNewNode;
        else
            pParent->pChild[1] = pNewNode;

        pNewNode->pChild[0] = pNode;
        pNewNode->pChild[1] = pLeaf;
        pNode->pParent = pNewNode;
        pLeaf->pParent = pNewNode;

        while( true ) {
            if ( pParent->bvAABox.Contains(pNewNode->bvAABox) )
                break;

            pParent->bvAABox = pParent->pChild[0]->bvAABox;
            pParent->bvAABox.Grow( pParent->pChild[1]->bvAABox );

            pNewNode = pParent;
            pParent = pNewNode->pParent;
            if ( pParent == NULL )
                break;
        }
    } else {
        pNewNode->pChild[0] = pNode;
        pNewNode->pChild[1] = pLeaf;
        pNode->pParent = pNewNode;
        pLeaf->pParent = pNewNode;
        m_pRoot = pNewNode;
    }
}
template<typename Real>
TAABoxNode<Real> * TAABoxTree<Real>::_RemoveLeaf( TAABoxNode<Real> * pLeaf )
{
    Assert( pLeaf != NULL );
    if ( m_pRoot == pLeaf ) {
        m_pRoot = NULL;
        return NULL;
    }

    TAABoxNode<Real> * pParent = pLeaf->pParent;
    TAABoxNode<Real> * pGrandParent = pParent->pParent;
    TAABoxNode<Real> * pBrother;
    if ( pParent->pChild[0] == pLeaf )
        pBrother = pParent->pChild[1];
    else
        pBrother = pParent->pChild[0];

    if ( pGrandParent != NULL ) {
        if ( pGrandParent->pChild[0] == pParent )
            pGrandParent->pChild[0] = pBrother;
        else
            pGrandParent->pChild[1] = pBrother;
        pBrother->pParent = pGrandParent;

        _SelectMemory();
        Delete( pParent );
        _UnSelectMemory();
        --m_iNodeCount;

        TBVAABox<Real> dbvOldShape;
        while( pGrandParent != NULL ) {
            dbvOldShape = pGrandParent->bvAABox;
            pGrandParent->bvAABox = pGrandParent->pChild[0]->bvAABox;
            pGrandParent->bvAABox.Grow( pGrandParent->pChild[1]->bvAABox );

            if ( pGrandParent->bvAABox == dbvOldShape )
                break;

            pGrandParent = pGrandParent->pParent;
        }

        return ( pGrandParent != NULL ) ? pGrandParent : m_pRoot;
    } else {
        m_pRoot = pBrother;
        pBrother->pParent = NULL;

        _SelectMemory();
        Delete( pParent );
        _UnSelectMemory();
        --m_iNodeCount;

        return m_pRoot;
    }
}

template<typename Real>
Void TAABoxTree<Real>::_rec_DestroyTree( TAABoxNode<Real> * pNode )
{
    if ( pNode->IsNode() ) {
        _rec_DestroyTree( pNode->pChild[0] );
        _rec_DestroyTree( pNode->pChild[1] );
    }

    _SelectMemory();
    Delete( pNode );
    _UnSelectMemory();
    --m_iNodeCount;
}

template<typename Real>
TAABoxNode<Real> * TAABoxTree<Real>::_BottomUp( TAABoxNode<Real> ** arrLeaves, UInt iLeafCount )
{
    TBVAABox<Real> dbvMergedShape;
    Real fMinSize, fSize;
    UInt iMinIndex[2];
    UInt i, j;

    TAABoxNode<Real> * pParent;

    m_iNodeCount += iLeafCount;

    while( iLeafCount > 1 ) {
        fMinSize = SCALAR_INFINITE;
        iMinIndex[0] = INVALID_OFFSET;
        iMinIndex[1] = INVALID_OFFSET;

        for( i = 0; i < iLeafCount; ++i ) {
            for( j = i+1; j < iLeafCount; ++j ) {
                dbvMergedShape = arrLeaves[i]->bvAABox;
                dbvMergedShape.Grow( arrLeaves[j]->bvAABox );
                fSize = dbvMergedShape.GetSizeEstimation();
                if ( fSize < fMinSize ) {
                    fMinSize = fSize;
                    iMinIndex[0] = i;
                    iMinIndex[1] = j;
                }
            }
        }

        dbvMergedShape = arrLeaves[iMinIndex[0]]->bvAABox;
        dbvMergedShape.Grow( arrLeaves[iMinIndex[1]]->bvAABox );

        _SelectMemory();
        pParent = New TAABoxNode<Real>( dbvMergedShape, NULL, arrLeaves[iMinIndex[0]], arrLeaves[iMinIndex[1]] );
        _UnSelectMemory();
        ++m_iNodeCount;

        arrLeaves[iMinIndex[0]]->pParent = pParent;
        arrLeaves[iMinIndex[1]]->pParent = pParent;

        arrLeaves[iMinIndex[0]] = pParent;
        arrLeaves[iMinIndex[1]] = arrLeaves[iLeafCount - 1];
        --iLeafCount;
    }

    return arrLeaves[0];
}
template<typename Real>
TAABoxNode<Real> * TAABoxTree<Real>::_rec_TopDown( TAABoxNode<Real> ** arrLeaves, UInt iLeafCount, UInt iThreshold )
{
    static const TVector3<Real> vAxis[3] = {
        TVector3<Real>::eI, TVector3<Real>::eJ, TVector3<Real>::eK
    };

    if ( iLeafCount <= 1 ) {
        ++m_iNodeCount;
        return arrLeaves[0];
    }

    if ( iLeafCount <= iThreshold )
        return _BottomUp( arrLeaves, iLeafCount );

    TVertex3<Real> vOrigin;
    TVector3<Real> vOffset;
    Real fDot;

    UInt iBestAxis = INVALID_OFFSET;
    UInt iBestMid = iLeafCount;
    UInt arrSplitCount[3][2] = { {0,0}, {0,0}, {0,0} };
    UInt i, iMid;

    TBVAABox<Real> dbvMergedShape = arrLeaves[0]->bvAABox;
    for( i = 1; i < iLeafCount; ++i )
        dbvMergedShape.Grow( arrLeaves[i]->bvAABox );

    vOrigin = dbvMergedShape.GetCenter();

    for( i = 0; i < iLeafCount; ++i ) {
        vOffset = ( arrLeaves[i]->bvAABox.GetCenter() - vOrigin );
        ++( arrSplitCount[0][((vOffset * vAxis[0]) > 0) ? 1 : 0] );
        ++( arrSplitCount[1][((vOffset * vAxis[1]) > 0) ? 1 : 0] );
        ++( arrSplitCount[2][((vOffset * vAxis[2]) > 0) ? 1 : 0] );
    }
    for( i = 0; i < 3; ++i ) {
        if ( (arrSplitCount[i][0] > 0) && (arrSplitCount[i][1] > 0) ) {
            if ( arrSplitCount[i][1] > arrSplitCount[i][0] )
                iMid = ( arrSplitCount[i][1] - arrSplitCount[i][0] );
            else
                iMid = ( arrSplitCount[i][0] - arrSplitCount[i][1] );
            if ( iMid < iBestMid ) {
                iBestMid = iMid;
                iBestAxis = i;
            }
        }
    }

    _SelectMemory();
    TAABoxNode<Real> ** arrNodes[2] = {
        New TAABoxNode<Real>*[iLeafCount],
        New TAABoxNode<Real>*[iLeafCount]
    };
    _UnSelectMemory();
    UInt iNodeCount[2] = { 0, 0 };

    if ( iBestAxis != INVALID_OFFSET ) {
        for( i = 0; i < iLeafCount; ++i ) {
            vOffset = ( arrLeaves[i]->bvAABox.GetCenter() - vOrigin );
            fDot = ( vOffset * vAxis[iBestAxis] );
            if ( fDot < (Real)0 )
                (arrNodes[0])[(iNodeCount[0])++] = arrLeaves[i];
            else
                (arrNodes[1])[(iNodeCount[1])++] = arrLeaves[i];
        }
    } else {
        for( i = 0; i < iLeafCount; ++i ) 
            (arrNodes[i&1])[(iNodeCount[i&1])++] = arrLeaves[i];
    }

    _SelectMemory();
    TAABoxNode<Real> * pNode = New TAABoxNode<Real>( dbvMergedShape, NULL, NULL, NULL );
    _UnSelectMemory();
    ++m_iNodeCount;

    pNode->pChild[0] = _rec_TopDown( arrNodes[0], iNodeCount[0], iThreshold );
    pNode->pChild[1] = _rec_TopDown( arrNodes[1], iNodeCount[1], iThreshold );
    pNode->pChild[0]->pParent = pNode;
    pNode->pChild[1]->pParent = pNode;

    _SelectMemory();
    DeleteA( arrNodes[1] );
    DeleteA( arrNodes[0] );
    _UnSelectMemory();

    return pNode;
}

template<typename Real>
UInt TAABoxTree<Real>::_rec_GetMaxDepth( const TAABoxNode<Real> * pNode )
{
    if ( pNode == NULL )
        return 0;
    if ( pNode->IsLeaf() )
        return 1;

    UInt iDepthLeft = _rec_GetMaxDepth( pNode->pChild[0] );
    UInt iDepthRight = _rec_GetMaxDepth( pNode->pChild[1] );

    return ( 1 + Max<UInt>(iDepthLeft, iDepthRight) );
}
template<typename Real>
UInt TAABoxTree<Real>::_rec_GetLeafCount( const TAABoxNode<Real> * pNode )
{
    if ( pNode == NULL )
        return 0;
    if ( pNode->IsLeaf() )
        return 1;

    UInt iCountLeft = _rec_GetLeafCount( pNode->pChild[0] );
    UInt iCountRight = _rec_GetLeafCount( pNode->pChild[1] );

    return ( iCountLeft + iCountRight );
}
template<typename Real>
UInt TAABoxTree<Real>::_rec_ExtractLeaves( TAABoxNode<Real> * pNode, TAABoxNode<Real> ** outLeaves, UInt iLeafCount,
                                           Bool bDeleteNodes )
{
    if ( pNode == NULL )
        return iLeafCount;

    if ( pNode->IsNode() ) {
        iLeafCount = _rec_ExtractLeaves( pNode->pChild[0], outLeaves, iLeafCount, bDeleteNodes );
        iLeafCount = _rec_ExtractLeaves( pNode->pChild[1], outLeaves, iLeafCount, bDeleteNodes );

        if ( bDeleteNodes ) {
            _SelectMemory();
            Delete( pNode );
            _UnSelectMemory();
            --m_iNodeCount;
        }
    } else {
        outLeaves[iLeafCount] = pNode;
        ++iLeafCount;
    }

    return iLeafCount;
}

template<typename Real>
Void TAABoxTree<Real>::_rec_Enumerate( const TAABoxNode<Real> * pNode, HEnumerator * pEnumerator )
{
    if ( pNode->IsNode() ) {
        pEnumerator->EnumerateNode( pNode );
        _rec_Enumerate( pNode->pChild[0], pEnumerator );
        _rec_Enumerate( pNode->pChild[1], pEnumerator );
    } else
        pEnumerator->EnumerateLeaf( pNode );
}

template<typename Real>
Void TAABoxTree<Real>::_rec_Clone( const TAABoxNode<Real> * pNode,
                                   TAABoxTree<Real> * pTarget, TAABoxNode<Real> * pTargetParent, UInt iTargetParentIndex,
                                   HCloner * pCloner )
{
    if ( pNode == NULL )
        return;

    pTarget->_SelectMemory();
    TAABoxNode<Real> * pClonedNode = New TAABoxNode<Real>( *pNode );
    pTarget->_UnSelectMemory();
    ++(pTarget->m_iNodeCount);

    if ( pTargetParent == NULL )
        pTarget->m_pRoot = pClonedNode;
    else
        pTargetParent->pChild[iTargetParentIndex] = pClonedNode;

    if ( pNode->IsNode() ) {
        _rec_Clone( pNode->pChild[0], pTarget, pClonedNode, 0, pCloner );
        _rec_Clone( pNode->pChild[1], pTarget, pClonedNode, 1, pCloner );
    } else
        pCloner->CloneLeaf( pClonedNode );
}

template<typename Real>
Void TAABoxTree<Real>::_rec_CollideRay( const TAABoxNode<Real> * pNode,
                                        const TLine3<Real> & vRay, Real fMinDistance, Real fMaxDistance,
                                        HCollider * pCollider )
{
    if ( pNode == NULL )
        return;

    Real fMin, fMax;
    Bool bIntersect = pNode->bvAABox.RayCast( vRay, &fMin, &fMax );
    if ( !bIntersect )
        return;
    if ( fMax < fMinDistance || fMin > fMaxDistance )
        return;

    if ( pNode->IsNode() ) {
        _rec_CollideRay( pNode->pChild[0], vRay, fMinDistance, fMaxDistance, pCollider );
        _rec_CollideRay( pNode->pChild[1], vRay, fMinDistance, fMaxDistance, pCollider );
    } else
        pCollider->Process( pNode );
}
template<typename Real>
Void TAABoxTree<Real>::_rec_CollideAABox( const TAABoxNode<Real> * pNode, const TBVAABox<Real> & bvAABox,
                                          HCollider * pCollider )
{
    if ( pNode == NULL )
        return;

    Bool bIntersect = pNode->bvAABox.Intersect( bvAABox );
    if ( !bIntersect )
        return;

    if ( pNode->IsNode() ) {
        _rec_CollideAABox( pNode->pChild[0], bvAABox, pCollider );
        _rec_CollideAABox( pNode->pChild[1], bvAABox, pCollider );
    } else
        pCollider->Process( pNode );
}
template<typename Real>
Void TAABoxTree<Real>::_rec_CollideTree( const TAABoxNode<Real> * pNodeA, const TAABoxNode<Real> * pNodeB,
                                         HCollider * pCollider )
{
    if ( pNodeA == NULL || pNodeB == NULL )
        return;

    if ( pNodeA == pNodeB ) {
        if ( pNodeA->IsNode() ) {
            _rec_CollideTree( pNodeA->pChild[0], pNodeA->pChild[0], pCollider );
            _rec_CollideTree( pNodeA->pChild[1], pNodeA->pChild[1], pCollider );
            _rec_CollideTree( pNodeA->pChild[0], pNodeA->pChild[1], pCollider );
        }
        return;
    }

    Bool bIntersect = pNodeA->bvAABox.Intersect( pNodeB->bvAABox );
    if ( !bIntersect )
        return;

    if ( pNodeA->IsNode() ) {
        if ( pNodeB->IsNode() ) {
            _rec_CollideTree( pNodeA->pChild[0], pNodeB->pChild[0], pCollider );
            _rec_CollideTree( pNodeA->pChild[1], pNodeB->pChild[0], pCollider );
            _rec_CollideTree( pNodeA->pChild[0], pNodeB->pChild[1], pCollider );
            _rec_CollideTree( pNodeA->pChild[1], pNodeB->pChild[1], pCollider );
        } else {
            _rec_CollideTree( pNodeA->pChild[0], pNodeB, pCollider );
            _rec_CollideTree( pNodeA->pChild[1], pNodeB, pCollider );
        }
    } else {
        if ( pNodeB->IsNode() ) {
            _rec_CollideTree( pNodeA, pNodeB->pChild[0], pCollider );
            _rec_CollideTree( pNodeA, pNodeB->pChild[1], pCollider );
        } else
            pCollider->Process( pNodeA, pNodeB );
    }
}
template<typename Real>
Void TAABoxTree<Real>::_rec_CollidePlanes( const TAABoxNode<Real> * pNode, UInt iMask,
                                           TPlane<Real> * arrPlanes, UInt iPlaneCount,
                                           HCollider * pCollider )
{
    static const UInt iMaxPlanes = ( sizeof(UInt) << 3 );
    Assert( iPlaneCount <= iMaxPlanes );

    if ( pNode == NULL )
        return;

    Int iSide;
    UInt iTestMask = 1;

    for( UInt i = 0; i < iPlaneCount; ++i ) {
        if ( (iMask & iTestMask) == 0 ) {
            iSide = pNode->bvAABox.WhichSide( arrPlanes[i] );
            if ( iSide < 0 )
                return;
            if ( iSide > 0 )
                iMask |= iTestMask;
        }
        iTestMask <<= 1;
    }

    UInt iInsideMask = ( 1 << iPlaneCount ) - 1;

    if ( (iMask != iInsideMask) && pNode->IsNode() ) {
        _rec_CollidePlanes( pNode->pChild[0], iMask, arrPlanes, iPlaneCount, pCollider );
        _rec_CollidePlanes( pNode->pChild[1], iMask, arrPlanes, iPlaneCount, pCollider );
    } else {
        if ( pCollider->AllLeaves(pNode) )
            _rec_CollidePlanes_AllLeaves( pNode, pCollider );
    }
}
template<typename Real>
Void TAABoxTree<Real>::_rec_CollideOcclusion( const TAABoxNode<Real> * pNode, UInt iMask, Real fProjection,
                                              TPlane<Real> * arrPlanes, UInt iPlaneCount, const TVector3<Real> & vSortAxis,
                                              HCollider * pCollider )
{
    static const UInt iMaxPlanes = ( sizeof(UInt) << 3 );
    Assert( iPlaneCount < iMaxPlanes );

    if ( pNode == NULL )
        return;

    UInt iInsideMask = ( 1 << iPlaneCount ) - 1;

    if ( iMask != iInsideMask ) {
        Int iSide;
        UInt iTestMask = 1;

        for( UInt i = 0; i < iPlaneCount; ++i ) {
            if ( (iMask & iTestMask) == 0 ) {
                iSide = pNode->bvAABox.WhichSide( arrPlanes[i] );
                if ( iSide < 0 )
                    return;
                if ( iSide > 0 )
                    iMask |= iTestMask;
            }
            iTestMask <<= 1;
        }
    }

    if ( pCollider->Descent(pNode) ) {
        if ( pNode->IsNode() ) {
            Real arrValues[2] = {
                _ProjectMinimum( pNode->pChild[0]->bvAABox, vSortAxis ),
                _ProjectMinimum( pNode->pChild[1]->bvAABox, vSortAxis )
            };
            const UInt iChild = ( arrValues[0] < arrValues[1] ) ? 1 : 0;

            _rec_CollideOcclusion( pNode->pChild[iChild], iMask, arrValues[iChild], arrPlanes, iPlaneCount, vSortAxis, pCollider );
            _rec_CollideOcclusion( pNode->pChild[1 - iChild], iMask, arrValues[1 - iChild], arrPlanes, iPlaneCount, vSortAxis, pCollider );
        } else
            pCollider->Process( pNode, fProjection );
    }
}
template<typename Real>
Void TAABoxTree<Real>::_rec_CollideUser( const TAABoxNode<Real> * pNode, HCollider * pCollider )
{
    if ( pNode == NULL )
        return;

    if ( pCollider->Descent(pNode) ) {
        if ( pNode->IsNode() ) {
            _rec_CollideUser( pNode->pChild[0], pCollider );
            _rec_CollideUser( pNode->pChild[1], pCollider );
        } else
            pCollider->Process( pNode );
    }
}

template<typename Real>
Void TAABoxTree<Real>::_rec_CollidePlanes_AllLeaves( const TAABoxNode<Real> * pNode, HCollider * pCollider )
{
    if ( pNode->IsNode() ) {
        _rec_CollidePlanes_AllLeaves( pNode->pChild[0], pCollider );
        _rec_CollidePlanes_AllLeaves( pNode->pChild[1], pCollider );
    } else
        pCollider->Process( pNode );
}

