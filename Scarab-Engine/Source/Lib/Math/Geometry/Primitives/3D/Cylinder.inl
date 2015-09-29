/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Primitives/3D/Cylinder.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives 3D : Cylinder
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TCylinder implementation
template<typename Real>
TCylinder<Real>::TCylinder():
    Center(), Axis()
{
    // nothing to do
}
template<typename Real>
TCylinder<Real>::TCylinder( const TVertex3<Real> & vCenter, const TVector3<Real> & vAxis, Real fHeight, Real fRadius ):
    Center(vCenter), Axis(vAxis)
{
    Height = fHeight;
    Radius = fRadius;
}
template<typename Real>
TCylinder<Real>::TCylinder( const TCylinder<Real> & rhs ):
    Center(rhs.Center), Axis(rhs.Axis)
{
    Height = rhs.Height;
    Radius = rhs.Radius;
}
template<typename Real>
TCylinder<Real>::~TCylinder()
{
    // nothing to do
}

template<typename Real>
inline TCylinder<Real> & TCylinder<Real>::operator=( const TCylinder<Real> & rhs ) {
    Center = rhs.Center;
    Axis = rhs.Axis;
    Height = rhs.Height;
    Radius = rhs.Radius;
    return (*this);
}

template<typename Real>
Bool TCylinder<Real>::TestInclusion( const TVertex3<Real> & vPoint ) const
{
    Assert(false);
    return false;
}

template<typename Real>
TVertex3<Real> TCylinder<Real>::ClosestPoint( const TVertex3<Real> & vPoint ) const
{
    Assert(false);
    return TVertex3<Real>::Null;
}
template<typename Real>
Real TCylinder<Real>::Distance( const TVertex3<Real> & vPoint ) const
{
    Assert(false);
    return (Real)0;
}


