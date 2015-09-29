/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Vector/VectorN.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Generic N-dimensional vector
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TVectorN implementation
template<typename Real>
const Real TVectorN<Real>::ZERO = (Real)0;

template<typename Real>
TVectorN<Real>::TVectorN()
{
    m_iSize = 0;
    m_arrVector = NULL;
}
template<typename Real>
TVectorN<Real>::TVectorN( UInt iSize )
{
    Assert( iSize > 0 );
    m_iSize = iSize;
    m_arrVector = New Real[m_iSize];
}
template<typename Real>
TVectorN<Real>::TVectorN( UInt iSize, const Real * vArr )
{
    Assert( iSize > 0 );
    m_iSize = iSize;
    m_arrVector = New Real[m_iSize];
    for( UInt i = 0; i < m_iSize; ++i )
        m_arrVector[i] = vArr[i];
}
template<typename Real>
TVectorN<Real>::TVectorN(const TVectorN<Real> & rhs)
{
    if ( rhs.m_arrVector == NULL ) {
        m_iSize = 0;
        m_arrVector = NULL;
    } else {
        m_iSize = rhs.iSize;
        m_arrVector = New Real[m_iSize];
        for( UInt i = 0; i < m_iSize; ++i )
            m_arrVector[i] = rhs.m_arrVector[i];
    }
}
template<typename Real>
TVectorN<Real>::~TVectorN()
{
    if ( m_arrVector != NULL )
        DeleteA( m_arrVector );
}

template<typename Real>
inline Void TVectorN<Real>::SetSize( UInt iSize ) {
    if ( m_arrVector != NULL ) {
        DeleteA( m_arrVector );
        m_arrVector = NULL;
    }
    m_iSize = iSize;
    m_arrVector = New Real[m_iSize];
}

template<typename Real>
inline TVectorN<Real> & TVectorN<Real>::operator=(const TVectorN<Real> & rhs) {
    if ( rhs.m_arrVector == NULL ) {
        m_iSize = 0;
        if ( m_arrVector != NULL )
            DeleteA( m_arrVector );
        m_arrVector = NULL;
    } else {
        if ( m_iSize != rhs.iSize ) {
            m_iSize = rhs.iSize;
            if ( m_arrVector != NULL )
                DeleteA( m_arrVector );
            m_arrVector = New Real[m_iSize];
        }
        for( UInt i = 0; i < m_iSize; ++i )
            m_arrVector[i] = rhs.m_arrVector[i];
    }
    return (*this);
}

template<typename Real>
inline TVectorN<Real>::operator Real*() const {
    Assert(m_arrVector != NULL);
    return m_arrVector;
}
template<typename Real>
inline TVectorN<Real>::operator const Real*() const {
    Assert(m_arrVector != NULL);
    return m_arrVector;
}

template<typename Real>
inline Real & TVectorN<Real>::operator[](Int i) {
    Assert(m_arrVector != NULL);
    Assert( i >= 0 && i < m_iSize );
    return m_arrVector[i];
}
template<typename Real>
inline Real TVectorN<Real>::operator[](Int i) const {
    Assert(m_arrVector != NULL);
    Assert( i >= 0 && i < m_iSize );
    return m_arrVector[i];
}
template<typename Real>
inline Real & TVectorN<Real>::operator[](UInt i) {
    Assert(m_arrVector != NULL);
    Assert( i < m_iSize );
    return m_arrVector[i];
}
template<typename Real>
inline Real TVectorN<Real>::operator[](UInt i) const {
    Assert(m_arrVector != NULL);
    Assert( i < m_iSize );
    return m_arrVector[i];
}

template<typename Real>
inline TVectorN<Real> TVectorN<Real>::operator+() const {
    Assert(m_arrVector != NULL);
    return TVectorN<Real>(m_iSize, m_arrVector);
}
template<typename Real>
inline TVectorN<Real> TVectorN<Real>::operator-() const {
    Assert(m_arrVector != NULL);
    TVectorN<Real> vRes(m_iSize);
    for( UInt i = 0; i < m_iSize; ++i )
        vRes.m_arrVector[i] = -(m_arrVector[i]);
    return vRes;
}

template<typename Real>
inline Bool TVectorN<Real>::operator==(const TVectorN<Real> & rhs) const {
    if ( m_arrVector == NULL ) {
        if ( rhs.m_arrVector == NULL )
            return true;
        return false;
    }
    if ( rhs.m_arrVector == NULL )
        return false;
    for( UInt i = 0; i < m_iSize; ++i ) {
        if ( !(MathRealFn->Equals(m_arrVector[i], rhs.m_arrVector[i])) )
            return false;
    }
    return true;
}
template<typename Real>
inline Bool TVectorN<Real>::operator!=(const TVectorN<Real> & rhs) const {
    if ( m_arrVector == NULL ) {
        if ( rhs.m_arrVector == NULL )
            return false;
        return true;
    }
    if ( rhs.m_arrVector == NULL )
        return true;
    for( UInt i = 0; i < m_iSize; ++i ) {
        if ( !(MathRealFn->Equals(m_arrVector[i], rhs.m_arrVector[i])) )
            return true;
    }
    return false;
}

