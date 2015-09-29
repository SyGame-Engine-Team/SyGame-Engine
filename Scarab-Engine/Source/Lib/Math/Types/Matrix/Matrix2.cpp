/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Matrix/Matrix2.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : 2D matrix
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
#include "Matrix2.h"
#include "Matrix3.h"
#include "Matrix4.h"

/////////////////////////////////////////////////////////////////////////////////
// TMatrix2 implementation
template<>
TMatrix2<Float> TMatrix2<Float>::operator*(Float rhs) const
{
    #ifdef MATH_USE_SSE
        TMatrix2<Float> matRes;
        MathSSEFn->Push(rhs, rhs);

        MathSSEFn->Push(m00, m01);
        MathSSEFn->MulF(0, 1);
        MathSSEFn->Pop(matRes.m00, matRes.m01);

        MathSSEFn->Push(m10, m11);
        MathSSEFn->MulF(0, 1);
        MathSSEFn->Pop(matRes.m10, matRes.m11);

        MathSSEFn->Pop(rhs, rhs);
        return matRes;
    #else
        return TMatrix2<Float>( m00 * rhs, m01 * rhs,
                                m10 * rhs, m11 * rhs );
    #endif
}
template<>
TMatrix2<Double> TMatrix2<Double>::operator*(Double rhs) const
{
    #ifdef MATH_USE_SSE
        TMatrix2<Double> matRes;
        MathSSEFn->Push(rhs, rhs);

        MathSSEFn->Push(m00, m01);
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop(matRes.m00, matRes.m01);

        MathSSEFn->Push(m10, m11);
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop(matRes.m10, matRes.m11);

        MathSSEFn->Pop(rhs, rhs);
        return matRes;
    #else
        return TMatrix2<Double>( m00 * rhs, m01 * rhs,
                                 m10 * rhs, m11 * rhs );
    #endif
}

template<>
TMatrix2<Float> TMatrix2<Float>::operator/(Float rhs) const
{
    #ifdef MATH_USE_SSE
        TMatrix2<Float> matRes;
        MathSSEFn->Push(rhs, rhs);

        MathSSEFn->Push(m00, m01);
        MathSSEFn->DivF(0, 1);
        MathSSEFn->Pop(matRes.m00, matRes.m01);

        MathSSEFn->Push(m10, m11);
        MathSSEFn->DivF(0, 1);
        MathSSEFn->Pop(matRes.m10, matRes.m11);

        MathSSEFn->Pop(rhs, rhs);
        return matRes;
    #else
        return TMatrix2<Float>( m00 / rhs, m01 / rhs,
                                m10 / rhs, m11 / rhs );
    #endif
}
template<>
TMatrix2<Double> TMatrix2<Double>::operator/(Double rhs) const
{
    #ifdef MATH_USE_SSE
        TMatrix2<Double> matRes;
        MathSSEFn->Push(rhs, rhs);

        MathSSEFn->Push(m00, m01);
        MathSSEFn->DivD(0, 1);
        MathSSEFn->Pop(matRes.m00, matRes.m01);

        MathSSEFn->Push(m10, m11);
        MathSSEFn->DivD(0, 1);
        MathSSEFn->Pop(matRes.m10, matRes.m11);

        MathSSEFn->Pop(rhs, rhs);
        return matRes;
    #else
        return TMatrix2<Double>( m00 / rhs, m01 / rhs,
                                 m10 / rhs, m11 / rhs );
    #endif
}

template<>
TMatrix2<Float> & TMatrix2<Float>::operator*=(Float rhs)
{
    #ifdef MATH_USE_SSE
        MathSSEFn->Push(rhs, rhs);

        MathSSEFn->Push(m00, m10);
        MathSSEFn->MulF(0, 1);
        MathSSEFn->Pop(m00, m10);

        MathSSEFn->Push(m01, m11);
        MathSSEFn->MulF(0, 1);
        MathSSEFn->Pop(m01, m11);

        MathSSEFn->Pop(rhs, rhs);
    #else
        m00 *= rhs; m01 *= rhs;
        m10 *= rhs; m11 *= rhs;
    #endif
    return (*this);
}
template<>
TMatrix2<Double> & TMatrix2<Double>::operator*=(Double rhs)
{
    #ifdef MATH_USE_SSE
        MathSSEFn->Push(rhs, rhs);

        MathSSEFn->Push(m00, m10);
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop(m00, m10);

        MathSSEFn->Push(m01, m11);
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop(m01, m11);

        MathSSEFn->Pop(rhs, rhs);
    #else
        m00 *= rhs; m01 *= rhs;
        m10 *= rhs; m11 *= rhs;
    #endif
    return (*this);
}

