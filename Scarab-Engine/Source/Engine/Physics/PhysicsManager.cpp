/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/PhysicsManager.cpp
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
// Known Bugs : none
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "PhysicsManager.h"

/////////////////////////////////////////////////////////////////////////////////
// PhysicsManager implementation
PhysicsManager::PhysicsManager():
    Manager<PhysicsManager>(),
    m_arrSystems(),
    m_vGravity( 0.0f, 0.0f, -PHYSICS_FORCE_GRAVITY )
{
    m_pIslandSolverListener = NULL;

    m_pBroadPhase = NULL;
    m_pIslandSolver = NULL;
    m_pConstraintSolver = NULL;

    m_fShockPropagationFactor = PHYSICS_SHOCK_PROPAGATION;
}
PhysicsManager::~PhysicsManager()
{
    // nothing to do
}

Void PhysicsManager::Initialize()
{
    SelectMemory( TEXT("IslandSolver") );
    m_pIslandSolver = New IslandSolver();
    m_pIslandSolverListener = New IslandSolverListener( m_pIslandSolver );
    UnSelectMemory();

    SelectMemory( TEXT("Broadphase") );
    m_pBroadPhase = New SweepAndPrune( m_pIslandSolverListener );
    UnSelectMemory();

    SelectMemory( TEXT("ConstraintSolver") );
    m_pConstraintSolver = New ConstraintSolver();
    UnSelectMemory();

    m_arrSystems.UseMemoryContext( GetMemoryContext(), TEXT("Bodies") );
    m_arrSystems.Create();
}
Void PhysicsManager::Cleanup()
{
    for( UInt i = 0; i < m_arrSystems.Count(); ++i ) {
        SelectMemory( TEXT("Bodies") );
        Delete( m_arrSystems[i] );
        UnSelectMemory();
    }
    m_arrSystems.Destroy();

    SelectMemory( TEXT("ConstraintSolver") );
    Delete( m_pConstraintSolver );
    UnSelectMemory();

    SelectMemory( TEXT("Broadphase") );
    Delete( m_pBroadPhase );
    UnSelectMemory();

    SelectMemory( TEXT("IslandSolver") );
    Delete( m_pIslandSolverListener );
    Delete( m_pIslandSolver );
    UnSelectMemory();
}

ShapeSphere * PhysicsManager::CreateShapeSphere( const Sphere & vSphere, UInt iSliceSamples, UInt iRadialSamples ) const
{
    // Create ShapeSphere
    SelectMemory( TEXT("Shapes") );
    ShapeSphere * pShape = New ShapeSphere( vSphere, iSliceSamples, iRadialSamples );
    UnSelectMemory();

    return pShape;
}
ShapeSphere * PhysicsManager::CreateShapeSphere( MeshSphere * pMesh ) const
{
    // Create ShapeSphere
    SelectMemory( TEXT("Shapes") );
    ShapeSphere * pShape = New ShapeSphere( pMesh );
    UnSelectMemory();

    return pShape;
}
ShapeBox * PhysicsManager::CreateShapeBox( const Box & vBox ) const
{
    // Create ShapeBox
    SelectMemory( TEXT("Shapes") );
    ShapeBox * pShape = New ShapeBox( vBox );
    UnSelectMemory();

    return pShape;
}
ShapeBox * PhysicsManager::CreateShapeBox( MeshBox * pMesh ) const
{
    // Create ShapeBox
    SelectMemory( TEXT("Shapes") );
    ShapeBox * pShape = New ShapeBox( pMesh );
    UnSelectMemory();

    return pShape;
}

ShapeConvexMesh * PhysicsManager::CreateShapeConvexMesh( MeshVoronoiGraph * pGraph, TriangleMesh * pMesh ) const
{
    // Create ShapeConvexMesh
    SelectMemory( TEXT("Shapes") );
    ShapeConvexMesh * pShape = New ShapeConvexMesh( pGraph, pMesh );
    UnSelectMemory();

    return pShape;
}
ShapeConvexMesh * PhysicsManager::CreateShapeConvexMesh( TriangleMesh * pMesh ) const
{
    // Create ShapeConvexMesh
    SelectMemory( TEXT("Shapes") );
    ShapeConvexMesh * pShape = New ShapeConvexMesh( pMesh );
    UnSelectMemory();

    return pShape;
}

