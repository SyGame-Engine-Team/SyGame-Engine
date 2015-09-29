/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Algebra/Matrix/EulerDecomposition.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Euler UDU and UDV decompositions for 3x3 matrices
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TEulerDecomposition implementation
template<typename Real>
const Real TEulerDecomposition<Real>::ZERO = (Real)0;
template<typename Real>
const Real TEulerDecomposition<Real>::ONE = (Real)1;
template<typename Real>
const Real TEulerDecomposition<Real>::PI_2 = (Real)SCALAR_PI_2;
template<typename Real>
const Real TEulerDecomposition<Real>::PI = (Real)SCALAR_PI;

template<typename Real>
TEulerDecomposition<Real>::TEulerDecomposition( const TMatrix3<Real> & matRotation ):
    m_matRotation( matRotation )
{
    // nothing to do
}
template<typename Real>
TEulerDecomposition<Real>::~TEulerDecomposition()
{
    // nothing to do
}

template<typename Real>
EulerResult TEulerDecomposition<Real>::ExtractXYZ( Real & outYaw, Real & outPitch, Real & outRoll ) const
{
    if ( m_matRotation.m02 < ONE ) {
        if ( m_matRotation.m02 > -ONE ) {
            outYaw = MathRealFn->ArcTan2( -m_matRotation.m12, m_matRotation.m22 );
            outPitch = MathRealFn->ArcSin( m_matRotation.m02 );
            outRoll = MathRealFn->ArcTan2( -m_matRotation.m01, m_matRotation.m00 );
            return EULER_UNIQUE;
        } else {
            outYaw = -MathRealFn->ArcTan2( m_matRotation.m10, m_matRotation.m11 );
            outPitch = -PI_2;
            outRoll = ZERO;
            return EULER_CONST_DIFF;
        }
    } else {
        outYaw = MathRealFn->ArcTan2( m_matRotation.m10, m_matRotation.m11 );
        outPitch = PI_2;
        outRoll = ZERO;
        return EULER_CONST_SUM;
    }
}
template<typename Real>
EulerResult TEulerDecomposition<Real>::ExtractXZY( Real & outYaw, Real & outPitch, Real & outRoll ) const
{
    if ( m_matRotation.m01 < ONE ) {
        if ( m_matRotation.m01 > -ONE ) {
            outYaw = MathRealFn->ArcTan2( m_matRotation.m21, m_matRotation.m11 );
            outPitch = MathRealFn->ArcTan2( m_matRotation.m02, m_matRotation.m00 );
            outRoll = MathRealFn->ArcSin( -m_matRotation.m01 );
            return EULER_UNIQUE;
        } else {
            outYaw = -MathRealFn->ArcTan2( -m_matRotation.m20, m_matRotation.m22 );
            outPitch = ZERO;
            outRoll = PI_2;
            return EULER_CONST_DIFF;
        }
    } else {
        outYaw = MathRealFn->ArcTan2( -m_matRotation.m20, m_matRotation.m22 );
        outPitch = ZERO;
        outRoll = -PI_2;
        return EULER_CONST_SUM;
    }
}
template<typename Real>
EulerResult TEulerDecomposition<Real>::ExtractYXZ( Real & outYaw, Real & outPitch, Real & outRoll ) const
{
    if ( m_matRotation.m12 < ONE ) {
        if ( m_matRotation.m12 > -ONE ) {
            outYaw = MathRealFn->ArcSin( -m_matRotation.m12 );
            outPitch = MathRealFn->ArcTan2( m_matRotation.m02, m_matRotation.m22 );
            outRoll = MathRealFn->ArcTan2( m_matRotation.m10, m_matRotation.m11 );
            return EULER_UNIQUE;
        } else {
            outYaw = PI_2;
            outPitch = -MathRealFn->ArcTan2( -m_matRotation.m01, m_matRotation.m00 );
            outRoll = ZERO;
            return EULER_CONST_DIFF;
        }
    } else {
        outYaw = -PI_2;
        outPitch = MathRealFn->ArcTan2( -m_matRotation.m01, m_matRotation.m00 );
        outRoll = ZERO;
        return EULER_CONST_SUM;
    }
}
template<typename Real>
EulerResult TEulerDecomposition<Real>::ExtractYZX( Real & outYaw, Real & outPitch, Real & outRoll ) const
{
    if ( m_matRotation.m10 < ONE ) {
        if ( m_matRotation.m10 > -ONE ) {
            outYaw = MathRealFn->ArcTan2( -m_matRotation.m12, m_matRotation.m11 );
            outPitch = MathRealFn->ArcTan2( -m_matRotation.m20, m_matRotation.m00 );
            outRoll = MathRealFn->ArcSin( m_matRotation.m10 );
            return EULER_UNIQUE;
        } else {
            outYaw = ZERO;
            outPitch = -MathRealFn->ArcTan2( m_matRotation.m21, m_matRotation.m22 );
            outRoll = -PI_2;
            return EULER_CONST_DIFF;
        }
    } else {
        outYaw = ZERO;
        outPitch = MathRealFn->ArcTan2( m_matRotation.m21, m_matRotation.m22 );
        outRoll = PI_2;
        return EULER_CONST_SUM;
    }
}
template<typename Real>
EulerResult TEulerDecomposition<Real>::ExtractZXY( Real & outYaw, Real & outPitch, Real & outRoll ) const
{
    if ( m_matRotation.m21 < ONE ) {
        if ( m_matRotation.m21 > -ONE ) {
            outYaw = MathRealFn->ArcSin( m_matRotation.m21 );
            outPitch = MathRealFn->ArcTan2( -m_matRotation.m20, m_matRotation.m22 );
            outRoll = MathRealFn->ArcTan2( -m_matRotation.m01, m_matRotation.m11 );
            return EULER_UNIQUE;
        } else {
            outYaw = -PI_2;
            outPitch = ZERO;
            outRoll = -MathRealFn->ArcTan2( m_matRotation.m02, m_matRotation.m00 );
            return EULER_CONST_DIFF;
        }
    } else {
        outYaw = PI_2;
        outPitch = ZERO;
        outRoll = MathRealFn->ArcTan2( m_matRotation.m02, m_matRotation.m00 );
        return EULER_CONST_SUM;
    }
}
template<typename Real>
EulerResult TEulerDecomposition<Real>::ExtractZYX( Real & outYaw, Real & outPitch, Real & outRoll ) const
{
    if ( m_matRotation.m20 < ONE ) {
        if ( m_matRotation.m20 > -ONE ) {
            outYaw = MathRealFn->ArcTan2( m_matRotation.m21, m_matRotation.m22 );
            outPitch = MathRealFn->ArcSin( -m_matRotation.m20 );
            outRoll = MathRealFn->ArcTan2( m_matRotation.m10, m_matRotation.m00 );
            return EULER_UNIQUE;
        } else {
            outYaw = ZERO;
            outPitch = PI_2;
            outRoll = -MathRealFn->ArcTan2( m_matRotation.m01, m_matRotation.m02 );
            return EULER_CONST_DIFF;
        }
    } else {
        outYaw = ZERO;
        outPitch = -PI_2;
        outRoll = MathRealFn->ArcTan2( -m_matRotation.m01, -m_matRotation.m02 );
        return EULER_CONST_SUM;
    }
}

