/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Joints/JointConstraint.h
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
// Header prelude
#ifndef SCARAB_ENGINE_PHYSICS_JOINTS_JOINTCONSTRAINT_H
#define SCARAB_ENGINE_PHYSICS_JOINTS_JOINTCONSTRAINT_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../IslandSolver/Nodes/BodyNode.h"

#include "../ConstraintSolver/JacobianConstraint.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
    // Joint types
enum JointConstraintType
{
    JOINT_GENERIC6DOF = 0, // 3 Linear DOF, 3 Angular DOF

    // Static, usual standard joints
    JOINT_FIXED,  // 0 DOF
    JOINT_ANCHOR, // 3 Angular DOF

    JOINT_SLIDER, // 1 Linear DOF

    JOINT_HINGE,     // 1 Angular DOF
    JOINT_WHEEL,     // 2 Angular DOF, axes x & z, orthogonal
    JOINT_UNIVERSAL, // 2 Angular DOF, axes x & y, orthogonal
    JOINT_CONETWIST, // 3 Angular DOF, conic rotation & twist

    //JOINT_SCREWER,      // 1 Linear DOF, 1 Angular DOF, same axis
    //JOINT_RAILPENDULUM, // 1 Linear DOF, 1 Angular DOF, axes x & y
    //JOINT_STEERWHEEL,   // 1 Linear DOF, 1 Angular DOF, axes x & z

    // Dynamic, behaviour-driven joints
    JOINT_GEAR,   // Angular coupling constraint
    //JOINT_ROPE, // Free movement inside sphere with rope radius

};

    // Joint axes
#define JOINT_MAX_ROW_COUNT 6

enum JointAxis {
        // For JointGeneric6DOF & derivatives
    JOINTAXIS_LINEAR_0 = 0,
    JOINTAXIS_LINEAR_1,
    JOINTAXIS_LINEAR_2,
    JOINTAXIS_ANGULAR_0,
    JOINTAXIS_ANGULAR_1,
    JOINTAXIS_ANGULAR_2,

        // For JointConeTwist
    JOINTAXIS_SWING = 0,
    JOINTAXIS_TWIST
};

    // Joint limit states
enum JointLimitState {
    JOINTLIMIT_FREE = 0, // lower > upper (disabled) or lower <= impulse <= upper, impulse is free
    JOINTLIMIT_LOWER,    // impulse < lower <= upper, impulse is clamped to lower bound
    JOINTLIMIT_UPPER     // lower <= upper < impulse, impulse is clamped to upper bound
};

    // Joint constraint rows
typedef struct _joint_constraint_row {
    JacobianConstraint constrJacobian;
    Scalar fError;
    Scalar fCFM;
    Scalar fLowerLimit;
    Scalar fUpperLimit;
    Scalar fImpulse;
} JointConstraintRow;

    // Joint feedback
typedef struct _joint_feedback {
    Vector3 vForceBodyA;
    Vector3 vTorqueBodyA;
    Vector3 vForceBodyB;
    Vector3 vTorqueBodyB;
} JointFeedback;

    // Prototypes
class JointLimitMotorSpring;
class JointConstraint;

/////////////////////////////////////////////////////////////////////////////////
// The JointLimitMotorSpring class
class JointLimitMotorSpring
{
public:
    JointLimitMotorSpring( Bool bAngularElseLinear = false );
    JointLimitMotorSpring( const JointLimitMotorSpring & rhs );
    ~JointLimitMotorSpring();

    JointLimitMotorSpring & operator=( const JointLimitMotorSpring & rhs );

    // Deferred construction
    Void MakeLinear();
    Void MakeAngular();

    // Type access
    inline Bool IsLinear() const;
    inline Bool IsAngular() const;

    // Limits access
    inline Bool IsFree() const;
    inline Bool IsLimited() const;
    inline Bool IsFixed() const;
    inline Scalar GetLowerLimit() const;
    inline Scalar GetUpperLimit() const;

    inline Void EnableLimits( Scalar fLowerLimit, Scalar fUpperLimit );
    inline Void DisableLimits();

    Scalar LimitRestitution; // Default = Linear 0.5f / Angular 0.0f
    Scalar LimitSoftness;    // Default = Linear 0.7f / Angular 0.5f
    Scalar LimitERP;         // Default = 0.2f
    Scalar LimitCFM;         // Default = 0.0f
    Scalar LimitMaxImpulse;  // Default = 300.0f (Bullet speed !)

    // Motor access
    inline Bool HasMotor() const;
    inline Bool IsServoMotor() const;
    inline Scalar GetMotorTargetVelocity() const;
    inline Scalar GetServoMotorTargetPosition() const;
    inline Scalar GetMotorMaxImpulse() const;

    inline Void EnableMotor( Scalar fTargetVelocity, Scalar fMaxImpulse );
    inline Void EnableServoMotor( Scalar fTargetVelocity, Scalar fTargetPosition, Scalar fMaxImpulse );
    inline Void DisableMotor();

    Scalar MotorERP; // Default = 0.9f
    Scalar MotorCFM; // Default = 0.0f

