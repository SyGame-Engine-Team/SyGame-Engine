/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Analysis/Interpolation/InterpolatorBilinear.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Bilinear Interpolation for 2D data
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TInterpolatorBilinear implementation
template<typename Real>
const TMatrix2<Real> TInterpolatorBilinear<Real>::sm_matBlend = TMatrix2<Real> (
    (Real)1, (Real)(-1),
    (Real)0, (Real)1
);

template<typename Real>
TInterpolatorBilinear<Real>::TInterpolatorBilinear( Real * arrValues, UInt iBoundX, UInt iBoundY,
                                                    const TVector2<Real> & vMin, const TVector2<Real> & vSpacing ):
    m_vMin(vMin), m_vMax(), m_vSpacing(vSpacing), m_vInvSpacing()
{
    Assert( arrValues != NULL );
    Assert( (iBoundX >= 2) && (iBoundY >= 2) );

    m_iBoundX = iBoundX;
    m_iBoundY = iBoundY;
    m_arrValues = arrValues;

    m_vMax.X = m_vMin.X + m_vSpacing.X * ( m_iBoundX - 1 );
    m_vMax.Y = m_vMin.Y + m_vSpacing.Y * ( m_iBoundY - 1 );
    m_vInvSpacing.X = MathRealFn->Invert( m_vSpacing.X );
    m_vInvSpacing.Y = MathRealFn->Invert( m_vSpacing.Y );
}
template<typename Real>
TInterpolatorBilinear<Real>::~TInterpolatorBilinear()
{
    // nothing to do
}

template<typename Real>
inline UInt TInterpolatorBilinear<Real>::GetBoundX() const {
    return m_iBoundX;
}
template<typename Real>
inline UInt TInterpolatorBilinear<Real>::GetBoundY() const {
    return m_iBoundY;
}
template<typename Real>
inline Real * TInterpolatorBilinear<Real>::GetValues() const {
    return m_arrValues;
}

template<typename Real>
inline const TVector2<Real> & TInterpolatorBilinear<Real>::GetMin() const {
    return m_vMin;
}
template<typename Real>
inline const TVector2<Real> & TInterpolatorBilinear<Real>::GetMax() const {
    return m_vMax;
}
template<typename Real>
inline const TVector2<Real> & TInterpolatorBilinear<Real>::GetSpacing() const {
    return m_vSpacing;
}

