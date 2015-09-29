/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Matrix/Matrix3.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : 3D matrix
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
// TMatrix3 implementation
template<>
TMatrix3<Float> TMatrix3<Float>::operator*(Float rhs) const
{
    #ifdef MATH_USE_SSE
        TMatrix3<Float> matRes;
        MathSSEFn->Push(rhs, rhs, rhs);

        MathSSEFn->Push(m00, m01, m02);
        MathSSEFn->MulF(0, 1);
        MathSSEFn->Pop(matRes.m00, matRes.m01, matRes.m02);

        MathSSEFn->Push(m10, m11, m12);
        MathSSEFn->MulF(0, 1);
        MathSSEFn->Pop(matRes.m10, matRes.m11, matRes.m12);

        MathSSEFn->Push(m20, m21, m22);
        MathSSEFn->MulF(0, 1);
        MathSSEFn->Pop(matRes.m20, matRes.m21, matRes.m22);

        MathSSEFn->Pop(rhs, rhs, rhs);
        return matRes;
    #else
        return TMatrix3<Float>( m00 * rhs, m01 * rhs, m02 * rhs,
                                m10 * rhs, m11 * rhs, m12 * rhs,
                                m20 * rhs, m21 * rhs, m22 * rhs );
    #endif
}
template<>
TMatrix3<Double> TMatrix3<Double>::operator*(Double rhs) const
{
    #ifdef MATH_USE_SSE
        TMatrix3<Double> matRes;
        MathSSEFn->Push(rhs, rhs);

        MathSSEFn->Push(m00, m01);
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop(matRes.m00, matRes.m01);
        MathSSEFn->Push(m02);
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop(matRes.m02);

        MathSSEFn->Push(m10, m11);
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop(matRes.m10, matRes.m11);
        MathSSEFn->Push(m12);
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop(matRes.m12);

        MathSSEFn->Push(m20, m21);
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop(matRes.m20, matRes.m21);
        MathSSEFn->Push(m22);
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop(matRes.m22);

        MathSSEFn->Pop(rhs, rhs);
        return matRes;
    #else
        return TMatrix3<Double>( m00 * rhs, m01 * rhs, m02 * rhs,
                                 m10 * rhs, m11 * rhs, m12 * rhs,
                                 m20 * rhs, m21 * rhs, m22 * rhs );
    #endif
}

template<>
TMatrix3<Float> TMatrix3<Float>::operator/(Float rhs) const
{
    #ifdef MATH_USE_SSE
        TMatrix3<Float> matRes;
        MathSSEFn->Push(rhs, rhs, rhs);

        MathSSEFn->Push(m00, m01, m02);
        MathSSEFn->DivF(0, 1);
        MathSSEFn->Pop(matRes.m00, matRes.m01, matRes.m02);

        MathSSEFn->Push(m10, m11, m12);
        MathSSEFn->DivF(0, 1);
        MathSSEFn->Pop(matRes.m10, matRes.m11, matRes.m12);

        MathSSEFn->Push(m20, m21, m22);
        MathSSEFn->DivF(0, 1);
        MathSSEFn->Pop(matRes.m20, matRes.m21, matRes.m22);

        MathSSEFn->Pop(rhs, rhs, rhs);
        return matRes;
    #else
        return TMatrix3<Float>( m00 / rhs, m01 / rhs, m02 / rhs,
                                m10 / rhs, m11 / rhs, m12 / rhs,
                                m20 / rhs, m21 / rhs, m22 / rhs );
    #endif
}
template<>
TMatrix3<Double> TMatrix3<Double>::operator/(Double rhs) const
{
    #ifdef MATH_USE_SSE
        TMatrix3<Double> matRes;
        MathSSEFn->Push(rhs, rhs);

        MathSSEFn->Push(m00, m01);
        MathSSEFn->DivD(0, 1);
        MathSSEFn->Pop(matRes.m00, matRes.m01);
        MathSSEFn->Push(m02);
        MathSSEFn->DivD(0, 1);
        MathSSEFn->Pop(matRes.m02);

        MathSSEFn->Push(m10, m11);
        MathSSEFn->DivD(0, 1);
        MathSSEFn->Pop(matRes.m10, matRes.m11);
        MathSSEFn->Push(m12);
        MathSSEFn->DivD(0, 1);
        MathSSEFn->Pop(matRes.m12);

        MathSSEFn->Push(m20, m21);
        MathSSEFn->DivD(0, 1);
        MathSSEFn->Pop(matRes.m20, matRes.m21);
        MathSSEFn->Push(m22);
        MathSSEFn->DivD(0, 1);
        MathSSEFn->Pop(matRes.m22);

        MathSSEFn->Pop(rhs, rhs);
        return matRes;
    #else
        return TMatrix3<Double>( m00 / rhs, m01 / rhs, m02 / rhs,
                                 m10 / rhs, m11 / rhs, m12 / rhs,
                                 m20 / rhs, m21 / rhs, m22 / rhs );
    #endif
}

template<>
TMatrix3<Float> & TMatrix3<Float>::operator*=(Float rhs)
{
    #ifdef MATH_USE_SSE
        MathSSEFn->Push(rhs, rhs, rhs);

        MathSSEFn->Push(m00, m10, m20);
        MathSSEFn->MulF(0, 1);
        MathSSEFn->Pop(m00, m10, m20);

        MathSSEFn->Push(m01, m11, m21);
        MathSSEFn->MulF(0, 1);
        MathSSEFn->Pop(m01, m11, m21);

        MathSSEFn->Push(m02, m12, m22);
        MathSSEFn->MulF(0, 1);
        MathSSEFn->Pop(m02, m12, m22);

        MathSSEFn->Pop(rhs, rhs, rhs);
    #else
        m00 *= rhs; m01 *= rhs; m02 *= rhs;
        m10 *= rhs; m11 *= rhs; m12 *= rhs;
        m20 *= rhs; m21 *= rhs; m22 *= rhs;
    #endif
    return (*this);
}
template<>
TMatrix3<Double> & TMatrix3<Double>::operator*=(Double rhs)
{
    #ifdef MATH_USE_SSE
        MathSSEFn->Push(rhs, rhs);

        MathSSEFn->Push(m00, m10);
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop(m00, m10);
        MathSSEFn->Push(m20);
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop(m20);

        MathSSEFn->Push(m01, m11);
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop(m01, m11);
        MathSSEFn->Push(m21);
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop(m21);

        MathSSEFn->Push(m02, m12);
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop(m02, m12);
        MathSSEFn->Push(m22);
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop(m22);

        MathSSEFn->Pop(rhs, rhs);
    #else
        m00 *= rhs; m01 *= rhs; m02 *= rhs;
        m10 *= rhs; m11 *= rhs; m12 *= rhs;
        m20 *= rhs; m21 *= rhs; m22 *= rhs;
    #endif
    return (*this);
}

