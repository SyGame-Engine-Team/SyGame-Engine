/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/CollisionDetection/NarrowPhase/GJK/GJK.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Narrow-Phase : Convex, Gilbert-Johnson-Keerti algorithm coupled
//               with Expanding-Polytope-Algorithm or Minkowski-Portal-Refinement
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// GJK implementation

/////////////////////////////////////////////////////////////////////////////////

template<typename T, UInt SIZE>
inline GJK::_PoolAllocator<T,SIZE>::_PoolAllocator() {
    _Reset();
}
template<typename T, UInt SIZE>
inline Void GJK::_PoolAllocator<T,SIZE>::_Reset() {
    iNextFree = 0;
    iPoolCount = 0;
    for( UInt i = 0; i < SIZE; ++i )
        *( (UInt*)(arrPool + i) ) = ( i + 1 );
    *( (UInt*)(arrPool + SIZE - 1) ) = INVALID_OFFSET;
}

template<typename T, UInt SIZE>
inline T * GJK::_PoolAllocator<T,SIZE>::_Allocate() {
    Assert( iPoolCount < SIZE );
    T * pPtr = ( arrPool + iNextFree );
    iNextFree = *( (UInt*)pPtr );
    ++iPoolCount;
    return pPtr;
}
template<typename T, UInt SIZE>
inline Void GJK::_PoolAllocator<T,SIZE>::_Free( T * pPtr ) {
    Assert( iPoolCount > 0 );
    *( (UInt*)pPtr ) = iNextFree;
    iNextFree = ( pPtr - arrPool );
    --iPoolCount;
}

/////////////////////////////////////////////////////////////////////////////////

inline GJK::_SupportVertex & GJK::_SupportVertex::operator=( const _SupportVertex & rhs ) {
    vDir = rhs.vDir;
    vSupport = rhs.vSupport;
    return (*this);
}

inline Void GJK::_GetSupportA( Vector3 * outSupport, const Vector3 & vDir ) const {
    Vertex3 vA;
    m_pShapeA->QueryExtremalVertices( vDir, &vA, NULL );
    *outSupport = vA.ToVector();
}
inline Void GJK::_GetSupportB( Vector3 * outSupport, const Vector3 & vDir ) const {
    Vertex3 vB;
    m_pShapeB->QueryExtremalVertices( vDir, &vB, NULL );
    *outSupport = vB.ToVector();
}
inline Void GJK::_GetSupportVertex( _SupportVertex * outSV, const Vector3 & vDir ) const {
    outSV->vDir = vDir;
    outSV->vDir.Normalize();

    Vertex3 vA, vB;
    m_pShapeA->QueryExtremalVertices( outSV->vDir, &vA, NULL );
    m_pShapeB->QueryExtremalVertices( -(outSV->vDir), &vB, NULL );
    outSV->vSupport = ( vA - vB );
}

inline GJK::_MPRSupportVertex & GJK::_MPRSupportVertex::operator=( const _MPRSupportVertex & rhs ) {
    vPointM = rhs.vPointM;
    vPointA = rhs.vPointA;
    vPointB = rhs.vPointB;
    return (*this);
}

inline Bool GJK::_MPR_CanContainOrigin( const _MPRSupportVertex * pSupport, const Vector3 & vDir ) const {
    Scalar fDot = ( pSupport->vPointM * vDir );
    return ( fDot >= -SCALAR_EPSILON );
}
inline Void GJK::_MPR_GetOrigin( _MPRSupportVertex * outOrigin ) const {
    outOrigin->vPointA = m_pShapeA->GetTransform().GetTranslate();
    outOrigin->vPointB = m_pShapeB->GetTransform().GetTranslate();
    outOrigin->vPointM = ( outOrigin->vPointA - outOrigin->vPointB );
}
inline Void GJK::_MPR_GetSupport( _MPRSupportVertex * outSupport, const Vector3 & vDir ) const {
    Vertex3 vA, vB;
    m_pShapeA->QueryExtremalVertices( vDir, &vA, NULL );
    m_pShapeB->QueryExtremalVertices( -vDir, &vB, NULL );
    outSupport->vPointA = vA.ToVector();
    outSupport->vPointB = vB.ToVector();
    outSupport->vPointM = ( outSupport->vPointA - outSupport->vPointB );
}

/////////////////////////////////////////////////////////////////////////////////

