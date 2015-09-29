/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Joints/JointGeneric6DOF.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Joints : Generic 6-DOF joint with limits & motors
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
#include "JointGeneric6DOF.h"

/////////////////////////////////////////////////////////////////////////////////
// JointGeneric6DOF implementation
JointGeneric6DOF::JointGeneric6DOF( BodyNode * pBodyA, BodyNode * pBodyB, const Transform3 & vJointFrame, EulerAngles iEulerAnglesOrdering ):
    JointConstraint( pBodyA, pBodyB, vJointFrame ),
    m_vTranslation(), m_vRotationEulerAngles()
{
    m_iEulerAnglesOrdering = iEulerAnglesOrdering;

    m_hLinearLMS[0].MakeLinear();
    m_hLinearLMS[1].MakeLinear();
    m_hLinearLMS[2].MakeLinear();
    m_hAngularLMS[0].MakeAngular();
    m_hAngularLMS[1].MakeAngular();
    m_hAngularLMS[2].MakeAngular();

    // Let derived / user classes do this so they get a chance to do some init before.
    // Also, their virtual override isn't accessible before ctor returns and it might depend
    // on user parameters ... this behaviour is typical for dynamic joints which adapt the
    // constraint setup to some conditions on the joint and/or bodies (ex: ropes).
    //_EvaluateJointState();
}
JointGeneric6DOF::JointGeneric6DOF( BodyNode * pBodyA, BodyNode * pBodyB, const Transform3 & vFrameInA, const Transform3 & vFrameInB, EulerAngles iEulerAnglesOrdering ):
    JointConstraint( pBodyA, pBodyB, vFrameInA, vFrameInB ),
    m_vTranslation(), m_vRotationEulerAngles()
{
    m_iEulerAnglesOrdering = iEulerAnglesOrdering;

    m_hLinearLMS[0].MakeLinear();
    m_hLinearLMS[1].MakeLinear();
    m_hLinearLMS[2].MakeLinear();
    m_hAngularLMS[0].MakeAngular();
    m_hAngularLMS[1].MakeAngular();
    m_hAngularLMS[2].MakeAngular();

    // Let derived / user classes do this so they get a chance to do some init before
    // Also, their virtual override isn't accessible before ctor returns and it might depend
    // on user parameters ... this behaviour is typical for dynamic joints which adapt the
    // constraint setup to some conditions on the joint and/or bodies (ex: ropes).
    //_EvaluateJointState();
}
JointGeneric6DOF::~JointGeneric6DOF()
{
    // nothing to do
}

