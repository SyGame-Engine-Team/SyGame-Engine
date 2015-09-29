/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Algebra/TLCPSolver/TLCPSolver.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Linear Complementarity Problem, Lemke's direct solver
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TLCPSolver implementation
template<typename Real>
TLCPSolver<Real>::TLCPSolver( UInt iMaxIterations, Real fZeroTolerance )
{
    m_iConstraintCount = 0;
    m_matM = NULL;
    m_vQ = NULL;

    m_bAllocated = false;
    m_arrConstraints = NULL;
    m_chNonBasicVarName = 0;
    m_iNonBasicVarIndex = INVALID_OFFSET;
    m_chMovingVarName = 0;
    m_iMovingVarIndex = INVALID_OFFSET;

    MaxIterations = iMaxIterations;
    ZeroTolerance = fZeroTolerance;
}
template<typename Real>
TLCPSolver<Real>::~TLCPSolver()
{
    // nothing to do
}

template<typename Real>
inline Void TLCPSolver<Real>::SetDimension( UInt iConstraintCount ) {
    Assert( !m_bAllocated );
    m_iConstraintCount = iConstraintCount;
}
template<typename Real>
inline Void TLCPSolver<Real>::SetInput( Real * matM, Real * vQ ) {
    m_matM = matM;
    m_vQ = vQ;
}

template<typename Real>
Void TLCPSolver<Real>::Allocate()
{
    Assert( m_matM != NULL && m_vQ != NULL );
    Assert( !m_bAllocated );

    m_arrConstraints = New LCPConstraint[m_iConstraintCount];
    UInt iConstraintCountP1 = m_iConstraintCount + 1;
    for( UInt i = 0; i < m_iConstraintCount; ++i ) {
        m_arrConstraints[i].W = New Real[iConstraintCountP1];
        m_arrConstraints[i].Z = New Real[iConstraintCountP1];
        m_arrConstraints[i].C = New Real[iConstraintCountP1];
    }

    m_arrFoundFactors[0] = New UInt[iConstraintCountP1];
    m_arrFoundFactors[1] = New UInt[iConstraintCountP1];
    m_SwapConstraint.W = New Real[iConstraintCountP1];
    m_SwapConstraint.Z = New Real[iConstraintCountP1];
    m_SwapConstraint.C = New Real[iConstraintCountP1];

    m_bAllocated = true;
}
template<typename Real>
Void TLCPSolver<Real>::Destroy()
{
    Assert( m_bAllocated );

    DeleteA( m_arrFoundFactors[0] );
    DeleteA( m_arrFoundFactors[1] );
    DeleteA( m_SwapConstraint.W );
    DeleteA( m_SwapConstraint.Z );
    DeleteA( m_SwapConstraint.C );

    for( UInt i = 0; i < m_iConstraintCount; ++i ) {
        DeleteA( m_arrConstraints[i].W );
        DeleteA( m_arrConstraints[i].Z );
        DeleteA( m_arrConstraints[i].C );
    }
    DeleteA( m_arrConstraints );

    m_bAllocated = false;
    m_arrConstraints = NULL;
    m_chNonBasicVarName = 0;
    m_iNonBasicVarIndex = INVALID_OFFSET;
    m_chMovingVarName = 0;
    m_iMovingVarIndex = INVALID_OFFSET;    
}

template<typename Real>
Void TLCPSolver<Real>::Solve( LCPState & outState, Real * outW, Real * outZ )
{
    Assert( m_bAllocated );

    const UInt iSize = m_iConstraintCount * sizeof(Real);

    // Trivial input => trivial solution
    Bool bTrivial = _InitializeConstraints();
    if ( bTrivial ) {
        MemCopy( outW, m_vQ, iSize );
        MemZero( outZ, iSize );
        outState = LCP_FOUND_TRIVIAL;
        return;
    }

    // NonTrivial, Solve it
    UInt iConstraint, i;
    Bool bSelected, bBasicZ0;
    UInt iRetry = 0;
    while( iRetry < MaxIterations ) {
        // Select an equation as pivot
        bSelected = _SelectConstraint( iConstraint );
        if ( !bSelected ) {
            outState = LCP_CANT_SOLVE;
            break;
        }

        // Solve that equation
        --iConstraint;
        _SolveConstraint( m_arrConstraints[iConstraint].VarName,
                          m_arrConstraints[iConstraint].VarIndex );

        // Check if Z0 is basic
        bBasicZ0 = false;
        for( i = 0; i < m_iConstraintCount; ++i ) {
            if ( m_arrConstraints[i].VarName == 'Z' && m_arrConstraints[i].VarIndex == 0 ) {
                bBasicZ0 = true;
                break;
            }
        }

        // Z0 is non-basic, solution is found
        if ( !bBasicZ0 ) {
            MemZero( outW, iSize );
            MemZero( outZ, iSize );
            for( i = 0; i < m_iConstraintCount; ++i ) {
                if ( m_arrConstraints[i].VarName == 'Z' )
                    outZ[m_arrConstraints[i].VarIndex - 1] = m_arrConstraints[i].C[0];
                else // m_arrConstraints[i].VarName == 'W'
                    outW[m_arrConstraints[i].VarIndex - 1] = m_arrConstraints[i].C[0];
            }
            outState = LCP_FOUND;
            break;
        }

        ++iRetry;
    }
    if ( iRetry == MaxIterations )
        outState = LCP_MAX_RETRIES_REACHED;
}

