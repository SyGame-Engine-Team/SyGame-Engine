/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Transform/Transform3.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Generic transformations in 3D
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Transform3.h"

/////////////////////////////////////////////////////////////////////////////////
// TTransform3 implementation

/////////////////////////////////////////////////////////////////////////////////

template<>
Void TTransform3<Float>::_UpdateHMatrix()
{
    if ( m_bIsIdentity )
        m_matHMatrix = TMatrix4<Float>::Identity;
    else {
        m_matHMatrix = m_matTransform;
        if ( m_bIsRotScale ) {
            #ifdef MATH_USE_SSE
                MathSSEFn->Push( m_vScale.X, m_vScale.Y, m_vScale.Z );

                MathSSEFn->Push( m_matHMatrix.m00, m_matHMatrix.m01, m_matHMatrix.m02 );
                MathSSEFn->MulF(0, 1);
                MathSSEFn->Pop( m_matHMatrix.m00, m_matHMatrix.m01, m_matHMatrix.m02 );

                MathSSEFn->Push( m_matHMatrix.m10, m_matHMatrix.m11, m_matHMatrix.m12 );
                MathSSEFn->MulF(0, 1);
                MathSSEFn->Pop( m_matHMatrix.m10, m_matHMatrix.m11, m_matHMatrix.m12 );

                MathSSEFn->Push( m_matHMatrix.m20, m_matHMatrix.m21, m_matHMatrix.m22 );
                MathSSEFn->MulF(0, 1);
                MathSSEFn->Pop( m_matHMatrix.m20, m_matHMatrix.m21, m_matHMatrix.m22 );

                MathSSEFn->PopF();
            #else
                m_matHMatrix.m00 *= m_vScale.X;
                m_matHMatrix.m01 *= m_vScale.Y;
                m_matHMatrix.m02 *= m_vScale.Z;
                m_matHMatrix.m10 *= m_vScale.X;
                m_matHMatrix.m11 *= m_vScale.Y;
                m_matHMatrix.m12 *= m_vScale.Z;
                m_matHMatrix.m20 *= m_vScale.X;
                m_matHMatrix.m21 *= m_vScale.Y;
                m_matHMatrix.m22 *= m_vScale.Z;
            #endif
        }
        m_matHMatrix.m03 = m_vTranslate.X;
        m_matHMatrix.m13 = m_vTranslate.Y;
        m_matHMatrix.m23 = m_vTranslate.Z;
        // last row is allways (0, 0, 0, 1)
    }
    m_bUpdateInverse = true;
}
template<>
Void TTransform3<Double>::_UpdateHMatrix()
{
    if ( m_bIsIdentity )
        m_matHMatrix = TMatrix4<Double>::Identity;
    else {
        m_matHMatrix = m_matTransform;
        if ( m_bIsRotScale ) {
            #ifdef MATH_USE_SSE
                MathSSEFn->Push( m_vScale.X, m_vScale.Y );

                MathSSEFn->Push( m_matHMatrix.m00, m_matHMatrix.m01 );
                MathSSEFn->MulD(0, 1);
                MathSSEFn->Pop( m_matHMatrix.m00, m_matHMatrix.m01 );

                MathSSEFn->Push( m_matHMatrix.m10, m_matHMatrix.m11 );
                MathSSEFn->MulD(0, 1);
                MathSSEFn->Pop( m_matHMatrix.m10, m_matHMatrix.m11 );

                MathSSEFn->Push( m_matHMatrix.m20, m_matHMatrix.m21 );
                MathSSEFn->MulD(0, 1);
                MathSSEFn->Pop( m_matHMatrix.m20, m_matHMatrix.m21 );

                MathSSEFn->PopD();

                MathSSEFn->Push( m_vScale.Z );

                MathSSEFn->Push( m_matHMatrix.m02 );
                MathSSEFn->MulD(0, 1);
                MathSSEFn->Pop( m_matHMatrix.m02 );

                MathSSEFn->Push( m_matHMatrix.m12 );
                MathSSEFn->MulD(0, 1);
                MathSSEFn->Pop( m_matHMatrix.m12 );

                MathSSEFn->Push( m_matHMatrix.m22 );
                MathSSEFn->MulD(0, 1);
                MathSSEFn->Pop( m_matHMatrix.m22 );

                MathSSEFn->PopD();
            #else
                m_matHMatrix.m00 *= m_vScale.X;
                m_matHMatrix.m01 *= m_vScale.Y;
                m_matHMatrix.m02 *= m_vScale.Z;
                m_matHMatrix.m10 *= m_vScale.X;
                m_matHMatrix.m11 *= m_vScale.Y;
                m_matHMatrix.m12 *= m_vScale.Z;
                m_matHMatrix.m20 *= m_vScale.X;
                m_matHMatrix.m21 *= m_vScale.Y;
                m_matHMatrix.m22 *= m_vScale.Z;
            #endif
        }
        m_matHMatrix.m03 = m_vTranslate.X;
        m_matHMatrix.m13 = m_vTranslate.Y;
        m_matHMatrix.m23 = m_vTranslate.Z;
        // last row is allways (0, 0, 0, 1)
    }
    m_bUpdateInverse = true;
}

