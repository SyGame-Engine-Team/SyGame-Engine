/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Primitives/3D/Planar/Triangle3.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives 3D : Planar, Triangle3
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs :
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TTriangle3 implementation
template<typename Real>
TTriangle3<Real>::TTriangle3():
    A(), B(), C()
{
    // nothing to do
}
template<typename Real>
TTriangle3<Real>::TTriangle3( const TVertex3<Real> & vA, const TVertex3<Real> & vB, const TVertex3<Real> & vC ):
    A(vA), B(vB), C(vC)
{
    // nothing to do
}
template<typename Real>
TTriangle3<Real>::TTriangle3( const TVertex3<Real> vArr[3] ):
    A(vArr[0]), B(vArr[1]), C(vArr[2])
{
    // nothing to do
}
template<typename Real>
TTriangle3<Real>::TTriangle3( const TTriangle3<Real> & rhs ):
    A(rhs.A), B(rhs.B), C(rhs.C)
{
    // nothing to do
}
template<typename Real>
TTriangle3<Real>::~TTriangle3()
{
    // nothing to do
}

template<typename Real>
inline TTriangle3<Real> & TTriangle3<Real>::operator=( const TTriangle3<Real> & rhs ) {
    A = rhs.A;
    B = rhs.B;
    C = rhs.C;
    return (*this);
}

template<typename Real>
inline Bool TTriangle3<Real>::IsDegenerated( Real fZeroTolerance ) const {
    TVector3<Real> vAB = (B - A), vAC = (C - A);
    Real fNorm = MathRealFn->InvSqrt( vAB.NormSqr() * vAC.NormSqr() );
    Real fFlatCoeff = ( (vAB * vAC) * fNorm );
    return ( MathRealFn->Abs(fFlatCoeff - (Real)1) < fZeroTolerance );
}

template<typename Real>
inline TPlane<Real> TTriangle3<Real>::GetPlane( Bool bCounterClockWise, Bool bRightHanded ) const {
    TVector3<Real> vNormal;
    if ( bCounterClockWise == bRightHanded )
        vNormal = ( (B - A) ^ (C - A) );
    else
        vNormal = ( (C - A) ^ (B - A) );
    vNormal.Normalize();
    return TPlane<Real>( vNormal, A );
}
template<typename Real>
Bool TTriangle3<Real>::Contains( const TVertex3<Real> & vPoint, const TVector3<Real> & vNormal ) const
{
    TVector3<Real> vEdge0 = ( B - A );
    TVector3<Real> vEdge1 = ( C - B );
    TVector3<Real> vEdge2 = ( A - C );
    TVector3<Real> vDeltaA = ( vPoint - A );
    TVector3<Real> vDeltaB = ( vPoint - B );
    TVector3<Real> vDeltaC = ( vPoint - C );
    TVector3<Real> vCross0 = ( vEdge0 ^ vNormal );
    TVector3<Real> vCross1 = ( vEdge1 ^ vNormal );
    TVector3<Real> vCross2 = ( vEdge2 ^ vNormal );
    Real fDot0 = ( vCross0 * vDeltaA );
    Real fDot1 = ( vCross1 * vDeltaB );
    Real fDot2 = ( vCross2 * vDeltaC );

    return ( ( (fDot0 >  (Real)0) && (fDot1 >  (Real)0) && (fDot2 >  (Real)0) ) ||
             ( (fDot0 <= (Real)0) && (fDot1 <= (Real)0) && (fDot2 <= (Real)0) ) );
}

template<typename Real>
UInt TTriangle3<Real>::ClosestFeature( const TVertex3<Real> & vPoint, Bool * outIsEdgeElseVertex ) const
{
    Assert(false);
    return INVALID_OFFSET;
}
template<typename Real>
TVertex3<Real> TTriangle3<Real>::ClosestPoint( const TVertex3<Real> & vPoint ) const
{
    Assert(false);
    return TVertex3<Real>::Null;
}
template<typename Real>
Real TTriangle3<Real>::Distance( const TVertex3<Real> & vPoint ) const
{
    Assert(false);
    return (Real)0;
}
