/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Joints/JointFixed.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Joints : Fixed joint
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
#include "JointFixed.h"

/////////////////////////////////////////////////////////////////////////////////
// JointFixed implementation
JointFixed::JointFixed( BodyNode * pBodyA, BodyNode * pBodyB, const Transform3 & vJointFrame ):
    JointConstraint( pBodyA, pBodyB, vJointFrame )
{
    _EvaluateJointState();
}
JointFixed::JointFixed( BodyNode * pBodyA, BodyNode * pBodyB, const Transform3 & vFrameInA, const Transform3 & vFrameInB ):
    JointConstraint( pBodyA, pBodyB, vFrameInA, vFrameInB )
{
    _EvaluateJointState();
}
JointFixed::~JointFixed()
{
    // nothing to do
}

Void JointFixed::Evaluate( Scalar fInvTimeStep, Scalar fDefaultERP, Scalar fDefaultCFM )
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

    // Fix all 3 angular axes
    m_iRowCount += 3;

        // Jacobians
    m_arrRows[3].constrJacobian.MakeAngular(
        Vector3::eI, m_pBodyA->GetInvInertiaTensor(), m_pBodyB->GetInvInertiaTensor()
    );
    m_arrRows[4].constrJacobian.MakeAngular(
        Vector3::eJ, m_pBodyA->GetInvInertiaTensor(), m_pBodyB->GetInvInertiaTensor()
    );
    m_arrRows[5].constrJacobian.MakeAngular(
        Vector3::eK, m_pBodyA->GetInvInertiaTensor(), m_pBodyB->GetInvInertiaTensor()
    );

        // Error term
    Vector3 vAxisA, vAxisB;
    m_vJointTransformA.GetRotate().GetColumn( vAxisA, 2 );
    m_vJointTransformB.GetRotate().GetColumn( vAxisB, 2 );

    Vector3 vAngularError = ( vAxisA ^ vAxisB ) * fK;
    m_arrRows[3].fError = vAngularError.X;
    m_arrRows[3].fCFM = fCFM;
    m_arrRows[4].fError = vAngularError.Y;
    m_arrRows[4].fCFM = fCFM;

    m_vJointTransformA.GetRotate().GetColumn( vAxisA, 1 );
    m_vJointTransformB.GetRotate().GetColumn( vAxisB, 1 );

    vAngularError = ( vAxisA ^ vAxisB ) * fK;
    m_arrRows[5].fError = vAngularError.Z;
    m_arrRows[5].fCFM = fCFM;

        // Limits
    m_arrRows[3].fLowerLimit = -m_fBreakingThreshold;
    m_arrRows[3].fUpperLimit = m_fBreakingThreshold;
    m_arrRows[4].fLowerLimit = -m_fBreakingThreshold;
    m_arrRows[4].fUpperLimit = m_fBreakingThreshold;
    m_arrRows[5].fLowerLimit = -m_fBreakingThreshold;
    m_arrRows[5].fUpperLimit = m_fBreakingThreshold;
}

