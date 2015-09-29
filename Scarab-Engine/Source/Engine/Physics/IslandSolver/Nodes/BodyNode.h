/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/IslandSolver/Nodes/BodyNode.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Island Node : Bodies
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
#ifndef SCARAB_ENGINE_PHYSICS_ISLANDSOLVER_NODES_BODYNODE_H
#define SCARAB_ENGINE_PHYSICS_ISLANDSOLVER_NODES_BODYNODE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Shape/Shape.h"
#include "../../Shape/TransformedShape.h"

#include "../IslandNode.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
enum BodyType
{
    BODY_TYPE_RIGID = 0,
    BODY_TYPE_SOFT,
    BODY_TYPE_KINEMATIC,

    BODY_TYPE_COUNT
};

#define BODY_MAX_COLLISION_GROUPS 64
typedef QWord BodyCollisionGroup;
typedef QWord BodyCollisionMask;

#define BODY_DAMPING_LINEAR_DEFAULT  0.2f
#define BODY_DAMPING_ANGULAR_DEFAULT 0.2f

//#define BODY_MAX_LINEAR_VELOCITY  100.0f
//#define BODY_MAX_ANGULAR_VELOCITY (SCALAR_PI * 0.25f)

/////////////////////////////////////////////////////////////////////////////////
// The BodyNode class
class BodyNode : public IslandNode
{
public:
    BodyNode( Bool bStatic, Shape * pLocalShape, Scalar fDensity,
              const Vertex3 & vPosition, const Quaternion & qOrientation,
              const Vector3 & vLinearSpeed, const Vector3 & vAngularSpeed );
    virtual ~BodyNode();

    // Type
    inline virtual IslandNodeType GetType() const;

    virtual BodyType GetBodyType() const = 0;

    inline Bool IsRigid() const;
    inline Bool IsSoft() const;
    inline Bool IsKinematic() const;

    // Behaviour
    virtual Bool IsInsulator() const = 0;

    inline Bool IsStatic() const;

    // Collision filtering access (toggles collision detection)
    inline BodyCollisionGroup GetCollisionGroup() const;
    inline Void SetCollisionGroup( BodyCollisionGroup iGroup );

    inline BodyCollisionMask GetCollisionMask() const;
    inline Void SetCollisionMask( BodyCollisionMask iMask );

    static Bool ShouldCollide( const BodyNode * pA, const BodyNode * pB );

    // Sensor access (toggles collision response)
    inline Bool IsSensor() const;
    inline Void SetSensor( Bool bSensor );

    // Sleeping access
    inline Bool IsSleeping() const;
    inline Void SetSleeping( Bool bSleeping );

    inline UInt GetSleepTime() const;
    inline Void IncrementSleepTime();
    inline Void ResetSleepTime();

    static Bool CheckSleepers( BodyNode * pA, BodyNode * pB );

    // Shape access
    inline Shape * GetLocalShape() const;
    inline TransformedShape * GetWorldShape() const;

    // Mass properties access
    inline Scalar GetDensity() const;

    inline const Vertex3 & GetCenterOfMass() const;

    inline Scalar GetMass() const;
    inline Scalar GetInvMass() const;

    inline const Matrix3 & GetInertiaTensor() const;
    inline const Matrix3 & GetInvInertiaTensor() const;

    // Physical properties access
    inline Scalar GetRestitution() const;              // <=> Hardness for
    inline Void SetRestitution( Scalar fRestitution ); // softbodies ...

    inline Scalar GetFriction() const;
    inline Void SetFriction( Scalar fFriction );

    inline Scalar GetRollingFriction() const;
    inline Void SetRollingFriction( Scalar fRollingFriction );

    // Damping access
    inline Scalar GetLinearDamping() const;
    inline Void SetLinearDamping( Scalar fDamping );

    inline Scalar GetAngularDamping() const;
    inline Void SetAngularDamping( Scalar fDamping );

    // Force & Torque access
    inline const Vector3 & GetForce() const;
    inline const Vector3 & GetTorque() const;

    inline Void ResetForce();
    inline Void ResetTorque();

    inline Void ApplyForce( const Vector3 & vForce );
    inline Void ApplyTorque( const Vector3 & vTorque );
    inline Void ApplyForce( const Vector3 & vForce, const Vertex3 & vWorldPoint );
    inline Void ApplyForce( const Vector3 & vForce, const Vector3 & vLocalPoint );

    // Impulse access
    inline const Vector3 & GetLinearImpulse() const;
    inline const Vector3 & GetAngularImpulse() const;

    inline Void ResetLinearImpulse();
    inline Void ResetAngularImpulse();

    inline virtual Void ApplyLinearImpulse( const Vector3 & vImpulse );
	inline virtual Void ApplyAngularImpulse( const Vector3 & vImpulse );
    inline virtual Void ApplyImpulse( const Vector3 & vImpulse, const Vertex3 & vWorldPoint );
    inline virtual Void ApplyImpulse( const Vector3 & vImpulse, const Vector3 & vLocalPoint );

