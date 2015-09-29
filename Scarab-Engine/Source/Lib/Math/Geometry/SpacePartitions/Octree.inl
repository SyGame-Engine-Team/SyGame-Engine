/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/SpacePartitions/Octree.inl
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
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TOctreeBaseNode implementation
template<typename Real>
TOctreeBaseNode<Real>::TOctreeBaseNode( const TBVAABox<Real> & bvNodeAABox ):
    bvAABox( bvNodeAABox )
{
    // nothing to do
}
template<typename Real>
TOctreeBaseNode<Real>::~TOctreeBaseNode()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// TOctreeNode implementation
template<typename Real>
TOctreeNode<Real>::TOctreeNode( const TBVAABox<Real> & bvNodeAABox ):
    TOctreeBaseNode<Real>( bvNodeAABox )
{
    for( UInt i = 0; i < 8; ++i )
        pChild[i] = NULL;
}
template<typename Real>
TOctreeNode<Real>::~TOctreeNode()
{
    // nothing to do
}

template<typename Real>
inline Bool TOctreeNode<Real>::IsNode() const {
    return true;
}
template<typename Real>
inline Bool TOctreeNode<Real>::IsLeaf() const {
    return false;
}

/////////////////////////////////////////////////////////////////////////////////
// TOctreeLeaf implementation
template<typename Real>
TOctreeLeaf<Real>::TOctreeLeaf( const TBVAABox<Real> & bvNodeAABox ):
    TOctreeBaseNode<Real>( bvNodeAABox )
{
    iDataCount = 0;
}
template<typename Real>
TOctreeLeaf<Real>::~TOctreeLeaf()
{
    // nothing to do
}

template<typename Real>
inline Bool TOctreeLeaf<Real>::IsNode() const {
    return false;
}
template<typename Real>
inline Bool TOctreeLeaf<Real>::IsLeaf() const {
    return true;
}

/////////////////////////////////////////////////////////////////////////////////
// TOctree implementation
template<typename Real>
TOctree<Real>::TOctree():
    Datastruct(), m_bvRootBox( TVertex3<Real>::Null, (Real)100.0f )
{
    m_pRoot = NULL;
    m_iNodeCount = 0;
    m_iLeafCount = 0;
}
template<typename Real>
TOctree<Real>::~TOctree()
{
    // nothing to do
    // Call Destroy explicitly
}

template<typename Real>
Void TOctree<Real>::Create()
{
    Assert( m_pRoot == NULL );
}
template<typename Real>
Void TOctree<Real>::Destroy()
{
    Clear();
}

template<typename Real>
inline UInt TOctree<Real>::MemorySize() const {
    return ( (m_iNodeCount * sizeof(TOctreeNode<Real>)) +
             (m_iLeafCount * sizeof(TOctreeLeaf<Real>)) );
}
template<typename Real>
inline UInt TOctree<Real>::Count() const {
    return ( m_iNodeCount + m_iLeafCount );
}

template<typename Real>
Void TOctree<Real>::Clear()
{
    if ( m_pRoot != NULL )
        _rec_DestroyTree( m_pRoot );
    m_pRoot = NULL;
    m_iNodeCount = 0;
    m_iLeafCount = 0;
}

template<typename Real>
inline Void TOctree<Real>::SetRootBox( const TBVAABox<Real> & bvAABox ) {
    Assert( m_pRoot == NULL );
    m_bvRootBox = bvAABox;
}

template<typename Real>
Void TOctree<Real>::Insert( const TBVAABox<Real> & bvAABox, Void * pData )
{
    if ( m_pRoot == NULL ) {
        _SelectMemory();
        TOctreeLeaf<Real> * pLeaf = New TOctreeLeaf<Real>( m_bvRootBox );
        _UnSelectMemory();
        ++m_iLeafCount;

        pLeaf->arrAABoxes[0] = bvAABox;
        pLeaf->arrDatas[0] = pData;
        pLeaf->iDataCount = 1;
        m_pRoot = pLeaf;

        return;
    }

    _rec_Insert( &m_pRoot, bvAABox, pData );
}

template<typename Real>
inline Void TOctree<Real>::Enumerate( HEnumerator * pEnumerator ) const {
    _rec_Enumerate( m_pRoot, pEnumerator );
}

template<typename Real>
inline Void TOctree<Real>::CollideRay( const TLine3<Real> & vRay, Real fMinDistance, Real fMaxDistance, HCollider * pCollider ) const {
    _rec_CollideRay( m_pRoot, vRay, fMinDistance, fMaxDistance, pCollider );
}
template<typename Real>
inline Void TOctree<Real>::CollideAABox( const TBVAABox<Real> & bvAABox, HCollider * pCollider ) const {
    _rec_CollideAABox( m_pRoot, bvAABox, pCollider );
}
template<typename Real>
inline Void TOctree<Real>::CollideTree( const TOctree<Real> * pTree, HCollider * pCollider ) const {
    _rec_CollideTree( m_pRoot, pTree->m_pRoot, pCollider );
}