template<>
TMatrix2<Float> & TMatrix2<Float>::operator/=(Float rhs)
{
    #ifdef MATH_USE_SSE
        MathSSEFn->Push(rhs, rhs);

        MathSSEFn->Push(m00, m10);
        MathSSEFn->DivF(0, 1);
        MathSSEFn->Pop(m00, m10);

        MathSSEFn->Push(m01, m11);
        MathSSEFn->DivF(0, 1);
        MathSSEFn->Pop(m01, m11);

        MathSSEFn->Pop(rhs, rhs);
    #else
        m00 /= rhs; m01 /= rhs;
        m10 /= rhs; m11 /= rhs;
    #endif
    return (*this);
}
template<>
TMatrix2<Double> & TMatrix2<Double>::operator/=(Double rhs)
{
    #ifdef MATH_USE_SSE
        MathSSEFn->Push(rhs, rhs);

        MathSSEFn->Push(m00, m10);
        MathSSEFn->DivD(0, 1);
        MathSSEFn->Pop(m00, m10);

        MathSSEFn->Push(m01, m11);
        MathSSEFn->DivD(0, 1);
        MathSSEFn->Pop(m01, m11);

        MathSSEFn->Pop(rhs, rhs);
    #else
        m00 /= rhs; m01 /= rhs;
        m10 /= rhs; m11 /= rhs;
    #endif
    return (*this);
}

template<>
TVertex2<Float> TMatrix2<Float>::operator*(const TVertex2<Float> & rhs) const
{
    #ifdef MATH_USE_SSE
        TVertex2<Float> vRes;

        MathSSEFn->Push( m00, m10 );
        MathSSEFn->Push( rhs.X, rhs.X );
        MathSSEFn->MulPF();

        MathSSEFn->Push( m01, m11 );
        MathSSEFn->Push( rhs.Y, rhs.Y );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();

        MathSSEFn->Pop( vRes.X, vRes.Y );
        return vRes;
    #else
        return TVertex2<Float>( (m00 * rhs.X + m01 * rhs.Y),
                                (m10 * rhs.X + m11 * rhs.Y) );
    #endif
}
template<>
TVertex2<Double> TMatrix2<Double>::operator*(const TVertex2<Double> & rhs) const
{
    #ifdef MATH_USE_SSE
        TVertex2<Double> vRes;

        MathSSEFn->Push( m00, m10 );
        MathSSEFn->Push( rhs.X, rhs.X );
        MathSSEFn->MulPD();

        MathSSEFn->Push( m01, m11 );
        MathSSEFn->Push( rhs.Y, rhs.Y );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();

        MathSSEFn->Pop( vRes.X, vRes.Y );
        return vRes;
    #else
        return TVertex2<Double>( (m00 * rhs.X + m01 * rhs.Y),
                                 (m10 * rhs.X + m11 * rhs.Y) );
    #endif
}

template<>
TVector2<Float> TMatrix2<Float>::operator*(const TVector2<Float> & rhs) const
{
    #ifdef MATH_USE_SSE
        TVector2<Float> vRes;

        MathSSEFn->Push( m00, m10 );
        MathSSEFn->Push( rhs.X, rhs.X );
        MathSSEFn->MulPF();

        MathSSEFn->Push( m01, m11 );
        MathSSEFn->Push( rhs.Y, rhs.Y );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();

        MathSSEFn->Pop( vRes.X, vRes.Y );
        return vRes;
    #else
        return TVector2<Float>( (m00 * rhs.X + m01 * rhs.Y),
                                (m10 * rhs.X + m11 * rhs.Y) );
    #endif
}
template<>
TVector2<Double> TMatrix2<Double>::operator*(const TVector2<Double> & rhs) const
{
    #ifdef MATH_USE_SSE
        TVector2<Double> vRes;

        MathSSEFn->Push( m00, m10 );
        MathSSEFn->Push( rhs.X, rhs.X );
        MathSSEFn->MulPD();

        MathSSEFn->Push( m01, m11 );
        MathSSEFn->Push( rhs.Y, rhs.Y );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();

        MathSSEFn->Pop( vRes.X, vRes.Y );
        return vRes;
    #else
        return TVector2<Double>( (m00 * rhs.X + m01 * rhs.Y),
                                 (m10 * rhs.X + m11 * rhs.Y) );
    #endif
}

