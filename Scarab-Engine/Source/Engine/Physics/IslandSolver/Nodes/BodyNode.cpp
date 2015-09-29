/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/IslandSolver/Nodes/BodyNode.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Island Node : Bodies
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
#include "BodyNode.h"

#include "../../PhysicsManager.h"

/////////////////////////////////////////////////////////////////////////////////
// BodyNode implementation
const Matrix3 BodyNode::sm_matInfinity( SCALAR_INFINITE, SCALAR_INFINITE, SCALAR_INFINITE,
                                        SCALAR_INFINITE, SCALAR_INFINITE, SCALAR_INFINITE,
                                        SCALAR_INFINITE, SCALAR_INFINITE, SCALAR_INFINITE );

BodyNode::BodyNode( Bool bStatic, Shape * pLocalShape, Scalar fDensity,
                    const Vertex3 & vPosition, const Quaternion & qOrientation,
                    const Vector3 & vLinearSpeed, const Vector3 & vAngularSpeed ):
    IslandNode(),
    m_vCenterOfMass(), m_matInertiaTensor(), m_matInvInertiaTensor(),
    m_vForceBuffer(), m_vTorqueBuffer(),
    m_vLinearImpulseBuffer(), m_vAngularImpulseBuffer(),
    m_vLinearCorrectionBuffer(), m_vAngularCorrectionBuffer(),
    m_vPosition(), m_qOrientation(), m_vTransform(),
    m_vLinearSpeed(), m_vAngularSpeed()
{
    // Behaviour
    m_bStatic = bStatic;

    // Collision filtering
    m_iCollisionGroup = 0xffffffffffffffffui64;
    m_iCollisionMask = 0xffffffffffffffffui64;

    // Sensor
    m_bSensor = false;

    // Sleeping
    m_bSleeping = false;
    m_iSleepTime = 0;

    // Local shape
    m_pLocalShape = pLocalShape;

    // Mass properties
    m_fDensity = fDensity;
    m_vCenterOfMass = m_pLocalShape->GetAAB().GetCenter();
    m_fMass = SCALAR_INFINITE;
    m_fInvMass = 0.0f;
    m_matInertiaTensor = sm_matInfinity;
    m_matInvInertiaTensor = Matrix3::Null;

    m_pLocalShape->ComputeMassProperties( m_fDensity, &m_fMass, &m_vCenterOfMass, &m_matInertiaTensor );

    Vector3 vDot( m_vCenterOfMass.X * m_vCenterOfMass.X,
                  m_vCenterOfMass.Y * m_vCenterOfMass.Y,
                  m_vCenterOfMass.Z * m_vCenterOfMass.Z );
    Vector3 vCross( m_vCenterOfMass.Y * m_vCenterOfMass.Z,
                    m_vCenterOfMass.Z * m_vCenterOfMass.X,
                    m_vCenterOfMass.X * m_vCenterOfMass.Y );

    m_matInertiaTensor.m00 -= ( m_fMass * ( vDot.Y + vDot.Z ) );
    m_matInertiaTensor.m01 += ( m_fMass * vCross.Z );
    m_matInertiaTensor.m02 += ( m_fMass * vCross.Y );
    m_matInertiaTensor.m10 = m_matInertiaTensor.m01;
    m_matInertiaTensor.m11 -= ( m_fMass * ( vDot.Z + vDot.X ) );
    m_matInertiaTensor.m12 += ( m_fMass * vCross.X );
    m_matInertiaTensor.m20 = m_matInertiaTensor.m02;
    m_matInertiaTensor.m21 = m_matInertiaTensor.m12;
    m_matInertiaTensor.m22 -= ( m_fMass * ( vDot.X + vDot.Y ) );

    m_fInvMass = MathFn->Invert( m_fMass );
    m_matInertiaTensor.Invert( m_matInvInertiaTensor );

    // Physical properties
    m_fRestitution = 0.0f;
    m_fFriction = 1.0f;
    m_fRollingFriction = 0.0f;

    // Damping
    m_fLinearDamping = BODY_DAMPING_LINEAR_DEFAULT;
    m_fAngularDamping = BODY_DAMPING_ANGULAR_DEFAULT;

    // Force buffers
    m_vForceBuffer = Vector3::Null;
    m_vTorqueBuffer = Vector3::Null;

    // Impulse buffers
    m_vLinearImpulseBuffer = Vector3::Null;
    m_vAngularImpulseBuffer = Vector3::Null;

    // Correction buffers
    m_vLinearCorrectionBuffer = Vector3::Null;
    m_vAngularCorrectionBuffer = Vector3::Null;

    // State
    m_vPosition = vPosition;
    m_qOrientation = qOrientation;
    m_qOrientation.Normalize();

    m_vLinearSpeed = vLinearSpeed;
    m_vAngularSpeed = vAngularSpeed;

    Matrix3 matOrient;
    m_qOrientation.GetMatrix( matOrient );
    m_vTransform.SetRotate( matOrient );
    m_vTransform.SetUniformScale( 1.0f );
    m_vTransform.SetTranslate( m_vPosition - m_vCenterOfMass );

    // World shape
    PhysicsFn->SelectMemory( TEXT("Shapes") );
    if ( m_pLocalShape->IsConvex() )
        m_pWorldShape = New TransformedShapeConvex( (const ShapeConvex *)m_pLocalShape, m_vTransform );
    else
        m_pWorldShape = New TransformedShapeConcave( (const ShapeConcave *)m_pLocalShape, m_vTransform );
    PhysicsFn->UnSelectMemory();
}
BodyNode::~BodyNode()
{
    // World shape
    PhysicsFn->SelectMemory( TEXT("Shapes") );
    Delete( m_pWorldShape );
    PhysicsFn->UnSelectMemory();
}

Bool BodyNode::ShouldCollide( const BodyNode * pA, const BodyNode * pB )
{
    // Behaviour checking
    if ( pA->IsInsulator() && pB->IsInsulator() )
        return false;

    // Joint checking
    // Check all joints between A & B and see if they prevent collision ...
    ///////////////////////////////////

    // Collision filtering
    if ( (pA->m_iCollisionMask & pB->m_iCollisionGroup) == 0 )
        return false;
    if ( (pB->m_iCollisionMask & pA->m_iCollisionGroup) == 0 )
        return false;

    // Accept
    return true;
}

Bool BodyNode::CheckSleepers( BodyNode * pA, BodyNode * pB )
{
    Bool bSleepyA = false;
    if ( pA->IsInsulator() || pA->m_bSleeping )
        bSleepyA = true;

    Bool bSleepyB = false;
    if ( pB->IsInsulator() || pB->m_bSleeping )
        bSleepyB = true;

    if ( bSleepyA && bSleepyB )
        return true;

    pA->SetSleeping( false );
    pB->SetSleeping( false );

    return false;
}