/////////////////////////////////////////////////////////////////////////////////

template<typename Real>
UInt TOctree<Real>::_SelectChildren( const TOctreeNode<Real> * pNode, const TBVAABox<Real> & bvAABox )
{
    TVertex3<Real> vNodeMin = pNode->bvAABox.GetMin();
    TVertex3<Real> vNodeMax = pNode->bvAABox.GetMax();

    TVertex3<Real> vBoxMin = bvAABox.GetMin();
    TVertex3<Real> vBoxMax = bvAABox.GetMax();

    static const UInt arrSideMasks[3] = { 0x55, 0x33, 0x0f };
    Real fMid;
    UInt iResult = 0xff;

    for( UInt i = 0; i < 3; ++i ) {
        fMid = ( vNodeMin[i] + vNodeMax[i] ) * (Real)0.5f;
        if ( vBoxMax[i] < fMid ) // Negative children on axis i
            iResult &= arrSideMasks[i];
        else if ( vBoxMin[i] > fMid ) // Positive children on axis i
            iResult &= ~( arrSideMasks[i] );
    }

    return iResult;
}

template<typename Real>
Void TOctree<Real>::_rec_SplitLeafToNode( TOctreeNode<Real> ** ppNode, const TOctreeLeaf<Real> * pLeaf )
{
    TOctreeNode<Real> * pNode = *ppNode;

    // Build children
    TVertex3<Real> vNodeMin = pNode->bvAABox.GetMin();
    TVertex3<Real> vNodeMax = pNode->bvAABox.GetMax();

    TVertex3<Real> vMin, vMax;
    Real arrMids[3] = {
        ( vNodeMin.X + vNodeMax.X ) * (Real)0.5f,
        ( vNodeMin.Y + vNodeMax.Y ) * (Real)0.5f,
        ( vNodeMin.Z + vNodeMax.Z ) * (Real)0.5f
    };
    UInt i, j;

    for( i = 0; i < 8; ++i ) {
        for( j = 0; j < 3; ++j ) {
            if ( i & (1 << j) ) {
                vMin[j] = arrMids[j];
                vMax[j] = vNodeMax[j];
            } else {
                vMin[j] = vNodeMin[j];
                vMax[j] = arrMids[j];
            }
        }

        _SelectMemory();
        pNode->pChild[i] = New TOctreeLeaf<Real>( TBVAABox<Real>(vMin, vMax) );
        _UnSelectMemory();
        ++m_iLeafCount;
    }

    // Insert back leaf's content
    for( i = 0; i < pLeaf->iDataCount; ++i )
        _rec_Insert( (TOctreeBaseNode<Real>**)ppNode, pLeaf->arrAABoxes[i], pLeaf->arrDatas[i] );
}
template<typename Real>
Void TOctree<Real>::_rec_Insert( TOctreeBaseNode<Real> ** ppNode, const TBVAABox<Real> & bvAABox, Void * pData )
{
    TOctreeBaseNode<Real> * pBaseNode = *ppNode;

    if ( pBaseNode == NULL )
        return;

    // Leaf case
    if ( pBaseNode->IsLeaf() ) {
        TOctreeLeaf<Real> * pLeaf = (TOctreeLeaf<Real>*)pBaseNode;

        // Leaf has room case
        if ( pLeaf->iDataCount < OCTREE_LEAF_CAPACITY ) {
            pLeaf->arrAABoxes[pLeaf->iDataCount] = bvAABox;
            pLeaf->arrDatas[pLeaf->iDataCount] = pData;
            ++(pLeaf->iDataCount);

            return;
        }

        // Split case
        _SelectMemory();
        *ppNode = New TOctreeNode<Real>( pLeaf->bvAABox );
        _UnSelectMemory();
        ++m_iNodeCount;

        _rec_SplitLeafToNode( (TOctreeNode<Real>**)ppNode, pLeaf );

        _SelectMemory();
        Delete( pLeaf );
        _UnSelectMemory();
        --m_iLeafCount;

        _rec_Insert( ppNode, bvAABox, pData );

        return;
    }

    // Node case
    TOctreeNode<Real> * pNode = (TOctreeNode<Real>*)pBaseNode;

    UInt iChildren = _SelectChildren( pNode, bvAABox );
    for( UInt i = 0; i < 8; ++i ) {
        if ( iChildren & (1 << i) )
            _rec_Insert( pNode->pChild + i, bvAABox, pData );
    }
}

template<typename Real>
Void TOctree<Real>::_rec_DestroyTree( TOctreeBaseNode<Real> * pNode )
{
    if ( pNode == NULL )
        return;

    if ( pNode->IsNode() ) {
        TOctreeNode<Real> * pDelete = (TOctreeNode<Real>*)pNode;

        for( UInt i = 0; i < 8; ++i )
            _rec_DestroyTree( pDelete->pChild[i] );

        _SelectMemory();
        Delete( pDelete );
        _UnSelectMemory();
        --m_iNodeCount;
    } else {
        TOctreeLeaf<Real> * pDelete = (TOctreeLeaf<Real>*)pNode;

        _SelectMemory();
        Delete( pDelete );
        _UnSelectMemory();
        --m_iLeafCount;
    }
}

