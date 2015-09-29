/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Patching/PatchManager.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Core-Class for generating & applying patches.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "PatchManager.h"

/////////////////////////////////////////////////////////////////////////////////
// PatchManager implementation
PatchManager::PatchManager():
    Manager<PatchManager>(), m_hTargetFile(), m_hPatchFile()
{
    // nothing to do
}
PatchManager::~PatchManager()
{
    // nothing to do
}

Void PatchManager::Initialize()
{
    // nothing to do
}
Void PatchManager::Cleanup()
{
    // nothing to do
}

Void PatchManager::GeneratePatchFile( const GChar * strOutputPatchFile, const GChar * strPatchTargetFile )
{
    // Open target file
    m_hTargetFile = SystemFn->OpenFile( strPatchTargetFile, FILE_READ );
    Assert( m_hTargetFile.IsValid() );

    // Create patch file
    m_hPatchFile = SystemFn->CreateFile( strOutputPatchFile, FILE_WRITE );
    Assert( m_hPatchFile.IsValid() );

    // Here is the core !
    ///////////////////////////////////////////////

    // Done
    m_hPatchFile.Close();
    m_hTargetFile.Close();
}

Void PatchManager::ApplyPatchFile( const GChar * strTargetFile, const GChar * strPatchFile )
{
    // Open target file
    m_hTargetFile = SystemFn->OpenFile( strTargetFile, FILE_READ | FILE_WRITE );
    Assert( m_hTargetFile.IsValid() );

    // Open patch file
    m_hPatchFile = SystemFn->OpenFile( strPatchFile, FILE_READ );
    Assert( m_hPatchFile.IsValid() );

    // Here is the core !
    ///////////////////////////////////////////////

    // Done
    m_hPatchFile.Close();
    m_hTargetFile.Close();
}

/////////////////////////////////////////////////////////////////////////////////

Void PatchManager::_ComputeProjectionWeights( const Byte * strS, UInt iSizeS )
{
    // Initialize all to 0
    for( UInt i = 0; i < 256; ++i )
        m_arrProjectionWeights[i] = 0.0f;

    // Count occurences
    for( UInt i = 0; i < iSizeS; ++i )
        m_arrProjectionWeights[strS[i]] += 1.0f;

    // Convert to frequencies then weights
    Scalar fInvSizeS = MathFn->Invert( (Scalar)iSizeS );
    for( UInt i = 0; i < 256; ++i )
        m_arrProjectionWeights[i] = MathFn->InvSqrt( m_arrProjectionWeights[i] * fInvSizeS );
}
Void PatchManager::_GeneratePrimeNumbers( UInt * outP, UInt iK, UInt iLowerBound, UInt iUpperBound )
{
    // Generate the set of primes in the given interval (we are guaranted to have at least L/log²(L) primes)

    // Select iK primes amongst them in a uniform & random way

}

Bool PatchManager::_Halfabet_DichotomicSearch( UInt * outIndex, UInt iSize, Byte iValue )
{
    UInt iStart = 0;
    UInt iLength = iSize;
    UInt iLeftLength, iRightLength, iPivot = 0;

    while( iLength > 0 ) {
        iLeftLength = ( iLength >> 1 );
        iRightLength = ( iLength - iLeftLength - 1 );
        iPivot = iStart + iLeftLength;

        if ( iValue == m_arrHalfabet[iPivot] ) {
            *outIndex = iPivot;
            return true;
        }
        if ( iValue < m_arrHalfabet[iPivot] )
            iLength = iLeftLength;
        else { // ( iValue > m_arrHalfabet[iPivot] )
            iStart = iPivot + 1;
            iLength = iRightLength;
        }
    }

    if ( iValue > m_arrHalfabet[iPivot] && iPivot < iSize )
        ++iPivot;
    *outIndex = iPivot;
    return false;
}
Void PatchManager::_Halfabet_Generate()
{
    UInt iCurrentIndex = 0;
    while( iCurrentIndex < 128 ) {
        // Uniform random function
        Byte iRand = (Byte)( RandFn->XorShiftWeyl() & 0xff );

        // Check if we already have that one
        UInt iInsertIndex = INVALID_OFFSET;
        Bool bFound = _Halfabet_DichotomicSearch( &iInsertIndex, iCurrentIndex, iRand );
        if ( bFound )
            continue;

        // Insert (keep sorted)
        for( UInt i = iCurrentIndex; i > iInsertIndex; --i )
            m_arrHalfabet[i] = m_arrHalfabet[i - 1];
        m_arrHalfabet[iInsertIndex] = iRand;
        ++iCurrentIndex;
    }
}
Scalar PatchManager::_Halfabet_Project( Byte iValue )
{
    // R.i(x) = if (x in E(i)) -W else +W
    UInt iInsertIndex = INVALID_OFFSET;
    Bool bFound = _Halfabet_DichotomicSearch( &iInsertIndex, 128, iValue );
    
    if ( bFound )
        return -(m_arrProjectionWeights[iValue]);
    return m_arrProjectionWeights[iValue];
}

