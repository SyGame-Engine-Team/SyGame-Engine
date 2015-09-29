/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Curves/3D/CurveLibrary/PolynomialPatch3.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Curves 3D : Polynomial curves
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TPolynomialPatch3 implementation
template<typename Real>
TPolynomialPatch3<Real>::TPolynomialPatch3( const TPolynomial1<Real> & vPolyX,
                                            const TPolynomial1<Real> & vPolyY,
                                            const TPolynomial1<Real> & vPolyZ ):
    TCurvePatch3<Real>( (Real)0, (Real)1 ),
    m_vPX( vPolyX ), m_vPY( vPolyY ), m_vPZ( vPolyZ )
{
    Assert( m_vPX.GetDegree() == m_vPY.GetDegree() );
    Assert( m_vPX.GetDegree() == m_vPZ.GetDegree() );

    m_vPXdT = m_vPX.GetDerivative();
    m_vPYdT = m_vPY.GetDerivative();
    m_vPZdT = m_vPZ.GetDerivative();

    m_vPXdT2 = m_vPXdT.GetDerivative();
    m_vPYdT2 = m_vPYdT.GetDerivative();
    m_vPZdT2 = m_vPZdT.GetDerivative();

    m_vPXdT3 = m_vPXdT2.GetDerivative();
    m_vPYdT3 = m_vPYdT2.GetDerivative();
    m_vPZdT3 = m_vPZdT2.GetDerivative();
}
template<typename Real>
TPolynomialPatch3<Real>::~TPolynomialPatch3()
{
    // nothing to do
}

template<typename Real>
inline UInt TPolynomialPatch3<Real>::GetDegree() const {
    return m_vPX.GetDegree();
}

template<typename Real>
inline const TPolynomial1<Real> & TPolynomialPatch3<Real>::GetPolynomialX() const {
    return m_vPX;
}
template<typename Real>
inline const TPolynomial1<Real> & TPolynomialPatch3<Real>::GetPolynomialY() const {
    return m_vPY;
}
template<typename Real>
inline const TPolynomial1<Real> & TPolynomialPatch3<Real>::GetPolynomialZ() const {
    return m_vPZ;
}

template<typename Real>
inline TVertex3<Real> TPolynomialPatch3<Real>::P( Real fT ) const {
    return TVertex3<Real>( m_vPX.Evaluate(fT), m_vPY.Evaluate(fT), m_vPZ.Evaluate(fT) );
}
template<typename Real>
inline TVector3<Real> TPolynomialPatch3<Real>::PdT( Real fT ) const {
    return TVector3<Real>( m_vPXdT.Evaluate(fT), m_vPYdT.Evaluate(fT), m_vPZdT.Evaluate(fT) );
}
template<typename Real>
inline TVector3<Real> TPolynomialPatch3<Real>::PdT2( Real fT ) const {
    return TVector3<Real>( m_vPXdT2.Evaluate(fT), m_vPYdT2.Evaluate(fT), m_vPZdT2.Evaluate(fT) );
}
template<typename Real>
inline TVector3<Real> TPolynomialPatch3<Real>::PdT3( Real fT ) const {
    return TVector3<Real>( m_vPXdT3.Evaluate(fT), m_vPYdT3.Evaluate(fT), m_vPZdT3.Evaluate(fT) );
}