template<>
TMatrix3<Float> & TMatrix3<Float>::operator/=(Float rhs)
{
    #ifdef MATH_USE_SSE
        MathSSEFn->Push(rhs, rhs, rhs);

        MathSSEFn->Push(m00, m10, m20);
        MathSSEFn->DivF(0, 1);
        MathSSEFn->Pop(m00, m10, m20);

        MathSSEFn->Push(m01, m11, m21);
        MathSSEFn->DivF(0, 1);
        MathSSEFn->Pop(m01, m11, m21);

        MathSSEFn->Push(m02, m12, m22);
        MathSSEFn->DivF(0, 1);
        MathSSEFn->Pop(m02, m12, m22);

        MathSSEFn->Pop(rhs, rhs, rhs);
    #else
        m00 /= rhs; m01 /= rhs; m02 /= rhs;
        m10 /= rhs; m11 /= rhs; m12 /= rhs;
        m20 /= rhs; m21 /= rhs; m22 /= rhs;
    #endif
    return (*this);
}
template<>
TMatrix3<Double> & TMatrix3<Double>::operator/=(Double rhs)
{
    #ifdef MATH_USE_SSE
        MathSSEFn->Push(rhs, rhs);

        MathSSEFn->Push(m00, m10);
        MathSSEFn->DivD(0, 1);
        MathSSEFn->Pop(m00, m10);
        MathSSEFn->Push(m20);
        MathSSEFn->DivD(0, 1);
        MathSSEFn->Pop(m20);

        MathSSEFn->Push(m01, m11);
        MathSSEFn->DivD(0, 1);
        MathSSEFn->Pop(m01, m11);
        MathSSEFn->Push(m21);
        MathSSEFn->DivD(0, 1);
        MathSSEFn->Pop(m21);

        MathSSEFn->Push(m02, m12);
        MathSSEFn->DivD(0, 1);
        MathSSEFn->Pop(m02, m12);
        MathSSEFn->Push(m22);
        MathSSEFn->DivD(0, 1);
        MathSSEFn->Pop(m22);

        MathSSEFn->Pop(rhs, rhs);
    #else
        m00 /= rhs; m01 /= rhs; m02 /= rhs;
        m10 /= rhs; m11 /= rhs; m12 /= rhs;
        m20 /= rhs; m21 /= rhs; m22 /= rhs;
    #endif
    return (*this);
}

template<>
TVertex3<Float> TMatrix3<Float>::operator*(const TVertex3<Float> & rhs) const
{
    #ifdef MATH_USE_SSE
        TVertex3<Float> vRes;

        MathSSEFn->Push( m00, m10, m20 );
        MathSSEFn->Push( rhs.X, rhs.X, rhs.X );
        MathSSEFn->MulPF();

        MathSSEFn->Push( m01, m11, m21 );
        MathSSEFn->Push( rhs.Y, rhs.Y, rhs.Y );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();

        MathSSEFn->Push( m02, m12, m22 );
        MathSSEFn->Push( rhs.Z, rhs.Z, rhs.Z );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();

        MathSSEFn->Pop( vRes.X, vRes.Y, vRes.Z );
        return vRes;
    #else
        return TVertex3<Float>( (m00 * rhs.X + m01 * rhs.Y + m02 * rhs.Z),
                                (m10 * rhs.X + m11 * rhs.Y + m12 * rhs.Z),
                                (m20 * rhs.X + m21 * rhs.Y + m22 * rhs.Z) );
    #endif
}
template<>
TVertex3<Double> TMatrix3<Double>::operator*(const TVertex3<Double> & rhs) const
{
    #ifdef MATH_USE_SSE
        TVertex3<Double> vRes;

        MathSSEFn->Push( m00, m10 );
        MathSSEFn->Push( rhs.X, rhs.X );
        MathSSEFn->MulPD();

        MathSSEFn->Push( m01, m11 );
        MathSSEFn->Push( rhs.Y, rhs.Y );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();

        MathSSEFn->Push( m02, m12 );
        MathSSEFn->Push( rhs.Z, rhs.Z );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();

        MathSSEFn->Pop( vRes.X, vRes.Y );

        MathSSEFn->Push( m20 );
        MathSSEFn->Push( rhs.X );
        MathSSEFn->MulPD();

        MathSSEFn->Push( m21 );
        MathSSEFn->Push( rhs.Y );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();

        MathSSEFn->Push( m22 );
        MathSSEFn->Push( rhs.Z );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();

        MathSSEFn->Pop( vRes.Z );
        return vRes;
    #else
        return TVertex3<Double>( (m00 * rhs.X + m01 * rhs.Y + m02 * rhs.Z),
                                 (m10 * rhs.X + m11 * rhs.Y + m12 * rhs.Z),
                                 (m20 * rhs.X + m21 * rhs.Y + m22 * rhs.Z) );
    #endif
}

