/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Analysis/Polynomial/Polynomial1.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Polynomial arithmetic : Polynomial 1D
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TPolynomial1 implementation
template <typename Real>
TPolynomial1<Real>::TPolynomial1()
{
    m_iDegree = INVALID_OFFSET;
    m_arrCoefficients = NULL;
}
template <typename Real>
TPolynomial1<Real>::TPolynomial1( UInt iDegree )
{
    Assert( iDegree != INVALID_OFFSET );
    m_iDegree = iDegree;
    m_arrCoefficients = New Real[m_iDegree + 1];
}
template <typename Real>
TPolynomial1<Real>::TPolynomial1( const TPolynomial1<Real> & rhs )
{
    m_iDegree = rhs.m_iDegree;
    m_arrCoefficients = NULL;
    if ( m_iDegree != INVALID_OFFSET ) {
        m_arrCoefficients = New Real[m_iDegree + 1];
        for( UInt i = 0; i <= m_iDegree; ++i )
            m_arrCoefficients[i] = rhs.m_arrCoefficients[i];
    }
}
template <typename Real>
TPolynomial1<Real>::~TPolynomial1()
{
    if ( m_arrCoefficients != NULL )
        DeleteA( m_arrCoefficients );
}

template <typename Real>
TPolynomial1<Real> & TPolynomial1<Real>::operator=( const TPolynomial1<Real> & rhs )
{
    m_iDegree = rhs.m_iDegree;
    if ( m_arrCoefficients != NULL ) {
        DeleteA( m_arrCoefficients );
        m_arrCoefficients = NULL;
    }
    if ( m_iDegree != INVALID_OFFSET ) {
        m_arrCoefficients = New Real[m_iDegree + 1];
        for( UInt i = 0; i <= m_iDegree; ++i )
            m_arrCoefficients[i] = rhs.m_arrCoefficients[i];
    }
    return (*this);
}

template <typename Real>
inline TPolynomial1<Real>::operator const Real*() const {
    return m_arrCoefficients;
}
template <typename Real>
inline TPolynomial1<Real>::operator Real*() {
    return m_arrCoefficients;
}

template <typename Real>
inline Real TPolynomial1<Real>::operator[]( Int i ) const {
    Assert( (i >= 0) && (i <= m_iDegree) );
    return m_arrCoefficients[i];
}
template <typename Real>
inline Real & TPolynomial1<Real>::operator[]( Int i ) {
    Assert( (i >= 0) && (i <= m_iDegree) );
    return m_arrCoefficients[i];
}
template <typename Real>
inline Real TPolynomial1<Real>::operator[]( UInt i ) const {
    Assert( i <= m_iDegree );
    return m_arrCoefficients[i];
}
template <typename Real>
inline Real & TPolynomial1<Real>::operator[]( UInt i ) {
    Assert( i <= m_iDegree );
    return m_arrCoefficients[i];
}

template <typename Real>
TPolynomial1<Real> TPolynomial1<Real>::operator-() const
{
    Assert( (m_iDegree != INVALID_OFFSET) && (m_arrCoefficients != NULL) );
    TPolynomial1<Real> polyNeg( m_iDegree );
    for( UInt i = 0; i <= m_iDegree; ++i )
        polyNeg.m_arrCoefficients[i] = -m_arrCoefficients[i];
    return polyNeg;
}

template <typename Real>
Bool TPolynomial1<Real>::operator==( const TPolynomial1<Real> & rhs ) const
{
    if ( m_iDegree != rhs.m_iDegree )
        return false;
    if ( m_iDegree == INVALID_OFFSET ) {
        Assert( (m_arrCoefficients == rhs.m_arrCoefficients) && (m_arrCoefficients == NULL) );
        return true;
    }
    for( UInt i = 0; i <= m_iDegree; ++i ) {
        if ( !(MathRealFn->Equals(m_arrCoefficients[i], rhs.m_arrCoefficients[i], (Real)SCALAR_ERROR)) )
            return false;
    }
    return true;
}
template <typename Real>
Bool TPolynomial1<Real>::operator!=( const TPolynomial1<Real> & rhs ) const
{
    if ( m_iDegree != rhs.m_iDegree )
        return true;
    if ( m_iDegree == INVALID_OFFSET ) {
        Assert( (m_arrCoefficients == rhs.m_arrCoefficients) && (m_arrCoefficients == NULL) );
        return false;
    }
    for( UInt i = 0; i <= m_iDegree; ++i ) {
        if ( !(MathRealFn->Equals(m_arrCoefficients[i], rhs.m_arrCoefficients[i], (Real)SCALAR_ERROR)) )
            return true;
    }
    return false;
}

