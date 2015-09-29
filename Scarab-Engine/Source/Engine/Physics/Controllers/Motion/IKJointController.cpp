/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Controllers/Motion/IKJointController.cpp
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
// Includes
#include "IKJointController.h"

#include "../../PhysicsManager.h"

/////////////////////////////////////////////////////////////////////////////////
// IKBone implementation
IKBone::IKBone( KinematicBody * pBody, UInt iJointCapacity )
{
    m_pBody = pBody;

    m_iJointCount = 0;
    m_iJointCapacity = iJointCapacity;

    PhysicsFn->SelectMemory( TEXT("Controllers") );
    m_arrJoints = New IKJointController*[m_iJointCapacity];
    PhysicsFn->UnSelectMemory();

    for( UInt i = 0; i < m_iJointCapacity; ++i )
        m_arrJoints[i] = NULL;
}
IKBone::~IKBone()
{
    PhysicsFn->SelectMemory( TEXT("Controllers") );

    // Recurse on child bones & joints
    for( UInt i = 0; i < m_iJointCount; ++i ) {
        if ( m_arrJoints[i] != NULL ) {
            Delete( m_arrJoints[i]->GetChild() );
            PhysicsFn->DestroyController( m_arrJoints[i] );
        }
    }
    DeleteA( m_arrJoints );

    PhysicsFn->UnSelectMemory();

    // Remove bone controller
    m_pBody->DetachMotionController();
}

IKJointController * IKBone::GetParentJoint() const
{
    return (IKJointController*)( m_pBody->GetMotionController() );
}
Void IKBone::SetParentJoint( IKJointController * pJoint )
{
    m_pBody->AttachController( pJoint );
}

/////////////////////////////////////////////////////////////////////////////////
// IKGoal implementation
IKGoal::IKGoal( IKBone * pEffector, BodyNode * pGoal, Scalar fWeight )
{
    m_pEffector = pEffector;
    m_pGoal = pGoal;
    Weight = fWeight;
}
IKGoal::~IKGoal()
{
    // nothing to do
}

const Vertex3 & IKGoal::GetEffectorPosition() const
{
    IKJointController * pParentJoint = m_pEffector->GetParentJoint();
    if ( pParentJoint != NULL )
        return pParentJoint->GetPosition();
    return m_pEffector->GetBody()->GetPosition();
}
const Vertex3 & IKGoal::GetGoalPosition() const
{
    return m_pGoal->GetPosition();
}

/////////////////////////////////////////////////////////////////////////////////
// IKJointController implementation
IKJointController::IKJointController( const GChar * strName, IKBone * pParent, IKBone * pChild,
                                                             IKGoal ** arrGoals, UInt iGoalCount,
                                                             EulerAngles iEulerAnglesMode ):
    MotionController( strName ),
    m_vJointTranslation(), m_vJointRotation(), m_vLastPosition(), m_qLastOrientation()
{
    m_pParent = pParent;
    m_pChild = pChild;

    m_iGoalCount = iGoalCount;
    m_arrGoals = arrGoals;

    m_vJointTranslation = Vector3::Null;
    m_vJointRotation = Vector3::Null;

    m_vLastPosition = Vertex3::Null;
    m_qLastOrientation = Quaternion::Identity;

    m_iEulerAnglesMode = iEulerAnglesMode;

    for( UInt i = 0; i < 3; ++i ) {
        AllowTranslate[i] = false;
        MinTranslate[i] = -SCALAR_INFINITE;
        MaxTranslate[i] = SCALAR_INFINITE;
        AllowRotate[i] = false;
        MinRotate[i] = -SCALAR_PI;
        MaxRotate[i] = SCALAR_PI;
    }
}
IKJointController::~IKJointController()
{
    // nothing to do
}