Void JointGeneric6DOF::Evaluate( Scalar fInvTimeStep, Scalar /*fDefaultERP*/, Scalar /*fDefaultCFM*/ )
{
    // Update joint state
    _EvaluateJointState();

    // Generate constraint rows
    m_iRowCount = 0;

        // Angular constraints first
    UInt iEulerAnglesOrdering[3];
    switch( m_iEulerAnglesOrdering ) {
        case EULER_ANGLES_XYZ: iEulerAnglesOrdering[0] = 0; iEulerAnglesOrdering[1] = 1; iEulerAnglesOrdering[2] = 2; break;
        case EULER_ANGLES_XZY: iEulerAnglesOrdering[0] = 0; iEulerAnglesOrdering[1] = 2; iEulerAnglesOrdering[2] = 1; break;
        case EULER_ANGLES_YXZ: iEulerAnglesOrdering[0] = 1; iEulerAnglesOrdering[1] = 0; iEulerAnglesOrdering[2] = 2; break;
        case EULER_ANGLES_YZX: iEulerAnglesOrdering[0] = 1; iEulerAnglesOrdering[1] = 2; iEulerAnglesOrdering[2] = 0; break;
        case EULER_ANGLES_ZXY: iEulerAnglesOrdering[0] = 2; iEulerAnglesOrdering[1] = 0; iEulerAnglesOrdering[2] = 1; break;
        case EULER_ANGLES_ZYX: iEulerAnglesOrdering[0] = 2; iEulerAnglesOrdering[1] = 1; iEulerAnglesOrdering[2] = 0; break;
        default: Assert(false); break;
    }
    for( UInt i = 0; i < 3; ++i ) {
        UInt iIndex = iEulerAnglesOrdering[i];
        m_iRowCount += _EvaluateLimitMotorSpring( m_arrRows + m_iRowCount, m_hAngularLMS + iIndex, fInvTimeStep, m_vRotationAxis[iIndex] );
    }

        // Linear constraints last
    for( UInt i = 0; i < 3; ++i ) {
        if ( m_hLinearLMS[i].NeedCorrection() ) {
            UInt iNext = (i+1) % 3;
            UInt iNext2 = (i+2) % 3;
            Bool bAllowRotation = ( ( !(m_hAngularLMS[iNext].IsFixed()) && m_hAngularLMS[iNext].GetLimitState() == JOINTLIMIT_FREE )
                                   || ( !(m_hAngularLMS[iNext2].IsFixed()) && m_hAngularLMS[iNext2].GetLimitState() == JOINTLIMIT_FREE ) );

            Vector3 vAxis;
            m_vJointTransformA.GetRotate().GetColumn( vAxis, i );

            Vector3 vAnchorA = ( m_vJointTransformA.GetTranslate() - m_pBodyA->GetTransform().GetTranslate() );
            Vector3 vAnchorB = ( m_vJointTransformB.GetTranslate() - m_pBodyB->GetTransform().GetTranslate() );
            Scalar fOffset = ( m_hLinearLMS[i].GetTestPosition() - m_hLinearLMS[i].GetLimitError() );
            _AdjustAnchors( &vAnchorA, &vAnchorB, vAxis, fOffset, bAllowRotation );

            m_iRowCount += _EvaluateLimitMotorSpring( m_arrRows + m_iRowCount, m_hLinearLMS + i, fInvTimeStep, vAxis, vAnchorA, vAnchorB );
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////

Void JointGeneric6DOF::_EvaluateJointState()
{
    // Setup transforms
    m_vJointTransformA = ( m_pBodyA->GetTransform() * m_vFrameInA );
    m_vJointTransformB = ( m_pBodyB->GetTransform() * m_vFrameInB );

    // Extract linear movement
    Transform3 vInvA, vInvB;
    m_vJointTransformA.Invert( vInvA );
    m_vJointTransformB.Invert( vInvB );

    m_vTranslation = vInvA * ( m_vJointTransformA.GetTranslate() - m_vJointTransformB.GetTranslate() );

    // Extract angular movement
    Transform3 vRelativeTransform = ( vInvB * m_vJointTransformA );
    EulerDecomposition hEulerDecomp( vRelativeTransform.GetRotate() );

    switch( m_iEulerAnglesOrdering ) {
        case EULER_ANGLES_XYZ: {
                hEulerDecomp.ExtractXYZ( m_vRotationEulerAngles.X, m_vRotationEulerAngles.Y, m_vRotationEulerAngles.Z );

                Vector3 vAxis0, vAxis2;
                m_vJointTransformB.GetRotate().GetColumn( vAxis0, 0 );
                m_vJointTransformA.GetRotate().GetColumn( vAxis2, 2 );
                m_vRotationAxis[1] = ( vAxis2 ^ vAxis0 );
                m_vRotationAxis[0] = ( m_vRotationAxis[1] ^ vAxis2 );
                m_vRotationAxis[2] = ( vAxis0 ^ m_vRotationAxis[1] );
            } break;
        case EULER_ANGLES_XZY: {
                hEulerDecomp.ExtractXZY( m_vRotationEulerAngles.X, m_vRotationEulerAngles.Y, m_vRotationEulerAngles.Z );

                Vector3 vAxis0, vAxis1;
                m_vJointTransformB.GetRotate().GetColumn( vAxis0, 0 );
                m_vJointTransformA.GetRotate().GetColumn( vAxis1, 1 );
                m_vRotationAxis[2] = ( vAxis0 ^ vAxis1 );
                m_vRotationAxis[0] = ( vAxis1 ^ m_vRotationAxis[2] );
                m_vRotationAxis[1] = ( m_vRotationAxis[2] ^ vAxis0 );
            } break;
        case EULER_ANGLES_YXZ: {
                hEulerDecomp.ExtractYXZ( m_vRotationEulerAngles.X, m_vRotationEulerAngles.Y, m_vRotationEulerAngles.Z );

                Vector3 vAxis1, vAxis2;
                m_vJointTransformB.GetRotate().GetColumn( vAxis1, 1 );
                m_vJointTransformA.GetRotate().GetColumn( vAxis2, 2 );
                m_vRotationAxis[0] = ( vAxis1 ^ vAxis2 );
                m_vRotationAxis[1] = ( vAxis2 ^ m_vRotationAxis[0] );
                m_vRotationAxis[2] = ( m_vRotationAxis[0] ^ vAxis1 );
            } break;
        case EULER_ANGLES_YZX: {
                hEulerDecomp.ExtractYZX( m_vRotationEulerAngles.X, m_vRotationEulerAngles.Y, m_vRotationEulerAngles.Z );

                Vector3 vAxis0, vAxis1;
                m_vJointTransformB.GetRotate().GetColumn( vAxis1, 1 );
                m_vJointTransformA.GetRotate().GetColumn( vAxis0, 0 );
                m_vRotationAxis[2] = ( vAxis0 ^ vAxis1 );
                m_vRotationAxis[0] = ( vAxis1 ^ m_vRotationAxis[2] );
                m_vRotationAxis[1] = ( m_vRotationAxis[2] ^ vAxis0 );
            } break;
        case EULER_ANGLES_ZXY: {
                hEulerDecomp.ExtractZXY( m_vRotationEulerAngles.X, m_vRotationEulerAngles.Y, m_vRotationEulerAngles.Z );

                Vector3 vAxis1, vAxis2;
                m_vJointTransformB.GetRotate().GetColumn( vAxis2, 2 );
                m_vJointTransformA.GetRotate().GetColumn( vAxis1, 1 );
                m_vRotationAxis[0] = ( vAxis1 ^ vAxis2 );
                m_vRotationAxis[1] = ( vAxis2 ^ m_vRotationAxis[0] );
                m_vRotationAxis[2] = ( m_vRotationAxis[0] ^ vAxis1 );
            } break;
        case EULER_ANGLES_ZYX: {
                hEulerDecomp.ExtractZYX( m_vRotationEulerAngles.X, m_vRotationEulerAngles.Y, m_vRotationEulerAngles.Z );

                Vector3 vAxis0, vAxis2;
                m_vJointTransformB.GetRotate().GetColumn( vAxis2, 2 );
                m_vJointTransformA.GetRotate().GetColumn( vAxis0, 0 );
                m_vRotationAxis[1] = ( vAxis2 ^ vAxis0 );
                m_vRotationAxis[0] = ( m_vRotationAxis[1] ^ vAxis2 );
                m_vRotationAxis[2] = ( vAxis0 ^ m_vRotationAxis[1] );
            } break;
        default: Assert(false); break;
    }

    m_vRotationAxis[0].Normalize();
    m_vRotationAxis[1].Normalize();
    m_vRotationAxis[2].Normalize();

    // LimitMotor constraints
    for( UInt i = 0; i < 3; i++ ) {
        m_hLinearLMS[i].TestLimit( m_vTranslation[i] );
        m_hAngularLMS[i].TestLimit( m_vRotationEulerAngles[i] );
    }
}