template <typename Real>
TPolynomial1<Real> TPolynomial1<Real>::operator+( Real rhs ) const
{
    Assert( (m_iDegree != INVALID_OFFSET) && (m_arrCoefficients != NULL) );

    TPolynomial1<Real> polySum( *this );
    polySum.m_arrCoefficients[0] += rhs;
    return polySum;
}
template <typename Real>
TPolynomial1<Real> TPolynomial1<Real>::operator-( Real rhs ) const
{
    Assert( (m_iDegree != INVALID_OFFSET) && (m_arrCoefficients != NULL) );

    TPolynomial1<Real> polySub( *this );
    polySub.m_arrCoefficients[0] -= rhs;
    return polySub;
}
template <typename Real>
TPolynomial1<Real> TPolynomial1<Real>::operator*( Real rhs ) const
{
    Assert( (m_iDegree != INVALID_OFFSET) && (m_arrCoefficients != NULL) );

    TPolynomial1<Real> polyProd( m_iDegree );
    for( UInt i = 0; i <= m_iDegree; ++i )
        polyProd.m_arrCoefficients[i] = m_arrCoefficients[i] * rhs;
    return polyProd;
}
template <typename Real>
TPolynomial1<Real> TPolynomial1<Real>::operator/( Real rhs ) const
{
    Assert( (m_iDegree != INVALID_OFFSET) && (m_arrCoefficients != NULL) );
    Real fInvRhs = MathRealFn->Invert( rhs );

    TPolynomial1<Real> polyQuot( m_iDegree );
    for( UInt i = 0; i <= m_iDegree; ++i )
        polyQuot.m_arrCoefficients[i] = m_arrCoefficients[i] * fInvRhs;
    return polyQuot;
}

template <typename Real>
inline TPolynomial1<Real> & TPolynomial1<Real>::operator+=( Real rhs ) {
    Assert( (m_iDegree != INVALID_OFFSET) && (m_arrCoefficients != NULL) );
    m_arrCoefficients[0] += rhs;
    return (*this);
}
template <typename Real>
inline TPolynomial1<Real> & TPolynomial1<Real>::operator-=( Real rhs ) {
    Assert( (m_iDegree != INVALID_OFFSET) && (m_arrCoefficients != NULL) );
    m_arrCoefficients[0] -= rhs;
    return (*this);
}
template <typename Real>
inline TPolynomial1<Real> & TPolynomial1<Real>::operator*=( Real rhs ) {
    Assert( (m_iDegree != INVALID_OFFSET) && (m_arrCoefficients != NULL) );
    for( UInt i = 0; i <= m_iDegree; ++i )
        m_arrCoefficients[i] *= rhs;
    return (*this);
}
template <typename Real>
inline TPolynomial1<Real> & TPolynomial1<Real>::operator/=( Real rhs ) {
    Assert( (m_iDegree != INVALID_OFFSET) && (m_arrCoefficients != NULL) );
    Real fInvRhs = MathRealFn->Invert( rhs );
    for( UInt i = 0; i <= m_iDegree; ++i )
        m_arrCoefficients[i] *= fInvRhs;
    return (*this);
}

