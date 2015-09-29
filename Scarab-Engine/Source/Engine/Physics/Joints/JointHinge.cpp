/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Joints/JointHinge.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Joints : Hinge joint (often called Revolute)
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
#include "JointHinge.h"

/////////////////////////////////////////////////////////////////////////////////
// JointHinge implementation
JointHinge::JointHinge( BodyNode * pBodyA, BodyNode * pBodyB, const Transform3 & vJointFrame ):
    JointConstraint( pBodyA, pBodyB, vJointFrame ),
    m_vHingeAxis(), m_hAngularLMS( true )
{
    _EvaluateJointState();
}
JointHinge::JointHinge( BodyNode * pBodyA, BodyNode * pBodyB, const Transform3 & vFrameInA, const Transform3 & vFrameInB ):
    JointConstraint( pBodyA, pBodyB, vFrameInA, vFrameInB ),
    m_vHingeAxis(), m_hAngularLMS( true )
{
    _EvaluateJointState();
}
JointHinge::~JointHinge()
{
    // nothing to do
}

Void JointHinge::Evaluate( Scalar fInvTimeStep, Scalar fDefaultERP, Scalar fDefaultCFM )
{
    Scalar fERP = /*m_bUseCustomERP ? m_fERP :*/ fDefaultERP;
    Scalar fCFM = /*m_bUseCustomCFM ? m_fCFM :*/ fDefaultCFM;

    Scalar fK = ( fInvTimeStep * fERP );

    // Update joint state
    _EvaluateJointState();

    Vector3 vAnchorA = ( m_vJointTransformA.GetTranslate() - m_pBodyA->GetTransform().GetTranslate() );
    Vector3 vAnchorB = ( m_vJointTransformB.GetTranslate() - m_pBodyB->GetTransform().GetTranslate() );
    Scalar fOffset = ( m_hAngularLMS.GetTestAngle() - m_hAngularLMS.GetLimitError() );
    _AdjustAnchors( &vAnchorA, &vAnchorB, m_vHingeAxis, fOffset, false );

    // Fix all 3 linear axes
    m_iRowCount = 3;

        // Jacobians
    m_arrRows[0].constrJacobian.MakeLinear(
        m_vLockedAxes[0], vAnchorA, vAnchorB,
        m_pBodyA->GetInvMass(), m_pBodyA->GetInvInertiaTensor(),
        m_pBodyB->GetInvMass(), m_pBodyB->GetInvInertiaTensor()
    );
    m_arrRows[1].constrJacobian.MakeLinear(
        m_vLockedAxes[1], vAnchorA, vAnchorB,
        m_pBodyA->GetInvMass(), m_pBodyA->GetInvInertiaTensor(),
        m_pBodyB->GetInvMass(), m_pBodyB->GetInvInertiaTensor()
    );
    m_arrRows[2].constrJacobian.MakeLinear(
        m_vHingeAxis, vAnchorA, vAnchorB,
        m_pBodyA->GetInvMass(), m_pBodyA->GetInvInertiaTensor(),
        m_pBodyB->GetInvMass(), m_pBodyB->GetInvInertiaTensor()
    );

        // Error term
    Vector3 vLinearError = ( m_vJointTransformB.GetTranslate() - m_vJointTransformA.GetTranslate() ) * fK;
    m_arrRows[0].fError = ( vLinearError * m_vLockedAxes[0] );
    m_arrRows[0].fCFM = fCFM;
    m_arrRows[1].fError = ( vLinearError * m_vLockedAxes[1] );
    m_arrRows[1].fCFM = fCFM;
    m_arrRows[2].fError = ( vLinearError * m_vHingeAxis );
    m_arrRows[2].fCFM = fCFM;

        // Limits
    m_arrRows[0].fLowerLimit = -m_fBreakingThreshold;
    m_arrRows[0].fUpperLimit = m_fBreakingThreshold;
    m_arrRows[1].fLowerLimit = -m_fBreakingThreshold;
    m_arrRows[1].fUpperLimit = m_fBreakingThreshold;
    m_arrRows[2].fLowerLimit = -m_fBreakingThreshold;
    m_arrRows[2].fUpperLimit = m_fBreakingThreshold;

    // Fix 2 angular axes
    m_iRowCount += 2;

        // Jacobians
    m_arrRows[3].constrJacobian.MakeAngular(
        m_vLockedAxes[0], m_pBodyA->GetInvInertiaTensor(), m_pBodyB->GetInvInertiaTensor()
    );
    m_arrRows[4].constrJacobian.MakeAngular(
        m_vLockedAxes[1], m_pBodyA->GetInvInertiaTensor(), m_pBodyB->GetInvInertiaTensor()
    );

        // Error term
    Vector3 vHingeAxisA, vHingeAxisB;
    m_vJointTransformA.GetRotate().GetColumn( vHingeAxisA, 2 );
    m_vJointTransformB.GetRotate().GetColumn( vHingeAxisB, 2 );

    Vector3 vAngularError = ( vHingeAxisA ^ vHingeAxisB ) * fK;
    m_arrRows[3].fError = ( vAngularError * m_vLockedAxes[0] );
    m_arrRows[3].fCFM = fCFM;
    m_arrRows[4].fError = ( vAngularError * m_vLockedAxes[1] );
    m_arrRows[4].fCFM = fCFM;

        // Limits
    m_arrRows[3].fLowerLimit = -m_fBreakingThreshold;
    m_arrRows[3].fUpperLimit = m_fBreakingThreshold;
    m_arrRows[4].fLowerLimit = -m_fBreakingThreshold;
    m_arrRows[4].fUpperLimit = m_fBreakingThreshold;

    // Limit-Motor-Spring for hinge axis
    m_iRowCount += _EvaluateLimitMotorSpring( m_arrRows + m_iRowCount, &m_hAngularLMS, fInvTimeStep, m_vHingeAxis );
}

