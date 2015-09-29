/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/ConstraintSolver/ConstraintSolver.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Iterative LCP solver using PGS/SOR-PGS with sequential impulses
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
#include "ConstraintSolver.h"

#include "../PhysicsManager.h"

/////////////////////////////////////////////////////////////////////////////////
// ConstraintSolver implementation
ConstraintSolver::ConstraintSolver():
    m_hSoftConstraintSolver(),
    m_arrContactNormalConstraints(), m_arrContactFrictionConstraints(), m_arrContactRollingFrictionConstraints(), m_arrJointConstraints(),
    m_arrContactNormalShuffle(), m_arrContactFrictionShuffle(), m_arrJointShuffle()
{
    m_arrContactNormalConstraints.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("ConstraintSolver") );
    m_arrContactNormalConstraints.Create();
    m_arrContactNormalConstraints.SetShrinking( false );

    m_arrContactFrictionConstraints.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("ConstraintSolver") );
    m_arrContactFrictionConstraints.Create();
    m_arrContactFrictionConstraints.SetShrinking( false );

    m_arrContactRollingFrictionConstraints.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("ConstraintSolver") );
    m_arrContactRollingFrictionConstraints.Create();
    m_arrContactRollingFrictionConstraints.SetShrinking( false );

    m_arrJointConstraints.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("ConstraintSolver") );
    m_arrJointConstraints.Create();
    m_arrJointConstraints.SetShrinking( false );

    m_arrContactNormalShuffle.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("ConstraintSolver") );
    m_arrContactNormalShuffle.Create();
    m_arrContactNormalShuffle.SetShrinking( false );

    m_arrContactFrictionShuffle.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("ConstraintSolver") );
    m_arrContactFrictionShuffle.Create();
    m_arrContactFrictionShuffle.SetShrinking( false );

    m_arrJointShuffle.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("ConstraintSolver") );
    m_arrJointShuffle.Create();
    m_arrJointShuffle.SetShrinking( false );

    SetDefaultParameters();

    // Solver state
    m_pIsland = NULL;
    m_fTimeStep = 0.0f;
    m_fInvTimeStep = 0.0f;
}
ConstraintSolver::~ConstraintSolver()
{
    m_arrContactNormalConstraints.Destroy();
    m_arrContactFrictionConstraints.Destroy();
    m_arrContactRollingFrictionConstraints.Destroy();
    m_arrJointConstraints.Destroy();

    m_arrContactNormalShuffle.Destroy();
    m_arrContactFrictionShuffle.Destroy();
    m_arrJointShuffle.Destroy();
}

Void ConstraintSolver::SetDefaultParameters()
{
    m_iIterations = CONSTRAINTSOLVER_ITERATIONS;

    m_bUseSOR = false;
    m_iSORPeriod = CONSTRAINTSOLVER_SOR_PERIOD;

    m_bUseCorrection = true;
    m_iCorrectionIterations = CONSTRAINTSOLVER_CORRECTION_ITERATIONS;
    m_fCorrectionThreshold = CONSTRAINTSOLVER_CORRECTION_THRESHOLD;
    m_fCorrectionERP = CONSTRAINTSOLVER_CORRECTION_ERP;

    m_bUseWarmStart = true;
    m_fWarmStartFactor = CONSTRAINTSOLVER_WARMSTART_FACTOR;

    m_fContactProcessThreshold = CONSTRAINTSOLVER_CONTACT_PROCESS_THRESHOLD;
    m_iContactRestingThreshold = CONSTRAINTSOLVER_CONTACT_RESTING_THRESHOLD;

    m_fContactNormalSlop = CONSTRAINTSOLVER_CONTACT_NORMAL_SLOP;
    m_fContactNormalERP = CONSTRAINTSOLVER_CONTACT_NORMAL_ERP;
    m_fContactNormalCFM = CONSTRAINTSOLVER_CONTACT_NORMAL_CFM;
    m_fContactFrictionCFM = CONSTRAINTSOLVER_CONTACT_FRICTION_CFM;
    m_fContactRollingFrictionCFM = CONSTRAINTSOLVER_CONTACT_ROLLINGFRICTION_CFM;
    m_fContactFrictionMAX = CONSTRAINTSOLVER_CONTACT_FRICTION_MAX;

    m_bUseVelocityDependantFrictionFrame = true;
    m_fSingleAxisRollingFrictionThreshold = CONSTRAINTSOLVER_CONTACT_ROLLINGFRICTION_SINGLEAXIS_THRESHOLD;

    m_fJointDefaultERP = CONSTRAINTSOLVER_JOINT_DEFAULT_ERP;
    m_fJointDefaultCFM = CONSTRAINTSOLVER_JOINT_DEFAULT_CFM;
}