inline Void GJK::_Simplex::_Initialize() {
    iRank = 0;
    for( UInt i = 0; i < 4; ++i ) {
        arrVertices[i] = NULL;
        arrWeights[i] = 0.0f;
    }
}

inline Void GJK::_PushVertex( _Simplex * pSimplex, const Vector3 & vDir ) {
    pSimplex->arrVertices[pSimplex->iRank] = m_hGJKSolver.hSVAllocator._Allocate();
    pSimplex->arrWeights[pSimplex->iRank] = 0.0f;
    _GetSupportVertex( pSimplex->arrVertices[pSimplex->iRank], vDir );
    ++(pSimplex->iRank);
}
inline Void GJK::_PopVertex( _Simplex * pSimplex ) {
    --(pSimplex->iRank);
    m_hGJKSolver.hSVAllocator._Free( pSimplex->arrVertices[pSimplex->iRank] );
    pSimplex->arrVertices[pSimplex->iRank] = NULL;
}

inline Void GJK::_MPRSimplex::_Initialize() {
    iRank = 0;
    for( UInt i = 0; i < 4; ++i )
        arrVertices[i] = NULL;
}

inline Void GJK::_MPRSimplex::SwapSupports( UInt iIndex1, UInt iIndex2 ) {
    Assert( iIndex1 < iRank && iIndex2 < iRank );
    _MPRSupportVertex * pSwapSupport = arrVertices[iIndex1];
    arrVertices[iIndex1] = arrVertices[iIndex2];
    arrVertices[iIndex2] = pSwapSupport;
}

inline Void GJK::_MPRSimplex::GetPortalDir( Vector3 * outPortalDir ) const {
    Assert( iRank == 4 );
    Vector3 v21 = ( arrVertices[2]->vPointM - arrVertices[1]->vPointM );
    Vector3 v31 = ( arrVertices[3]->vPointM - arrVertices[1]->vPointM );
    *outPortalDir = ( v21 ^ v31 );
    outPortalDir->Normalize();
}
inline Bool GJK::_MPRSimplex::ContainsOrigin( const Vector3 & vPortalDir ) const {
    Assert( iRank >= 2 );
    Scalar fDot = ( vPortalDir * arrVertices[1]->vPointM );
    return ( fDot >= -SCALAR_EPSILON );
}
inline Bool GJK::_MPRSimplex::TestTolerance( const _MPRSupportVertex * pSupport, const Vector3 & vPortalDir ) const {
    Assert( iRank == 4 );
    Scalar fDot1 = ( arrVertices[1]->vPointM * vPortalDir );
    Scalar fDot2 = ( arrVertices[2]->vPointM * vPortalDir );
    Scalar fDot3 = ( arrVertices[3]->vPointM * vPortalDir );
    Scalar fDot4 = ( pSupport->vPointM * vPortalDir );
    Scalar f41 = ( fDot4 - fDot1 );
    Scalar f42 = ( fDot4 - fDot2 );
    Scalar f43 = ( fDot4 - fDot3 );
    Scalar fMinDot = Min<Scalar>( f41, f42 );
    fMinDot = Min<Scalar>( fMinDot, f43 );
    return ( _MPR_TestEquality(fMinDot, MPR_ACCURACY) || fMinDot < MPR_ACCURACY );
}