template<typename Real>
Real TInterpolatorBilinear<Real>::Evaluate( const TVector2<Real> & vParameter ) const
{
    // Domain bounds
    if ( (vParameter.X < m_vMin.X) || (vParameter.X > m_vMax.X) ||
         (vParameter.Y < m_vMin.Y) || (vParameter.Y > m_vMax.Y) ) {
        Assert(false);
        return MathRealFn->Infinity;
    }

    // Indices along X
    Real fX = (vParameter.X - m_vMin.X) * m_vInvSpacing.X;
    Int iX = MathRealFn->Floor(fX);
    if ( iX < 0 )
        iX = 0;
    else if ( iX >= m_iBoundX )
        iX = m_iBoundX - 1;
    Int iXP1 = iX + 1;
    if ( iXP1 >= m_iBoundX )
        iXP1 = m_iBoundX - 1;
    Real fOffsetX = ( fX - (Real)iX );

    // Indices along Y
    Real fY = (vParameter.Y - m_vMin.Y) * m_vInvSpacing.Y;
    Int iY = MathRealFn->Floor(fY);
    if ( iY < 0 )
        iY = 0;
    else if ( iY >= m_iBoundY )
        iY = m_iBoundY - 1;
    Int iYP1 = iY + 1;
    if ( iYP1 >= m_iBoundY )
        iYP1 = m_iBoundY - 1;
    Real fOffsetY = ( fY - (Real)iY );

    // Blended vectors
    TVector2<Real> vU( (Real)1, fOffsetX );
    TVector2<Real> vV( (Real)1, fOffsetY );
    TVector2<Real> vP = ( sm_matBlend * vU );
    TVector2<Real> vQ = ( sm_matBlend * vV );

    // Row offsets
    Real * pRowY = ( m_arrValues + (iY * m_iBoundX) );
    Real * pRowYP1 = ( m_arrValues + (iYP1 * m_iBoundX) );

    // Interpolate
    Real fRes = (Real)0;
    fRes += ( vQ.X * vP.X * pRowY[iX] ); // TL
    fRes += ( vQ.X * vP.Y * pRowY[iXP1] ); // TR
    fRes += ( vQ.Y * vP.X * pRowYP1[iX] ); // BL
    fRes += ( vQ.Y * vP.Y * pRowYP1[iXP1] ); // BR
    return fRes;
}
template<typename Real>
Real TInterpolatorBilinear<Real>::Evaluate( const TVector2<Real> & vParameter, UInt iOrderX, UInt iOrderY ) const
{
    // Domain bounds
    if ( (vParameter.X < m_vMin.X) || (vParameter.X > m_vMax.X) ||
         (vParameter.Y < m_vMin.Y) || (vParameter.Y > m_vMax.Y) ) {
        Assert(false);
        return MathRealFn->Infinity;
    }

    // Linear function derivatives are null from 2nd order and upper
    if ( iOrderX >= 2 || iOrderY >= 2 )
        return MathRealFn->Zero;

    // Indices along X
    Real fX = (vParameter.X - m_vMin.X) * m_vInvSpacing.X;
    Int iX = MathRealFn->Floor(fX);
    if ( iX < 0 )
        iX = 0;
    else if ( iX >= m_iBoundX )
        iX = m_iBoundX - 1;
    Int iXP1 = iX + 1;
    if ( iXP1 >= m_iBoundX )
        iXP1 = m_iBoundX - 1;
    Real fOffsetX = ( fX - (Real)iX );

    // Indices along Y
    Real fY = (vParameter.Y - m_vMin.Y) * m_vInvSpacing.Y;
    Int iY = MathRealFn->Floor(fY);
    if ( iY < 0 )
        iY = 0;
    else if ( iY >= m_iBoundY )
        iY = m_iBoundY - 1;
    Int iYP1 = iY + 1;
    if ( iYP1 >= m_iBoundY )
        iYP1 = m_iBoundY - 1;
    Real fOffsetY = ( fY - (Real)iY );

    // Blended vectors
    TVector2<Real> vU, vV, vFactor;
    if ( iOrderX == 0 ) {
        vU.X = (Real)1;
        vU.Y = fOffsetX;
        vFactor.X = (Real)1;
    } else { // ( iOrderX == 1 )
        vU.X = (Real)0;
        vU.Y = (Real)1;
        vFactor.X = m_vInvSpacing.X;
    }
    if ( iOrderY == 0 ) {
        vV.X = (Real)1;
        vV.Y = fOffsetY;
        vFactor.Y = (Real)1;
    } else { // ( iOrderY == 1 )
        vV.X = (Real)0;
        vV.Y = (Real)1;
        vFactor.Y = m_vInvSpacing.Y;
    }
    TVector2<Real> vP = ( sm_matBlend * vU );
    TVector2<Real> vQ = ( sm_matBlend * vV );

    // Row offsets
    Real * pRowY = ( m_arrValues + (iY * m_iBoundX) );
    Real * pRowYP1 = ( m_arrValues + (iYP1 * m_iBoundX) );

    // Interpolate
    Real fRes = (Real)0;
    fRes += ( vQ.X * vP.X * pRowY[iX] ); // TL
    fRes += ( vQ.X * vP.Y * pRowY[iXP1] ); // TR
    fRes += ( vQ.Y * vP.X * pRowYP1[iX] ); // BL
    fRes += ( vQ.Y * vP.Y * pRowYP1[iXP1] ); // BR
    fRes *= ( vFactor.X * vFactor.Y );
    return fRes;
}