//template<typename Real>
//EulerResult TEulerDecomposition<Real>::ExtractXYX( Real & outYaw, Real & outPitch, Real & outRoll ) const
//{
//    if ( m_matRotation.m00 < ONE ) {
//        if ( m_matRotation.m00 > -ONE ) {
//            outYaw = MathRealFn->ArcTan2( m_matRotation.m10, -m_matRotation.m20 );
//            outPitch = MathRealFn->ArcCos( m_matRotation.m00 );
//            outRoll = MathRealFn->ArcTan2( m_matRotation.m01, m_matRotation.m02 );
//            return EULER_UNIQUE;
//        } else {
//            outYaw = -MathRealFn->ArcTan2( -m_matRotation.m12, m_matRotation.m11 );
//            outPitch = PI;
//            outRoll = ZERO;
//            return EULER_CONST_DIFF;
//        }
//    } else {
//        outYaw = MathRealFn->ArcTan2( -m_matRotation.m12, m_matRotation.m11 );
//        outPitch = ZERO;
//        outRoll = ZERO;
//        return EULER_CONST_SUM;
//    }
//}
//template<typename Real>
//EulerResult TEulerDecomposition<Real>::ExtractXZX( Real & outYaw, Real & outPitch, Real & outRoll ) const
//{
//    if ( m_matRotation.m00 < ONE ) {
//        if ( m_matRotation.m00 > -ONE ) {
//            outYaw = MathRealFn->ArcTan2( m_matRotation.m20, m_matRotation.m10 );
//            outPitch = MathRealFn->ArcCos( m_matRotation.m00 );
//            outRoll = MathRealFn->ArcTan2( m_matRotation.m02, -m_matRotation.m01 );
//            return EULER_UNIQUE;
//        } else {
//            outYaw = -MathRealFn->ArcTan2( m_matRotation.m21, m_matRotation.m22 );
//            outPitch = PI;
//            outRoll = ZERO;
//            return EULER_CONST_DIFF;
//        }
//    } else {
//        outYaw = MathRealFn->ArcTan2( m_matRotation.m21, m_matRotation.m22 );
//        outPitch = ZERO;
//        outRoll = ZERO;
//        return EULER_CONST_SUM;
//    }
//}
//template<typename Real>
//EulerResult TEulerDecomposition<Real>::ExtractYXY( Real & outYaw, Real & outPitch, Real & outRoll ) const
//{
//    if ( m_matRotation.m11 < ONE ) {
//        if ( m_matRotation.m11 > -ONE ) {
//            outYaw = MathRealFn->ArcTan2( m_matRotation.m01, m_matRotation.m21 );
//            outPitch = MathRealFn->ArcCos( m_matRotation.m11 );
//            outRoll = MathRealFn->ArcTan2( m_matRotation.m10, -m_matRotation.m12 );
//            return EULER_UNIQUE;
//        } else {
//            outYaw = -MathRealFn->ArcTan2( m_matRotation.m02, m_matRotation.m00 );
//            outPitch = PI;
//            outRoll = ZERO;
//            return EULER_CONST_DIFF;
//        }
//    } else {
//        outYaw = MathRealFn->ArcTan2( m_matRotation.m02, m_matRotation.m00 );
//        outPitch = ZERO;
//        outRoll = ZERO;
//        return EULER_CONST_SUM;
//    }
//}
//template<typename Real>
//EulerResult TEulerDecomposition<Real>::ExtractYZY( Real & outYaw, Real & outPitch, Real & outRoll ) const
//{
//    if ( m_matRotation.m11 < ONE ) {
//        if ( m_matRotation.m11 > -ONE ) {
//            outYaw = MathRealFn->ArcTan2( m_matRotation.m21, -m_matRotation.m01 );
//            outPitch = MathRealFn->ArcCos( m_matRotation.m11 );
//            outRoll = MathRealFn->ArcTan2( m_matRotation.m12, m_matRotation.m10 );
//            return EULER_UNIQUE;
//        } else {
//            outYaw = -MathRealFn->ArcTan2( -m_matRotation.m20, m_matRotation.m22 );
//            outPitch = PI;
//            outRoll = ZERO;
//            return EULER_CONST_DIFF;
//        }
//    } else {
//        outYaw = MathRealFn->ArcTan2( -m_matRotation.m20, m_matRotation.m22 );
//        outPitch = ZERO;
//        outRoll = ZERO;
//        return EULER_CONST_SUM;
//    }
//}
//template<typename Real>
//EulerResult TEulerDecomposition<Real>::ExtractZXZ( Real & outYaw, Real & outPitch, Real & outRoll ) const
//{
//    if ( m_matRotation.m22 < ONE ) {
//        if ( m_matRotation.m22 > -ONE ) {
//            outYaw = MathRealFn->ArcTan2( m_matRotation.m02, -m_matRotation.m12 );
//            outPitch = MathRealFn->ArcCos( m_matRotation.m22 );
//            outRoll = MathRealFn->ArcTan2( m_matRotation.m20, m_matRotation.m21 );
//            return EULER_UNIQUE;
//        } else {
//            outYaw = -MathRealFn->ArcTan2( -m_matRotation.m01, m_matRotation.m00 );
//            outPitch = PI;
//            outRoll = ZERO;
//            return EULER_CONST_DIFF;
//        }
//    } else {
//        outYaw = MathRealFn->ArcTan2( -m_matRotation.m01, m_matRotation.m00 );
//        outPitch = ZERO;
//        outRoll = ZERO;
//        return EULER_CONST_SUM;
//    }
//}
//template<typename Real>
//EulerResult TEulerDecomposition<Real>::ExtractZYZ( Real & outYaw, Real & outPitch, Real & outRoll ) const
//{
//    if ( m_matRotation.m22 < ONE ) {
//        if ( m_matRotation.m22 > -ONE ) {
//            outYaw = MathRealFn->ArcTan2( m_matRotation.m12, m_matRotation.m02 );
//            outPitch = MathRealFn->ArcCos( m_matRotation.m22 );
//            outRoll = MathRealFn->ArcTan2( m_matRotation.m21, -m_matRotation.m20 );
//            return EULER_UNIQUE;
//        } else {
//            outYaw = -MathRealFn->ArcTan2( m_matRotation.m10, m_matRotation.m11 );
//            outPitch = PI;
//            outRoll = ZERO;
//            return EULER_CONST_DIFF;
//        }
//    } else {
//        outYaw = MathRealFn->ArcTan2( m_matRotation.m10, m_matRotation.m11 );
//        outPitch = ZERO;
//        outRoll = ZERO;
//        return EULER_CONST_SUM;
//    }
//}