inline Void GJK::_MPR_ExpandPortal( _MPRSimplex * pSimplex, _MPRSupportVertex * pSupport ) {
    Assert( pSimplex->iRank == 4 );
    Vector3 vCross = ( pSupport->vPointM ^ pSimplex->arrVertices[0]->vPointM );
    Scalar fDot = ( pSimplex->arrVertices[1]->vPointM * vCross );
    if ( fDot > 0.0f ) {
        fDot = ( pSimplex->arrVertices[2]->vPointM * vCross );
        if ( fDot > 0.0f ) {
            m_hMPRSolver.hSVAllocator._Free( pSimplex->arrVertices[1] );
            pSimplex->arrVertices[1] = pSupport;
        } else {
            m_hMPRSolver.hSVAllocator._Free( pSimplex->arrVertices[3] );
            pSimplex->arrVertices[3] = pSupport;
        }
    } else {
        fDot = ( pSimplex->arrVertices[3]->vPointM * vCross );
        if ( fDot > 0.0f ) {
            m_hMPRSolver.hSVAllocator._Free( pSimplex->arrVertices[2] );
            pSimplex->arrVertices[2] = pSupport;
        } else {
            m_hMPRSolver.hSVAllocator._Free( pSimplex->arrVertices[1] );
            pSimplex->arrVertices[1] = pSupport;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////

inline Void GJK::_SimplexFacet::_Initialize() {
    iPass = 0;
    vNormal = Vector3::Null;
    fDistance = 0.0f;
    for( UInt i = 0; i < 3; ++i ) {
        arrVertices[i] = NULL;
        arrEdgeLinks[i] = 0;
        arrFacetLinks[i] = NULL;
    }
    arrListLinks[0] = NULL;
    arrListLinks[1] = NULL;
}

inline GJK::_SimplexFacet & GJK::_SimplexFacet::operator=( const _SimplexFacet & rhs ) {
    iPass = rhs.iPass;
    vNormal = rhs.vNormal;
    fDistance = rhs.fDistance;
    for( UInt i = 0; i < 3; ++i ) {
        arrVertices[i] = rhs.arrVertices[i];
        arrEdgeLinks[i] = rhs.arrEdgeLinks[i];
        arrFacetLinks[i] = rhs.arrFacetLinks[i];
    }
    arrListLinks[0] = rhs.arrListLinks[0];
    arrListLinks[1] = rhs.arrListLinks[1];
    return (*this);
}

inline Void GJK::_SimplexFacet::_Link( UInt iIndexA, struct _simplex_facet * pFacetB, UInt iIndexB ) {
    arrEdgeLinks[iIndexA] = (Byte)iIndexB;
    arrFacetLinks[iIndexA] = pFacetB;
    pFacetB->arrEdgeLinks[iIndexB] = (Byte)iIndexA;
    pFacetB->arrFacetLinks[iIndexB] = this;
}

/////////////////////////////////////////////////////////////////////////////////

inline Void GJK::_SimplexFacetList::_Initialize() {
    iCount = 0;
    pHead = NULL;
}

inline Void GJK::_SimplexFacetList::_Append( _SimplexFacet * pFacet ) {
    pFacet->arrListLinks[0] = NULL;
    pFacet->arrListLinks[1] = pHead;
    if ( pHead != NULL )
        pHead->arrListLinks[0] = pFacet;
    pHead = pFacet;
    ++iCount;
}
inline Void GJK::_SimplexFacetList::_Remove( _SimplexFacet * pFacet )
{
    if ( pFacet->arrListLinks[1] != NULL )
        pFacet->arrListLinks[1]->arrListLinks[0] = pFacet->arrListLinks[0];
    if ( pFacet->arrListLinks[0] != NULL )
        pFacet->arrListLinks[0]->arrListLinks[1] = pFacet->arrListLinks[1];
    if ( pFacet == pHead )
        pHead = pFacet->arrListLinks[1];
    --iCount;
}

/////////////////////////////////////////////////////////////////////////////////

inline Void GJK::_SimplexHorizon::_Initialize() {
    iCount = 0;
    pFacetC = NULL;
    pFacetF = NULL;
}

/////////////////////////////////////////////////////////////////////////////////

inline Scalar GJK::_GJK_Det( const Vector3 & vA, const Vector3 & vB, const Vector3 & vC ) {
    return ( vA.X*vB.Y*vC.Z - vA.Z*vB.Y*vC.X
            + vA.Y*vB.Z*vC.X - vA.X*vB.Z*vC.Y
            + vA.Z*vB.X*vC.Y - vA.Y*vB.X*vC.Z );
}

/////////////////////////////////////////////////////////////////////////////////

inline Bool GJK::_MPR_TestEquality( Scalar fA, Scalar fB ) {
    Scalar fAB = MathFn->Abs( fA - fB );
    if ( fAB < SCALAR_EPSILON )
        return true;
    fA = MathFn->Abs( fA );
    fB = MathFn->Abs( fB );
    if ( fB > fA )
        return ( fAB < (fB *SCALAR_EPSILON) );
    return ( fAB < (fA *SCALAR_EPSILON) );
}
inline Bool GJK::_MPR_TestEquality( const Vector3 & vA, const Vector3 & vB ) {
    return ( _MPR_TestEquality(vA.X, vB.X) && _MPR_TestEquality(vA.Y, vB.Y) && _MPR_TestEquality(vA.Z, vB.Z) );
}


