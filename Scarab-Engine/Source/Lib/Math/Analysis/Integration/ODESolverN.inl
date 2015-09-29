/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Analysis/Integration/ODESolverN.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Numerical solver for ordinary differential equations
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TODESolverN implementation
template<typename Real>
TODESolverN<Real>::TODESolverN( Function pfFunction, UInt iDimension, Void * pUserData )
{
    m_iDimension = iDimension;
    m_pfFunction = pfFunction;
    m_pUserData = pUserData;

    SetStepValue( (Real)SCALAR_ERROR );
}
template<typename Real>
TODESolverN<Real>::~TODESolverN()
{
    // nothing to do
}

template<typename Real>
inline Real TODESolverN<Real>::GetStepValue() const {
    return m_fStepValue;
}

template<typename Real>
inline Void * TODESolverN<Real>::GetUserData() const {
    return m_pUserData;
}
template<typename Real>
inline Void TODESolverN<Real>::SetUserData( Void * pUserData ) {
    m_pUserData = pUserData;
}

template<typename Real>
inline UInt TODESolverN<Real>::GetDimension() const {
    return m_iDimension;
}
template<typename Real>
inline typename TODESolverN<Real>::Function TODESolverN<Real>::GetFunction() const {
    return m_pfFunction;
}
template<typename Real>
inline Void TODESolverN<Real>::SetFunction( Function pfFunction ) {
    m_pfFunction = pfFunction;
}

/////////////////////////////////////////////////////////////////////////////////
// TODESolverEulerN implementation
template<typename Real>
TODESolverEulerN<Real>::TODESolverEulerN( Function pfFunction, UInt iDimension, Void * pUserData ):
    TODESolverN<Real>( pfFunction, iDimension, pUserData )
{
    // nothing to do
}
template<typename Real>
TODESolverEulerN<Real>::~TODESolverEulerN()
{
    // nothing to do
}

template<typename Real>
Void TODESolverEulerN<Real>::SetStepValue( Real fStepValue )
{
    m_fStepValue = fStepValue;
}

template<typename Real>
Void TODESolverEulerN<Real>::Update( TVectorN<Real> & outX, Real & outT, const TVectorN<Real> & vX, Real fT )
{
    Assert( m_iDimension == vX.GetSize() );

    m_pfFunction( outX, vX, fT, m_pUserData );
    for( UInt i = 0; i < m_iDimension; ++i )
        outX[i] = vX[i] + ( outX[i] * m_fStepValue );

    outT = ( fT + m_fStepValue );
}

/////////////////////////////////////////////////////////////////////////////////
// TODESolverImplicitEulerN implementation
template<typename Real>
TODESolverImplicitEulerN<Real>::TODESolverImplicitEulerN( Function pfFunction, DerivativeFunction pfDerivativeFunction,
                                                          UInt iDimension, Void * pUserData ):
    TODESolverN<Real>( pfFunction, iDimension, pUserData ),
    m_matIdentity( iDimension, iDimension ), m_matDF( iDimension, iDimension )
{
    m_pfDerivativeFunction = pfDerivativeFunction;
    m_matIdentity.MakeIdentity();
}
template<typename Real>
TODESolverImplicitEulerN<Real>::~TODESolverImplicitEulerN()
{
    // nothing to do
}

template<typename Real>
Void TODESolverImplicitEulerN<Real>::SetStepValue( Real fStepValue )
{
    m_fStepValue = fStepValue;
}

template<typename Real>
Void TODESolverImplicitEulerN<Real>::Update( TVectorN<Real> & outX, Real & outT, const TVectorN<Real> & vX, Real fT )
{
    Assert( m_iDimension == vX.GetSize() );

    m_pfDerivativeFunction( m_matDF, vX, fT, m_pUserData );
    TMatrixN<Real> matDG = ( m_matIdentity - (m_matDF * m_fStepValue) );
    TMatrixN<Real> matInvDG(iDimension, iDimension);
    Bool bInvertible = matDG.Invert( matInvDG );
    if ( bInvertible ) {
        m_pfFunction( outX, vX, fT, m_pUserData );
        outX = (matInvDG * outX);
        for( UInt i = 0; i < m_iDimension; ++i )
            outX[i] = vX[i] + ( outX[i] * m_fStepValue );
    } else
        outX = vX;

    outT = ( fT + m_fStepValue );
}