template<>
TMatrix2<Float> TMatrix2<Float>::operator+(const TMatrix2<Float> & rhs) const
{
    #ifdef MATH_USE_SSE
        TMatrix2<Float> matRes;

        MathSSEFn->Push( m00, m01 );
        MathSSEFn->Push( rhs.m00, rhs.m01 );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( matRes.m00, matRes.m01 );

        MathSSEFn->Push( m10, m11 );
        MathSSEFn->Push( rhs.m10, rhs.m11 );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( matRes.m10, matRes.m11 );

        return matRes;
    #else
        return TMatrix2<Float>( m00 + rhs.m00, m01 + rhs.m01,
                                m10 + rhs.m10, m11 + rhs.m11 );
    #endif
}
template<>
TMatrix2<Double> TMatrix2<Double>::operator+(const TMatrix2<Double> & rhs) const
{
    #ifdef MATH_USE_SSE
        TMatrix2<Double> matRes;

        MathSSEFn->Push( m00, m01 );
        MathSSEFn->Push( rhs.m00, rhs.m01 );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( matRes.m00, matRes.m01 );

        MathSSEFn->Push( m10, m11 );
        MathSSEFn->Push( rhs.m10, rhs.m11 );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( matRes.m10, matRes.m11 );

        return matRes;
    #else
        return TMatrix2<Double>( m00 + rhs.m00, m01 + rhs.m01,
                                 m10 + rhs.m10, m11 + rhs.m11 );
    #endif
}

template<>
TMatrix2<Float> TMatrix2<Float>::operator-(const TMatrix2<Float> & rhs) const
{
    #ifdef MATH_USE_SSE
        TMatrix2<Float> matRes;

        MathSSEFn->Push( m00, m01 );
        MathSSEFn->Push( rhs.m00, rhs.m01 );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( matRes.m00, matRes.m01 );

        MathSSEFn->Push( m10, m11 );
        MathSSEFn->Push( rhs.m10, rhs.m11 );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( matRes.m10, matRes.m11 );

        return matRes;
    #else
        return TMatrix2<Float>( m00 - rhs.m00, m01 - rhs.m01,
                                m10 - rhs.m10, m11 - rhs.m11 );
    #endif
}
template<>
TMatrix2<Double> TMatrix2<Double>::operator-(const TMatrix2<Double> & rhs) const
{
    #ifdef MATH_USE_SSE
        TMatrix2<Double> matRes;

        MathSSEFn->Push( m00, m01 );
        MathSSEFn->Push( rhs.m00, rhs.m01 );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( matRes.m00, matRes.m01 );

        MathSSEFn->Push( m10, m11 );
        MathSSEFn->Push( rhs.m10, rhs.m11 );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( matRes.m10, matRes.m11 );

        return matRes;
    #else
        return TMatrix2<Double>( m00 - rhs.m00, m01 - rhs.m01,
                                 m10 - rhs.m10, m11 - rhs.m11 );
    #endif
}

