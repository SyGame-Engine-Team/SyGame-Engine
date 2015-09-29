/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Matrix/Matrix4.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Homogeneous 4D matrix
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
// TMatrix4 implementation
template<>
TMatrix4<Float> TMatrix4<Float>::operator*(Float rhs) const
{
    #ifdef MATH_USE_SSE
        TMatrix4<Float> matRes;
        MathSSEFn->Push(rhs, rhs, rhs, rhs);

        MathSSEFn->Push(m00, m01, m02, m03);
        MathSSEFn->MulF(0, 1);
        MathSSEFn->Pop(matRes.m00, matRes.m01, matRes.m02, matRes.m03);

        MathSSEFn->Push(m10, m11, m12, m13);
        MathSSEFn->MulF(0, 1);
        MathSSEFn->Pop(matRes.m10, matRes.m11, matRes.m12, matRes.m13);

        MathSSEFn->Push(m20, m21, m22, m23);
        MathSSEFn->MulF(0, 1);
        MathSSEFn->Pop(matRes.m20, matRes.m21, matRes.m22, matRes.m23);

        MathSSEFn->Push(m30, m31, m32, m33);
        MathSSEFn->MulF(0, 1);
        MathSSEFn->Pop(matRes.m30, matRes.m31, matRes.m32, matRes.m33);

        MathSSEFn->Pop(rhs, rhs, rhs, rhs);
        return matRes;
    #else
        return TMatrix4<Float>( m00 * rhs, m01 * rhs, m02 * rhs, m03 * rhs,
                                m10 * rhs, m11 * rhs, m12 * rhs, m13 * rhs,
                                m20 * rhs, m21 * rhs, m22 * rhs, m23 * rhs,
                                m30 * rhs, m31 * rhs, m32 * rhs, m33 * rhs );
    #endif
}
template<>
TMatrix4<Double> TMatrix4<Double>::operator*(Double rhs) const
{
    #ifdef MATH_USE_SSE
        TMatrix4<Double> matRes;
        MathSSEFn->Push(rhs, rhs);

        MathSSEFn->Push(m00, m01);
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop(matRes.m00, matRes.m01);
        MathSSEFn->Push(m02, m03);
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop(matRes.m02, matRes.m03);

        MathSSEFn->Push(m10, m11);
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop(matRes.m10, matRes.m11);
        MathSSEFn->Push(m12, m13);
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop(matRes.m12, matRes.m13);

        MathSSEFn->Push(m20, m21);
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop(matRes.m20, matRes.m21);
        MathSSEFn->Push(m22, m23);
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop(matRes.m22, matRes.m23);

        MathSSEFn->Push(m30, m31);
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop(matRes.m30, matRes.m31);
        MathSSEFn->Push(m32, m33);
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop(matRes.m32, matRes.m33);

        MathSSEFn->Pop(rhs, rhs);
        return matRes;
    #else
        return TMatrix4<Double>( m00 * rhs, m01 * rhs, m02 * rhs, m03 * rhs,
                                 m10 * rhs, m11 * rhs, m12 * rhs, m13 * rhs,
                                 m20 * rhs, m21 * rhs, m22 * rhs, m23 * rhs,
                                 m30 * rhs, m31 * rhs, m32 * rhs, m33 * rhs );
    #endif
}

template<>
TMatrix4<Float> TMatrix4<Float>::operator/(Float rhs) const
{
    #ifdef MATH_USE_SSE
        TMatrix4<Float> matRes;
        MathSSEFn->Push(rhs, rhs, rhs, rhs);

        MathSSEFn->Push(m00, m01, m02, m03);
        MathSSEFn->DivF(0, 1);
        MathSSEFn->Pop(matRes.m00, matRes.m01, matRes.m02, matRes.m03);

        MathSSEFn->Push(m10, m11, m12, m13);
        MathSSEFn->DivF(0, 1);
        MathSSEFn->Pop(matRes.m10, matRes.m11, matRes.m12, matRes.m13);

        MathSSEFn->Push(m20, m21, m22, m23);
        MathSSEFn->DivF(0, 1);
        MathSSEFn->Pop(matRes.m20, matRes.m21, matRes.m22, matRes.m23);

        MathSSEFn->Push(m30, m31, m32, m33);
        MathSSEFn->DivF(0, 1);
        MathSSEFn->Pop(matRes.m30, matRes.m31, matRes.m32, matRes.m33);

        MathSSEFn->Pop(rhs, rhs, rhs, rhs);
        return matRes;
    #else
        return TMatrix4<Float>( m00 / rhs, m01 / rhs, m02 / rhs, m03 / rhs,
                                m10 / rhs, m11 / rhs, m12 / rhs, m13 / rhs,
                                m20 / rhs, m21 / rhs, m22 / rhs, m23 / rhs,
                                m30 / rhs, m31 / rhs, m32 / rhs, m33 / rhs );
    #endif
}
template<>
TMatrix4<Double> TMatrix4<Double>::operator/(Double rhs) const
{
    #ifdef MATH_USE_SSE
        TMatrix4<Double> matRes;
        MathSSEFn->Push(rhs, rhs);

        MathSSEFn->Push(m00, m01);
        MathSSEFn->DivD(0, 1);
        MathSSEFn->Pop(matRes.m00, matRes.m01);
        MathSSEFn->Push(m02, m03);
        MathSSEFn->DivD(0, 1);
        MathSSEFn->Pop(matRes.m02, matRes.m03);

        MathSSEFn->Push(m10, m11);
        MathSSEFn->DivD(0, 1);
        MathSSEFn->Pop(matRes.m10, matRes.m11);
        MathSSEFn->Push(m12, m13);
        MathSSEFn->DivD(0, 1);
        MathSSEFn->Pop(matRes.m12, matRes.m13);

        MathSSEFn->Push(m20, m21);
        MathSSEFn->DivD(0, 1);
        MathSSEFn->Pop(matRes.m20, matRes.m21);
        MathSSEFn->Push(m22, m23);
        MathSSEFn->DivD(0, 1);
        MathSSEFn->Pop(matRes.m22, matRes.m23);

        MathSSEFn->Push(m30, m31);
        MathSSEFn->DivD(0, 1);
        MathSSEFn->Pop(matRes.m30, matRes.m31);
        MathSSEFn->Push(m32, m33);
        MathSSEFn->DivD(0, 1);
        MathSSEFn->Pop(matRes.m32, matRes.m33);

        MathSSEFn->Pop(rhs, rhs);
        return matRes;
    #else
        return TMatrix4<Double>( m00 / rhs, m01 / rhs, m02 / rhs, m03 / rhs,
                                 m10 / rhs, m11 / rhs, m12 / rhs, m13 / rhs,
                                 m20 / rhs, m21 / rhs, m22 / rhs, m23 / rhs,
                                 m30 / rhs, m31 / rhs, m32 / rhs, m33 / rhs );
    #endif
}

template<>
TMatrix4<Float> & TMatrix4<Float>::operator*=(Float rhs)
{
    #ifdef MATH_USE_SSE
        MathSSEFn->Push(rhs, rhs, rhs, rhs);

        MathSSEFn->Push(m00, m10, m20, m30);
        MathSSEFn->MulF(0, 1);
        MathSSEFn->Pop(m00, m10, m20, m30);

        MathSSEFn->Push(m01, m11, m21, m31);
        MathSSEFn->MulF(0, 1);
        MathSSEFn->Pop(m01, m11, m21, m31);

        MathSSEFn->Push(m02, m12, m22, m32);
        MathSSEFn->MulF(0, 1);
        MathSSEFn->Pop(m02, m12, m22, m32);

        MathSSEFn->Push(m03, m13, m23, m33);
        MathSSEFn->MulF(0, 1);
        MathSSEFn->Pop(m03, m13, m23, m33);

        MathSSEFn->Pop(rhs, rhs, rhs, rhs);
    #else
        m00 *= rhs; m01 *= rhs; m02 *= rhs; m03 *= rhs;
        m10 *= rhs; m11 *= rhs; m12 *= rhs; m13 *= rhs;
        m20 *= rhs; m21 *= rhs; m22 *= rhs; m23 *= rhs;
        m30 *= rhs; m31 *= rhs; m32 *= rhs; m33 *= rhs;
    #endif
    return (*this);
}
template<>
TMatrix4<Double> & TMatrix4<Double>::operator*=(Double rhs)
{
    #ifdef MATH_USE_SSE
        MathSSEFn->Push(rhs, rhs);

        MathSSEFn->Push(m00, m10);
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop(m00, m10);
        MathSSEFn->Push(m20, m30);
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop(m20, m30);

        MathSSEFn->Push(m01, m11);
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop(m01, m11);
        MathSSEFn->Push(m21, m31);
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop(m21, m31);

        MathSSEFn->Push(m02, m12);
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop(m02, m12);
        MathSSEFn->Push(m22, m32);
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop(m22, m32);

        MathSSEFn->Push(m03, m13);
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop(m03, m13);
        MathSSEFn->Push(m23, m33);
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop(m23, m33);

        MathSSEFn->Pop(rhs, rhs);
    #else
        m00 *= rhs; m01 *= rhs; m02 *= rhs; m03 *= rhs;
        m10 *= rhs; m11 *= rhs; m12 *= rhs; m13 *= rhs;
        m20 *= rhs; m21 *= rhs; m22 *= rhs; m23 *= rhs;
        m30 *= rhs; m31 *= rhs; m32 *= rhs; m33 *= rhs;
    #endif
    return (*this);
}

template<>
TMatrix4<Float> & TMatrix4<Float>::operator/=(Float rhs)
{
    #ifdef MATH_USE_SSE
        MathSSEFn->Push(rhs, rhs, rhs, rhs);

        MathSSEFn->Push(m00, m10, m20, m30);
        MathSSEFn->DivF(0, 1);
        MathSSEFn->Pop(m00, m10, m20, m30);

        MathSSEFn->Push(m01, m11, m21, m31);
        MathSSEFn->DivF(0, 1);
        MathSSEFn->Pop(m01, m11, m21, m31);

        MathSSEFn->Push(m02, m12, m22, m32);
        MathSSEFn->DivF(0, 1);
        MathSSEFn->Pop(m02, m12, m22, m32);

        MathSSEFn->Push(m03, m13, m23, m33);
        MathSSEFn->DivF(0, 1);
        MathSSEFn->Pop(m03, m13, m23, m33);

        MathSSEFn->Pop(rhs, rhs, rhs, rhs);
    #else
        m00 /= rhs; m01 /= rhs; m02 /= rhs; m03 /= rhs;
        m10 /= rhs; m11 /= rhs; m12 /= rhs; m13 /= rhs;
        m20 /= rhs; m21 /= rhs; m22 /= rhs; m23 /= rhs;
        m30 /= rhs; m31 /= rhs; m32 /= rhs; m33 /= rhs;
    #endif
    return (*this);
}
template<>
TMatrix4<Double> & TMatrix4<Double>::operator/=(Double rhs)
{
    #ifdef MATH_USE_SSE
        MathSSEFn->Push(rhs, rhs);

        MathSSEFn->Push(m00, m10);
        MathSSEFn->DivD(0, 1);
        MathSSEFn->Pop(m00, m10);
        MathSSEFn->Push(m20, m30);
        MathSSEFn->DivD(0, 1);
        MathSSEFn->Pop(m20, m30);

        MathSSEFn->Push(m01, m11);
        MathSSEFn->DivD(0, 1);
        MathSSEFn->Pop(m01, m11);
        MathSSEFn->Push(m21, m31);
        MathSSEFn->DivD(0, 1);
        MathSSEFn->Pop(m21, m31);

        MathSSEFn->Push(m02, m12);
        MathSSEFn->DivD(0, 1);
        MathSSEFn->Pop(m02, m12);
        MathSSEFn->Push(m22, m32);
        MathSSEFn->DivD(0, 1);
        MathSSEFn->Pop(m22, m32);

        MathSSEFn->Push(m03, m13);
        MathSSEFn->DivD(0, 1);
        MathSSEFn->Pop(m03, m13);
        MathSSEFn->Push(m23, m33);
        MathSSEFn->DivD(0, 1);
        MathSSEFn->Pop(m23, m33);

        MathSSEFn->Pop(rhs, rhs);
    #else
        m00 /= rhs; m01 /= rhs; m02 /= rhs; m03 /= rhs;
        m10 /= rhs; m11 /= rhs; m12 /= rhs; m13 /= rhs;
        m20 /= rhs; m21 /= rhs; m22 /= rhs; m23 /= rhs;
        m30 /= rhs; m31 /= rhs; m32 /= rhs; m33 /= rhs;
    #endif
    return (*this);
}

