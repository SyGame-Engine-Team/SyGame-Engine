/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Surfaces/ImplicitSurface.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Surfaces : Base class for surfaces defined by a function
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TImplicitSurface implementation
template<typename Real>
TImplicitSurface<Real>::TImplicitSurface()
{
    // nothing to do
}
template<typename Real>
TImplicitSurface<Real>::~TImplicitSurface()
{
    // nothing to do
}

template<typename Real>
inline Int TImplicitSurface<Real>::ClassifyPoint( const TVertex3<Real> & vPosition, Real fTolerance ) const {
    Real fValueF = F( vPosition );
    if ( fValueF < -fTolerance )
        return -1;
    if ( fValueF > +fTolerance )
        return +1;
    return 0;
}

template<typename Real>
inline TVector3<Real> TImplicitSurface<Real>::GetGradient( const TVertex3<Real> & vPosition ) const {
    return TVector3<Real>( FdX(vPosition), FdY(vPosition), FdZ(vPosition) );
}
template<typename Real>
TMatrix3<Real> TImplicitSurface<Real>::GetHessian( const TVertex3<Real> & vPosition ) const
{
    Real fDX2 = FdX2(vPosition);
    Real fDY2 = FdY2(vPosition);
    Real fDZ2 = FdZ2(vPosition);
    Real fDXDY = FdXdY(vPosition);
    Real fDXDZ = FdXdZ(vPosition);
    Real fDYDZ = FdYdZ(vPosition);
    return TMatrix3<Real>( fDX2,  fDXDY, fDXDZ,
                           fDXDY, fDY2,  fDYDZ,
                           fDXDZ, fDYDZ, fDZ2 );
}

template<typename Real>
Void TImplicitSurface<Real>::Frame( const TVertex3<Real> & vPosition, TVector3<Real> & outNormal,
                                    TVector3<Real> & outTangentU, TVector3<Real> & outTangentV ) const
{
    outNormal = GetGradient( vPosition );
    outNormal.Normalize();
    TVector3<Real>::MakeComplementBasis( outTangentU, outTangentV, outNormal );
}

