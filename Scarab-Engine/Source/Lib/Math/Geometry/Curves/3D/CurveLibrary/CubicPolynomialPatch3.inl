/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Curves/3D/CurveLibrary/CubicPolynomialPatch3.inl
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
// TCubicPolynomialPatch3 implementation
template<typename Real>
TCubicPolynomialPatch3<Real>::TCubicPolynomialPatch3( const TPolynomial1<Real> & vPolyX,
                                                      const TPolynomial1<Real> & vPolyY,
                                                      const TPolynomial1<Real> & vPolyZ ):
    TPolynomialPatch3<Real>( vPolyX, vPolyY, vPolyZ )
{
    Assert( vPolyX.GetDegree() == 3 );
    Assert( vPolyY.GetDegree() == 3 );
    Assert( vPolyZ.GetDegree() == 3 );

    m_iVertexCount = 0;
    m_arrVertices = NULL;
}
template<typename Real>
TCubicPolynomialPatch3<Real>::~TCubicPolynomialPatch3()
{
    if ( m_arrVertices != NULL )
        DeleteA( m_arrVertices );
}

template<typename Real>
Void TCubicPolynomialPatch3<Real>::Tesselate( UInt iLevel )
{
    if ( m_arrVertices != NULL )
        DeleteA( m_arrVertices );

    m_iVertexCount = (1 << iLevel) + 1;
    m_arrVertices = New TVertex3<Real>[m_iVertexCount];

    _IntervalParameters intervalParams;
    intervalParams.iBound0 = 0;
    intervalParams.iBound1 = m_iVertexCount - 1;

    // Initialize
    m_arrVertices[intervalParams.iBound0] = P( m_fMinT );
    m_arrVertices[intervalParams.iBound1] = P( m_fMaxT );

    // Recurse
    if ( iLevel > 0 ) {
        intervalParams.arrPdT2[0] = PdT2( m_fMinT );
        intervalParams.arrPdT2[1] = PdT2( m_fMaxT );
        _RecursiveSubdivision( intervalParams, iLevel - 1, (Real)0.25f );
    }
}

template<typename Real>
inline UInt TCubicPolynomialPatch3<Real>::GetVertexCount() const {
    return m_iVertexCount;
}
template<typename Real>
inline const TVertex3<Real> * TCubicPolynomialPatch3<Real>::GetVertices() const {
    return m_arrVertices;
}
template<typename Real>
inline const TVertex3<Real> & TCubicPolynomialPatch3<Real>::GetVertex( UInt iVertex ) const {
    Assert( iVertex < m_iVertexCount );
    return m_arrVertices[iVertex];
}

/////////////////////////////////////////////////////////////////////////////////

template<typename Real>
Void TCubicPolynomialPatch3<Real>::_RecursiveSubdivision( _IntervalParameters & intervalParams, UInt iLevel, Real fSqrD )
{
    // Get interval middle index
    UInt iMiddle = ( intervalParams.iBound0 + intervalParams.iBound1 ) >> 1;

    // Setup middle vertex
    TVector3<Real> vAveragePdT2 = ( intervalParams.arrPdT2[0] + intervalParams.arrPdT2[1] ) * (Real)0.5f;
    m_arrVertices[iMiddle] = m_arrVertices[intervalParams.iBound0];
    m_arrVertices[iMiddle] += m_arrVertices[intervalParams.iBound1].ToVector();
    m_arrVertices[iMiddle] -= ( vAveragePdT2 * fSqrD );
    m_arrVertices[iMiddle] *= (Real)0.5f;

    // Exit case
    if ( iLevel == 0 )
        return;

    // Recursive case
    _IntervalParameters subIntervalParams;

    subIntervalParams.iBound0 = intervalParams.iBound0;
    subIntervalParams.iBound1 = iMiddle;
    subIntervalParams.arrPdT2[0] = intervalParams.arrPdT2[0];
    subIntervalParams.arrPdT2[1] = vAveragePdT2;
    _RecursiveSubdivision( subIntervalParams, iLevel - 1, fSqrD * (Real)0.25f );

    subIntervalParams.iBound0 = iMiddle;
    subIntervalParams.iBound1 = intervalParams.iBound1;
    subIntervalParams.arrPdT2[0] = vAveragePdT2;
    subIntervalParams.arrPdT2[1] = intervalParams.arrPdT2[1];
    _RecursiveSubdivision( subIntervalParams, iLevel - 1, fSqrD * (Real)0.25f );
}

