/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Joints/JointSlider.h
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
// Header prelude
#ifndef SCARAB_ENGINE_PHYSICS_JOINTS_JOINTSLIDER_H
#define SCARAB_ENGINE_PHYSICS_JOINTS_JOINTSLIDER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "JointConstraint.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The JointSlider class
class JointSlider : public JointConstraint
{
public:
    JointSlider( BodyNode * pBodyA, BodyNode * pBodyB, const Transform3 & vJointFrame );
    JointSlider( BodyNode * pBodyA, BodyNode * pBodyB, const Transform3 & vFrameInA, const Transform3 & vFrameInB );
    virtual ~JointSlider();

    // Type
    inline virtual JointConstraintType GetType() const;

    // Limits, Motors & Springs
    inline JointLimitMotorSpring * GetLimitMotorSpring();

    inline Bool IsFree() const;
    inline Bool IsLimited() const;
    inline Bool IsFixed() const;
    inline Scalar GetLowerLimit() const;
    inline Scalar GetUpperLimit() const;

    inline Void EnableLimits( Scalar fLowerLimit, Scalar fUpperLimit );
    inline Void DisableLimits();

    inline Bool HasMotor() const;
    inline Bool IsServoMotor() const;
    inline Scalar GetMotorTargetVelocity() const;
    inline Scalar GetServoMotorTargetPosition() const;
    inline Scalar GetMotorMaxImpulse() const;

    inline Void EnableMotor( Scalar fTargetVelocity, Scalar fMaxImpulse );
    inline Void EnableServoMotor( Scalar fTargetVelocity, Scalar fTargetPosition, Scalar fMaxImpulse );
    inline Void DisableMotor();

    inline Bool HasSpring() const;
    inline Scalar GetSpringStiffness() const;
    inline Scalar GetSpringEquilibriumPoint() const;

    inline Void EnableSpring( Scalar fStiffness, Scalar fEquilibriumPoint );
    inline Void DisableSpring();

    // Joint transform access
    inline Scalar GetTranslation( Vector3 * outAxis ) const;

    // Constraint evaluation
    virtual Void Evaluate( Scalar fInvTimeStep, Scalar fDefaultERP, Scalar fDefaultCFM );

protected:
    // Joint state data
    virtual Void _EvaluateJointState();

    Vector3 m_vLockedAxes[2];
    Vector3 m_vSliderAxis;
    Scalar m_fSliderPosition;

    // Constraint rows
    JointLimitMotorSpring m_hLinearLMS;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "JointSlider.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_JOINTS_JOINTSLIDER_H

