/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Joints/JointGear.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Joints : Gear joint (angular coupling)
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
#include "JointGear.h"

/////////////////////////////////////////////////////////////////////////////////
// JointGear implementation
JointGear::JointGear( BodyNode * pBodyA, BodyNode * pBodyB, const Transform3 & vJointFrame, Scalar fRatio ):
    JointConstraint( pBodyA, pBodyB, vJointFrame )
{
    m_fRatio = fRatio;

    _EvaluateJointState();
}
JointGear::JointGear( BodyNode * pBodyA, BodyNode * pBodyB, const Transform3 & vFrameInA, const Transform3 & vFrameInB, Scalar fRatio ):
    JointConstraint( pBodyA, pBodyB, vFrameInA, vFrameInB )
{
    m_fRatio = fRatio;

    _EvaluateJointState();
}
JointGear::~JointGear()
{
    // nothing to do
}

Void JointGear::Evaluate( Scalar /*fInvTimeStep*/, Scalar /*fDefaultERP*/, Scalar fDefaultCFM )
{
    // Update joint state
    _EvaluateJointState();

    Vector3 vAxisA, vAxisB;
    m_vJointTransformA.GetRotate().GetColumn( vAxisA, 2 );
    m_vJointTransformB.GetRotate().GetColumn( vAxisB, 2 );

    // Angular Coupling
    m_iRowCount = 1;

        // Jacobian
    m_arrRows[0].constrJacobian.MakeAngular(
        vAxisA, vAxisB * m_fRatio, m_pBodyA->GetInvInertiaTensor(), m_pBodyB->GetInvInertiaTensor()
    );

        // Error term & limits
    m_arrRows[0].fError = 0.0f;
    m_arrRows[0].fCFM = fDefaultCFM;
    m_arrRows[0].fLowerLimit = -m_fBreakingThreshold;
    m_arrRows[0].fUpperLimit = m_fBreakingThreshold;
}

