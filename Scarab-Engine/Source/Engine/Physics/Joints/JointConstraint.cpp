/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Joints/JointConstraint.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Joint constraints definitions
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
#include "JointConstraint.h"

/////////////////////////////////////////////////////////////////////////////////
// JointLimitMotorSpring implementation
JointLimitMotorSpring::JointLimitMotorSpring( Bool bAngularElseLinear )
{
    if ( bAngularElseLinear )
        MakeAngular();
    else
        MakeLinear();
}
JointLimitMotorSpring::JointLimitMotorSpring( const JointLimitMotorSpring & rhs )
{
    m_bAngularElseLinear = rhs.m_bAngularElseLinear;

    m_fLowerLimit = rhs.m_fLowerLimit;
    m_fUpperLimit = rhs.m_fUpperLimit;
    LimitRestitution = rhs.LimitRestitution;
    LimitSoftness = rhs.LimitSoftness;
    LimitERP = rhs.LimitERP;
    LimitCFM = rhs.LimitCFM;
    LimitMaxImpulse = rhs.LimitMaxImpulse;

    m_bMotorEnabled = rhs.m_bMotorEnabled;
    m_bIsServoMotor = rhs.m_bIsServoMotor;
    m_fMotorTargetVelocity = rhs.m_fMotorTargetVelocity;
    m_fServoMotorTargetPosition = rhs.m_fServoMotorTargetPosition;
    m_fMotorMaxImpulse = rhs.m_fMotorMaxImpulse;
    MotorERP = rhs.MotorERP;
    MotorCFM = rhs.MotorCFM;

    m_bSpringEnabled = rhs.m_bSpringEnabled;
    m_fSpringStiffness = rhs.m_fSpringStiffness;
    m_fSpringEquilibriumPoint = rhs.m_fSpringEquilibriumPoint;
    SpringDamping = rhs.SpringDamping;

    m_fTestPositionOrAngle = rhs.m_fTestPositionOrAngle;

    m_iLimitState = rhs.m_iLimitState;
    m_fLimitError = rhs.m_fLimitError;
}
JointLimitMotorSpring::~JointLimitMotorSpring()
{
    // nothing to do
}

JointLimitMotorSpring & JointLimitMotorSpring::operator=( const JointLimitMotorSpring & rhs )
{
    m_bAngularElseLinear = rhs.m_bAngularElseLinear;

    m_fLowerLimit = rhs.m_fLowerLimit;
    m_fUpperLimit = rhs.m_fUpperLimit;
    LimitRestitution = rhs.LimitRestitution;
    LimitSoftness = rhs.LimitSoftness;
    LimitERP = rhs.LimitERP;
    LimitCFM = rhs.LimitCFM;
    LimitMaxImpulse = rhs.LimitMaxImpulse;

    m_bMotorEnabled = rhs.m_bMotorEnabled;
    m_bIsServoMotor = rhs.m_bIsServoMotor;
    m_fMotorTargetVelocity = rhs.m_fMotorTargetVelocity;
    m_fServoMotorTargetPosition = rhs.m_fServoMotorTargetPosition;
    m_fMotorMaxImpulse = rhs.m_fMotorMaxImpulse;
    MotorERP = rhs.MotorERP;
    MotorCFM = rhs.MotorCFM;

    m_bSpringEnabled = rhs.m_bSpringEnabled;
    m_fSpringStiffness = rhs.m_fSpringStiffness;
    m_fSpringEquilibriumPoint = rhs.m_fSpringEquilibriumPoint;
    SpringDamping = rhs.SpringDamping;

    m_fTestPositionOrAngle = rhs.m_fTestPositionOrAngle;

    m_iLimitState = rhs.m_iLimitState;
    m_fLimitError = rhs.m_fLimitError;

    return (*this);
}

