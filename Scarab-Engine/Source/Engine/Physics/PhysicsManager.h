/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/PhysicsManager.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Core-Class for the Physics system (Forward Dynamics)
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
#ifndef SCARAB_ENGINE_PHYSICS_PHYSICSMANAGER_H
#define SCARAB_ENGINE_PHYSICS_PHYSICSMANAGER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Lib/Object/Patterns/Manager.h"

// Shapes
#include "Shape/TransformedShape.h"

#include "Shape/Primitive/ShapeTriangle.h"
#include "Shape/Primitive/ShapeSphere.h"
#include "Shape/Primitive/ShapeBox.h"

#include "Shape/Convex/ShapeConvexMesh.h"

#include "Shape/Concave/ShapeConcaveMesh.h"
#include "Shape/Concave/ShapeHeightField.h"

//#include "Shape/Deformable/ShapeDeformableMesh.h"

// Bodies
#include "Bodies/RigidBody.h"
#include "Bodies/SoftBody.h"
#include "Bodies/KinematicBody.h"

// Controllers
#include "Controllers/Motion/ManualController.h"
#include "Controllers/Motion/CharacterController.h"
#include "Controllers/Motion/KeyFrameController.h"
#include "Controllers/Motion/PathController.h"
#include "Controllers/Motion/BlendController.h"
#include "Controllers/Motion/ParticleController.h"
#include "Controllers/Motion/IKJointController.h"

#include "Controllers/Shape/MorphController.h"
#include "Controllers/Shape/BoneController.h"

// Joints
#include "Joints/JointGeneric6DOF.h"
#include "Joints/JointFixed.h"
#include "Joints/JointAnchor.h"
#include "Joints/JointSlider.h"
#include "Joints/JointHinge.h"
#include "Joints/JointWheel.h"
#include "Joints/JointUniversal.h"
#include "Joints/JointConeTwist.h"
#include "Joints/JointGear.h"

// Systems
#include "Bodies/Systems/ParticleSystem.h"
#include "Bodies/Systems/Skeleton.h"
#include "Bodies/Systems/RagDoll.h"

// Collision detection
#include "CollisionDetection/BroadPhase/SAP/SweepAndPrune.h"

#include "CollisionDetection/NarrowPhase/Primitive/TriangleTriangleCollision.h"
#include "CollisionDetection/NarrowPhase/Primitive/TriangleSphereCollision.h"
#include "CollisionDetection/NarrowPhase/Primitive/TriangleBoxCollision.h"
#include "CollisionDetection/NarrowPhase/Primitive/SphereSphereCollision.h"
#include "CollisionDetection/NarrowPhase/Primitive/SphereBoxCollision.h"
#include "CollisionDetection/NarrowPhase/Primitive/BoxBoxCollision.h"

#include "CollisionDetection/NarrowPhase/Convex/ConvexTriangleCollision.h"
#include "CollisionDetection/NarrowPhase/Convex/ConvexSphereCollision.h"
#include "CollisionDetection/NarrowPhase/Convex/ConvexBoxCollision.h"
#include "CollisionDetection/NarrowPhase/Convex/ConvexConvexCollision.h"

//#include "CollisionDetection/NarrowPhase/Concave/ConcaveTriangleCollision.h"
#include "CollisionDetection/NarrowPhase/Concave/ConcaveSphereCollision.h"
#include "CollisionDetection/NarrowPhase/Concave/ConcaveBoxCollision.h"
#include "CollisionDetection/NarrowPhase/Concave/ConcaveConvexCollision.h"
#include "CollisionDetection/NarrowPhase/Concave/ConcaveConcaveCollision.h"

// Solvers
#include "IslandSolver/IslandSolver.h"
#include "ConstraintSolver/ConstraintSolver.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define PhysicsFn PhysicsManager::GetInstancePtr()

