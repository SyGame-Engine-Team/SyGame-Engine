/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/BoundingShapes/3D/BVAABox.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Bounding Volumes : Axis-Aligned Box
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs :
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TBVAABox implementation
template<typename Real>
TBVAABox<Real>::TBVAABox():
    TBoundingVolume<Real>(), m_vCenter(), m_vExtents()
{
    m_vCenter = TVertex3<Real>::Null;
    m_vExtents = TVector3<Real>::Null;
}
template<typename Real>
TBVAABox<Real>::TBVAABox( const TVertex3<Real> & vCenter, Real fRadius ):
    TBoundingVolume<Real>(), m_vCenter(), m_vExtents()
{
    m_vCenter = vCenter;
    m_vExtents = TVector3<Real>( fRadius, fRadius, fRadius );
}
template<typename Real>
TBVAABox<Real>::TBVAABox( const TVertex3<Real> & vCenter, const TVector3<Real> & vExtents ):
    TBoundingVolume<Real>(), m_vCenter(), m_vExtents()
{
    m_vCenter = vCenter;
    m_vExtents = vExtents;
}
template<typename Real>
TBVAABox<Real>::TBVAABox( const TVertex3<Real> & vMin, const TVertex3<Real> & vMax ):
    TBoundingVolume<Real>(), m_vCenter(), m_vExtents()
{
    m_vCenter.X = ( vMin.X + vMax.X ) * (Real)0.5f;
    m_vCenter.Y = ( vMin.Y + vMax.Y ) * (Real)0.5f;
    m_vCenter.Z = ( vMin.Z + vMax.Z ) * (Real)0.5f;
    m_vExtents = ( vMax - vMin ) * (Real)0.5f;
}
template<typename Real>
TBVAABox<Real>::TBVAABox( const TVertex3<Real> * arrVertices, UInt iVertexCount ):
    TBoundingVolume<Real>(), m_vCenter(), m_vExtents()
{
    m_vCenter = TVertex3<Real>::Null;
    m_vExtents = TVector3<Real>::Null;
    Compute( (const Byte *)arrVertices, iVertexCount, sizeof(TVertex3<Real>) );
}
template<typename Real>
TBVAABox<Real>::TBVAABox( const TBVAABox<Real> & rhs ):
    TBoundingVolume<Real>(), m_vCenter(), m_vExtents()
{
    m_vCenter = rhs.m_vCenter;
    m_vExtents = rhs.m_vExtents;
}
template<typename Real>
TBVAABox<Real>::~TBVAABox()
{
    // nothing to do
}

template<typename Real>
inline TBVAABox<Real> & TBVAABox<Real>::operator=( const TBVAABox<Real> & rhs ) {
    m_vCenter = rhs.m_vCenter;
    m_vExtents = rhs.m_vExtents;
    return (*this);
}

template<typename Real>
inline Bool TBVAABox<Real>::operator==( const TBVAABox<Real> & rhs ) const {
    return ( (m_vCenter == rhs.m_vCenter) && (m_vExtents == rhs.m_vExtents) );
}
template<typename Real>
inline Bool TBVAABox<Real>::operator!=( const TBVAABox<Real> & rhs ) const {
    return ( (m_vCenter != rhs.m_vCenter) || (m_vExtents != rhs.m_vExtents) );
}

template<typename Real>
inline BoundingVolumeShape TBVAABox<Real>::GetShape() const {
    return BVSHAPE_AABOX;
}

template<typename Real>
inline Bool TBVAABox<Real>::IsEmpty() const {
    return ( m_vExtents == TVector3<Real>::Null );
}
template<typename Real>
inline Real TBVAABox<Real>::GetVolume() const {
    return ( m_vExtents.X * m_vExtents.Y * m_vExtents.Z ) * (Real)8.0f;
}
template<typename Real>
inline Real TBVAABox<Real>::GetSizeEstimation() const {
    TVector3<Real> vLengths = GetLengths();
    return ( ( vLengths.X * vLengths.Y * vLengths.Z ) +
             ( vLengths.X + vLengths.Y + vLengths.Z ) );
}