Void JointLimitMotorSpring::MakeLinear()
{
    m_bAngularElseLinear = false;

    m_fLowerLimit = 1.0f;
    m_fUpperLimit = -1.0f;
    LimitRestitution = 0.5f;
    LimitSoftness = 0.7f;
    LimitERP = 0.2f;
    LimitCFM = 0.0f;
    LimitMaxImpulse = 300.0f;

    m_bMotorEnabled = false;
    m_bIsServoMotor = false;
    m_fMotorTargetVelocity = 0.0f;
    m_fServoMotorTargetPosition = 0.0f;
    m_fMotorMaxImpulse = 0.0f;
    MotorERP = 0.9f;
    MotorCFM = 0.0f;

    m_bSpringEnabled = false;
    m_fSpringStiffness = 0.0f;
    m_fSpringEquilibriumPoint = 0.0f;
    SpringDamping = 0.0f;

    m_fTestPositionOrAngle = 0.0f;

    m_iLimitState = JOINTLIMIT_FREE;
    m_fLimitError = 0.0f;
}
Void JointLimitMotorSpring::MakeAngular()
{
    m_bAngularElseLinear = true;

    m_fLowerLimit = 1.0f;
    m_fUpperLimit = -1.0f;
    LimitRestitution = 0.5f;
    LimitSoftness = 0.7f;
    LimitERP = 0.2f;
    LimitCFM = 0.0f;
    LimitMaxImpulse = 300.0f;

    m_bMotorEnabled = false;
    m_bIsServoMotor = false;
    m_fMotorTargetVelocity = 0.0f;
    m_fServoMotorTargetPosition = 0.0f;
    m_fMotorMaxImpulse = 0.0f;
    MotorERP = 0.9f;
    MotorCFM = 0.0f;

    m_bSpringEnabled = false;
    m_fSpringStiffness = 0.0f;
    m_fSpringEquilibriumPoint = 0.0f;
    SpringDamping = 0.0f;

    m_fTestPositionOrAngle = 0.0f;

    m_iLimitState = JOINTLIMIT_FREE;
    m_fLimitError = 0.0f;
}

Void JointLimitMotorSpring::TestLimit( Scalar fPositionOrAngle )
{
    // Save test value
    if ( m_bAngularElseLinear )
        m_fTestPositionOrAngle = MathFn->NormalizeAngle( fPositionOrAngle );
    else
        m_fTestPositionOrAngle = fPositionOrAngle;

    // Limits disabled case
    if ( m_fLowerLimit > m_fUpperLimit ) {
        m_iLimitState = JOINTLIMIT_FREE;
        m_fLimitError = 0.0f;
        return;
    }

    // Lower limit violation case
    if ( m_fTestPositionOrAngle < m_fLowerLimit ) {
        m_iLimitState = JOINTLIMIT_LOWER;

        if ( m_bAngularElseLinear ) {
            // Adjust angle to limit
            Scalar fDiffLow = MathFn->Abs( MathFn->NormalizeAngle(m_fLowerLimit - m_fTestPositionOrAngle) );
            Scalar fDiffUp = MathFn->Abs( MathFn->NormalizeAngle(m_fUpperLimit - m_fTestPositionOrAngle) );
            m_fTestPositionOrAngle = (fDiffLow < fDiffUp) ? m_fTestPositionOrAngle : (m_fTestPositionOrAngle + SCALAR_2PI);

            m_fLimitError = ( m_fTestPositionOrAngle - m_fLowerLimit );

            // Normalize again
            if ( m_fLimitError > SCALAR_PI )
                m_fLimitError -= SCALAR_2_PI;
            else if ( m_fLimitError < -SCALAR_PI )
                m_fLimitError += SCALAR_2_PI;
        } else
            m_fLimitError = ( m_fTestPositionOrAngle - m_fLowerLimit );

        return;
    }

    // Upper limit violation case
    if ( m_fTestPositionOrAngle > m_fUpperLimit ) {
        m_iLimitState = JOINTLIMIT_UPPER;

        if ( m_bAngularElseLinear ) {
            // Adjust angle to limit
            Scalar fDiffLow = MathFn->Abs( MathFn->NormalizeAngle(m_fTestPositionOrAngle - m_fLowerLimit) );
            Scalar fDiffUp = MathFn->Abs( MathFn->NormalizeAngle(m_fTestPositionOrAngle - m_fUpperLimit) );
            m_fTestPositionOrAngle = (fDiffLow < fDiffUp) ? (m_fTestPositionOrAngle - SCALAR_2PI) : m_fTestPositionOrAngle;

            m_fLimitError = ( m_fTestPositionOrAngle - m_fUpperLimit );

            // Normalize again
            if ( m_fLimitError > SCALAR_PI )
                m_fLimitError -= SCALAR_2_PI;
            else if ( m_fLimitError < -SCALAR_PI )
                m_fLimitError += SCALAR_2_PI;
        } else
            m_fLimitError = ( m_fTestPositionOrAngle - m_fUpperLimit );

        return;
    }

    // Free inside limits case
    m_iLimitState = JOINTLIMIT_FREE;
    m_fLimitError = 0.0f;
}