template <typename Real>
TPolynomial1<Real> TPolynomial1<Real>::operator+( const TPolynomial1<Real> & rhs ) const
{
    Assert( (m_iDegree != INVALID_OFFSET) && (rhs.m_iDegree != INVALID_OFFSET) );
    Assert( (m_arrCoefficients != NULL) && (rhs.m_arrCoefficients != NULL) );

    TPolynomial1<Real> polySum;
    UInt i;

    if ( m_iDegree > rhs.m_iDegree ) {
        polySum.SetDegree( m_iDegree );
        for( i = 0; i <= rhs.m_iDegree; ++i )
            polySum.m_arrCoefficients[i] = ( m_arrCoefficients[i] + rhs.m_arrCoefficients[i] );
        for( i = rhs.m_iDegree + 1; i <= m_iDegree; ++i )
            polySum.m_arrCoefficients[i] = m_arrCoefficients[i];
    } else {
        polySum.SetDegree( rhs.m_iDegree );
        for( i = 0; i <= m_iDegree; ++i )
            polySum.m_arrCoefficients[i] = ( m_arrCoefficients[i] + rhs.m_arrCoefficients[i] );
        for( i = m_iDegree + 1; i <= rhs.m_iDegree; ++i )
            polySum.m_arrCoefficients[i] = rhs.m_arrCoefficients[i];
    }
    return polySum;
}
template <typename Real>
TPolynomial1<Real> TPolynomial1<Real>::operator-( const TPolynomial1<Real> & rhs ) const
{
    Assert( (m_iDegree != INVALID_OFFSET) && (rhs.m_iDegree != INVALID_OFFSET) );
    Assert( (m_arrCoefficients != NULL) && (rhs.m_arrCoefficients != NULL) );

    TPolynomial1<Real> polySub;
    UInt i;

    if ( m_iDegree > rhs.m_iDegree ) {
        polySub.SetDegree( m_iDegree );
        for( i = 0; i <= rhs.m_iDegree; ++i )
            polySub.m_arrCoefficients[i] = ( m_arrCoefficients[i] - rhs.m_arrCoefficients[i] );
        for( i = rhs.m_iDegree + 1; i <= m_iDegree; ++i )
            polySub.m_arrCoefficients[i] = m_arrCoefficients[i];
    } else {
        polySub.SetDegree( rhs.m_iDegree );
        for( i = 0; i <= m_iDegree; ++i )
            polySub.m_arrCoefficients[i] = ( m_arrCoefficients[i] - rhs.m_arrCoefficients[i] );
        for( i = m_iDegree + 1; i <= rhs.m_iDegree; ++i )
            polySub.m_arrCoefficients[i] = -rhs.m_arrCoefficients[i];
    }
    return polySub;
}
template <typename Real>
TPolynomial1<Real> TPolynomial1<Real>::operator*( const TPolynomial1<Real> & rhs ) const
{
    Assert( (m_iDegree != INVALID_OFFSET) && (rhs.m_iDegree != INVALID_OFFSET) );
    Assert( (m_arrCoefficients != NULL) && (rhs.m_arrCoefficients != NULL) );

    TPolynomial1<Real> polyProd( m_iDegree + rhs.m_iDegree );
    UInt i, j;

    for( i = 0; i <= polyProd.m_iDegree; ++i )
        polyProd.m_arrCoefficients[i] = (Real)0;
    for( i = 0; i <= m_iDegree; ++i ) {
        for( j = 0; j <= rhs.m_iDegree; ++j )
            polyProd.m_arrCoefficients[i+j] += m_arrCoefficients[i] * rhs.m_arrCoefficients[j];
    }
    return polyProd;
}

template <typename Real>
inline TPolynomial1<Real> & TPolynomial1<Real>::operator+=( const TPolynomial1<Real> & rhs ) {
    Assert( (m_iDegree != INVALID_OFFSET) && (rhs.m_iDegree != INVALID_OFFSET) );
    Assert( (m_arrCoefficients != NULL) && (rhs.m_arrCoefficients != NULL) );
    *this = (*this) + rhs;
    return (*this);
}
template <typename Real>
inline TPolynomial1<Real> & TPolynomial1<Real>::operator-=( const TPolynomial1<Real> & rhs ) {
    Assert( (m_iDegree != INVALID_OFFSET) && (rhs.m_iDegree != INVALID_OFFSET) );
    Assert( (m_arrCoefficients != NULL) && (rhs.m_arrCoefficients != NULL) );
    *this = (*this) - rhs;
    return (*this);
}
template <typename Real>
inline TPolynomial1<Real> & TPolynomial1<Real>::operator*=( const TPolynomial1<Real> & rhs ) {
    Assert( (m_iDegree != INVALID_OFFSET) && (rhs.m_iDegree != INVALID_OFFSET) );
    Assert( (m_arrCoefficients != NULL) && (rhs.m_arrCoefficients != NULL) );
    *this = (*this) * rhs;
    return (*this);
}

template <typename Real>
inline UInt TPolynomial1<Real>::GetDegree() const {
    return m_iDegree;
}
template <typename Real>
Void TPolynomial1<Real>::SetDegree( UInt iDegree )
{
    if ( m_iDegree == iDegree )
        return;
    m_iDegree = iDegree;
    if ( m_arrCoefficients != NULL ) {
        DeleteA( m_arrCoefficients );
        m_arrCoefficients = NULL;
    }
    if ( m_iDegree != INVALID_OFFSET )
        m_arrCoefficients = New Real[m_iDegree + 1];
}

