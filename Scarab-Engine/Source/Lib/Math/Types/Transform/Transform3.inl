/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Transform/Transform3.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Generic transformations in 3D
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TTransform3 implementation
template<typename Real>
const TTransform3<Real> TTransform3<Real>::Identity;

template<typename Real>
TTransform3<Real>::TTransform3():
    m_matTransform( TMatrix3<Real>::Identity ),
    m_vScale( (Real)1, (Real)1, (Real)1 ),
    m_vTranslate( TVector3<Real>::Null ),
    m_matHMatrix( TMatrix4<Real>::Identity ),
    m_matInvTransform( TMatrix3<Real>::Identity ),
    m_vInvScale( (Real)1, (Real)1, (Real)1 ),
    m_vInvTranslate( TVector3<Real>::Null )
{
    m_bIsIdentity = true;
    m_bIsRotScale = true;
    m_bIsUniformScale = true;
    m_bUpdateInverse = false;
}
template<typename Real>
TTransform3<Real>::TTransform3( const TMatrix3<Real> & matRotate, const TVector3<Real> & vScale, const TVector3<Real> & vTranslate ):
    m_matTransform( matRotate ),
    m_vScale( vScale ),
    m_vTranslate( vTranslate ),
    m_matHMatrix( TMatrix4<Real>::Identity ),
    m_matInvTransform( TMatrix3<Real>::Identity ),
    m_vInvScale( (Real)1, (Real)1, (Real)1 ),
    m_vInvTranslate( TVector3<Real>::Null )
{
    m_bIsIdentity = false;
    m_bIsRotScale = true;
    m_bIsUniformScale = false;
    m_bUpdateInverse = false;
    _UpdateHMatrix();
}
template<typename Real>
TTransform3<Real>::TTransform3( const TMatrix3<Real> & matRotate, Real fUniformScale, const TVector3<Real> & vTranslate ):
    m_matTransform( matRotate ),
    m_vScale( fUniformScale, fUniformScale, fUniformScale ),
    m_vTranslate( vTranslate ),
    m_matHMatrix( TMatrix4<Real>::Identity ),
    m_matInvTransform( TMatrix3<Real>::Identity ),
    m_vInvScale( (Real)1, (Real)1, (Real)1 ),
    m_vInvTranslate( TVector3<Real>::Null )
{
    m_bIsIdentity = false;
    m_bIsRotScale = true;
    m_bIsUniformScale = true;
    m_bUpdateInverse = false;
    _UpdateHMatrix();
}
template<typename Real>
TTransform3<Real>::TTransform3( const TMatrix3<Real> & matTransform, const TVector3<Real> & vTranslate ):
    m_matTransform( matTransform ),
    m_vScale( (Real)1, (Real)1, (Real)1 ),
    m_vTranslate( vTranslate ),
    m_matHMatrix( TMatrix4<Real>::Identity ),
    m_matInvTransform( TMatrix3<Real>::Identity ),
    m_vInvScale( (Real)1, (Real)1, (Real)1 ),
    m_vInvTranslate( TVector3<Real>::Null )
{
    m_bIsIdentity = false;
    m_bIsRotScale = false;
    m_bIsUniformScale = false;
    m_bUpdateInverse = false;
    _UpdateHMatrix();
}
template<typename Real>
TTransform3<Real>::TTransform3( const TTransform3<Real> & rhs ):
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
TTransform3<Real>::~TTransform3()
{
    // nothing to do
}

template<typename Real>
TTransform3<Real> & TTransform3<Real>::operator=( const TTransform3<Real> & rhs )
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
inline TVertex4<Real> TTransform3<Real>::operator*( const TVertex4<Real> & rhs ) const {
    return m_matHMatrix * rhs;
}
template<typename Real>
inline TVertex3<Real> TTransform3<Real>::operator*( const TVertex3<Real> & rhs ) const {
    return TVertex3<Real>( m_matHMatrix * TVertex4<Real>(rhs) );
}

template<typename Real>
inline TVector4<Real> TTransform3<Real>::operator*( const TVector4<Real> & rhs ) const {
    return m_matHMatrix * rhs;
}
template<typename Real>
inline TVector3<Real> TTransform3<Real>::operator*( const TVector3<Real> & rhs ) const {
    return TVector3<Real>( m_matHMatrix * TVector4<Real>(rhs) );
}