/////////////////////////////////////////////////////////////////////////////////

Scalar JointLimitMotorSpring::_GetMotorFactor( Scalar fTargetVelocity, Scalar fInvTimeStep, Scalar fOverrideLowerLimit, Scalar fOverrideUpperLimit ) const
{
    Scalar fLowerLimit = m_fLowerLimit;
    Scalar fUpperLimit = m_fUpperLimit;
    if ( fOverrideUpperLimit >= fOverrideLowerLimit ) {
        fLowerLimit = fOverrideLowerLimit;
        fUpperLimit = fOverrideUpperLimit;
    }

    if ( fLowerLimit > fUpperLimit )
        return 1.0f;

    if ( fLowerLimit == fUpperLimit )
        return 0.0f;

    Scalar fMotorFactor = 1.0f;

    Scalar fMaxDelta = ( fTargetVelocity / fInvTimeStep ); // That one div causes no harm and avoids passing a far fetch value down from solver ...
    if ( fMaxDelta < 0.0f ) {
        if ( (m_fTestPositionOrAngle >= fLowerLimit) && (m_fTestPositionOrAngle < (fLowerLimit - fMaxDelta)) )
            fMotorFactor = (fLowerLimit - m_fTestPositionOrAngle) / fMaxDelta;
        else if ( m_fTestPositionOrAngle < fLowerLimit )
            fMotorFactor = 0.0f;
    } else if ( fMaxDelta > 0.0f ) {
        if ( (m_fTestPositionOrAngle <= m_fUpperLimit) && (m_fTestPositionOrAngle > (fUpperLimit - fMaxDelta)) )
            fMotorFactor = (fUpperLimit - m_fTestPositionOrAngle) / fMaxDelta;
        else if ( m_fTestPositionOrAngle > fUpperLimit )
            fMotorFactor = 0.0f;
    } else
        fMotorFactor = 0.0f;

    return fMotorFactor;
}