template<>
TMatrix2<Float> TMatrix2<Float>::operator*(const TMatrix2<Float> & rhs) const
{
    #ifdef MATH_USE_SSE
        // That's the funny one xD
        TMatrix2<Float> matRes;

        // Row 0
        MathSSEFn->Push( m00, m00 );
        MathSSEFn->Push( rhs.m00, rhs.m01 );
        MathSSEFn->MulPF();
        MathSSEFn->Push( m01, m01 );
        MathSSEFn->Push( rhs.m10, rhs.m11 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Pop( matRes.m00, matRes.m01 );

        // Row 1
        MathSSEFn->Push( m10, m10 );
        MathSSEFn->Push( rhs.m00, rhs.m01 );
        MathSSEFn->MulPF();
        MathSSEFn->Push( m11, m11 );
        MathSSEFn->Push( rhs.m10, rhs.m11 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Pop( matRes.m10, matRes.m11 );

        return matRes;
    #else
	    return TMatrix2<Float> (
            (m00 * rhs.m00 + m01 * rhs.m10),
            (m00 * rhs.m01 + m01 * rhs.m11),

		    (m10 * rhs.m00 + m11 * rhs.m10),
            (m10 * rhs.m01 + m11 * rhs.m11)
        );
    #endif
}
template<>
TMatrix2<Double> TMatrix2<Double>::operator*(const TMatrix2<Double> & rhs) const
{
    #ifdef MATH_USE_SSE
        // That's the even more funny one xD
        TMatrix2<Double> matRes;

        // Row 0
        MathSSEFn->Push( m00, m00 );
        MathSSEFn->Push( rhs.m00, rhs.m01 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m01, m01 );
        MathSSEFn->Push( rhs.m10, rhs.m11 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Pop( matRes.m00, matRes.m01 );

        // Row 1
        MathSSEFn->Push( m10, m10 );
        MathSSEFn->Push( rhs.m00, rhs.m01 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m11, m11 );
        MathSSEFn->Push( rhs.m10, rhs.m11 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Pop( matRes.m10, matRes.m11 );

        return matRes;
    #else
	    return TMatrix2<Double> (
            (m00 * rhs.m00 + m01 * rhs.m10),
            (m00 * rhs.m01 + m01 * rhs.m11),

		    (m10 * rhs.m00 + m11 * rhs.m10),
            (m10 * rhs.m01 + m11 * rhs.m11)
        );
    #endif
}

template<>
TMatrix2<Float> & TMatrix2<Float>::operator+=(const TMatrix2<Float> & rhs)
{
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( m00, m01 );
        MathSSEFn->Push( rhs.m00, rhs.m01 );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( m00, m01 );

        MathSSEFn->Push( m10, m11 );
        MathSSEFn->Push( rhs.m10, rhs.m11 );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( m10, m11 );
    #else
        m00 += rhs.m00; m01 += rhs.m01;
		m10 += rhs.m10; m11 += rhs.m11;
    #endif
    return (*this);
}
template<>
TMatrix2<Double> & TMatrix2<Double>::operator+=(const TMatrix2<Double> & rhs)
{
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( m00, m01 );
        MathSSEFn->Push( rhs.m00, rhs.m01 );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( m00, m01 );

        MathSSEFn->Push( m10, m11 );
        MathSSEFn->Push( rhs.m10, rhs.m11 );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( m10, m11 );
    #else
        m00 += rhs.m00; m01 += rhs.m01;
		m10 += rhs.m10; m11 += rhs.m11;
    #endif
    return (*this);
}

template<>
TMatrix2<Float> & TMatrix2<Float>::operator-=(const TMatrix2<Float> & rhs)
{
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( m00, m01 );
        MathSSEFn->Push( rhs.m00, rhs.m01 );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( m00, m01 );

        MathSSEFn->Push( m10, m11 );
        MathSSEFn->Push( rhs.m10, rhs.m11 );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( m10, m11 );
    #else
        m00 -= rhs.m00; m01 -= rhs.m01;
	    m10 -= rhs.m10; m11 -= rhs.m11;
    #endif
    return (*this);
}
template<>
TMatrix2<Double> & TMatrix2<Double>::operator-=(const TMatrix2<Double> & rhs)
{
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( m00, m01 );
        MathSSEFn->Push( rhs.m00, rhs.m01 );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( m00, m01 );

        MathSSEFn->Push( m10, m11 );
        MathSSEFn->Push( rhs.m10, rhs.m11 );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( m10, m11 );
    #else
        m00 -= rhs.m00; m01 -= rhs.m01;
	    m10 -= rhs.m10; m11 -= rhs.m11;
    #endif
    return (*this);
}

template<>
TMatrix2<Float> & TMatrix2<Float>::operator*=(const TMatrix2<Float> & rhs)
{
    #ifdef MATH_USE_SSE
        // That's the funny one xD

        // Row 0
        MathSSEFn->Push( m00, m00 );
        MathSSEFn->Push( rhs.m00, rhs.m01 );
        MathSSEFn->MulPF();
        MathSSEFn->Push( m01, m01 );
        MathSSEFn->Push( rhs.m10, rhs.m11 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Pop( m00, m01 );

        // Row 1
        MathSSEFn->Push( m10, m10 );
        MathSSEFn->Push( rhs.m00, rhs.m01 );
        MathSSEFn->MulPF();
        MathSSEFn->Push( m11, m11 );
        MathSSEFn->Push( rhs.m10, rhs.m11 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Pop( m10, m11 );
    #else
        Float f0 = (m00 * rhs.m00 + m01 * rhs.m10);
        Float f1 = (m00 * rhs.m01 + m01 * rhs.m11);
        m00 = f0; m01 = f1;

        f0 = (m10 * rhs.m00 + m11 * rhs.m10);
        f1 = (m10 * rhs.m01 + m11 * rhs.m11);
        m10 = f0; m11 = f1;
    #endif
    return (*this);
}
template<>
TMatrix2<Double> & TMatrix2<Double>::operator*=(const TMatrix2<Double> & rhs)
{
    #ifdef MATH_USE_SSE
        // That's the even more funny one xD

        // Row 0
        MathSSEFn->Push( m00, m00 );
        MathSSEFn->Push( rhs.m00, rhs.m01 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m01, m01 );
        MathSSEFn->Push( rhs.m10, rhs.m11 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Pop( m00, m01 );

        // Row 1
        MathSSEFn->Push( m10, m10 );
        MathSSEFn->Push( rhs.m00, rhs.m01 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m11, m11 );
        MathSSEFn->Push( rhs.m10, rhs.m11 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Pop( m10, m11 );
    #else
        Double f0 = (m00 * rhs.m00 + m01 * rhs.m10);
        Double f1 = (m00 * rhs.m01 + m01 * rhs.m11);
        m00 = f0; m01 = f1;

        f0 = (m10 * rhs.m00 + m11 * rhs.m10);
        f1 = (m10 * rhs.m01 + m11 * rhs.m11);
        m10 = f0; m11 = f1;
    #endif
    return (*this);
}

template<>
Bool TMatrix2<Float>::Invert(TMatrix2<Float> & outInvMatrix, Float fZeroTolerance) const
{
    Float fInvDet = ( m00 * m11 - m10 * m01 );
    if ( MathFFn->Abs(fInvDet) < fZeroTolerance )
        return false;
    fInvDet = MathFFn->Invert(fInvDet);

    #ifdef MATH_USE_SSE
        MathSSEFn->Push( fInvDet, fInvDet, fInvDet, fInvDet );
        MathSSEFn->Push( m11, -m01, -m10, m00 );
        MathSSEFn->MulPF();
        MathSSEFn->Pop( outInvMatrix.m00, outInvMatrix.m01, outInvMatrix.m10, outInvMatrix.m11 );
    #else
        outInvMatrix.m00 = +m11 * fInvDet; outInvMatrix.m01 = -m01 * fInvDet;
        outInvMatrix.m10 = -m10 * fInvDet; outInvMatrix.m11 = +m00 * fInvDet;
    #endif

    return true;
}
template<>
Bool TMatrix2<Double>::Invert(TMatrix2<Double> & outInvMatrix, Double fZeroTolerance) const
{
    Double fInvDet = ( m00 * m11 - m10 * m01 );
    if ( MathDFn->Abs(fInvDet) < fZeroTolerance )
        return false;
    fInvDet = MathDFn->Invert(fInvDet);

    #ifdef MATH_USE_SSE
        MathSSEFn->Push( fInvDet, fInvDet );
        MathSSEFn->Push( m11, -m01 );
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop( outInvMatrix.m00, outInvMatrix.m01 );
        MathSSEFn->Push( -m10, m00 );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( outInvMatrix.m10, outInvMatrix.m11 );
    #else
        outInvMatrix.m00 = +m11 * fInvDet; outInvMatrix.m01 = -m01 * fInvDet;
        outInvMatrix.m10 = -m10 * fInvDet; outInvMatrix.m11 = +m00 * fInvDet;
    #endif

    return true;
}

