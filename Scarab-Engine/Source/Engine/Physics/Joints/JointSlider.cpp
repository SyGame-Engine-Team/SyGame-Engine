/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Joints/JointSlider.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Joints : Slider joint (often called Prismatic)
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
#include "JointSlider.h"

/////////////////////////////////////////////////////////////////////////////////
// JointSlider implementation
JointSlider::JointSlider( BodyNode * pBodyA, BodyNode * pBodyB, const Transform3 & vJointFrame ):
    JointConstraint( pBodyA, pBodyB, vJointFrame ),
    m_vSliderAxis(), m_hLinearLMS( false )
{
    _EvaluateJointState();
}
JointSlider::JointSlider( BodyNode * pBodyA, BodyNode * pBodyB, const Transform3 & vFrameInA, const Transform3 & vFrameInB ):
    JointConstraint( pBodyA, pBodyB, vFrameInA, vFrameInB ),
    m_vSliderAxis(), m_hLinearLMS( false )
{
    _EvaluateJointState();
}
JointSlider::~JointSlider()
{
    // nothing to do
}

Void JointSlider::Evaluate( Scalar fInvTimeStep, Scalar fDefaultERP, Scalar fDefaultCFM )
{
    Scalar fERP = /*m_bUseCustomERP ? m_fERP :*/ fDefaultERP;
    Scalar fCFM = /*m_bUseCustomCFM ? m_fCFM :*/ fDefaultCFM;

    Scalar fK = ( fInvTimeStep * fERP );

    // Update joint state
    _EvaluateJointState();

    Vector3 vAnchorA = ( m_vJointTransformA.GetTranslate() - m_pBodyA->GetTransform().GetTranslate() );
    Vector3 vAnchorB = ( m_vJointTransformB.GetTranslate() - m_pBodyB->GetTransform().GetTranslate() );
    Scalar fOffset = ( m_hLinearLMS.GetTestPosition() - m_hLinearLMS.GetLimitError() );
    _AdjustAnchors( &vAnchorA, &vAnchorB, m_vSliderAxis, fOffset, false );

    // Fix all 3 angular axes
    m_iRowCount = 3;

        // Jacobians
    m_arrRows[0].constrJacobian.MakeAngular(
        m_vLockedAxes[0], m_pBodyA->GetInvInertiaTensor(), m_pBodyB->GetInvInertiaTensor()
    );
    m_arrRows[1].constrJacobian.MakeAngular(
        m_vLockedAxes[1], m_pBodyA->GetInvInertiaTensor(), m_pBodyB->GetInvInertiaTensor()
    );
    m_arrRows[2].constrJacobian.MakeAngular(
        m_vSliderAxis, m_pBodyA->GetInvInertiaTensor(), m_pBodyB->GetInvInertiaTensor()
    );

        // Error term
    Vector3 vSliderAxisA, vSliderAxisB;
    m_vJointTransformA.GetRotate().GetColumn( vSliderAxisA, 2 );
    m_vJointTransformB.GetRotate().GetColumn( vSliderAxisB, 2 );

    Vector3 vAngularError = ( vSliderAxisA ^ vSliderAxisB ) * fK;
    m_arrRows[0].fError = ( vAngularError * m_vLockedAxes[0] );
    m_arrRows[0].fCFM = fCFM;
    m_arrRows[1].fError = ( vAngularError * m_vLockedAxes[1] );
    m_arrRows[1].fCFM = fCFM;

    m_vJointTransformA.GetRotate().GetColumn( vSliderAxisA, 1 );
    m_vJointTransformB.GetRotate().GetColumn( vSliderAxisB, 1 );

    vAngularError = ( vSliderAxisA ^ vSliderAxisB ) * fK;
    m_arrRows[2].fError = ( vAngularError * m_vSliderAxis );
    m_arrRows[2].fCFM = fCFM;

        // Limits
    m_arrRows[0].fLowerLimit = -m_fBreakingThreshold;
    m_arrRows[0].fUpperLimit = m_fBreakingThreshold;
    m_arrRows[1].fLowerLimit = -m_fBreakingThreshold;
    m_arrRows[1].fUpperLimit = m_fBreakingThreshold;
    m_arrRows[2].fLowerLimit = -m_fBreakingThreshold;
    m_arrRows[2].fUpperLimit = m_fBreakingThreshold;

    // Fix 2 linear axes
    m_iRowCount += 2;

        // Jacobians
    m_arrRows[3].constrJacobian.MakeLinear(
        m_vLockedAxes[0], vAnchorA, vAnchorB,
        m_pBodyA->GetInvMass(), m_pBodyA->GetInvInertiaTensor(),
        m_pBodyB->GetInvMass(), m_pBodyB->GetInvInertiaTensor()
    );
    m_arrRows[4].constrJacobian.MakeLinear(
        m_vLockedAxes[1], vAnchorA, vAnchorB,
        m_pBodyA->GetInvMass(), m_pBodyA->GetInvInertiaTensor(),
        m_pBodyB->GetInvMass(), m_pBodyB->GetInvInertiaTensor()
    );

        // Error term
    Vector3 vLinearError = ( m_vJointTransformB.GetTranslate() - m_vJointTransformA.GetTranslate() ) * fK;
    m_arrRows[3].fError = ( vLinearError * m_vLockedAxes[0] );
    m_arrRows[3].fCFM = fCFM;
    m_arrRows[4].fError = ( vLinearError * m_vLockedAxes[1] );
    m_arrRows[4].fCFM = fCFM;

        // Limits
    m_arrRows[3].fLowerLimit = -m_fBreakingThreshold;
    m_arrRows[3].fUpperLimit = m_fBreakingThreshold;
    m_arrRows[4].fLowerLimit = -m_fBreakingThreshold;
    m_arrRows[4].fUpperLimit = m_fBreakingThreshold;

    // Limit-Motor-Spring for slider axis
    m_iRowCount += _EvaluateLimitMotorSpring( m_arrRows + m_iRowCount, &m_hLinearLMS, fInvTimeStep, m_vSliderAxis, vAnchorA, vAnchorB );
}