// Shock propagation influence factor
// Default is disabled (0.0f), enable with any non-0 value (range = [0;1])
// 1.0f is hugely discouraged unless very specific circumstances ...
// Use 0.0f      for lightly/not stacked environments (performance gain)
// Use 0.5f/0.9f for averagely stacked environments (stacks around 4-5 objects)
// Use 0.99f     for heavily stacked environments (stacks above 10 objects)
// This tends to reduce restitution impact ...
#define PHYSICS_SHOCK_PROPAGATION 0.5f

// External forces
#define PHYSICS_FORCE_GRAVITY 9.81f

/////////////////////////////////////////////////////////////////////////////////
// The PhysicsManager class
class PhysicsManager : public Manager<PhysicsManager>
{
public:
    inline static const GChar * GetName();
    inline static const GChar * GetMemoryBudget();

protected:
    friend class Manager<PhysicsManager>;
    PhysicsManager();
    virtual ~PhysicsManager();

public:
    Void Initialize();
    Void Cleanup();

    // Shock propagation factor
    // Controls the amount of shock propagation over a given timestep,
    // ranges in [0;1] where 0 means no shock propagation at all.
    inline Scalar GetShockPropagation() const;
    inline Void SetShockPropagation( Scalar fFactor );

    // External forces
    inline const Vector3 & GetGravity() const;

    // Islands access
    inline UInt GetIslandCount() const;
    inline const Island * GetIslandList() const;

    // Shape access
    ShapeSphere * CreateShapeSphere( const Sphere & vSphere, UInt iSliceSamples, UInt iRadialSamples ) const;
    ShapeSphere * CreateShapeSphere( MeshSphere * pMesh ) const;
    ShapeBox * CreateShapeBox( const Box & vBox ) const;
    ShapeBox * CreateShapeBox( MeshBox * pMesh ) const;

    ShapeConvexMesh * CreateShapeConvexMesh( MeshVoronoiGraph * pGraph, TriangleMesh * pMesh = NULL ) const;
    ShapeConvexMesh * CreateShapeConvexMesh( TriangleMesh * pMesh ) const;

    ShapeConcaveMesh * CreateShapeConcaveMesh( const Vertex3 * arrNodes, UInt iNodeCount,
                                               const Triangle3 * arrTriangles, UInt iTriangleCount, TriangleMesh * pMesh = NULL ) const;
    ShapeConcaveMesh * CreateShapeConcaveMesh( TriangleMesh * pMesh ) const;
    ShapeHeightField * CreateShapeHeightField( MeshHeightField * pMesh ) const;

    //ShapeDeformableMesh * CreateShapeDeformableMesh( MeshDeformableGraph * pGraph, TriangleMesh * pMesh = NULL ) const;
    //ShapeDeformableMesh * CreateShapeDeformableMesh( TriangleMesh * pMesh ) const;

    Void DestroyShape( Shape * pShape ) const;

    // Bodies access
    inline UInt GetBodyCount() const;
    inline const BodyNode * GetBodyList() const;

    RigidBody * CreateRigidBody( Bool bStatic, Shape * pLocalShape, Scalar fDensity,
                                 const Vertex3 & vPosition = Vertex3::Null, const Quaternion & qOrientation = Quaternion::Identity,
                                 const Vector3 & vLinearSpeed = Vector3::Null, const Vector3 & vAngularSpeed = Vector3::Null );
    KinematicBody * CreateKinematicBody( Bool bStatic, Shape * pLocalShape, Scalar fDensity,
                                         const Vertex3 & vPosition = Vertex3::Null, const Quaternion & qOrientation = Quaternion::Identity,
                                         const Vector3 & vLinearSpeed = Vector3::Null, const Vector3 & vAngularSpeed = Vector3::Null );

    Void DestroyBody( BodyNode * pBody, Bool bDestroyShape = false );

    // Contacts access
    inline UInt GetContactCount() const;
    inline const ContactEdge * GetContactList() const;

    inline ContactEdge * GetContact( const BodyNode * pBodyA, const BodyNode * pBodyB ) const;

