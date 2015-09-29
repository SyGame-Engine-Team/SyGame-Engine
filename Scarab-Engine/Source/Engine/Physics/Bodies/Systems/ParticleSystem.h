/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Bodies/Systems/ParticleSystem.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : System : Particles
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : TODO = Add flags for collision filters with environment and / or
//                     between particles ...
//                     Add non-uniform distributions to shape/motion patterns,
//                     needs them implemented in /Lib/Math/Random/Random.h
//                     Add more patterns (helpers for fire, water, snow, ...)
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_PHYSICS_BODIES_SYSTEMS_PARTICLESYSTEM_H
#define SCARAB_ENGINE_PHYSICS_BODIES_SYSTEMS_PARTICLESYSTEM_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../Lib/Datastruct/Array/Array.h"

#include "../../../../Lib/Math/Random/Random.h"

#include "../../../../Lib/Math/Geometry/Primitives/3D/Planar/Plane.h"
#include "../../../../Lib/Math/Geometry/Primitives/3D/Planar/Circle3.h"
#include "../../../../Lib/Math/Geometry/Primitives/3D/Sphere.h"
#include "../../../../Lib/Math/Geometry/Primitives/3D/Box.h"

#include "../../Shape/Primitive/ShapeSphere.h"
#include "../../Controllers/Motion/ParticleController.h"

#include "../SystemBody.h"
#include "../KinematicBody.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define PARTICLE_SIZE_DEFAULT 0.1f

    // Emitter types
enum ParticleEmitterType
{
    PARTICLE_EMITTER_POINT = 0,
    PARTICLE_EMITTER_QUAD,
    PARTICLE_EMITTER_DISK
};

/////////////////////////////////////////////////////////////////////////////////
// The ParticleEmitter class
class ParticleEmitter
{
public:
    ParticleEmitter();
    ~ParticleEmitter();

    ParticleController * SpawnParticle();

    ParticleEmitterType Type;

    Vertex3 Position;
    Vector3 PositionVariance;

    Vector3 Orientation; // Euler angles
    Vector3 OrientationVariance;

    Vector3 LinearSpeed;
    Vector3 LinearSpeedVariance;

    Vector3 AngularSpeed;
    Vector3 AngularSpeedVariance;

    Vector3 Force;
    Vector3 ForceVariance;

    Vector3 Torque;
    Vector3 TorqueVariance;

    UInt LifeTime;
    UInt LifeTimeVariance;
};

/////////////////////////////////////////////////////////////////////////////////
// The ParticleSystem class
//class ParticleSystem : public SystemBody
//{
//public:
//    ParticleSystem( Scalar fParticleSize = PARTICLE_SIZE_DEFAULT );
//    virtual ~ParticleSystem();
//
//    // Type
//    inline virtual SystemBodyType GetType() const;
//
//    // System control
//    inline const Vector3 & GetLinearSpeed() const;
//    inline const Vector3 & GetAngularSpeed() const;
//
//    inline Void SetLinearSpeed( const Vector3 & vLinearSpeed );
//    inline Void SetAngularSpeed( const Vector3 & vAngularSpeed );
//
//    // Particles control
//    inline UInt GetParticleCount() const;
//
//    inline UInt GetActiveParticleCount() const;
//    inline Void SetActiveParticleCount( UInt iActiveParticleCount );
//
//    inline KinematicBody * GetParticleBody( UInt iParticle ) const;
//    inline ManualController * GetParticleController( UInt iParticle ) const;
//    inline const Vector3 & GetParticleLinearSpeed( UInt iParticle ) const;
//    inline const Vector3 & GetParticleAngularSpeed( UInt iParticle ) const;
//
//    inline Void SetParticleLinearSpeed( UInt iParticle, const Vector3 & vLinearSpeed );
//    inline Void SetParticleAngularSpeed( UInt iParticle, const Vector3 & vAngularSpeed );
//
//    Void SetAllParticlesLinearSpeed( const Vector3 & vLinearSpeed );
//    Void SetAllParticlesAngularSpeed( const Vector3 & vAngularSpeed );
//
//    Void PushParticle( const Vertex3 & vPosition = Vertex3::Null,
//                       const Quaternion & qOrientation = Quaternion::Identity );
//    Void PopParticle();
//    Void PopAllParticles();
//
//    // Particle generators
//    Void GeneratePlane( const Plane & vPlane, const Vector3 & vAxisU, const Vector3 & vAxisV,
//                        Scalar fExtentU, Scalar fExtentV, UInt iParticleCount );
//    Void GenerateCircle( const Circle3 & vCircle, UInt iParticleCount );
//
//    Void GenerateBox( const Box & vBox, UInt iParticleCount );
//    Void GenerateSphere( const Sphere & vSphere, UInt iParticleCount );
//
//    // Speed-Field generators
//    Void GenerateLinearField( const Vector3 & vLinearAxis, Scalar fMinSpeed, Scalar fMaxSpeed, Scalar fMaxDeviation = 0.0f );
//    Void GenerateAngularField( const Vector3 & vAngularAxis, Scalar fMinSpeed, Scalar fMaxSpeed, Scalar fMaxDeviation = 0.0f );
//
//    // Update support
//    virtual Void Update( Scalar fTimeStep );
//
//private:
//    // Helpers
//    class _Particle
//    {
//    public:
//        _Particle();
//        _Particle( const _Particle & rhs );
//        ~_Particle();
//
//        inline _Particle & operator=( const _Particle & rhs );
//
//        // Particle body & controller
//        KinematicBody * pBody;
//        ManualController * pController;
//
//        // Particles parameters
//        Vector3 vLinearSpeed;
//        Vector3 vAngularSpeed;
//    };
//
//    inline static Scalar _RandScalar_0_1(); // Rands in [0;1]
//    inline static Scalar _RandScalar_M1_1(); // Rands in [-1;1]
//    inline static Scalar _RandScalar_Range( Scalar fMin, Scalar fMax ); // Rands in [min;max]
//
//    // System parameters
//    Vector3 m_vLinearSpeed;
//    Vector3 m_vAngularSpeed;
//
//    // Shape instance
//    Sphere m_vParticleSphere;
//    ShapeSphere m_sParticleShape;
//
//    // Particles
//    UInt m_iActiveParticleCount;
//    Array<_Particle> m_arrParticles;
//};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "ParticleSystem.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_BODIES_SYSTEMS_PARTICLESYSTEM_H

