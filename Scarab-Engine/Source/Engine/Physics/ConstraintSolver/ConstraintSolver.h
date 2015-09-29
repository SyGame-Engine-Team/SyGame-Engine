/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/ConstraintSolver/ConstraintSolver.h
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
// Header prelude
#ifndef SCARAB_ENGINE_PHYSICS_CONSTRAINTSOLVER_CONSTRAINTSOLVER_H
#define SCARAB_ENGINE_PHYSICS_CONSTRAINTSOLVER_CONSTRAINTSOLVER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Datastruct/Array/Array.h"

#include "../../../Lib/Math/Random/Random.h"

#include "../IslandSolver/Island.h"
#include "../IslandSolver/Nodes/BodyNode.h"
#include "../IslandSolver/Edges/ContactEdge.h"
#include "../IslandSolver/Edges/JointEdge.h"

#include "JacobianConstraint.h"

#include "SoftConstraintSolver.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
    // Solver parameters
#define CONSTRAINTSOLVER_ITERATIONS 16 // Iterations count for solver

#define CONSTRAINTSOLVER_SOR_PERIOD  8 // Number of iterations between SOR
                                       // shuffles, must be power of 2

#define CONSTRAINTSOLVER_CORRECTION_ITERATIONS 8    // Iterations count for correction
#define CONSTRAINTSOLVER_CORRECTION_THRESHOLD  0.0f // Use correction when signed distance goes below this
#define CONSTRAINTSOLVER_CORRECTION_ERP        0.8f // Use that value instead of contact value

#define CONSTRAINTSOLVER_WARMSTART_FACTOR 0.85f // Amount of previous impulse to re-use
                                                // as a starting guess for next solve

    // Contact parameters
#define CONSTRAINTSOLVER_CONTACT_PROCESS_THRESHOLD 0.02f // Must be of lesser order than narrowphase's ZeroTolerance values (2 at least)

#define CONSTRAINTSOLVER_CONTACT_RESTING_THRESHOLD 4 // Minimal lifetime value to assume a contact
                                                     // to be resting and remove restitution

#define CONSTRAINTSOLVER_CONTACT_ROLLINGFRICTION_SINGLEAXIS_THRESHOLD 1.0e30f // Above that, use only one axis for rolling friction

#define CONSTRAINTSOLVER_CONTACT_NORMAL_SLOP         0.05f // Linear offset along normal (= linear slop)
#define CONSTRAINTSOLVER_CONTACT_NORMAL_ERP          0.2f  // Error Reduction Parameter along normal (= Baumgarte)
#define CONSTRAINTSOLVER_CONTACT_NORMAL_CFM          0.0f  // Constraint Force Mixing for normal (should be 0)
#define CONSTRAINTSOLVER_CONTACT_FRICTION_CFM        0.0f  // Constraint Force Mixing for friction
#define CONSTRAINTSOLVER_CONTACT_ROLLINGFRICTION_CFM 0.0f  // Constraint Force Mixing for rolling friction
#define CONSTRAINTSOLVER_CONTACT_FRICTION_MAX        10.0f // Maximal friction factor (clamp to this)

    // Joint parameters
#define CONSTRAINTSOLVER_JOINT_DEFAULT_ERP 0.2f // Default ERP for joints
#define CONSTRAINTSOLVER_JOINT_DEFAULT_CFM 0.0f // Default CFM for joints

    // Constraints data
typedef struct _constraint_data
{
    // Object that created the constraint
    //  - Contact => (ContactPoint*) + Links between normal/friction
    //  - Joint   => (JointConstraint*) + Row index
    Void * pObject;
    UInt iConstraintLink;

    // Bodies
    BodyNode * pBodyA;
    BodyNode * pBodyB;

    // Jacobian
    JacobianConstraint jacConstraint;

    // Error
    Scalar fBias;
    Scalar fBiasCorrection; // Contact normal only
    //Scalar fERP;
    Scalar fCFM;

    // Limits
    Scalar fLowerLimit;
    Scalar fUpperLimit;

    // Impulse
    Scalar fImpulse;
    Scalar fCorrection; // Contact normal only

    // Friction
    Scalar fFriction; // Contact friction only
} ConstraintData;

/////////////////////////////////////////////////////////////////////////////////
// The ConstraintSolver class
class ConstraintSolver
{
public:
    ConstraintSolver();
    ~ConstraintSolver();

    // Solver parameters
    Void SetDefaultParameters();

    inline Void SetIterations( UInt iIterations );