Void ConstraintSolver::Prepare( Island * pIsland, Scalar fTimeStep, UInt iStackLayer )
{
    Assert( m_pIsland == NULL );

    BodyNode * pBodyA;
    BodyNode * pBodyB;

    // Solver state
    m_pIsland = pIsland;
    m_fTimeStep = fTimeStep;
    m_fInvTimeStep = MathFn->Invert( fTimeStep );

    // Evaluate all soft bodies
    BodyNode * pBody = pIsland->GetBodyList();
    while( pBody != NULL ) {
        // Select top-nodes in stack layer
        if ( iStackLayer != INVALID_OFFSET ) {
            if ( pBody->GetStackLayer() != (iStackLayer + 1) ) {
                pBody = (BodyNode*)( pBody->GetNextInIsland() );
                continue;
            }
        }

        // Filter body type (soft only)
        if ( !(pBody->IsSoft()) ) {
            pBody = (BodyNode*)( pBody->GetNextInIsland() );
            continue;
        }

        // Check sleepers
        if ( pBody->IsSleeping() ) {
            pBody = (BodyNode*)( pBody->GetNextInIsland() );
            continue;
        }

        m_hSoftConstraintSolver.PushSoftLinks( (SoftBody*)pBody );
        if ( !(pBody->IsSensor()) )
            m_hSoftConstraintSolver.PushSelfContacts( (SoftBody*)pBody );

        // Next body
        pBody = (BodyNode*)( pBody->GetNextInIsland() );
    }

    // Evaluate all contacts
    ContactEdge * pContactEdge = pIsland->GetContactList();
    while( pContactEdge != NULL ) {
        pBodyA = pContactEdge->GetBodyA();
        pBodyB = pContactEdge->GetBodyB();

        // Skip contacts not in that layer
        if ( iStackLayer != INVALID_OFFSET ) {
            if ( pContactEdge->GetStackLayer() != iStackLayer ) {
                pContactEdge = (ContactEdge*)( pContactEdge->GetNextInIsland() );
                continue;
            }
        }

        // Skip sensors
        if ( pBodyA->IsSensor() || pBodyB->IsSensor() ) {
            pContactEdge = (ContactEdge*)( pContactEdge->GetNextInIsland() );
            continue;
        }

        // Check sleepers
        if ( BodyNode::CheckSleepers(pBodyA, pBodyB) ) {
            pContactEdge = (ContactEdge*)( pContactEdge->GetNextInIsland() );
            continue;
        }

        // Skip Insulators & Fixed bodies
        if ( (pBodyA->IsInsulator() || pBodyA->IsFixed()) && (pBodyB->IsInsulator() || pBodyB->IsFixed()) ) {
            pContactEdge = (ContactEdge*)( pContactEdge->GetNextInIsland() );
            continue;
        }

        // Filter body types
        if ( pBodyA->IsSoft() ) {
            if ( pBodyB->IsSoft() ) {
                // Soft/Soft contact
                //m_hSoftConstraintSolver.PushSoftSoftContacts( pContactEdge->GetManifold() );
            } else {
                // Soft/Rigid contact
                //m_hSoftConstraintSolver.PushRigidSoftContacts( pBodyB, pContactEdge->GetManifold(),
                //                                               m_fTimeStep, m_fInvTimeStep );
            }
        } else {
            if ( pBodyB->IsSoft() ) {
                // Rigid/Soft contact
                //m_hSoftConstraintSolver.PushRigidSoftContacts( pBodyA, pContactEdge->GetManifold(),
                //                                               m_fTimeStep, m_fInvTimeStep );
            } else {
                // Rigid/Rigid contact
                _EvaluateContact( pBodyA, pBodyB, pContactEdge->GetManifold() );
            }
        }

        // Next contact
        pContactEdge = (ContactEdge*)( pContactEdge->GetNextInIsland() );
    }

    // Evaluate all joints
    JointEdge * pJointEdge = pIsland->GetJointList();
    while( pJointEdge != NULL ) {
        pBodyA = pJointEdge->GetBodyA();
        pBodyB = pJointEdge->GetBodyB();

        // Skip joints not in that layer
        if ( iStackLayer != INVALID_OFFSET ) {
            if ( pJointEdge->GetStackLayer() != iStackLayer ) {
                pJointEdge = (JointEdge*)( pJointEdge->GetNextInIsland() );
                continue;
            }
        }

        // Check sleepers
        if ( BodyNode::CheckSleepers(pBodyA, pBodyB) ) {
            pJointEdge = (JointEdge*)( pJointEdge->GetNextInIsland() );
            continue;
        }

        // Skip Insulators & Fixed bodies
        if ( (pBodyA->IsInsulator() || pBodyA->IsFixed()) && (pBodyB->IsInsulator() || pBodyB->IsFixed()) ) {
            pContactEdge = (ContactEdge*)( pContactEdge->GetNextInIsland() );
            continue;
        }

        // Filter body types
        if ( pBodyA->IsSoft() ) {
            if ( pBodyB->IsSoft() ) {
                // Soft/Soft joint
                //m_hSoftConstraintSolver.PushSoftSoftJoint(  );
            } else {
                // Soft/Rigid joint
                //m_hSoftConstraintSolver.PushRigidSoftAnchor( (const SoftAnchorEdge *)pJointEdge,
                //                                             m_fTimeStep, m_fInvTimeStep );
                //m_hSoftConstraintSolver.PushRigidSoftJoint(  );
            }
        } else {
            if ( pBodyB->IsSoft() ) {
                // Rigid/Soft joint
                //m_hSoftConstraintSolver.PushRigidSoftAnchor( (const SoftAnchorEdge *)pJointEdge,
                //                                             m_fTimeStep, m_fInvTimeStep );
                //m_hSoftConstraintSolver.PushRigidSoftJoint(  );
            } else {
                // Rigid/Rigid joint
                _EvaluateJoint( pBodyA, pBodyB, pJointEdge->GetJoint() );
            }
        }

        // Next joint
        pJointEdge = (JointEdge*)( pJointEdge->GetNextInIsland() );
    }

    // Check for emptyness
    UInt iConstraintCount = m_arrContactNormalConstraints.Count();
    iConstraintCount += m_arrContactFrictionConstraints.Count();
    iConstraintCount += m_arrContactRollingFrictionConstraints.Count();
    iConstraintCount += m_arrJointConstraints.Count();
    iConstraintCount += m_hSoftConstraintSolver.GetConstraintCount();

    if ( iConstraintCount == 0 ) {
        // Solver state
        m_pIsland = NULL;
        m_fTimeStep = 0.0f;
        m_fInvTimeStep = 0.0f;

        return;
    }
}

Void ConstraintSolver::SolveVelocities()
{
    // Solver state
    if ( m_pIsland == NULL )
        return;

    // Solve soft constraints
    m_hSoftConstraintSolver.SolveVelocities( m_fTimeStep );

    // Solve rigid constraints
    if ( m_bUseSOR )
        _SolveConstraints_SORPGS();
    else
        _SolveConstraints_PGS();
    _SolveConstraints_Finalize();

    // Clear cached data
    m_arrContactNormalConstraints.Clear();
    m_arrContactFrictionConstraints.Clear();
    m_arrContactRollingFrictionConstraints.Clear();
    m_arrJointConstraints.Clear();
    if ( m_bUseSOR ) {
        m_arrContactNormalShuffle.Clear();
        m_arrContactFrictionShuffle.Clear();
        m_arrJointShuffle.Clear();
    }
}
Void ConstraintSolver::SolvePositions()
{
    // Solver state
    if ( m_pIsland == NULL )
        return;

    // Solve soft constraints
    m_hSoftConstraintSolver.SolvePositions( m_fTimeStep, m_fInvTimeStep );

    // Clear cached data
    m_hSoftConstraintSolver.ClearConstraints();

    // Solver state
    m_pIsland = NULL;
    m_fTimeStep = 0.0f;
    m_fInvTimeStep = 0.0f;
}

/////////////////////////////////////////////////////////////////////////////////

