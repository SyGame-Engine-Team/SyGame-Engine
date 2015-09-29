/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Primitives/3D/Planar/Circle3.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives 3D : Planar, Circle3
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs :
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TCircle3 implementation
template<typename Real>
TCircle3<Real>::TCircle3():
    Center(), Axis()
{
    // nothing to do
}
template<typename Real>
TCircle3<Real>::TCircle3( const TVertex3<Real> & vCenter, const TVector3<Real> & vAxis, Real fRadius ):
    Center(vCenter), Axis(vAxis)
{
    Radius = fRadius;
}
template<typename Real>
TCircle3<Real>::TCircle3( const TCircle3<Real> & rhs ):
    Center(rhs.Center), Axis(rhs.Axis)
{
    Radius = rhs.Radius;
}
template<typename Real>
TCircle3<Real>::~TCircle3()
{
    // nothing to do
}

template<typename Real>
inline TCircle3<Real> & TCircle3<Real>::operator=( const TCircle3<Real> & rhs ) {
    Center = rhs.Center;
    Axis = rhs.Axis;
    Radius = rhs.Radius;
    return (*this);
}

template<typename Real>
inline TPlane<Real> TCircle3<Real>::GetPlane() const {
    return TPlane<Real>( Axis, Center );
}

template<typename Real>
TVertex3<Real> TCircle3<Real>::ClosestPoint( const TVertex3<Real> & vPoint ) const
{
    Assert(false);
    return TVertex3<Real>::Null;
}
template<typename Real>
Real TCircle3<Real>::Distance( const TVertex3<Real> & vPoint ) const
{
    Assert(false);
    return (Real)0;
}

