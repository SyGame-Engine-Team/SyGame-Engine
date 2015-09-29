/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Bodies/Systems/Skeleton.h
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
// Known Bugs : Please note that a skeleton should be a tree most of the time,
//              but in the general case, this is more of a data-flow graph
//              where derived attributes follow parent links while inherited
//              attributes follow child links ...
//              Inheritence here meaning parent->target update and derivation
//              means target->parent constrained dependencies ...
//              TODO = Add BoneControllers as shape controllers and manage
//                     them from Skeleton ...
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_PHYSICS_BODIES_SYSTEMS_SKELETON_H
#define SCARAB_ENGINE_PHYSICS_BODIES_SYSTEMS_SKELETON_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Controllers/Motion/IKJointController.h"

#include "../SystemBody.h"
#include "../KinematicBody.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

    // IK-Solver max iterations
#define SKELETON_MAX_ITERATIONS 1
#define SKELETON_IK_THRESHOLD   0.001f

/////////////////////////////////////////////////////////////////////////////////
// The Skeleton class
class Skeleton : public SystemBody
{
public:
    Skeleton( KinematicBody * pRootBone, UInt iRootChildCount, UInt iGoalCount );
    virtual ~Skeleton();

    // Type
    inline virtual SystemBodyType GetType() const;

    // Rigging process : Begin with bones, then joints and goals. Be carefull to match
    //                   joint's goals indices with the goal array.
    //                   Use finalize when finished to lock the skeleton before using it.
    IKBone * CreateBone( KinematicBody * pBoneBody, UInt iChildCount );

    IKJointController * CreateJoint( const GChar * strName, IKBone * pParentBone, IKBone * pChildBone,
                                     UInt iFirstGoal, UInt iGoalCount,
                                     EulerAngles iEulerAnglesMode = EULER_ANGLES_ZYX );

    IKGoal * CreateGoal( UInt iGoal, IKBone * pEffector, BodyNode * pGoal, Scalar fWeight );

    Void Finalize();

    // Skeleton access
    inline IKBone * GetRootBone() const;

    // Goals access
    inline UInt GetGoalCount() const;
    inline IKGoal * GetGoal( UInt iGoal ) const;

    // Update support
    virtual Void Update( Scalar fTimeStep );

    // IK parameters
    UInt MaxIterations; // default = 1

private:
    // Helpers
    //Scalar _ComputeGoalDistance() const;

    Void _rec_UpdateJoint( IKJointController * pJoint );
    Void _rec_UpdateTargets( IKJointController * pJoint );

    // Skeleton structure
    IKBone * m_pRootBone;

    // Goals
    UInt m_iGoalCount;
    IKGoal ** m_arrGoals;

    // State
    Bool m_bFinalized;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Skeleton.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_BODIES_SYSTEMS_SKELETON_H

