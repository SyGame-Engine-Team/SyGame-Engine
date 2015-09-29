/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/BoundingShapes/3D/BVSphere.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Bounding Volumes : Sphere
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs :
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TBVSphere implementation
template<typename Real>
TBVSphere<Real>::TBVSphere():
    TBoundingVolume<Real>(), m_vCenter()
{
    m_vCenter = TVertex3<Real>::Null;
    m_fRadius = (Real)0;
}
template<typename Real>
TBVSphere<Real>::TBVSphere( const TVertex3<Real> & vCenter, Real fRadius ):
    TBoundingVolume<Real>(), m_vCenter()
{
    m_vCenter = vCenter;
    m_fRadius = fRadius;
}
template<typename Real>
TBVSphere<Real>::TBVSphere( const TVertex3<Real> * arrVertices, UInt iVertexCount ):
    TBoundingVolume<Real>(), m_vCenter()
{
    m_vCenter = TVertex3<Real>::Null;
    m_fRadius = (Real)0;
    Compute( (const Byte *)arrVertices, iVertexCount, sizeof(TVertex3<Real>) );
}
template<typename Real>
TBVSphere<Real>::TBVSphere( const TBVSphere<Real> & rhs ):
    TBoundingVolume<Real>(), m_vCenter()
{
    m_vCenter = rhs.m_vCenter;
    m_fRadius = rhs.m_fRadius;
}
template<typename Real>
TBVSphere<Real>::~TBVSphere()
{
    // nothing to do
}

template<typename Real>
inline TBVSphere<Real> & TBVSphere<Real>::operator=( const TBVSphere<Real> & rhs ) {
    m_vCenter = rhs.m_vCenter;
    m_fRadius = rhs.m_fRadius;
    return (*this);
}

template<typename Real>
inline Bool TBVSphere<Real>::operator==( const TBVSphere<Real> & rhs ) const {
    return ( (m_vCenter == rhs.m_vCenter) && MathRealFn->Equals(m_fRadius,rhs.m_fRadius) );
}
template<typename Real>
inline Bool TBVSphere<Real>::operator!=( const TBVSphere<Real> & rhs ) const {
    return ( (m_vCenter != rhs.m_vCenter) || !(MathRealFn->Equals(m_fRadius,rhs.m_fRadius)) );
}

template<typename Real>
inline BoundingVolumeShape TBVSphere<Real>::GetShape() const {
    return BVSHAPE_SPHERE;
}

template<typename Real>
inline Bool TBVSphere<Real>::IsEmpty() const {
    return ( m_fRadius == (Real)0 );
}
template<typename Real>
inline Real TBVSphere<Real>::GetVolume() const {
    return ( (m_fRadius * m_fRadius * m_fRadius) * (Real)SCALAR_4PI_3 );
}
template<typename Real>
inline Real TBVSphere<Real>::GetSizeEstimation() const {
    return ( GetVolume() + m_fRadius );
}

template<typename Real>
inline const TVertex3<Real> & TBVSphere<Real>::GetCenter() const {
    return m_vCenter;
}

template<typename Real>
inline Real TBVSphere<Real>::GetRadius() const {
    return m_fRadius;
}

template<typename Real>
Void TBVSphere<Real>::Compute( const Byte * pVertices, UInt iVertexCount, UInt iStride )
{
    // Make an approximation
    _BuildFromAABox( pVertices, iVertexCount, iStride );

    // Make a first guess at optimal sphere
    const TVertex3<Real> * pPosition;

    const Byte * pVertex = pVertices;
    for( UInt i = 0; i < iVertexCount; ++i ) {
        pPosition = (const TVertex3<Real> *)pVertex;

        _MergeWithPoint( *pPosition );

        pVertex += iStride;
    }

    // Refine
    _Refine( pVertices, iVertexCount, iStride, 8 );
}

template<typename Real>
inline Void TBVSphere<Real>::Reset( const TVertex3<Real> & vCenter ) {
    m_vCenter = vCenter;
    m_fRadius = (Real)0;
}
template<typename Real>
inline Void TBVSphere<Real>::Grow( const TVertex3<Real> & vVertex ) {
    _MergeWithPoint( vVertex );
}
template<typename Real>
inline Void TBVSphere<Real>::Grow( const TBoundingVolume<Real> & newBV ) {
    _MergeWithSphere( (const TBVSphere<Real> &)newBV );
}

template<typename Real>
inline Void TBVSphere<Real>::AddMargin( Real fMargin ) {
    m_fRadius += fMargin;
}

