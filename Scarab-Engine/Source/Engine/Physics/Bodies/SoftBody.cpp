/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Bodies/SoftBody.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Body : Soft bodies
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
#include "SoftBody.h"

#include "../PhysicsManager.h"

/////////////////////////////////////////////////////////////////////////////////
// SoftBody::Node implementation
SoftBody::Node::Node( const MeshVertexKey & vertexKey ):
    ShapeDeformable::Node( vertexKey),
    OldPosition(), Velocity(), ForceBuffer()
{
    pMaterial = NULL;

    Mass = 0.0f;
    InvMass = 0.0f;

    OldPosition = Vertex3::Null;
    Velocity = Vector3::Null;
    ForceBuffer = Vector3::Null;
    
    IsAttached = false;
}
SoftBody::Node::~Node()
{
    // nothing to do
}

Void SoftBody::Node::ApplyClampedForce( const Vector3 & vForce, Scalar fTimeStep )
{
    Scalar fInvMassDT = ( InvMass * fTimeStep );
    Vector3 vNormalizedForce = ( vForce * fInvMassDT );

    if ( vNormalizedForce.NormSqr() > Velocity.NormSqr() ) {
        vNormalizedForce.Normalize();
        ForceBuffer -= ( Velocity.ProjectToNormal(vNormalizedForce) / fInvMassDT );
    } else
        ForceBuffer += vForce;
}

/////////////////////////////////////////////////////////////////////////////////
// SoftBody::Link implementation
SoftBody::Link::Link( const MeshEdgeKey & edgeKey ):
    ShapeDeformable::Link( edgeKey )
{
    pMaterial = NULL;

    RestLength = 0.0f;
    SqrRestLength = 0.0f;
    LinearFactor = 0.0f;

    IsBridge = false;
}
SoftBody::Link::~Link()
{
    // nothing to do
}

Void SoftBody::Link::UpdateConstants()
{
    RestLength = ( GetNode(1)->Position - GetNode(0)->Position ).Norm();
    SqrRestLength = ( RestLength * RestLength );
    LinearFactor = ( GetNode(0)->InvMass + GetNode(1)->InvMass ) / pMaterial->fLinearStiffness;
}

/////////////////////////////////////////////////////////////////////////////////
// SoftBody::Facet implementation
SoftBody::Facet::Facet( const MeshTriangleKey & triangleKey ):
    ShapeDeformable::Facet( triangleKey )
{
    // nothing to do
}
SoftBody::Facet::~Facet()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// SoftBody implementation
SoftBody::SoftBody( Bool bStatic, ShapeDeformable * pLocalShape, SoftBodyEnvironment * pEnvironment,
                    const Vertex3 & vPosition, const Quaternion & qOrientation,
                    const Vector3 & vLinearSpeed, const Vector3 & vAngularSpeed ):
    BodyNode( bStatic, pLocalShape, 1.0f, vPosition, qOrientation, vLinearSpeed, vAngularSpeed ),
    m_arrMaterials()
    //m_arrClusters(), m_hClusterTree(), m_arrClusterConnectivity()
{
    // SoftBody environment
    m_pEnvironment = pEnvironment;

    // Shape data
    m_pDeformableShape = (ShapeDeformable*)m_pLocalShape;

    // Physical properties
    m_iAeroDynamicsModel  = SOFTBODY_AERODYNAMICS_V_Point;
    m_fDrag               = 0.0f;
    m_fLift               = 0.0f;
    m_fPressure           = 0.0f;
    m_fVolumeConversation = 0.0f;
    //m_fDynamicFriction    = 0.2f;

    m_arrMaterials.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("SoftBodies") );
    m_arrMaterials.Create();

    // Pose-Matching
    m_hPose.arrDeltas.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("SoftBodies") );
    m_hPose.arrDeltas.Create();
    m_hPose.arrWeights.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("SoftBodies") );
    m_hPose.arrWeights.Create();

    m_hPose.bIsVolume      = false;
    m_hPose.bIsFrame       = false;
    m_hPose.fVolume        = 0.0f;
    m_hPose.matBaseScaling = Matrix3::Identity;
    m_hPose.vCenterOfMass  = Vertex3::Null;
    m_hPose.matRotation    = Matrix3::Identity;
    m_hPose.matScaling     = Matrix3::Identity;

    m_fPoseMatching = 0.0f;
    m_fMaxVolumeRatio = 1.0f;

    // Default material
    Material * pMaterial = PushMaterial();
    pMaterial->fLinearStiffness = 1.0f;
    pMaterial->fAngularStiffness = 1.0f;
    pMaterial->fAreaStiffness = 1.0f;
    pMaterial->fVolumeStiffness = 1.0f;
    pMaterial->dwFlags = 0;
}
SoftBody::~SoftBody()
{
    // Pose
    m_hPose.arrWeights.Destroy();
    m_hPose.arrDeltas.Destroy();

    // Physical properties
    UInt iMaterialCount = m_arrMaterials.Count();
    UInt i;

    PhysicsFn->SelectMemory( TEXT("SoftBodies") );
    for( i = 0; i < iMaterialCount; ++i ) {
        Delete( m_arrMaterials[i] );
        m_arrMaterials[i] = NULL;
    }
    PhysicsFn->UnSelectMemory();

    m_arrMaterials.Destroy();
}