/////////////////////////////////////////////////////////////////////////////////
// TODESolverHeunN implementation
template<typename Real>
TODESolverHeunN<Real>::TODESolverHeunN( Function pfFunction, UInt iDimension, Void * pUserData ):
    TODESolverN<Real>( pfFunction, iDimension, pUserData ),
    m_vHalfX( iDimension )
{
    // nothing to do
}
template<typename Real>
TODESolverHeunN<Real>::~TODESolverHeunN()
{
    // nothing to do
}

template<typename Real>
Void TODESolverHeunN<Real>::SetStepValue( Real fStepValue )
{
    m_fStepValue = fStepValue;
    m_fHalfStep = ( m_fStepValue * (Real)0.5f );
}

template<typename Real>
Void TODESolverHeunN<Real>::Update( TVectorN<Real> & outX, Real & outT, const TVectorN<Real> & vX, Real fT )
{
    Assert( m_iDimension == vX.GetSize() );
    UInt i;

    // Step 1
    m_pfFunction( outX, vX, fT, m_pUserData );
    for( i = 0; i < m_iDimension; ++i )
        m_vHalfX[i] = vX[i] + ( outX[i] * m_fHalfStep );

    // Step 2
    Real fHalfT = fT + m_fHalfStep;
    m_pfFunction( outX, m_vHalfX, fHalfT, m_pUserData );
    for( i = 0; i < m_iDimension; ++i )
        outX[i] = vX[i] + ( outX[i] * m_fStepValue );

    outT = ( fT + m_fStepValue );
}

/////////////////////////////////////////////////////////////////////////////////
// TODESolverRungeKuttaN implementation
template<typename Real>
TODESolverRungeKuttaN<Real>::TODESolverRungeKuttaN( Function pfFunction, UInt iDimension, Void * pUserData ):
    TODESolverN<Real>( pfFunction, iDimension, pUserData ),
    m_vFX1( iDimension ), m_vFX2( iDimension ), m_vFX3( iDimension ), m_vFX4( iDimension )
{
    // nothing to do
}
template<typename Real>
TODESolverRungeKuttaN<Real>::~TODESolverRungeKuttaN()
{
    // nothing to do
}

template<typename Real>
Void TODESolverRungeKuttaN<Real>::SetStepValue( Real fStepValue )
{
    m_fStepValue = fStepValue;
    m_fHalfStep = ( m_fStepValue * (Real)0.5f );
    m_fSixthStep = ( m_fStepValue / (Real)6 );
}

template<typename Real>
Void TODESolverRungeKuttaN<Real>::Update( TVectorN<Real> & outX, Real & outT, const TVectorN<Real> & vX, Real fT )
{
    Assert( m_iDimension == vX.GetSize() );
    UInt i;

    // Step 1
    m_pfFunction( m_vFX1, vX, fT, m_pUserData );
    for( i = 0; i < m_iDimension; ++i )
        outX[i] = vX[i] + ( m_vFX1[i] * m_fHalfStep );

    // Step 2
    Real fHalfT = fT + m_fHalfStep;
    m_pfFunction( m_vFX2, outX, fHalfT, m_pUserData );
    for( i = 0; i < m_iDimension; ++i )
        outX[i] = vX[i] + ( m_vFX2[i] * m_fHalfStep );

    // Step 3
    m_pfFunction( m_vFX3, outX, fHalfT, m_pUserData );
    for( i = 0; i < m_iDimension; ++i )
        outX[i] = vX[i] + ( m_vFX3[i] * m_fStep );

    // Step 4
    outT = ( fT + m_fStepValue );
    m_pfFunction( m_vFX4, outX, outT, m_pUserData );
    for( i = 0; i < m_iDimension; ++i ) {
        outX[i] = vX[i] + m_fSixthStep * (
            m_vFX1[i] + ((m_vFX2[i] + m_vFX3[i]) * (Real)2) + m_vFX4[i]
        );
    }
}