template<typename Real>
inline const TVertex3<Real> & TBVAABox<Real>::GetCenter() const {
    return m_vCenter;
}

template<typename Real>
inline const TVector3<Real> & TBVAABox<Real>::GetExtents() const {
    return m_vExtents;
}
template<typename Real>
inline TVector3<Real> TBVAABox<Real>::GetLengths() const {
    return ( m_vExtents * (Real)2.0f );
}

template<typename Real>
inline TVertex3<Real> TBVAABox<Real>::GetMin() const {
    return ( m_vCenter - m_vExtents );
}
template<typename Real>
inline TVertex3<Real> TBVAABox<Real>::GetMax() const {
    return ( m_vCenter + m_vExtents );
}

template<typename Real>
Void TBVAABox<Real>::Compute( const Byte * pVertices, UInt iVertexCount, UInt iStride )
{
    TVector3<Real> vMin( (Real)SCALAR_INFINITE, (Real)SCALAR_INFINITE, (Real)SCALAR_INFINITE );
    TVector3<Real> vMax( (Real)(-SCALAR_INFINITE), (Real)(-SCALAR_INFINITE), (Real)(-SCALAR_INFINITE) );

    // Compute Min-Max along axes
    const TVertex3<Real> * pPosition;

    const Byte * pVertex = pVertices;
    for( UInt i = 0; i < iVertexCount; ++i ) {
        pPosition = (const TVertex3<Real> *)pVertex;

        if ( pPosition->X < vMin.X )
            vMin.X = pPosition->X;
        if ( pPosition->X > vMax.X )
            vMax.X = pPosition->X;

        if ( pPosition->Y < vMin.Y )
            vMin.Y = pPosition->Y;
        if ( pPosition->Y > vMax.Y )
            vMax.Y = pPosition->Y;

        if ( pPosition->Z < vMin.Z )
            vMin.Z = pPosition->Z;
        if ( pPosition->Z > vMax.Z )
            vMax.Z = pPosition->Z;

        pVertex += iStride;
    }

    // Convert to Center-Extent
    m_vCenter.X = ( vMin.X + vMax.X ) * (Real)0.5f;
    m_vCenter.Y = ( vMin.Y + vMax.Y ) * (Real)0.5f;
    m_vCenter.Z = ( vMin.Z + vMax.Z ) * (Real)0.5f;
    m_vExtents = ( vMax - vMin ) * (Real)0.5f;
}

template<typename Real>
Void TBVAABox<Real>::Reset( const TVertex3<Real> & vCenter )
{
    m_vCenter = vCenter;
    m_vExtents = TVector3<Real>::Null;
}
template<typename Real>
Void TBVAABox<Real>::Grow( const TVertex3<Real> & vVertex )
{
    // Update along each axis
    Real fMin, fMax;
    for( UInt i = 0; i < 3; ++i ) {
        fMin = ( m_vCenter[i] - m_vExtents[i] );
        fMax = ( m_vCenter[i] + m_vExtents[i] );
        if ( vVertex[i] < fMin ) {
            m_vCenter[i] = ( vVertex[i] + fMax ) * (Real)0.5f;
            m_vExtents[i] = ( m_vCenter[i] - vVertex[i] );
        }
        if ( vVertex[i] > fMax ) {
            m_vCenter[i] = ( fMin + vVertex[i] ) * (Real)0.5f;
            m_vExtents[i] = ( vVertex[i] - m_vCenter[i] );
        }
    }
}
template<typename Real>
Void TBVAABox<Real>::Grow( const TBoundingVolume<Real> & newBV )
{
    const TBVAABox<Real> & bvAABox = (const TBVAABox<Real> &)newBV;

    // Update along each axis
    Real fMinA, fMaxA, fMinB, fMaxB;
    for( UInt i = 0; i < 3; ++i ) {
        fMinA = ( m_vCenter[i] - m_vExtents[i] );
        fMaxA = ( m_vCenter[i] + m_vExtents[i] );
        fMinB = ( bvAABox.m_vCenter[i] - bvAABox.m_vExtents[i] );
        fMaxB = ( bvAABox.m_vCenter[i] + bvAABox.m_vExtents[i] );
        if ( fMinB < fMinA )
            fMinA = fMinB;
        if ( fMaxB > fMaxA )
            fMaxA = fMaxB;
        m_vCenter[i] = ( fMinA + fMaxA ) * (Real)0.5f;
        m_vExtents[i] = ( fMaxA - m_vCenter[i] );
    }
}