/////////////////////////////////////////////////////////////////////////////////
// JointConstraint implementation
JointConstraint::JointConstraint( BodyNode * pBodyA, BodyNode * pBodyB, const Transform3 & vJointFrame ):
    m_vFrameInA(), m_vFrameInB(), m_vJointTransformA(), m_vJointTransformB()
{
    // Bodies
    m_pBodyA = pBodyA;
    m_pBodyB = pBodyB;

    Transform3 vInvA, vInvB;
    m_pBodyA->GetTransform().Invert( vInvA );
    m_pBodyB->GetTransform().Invert( vInvB );

    m_vFrameInA = vInvA * vJointFrame;
    m_vFrameInB = vInvB * vJointFrame;

    // Constraint rows
    m_iRowCount = 0;
    for( UInt i = 0; i < JOINT_MAX_ROW_COUNT; ++i ) {
        m_arrRows[i].fError = 0.0f;
        m_arrRows[i].fCFM = 0.0f;
        m_arrRows[i].fImpulse = 0.0f;
        m_arrRows[i].fLowerLimit = -SCALAR_INFINITE;
        m_arrRows[i].fUpperLimit = SCALAR_INFINITE;
    }

    // Constraint frame offset support
    m_bHasStaticBody = ( m_pBodyA->IsInsulator() || m_pBodyA->IsFixed() || m_pBodyB->IsInsulator() || m_pBodyB->IsFixed() );
    Scalar fInvSum = ( m_pBodyA->GetInvMass() + m_pBodyB->GetInvMass() );
    if ( fInvSum > 0.0f )
        m_fWeightFactorA = ( m_pBodyB->GetInvMass() / fInvSum );
    else
        m_fWeightFactorA = 0.5f;
    m_fWeightFactorB = ( 1.0f - m_fWeightFactorA );

    // Breaking support
    m_bEnabled = true;
    m_fBreakingThreshold = SCALAR_INFINITE;

    // Feedback support
    m_bFeedbackEnabled = false;
    m_pFeedback = NULL;
}
JointConstraint::JointConstraint( BodyNode * pBodyA, BodyNode * pBodyB, const Transform3 & vFrameInA, const Transform3 & vFrameInB ):
    m_vFrameInA(), m_vFrameInB(), m_vJointTransformA(), m_vJointTransformB()
{
    // Bodies
    m_pBodyA = pBodyA;
    m_pBodyB = pBodyB;

    m_vFrameInA = vFrameInA;
    m_vFrameInB = vFrameInB;

    // Constraint rows
    m_iRowCount = 0;
    for( UInt i = 0; i < JOINT_MAX_ROW_COUNT; ++i ) {
        m_arrRows[i].fError = 0.0f;
        m_arrRows[i].fCFM = 0.0f;
        m_arrRows[i].fImpulse = 0.0f;
        m_arrRows[i].fLowerLimit = -SCALAR_INFINITE;
        m_arrRows[i].fUpperLimit = SCALAR_INFINITE;
    }

    // Constraint frame offset support
    m_bHasStaticBody = ( m_pBodyA->IsInsulator() || m_pBodyA->IsFixed() || m_pBodyB->IsInsulator() || m_pBodyB->IsFixed() );
    Scalar fInvSum = ( m_pBodyA->GetInvMass() + m_pBodyB->GetInvMass() );
    if ( fInvSum > 0.0f )
        m_fWeightFactorA = ( m_pBodyB->GetInvMass() / fInvSum );
    else
        m_fWeightFactorA = 0.5f;
    m_fWeightFactorB = ( 1.0f - m_fWeightFactorA );

    // Breaking support
    m_bEnabled = true;
    m_fBreakingThreshold = SCALAR_INFINITE;

    // Feedback support
    m_bFeedbackEnabled = false;
    m_pFeedback = NULL;
}
JointConstraint::~JointConstraint()
{
    // nothing to do
}

Void JointConstraint::SetFrame( const Transform3 & vJointFrame )
{
    Transform3 vInvA, vInvB;
    m_pBodyA->GetTransform().Invert( vInvA );
    m_pBodyB->GetTransform().Invert( vInvB );

    m_vFrameInA = vInvA * vJointFrame;
    m_vFrameInB = vInvB * vJointFrame;

    _EvaluateJointState();
}
Void JointConstraint::SetFrame( const Transform3 & vFrameInA, const Transform3 & vFrameInB )
{
    m_vFrameInA = vFrameInA;
    m_vFrameInB = vFrameInB;

    _EvaluateJointState();
}

/////////////////////////////////////////////////////////////////////////////////