template<>
TVertex4<Float> TMatrix4<Float>::operator*(const TVertex4<Float> & rhs) const
{
    #ifdef MATH_USE_SSE
        TVertex4<Float> vRes;

        MathSSEFn->Push( m00, m10, m20, m30 );
        MathSSEFn->Push( rhs.X, rhs.X, rhs.X, rhs.X );
        MathSSEFn->MulPF();

        MathSSEFn->Push( m01, m11, m21, m31 );
        MathSSEFn->Push( rhs.Y, rhs.Y, rhs.Y, rhs.Y );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();

        MathSSEFn->Push( m02, m12, m22, m32 );
        MathSSEFn->Push( rhs.Z, rhs.Z, rhs.Z, rhs.Z );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();

        MathSSEFn->Push( m03, m13, m23, m33 );
        MathSSEFn->Push( rhs.W, rhs.W, rhs.W, rhs.W );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();

        MathSSEFn->Pop( vRes.X, vRes.Y, vRes.Z, vRes.W );
        return vRes;
    #else
        return TVertex4<Float>( (m00 * rhs.X + m01 * rhs.Y + m02 * rhs.Z + m03 * rhs.W),
                                (m10 * rhs.X + m11 * rhs.Y + m12 * rhs.Z + m13 * rhs.W),
                                (m20 * rhs.X + m21 * rhs.Y + m22 * rhs.Z + m23 * rhs.W),
                                (m30 * rhs.X + m31 * rhs.Y + m32 * rhs.Z + m33 * rhs.W) );
    #endif
}
template<>
TVertex4<Double> TMatrix4<Double>::operator*(const TVertex4<Double> & rhs) const
{
    #ifdef MATH_USE_SSE
        TVertex4<Double> vRes;

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

        MathSSEFn->Push( m03, m13 );
        MathSSEFn->Push( rhs.W, rhs.W );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();

        MathSSEFn->Pop( vRes.X, vRes.Y );

        MathSSEFn->Push( m20, m30 );
        MathSSEFn->Push( rhs.X, rhs.X );
        MathSSEFn->MulPD();

        MathSSEFn->Push( m21, m31 );
        MathSSEFn->Push( rhs.Y, rhs.Y );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();

        MathSSEFn->Push( m22, m32 );
        MathSSEFn->Push( rhs.Z, rhs.Z );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();

        MathSSEFn->Push( m23, m33 );
        MathSSEFn->Push( rhs.W, rhs.W );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();

        MathSSEFn->Pop( vRes.Z, vRes.W );
        return vRes;
    #else
        return TVertex4<Double>( (m00 * rhs.X + m01 * rhs.Y + m02 * rhs.Z + m03 * rhs.W),
                                 (m10 * rhs.X + m11 * rhs.Y + m12 * rhs.Z + m13 * rhs.W),
                                 (m20 * rhs.X + m21 * rhs.Y + m22 * rhs.Z + m23 * rhs.W),
                                 (m30 * rhs.X + m31 * rhs.Y + m32 * rhs.Z + m33 * rhs.W) );
    #endif
}

template<>
TVector4<Float> TMatrix4<Float>::operator*(const TVector4<Float> & rhs) const
{
    #ifdef MATH_USE_SSE
        TVector4<Float> vRes;

        MathSSEFn->Push( m00, m10, m20, m30 );
        MathSSEFn->Push( rhs.X, rhs.X, rhs.X, rhs.X );
        MathSSEFn->MulPF();

        MathSSEFn->Push( m01, m11, m21, m31 );
        MathSSEFn->Push( rhs.Y, rhs.Y, rhs.Y, rhs.Y );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();

        MathSSEFn->Push( m02, m12, m22, m32 );
        MathSSEFn->Push( rhs.Z, rhs.Z, rhs.Z, rhs.Z );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();

        MathSSEFn->Push( m03, m13, m23, m33 );
        MathSSEFn->Push( rhs.W, rhs.W, rhs.W, rhs.W );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();

        MathSSEFn->Pop( vRes.X, vRes.Y, vRes.Z, vRes.W );
        return vRes;
    #else
        return TVector4<Float>( (m00 * rhs.X + m01 * rhs.Y + m02 * rhs.Z + m03 * rhs.W),
                                (m10 * rhs.X + m11 * rhs.Y + m12 * rhs.Z + m13 * rhs.W),
                                (m20 * rhs.X + m21 * rhs.Y + m22 * rhs.Z + m23 * rhs.W),
                                (m30 * rhs.X + m31 * rhs.Y + m32 * rhs.Z + m33 * rhs.W) );
    #endif
}
template<>
TVector4<Double> TMatrix4<Double>::operator*(const TVector4<Double> & rhs) const
{
    #ifdef MATH_USE_SSE
        TVector4<Double> vRes;

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

        MathSSEFn->Push( m03, m13 );
        MathSSEFn->Push( rhs.W, rhs.W );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();

        MathSSEFn->Pop( vRes.X, vRes.Y );

        MathSSEFn->Push( m20, m30 );
        MathSSEFn->Push( rhs.X, rhs.X );
        MathSSEFn->MulPD();

        MathSSEFn->Push( m21, m31 );
        MathSSEFn->Push( rhs.Y, rhs.Y );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();

        MathSSEFn->Push( m22, m32 );
        MathSSEFn->Push( rhs.Z, rhs.Z );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();

        MathSSEFn->Push( m23, m33 );
        MathSSEFn->Push( rhs.W, rhs.W );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();

        MathSSEFn->Pop( vRes.Z, vRes.W );
        return vRes;
    #else
        return TVector4<Double>( (m00 * rhs.X + m01 * rhs.Y + m02 * rhs.Z + m03 * rhs.W),
                                 (m10 * rhs.X + m11 * rhs.Y + m12 * rhs.Z + m13 * rhs.W),
                                 (m20 * rhs.X + m21 * rhs.Y + m22 * rhs.Z + m23 * rhs.W),
                                 (m30 * rhs.X + m31 * rhs.Y + m32 * rhs.Z + m33 * rhs.W) );
    #endif
}

template<>
TMatrix4<Float> TMatrix4<Float>::operator+(const TMatrix4<Float> & rhs) const
{
    #ifdef MATH_USE_SSE
        TMatrix4<Float> matRes;

        MathSSEFn->Push( m00, m01, m02, m03 );
        MathSSEFn->Push( rhs.m00, rhs.m01, rhs.m02, rhs.m03 );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( matRes.m00, matRes.m01, matRes.m02, matRes.m03 );

        MathSSEFn->Push( m10, m11, m12, m13 );
        MathSSEFn->Push( rhs.m10, rhs.m11, rhs.m12, rhs.m13 );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( matRes.m10, matRes.m11, matRes.m12, matRes.m13 );

        MathSSEFn->Push( m20, m21, m22, m23 );
        MathSSEFn->Push( rhs.m20, rhs.m21, rhs.m22, rhs.m23 );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( matRes.m20, matRes.m21, matRes.m22, matRes.m23 );

        MathSSEFn->Push( m30, m31, m32, m33 );
        MathSSEFn->Push( rhs.m30, rhs.m31, rhs.m32, rhs.m33 );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( matRes.m30, matRes.m31, matRes.m32, matRes.m33 );

        return matRes;
    #else
        return TMatrix4<Float>( m00 + rhs.m00, m01 + rhs.m01, m02 + rhs.m02, m03 + rhs.m03,
                                m10 + rhs.m10, m11 + rhs.m11, m12 + rhs.m12, m13 + rhs.m13,
                                m20 + rhs.m20, m21 + rhs.m21, m22 + rhs.m22, m23 + rhs.m23,
                                m30 + rhs.m30, m31 + rhs.m31, m32 + rhs.m32, m33 + rhs.m33 );
    #endif
}
template<>
TMatrix4<Double> TMatrix4<Double>::operator+(const TMatrix4<Double> & rhs) const
{
    #ifdef MATH_USE_SSE
        TMatrix4<Double> matRes;

        MathSSEFn->Push( m00, m01 );
        MathSSEFn->Push( rhs.m00, rhs.m01 );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( matRes.m00, matRes.m01 );
        MathSSEFn->Push( m02, m03 );
        MathSSEFn->Push( rhs.m02, rhs.m03 );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( matRes.m02, matRes.m03 );

        MathSSEFn->Push( m10, m11 );
        MathSSEFn->Push( rhs.m10, rhs.m11 );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( matRes.m10, matRes.m11 );
        MathSSEFn->Push( m12, m13 );
        MathSSEFn->Push( rhs.m12, rhs.m13 );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( matRes.m12, matRes.m13 );

        MathSSEFn->Push( m20, m21 );
        MathSSEFn->Push( rhs.m20, rhs.m21 );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( matRes.m20, matRes.m21 );
        MathSSEFn->Push( m22, m23 );
        MathSSEFn->Push( rhs.m22, rhs.m23 );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( matRes.m22, matRes.m23 );

        MathSSEFn->Push( m30, m31 );
        MathSSEFn->Push( rhs.m30, rhs.m31 );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( matRes.m30, matRes.m31 );
        MathSSEFn->Push( m32, m33 );
        MathSSEFn->Push( rhs.m32, rhs.m33 );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( matRes.m32, matRes.m33 );

        return matRes;
    #else
        return TMatrix4<Double>( m00 + rhs.m00, m01 + rhs.m01, m02 + rhs.m02, m03 + rhs.m03,
                                 m10 + rhs.m10, m11 + rhs.m11, m12 + rhs.m12, m13 + rhs.m13,
                                 m20 + rhs.m20, m21 + rhs.m21, m22 + rhs.m22, m23 + rhs.m23,
                                 m30 + rhs.m30, m31 + rhs.m31, m32 + rhs.m32, m33 + rhs.m33 );
    #endif
}

