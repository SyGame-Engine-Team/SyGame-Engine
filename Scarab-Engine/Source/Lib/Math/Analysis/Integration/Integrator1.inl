/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Analysis/Integration/Integrator1.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Numerical Integration for single variable functions
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TIntegrator1 implementation
template<typename Real>
TIntegrator1<Real>::TIntegrator1( Function pfFunction, Void * pUserData )
{
    m_pfFunction = pfFunction;
    m_pUserData = pUserData;

    TrapezoidSampleCount = 16;
    RombergOrder = 8;
}
template<typename Real>
TIntegrator1<Real>::~TIntegrator1()
{
    // nothing to do
}

template<typename Real>
inline Void * TIntegrator1<Real>::GetUserData() const {
    return m_pUserData;
}
template<typename Real>
inline Void TIntegrator1<Real>::SetUserData( Void * pUserData ) {
    m_pUserData = pUserData;
}

template<typename Real>
inline typename TIntegrator1<Real>::Function TIntegrator1<Real>::GetFunction() const {
    return m_pfFunction;
}
template<typename Real>
inline Void TIntegrator1<Real>::SetFunction( Function pfFunction ) {
    m_pfFunction = pfFunction;
}

template<typename Real>
Real TIntegrator1<Real>::GaussianQuadrature( Real fT0, Real fT1 )
{
    // Okay a little about Legendre polynomial serie ...
    // Here is Legendre's ODE :
    //   d/dx[ (1-x^2) * d/dx(Pn(x)) ] + n * (n+1) * Pn(x) = 0
    // Pn(x) is a sequence of polynomials called Legendre polynoms.
    // By solving we find :
    //   Pn(x) = ( 1 / (2^n * n!) ) * ((d/dx)^n)[ (x^2 - 1)^n ]
    // Pn(x) has degree n, and some useful properties :
    //   (n+1) * Pn+1(x) = (2n+1) * x * Pn(x) - n * Pn-1(x)  // Recursive definition
    //   (2n+1) * Pn(x) = d/dx[ Pn+1(x) - Pn-1(x) ]          // Integrable relation
    // Let's use P5(x) which is quite sufficient here (computed with recursive formula) :
    //   P5(x) = 1/8 * ( 63 * x^5 - 70 * x^3 + 15 * x )
    // Then we compute it's roots :
    //   x=0 is evident and leaves P5(x) = x * 1/8 * ( 63 * x^4 - 70 * x^2 + 15 )
    // We define X = x^2, leaving ( 63 * X^2 - 70 * X + 15 ) to solve ...
    // And finally we use pounding coeffs with respect to Taylor decomposition
    static const UInt iLegendreDegree = 5;
    static const Real arrLegendreRoots[iLegendreDegree] = {
        -0.9061798459f, -0.5384693101f, 0.0f, +0.5384693101f, +0.9061798459f
    };
    static const Real arrLegendreCoeffs[iLegendreDegree] = {
        0.2369268850f, 0.4786286705f, 0.5688888889f, 0.4786286705f, 0.2369268850f
    };
    Real fRadius = (fT1 - fT0) * 0.5f;
    Real fCenter = (fT1 + fT0) * 0.5f;
    Real fRes = 0.0f;
    for( UInt i = 0; i < iLegendreDegree; ++i )
        fRes += arrLegendreCoeffs[i] * m_pfFunction( fCenter + (fRadius * arrLegendreRoots[i]), m_pUserData );
    fRes *= fRadius;
    return fRes;
}
template<typename Real>
Real TIntegrator1<Real>::TrapezoidRule( Real fT0, Real fT1 )
{
    Assert( TrapezoidSampleCount >= 2 );
    UInt iSampleMax = ( TrapezoidSampleCount - 1 );

    Real fSlope = (fT1 - fT0) / (Real)iSampleMax;
    Real fRes = 0.5f * ( m_pfFunction(fT0, m_pUserData) + m_pfFunction(fT1, m_pUserData) );
    for( UInt i = 1; i < iSampleMax; ++i )
        fRes += m_pfFunction( fT0 + (i * fSlope), m_pUserData );
    fRes *= fSlope;
    return fRes;
}
template<typename Real>
Real TIntegrator1<Real>::RombergIntegral( Real fT0, Real fT1 )
{
    Assert( RombergOrder > 0 && RombergOrder <= INTEGRATOR1_ROMBERG_MAX_ORDER );

    static Real arrRomberg0[INTEGRATOR1_ROMBERG_MAX_ORDER];
    static Real arrRomberg1[INTEGRATOR1_ROMBERG_MAX_ORDER];

    Real fDelta = (fT1 - fT0);
    arrRomberg0[0] = 0.5f * fDelta * ( m_pfFunction(fT0, m_pUserData) + m_pfFunction(fT1, m_pUserData) );

    Real fSum;
    UInt i0, i1, i2, iPow4, iPow2 = 1;
    for( i0 = 2; i0 <= RombergOrder; ++i0 ) {
        // Trapezoid approximation
        fSum = 0.0f;
        for( i1 = 1; i1 <= iPow2; ++i1 )
            fSum += m_pfFunction( fT0 + ((Real)i1 - 0.5f) * fDelta, m_pUserData );

        // Richardson extrapolation
        arrRomberg1[0] = 0.5f * ( arrRomberg0[0] + fSum * fDelta );
        iPow4 = 4;
        for( i2 = 1; i2 < i0; ++i2 ) {
            arrRomberg1[i2] = ( (iPow4 * arrRomberg1[i2-1]) - arrRomberg0[i2-1] ) / ( iPow4 - 1 );
            iPow4 <<= 2;
        }
        for( i1 = 0; i1 < i0; ++i1 )
            arrRomberg0[i1] = arrRomberg1[i1];

        iPow2 <<= 1;
        fDelta *= 0.5f;
    }

    return arrRomberg0[RombergOrder - 1];
}


