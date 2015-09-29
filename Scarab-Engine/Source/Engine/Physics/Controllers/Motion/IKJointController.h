/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Controllers/Motion/IKJointController.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Motion Controller : Inverse Kinematics
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
#ifndef SCARAB_ENGINE_PHYSICS_CONTROLLERS_MOTION_IKJOINTCONTROLLER_H
#define SCARAB_ENGINE_PHYSICS_CONTROLLERS_MOTION_IKJOINTCONTROLLER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../Lib/Math/Algebra/Matrix/EulerDecomposition.h"

#include "../KinematicController.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

    // Prototypes
class IKBone;
class IKGoal;

class IKJointController;

/////////////////////////////////////////////////////////////////////////////////
// The IKBone class
class IKBone
{
public:
    IKBone( KinematicBody * pBody, UInt iJointCapacity );
    ~IKBone();

    // Getters
    inline KinematicBody * GetBody() const;

    IKJointController * GetParentJoint() const;
    Void SetParentJoint( IKJointController * pJoint );

    inline UInt GetChildJointCount() const;
    inline IKJointController * GetChildJoint( UInt iChild ) const;

    inline Void PushChildJoint( IKJointController * pJoint );

private:
    KinematicBody * m_pBody; // Motion controllers from this bone are parent joints which,
                             // in turn, leads to parent bones ...
                             // Excepting the root node which can be of any behaviour
                             // and is free to use any controller ...

    UInt m_iJointCount, m_iJointCapacity; // Children joints have this bone as parent,
    IKJointController ** m_arrJoints;     // their targets are child bones ...
};

/////////////////////////////////////////////////////////////////////////////////
// The IKGoal class
class IKGoal
{
public:
    IKGoal( IKBone * pEffector, BodyNode * pGoal, Scalar fWeight );
    virtual ~IKGoal();

    // Getters
    inline IKBone * GetEffector() const;
    inline BodyNode * GetGoal() const;

    const Vertex3 & GetEffectorPosition() const;
    const Vertex3 & GetGoalPosition() const;

    // Weight factor
    Scalar Weight; // default = 1.0f

private:
    IKBone * m_pEffector;
    BodyNode * m_pGoal;
};

/////////////////////////////////////////////////////////////////////////////////
// The IKJointController class
class IKJointController : public MotionController
{
public:
    IKJointController( const GChar * strName, IKBone * pParent, IKBone * pChild,
                                              IKGoal ** arrGoals, UInt iGoalCount,
                                              EulerAngles iEulerAnglesMode = EULER_ANGLES_ZYX );
    virtual ~IKJointController();

    // Type
    inline virtual ControllerType GetType() const;

    // Target access
    virtual Void SetTarget( KinematicBody * pTarget );

    // Joint access
    inline IKBone * GetParent() const;
    inline IKBone * GetChild() const;

    inline UInt GetGoalCount() const;
    inline IKGoal * GetGoal( UInt iGoal ) const;

    inline const Vector3 & GetJointTranslation() const;
    inline const Vector3 & GetJointRotation() const; // Euler angles

    // Joint constraints
    Bool AllowTranslate[3]; // default = false
    Scalar MinTranslate[3]; // default = -inf
    Scalar MaxTranslate[3]; // default = +inf
    Bool AllowRotate[3];    // default = false
    Scalar MinRotate[3];    // default = -pi
    Scalar MaxRotate[3];    // default = +pi

protected:
    // Give access to Skeleton
    friend class IKGoal;
    friend class Skeleton;

    // Helpers
    const Vertex3 & _GetParentPosition() const;
    const Quaternion & _GetParentOrientation() const;
    Vector3 _GetParentAxis( UInt iDim ) const;

    // Update support
    Bool _UpdateJointTranslation( UInt iDim );
    Bool _UpdateJointRotation( UInt iDim );
    Void _UpdateTarget();

    virtual Bool _UpdateControllerState( Scalar fTimeStep );

    // Joint parent & child
    IKBone * m_pParent;
    IKBone * m_pChild;

    // Goals
    UInt m_iGoalCount;
    IKGoal ** m_arrGoals;

    // Joint transform
    Vector3 m_vJointTranslation;
    Vector3 m_vJointRotation; // Euler angles

    Vertex3 m_vLastPosition;
    Quaternion m_qLastOrientation;

    // Euler angles interpretation
    EulerAngles m_iEulerAnglesMode;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "IKJointController.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_CONTROLLERS_MOTION_IKJOINTCONTROLLER_H

