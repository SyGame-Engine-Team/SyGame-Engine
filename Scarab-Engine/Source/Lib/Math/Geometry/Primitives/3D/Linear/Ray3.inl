/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Primitives/3D/Linear/Ray3.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives 3D : Linear, Ray3
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TRay3 implementation
template<typename Real>
TRay3<Real>::TRay3():
    Position(), Direction()
{
    // nothing to do
}
template<typename Real>
TRay3<Real>::TRay3( const TVertex3<Real> & vPos, const TVector3<Real> & vDir ):
    Position(vPos), Direction(vDir)
{
	// nothing to do
}
template<typename Real>
TRay3<Real>::TRay3( const TVertex3<Real> & vA, const TVertex3<Real> & vB ):
	Position(vA), Direction(vB - vA)
{
	// nothing to do
}
template<typename Real>
TRay3<Real>::TRay3( const TRay3<Real> & rhs ):
	Position(rhs.Position), Direction(rhs.Direction)
{
	// nothing to do
}
template<typename Real>
TRay3<Real>::~TRay3()
{
    // nothing to do
}

template<typename Real>
inline TRay3<Real> & TRay3<Real>::operator=( const TRay3<Real> & rhs ) {
	Position = rhs.Position;
	Direction = rhs.Direction;
	return (*this);
}

template<typename Real>
inline TVertex3<Real> TRay3<Real>::GetPoint( Real fParameter ) const {
    Assert( fParameter >= (Real)0 );
    return ( Position + (Direction * fParameter) );
}
template<typename Real>
inline Real TRay3<Real>::GetParameter( const TVertex3<Real> & vPoint ) const {
    return (vPoint - Position).Norm();
}

template<typename Real>
TVertex3<Real> TRay3<Real>::ClosestPoint( const TVertex3<Real> & vPoint, Real * outParameter ) const
{
    Assert(false);
    return TVertex3<Real>::Null;
}
template<typename Real>
Real TRay3<Real>::Distance( const TVertex3<Real> & vPoint ) const
{
    Assert(false);
    return (Real)0;
}