/////////////////////////////////////////////////////////////////////////////////

Void JointSlider::_EvaluateJointState()
{
    // Setup transforms
    m_vJointTransformA = ( m_pBodyA->GetTransform() * m_vFrameInA );
    m_vJointTransformB = ( m_pBodyB->GetTransform() * m_vFrameInB );

    // Extract linear movement
    Vector3 vTmpA, vTmpB;
    m_vJointTransformA.GetRotate().GetColumn( vTmpA, 2 );
    m_vJointTransformB.GetRotate().GetColumn( vTmpB, 2 );
    m_vSliderAxis = ( (vTmpA * m_fWeightFactorA) + (vTmpB * m_fWeightFactorB) );
    m_vSliderAxis.Normalize();

    Vector3 vAnchorA = ( m_vJointTransformA.GetTranslate() - m_pBodyA->GetTransform().GetTranslate() );
    Vector3 vAnchorB = ( m_vJointTransformB.GetTranslate() - m_pBodyB->GetTransform().GetTranslate() );

    Vector3 vProjA = m_vSliderAxis * (vAnchorA * m_vSliderAxis);
    Vector3 vProjB = m_vSliderAxis * (vAnchorB * m_vSliderAxis);
    Vector3 vOrthoA = ( vAnchorA - vProjA );
    Vector3 vOrthoB = ( vAnchorB - vProjB );

    m_vLockedAxes[0] = ( (vOrthoA * m_fWeightFactorA) + (vOrthoB * m_fWeightFactorB) );
    Scalar fNormSqr = m_vLockedAxes[0].NormSqr();
    if ( fNormSqr > SCALAR_EPSILON )
        m_vLockedAxes[0] *= MathFn->InvSqrt( fNormSqr );
    else
        m_vJointTransformA.GetRotate().GetColumn( m_vLockedAxes[0], 1 );
    m_vLockedAxes[1] = ( m_vSliderAxis ^ m_vLockedAxes[0] );

    Vector3 vRelativeTranslate = ( m_vJointTransformA.GetTranslate() - m_vJointTransformB.GetTranslate() );
    m_fSliderPosition = ( vRelativeTranslate * m_vSliderAxis );

    // LimitMotorSpring constraints
    m_hLinearLMS.TestLimit( m_fSliderPosition );
}