template<>
TMatrix4<Float> TMatrix4<Float>::operator-(const TMatrix4<Float> & rhs) const
{
    #ifdef MATH_USE_SSE
        TMatrix4<Float> matRes;

        MathSSEFn->Push( m00, m01, m02, m03 );
        MathSSEFn->Push( rhs.m00, rhs.m01, rhs.m02, rhs.m03 );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( matRes.m00, matRes.m01, matRes.m02, matRes.m03 );

        MathSSEFn->Push( m10, m11, m12, m13 );
        MathSSEFn->Push( rhs.m10, rhs.m11, rhs.m12, rhs.m13 );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( matRes.m10, matRes.m11, matRes.m12, matRes.m13 );

        MathSSEFn->Push( m20, m21, m22, m23 );
        MathSSEFn->Push( rhs.m20, rhs.m21, rhs.m22, rhs.m23 );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( matRes.m20, matRes.m21, matRes.m22, matRes.m23 );

        MathSSEFn->Push( m30, m31, m32, m33 );
        MathSSEFn->Push( rhs.m30, rhs.m31, rhs.m32, rhs.m33 );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( matRes.m30, matRes.m31, matRes.m32, matRes.m33 );

        return matRes;
    #else
        return TMatrix4<Float>( m00 - rhs.m00, m01 - rhs.m01, m02 - rhs.m02, m03 - rhs.m03,
                                m10 - rhs.m10, m11 - rhs.m11, m12 - rhs.m12, m13 - rhs.m13,
                                m20 - rhs.m20, m21 - rhs.m21, m22 - rhs.m22, m23 - rhs.m23,
                                m30 - rhs.m30, m31 - rhs.m31, m32 - rhs.m32, m33 - rhs.m33 );
    #endif
}
template<>
TMatrix4<Double> TMatrix4<Double>::operator-(const TMatrix4<Double> & rhs) const
{
    #ifdef MATH_USE_SSE
        TMatrix4<Double> matRes;

        MathSSEFn->Push( m00, m01 );
        MathSSEFn->Push( rhs.m00, rhs.m01 );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( matRes.m00, matRes.m01 );
        MathSSEFn->Push( m02, m03 );
        MathSSEFn->Push( rhs.m02, rhs.m03 );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( matRes.m02, matRes.m03 );

        MathSSEFn->Push( m10, m11 );
        MathSSEFn->Push( rhs.m10, rhs.m11 );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( matRes.m10, matRes.m11 );
        MathSSEFn->Push( m12, m13 );
        MathSSEFn->Push( rhs.m12, rhs.m13 );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( matRes.m12, matRes.m13 );

        MathSSEFn->Push( m20, m21 );
        MathSSEFn->Push( rhs.m20, rhs.m21 );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( matRes.m20, matRes.m21 );
        MathSSEFn->Push( m22, m23 );
        MathSSEFn->Push( rhs.m22, rhs.m23 );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( matRes.m22, matRes.m23 );

        MathSSEFn->Push( m30, m31 );
        MathSSEFn->Push( rhs.m30, rhs.m31 );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( matRes.m30, matRes.m31 );
        MathSSEFn->Push( m32, m33 );
        MathSSEFn->Push( rhs.m32, rhs.m33 );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( matRes.m32, matRes.m33 );

        return matRes;
    #else
        return TMatrix4<Double>( m00 - rhs.m00, m01 - rhs.m01, m02 - rhs.m02, m03 - rhs.m03,
                                 m10 - rhs.m10, m11 - rhs.m11, m12 - rhs.m12, m13 - rhs.m13,
                                 m20 - rhs.m20, m21 - rhs.m21, m22 - rhs.m22, m23 - rhs.m23,
                                 m30 - rhs.m30, m31 - rhs.m31, m32 - rhs.m32, m33 - rhs.m33 );
    #endif
}

