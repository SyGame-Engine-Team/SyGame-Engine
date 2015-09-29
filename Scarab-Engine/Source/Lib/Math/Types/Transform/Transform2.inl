/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Transform/Transform2.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Generic transformations in 2D
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TTransform2 implementation
template<typename Real>
const TTransform2<Real> TTransform2<Real>::Identity;

template<typename Real>
TTransform2<Real>::TTransform2():
    m_matTransform( TMatrix2<Real>::Identity ),
    m_vScale( (Real)1, (Real)1 ),
    m_vTranslate( TVector2<Real>::Null ),
    m_matHMatrix( TMatrix3<Real>::Identity ),
    m_matInvTransform( TMatrix2<Real>::Identity ),
    m_vInvScale( (Real)1, (Real)1 ),
    m_vInvTranslate( TVector2<Real>::Null )
{
    m_bIsIdentity = true;
    m_bIsRotScale = true;
    m_bIsUniformScale = true;
    m_bUpdateInverse = false;
}
template<typename Real>
TTransform2<Real>::TTransform2( const TMatrix2<Real> & matRotate, const TVector2<Real> & vScale, const TVector2<Real> & vTranslate ):
    m_matTransform( matRotate ),
    m_vScale( vScale ),
    m_vTranslate( vTranslate ),
    m_matHMatrix( TMatrix3<Real>::Identity ),
    m_matInvTransform( TMatrix2<Real>::Identity ),
    m_vInvScale( (Real)1, (Real)1 ),
    m_vInvTranslate( TVector2<Real>::Null )
{
    m_bIsIdentity = false;
    m_bIsRotScale = true;
    m_bIsUniformScale = false;
    m_bUpdateInverse = false;
    _UpdateHMatrix();
}
template<typename Real>
TTransform2<Real>::TTransform2( const TMatrix2<Real> & matRotate, Real fUniformScale, const TVector2<Real> & vTranslate ):
    m_matTransform( matRotate ),
    m_vScale( fUniformScale, fUniformScale ),
    m_vTranslate( vTranslate ),
    m_matHMatrix( TMatrix3<Real>::Identity ),
    m_matInvTransform( TMatrix2<Real>::Identity ),
    m_vInvScale( (Real)1, (Real)1 ),
    m_vInvTranslate( TVector2<Real>::Null )
{
    m_bIsIdentity = false;
    m_bIsRotScale = true;
    m_bIsUniformScale = true;
    m_bUpdateInverse = false;
    _UpdateHMatrix();
}
template<typename Real>
TTransform2<Real>::TTransform2( const TMatrix2<Real> & matTransform, const TVector2<Real> & vTranslate ):
    m_matTransform( matTransform ),
    m_vScale( (Real)1, (Real)1 ),
    m_vTranslate( vTranslate ),
    m_matHMatrix( TMatrix3<Real>::Identity ),
    m_matInvTransform( TMatrix2<Real>::Identity ),
    m_vInvScale( (Real)1, (Real)1 ),
    m_vInvTranslate( TVector2<Real>::Null )
{
    m_bIsIdentity = false;
    m_bIsRotScale = false;
    m_bIsUniformScale = false;
    m_bUpdateInverse = false;
    _UpdateHMatrix();
}
template<typename Real>
TTransform2<Real>::TTransform2( const TTransform2<Real> & rhs ):
    m_matTransform( rhs.m_matTransform ),
    m_vScale( rhs.m_vScale ),
    m_vTranslate( rhs.m_vTranslate ),
    m_matHMatrix( rhs.m_matHMatrix ),
    m_matInvTransform( rhs.m_matInvTransform ),
    m_vInvScale( rhs.m_vInvScale ),
    m_vInvTranslate( rhs.m_vInvTranslate )
{
    m_bIsIdentity = rhs.m_bIsIdentity;
    m_bIsRotScale = rhs.m_bIsRotScale;
    m_bIsUniformScale = rhs.m_bIsUniformScale;
    m_bUpdateInverse = rhs.m_bUpdateInverse;
}
template<typename Real>
TTransform2<Real>::~TTransform2()
{
    // nothing to do
}

template<typename Real>
TTransform2<Real> & TTransform2<Real>::operator=( const TTransform2<Real> & rhs )
{
    m_bIsIdentity = rhs.m_bIsIdentity;
    m_bIsRotScale = rhs.m_bIsRotScale;
    m_bIsUniformScale = rhs.m_bIsUniformScale;
    m_bUpdateInverse = rhs.m_bUpdateInverse;

    m_matTransform = rhs.m_matTransform;
    m_vScale = rhs.m_vScale;
    m_vTranslate = rhs.m_vTranslate;
    m_matHMatrix = rhs.m_matHMatrix;

    m_matInvTransform = rhs.m_matInvTransform;
    m_vInvScale = rhs.m_vInvScale;
    m_vInvTranslate = rhs.m_vInvTranslate;

    return (*this);
}