template<typename Real>
inline Void TBVAABox<Real>::AddMargin( Real fMargin ) {
    m_vExtents += Vector3( fMargin, fMargin, fMargin );
}

template<typename Real>
inline Void TBVAABox<Real>::Expand( const TVector3<Real> & vExtent ) {
    m_vExtents += vExtent;
}
template<typename Real>
Void TBVAABox<Real>::ExpandMinMax( const TVector3<Real> & vExtent )
{
    TVertex3<Real> vMin = GetMin();
    TVertex3<Real> vMax = GetMax();

    if ( vExtent.X > (Real)0 )
        vMax.X += vExtent.X;
    else
        vMin.X += vExtent.X;
    if ( vExtent.Y > (Real)0 )
        vMax.Y += vExtent.Y;
    else
        vMin.Y += vExtent.Y;
    if ( vExtent.Z > (Real)0 )
        vMax.Z += vExtent.Z;
    else
        vMin.Z += vExtent.Z;

    // Convert to Center-Extent
    m_vCenter.X = ( vMin.X + vMax.X ) * (Real)0.5f;
    m_vCenter.Y = ( vMin.Y + vMax.Y ) * (Real)0.5f;
    m_vCenter.Z = ( vMin.Z + vMax.Z ) * (Real)0.5f;
    m_vExtents = ( vMax - vMin ) * (Real)0.5f;
}

template<typename Real>
Void TBVAABox<Real>::Transform( TBoundingVolume<Real> & outBV, const TTransform3<Real> & bvTransform ) const
{
    TBVAABox<Real> & outAABox = (TBVAABox<Real> &)outBV;

    const TVector3<Real> & vTranslate = bvTransform.GetTranslate();
    TMatrix3<Real> matTransform( bvTransform.GetHMatrix() );

    // Abolute-transform trick
    for( UInt i = 0; i < 3; ++i ) {
        outAABox.m_vCenter[i] = vTranslate[i];
        outAABox.m_vExtents[i] = (Real)0;
        for( UInt j = 0; j < 3; ++j ) {
            outAABox.m_vCenter[i] += ( matTransform(i,j) * m_vCenter[j] );
            outAABox.m_vExtents[i] += ( MathRealFn->Abs( matTransform(i,j) ) * m_vExtents[j] );
        }
    }
}