template<typename Real>
Void TBVSphere<Real>::Transform( TBoundingVolume<Real> & outBV, const TTransform3<Real> & bvTransform ) const
{
    TBVSphere<Real> & outBVSphere = (TBVSphere<Real> &)outBV;

    outBVSphere.m_vCenter = bvTransform * m_vCenter;
    outBVSphere.m_fRadius = bvTransform.GetMaxScale() * m_fRadius;
}

template<typename Real>
Real TBVSphere<Real>::Distance( const TVertex3<Real> & vPoint ) const
{
    TVector3<Real> W = ( vPoint - m_vCenter );
    return ( W.Norm() - m_fRadius );
}

template<typename Real>
Bool TBVSphere<Real>::Contains( const TVertex3<Real> & vPoint ) const
{
    TVector3<Real> W = ( vPoint - m_vCenter );
    Real fSquaredDist = W.NormSqr();
    Real fSquaredRadius = m_fRadius * m_fRadius;
    return ( fSquaredDist <= fSquaredRadius );
}
template<typename Real>
Bool TBVSphere<Real>::Contains( const TBoundingVolume<Real> & vBV ) const
{
    const TBVSphere<Real> & vBVSphere = (const TBVSphere<Real> &)vBV;

    if ( !(Contains(vBVSphere.m_vCenter)) )
        return false;

    TVector3<Real> vAxis = ( vBVSphere.m_vCenter - m_vCenter );
    Real fCenterDistance = vAxis.Normalize();

    if ( (fCenterDistance + vBVSphere.m_fRadius) > m_fRadius )
        return false;

    return true;
}

template<typename Real>
Int TBVSphere<Real>::WhichSide( const TPlane<Real> & vPlane ) const
{
    Real fDistance = vPlane.DistanceNC( m_vCenter );
    if ( fDistance < -m_fRadius )
        return -1;
    if ( fDistance > m_fRadius )
        return +1;
    return 0;
}

template<typename Real>
Bool TBVSphere<Real>::RayCast( const TLine3<Real> & vRay, Real * outMinDistance, Real * outMaxDistance ) const
{
    if ( m_fRadius == (Real)0 )
        return false;

    TVector3<Real> W = ( vRay.Position - m_vCenter );
    Real fA = ( W.NormSqr() - (m_fRadius * m_fRadius) );
    Real fB = ( W * vRay.Direction );
    Real fDiscr = (fB * fB) - fA;
    if ( fDiscr < (Real)0 )
        return false;

    
    if ( outMinDistance != NULL ) {
        fDiscr = MathRealFn->Sqrt( fDiscr );
        *outMinDistance = -fB - fDiscr;
        if ( outMaxDistance != NULL )
            *outMaxDistance = -fB + fDiscr;
    } else if ( outMaxDistance != NULL )
        *outMaxDistance = -fB + fDiscr;
    
    return true;
}

template<typename Real>
Bool TBVSphere<Real>::Intersect( const TBoundingVolume<Real> & vBV ) const
{
    const TBVSphere<Real> & vBVSphere = (const TBVSphere<Real> &)vBV;

    TVector3<Real> W = ( vBVSphere.m_vCenter - m_vCenter );
    Real fSqrDist = W.NormSqr();
    Real fSumRadius = ( m_fRadius + vBVSphere.m_fRadius );
    return ( fSqrDist <= (fSumRadius * fSumRadius) );
}

template<typename Real>
Bool TBVSphere<Real>::Collide( const TVector3<Real> & vThisSpeed, const TBoundingVolume<Real> & vBV, const TVector3<Real> & vBVSpeed,
                               Real * outTimeOfImpact ) const
{
    const TBVSphere<Real> & vBVSphere = (const TBVSphere<Real> &)vBV;
    if ( m_fRadius == (Real)0 || vBVSphere.m_fRadius == (Real)0 )
        return false;

    TVector3<Real> W = ( vBVSphere.m_vCenter - m_vCenter );
    TVector3<Real> vRelSpeed = (vBVSpeed - vThisSpeed);

    Real fA = vRelSpeed.NormSqr();
    Real fC = ( W.NormSqr() - MathRealFn->Sqr(m_fRadius + vBVSphere.m_fRadius) );
    if ( MathRealFn->EqualsZero(fA) ) {
        if ( outTimeOfImpact != NULL )
            *outTimeOfImpact = (Real)0;
        return ( fC <= (Real)0 );
    }

    Real fB = ( W * vRelSpeed );
    if ( fB >= (Real)0 )
        return false;
    Real fDiscr = (fB * fB) - (fA * fC);
    if ( fDiscr < (Real)0 )
        return false;

    Real fSqrtDiscr = MathRealFn->Sqrt( fDiscr );
    Real fInvA = MathRealFn->Invert( fA );
    Real fTOI = (-fB - fSqrtDiscr) * fInvA;
    if ( fTOI < (Real)0 )
        fTOI = (-fB + fSqrtDiscr) * fInvA;
    if ( outTimeOfImpact != NULL )
        *outTimeOfImpact = fTOI;
    return ( fTOI >= (Real)0 );
}

