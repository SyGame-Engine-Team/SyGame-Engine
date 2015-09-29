/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/ConstraintSolver/SoftConstraintSolver.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Iterative Velocity & Position solvers for soft bodies
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
#ifndef SCARAB_ENGINE_PHYSICS_CONSTRAINTSOLVER_SOFTCONSTRAINTSOLVER_H
#define SCARAB_ENGINE_PHYSICS_CONSTRAINTSOLVER_SOFTCONSTRAINTSOLVER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Datastruct/Array/Array.h"

#include "../Bodies/SoftBody.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

    // Soft constraint solvers
enum SoftConstraintSolverType
{
    SOFTCONSTRAINT_SOLVER_VELOCITIES = 0,
    SOFTCONSTRAINT_SOLVER_POSITIONS
};

enum SoftConstraintVelocitySolverType
{
    SOFTCONSTRAINT_VSOLVER_LINKS = 0,

    SOFTCONSTRAINT_VSOLVER_COUNT
};
enum SoftConstraintPositionSolverType
{
    SOFTCONSTRAINT_PSOLVER_LINKS = 0,
    SOFTCONSTRAINT_PSOLVER_CONTACTS_RIGID,
    SOFTCONSTRAINT_PSOLVER_CONTACTS_SOFT,
    SOFTCONSTRAINT_PSOLVER_ANCHORS,

    SOFTCONSTRAINT_PSOLVER_COUNT
};

    // Constraints data
typedef struct _softlink_data
{
    SoftBody::Link * pLink;

    Vector3 vGradient;
    Scalar fGradientFactor;
} SoftLinkData;

typedef struct _rigidsoftcontact_data
{
    BodyNode * pBody; // Rigid or Kinematic
    SoftBody::Node * pNode;

    Vector3 vNormal;
    Scalar fDistance;

    Scalar fHardness;
    Scalar fFriction;

    Scalar fInvMassDT;
    Vector3 vRelativeAnchor;
    Matrix3 matImpulse;
} RigidSoftContactData;

typedef struct _softsoftcontact_data
{
    SoftBody::Facet * pFacet;
    SoftBody::Node * pNode;

    Vector3 vNormal;
    Vector3 vWeights;
    Scalar fMargin;

    Scalar fFriction;
    Scalar fCFM[2];
} SoftSoftContactData;

typedef struct _rigidsoftanchor_data
{
    BodyNode * pBody; // Rigid or Kinematic
    SoftBody::Node * pNode;

    Vector3 vLocalAnchor;

    Scalar fInvMassDT;
    Vector3 vRelativeAnchor;
    Matrix3 matImpulse;
} RigidSoftAnchorData;

/////////////////////////////////////////////////////////////////////////////////
// The SoftConstraintSolver class
class SoftConstraintSolver
{
public:
    SoftConstraintSolver();
    ~SoftConstraintSolver();

    // Solver parameters
    Void SetDefaultParameters();

    /////////////////////////////////////////

    // Constraints access
    inline UInt GetConstraintCount() const;

    Void PushSoftLinks( SoftBody * pSoftBody );
    Void PushSelfContacts( SoftBody * pSoftBody );

    Void PushRigidSoftContacts( BodyNode * pBody, const ContactManifold * pManifold,
                                Scalar fTimeStep, Scalar fInvTimeStep );
    Void PushSoftSoftContacts( const ContactManifold * pManifold );

    Void PushRigidSoftAnchor( const JointEdge * pJointEdge, Scalar fTimeStep, Scalar fInvTimeStep );

    Void ClearConstraints();

    // Solver methods
    Void SetSolver( SoftConstraintSolverType iSolver = SOFTCONSTRAINT_SOLVER_POSITIONS );

    Void SolveVelocities( Scalar fTimeStep );
    Void SolvePositions( Scalar fTimeStep, Scalar fInvTimeStep );

private:
    // Solver sequences
    typedef Void (*VelocitySolver)( SoftConstraintSolver* );
    typedef Void (*PositionSolver)( SoftConstraintSolver*, Scalar );

    static VelocitySolver _GetVelocitySolver( SoftConstraintVelocitySolverType iSolver );
    static PositionSolver _GetPositionSolver( SoftConstraintPositionSolverType iSolver );

    UInt m_iSequenceLengthVelocities;
    UInt m_iSequenceLengthPositions;
    UInt m_iSequenceLengthDrift;

    SoftConstraintVelocitySolverType m_arrSolverSequenceVelocities[SOFTCONSTRAINT_VSOLVER_COUNT];
    SoftConstraintPositionSolverType m_arrSolverSequencePositions[SOFTCONSTRAINT_PSOLVER_COUNT];
    SoftConstraintPositionSolverType m_arrSolverSequenceDrift[SOFTCONSTRAINT_PSOLVER_COUNT];

    // Solver sub-routines
    static Void _SolveVelocities_Links( SoftConstraintSolver * pThis );

    static Void _SolvePositions_Links( SoftConstraintSolver * pThis, Scalar fTimeStep );
    static Void _SolvePositions_RigidContacts( SoftConstraintSolver * pThis, Scalar fTimeStep );
    static Void _SolvePositions_SoftContacts( SoftConstraintSolver * pThis, Scalar fTimeStep );
    static Void _SolvePositions_Anchors( SoftConstraintSolver * pThis, Scalar fTimeStep );

    Void _SolveVelocities();
    Void _SolvePositions( Scalar fTimeStep );
    Void _SolveDrift( Scalar fTimeStep );

    // Solver iterations
    UInt m_iMaxIterationsVelocities; // m_cfg.viterations
    UInt m_iMaxIterationsPositions;  // m_cfg.piterations
    UInt m_iMaxIterationsDrift;      // m_cfg.diterations

    // Solving parameters
    Scalar m_fBaumgarte; // m_cfg.kVCF, Velocity Correction Factor [0,1]
    Scalar m_fDamping;   // m_cfg.kDP, [0,1]

    //Scalar m_fRigidHardness;       // m_cfg.kCHR, [0,1]
    //Scalar m_fKineticHardness;     // m_cfg.kKHR, [0,1]
    //Scalar m_fSoftHardness;        // m_cfg.kSHR, [0,1]
    Scalar m_fAnchorHardness;        // m_cfg.kAHR, [0,1]
    //Scalar m_fSoftRigidHardness;   // m_cfg.kSRHR_CL, [0,1] (cluster only)
    //Scalar m_fSoftKineticHardness; // m_cfg.kSKHR_CL, [0,1] (cluster only)
    //Scalar m_fSoftSoftHardness;    // m_cfg.kSSHR_CL, [0,1] (cluster only)
    //Scalar m_fSoftRigidSplit;      // m_cfg.kSR_SPLT_CL, [0,1] (cluster only)
    //Scalar m_fSoftKineticSplit;    // m_cfg.kSK_SPLT_CL, [0,1] (cluster only)
    //Scalar m_fSoftSoftSplit;       // m_cfg.kSS_SPLT_CL, [0,1] (cluster only)

    // Solver constraints
    Array<SoftLinkData> m_arrSoftLinkConstraints;
    Array<RigidSoftContactData> m_arrRigidSoftContactConstraints;
    Array<SoftSoftContactData> m_arrSoftSoftContactConstraints;
    Array<RigidSoftAnchorData> m_arrRigidSoftAnchorConstraints;

    Array<SoftBody*> m_arrInvolvedBodies;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "SoftConstraintSolver.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_CONSTRAINTSOLVER_SOFTCONSTRAINTSOLVER_H