UInt PatchManager::_ComputeRho( UInt iN, UInt iM, UInt iJ, UInt iP )
{
    // Count number of couples (x,y) < (n,m) such that x := (y+j) mod P
    UInt iCount = 0;
    for( Int x = 0; x < iN; ++x ) {
        for( Int y = 0; y < iM; ++y ) {
            Int iDiff = ( x - y - (Int)iJ );
            if ( (iDiff % (Int)iP) == 0 )
                ++iCount;
        }
    }
    return iCount;
    // A good approximation for the N, M and L values which can occur here :
    //return ( (iN * iM) / iP );
}

Void PatchManager::_OutputIndex( UInt * arrMatchIndices, Scalar * arrMaximalSums, UInt * pCurrentIndex, UInt iIndexCount, UInt iIndex, Scalar fSum )
{
    Bool bMoveLeft = false;
    UInt iSwapIndex = 0;
    UInt iSwap;
    Scalar fSwap;

    if ( (*pCurrentIndex) < iIndexCount ) {
        // There's room, keep filling
        arrMatchIndices[*pCurrentIndex] = iIndex;
        arrMaximalSums[*pCurrentIndex] = fSum;

        bMoveLeft = true;
        iSwapIndex = *pCurrentIndex;
        ++(*pCurrentIndex);
    } else if ( fSum > arrMaximalSums[iIndexCount - 1] ) {
        // Replace minimal entry
        arrMatchIndices[iIndexCount - 1] = iIndex;
        arrMaximalSums[iIndexCount - 1] = fSum;

        bMoveLeft = true;
        iSwapIndex = iIndexCount - 1;    
    }

    // Move left as much as needed, keeping output sorted
    if ( bMoveLeft ) {
        while( iSwapIndex > 0 ) {
            if ( arrMaximalSums[iSwapIndex-1] >= arrMaximalSums[iSwapIndex] )
                break;
            iSwap = arrMatchIndices[iSwapIndex-1];
            arrMatchIndices[iSwapIndex-1] = arrMatchIndices[iSwapIndex];
            arrMatchIndices[iSwapIndex] = iSwap;
            fSwap = arrMaximalSums[iSwapIndex-1];
            arrMaximalSums[iSwapIndex-1] = arrMaximalSums[iSwapIndex];
            arrMaximalSums[iSwapIndex] = fSwap;
            --iSwapIndex;
        }
    }
}

