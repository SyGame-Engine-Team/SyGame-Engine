/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Analysis/Interpolation/InterpolatorBilinear.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Bilinear Interpolation for 2D data
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
#ifndef SCARAB_LIB_MATH_ANALYSIS_INTERPOLATION_INTERPOLATORBILINEAR_H
#define SCARAB_LIB_MATH_ANALYSIS_INTERPOLATION_INTERPOLATORBILINEAR_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Error/ErrorManager.h"

#include "../../Types/Matrix/Matrix2.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TInterpolatorBilinear class
template<typename Real>
class TInterpolatorBilinear
{
public:
    // Values are arranged in Y-major style : f(x,y) = arrValues[ (y * iBoundX) + x ]
    TInterpolatorBilinear( Real * arrValues, UInt iBoundX, UInt iBoundY,
                           const TVector2<Real> & vMin, const TVector2<Real> & vSpacing );
    ~TInterpolatorBilinear();

    // Function access
    inline UInt GetBoundX() const;
    inline UInt GetBoundY() const;
    inline Real * GetValues() const;

    // Domain access
    inline const TVector2<Real> & GetMin() const;
    inline const TVector2<Real> & GetMax() const;
    inline const TVector2<Real> & GetSpacing() const;

    // Interpolation routines
    Real Evaluate( const TVector2<Real> & vParameter ) const;
    Real Evaluate( const TVector2<Real> & vParameter, UInt iOrderX, UInt iOrderY ) const;

private:
    static const TMatrix2<Real> sm_matBlend;

    UInt m_iBoundX, m_iBoundY;
    Real * m_arrValues;

    TVector2<Real> m_vMin, m_vMax;
    TVector2<Real> m_vSpacing, m_vInvSpacing;
};

// Explicit instanciation
typedef TInterpolatorBilinear<Float> InterpolatorBilinearf;
typedef TInterpolatorBilinear<Double> InterpolatorBilineard;
typedef TInterpolatorBilinear<Scalar> InterpolatorBilinear;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "InterpolatorBilinear.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_ANALYSIS_INTERPOLATION_INTERPOLATORBILINEAR_H