template<typename Real>
Void TOctree<Real>::_rec_Enumerate( const TOctreeBaseNode<Real> * pNode, HEnumerator * pEnumerator )
{
    if ( pNode == NULL )
        return;

    // Leaf case
    if ( pNode->IsLeaf() ) {
        pEnumerator->EnumerateLeaf( (TOctreeLeaf<Real>*)pNode );
        return;
    }

    // Node case
    TOctreeNode<Real> * pCurNode = (TOctreeNode<Real>*)pNode;

    pEnumerator->EnumerateNode( pCurNode );
    for( UInt i = 0; i < 8; ++i )
        _rec_Enumerate( pCurNode->pChild[i], pEnumerator );
}

template<typename Real>
Void TOctree<Real>::_rec_CollideRay( const TOctreeBaseNode<Real> * pNode,
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

    // Leaf case
    if ( pNode->IsLeaf() ) {
        pCollider->Process( (TOctreeLeaf<Real>*)pNode );
        return;
    }

    // Node case
    TOctreeNode<Real> * pCurNode = (TOctreeNode<Real>*)pNode;

    for( UInt i = 0; i < 8; ++i )
        _rec_CollideRay( pCurNode->pChild[i], vRay, fMinDistance, fMaxDistance, pCollider );
}
template<typename Real>
Void TOctree<Real>::_rec_CollideAABox( const TOctreeBaseNode<Real> * pNode, const TBVAABox<Real> & bvAABox,
                                             HCollider * pCollider )
{
    if ( pNode == NULL )
        return;

    Bool bIntersect = pNode->bvAABox.Intersect( bvAABox );
    if ( !bIntersect )
        return;

    // Leaf case
    if ( pNode->IsLeaf() ) {
        pCollider->Process( (TOctreeLeaf<Real>*)pNode );
        return;
    }

    // Node case
    TOctreeNode<Real> * pCurNode = (TOctreeNode<Real>*)pNode;

    UInt iChildren = _SelectChildren( pCurNode, bvAABox );
    for( UInt i = 0; i < 8; ++i ) {
        if ( iChildren & (1 << i) )
            _rec_CollideAABox( pCurNode->pChild[i], bvAABox, pCollider );
    }
}
template<typename Real>
Void TOctree<Real>::_rec_CollideTree( const TOctreeBaseNode<Real> * pNodeA, const TOctreeBaseNode<Real> * pNodeB,
                                            HCollider * pCollider )
{
    if ( pNodeA == NULL || pNodeB == NULL )
        return;

    Bool bIntersect = pNodeA->bvAABox.Intersect( pNodeB->bvAABox );
    if ( !bIntersect )
        return;

    // Leaf case
    if ( pNodeA->IsLeaf() && pNodeB->IsLeaf() ) {
        pCollider->Process( (TOctreeLeaf<Real>*)pNodeA, (TOctreeLeaf<Real>*)pNodeB );
        return;
    }

    // Leaf/Node cases
    if ( pNodeA->IsLeaf() ) {
        TOctreeNode<Real> * pCurNodeB = (TOctreeNode<Real>*)pNodeB;

        UInt iChildren = _SelectChildren( pCurNodeB, pNodeA->bvAABox );
        for( UInt i = 0; i < 8; ++i ) {
            if ( iChildren & (1 << i) )
                _rec_CollideTree( pNodeA, pCurNodeB->pChild[i], pCollider );
        }

        return;
    }
    if ( pNodeB->IsLeaf() ) {
        TOctreeNode<Real> * pCurNodeA = (TOctreeNode<Real>*)pNodeA;

        UInt iChildren = _SelectChildren( pCurNodeA, pNodeB->bvAABox );
        for( UInt i = 0; i < 8; ++i ) {
            if ( iChildren & (1 << i) )
                _rec_CollideTree( pCurNodeA->pChild[i], pNodeB, pCollider );
        }

        return;
    }

    // Node case
    TOctreeNode<Real> * pCurNodeA = (TOctreeNode<Real>*)pNodeA;
    TOctreeNode<Real> * pCurNodeB = (TOctreeNode<Real>*)pNodeB;

    UInt iChildrenA = _SelectChildren( pCurNodeA, pCurNodeB->bvAABox );
    UInt iChildrenB;
    for( UInt i = 0; i < 8; ++i ) {
        if ( iChildrenA & (1 << i) ) {
            iChildrenB = _SelectChildren( pCurNodeB, pCurNodeA->pChild[i]->bvAABox );
            for( UInt j = 0; j < 8; ++j ) {
                if ( iChildrenB & (1 << j) )
                    _rec_CollideTree( pCurNodeA->pChild[i], pCurNodeB->pChild[j], pCollider );
            }
        }
    }
}


