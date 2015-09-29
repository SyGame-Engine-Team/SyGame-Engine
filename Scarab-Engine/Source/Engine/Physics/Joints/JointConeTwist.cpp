/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Joints/JointConeTwist.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Joints : Cone-Twist joint (often called ... Shoulder ?!)
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
#include "JointConeTwist.h"

/////////////////////////////////////////////////////////////////////////////////
// JointConeTwist implementation
JointConeTwist::JointConeTwist( BodyNode * pBodyA, BodyNode * pBodyB, const Transform3 & vJointFrame ):
    JointConstraint( pBodyA, pBodyB, vJointFrame ),
    m_vSwingAxis(), m_vTwistAxis(), m_hSwingLMS( true ), m_hTwistLMS( true )
{
    m_fSwingSpan1 = SCALAR_PI_4;
    m_fSwingSpan2 = SCALAR_PI_4;

    _EvaluateJointState();
}
JointConeTwist::JointConeTwist( BodyNode * pBodyA, BodyNode * pBodyB, const Transform3 & vFrameInA, const Transform3 & vFrameInB ):
    JointConstraint( pBodyA, pBodyB, vFrameInA, vFrameInB ),
    m_vSwingAxis(), m_vTwistAxis(), m_hSwingLMS( true ), m_hTwistLMS( true )
{
    m_fSwingSpan1 = SCALAR_PI_4;
    m_fSwingSpan2 = SCALAR_PI_4;

    _EvaluateJointState();
}
JointConeTwist::~JointConeTwist()
{
    // nothing to do
}

//Void JointConeTwist::EnableServoMotor( Scalar fTargetVelocity, const Quaternion & qTarget, Scalar fMaxImpulse )
//{
//    // Extract angular movement
//    Quaternion qA( m_vFrameInA.GetRotate() );
//
//    Matrix3 matInvB;
//    m_vFrameInB.GetRotate().Invert( matInvB );
//    Quaternion qInvB( matInvB );
//
//    Quaternion qConstrained = ( qInvB * qTarget * qA );
//
//    Quaternion qTargetSwing, qTargetTwist;
//    qConstrained.SplitSwingTwist( Vector3::eI, qTargetSwing, qTargetTwist );
//
//        // Swing
//    Vector3 vTargetSwingAxis;
//    Scalar fTargetSwingAngle;
//    qTargetSwing.GetAxisAngle( vTargetSwingAxis, fTargetSwingAngle );
//
//    Scalar fEllipseX = vTargetSwingAxis.Y;
//    Scalar fEllipseY = -(vTargetSwingAxis.Z);
//    Scalar fSwingLimit = m_fSwingSpan1;
//    if ( MathFn->Abs(fEllipseX) > SCALAR_EPSILON ) {
//        Scalar fSlopeSqr = (fEllipseY*fEllipseY) / (fEllipseX*fEllipseX);
//        Scalar fNormSqr = MathFn->Invert( m_fSwingSpan2 * m_fSwingSpan2 ) + ( fSlopeSqr / (m_fSwingSpan1 * m_fSwingSpan1) );
//        fSwingLimit = MathFn->Sqrt( (1.0f + fSlopeSqr) / fNormSqr );
//    }
//
//        // Twist
//    Vector3 vTargetTwistAxis;
//    Scalar fTargetTwistAngle;
//    qTargetTwist.GetAxisAngle( vTargetTwistAxis, fTargetTwistAngle );
//
//    if ( fTargetTwistAngle > SCALAR_PI ) {
//        qTargetTwist = -qTargetTwist;
//        qTargetTwist.GetAxisAngle( vTargetTwistAxis, fTargetTwistAngle );
//    }
//
//    // LimitMotorSpring constraints
//    m_hSwingLMS.EnableLimits( -fSwingLimit, fSwingLimit );
//    m_hSwingLMS.TestLimit( fTargetSwingAngle );
//    m_hSwingLMS.EnableServoMotor( fTargetVelocity, m_hSwingLMS.GetTestAngle(), fMaxImpulse );
//
//    m_hTwistLMS.TestLimit( fTargetTwistAngle );
//    m_hTwistLMS.EnableServoMotor( fTargetVelocity, m_hTwistLMS.GetTestAngle(), fMaxImpulse );
//
//    // Wake up bodies when enabling motor !
//    m_pBodyA->SetSleeping( false );
//    m_pBodyB->SetSleeping( false );
//}