ShapeConcaveMesh * PhysicsManager::CreateShapeConcaveMesh( const Vertex3 * arrNodes, UInt iNodeCount,
                                                           const Triangle3 * arrTriangles, UInt iTriangleCount, TriangleMesh * pMesh ) const
{
    // Create ShapeConcaveMesh
    SelectMemory( TEXT("Shapes") );
    ShapeConcaveMesh * pShape = New ShapeConcaveMesh( arrNodes, iNodeCount, arrTriangles, iTriangleCount, pMesh );
    UnSelectMemory();

    return pShape;
}
ShapeConcaveMesh * PhysicsManager::CreateShapeConcaveMesh( TriangleMesh * pMesh ) const
{
    // Create ShapeConcaveMesh
    SelectMemory( TEXT("Shapes") );
    ShapeConcaveMesh * pShape = New ShapeConcaveMesh( pMesh );
    UnSelectMemory();

    return pShape;
}
ShapeHeightField * PhysicsManager::CreateShapeHeightField( MeshHeightField * pMesh ) const
{
    // Create ShapeHeightField
    SelectMemory( TEXT("Shapes") );
    ShapeHeightField * pShape = New ShapeHeightField( pMesh );
    UnSelectMemory();

    return pShape;
}

Void PhysicsManager::DestroyShape( Shape * pShape ) const
{
    // Destroy shape
    SelectMemory( TEXT("Shapes") );
    Delete( pShape );
    UnSelectMemory();
}

RigidBody * PhysicsManager::CreateRigidBody( Bool bStatic, Shape * pLocalShape, Scalar fDensity,
                                             const Vertex3 & vPosition, const Quaternion & qOrientation,
                                             const Vector3 & vLinearSpeed, const Vector3 & vAngularSpeed )
{
    // Create RigidBody
    SelectMemory( TEXT("Bodies") );
    RigidBody * pRigidBody = New RigidBody( bStatic, pLocalShape, fDensity,
                                            vPosition, qOrientation, vLinearSpeed, vAngularSpeed );
    UnSelectMemory();

    // Register in island solver
    m_pIslandSolver->AddBody( pRigidBody );

    // Register in broadphase
    m_pBroadPhase->CreateProxy( pRigidBody, pRigidBody->GetWorldShape()->GetAAB() );

    return pRigidBody;
}
KinematicBody * PhysicsManager::CreateKinematicBody( Bool bStatic, Shape * pLocalShape, Scalar fDensity,
                                                     const Vertex3 & vPosition, const Quaternion & qOrientation,
                                                     const Vector3 & vLinearSpeed, const Vector3 & vAngularSpeed )
{
    // Create KinematicBody
    SelectMemory( TEXT("Bodies") );
    KinematicBody * pKinematicBody = New KinematicBody( bStatic, pLocalShape, fDensity,
                                                        vPosition, qOrientation, vLinearSpeed, vAngularSpeed );
    UnSelectMemory();

    // Register in island solver
    m_pIslandSolver->AddBody( pKinematicBody );

    // Register in broadphase
    m_pBroadPhase->CreateProxy( pKinematicBody, pKinematicBody->GetWorldShape()->GetAAB() );

    return pKinematicBody;
}

Void PhysicsManager::DestroyBody( BodyNode * pBody, Bool bDestroyShape )
{
    // Unregister from island solver
    m_pIslandSolver->RemoveBody( pBody, bDestroyShape );

    // Don't destroy proxy/body yet, IslandSolver uses deferred removal !
}