template<typename Real>
Real TBVAABox<Real>::Distance( const TVertex3<Real> & vPoint ) const
{
    // Compute closest point on the box to vPoint, handle penetration case
    Vector3 vAxis[3] = { Vector3::eI, Vector3::eJ, Vector3::eK };
    Vector3 vU = ( vPoint - m_vCenter );
    Scalar fDist, fMinPenetration;
    UInt iMinAxis;

    UInt iPenetrationCount = 0;
    Scalar fMinAbsPenetration = SCALAR_INFINITE;
    Scalar fAbsPenetration;

    Vertex3 vClosestPoint = m_vCenter;
    for( UInt i = 0; i < 3; ++i ) {
        fDist = ( vU * vAxis[i] );
        if ( fDist > m_vExtents[i] )
            fDist = m_vExtents[i];
        else if ( fDist < -m_vExtents[i] )
            fDist = -m_vExtents[i];
        else {
            ++iPenetrationCount;
            fAbsPenetration = ( m_vExtents[i] - MathFn->Abs(fDist) );
            if ( fAbsPenetration < fMinAbsPenetration ) {
                fMinAbsPenetration = fAbsPenetration;
                fMinPenetration = (fDist < 0.0f) ? -fAbsPenetration : fAbsPenetration;
                iMinAxis = i;
            }
        }
        vClosestPoint += ( vAxis[i] * fDist );
    }
    if ( iPenetrationCount == 3 )
        vClosestPoint += ( vAxis[iMinAxis] * fMinPenetration );

    // Compute distance
    Vector3 vW = ( vClosestPoint - vPoint );
    return ( iPenetrationCount == 3 ) ? -(vW.Norm()) : vW.Norm();
}

template<typename Real>
Bool TBVAABox<Real>::Contains( const TVertex3<Real> & vPoint ) const
{
    TVector3<Real> W = ( vPoint - m_vCenter );
    if ( MathRealFn->Abs( W.X ) > m_vExtents.X )
        return false;
    if ( MathRealFn->Abs( W.Y ) > m_vExtents.Y )
        return false;
    if ( MathRealFn->Abs( W.Z ) > m_vExtents.Z )
        return false;
    return true;
}
template<typename Real>
Bool TBVAABox<Real>::Contains( const TBoundingVolume<Real> & vBV ) const
{
    const TBVAABox<Real> & vBVAABox = (const TBVAABox<Real> &)vBV;

    TVertex3<Real> vMin = GetMin();
    TVertex3<Real> vMax = GetMax();
    TVertex3<Real> vBVMin = vBVAABox.GetMin();
    TVertex3<Real> vBVMax = vBVAABox.GetMax();

    if ( vBVMin.X < vMin.X )
        return false;
    if ( vBVMin.Y < vMin.Y )
        return false;
    if ( vBVMin.Z < vMin.Z )
        return false;

    if ( vBVMax.X > vMax.X )
        return false;
    if ( vBVMax.Y > vMax.Y )
        return false;
    if ( vBVMax.Z > vMax.Z )
        return false;

    return true;
}

template<typename Real>
Int TBVAABox<Real>::WhichSide( const TPlane<Real> & vPlane ) const
{
    Real fProj = ( ( m_vExtents.X * MathRealFn->Abs( vPlane.Normal.X ) ) +
                   ( m_vExtents.Y * MathRealFn->Abs( vPlane.Normal.Y ) ) +
                   ( m_vExtents.Z * MathRealFn->Abs( vPlane.Normal.Z ) ) );
    Real fDist = ( m_vCenter.ToVector() * vPlane.Normal ) - vPlane.Constant;
    if ( fDist < -fProj )
        return -1;
    if ( fDist > fProj )
        return +1;
    return 0;
}

template<typename Real>
Bool TBVAABox<Real>::RayCast( const TLine3<Real> & vRay, Real * outMinDistance, Real * outMaxDistance ) const
{
    // Ray bounds
    Real fMinDist = (Real)(-SCALAR_INFINITE);
    Real fMaxDist = (Real)SCALAR_INFINITE;

    // Test each axis (3-Slab approach)
    Real fMin, fMax;
    Real fInvDist, fNear, fFar, fTmp;

    for( UInt i = 0; i < 3; ++i ) {
        fMin = ( m_vCenter[i] - m_vExtents[i] );
        fMax = ( m_vCenter[i] + m_vExtents[i] );

        // Parallel ray
        if ( MathRealFn->Abs( vRay.Direction[i] ) < (Real)SCALAR_ERROR ) {
            if ( vRay.Position[i] < fMin || vRay.Position[i] > fMax )
                return false;
            continue;
        }

        // Compute Slab intersection
        fInvDist = MathRealFn->Invert( vRay.Direction[i] );
        fNear = ( fMin - vRay.Position[i] ) * fInvDist;
        fFar = ( fMax - vRay.Position[i] ) * fInvDist;
        if ( fNear > fFar ) {
            fTmp = fNear;
            fNear = fFar;
            fFar = fTmp;
        }

        // Update bounds
        if ( fNear > fMinDist )
            fMinDist = fNear;
        if ( fFar < fMaxDist )
            fMaxDist = fFar;

        // Empty slab intersection
        if ( fMinDist > fMaxDist )
            return false;
    }

    // Done
    if ( outMinDistance != NULL )
        *outMinDistance = fMinDist;
    if ( outMaxDistance != NULL )
        *outMaxDistance = fMaxDist;
    return true;
}

