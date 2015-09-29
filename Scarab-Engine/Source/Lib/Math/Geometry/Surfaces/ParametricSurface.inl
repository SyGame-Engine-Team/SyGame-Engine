/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Surfaces/ParametricSurface.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Surfaces : Base class for parametric surfaces
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TParametricSurface implementation
template<typename Real>
TParametricSurface<Real>::TParametricSurface( Real fMinU, Real fMaxU, Real fMinV, Real fMaxV, Bool bTriangular )
{
    Assert( (fMinU < fMaxU) && (fMinV < fMaxV) );

    m_bTriangular = bTriangular;

    m_fMinU = fMinU;
    m_fMaxU = fMaxU;
    m_fMinV = fMinV;
    m_fMaxV = fMaxV;
}
template<typename Real>
TParametricSurface<Real>::~TParametricSurface()
{
    // nothing to do
}

template<typename Real>
inline Bool TParametricSurface<Real>::IsTriangular() const {
    return m_bTriangular;
}

template<typename Real>
inline Real TParametricSurface<Real>::GetMinU() const {
    return m_fMinU;
}
template<typename Real>
inline Real TParametricSurface<Real>::GetMaxU() const {
    return m_fMaxU;
}
template<typename Real>
inline Real TParametricSurface<Real>::GetMinV() const {
    return m_fMinV;
}
template<typename Real>
inline Real TParametricSurface<Real>::GetMaxV() const {
    return m_fMaxV;
}
template<typename Real>
inline Void TParametricSurface<Real>::SetIntervalU( Real fMinU, Real fMaxU ) {
    Assert( fMinU < fMaxU );
    m_fMinU = fMinU;
    m_fMaxU = fMaxU;
}
template<typename Real>
inline Void TParametricSurface<Real>::SetIntervalV( Real fMinV, Real fMaxV ) {
    Assert( fMinV < fMaxV );
    m_fMinV = fMinV;
    m_fMaxV = fMaxV;
}

template<typename Real>
inline TVertex3<Real> TParametricSurface<Real>::Position( Real fU, Real fV ) const {
    return P( fU, fV );
}

template<typename Real>
inline TVector3<Real> TParametricSurface<Real>::TangentU( Real fU, Real fV ) const {
    TVector3<Real> vTangentU = PdU( fU, fV );
    vTangentU.Normalize();
    return vTangentU;
}
template<typename Real>
inline TVector3<Real> TParametricSurface<Real>::TangentV( Real fU, Real fV ) const {
    TVector3<Real> vTangentV = PdV( fU, fV );
    vTangentV.Normalize();
    return vTangentV;
}
template<typename Real>
TVector3<Real> TParametricSurface<Real>::Normal( Real fU, Real fV ) const
{
    TVector3<Real> vTangentU = PdU( fU, fV );
    TVector3<Real> vTangentV = PdV( fU, fV );
    TVector3<Real> vNormal = ( vTangentU ^ vTangentV );
    vNormal.Normalize();
    return vNormal;
}

template<typename Real>
Void TParametricSurface<Real>::Frame( Real fU, Real fV, TVertex3<Real> & outPosition, TVector3<Real> & outNormal,
                                                        TVector3<Real> & outTangentU, TVector3<Real> & outTangentV ) const
{
    outPosition = P( fU, fV );
    outTangentU = PdU( fU, fV );
    outTangentV = PdV( fU, fV );
    outTangentU.Normalize();
    outTangentV.Normalize();
    outNormal = ( outTangentU ^ outTangentV );
    outTangentV = ( outNormal ^ outTangentU );
}

