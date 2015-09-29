/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Joints/JointConeTwist.h
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
// Header prelude
#ifndef SCARAB_ENGINE_PHYSICS_JOINTS_JOINTCONETWIST_H
#define SCARAB_ENGINE_PHYSICS_JOINTS_JOINTCONETWIST_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "JointConstraint.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define JOINT_CONETWIST_MINIMAL_SPAN 0.05f

/////////////////////////////////////////////////////////////////////////////////
// The JointConeTwist class
class JointConeTwist : public JointConstraint
{
public:
    JointConeTwist( BodyNode * pBodyA, BodyNode * pBodyB, const Transform3 & vJointFrame );
    JointConeTwist( BodyNode * pBodyA, BodyNode * pBodyB, const Transform3 & vFrameInA, const Transform3 & vFrameInB );
    virtual ~JointConeTwist();

    // Type
    inline virtual JointConstraintType GetType() const;

    // Limits, Motors & Springs
    inline JointLimitMotorSpring * GetLimitMotorSpring( JointAxis iAxis );

    inline Bool IsFree( JointAxis iAxis ) const;
    inline Bool IsLimited( JointAxis iAxis ) const;
    inline Bool IsFixed( JointAxis iAxis ) const;
    inline Scalar GetLowerLimit( JointAxis iAxis ) const;
    inline Scalar GetUpperLimit( JointAxis iAxis ) const;

    inline Void SetSwingLimits( Scalar fSwingSpan1, Scalar fSwingSpan2 ); // Angular span projecting as an ellipse on YZ-plane
    inline Void EnableTwistLimits( Scalar fLowerLimit, Scalar fUpperLimit );
    inline Void DisableTwistLimits();

    inline Bool HasMotor( JointAxis iAxis ) const;
    inline Bool IsServoMotor( JointAxis iAxis ) const;
    inline Scalar GetMotorTargetVelocity( JointAxis iAxis ) const;
    inline Scalar GetServoMotorTargetPosition( JointAxis iAxis ) const;
    inline Scalar GetMotorMaxImpulse( JointAxis iAxis ) const;

    inline Void EnableMotor( JointAxis iAxis, Scalar fTargetVelocity, Scalar fMaxImpulse );
    inline Void EnableServoMotor( JointAxis iAxis, Scalar fTargetVelocity, Scalar fTargetPosition, Scalar fMaxImpulse );
    inline Void DisableMotor( JointAxis iAxis );

    //Void EnableServoMotor( Scalar fTargetVelocity, const Quaternion & qTarget, Scalar fMaxImpulse );

    inline Bool HasSpring( JointAxis iAxis ) const;
    inline Scalar GetSpringStiffness( JointAxis iAxis ) const;
    inline Scalar GetSpringEquilibriumPoint( JointAxis iAxis ) const;

    inline Void EnableSpring( JointAxis iAxis, Scalar fStiffness, Scalar fEquilibriumPoint );
    inline Void DisableSpring( JointAxis iAxis );

    // Joint transform access
    inline Scalar GetSwingRotation( Vector3 * outSwingAxis ) const;
    inline Scalar GetTwistRotation( Vector3 * outTwistAxis ) const;

    // Constraint evaluation
    virtual Void Evaluate( Scalar fInvTimeStep, Scalar fDefaultERP, Scalar fDefaultCFM );

protected:
    // Joint data
    Scalar m_fSwingSpan1;
    Scalar m_fSwingSpan2;

    // Joint state data
    virtual Void _EvaluateJointState();

    Vector3 m_vSwingAxis;
    Scalar m_fSwingAngle;
    
    Vector3 m_vTwistAxis;
    Scalar m_fTwistAngle;

    // Constraint rows
    JointLimitMotorSpring m_hSwingLMS;
    JointLimitMotorSpring m_hTwistLMS;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "JointConeTwist.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_JOINTS_JOINTCONETWIST_H

