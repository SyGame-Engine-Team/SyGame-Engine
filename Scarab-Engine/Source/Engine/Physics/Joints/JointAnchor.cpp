/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Joints/JointAnchor.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Joints : Anchor point (often called Ball-Socket)
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
#include "JointAnchor.h"

/////////////////////////////////////////////////////////////////////////////////
// JointAnchor implementation
JointAnchor::JointAnchor( BodyNode * pBodyA, BodyNode * pBodyB, const Transform3 & vJointFrame ):
    JointConstraint( pBodyA, pBodyB, vJointFrame )
{
    _EvaluateJointState();
}
JointAnchor::JointAnchor( BodyNode * pBodyA, BodyNode * pBodyB, const Transform3 & vFrameInA, const Transform3 & vFrameInB ):
    JointConstraint( pBodyA, pBodyB, vFrameInA, vFrameInB )
{
    _EvaluateJointState();
}
JointAnchor::~JointAnchor()
{
    // nothing to do
}

Void JointAnchor::Evaluate( Scalar fInvTimeStep, Scalar fDefaultERP, Scalar fDefaultCFM )
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
}


