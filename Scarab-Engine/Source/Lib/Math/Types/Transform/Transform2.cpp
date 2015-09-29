/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Transform/Transform2.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Generic transformations in 2D
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
#include "Transform2.h"

/////////////////////////////////////////////////////////////////////////////////
// TTransform2 implementation

/////////////////////////////////////////////////////////////////////////////////

template<>
Void TTransform2<Float>::_UpdateHMatrix()
{
    if ( m_bIsIdentity )
        m_matHMatrix = TMatrix3<Float>::Identity;
    else {
        m_matHMatrix = m_matTransform;
        if ( m_bIsRotScale ) {
            #ifdef MATH_USE_SSE
                MathSSEFn->Push( m_matHMatrix.m00, m_matHMatrix.m01, m_matHMatrix.m10, m_matHMatrix.m11 );
                MathSSEFn->Push( m_vScale.X, m_vScale.Y, m_vScale.X, m_vScale.Y );
                MathSSEFn->MulPF();
                MathSSEFn->Pop( m_matHMatrix.m00, m_matHMatrix.m01, m_matHMatrix.m10, m_matHMatrix.m11 );
            #else
                m_matHMatrix.m00 *= m_vScale.X;
                m_matHMatrix.m01 *= m_vScale.Y;
                m_matHMatrix.m10 *= m_vScale.X;
                m_matHMatrix.m11 *= m_vScale.Y;
            #endif
        }
        m_matHMatrix.m02 = m_vTranslate.X;
        m_matHMatrix.m12 = m_vTranslate.Y;
        // last row is allways (0, 0, 1)
    }
    m_bUpdateInverse = true;
}
template<>
Void TTransform2<Double>::_UpdateHMatrix()
{
    if ( m_bIsIdentity )
        m_matHMatrix = TMatrix3<Double>::Identity;
    else {
        m_matHMatrix = m_matTransform;
        if ( m_bIsRotScale ) {
            #ifdef MATH_USE_SSE
                MathSSEFn->Push( m_matHMatrix.m00, m_matHMatrix.m01 );
                MathSSEFn->Push( m_vScale.X, m_vScale.Y );
                MathSSEFn->MulPD();
                MathSSEFn->Pop( m_matHMatrix.m00, m_matHMatrix.m01 );

                MathSSEFn->Push( m_matHMatrix.m10, m_matHMatrix.m11 );
                MathSSEFn->Push( m_vScale.X, m_vScale.Y );
                MathSSEFn->MulPD();
                MathSSEFn->Pop( m_matHMatrix.m10, m_matHMatrix.m11 );
            #else
                m_matHMatrix.m00 *= m_vScale.X;
                m_matHMatrix.m01 *= m_vScale.Y;
                m_matHMatrix.m10 *= m_vScale.X;
                m_matHMatrix.m11 *= m_vScale.Y;
            #endif
        }
        m_matHMatrix.m02 = m_vTranslate.X;
        m_matHMatrix.m12 = m_vTranslate.Y;
        // last row is allways (0, 0, 1)
    }
    m_bUpdateInverse = true;
}