    inline ContactCallback * GetContactCallback() const;
    inline Void SetContactCallback( ContactCallback * pContactCallback );

    // Joints access
    inline UInt GetJointCount() const;
    inline const JointEdge * GetJointList() const;

    inline JointEdge * GetJoint( const BodyNode * pBodyA, const BodyNode * pBodyB ) const;

    JointEdge * CreateJoint( JointConstraintType iType, BodyNode * pBodyA, BodyNode * pBodyB, const Transform3 & vJointFrame );
    JointEdge * CreateJoint( JointConstraintType iType, BodyNode * pBodyA, BodyNode * pBodyB, const Transform3 & vFrameInA, const Transform3 & vFrameInB );
    Void DestroyJoint( JointEdge * pJoint );

    // Controllers access
    ManualController * CreateManualController( const GChar * strName, const Vertex3 & vPosition, const Quaternion & qOrientation,
                                                                      const Vector3 & vLinearSpeed, const Vector3 & vAngularSpeed );
    CharacterController * CreateCharacterController( const GChar * strName, const Vertex3 & vPosition, const Quaternion & qOrientation,
                                                                            const Vector3 & vLinearSpeed, const Vector3 & vAngularSpeed );
    KeyFrameController * CreateKeyFrameController( const GChar * strName, UInt iPositionKeyCount, UInt iOrientationKeyCount,
                                                                          UInt iSharedKeyCount = 0 );
    PathController * CreatePathController( const GChar * strName, Curve3 * pLinearPath, Curve2 * pLinearTravel,
                                                                  Curve3 * pAngularPath, Curve2 * pAngularTravel,
                                                                  EulerAngles iEulerAnglesMode = EULER_ANGLES_ZYX );
    BlendController * CreateBlendController( const GChar * strName, MotionController * pSource, MotionController * pDestination,
                                                                    Bool bGeometricBlend = true );

    ParticleController * CreateParticleController( const GChar * strName, const Vertex3 & vPosition, const Quaternion & qOrientation,
                                                                          const Vector3 & vLinearSpeed, const Vector3 & vAngularSpeed );
    IKJointController * CreateIKJointController( const GChar * strName, IKBone * pParent, IKBone * pChild,
                                                                        IKGoal ** arrGoals, UInt iGoalCount,
                                                                        EulerAngles iEulerAnglesMode = EULER_ANGLES_ZYX );

    Void DestroyController( KinematicController * pController );

    // Systems access
    //ParticleSystem * CreateParticleSystem( Scalar fParticleSize = PARTICLE_SIZE_DEFAULT );
    Skeleton * CreateSkeleton( KinematicBody * pRootBone, UInt iRootChildCount, UInt iGoalCount );
    RagDoll * CreateRagDoll( UInt iBoneCount, UInt iJointCount );

    Void DestroySystem( SystemBody * pSystem );

    // Simulation loop
    Void SimulateWorld( Scalar fTimeStep );

private:
    // Simulation helpers
    Void _CollisionDetection();

    Void _ApplyExternalForces( Island * pIsland );
    Void _ResetExternalForces( Island * pIsland );
    Void _FixStackLayer( Island * pIsland, UInt iStackLayer, Bool bFixed );

    Void _SingletonBodyStep( BodyNode * pBody, Scalar fTimeStep );
    Void _Dynamics( Island * pIsland, Scalar fTimeStep, UInt iStackLayer = INVALID_OFFSET );
    Void _ShockPropagation( Island * pIsland, Scalar fShockStep );

    // Delegates
    IslandSolverListener * m_pIslandSolverListener;

    BroadPhase * m_pBroadPhase;
    IslandSolver * m_pIslandSolver;
    ConstraintSolver * m_pConstraintSolver;

    Array<SystemBody*> m_arrSystems;

    // Shock propagation factor
    Scalar m_fShockPropagationFactor;

    // External forces
    Vector3 m_vGravity;
    //Vector3 m_vArchimedePush;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "PhysicsManager.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_PHYSICSMANAGER_H

