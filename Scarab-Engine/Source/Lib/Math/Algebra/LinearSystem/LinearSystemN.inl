/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Algebra/LinearSystem/LinearSystemN.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : General NxM linear systems solver
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TLinearSystemN implementation
template<typename Real>
TLinearSystemN<Real>::TLinearSystemN( Real fZeroTolerance ):
    m_matA(), m_vB(), m_matInvA(), m_vX()
{
    ZeroTolerance = fZeroTolerance;
}
template<typename Real>
TLinearSystemN<Real>::TLinearSystemN( const TMatrixN<Real> & matA, const TVectorN<Real> & vB,
                                      Real fZeroTolerance ):
    m_matA(matA), m_vB(vB), m_matInvA(), m_vX()
{
    ZeroTolerance = fZeroTolerance;
}
template<typename Real>
TLinearSystemN<Real>::~TLinearSystemN()
{
    // nothing to do
}

template<typename Real>
inline TMatrixN<Real> & TLinearSystemN<Real>::GetA() {
    return m_matA;
}
template<typename Real>
inline TVectorN<Real> & TLinearSystemN<Real>::GetB() {
    return m_vB;
}

template<typename Real>
Bool TLinearSystemN<Real>::SolveSystem()
{
    Bool bInvertible = m_matA.Invert( m_matInvA, ZeroTolerance );
    if ( !bInvertible )
        return false;
    m_vX = ( m_matInvA * m_vB );
    return true;
}
template<typename Real>
Bool TLinearSystemN<Real>::SolveSymmetricSystem()
{
    // "Matrix Computations", Conjugate gradient method,
    // Golum and Van Loan.
    const Real ZERO = (Real)0;
    TVectorN<Real> vR, vP, vW;
    Real fRho0, fRho1, fAlpha;
    Real fRoot0, fRoot1, fNorm, fBeta;
    UInt i;

    // First iteration
    m_vX.SetSize( m_vB.GetSize() );
    for( i = 0; i < iSystemSize; ++i )
        m_vX[i] = ZERO;
    vR = m_vB;
    fRho0 = ( vR * vR );
    vP = vR;
    vW = ( m_matA * vP );
    fAlpha = ( fRho0 / (vP * vW) );
    vX += ( vP * fAlpha );
    vR -= ( vW * fAlpha );
    fRho1 = ( vR * vR );

    // Main loop
    const UInt iMaxIterations = 1024;
    for( i = 0; i < iMaxIterations; ++i ) {
        fRoot0 = MathRealFn->Sqrt( fRho1 );
        fNorm = ( vB * vB );
        fRoot1 = MathRealFn->Sqrt( fNorm );
        if ( fRoot0 <= (fRoot1 * ZeroTolerance) )
            break;
        fBeta = ( fRho1 / fRho0 );
        vP *= fBeta;
        vP += vR;
        vW = ( m_matA * vP );
        fAlpha = ( fRho1 / (vP * vW) );
        vX += ( vP * fAlpha );
        vR -= ( vW * fAlpha );
        fRho0 = fRho1;
        fRho1 = ( vR * vR );
    }

    return ( i < iMaxIterations );
}

template<typename Real>
inline const TMatrixN<Real> & TLinearSystemN<Real>::GetInvA() const {
    return m_matInvA;
}
template<typename Real>
inline const TVectorN<Real> & TLinearSystemN<Real>::GetX() const {
    return m_vX;
}