/////////////////////////////////////////////////////////////////////////////////

template<typename Real>
Void TBVSphere<Real>::_BuildFromAABox( const Byte * pVertices, UInt iVertexCount, UInt iStride )
{
    // Compute AABox extrema along axes
    const TVertex3<Real> * pMinX = (const TVertex3<Real> *)pVertices;
    const TVertex3<Real> * pMaxX = (const TVertex3<Real> *)pVertices;
    const TVertex3<Real> * pMinY = (const TVertex3<Real> *)pVertices;
    const TVertex3<Real> * pMaxY = (const TVertex3<Real> *)pVertices;
    const TVertex3<Real> * pMinZ = (const TVertex3<Real> *)pVertices;
    const TVertex3<Real> * pMaxZ = (const TVertex3<Real> *)pVertices;
    
    const TVertex3<Real> * pPosition;

    const Byte * pVertex = ( pVertices + iStride );
    for( UInt i = 1; i < iVertexCount; ++i ) {
        pPosition = (const TVertex3<Real> *)pVertex;

        if ( pPosition->X < pMinX->X ) pMinX = pPosition;
        if ( pPosition->X > pMaxX->X ) pMaxX = pPosition;
        if ( pPosition->Y < pMinY->Y ) pMinY = pPosition;
        if ( pPosition->Y > pMaxY->Y ) pMaxY = pPosition;
        if ( pPosition->Z < pMinZ->Z ) pMinZ = pPosition;
        if ( pPosition->Z > pMaxZ->Z ) pMaxZ = pPosition;

        pVertex += iStride;
    }

    // Squared distances
    Real fSqrDistX = ( (*pMaxX) - (*pMinX) ).NormSqr();
    Real fSqrDistY = ( (*pMaxY) - (*pMinY) ).NormSqr();
    Real fSqrDistZ = ( (*pMaxZ) - (*pMinZ) ).NormSqr();

    const TVertex3<Real> * pMin = pMinX;
    const TVertex3<Real> * pMax = pMaxX;
    if ( fSqrDistY > fSqrDistX && fSqrDistY > fSqrDistZ ) {
        pMin = pMinY;
        pMax = pMaxY;
    }
    if ( fSqrDistZ > fSqrDistX && fSqrDistZ > fSqrDistY ) {
        pMin = pMinZ;
        pMax = pMaxZ;
    }

    // Setup sphere
    m_vCenter.X = ( pMin->X + pMax->X ) * (Real)0.5f;
    m_vCenter.Y = ( pMin->Y + pMax->Y ) * (Real)0.5f;
    m_vCenter.Z = ( pMin->Z + pMax->Z ) * (Real)0.5f;

    m_fRadius = ( (*pMax) - m_vCenter ).Norm();
}
template<typename Real>
Void TBVSphere<Real>::_MergeWithPoint( const TVertex3<Real> & vVertex )
{
    TVector3<Real> vCenterDiff = ( vVertex - m_vCenter );
    Real fSqrDist = vCenterDiff.NormSqr();
    Real fSqrRadius = ( m_fRadius * m_fRadius );

    if ( fSqrRadius >= fSqrDist )
        return;

    Real fDist = MathRealFn->Sqrt( fSqrDist );
    Real fNewRadius = ( fDist + m_fRadius ) * (Real)0.5f;
    if ( fDist > (Real)SCALAR_ERROR ) {
        Real fWeight = ( fNewRadius - m_fRadius ) / fDist;
        m_vCenter += ( vCenterDiff * fWeight );
    }
    m_fRadius = fNewRadius;
}
template<typename Real>
Void TBVSphere<Real>::_MergeWithSphere( const TBVSphere<Real> & bvSphere )
{
    if ( bvSphere.m_fRadius == (Real)0 )
        return; // dummy BV

    TVector3<Real> vCenterDiff = ( bvSphere.m_vCenter - m_vCenter );
    Real fSqrDist = vCenterDiff.NormSqr();
    Real fRadiusDiff = ( bvSphere.m_fRadius - m_fRadius );
    Real fSqrRadiusDiff = ( fRadiusDiff * fRadiusDiff );

    if ( fSqrRadiusDiff >= fSqrDist ) {
        if ( bvSphere.m_fRadius >= m_fRadius ) {
            m_vCenter = bvSphere.m_vCenter;
            m_fRadius = bvSphere.m_fRadius;
        }
        return;
    }

    Real fDist = MathRealFn->Sqrt( fSqrDist );
    Real fNewRadius = ( fDist + m_fRadius + bvSphere.m_fRadius ) * (Real)0.5f;
    if ( fDist > (Real)SCALAR_ERROR ) {
        Real fWeight = ( fNewRadius - m_fRadius ) / fDist;
        m_vCenter += ( vCenterDiff * fWeight );
    }
    m_fRadius = fNewRadius;
}

