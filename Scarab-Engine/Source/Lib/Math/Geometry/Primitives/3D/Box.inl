/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Primitives/3D/Box.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives 3D : Box
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TBox implementation
template<typename Real>
TBox<Real>::TBox():
    Center(), Extents()
{
    // nothing to do
}
template<typename Real>
TBox<Real>::TBox( const TVertex3<Real> & vCenter, const TVector3<Real> & vExtents ):
    Center(vCenter), Extents(vExtents)
{
    // nothing to do
}
template<typename Real>
TBox<Real>::TBox( const TBox<Real> & rhs ):
    Center(rhs.Center), Extents(rhs.Extents)
{
    // nothing to do
}
template<typename Real>
TBox<Real>::~TBox()
{
    // nothing to do
}

template<typename Real>
inline TBox<Real> & TBox<Real>::operator=( const TBox<Real> & rhs ) {
    Center = rhs.Center;
    Extents = rhs.Extents;
    return (*this);
}

template<typename Real>
inline Bool TBox<Real>::TestInclusion( const TVertex3<Real> & vPoint ) const {
    TVector3<Real> vU = ( vPoint - Center );
    if ( MathRealFn->Abs( vU.X ) > Extents.X )
        return false;
    if ( MathRealFn->Abs( vU.Y ) > Extents.Y )
        return false;
    if ( MathRealFn->Abs( vU.Z ) > Extents.Z )
        return false;
    return true;
}

template<typename Real>
inline TVertex3<Real> TBox<Real>::ClosestPoint( const TVertex3<Real> & vPoint ) const {
    TVector3<Real> vU = ( vPoint - Center );
    TVertex3<Real> vClosestPoint = Center;
    for( UInt i = 0; i < 3; ++i )
        vClosestPoint[i] += Clamp<Real>( vU[i], -Extents[i], +Extents[i] );
    return vClosestPoint;
}
template<typename Real>
inline Real TBox<Real>::Distance( const TVertex3<Real> & vPoint ) const {
    if ( TestInclusion(vPoint) )
        return (Real)0;
    TVertex3<Real> vClosestPoint = ComputeClosestPoint( vPoint );
    return (vPoint - vClosestPoint).Norm();
}

template<typename Real>
Void TBox<Real>::ExtractVertices( TVertex3<Real> outVertices[8] ) const
{
    TVector3<Real> vX = ( TVector3<Real>::eI * Extents.X );
    TVector3<Real> vY = ( TVector3<Real>::eJ * Extents.Y );
    TVector3<Real> vZ = ( TVector3<Real>::eK * Extents.Z );
    outVertices[0] = ( Center - vX + vY + vZ );
    outVertices[1] = ( Center - vX + vY - vZ );
    outVertices[2] = ( Center - vX - vY - vZ );
    outVertices[3] = ( Center - vX - vY + vZ );
    outVertices[4] = ( Center + vX + vY + vZ );
    outVertices[5] = ( Center + vX - vY + vZ );
    outVertices[6] = ( Center + vX - vY - vZ );
    outVertices[7] = ( Center + vX + vY - vZ );
}