Void JointConeTwist::Evaluate( Scalar fInvTimeStep, Scalar fDefaultERP, Scalar fDefaultCFM )
{
    Scalar fERP = /*m_bUseCustomERP ? m_fERP :*/ fDefaultERP;
    Scalar fCFM = /*m_bUseCustomCFM ? m_fCFM :*/ fDefaultCFM;

    Scalar fK = ( fInvTimeStep * fERP );

    // Update joint state
    _EvaluateJointState();

    Vector3 vAnchorA = ( m_vJointTransformA.GetTranslate() - m_pBodyA->GetTransform().GetTranslate() );
    Vector3 vAnchorB = ( m_vJointTransformB.GetTranslate() - m_pBodyB->GetTransform().GetTranslate() );

    // Fix all 3 linear axes
    m_iRowCount = 3;

        // Jacobians
    m_arrRows[0].constrJacobian.MakeLinear(
        Vector3::eI, vAnchorA, vAnchorB,
        m_pBodyA->GetInvMass(), m_pBodyA->GetInvInertiaTensor(),
        m_pBodyB->GetInvMass(), m_pBodyB->GetInvInertiaTensor()
    );
    m_arrRows[1].constrJacobian.MakeLinear(
        Vector3::eJ, vAnchorA, vAnchorB,
        m_pBodyA->GetInvMass(), m_pBodyA->GetInvInertiaTensor(),
        m_pBodyB->GetInvMass(), m_pBodyB->GetInvInertiaTensor()
    );
    m_arrRows[2].constrJacobian.MakeLinear(
        Vector3::eK, vAnchorA, vAnchorB,
        m_pBodyA->GetInvMass(), m_pBodyA->GetInvInertiaTensor(),
        m_pBodyB->GetInvMass(), m_pBodyB->GetInvInertiaTensor()
    );

        // Error term
    Vector3 vLinearError = ( m_vJointTransformB.GetTranslate() - m_vJointTransformA.GetTranslate() ) * fK;
    m_arrRows[0].fError = vLinearError.X;
    m_arrRows[0].fCFM = fCFM;
    m_arrRows[1].fError = vLinearError.Y;
    m_arrRows[1].fCFM = fCFM;
    m_arrRows[2].fError = vLinearError.Z;
    m_arrRows[2].fCFM = fCFM;

        // Limits
    m_arrRows[0].fLowerLimit = -m_fBreakingThreshold;
    m_arrRows[0].fUpperLimit = m_fBreakingThreshold;
    m_arrRows[1].fLowerLimit = -m_fBreakingThreshold;
    m_arrRows[1].fUpperLimit = m_fBreakingThreshold;
    m_arrRows[2].fLowerLimit = -m_fBreakingThreshold;
    m_arrRows[2].fUpperLimit = m_fBreakingThreshold;

    // Cone limits
    m_iRowCount += _EvaluateLimitMotorSpring( m_arrRows + m_iRowCount, &m_hSwingLMS, fInvTimeStep, m_vSwingAxis );
    m_iRowCount += _EvaluateLimitMotorSpring( m_arrRows + m_iRowCount, &m_hTwistLMS, fInvTimeStep, m_vTwistAxis );
}

/////////////////////////////////////////////////////////////////////////////////

Void JointConeTwist::_EvaluateJointState()
{
    // Setup transforms
    m_vJointTransformA = ( m_pBodyA->GetTransform() * m_vFrameInA );
    m_vJointTransformB = ( m_pBodyB->GetTransform() * m_vFrameInB );

    // Extract angular movement
    Quaternion qA( m_vJointTransformA.GetRotate() );
    Quaternion qB( m_vJointTransformB.GetRotate() );
    Quaternion qInvB;
    qB.Invert( qInvB );
    Quaternion qRelativeAB = qInvB * qA;
    qRelativeAB.Normalize();

    Quaternion qSwing, qTwist;
    qRelativeAB.SplitSwingTwist( Vector3::eI, qSwing, qTwist );
    
        // Swing
    qSwing.GetAxisAngle( m_vSwingAxis, m_fSwingAngle );

    Scalar fEllipseX = m_vSwingAxis.Y;
    Scalar fEllipseY = -(m_vSwingAxis.Z);
    if ( MathFn->Abs(fEllipseX) > SCALAR_EPSILON ) {
        // Normal of ellipse curve (using gradient)
        Scalar fNormal = ( (fEllipseY * m_fSwingSpan2) / (fEllipseX * m_fSwingSpan1) );
        if ( fEllipseY > 0.0f )
            fEllipseY = MathFn->Abs( fEllipseX * fNormal );
        else
            fEllipseY = -( MathFn->Abs( fEllipseX * fNormal ) );

        // Convert ellipse normal back to swing axis
        m_vSwingAxis.Y = fEllipseX;
        m_vSwingAxis.Z = -fEllipseY;
    }
    m_vSwingAxis = ( qB * m_vSwingAxis );
    m_vSwingAxis.Normalize();

        // Swing limit
    Scalar fSwingLimit = m_fSwingSpan1;
    if ( MathFn->Abs(fEllipseX) > SCALAR_EPSILON ) {
        Scalar fSlopeSqr = (fEllipseY*fEllipseY) / (fEllipseX*fEllipseX);
        Scalar fNormSqr = MathFn->Invert( m_fSwingSpan2 * m_fSwingSpan2 ) + ( fSlopeSqr / (m_fSwingSpan1 * m_fSwingSpan1) );
        fSwingLimit = MathFn->Sqrt( (1.0f + fSlopeSqr) / fNormSqr );
    }

        // Twist
    qTwist.GetAxisAngle( m_vTwistAxis, m_fTwistAngle );

    if ( m_fTwistAngle > SCALAR_PI ) {
        qTwist = -qTwist;
        qTwist.GetAxisAngle( m_vTwistAxis, m_fTwistAngle );
    }
    m_vTwistAxis = ( qB * m_vTwistAxis );
    m_vTwistAxis.Normalize();

    // LimitMotorSpring constraints
    m_hSwingLMS.EnableLimits( -fSwingLimit, +fSwingLimit );
    m_hSwingLMS.TestLimit( m_fSwingAngle );

    if ( m_hTwistLMS.IsFree() )
        m_hTwistLMS.EnableLimits( -SCALAR_PI_2 + SCALAR_EPSILON, SCALAR_PI_2 - SCALAR_EPSILON );
    m_hTwistLMS.TestLimit( m_fTwistAngle );
}

