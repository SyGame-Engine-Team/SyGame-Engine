/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Algebra/LinearSystem/LinearSystem2.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : 2D linear systems solver
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TLinearSystem2 implementation
template<typename Real>
TLinearSystem2<Real>::TLinearSystem2( Real fZeroTolerance ):
    m_matA(), m_vB(), m_matInvA(), m_vX()
{
    ZeroTolerance = fZeroTolerance;
}
template<typename Real>
TLinearSystem2<Real>::TLinearSystem2( const TMatrix2<Real> & matA, const TVector2<Real> & vB,
                                      Real fZeroTolerance ):
    m_matA(matA), m_vB(vB), m_matInvA(), m_vX()
{
    ZeroTolerance = fZeroTolerance;
}
template<typename Real>
TLinearSystem2<Real>::~TLinearSystem2()
{
    // nothing to do
}

template<typename Real>
inline TMatrix2<Real> & TLinearSystem2<Real>::GetA() {
    return m_matA;
}
template<typename Real>
inline TVector2<Real> & TLinearSystem2<Real>::GetB() {
    return m_vB;
}

template<typename Real>
Bool TLinearSystem2<Real>::SolveSystem()
{
    Bool bInvertible = m_matA.Invert( m_matInvA, ZeroTolerance );
    if ( !bInvertible )
        return false;
    m_vX = ( m_matInvA * m_vB );
    return true;
}

template<typename Real>
inline const TMatrix2<Real> & TLinearSystem2<Real>::GetInvA() const {
    return m_matInvA;
}
template<typename Real>
inline const TVector2<Real> & TLinearSystem2<Real>::GetX() const {
    return m_vX;
}

