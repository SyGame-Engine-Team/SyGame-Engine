/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Primitives/3D/Sphere.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives 3D : Sphere
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TSphere implementation
template<typename Real>
TSphere<Real>::TSphere():
    Center()
{
    // nothing to do
}
template<typename Real>
TSphere<Real>::TSphere( const TVertex3<Real> & vCenter, Real fRadius ):
    Center(vCenter)
{
    Radius = fRadius;
}
template<typename Real>
TSphere<Real>::TSphere( const TSphere<Real> & rhs ):
    Center(rhs.Center)
{
    Radius = rhs.Radius;
}
template<typename Real>
TSphere<Real>::~TSphere()
{
    // nothing to do
}

template<typename Real>
inline TSphere<Real> & TSphere<Real>::operator=( const TSphere<Real> & rhs ) {
    Center = rhs.Center;
    Radius = rhs.Radius;
    return (*this);
}

template<typename Real>
inline Bool TSphere<Real>::TestInclusion( const TVertex3<Real> & vPoint ) const {
    Real fDistanceSqr = (vPoint - Center).NormSqr();
    return ( fDistanceSqr <= (Radius * Radius) );
}

template<typename Real>
inline TVertex3<Real> TSphere<Real>::ClosestPoint( const TVertex3<Real> & vPoint ) const {
    TVector3<Real> vU = (vPoint - Center);
    vU.Normalize();
    return Position + ( vU * Radius );
}
template<typename Real>
inline Real TSphere<Real>::Distance( const TVertex3<Real> & vPoint ) const {
    return ( (vPoint - Center).Norm() - Radius );
}