template <typename Real>
Real TPolynomial1<Real>::Evaluate( Real fT ) const
{
    Assert( (m_iDegree != INVALID_OFFSET) && (m_arrCoefficients != NULL) );
    Real fValue = m_arrCoefficients[m_iDegree];
    for( Int i = m_iDegree - 1; i >= 0; --i ) {
        fValue *= fT;
        fValue += m_arrCoefficients[i];
    }
    return fValue;
}

template <typename Real>
TPolynomial1<Real> TPolynomial1<Real>::GetDerivative() const
{
    if ( m_iDegree == INVALID_OFFSET ) {
        Assert( m_arrCoefficients == NULL );
        return TPolynomial1<Real>();
    }
    if ( m_iDegree == 0 ) {
        TPolynomial1<Real> polyDeriv(0);
        polyDeriv.m_arrCoefficients[0] = (Real)0;
        return polyDeriv;
    }
    TPolynomial1<Real> polyDeriv( m_iDegree - 1 );
    for( UInt i = 0, j = 1; i < m_iDegree; ++i, ++j )
        polyDeriv.m_arrCoefficients[i] = ( m_arrCoefficients[j] * (Real)j );
    return polyDeriv;
}
template <typename Real>
TPolynomial1<Real> TPolynomial1<Real>::GetInvert() const
{
    TPolynomial1<Real> polyInvert( m_iDegree );
    for( UInt i = 0; i <= m_iDegree; ++i )
        polyDeriv.m_arrCoefficients[i] = m_arrCoefficients[m_iDegree - i];
    return polyInvert;
}

template <typename Real>
Void TPolynomial1<Real>::Divide( TPolynomial1<Real> * outQuotient, TPolynomial1<Real> * outRemainder,
                                const TPolynomial1<Real> & rDivisor, Real fTolerance ) const
{
    Assert( (m_iDegree != INVALID_OFFSET) && (rDivisor.m_iDegree != INVALID_OFFSET) );
    Assert( (m_arrCoefficients != NULL) && (rDivisor.m_arrCoefficients != NULL) );

    if ( m_iDegree < rDivisor.m_iDegree ) {
        outQuotient->SetDegree(0);
        outQuotient->m_arrCoefficients[0] = (Real)0;
        *outRemainder = (*this);
        return;
    }

    // Euclidian division
    UInt iDegreeQ = ( m_iDegree - rDivisor.m_iDegree );
    outQuotient->SetDegree( iDegreeQ );

    TPolynomial1<Real> tmpPoly = (*this);
    Real fInv = MathRealFn->Invert( rDivisor.m_arrCoefficients[rDivisor.m_iDegree] );
    Int i, j;

    for( i = iDegreeQ; i >= 0; --i ) {
        j = rDivisor.m_iDegree + i;
        outQuotient->m_arrCoefficients[i] = (tmpPoly.m_arrCoefficients[j] * fInv);
        --j;
        while( j >= i ) {
            tmpPoly.m_arrCoefficients[j] -= ( rDivisor.m_arrCoefficients[j-i] * outQuotient->m_arrCoefficients[i] );
            --j;
        }
    }

    // Compute remainder
    UInt iDegreeR = rDivisor.m_iDegree - 1;
    while( (iDegreeR > 0) && MathRealFn->EqualsZero(tmpPoly.m_arrCoefficients[iDegreeR], fTolerance) )
        --iDegreeR;
    if ( (iDegreeR == 0) && MathRealFn->EqualsZero(tmpPoly.m_arrCoefficients[0], fTolerance) )
        tmpPoly.m_arrCoefficients[0] = (Real)0;

    outRemainder->SetDegree( iDegreeR );
    for( i = 0; i <= iDegreeR; ++i )
        outRemainder->m_arrCoefficients[i] = tmpPoly.m_arrCoefficients[i];
}

template <typename Real>
Void TPolynomial1<Real>::Compression( Real fTolerance )
{
    Assert( m_iDegree != INVALID_OFFSET );

    if ( m_iDegree == 0 ) {
        if ( MathRealFn->EqualsZero(m_arrCoefficients[0], fTolerance) )
            m_arrCoefficients[0] = (Real)0;
        return;
    }

    Int i;
    for( i = m_iDegree; i >= 0; --i ) {
        if ( MathRealFn->EqualsZero(m_arrCoefficients[i], fTolerance) )
            --m_iDegree;
        else
            break;
    }

    Real fInvBiggest = MathRealFn->Invert( m_arrCoefficients[m_iDegree] );
    m_arrCoefficients[m_iDegree] = (Real)1;
    for( i = 0; i < m_iDegree; ++i )
        m_arrCoefficients[i] *= fInvBiggest;
}