Void ConstraintSolver::_ShuffleConstraints()
{
    UInt iNormalConstraintCount = m_arrContactNormalShuffle.Count();
    UInt iFrictionConstraintCount = m_arrContactFrictionShuffle.Count();
    UInt iJointConstraintCount = m_arrJointShuffle.Count();
    UInt i, iRand, iSwap;

    for( i = 1; i < iNormalConstraintCount; ++i ) {
        iRand = ( RandFn->XorShift128() % i );
        iSwap = m_arrContactNormalShuffle[i];
        m_arrContactNormalShuffle[i] = m_arrContactNormalShuffle[iRand];
        m_arrContactNormalShuffle[iRand] = iSwap;
    }
    for( i = 1; i < iFrictionConstraintCount; ++i ) {
        iRand = ( RandFn->XorShift128() % i );
        iSwap = m_arrContactFrictionShuffle[i];
        m_arrContactFrictionShuffle[i] = m_arrContactFrictionShuffle[iRand];
        m_arrContactFrictionShuffle[iRand] = iSwap;
    }
    for( i = 1; i < iJointConstraintCount; ++i ) {
        iRand = ( RandFn->XorShift128() % i );
        iSwap = m_arrJointShuffle[i];
        m_arrJointShuffle[i] = m_arrJointShuffle[iRand];
        m_arrJointShuffle[iRand] = iSwap;
    }
}

