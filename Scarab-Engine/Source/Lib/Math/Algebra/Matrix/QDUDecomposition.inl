/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Algebra/Matrix/QDUDecomposition.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : QDU decomposition for 3x3 matrices
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TQDUDecomposition implementation
template<typename Real>
const Real TQDUDecomposition<Real>::ZERO = (Real)0;
template<typename Real>
const Real TQDUDecomposition<Real>::ONE = (Real)1;

template<typename Real>
TQDUDecomposition<Real>::TQDUDecomposition( const TMatrix3<Real> & matRotation ):
    m_matRotation( matRotation )
{
    // nothing to do
}
template<typename Real>
TQDUDecomposition<Real>::~TQDUDecomposition()
{
    // nothing to do
}

template<typename Real>
Void TQDUDecomposition<Real>::Compute( TMatrix3<Real> & outQ, TMatrix3<Real> & outD, TMatrix3<Real> & outU ) const
{
    // Build Q (Gram-Schmidt)
        // Normalize col 0
    Real fInvLength = MathRealFn->InvSqrt( (m_matRotation.m00 * m_matRotation.m00) +
                                           (m_matRotation.m10 * m_matRotation.m10) +
                                           (m_matRotation.m20 * m_matRotation.m20) );
    outQ.m00 = m_matRotation.m00 * fInvLength;
    outQ.m10 = m_matRotation.m10 * fInvLength;
    outQ.m20 = m_matRotation.m20 * fInvLength;
        // Setup col 1
    Real fDot = ( (outQ.m00 * m_matRotation.m01) +
                  (outQ.m10 * m_matRotation.m11) +
                  (outQ.m20 * m_matRotation.m21) );
    outQ.m01 = m_matRotation.m01 - (fDot * outQ.m00);
    outQ.m11 = m_matRotation.m11 - (fDot * outQ.m10);
    outQ.m21 = m_matRotation.m21 - (fDot * outQ.m20);
        // Normalize col 1
    fInvLength = MathRealFn->InvSqrt( (outQ.m01 * outQ.m01) +
                                      (outQ.m11 * outQ.m11) +
                                      (outQ.m21 * outQ.m21) );
    outQ.m01 *= fInvLength;
    outQ.m11 *= fInvLength;
    outQ.m21 *= fInvLength;
        // Setup col 2
    fDot = ( (outQ.m00 * m_matRotation.m02) +
             (outQ.m10 * m_matRotation.m12) +
             (outQ.m20 * m_matRotation.m22) );
    outQ.m02 = m_matRotation.m02 - (fDot * outQ.m00);
    outQ.m12 = m_matRotation.m12 - (fDot * outQ.m10);
    outQ.m22 = m_matRotation.m22 - (fDot * outQ.m20);
    fDot = ( (outQ.m01 * m_matRotation.m02) +
             (outQ.m11 * m_matRotation.m12) +
             (outQ.m21 * m_matRotation.m22) );
    outQ.m02 -= (fDot * outQ.m01);
    outQ.m12 -= (fDot * outQ.m11);
    outQ.m22 -= (fDot * outQ.m21);
        // Normalize col 2
    fInvLength = MathRealFn->InvSqrt( (outQ.m02 * outQ.m02) +
                                      (outQ.m12 * outQ.m12) +
                                      (outQ.m22 * outQ.m22) );
    outQ.m02 *= fInvLength;
    outQ.m12 *= fInvLength;
    outQ.m22 *= fInvLength;

    // Ensure Q has det = 1 (no reflection)
    Real fDet = outQ.Determinant();
    if ( fDet < ZERO ) {
        outQ = -outQ;
    }

    // Build R
    TVector3<Real> vDiagR;
    vDiagR.X = (outQ.m00 * m_matRotation.m00) + (outQ.m10 * m_matRotation.m10) + (outQ.m20 * m_matRotation.m20);
    vDiagR.Y = (outQ.m01 * m_matRotation.m01) + (outQ.m11 * m_matRotation.m11) + (outQ.m21 * m_matRotation.m21);
    vDiagR.Z = (outQ.m02 * m_matRotation.m02) + (outQ.m12 * m_matRotation.m12) + (outQ.m22 * m_matRotation.m22);
    TVector3<Real> vShearR;
    vShearR.X = (outQ.m00 * m_matRotation.m01) + (outQ.m10 * m_matRotation.m11) + (outQ.m20 * m_matRotation.m21);
    vShearR.Y = (outQ.m00 * m_matRotation.m02) + (outQ.m10 * m_matRotation.m12) + (outQ.m20 * m_matRotation.m22);
    vShearR.Z = (outQ.m01 * m_matRotation.m02) + (outQ.m11 * m_matRotation.m12) + (outQ.m21 * m_matRotation.m22);

    // Extract scale D
    outD.MakeDiagonal( vDiagR );

    // Extract shear U
    Real fInvDiagX = MathRealFn->Invert( vDiagR.X );
    outU.m00 = ONE;  outU.m01 = vShearR.X * fInvDiagX; outU.m02 = vShearR.Y * fInvDiagX;
    outU.m10 = ZERO; outU.m11 = ONE;                   outU.m12 = vShearR.Z / vDiagR.Y;
    outU.m20 = ZERO; outU.m21 = ZERO;                  outU.m22 = ONE;
}

