/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Joints/JointGeneric6DOF.h
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
// Header prelude
#ifndef SCARAB_ENGINE_PHYSICS_JOINTS_JOINTGENERIC6DOF_H
#define SCARAB_ENGINE_PHYSICS_JOINTS_JOINTGENERIC6DOF_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Math/Algebra/Matrix/EulerDecomposition.h"

#include "JointConstraint.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The JointGeneric6DOF class
class JointGeneric6DOF : public JointConstraint
{
public:
    JointGeneric6DOF( BodyNode * pBodyA, BodyNode * pBodyB, const Transform3 & vJointFrame, EulerAngles iEulerAnglesOrdering = EULER_ANGLES_XYZ );
    JointGeneric6DOF( BodyNode * pBodyA, BodyNode * pBodyB, const Transform3 & vFrameInA, const Transform3 & vFrameInB, EulerAngles iEulerAnglesOrdering = EULER_ANGLES_XYZ );
    virtual ~JointGeneric6DOF();

    // Type
    inline virtual JointConstraintType GetType() const;

    // Parameters
    inline EulerAngles GetEulerAnglesOrdering() const;

    // Limits, Motors & Springs
    inline JointLimitMotorSpring * GetLimitMotorSpring( JointAxis iAxis );

    inline Bool IsFree( JointAxis iAxis ) const;
    inline Bool IsLimited( JointAxis iAxis ) const;
    inline Bool IsFixed( JointAxis iAxis ) const;
    inline Scalar GetLowerLimit( JointAxis iAxis ) const;
    inline Scalar GetUpperLimit( JointAxis iAxis ) const;

    inline Void EnableLimits( JointAxis iAxis, Scalar fLowerLimit, Scalar fUpperLimit );
    inline Void DisableLimits( JointAxis iAxis );

    inline Bool HasMotor( JointAxis iAxis ) const;
    inline Bool IsServoMotor( JointAxis iAxis ) const;
    inline Scalar GetMotorTargetVelocity( JointAxis iAxis ) const;
    inline Scalar GetServoMotorTargetPosition( JointAxis iAxis ) const;
    inline Scalar GetMotorMaxImpulse( JointAxis iAxis ) const;

    inline Void EnableMotor( JointAxis iAxis, Scalar fTargetVelocity, Scalar fMaxImpulse );
    inline Void EnableServoMotor( JointAxis iAxis, Scalar fTargetVelocity, Scalar fTargetPosition, Scalar fMaxImpulse );
    inline Void DisableMotor( JointAxis iAxis );

    inline Bool HasSpring( JointAxis iAxis ) const;
    inline Scalar GetSpringStiffness( JointAxis iAxis ) const;
    inline Scalar GetSpringEquilibriumPoint( JointAxis iAxis ) const;

    inline Void EnableSpring( JointAxis iAxis, Scalar fStiffness, Scalar fEquilibriumPoint );
    inline Void DisableSpring( JointAxis iAxis );

    // Joint transform access
    inline Scalar GetTranslation( JointAxis iAxis ) const;
    inline Scalar GetRotation( Vector3 * outAxis, JointAxis iAxis ) const;

    // Constraint evaluation
    virtual Void Evaluate( Scalar fInvTimeStep, Scalar fDefaultERP, Scalar fDefaultCFM );

protected:
    // Joint data
    EulerAngles m_iEulerAnglesOrdering;

    // Joint state data
    virtual Void _EvaluateJointState();

    Vector3 m_vTranslation;
    Vector3 m_vRotationAxis[3];
    Vector3 m_vRotationEulerAngles;

    // Constraint rows
    JointLimitMotorSpring m_hLinearLMS[3];
    JointLimitMotorSpring m_hAngularLMS[3];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "JointGeneric6DOF.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_JOINTS_JOINTGENERIC6DOF_H