Void IKJointController::SetTarget( KinematicBody * pTarget )
{
    Assert( pTarget == m_pChild->GetBody() );

    // Set m_pTarget
    KinematicController::SetTarget( pTarget );

    // Target state
    m_vPosition = m_pTarget->GetPosition();
    m_qOrientation = m_pTarget->GetOrientation();

    m_vLinearSpeed = m_pTarget->GetLinearSpeed();
    m_vAngularSpeed = m_pTarget->GetAngularSpeed();

    // Joint transform
    KinematicBody * pParentBone = m_pParent->GetBody();

    m_vJointTranslation = ( m_vPosition - pParentBone->GetPosition() );

    Quaternion qLocalOrientation;
    pParentBone->GetOrientation().Invert( qLocalOrientation );
    qLocalOrientation = ( m_qOrientation * qLocalOrientation );

    Matrix3 matRotation;
    qLocalOrientation.GetMatrix( matRotation );
    EulerDecomposition eulerSolver( matRotation );
    switch( m_iEulerAnglesMode ) {
        case EULER_ANGLES_XYZ: eulerSolver.ExtractXYZ( m_vJointRotation.X, m_vJointRotation.Y, m_vJointRotation.Z ); break;
        case EULER_ANGLES_XZY: eulerSolver.ExtractXZY( m_vJointRotation.X, m_vJointRotation.Y, m_vJointRotation.Z ); break;
        case EULER_ANGLES_YXZ: eulerSolver.ExtractYXZ( m_vJointRotation.X, m_vJointRotation.Y, m_vJointRotation.Z ); break;
        case EULER_ANGLES_YZX: eulerSolver.ExtractYZX( m_vJointRotation.X, m_vJointRotation.Y, m_vJointRotation.Z ); break;
        case EULER_ANGLES_ZXY: eulerSolver.ExtractZXY( m_vJointRotation.X, m_vJointRotation.Y, m_vJointRotation.Z ); break;
        case EULER_ANGLES_ZYX: eulerSolver.ExtractZYX( m_vJointRotation.X, m_vJointRotation.Y, m_vJointRotation.Z ); break;
        default: Assert(false); break;
    }

    m_vLastPosition = m_vPosition;
    m_qLastOrientation = m_qOrientation;
}

/////////////////////////////////////////////////////////////////////////////////

