/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Analysis/Interpolation/InterpolatorLinear.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Linear Interpolation for 1D data
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TInterpolatorLinear implementation
template<typename Real>
const TMatrix2<Real> TInterpolatorLinear<Real>::sm_matBlend = TMatrix2<Real> (
    (Real)1, (Real)(-1),
    (Real)0, (Real)1
);

template<typename Real>
TInterpolatorLinear<Real>::TInterpolatorLinear( Real * arrValues, UInt iBound,
                                                Real fMin, Real fSpacing )
{
    Assert( arrValues != NULL );
    Assert( iBound >= 2 );

    m_iBound = iBound;
    m_arrValues = arrValues;

    m_fMin = fMin;
    m_fSpacing = fSpacing;
    m_fMax = m_fMin + m_fSpacing * ( m_iBound - 1 );
    m_fInvSpacing = MathRealFn->Invert( m_fSpacing );
}
template<typename Real>
TInterpolatorLinear<Real>::~TInterpolatorLinear()
{
    // nothing to do
}

template<typename Real>
inline UInt TInterpolatorLinear<Real>::GetBound() const {
    return m_iBound;
}
template<typename Real>
inline Real * TInterpolatorLinear<Real>::GetValues() const {
    return m_arrValues;
}

template<typename Real>
inline Real TInterpolatorLinear<Real>::GetMin() const {
    return m_fMin;
}
template<typename Real>
inline Real TInterpolatorLinear<Real>::GetMax() const {
    return m_fMax;
}
template<typename Real>
inline Real TInterpolatorLinear<Real>::GetSpacing() const {
    return m_fSpacing;
}

template<typename Real>
Real TInterpolatorLinear<Real>::Evaluate( Real fParameter ) const
{
    // Domain bounds
    if ( (fParameter < m_fMin) || (fParameter > m_fMax) ) {
        Assert(false);
        return MathRealFn->Infinity;
    }

    // Indices
    Real fT = (fParameter - m_fMin) * m_fInvSpacing;
    Int iT = MathRealFn->Floor(fT);
    if ( iT < 0 )
        iT = 0;
    else if ( iT >= m_iBound )
        iT = m_iBound - 1;
    Int iTP1 = iT + 1;
    if ( iTP1 >= m_iBound )
        iTP1 = m_iBound - 1;
    Real fOffsetT = ( fT - (Real)iT );

    // Blended vectors
    TVector2<Real> vU( (Real)1, fOffsetT );
    TVector2<Real> vP = ( sm_matBlend * vU );

    // Interpolate
    Real fRes = (Real)0;
    fRes += ( vP.X * m_arrValues[iT] ); // L
    fRes += ( vP.Y * m_arrValues[iTP1] ); // R
    return fRes;
}
template<typename Real>
Real TInterpolatorLinear<Real>::Evaluate( Real fParameter, UInt iOrder ) const
{
    // Domain bounds
    if ( (fParameter < m_fMin) || (fParameter > m_fMax) ) {
        Assert(false);
        return MathRealFn->Infinity;
    }

    // Linear function derivatives are null from 2nd order and upper
    if ( iOrder >= 2 )
        return MathRealFn->Zero;

    // Indices
    Real fT = (fParameter - m_fMin) * m_fInvSpacing;
    Int iT = MathRealFn->Floor(fT);
    if ( iT < 0 )
        iT = 0;
    else if ( iT >= m_iBound )
        iT = m_iBound - 1;
    Int iTP1 = iT + 1;
    if ( iTP1 >= m_iBound )
        iTP1 = m_iBound - 1;
    Real fOffsetT = ( fT - (Real)iT );

    // Blended vectors
    TVector2<Real> vU;
    Real fFactor;
    if ( iOrder == 0 ) {
        vU.X = (Real)1;
        vU.Y = fOffsetT;
        fFactor = (Real)1;
    } else { // ( iOrder == 1 )
        vU.X = (Real)0;
        vU.Y = (Real)1;
        fFactor = m_fInvSpacing;
    }
    TVector2<Real> vP = ( sm_matBlend * vU );

    // Interpolate
    Real fRes = (Real)0;
    fRes += ( vP.X * m_arrValues[iT] ); // L
    fRes += ( vP.Y * m_arrValues[iTP1] ); // R
    fRes *= fFactor;
    return fRes;
}

