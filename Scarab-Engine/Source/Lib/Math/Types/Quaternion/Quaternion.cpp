/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Quaternion/Quaternion.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Homogeneous 4D quaternion
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
#include "Quaternion.h"

/////////////////////////////////////////////////////////////////////////////////
// TQuaternion implementation
template<>
TQuaternion<Float> TQuaternion<Float>::operator*(const TQuaternion<Float> & rhs) const
{
    #ifdef MATH_USE_SSE
        TQuaternion<Float> qRes;
        MathSSEFn->Push( W, W, W, W );
        MathSSEFn->Push( rhs.X, rhs.Y, rhs.Z, rhs.W );
        MathSSEFn->MulPF();
        MathSSEFn->Push( X, Y, Z, -X );
        MathSSEFn->Push( rhs.W, rhs.W, rhs.W, rhs.X );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Push( Y, Z, X, -Y );
        MathSSEFn->Push( rhs.Z, rhs.X, rhs.Y, rhs.Y );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Push( -Z, -X, -Y, -Z );
        MathSSEFn->Push( rhs.Y, rhs.Z, rhs.X, rhs.Z );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Pop( qRes.X, qRes.Y, qRes.Z, qRes.W );
        return qRes;
    #else
        return TQuaternion<Float> (
		    (W * rhs.X) + (X * rhs.W) + (Y * rhs.Z - Z * rhs.Y),
		    (W * rhs.Y) + (Y * rhs.W) + (Z * rhs.X - X * rhs.Z),
		    (W * rhs.Z) + (Z * rhs.W) + (X * rhs.Y - Y * rhs.X),
		    (W * rhs.W) - (X * rhs.X + Y * rhs.Y + Z * rhs.Z)
	    );
    #endif
}
template<>
TQuaternion<Double> TQuaternion<Double>::operator*(const TQuaternion<Double> & rhs) const
{
    #ifdef MATH_USE_SSE
        TQuaternion<Double> qRes;
        MathSSEFn->Push( W, W );
        MathSSEFn->Push( rhs.X, rhs.Y );
        MathSSEFn->MulPD();
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs.W, rhs.W );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( Y, Z );
        MathSSEFn->Push( rhs.Z, rhs.X );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( -Z, -X );
        MathSSEFn->Push( rhs.Y, rhs.Z );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Pop( qRes.X, qRes.Y );
        MathSSEFn->Push( W, W );
        MathSSEFn->Push( rhs.Z, rhs.W );
        MathSSEFn->MulPD();
        MathSSEFn->Push( Z, -X );
        MathSSEFn->Push( rhs.W, rhs.X );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( X, -Y );
        MathSSEFn->Push( rhs.Y, rhs.Y );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( -Y, -Z );
        MathSSEFn->Push( rhs.X, rhs.Z );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Pop( qRes.Z, qRes.W );
        return qRes;
    #else
        return TQuaternion<Double> (
		    (W * rhs.X) + (X * rhs.W) + (Y * rhs.Z - Z * rhs.Y),
		    (W * rhs.Y) + (Y * rhs.W) + (Z * rhs.X - X * rhs.Z),
		    (W * rhs.Z) + (Z * rhs.W) + (X * rhs.Y - Y * rhs.X),
		    (W * rhs.W) - (X * rhs.X + Y * rhs.Y + Z * rhs.Z)
	    );
    #endif
}