template<>
TVector3<Float> TMatrix3<Float>::operator*(const TVector3<Float> & rhs) const
{
    #ifdef MATH_USE_SSE
        TVector3<Float> vRes;

        MathSSEFn->Push( m00, m10, m20 );
        MathSSEFn->Push( rhs.X, rhs.X, rhs.X );
        MathSSEFn->MulPF();

        MathSSEFn->Push( m01, m11, m21 );
        MathSSEFn->Push( rhs.Y, rhs.Y, rhs.Y );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();

        MathSSEFn->Push( m02, m12, m22 );
        MathSSEFn->Push( rhs.Z, rhs.Z, rhs.Z );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();

        MathSSEFn->Pop( vRes.X, vRes.Y, vRes.Z );
        return vRes;
    #else
        return TVector3<Float>( (m00 * rhs.X + m01 * rhs.Y + m02 * rhs.Z),
                                (m10 * rhs.X + m11 * rhs.Y + m12 * rhs.Z),
                                (m20 * rhs.X + m21 * rhs.Y + m22 * rhs.Z) );
    #endif
}
template<>
TVector3<Double> TMatrix3<Double>::operator*(const TVector3<Double> & rhs) const
{
    #ifdef MATH_USE_SSE
        TVector3<Double> vRes;

        MathSSEFn->Push( m00, m10 );
        MathSSEFn->Push( rhs.X, rhs.X );
        MathSSEFn->MulPD();

        MathSSEFn->Push( m01, m11 );
        MathSSEFn->Push( rhs.Y, rhs.Y );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();

        MathSSEFn->Push( m02, m12 );
        MathSSEFn->Push( rhs.Z, rhs.Z );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();

        MathSSEFn->Pop( vRes.X, vRes.Y );

        MathSSEFn->Push( m20 );
        MathSSEFn->Push( rhs.X );
        MathSSEFn->MulPD();

        MathSSEFn->Push( m21 );
        MathSSEFn->Push( rhs.Y );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();

        MathSSEFn->Push( m22 );
        MathSSEFn->Push( rhs.Z );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();

        MathSSEFn->Pop( vRes.Z );
        return vRes;
    #else
        return TVector3<Double>( (m00 * rhs.X + m01 * rhs.Y + m02 * rhs.Z),
                                 (m10 * rhs.X + m11 * rhs.Y + m12 * rhs.Z),
                                 (m20 * rhs.X + m21 * rhs.Y + m22 * rhs.Z) );
    #endif
}

template<>
TMatrix3<Float> TMatrix3<Float>::operator+(const TMatrix3<Float> & rhs) const
{
    #ifdef MATH_USE_SSE
        TMatrix3<Float> matRes;

        MathSSEFn->Push( m00, m01, m02 );
        MathSSEFn->Push( rhs.m00, rhs.m01, rhs.m02 );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( matRes.m00, matRes.m01, matRes.m02 );

        MathSSEFn->Push( m10, m11, m12 );
        MathSSEFn->Push( rhs.m10, rhs.m11, rhs.m12 );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( matRes.m10, matRes.m11, matRes.m12 );

        MathSSEFn->Push( m20, m21, m22 );
        MathSSEFn->Push( rhs.m20, rhs.m21, rhs.m22 );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( matRes.m20, matRes.m21, matRes.m22 );

        return matRes;
    #else
        return TMatrix3<Float>( m00 + rhs.m00, m01 + rhs.m01, m02 + rhs.m02,
                                m10 + rhs.m10, m11 + rhs.m11, m12 + rhs.m12,
                                m20 + rhs.m20, m21 + rhs.m21, m22 + rhs.m22 );
    #endif
}
template<>
TMatrix3<Double> TMatrix3<Double>::operator+(const TMatrix3<Double> & rhs) const
{
    #ifdef MATH_USE_SSE
        TMatrix3<Double> matRes;

        MathSSEFn->Push( m00, m01 );
        MathSSEFn->Push( rhs.m00, rhs.m01 );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( matRes.m00, matRes.m01 );
        MathSSEFn->Push( m02 );
        MathSSEFn->Push( rhs.m02 );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( matRes.m02 );

        MathSSEFn->Push( m10, m11 );
        MathSSEFn->Push( rhs.m10, rhs.m11 );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( matRes.m10, matRes.m11 );
        MathSSEFn->Push( m12 );
        MathSSEFn->Push( rhs.m12 );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( matRes.m12 );

        MathSSEFn->Push( m20, m21 );
        MathSSEFn->Push( rhs.m20, rhs.m21 );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( matRes.m20, matRes.m21 );
        MathSSEFn->Push( m22 );
        MathSSEFn->Push( rhs.m22 );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( matRes.m22 );

        return matRes;
    #else
        return TMatrix3<Double>( m00 + rhs.m00, m01 + rhs.m01, m02 + rhs.m02,
                                 m10 + rhs.m10, m11 + rhs.m11, m12 + rhs.m12,
                                 m20 + rhs.m20, m21 + rhs.m21, m22 + rhs.m22 );
    #endif
}

template<>
TMatrix3<Float> TMatrix3<Float>::operator-(const TMatrix3<Float> & rhs) const
{
    #ifdef MATH_USE_SSE
        TMatrix3<Float> matRes;

        MathSSEFn->Push( m00, m01, m02 );
        MathSSEFn->Push( rhs.m00, rhs.m01, rhs.m02 );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( matRes.m00, matRes.m01, matRes.m02 );

        MathSSEFn->Push( m10, m11, m12 );
        MathSSEFn->Push( rhs.m10, rhs.m11, rhs.m12 );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( matRes.m10, matRes.m11, matRes.m12 );

        MathSSEFn->Push( m20, m21, m22 );
        MathSSEFn->Push( rhs.m20, rhs.m21, rhs.m22 );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( matRes.m20, matRes.m21, matRes.m22 );

        return matRes;
    #else
        return TMatrix3<Float>( m00 - rhs.m00, m01 - rhs.m01, m02 - rhs.m02,
                                m10 - rhs.m10, m11 - rhs.m11, m12 - rhs.m12,
                                m20 - rhs.m20, m21 - rhs.m21, m22 - rhs.m22 );
    #endif
}
template<>
TMatrix3<Double> TMatrix3<Double>::operator-(const TMatrix3<Double> & rhs) const
{
    #ifdef MATH_USE_SSE
        TMatrix3<Double> matRes;

        MathSSEFn->Push( m00, m01 );
        MathSSEFn->Push( rhs.m00, rhs.m01 );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( matRes.m00, matRes.m01 );
        MathSSEFn->Push( m02 );
        MathSSEFn->Push( rhs.m02 );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( matRes.m02 );

        MathSSEFn->Push( m10, m11 );
        MathSSEFn->Push( rhs.m10, rhs.m11 );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( matRes.m10, matRes.m11 );
        MathSSEFn->Push( m12 );
        MathSSEFn->Push( rhs.m12 );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( matRes.m12 );

        MathSSEFn->Push( m20, m21 );
        MathSSEFn->Push( rhs.m20, rhs.m21 );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( matRes.m20, matRes.m21 );
        MathSSEFn->Push( m22 );
        MathSSEFn->Push( rhs.m22 );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( matRes.m22 );

        return matRes;
    #else
        return TMatrix3<Double>( m00 - rhs.m00, m01 - rhs.m01, m02 - rhs.m02,
                                 m10 - rhs.m10, m11 - rhs.m11, m12 - rhs.m12,
                                 m20 - rhs.m20, m21 - rhs.m21, m22 - rhs.m22 );
    #endif
}