    inline Void UseSOR( Bool bUseSOR );
    inline Void SetSORPeriod( UInt iPeriod );

    inline Void UseCorrection( Bool bUseCorrection );
    inline Void SetCorrectionIterations( UInt iIterations );
    inline Void SetCorrectionThreshold( Scalar fThreshold );
    inline Void SetCorrectionERP( Scalar fERP );

    inline Void UseWarmStart( Bool bUseWarmStart );
    inline Void SetWarmStartFactor( Scalar fFactor );

    inline Void SetContactProcessThreshold( Scalar fThreshold );
    inline Void SetContactRestingThreshold( UInt iThreshold );

    inline Void SetContactNormalSlop( Scalar fSlop );
    inline Void SetContactNormalERP( Scalar fERP );
    inline Void SetContactNormalCFM( Scalar fCFM );
    inline Void SetContactFrictionCFM( Scalar fCFM );
    inline Void SetContactRollingFrictionCFM( Scalar fCFM );
    inline Void SetContactFrictionMAX( Scalar fMAX );

    inline Void UseVelocityDependantFrictionFrame( Bool bUseVelocityDependantFrictionFrame );
    inline Void SetSingleAxisRollingFrictionThreshold( Scalar fSingleAxisRollingFrictionThreshold );

    inline Void SetJointDefaultERP( Scalar fERP );
    inline Void SetJointDefaultCFM( Scalar fCFM );

    // Solver methods
    Void Prepare( Island * pIsland, Scalar fTimeStep, UInt iStackLayer = INVALID_OFFSET );

    Void SolveVelocities();
    Void SolvePositions();

private:
    // Helpers
    inline Scalar _ComputeMixedRestitution( Scalar fRestitutionA, Scalar fRestitutionB ) const;
    inline Scalar _ComputeMixedFriction( Scalar fFrictionA, Scalar fFrictionB ) const;

    Void _ShuffleConstraints();

    // Solver sub-routines
    Void _EvaluateContact( BodyNode * pBodyA, BodyNode * pBodyB, ContactManifold * pContactManifold );
    Void _EvaluateJoint( BodyNode * pBodyA, BodyNode * pBodyB, JointConstraint * pJoint );

    Void _ProjectedGaussSeidel_Interval( ConstraintData * pConstraintData );
    Void _ProjectedGaussSeidel_LowBound( ConstraintData * pConstraintData );
    Void _ProjectedGaussSeidel_Correction( ConstraintData * pConstraintData );

    Void _SolveConstraints_PGS();
    Void _SolveConstraints_SORPGS();
    Void _SolveConstraints_Finalize();

    // Soft constraints solver
    SoftConstraintSolver m_hSoftConstraintSolver;

    // Solver iterations
    UInt m_iIterations;

    // Successive Over-Relaxation
    Bool m_bUseSOR;
    UInt m_iSORPeriod;

    // Position/Orientation Correction
    Bool m_bUseCorrection;
    UInt m_iCorrectionIterations;
    Scalar m_fCorrectionThreshold;
    Scalar m_fCorrectionERP;

    // Warm-Starting (persistent impulses)
    Bool m_bUseWarmStart;
    Scalar m_fWarmStartFactor;

    // Contact solving parameters
    Scalar m_fContactProcessThreshold;
    UInt m_iContactRestingThreshold;

    Scalar m_fContactNormalSlop;
    Scalar m_fContactNormalERP;
    Scalar m_fContactNormalCFM;
    Scalar m_fContactFrictionCFM;
    Scalar m_fContactRollingFrictionCFM;
    Scalar m_fContactFrictionMAX;
    Bool m_bUseVelocityDependantFrictionFrame;
    Scalar m_fSingleAxisRollingFrictionThreshold;

    // Joint solving parameters
    Scalar m_fJointDefaultERP;
    Scalar m_fJointDefaultCFM;

    // Solver constraints
    Array<ConstraintData> m_arrContactNormalConstraints;
    Array<ConstraintData> m_arrContactFrictionConstraints;
    Array<ConstraintData> m_arrContactRollingFrictionConstraints;
    Array<ConstraintData> m_arrJointConstraints;

    Array<UInt> m_arrContactNormalShuffle;
    Array<UInt> m_arrContactFrictionShuffle;
    Array<UInt> m_arrJointShuffle;

    // Solver state
    Island * m_pIsland;
    Scalar m_fTimeStep;
    Scalar m_fInvTimeStep;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "ConstraintSolver.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_CONSTRAINTSOLVER_CONSTRAINTSOLVER_H