SoftBody::Material * SoftBody::PushMaterial()
{
    PhysicsFn->SelectMemory( TEXT("SoftBodies") );
    Material * pMaterial = New Material();
    PhysicsFn->UnSelectMemory();

    pMaterial->fLinearStiffness = 0.0f;
    pMaterial->fAngularStiffness = 0.0f;
    pMaterial->fAreaStiffness = 0.0f;
    pMaterial->fVolumeStiffness = 0.0f;
    pMaterial->dwFlags = 0;

    m_arrMaterials.Push( pMaterial );

    return pMaterial;
}

Void SoftBody::IntegrateVelocity( Scalar fTimeStep )
{
    Node * pNode;

    // Apply external forces
    EnumNodes();
    pNode = EnumNextNode();
    while( pNode != NULL ) {
        if ( pNode->InvMass > 0.0f )
            pNode->ForceBuffer += m_vForceBuffer;

        pNode = EnumNextNode();
    }

    // Apply internal forces
    _ApplyInternalForces( fTimeStep );

    // Integrate velocities
    EnumNodes();
    pNode = EnumNextNode();
    while( pNode != NULL ) {
        pNode->Velocity += pNode->ForceBuffer * ( pNode->InvMass * fTimeStep );
        pNode->ForceBuffer = Vector3::Null;

        pNode = EnumNextNode();
    }
}
Void SoftBody::IntegratePosition( Scalar fTimeStep )
{
    Node * pNode;

    // Integrate positions
    EnumNodes();
    pNode = EnumNextNode();
    while( pNode != NULL ) {
        pNode->OldPosition = pNode->Position;
        pNode->Position += pNode->Velocity * fTimeStep;

        pNode = EnumNextNode();
    }

    // Pose Matching
    _UpdatePose();

    if ( m_hPose.bIsFrame && (m_fPoseMatching > 0.0f) ) {
        UInt iNodeIndex;
        Vertex3 vPosition;

        EnumNodes();
        pNode = EnumNextNode();
        while( pNode != NULL ) {
            if ( pNode->InvMass > 0.0f ) {
                iNodeIndex = pNode->GetIndex();
                vPosition = m_hPose.vCenterOfMass;
                vPosition += ( m_hPose.matRotation * m_hPose.arrDeltas[iNodeIndex] );
                pNode->Position *= ( 1.0f - m_fPoseMatching );
                pNode->Position += ( vPosition.ToVector() * m_fPoseMatching );
            }
            pNode = EnumNextNode();
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////

Scalar SoftBody::_ComputeVolume() const
{
    static const Scalar fInv6 = MathFn->Invert( 6.0f );

    if ( GetNodeCount() == 0 )
        return 0.0f;

    const Vertex3 & vOrigin = GetNode(0)->Position;

    Vector3 vCross;
    Scalar fVolume = 0.0f;

    EnumFacets();
    const Facet * pFacet = EnumNextFacet();
    while( pFacet != NULL ) {
        vCross = ( (pFacet->GetNode(1)->Position - vOrigin) ^ (pFacet->GetNode(2)->Position - vOrigin) );
        fVolume += ( (pFacet->GetNode(0)->Position - vOrigin) * vCross );
        pFacet = EnumNextFacet();
    }
    fVolume *= fInv6;

    return fVolume;
}
Scalar SoftBody::_ComputeMass() const
{
    Scalar fTotalMass = 0.0f;

    EnumNodes();
    const Node * pNode = EnumNextNode();
    while( pNode != NULL ) {
        fTotalMass += pNode->Mass;
        pNode = EnumNextNode();
    }

    return fTotalMass;
}
Vertex3 SoftBody::_ComputeCenterOfMass() const
{
    Vertex3 vCOM( Vertex3::Null );
    if ( m_hPose.bIsFrame ) {
        EnumNodes();
        const Node * pNode = EnumNextNode();
        while( pNode != NULL ) {
            vCOM += ( pNode->Position.ToVector() * m_hPose.arrWeights[pNode->GetIndex()] );
            pNode = EnumNextNode();
        }
    }
    return vCOM;
}

Void SoftBody::_EvaluateMedium( Medium * outMedium, const Vertex3 & vPosition )
{
    outMedium->vVelocity = Vector3::Null;
    outMedium->fPressure = 0.0f;
    outMedium->fDensity = m_pEnvironment->fAirDensity;
    if ( m_pEnvironment->fWaterDensity > 0.0f ) {
        Scalar fDepth = -( (vPosition.ToVector() * m_pEnvironment->vWaterNormal) + m_pEnvironment->fWaterOffset );
        if ( fDepth > 0.0f ) {
            Scalar fGravity = m_pEnvironment->vGravity.Norm();
            outMedium->fPressure = ( fDepth * m_pEnvironment->fWaterDensity * fGravity );
            outMedium->fDensity = m_pEnvironment->fWaterDensity;
        }
    }
}

Void SoftBody::_ApplyInternalForces( Scalar fTimeStep )
{
    Node * pNode;
    Facet * pFacet;

    Bool bLift = ( m_fLift > 0.0f );
    Bool bDrag = ( m_fDrag > 0.0f );
    Bool bPressure = ( m_fPressure != 0.0f );
    Bool bVolumeConversation = ( m_fVolumeConversation > 0.0f );

    Bool bAeroDynamics = ( bLift || bDrag );
    Bool bAeroDynamicsV = bAeroDynamics && ( m_iAeroDynamicsModel <= SOFTBODY_AERODYNAMICS_V_TwoSided );
    Bool bAeroDynamicsF = bAeroDynamics && ( m_iAeroDynamicsModel > SOFTBODY_AERODYNAMICS_V_TwoSided );

    Bool bUseMedium = bAeroDynamics;
    Bool bUseVolume = ( bPressure || bVolumeConversation );

    Scalar fVolume = 0.0f;
    Scalar fIVolumeTP = 0.0f;
    Scalar fDVolumeTV = 0.0f;

    Medium curMedium;

    if ( bUseVolume ) {
        fVolume = _ComputeVolume();
        fIVolumeTP = MathFn->Invert( fVolume ) * m_fPressure;
        fDVolumeTV = ( m_hPose.fVolume - fVolume ) * m_fVolumeConversation;
    }

    // Link forces (??!)
    //////////////////////////////////////////

    // Node forces
    Vector3 vRelVelocity, vNormal, vRelVelNormalized, vForce;
    Scalar fSqrRelVelocity, fDot, fAmplitude;

    EnumNodes();
    pNode = EnumNextNode();
    while( pNode != NULL ) {
        if ( pNode->InvMass == 0.0f ) {
            pNode = EnumNextNode();
            continue;
        }

        if ( bUseMedium ) {
            _EvaluateMedium( &curMedium, pNode->Position );
            if ( bAeroDynamicsV ) {
                vRelVelocity = ( pNode->Velocity - curMedium.vVelocity );
                fSqrRelVelocity = vRelVelocity.NormSqr();
                if ( fSqrRelVelocity > SCALAR_ERROR ) {
                    switch( m_iAeroDynamicsModel ) {
                        case SOFTBODY_AERODYNAMICS_V_Point:
                            vNormal = vRelVelocity;
                            vNormal.Normalize();
                            break;
                        case SOFTBODY_AERODYNAMICS_V_OneSided:
                            vNormal = pNode->Normal;
                            break;
                        case SOFTBODY_AERODYNAMICS_V_TwoSided:
                            vNormal = pNode->Normal;
                            vNormal *= ( (vRelVelocity*vNormal) < 0.0f ) ? -1.0f : +1.0f;
                            break;
                        default: Assert(false); break;
                    }
                    fDot = ( vRelVelocity * vNormal );
                    if ( fDot > 0.0f ) {
                        vRelVelNormalized = vRelVelocity;
                        vRelVelNormalized.Normalize();

                        vForce = Vector3::Null;
                        fAmplitude = ( pNode->Area * fDot * fSqrRelVelocity * 0.5f );
                        fAmplitude *= curMedium.fDensity;
                        vForce += ( vNormal * (-fAmplitude * m_fLift) );
                        vForce += ( vRelVelNormalized * (-fAmplitude * m_fDrag) );
                        pNode->ApplyClampedForce( vForce, fTimeStep );
                    }
                }
            }
        }
        if ( bPressure )
            pNode->ForceBuffer += ( pNode->Normal * (pNode->Area * fIVolumeTP) );
        if ( bVolumeConversation )
            pNode->ForceBuffer += ( pNode->Normal * (pNode->Area * fDVolumeTV) );

        pNode = EnumNextNode();
    }

    // Facet forces
    static const Scalar fInv3 = MathFn->Invert( 3.0f );

    Vector3 vFacetVelocity;
    Vertex3 vFacetPosition;
    Node *pA, *pB, *pC;

    EnumFacets();
    pFacet = EnumNextFacet();
    while( pFacet != NULL ) {
        if ( bAeroDynamicsF ) {
            pA = pFacet->GetNode( 0 );
            pB = pFacet->GetNode( 1 );
            pC = pFacet->GetNode( 2 );

            vFacetVelocity = ( pA->Velocity + pB->Velocity + pC->Velocity );
            vFacetVelocity *= fInv3;
            vFacetPosition = pA->Position;
            vFacetPosition += pB->Position.ToVector();
            vFacetPosition += pC->Position.ToVector();
            vFacetPosition *= fInv3;

            _EvaluateMedium( &curMedium, vFacetPosition );

            vRelVelocity = ( vFacetVelocity - curMedium.vVelocity );
            fSqrRelVelocity = vRelVelocity.NormSqr();
            if ( fSqrRelVelocity > SCALAR_ERROR ) {
                switch( m_iAeroDynamicsModel ) {
                    case SOFTBODY_AERODYNAMICS_F_OneSided:
                        vNormal = pFacet->Normal;
                        break;
                    case SOFTBODY_AERODYNAMICS_F_TwoSided:
                        vNormal = pFacet->Normal;
                        vNormal *= ( (vRelVelocity*vNormal) < 0.0f ) ? -1.0f : +1.0f;
                        break;
                    default: Assert(false); break;
                }
                fDot = ( vRelVelocity * vNormal );
                if ( fDot > 0.0f ) {
                    vRelVelNormalized = vRelVelocity;
                    vRelVelNormalized.Normalize();

                    vForce = Vector3::Null;
                    fAmplitude = ( pFacet->Area * fDot * fSqrRelVelocity );
                    fAmplitude *= curMedium.fDensity;
                    vForce += ( vNormal * (-fAmplitude * m_fLift) );
                    vForce += ( vRelVelNormalized * (-fAmplitude * m_fDrag) );
                    vForce *= fInv3;
                    pA->ApplyClampedForce( vForce, fTimeStep );
                    pB->ApplyClampedForce( vForce, fTimeStep );
                    pC->ApplyClampedForce( vForce, fTimeStep );
                }
            }
        }

        pFacet = EnumNextFacet();
    }
}

Void SoftBody::_SetupPose( Bool bIsVolume, Bool bIsFrame )
{
    const Node * pNode;
    UInt iNodeIndex;

    // Update links
    EnumLinks();
    Link * pLink = EnumNextLink();
    while( pLink != NULL ) {
        pLink->UpdateConstants();
        pLink = EnumNextLink();
    }

    // Compute total mass
    Scalar fTotalMass = _ComputeMass();
    Scalar fMassK = fTotalMass * 1000.0f * (Scalar)( GetNodeCount() );

    EnumNodes();
    pNode = EnumNextNode();
    while( pNode != NULL ) {
        if ( pNode->InvMass == 0.0f )
            fTotalMass += fMassK;
        pNode = EnumNextNode();
    }
    Scalar fInvTotalMass = MathFn->Invert( fTotalMass );

    // Flags
    m_hPose.bIsVolume = bIsVolume;
    m_hPose.bIsFrame = bIsFrame;

    // Weights
    m_hPose.arrWeights.Clear();

    EnumNodes();
    pNode = EnumNextNode();
    while( pNode != NULL ) {
        if ( pNode->InvMass > 0.0f )
            m_hPose.arrWeights.Push( pNode->Mass * fInvTotalMass );
        else
            m_hPose.arrWeights.Push( fMassK * fInvTotalMass );
        pNode = EnumNextNode();
    }

    // COM
    m_hPose.vCenterOfMass = _ComputeCenterOfMass();

    // Deltas
    m_hPose.arrDeltas.Clear();

    EnumNodes();
    pNode = EnumNextNode();
    while( pNode != NULL ) {
        m_hPose.arrDeltas.Push( pNode->Position - m_hPose.vCenterOfMass );
        pNode = EnumNextNode();
    }

    // Volume
    m_hPose.fVolume = ( (bIsVolume) ? _ComputeVolume() : 0.0f );

    // BaseScaling
    Matrix3 matBaseScaling;
    matBaseScaling.MakeNull();

    Vector3 vTmpRow;

    EnumNodes();
    pNode = EnumNextNode();
    while( pNode != NULL ) {
        iNodeIndex = pNode->GetIndex();
        const Vector3 & vDelta = m_hPose.arrDeltas[iNodeIndex];
        const Vector3 & vWeightedDelta = ( vDelta * m_hPose.arrWeights[iNodeIndex] );
        matBaseScaling.GetRow( vTmpRow, 0 );
        matBaseScaling.SetRow( 0, vTmpRow + (vDelta * vWeightedDelta.X) );
        matBaseScaling.GetRow( vTmpRow, 1 );
        matBaseScaling.SetRow( 1, vTmpRow + (vDelta * vWeightedDelta.Y) );
        matBaseScaling.GetRow( vTmpRow, 2 );
        matBaseScaling.SetRow( 2, vTmpRow + (vDelta * vWeightedDelta.Z) );
        pNode = EnumNextNode();
    }
    matBaseScaling.Invert( m_hPose.matBaseScaling );

    // Rotation & Scaling
    m_hPose.matRotation.MakeIdentity();
    m_hPose.matScaling.MakeIdentity();
}
Void SoftBody::_UpdatePose()
{
    if ( !(m_hPose.bIsFrame) )
        return;

    // Update COM
    m_hPose.vCenterOfMass = _ComputeCenterOfMass();

    // Update Rotation & Scaling
    const Node * pNode;
    UInt iNodeIndex;

    Vector3 vTmpRow, vWeigthedDelta;
    Matrix3 matRotScale;
    matRotScale.MakeNull();
    matRotScale.m00 = SCALAR_EPSILON;
    matRotScale.m11 = SCALAR_EPSILON * 2.0f;
    matRotScale.m22 = SCALAR_EPSILON * 3.0f;

    EnumNodes();
    pNode = EnumNextNode();
    while( pNode != NULL ) {
        iNodeIndex = pNode->GetIndex();
        const Vector3 & vDelta = m_hPose.arrDeltas[iNodeIndex];
        vWeigthedDelta = ( (pNode->Position - m_hPose.vCenterOfMass) * m_hPose.arrWeights[iNodeIndex] );
        matRotScale.GetRow( vTmpRow, 0 );
        matRotScale.SetRow( 0, vTmpRow + (vDelta * vWeigthedDelta.X) );
        matRotScale.GetRow( vTmpRow, 1 );
        matRotScale.SetRow( 1, vTmpRow + (vDelta * vWeigthedDelta.Y) );
        matRotScale.GetRow( vTmpRow, 2 );
        matRotScale.SetRow( 2, vTmpRow + (vDelta * vWeigthedDelta.Z) );
        pNode = EnumNextNode();
    }

    Matrix3 matRot, matScale;
    matRotScale.PolarDecomposition( matRot, matScale );

    m_hPose.matRotation = matRot;
    m_hPose.matScaling = ( m_hPose.matBaseScaling * matScale );
    if ( m_fMaxVolumeRatio > 1.0f ) {
        Scalar fInvDet = Clamp<Scalar>( MathFn->Invert( m_hPose.matScaling.Determinant() ),
                                        1.0f, m_fMaxVolumeRatio );
        m_hPose.matScaling *= fInvDet;
    }
}