JointEdge * PhysicsManager::CreateJoint( JointConstraintType iType, BodyNode * pBodyA, BodyNode * pBodyB, const Transform3 & vJointFrame )
{
    // Create Joint
    JointConstraint * pJoint = NULL;

    SelectMemory( TEXT("Joints") );
    switch( iType ) {
        case JOINT_GENERIC6DOF: pJoint = New JointGeneric6DOF( pBodyA, pBodyB, vJointFrame ); break;
        case JOINT_FIXED:       pJoint = New JointFixed( pBodyA, pBodyB, vJointFrame );       break;
        case JOINT_ANCHOR:      pJoint = New JointAnchor( pBodyA, pBodyB, vJointFrame );      break;
        case JOINT_SLIDER:      pJoint = New JointSlider( pBodyA, pBodyB, vJointFrame );      break;
        case JOINT_HINGE:       pJoint = New JointHinge( pBodyA, pBodyB, vJointFrame );       break;
        case JOINT_WHEEL:       pJoint = New JointWheel( pBodyA, pBodyB, vJointFrame );       break;
        case JOINT_UNIVERSAL:   pJoint = New JointUniversal( pBodyA, pBodyB, vJointFrame );   break;
        case JOINT_CONETWIST:   pJoint = New JointConeTwist( pBodyA, pBodyB, vJointFrame );   break;
        case JOINT_GEAR:        pJoint = New JointGear( pBodyA, pBodyB, vJointFrame );        break;
        default: Assert( false ); break;
    }
    UnSelectMemory();

    // Create joint
    SelectMemory( TEXT("IslandSolver") );
    JointEdge * pJointEdge = New JointEdge( pBodyA, pBodyB, pJoint );
    UnSelectMemory();

    // Register in island solver
    m_pIslandSolver->AddJoint( pJointEdge );

    return pJointEdge;
}
JointEdge * PhysicsManager::CreateJoint( JointConstraintType iType, BodyNode * pBodyA, BodyNode * pBodyB, const Transform3 & vFrameInA, const Transform3 & vFrameInB )
{
    // Create Joint
    JointConstraint * pJoint = NULL;

    SelectMemory( TEXT("Joints") );
    switch( iType ) {
        case JOINT_GENERIC6DOF: pJoint = New JointGeneric6DOF( pBodyA, pBodyB, vFrameInA, vFrameInB ); break;
        case JOINT_FIXED:       pJoint = New JointFixed( pBodyA, pBodyB, vFrameInA, vFrameInB );       break;
        case JOINT_ANCHOR:      pJoint = New JointAnchor( pBodyA, pBodyB, vFrameInA, vFrameInB );      break;
        case JOINT_SLIDER:      pJoint = New JointSlider( pBodyA, pBodyB, vFrameInA, vFrameInB );      break;
        case JOINT_HINGE:       pJoint = New JointHinge( pBodyA, pBodyB, vFrameInA, vFrameInB );       break;
        case JOINT_WHEEL:       pJoint = New JointWheel( pBodyA, pBodyB, vFrameInA, vFrameInB );       break;
        case JOINT_UNIVERSAL:   pJoint = New JointUniversal( pBodyA, pBodyB, vFrameInA, vFrameInB );   break;
        case JOINT_CONETWIST:   pJoint = New JointConeTwist( pBodyA, pBodyB, vFrameInA, vFrameInB );   break;
        case JOINT_GEAR:        pJoint = New JointGear( pBodyA, pBodyB, vFrameInA, vFrameInB );        break;
        default: Assert( false ); break;
    }
    UnSelectMemory();

    // Create joint
    SelectMemory( TEXT("IslandSolver") );
    JointEdge * pJointEdge = New JointEdge( pBodyA, pBodyB, pJoint );
    UnSelectMemory();

    // Register in island solver
    m_pIslandSolver->AddJoint( pJointEdge );

    return pJointEdge;
}
Void PhysicsManager::DestroyJoint( JointEdge * pJoint )
{
    // Unregister in island solver
    m_pIslandSolver->RemoveJoint( pJoint );

    // Destroy joint
    SelectMemory( TEXT("IslandSolver") );
    Delete( pJoint );
    UnSelectMemory();
}