template<typename Real>
Void TParametricSurface<Real>::PrincipalCurvature( Real fU, Real fV, Real & outCurvature0, Real & outCurvature1,
                                                                     TVector3<Real> & outAxis0, TVector3<Real> & outAxis1 ) const
{
    TVector3<Real> vDU = PdU(fU, fV);
    TVector3<Real> vDV = PdV(fU, fV);
    TVector3<Real> vDU2 = PdU2(fU, fV);
    TVector3<Real> vDV2 = PdV2(fU, fV);
    TVector3<Real> vDUDV = PdUdV(fU, fV);

    TVector3<Real> vNormal = (vDU ^ vDV);
    vNormal.Normalize();

    Real fTmp;

    // Metric Tensor: G = ( (dU*dU), (dU*dV) )
    //                    ( (dV*dU), (dV*dV) )
    fTmp = (vDU * vDV);
    TMatrix2<Real> matMetricTensor;
    matMetricTensor.m00 = (vDU * vDU); matMetricTensor.m01 = fTmp;
    matMetricTensor.m10 = fTmp;        matMetricTensor.m11 = (vDV * vDV);

    // Curvature Tensor:  B = ( -(N*dU2) -(N*dUdV) )
    //                        ( -(N*dVdU) -(N*dV2) )
    fTmp = -(vNormal * vDUDV);
    TMatrix2<Real> matCurvatureTensor;
    matCurvatureTensor.m00 = -(vNormal * vDU2); matCurvatureTensor.m01 = fTmp;
    matCurvatureTensor.m10 = fTmp;              matCurvatureTensor.m11 = -(vNormal * vDV2);

    // Principal curvatures k are the generalized eigenvalues of (Bw = kGw)
    // If k is a curvature and w=(a,b) is the corresponding solution to
    // Bw = kGw, then the principal direction as a 3D vector is d = a*U+b*V.
    // Let k1 and k2 be the principal curvatures.  The Mean curvature
    // is (k1+k2)/2 and the Gaussian curvature is k1*k2.

    // Characteristic polynom : det(B-kG) = ( a*k^2 + b*k + c ) = 0
    Real fC = matCurvatureTensor.Determinant();
    Real fB = ( ( matCurvatureTensor.m01 * matMetricTensor.m01 * (Real)2 ) -
                ( matCurvatureTensor.m00 * matMetricTensor.m11 ) -
                ( matCurvatureTensor.m11 * matMetricTensor.m00 ) );
    Real fA = matMetricTensor.Determinant();

    // Roots of the polynomial
    fTmp = MathRealFn->Sqrt( MathRealFn->Abs( fB*fB - fA*fC*(Real)4 ) );
    Real fInvDenom = MathRealFn->Invert( fA * (Real)2 );
    outCurvature0 = ( -fB - fTmp ) * fInvDenom;
    outCurvature1 = ( -fB + fTmp ) * fInvDenom;

    // Principal axes are solution to (B-kG)w = 0 :
    //    W = ( b01 - k0*g01, -(b00 - k0*g00) )
    //     OR ( b11 - k0*g11, -(b01 - k0*g01) ).
    Real fW0 = matCurvatureTensor.m01 - outCurvature0 * matMetricTensor.m01;
    Real fW1 = outCurvature0 * matMetricTensor.m00 - matCurvatureTensor.m00;
    Real fLength = MathRealFn->Sqrt( fW0*fW0 + fW1*fW1 );
    if ( fLength >= (Real)SCALAR_ERROR )
        outAxis0 = ( vDU * fW0 + vDV * fW1 );
    else {
        fW0 = matCurvatureTensor.m11 - outCurvature0 * matMetricTensor.m11;
        fW1 = outCurvature0 * matMetricTensor.m01 - matCurvatureTensor.m01;
        fLength = MathRealFn->Sqrt( fW0*fW0 + fW1*fW1 );
        if ( fLength >= (Real)SCALAR_ERROR )
            outAxis0 = ( vDU * fW0 + vDV * fW1 );
        else {
            // Umbilic (surface is locally sphere, any direction principal)
            outAxis0 = vDU;
        }
    }
    outAxis0.Normalize();
    outAxis1 = ( outAxis0 ^ vNormal );
    outAxis1.Normalize();
}

template<typename Real>
Void TParametricSurface<Real>::SubDivideByParameter( Real * outParametersU, UInt iCountU, Real * outParametersV, UInt iCountV ) const
{
    Assert( (iCountU >= 2) && (iCountV >= 2) );

    Real fIntervalU = ( (m_fMaxU - m_fMinU) / (Real)(iCountU - 1) );
    Real fU = m_fMinU;
    for( UInt i = 0; i < iCountU; ++i ) {
        outParametersU[i] = fU;
        fU += fIntervalU;
    }

    Real fIntervalV = ( (m_fMaxV - m_fMinV) / (Real)(iCountV - 1) );
    Real fV = m_fMinV;
    for( UInt i = 0; i < iCountV; ++i ) {
        outParametersV[i] = fV;
        fV += fIntervalV;
    }
}

