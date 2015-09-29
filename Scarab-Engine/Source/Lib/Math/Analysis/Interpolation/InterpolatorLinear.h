/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Analysis/Interpolation/InterpolatorLinear.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Linear Interpolation for 1D data
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_LIB_MATH_ANALYSIS_INTERPOLATION_INTERPOLATORLINEAR_H
#define SCARAB_LIB_MATH_ANALYSIS_INTERPOLATION_INTERPOLATORLINEAR_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Error/ErrorManager.h"

#include "../../Types/Matrix/Matrix2.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TInterpolatorLinear class
template<typename Real>
class TInterpolatorLinear
{
public:
    TInterpolatorLinear( Real * arrValues, UInt iBound,
                         Real fMin, Real fSpacing );
    ~TInterpolatorLinear();

    // Function access
    inline UInt GetBound() const;
    inline Real * GetValues() const;

    // Domain access
    inline Real GetMin() const;
    inline Real GetMax() const;
    inline Real GetSpacing() const;

    // Interpolation routines
    Real Evaluate( Real fParameter ) const;
    Real Evaluate( Real fParameter, UInt iOrder ) const;

private:
    static const TMatrix2<Real> sm_matBlend;

    UInt m_iBound;
    Real * m_arrValues;

    Real m_fMin, m_fMax;
    Real m_fSpacing, m_fInvSpacing;
};

// Explicit instanciation
typedef TInterpolatorLinear<Float> InterpolatorLinearf;
typedef TInterpolatorLinear<Double> InterpolatorLineard;
typedef TInterpolatorLinear<Scalar> InterpolatorLinear;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "InterpolatorLinear.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_ANALYSIS_INTERPOLATION_INTERPOLATORLINEAR_H
