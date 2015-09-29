/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Primitives/3D/Torus.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives 3D : Torus
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TTorus implementation
template<typename Real>
TTorus<Real>::TTorus():
    Center(), Axis()
{
    // nothing to do
}
template<typename Real>
TTorus<Real>::TTorus( const TVertex3<Real> & vCenter, const TVector3<Real> & vAxis, Real fOuterRadius, Real fInnerRadius ):
    Center(vCenter), Axis(vAxis)
{
    OuterRadius = fOuterRadius;
    InnerRadius = fInnerRadius;
}
template<typename Real>
TTorus<Real>::TTorus( const TTorus<Real> & rhs ):
    Center(rhs.Center), Axis(rhs.Axis)
{
    OuterRadius = rhs.OuterRadius;
    InnerRadius = rhs.InnerRadius;
}
template<typename Real>
TTorus<Real>::~TTorus()
{
    // nothing to do
}

template<typename Real>
inline TTorus<Real> & TTorus<Real>::operator=( const TTorus<Real> & rhs ) {
    Center = rhs.Center;
    Axis = rhs.Axis;
    OuterRadius = rhs.OuterRadius;
    InnerRadius = rhs.InnerRadius;
    return (*this);
}

template<typename Real>
Bool TTorus<Real>::TestInclusion( const TVertex3<Real> & vPoint ) const
{
    Assert(false);
    return false;
}

template<typename Real>
TVertex3<Real> TTorus<Real>::ClosestPoint( const TVertex3<Real> & vPoint ) const
{
    Assert(false);
    return TVertex3<Real>::Null;
}
template<typename Real>
Real TTorus<Real>::Distance( const TVertex3<Real> & vPoint ) const
{
    Assert(false);
    return (Real)0;
}