Void PatchManager::_MismatchSearch( UInt * outMatchIndices, UInt iIndexCount, const Byte * strS, UInt iSizeS, const Byte * strT, UInt iSizeT, Scalar fError )
{
    // This is the heart of this implementation.
    // This function will return indices in S for most-likely-possible (partial)matches of T with a probability of at least (1 - fError)

    // Compute starting values
    Scalar fT = (Scalar)iIndexCount;
    Scalar fN = (Scalar)iSizeS;
    Scalar fM = (Scalar)iSizeT;

    Scalar fP = ( 1.0f - fError );
    Scalar fSqrP = ( fP * fP );
    Scalar fNT_Err = (fN * fT) / fError;
    Scalar fErr_NT = MathFn->Invert( fNT_Err );
    Scalar fLogErr = MathFn->Ln( fError );

    // iK = Projection count / Allowed Levenstein distance for T to approximately match at a given offset
    Scalar fDelta = ( fT * fM * fSqrP * MathFn->Ln(fN) ) / fN;
    UInt iK = (UInt)( MathFn->Ceil( -MathFn->Ln(fNT_Err) / MathFn->Ln(4.0f*fDelta) ) );

    // fL = Minimum useful size of groups to project onto
    Scalar fSqrtDelta = MathFn->Sqrt( fDelta );
    Scalar fTmp =  8.0f * MathFn->Ln( MathFn->RootN(2*iK,fErr_NT) - fSqrtDelta );
    Scalar fL = -( fN * fTmp ) / ( fM * fSqrP + fTmp );
    Assert( MathFn->Sqrt(fN) < fL && fL < fN );

    // Pre-Compute projection weights for all possible byte values
    _ComputeProjectionWeights( strS, iSizeS );

    // Generate iK prime numbers in the interval given by L
    Scalar fLowerBound = fL;                                      // inclusive
    Scalar fUpperBound = fL * ( 1.0f + (2.0f / MathFn->Ln(fL)) ); // exclusive

    UInt * arrP = New UInt[iK];
    _GeneratePrimeNumbers( arrP, iK, MathFn->Ceil(fLowerBound), MathFn->Floor(fUpperBound) );

    // Project along each prime in P
    Scalar ** arrD = New Scalar*[iK];

    for( UInt i = 0; i < iK; ++i ) {
        UInt iP = arrP[i];                // P.i
        Scalar fInvPi = MathFn->Invert( (Scalar)iP );

        // Generate halfabet E(i)
        _Halfabet_Generate();

        // Compute A & B, projections of S & T
        Scalar * arrAi = New Scalar[iP];
        Scalar * arrBi = New Scalar[iP];
        for( UInt j = 0; j < iP; ++j ) {
            // A.i(j)
            arrAi[j] = 0.0f;
            UInt iCeil = (UInt)( MathFn->Ceil( (fN-(Scalar)j) * fInvPi ) );
            for( UInt l = 0; l < iCeil; ++l )
                arrAi[j] += _Halfabet_Project( strS[j+l*iP] );

            // B.i(j)
            arrBi[j] = 0.0f;
            iCeil = (UInt)( MathFn->Ceil( (fM-(Scalar)j) * fInvPi ) );
            for( UInt l = 0; l < iCeil; ++l )
                arrBi[j] += _Halfabet_Project( strT[j+l*iP] );
        }

        // Compute C, cyclic correlation between A & B using the convolution theorem
        SignalTransform hSigA( arrAi, iP );
        SignalTransform hSigB( arrBi, iP );
        hSigA.DFT( arrAi );
        hSigB.DFT( arrBi );

        Scalar * arrCi = New Scalar[iP];
        for( UInt j = 0; j < iP; ++j ) {
            // C.i(j)
            Scalar fCij = 0.0f;
            for( UInt r = 0; r <= iP; ++r )
                fCij += ( arrAi[j+r] * arrBi[r] );
        }

        DeleteA( arrAi );
        DeleteA( arrBi );

        SignalTransform hSigC( arrCi, iP );
        hSigC.InvDFT( arrCi );

        // Deduce D, using "good-offsets" filtering
        arrD[i] = New Scalar[iP];
        for( UInt j = 0; j < iP; ++j ) {
            // O.i(j)
            Scalar fRho = (Scalar)( _ComputeRho(iSizeS, iSizeT, j, iP) );

            // D.i(j)
            arrD[i][j] = -MathFn->Ln( fSqrtDelta + MathFn->Exp(-fM*fP*(arrCi[j] - fM*fP*0.5f) / (2.0f * fRho)) );
        }
        
        DeleteA( arrCi );
    }

    // Extract X = Offsets within S where T matches with probability p >= (1 - fError)
    Scalar * arrMaximalSums = New Scalar[iIndexCount];
    UInt iCurrentIndex = 0;
    for( UInt j = 0; j < iSizeS; ++j ) {
        // Integrate D along i
        Scalar fSumD = 0.0f;
        for( UInt i = 0; i < iK; ++i ) {
            if ( j >= arrP[i] )
                continue;
            fSumD += arrD[i][j];
        }

        // Fill X
        _OutputIndex( outMatchIndices, arrMaximalSums, &iCurrentIndex, iIndexCount, j, fSumD );
    }

    // Done
    DeleteA( arrMaximalSums );
    for( UInt i = 0; i < iK; ++i )
        DeleteA( arrD[i] );
    DeleteA( arrD );
    DeleteA( arrP );
}