/////////////////////////////////////////////////////////////////////////////////

template<typename Real>
Bool TLCPSolver<Real>::_InitializeConstraints()
{
    const UInt iConstraintCountP1 = m_iConstraintCount + 1;
    const UInt iSize = iConstraintCountP1 * sizeof(Real);

    // All W basic, indices from 1 to m_iConstraintCount,
    // extra var at index 0 is Z0
    UInt i;
    for( i = 0; i < m_iConstraintCount; ++i ) {
        m_arrConstraints[i].VarName = 'W';
        m_arrConstraints[i].VarIndex = i + 1;
        MemZero( m_arrConstraints[i].W, iSize );
        MemZero( m_arrConstraints[i].Z, iSize );
        MemZero( m_arrConstraints[i].C, iSize );
        m_arrConstraints[i].Z[0] = 1.0;
        m_arrConstraints[i].C[i + 1] = 1.0;
    }

    // Check constant terms positivity and signal trivial solution
    Real fValue = 0.0;
    for( i = 0; i < m_iConstraintCount; ++i ) {
        m_arrConstraints[i].C[0] = m_vQ[i];
        if ( m_vQ[i] < fValue )
            fValue = m_vQ[i];
    }
    if ( fValue >= 0.0 )
        return true; // Trivial

    // All Z nonbasic
    UInt j, iOffset;
    Real fIsZero;
    for( i = 0; i < m_iConstraintCount; ++i ) {
        iOffset = ( i * m_iConstraintCount );
        fIsZero = 0.0;
        for( j = 0; j < m_iConstraintCount; ++j ) {
            fValue = m_matM[iOffset + j];
            m_arrConstraints[i].Z[j + 1] = fValue;
            if ( !(MathDFn->EqualsZero(fValue, ZeroTolerance)) )
                fIsZero = 1.0;
        }
        m_arrConstraints[i].Z[0] *= fIsZero;
    }

    // "Normalize" all constraints
    Real fMax, fInvMax;
    for( i = 0; i < m_iConstraintCount; ++i ) {
        // Find max abs value
        fMax = 0.0;
        for( j = 0; j < iConstraintCountP1; ++j ) {
            fValue = MathDFn->Abs( m_arrConstraints[i].W[j] );
            if ( fValue > fMax )
                fMax = fValue;
            fValue = MathDFn->Abs( m_arrConstraints[i].Z[j] );
            if ( fValue > fMax )
                fMax = fValue;
            fValue = MathDFn->Abs( m_arrConstraints[i].C[j] );
            if ( fValue > fMax )
                fMax = fValue;
        }

        // "Normalize" the constraint
        fInvMax = MathDFn->Invert(fMax);
        for( j = 0; j < iConstraintCountP1; ++j ) {
            m_arrConstraints[i].W[j] *= fInvMax;
            m_arrConstraints[i].Z[j] *= fInvMax;
            m_arrConstraints[i].C[j] *= fInvMax;
        }
    }

    // NonTrivial
    return false;
}
template<typename Real>
Bool TLCPSolver<Real>::_SearchConstraint( UInt & iConstraint )
{
    if ( m_iMovingVarIndex == 0 ) {
        // Special case for nonbasic Z0, find limiting constraint for Z0
        // which is the one with the highest abs value quotient amongst
        // negative terms for Z0 ...
        Real fValue, fMin = 0.0;
        for( UInt i = 0; i < m_iConstraintCount; ++i ) {
            if ( !(MathDFn->EqualsZero(m_arrConstraints[i].Z[0], ZeroTolerance)) ) {
                fValue = ( m_arrConstraints[i].C[0] / m_arrConstraints[i].Z[0] );
                if ( fMin == 0.0 || fValue <= fMin ) {
                    fMin = fValue;
                    iConstraint = i + 1;
                }
            }
        }
        return (fMin < 0.0);
    }

    // Find all terms in W or Z arrays for which the chosen term's
    // factor is negative. For C, find the constraints for which
    // (C / (W or Z)) is  the smallest. These 2 "filters" reduce
    // the row search, then continue with next constraint until
    // there's only one left.

    // Find constraints with negative factors for moving index
    Real fTemp;
    UInt i, j = 0;
    for( i = 0; i < m_iConstraintCount; ++i ) {
        if ( m_chNonBasicVarName == 'W' )
            fTemp = m_arrConstraints[i].W[m_iMovingVarIndex];
        else // m_chNonBasicVarName == 'Z'
            fTemp = m_arrConstraints[i].Z[m_iMovingVarIndex];
        if ( fTemp < 0.0 )
            (m_arrFoundFactors[0])[j++] = i;
    }
    (m_arrFoundFactors[0])[j] = INVALID_OFFSET;

    // At this stage we should allways find one
    if ( j == 0 ) {
        Assert(false);
        return false;
    }

    // Find smallest (C / (W or Z))
    UInt iConstraintCountP1 = m_iConstraintCount + 1;
    UInt iFF0 = 0, iFF1;
    UInt iFound0, iFound1, iCur0, iCur1, k;
    Real fDenom0, fDenom1;
    for( i = 0; i < iConstraintCountP1; ++i ) {
        iFF1 = (1 - iFF0);
        iFound0 = 0;
        iFound1 = 0;
        iCur0 = (m_arrFoundFactors[iFF0])[iFound0++];
        (m_arrFoundFactors[iFF1])[iFound1++] = iCur0;
        k = iFound0;
        while( (m_arrFoundFactors[iFF0])[k] != INVALID_OFFSET ) {
            iCur1 = (m_arrFoundFactors[iFF0])[k];
            if ( iCur1 == INVALID_OFFSET )
                break;
            if ( m_chNonBasicVarName == 'W' ) {
                fDenom0 = m_arrConstraints[iCur0].W[m_iMovingVarIndex];
                fDenom1 = m_arrConstraints[iCur1].W[m_iMovingVarIndex];
            } else { // m_chNonBasicVarName == 'Z'
                fDenom0 = m_arrConstraints[iCur0].Z[m_iMovingVarIndex];
                fDenom1 = m_arrConstraints[iCur1].Z[m_iMovingVarIndex];
            }
            fTemp = ( (m_arrConstraints[iCur1].C[i] / fDenom1) -
                      (m_arrConstraints[iCur0].C[i] / fDenom0) );
            if ( fTemp < -ZeroTolerance ) {
                // FF0 is the smallest, nothing to do
            } else if ( fTemp > ZeroTolerance ) {
                // FF1 is the smallest
                iFound0 = k;
                iFound1 = 0;
                iCur0 = (m_arrFoundFactors[iFF0])[iFound0++];
                (m_arrFoundFactors[iFF1])[iFound1++] = iCur0;
            } else
                (m_arrFoundFactors[iFF1])[iFound1++] = iCur1;
            ++k;
            (m_arrFoundFactors[iFF1])[iFound1] = INVALID_OFFSET;
        }
        if ( iFound1 == 1 ) {
            iConstraint = (m_arrFoundFactors[iFF1])[0] + 1;
            return true;
        }
        iFF0 = (1 - iFF0);
    }

    // Should never get there
    Assert(false);
    return false;
}
template<typename Real>
Bool TLCPSolver<Real>::_SelectConstraint( UInt & iConstraint )
{
    // Select the constraint to be solved :
    // - If Z0 is nonbasic, solve for Z0 and pick constraint
    //   with smallest negative C.
    // - If a Wi has just been set nonbasic, solve for Zj with
    //   constraint : Cj factor of Zj is negative and smallest Cj/Zj.

    // Check if Z0 is basic
    Bool bBasicZ0 = false;
    for( UInt i = 0; i < m_iConstraintCount; ++i ) {
        if ( m_arrConstraints[i].VarName == 'Z' && m_arrConstraints[i].VarIndex == 0 )
            bBasicZ0 = true;
    }
    if ( bBasicZ0 )
        m_chNonBasicVarName = (m_iMovingVarIndex == 'W') ? 'Z' : 'W';
    else {
        m_iMovingVarIndex = 0;
        m_chNonBasicVarName = 'Z';
        m_iNonBasicVarIndex = 0;
    }

    // Search for the constraint
    Bool bFound = _SearchConstraint( iConstraint );
    if (bFound) {
        UInt iPrev = iConstraint - 1;
        m_iNonBasicVarIndex = m_iMovingVarIndex;
        m_chMovingVarName = m_arrConstraints[iPrev].VarName;
        m_iMovingVarIndex = m_arrConstraints[iPrev].VarIndex;
    }
    return bFound;
}