/////////////////////////////////////////////////////////////////////////////////

Void JointHinge::_EvaluateJointState()
{
    // Setup transforms
    m_vJointTransformA = ( m_pBodyA->GetTransform() * m_vFrameInA );
    m_vJointTransformB = ( m_pBodyB->GetTransform() * m_vFrameInB );

    // Extract angular movement
    Vector3 vTmpA, vTmpB;
    m_vJointTransformA.GetRotate().GetColumn( vTmpA, 2 );
    m_vJointTransformB.GetRotate().GetColumn( vTmpB, 2 );
    m_vHingeAxis = ( (vTmpA * m_fWeightFactorA) + (vTmpB * m_fWeightFactorB) );
    m_vHingeAxis.Normalize();

    Vector3 vAnchorA = ( m_vJointTransformA.GetTranslate() - m_pBodyA->GetTransform().GetTranslate() );
    Vector3 vAnchorB = ( m_vJointTransformB.GetTranslate() - m_pBodyB->GetTransform().GetTranslate() );

    Vector3 vProjA = m_vHingeAxis * (vAnchorA * m_vHingeAxis);
    Vector3 vProjB = m_vHingeAxis * (vAnchorB * m_vHingeAxis);
    Vector3 vOrthoA = ( vAnchorA - vProjA );
    Vector3 vOrthoB = ( vAnchorB - vProjB );

    m_vLockedAxes[0] = ( (vOrthoA * m_fWeightFactorA) + (vOrthoB * m_fWeightFactorB) );
    Scalar fNormSqr = m_vLockedAxes[0].NormSqr();
    if ( fNormSqr > SCALAR_EPSILON )
        m_vLockedAxes[0] *= MathFn->InvSqrt( fNormSqr );
    else
        m_vJointTransformA.GetRotate().GetColumn( m_vLockedAxes[0], 1 );
    m_vLockedAxes[1] = ( m_vHingeAxis ^ m_vLockedAxes[0] );

    Vector3 vSwingAxis;
    m_vJointTransformB.GetRotate().GetColumn( vSwingAxis, 1 );

    m_fHingeAngle = MathFn->ArcTan2( vSwingAxis * m_vLockedAxes[0], vSwingAxis * m_vLockedAxes[1] ) - SCALAR_PI_2;

    // LimitMotorSpring constraints
    m_hAngularLMS.TestLimit( m_fHingeAngle );
}