/*
Symbol Reference =
  N/Z/R = Sets of natural, relative integers and reals, respectively
  log   = Natural logarithm
  e     = Base of natural logarithm
  pi(x) = Number of positive prime integers less than or equal to x

  E   = Alphabet
  S   = String over E to search into
  n   = Length of S
  T   = String over E to search for in S
  m   = Length of T
  [S] = Index generated from S
  S'  = Substrings of S which approximately match T

  d(x,y) = Indication of how well symbols x, y match. Usually restricted to {0,1}. (Kroenecker Symbol or (weighted) variations)
  V      = Match count vector, V.i is the sum of d(S.(i+j), T.j) for j in [0,m-1]
  ~V     = Randomized approximation to V
  X      = Offsets within S where T approximately matches
  ~X     = Computed estimate of X
  t      = |X|, Number of Offsets in X
  p      = Probability of symbols from S, T matching at offsets in X
  err    = Allowed probability of error (p = 1 - err)
  k      = Allowed Levenstein distance for T to approximately match at a given offset

  k   = Number of projections used
  L   = Minimum useful size of groups to project onto
  P   = Set of primes in the range [L, L(1 + 2/log(L))]
  P.i = Size of groups to project onto
  R.i = Random projections from E to {-1,+1}
  A.i = Projection of S via R.i, Z(P.i) from E^n to R^(P.i)
  B.i = Projection of T via R.i, Z(P.i) from E^m to R^(P.i)
  X.i = Computed approximation of (X mod P.i)

Algorithm description :
  - Let E, S, T, n, m, t, p and err be given, setup :
    => delta = ( t*m*p²*log(n) ) / n
    => k     = Ceil( log(n*t/err) / log(1/(4*delta)) )
    => L     = -8*n*log( nroot(2*k,err/n*t) - sqrt(delta) ) / ( m*p² + 8*log( nroot(2*k,err/n*t) - sqrt(delta) ) )

    => P = { x in N / L <= x < L(1+2/log(L)), x prime }
    => For 1 <= i <= k
    =>   Pick P.i in P and E(i) C E / |E(i)| = |E|/2, uniformly at random
    =>   R.i(x) = (-1)^( |E(i) intersect {x}| ) = if x in E(i) -1 else +1
    =>   For 0 <= j < P.i
    =>      A.i(j) = Sum( l=0, l <= (Ceil((n-j)/P.i) - 1), R.i( S.(j+l*P.i) )
    =>      B.i(j) = Sum( l=0, l <= (Ceil((m-j)/P.i) - 1), R.i( T.(j+l*P.i) )
    =>      C.i(j) = Sum( r=0, r <= P.i, (A.i(j+r) * B.i(r))      // Cyclic correlation of A.i & B.i, this is a FFT
    =>      O.i(j) = | {(x,y)<(n,m) / x := (y+j) mod P.i } |
    =>      D.i(j) = -log( sqrt(delta) + exp( -m*p*(C.i(j) - m*p/2) / (2*O.(P.i)(n,m,j)) ) )
    =>   ForEnd
    => ForEnd

    => Extract t values of j for which Sum(i, D.i(j)) is the largest and 0 <= j < n
    => Use a priority queue ...
    => These are the elements of X with probability at least (1-err), provided sqrt(n) < L < n

*/