template<>
TQuaternion<Float> & TQuaternion<Float>::operator*=(const TQuaternion<Float> & rhs)
{
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( W, W, W, W );
        MathSSEFn->Push( rhs.X, rhs.Y, rhs.Z, rhs.W );
        MathSSEFn->MulPF();
        MathSSEFn->Push( X, Y, Z, -X );
        MathSSEFn->Push( rhs.W, rhs.W, rhs.W, rhs.X );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Push( Y, Z, X, -Y );
        MathSSEFn->Push( rhs.Z, rhs.X, rhs.Y, rhs.Y );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Push( -Z, -X, -Y, -Z );
        MathSSEFn->Push( rhs.Y, rhs.Z, rhs.X, rhs.Z );
        MathSSEFn->MulPF();
        MathSSEFn->AddPF();
        MathSSEFn->Pop( X, Y, Z, W );
    #else
        Float f0 = (W * rhs.X) + (X * rhs.W) + (Y * rhs.Z - Z * rhs.Y);
        Float f1 = (W * rhs.Y) + (Y * rhs.W) + (Z * rhs.X - X * rhs.Z);
        Float f2 = (W * rhs.Z) + (Z * rhs.W) + (X * rhs.Y - Y * rhs.X);
        Float f3 = (W * rhs.W) - (X * rhs.X + Y * rhs.Y + Z * rhs.Z);
        X = f0;
        Y = f1;
        Z = f2;
        W = f3;
    #endif
    return (*this);
}
template<>
TQuaternion<Double> & TQuaternion<Double>::operator*=(const TQuaternion<Double> & rhs)
{
    #ifdef MATH_USE_SSE
        MathSSEFn->Push( W, W );
        MathSSEFn->Push( rhs.X, rhs.Y );
        MathSSEFn->MulPD();
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( rhs.W, rhs.W );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( Y, Z );
        MathSSEFn->Push( rhs.Z, rhs.X );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( -Z, -X );
        MathSSEFn->Push( rhs.Y, rhs.Z );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();

        MathSSEFn->Push( W, W );
        MathSSEFn->Push( rhs.Z, rhs.W );
        MathSSEFn->MulPD();
        MathSSEFn->Push( Z, -X );
        MathSSEFn->Push( rhs.W, rhs.X );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( X, -Y );
        MathSSEFn->Push( rhs.Y, rhs.Y );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();
        MathSSEFn->Push( -Y, -Z );
        MathSSEFn->Push( rhs.X, rhs.Z );
        MathSSEFn->MulPD();
        MathSSEFn->AddPD();

        MathSSEFn->Pop( Z, W );
        MathSSEFn->Pop( X, Y );
    #else
        Double f0 = (W * rhs.X) + (X * rhs.W) + (Y * rhs.Z - Z * rhs.Y);
        Double f1 = (W * rhs.Y) + (Y * rhs.W) + (Z * rhs.X - X * rhs.Z);
        Double f2 = (W * rhs.Z) + (Z * rhs.W) + (X * rhs.Y - Y * rhs.X);
        Double f3 = (W * rhs.W) - (X * rhs.X + Y * rhs.Y + Z * rhs.Z);
        X = f0;
        Y = f1;
        Z = f2;
        W = f3;
    #endif
    return (*this);
}

/////////////////////////////////////////////////////////////////////////////////

template<>
Void TQuaternion<Float>::_Quaternion_To_Matrix(TMatrix3<Float> & outRot) const
{
    Float fTx, fTy, fTz;
    Float fTwx, fTwy, fTwz, fTxx, fTxy, fTxz, fTyy, fTyz, fTzz;

    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y, Z );
        MathSSEFn->Push( 2.0f, 2.0f, 2.0f );
        MathSSEFn->MulPF();
        MathSSEFn->Get( 0, fTx, fTy, fTz );
        MathSSEFn->Push( W, W, W );
        MathSSEFn->MulF(0, 1);
        MathSSEFn->Pop( fTwx, fTwy, fTwz );
        MathSSEFn->Push( X, X, X );
        MathSSEFn->MulPF();
        MathSSEFn->Pop( fTxx, fTxy, fTxz );
        MathSSEFn->Push( fTy, fTz, fTz );
        MathSSEFn->Push( Y, Y, Z );
        MathSSEFn->MulPF();
        MathSSEFn->Pop( fTyy, fTyz, fTzz );

        MathSSEFn->Push( 1.0f, 1.0f, 1.0f );
        MathSSEFn->Push( fTyy, fTxx, fTxx );
        MathSSEFn->Push( fTzz, fTzz, fTyy );
        MathSSEFn->AddPF();
        MathSSEFn->SubPF();
        MathSSEFn->Pop( outRot.m00, outRot.m11, outRot.m22 );

        MathSSEFn->Push( fTxy, fTxz, fTxy );
        MathSSEFn->Push( -fTwz, fTwy, fTwz );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( outRot.m01, outRot.m02, outRot.m10 );

        MathSSEFn->Push( fTyz, fTxz, fTyz );
        MathSSEFn->Push( -fTwx, -fTwy, fTwx );
        MathSSEFn->AddPF();
        MathSSEFn->Pop( outRot.m12, outRot.m20, outRot.m21 );
    #else
        fTx  = X * 2.0f;
        fTy  = Y * 2.0f;
        fTz  = Z * 2.0f;
        fTwx = W * fTx;
        fTwy = W * fTy;
        fTwz = W * fTz;
        fTxx = X * fTx;
        fTxy = X * fTy;
        fTxz = X * fTz;
        fTyy = Y * fTy;
        fTyz = Y * fTz;
        fTzz = Z * fTz;

        outRot.m00 = 1.0f - (fTyy + fTzz); outRot.m01 = fTxy - fTwz;          outRot.m02 = fTxz + fTwy;
        outRot.m10 = fTxy + fTwz;          outRot.m11 = 1.0f - (fTxx + fTzz); outRot.m12 = fTyz - fTwx;
        outRot.m20 = fTxz - fTwy;          outRot.m21 = fTyz + fTwx;          outRot.m22 = 1.0f - (fTxx + fTyy);
    #endif    
}