template<typename Real>
Bool TBVAABox<Real>::Intersect( const TBoundingVolume<Real> & vBV ) const
{
    const TBVAABox<Real> & bvAABox = (const TBVAABox<Real> &)vBV;

    if ( MathRealFn->Abs(bvAABox.m_vCenter.X - m_vCenter.X) > (bvAABox.m_vExtents.X + m_vExtents.X) )
        return false;
    if ( MathRealFn->Abs(bvAABox.m_vCenter.Y - m_vCenter.Y) > (bvAABox.m_vExtents.Y + m_vExtents.Y) )
        return false;
    if ( MathRealFn->Abs(bvAABox.m_vCenter.Z - m_vCenter.Z) > (bvAABox.m_vExtents.Z + m_vExtents.Z) )
        return false;

    return true;
}

template<typename Real>
Bool TBVAABox<Real>::Collide( const TVector3<Real> & vThisSpeed, const TBoundingVolume<Real> & vBV, const TVector3<Real> & vBVSpeed,
                              Real * outTimeOfImpact ) const
{
    const TBVAABox<Real> & bvAABox = (const TBVAABox<Real> &)vBV;

    // Already intersecting
    if ( Intersect(vBV) ) {
        if ( outTimeOfImpact != NULL )
            *outTimeOfImpact = (Real)0;
        return true;
    }

    // Relative speed
    TVector3<Real> vRelSpeed = ( vBVSpeed - vThisSpeed );

    // Compute first TOI
    Real fFirstTOI = (Real)0;
    Real fLastTOI = (Real)SCALAR_INFINITE;

    // Test each axis
    Real fMinA, fMaxA, fMinB, fMaxB;
    for( UInt i = 0; i < 3; ++i ) {
        fMinA = ( m_vCenter[i] - m_vExtents[i] );
        fMinB = ( bvAABox.m_vCenter[i] - bvAABox.m_vExtents[i] );
        fMaxA = ( m_vCenter[i] + m_vExtents[i] );
        fMaxB = ( bvAABox.m_vCenter[i] + bvAABox.m_vExtents[i] );
        if ( vRelSpeed[i] < (Real)0 ) {
            if ( fMaxB < fMinA )
                return false;
            if ( fMaxA < fMinB )
                fFirstTOI = Max<Real>( (fMaxA - fMinB) / vRelSpeed[i], fFirstTOI );
            if ( fMinA < fMaxB )
                fLastTOI = Min<Real>( (fMinA - fMaxB) / vRelSpeed[i], fLastTOI );
        }
        if ( vRelSpeed[i] > (Real)0 ) {
            if ( fMaxA < fMinB )
                return false;
            if ( fMaxB < fMinA )
                fFirstTOI = Max<Real>( (fMinA - fMaxB) / vRelSpeed[i], fFirstTOI );
            if ( fMinB < fMaxA )
                fLastTOI = Min<Real>( (fMaxA - fMinB) / vRelSpeed[i], fLastTOI );
        }
        if ( fFirstTOI > fLastTOI )
            return false;
    }

    // Done
    if ( outTimeOfImpact != NULL )
        *outTimeOfImpact = fFirstTOI;
    return true;
}