template<typename Real>
Bool TImplicitSurface<Real>::PrincipalCurvature( const TVertex3<Real> & vPosition, Real & outCurvature0, Real & outCurvature1,
                                                                                   TVector3<Real> & outAxis0, TVector3<Real> & outAxis1 ) const
{
    // Gradient
    Real fDX = FdX(vPosition);
    Real fDY = FdY(vPosition);
    Real fDZ = FdZ(vPosition);
    Real fLength = MathRealFn->Sqrt( fDX*fDX + fDY*fDY + fDZ*fDZ );
    if ( fLength <= (Real)SCALAR_ERROR )
        return false;

    Real fSqrDX = fDX * fDX;
    Real fSqrDY = fDY * fDY;
    Real fSqrDZ = fDZ * fDZ;
    Real fDXmDY = fDX * fDY;
    Real fDXmDZ = fDX * fDZ;
    Real fDYmDZ = fDY * fDZ;
    Real fInvLength = MathRealFn->Invert( fLength );
    Real fInvLength2 = fInvLength * fInvLength;
    Real fInvLength3 = fInvLength * fInvLength2;
    Real fInvLength4 = fInvLength2 * fInvLength2;

    // Hessian
    Real fDX2 = FdX2(vPosition);
    Real fDY2 = FdY2(vPosition);
    Real fDZ2 = FdZ2(vPosition);
    Real fDXDY = FdXdY(vPosition);
    Real fDXDZ = FdXdZ(vPosition);
    Real fDYDZ = FdYdZ(vPosition);

    // Mean curvature
    Real fMeanCurv = (
          fDX2 * (fSqrDY + fSqrDZ)
        + fDY2 * (fSqrDX + fSqrDZ)
        + fDZ2 * (fSqrDX + fSqrDY)
        - ( fDXDY * fDXmDY + fDXDZ * fDXmDZ + fDYDZ * fDYmDZ ) * (Real)2
    ) * fInvLength3 * (Real)0.5f;

    // Gaussian curvature
    Real fGaussCurv = (
          fSqrDX * (fDY2 * fDZ2 - fDYDZ * fDYDZ)
        + fSqrDY * (fDX2 * fDZ2 - fDXDZ * fDXDZ)
        + fSqrDZ * (fDX2 * fDY2 - fDXDY * fDXDY)
        + ( fDXmDY * (fDXDZ * fDYDZ - fDXDY * fDZ2)
          + fDXmDZ * (fDXDY * fDYDZ - fDXDZ * fDY2)
          + fDYmDZ * (fDXDY * fDXDZ - fDYDZ * fDX2) ) * (Real)2
    ) * fInvLength4;

    // Principal curvature
    Real fDiscr = MathRealFn->Sqrt( MathRealFn->Abs( fMeanCurv * fMeanCurv - fGaussCurv ) );
    outCurvature0 = fMeanCurv - fDiscr;
    outCurvature1 = fMeanCurv + fDiscr; 

    // Principal directions
    fSqrDXd2m1 = ( fSqrDX * fInvLength2 - (Real)1 );
    fSqrDYd2m1 = ( fSqrDY * fInvLength2 - (Real)1 );
    fSqrDZd2m1 = ( fSqrDZ * fInvLength2 - (Real)1 );

    TMatrix3<Real> matSystem;
    matSystem.m00 = ( fInvLength * ( fSqrDXd2m1 * fDX2  ) + fInvLength3 * ( fDXmDY * fDXDY + fDXmDZ * fDXDZ ) );
    matSystem.m01 = ( fInvLength * ( fSqrDXd2m1 * fDXDY ) + fInvLength3 * ( fDXmDY * fDY2  + fDXmDZ * fDYDZ ) );
    matSystem.m02 = ( fInvLength * ( fSqrDXd2m1 * fDXDZ ) + fInvLength3 * ( fDXmDY * fDYDZ + fDXmDZ * fDZ2  ) );

    matSystem.m10 = ( fInvLength * ( fSqrDYd2m1 * fDXDY ) + fInvLength3 * ( fDXmDY * fDX2  + fDYmDZ * fDXDZ ) );
    matSystem.m11 = ( fInvLength * ( fSqrDYd2m1 * fDY2  ) + fInvLength3 * ( fDXmDY * fDXDY + fDYmDZ * fDYDZ ) );
    matSystem.m12 = ( fInvLength * ( fSqrDYd2m1 * fDYDZ ) + fInvLength3 * ( fDXmDY * fDXDZ + fDYmDZ * fDZ2  ) );

    matSystem.m20 = ( fInvLength * ( fSqrDZd2m1 * fDXDZ ) + fInvLength3 * ( fDXmDZ * fDX2  + fDYmDZ * fDXDY ) );
    matSystem.m21 = ( fInvLength * ( fSqrDZd2m1 * fDYDZ ) + fInvLength3 * ( fDXmDZ * fDXDY + fDYmDZ * fDY2  ) );
    matSystem.m22 = ( fInvLength * ( fSqrDZd2m1 * fDZ2  ) + fInvLength3 * ( fDXmDZ * fDXDZ + fDYmDZ * fDYDZ ) );

    Real fTmp1 = matSystem.m00 + outCurvature0;
    Real fTmp2 = matSystem.m11 + outCurvature0;
    Real fTmp3 = matSystem.m22 + outCurvature0;

    TVector3<Real> arrUs[3];
    Real arrLengths[3];

    arrUs[0].X = matSystem.m01 * matSystem.m12 - matSystem.m02 * fTmp2;
    arrUs[0].Y = matSystem.m02 * matSystem.m10 - matSystem.m12 * fTmp1;
    arrUs[0].Z = fTmp1 * fTmp2 - matSystem.m01 * matSystem.m10;
    arrLengths[0] = arrUs[0].Norm();

    arrUs[1].X = matSystem.m01 * fTmp3 - matSystem.m02 * matSystem.m21;
    arrUs[1].Y = matSystem.m02 * matSystem.m20 - fTmp1 * fTmp3;
    arrUs[1].Z = fTmp1 * matSystem.m21 - matSystem.m01 * matSystem.m20;
    arrLengths[1] = arrUs[1].Norm();

    arrUs[2].X = fTmp2 * fTmp3 - matSystem.m12 * matSystem.m21;
    arrUs[2].Y = matSystem.m12 * matSystem.m20 - matSystem.m10 * fTmp3;
    arrUs[2].Z = matSystem.m10 * matSystem.m21 - matSystem.m20 * fTmp2;
    arrLengths[2] = arrUs[2].Norm();

    UInt iMaxIndex = 0;
    Real fMaxLength = arrLengths[0];
    if ( arrLengths[1] > fMaxLength ) {
        iMaxIndex = 1;
        fMaxLength = arrLengths[1];
    }
    if ( arrLengths[2] > fMaxLength )
        iMaxIndex = 2;
    fInvLength = MathRealFn->Invert( arrLengths[iMaxIndex] );
    arrUs[iMaxIndex] *= fInvLength;

    outAxis1 = arrUs[iMaxIndex];
    outAxis0 = ( outAxis1 ^ TVector3<Real>(fDX, fDY, fDZ) );
    outAxis0.Normalize();

    return true;
}