template<>
Void TQuaternion<Double>::_Quaternion_To_Matrix(TMatrix3<Double> & outRot) const
{
    Double fTx, fTy, fTz;
    Double fTwx, fTwy, fTwz, fTxx, fTxy, fTxz, fTyy, fTyz, fTzz;

    #ifdef MATH_USE_SSE
        MathSSEFn->Push( X, Y );
        MathSSEFn->Push( 2.0, 2.0 );
        MathSSEFn->MulPD();
        MathSSEFn->Get( 0, fTx, fTy );
        MathSSEFn->Push( W, W );
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop( fTwx, fTwy );
        MathSSEFn->Push( X, X );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( fTxx, fTxy );

        MathSSEFn->Push( Z, Z );
        MathSSEFn->Push( 2.0, 2.0 );
        MathSSEFn->MulPD();
        MathSSEFn->Get( 0, fTz, fTz );
        MathSSEFn->Push( W, X );
        MathSSEFn->MulD(0, 1);
        MathSSEFn->Pop( fTwz, fTxz );
        MathSSEFn->Push( Z, Z );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( fTzz, fTzz );

        MathSSEFn->Push( fTy, fTz );
        MathSSEFn->Push( Y, Y );
        MathSSEFn->MulPD();
        MathSSEFn->Pop( fTyy, fTyz );

        MathSSEFn->Push( 1.0, 1.0 );
        MathSSEFn->Push( fTyy, fTxx );
        MathSSEFn->Push( fTzz, fTzz );
        MathSSEFn->AddPD();
        MathSSEFn->SubPD();
        MathSSEFn->Pop( outRot.m00, outRot.m11 );
        MathSSEFn->Push( 1.0 );
        MathSSEFn->Push( fTxx );
        MathSSEFn->Push( fTyy );
        MathSSEFn->AddPD();
        MathSSEFn->SubPD();
        MathSSEFn->Pop( outRot.m22 );

        MathSSEFn->Push( fTxy, fTxz );
        MathSSEFn->Push( -fTwz, fTwy );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( outRot.m01, outRot.m02 );

        MathSSEFn->Push( fTxy, fTyz );
        MathSSEFn->Push( fTwz, -fTwx );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( outRot.m10, outRot.m12 );

        MathSSEFn->Push( fTxz, fTyz );
        MathSSEFn->Push( -fTwy, fTwx );
        MathSSEFn->AddPD();
        MathSSEFn->Pop( outRot.m20, outRot.m21 );
    #else
        fTx  = X * 2.0;
        fTy  = Y * 2.0;
        fTz  = Z * 2.0;
        fTwx = W * fTx;
        fTwy = W * fTy;
        fTwz = W * fTz;
        fTxx = X * fTx;
        fTxy = X * fTy;
        fTxz = X * fTz;
        fTyy = Y * fTy;
        fTyz = Y * fTz;
        fTzz = Z * fTz;

        outRot.m00 = 1.0 - (fTyy + fTzz); outRot.m01 = fTxy - fTwz;         outRot.m02 = fTxz + fTwy;
        outRot.m10 = fTxy + fTwz;         outRot.m11 = 1.0 - (fTxx + fTzz); outRot.m12 = fTyz - fTwx;
        outRot.m20 = fTxz - fTwy;         outRot.m21 = fTyz + fTwx;         outRot.m22 = 1.0 - (fTxx + fTyy);
    #endif    
}