UInt JointConstraint::_EvaluateLimitMotorSpring( JointConstraintRow * outRow, const JointLimitMotorSpring * pLimitMotorSpring, Scalar fInvTimeStep,
                                                 const Vector3 & vAxis, const Vector3 & vAnchorA, const Vector3 & vAnchorB )
{
    // Nothing to constrain case
    if ( !(pLimitMotorSpring->NeedCorrection()) )
        return 0; // No row is created

    // Retrieve Limit-Motor-Spring state
    Bool bAngular = pLimitMotorSpring->IsAngular();

    Bool bPoweredMotor = pLimitMotorSpring->HasMotor();
    JointLimitState iLimitState = pLimitMotorSpring->GetLimitState();
    if ( pLimitMotorSpring->IsFixed() && iLimitState != JOINTLIMIT_FREE )
        bPoweredMotor = false; // If we have a fixed limit, motor is ineffective so disable it

    // Jacobian
    if ( bAngular ) {
        outRow->constrJacobian.MakeAngular(
            vAxis, m_pBodyA->GetInvInertiaTensor(), m_pBodyB->GetInvInertiaTensor()
        );
    } else {
        outRow->constrJacobian.MakeLinear(
            vAxis, vAnchorA, vAnchorB,
            m_pBodyA->GetInvMass(), m_pBodyA->GetInvInertiaTensor(),
            m_pBodyB->GetInvMass(), m_pBodyB->GetInvInertiaTensor()
        );
    }

    // Error term & limits
    Scalar fMotorK = ( fInvTimeStep * pLimitMotorSpring->MotorERP );
    Scalar fLimitK = ( fInvTimeStep * pLimitMotorSpring->LimitERP );

    outRow->fError = 0.0f;
    outRow->fCFM = 0.0f;
    outRow->fLowerLimit = -m_fBreakingThreshold;
    outRow->fUpperLimit = m_fBreakingThreshold;

        // Limits
    if ( iLimitState != JOINTLIMIT_FREE ) {
        outRow->fError = -( pLimitMotorSpring->GetLimitError() * fLimitK );
        outRow->fCFM = pLimitMotorSpring->LimitCFM;
        outRow->fLowerLimit = -(pLimitMotorSpring->LimitMaxImpulse);
        outRow->fUpperLimit = pLimitMotorSpring->LimitMaxImpulse;

        if ( !(pLimitMotorSpring->IsFixed()) ) {
            if ( iLimitState == JOINTLIMIT_LOWER ) {
                outRow->fLowerLimit = 0.0f;
                outRow->fUpperLimit = pLimitMotorSpring->LimitMaxImpulse;
            } else {
                Assert( iLimitState == JOINTLIMIT_UPPER );
                outRow->fLowerLimit = -(pLimitMotorSpring->LimitMaxImpulse);
                outRow->fUpperLimit = 0.0f;
            }

            // Restitution
            if ( pLimitMotorSpring->LimitRestitution > 0.0f ) {
                Scalar fRelativeVelocity;
                if ( bAngular )
                    fRelativeVelocity = ( m_pBodyA->GetAngularSpeed() - m_pBodyB->GetAngularSpeed() ) * vAxis;
                else
                    fRelativeVelocity = ( m_pBodyA->GetLinearSpeed() - m_pBodyB->GetLinearSpeed() ) * vAxis;

                if ( iLimitState == JOINTLIMIT_LOWER ) {
                    if ( fRelativeVelocity < 0.0f ) {
                        Scalar fLimitError = -( fRelativeVelocity * pLimitMotorSpring->LimitRestitution );
                        if ( fLimitError > outRow->fError )
                            outRow->fError = fLimitError;
                    }
                } else {
                    Assert( iLimitState == JOINTLIMIT_UPPER );
                    if ( fRelativeVelocity > 0.0f ) {
                        Scalar fLimitError = -( fRelativeVelocity * pLimitMotorSpring->LimitRestitution );
                        if ( fLimitError < outRow->fError )
                            outRow->fError = fLimitError;
                    }
                }
            }
        }
    }

        // Motor
    if ( bPoweredMotor ) {
        Scalar fTargetVelocity;
        Scalar fMotorFactor = 0.0f;
        if ( pLimitMotorSpring->IsServoMotor() ) {
            Scalar fError = ( pLimitMotorSpring->GetTestPosition() - pLimitMotorSpring->GetServoMotorTargetPosition() );
            fTargetVelocity = ( fError < 0.0f ) ? pLimitMotorSpring->GetMotorTargetVelocity() : -(pLimitMotorSpring->GetMotorTargetVelocity());
            if ( fError != 0.0f ) {
                Scalar fTargetPosition = pLimitMotorSpring->GetServoMotorTargetPosition();
                Scalar fLowerLimit;
                Scalar fUpperLimit;
                if ( pLimitMotorSpring->IsFree() ) {
                    fLowerLimit = ( fError > 0.0f ) ? fTargetPosition : -m_fBreakingThreshold;
                    fUpperLimit = ( fError < 0.0f ) ? fTargetPosition : m_fBreakingThreshold;
                } else {
                    fLowerLimit = ( fError > 0.0f && fTargetPosition > pLimitMotorSpring->GetLowerLimit() ) ? fTargetPosition : pLimitMotorSpring->GetLowerLimit();
                    fUpperLimit = ( fError < 0.0f && fTargetPosition < pLimitMotorSpring->GetUpperLimit() ) ? fTargetPosition : pLimitMotorSpring->GetUpperLimit();
                }
                fMotorFactor = pLimitMotorSpring->_GetMotorFactor( fTargetVelocity, fMotorK, fLowerLimit, fUpperLimit );
            }
        } else {
            fTargetVelocity = pLimitMotorSpring->GetMotorTargetVelocity();
            fMotorFactor = pLimitMotorSpring->_GetMotorFactor( fTargetVelocity, fMotorK );
        }

        outRow->fError += ( fMotorFactor * fTargetVelocity );
        outRow->fCFM = pLimitMotorSpring->MotorCFM;
        outRow->fLowerLimit = -(pLimitMotorSpring->GetMotorMaxImpulse());
        outRow->fUpperLimit = pLimitMotorSpring->GetMotorMaxImpulse();
    }

        // Spring
    if ( pLimitMotorSpring->HasSpring() ) {
        Scalar fError = ( pLimitMotorSpring->GetTestPosition() - pLimitMotorSpring->GetSpringEquilibriumPoint() );
        Scalar fRelativeVelocity;
        if ( bAngular )
            fRelativeVelocity = ( m_pBodyA->GetAngularSpeed() - m_pBodyB->GetAngularSpeed() ) * vAxis;
        else
            fRelativeVelocity = ( m_pBodyA->GetLinearSpeed() - m_pBodyB->GetLinearSpeed() ) * vAxis;

        Scalar fKS = ( pLimitMotorSpring->GetSpringStiffness() * fError * fInvTimeStep );
        Scalar fKD = ( pLimitMotorSpring->SpringDamping * fRelativeVelocity );
        Scalar fKSum = ( fKS + fKD );
        Scalar fKDiff = ( -fKS + fKD );
        Scalar fMinK, fMaxK;
        if ( fKS < 0.0f ) {
            fMinK = fKD;
            fMaxK = fKDiff;
        } else {
            fMinK = fKDiff;
            fMaxK = fKD;
        }

        outRow->fError += -( fRelativeVelocity + fKSum );
        outRow->fCFM = 0.0f;
        outRow->fLowerLimit = ( fMinK > 0.0f ) ? 0.0f : fMinK;
        outRow->fUpperLimit = ( fMaxK < 0.0f ) ? 0.0f : fMaxK;
    }

    // Done
    return 1; // We created 1 row
}

Void JointConstraint::_AdjustAnchors( Vector3 * pAnchorA, Vector3 * pAnchorB, const Vector3 & vAxis, Scalar fOffset, Bool bAllowRotation )
{
    Vector3 vProjA = vAxis * (*pAnchorA * vAxis);
    Vector3 vProjB = vAxis * (*pAnchorB * vAxis);
    Vector3 vOrthoA = ( *pAnchorA - vProjA );
    Vector3 vOrthoB = ( *pAnchorB - vProjB );
    Vector3 vDistance = ( (vProjA - vProjB) + vAxis * fOffset );

    *pAnchorA = vOrthoA + (vDistance * m_fWeightFactorA);
    *pAnchorB = vOrthoB - (vDistance * m_fWeightFactorB);

    // Make constraints with statics more rigid
    if ( m_bHasStaticBody && !bAllowRotation ) {
        *pAnchorA *= m_fWeightFactorA;
        *pAnchorB *= m_fWeightFactorB;
    }
}

Void JointConstraint::_EvaluateJointState()
{
    // Setup transforms
    m_vJointTransformA = ( m_pBodyA->GetTransform() * m_vFrameInA );
    m_vJointTransformB = ( m_pBodyB->GetTransform() * m_vFrameInB );
}