ManualController * PhysicsManager::CreateManualController( const GChar * strName, const Vertex3 & vPosition, const Quaternion & qOrientation,
                                                                                  const Vector3 & vLinearSpeed, const Vector3 & vAngularSpeed )
{
    // Create controller
    SelectMemory( TEXT("Controllers") );
    ManualController * pController = New ManualController( strName, vPosition, qOrientation,
                                                                    vLinearSpeed, vAngularSpeed );
    UnSelectMemory();

    return pController;
}
CharacterController * PhysicsManager::CreateCharacterController( const GChar * strName, const Vertex3 & vPosition, const Quaternion & qOrientation,
                                                                                        const Vector3 & vLinearSpeed, const Vector3 & vAngularSpeed )
{
    // Create controller
    SelectMemory( TEXT("Controllers") );
    CharacterController * pController = New CharacterController( strName, vPosition, qOrientation,
                                                                          vLinearSpeed, vAngularSpeed );
    UnSelectMemory();

    return pController;
}
KeyFrameController * PhysicsManager::CreateKeyFrameController( const GChar * strName, UInt iPositionKeyCount, UInt iOrientationKeyCount,
                                                                                      UInt iSharedKeyCount )
{
    // Create controller
    SelectMemory( TEXT("Controllers") );
    KeyFrameController * pController = New KeyFrameController( strName, iPositionKeyCount, iOrientationKeyCount,
                                                                        iSharedKeyCount );
    UnSelectMemory();

    return pController;
}
PathController * PhysicsManager::CreatePathController( const GChar * strName, Curve3 * pLinearPath, Curve2 * pLinearTravel,
                                                                              Curve3 * pAngularPath, Curve2 * pAngularTravel,
                                                                              EulerAngles iEulerAnglesMode )
{
    // Create controller
    SelectMemory( TEXT("Controllers") );
    PathController * pController = New PathController( strName, pLinearPath, pLinearTravel,
                                                                pAngularPath, pAngularTravel,
                                                                iEulerAnglesMode );
    UnSelectMemory();

    return pController;
}
BlendController * PhysicsManager::CreateBlendController( const GChar * strName, MotionController * pSource, MotionController * pDestination,
                                                                                Bool bGeometricBlend )
{
    // Create controller
    SelectMemory( TEXT("Controllers") );
    BlendController * pController = New BlendController( strName, pSource, pDestination,
                                                                  bGeometricBlend );
    UnSelectMemory();

    return pController;
}

ParticleController * PhysicsManager::CreateParticleController( const GChar * strName, const Vertex3 & vPosition, const Quaternion & qOrientation,
                                                                                      const Vector3 & vLinearSpeed, const Vector3 & vAngularSpeed )
{
    // Create controller
    SelectMemory( TEXT("Controllers") );
    ParticleController * pController = New ParticleController( strName, vPosition, qOrientation,
                                                                        vLinearSpeed, vAngularSpeed );
    UnSelectMemory();

    return pController;
}
IKJointController * PhysicsManager::CreateIKJointController( const GChar * strName, IKBone * pParent, IKBone * pChild,
                                                                                    IKGoal ** arrGoals, UInt iGoalCount,
                                                                                    EulerAngles iEulerAnglesMode )
{
    // Create controller
    SelectMemory( TEXT("Controllers") );
    IKJointController * pController = New IKJointController( strName, pParent, pChild,
                                                                      arrGoals, iGoalCount,
                                                                      iEulerAnglesMode );
    UnSelectMemory();

    return pController;
}




Void PhysicsManager::DestroyController( KinematicController * pController )
{
    // Destroy controller
    SelectMemory( TEXT("Controllers") );
    Delete( pController );
    UnSelectMemory();
}

//ParticleSystem * PhysicsManager::CreateParticleSystem( Scalar fParticleSize )
//{
//    // Create system
//    SelectMemory( TEXT("Bodies") );
//    ParticleSystem * pSystem = New ParticleSystem( fParticleSize );
//    UnSelectMemory();
//
//    // Register in system array
//    m_arrSystems.Push( pSystem );
//
//    return pSystem;
//}
Skeleton * PhysicsManager::CreateSkeleton( KinematicBody * pRootBone, UInt iRootChildCount, UInt iGoalCount )
{
    // Create system
    SelectMemory( TEXT("Bodies") );
    Skeleton * pSystem = New Skeleton( pRootBone, iRootChildCount, iGoalCount );
    UnSelectMemory();

    // Register in system array
    m_arrSystems.Push( pSystem );

    return pSystem;
}
RagDoll * PhysicsManager::CreateRagDoll( UInt iBoneCount, UInt iJointCount )
{
    // Create system
    SelectMemory( TEXT("Bodies") );
    RagDoll * pSystem = New RagDoll( iBoneCount, iJointCount );
    UnSelectMemory();

    // Register in system array
    m_arrSystems.Push( pSystem );

    return pSystem;
}

Void PhysicsManager::DestroySystem( SystemBody * pSystem )
{
    // Unregister in system array
    UInt iFound = m_arrSystems.Search( pSystem );
    if ( iFound == INVALID_OFFSET )
        return;
    m_arrSystems.Remove( iFound, NULL, 1 );

    // Destroy system
    SelectMemory( TEXT("Bodies") );
    Delete( pSystem );
    UnSelectMemory();
}