Void ConstraintSolver::_EvaluateContact( BodyNode * pBodyA, BodyNode * pBodyB, ContactManifold * pContactManifold )
{
    // Enumerate all contact points
    UInt iContactPointCount = pContactManifold->GetContactPointCount();
    ContactPoint * pCP;

    ConstraintData normalConstraint;
    ConstraintData friction0Constraint;
    ConstraintData friction1Constraint;
    ConstraintData rollingFrictionConstraint0;
    ConstraintData rollingFrictionConstraint1;
    ConstraintData rollingFrictionConstraint2;

    Bool bRollingFriction = true;
    Bool bSingleAxisRollingFriction = false;

    for( UInt i = 0; i < iContactPointCount; ++i ) {
        pCP = pContactManifold->GetContactPoint( i );

        // Check processing threshold
        if ( pCP->fDistance > m_fContactProcessThreshold )
            continue;

        // Get penetration distance
        Scalar fPenetration = pCP->fDistance + m_fContactNormalSlop;

        // Pick correct ERP value
        Scalar fNormalERP = m_fContactNormalERP;
        if ( m_bUseCorrection && fPenetration < m_fCorrectionThreshold ) {
            fNormalERP = m_fCorrectionERP;
        }

        // Get anchor points
        Vector3 vAnchorA = ( pCP->vWorldPointA - pBodyA->GetPosition() );
        Vector3 vAnchorB = ( pCP->vWorldPointB - pBodyB->GetPosition() );

        // Relative velocity at those points
        Vector3 vRelativeVelocity = ( pBodyA->GetLinearSpeedAt(vAnchorA) - pBodyB->GetLinearSpeedAt(vAnchorB) );
        Scalar fNormalVelocity = ( vRelativeVelocity * pCP->vWorldNormalBA );

        // Compute restitution value
        Scalar fRestitution = 0.0f;
        if ( pCP->iLifeTime < m_iContactRestingThreshold ) {
            fRestitution = (-fNormalVelocity) * _ComputeMixedRestitution( pBodyA->GetRestitution(), pBodyB->GetRestitution() );
            if ( fRestitution < 0.0f )
                fRestitution = 0.0f;
        }

        // Compute friction frame & value
        if ( !(pCP->bFrictionInitialized) ) {
            pCP->vFrictionAxis0 = ( vRelativeVelocity - (pCP->vWorldNormalBA * fNormalVelocity) );
            Scalar fSqrLateralVelocity = pCP->vFrictionAxis0.NormSqr();
            if( m_bUseVelocityDependantFrictionFrame && fSqrLateralVelocity > SCALAR_EPSILON ) {
                pCP->vFrictionAxis0 *= MathFn->InvSqrt( fSqrLateralVelocity );
                pCP->vFrictionAxis1 = ( pCP->vWorldNormalBA ^ pCP->vFrictionAxis0 );
                pCP->vFrictionAxis1.Normalize();
            } else {
                Vector3::MakeComplementBasis( pCP->vFrictionAxis0, pCP->vFrictionAxis1, pCP->vWorldNormalBA );
                if ( !m_bUseVelocityDependantFrictionFrame )
                    pCP->bFrictionInitialized = true;
            }
        }
        Scalar fFriction = _ComputeMixedFriction( pBodyA->GetFriction(), pBodyB->GetFriction() );

        // Compute rolling friction frame & value (only for first point, the deepest one, as pivot)
        Vector3 vRollingFrictionAxis0 = Vector3::Null;
        Vector3 vRollingFrictionAxis1 = Vector3::Null;
        Vector3 vRollingFrictionAxis2 = Vector3::Null;
        Scalar fRollingFriction = 0.0f;
        if ( bRollingFriction ) {
            Vector3 vRelativeAngularVelocity = ( pBodyA->GetAngularSpeed() - pBodyB->GetAngularSpeed() );
            Scalar fNorm = vRelativeAngularVelocity.Norm();

            bSingleAxisRollingFriction = ( fNorm > m_fSingleAxisRollingFrictionThreshold );
            if ( bSingleAxisRollingFriction )
                vRollingFrictionAxis0 = ( vRelativeAngularVelocity * MathFn->Invert(fNorm) ); // Normalize
            else {
                vRollingFrictionAxis2 = -(pCP->vWorldNormalBA);
                Vector3::MakeComplementBasis( vRollingFrictionAxis0, vRollingFrictionAxis1, vRollingFrictionAxis2 );
            }

            fRollingFriction = _ComputeMixedFriction( pBodyA->GetRollingFriction(), pBodyB->GetRollingFriction() );
        }

        // Save pointer to contact point
        UInt iNormalConstraintIndex = m_arrContactNormalConstraints.Count();
        UInt iFrictionConstraintIndex = m_arrContactFrictionConstraints.Count();

        normalConstraint.pObject = (Void*)pCP;
        normalConstraint.iConstraintLink = iFrictionConstraintIndex;
        friction0Constraint.pObject = (Void*)pCP;
        friction0Constraint.iConstraintLink = iNormalConstraintIndex;
        friction1Constraint.pObject = (Void*)pCP;
        friction1Constraint.iConstraintLink = iNormalConstraintIndex;
        if ( bRollingFriction ) {
            rollingFrictionConstraint0.pObject = (Void*)pCP;
            rollingFrictionConstraint0.iConstraintLink = iNormalConstraintIndex;
            if ( !bSingleAxisRollingFriction ) {
                rollingFrictionConstraint1.pObject = (Void*)pCP;
                rollingFrictionConstraint1.iConstraintLink = iNormalConstraintIndex;
                rollingFrictionConstraint2.pObject = (Void*)pCP;
                rollingFrictionConstraint2.iConstraintLink = iNormalConstraintIndex;
            }
        }

        // Save pointers to bodies
        normalConstraint.pBodyA = pBodyA;
        normalConstraint.pBodyB = pBodyB;
        friction0Constraint.pBodyA = pBodyA;
        friction0Constraint.pBodyB = pBodyB;
        friction1Constraint.pBodyA = pBodyA;
        friction1Constraint.pBodyB = pBodyB;
        if ( bRollingFriction ) {
            rollingFrictionConstraint0.pBodyA = pBodyA;
            rollingFrictionConstraint0.pBodyB = pBodyB;
            if ( !bSingleAxisRollingFriction ) {
                rollingFrictionConstraint1.pBodyA = pBodyA;
                rollingFrictionConstraint1.pBodyB = pBodyB;
                rollingFrictionConstraint2.pBodyA = pBodyA;
                rollingFrictionConstraint2.pBodyB = pBodyB;
            }
        }

        // Save friction factors
        normalConstraint.fFriction = fFriction;
        friction0Constraint.fFriction = fFriction;
        friction1Constraint.fFriction = fFriction;
        if ( bRollingFriction ) {
            rollingFrictionConstraint0.fFriction = fRollingFriction;
            if ( !bSingleAxisRollingFriction ) {
                rollingFrictionConstraint1.fFriction = fRollingFriction;
                rollingFrictionConstraint2.fFriction = fRollingFriction;
            }
        }

        // Compute Jacobians
        normalConstraint.jacConstraint.MakeLinear(
            pCP->vWorldNormalBA, vAnchorA, vAnchorB,
            pBodyA->GetInvMass(), pBodyA->GetInvInertiaTensor(),
            pBodyB->GetInvMass(), pBodyB->GetInvInertiaTensor()
        );
        friction0Constraint.jacConstraint.MakeLinear(
            pCP->vFrictionAxis0, vAnchorA, vAnchorB,
            pBodyA->GetInvMass(), pBodyA->GetInvInertiaTensor(),
            pBodyB->GetInvMass(), pBodyB->GetInvInertiaTensor()
        );
        friction1Constraint.jacConstraint.MakeLinear(
            pCP->vFrictionAxis1, vAnchorA, vAnchorB,
            pBodyA->GetInvMass(), pBodyA->GetInvInertiaTensor(),
            pBodyB->GetInvMass(), pBodyB->GetInvInertiaTensor()
        );
        if ( bRollingFriction ) {
            rollingFrictionConstraint0.jacConstraint.MakeAngular(
                vRollingFrictionAxis0, pBodyA->GetInvInertiaTensor(), pBodyB->GetInvInertiaTensor()
            );
            if ( !bSingleAxisRollingFriction ) {
                rollingFrictionConstraint1.jacConstraint.MakeAngular(
                    vRollingFrictionAxis1, pBodyA->GetInvInertiaTensor(), pBodyB->GetInvInertiaTensor()
                );
                rollingFrictionConstraint2.jacConstraint.MakeAngular(
                    vRollingFrictionAxis2, pBodyA->GetInvInertiaTensor(), pBodyB->GetInvInertiaTensor()
                );
            }
        }

        // Compute normal bias
        Scalar fRelativeVelocity = normalConstraint.jacConstraint.GetRelativeVelocity (
            pBodyA->GetLinearSpeed(), pBodyA->GetAngularSpeed(),
            pBodyB->GetLinearSpeed(), pBodyB->GetAngularSpeed()
        );
        Scalar fVelocityError = ( fRestitution - fRelativeVelocity );
        Scalar fPositionError = 0.0f;
        if ( fPenetration > 0.0f )
            fVelocityError -= ( fPenetration * m_fInvTimeStep );
        else
            fPositionError = -(fPenetration * fNormalERP) * m_fInvTimeStep;

        if ( m_bUseCorrection && fPenetration < m_fCorrectionThreshold ) {
            normalConstraint.fBias = ( fVelocityError * normalConstraint.jacConstraint.fInvDiagJWJtAB );
            normalConstraint.fBiasCorrection = ( fPositionError * normalConstraint.jacConstraint.fInvDiagJWJtAB );
        } else {
            normalConstraint.fBias = ( (fVelocityError + fPositionError) * normalConstraint.jacConstraint.fInvDiagJWJtAB );
            normalConstraint.fBiasCorrection = 0.0f;
        }

        // Compute friction bias
        Scalar fDesiredVelocity = 0.0f;

        fRelativeVelocity = friction0Constraint.jacConstraint.GetRelativeVelocity (
            pBodyA->GetLinearSpeed(), pBodyA->GetAngularSpeed(),
            pBodyB->GetLinearSpeed(), pBodyB->GetAngularSpeed()
        );
        fVelocityError = ( fDesiredVelocity - fRelativeVelocity );
        friction0Constraint.fBias = ( fVelocityError * friction0Constraint.jacConstraint.fInvDiagJWJtAB );
        friction0Constraint.fBiasCorrection = 0.0f;

        fRelativeVelocity = friction1Constraint.jacConstraint.GetRelativeVelocity (
            pBodyA->GetLinearSpeed(), pBodyA->GetAngularSpeed(),
            pBodyB->GetLinearSpeed(), pBodyB->GetAngularSpeed()
        );
        fVelocityError = ( fDesiredVelocity - fRelativeVelocity );
        friction1Constraint.fBias = ( fVelocityError * friction1Constraint.jacConstraint.fInvDiagJWJtAB );
        friction1Constraint.fBiasCorrection = 0.0f;

        // Compute rolling friction bias
        fDesiredVelocity = 0.0f;

        if ( bRollingFriction ) {
            fRelativeVelocity = rollingFrictionConstraint0.jacConstraint.GetRelativeVelocity (
                pBodyA->GetLinearSpeed(), pBodyA->GetAngularSpeed(),
                pBodyB->GetLinearSpeed(), pBodyB->GetAngularSpeed()
            );
            fVelocityError = ( fDesiredVelocity - fRelativeVelocity );
            rollingFrictionConstraint0.fBias = ( fVelocityError * rollingFrictionConstraint0.jacConstraint.fInvDiagJWJtAB );
            rollingFrictionConstraint0.fBiasCorrection = 0.0f;
            if ( !bSingleAxisRollingFriction ) {
                fRelativeVelocity = rollingFrictionConstraint1.jacConstraint.GetRelativeVelocity (
                    pBodyA->GetLinearSpeed(), pBodyA->GetAngularSpeed(),
                    pBodyB->GetLinearSpeed(), pBodyB->GetAngularSpeed()
                );
                fVelocityError = ( fDesiredVelocity - fRelativeVelocity );
                rollingFrictionConstraint1.fBias = ( fVelocityError * rollingFrictionConstraint1.jacConstraint.fInvDiagJWJtAB );
                rollingFrictionConstraint1.fBiasCorrection = 0.0f;
                fRelativeVelocity = rollingFrictionConstraint2.jacConstraint.GetRelativeVelocity (
                    pBodyA->GetLinearSpeed(), pBodyA->GetAngularSpeed(),
                    pBodyB->GetLinearSpeed(), pBodyB->GetAngularSpeed()
                );
                fVelocityError = ( fDesiredVelocity - fRelativeVelocity );
                rollingFrictionConstraint2.fBias = ( fVelocityError * rollingFrictionConstraint2.jacConstraint.fInvDiagJWJtAB );
                rollingFrictionConstraint2.fBiasCorrection = 0.0f;
            }
        }

        // Fill CFMs & Limits
        normalConstraint.fCFM = m_fContactNormalCFM;
        friction0Constraint.fCFM = m_fContactFrictionCFM;
        friction1Constraint.fCFM = m_fContactFrictionCFM;
        if ( bRollingFriction ) {
            rollingFrictionConstraint0.fCFM = m_fContactRollingFrictionCFM;
            if ( !bSingleAxisRollingFriction ) {
                rollingFrictionConstraint1.fCFM = m_fContactRollingFrictionCFM;
                rollingFrictionConstraint2.fCFM = m_fContactRollingFrictionCFM;
            }
        }

        normalConstraint.fLowerLimit = 0.0f;
        normalConstraint.fUpperLimit = SCALAR_INFINITE;
        friction0Constraint.fLowerLimit = 0.0f;
        friction0Constraint.fUpperLimit = SCALAR_INFINITE;
        friction1Constraint.fLowerLimit = 0.0f;
        friction1Constraint.fUpperLimit = SCALAR_INFINITE;
        if ( bRollingFriction ) {
            rollingFrictionConstraint0.fLowerLimit = 0.0f;
            rollingFrictionConstraint0.fUpperLimit = SCALAR_INFINITE;
            if ( !bSingleAxisRollingFriction ) {
                rollingFrictionConstraint1.fLowerLimit = 0.0f;
                rollingFrictionConstraint1.fUpperLimit = SCALAR_INFINITE;
                rollingFrictionConstraint2.fLowerLimit = 0.0f;
                rollingFrictionConstraint2.fUpperLimit = SCALAR_INFINITE;
           }
        }

        // Warm-Starting
        if ( m_bUseWarmStart ) {
            normalConstraint.fImpulse = pCP->fNormalImpulse * m_fWarmStartFactor;
            friction0Constraint.fImpulse = pCP->fFrictionImpulse0 * m_fWarmStartFactor;
            friction1Constraint.fImpulse = pCP->fFrictionImpulse1 * m_fWarmStartFactor;

            pBodyA->AccumulateImpulse( +( normalConstraint.jacConstraint.vJLinearAxis * pBodyA->GetInvMass() ), normalConstraint.jacConstraint.vInvIAxJtAngA, normalConstraint.fImpulse );
            pBodyB->AccumulateImpulse( -( normalConstraint.jacConstraint.vJLinearAxis * pBodyB->GetInvMass() ), normalConstraint.jacConstraint.vInvIBxJtAngB, normalConstraint.fImpulse );

            pBodyA->AccumulateImpulse( +( friction0Constraint.jacConstraint.vJLinearAxis * pBodyA->GetInvMass() ), friction0Constraint.jacConstraint.vInvIAxJtAngA, friction0Constraint.fImpulse );
            pBodyB->AccumulateImpulse( -( friction0Constraint.jacConstraint.vJLinearAxis * pBodyB->GetInvMass() ), friction0Constraint.jacConstraint.vInvIBxJtAngB, friction0Constraint.fImpulse );

            pBodyA->AccumulateImpulse( +( friction1Constraint.jacConstraint.vJLinearAxis * pBodyA->GetInvMass() ), friction1Constraint.jacConstraint.vInvIAxJtAngA, friction1Constraint.fImpulse );
            pBodyB->AccumulateImpulse( -( friction1Constraint.jacConstraint.vJLinearAxis * pBodyB->GetInvMass() ), friction1Constraint.jacConstraint.vInvIBxJtAngB, friction1Constraint.fImpulse );
        } else {
            normalConstraint.fImpulse = 0.0f;
            friction0Constraint.fImpulse = 0.0f;
            friction1Constraint.fImpulse = 0.0f;
        }
        normalConstraint.fCorrection = 0.0f;
        friction0Constraint.fCorrection = 0.0f;
        friction1Constraint.fCorrection = 0.0f;
        if ( bRollingFriction ) {
            rollingFrictionConstraint0.fImpulse = 0.0f;
            rollingFrictionConstraint0.fCorrection = 0.0f;
            if ( !bSingleAxisRollingFriction ) {
                rollingFrictionConstraint1.fImpulse = 0.0f;
                rollingFrictionConstraint1.fCorrection = 0.0f;
                rollingFrictionConstraint2.fImpulse = 0.0f;
                rollingFrictionConstraint2.fCorrection = 0.0f;
            }
        }

        // Save constraint data
        m_arrContactNormalConstraints.Push( normalConstraint );
        m_arrContactFrictionConstraints.Push( friction0Constraint );
        m_arrContactFrictionConstraints.Push( friction1Constraint );
        if ( bRollingFriction ) {
            m_arrContactRollingFrictionConstraints.Push( rollingFrictionConstraint0 );
            if ( !bSingleAxisRollingFriction ) {
                m_arrContactRollingFrictionConstraints.Push( rollingFrictionConstraint1 );
                m_arrContactRollingFrictionConstraints.Push( rollingFrictionConstraint2 );
            }
            bRollingFriction = false;
        }
    }
}
Void ConstraintSolver::_EvaluateJoint( BodyNode * pBodyA, BodyNode * pBodyB, JointConstraint * pJoint )
{
    // Check processing
    if ( !(pJoint->IsEnabled()) )
        return;

    // Evaluate the joint
    pJoint->Evaluate( m_fInvTimeStep, m_fJointDefaultERP, m_fJointDefaultCFM );

    // Initialize feedback
    JointFeedback * pJointFeedback = pJoint->GetFeedback();
    if ( pJointFeedback != NULL ) {
        pJointFeedback->vForceBodyA = Vector3::Null;
        pJointFeedback->vForceBodyB = Vector3::Null;
        pJointFeedback->vTorqueBodyA = Vector3::Null;
        pJointFeedback->vTorqueBodyB = Vector3::Null;
    }

    // Enumerate all constraint rows
    UInt iRowCount = pJoint->GetRowCount();
    ConstraintData jointConstraint;

    for( UInt i = 0; i < iRowCount; ++i ) {
        const JointConstraintRow * pJointRow = pJoint->GetRow( i );

        // Save pointer to constraint
        jointConstraint.pObject = (Void*)pJoint;
        jointConstraint.iConstraintLink = i;

        // Save pointers to bodies
        jointConstraint.pBodyA = pBodyA;
        jointConstraint.pBodyB = pBodyB;

        // Friction (not applicable)
        jointConstraint.fFriction = 0.0f;

        // Compute Jacobian
        jointConstraint.jacConstraint = pJointRow->constrJacobian;

        // Compute joint bias
        Scalar fDesiredVelocity = 0.0f;

        Scalar fRelativeVelocity = jointConstraint.jacConstraint.GetRelativeVelocity (
            pBodyA->GetLinearSpeed(), pBodyA->GetAngularSpeed(),
            pBodyB->GetLinearSpeed(), pBodyB->GetAngularSpeed()
        );
        Scalar fVelocityError = ( fDesiredVelocity - fRelativeVelocity );
        Scalar fPositionError = pJointRow->fError;

        jointConstraint.fBias = ( (fVelocityError + fPositionError) * jointConstraint.jacConstraint.fInvDiagJWJtAB );
        jointConstraint.fBiasCorrection = 0.0f;

        // Fill CFM & Limits
        jointConstraint.fCFM = pJointRow->fCFM;
        jointConstraint.fLowerLimit = pJointRow->fLowerLimit;
        jointConstraint.fUpperLimit = pJointRow->fUpperLimit;

        // Warm-Starting
        if ( m_bUseWarmStart ) {
            jointConstraint.fImpulse = ( pJointRow->fImpulse * m_fWarmStartFactor );

            pBodyA->AccumulateImpulse( +( jointConstraint.jacConstraint.vJLinearAxis * pBodyA->GetInvMass() ), jointConstraint.jacConstraint.vInvIAxJtAngA, jointConstraint.fImpulse );
            pBodyB->AccumulateImpulse( -( jointConstraint.jacConstraint.vJLinearAxis * pBodyB->GetInvMass() ), jointConstraint.jacConstraint.vInvIBxJtAngB, jointConstraint.fImpulse );
        } else
            jointConstraint.fImpulse = 0.0f;
        jointConstraint.fCorrection = 0.0f;

        // Save constraint data
        m_arrJointConstraints.Push( jointConstraint );
    }
}

