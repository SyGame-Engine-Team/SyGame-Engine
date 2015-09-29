/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Primitives/2D/Linear/Line2.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives 2D : Linear, Line2
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TLine2 implementation
template<typename Real>
TLine2<Real>::TLine2():
    Position(), Direction()
{
    // nothing to do
}
template<typename Real>
TLine2<Real>::TLine2( const TVertex2<Real> & vPos, const TVector2<Real> & vDir ):
    Position(vPos), Direction(vDir)
{
	// nothing to do
}
template<typename Real>
TLine2<Real>::TLine2( const TVertex2<Real> & vA, const TVertex2<Real> & vB ):
	Position(vA), Direction(vB - vA)
{
	// nothing to do
}
template<typename Real>
TLine2<Real>::TLine2( const TLine2<Real> & rhs ):
	Position(rhs.Position), Direction(rhs.Direction)
{
	// nothing to do
}
template<typename Real>
TLine2<Real>::~TLine2()
{
    // nothing to do
}

template<typename Real>
inline TLine2<Real> & TLine2<Real>::operator=( const TLine2<Real> & rhs ) {
	Position = rhs.Position;
	Direction = rhs.Direction;
	return (*this);
}

template<typename Real>
inline TVertex2<Real> TLine2<Real>::GetPoint( Real fParameter ) const {
    return ( Position + (Direction * fParameter) );
}
template<typename Real>
inline Real TLine2<Real>::GetParameter( const TVertex2<Real> & vPoint ) const {
    return (vPoint - Position).Norm();
}

template<typename Real>
TVertex2<Real> TLine2<Real>::ClosestPoint( const TVertex2<Real> & vPoint, Real * outParameter ) const
{
    Assert(false);
    return TVertex2<Real>::Null;
}
template<typename Real>
inline Real TLine2<Real>::Distance( const TVertex2<Real> & vPoint ) const {
    Assert(false);
    return (Real)0;
}

template<typename Real>
inline Int TLine2<Real>::ClassifyPoint( const TVertex2<Real> & vPoint, Real fThreshold ) const {
    Real fCross = ( Direction ^ (vPoint - Position) );
    if ( fCross < -fThreshold )
        return -1;
    if ( fCross > fThreshold )
        return +1;
    return 0;
}