template<typename Real>
inline TVertex3<Real> TTransform2<Real>::operator*( const TVertex3<Real> & rhs ) const {
    return m_matHMatrix * rhs;
}
template<typename Real>
inline TVertex2<Real> TTransform2<Real>::operator*( const TVertex2<Real> & rhs ) const {
    return TVertex2<Real>( m_matHMatrix * TVertex3<Real>(rhs) );
}

template<typename Real>
inline TVector3<Real> TTransform2<Real>::operator*( const TVector3<Real> & rhs ) const {
    return m_matHMatrix * rhs;
}
template<typename Real>
inline TVector2<Real> TTransform2<Real>::operator*( const TVector2<Real> & rhs ) const {
    return TVector2<Real>( m_matHMatrix * TVector3<Real>(rhs) );
}

template<typename Real>
TTransform2<Real> TTransform2<Real>::operator*( const TTransform2<Real> & rhs ) const
{
    if ( m_bIsIdentity )
        return rhs;
    if ( rhs.m_bIsIdentity )
        return (*this);

    TTransform2<Real> trComposed;
    if ( m_bIsRotScale && rhs.m_bIsRotScale ) {
        if ( m_bIsUniformScale ) {
            trComposed.SetRotate( m_matTransform * rhs.m_matTransform );
            if ( rhs.m_bIsUniformScale )
                trComposed.SetUniformScale( rhs.GetUniformScale() * GetUniformScale() );
            else
                trComposed.SetScale( rhs.GetScale() * GetUniformScale() );
            trComposed.SetTranslate (
                ( (m_matTransform * rhs.m_vTranslate) * GetUniformScale() ) + m_vTranslate
            );
            return trComposed;
        }
    }

    TMatrix2<Real> matMA = m_matTransform;
    if ( m_bIsRotScale ) {
        matMA.m00 *= m_vScale.X; matMA.m01 *= m_vScale.Y;
        matMA.m10 *= m_vScale.X; matMA.m11 *= m_vScale.Y;
    }
    TMatrix2<Real> matMB = rhs.m_matTransform;
    if ( rhs.m_bIsRotScale ) {
        matMB.m00 *= rhs.m_vScale.X; matMB.m01 *= rhs.m_vScale.Y;
        matMB.m10 *= rhs.m_vScale.X; matMB.m11 *= rhs.m_vScale.Y;
    }
    trComposed.SetTransform( matMA * matMB );
    trComposed.SetTranslate( (matMA * rhs.m_vTranslate) + m_vTranslate );
    return trComposed;
}

template<typename Real>
Void TTransform2<Real>::SetTransform( const TMatrix2<Real> & matTransform )
{
    m_matTransform = matTransform;

    m_bIsIdentity = false;
    m_bIsRotScale = false;
    m_bIsUniformScale = false;
    _UpdateHMatrix();
}
template<typename Real>
Void TTransform2<Real>::SetRotate( const TMatrix2<Real> & matRotate )
{
    m_matTransform = matRotate;

    m_bIsIdentity = false;
    m_bIsRotScale = true;
    _UpdateHMatrix();
}
template<typename Real>
Void TTransform2<Real>::SetScale( const TVector2<Real> & vScale )
{
    Assert(m_bIsRotScale);
    Assert(vScale != TVector2<Real>::Null);

    m_vScale = vScale;

    m_bIsIdentity = false;
    m_bIsUniformScale = false;
    _UpdateHMatrix();
}
template<typename Real>
Void TTransform2<Real>::SetUniformScale( Real fScale )
{
    Assert(m_bIsRotScale);
    Assert(fScale != (Real)0);

    m_vScale = TVector2<Real>( fScale, fScale );

    m_bIsIdentity = false;
    m_bIsUniformScale = true;
    _UpdateHMatrix();
}
template<typename Real>
Void TTransform2<Real>::SetTranslate( const TVector2<Real> & vTranslate )
{
    m_vTranslate = vTranslate;

    m_bIsIdentity = false;
    _UpdateHMatrix();
}

template<typename Real>
inline Bool TTransform2<Real>::IsIdentity() const {
    return m_bIsIdentity;
}
template<typename Real>
inline Bool TTransform2<Real>::IsRotScale() const {
    return m_bIsRotScale;
}
template<typename Real>
inline Bool TTransform2<Real>::IsUniformScale() const {
    return (m_bIsRotScale && m_bIsUniformScale);
}

template<typename Real>
inline const TMatrix2<Real> & TTransform2<Real>::GetTransform() const {
    return m_matTransform;
}
template<typename Real>
inline const TMatrix2<Real> & TTransform2<Real>::GetRotate() const {
    Assert(m_bIsRotScale);
    return m_matTransform;
}
template<typename Real>
inline const TVector2<Real> & TTransform2<Real>::GetScale() const {
    Assert(m_bIsRotScale);
    return m_vScale;
}
template<typename Real>
inline Real TTransform2<Real>::GetUniformScale() const {
    Assert(m_bIsRotScale && m_bIsUniformScale);
    return m_vScale.X;
}
template<typename Real>
inline const TVector2<Real> & TTransform2<Real>::GetTranslate() const {
    return m_vTranslate;
}