Void ConstraintSolver::_ProjectedGaussSeidel_Interval( ConstraintData * pConstraintData )
{
    BodyNode * pBodyA = pConstraintData->pBodyA;
    BodyNode * pBodyB = pConstraintData->pBodyB;

    // Compute delta impulse
    Scalar fDeltaImpulse = pConstraintData->fBias - ( pConstraintData->fCFM * pConstraintData->fImpulse );
    Scalar fRelativeVelocity = pConstraintData->jacConstraint.GetRelativeVelocity (
        pBodyA->GetLinearImpulse(), pBodyA->GetAngularImpulse(),
        pBodyB->GetLinearImpulse(), pBodyB->GetAngularImpulse()
    );
    fDeltaImpulse -= ( fRelativeVelocity * pConstraintData->jacConstraint.fInvDiagJWJtAB );

    // Clamp accumulated impulse
    Scalar fNewImpulse = ( pConstraintData->fImpulse + fDeltaImpulse );
    if ( fNewImpulse < pConstraintData->fLowerLimit ) {
        fDeltaImpulse = ( pConstraintData->fLowerLimit - pConstraintData->fImpulse );
        pConstraintData->fImpulse = pConstraintData->fLowerLimit;
    } else if ( fNewImpulse > pConstraintData->fUpperLimit ) {
        fDeltaImpulse = ( pConstraintData->fUpperLimit - pConstraintData->fImpulse );
        pConstraintData->fImpulse = pConstraintData->fUpperLimit;
    } else
        pConstraintData->fImpulse = fNewImpulse;

    // Accumulate
    pBodyA->AccumulateImpulse( +( pConstraintData->jacConstraint.vJLinearAxis * pBodyA->GetInvMass() ), pConstraintData->jacConstraint.vInvIAxJtAngA, fDeltaImpulse );
    pBodyB->AccumulateImpulse( -( pConstraintData->jacConstraint.vJLinearAxis * pBodyB->GetInvMass() ), pConstraintData->jacConstraint.vInvIBxJtAngB, fDeltaImpulse );
}
Void ConstraintSolver::_ProjectedGaussSeidel_LowBound( ConstraintData * pConstraintData )
{
    BodyNode * pBodyA = pConstraintData->pBodyA;
    BodyNode * pBodyB = pConstraintData->pBodyB;

    // Compute delta impulse
    Scalar fDeltaImpulse = pConstraintData->fBias - ( pConstraintData->fCFM * pConstraintData->fImpulse );
    Scalar fRelativeVelocity = pConstraintData->jacConstraint.GetRelativeVelocity (
        pBodyA->GetLinearImpulse(), pBodyA->GetAngularImpulse(),
        pBodyB->GetLinearImpulse(), pBodyB->GetAngularImpulse()
    );
    fDeltaImpulse -= ( fRelativeVelocity * pConstraintData->jacConstraint.fInvDiagJWJtAB );

    // Clamp accumulated impulse
    Scalar fNewImpulse = ( pConstraintData->fImpulse + fDeltaImpulse );
    if ( fNewImpulse < pConstraintData->fLowerLimit ) {
        fDeltaImpulse = ( pConstraintData->fLowerLimit - pConstraintData->fImpulse );
        pConstraintData->fImpulse = pConstraintData->fLowerLimit;
    } else
        pConstraintData->fImpulse = fNewImpulse;

    // Accumulate
    pBodyA->AccumulateImpulse( +( pConstraintData->jacConstraint.vJLinearAxis * pBodyA->GetInvMass() ), pConstraintData->jacConstraint.vInvIAxJtAngA, fDeltaImpulse );
    pBodyB->AccumulateImpulse( -( pConstraintData->jacConstraint.vJLinearAxis * pBodyB->GetInvMass() ), pConstraintData->jacConstraint.vInvIBxJtAngB, fDeltaImpulse );
}
Void ConstraintSolver::_ProjectedGaussSeidel_Correction( ConstraintData * pConstraintData )
{
    if ( pConstraintData->fBiasCorrection == 0.0f )
        return;

    BodyNode * pBodyA = pConstraintData->pBodyA;
    BodyNode * pBodyB = pConstraintData->pBodyB;

    // Compute delta correction
    Scalar fDeltaCorrection = pConstraintData->fBiasCorrection - ( pConstraintData->fCFM * pConstraintData->fCorrection );
    Scalar fRelativeVelocity = pConstraintData->jacConstraint.GetRelativeVelocity (
        pBodyA->GetLinearCorrection(), pBodyA->GetAngularCorrection(),
        pBodyB->GetLinearCorrection(), pBodyB->GetAngularCorrection()
    );
    fDeltaCorrection -= ( fRelativeVelocity * pConstraintData->jacConstraint.fInvDiagJWJtAB );

    // Clamp accumulated impulse
    Scalar fNewCorrection = ( pConstraintData->fCorrection + fDeltaCorrection );
    if ( fNewCorrection < pConstraintData->fLowerLimit ) {
        fDeltaCorrection = ( pConstraintData->fLowerLimit - pConstraintData->fCorrection );
        pConstraintData->fCorrection = pConstraintData->fLowerLimit;
    } else
        pConstraintData->fCorrection = fNewCorrection;

    // Accumulate
    pBodyA->AccumulateCorrection( +( pConstraintData->jacConstraint.vJLinearAxis * pBodyA->GetInvMass() ), pConstraintData->jacConstraint.vInvIAxJtAngA, fDeltaCorrection );
    pBodyB->AccumulateCorrection( -( pConstraintData->jacConstraint.vJLinearAxis * pBodyB->GetInvMass() ), pConstraintData->jacConstraint.vInvIBxJtAngB, fDeltaCorrection );
}