template<typename Real>
Void TBVSphere<Real>::_Refine( const Byte * pVertices, UInt iVertexCount, UInt iStride, UInt iIterations )
{
    TBVSphere<Real> tmpSphere(*this);

    const TVertex3<Real> * pPosition;

    for( UInt iter = 0; iter < iIterations; ++iter ) {
        // Shrink a little
        tmpSphere.m_fRadius *= (Real)0.9f;

        // Randomize input order
        ULong iRand = ( RandFn->XorShift128() & 0x07 );
        UInt iStartOffset = iRand * ( iVertexCount >> 3 );
        UInt iJump = ( iRand >> 1 );
        Bool bReverse = ( (iRand & 1) != 0 );
        Bool bLeftFirst = ( (iRand & 4) == 0 );

        // Grow back
        UInt iRun;
        Int i;

        if ( bLeftFirst ) {
            if ( bReverse ) {
                for( iRun = 0; iRun < iJump; ++iRun ) {
                    for( i = (Int)(iStartOffset - 1 - iRun); i >= 0; i -= (Int)iJump ) {
                        pPosition = (const TVertex3<Real> *)( pVertices + (i * iStride) );
                        tmpSphere._MergeWithPoint( *pPosition );
                    }
                    for( i = (Int)(iVertexCount - 1 - iRun); i >= (Int)iStartOffset; i -= (Int)iJump ) {
                        pPosition = (const TVertex3<Real> *)( pVertices + (i * iStride) );
                        tmpSphere._MergeWithPoint( *pPosition );
                    }
                }
            } else {
                for( iRun = 0; iRun < iJump; ++iRun ) {
                    for( i = (Int)iRun; i < (Int)iStartOffset; i += (Int)iJump ) {
                        pPosition = (const TVertex3<Real> *)( pVertices + (i * iStride) );
                        tmpSphere._MergeWithPoint( *pPosition );
                    }
                    for( i = (Int)(iStartOffset + iRun); i < (Int)iVertexCount; i += (Int)iJump ) {
                        pPosition = (const TVertex3<Real> *)( pVertices + (i * iStride) );
                        tmpSphere._MergeWithPoint( *pPosition );
                    }
                }
            }
        } else {
            if ( bReverse ) {
                for( iRun = 0; iRun < iJump; ++iRun ) {
                    for( i = (Int)(iVertexCount - 1 - iRun); i >= (Int)iStartOffset; i -= (Int)iJump ) {
                        pPosition = (const TVertex3<Real> *)( pVertices + (i * iStride) );
                        tmpSphere._MergeWithPoint( *pPosition );
                    }
                    for( i = (Int)(iStartOffset - 1 - iRun); i >= 0; i -= (Int)iJump ) {
                        pPosition = (const TVertex3<Real> *)( pVertices + (i * iStride) );
                        tmpSphere._MergeWithPoint( *pPosition );
                    }
                }
            } else {
                for( iRun = 0; iRun < iJump; ++iRun ) {
                    for( i = (Int)(iStartOffset + iRun); i < (Int)iVertexCount; i += (Int)iJump ) {
                        pPosition = (const TVertex3<Real> *)( pVertices + (i * iStride) );
                        tmpSphere._MergeWithPoint( *pPosition );
                    }
                    for( i = (Int)iRun; i < (Int)iStartOffset; i += (Int)iJump ) {
                        pPosition = (const TVertex3<Real> *)( pVertices + (i * iStride) );
                        tmpSphere._MergeWithPoint( *pPosition );
                    }
                }
            }
        }

        // Update
        if ( tmpSphere.m_fRadius < m_fRadius )
            *this = tmpSphere;
    }
}