template<>
TMatrix3<Float> TMatrix3<Float>::operator*(const TMatrix3<Float> & rhs) const
{
    #ifdef MATH_USE_SSE
        // That's the funny one xD
        TMatrix3<Float> matRes;

        // Row 0
        MathSSEFn->Push( m00, m00, m00 );
        MathSSEFn->Push( rhs.m00, rhs.m01, rhs.m02 );
        MathSSEFn->MulPF();
        MathSSEFn->Push( m01, m01, m01 );
        MathSSEFn->Push( rhs.m10, rhs.m11, rhs.m12 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Push( m02, m02, m02 );
        MathSSEFn->Push( rhs.m20, rhs.m21, rhs.m22 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Pop( matRes.m00, matRes.m01, matRes.m02 );

        // Row 1
        MathSSEFn->Push( m10, m10, m10 );
        MathSSEFn->Push( rhs.m00, rhs.m01, rhs.m02 );
        MathSSEFn->MulPF();
        MathSSEFn->Push( m11, m11, m11 );
        MathSSEFn->Push( rhs.m10, rhs.m11, rhs.m12 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Push( m12, m12, m12 );
        MathSSEFn->Push( rhs.m20, rhs.m21, rhs.m22 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Pop( matRes.m10, matRes.m11, matRes.m12 );

        // Row 2
        MathSSEFn->Push( m20, m20, m20 );
        MathSSEFn->Push( rhs.m00, rhs.m01, rhs.m02 );
        MathSSEFn->MulPF();
        MathSSEFn->Push( m21, m21, m21 );
        MathSSEFn->Push( rhs.m10, rhs.m11, rhs.m12 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Push( m22, m22, m22 );
        MathSSEFn->Push( rhs.m20, rhs.m21, rhs.m22 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Pop( matRes.m20, matRes.m21, matRes.m22 );

        return matRes;
    #else
	    return TMatrix3<Float> (
            (m00 * rhs.m00 + m01 * rhs.m10 + m02 * rhs.m20),
            (m00 * rhs.m01 + m01 * rhs.m11 + m02 * rhs.m21),
            (m00 * rhs.m02 + m01 * rhs.m12 + m02 * rhs.m22),

		    (m10 * rhs.m00 + m11 * rhs.m10 + m12 * rhs.m20),
            (m10 * rhs.m01 + m11 * rhs.m11 + m12 * rhs.m21),
            (m10 * rhs.m02 + m11 * rhs.m12 + m12 * rhs.m22),

		    (m20 * rhs.m00 + m21 * rhs.m10 + m22 * rhs.m20),
            (m20 * rhs.m01 + m21 * rhs.m11 + m22 * rhs.m21),
            (m20 * rhs.m02 + m21 * rhs.m12 + m22 * rhs.m22)
        );
    #endif
}
template<>
TMatrix3<Double> TMatrix3<Double>::operator*(const TMatrix3<Double> & rhs) const
{
    #ifdef MATH_USE_SSE
        // That's the even more funny one xD
        TMatrix3<Double> matRes;

        // Row 0
        MathSSEFn->Push( m00, m00 );
        MathSSEFn->Push( rhs.m00, rhs.m01 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m01, m01 );
        MathSSEFn->Push( rhs.m10, rhs.m11 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( m02, m02 );
        MathSSEFn->Push( rhs.m20, rhs.m21 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Pop( matRes.m00, matRes.m01 );

        MathSSEFn->Push( m00 );
        MathSSEFn->Push( rhs.m02 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m01 );
        MathSSEFn->Push( rhs.m12 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( m02 );
        MathSSEFn->Push( rhs.m22 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Pop( matRes.m02 );

        // Row 1
        MathSSEFn->Push( m10, m10 );
        MathSSEFn->Push( rhs.m00, rhs.m01 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m11, m11 );
        MathSSEFn->Push( rhs.m10, rhs.m11 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( m12, m12 );
        MathSSEFn->Push( rhs.m20, rhs.m21 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Pop( matRes.m10, matRes.m11 );

        MathSSEFn->Push( m10 );
        MathSSEFn->Push( rhs.m02 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m11 );
        MathSSEFn->Push( rhs.m12 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( m12 );
        MathSSEFn->Push( rhs.m22 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Pop( matRes.m12 );

        // Row 2
        MathSSEFn->Push( m20, m20 );
        MathSSEFn->Push( rhs.m00, rhs.m01 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m21, m21 );
        MathSSEFn->Push( rhs.m10, rhs.m11 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( m22, m22 );
        MathSSEFn->Push( rhs.m20, rhs.m21 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Pop( matRes.m20, matRes.m21 );

        MathSSEFn->Push( m20 );
        MathSSEFn->Push( rhs.m02 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m21 );
        MathSSEFn->Push( rhs.m12 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( m22 );
        MathSSEFn->Push( rhs.m22 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Pop( matRes.m22 );

        return matRes;
    #else
	    return TMatrix3<Double> (
            (m00 * rhs.m00 + m01 * rhs.m10 + m02 * rhs.m20),
            (m00 * rhs.m01 + m01 * rhs.m11 + m02 * rhs.m21),
            (m00 * rhs.m02 + m01 * rhs.m12 + m02 * rhs.m22),

		    (m10 * rhs.m00 + m11 * rhs.m10 + m12 * rhs.m20),
            (m10 * rhs.m01 + m11 * rhs.m11 + m12 * rhs.m21),
            (m10 * rhs.m02 + m11 * rhs.m12 + m12 * rhs.m22),

		    (m20 * rhs.m00 + m21 * rhs.m10 + m22 * rhs.m20),
            (m20 * rhs.m01 + m21 * rhs.m11 + m22 * rhs.m21),
            (m20 * rhs.m02 + m21 * rhs.m12 + m22 * rhs.m22)
        );
    #endif
}

template<>
TMatrix3<Float> & TMatrix3<Float>::operator+=(const TMatrix3<Float> & rhs)
{
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( m00, m01, m02 );
        MathSSEFn->Push( rhs.m00, rhs.m01, rhs.m02 );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( m00, m01, m02 );

        MathSSEFn->Push( m10, m11, m12 );
        MathSSEFn->Push( rhs.m10, rhs.m11, rhs.m12 );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( m10, m11, m12 );

        MathSSEFn->Push( m20, m21, m22 );
        MathSSEFn->Push( rhs.m20, rhs.m21, rhs.m22 );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( m20, m21, m22 );
    #else
        m00 += rhs.m00; m01 += rhs.m01; m02 += rhs.m02;
		m10 += rhs.m10; m11 += rhs.m11; m12 += rhs.m12;
		m20 += rhs.m20; m21 += rhs.m21; m22 += rhs.m22;
    #endif
    return (*this);
}
template<>
TMatrix3<Double> & TMatrix3<Double>::operator+=(const TMatrix3<Double> & rhs)
{
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( m00, m01 );
        MathSSEFn->Push( rhs.m00, rhs.m01 );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( m00, m01 );
        MathSSEFn->Push( m02 );
        MathSSEFn->Push( rhs.m02 );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( m02 );

        MathSSEFn->Push( m10, m11 );
        MathSSEFn->Push( rhs.m10, rhs.m11 );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( m10, m11 );
        MathSSEFn->Push( m12 );
        MathSSEFn->Push( rhs.m12 );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( m12 );

        MathSSEFn->Push( m20, m21 );
        MathSSEFn->Push( rhs.m20, rhs.m21 );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( m20, m21 );
        MathSSEFn->Push( m22 );
        MathSSEFn->Push( rhs.m22 );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( m22 );
    #else
        m00 += rhs.m00; m01 += rhs.m01; m02 += rhs.m02;
		m10 += rhs.m10; m11 += rhs.m11; m12 += rhs.m12;
		m20 += rhs.m20; m21 += rhs.m21; m22 += rhs.m22;
    #endif
    return (*this);
}

template<>
TMatrix3<Float> & TMatrix3<Float>::operator-=(const TMatrix3<Float> & rhs)
{
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( m00, m01, m02 );
        MathSSEFn->Push( rhs.m00, rhs.m01, rhs.m02 );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( m00, m01, m02 );

        MathSSEFn->Push( m10, m11, m12 );
        MathSSEFn->Push( rhs.m10, rhs.m11, rhs.m12 );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( m10, m11, m12 );

        MathSSEFn->Push( m20, m21, m22 );
        MathSSEFn->Push( rhs.m20, rhs.m21, rhs.m22 );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( m20, m21, m22 );
    #else
        m00 -= rhs.m00; m01 -= rhs.m01; m02 -= rhs.m02;
	    m10 -= rhs.m10; m11 -= rhs.m11; m12 -= rhs.m12;
	    m20 -= rhs.m20; m21 -= rhs.m21; m22 -= rhs.m22;
    #endif
    return (*this);
}
template<>
TMatrix3<Double> & TMatrix3<Double>::operator-=(const TMatrix3<Double> & rhs)
{
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( m00, m01 );
        MathSSEFn->Push( rhs.m00, rhs.m01 );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( m00, m01 );
        MathSSEFn->Push( m02 );
        MathSSEFn->Push( rhs.m02 );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( m02 );

        MathSSEFn->Push( m10, m11 );
        MathSSEFn->Push( rhs.m10, rhs.m11 );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( m10, m11 );
        MathSSEFn->Push( m12 );
        MathSSEFn->Push( rhs.m12 );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( m12 );

        MathSSEFn->Push( m20, m21 );
        MathSSEFn->Push( rhs.m20, rhs.m21 );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( m20, m21 );
        MathSSEFn->Push( m22 );
        MathSSEFn->Push( rhs.m22 );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( m22 );
    #else
        m00 -= rhs.m00; m01 -= rhs.m01; m02 -= rhs.m02;
	    m10 -= rhs.m10; m11 -= rhs.m11; m12 -= rhs.m12;
	    m20 -= rhs.m20; m21 -= rhs.m21; m22 -= rhs.m22;
    #endif
    return (*this);
}

template<>
TMatrix3<Float> & TMatrix3<Float>::operator*=(const TMatrix3<Float> & rhs)
{
    #ifdef MATH_USE_SSE
        // That's the funny one xD

        // Row 0
        MathSSEFn->Push( m00, m00, m00 );
        MathSSEFn->Push( rhs.m00, rhs.m01, rhs.m02 );
        MathSSEFn->MulPF();
        MathSSEFn->Push( m01, m01, m01 );
        MathSSEFn->Push( rhs.m10, rhs.m11, rhs.m12 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Push( m02, m02, m02 );
        MathSSEFn->Push( rhs.m20, rhs.m21, rhs.m22 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Pop( m00, m01, m02 );

        // Row 1
        MathSSEFn->Push( m10, m10, m10 );
        MathSSEFn->Push( rhs.m00, rhs.m01, rhs.m02 );
        MathSSEFn->MulPF();
        MathSSEFn->Push( m11, m11, m11 );
        MathSSEFn->Push( rhs.m10, rhs.m11, rhs.m12 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Push( m12, m12, m12 );
        MathSSEFn->Push( rhs.m20, rhs.m21, rhs.m22 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Pop( m10, m11, m12 );

        // Row 2
        MathSSEFn->Push( m20, m20, m20 );
        MathSSEFn->Push( rhs.m00, rhs.m01, rhs.m02 );
        MathSSEFn->MulPF();
        MathSSEFn->Push( m21, m21, m21 );
        MathSSEFn->Push( rhs.m10, rhs.m11, rhs.m12 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Push( m22, m22, m22 );
        MathSSEFn->Push( rhs.m20, rhs.m21, rhs.m22 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Pop( m20, m21, m22 );
    #else
        Float f0 = (m00 * rhs.m00 + m01 * rhs.m10 + m02 * rhs.m20);
        Float f1 = (m00 * rhs.m01 + m01 * rhs.m11 + m02 * rhs.m21);
        Float f2 = (m00 * rhs.m02 + m01 * rhs.m12 + m02 * rhs.m22);
        m00 = f0; m01 = f1; m02 = f2;

        f0 = (m10 * rhs.m00 + m11 * rhs.m10 + m12 * rhs.m20);
        f1 = (m10 * rhs.m01 + m11 * rhs.m11 + m12 * rhs.m21);
        f2 = (m10 * rhs.m02 + m11 * rhs.m12 + m12 * rhs.m22);
        m10 = f0; m11 = f1; m12 = f2;

        f0 = (m20 * rhs.m00 + m21 * rhs.m10 + m22 * rhs.m20);
        f1 = (m20 * rhs.m01 + m21 * rhs.m11 + m22 * rhs.m21);
        f2 = (m20 * rhs.m02 + m21 * rhs.m12 + m22 * rhs.m22);
        m20 = f0; m21 = f1; m22 = f2;
    #endif
    return (*this);
}
template<>
TMatrix3<Double> & TMatrix3<Double>::operator*=(const TMatrix3<Double> & rhs)
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
        MathSSEFn->Push( m02, m02 );
        MathSSEFn->Push( rhs.m20, rhs.m21 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();

        MathSSEFn->Push( m00 );
        MathSSEFn->Push( rhs.m02 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m01 );
        MathSSEFn->Push( rhs.m12 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( m02 );
        MathSSEFn->Push( rhs.m22 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();

        MathSSEFn->Pop( m02 );
        MathSSEFn->Pop( m00, m01 );

        // Row 1
        MathSSEFn->Push( m10, m10 );
        MathSSEFn->Push( rhs.m00, rhs.m01 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m11, m11 );
        MathSSEFn->Push( rhs.m10, rhs.m11 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( m12, m12 );
        MathSSEFn->Push( rhs.m20, rhs.m21 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();

        MathSSEFn->Push( m10 );
        MathSSEFn->Push( rhs.m02 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m11 );
        MathSSEFn->Push( rhs.m12 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( m12 );
        MathSSEFn->Push( rhs.m22 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();

        MathSSEFn->Pop( m12 );
        MathSSEFn->Pop( m10, m11 );

        // Row 2
        MathSSEFn->Push( m20, m20 );
        MathSSEFn->Push( rhs.m00, rhs.m01 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m21, m21 );
        MathSSEFn->Push( rhs.m10, rhs.m11 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( m22, m22 );
        MathSSEFn->Push( rhs.m20, rhs.m21 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();

        MathSSEFn->Push( m20 );
        MathSSEFn->Push( rhs.m02 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m21 );
        MathSSEFn->Push( rhs.m12 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( m22 );
        MathSSEFn->Push( rhs.m22 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();

        MathSSEFn->Pop( m22 );
        MathSSEFn->Pop( m20, m21 );
    #else
        Double f0 = (m00 * rhs.m00 + m01 * rhs.m10 + m02 * rhs.m20);
        Double f1 = (m00 * rhs.m01 + m01 * rhs.m11 + m02 * rhs.m21);
        Double f2 = (m00 * rhs.m02 + m01 * rhs.m12 + m02 * rhs.m22);
        m00 = f0; m01 = f1; m02 = f2;

        f0 = (m10 * rhs.m00 + m11 * rhs.m10 + m12 * rhs.m20);
        f1 = (m10 * rhs.m01 + m11 * rhs.m11 + m12 * rhs.m21);
        f2 = (m10 * rhs.m02 + m11 * rhs.m12 + m12 * rhs.m22);
        m10 = f0; m11 = f1; m12 = f2;

        f0 = (m20 * rhs.m00 + m21 * rhs.m10 + m22 * rhs.m20);
        f1 = (m20 * rhs.m01 + m21 * rhs.m11 + m22 * rhs.m21);
        f2 = (m20 * rhs.m02 + m21 * rhs.m12 + m22 * rhs.m22);
        m20 = f0; m21 = f1; m22 = f2;
    #endif
    return (*this);
}

template<>
Float TMatrix3<Float>::Determinant() const
{
    Float fDet;

    #ifdef MATH_USE_SSE
        MathSSEFn->Push( m11, m10, m10 );
        MathSSEFn->Push( m22, m22, m21 );
        MathSSEFn->MulPF();
        MathSSEFn->Push( m21, m20, m20 );
        MathSSEFn->Push( m12, m12, m11 );
        MathSSEFn->MulPF();
        MathSSEFn->SubPF();
        MathSSEFn->Push( m00, -m01, m02, 0.0f );
        MathSSEFn->MulPF();
        MathSSEFn->HAddF(0);
        MathSSEFn->HAddF(0);
        MathSSEFn->Pop(fDet);
    #else
        Float fC00, fC01, fC02;
        fC00 = m11 * m22 - m21 * m12;
        fC01 = m10 * m22 - m20 * m12;
        fC02 = m10 * m21 - m20 * m11;
        fDet = m00 * fC00 - m01 * fC01 + m02 * fC02;
    #endif

    return fDet;
}
template<>
Double TMatrix3<Double>::Determinant() const
{
    Double fDet;

    #ifdef MATH_USE_SSE
        MathSSEFn->Push( m11, m10 );
        MathSSEFn->Push( m22, m22 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m21, m20 );
        MathSSEFn->Push( m12, m12 );
        MathSSEFn->MulPD();
        MathSSEFn->SubPD();
        MathSSEFn->Push( m00, -m01 );
        MathSSEFn->MulPD();
        MathSSEFn->HAddD(0);
        MathSSEFn->Push( m10 );
        MathSSEFn->Push( m21 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m20 );
        MathSSEFn->Push( m11 );
        MathSSEFn->MulPD();
        MathSSEFn->SubPD();
        MathSSEFn->Push( m02 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Pop(fDet);
    #else
        Double fC00, fC01, fC02;
        fC00 = m11 * m22 - m21 * m12;
        fC01 = m10 * m22 - m20 * m12;
        fC02 = m10 * m21 - m20 * m11;
        fDet = m00 * fC00 - m01 * fC01 + m02 * fC02;
    #endif

    return fDet;
}

template<>
Void TMatrix3<Float>::Adjoint(TMatrix3<Float> & outAdjointMatrix) const
{
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( m11, -m01, m01 );
        MathSSEFn->Push( m22, m22, m12 );
        MathSSEFn->MulPF();
        MathSSEFn->Push( -m21, m21, -m11 );
        MathSSEFn->Push( m12, m02, m02 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Pop( outAdjointMatrix.m00, outAdjointMatrix.m01, outAdjointMatrix.m02 );

        MathSSEFn->Push( -m10, m00, -m00 );
        MathSSEFn->Push( m22, m22, m12 );
        MathSSEFn->MulPF();
        MathSSEFn->Push( m20, -m20, m10 );
        MathSSEFn->Push( m12, m02, m02 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Pop( outAdjointMatrix.m10, outAdjointMatrix.m11, outAdjointMatrix.m12 );

        MathSSEFn->Push( m10, -m00, m00 );
        MathSSEFn->Push( m21, m21, m11 );
        MathSSEFn->MulPF();
        MathSSEFn->Push( -m20, m20, -m10 );
        MathSSEFn->Push( m11, m01, m01 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Pop( outAdjointMatrix.m20, outAdjointMatrix.m21, outAdjointMatrix.m22 );
    #else
        outAdjointMatrix.m00 = +(m11 * m22 - m21 * m12);
        outAdjointMatrix.m01 = -(m01 * m22 - m21 * m02);
        outAdjointMatrix.m02 = +(m01 * m12 - m11 * m02);

        outAdjointMatrix.m10 = -(m10 * m22 - m20 * m12);
        outAdjointMatrix.m11 = +(m00 * m22 - m20 * m02);
        outAdjointMatrix.m12 = -(m00 * m12 - m10 * m02);

        outAdjointMatrix.m20 = +(m10 * m21 - m20 * m11);
        outAdjointMatrix.m21 = -(m00 * m21 - m20 * m01);
        outAdjointMatrix.m22 = +(m00 * m11 - m10 * m01);
    #endif
}
template<>
Void TMatrix3<Double>::Adjoint(TMatrix3<Double> & outAdjointMatrix) const
{
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( m11, -m01 );
        MathSSEFn->Push( m22, m22 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( -m21, m21 );
        MathSSEFn->Push( m12, m02 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Pop( outAdjointMatrix.m00, outAdjointMatrix.m01 );

        MathSSEFn->Push( m01, -m10 );
        MathSSEFn->Push( m12, m22 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( -m11, m20 );
        MathSSEFn->Push( m02, m12 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Pop( outAdjointMatrix.m02, outAdjointMatrix.m10 );

        MathSSEFn->Push( m00, -m00 );
        MathSSEFn->Push( m22, m12 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( -m20, m10 );
        MathSSEFn->Push( m02, m02 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Pop( outAdjointMatrix.m11, outAdjointMatrix.m12 );

        MathSSEFn->Push( m10, -m00 );
        MathSSEFn->Push( m21, m21 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( -m20, m20 );
        MathSSEFn->Push( m11, m01 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Pop( outAdjointMatrix.m20, outAdjointMatrix.m21 );

        MathSSEFn->Push( m00 );
        MathSSEFn->Push( m11 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( -m10 );
        MathSSEFn->Push( m01 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Pop( outAdjointMatrix.m22 );
    #else
        outAdjointMatrix.m00 = +(m11 * m22 - m21 * m12);
        outAdjointMatrix.m01 = -(m01 * m22 - m21 * m02);
        outAdjointMatrix.m02 = +(m01 * m12 - m11 * m02);

        outAdjointMatrix.m10 = -(m10 * m22 - m20 * m12);
        outAdjointMatrix.m11 = +(m00 * m22 - m20 * m02);
        outAdjointMatrix.m12 = -(m00 * m12 - m10 * m02);

        outAdjointMatrix.m20 = +(m10 * m21 - m20 * m11);
        outAdjointMatrix.m21 = -(m00 * m21 - m20 * m01);
        outAdjointMatrix.m22 = +(m00 * m11 - m10 * m01);
    #endif
}

template<>
Bool TMatrix3<Float>::Invert(TMatrix3<Float> & outInvMatrix, Float fZeroTolerance) const
{
    Float fC00, fC01, fC02, fInvDet;

    #ifdef MATH_USE_SSE
        MathSSEFn->Push( m11, m10, m10 );
        MathSSEFn->Push( m22, m22, m21 );
        MathSSEFn->MulPF();
        MathSSEFn->Push( m21, m20, m20 );
        MathSSEFn->Push( m12, m12, m11 );
        MathSSEFn->MulPF();
        MathSSEFn->SubPF();
        MathSSEFn->Get( 0, fC00, fC01, fC02 );
        MathSSEFn->Push( m00, -m01, m02, 0.0f );
        MathSSEFn->MulPF();
        MathSSEFn->HAddF(0);
        MathSSEFn->HAddF(0);
        MathSSEFn->Pop(fInvDet);
    #else
        fC00 = m11 * m22 - m21 * m12;
        fC01 = m10 * m22 - m20 * m12;
        fC02 = m10 * m21 - m20 * m11;
        fInvDet = m00 * fC00 - m01 * fC01 + m02 * fC02;
    #endif

    if ( MathFFn->Abs(fInvDet) < fZeroTolerance )
        return false;
    fInvDet = MathFFn->Invert(fInvDet);

    #ifdef MATH_USE_SSE
        MathSSEFn->Push( fInvDet, fInvDet, fInvDet );

        MathSSEFn->Push( fC00, -fC01, fC02 );
        MathSSEFn->MulF(0, 1);
        MathSSEFn->Pop( outInvMatrix.m00, outInvMatrix.m10, outInvMatrix.m20 );

        MathSSEFn->Push( -m01, m01, m00 );
        MathSSEFn->Push( m22, m12, m22 );
        MathSSEFn->MulPF();
        MathSSEFn->Push( m21, -m11, -m20 );
        MathSSEFn->Push( m02, m02, m02 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->MulF(0, 1);
        MathSSEFn->Pop( outInvMatrix.m01, outInvMatrix.m02, outInvMatrix.m11 );

        MathSSEFn->Push( -m00, -m00, m00 );
        MathSSEFn->Push( m12, m21, m11 );
        MathSSEFn->MulPF();
        MathSSEFn->Push( m10, m20, -m10 );
        MathSSEFn->Push( m02, m01, m01 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->MulF(0, 1);
        MathSSEFn->Pop( outInvMatrix.m12, outInvMatrix.m21, outInvMatrix.m22 );

        MathSSEFn->Pop( fInvDet, fInvDet, fInvDet );
    #else
        outInvMatrix.m00 = +fC00 * fInvDet;
        outInvMatrix.m01 = -(m01 * m22 - m21 * m02) * fInvDet;
        outInvMatrix.m02 = +(m01 * m12 - m11 * m02) * fInvDet;

        outInvMatrix.m10 = -fC01 * fInvDet;
        outInvMatrix.m11 = +(m00 * m22 - m20 * m02) * fInvDet;
        outInvMatrix.m12 = -(m00 * m12 - m10 * m02) * fInvDet;

        outInvMatrix.m20 = +fC02 * fInvDet;
        outInvMatrix.m21 = -(m00 * m21 - m20 * m01) * fInvDet;
        outInvMatrix.m22 = +(m00 * m11 - m10 * m01) * fInvDet;
    #endif

    return true;
}
template<>
Bool TMatrix3<Double>::Invert(TMatrix3<Double> & outInvMatrix, Double fZeroTolerance) const
{
    Double fC00, fC01, fC02, fInvDet;

    #ifdef MATH_USE_SSE
        MathSSEFn->Push( m11, m10 );
        MathSSEFn->Push( m22, m22 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m21, m20 );
        MathSSEFn->Push( m12, m12 );
        MathSSEFn->MulPD();
        MathSSEFn->SubPD();
        MathSSEFn->Get( 0, fC00, fC01 );
        MathSSEFn->Push( m00, -m01 );
        MathSSEFn->MulPD();
        MathSSEFn->HAddD(0);
        MathSSEFn->Push( m10 );
        MathSSEFn->Push( m21 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m20 );
        MathSSEFn->Push( m11 );
        MathSSEFn->MulPD();
        MathSSEFn->SubPD();
        MathSSEFn->Get( 0, fC02 );
        MathSSEFn->Push( m02 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Pop(fInvDet);
    #else
        fC00 = m11 * m22 - m21 * m12;
        fC01 = m10 * m22 - m20 * m12;
        fC02 = m10 * m21 - m20 * m11;
        fInvDet = m00 * fC00 - m01 * fC01 + m02 * fC02;
    #endif

    if ( MathDFn->Abs(fInvDet) < fZeroTolerance )
        return false;
    fInvDet = MathDFn->Invert(fInvDet);

    #ifdef MATH_USE_SSE
        MathSSEFn->Push( fInvDet, fInvDet );

        MathSSEFn->Push( fC00, -fC01 );
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop( outInvMatrix.m00, outInvMatrix.m10 );
        MathSSEFn->Push( fC02 );
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop( outInvMatrix.m20 );

        MathSSEFn->Push( -m01, m01 );
        MathSSEFn->Push( m22, m12 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m21, -m11 );
        MathSSEFn->Push( m02, m02 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop( outInvMatrix.m01, outInvMatrix.m02 );

        MathSSEFn->Push( m00, -m00 );
        MathSSEFn->Push( m22, m12 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( -m20, m10 );
        MathSSEFn->Push( m02, m02 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop( outInvMatrix.m11, outInvMatrix.m12 );

        MathSSEFn->Push( -m00, m00 );
        MathSSEFn->Push( m21, m11 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m20, -m10 );
        MathSSEFn->Push( m01, m01 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop( outInvMatrix.m21, outInvMatrix.m22 );

        MathSSEFn->Pop( fInvDet, fInvDet );
    #else
        outInvMatrix.m00 = +fC00 * fInvDet;
        outInvMatrix.m01 = -(m01 * m22 - m21 * m02) * fInvDet;
        outInvMatrix.m02 = +(m01 * m12 - m11 * m02) * fInvDet;

        outInvMatrix.m10 = -fC01 * fInvDet;
        outInvMatrix.m11 = +(m00 * m22 - m20 * m02) * fInvDet;
        outInvMatrix.m12 = -(m00 * m12 - m10 * m02) * fInvDet;

        outInvMatrix.m20 = +fC02 * fInvDet;
        outInvMatrix.m21 = -(m00 * m21 - m20 * m01) * fInvDet;
        outInvMatrix.m22 = +(m00 * m11 - m10 * m01) * fInvDet;
    #endif

    return true;
}