Void ConstraintSolver::_SolveConstraints_PGS()
{
    UInt iContactNormalConstraintCount = m_arrContactNormalConstraints.Count();
    UInt iContactFrictionConstraintCount = m_arrContactFrictionConstraints.Count();
    UInt iContactRollingFrictionConstraintCount = m_arrContactRollingFrictionConstraints.Count();
    UInt iJointConstraintCount = m_arrJointConstraints.Count();

    ConstraintData * pConstraint;
    ConstraintData * pParentConstraint;
    Scalar fFrictionLimit;

    UInt iIteration, iConstraint;

    // Correction loop
    if ( m_bUseCorrection ) {
        for( iIteration = 0; iIteration < m_iCorrectionIterations; ++iIteration ) {
            // Contact normal constraints only
            for( iConstraint = 0; iConstraint < iContactNormalConstraintCount; ++iConstraint ) {
                pConstraint = &(m_arrContactNormalConstraints[iConstraint]);
                _ProjectedGaussSeidel_Correction( pConstraint );
            }
        }
    }

    // Solver loop
    for( iIteration = 0; iIteration < m_iIterations; ++iIteration ) {
        // Joints first
        for( iConstraint = 0; iConstraint < iJointConstraintCount; ++iConstraint ) {
            pConstraint = &(m_arrJointConstraints[iConstraint]);
            _ProjectedGaussSeidel_Interval( pConstraint );
        }

        // Contact normal constraints then
        for( iConstraint = 0; iConstraint < iContactNormalConstraintCount; ++iConstraint ) {
            pConstraint = &(m_arrContactNormalConstraints[iConstraint]);
            _ProjectedGaussSeidel_LowBound( pConstraint );
        }

        // Contact friction constraints then
        for( iConstraint = 0; iConstraint < iContactFrictionConstraintCount; ++iConstraint ) {
            pConstraint = &(m_arrContactFrictionConstraints[iConstraint]);
            pParentConstraint = &(m_arrContactNormalConstraints[pConstraint->iConstraintLink]);
            if ( pParentConstraint->fImpulse > 0.0f ) {
                fFrictionLimit = ( pConstraint->fFriction * pParentConstraint->fImpulse );
                pConstraint->fLowerLimit = -fFrictionLimit;
                pConstraint->fUpperLimit = +fFrictionLimit;
                _ProjectedGaussSeidel_Interval( pConstraint );
            }
        }

        // Contact rolling friction constraints last
        for( iConstraint = 0; iConstraint < iContactRollingFrictionConstraintCount; ++iConstraint ) {
            pConstraint = &(m_arrContactRollingFrictionConstraints[iConstraint]);
            pParentConstraint = &(m_arrContactNormalConstraints[pConstraint->iConstraintLink]);
            if ( pParentConstraint->fImpulse > 0.0f ) {
                fFrictionLimit = ( pConstraint->fFriction * pParentConstraint->fImpulse );
                if ( fFrictionLimit > pConstraint->fFriction) 
                    fFrictionLimit = pConstraint->fFriction;
                pConstraint->fLowerLimit = -fFrictionLimit;
                pConstraint->fUpperLimit = +fFrictionLimit;
                _ProjectedGaussSeidel_Interval( pConstraint );
            }
        }
    }
}
Void ConstraintSolver::_SolveConstraints_SORPGS()
{
    UInt iContactNormalConstraintCount = m_arrContactNormalConstraints.Count();
    UInt iContactFrictionConstraintCount = m_arrContactFrictionConstraints.Count();
    UInt iContactRollingFrictionConstraintCount = m_arrContactRollingFrictionConstraints.Count();
    UInt iJointConstraintCount = m_arrJointConstraints.Count();

    ConstraintData * pConstraint;
    ConstraintData * pParentConstraint;
    Scalar fFrictionLimit;

    UInt iIteration, iConstraint, i;

    // Generate shuffle arrays
    for( i = 0; i < iContactNormalConstraintCount; ++i )
        m_arrContactNormalShuffle.Push( i );
    for( i = 0; i < iContactFrictionConstraintCount; ++i )
        m_arrContactFrictionShuffle.Push( i );
    for( i = 0; i < iJointConstraintCount; ++i )
        m_arrJointShuffle.Push( i );

    // Correction loop
    if ( m_bUseCorrection ) {
        for( iIteration = 0; iIteration < m_iCorrectionIterations; ++iIteration ) {
            // Contacts normal constraints only
            for( iConstraint = 0; iConstraint < iContactNormalConstraintCount; ++iConstraint ) {
                i = m_arrContactNormalShuffle[iConstraint];
                pConstraint = &(m_arrContactNormalConstraints[i]);
                _ProjectedGaussSeidel_Correction( pConstraint );
            }
        }
    }

    // Solver loop
    for( iIteration = 0; iIteration < m_iIterations; ++iIteration ) {
        // Shuffling
        if ( (iIteration & (m_iSORPeriod - 1)) == 0 )
            _ShuffleConstraints();

        // Joints first
        for( iConstraint = 0; iConstraint < iJointConstraintCount; ++iConstraint ) {
            i = m_arrJointShuffle[iConstraint];
            pConstraint = &(m_arrJointConstraints[i]);
            _ProjectedGaussSeidel_Interval( pConstraint );
        }

        // Contact normal constraints then
        for( iConstraint = 0; iConstraint < iContactNormalConstraintCount; ++iConstraint ) {
            i = m_arrContactNormalShuffle[iConstraint];
            pConstraint = &(m_arrContactNormalConstraints[i]);
            _ProjectedGaussSeidel_LowBound( pConstraint );
        }

        // Contact friction constraints then
        for( iConstraint = 0; iConstraint < iContactFrictionConstraintCount; ++iConstraint ) {
            i = m_arrContactFrictionShuffle[iConstraint];
            pConstraint = &(m_arrContactFrictionConstraints[i]);
            pParentConstraint = &(m_arrContactNormalConstraints[pConstraint->iConstraintLink]);
            if ( pParentConstraint->fImpulse > 0.0f ) {
                fFrictionLimit = ( pConstraint->fFriction * pParentConstraint->fImpulse );
                pConstraint->fLowerLimit = -fFrictionLimit;
                pConstraint->fUpperLimit = +fFrictionLimit;
                _ProjectedGaussSeidel_Interval( pConstraint );
            }
        }

        // Contact rolling friction constraints last
        for( iConstraint = 0; iConstraint < iContactRollingFrictionConstraintCount; ++iConstraint ) {
            pConstraint = &(m_arrContactRollingFrictionConstraints[iConstraint]);
            pParentConstraint = &(m_arrContactNormalConstraints[pConstraint->iConstraintLink]);
            if ( pParentConstraint->fImpulse > 0.0f ) {
                fFrictionLimit = ( pConstraint->fFriction * pParentConstraint->fImpulse );
                if ( fFrictionLimit > pConstraint->fFriction) 
                    fFrictionLimit = pConstraint->fFriction;
                pConstraint->fLowerLimit = -fFrictionLimit;
                pConstraint->fUpperLimit = +fFrictionLimit;
                _ProjectedGaussSeidel_Interval( pConstraint );
            }
        }
    }
}
Void ConstraintSolver::_SolveConstraints_Finalize()
{
    // Backup impulses to contact points and joints
    UInt iContactNormalConstraintCount = m_arrContactNormalConstraints.Count();
    UInt iJointConstraintCount = m_arrJointConstraints.Count();

    ConstraintData * pConstraint;
    ConstraintData * pFrictionConstraint0;
    ConstraintData * pFrictionConstraint1;
    ContactPoint * pCP;
    JointConstraint * pJoint;
    JointConstraintRow * pJointRow;
    UInt iConstraint;

    // Warm-Starting
    if ( m_bUseWarmStart ) {
        for( iConstraint = 0; iConstraint < iContactNormalConstraintCount; ++iConstraint ) {
            pConstraint = &(m_arrContactNormalConstraints[iConstraint]);
            pFrictionConstraint0 = &(m_arrContactFrictionConstraints[pConstraint->iConstraintLink]);
            pFrictionConstraint1 = &(m_arrContactFrictionConstraints[pConstraint->iConstraintLink + 1]);

            pCP = (ContactPoint*)( pConstraint->pObject );
            pCP->fNormalImpulse = pConstraint->fImpulse;
            pCP->fFrictionImpulse0 = pFrictionConstraint0->fImpulse;
            pCP->fFrictionImpulse1 = pFrictionConstraint1->fImpulse;
        }

        for( iConstraint = 0; iConstraint < iJointConstraintCount; ++iConstraint ) {
            pConstraint = &(m_arrJointConstraints[iConstraint]);

            pJoint = (JointConstraint*)( pConstraint->pObject );
            pJointRow = pJoint->GetRow( pConstraint->iConstraintLink );
            pJointRow->fImpulse = pConstraint->fImpulse;
            if ( MathFn->Abs(pConstraint->fImpulse) > pJoint->GetBreakingThreshold() )
                pJoint->Disable();

            JointFeedback * pJointFeedback = pJoint->GetFeedback();
            if ( pJointFeedback != NULL ) {
                pJointFeedback->vForceBodyA += pConstraint->jacConstraint.vJLinearAxis * ( pConstraint->fImpulse * m_fInvTimeStep );
                pJointFeedback->vForceBodyB -= pConstraint->jacConstraint.vJLinearAxis * ( pConstraint->fImpulse * m_fInvTimeStep );
                pJointFeedback->vTorqueBodyA += pConstraint->jacConstraint.vJAngularA * ( pConstraint->fImpulse * m_fInvTimeStep );
                pJointFeedback->vTorqueBodyB -= pConstraint->jacConstraint.vJAngularB * ( pConstraint->fImpulse * m_fInvTimeStep );
            }
        }
    }

    // Apply impulses & corrections to rigid bodies
    BodyNode * pBody = m_pIsland->GetBodyList();
    while( pBody != NULL ) {
        // Soft (skip)
        if ( pBody->IsSoft() ) {
            pBody = (BodyNode*)( pBody->GetNextInIsland() );
            continue;
        }

        pBody->ApplyAccumulatedImpulses();
        pBody->ResetLinearImpulse();
        pBody->ResetAngularImpulse();
        if ( m_bUseCorrection ) {
            pBody->ApplyAccumulatedCorrections( m_fTimeStep );
            pBody->ResetLinearCorrection();
            pBody->ResetAngularCorrection();
        }

        pBody = (BodyNode*)( pBody->GetNextInIsland() );
    }
}















