/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Analysis/Signal/Transforms.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Signal analysis : Various transforms for signal analysis
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : Implementation reported, not enough time avail right now :s ...
//              This is going to be a lot of work to provide an efficient and
//              generic implementation, similar to http://www.fftw.org/ ...
//              We'll need 2/Split-radix methods, Radel & Bluestein, and a
//              generic composite version or a decomposition-radix pattern.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_LIB_MATH_ANALYSIS_SIGNAL_TRANSFORMS_H
#define SCARAB_LIB_MATH_ANALYSIS_SIGNAL_TRANSFORMS_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Memory/MemoryManager.h"

#include "../../Types/Complex/Complex.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TSignalTransform class
template <typename Real>
class TSignalTransform
{
public:
    TSignalTransform( const TComplex<Real> * arrSignal, UInt iSampleCount );
    TSignalTransform( const Real * arrSignal, UInt iSampleCount );
    ~TSignalTransform();

    inline Bool IsComplexSignal() const;
    inline Bool IsRealSignal() const;
    inline const Void * GetSignal() const;

    inline UInt GetSampleCount() const;
    inline Void GetSignalSample( TComplex<Real> * outValue, UInt iSample ) const;
    inline Void GetSignalSample( Real * outValue, UInt iSample ) const;

    // Fourrier transforms
    Void DFT( TComplex<Real> * arrOutputSignal );
    Void InvDFT( TComplex<Real> * arrOutputSignal );

    Void DFT( Real * arrOutputSignal );
    Void InvDFT( Real * arrOutputSignal );

    // DCT/DST/DHT

    // Laplace transforms

    // Wavelet transforms

protected:
    // Fourrier transforms
        // Fallback reference algorithm ( O(n^2) )
    Void _DFT_Reference( TComplex<Real> * arrOutput, Bool bInvert );
    Void _DFT_Reference_R( Real * arrOutput, Bool bInvert );

        // Base cases : N <= 4
    inline Void _FFT_BaseCase_1_R( Real * arrOutput, UInt iStride );
    inline Void _FFT_BaseCase_2_R( Real * arrOutput, UInt iStride );
    inline Void _FFT_BaseCase_3_R( Real * arrOutput, UInt iStride );
    inline Void _FFT_BaseCase_4_R( Real * arrOutput, UInt iStride );

    inline Void _FFT_BaseCase_N_R( Real * arrOutput, UInt iN, UInt iStride = 1 );

    typedef Void (*_FFT_BaseCase_R)( Real *, UInt );
    static _FFT_BaseCase_R sm_arrBaseCases_R[4];

        // Twiddle factors ( exp(-i*2*pi * k/N) )
    Void _FFT_GenerateTwiddleFactors_R( UInt iN );

    Real ** m_arrTwiddleFactors_R;


        // N is power of 2 case
    Void _FFT_Radix2_R( Real * arrOutput, const Real * arrInput, UInt iN, UInt iStride = 1 ); 
    //Void _FFT_Radix2_C( TComplex<Real> * arrOutput, const TComplex<Real> * arrInput, UInt iN ); 

        // N divisible by 4
    Void _FFT_SplitRadix_R();

        // N is composite case

        // N is prime case
    Void _FFT_Rader_R();
    Void _FFT_BlueStein_R();

    // Signal data
    UInt m_iSampleCount;
    TComplex<Real> * m_arrBaseSignal;
    Real * m_arrBaseSignal_R;
};

// Explicit instanciation
typedef TSignalTransform<Float> SignalTransformf;
typedef TSignalTransform<Double> SignalTransformd;
typedef TSignalTransform<Scalar> SignalTransform;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Transforms.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_ANALYSIS_SIGNAL_TRANSFORMS_H