template<typename Real>
Void TLCPSolver<Real>::_SolveConstraint( Char chBasicVarName, UInt iBasicVarIndex )
{
    // Search for the basic var
    UInt i, iFound = INVALID_OFFSET;
    for( i = 0; i < m_iConstraintCount; ++i ) {
        if ( m_arrConstraints[i].VarName == chBasicVarName ) {
            if ( m_arrConstraints[i].VarIndex == iBasicVarIndex )
                iFound = i;
        }
    }
    if ( iFound == INVALID_OFFSET )
        return; // no basic var

    // Prepare swap constraint
    Real fDenom, fInvDenom;
    if ( m_chNonBasicVarName == 'W' )
        fDenom = -( m_arrConstraints[iFound].W[m_iNonBasicVarIndex] );
    else // m_chNonBasicVarName == 'Z'
        fDenom = -( m_arrConstraints[iFound].Z[m_iNonBasicVarIndex] );
    fInvDenom = MathDFn->Invert(fDenom);

    UInt iConstraintCountP1 = m_iConstraintCount + 1;
    m_SwapConstraint.VarName = m_chNonBasicVarName;
    m_SwapConstraint.VarIndex = m_iNonBasicVarIndex;
    for ( i = 0; i < iConstraintCountP1; ++i ) {
        m_SwapConstraint.W[i] = m_arrConstraints[iFound].W[i] * fInvDenom;
        m_SwapConstraint.Z[i] = m_arrConstraints[iFound].Z[i] * fInvDenom;
        m_SwapConstraint.C[i] = m_arrConstraints[iFound].C[i] * fInvDenom;
    }

    if ( m_chNonBasicVarName == 'W' )
        m_SwapConstraint.W[m_iNonBasicVarIndex] = 0.0;
    else // m_chNonBasicVarName == 'Z'
        m_SwapConstraint.Z[m_iNonBasicVarIndex] = 0.0;

    if ( chBasicVarName == 'W' )
        m_SwapConstraint.W[iBasicVarIndex] = -fInvDenom;
    else // m_chNonBasicVarName == 'Z'
        m_SwapConstraint.Z[iBasicVarIndex] = -fInvDenom;

    // Substitute
    UInt j;
    Real fFactor;
    for( i = 0; i < m_iConstraintCount; ++i ) {
        if ( i == iFound )
            continue;
        if ( m_SwapConstraint.VarName == 'W' )
            fFactor = m_arrConstraints[i].W[m_iNonBasicVarIndex];
        else // m_SwapConstraint.VarName == 'Z'
            fFactor = m_arrConstraints[i].Z[m_iNonBasicVarIndex];
        if ( MathDFn->EqualsZero(fFactor, ZeroTolerance) )
            continue;

        for( j = 0; j < iConstraintCountP1; ++j ) {
            m_arrConstraints[i].W[j] += ( fFactor * m_SwapConstraint.W[j] );
            if ( MathDFn->Abs(m_arrConstraints[i].W[j]) < m_fZeroClamp * MathDFn->Abs(m_SwapConstraint.W[j]) )
                m_arrConstraints[i].W[j] = 0.0;

            m_arrConstraints[i].Z[j] += ( fFactor * m_SwapConstraint.Z[j] );
            if ( MathDFn->Abs(m_arrConstraints[i].Z[j]) < m_fZeroClamp * MathDFn->Abs(m_SwapConstraint.Z[j]) )
                m_arrConstraints[i].Z[j] = 0.0;

            m_arrConstraints[i].C[j] += ( fFactor * m_SwapConstraint.C[j] );
            if ( MathDFn->Abs(m_arrConstraints[i].C[j]) < m_fZeroClamp * MathDFn->Abs(m_SwapConstraint.C[j]) )
                m_arrConstraints[i].C[j] = 0.0;
        }

        if ( m_SwapConstraint.VarName == 'W' )
            m_arrConstraints[i].W[m_SwapConstraint.VarIndex] = 0.0;
        else // m_SwapConstraint.VarName == 'Z'
            m_arrConstraints[i].Z[m_SwapConstraint.VarIndex] = 0.0;
    }
    const UInt iSize = ( iConstraintCountP1 * sizeof(Real) );
    m_arrConstraints[iFound].VarName = m_SwapConstraint.VarName;
    m_arrConstraints[iFound].VarIndex = m_SwapConstraint.VarIndex;
    MemCopy( m_arrConstraints[iFound].W, m_SwapConstraint.W, iSize );
    MemCopy( m_arrConstraints[iFound].Z, m_SwapConstraint.Z, iSize );
    MemCopy( m_arrConstraints[iFound].C, m_SwapConstraint.C, iSize );
}


