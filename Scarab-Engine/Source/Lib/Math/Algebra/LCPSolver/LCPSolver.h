/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Algebra/LCPSolver/LCPSolver.h
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
// Known Bugs : We solve the LCP problem formulated as
//            | Ax + b >= 0   OR  | Mz + q = w           which are totally
//            | x >= 0            | z >= 0, w >= 0       equivalent.
//            | (Ax+b).x = 0      | w.z = 0
//            | Solve x           | Solve (z,w)
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_LIB_MATH_ALGEBRA_LCPSOLVER_LCPSOLVER_H
#define SCARAB_LIB_MATH_ALGEBRA_LCPSOLVER_LCPSOLVER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Memory/MemoryManager.h"

#include "../../Functions/MathFunction.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
enum LCPState
{
    LCP_FOUND = 0,          // found solution
    LCP_FOUND_TRIVIAL,      // found trivial solution ( w=q, z=0 )
    LCP_CANT_SOLVE,         // no solution, unbounded constraint domain
    LCP_MAX_RETRIES_REACHED // no solution, exceeded max retries (rounding error ?)
};

/////////////////////////////////////////////////////////////////////////////////
// The TLCPSolver class
template<typename Real>
class TLCPSolver
{
public:
    TLCPSolver( UInt iMaxIterations = 100, Real fZeroTolerance = 0.0 );
    virtual ~TLCPSolver();

    // Solving parameters
    UInt MaxIterations;
    Real ZeroTolerance;  // <Real>_ERROR

    // LCP input access
    inline Void SetDimension( UInt iConstraintCount );
    inline Void SetInput( Real * matM, Real * vQ );

    // Allocation & destruction of temp memory is made by the user when
    // he whishes it, but beware allocation is valid only for a given
    // constant number of constraints (dimension).
    Void Allocate();
    Void Destroy();

    // This class uses Lemke's dictionary approach (direct solver) ...
    // We solve a system of N linear constraints :
    // ( Mz + q = w    M is a positive semi-definite (N*N)-matrix
    //   w >= 0        q is an N-vector
    //   z >= 0        z and w are the solutions which can exist or not
    //   w.z = 0    )
    // One iteration is theorically sufficient but rounding errors might
    // produce fails. The algorithm can try again for iMaxRetries times.
    // All comparisons to zero use fZeroTolerance as a threshold, particularly
    // when looking for a pivot constraint.
    // Some intermediate values and computed constants or (w|z)-factors
    // are automatically clamped to 0.0 when reaching +/- fZeroClamp range.
    // Note that the whole solver uses double-precision floats ...
    Void Solve( LCPState & outState, Real * outW, Real * outZ );

private:
    // Constraint - Equation type
    typedef struct _lcp_constraint
    {
        Char VarName;  // 'w' or 'z'
        UInt VarIndex; // index of w or z
        Real * W;      // factors of the w terms
        Real * Z;      // factors of the z terms
        Real * C;      // constant terms
    } LCPConstraint;

    // Solver methods
    Bool _InitializeConstraints();
    Bool _SearchConstraint( UInt & iConstraint );
    Bool _SelectConstraint( UInt & iConstraint );

    Void _SolveConstraint( Char chBasicVarName, UInt iBasicVarIndex );

    // Solver input
    UInt m_iConstraintCount;
    Real * m_matM;
    Real * m_vQ;

    // Solver state
    Bool m_bAllocated;
    LCPConstraint * m_arrConstraints;
    Char m_chNonBasicVarName;
    UInt m_iNonBasicVarIndex;
    Char m_chMovingVarName;
    UInt m_iMovingVarIndex;

    // Temporary variables
    UInt * m_arrFoundFactors[2];
    LCPConstraint m_SwapConstraint;
};

// Explicit instanciation
typedef TLCPSolver<Float> LCPSolverf;
typedef TLCPSolver<Double> LCPSolverd;
typedef TLCPSolver<Scalar> LCPSolver;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "LCPSolver.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_ALGEBRA_LCPSOLVER_LCPSOLVER_H