Void PhysicsManager::SimulateWorld( Scalar fTimeStep )
{
    // Split time step
    Scalar fFirstStep = ( fTimeStep * (1.0f - m_fShockPropagationFactor) );
    Scalar fShockStep = ( fTimeStep * m_fShockPropagationFactor );

    // Collision detection
    _CollisionDetection();

    // Simulate all systems
    UInt iSystemCount = m_arrSystems.Count();
    for( UInt i = 0; i < iSystemCount; ++i )
        m_arrSystems[i]->Update( fTimeStep );

    // Simulate all singletons (including insulators)
    BodyNode * pBody = m_pIslandSolver->GetBodyList();
    while( pBody != NULL ) {
        // Non-singleton bodies (skip)
        if ( pBody->GetIsland() != NULL ) {
            pBody = (BodyNode*)( pBody->GetNextInWorld() );
            continue;
        }

        _SingletonBodyStep( pBody, fTimeStep );

        // Next body
        pBody = (BodyNode*)( pBody->GetNextInWorld() );
    }

    // Simulate all islands
    Island * pIsland = m_pIslandSolver->GetIslandList();
    while( pIsland != NULL ) {
        // Solve island
        _ApplyExternalForces( pIsland );

        if ( m_fShockPropagationFactor > 0.0f ) {
            m_pIslandSolver->ComputeStackLayers( pIsland );
            if ( pIsland->GetStackLayerCount() > 0 ) {
                _Dynamics( pIsland, fFirstStep );
                _ShockPropagation( pIsland, fShockStep );

                // Rippling effect prevention
                //m_pIslandSolver->ProcessNarrowPhase( pIsland );
                //_ErrorCorrection( pIsland );
            } else
                _Dynamics( pIsland, fTimeStep );
        } else
            _Dynamics( pIsland, fTimeStep );

        _ResetExternalForces( pIsland );

        // Next island
        pIsland = pIsland->GetNextInWorld();
    }

    // Sleeping policy
    m_pIslandSolver->ProcessSleepPhase();

    // Update shapes' meshes
    //pBody = m_pIslandSolver->GetBodyList();
    //while( pBody != NULL ) {
    //    // Non-deformable shape (skip)
    //    if ( !(pBody->GetLocalShape()->IsDeformable()) ) {
    //        pBody = (BodyNode*)( pBody->GetNextInWorld() );
    //        continue;
    //    }

    //    pBody->GetLocalShape()->UpdateMesh();

    //    // Next body
    //    pBody = (BodyNode*)( pBody->GetNextInWorld() );
    //}
}

/////////////////////////////////////////////////////////////////////////////////

Void PhysicsManager::_CollisionDetection()
{
    // Self-Collision detection
    ShapeDeformable * pShape;
    BodyNode * pBody = m_pIslandSolver->GetBodyList();
    while( pBody != NULL ) {
        // Non-deformable shape (skip)
        if ( !(pBody->GetLocalShape()->IsDeformable()) ) {
            pBody = (BodyNode*)( pBody->GetNextInWorld() );
            continue;
        }
        pShape = (ShapeDeformable*)( pBody->GetLocalShape() );

        // Broad phase
        pShape->UpdateCollisionData();
        pBody->GetWorldShape()->OnShapeUpdate();

        // Narrow + Contact phases
        pShape->SelfCollisionDetection();

        // Next body
        pBody = (BodyNode*)( pBody->GetNextInWorld() );
    }

    // Broad phase
    pBody = m_pIslandSolver->GetBodyList();
    while( pBody != NULL ) {
        // Static rigid bodies (skip)
        if ( pBody->IsRigid() && pBody->IsStatic() ) {
            pBody = (BodyNode*)( pBody->GetNextInWorld() );
            continue;
        }

        // Sleeping bodies (skip)
        if ( pBody->IsSleeping() ) {
            pBody = (BodyNode*)( pBody->GetNextInWorld() );
            continue;
        }

        m_pBroadPhase->UpdateProxy( pBody, pBody->GetWorldShape()->GetAAB() );

        // Next body
        pBody = (BodyNode*)( pBody->GetNextInWorld() );
    }
    m_pIslandSolver->DestroyProxies( m_pBroadPhase );
    m_pBroadPhase->Commit();
    m_pIslandSolver->ProcessBroadPhase();

    // Narrow + Contact phases
    Island * pIsland = m_pIslandSolver->GetIslandList();
    while( pIsland != NULL ) {
        // Update contact data
        m_pIslandSolver->ProcessNarrowPhase( pIsland );

        // Next island
        pIsland = pIsland->GetNextInWorld();
    }
}

