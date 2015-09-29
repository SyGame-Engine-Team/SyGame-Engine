/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Primitives/3D/Linear/Segment3.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives 3D : Linear, Segment3
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TSegment3 implementation
template<typename Real>
TSegment3<Real>::TSegment3():
    EndA(), EndB(), Center(), Direction()
{
    // nothing to do
}
template<typename Real>
TSegment3<Real>::TSegment3( const TVertex3<Real> & vA, const TVertex3<Real> & vB ):
    EndA(vA), EndB(vB), Center(), Direction()
{
    UpdateCenterDirExtent();
}
template<typename Real>
TSegment3<Real>::TSegment3( const TVertex3<Real> & vCenter, const TVector3<Real> & vDir, Real fExtent ):
    EndA(), EndB(), Center(vCenter), Direction(vDir)
{
    Extent = fExtent;
    UpdateEndPoints();
}
template<typename Real>
TSegment3<Real>::TSegment3( const TSegment3<Real> & rhs ):
    EndA(rhs.EndA), EndB(rhs.EndB), Center(rhs.Center), Direction(rhs.Direction)
{
    Extent = rhs.Extent;
}
template<typename Real>
TSegment3<Real>::~TSegment3()
{
    // nothing to do
}

template<typename Real>
inline TSegment3<Real> & TSegment3<Real>::operator=( const TSegment3<Real> & rhs ) {
	EndA = rhs.EndA;
	EndB = rhs.EndB;
	Center = rhs.Center;
	Direction = rhs.Direction;
	Extent = rhs.Extent;
	return (*this);
}

template<typename Real>
inline TVertex3<Real> TSegment3<Real>::GetPointAB( Real fParameter ) const {
    Assert( (fParameter >= (Real)0) && (fParameter <= (Real)1) );
    TVector3<Real> vB( EndB.X, EndB.Y, EndB.Z );
    return ( ( EndA * (((Real)1) - fParameter) ) + ( vB * fParameter ) );
}
template<typename Real>
inline TVertex3<Real> TSegment3<Real>::GetPointCDE( Real fParameter ) const {
    Assert( MathRealFn->Abs(fParameter) <= Extent );
    return ( Center + (Direction * fParameter) );
}
template<typename Real>
inline Real TSegment3<Real>::GetParameterAB( const TVertex3<Real> & vPoint ) const {
    return MathRealFn->Sqrt( (vPoint - EndA).NormSqr() / (EndB - EndA).NormSqr() );
}
template<typename Real>
inline Real TSegment3<Real>::GetParameterCDE( const TVertex3<Real> & vPoint ) const {
    return (vPoint - Center).Norm();
}

template<typename Real>
inline Void TSegment3<Real>::UpdateEndPoints() {
    EndA = Center - (Direction * Extent);
    EndB = Center + (Direction * Extent);
}
template<typename Real>
inline Void TSegment3<Real>::UpdateCenterDirExtent() {
    TVector3<Real> vB( EndB.X, EndB.Y, EndB.Z );
    Center = ( EndA + vB ) * (Real)0.5f;
    Direction = EndB - EndA;
    Extent = ( Direction.Normalize() * (Real)0.5f );
}

template<typename Real>
TVertex3<Real> TSegment3<Real>::ClosestPoint( const TVertex3<Real> & vPoint, Real * outParameter ) const
{
    Real fZeroTolerance = (Real)SCALAR_EPSILON;
    TVector3<Real> vAB = ( EndB - EndA );

    Real fT = ( (vPoint - EndA) * vAB );
    if ( fT <= fZeroTolerance ) {
        fT = (Real)0;
        return EndA;
    }

    Real fDenom = ( vAB * vAB );
    if ( fT >= (fDenom - fZeroTolerance) ) {
        fT = (Real)1;
        return EndB;
    }

    fT /= fDenom;
    if ( outParameter != NULL )
        *outParameter = fT;
    return ( EndA + (vAB * fT) );
}
template<typename Real>
Real TSegment3<Real>::Distance( const TVertex3<Real> & vPoint ) const
{
    Assert(false);
    return (Real)0;
}