    // Spring access
    inline Bool HasSpring() const;
    inline Scalar GetSpringStiffness() const;
    inline Scalar GetSpringEquilibriumPoint() const;

    inline Void EnableSpring( Scalar fStiffness, Scalar fEquilibriumPoint );
    inline Void DisableSpring();

    Scalar SpringDamping; // Default = 1.0f

    // State testing
    Void TestLimit( Scalar fPositionOrAngle ); // Sets Position/Angle, Angle is normalized & adjusted to limits

    inline Scalar GetTestPosition() const;
    inline Scalar GetTestAngle() const;

    inline JointLimitState GetLimitState() const;
    inline Scalar GetLimitError() const;

    inline Bool NeedCorrection() const;

private:
    // Helper for JointConstraint::_EvaluateLimitMotor()
    friend class JointConstraint;
    Scalar _GetMotorFactor( Scalar fTargetVelocity, Scalar fInvTimeStep, Scalar fOverrideLowerLimit = 1.0f, Scalar fOverrideUpperLimit = -1.0f ) const;

    // Type data
    Bool m_bAngularElseLinear;

    // Limit data
    Scalar m_fLowerLimit; // Limit is enabled if
    Scalar m_fUpperLimit; // lower <= upper

    // Motor data
    Bool m_bMotorEnabled;
    Bool m_bIsServoMotor;
    Scalar m_fMotorTargetVelocity;
    Scalar m_fServoMotorTargetPosition;
    Scalar m_fMotorMaxImpulse;

    // Spring data
    Bool m_bSpringEnabled;
    Scalar m_fSpringStiffness;
    Scalar m_fSpringEquilibriumPoint;

    // State testing
    Scalar m_fTestPositionOrAngle;

    JointLimitState m_iLimitState;
    Scalar m_fLimitError;
};

/////////////////////////////////////////////////////////////////////////////////
// The JointConstraint class
class JointConstraint
{
public:
    JointConstraint( BodyNode * pBodyA, BodyNode * pBodyB, const Transform3 & vJointFrame );
    JointConstraint( BodyNode * pBodyA, BodyNode * pBodyB, const Transform3 & vFrameInA, const Transform3 & vFrameInB );
    virtual ~JointConstraint();

    // Type
    virtual JointConstraintType GetType() const = 0;

    // Bodies access
    inline BodyNode * GetBodyA() const;
    inline BodyNode * GetBodyB() const;

    // Parameters access
    inline const Transform3 & GetFrameInA() const;            // w.r.t A
    inline const Transform3 & GetFrameInB() const;            // w.r.t B
    inline Void GetFrame( Transform3 * outJointFrame ) const; // w.r.t World

    Void SetFrame( const Transform3 & vJointFrame );
    Void SetFrame( const Transform3 & vFrameInA, const Transform3 & vFrameInB );

    // Breaking support
    inline Bool IsEnabled() const;
    inline Void Enable();
    inline Void Disable();

    inline Scalar GetBreakingThreshold() const;
    inline Void SetBreakingThreshold( Scalar fBreakingThreshold );

    // Feedback support
    inline Bool IsFeedbackEnabled() const;
    inline Void EnableFeedback();
    inline Void DisableFeedback();

    inline JointFeedback * GetFeedback() const;
    inline Void SetFeedback( JointFeedback * pJointFeedback );

    // Joint transform access
    inline const Transform3 & GetTransformA() const;
    inline const Transform3 & GetTransformB() const;

    // Constraint evaluation
    inline UInt GetRowCount() const;
    inline JointConstraintRow * GetRow( UInt iRow );

    virtual Void Evaluate( Scalar fInvTimeStep, Scalar fDefaultERP, Scalar fDefaultCFM ) = 0;

protected:
    // LMS support for DOF axes control
    UInt _EvaluateLimitMotorSpring( JointConstraintRow * outRow, const JointLimitMotorSpring * pLimitMotorSpring, Scalar fInvTimeStep,
                                    const Vector3 & vAxis, const Vector3 & vAnchorA = Vector3::Null, const Vector3 & vAnchorB = Vector3::Null );

    // Bodies
    BodyNode * m_pBodyA;
    BodyNode * m_pBodyB;

    Transform3 m_vFrameInA;
    Transform3 m_vFrameInB;

    // Constraint rows
    UInt m_iRowCount;
    JointConstraintRow m_arrRows[JOINT_MAX_ROW_COUNT];

    // Constraint frame offset support
    Void _AdjustAnchors( Vector3 * pAnchorA, Vector3 * pAnchorB, const Vector3 & vAxis, Scalar fOffset, Bool bAllowRotation );

    Bool m_bHasStaticBody;
    Scalar m_fWeightFactorA;
    Scalar m_fWeightFactorB;

    // Breaking support
    Bool m_bEnabled;
    Scalar m_fBreakingThreshold;

    // Feedback support
    Bool m_bFeedbackEnabled;
    JointFeedback * m_pFeedback;

    // Joint state
    virtual Void _EvaluateJointState();

    Transform3 m_vJointTransformA;
    Transform3 m_vJointTransformB;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "JointConstraint.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_JOINTS_JOINTCONSTRAINT_H