const Vertex3 & IKJointController::_GetParentPosition() const
{
    IKJointController * pParentJoint = m_pParent->GetParentJoint();
    if ( pParentJoint != NULL )
        return pParentJoint->m_vPosition;
    return m_pParent->GetBody()->GetPosition();
}
const Quaternion & IKJointController::_GetParentOrientation() const
{
    IKJointController * pParentJoint = m_pParent->GetParentJoint();
    if ( pParentJoint != NULL )
        return pParentJoint->m_qOrientation;
    return m_pParent->GetBody()->GetOrientation();
}
Vector3 IKJointController::_GetParentAxis( UInt iDim ) const
{
    Assert( iDim < 3 );

    Matrix3 matRotation;
    _GetParentOrientation().GetMatrix( matRotation );
        
    Vector3 vAxis;
    matRotation.GetColumn( vAxis, iDim );
    return vAxis;

    //if ( iDim == 0 )
    //    return Vector3::eI;
    //if ( iDim == 1 )
    //    return Vector3::eJ;
    //// ( iDim == 2 )
    //return Vector3::eK;
}
Bool IKJointController::_UpdateJointTranslation( UInt iDim )
{
    Vector3 vParentAxis = _GetParentAxis( iDim );

    IKGoal * pGoal;
    Vector3 vEG;
    Scalar fOldNorm = 0.0f;
    Scalar fNum = 0.0f;
    Scalar fDenom = 0.0f;
    UInt i;

    // Compute distance from effectors to goals
    for( i = 0; i < m_iGoalCount; ++i ) {
        pGoal = m_arrGoals[i];
        vEG = ( pGoal->GetGoalPosition() - pGoal->GetEffectorPosition() );
        fOldNorm += vEG.NormSqr();
        fNum += pGoal->Weight * ( vParentAxis * vEG );
        fDenom += pGoal->Weight;
    }
    if ( MathFn->EqualsZero(fDenom) )
        return false; // null translation
    Scalar fDistance = ( fNum / fDenom );

    // Clamping
    Vector3 vLocalTranslate = m_vJointTranslation;
    Scalar fTranslate = ( vLocalTranslate[iDim] + fDistance );

    if ( fTranslate < MinTranslate[iDim] ) {
        fDistance = MinTranslate[iDim] - vLocalTranslate[iDim];
        vLocalTranslate[iDim] = MinTranslate[iDim];
    } else if ( fTranslate > MaxTranslate[iDim] ) {
        fDistance = MaxTranslate[iDim] - vLocalTranslate[iDim];
        vLocalTranslate[iDim] = MaxTranslate[iDim];
    } else
        vLocalTranslate[iDim] = fTranslate;

    // Check if we got closer
    Vector3 vStep = ( vParentAxis * fDistance );
    Scalar fNewNorm = 0.0f;
    Vertex3 vNewEffector;

    for( i = 0; i < m_iGoalCount; ++i ) {
        pGoal = m_arrGoals[i];
        vNewEffector = ( pGoal->GetEffectorPosition() + vStep );
        vEG = ( pGoal->GetGoalPosition() - vNewEffector );
        fNewNorm += vEG.NormSqr();
    }
    if ( fNewNorm >= fOldNorm )
        return false;

    // Update
    m_vJointTranslation = vLocalTranslate;
    return true;
}
Bool IKJointController::_UpdateJointRotation( UInt iDim )
{
    Vector3 vParentAxis = _GetParentAxis( iDim );
    const Vertex3 & vParentPosition = _GetParentPosition();

    IKGoal * pGoal;
    Vector3 vPE, vPG, vEG, vTmp;
    Scalar fOldNorm = 0.0f;
    Scalar fNum = 0.0f;
    Scalar fDenom = 0.0f;
    UInt i;

    // Compute angle from effectors to goals
    for( i = 0; i < m_iGoalCount; ++i ) {
        pGoal = m_arrGoals[i];
        vEG = ( pGoal->GetGoalPosition() - pGoal->GetEffectorPosition() );
        fOldNorm += vEG.NormSqr();
        vPE = ( pGoal->GetEffectorPosition() - vParentPosition );
        vPG = ( pGoal->GetGoalPosition() - vParentPosition );
        vTmp = ( vParentAxis ^ vPE );
        fNum += ( pGoal->Weight * ( vPG * vTmp ) );
        fDenom += ( pGoal->Weight * ( vPG * vPE ) );
    }
    if ( (fNum*fNum + fDenom*fDenom) < SCALAR_ERROR )
        return false; // no rotation
    Scalar fAngle = MathFn->ArcTan2( fNum, fDenom );

    // Clamping
    Vector3 vLocalRotate = m_vJointRotation;
    Scalar fRotate = ( vLocalRotate[iDim] + fAngle );

    if ( fRotate <= MinRotate[iDim] ) {
        fAngle = MinRotate[iDim] - vLocalRotate[iDim];
        vLocalRotate[iDim] = MinRotate[iDim];
    } else if ( fRotate >= MaxRotate[iDim] ) {
        fAngle = MaxRotate[iDim] - vLocalRotate[iDim];
        vLocalRotate[iDim] = MaxRotate[iDim];
    } else
        vLocalRotate[iDim] = fRotate;

    // Check if we got closer
    Matrix3 matStep;
    matStep.MakeRotate( vParentAxis, fAngle );
    Scalar fNewNorm = 0.0f;
    Vertex3 vNewEffector;

    for( i = 0; i < m_iGoalCount; ++i ) {
        pGoal = m_arrGoals[i];
        vPE = ( pGoal->GetEffectorPosition() - vParentPosition );
        vNewEffector = ( vParentPosition + (matStep * vPE) );
        vEG = ( pGoal->GetGoalPosition() - vNewEffector );
        fNewNorm += vEG.NormSqr();
    }
    if ( fNewNorm >= fOldNorm )
        return false;

    // Update
    m_vJointRotation = vLocalRotate;
    return true;
}
Void IKJointController::_UpdateTarget()
{
    m_vPosition = _GetParentPosition();
    m_qOrientation = _GetParentOrientation();

    Quaternion qTmp;
    switch( m_iEulerAnglesMode ) {
        case EULER_ANGLES_XYZ: qTmp.MakeRotate( m_vJointRotation.X, m_vJointRotation.Y, m_vJointRotation.Z, m_iEulerAnglesMode ); break;
        case EULER_ANGLES_XZY: qTmp.MakeRotate( m_vJointRotation.X, m_vJointRotation.Z, m_vJointRotation.Y, m_iEulerAnglesMode ); break;
        case EULER_ANGLES_YXZ: qTmp.MakeRotate( m_vJointRotation.Y, m_vJointRotation.X, m_vJointRotation.Z, m_iEulerAnglesMode ); break;
        case EULER_ANGLES_YZX: qTmp.MakeRotate( m_vJointRotation.Y, m_vJointRotation.Z, m_vJointRotation.X, m_iEulerAnglesMode ); break;
        case EULER_ANGLES_ZXY: qTmp.MakeRotate( m_vJointRotation.Z, m_vJointRotation.X, m_vJointRotation.Y, m_iEulerAnglesMode ); break;
        case EULER_ANGLES_ZYX: qTmp.MakeRotate( m_vJointRotation.Z, m_vJointRotation.Y, m_vJointRotation.X, m_iEulerAnglesMode ); break;
        default: Assert(false); break;
    }
    m_qOrientation = qTmp * m_qOrientation;

    m_vPosition += ( m_qOrientation * m_vJointTranslation );
}

Bool IKJointController::_UpdateControllerState( Scalar fTimeStep )
{
    Scalar fInvTimeStep = MathFn->Invert( fTimeStep );

    // m_vPosition & m_qOrientation
    // => Already updated from IKController using _UpdateTarget()

    // Compute linear & angular speeds
    m_vLinearSpeed = ( m_vPosition - m_vLastPosition ) * fInvTimeStep;

    Quaternion qRotation;
    m_qLastOrientation.Invert( qRotation );
    qRotation = m_qOrientation * qRotation;
    Scalar fAngle;
    qRotation.GetAxisAngle( m_vAngularSpeed, fAngle );
    m_vAngularSpeed *= ( fAngle * fInvTimeStep );

    m_vLastPosition = m_vPosition;
    m_qLastOrientation = m_qOrientation;

    return true;
}