template<typename Real>
inline const TMatrix3<Real> & TTransform2<Real>::GetHMatrix() const {
    return m_matHMatrix;
}

template<typename Real>
Void TTransform2<Real>::MakeIdentity()
{
    m_matTransform = TMatrix2<Real>::Identity;
    m_vScale = TVector2<Real>( (Real)1, (Real)1 );
    m_vTranslate = TVector2<Real>::Null;

    m_bIsIdentity = true;
    m_bIsRotScale = true;
    m_bIsUniformScale = true;
    _UpdateHMatrix();
}
template<typename Real>
Void TTransform2<Real>::MakeUnitScale()
{
    Assert(m_bIsRotScale);

    m_vScale = TVector2<Real>( (Real)1, (Real)1 );

    m_bIsUniformScale = true;
    _UpdateHMatrix();
}

template<typename Real>
Real TTransform2<Real>::GetMaxScale() const
{
    if (m_bIsRotScale) {
        Real fMaxScale = MathRealFn->Abs(m_vScale.X);
        Real fScale = MathRealFn->Abs(m_vScale.Y);
        if ( fScale > fMaxScale )
            fMaxScale = fScale;
        return fMaxScale;
    }

    // Max Row-Sum is a good approximation of the maximal scale.
    // The spectral norm (maximum absolute value of eigenvalues)
    // is smaller or equal to this norm.
    Real fMaxRowSum = ( MathRealFn->Abs(m_matTransform.m00) + MathRealFn->Abs(m_matTransform.m01) );
    Real fRowSum = ( MathRealFn->Abs(m_matTransform.m10) + MathRealFn->Abs(m_matTransform.m11) );
    if ( fRowSum > fMaxRowSum )
        fMaxRowSum = fRowSum;
    return fMaxRowSum;
}

template<typename Real>
Bool TTransform2<Real>::Invert( TTransform2<Real> & outInvTransform ) const
{
    if ( m_bUpdateInverse ) {
        Bool bInvertible = _UpdateInverse();
        if ( !bInvertible )
            return false;
    }

    if ( m_bIsIdentity ) {
        outInvTransform.MakeIdentity();
        return true;
    }

    if ( m_bIsRotScale ) {
        outInvTransform.SetRotate( m_matInvTransform );
        if ( m_bIsUniformScale )
            outInvTransform.SetUniformScale( m_vInvScale.X );
        else
            outInvTransform.SetScale( m_vInvScale );
    } else
        outInvTransform.SetTransform( m_matInvTransform );
    outInvTransform.SetTranslate( m_vInvTranslate );

    return true;
}

/////////////////////////////////////////////////////////////////////////////////

template<typename Real>
Void TTransform2<Real>::_UpdateHMatrix()
{
    if ( m_bIsIdentity )
        m_matHMatrix = TMatrix3<Real>::Identity;
    else {
        m_matHMatrix = m_matTransform;
        if ( m_bIsRotScale ) {
            m_matHMatrix.m00 *= m_vScale.X;
            m_matHMatrix.m01 *= m_vScale.Y;
            m_matHMatrix.m10 *= m_vScale.X;
            m_matHMatrix.m11 *= m_vScale.Y;
        }
        m_matHMatrix.m02 = m_vTranslate.X;
        m_matHMatrix.m12 = m_vTranslate.Y;
        // last row is allways (0, 0, 1)
    }
    m_bUpdateInverse = true;
}
template<typename Real>
Bool TTransform2<Real>::_UpdateInverse() const
{
    if ( !m_bUpdateInverse )
        return true;

    if ( m_bIsIdentity ) {
        m_matInvTransform = TMatrix2<Real>::Identity;
        m_vInvScale = TVector2<Real>( (Real)1, (Real)1 );
        m_vInvTranslate = TVector2<Real>::Null;

        m_bUpdateInverse = false;
        return true;
    }

    if ( m_bIsRotScale ) {
        m_matTransform.Transpose( m_matInvTransform );
        m_vInvTranslate = -( m_matInvTransform * m_vTranslate );
        if ( m_bIsUniformScale )  {
            Real fInvScale = MathRealFn->Invert(m_vScale.X);
            m_vInvScale.X = fInvScale;
            m_vInvScale.Y = fInvScale;

            m_vInvTranslate *= fInvScale;
        } else {
            m_vInvScale.X = MathRealFn->Invert(m_vScale.X);
            m_vInvScale.Y = MathRealFn->Invert(m_vScale.Y);

            m_vInvTranslate.X *= m_vInvScale.X;
            m_vInvTranslate.Y *= m_vInvScale.Y;
        }
    } else {
        Bool bInvertible = m_matTransform.Invert( m_matInvTransform );
        if ( !bInvertible )
            return false;
        m_vInvTranslate = -( m_matInvTransform * m_vTranslate );
    }

    m_bUpdateInverse = false;
    return true;
}