template<typename Real>
TTransform3<Real> TTransform3<Real>::operator*( const TTransform3<Real> & rhs ) const
{
    if ( m_bIsIdentity )
        return rhs;
    if ( rhs.m_bIsIdentity )
        return (*this);

    TTransform3<Real> trComposed;
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

    TMatrix3<Real> matMA = m_matTransform;
    if ( m_bIsRotScale ) {
        matMA.m00 *= m_vScale.X; matMA.m01 *= m_vScale.Y; matMA.m02 *= m_vScale.Z;
        matMA.m10 *= m_vScale.X; matMA.m11 *= m_vScale.Y; matMA.m12 *= m_vScale.Z;
        matMA.m20 *= m_vScale.X; matMA.m21 *= m_vScale.Y; matMA.m22 *= m_vScale.Z;
    }
    TMatrix3<Real> matMB = rhs.m_matTransform;
    if ( rhs.m_bIsRotScale ) {
        matMB.m00 *= rhs.m_vScale.X; matMB.m01 *= rhs.m_vScale.Y; matMB.m02 *= rhs.m_vScale.Z;
        matMB.m10 *= rhs.m_vScale.X; matMB.m11 *= rhs.m_vScale.Y; matMB.m12 *= rhs.m_vScale.Z;
        matMB.m20 *= rhs.m_vScale.X; matMB.m21 *= rhs.m_vScale.Y; matMB.m22 *= rhs.m_vScale.Z;
    }
    trComposed.SetTransform( matMA * matMB );
    trComposed.SetTranslate( (matMA * rhs.m_vTranslate) + m_vTranslate );
    return trComposed;
}

template<typename Real>
Void TTransform3<Real>::SetTransform( const TMatrix3<Real> & matTransform )
{
    m_matTransform = matTransform;

    m_bIsIdentity = false;
    m_bIsRotScale = false;
    m_bIsUniformScale = false;
    _UpdateHMatrix();
}
template<typename Real>
Void TTransform3<Real>::SetRotate( const TMatrix3<Real> & matRotate )
{
    m_matTransform = matRotate;

    m_bIsIdentity = false;
    m_bIsRotScale = true;
    _UpdateHMatrix();
}
template<typename Real>
Void TTransform3<Real>::SetScale( const TVector3<Real> & vScale )
{
    Assert(m_bIsRotScale);
    Assert(vScale != TVector3<Real>::Null);

    m_vScale = vScale;

    m_bIsIdentity = false;
    m_bIsUniformScale = false;
    _UpdateHMatrix();
}
template<typename Real>
Void TTransform3<Real>::SetUniformScale( Real fScale )
{
    Assert(m_bIsRotScale);
    Assert(fScale != (Real)0);

    m_vScale = TVector3<Real>( fScale, fScale, fScale );

    m_bIsIdentity = false;
    m_bIsUniformScale = true;
    _UpdateHMatrix();
}
template<typename Real>
Void TTransform3<Real>::SetTranslate( const TVector3<Real> & vTranslate )
{
    m_vTranslate = vTranslate;

    m_bIsIdentity = false;
    _UpdateHMatrix();
}

template<typename Real>
inline Bool TTransform3<Real>::IsIdentity() const {
    return m_bIsIdentity;
}
template<typename Real>
inline Bool TTransform3<Real>::IsRotScale() const {
    return m_bIsRotScale;
}
template<typename Real>
inline Bool TTransform3<Real>::IsUniformScale() const {
    return (m_bIsRotScale && m_bIsUniformScale);
}

template<typename Real>
inline const TMatrix3<Real> & TTransform3<Real>::GetTransform() const {
    return m_matTransform;
}
template<typename Real>
inline const TMatrix3<Real> & TTransform3<Real>::GetRotate() const {
    Assert(m_bIsRotScale);
    return m_matTransform;
}
template<typename Real>
inline const TVector3<Real> & TTransform3<Real>::GetScale() const {
    Assert(m_bIsRotScale);
    return m_vScale;
}
template<typename Real>
inline Real TTransform3<Real>::GetUniformScale() const {
    Assert(m_bIsRotScale && m_bIsUniformScale);
    return m_vScale.X;
}
template<typename Real>
inline const TVector3<Real> & TTransform3<Real>::GetTranslate() const {
    return m_vTranslate;
}

template<typename Real>
inline const TMatrix4<Real> & TTransform3<Real>::GetHMatrix() const {
    return m_matHMatrix;
}