template<>
TMatrix4<Float> TMatrix4<Float>::operator*(const TMatrix4<Float> & rhs) const
{
    #ifdef MATH_USE_SSE
        // That's the funny one xD
        TMatrix4<Float> matRes;

        // Row 0
        MathSSEFn->Push( m00, m00, m00, m00 );
        MathSSEFn->Push( rhs.m00, rhs.m01, rhs.m02, rhs.m03 );
        MathSSEFn->MulPF();
        MathSSEFn->Push( m01, m01, m01, m01 );
        MathSSEFn->Push( rhs.m10, rhs.m11, rhs.m12, rhs.m13 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Push( m02, m02, m02, m02 );
        MathSSEFn->Push( rhs.m20, rhs.m21, rhs.m22, rhs.m23 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Push( m03, m03, m03, m03 );
        MathSSEFn->Push( rhs.m30, rhs.m31, rhs.m32, rhs.m33 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Pop( matRes.m00, matRes.m01, matRes.m02, matRes.m03 );

        // Row 1
        MathSSEFn->Push( m10, m10, m10, m10 );
        MathSSEFn->Push( rhs.m00, rhs.m01, rhs.m02, rhs.m03 );
        MathSSEFn->MulPF();
        MathSSEFn->Push( m11, m11, m11, m11 );
        MathSSEFn->Push( rhs.m10, rhs.m11, rhs.m12, rhs.m13 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Push( m12, m12, m12, m12 );
        MathSSEFn->Push( rhs.m20, rhs.m21, rhs.m22, rhs.m23 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Push( m13, m13, m13, m13 );
        MathSSEFn->Push( rhs.m30, rhs.m31, rhs.m32, rhs.m33 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Pop( matRes.m10, matRes.m11, matRes.m12, matRes.m13 );

        // Row 2
        MathSSEFn->Push( m20, m20, m20, m20 );
        MathSSEFn->Push( rhs.m00, rhs.m01, rhs.m02, rhs.m03 );
        MathSSEFn->MulPF();
        MathSSEFn->Push( m21, m21, m21, m21 );
        MathSSEFn->Push( rhs.m10, rhs.m11, rhs.m12, rhs.m13 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Push( m22, m22, m22, m22 );
        MathSSEFn->Push( rhs.m20, rhs.m21, rhs.m22, rhs.m23 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Push( m23, m23, m23, m23 );
        MathSSEFn->Push( rhs.m30, rhs.m31, rhs.m32, rhs.m33 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Pop( matRes.m20, matRes.m21, matRes.m22, matRes.m23 );

        // Row 3
        MathSSEFn->Push( m30, m30, m30, m30 );
        MathSSEFn->Push( rhs.m00, rhs.m01, rhs.m02, rhs.m03 );
        MathSSEFn->MulPF();
        MathSSEFn->Push( m31, m31, m31, m31 );
        MathSSEFn->Push( rhs.m10, rhs.m11, rhs.m12, rhs.m13 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Push( m32, m32, m32, m32 );
        MathSSEFn->Push( rhs.m20, rhs.m21, rhs.m22, rhs.m23 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Push( m33, m33, m33, m33 );
        MathSSEFn->Push( rhs.m30, rhs.m31, rhs.m32, rhs.m33 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Pop( matRes.m30, matRes.m31, matRes.m32, matRes.m33 );

        return matRes;
    #else
	    return TMatrix4<Float> (
            (m00 * rhs.m00 + m01 * rhs.m10 + m02 * rhs.m20 + m03 * rhs.m30),
            (m00 * rhs.m01 + m01 * rhs.m11 + m02 * rhs.m21 + m03 * rhs.m31),
            (m00 * rhs.m02 + m01 * rhs.m12 + m02 * rhs.m22 + m03 * rhs.m32),
            (m00 * rhs.m03 + m01 * rhs.m13 + m02 * rhs.m23 + m03 * rhs.m33),

		    (m10 * rhs.m00 + m11 * rhs.m10 + m12 * rhs.m20 + m13 * rhs.m30),
            (m10 * rhs.m01 + m11 * rhs.m11 + m12 * rhs.m21 + m13 * rhs.m31),
            (m10 * rhs.m02 + m11 * rhs.m12 + m12 * rhs.m22 + m13 * rhs.m32),
            (m10 * rhs.m03 + m11 * rhs.m13 + m12 * rhs.m23 + m13 * rhs.m33),

		    (m20 * rhs.m00 + m21 * rhs.m10 + m22 * rhs.m20 + m23 * rhs.m30),
            (m20 * rhs.m01 + m21 * rhs.m11 + m22 * rhs.m21 + m23 * rhs.m31),
            (m20 * rhs.m02 + m21 * rhs.m12 + m22 * rhs.m22 + m23 * rhs.m32),
            (m20 * rhs.m03 + m21 * rhs.m13 + m22 * rhs.m23 + m23 * rhs.m33),

		    (m30 * rhs.m00 + m31 * rhs.m10 + m32 * rhs.m20 + m33 * rhs.m30),
            (m30 * rhs.m01 + m31 * rhs.m11 + m32 * rhs.m21 + m33 * rhs.m31),
            (m30 * rhs.m02 + m31 * rhs.m12 + m32 * rhs.m22 + m33 * rhs.m32),
            (m30 * rhs.m03 + m31 * rhs.m13 + m32 * rhs.m23 + m33 * rhs.m33)
        );
    #endif
}
template<>
TMatrix4<Double> TMatrix4<Double>::operator*(const TMatrix4<Double> & rhs) const
{
    #ifdef MATH_USE_SSE
        // That's the even more funny one xD
        TMatrix4<Double> matRes;

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
        MathSSEFn->Push( m03, m03 );
        MathSSEFn->Push( rhs.m30, rhs.m31 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Pop( matRes.m00, matRes.m01 );

        MathSSEFn->Push( m00, m00 );
        MathSSEFn->Push( rhs.m02, rhs.m03 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m01, m01 );
        MathSSEFn->Push( rhs.m12, rhs.m13 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( m02, m02 );
        MathSSEFn->Push( rhs.m22, rhs.m23 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( m03, m03 );
        MathSSEFn->Push( rhs.m32, rhs.m33 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Pop( matRes.m02, matRes.m03 );

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
        MathSSEFn->Push( m13, m13 );
        MathSSEFn->Push( rhs.m30, rhs.m31 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Pop( matRes.m10, matRes.m11 );

        MathSSEFn->Push( m10, m10 );
        MathSSEFn->Push( rhs.m02, rhs.m03 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m11, m11 );
        MathSSEFn->Push( rhs.m12, rhs.m13 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( m12, m12 );
        MathSSEFn->Push( rhs.m22, rhs.m23 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( m13, m13 );
        MathSSEFn->Push( rhs.m32, rhs.m33 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Pop( matRes.m12, matRes.m13 );

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
        MathSSEFn->Push( m23, m23 );
        MathSSEFn->Push( rhs.m30, rhs.m31 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Pop( matRes.m20, matRes.m21 );

        MathSSEFn->Push( m20, m20 );
        MathSSEFn->Push( rhs.m02, rhs.m03 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m21, m21 );
        MathSSEFn->Push( rhs.m12, rhs.m13 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( m22, m22 );
        MathSSEFn->Push( rhs.m22, rhs.m23 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( m23, m23 );
        MathSSEFn->Push( rhs.m32, rhs.m33 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Pop( matRes.m22, matRes.m23 );

        // Row 3
        MathSSEFn->Push( m30, m30 );
        MathSSEFn->Push( rhs.m00, rhs.m01 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m31, m31 );
        MathSSEFn->Push( rhs.m10, rhs.m11 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( m32, m32 );
        MathSSEFn->Push( rhs.m20, rhs.m21 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( m33, m33 );
        MathSSEFn->Push( rhs.m30, rhs.m31 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Pop( matRes.m30, matRes.m31 );

        MathSSEFn->Push( m30, m30 );
        MathSSEFn->Push( rhs.m02, rhs.m03 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m31, m31 );
        MathSSEFn->Push( rhs.m12, rhs.m13 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( m32, m32 );
        MathSSEFn->Push( rhs.m22, rhs.m23 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( m33, m33 );
        MathSSEFn->Push( rhs.m32, rhs.m33 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Pop( matRes.m32, matRes.m33 );

        return matRes;
    #else
	    return TMatrix4<Double> (
            (m00 * rhs.m00 + m01 * rhs.m10 + m02 * rhs.m20 + m03 * rhs.m30),
            (m00 * rhs.m01 + m01 * rhs.m11 + m02 * rhs.m21 + m03 * rhs.m31),
            (m00 * rhs.m02 + m01 * rhs.m12 + m02 * rhs.m22 + m03 * rhs.m32),
            (m00 * rhs.m03 + m01 * rhs.m13 + m02 * rhs.m23 + m03 * rhs.m33),

		    (m10 * rhs.m00 + m11 * rhs.m10 + m12 * rhs.m20 + m13 * rhs.m30),
            (m10 * rhs.m01 + m11 * rhs.m11 + m12 * rhs.m21 + m13 * rhs.m31),
            (m10 * rhs.m02 + m11 * rhs.m12 + m12 * rhs.m22 + m13 * rhs.m32),
            (m10 * rhs.m03 + m11 * rhs.m13 + m12 * rhs.m23 + m13 * rhs.m33),

		    (m20 * rhs.m00 + m21 * rhs.m10 + m22 * rhs.m20 + m23 * rhs.m30),
            (m20 * rhs.m01 + m21 * rhs.m11 + m22 * rhs.m21 + m23 * rhs.m31),
            (m20 * rhs.m02 + m21 * rhs.m12 + m22 * rhs.m22 + m23 * rhs.m32),
            (m20 * rhs.m03 + m21 * rhs.m13 + m22 * rhs.m23 + m23 * rhs.m33),

		    (m30 * rhs.m00 + m31 * rhs.m10 + m32 * rhs.m20 + m33 * rhs.m30),
            (m30 * rhs.m01 + m31 * rhs.m11 + m32 * rhs.m21 + m33 * rhs.m31),
            (m30 * rhs.m02 + m31 * rhs.m12 + m32 * rhs.m22 + m33 * rhs.m32),
            (m30 * rhs.m03 + m31 * rhs.m13 + m32 * rhs.m23 + m33 * rhs.m33)
        );
    #endif
}

template<>
TMatrix4<Float> & TMatrix4<Float>::operator+=(const TMatrix4<Float> & rhs)
{
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( m00, m01, m02, m03 );
        MathSSEFn->Push( rhs.m00, rhs.m01, rhs.m02, rhs.m03 );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( m00, m01, m02, m03 );

        MathSSEFn->Push( m10, m11, m12, m13 );
        MathSSEFn->Push( rhs.m10, rhs.m11, rhs.m12, rhs.m13 );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( m10, m11, m12, m13 );

        MathSSEFn->Push( m20, m21, m22, m23 );
        MathSSEFn->Push( rhs.m20, rhs.m21, rhs.m22, rhs.m23 );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( m20, m21, m22, m23 );

        MathSSEFn->Push( m30, m31, m32, m33 );
        MathSSEFn->Push( rhs.m30, rhs.m31, rhs.m32, rhs.m33 );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( m30, m31, m32, m33 );
    #else
        m00 += rhs.m00; m01 += rhs.m01; m02 += rhs.m02; m03 += rhs.m03;
		m10 += rhs.m10; m11 += rhs.m11; m12 += rhs.m12; m13 += rhs.m13;
		m20 += rhs.m20; m21 += rhs.m21; m22 += rhs.m22; m23 += rhs.m23;
		m30 += rhs.m30; m31 += rhs.m31; m32 += rhs.m32; m33 += rhs.m33;
    #endif
    return (*this);
}
template<>
TMatrix4<Double> & TMatrix4<Double>::operator+=(const TMatrix4<Double> & rhs)
{
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( m00, m01 );
        MathSSEFn->Push( rhs.m00, rhs.m01 );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( m00, m01 );
        MathSSEFn->Push( m02, m03 );
        MathSSEFn->Push( rhs.m02, rhs.m03 );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( m02, m03 );

        MathSSEFn->Push( m10, m11 );
        MathSSEFn->Push( rhs.m10, rhs.m11 );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( m10, m11 );
        MathSSEFn->Push( m12, m13 );
        MathSSEFn->Push( rhs.m12, rhs.m13 );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( m12, m13 );

        MathSSEFn->Push( m20, m21 );
        MathSSEFn->Push( rhs.m20, rhs.m21 );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( m20, m21 );
        MathSSEFn->Push( m22, m23 );
        MathSSEFn->Push( rhs.m22, rhs.m23 );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( m22, m23 );

        MathSSEFn->Push( m30, m31 );
        MathSSEFn->Push( rhs.m30, rhs.m31 );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( m30, m31 );
        MathSSEFn->Push( m32, m33 );
        MathSSEFn->Push( rhs.m32, rhs.m33 );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( m32, m33 );
    #else
        m00 += rhs.m00; m01 += rhs.m01; m02 += rhs.m02; m03 += rhs.m03;
		m10 += rhs.m10; m11 += rhs.m11; m12 += rhs.m12; m13 += rhs.m13;
		m20 += rhs.m20; m21 += rhs.m21; m22 += rhs.m22; m23 += rhs.m23;
		m30 += rhs.m30; m31 += rhs.m31; m32 += rhs.m32; m33 += rhs.m33;
    #endif
    return (*this);
}

template<>
TMatrix4<Float> & TMatrix4<Float>::operator-=(const TMatrix4<Float> & rhs)
{
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( m00, m01, m02, m03 );
        MathSSEFn->Push( rhs.m00, rhs.m01, rhs.m02, rhs.m03 );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( m00, m01, m02, m03 );

        MathSSEFn->Push( m10, m11, m12, m13 );
        MathSSEFn->Push( rhs.m10, rhs.m11, rhs.m12, rhs.m13 );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( m10, m11, m12, m13 );

        MathSSEFn->Push( m20, m21, m22, m23 );
        MathSSEFn->Push( rhs.m20, rhs.m21, rhs.m22, rhs.m23 );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( m20, m21, m22, m23 );

        MathSSEFn->Push( m30, m31, m32, m33 );
        MathSSEFn->Push( rhs.m30, rhs.m31, rhs.m32, rhs.m33 );
        MathSSEFn->SubPF();
        MathSSEFn->Pop( m30, m31, m32, m33 );
    #else
        m00 -= rhs.m00; m01 -= rhs.m01; m02 -= rhs.m02; m03 -= rhs.m03;
	    m10 -= rhs.m10; m11 -= rhs.m11; m12 -= rhs.m12; m13 -= rhs.m13;
	    m20 -= rhs.m20; m21 -= rhs.m21; m22 -= rhs.m22; m23 -= rhs.m23;
	    m30 -= rhs.m30; m31 -= rhs.m31; m32 -= rhs.m32; m33 -= rhs.m33;
    #endif
    return (*this);
}
template<>
TMatrix4<Double> & TMatrix4<Double>::operator-=(const TMatrix4<Double> & rhs)
{
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( m00, m01 );
        MathSSEFn->Push( rhs.m00, rhs.m01 );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( m00, m01 );
        MathSSEFn->Push( m02, m03 );
        MathSSEFn->Push( rhs.m02, rhs.m03 );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( m02, m03 );

        MathSSEFn->Push( m10, m11 );
        MathSSEFn->Push( rhs.m10, rhs.m11 );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( m10, m11 );
        MathSSEFn->Push( m12, m13 );
        MathSSEFn->Push( rhs.m12, rhs.m13 );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( m12, m13 );

        MathSSEFn->Push( m20, m21 );
        MathSSEFn->Push( rhs.m20, rhs.m21 );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( m20, m21 );
        MathSSEFn->Push( m22, m23 );
        MathSSEFn->Push( rhs.m22, rhs.m23 );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( m22, m23 );

        MathSSEFn->Push( m30, m31 );
        MathSSEFn->Push( rhs.m30, rhs.m31 );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( m30, m31 );
        MathSSEFn->Push( m32, m33 );
        MathSSEFn->Push( rhs.m32, rhs.m33 );
        MathSSEFn->SubPD();
        MathSSEFn->Pop( m32, m33 );
    #else
        m00 -= rhs.m00; m01 -= rhs.m01; m02 -= rhs.m02; m03 -= rhs.m03;
	    m10 -= rhs.m10; m11 -= rhs.m11; m12 -= rhs.m12; m13 -= rhs.m13;
	    m20 -= rhs.m20; m21 -= rhs.m21; m22 -= rhs.m22; m23 -= rhs.m23;
	    m30 -= rhs.m30; m31 -= rhs.m31; m32 -= rhs.m32; m33 -= rhs.m33;
    #endif
    return (*this);
}

template<>
TMatrix4<Float> & TMatrix4<Float>::operator*=(const TMatrix4<Float> & rhs)
{
    #ifdef MATH_USE_SSE
        // That's the funny one xD

        // Row 0
        MathSSEFn->Push( m00, m00, m00, m00 );
        MathSSEFn->Push( rhs.m00, rhs.m01, rhs.m02, rhs.m03 );
        MathSSEFn->MulPF();
        MathSSEFn->Push( m01, m01, m01, m01 );
        MathSSEFn->Push( rhs.m10, rhs.m11, rhs.m12, rhs.m13 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Push( m02, m02, m02, m02 );
        MathSSEFn->Push( rhs.m20, rhs.m21, rhs.m22, rhs.m23 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Push( m03, m03, m03, m03 );
        MathSSEFn->Push( rhs.m30, rhs.m31, rhs.m32, rhs.m33 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Pop( m00, m01, m02, m03 );

        // Row 1
        MathSSEFn->Push( m10, m10, m10, m10 );
        MathSSEFn->Push( rhs.m00, rhs.m01, rhs.m02, rhs.m03 );
        MathSSEFn->MulPF();
        MathSSEFn->Push( m11, m11, m11, m11 );
        MathSSEFn->Push( rhs.m10, rhs.m11, rhs.m12, rhs.m13 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Push( m12, m12, m12, m12 );
        MathSSEFn->Push( rhs.m20, rhs.m21, rhs.m22, rhs.m23 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Push( m13, m13, m13, m13 );
        MathSSEFn->Push( rhs.m30, rhs.m31, rhs.m32, rhs.m33 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Pop( m10, m11, m12, m13 );

        // Row 2
        MathSSEFn->Push( m20, m20, m20, m20 );
        MathSSEFn->Push( rhs.m00, rhs.m01, rhs.m02, rhs.m03 );
        MathSSEFn->MulPF();
        MathSSEFn->Push( m21, m21, m21, m21 );
        MathSSEFn->Push( rhs.m10, rhs.m11, rhs.m12, rhs.m13 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Push( m22, m22, m22, m22 );
        MathSSEFn->Push( rhs.m20, rhs.m21, rhs.m22, rhs.m23 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Push( m23, m23, m23, m23 );
        MathSSEFn->Push( rhs.m30, rhs.m31, rhs.m32, rhs.m33 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Pop( m20, m21, m22, m23 );

        // Row 3
        MathSSEFn->Push( m30, m30, m30, m30 );
        MathSSEFn->Push( rhs.m00, rhs.m01, rhs.m02, rhs.m03 );
        MathSSEFn->MulPF();
        MathSSEFn->Push( m31, m31, m31, m31 );
        MathSSEFn->Push( rhs.m10, rhs.m11, rhs.m12, rhs.m13 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Push( m32, m32, m32, m32 );
        MathSSEFn->Push( rhs.m20, rhs.m21, rhs.m22, rhs.m23 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Push( m33, m33, m33, m33 );
        MathSSEFn->Push( rhs.m30, rhs.m31, rhs.m32, rhs.m33 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Pop( m30, m31, m32, m33 );
    #else
        Float f0 = (m00 * rhs.m00 + m01 * rhs.m10 + m02 * rhs.m20 + m03 * rhs.m30);
        Float f1 = (m00 * rhs.m01 + m01 * rhs.m11 + m02 * rhs.m21 + m03 * rhs.m31);
        Float f2 = (m00 * rhs.m02 + m01 * rhs.m12 + m02 * rhs.m22 + m03 * rhs.m32);
        Float f3 = (m00 * rhs.m03 + m01 * rhs.m13 + m02 * rhs.m23 + m03 * rhs.m33);
        m00 = f0; m01 = f1; m02 = f2; m03 = f3;

        f0 = (m10 * rhs.m00 + m11 * rhs.m10 + m12 * rhs.m20 + m13 * rhs.m30);
        f1 = (m10 * rhs.m01 + m11 * rhs.m11 + m12 * rhs.m21 + m13 * rhs.m31);
        f2 = (m10 * rhs.m02 + m11 * rhs.m12 + m12 * rhs.m22 + m13 * rhs.m32);
        f3 = (m10 * rhs.m03 + m11 * rhs.m13 + m12 * rhs.m23 + m13 * rhs.m33);
        m10 = f0; m11 = f1; m12 = f2; m13 = f3;

        f0 = (m20 * rhs.m00 + m21 * rhs.m10 + m22 * rhs.m20 + m23 * rhs.m30);
        f1 = (m20 * rhs.m01 + m21 * rhs.m11 + m22 * rhs.m21 + m23 * rhs.m31);
        f2 = (m20 * rhs.m02 + m21 * rhs.m12 + m22 * rhs.m22 + m23 * rhs.m32);
        f3 = (m20 * rhs.m03 + m21 * rhs.m13 + m22 * rhs.m23 + m23 * rhs.m33);
        m20 = f0; m21 = f1; m22 = f2; m23 = f3;

        f0 = (m30 * rhs.m00 + m31 * rhs.m10 + m32 * rhs.m20 + m33 * rhs.m30);
        f1 = (m30 * rhs.m01 + m31 * rhs.m11 + m32 * rhs.m21 + m33 * rhs.m31);
        f2 = (m30 * rhs.m02 + m31 * rhs.m12 + m32 * rhs.m22 + m33 * rhs.m32);
        f3 = (m30 * rhs.m03 + m31 * rhs.m13 + m32 * rhs.m23 + m33 * rhs.m33);
        m30 = f0; m31 = f1; m32 = f2; m33 = f3;
    #endif
    return (*this);
}
template<>
TMatrix4<Double> & TMatrix4<Double>::operator*=(const TMatrix4<Double> & rhs)
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
        MathSSEFn->Push( m03, m03 );
        MathSSEFn->Push( rhs.m30, rhs.m31 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();

        MathSSEFn->Push( m00, m00 );
        MathSSEFn->Push( rhs.m02, rhs.m03 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m01, m01 );
        MathSSEFn->Push( rhs.m12, rhs.m13 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( m02, m02 );
        MathSSEFn->Push( rhs.m22, rhs.m23 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( m03, m03 );
        MathSSEFn->Push( rhs.m32, rhs.m33 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();

        MathSSEFn->Pop( m02, m03 );
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
        MathSSEFn->Push( m13, m13 );
        MathSSEFn->Push( rhs.m30, rhs.m31 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();

        MathSSEFn->Push( m10, m10 );
        MathSSEFn->Push( rhs.m02, rhs.m03 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m11, m11 );
        MathSSEFn->Push( rhs.m12, rhs.m13 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( m12, m12 );
        MathSSEFn->Push( rhs.m22, rhs.m23 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( m13, m13 );
        MathSSEFn->Push( rhs.m32, rhs.m33 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();

        MathSSEFn->Pop( m12, m13 );
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
        MathSSEFn->Push( m23, m23 );
        MathSSEFn->Push( rhs.m30, rhs.m31 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();

        MathSSEFn->Push( m20, m20 );
        MathSSEFn->Push( rhs.m02, rhs.m03 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m21, m21 );
        MathSSEFn->Push( rhs.m12, rhs.m13 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( m22, m22 );
        MathSSEFn->Push( rhs.m22, rhs.m23 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( m23, m23 );
        MathSSEFn->Push( rhs.m32, rhs.m33 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();

        MathSSEFn->Pop( m22, m23 );
        MathSSEFn->Pop( m20, m21 );

        // Row 3
        MathSSEFn->Push( m30, m30 );
        MathSSEFn->Push( rhs.m00, rhs.m01 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m31, m31 );
        MathSSEFn->Push( rhs.m10, rhs.m11 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( m32, m32 );
        MathSSEFn->Push( rhs.m20, rhs.m21 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( m33, m33 );
        MathSSEFn->Push( rhs.m30, rhs.m31 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();

        MathSSEFn->Push( m30, m30 );
        MathSSEFn->Push( rhs.m02, rhs.m03 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m31, m31 );
        MathSSEFn->Push( rhs.m12, rhs.m13 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( m32, m32 );
        MathSSEFn->Push( rhs.m22, rhs.m23 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( m33, m33 );
        MathSSEFn->Push( rhs.m32, rhs.m33 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();

        MathSSEFn->Pop( m32, m33 );
        MathSSEFn->Pop( m30, m31 );
    #else
        Double f0 = (m00 * rhs.m00 + m01 * rhs.m10 + m02 * rhs.m20 + m03 * rhs.m30);
        Double f1 = (m00 * rhs.m01 + m01 * rhs.m11 + m02 * rhs.m21 + m03 * rhs.m31);
        Double f2 = (m00 * rhs.m02 + m01 * rhs.m12 + m02 * rhs.m22 + m03 * rhs.m32);
        Double f3 = (m00 * rhs.m03 + m01 * rhs.m13 + m02 * rhs.m23 + m03 * rhs.m33);
        m00 = f0; m01 = f1; m02 = f2; m03 = f3;

        f0 = (m10 * rhs.m00 + m11 * rhs.m10 + m12 * rhs.m20 + m13 * rhs.m30);
        f1 = (m10 * rhs.m01 + m11 * rhs.m11 + m12 * rhs.m21 + m13 * rhs.m31);
        f2 = (m10 * rhs.m02 + m11 * rhs.m12 + m12 * rhs.m22 + m13 * rhs.m32);
        f3 = (m10 * rhs.m03 + m11 * rhs.m13 + m12 * rhs.m23 + m13 * rhs.m33);
        m10 = f0; m11 = f1; m12 = f2; m13 = f3;

        f0 = (m20 * rhs.m00 + m21 * rhs.m10 + m22 * rhs.m20 + m23 * rhs.m30);
        f1 = (m20 * rhs.m01 + m21 * rhs.m11 + m22 * rhs.m21 + m23 * rhs.m31);
        f2 = (m20 * rhs.m02 + m21 * rhs.m12 + m22 * rhs.m22 + m23 * rhs.m32);
        f3 = (m20 * rhs.m03 + m21 * rhs.m13 + m22 * rhs.m23 + m23 * rhs.m33);
        m20 = f0; m21 = f1; m22 = f2; m23 = f3;

        f0 = (m30 * rhs.m00 + m31 * rhs.m10 + m32 * rhs.m20 + m33 * rhs.m30);
        f1 = (m30 * rhs.m01 + m31 * rhs.m11 + m32 * rhs.m21 + m33 * rhs.m31);
        f2 = (m30 * rhs.m02 + m31 * rhs.m12 + m32 * rhs.m22 + m33 * rhs.m32);
        f3 = (m30 * rhs.m03 + m31 * rhs.m13 + m32 * rhs.m23 + m33 * rhs.m33);
        m30 = f0; m31 = f1; m32 = f2; m33 = f3;
    #endif
    return (*this);
}

template<>
Float TMatrix4<Float>::Determinant() const
{
    Float fA0, fA1, fA2, fA3, fA4, fA5;
    Float fB0, fB1, fB2, fB3, fB4, fB5, fDet;
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( m00, m00, m00, m01 );
        MathSSEFn->Push( m11, m12, m13, m12 );
        MathSSEFn->MulPF();
        MathSSEFn->Push( m01, m02, m03, m02 );
        MathSSEFn->Push( m10, m10, m10, m11 );
        MathSSEFn->MulPF();
        MathSSEFn->SubPF();
        MathSSEFn->Pop( fA0, fA1, fA2, fA3 );

        MathSSEFn->Push( m01, m02, m20, m20 );
        MathSSEFn->Push( m13, m13, m31, m32 );
        MathSSEFn->MulPF();
        MathSSEFn->Push( m03, m03, m21, m22 );
        MathSSEFn->Push( m11, m12, m30, m30 );
        MathSSEFn->MulPF();
        MathSSEFn->SubPF();
        MathSSEFn->Pop( fA4, fA5, fB0, fB1 );

        MathSSEFn->Push( m20, m21, m21, m22 );
        MathSSEFn->Push( m33, m32, m33, m33 );
        MathSSEFn->MulPF();
        MathSSEFn->Push( m23, m22, m23, m23 );
        MathSSEFn->Push( m30, m31, m31, m32 );
        MathSSEFn->MulPF();
        MathSSEFn->SubPF();
        MathSSEFn->Pop( fB2, fB3, fB4, fB5 );

        MathSSEFn->Push( fA0, -fA1, fA2, 0.0f );
        MathSSEFn->Push( fB5, fB4, fB3, 0.0f );
        MathSSEFn->MulPF();
        MathSSEFn->Push( fA3, -fA4, fA5, 0.0f );
        MathSSEFn->Push( fB2, fB1, fB0, 0.0f );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->HAddF(0);
        MathSSEFn->HAddF(0);
        MathSSEFn->Pop(fDet);
    #else
        fA0 = m00 * m11 - m01 * m10;
        fA1 = m00 * m12 - m02 * m10;
        fA2 = m00 * m13 - m03 * m10;
        fA3 = m01 * m12 - m02 * m11;
        fA4 = m01 * m13 - m03 * m11;
        fA5 = m02 * m13 - m03 * m12;
        fB0 = m20 * m31 - m21 * m30;
        fB1 = m20 * m32 - m22 * m30;
        fB2 = m20 * m33 - m23 * m30;
        fB3 = m21 * m32 - m22 * m31;
        fB4 = m21 * m33 - m23 * m31;
        fB5 = m22 * m33 - m23 * m32;
        fDet = ( + fA0*fB5 - fA1*fB4 + fA2*fB3
                 + fA3*fB2 - fA4*fB1 + fA5*fB0 );
    #endif
    return fDet;
}
template<>
Double TMatrix4<Double>::Determinant() const
{
    Double fA0, fA1, fA2, fA3, fA4, fA5;
    Double fB0, fB1, fB2, fB3, fB4, fB5, fDet;
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( m00, m00 );
        MathSSEFn->Push( m11, m12 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m01, m02 );
        MathSSEFn->Push( m10, m10 );
        MathSSEFn->MulPD();
        MathSSEFn->SubPD();
        MathSSEFn->Pop( fA0, fA1 );

        MathSSEFn->Push( m00, m01 );
        MathSSEFn->Push( m13, m12 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m03, m02 );
        MathSSEFn->Push( m10, m11 );
        MathSSEFn->MulPD();
        MathSSEFn->SubPD();
        MathSSEFn->Pop( fA2, fA3 );

        MathSSEFn->Push( m01, m02 );
        MathSSEFn->Push( m13, m13 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m03, m03 );
        MathSSEFn->Push( m11, m12 );
        MathSSEFn->MulPD();
        MathSSEFn->SubPD();
        MathSSEFn->Pop( fA4, fA5 );

        MathSSEFn->Push( m20, m20 );
        MathSSEFn->Push( m31, m32 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m21, m22 );
        MathSSEFn->Push( m30, m30 );
        MathSSEFn->MulPD();
        MathSSEFn->SubPD();
        MathSSEFn->Pop( fB0, fB1 );

        MathSSEFn->Push( m20, m21 );
        MathSSEFn->Push( m33, m32 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m23, m22 );
        MathSSEFn->Push( m30, m31 );
        MathSSEFn->MulPD();
        MathSSEFn->SubPD();
        MathSSEFn->Pop( fB2, fB3 );

        MathSSEFn->Push( m21, m22 );
        MathSSEFn->Push( m33, m33 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m23, m23 );
        MathSSEFn->Push( m31, m32 );
        MathSSEFn->MulPD();
        MathSSEFn->SubPD();
        MathSSEFn->Pop( fB4, fB5 );

        MathSSEFn->Push( fA0, -fA1 );
        MathSSEFn->Push( fB5, fB4 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( fA2, fA3 );
        MathSSEFn->Push( fB3, fB2 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( -fA4, fA5 );
        MathSSEFn->Push( fB1, fB0 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->HAddD(0);
        MathSSEFn->Pop(fDet);
    #else
        fA0 = m00 * m11 - m01 * m10;
        fA1 = m00 * m12 - m02 * m10;
        fA2 = m00 * m13 - m03 * m10;
        fA3 = m01 * m12 - m02 * m11;
        fA4 = m01 * m13 - m03 * m11;
        fA5 = m02 * m13 - m03 * m12;
        fB0 = m20 * m31 - m21 * m30;
        fB1 = m20 * m32 - m22 * m30;
        fB2 = m20 * m33 - m23 * m30;
        fB3 = m21 * m32 - m22 * m31;
        fB4 = m21 * m33 - m23 * m31;
        fB5 = m22 * m33 - m23 * m32;
        fDet = ( + fA0*fB5 - fA1*fB4
                 + fA2*fB3 + fA3*fB2
                 - fA4*fB1 + fA5*fB0 );
    #endif
    return fDet;
}

template<>
Void TMatrix4<Float>::Adjoint(TMatrix4<Float> & outAdjointMatrix) const
{
    Float fA0, fA1, fA2, fA3, fA4, fA5;
    Float fB0, fB1, fB2, fB3, fB4, fB5;

    #ifdef MATH_USE_SSE
        MathSSEFn->Push( m00, m00, m00, m01 );
        MathSSEFn->Push( m11, m12, m13, m12 );
        MathSSEFn->MulPF();
        MathSSEFn->Push( m01, m02, m03, m02 );
        MathSSEFn->Push( m10, m10, m10, m11 );
        MathSSEFn->MulPF();
        MathSSEFn->SubPF();
        MathSSEFn->Pop( fA0, fA1, fA2, fA3 );

        MathSSEFn->Push( m01, m02, m20, m20 );
        MathSSEFn->Push( m13, m13, m31, m32 );
        MathSSEFn->MulPF();
        MathSSEFn->Push( m03, m03, m21, m22 );
        MathSSEFn->Push( m11, m12, m30, m30 );
        MathSSEFn->MulPF();
        MathSSEFn->SubPF();
        MathSSEFn->Pop( fA4, fA5, fB0, fB1 );

        MathSSEFn->Push( m20, m21, m21, m22 );
        MathSSEFn->Push( m33, m32, m33, m33 );
        MathSSEFn->MulPF();
        MathSSEFn->Push( m23, m22, m23, m23 );
        MathSSEFn->Push( m30, m31, m31, m32 );
        MathSSEFn->MulPF();
        MathSSEFn->SubPF();
        MathSSEFn->Pop( fB2, fB3, fB4, fB5 );
    #else
        fA0 = m00 * m11 - m01 * m10;
        fA1 = m00 * m12 - m02 * m10;
        fA2 = m00 * m13 - m03 * m10;
        fA3 = m01 * m12 - m02 * m11;
        fA4 = m01 * m13 - m03 * m11;
        fA5 = m02 * m13 - m03 * m12;
        fB0 = m20 * m31 - m21 * m30;
        fB1 = m20 * m32 - m22 * m30;
        fB2 = m20 * m33 - m23 * m30;
        fB3 = m21 * m32 - m22 * m31;
        fB4 = m21 * m33 - m23 * m31;
        fB5 = m22 * m33 - m23 * m32;
    #endif

    #ifdef MATH_USE_SSE
        MathSSEFn->Push( +m11, -m01, +m31, -m21 );
        MathSSEFn->Push( fB5, fB5, fA5, fA5 );
        MathSSEFn->MulPF();
        MathSSEFn->Push( -m12, +m02, -m32, +m22 );
        MathSSEFn->Push( fB4, fB4, fA4, fA4 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Push( +m13, -m03, +m33, -m23 );
        MathSSEFn->Push( fB3, fB3, fA3, fA3 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Pop( outAdjointMatrix.m00, outAdjointMatrix.m01, outAdjointMatrix.m02, outAdjointMatrix.m03 );

        MathSSEFn->Push( -m10, +m00, -m30, +m20 );
        MathSSEFn->Push( fB5, fB5, fA5, fA5 );
        MathSSEFn->MulPF();
        MathSSEFn->Push( +m12, -m02, +m32, -m22 );
        MathSSEFn->Push( fB2, fB2, fA2, fA2 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Push( -m13, +m03, -m33, +m23 );
        MathSSEFn->Push( fB1, fB1, fA1, fA1 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Pop( outAdjointMatrix.m10, outAdjointMatrix.m11, outAdjointMatrix.m12, outAdjointMatrix.m13 );

        MathSSEFn->Push( +m10, -m00, +m30, -m20 );
        MathSSEFn->Push( fB4, fB4, fA4, fA4 );
        MathSSEFn->MulPF();
        MathSSEFn->Push( -m11, +m01, -m31, +m21 );
        MathSSEFn->Push( fB2, fB2, fA2, fA2 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Push( +m13, -m03, +m33, -m23 );
        MathSSEFn->Push( fB0, fB0, fA0, fA0 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Pop( outAdjointMatrix.m20, outAdjointMatrix.m21, outAdjointMatrix.m22, outAdjointMatrix.m23 );

        MathSSEFn->Push( -m10, +m00, -m30, +m20 );
        MathSSEFn->Push( fB3, fB3, fA3, fA3 );
        MathSSEFn->MulPF();
        MathSSEFn->Push( +m11, -m01, +m31, -m21 );
        MathSSEFn->Push( fB1, fB1, fA1, fA1 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Push( -m12, +m02, -m32, +m22 );
        MathSSEFn->Push( fB0, fB0, fA0, fA0 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Pop( outAdjointMatrix.m30, outAdjointMatrix.m31, outAdjointMatrix.m32, outAdjointMatrix.m33 );
    #else
        outAdjointMatrix.m00 = + (m11 * fB5) - (m12 * fB4) + (m13 * fB3);
        outAdjointMatrix.m01 = - (m01 * fB5) + (m02 * fB4) - (m03 * fB3);
        outAdjointMatrix.m02 = + (m31 * fA5) - (m32 * fA4) + (m33 * fA3);
        outAdjointMatrix.m03 = - (m21 * fA5) + (m22 * fA4) - (m23 * fA3);

	    outAdjointMatrix.m10 = - (m10 * fB5) + (m12 * fB2) - (m13 * fB1);
        outAdjointMatrix.m11 = + (m00 * fB5) - (m02 * fB2) + (m03 * fB1);
        outAdjointMatrix.m12 = - (m30 * fA5) + (m32 * fA2) - (m33 * fA1);
        outAdjointMatrix.m13 = + (m20 * fA5) - (m22 * fA2) + (m23 * fA1);

	    outAdjointMatrix.m20 = + (m10 * fB4) - (m11 * fB2) + (m13 * fB0);
        outAdjointMatrix.m21 = - (m00 * fB4) + (m01 * fB2) - (m03 * fB0);
        outAdjointMatrix.m22 = + (m30 * fA4) - (m31 * fA2) + (m33 * fA0);
        outAdjointMatrix.m23 = - (m20 * fA4) + (m21 * fA2) - (m23 * fA0);

	    outAdjointMatrix.m30 = - (m10 * fB3) + (m11 * fB1) - (m12 * fB0);
        outAdjointMatrix.m31 = + (m00 * fB3) - (m01 * fB1) + (m02 * fB0);
        outAdjointMatrix.m32 = - (m30 * fA3) + (m31 * fA1) - (m32 * fA0);
        outAdjointMatrix.m33 = + (m20 * fA3) - (m21 * fA1) + (m22 * fA0);
    #endif
}
template<>
Void TMatrix4<Double>::Adjoint(TMatrix4<Double> & outAdjointMatrix) const
{
    Double fA0, fA1, fA2, fA3, fA4, fA5;
    Double fB0, fB1, fB2, fB3, fB4, fB5;

    #ifdef MATH_USE_SSE
        MathSSEFn->Push( m00, m00 );
        MathSSEFn->Push( m11, m12 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m01, m02 );
        MathSSEFn->Push( m10, m10 );
        MathSSEFn->MulPD();
        MathSSEFn->SubPD();
        MathSSEFn->Pop( fA0, fA1 );

        MathSSEFn->Push( m00, m01 );
        MathSSEFn->Push( m13, m12 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m03, m02 );
        MathSSEFn->Push( m10, m11 );
        MathSSEFn->MulPD();
        MathSSEFn->SubPD();
        MathSSEFn->Pop( fA2, fA3 );

        MathSSEFn->Push( m01, m02 );
        MathSSEFn->Push( m13, m13 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m03, m03 );
        MathSSEFn->Push( m11, m12 );
        MathSSEFn->MulPD();
        MathSSEFn->SubPD();
        MathSSEFn->Pop( fA4, fA5 );

        MathSSEFn->Push( m20, m20 );
        MathSSEFn->Push( m31, m32 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m21, m22 );
        MathSSEFn->Push( m30, m30 );
        MathSSEFn->MulPD();
        MathSSEFn->SubPD();
        MathSSEFn->Pop( fB0, fB1 );

        MathSSEFn->Push( m20, m21 );
        MathSSEFn->Push( m33, m32 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m23, m22 );
        MathSSEFn->Push( m30, m31 );
        MathSSEFn->MulPD();
        MathSSEFn->SubPD();
        MathSSEFn->Pop( fB2, fB3 );

        MathSSEFn->Push( m21, m22 );
        MathSSEFn->Push( m33, m33 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m23, m23 );
        MathSSEFn->Push( m31, m32 );
        MathSSEFn->MulPD();
        MathSSEFn->SubPD();
        MathSSEFn->Pop( fB4, fB5 );
    #else
        fA0 = m00 * m11 - m01 * m10;
        fA1 = m00 * m12 - m02 * m10;
        fA2 = m00 * m13 - m03 * m10;
        fA3 = m01 * m12 - m02 * m11;
        fA4 = m01 * m13 - m03 * m11;
        fA5 = m02 * m13 - m03 * m12;
        fB0 = m20 * m31 - m21 * m30;
        fB1 = m20 * m32 - m22 * m30;
        fB2 = m20 * m33 - m23 * m30;
        fB3 = m21 * m32 - m22 * m31;
        fB4 = m21 * m33 - m23 * m31;
        fB5 = m22 * m33 - m23 * m32;
    #endif

    #ifdef MATH_USE_SSE
        MathSSEFn->Push( +m11, -m01 );
        MathSSEFn->Push( fB5, fB5 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( -m12, +m02 );
        MathSSEFn->Push( fB4, fB4 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( +m13, -m03 );
        MathSSEFn->Push( fB3, fB3 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Pop( outAdjointMatrix.m00, outAdjointMatrix.m01 );

        MathSSEFn->Push( +m31, -m21 );
        MathSSEFn->Push( fA5, fA5 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( -m32, +m22 );
        MathSSEFn->Push( fA4, fA4 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( +m33, -m23 );
        MathSSEFn->Push( fA3, fA3 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Pop( outAdjointMatrix.m02, outAdjointMatrix.m03 );

        MathSSEFn->Push( -m10, +m00 );
        MathSSEFn->Push( fB5, fB5 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( +m12, -m02 );
        MathSSEFn->Push( fB2, fB2 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( -m13, +m03 );
        MathSSEFn->Push( fB1, fB1 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Pop( outAdjointMatrix.m10, outAdjointMatrix.m11 );

        MathSSEFn->Push( -m30, +m20 );
        MathSSEFn->Push( fA5, fA5 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( +m32, -m22 );
        MathSSEFn->Push( fA2, fA2 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( -m33, +m23 );
        MathSSEFn->Push( fA1, fA1 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Pop( outAdjointMatrix.m12, outAdjointMatrix.m13 );

        MathSSEFn->Push( +m10, -m00 );
        MathSSEFn->Push( fB4, fB4 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( -m11, +m01 );
        MathSSEFn->Push( fB2, fB2 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( +m13, -m03 );
        MathSSEFn->Push( fB0, fB0 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Pop( outAdjointMatrix.m20, outAdjointMatrix.m21 );

        MathSSEFn->Push( +m30, -m20 );
        MathSSEFn->Push( fA4, fA4 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( -m31, +m21 );
        MathSSEFn->Push( fA2, fA2 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( +m33, -m23 );
        MathSSEFn->Push( fA0, fA0 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Pop( outAdjointMatrix.m22, outAdjointMatrix.m23 );

        MathSSEFn->Push( -m10, +m00 );
        MathSSEFn->Push( fB3, fB3 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( +m11, -m01 );
        MathSSEFn->Push( fB1, fB1 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( -m12, +m02 );
        MathSSEFn->Push( fB0, fB0 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Pop( outAdjointMatrix.m30, outAdjointMatrix.m31 );

        MathSSEFn->Push( -m30, +m20 );
        MathSSEFn->Push( fA3, fA3 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( +m31, -m21 );
        MathSSEFn->Push( fA1, fA1 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( -m32, +m22 );
        MathSSEFn->Push( fA0, fA0 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Pop( outAdjointMatrix.m32, outAdjointMatrix.m33 );
    #else
        outAdjointMatrix.m00 = + (m11 * fB5) - (m12 * fB4) + (m13 * fB3);
        outAdjointMatrix.m01 = - (m01 * fB5) + (m02 * fB4) - (m03 * fB3);
        outAdjointMatrix.m02 = + (m31 * fA5) - (m32 * fA4) + (m33 * fA3);
        outAdjointMatrix.m03 = - (m21 * fA5) + (m22 * fA4) - (m23 * fA3);

	    outAdjointMatrix.m10 = - (m10 * fB5) + (m12 * fB2) - (m13 * fB1);
        outAdjointMatrix.m11 = + (m00 * fB5) - (m02 * fB2) + (m03 * fB1);
        outAdjointMatrix.m12 = - (m30 * fA5) + (m32 * fA2) - (m33 * fA1);
        outAdjointMatrix.m13 = + (m20 * fA5) - (m22 * fA2) + (m23 * fA1);

	    outAdjointMatrix.m20 = + (m10 * fB4) - (m11 * fB2) + (m13 * fB0);
        outAdjointMatrix.m21 = - (m00 * fB4) + (m01 * fB2) - (m03 * fB0);
        outAdjointMatrix.m22 = + (m30 * fA4) - (m31 * fA2) + (m33 * fA0);
        outAdjointMatrix.m23 = - (m20 * fA4) + (m21 * fA2) - (m23 * fA0);

	    outAdjointMatrix.m30 = - (m10 * fB3) + (m11 * fB1) - (m12 * fB0);
        outAdjointMatrix.m31 = + (m00 * fB3) - (m01 * fB1) + (m02 * fB0);
        outAdjointMatrix.m32 = - (m30 * fA3) + (m31 * fA1) - (m32 * fA0);
        outAdjointMatrix.m33 = + (m20 * fA3) - (m21 * fA1) + (m22 * fA0);
    #endif
}

template<>
Bool TMatrix4<Float>::Invert(TMatrix4<Float> & outInvMatrix, Float fZeroTolerance) const
{
    Float fA0, fA1, fA2, fA3, fA4, fA5;
    Float fB0, fB1, fB2, fB3, fB4, fB5, fInvDet;

    #ifdef MATH_USE_SSE
        MathSSEFn->Push( m00, m00, m00, m01 );
        MathSSEFn->Push( m11, m12, m13, m12 );
        MathSSEFn->MulPF();
        MathSSEFn->Push( m01, m02, m03, m02 );
        MathSSEFn->Push( m10, m10, m10, m11 );
        MathSSEFn->MulPF();
        MathSSEFn->SubPF();
        MathSSEFn->Pop( fA0, fA1, fA2, fA3 );

        MathSSEFn->Push( m01, m02, m20, m20 );
        MathSSEFn->Push( m13, m13, m31, m32 );
        MathSSEFn->MulPF();
        MathSSEFn->Push( m03, m03, m21, m22 );
        MathSSEFn->Push( m11, m12, m30, m30 );
        MathSSEFn->MulPF();
        MathSSEFn->SubPF();
        MathSSEFn->Pop( fA4, fA5, fB0, fB1 );

        MathSSEFn->Push( m20, m21, m21, m22 );
        MathSSEFn->Push( m33, m32, m33, m33 );
        MathSSEFn->MulPF();
        MathSSEFn->Push( m23, m22, m23, m23 );
        MathSSEFn->Push( m30, m31, m31, m32 );
        MathSSEFn->MulPF();
        MathSSEFn->SubPF();
        MathSSEFn->Pop( fB2, fB3, fB4, fB5 );

        MathSSEFn->Push( fA0, -fA1, fA2, 0.0f );
        MathSSEFn->Push( fB5, fB4, fB3, 0.0f );
        MathSSEFn->MulPF();
        MathSSEFn->Push( fA3, -fA4, fA5, 0.0f );
        MathSSEFn->Push( fB2, fB1, fB0, 0.0f );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->HAddF(0);
        MathSSEFn->HAddF(0);
        MathSSEFn->Pop(fInvDet);
    #else
        fA0 = m00 * m11 - m01 * m10;
        fA1 = m00 * m12 - m02 * m10;
        fA2 = m00 * m13 - m03 * m10;
        fA3 = m01 * m12 - m02 * m11;
        fA4 = m01 * m13 - m03 * m11;
        fA5 = m02 * m13 - m03 * m12;
        fB0 = m20 * m31 - m21 * m30;
        fB1 = m20 * m32 - m22 * m30;
        fB2 = m20 * m33 - m23 * m30;
        fB3 = m21 * m32 - m22 * m31;
        fB4 = m21 * m33 - m23 * m31;
        fB5 = m22 * m33 - m23 * m32;
        fInvDet = ( + fA0*fB5 - fA1*fB4 + fA2*fB3
                    + fA3*fB2 - fA4*fB1 + fA5*fB0 );
    #endif

    if ( MathFFn->Abs(fInvDet) < fZeroTolerance )
        return false;
    fInvDet = MathFFn->Invert(fInvDet);

    #ifdef MATH_USE_SSE
        MathSSEFn->Push( fInvDet, fInvDet, fInvDet, fInvDet );

        MathSSEFn->Push( +m11, -m01, +m31, -m21 );
        MathSSEFn->Push( fB5, fB5, fA5, fA5 );
        MathSSEFn->MulPF();
        MathSSEFn->Push( -m12, +m02, -m32, +m22 );
        MathSSEFn->Push( fB4, fB4, fA4, fA4 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Push( +m13, -m03, +m33, -m23 );
        MathSSEFn->Push( fB3, fB3, fA3, fA3 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->MulF(0, 1);
        MathSSEFn->Pop( outInvMatrix.m00, outInvMatrix.m01, outInvMatrix.m02, outInvMatrix.m03 );

        MathSSEFn->Push( -m10, +m00, -m30, +m20 );
        MathSSEFn->Push( fB5, fB5, fA5, fA5 );
        MathSSEFn->MulPF();
        MathSSEFn->Push( +m12, -m02, +m32, -m22 );
        MathSSEFn->Push( fB2, fB2, fA2, fA2 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Push( -m13, +m03, -m33, +m23 );
        MathSSEFn->Push( fB1, fB1, fA1, fA1 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->MulF(0, 1);
        MathSSEFn->Pop( outInvMatrix.m10, outInvMatrix.m11, outInvMatrix.m12, outInvMatrix.m13 );

        MathSSEFn->Push( +m10, -m00, +m30, -m20 );
        MathSSEFn->Push( fB4, fB4, fA4, fA4 );
        MathSSEFn->MulPF();
        MathSSEFn->Push( -m11, +m01, -m31, +m21 );
        MathSSEFn->Push( fB2, fB2, fA2, fA2 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Push( +m13, -m03, +m33, -m23 );
        MathSSEFn->Push( fB0, fB0, fA0, fA0 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->MulF(0, 1);
        MathSSEFn->Pop( outInvMatrix.m20, outInvMatrix.m21, outInvMatrix.m22, outInvMatrix.m23 );

        MathSSEFn->Push( -m10, +m00, -m30, +m20 );
        MathSSEFn->Push( fB3, fB3, fA3, fA3 );
        MathSSEFn->MulPF();
        MathSSEFn->Push( +m11, -m01, +m31, -m21 );
        MathSSEFn->Push( fB1, fB1, fA1, fA1 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Push( -m12, +m02, -m32, +m22 );
        MathSSEFn->Push( fB0, fB0, fA0, fA0 );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->MulF(0, 1);
        MathSSEFn->Pop( outInvMatrix.m30, outInvMatrix.m31, outInvMatrix.m32, outInvMatrix.m33 );

        MathSSEFn->Pop( fInvDet, fInvDet, fInvDet, fInvDet );
    #else
        outInvMatrix.m00 = ( + (m11 * fB5) - (m12 * fB4) + (m13 * fB3) ) * fInvDet;
        outInvMatrix.m01 = ( - (m01 * fB5) + (m02 * fB4) - (m03 * fB3) ) * fInvDet;
        outInvMatrix.m02 = ( + (m31 * fA5) - (m32 * fA4) + (m33 * fA3) ) * fInvDet;
        outInvMatrix.m03 = ( - (m21 * fA5) + (m22 * fA4) - (m23 * fA3) ) * fInvDet;

        outInvMatrix.m10 = ( - (m10 * fB5) + (m12 * fB2) - (m13 * fB1) ) * fInvDet;
        outInvMatrix.m11 = ( + (m00 * fB5) - (m02 * fB2) + (m03 * fB1) ) * fInvDet;
        outInvMatrix.m12 = ( - (m30 * fA5) + (m32 * fA2) - (m33 * fA1) ) * fInvDet;
        outInvMatrix.m13 = ( + (m20 * fA5) - (m22 * fA2) + (m23 * fA1) ) * fInvDet;

        outInvMatrix.m20 = ( + (m10 * fB4) - (m11 * fB2) + (m13 * fB0) ) * fInvDet;
        outInvMatrix.m21 = ( - (m00 * fB4) + (m01 * fB2) - (m03 * fB0) ) * fInvDet;
        outInvMatrix.m22 = ( + (m30 * fA4) - (m31 * fA2) + (m33 * fA0) ) * fInvDet;
        outInvMatrix.m23 = ( - (m20 * fA4) + (m21 * fA2) - (m23 * fA0) ) * fInvDet;

        outInvMatrix.m30 = ( - (m10 * fB3) + (m11 * fB1) - (m12 * fB0) ) * fInvDet;
        outInvMatrix.m31 = ( + (m00 * fB3) - (m01 * fB1) + (m02 * fB0) ) * fInvDet;
        outInvMatrix.m32 = ( - (m30 * fA3) + (m31 * fA1) - (m32 * fA0) ) * fInvDet;
        outInvMatrix.m33 = ( + (m20 * fA3) - (m21 * fA1) + (m22 * fA0) ) * fInvDet;
    #endif

    return true;
}
template<>
Bool TMatrix4<Double>::Invert(TMatrix4<Double> & outInvMatrix, Double fZeroTolerance) const
{
    Double fA0, fA1, fA2, fA3, fA4, fA5;
    Double fB0, fB1, fB2, fB3, fB4, fB5, fInvDet;

    #ifdef MATH_USE_SSE
        MathSSEFn->Push( m00, m00 );
        MathSSEFn->Push( m11, m12 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m01, m02 );
        MathSSEFn->Push( m10, m10 );
        MathSSEFn->MulPD();
        MathSSEFn->SubPD();
        MathSSEFn->Pop( fA0, fA1 );

        MathSSEFn->Push( m00, m01 );
        MathSSEFn->Push( m13, m12 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m03, m02 );
        MathSSEFn->Push( m10, m11 );
        MathSSEFn->MulPD();
        MathSSEFn->SubPD();
        MathSSEFn->Pop( fA2, fA3 );

        MathSSEFn->Push( m01, m02 );
        MathSSEFn->Push( m13, m13 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m03, m03 );
        MathSSEFn->Push( m11, m12 );
        MathSSEFn->MulPD();
        MathSSEFn->SubPD();
        MathSSEFn->Pop( fA4, fA5 );

        MathSSEFn->Push( m20, m20 );
        MathSSEFn->Push( m31, m32 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m21, m22 );
        MathSSEFn->Push( m30, m30 );
        MathSSEFn->MulPD();
        MathSSEFn->SubPD();
        MathSSEFn->Pop( fB0, fB1 );

        MathSSEFn->Push( m20, m21 );
        MathSSEFn->Push( m33, m32 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m23, m22 );
        MathSSEFn->Push( m30, m31 );
        MathSSEFn->MulPD();
        MathSSEFn->SubPD();
        MathSSEFn->Pop( fB2, fB3 );

        MathSSEFn->Push( m21, m22 );
        MathSSEFn->Push( m33, m33 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( m23, m23 );
        MathSSEFn->Push( m31, m32 );
        MathSSEFn->MulPD();
        MathSSEFn->SubPD();
        MathSSEFn->Pop( fB4, fB5 );

        MathSSEFn->Push( fA0, -fA1 );
        MathSSEFn->Push( fB5, fB4 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( fA2, fA3 );
        MathSSEFn->Push( fB3, fB2 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( -fA4, fA5 );
        MathSSEFn->Push( fB1, fB0 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->HAddD(0);
        MathSSEFn->Pop(fInvDet);
    #else
        fA0 = m00 * m11 - m01 * m10;
        fA1 = m00 * m12 - m02 * m10;
        fA2 = m00 * m13 - m03 * m10;
        fA3 = m01 * m12 - m02 * m11;
        fA4 = m01 * m13 - m03 * m11;
        fA5 = m02 * m13 - m03 * m12;
        fB0 = m20 * m31 - m21 * m30;
        fB1 = m20 * m32 - m22 * m30;
        fB2 = m20 * m33 - m23 * m30;
        fB3 = m21 * m32 - m22 * m31;
        fB4 = m21 * m33 - m23 * m31;
        fB5 = m22 * m33 - m23 * m32;
        fInvDet = ( + fA0*fB5 - fA1*fB4
                    + fA2*fB3 + fA3*fB2
                    - fA4*fB1 + fA5*fB0 );
    #endif

    if ( MathDFn->Abs(fInvDet) < fZeroTolerance )
        return false;
    fInvDet = MathDFn->Invert(fInvDet);

    #ifdef MATH_USE_SSE
        MathSSEFn->Push( fInvDet, fInvDet );

        MathSSEFn->Push( +m11, -m01 );
        MathSSEFn->Push( fB5, fB5 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( -m12, +m02 );
        MathSSEFn->Push( fB4, fB4 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( +m13, -m03 );
        MathSSEFn->Push( fB3, fB3 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop( outInvMatrix.m00, outInvMatrix.m01 );

        MathSSEFn->Push( +m31, -m21 );
        MathSSEFn->Push( fA5, fA5 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( -m32, +m22 );
        MathSSEFn->Push( fA4, fA4 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( +m33, -m23 );
        MathSSEFn->Push( fA3, fA3 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop( outInvMatrix.m02, outInvMatrix.m03 );

        MathSSEFn->Push( -m10, +m00 );
        MathSSEFn->Push( fB5, fB5 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( +m12, -m02 );
        MathSSEFn->Push( fB2, fB2 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( -m13, +m03 );
        MathSSEFn->Push( fB1, fB1 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop( outInvMatrix.m10, outInvMatrix.m11 );

        MathSSEFn->Push( -m30, +m20 );
        MathSSEFn->Push( fA5, fA5 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( +m32, -m22 );
        MathSSEFn->Push( fA2, fA2 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( -m33, +m23 );
        MathSSEFn->Push( fA1, fA1 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop( outInvMatrix.m12, outInvMatrix.m13 );

        MathSSEFn->Push( +m10, -m00 );
        MathSSEFn->Push( fB4, fB4 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( -m11, +m01 );
        MathSSEFn->Push( fB2, fB2 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( +m13, -m03 );
        MathSSEFn->Push( fB0, fB0 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop( outInvMatrix.m20, outInvMatrix.m21 );

        MathSSEFn->Push( +m30, -m20 );
        MathSSEFn->Push( fA4, fA4 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( -m31, +m21 );
        MathSSEFn->Push( fA2, fA2 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( +m33, -m23 );
        MathSSEFn->Push( fA0, fA0 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop( outInvMatrix.m22, outInvMatrix.m23 );

        MathSSEFn->Push( -m10, +m00 );
        MathSSEFn->Push( fB3, fB3 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( +m11, -m01 );
        MathSSEFn->Push( fB1, fB1 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( -m12, +m02 );
        MathSSEFn->Push( fB0, fB0 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop( outInvMatrix.m30, outInvMatrix.m31 );

        MathSSEFn->Push( -m30, +m20 );
        MathSSEFn->Push( fA3, fA3 );
        MathSSEFn->MulPD();
        MathSSEFn->Push( +m31, -m21 );
        MathSSEFn->Push( fA1, fA1 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( -m32, +m22 );
        MathSSEFn->Push( fA0, fA0 );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop( outInvMatrix.m32, outInvMatrix.m33 );

        MathSSEFn->Pop( fInvDet, fInvDet );
    #else
        outInvMatrix.m00 = ( + (m11 * fB5) - (m12 * fB4) + (m13 * fB3) ) * fInvDet;
        outInvMatrix.m01 = ( - (m01 * fB5) + (m02 * fB4) - (m03 * fB3) ) * fInvDet;
        outInvMatrix.m02 = ( + (m31 * fA5) - (m32 * fA4) + (m33 * fA3) ) * fInvDet;
        outInvMatrix.m03 = ( - (m21 * fA5) + (m22 * fA4) - (m23 * fA3) ) * fInvDet;

        outInvMatrix.m10 = ( - (m10 * fB5) + (m12 * fB2) - (m13 * fB1) ) * fInvDet;
        outInvMatrix.m11 = ( + (m00 * fB5) - (m02 * fB2) + (m03 * fB1) ) * fInvDet;
        outInvMatrix.m12 = ( - (m30 * fA5) + (m32 * fA2) - (m33 * fA1) ) * fInvDet;
        outInvMatrix.m13 = ( + (m20 * fA5) - (m22 * fA2) + (m23 * fA1) ) * fInvDet;

        outInvMatrix.m20 = ( + (m10 * fB4) - (m11 * fB2) + (m13 * fB0) ) * fInvDet;
        outInvMatrix.m21 = ( - (m00 * fB4) + (m01 * fB2) - (m03 * fB0) ) * fInvDet;
        outInvMatrix.m22 = ( + (m30 * fA4) - (m31 * fA2) + (m33 * fA0) ) * fInvDet;
        outInvMatrix.m23 = ( - (m20 * fA4) + (m21 * fA2) - (m23 * fA0) ) * fInvDet;

        outInvMatrix.m30 = ( - (m10 * fB3) + (m11 * fB1) - (m12 * fB0) ) * fInvDet;
        outInvMatrix.m31 = ( + (m00 * fB3) - (m01 * fB1) + (m02 * fB0) ) * fInvDet;
        outInvMatrix.m32 = ( - (m30 * fA3) + (m31 * fA1) - (m32 * fA0) ) * fInvDet;
        outInvMatrix.m33 = ( + (m20 * fA3) - (m21 * fA1) + (m22 * fA0) ) * fInvDet;
    #endif

    return true;
}