template<typename Real>
inline TVectorN<Real> TVectorN<Real>::operator+(Real rhs) const {
    Assert( m_arrVector != NULL );
    TVectorN<Real> vRes(m_iSize);
    for( UInt i = 0; i < m_iSize; ++i )
        vRes.m_arrVector[i] = m_arrVector[i] + rhs;
    return vRes;
}
template<typename Real>
inline TVectorN<Real> TVectorN<Real>::operator-(Real rhs) const {
    Assert( m_arrVector != NULL );
    TVectorN<Real> vRes(m_iSize);
    for( UInt i = 0; i < m_iSize; ++i )
        vRes.m_arrVector[i] = m_arrVector[i] - rhs;
    return vRes;
}
template<typename Real>
inline TVectorN<Real> TVectorN<Real>::operator*(Real rhs) const {
    Assert( m_arrVector != NULL );
    TVectorN<Real> vRes(m_iSize);
    for( UInt i = 0; i < m_iSize; ++i )
        vRes.m_arrVector[i] = m_arrVector[i] * rhs;
    return vRes;
}
template<typename Real>
inline TVectorN<Real> TVectorN<Real>::operator/(Real rhs) const {
    Assert( m_arrVector != NULL );
    TVectorN<Real> vRes(m_iSize);
    for( UInt i = 0; i < m_iSize; ++i )
        vRes.m_arrVector[i] = m_arrVector[i] / rhs;
    return vRes;
}

template<typename Real>
inline TVectorN<Real> & TVectorN<Real>::operator+=(Real rhs) {
    Assert( m_arrVector != NULL );
    for( UInt i = 0; i < m_iSize; ++i )
        m_arrVector[i] += rhs;
}
template<typename Real>
inline TVectorN<Real> & TVectorN<Real>::operator-=(Real rhs) {
    Assert( m_arrVector != NULL );
    for( UInt i = 0; i < m_iSize; ++i )
        m_arrVector[i] -= rhs;
}
template<typename Real>
inline TVectorN<Real> & TVectorN<Real>::operator*=(Real rhs) {
    Assert( m_arrVector != NULL );
    for( UInt i = 0; i < m_iSize; ++i )
        m_arrVector[i] *= rhs;
}
template<typename Real>
inline TVectorN<Real> & TVectorN<Real>::operator/=(Real rhs) {
    Assert( m_arrVector != NULL );
    for( UInt i = 0; i < m_iSize; ++i )
        m_arrVector[i] /= rhs;
}

template<typename Real>
inline TVectorN<Real> TVectorN<Real>::operator+(const TVectorN<Real> & rhs) const {
    Assert( (m_arrVector != NULL) && (rhs.m_arrVector != NULL) );
    TVectorN<Real> vRes(m_iSize);
    for( UInt i = 0; i < m_iSize; ++i )
        vRes.m_arrVector[i] = m_arrVector[i] + rhs.m_arrVector[i];
    return vRes;
}
template<typename Real>
inline TVectorN<Real> TVectorN<Real>::operator-(const TVectorN<Real> & rhs) const {
    Assert( (m_arrVector != NULL) && (rhs.m_arrVector != NULL) );
    TVectorN<Real> vRes(m_iSize);
    for( UInt i = 0; i < m_iSize; ++i )
        vRes.m_arrVector[i] = m_arrVector[i] - rhs.m_arrVector[i];
    return vRes;
}

template<typename Real>
inline TVectorN<Real> & TVectorN<Real>::operator+=(const TVectorN<Real> & rhs) {
    Assert( (m_arrVector != NULL) && (rhs.m_arrVector != NULL) );
    for( UInt i = 0; i < m_iSize; ++i )
        m_arrVector[i] += rhs.m_arrVector[i];
}
template<typename Real>
inline TVectorN<Real> & TVectorN<Real>::operator-=(const TVectorN<Real> & rhs) {
    Assert( (m_arrVector != NULL) && (rhs.m_arrVector != NULL) );
    for( UInt i = 0; i < m_iSize; ++i )
        m_arrVector[i] -= rhs.m_arrVector[i];
}

template<typename Real>
inline Real TVectorN<Real>::operator*(const TVectorN<Real> & rhs) const {
    Assert( (m_arrVector != NULL) && (rhs.m_arrVector != NULL) );
    Real fDot = ZERO;
    for( UInt i = 0; i < m_iSize; ++i )
        fDot += (m_arrVector[i] * rhs.m_arrVector[i]);
    return fDot;
}

template<typename Real>
inline UInt TVectorN<Real>::GetSize() const {
    return m_iSize;
}

template<typename Real>
inline Real TVectorN<Real>::NormSqr() const {
    Assert( m_arrVector != NULL );
    Real fNormSqr = ZERO;
    for( UInt i = 0; i < m_iSize; ++i )
        fNormSqr += (m_arrVector[i] * m_arrVector[i]);
    return fNormSqr;
}

template<typename Real>
inline Real TVectorN<Real>::Norm() const {
    return MathRealFn->Sqrt( NormSqr() );
}
template<typename Real>
inline Real TVectorN<Real>::InvNormSqr() const {
    return MathRealFn->Invert( NormSqr() );
}
template<typename Real>
inline Real TVectorN<Real>::InvNorm() const {
    return MathRealFn->InvSqrt( NormSqr() );
}

template<typename Real>
inline Real TVectorN<Real>::Normalize() {
    Assert( m_arrVector != NULL );
    Real fNorm = Norm(), fInvNorm;
    if ( MathRealFn->EqualsZero(fNorm) ) {
        for( UInt i = 0; i < m_iSize; ++i )
            m_arrVector[i] = ZERO;
        return ZERO;
    } else {
        fInvNorm = MathRealFn->Invert(fNorm);
        for( UInt i = 0; i < m_iSize; ++i )
            m_arrVector[i] *= fInvNorm;
        return fNorm;
    }
}