    inline Void AccumulateLinearImpulse( const Vector3 & vImpulse );
    inline Void AccumulateAngularImpulse( const Vector3 & vImpulse );
    inline Void AccumulateImpulse( const Vector3 & vImpulse, const Vertex3 & vWorldPoint );
    inline Void AccumulateImpulse( const Vector3 & vImpulse, const Vector3 & vLocalPoint );
    inline Void AccumulateImpulse( const Vector3 & vLinearAxis, const Vector3 & vAngularAxis, Scalar fImpulse );
    inline virtual Void ApplyAccumulatedImpulses();

    // Correction access
    inline const Vector3 & GetLinearCorrection() const;
    inline const Vector3 & GetAngularCorrection() const;

    inline Void ResetLinearCorrection();
    inline Void ResetAngularCorrection();

    inline virtual Void ApplyLinearCorrection( const Vector3 & vCorrection, Scalar fTimeStep );
	inline virtual Void ApplyAngularCorrection( const Vector3 & vCorrection, Scalar fTimeStep );
    inline virtual Void ApplyCorrection( const Vector3 & vCorrection, const Vertex3 & vWorldPoint, Scalar fTimeStep );
    inline virtual Void ApplyCorrection( const Vector3 & vCorrection, const Vector3 & vLocalPoint, Scalar fTimeStep );

    inline Void AccumulateLinearCorrection( const Vector3 & vCorrection );
    inline Void AccumulateAngularCorrection( const Vector3 & vCorrection );
    inline Void AccumulateCorrection( const Vector3 & vCorrection, const Vertex3 & vWorldPoint );
    inline Void AccumulateCorrection( const Vector3 & vCorrection, const Vector3 & vLocalPoint );
    inline Void AccumulateCorrection( const Vector3 & vLinearAxis, const Vector3 & vAngularAxis, Scalar fCorrection );
    inline virtual Void ApplyAccumulatedCorrections( Scalar fTimeStep );

    // State access
    inline const Vertex3 & GetPosition() const;
    inline const Quaternion & GetOrientation() const;
    inline const Transform3 & GetTransform() const;

    inline const Vector3 & GetLinearSpeed() const;
    inline const Vector3 & GetAngularSpeed() const;

    inline Vector3 GetLinearSpeedAt( const Vertex3 & vWorldPoint ) const;
    inline Vector3 GetLinearSpeedAt( const Vector3 & vLocalPoint ) const;

    inline Vector3 GetAngularSpeedAt( const Vertex3 & vWorldPoint ) const;
    inline Vector3 GetAngularSpeedAt( const Vector3 & vLocalPoint ) const;

    // Integration
    virtual Void IntegrateVelocity( Scalar fTimeStep ) = 0;
    virtual Void IntegratePosition( Scalar fTimeStep ) = 0;

protected:
    // Helpers
    static const Matrix3 sm_matInfinity; // Helper for infinite inertia tensors

    // Behaviour
    Bool m_bStatic; // Kinematics : A static kinematic means he doesn't accept external constraints
                    //              while a non-static one can be constrained and have his motion altered.

    // Collision filtering
    BodyCollisionGroup m_iCollisionGroup;
    BodyCollisionMask m_iCollisionMask;

    // Sensor
    Bool m_bSensor;

    // Sleeping
    Bool m_bSleeping;  // Kinematics : A sleeping kinematic has all its controllers disabled/paused/stopped/finished.
    UInt m_iSleepTime; //              If at least one controller gives motion to him, it is awake.

    // Shape
    Shape * m_pLocalShape;
    TransformedShape * m_pWorldShape;

    // Mass properties
    Scalar m_fDensity;
    Scalar m_fMass, m_fInvMass;
    Vertex3 m_vCenterOfMass;
    Matrix3 m_matInertiaTensor;    // in COM
    Matrix3 m_matInvInertiaTensor; // space

    // Physical properties
    Scalar m_fRestitution;
    Scalar m_fFriction;
    Scalar m_fRollingFriction;

    // Damping
    Scalar m_fLinearDamping;
    Scalar m_fAngularDamping;

    // Force buffers
    Vector3 m_vForceBuffer;
    Vector3 m_vTorqueBuffer;

    // Impulse buffers
    Vector3 m_vLinearImpulseBuffer;
    Vector3 m_vAngularImpulseBuffer;

    // Correction buffers
    Vector3 m_vLinearCorrectionBuffer;
    Vector3 m_vAngularCorrectionBuffer;

    // State
    Vertex3 m_vPosition;
    Quaternion m_qOrientation;
    Transform3 m_vTransform;

    Vector3 m_vLinearSpeed;
    Vector3 m_vAngularSpeed;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "BodyNode.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_ISLANDSOLVER_NODES_BODYNODE_H