template<typename Real>
Void TTransform3<Real>::MakeIdentity()
{
    m_matTransform = TMatrix3<Real>::Identity;
    m_vScale = TVector3<Real>( (Real)1, (Real)1, (Real)1 );
    m_vTranslate = TVector3<Real>::Null;

    m_bIsIdentity = true;
    m_bIsRotScale = true;
    m_bIsUniformScale = true;
    _UpdateHMatrix();
}
template<typename Real>
Void TTransform3<Real>::MakeUnitScale()
{
    Assert(m_bIsRotScale);

    m_vScale = TVector3<Real>( (Real)1, (Real)1, (Real)1 );

    m_bIsUniformScale = true;
    _UpdateHMatrix();
}

template<typename Real>
Real TTransform3<Real>::GetMaxScale() const
{
    if (m_bIsRotScale) {
        Real fMaxScale = MathRealFn->Abs(m_vScale.X);
        Real fScale = MathRealFn->Abs(m_vScale.Y);
        if ( fScale > fMaxScale )
            fMaxScale = fScale;
        fScale = MathRealFn->Abs(m_vScale.Z);
        if ( fScale > fMaxScale )
            fMaxScale = fScale;
        return fMaxScale;
    }

    // Max Row-Sum is a good approximation of the maximal scale.
    // The spectral norm (maximum absolute value of eigenvalues)
    // is smaller or equal to this norm.
    Real fMaxRowSum = ( MathRealFn->Abs(m_matTransform.m00) + MathRealFn->Abs(m_matTransform.m01) +
                        MathRealFn->Abs(m_matTransform.m02) );
    Real fRowSum = ( MathRealFn->Abs(m_matTransform.m10) + MathRealFn->Abs(m_matTransform.m11) +
                     MathRealFn->Abs(m_matTransform.m12) );
    if ( fRowSum > fMaxRowSum )
        fMaxRowSum = fRowSum;
    fRowSum = ( MathRealFn->Abs(m_matTransform.m20) + MathRealFn->Abs(m_matTransform.m21) +
                MathRealFn->Abs(m_matTransform.m22) );
    if ( fRowSum > fMaxRowSum )
        fMaxRowSum = fRowSum;
    return fMaxRowSum;
}

template<typename Real>
Bool TTransform3<Real>::Invert( TTransform3<Real> & outInvTransform ) const
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
Void TTransform3<Real>::_UpdateHMatrix()
{
    if ( m_bIsIdentity )
        m_matHMatrix = TMatrix4<Real>::Identity;
    else {
        m_matHMatrix = m_matTransform;
        if ( m_bIsRotScale ) {
            m_matHMatrix.m00 *= m_vScale.X;
            m_matHMatrix.m01 *= m_vScale.Y;
            m_matHMatrix.m02 *= m_vScale.Z;
            m_matHMatrix.m10 *= m_vScale.X;
            m_matHMatrix.m11 *= m_vScale.Y;
            m_matHMatrix.m12 *= m_vScale.Z;
            m_matHMatrix.m20 *= m_vScale.X;
            m_matHMatrix.m21 *= m_vScale.Y;
            m_matHMatrix.m22 *= m_vScale.Z;
        }
        m_matHMatrix.m03 = m_vTranslate.X;
        m_matHMatrix.m13 = m_vTranslate.Y;
        m_matHMatrix.m23 = m_vTranslate.Z;
        // last row is allways (0, 0, 0, 1)
    }

    m_bUpdateInverse = true;
}
template<typename Real>
Bool TTransform3<Real>::_UpdateInverse() const
{
    if ( !m_bUpdateInverse )
        return true;

    if ( m_bIsIdentity ) {
        m_matInvTransform = TMatrix3<Real>::Identity;
        m_vInvScale = TVector3<Real>( (Real)1, (Real)1, (Real)1 );
        m_vInvTranslate = TVector3<Real>::Null;

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
            m_vInvScale.Z = fInvScale;

            m_vInvTranslate *= fInvScale;
        } else {
            m_vInvScale.X = MathRealFn->Invert(m_vScale.X);
            m_vInvScale.Y = MathRealFn->Invert(m_vScale.Y);
            m_vInvScale.Z = MathRealFn->Invert(m_vScale.Z);

            m_vInvTranslate.X *= m_vInvScale.X;
            m_vInvTranslate.Y *= m_vInvScale.Y;
            m_vInvTranslate.Z *= m_vInvScale.Z;
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


