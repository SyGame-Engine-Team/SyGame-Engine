/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Primitives/3D/Planar/Quad3.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives 3D : Planar, Quad3
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs :
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TQuad3 implementation
template<typename Real>
TQuad3<Real>::TQuad3():
    A(), B(), C(), D()
{
    // nothing to do
}
template<typename Real>
TQuad3<Real>::TQuad3( const TVertex3<Real> & vA, const TVertex3<Real> & vB,
                      const TVertex3<Real> & vC, const TVertex3<Real> & vD ):
    A(vA), B(vB), C(vC), D(vD)
{
    // nothing to do
}
template<typename Real>
TQuad3<Real>::TQuad3( const TVertex3<Real> vArr[4] ):
    A(vArr[0]), B(vArr[1]), C(vArr[2]), D(vArr[3])
{
    // nothing to do
}
template<typename Real>
TQuad3<Real>::TQuad3( const TQuad3<Real> & rhs ):
    A(rhs.A), B(rhs.B), C(rhs.C), D(rhs.D)
{
    // nothing to do
}
template<typename Real>
TQuad3<Real>::~TQuad3()
{
    // nothing to do
}

template<typename Real>
inline TQuad3<Real> & TQuad3<Real>::operator=( const TQuad3<Real> & rhs ) {
    A = rhs.A;
    B = rhs.B;
    C = rhs.C;
    D = rhs.D;
    return (*this);
}

template<typename Real>
inline Bool TQuad3<Real>::IsCoplanar( Real fZeroTolerance ) const {
    TVector3<Real> vCross0 = ( (B - A) ^ (C - A) );
    TVector3<Real> vCross1 = ( (D - C) ^ (A - C) );
    return ( MathRealFn->SignI(vCross0 ^ vCross1, fZeroTolerance) == 0 );
}

template<typename Real>
inline TPlane<Real> TQuad3<Real>::GetPlane( Bool bCounterClockWise, Bool bRightHanded ) const {
    TVector3<Real> vNormal;
    if ( bCounterClockWise == bRightHanded )
        vNormal = ( (B - A) ^ (C - A) );
    else
        vNormal = ( (C - A) ^ (B - A) );
    vNormal.Normalize();
    return TPlane<Real>( vNormal, A );
}

template<typename Real>
UInt TQuad3<Real>::ClosestFeature( const TVertex3<Real> & vPoint, Bool * outIsEdgeElseVertex ) const
{
    Assert(false);
    return INVALID_OFFSET;
}
template<typename Real>
TVertex3<Real> TQuad3<Real>::ClosestPoint( const TVertex3<Real> & vPoint ) const
{
    Assert(false);
    return TVertex3<Real>::Null;
}
template<typename Real>
Real TQuad3<Real>::Distance( const TVertex3<Real> & vPoint ) const
{
    Assert(false);
    return (Real)0;
}


