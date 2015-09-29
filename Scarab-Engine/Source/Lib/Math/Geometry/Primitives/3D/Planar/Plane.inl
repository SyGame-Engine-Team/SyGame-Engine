/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Primitives/3D/Planar/Plane.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives 3D : Planar, Plane
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs :
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TPlane implementation
template<typename Real>
TPlane<Real>::TPlane():
	Normal(), Position()
{
    // nothing to do
}
template<typename Real>
TPlane<Real>::TPlane( const TVector3<Real> & vNormal, Real fConstant ):
    Normal(vNormal), Position()
{
	Constant = fConstant;
    UpdatePosition();
}
template<typename Real>
TPlane<Real>::TPlane( const TVector3<Real> & vNormal, const TVertex3<Real> & vPosition ):
	Normal(vNormal), Position(vPosition)
{
	UpdateConstant();
}
template<typename Real>
TPlane<Real>::TPlane( const TPlane<Real> & rhs ):
    Normal(rhs.Normal), Position(rhs.Position)
{
    Constant = rhs.Constant;
}
template<typename Real>
TPlane<Real>::~TPlane()
{
    // nothing to do
}

template<typename Real>
inline TPlane<Real> & TPlane<Real>::operator=( const TPlane<Real> & rhs ) {
    Normal = rhs.Normal;
    Position = rhs.Position;
    Constant = rhs.Constant;
    return (*this);
}

template<typename Real>
inline Void TPlane<Real>::UpdatePosition() {
    Position = TVertex3<Real>::Null + ( Normal * Constant );
}
template<typename Real>
inline Void TPlane<Real>::UpdateConstant() {
    Constant = ( Normal * Position.ToVector() );
}

template<typename Real>
inline TVertex3<Real> TPlane<Real>::ClosestPointNP( const TVertex3<Real> & vPoint ) const {
    return ( vPoint - ( Normal * DistanceNP(vPoint) ) );
}
template<typename Real>
inline TVertex3<Real> TPlane<Real>::ClosestPointNC( const TVertex3<Real> & vPoint ) const {
    return ( vPoint - ( Normal * DistanceNC(vPoint) ) );
}
template<typename Real>
inline Real TPlane<Real>::DistanceNP( const TVertex3<Real> & vPoint ) const {
    return ( Normal * (vPoint - Position) );
}
template<typename Real>
inline Real TPlane<Real>::DistanceNC( const TVertex3<Real> & vPoint ) const {
    return ( (Normal * vPoint.ToVector()) - Constant );
}

template<typename Real>
inline Int TPlane<Real>::ClassifyPointNP( const TVertex3<Real> & vPoint, Real fZeroTolerance ) const {
    Real fDistance = DistanceNP(vPoint);
    if ( fDistance <= -fZeroTolerance ) return -1;
    if ( fDistance >= fZeroTolerance ) return +1;
    return 0;
}
template<typename Real>
inline Int TPlane<Real>::ClassifyPointNC( const TVertex3<Real> & vPoint, Real fZeroTolerance ) const {
    Real fDistance = DistanceNC(vPoint);
    if ( fDistance <= -fZeroTolerance ) return -1;
    if ( fDistance >= fZeroTolerance ) return +1;
    return 0;
}

