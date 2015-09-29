/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Bodies/Systems/Skeleton.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : System : Kinematic Skeleton
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
#include "Skeleton.h"

#include "../../PhysicsManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Skeleton implementation
Skeleton::Skeleton( KinematicBody * pRootBone, UInt iRootChildCount, UInt iGoalCount ):
    SystemBody()
{
    PhysicsFn->SelectMemory( TEXT("Controllers") );

    // Skeleton structure
    m_pRootBone = New IKBone( pRootBone, iRootChildCount );

    // Goals
    m_iGoalCount = iGoalCount;
    m_arrGoals = New IKGoal*[m_iGoalCount];
    for( UInt i = 0; i < m_iGoalCount; ++i )
        m_arrGoals[i] = NULL;

    PhysicsFn->UnSelectMemory();

    // Ready for rigging
    m_bFinalized = false;

    // Ready for solving
    MaxIterations = SKELETON_MAX_ITERATIONS;
}
Skeleton::~Skeleton()
{
    Assert( m_bFinalized );

    PhysicsFn->SelectMemory( TEXT("Controllers") );

    // Goals
    for( UInt i = 0; i < m_iGoalCount; ++i ) {
        if ( m_arrGoals[i] != NULL )
            Delete( m_arrGoals[i] );
    }
    DeleteA( m_arrGoals );

    // Skeleton structure
    Delete( m_pRootBone ); // Note this also removes root's controllers (consistency)

    PhysicsFn->UnSelectMemory();
}

IKBone * Skeleton::CreateBone( KinematicBody * pBoneBody, UInt iChildCount )
{
    Assert( !m_bFinalized );

    PhysicsFn->SelectMemory( TEXT("Controllers") );
    IKBone * pBone = New IKBone( pBoneBody, iChildCount );
    PhysicsFn->UnSelectMemory();

    return pBone;
}

IKJointController * Skeleton::CreateJoint( const GChar * strName, IKBone * pParentBone, IKBone * pChildBone,
                                           UInt iFirstGoal, UInt iGoalCount, EulerAngles iEulerAnglesMode )
{
    Assert( !m_bFinalized );

    IKJointController * pJoint = PhysicsFn->CreateIKJointController( strName, pParentBone, pChildBone,
                                                                     (m_arrGoals + iFirstGoal), iGoalCount,
                                                                     iEulerAnglesMode );

    // Linkage
    pChildBone->SetParentJoint( pJoint );
    pParentBone->PushChildJoint( pJoint );

    return pJoint;
}

IKGoal * Skeleton::CreateGoal( UInt iGoal, IKBone * pEffector, BodyNode * pGoal, Scalar fWeight )
{
    Assert( !m_bFinalized );

    PhysicsFn->SelectMemory( TEXT("Controllers") );
    IKGoal * pNewGoal = New IKGoal( pEffector, pGoal, fWeight );
    PhysicsFn->UnSelectMemory();

    // Add to goal array
    m_arrGoals[iGoal] = pNewGoal;

    return pNewGoal;
}

Void Skeleton::Finalize()
{
    Assert( !m_bFinalized );

    // nothing to do
    // Check skeleton structure ?

    m_bFinalized = true;
}

Void Skeleton::Update( Scalar /*fTimeStep*/ )
{
    Assert( m_bFinalized );

    UInt iJoint, iJointCount;
    IKJointController * pJoint;

    // Perform IK-Solver iterations
    //Scalar fPrevGoalDistance = _ComputeGoalDistance();
    //Scalar fGoalDistance;

    for( UInt iIteration = 0; iIteration < MaxIterations; ++iIteration ) {
        iJointCount = m_pRootBone->GetChildJointCount();
        for( iJoint = 0; iJoint < iJointCount; ++iJoint ) {
            pJoint = m_pRootBone->GetChildJoint( iJoint );
            _rec_UpdateJoint( pJoint );
        }

        //fGoalDistance = _ComputeGoalDistance();
        //if ( fGoalDistance < SKELETON_IK_THRESHOLD )
        //    break;
        //if ( MathFn->Equals(fGoalDistance, fPrevGoalDistance) )
        //    break;
        //fPrevGoalDistance = fGoalDistance;
    }
}

/////////////////////////////////////////////////////////////////////////////////

//Scalar Skeleton::_ComputeGoalDistance() const
//{
//    const IKGoal * pGoal;
//    Vector3 vEG;
//    Scalar fDistanceSqr = 0.0f;
//
//    for( UInt i = 0; i < m_iGoalCount; ++i ) {
//        pGoal = m_arrGoals[i];
//        vEG = ( pGoal->GetGoalPosition() - pGoal->GetEffectorPosition() );
//        fDistanceSqr += vEG.NormSqr();
//    }
//
//    return fDistanceSqr;
//}

Void Skeleton::_rec_UpdateJoint( IKJointController * pJoint )
{
    UInt iDim;

    // Recurse on child joints
    IKBone * pChildBone = pJoint->GetChild();

    UInt iJointCount = pChildBone->GetChildJointCount();
    IKJointController * pChildJoint;

    for( UInt iJoint = 0; iJoint < iJointCount; ++iJoint ) {
        pChildJoint = pChildBone->GetChildJoint( iJoint );
        _rec_UpdateJoint( pChildJoint );
    }

    // Update translations
    for( iDim = 0; iDim < 3; ++iDim ) {
        if ( pJoint->AllowTranslate[iDim] ) {
            if ( pJoint->_UpdateJointTranslation(iDim) )
                _rec_UpdateTargets( pJoint );
        }
    }

    // Update rotations
    for( iDim = 0; iDim < 3; ++iDim ) {
        if ( pJoint->AllowRotate[iDim] ) {
            if ( pJoint->_UpdateJointRotation(iDim) )
                _rec_UpdateTargets( pJoint );
        }
    }
}
Void Skeleton::_rec_UpdateTargets( IKJointController * pJoint )
{
    // Update this joint's target
    pJoint->_UpdateTarget();

    // Recurse on child joints
    IKBone * pChildBone = pJoint->GetChild();

    UInt iJointCount = pChildBone->GetChildJointCount();
    IKJointController * pChildJoint;

    for( UInt iJoint = 0; iJoint < iJointCount; ++iJoint ) {
        pChildJoint = pChildBone->GetChildJoint( iJoint );
        _rec_UpdateTargets( pChildJoint );
    }
}
