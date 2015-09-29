/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Primitives/2D/Linear/Segment2.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives 2D : Linear, Segment2
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TSegment2 implementation
template<typename Real>
TSegment2<Real>::TSegment2():
    EndA(), EndB(), Center(), Direction()
{
    // nothing to do
}
template<typename Real>
TSegment2<Real>::TSegment2( const TVertex2<Real> & vA, const TVertex2<Real> & vB ):
    EndA(vA), EndB(vB), Center(), Direction()
{
    UpdateCenterDirExtent();
}
template<typename Real>
TSegment2<Real>::TSegment2( const TVertex2<Real> & vCenter, const TVector2<Real> & vDir, Real fExtent ):
    EndA(), EndB(), Center(vCenter), Direction(vDir)
{
    Extent = fExtent;
    UpdateEndPoints();
}
template<typename Real>
TSegment2<Real>::TSegment2( const TSegment2<Real> & rhs ):
    EndA(rhs.EndA), EndB(rhs.EndB), Center(rhs.Center), Direction(rhs.Direction)
{
    Extent = rhs.Extent;
}
template<typename Real>
TSegment2<Real>::~TSegment2()
{
    // nothing to do
}

template<typename Real>
inline TSegment2<Real> & TSegment2<Real>::operator=( const TSegment2<Real> & rhs ) {
	EndA = rhs.EndA;
	EndB = rhs.EndB;
	Center = rhs.Center;
	Direction = rhs.Direction;
	Extent = rhs.Extent;
	return (*this);
}

template<typename Real>
inline TVertex2<Real> TSegment2<Real>::GetPointAB( Real fParameter ) const {
    Assert( (fParameter >= (Real)0) && (fParameter <= (Real)1) );
    TVector2<Real> vB( EndB.X, EndB.Y );
    return ( ( EndA * (((Real)1) - fParameter) ) + ( vB * fParameter ) );
}
template<typename Real>
inline TVertex2<Real> TSegment2<Real>::GetPointCDE( Real fParameter ) const {
    Assert( MathRealFn->Abs(fParameter) <= Extent );
    return ( Center + (Direction * fParameter) );
}
template<typename Real>
inline Real TSegment2<Real>::GetParameterAB( const TVertex2<Real> & vPoint ) const {
    return MathRealFn->Sqrt( (vPoint - EndA).NormSqr() / (EndB - EndA).NormSqr() );
}
template<typename Real>
inline Real TSegment2<Real>::GetParameterCDE( const TVertex2<Real> & vPoint ) const {
    return (vPoint - Center).Norm();
}

template<typename Real>
inline Void TSegment2<Real>::UpdateEndPoints() {
    EndA = Center - (Direction * Extent);
    EndB = Center + (Direction * Extent);
}
template<typename Real>
inline Void TSegment2<Real>::UpdateCenterDirExtent() {
    TVector2<Real> vB( EndB.X, EndB.Y );
    Center = ( EndA + vB ) * (Real)0.5f;
    Direction = EndB - EndA;
    Extent = ( Direction.Normalize() * (Real)0.5f );
}

template<typename Real>
TVertex2<Real> TSegment2<Real>::ClosestPoint( const TVertex2<Real> & vPoint, Real * outParameter ) const
{
    Assert(false);
    return TVertex2<Real>::Null;
}
template<typename Real>
inline Real TSegment2<Real>::Distance( const TVertex2<Real> & vPoint ) const {
    Assert(false);
    return (Real)0;
}

template<typename Real>
inline Int TSegment2<Real>::ClassifyPoint( const TVertex2<Real> & vPoint, Real fThreshold ) const {
    Real fCross = ( (EndB - EndA) ^ (vPoint - EndA) );
    if ( fCross < -fThreshold )
        return -1;
    if ( fCross > fThreshold )
        return +1;
    return 0;
}