Void PhysicsManager::_ApplyExternalForces( Island * pIsland )
{
    // Here we don't want to wake up anything
    BodyNode * pBody = pIsland->GetBodyList();
    Bool bSleeping;
    while( pBody != NULL ) {
        bSleeping = pBody->IsSleeping();

        pBody->ApplyForce( m_vGravity * pBody->GetMass() ); // Gravity
        //////////////////////////

        pBody->SetSleeping( bSleeping );

        // Next body
        pBody = (BodyNode*)( pBody->GetNextInIsland() );
    }
}
Void PhysicsManager::_ResetExternalForces( Island * pIsland )
{
    BodyNode * pBody = pIsland->GetBodyList();
    while( pBody != NULL ) {
        pBody->ResetForce();
        pBody->ResetTorque();

        // Next body
        pBody = (BodyNode*)( pBody->GetNextInIsland() );
    }
}
Void PhysicsManager::_FixStackLayer( Island * pIsland, UInt iStackLayer, Bool bFixed )
{
    BodyNode * pBody = pIsland->GetBodyList();
    while( pBody != NULL ) {
        // Select bottom nodes in this layer
        if ( pBody->GetStackLayer() != iStackLayer ) {
            pBody = (BodyNode*)( pBody->GetNextInIsland() );
            continue;
        }

        pBody->SetFixed( bFixed );

        // Next body
        pBody = (BodyNode*)( pBody->GetNextInIsland() );
    }
}

Void PhysicsManager::_SingletonBodyStep( BodyNode * pBody, Scalar fTimeStep )
{
    // Sleeping bodies (skip)
    if ( pBody->IsSleeping() )
        return;

    // Apply external forces
    pBody->ApplyForce( m_vGravity * pBody->GetMass() ); // Gravity
    //////////////////////////

    // Integration
    pBody->IntegrateVelocity( fTimeStep );
    pBody->IntegratePosition( fTimeStep );

    // Reset external forces
    pBody->ResetForce();
    pBody->ResetTorque();
}
Void PhysicsManager::_Dynamics( Island * pIsland, Scalar fTimeStep, UInt iStackLayer )
{
    // Integrate velocities
    BodyNode * pBody = pIsland->GetBodyList();
    while( pBody != NULL ) {
        // Select top-nodes in stack layer
        if ( iStackLayer != INVALID_OFFSET ) {
            if ( pBody->GetStackLayer() != (iStackLayer + 1) ) {
                pBody = (BodyNode*)( pBody->GetNextInIsland() );
                continue;
            }
        }

        pBody->IntegrateVelocity( fTimeStep );

        // Next body
        pBody = (BodyNode*)( pBody->GetNextInIsland() );
    }

    // Solve velocity constraints
    m_pConstraintSolver->Prepare( pIsland, fTimeStep, iStackLayer );
    m_pConstraintSolver->SolveVelocities();

    // Integrate positions
    pBody = pIsland->GetBodyList();
    while( pBody != NULL ) {
        // Select top-nodes in stack layer
        if ( iStackLayer != INVALID_OFFSET ) {
            if ( pBody->GetStackLayer() != (iStackLayer + 1) ) {
                pBody = (BodyNode*)( pBody->GetNextInIsland() );
                continue;
            }
        }

        pBody->IntegratePosition( fTimeStep );

        // Next body
        pBody = (BodyNode*)( pBody->GetNextInIsland() );
    }

    // Solve position constraints
    m_pConstraintSolver->SolvePositions();
}
Void PhysicsManager::_ShockPropagation( Island * pIsland, Scalar fShockStep )
{
    // Collision detection at t0 + firststep
    m_pIslandSolver->ProcessNarrowPhase( pIsland );

    // Enumerate island layers
    UInt iStackLayerCount = pIsland->GetStackLayerCount();

    for( UInt iStackLayer = 0; iStackLayer < iStackLayerCount; ++iStackLayer ) {
        // Fix bottom nodes
        _FixStackLayer( pIsland, iStackLayer, true );

        // Solve this layer
        _Dynamics( pIsland, fShockStep, iStackLayer );

        // Unfix bottom nodes
        _FixStackLayer( pIsland, iStackLayer, false );
    }
}

