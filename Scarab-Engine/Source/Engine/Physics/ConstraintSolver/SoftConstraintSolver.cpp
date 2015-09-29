/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/ConstraintSolver/SoftConstraintSolver.cpp
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
// Includes
#include "SoftConstraintSolver.h"

#include "../PhysicsManager.h"

/////////////////////////////////////////////////////////////////////////////////
// SoftConstraintSolver implementation
SoftConstraintSolver::SoftConstraintSolver():
    m_arrSoftLinkConstraints(), m_arrRigidSoftContactConstraints(),
    m_arrSoftSoftContactConstraints(), m_arrRigidSoftAnchorConstraints(),
    m_arrInvolvedBodies()
{
    m_arrSoftLinkConstraints.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("ConstraintSolver") );
    m_arrSoftLinkConstraints.Create();
    m_arrSoftLinkConstraints.SetShrinking( false );

    m_arrRigidSoftContactConstraints.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("ConstraintSolver") );
    m_arrRigidSoftContactConstraints.Create();
    m_arrRigidSoftContactConstraints.SetShrinking( false );

    m_arrSoftSoftContactConstraints.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("ConstraintSolver") );
    m_arrSoftSoftContactConstraints.Create();
    m_arrSoftSoftContactConstraints.SetShrinking( false );

    m_arrRigidSoftAnchorConstraints.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("ConstraintSolver") );
    m_arrRigidSoftAnchorConstraints.Create();
    m_arrRigidSoftAnchorConstraints.SetShrinking( false );

    m_arrInvolvedBodies.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("ConstraintSolver") );
    m_arrInvolvedBodies.Create();
    m_arrInvolvedBodies.SetShrinking( false );

    SetDefaultParameters();
}
SoftConstraintSolver::~SoftConstraintSolver()
{
    m_arrInvolvedBodies.Destroy();

    m_arrRigidSoftAnchorConstraints.Destroy();
    m_arrSoftSoftContactConstraints.Destroy();
    m_arrRigidSoftContactConstraints.Destroy();
    m_arrSoftLinkConstraints.Destroy();
}

Void SoftConstraintSolver::SetDefaultParameters()
{
    m_iSequenceLengthVelocities = 0;
    m_iSequenceLengthPositions = 0;
    m_iSequenceLengthDrift = 0;

    m_iMaxIterationsVelocities = 1;
    m_iMaxIterationsPositions  = 1;
    m_iMaxIterationsDrift      = 1;

    m_fBaumgarte = 1.0f;
    m_fDamping   = 0.0f;

    //m_fRigidHardness       = 1.0f;
    //m_fKineticHardness     = 0.1f;
    //m_fSoftHardness        = 1.0f;
    m_fAnchorHardness      = 0.7f;
    //m_fSoftRigidHardness   = 0.1f;
    //m_fSoftKineticHardness = 1.0f;
    //m_fSoftSoftHardness    = 0.5f;
    //m_fSoftRigidSplit      = 0.5f;
    //m_fSoftKineticSplit    = 0.5f;
    //m_fSoftSoftSplit       = 0.5f;

    SetSolver( SOFTCONSTRAINT_SOLVER_POSITIONS );
}

Void SoftConstraintSolver::PushSoftLinks( SoftBody * pSoftBody )
{
    SoftLinkData curConstraintData;

    pSoftBody->EnumLinks();
    SoftBody::Link * pLink = pSoftBody->EnumNextLink();
    while( pLink != NULL ) {
        curConstraintData.pLink = pLink;
        curConstraintData.vGradient = ( pLink->GetNode(1)->OldPosition - pLink->GetNode(0)->OldPosition );
        curConstraintData.fGradientFactor = MathFn->Invert( curConstraintData.vGradient.NormSqr() * pLink->LinearFactor );

        m_arrSoftLinkConstraints.Push( curConstraintData );

        pLink = pSoftBody->EnumNextLink();
    }

    // Also record soft bodies
    m_arrInvolvedBodies.Push( pSoftBody );
}
Void SoftConstraintSolver::PushSelfContacts( SoftBody * pSoftBody )
{
    const ShapeDeformable * pShape = (const ShapeDeformable *)( pSoftBody->GetLocalShape() );

    UInt iContactCount = pShape->GetSelfContactCount();
    const ContactPoint * pCP;

    SoftSoftContactData curConstraintData;

    for( UInt i = 0; i < iContactCount; ++i ) {
        pCP = pShape->GetSelfContact( i );

        curConstraintData.pFacet = (SoftBody::Facet*)( pCP->idCPID.dwFeatureA );
        curConstraintData.pNode = (SoftBody::Node*)( pCP->idCPID.dwFeatureB );

        curConstraintData.vNormal = pCP->vWorldNormalBA;
        //curConstraintData.vWeights = ; // Bary coords of closest point on facet ?
        //curConstraintData.fMargin = ; // distance ?

        //curConstraintData.fFriction = ; // SoftBody friction ?
        curConstraintData.fCFM[0] = 0.0f;
        curConstraintData.fCFM[1] = 0.0f;

        m_arrSoftSoftContactConstraints.Push( curConstraintData );
    }
}

Void SoftConstraintSolver::PushRigidSoftContacts( BodyNode * pBody, const ContactManifold * pManifold,
                                                  Scalar fTimeStep, Scalar fInvTimeStep )
{
    UInt iContactCount = pManifold->GetContactPointCount();
    const ContactPoint * pCP;

    RigidSoftContactData curConstraintData;
    curConstraintData.pBody = pBody;

    for( UInt i = 0; i < iContactCount; ++i ) {
        pCP = pManifold->GetContactPoint( i );

        // WARNING !!!
        // Find a way to retrieve pNode ...
        //curConstraintData.pNode = (SoftBody::Node*)( pCP->idCPID.dwSpecifierA );

        curConstraintData.vNormal = pCP->vWorldNormalBA;
        curConstraintData.fDistance = pCP->fDistance;

        //curConstraintData.fHardness = ; // SoftBody "restitution" ?
        //curConstraintData.fFriction = ; // SoftBody friction ?

        //curConstraintData.fInvMassDT = ( curConstraintData.pNode->InvMass * fTimeStep ); // mass from node ?
        //curConstraintData.vRelativeAnchor = ; // body's local closest point ?

        // Impulse matrix
        //curConstraintData.matImpulse = ; // same as anchor ?

        m_arrRigidSoftContactConstraints.Push( curConstraintData );
    }
}
Void SoftConstraintSolver::PushSoftSoftContacts( const ContactManifold * pManifold )
{
    UInt iContactCount = pManifold->GetContactPointCount();
    const ContactPoint * pCP;

    SoftSoftContactData curConstraintData;

    for( UInt i = 0; i < iContactCount; ++i ) {
        pCP = pManifold->GetContactPoint( i );

        // WARNING !!!
        // Self contacts are guaranteed, but Soft-Soft can be either ordering ...
        // We need either features to know their owning body (memory consuming)
        // or contact points to know ordering (less consuming) ...
        //curConstraintData.pFacet = (SoftBody::Facet*)( pCP->idCPID.dwSpecifierA );
        //curConstraintData.pNode = (SoftBody::Node*)( pCP->idCPID.dwSpecifierB );

        curConstraintData.vNormal = pCP->vWorldNormalBA;
        //curConstraintData.vWeights = ; // Bary coords of closest point on facet ?
        //curConstraintData.fMargin = ; // distance ?

        //curConstraintData.fFriction = ; // SoftBody friction ?
        curConstraintData.fCFM[0] = 0.0f;
        curConstraintData.fCFM[1] = 0.0f;

        m_arrSoftSoftContactConstraints.Push( curConstraintData );
    }
}

Void SoftConstraintSolver::PushRigidSoftAnchor( const JointEdge * pJointEdge, Scalar fTimeStep, Scalar fInvTimeStep )
{
    //const SoftAnchorEdge * pSoftAnchorEdge = (const SoftAnchorEdge *)pJointEdge;

    BodyNode * pBody = NULL; //pSoftAnchorEdge->GetAnchorBody();
    SoftBody::Node * pNode = NULL; //pSoftAnchorEdge->GetAnchorNode();

    // Prepare some values
    Scalar fBodyInvMass = pBody->GetInvMass();
    const Matrix3 & matBodyInvInertia = pBody->GetInvInertiaTensor();
    Scalar fNodeInvMass = pNode->InvMass;

    // Build constraint
    RigidSoftAnchorData curConstraintData;

    curConstraintData.pBody = pBody;
    curConstraintData.pNode = pNode;

    //curConstraintData.vLocalAnchor = pSoftAnchorEdge->GetLocalAnchor();

    curConstraintData.fInvMassDT = ( pNode->InvMass * fTimeStep );
    curConstraintData.vRelativeAnchor = ( pBody->GetTransform() * curConstraintData.vLocalAnchor );

    // Impulse matrix
    Matrix3 matMass, matTemp;

    matTemp.MakeSkewSymmetric( curConstraintData.vRelativeAnchor );
    matMass.MakeDiagonal( Vector3(fBodyInvMass, fBodyInvMass, fBodyInvMass) );
    matMass -= ( matTemp * matBodyInvInertia * matTemp );

    matTemp.MakeDiagonal( Vector3(fNodeInvMass, fNodeInvMass, fNodeInvMass) );
    matTemp += matMass;
    matTemp.Invert( matMass );

    matTemp.MakeDiagonal( Vector3(fInvTimeStep, fInvTimeStep, fInvTimeStep) );
    curConstraintData.matImpulse = ( matTemp * matMass );

    m_arrRigidSoftAnchorConstraints.Push( curConstraintData );
}

Void SoftConstraintSolver::ClearConstraints()
{
    m_arrSoftLinkConstraints.Clear();
    m_arrRigidSoftContactConstraints.Clear();
    m_arrSoftSoftContactConstraints.Clear();
    m_arrRigidSoftAnchorConstraints.Clear();

    m_arrInvolvedBodies.Clear();
}

Void SoftConstraintSolver::SetSolver( SoftConstraintSolverType iSolver )
{
    switch( iSolver ) {
        case SOFTCONSTRAINT_SOLVER_VELOCITIES:
            m_iSequenceLengthVelocities = 1;
            m_iSequenceLengthPositions = 3;
            m_iSequenceLengthDrift = 1;

            m_arrSolverSequenceVelocities[0] = SOFTCONSTRAINT_VSOLVER_LINKS;
            m_arrSolverSequencePositions[0] = SOFTCONSTRAINT_PSOLVER_ANCHORS;
            m_arrSolverSequencePositions[1] = SOFTCONSTRAINT_PSOLVER_CONTACTS_RIGID;
            m_arrSolverSequencePositions[2] = SOFTCONSTRAINT_PSOLVER_CONTACTS_SOFT;
            m_arrSolverSequenceDrift[0] = SOFTCONSTRAINT_PSOLVER_LINKS;
            break;
        case SOFTCONSTRAINT_SOLVER_POSITIONS:
            m_iSequenceLengthVelocities = 0;
            m_iSequenceLengthPositions = 4;
            m_iSequenceLengthDrift = 0;

            m_arrSolverSequencePositions[0] = SOFTCONSTRAINT_PSOLVER_ANCHORS;
            m_arrSolverSequencePositions[1] = SOFTCONSTRAINT_PSOLVER_CONTACTS_RIGID;
            m_arrSolverSequencePositions[2] = SOFTCONSTRAINT_PSOLVER_CONTACTS_SOFT;
            m_arrSolverSequencePositions[3] = SOFTCONSTRAINT_PSOLVER_LINKS;
            break;
        default: Assert(false); break;
    }
}

Void SoftConstraintSolver::SolveVelocities( Scalar fTimeStep )
{
    UInt iBodyCount = m_arrInvolvedBodies.Count();
    UInt i;

    SoftBody * pSoftBody;
    SoftBody::Node * pNode;

    if ( m_iMaxIterationsVelocities > 0 ) {
        for( i = 0; i < m_iMaxIterationsVelocities; ++i )
            _SolveVelocities();

        for( i = 0; i < iBodyCount; ++i ) {
            pSoftBody = m_arrInvolvedBodies[i];

            pSoftBody->EnumNodes();
            pNode = pSoftBody->EnumNextNode();
            while( pNode != NULL ) {
                pNode->Position = pNode->OldPosition + ( pNode->Velocity * fTimeStep );
                pNode = pSoftBody->EnumNextNode();
            }
        }
    }
}
Void SoftConstraintSolver::SolvePositions( Scalar fTimeStep, Scalar fInvTimeStep )
{
    UInt iBodyCount = m_arrInvolvedBodies.Count();
    UInt i;

    SoftBody * pSoftBody;
    SoftBody::Node * pNode;

    Scalar fT;

    // Solve positions
    if ( m_iMaxIterationsPositions > 0 ) {
        fT = ( (1.0f - m_fDamping) * fInvTimeStep );

        for( i = 0; i < m_iMaxIterationsPositions; ++i )
            _SolvePositions( fTimeStep );

        for( i = 0; i < iBodyCount; ++i ) {
            pSoftBody = m_arrInvolvedBodies[i];

            pSoftBody->EnumNodes();
            pNode = pSoftBody->EnumNextNode();
            while( pNode != NULL ) {
                pNode->Velocity = ( pNode->Position - pNode->OldPosition ) * fT;
                pNode = pSoftBody->EnumNextNode();
            }
        }
    }

    // Solve drift
    if ( m_iMaxIterationsDrift > 0 ) {
        fT = ( m_fBaumgarte * fInvTimeStep );

        for( i = 0; i < iBodyCount; ++i ) {
            pSoftBody = m_arrInvolvedBodies[i];

            pSoftBody->EnumNodes();
            pNode = pSoftBody->EnumNextNode();
            while( pNode != NULL ) {
                pNode->OldPosition = pNode->Position;
                pNode = pSoftBody->EnumNextNode();
            }
        }

        for( i = 0; i < m_iMaxIterationsDrift; ++i )
            _SolveDrift( fTimeStep );

        for( i = 0; i < iBodyCount; ++i ) {
            pSoftBody = m_arrInvolvedBodies[i];

            pSoftBody->EnumNodes();
            pNode = pSoftBody->EnumNextNode();
            while( pNode != NULL ) {
                pNode->Velocity += ( pNode->Position - pNode->OldPosition ) * fT;
                pNode = pSoftBody->EnumNextNode();
            }
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////

SoftConstraintSolver::VelocitySolver SoftConstraintSolver::_GetVelocitySolver( SoftConstraintVelocitySolverType iSolver )
{
    switch( iSolver ) {
        case SOFTCONSTRAINT_VSOLVER_LINKS: return _SolveVelocities_Links;
        default: Assert(false); break;
    }
    return NULL;
}
SoftConstraintSolver::PositionSolver SoftConstraintSolver::_GetPositionSolver( SoftConstraintPositionSolverType iSolver )
{
    switch( iSolver ) {
        case SOFTCONSTRAINT_PSOLVER_LINKS:          return _SolvePositions_Links;
        case SOFTCONSTRAINT_PSOLVER_CONTACTS_RIGID: return _SolvePositions_RigidContacts;
        case SOFTCONSTRAINT_PSOLVER_CONTACTS_SOFT:  return _SolvePositions_SoftContacts;
        case SOFTCONSTRAINT_PSOLVER_ANCHORS:        return _SolvePositions_Anchors;
        default: Assert(false); break;
    }
    return NULL;
}

Void SoftConstraintSolver::_SolveVelocities()
{
    VelocitySolver pfVelocitySolver;

    for( UInt iSeq = 0; iSeq < m_iSequenceLengthVelocities; ++iSeq ) {
        pfVelocitySolver = _GetVelocitySolver( m_arrSolverSequenceVelocities[iSeq] );
        pfVelocitySolver( this );
    }
}
Void SoftConstraintSolver::_SolvePositions( Scalar fTimeStep )
{
    PositionSolver pfPositionSolver;

    for( UInt iSeq = 0; iSeq < m_iSequenceLengthPositions; ++iSeq ) {
        pfPositionSolver = _GetPositionSolver( m_arrSolverSequencePositions[iSeq] );
        pfPositionSolver( this, fTimeStep );
    }
}
Void SoftConstraintSolver::_SolveDrift( Scalar fTimeStep )
{
    PositionSolver pfPositionSolver;

    for( UInt iSeq = 0; iSeq < m_iSequenceLengthDrift; ++iSeq ) {
        pfPositionSolver = _GetPositionSolver( m_arrSolverSequenceDrift[iSeq] );
        pfPositionSolver( this, fTimeStep );
    }
}

Void SoftConstraintSolver::_SolveVelocities_Links( SoftConstraintSolver * pThis )
{
    Array<SoftLinkData> & arrLinks = pThis->m_arrSoftLinkConstraints;
    UInt iLinkCount = arrLinks.Count();

    SoftLinkData * pLinkData;
    SoftBody::Node *pNodeA, *pNodeB;
    Vector3 vRelVelocity;
    Scalar fJ;

    for( UInt i = 0; i < iLinkCount; ++i ) {
        pLinkData = &( arrLinks[i] );
        pNodeA = pLinkData->pLink->GetNode( 0 );
        pNodeB = pLinkData->pLink->GetNode( 1 );

        vRelVelocity = ( pNodeA->Velocity - pNodeB->Velocity );
        fJ = -(pLinkData->vGradient * vRelVelocity) * pLinkData->fGradientFactor;

        pNodeA->Velocity += pLinkData->vGradient * (fJ * pNodeA->InvMass);
        pNodeB->Velocity -= pLinkData->vGradient * (fJ * pNodeB->InvMass);
    }
}

Void SoftConstraintSolver::_SolvePositions_Links( SoftConstraintSolver * pThis, Scalar fTimeStep )
{
    Array<SoftLinkData> & arrLinks = pThis->m_arrSoftLinkConstraints;
    UInt iLinkCount = arrLinks.Count();

    SoftLinkData * pLinkData;
    SoftBody::Node *pNodeA, *pNodeB;
    Vector3 vDelta;
    Scalar fSqrLength, fNum, fDen, fK;

    for( UInt i = 0; i < iLinkCount; ++i ) {
        pLinkData = &( arrLinks[i] );
        if ( pLinkData->pLink->LinearFactor > 0.0f ) {
            pNodeA = pLinkData->pLink->GetNode( 0 );
            pNodeB = pLinkData->pLink->GetNode( 1 );

            vDelta = ( pNodeB->Position - pNodeA->Position );
            fSqrLength = vDelta.NormSqr();
            fNum = ( pLinkData->pLink->SqrRestLength - fSqrLength );
            fDen = ( pLinkData->pLink->LinearFactor * (pLinkData->pLink->SqrRestLength + fSqrLength) );
            fK = ( fNum / fDen );

            pNodeA->Position -= vDelta * (fK * pNodeA->InvMass);
            pNodeB->Position += vDelta * (fK * pNodeB->InvMass);
        }
    }
}
Void SoftConstraintSolver::_SolvePositions_RigidContacts( SoftConstraintSolver * pThis, Scalar fTimeStep )
{
    Array<RigidSoftContactData> & arrContacts = pThis->m_arrRigidSoftContactConstraints;
    UInt iContactCount = arrContacts.Count();

    RigidSoftContactData * pContactData;
    BodyNode * pBody;
    SoftBody::Node * pNode;

    Vector3 vBodyVelocity, vNodeVelocity, vRelVelocity;
    Vector3 vFrictionImpulse, vImpulse;
    Scalar fDotN, fDotP;

    for( UInt i = 0; i < iContactCount; ++i ) {
        pContactData = &( arrContacts[i] );
        pBody = pContactData->pBody;
        pNode = pContactData->pNode;

        vBodyVelocity = pBody->GetLinearSpeedAt( pContactData->vRelativeAnchor ) * fTimeStep;
        vNodeVelocity = ( pNode->Position - pNode->OldPosition );
        vRelVelocity = ( vBodyVelocity - vNodeVelocity );

        fDotN = ( vRelVelocity * pContactData->vNormal );
        if ( fDotN <= SCALAR_ERROR ) {
            fDotP = ( pNode->Position.ToVector() * pContactData->vNormal ) + pContactData->fDistance;
            //fDotP = Min<Scalar>( fDotP, SHAPE_MARGIN );

            vFrictionImpulse = ( vRelVelocity - (pContactData->vNormal * fDotN) );
            vImpulse = pContactData->matImpulse * (
                vRelVelocity
                - ( vFrictionImpulse * pContactData->fFriction )
                + ( pContactData->vNormal * (fDotP * pContactData->fHardness) )
            );

            pBody->ApplyImpulse( -vImpulse, pContactData->vRelativeAnchor );
            pNode->Position += ( vImpulse * pContactData->fInvMassDT );
        }
    }
}
Void SoftConstraintSolver::_SolvePositions_SoftContacts( SoftConstraintSolver * pThis, Scalar fTimeStep )
{
    Array<SoftSoftContactData> & arrContacts = pThis->m_arrSoftSoftContactConstraints;
    UInt iContactCount = arrContacts.Count();

    SoftSoftContactData * pContactData;
    SoftBody::Facet * pFacet;
    SoftBody::Node * pNode;

    Vertex3 vBary, vOldBary;
    Vector3 vNormal, vRelVelocity, vCorrection;
    Scalar fDot, fCorrection;

    for( UInt i = 0; i < iContactCount; ++i ) {
        pContactData = &( arrContacts[i] );
        pFacet = pContactData->pFacet;
        pNode = pContactData->pNode;

        vNormal = pContactData->vNormal;
        vBary = pFacet->BaryEval( pContactData->vWeights, false );
        vOldBary = pFacet->BaryEval( pContactData->vWeights, true );
        vRelVelocity = ( (pNode->Position - pNode->OldPosition) - (vBary - vOldBary) );
        vCorrection = Vector3::Null;

        fDot = ( vRelVelocity * vNormal );
        if ( fDot < 0.0f ) {
            fCorrection = (
                ( vNormal * vBary.ToVector() )
                - ( vNormal * pNode->Position.ToVector() )
            ) + pContactData->fMargin;
            vCorrection += ( vNormal * fCorrection );
        }
        vCorrection -= ( vRelVelocity.ProjectToPlane(vNormal) * pContactData->fFriction );

        pFacet->GetNode(0)->Position -= vCorrection * ( pContactData->vWeights.X * pContactData->fCFM[0] );
        pFacet->GetNode(1)->Position -= vCorrection * ( pContactData->vWeights.Y * pContactData->fCFM[0] );
        pFacet->GetNode(2)->Position -= vCorrection * ( pContactData->vWeights.Z * pContactData->fCFM[0] );
        pNode->Position += ( vCorrection * pContactData->fCFM[1] );
    }
}
Void SoftConstraintSolver::_SolvePositions_Anchors( SoftConstraintSolver * pThis, Scalar fTimeStep )
{
    Array<RigidSoftAnchorData> & arrAnchors = pThis->m_arrRigidSoftAnchorConstraints;
    UInt iAnchorCount = arrAnchors.Count();

    RigidSoftAnchorData * pAnchorData;
    BodyNode * pBody;
    SoftBody::Node * pNode;

    Scalar fAnchorHardness = pThis->m_fAnchorHardness;

    Vertex3 vWorldAnchor;
    Vector3 vBodyVelocity, vNodeVelocity, vRelVelocity;
    Vector3 vImpulse;

    for( UInt i = 0; i < iAnchorCount; ++i ) {
        pAnchorData = &( arrAnchors[i] );
        pBody = pAnchorData->pBody;
        pNode = pAnchorData->pNode;

        const Transform3 & vWorld = pBody->GetTransform();

        vWorldAnchor = ( vWorld * Vertex3((Scalar*)(pAnchorData->vLocalAnchor)) );
        vBodyVelocity = pBody->GetLinearSpeedAt( pAnchorData->vRelativeAnchor ) * fTimeStep;
        vNodeVelocity = ( pNode->Position - pNode->OldPosition );
        vRelVelocity = ( vBodyVelocity - vNodeVelocity ) + ( (vWorldAnchor - pNode->Position) * fAnchorHardness );
        vImpulse = ( pAnchorData->matImpulse * vRelVelocity );

        pBody->ApplyImpulse( -vImpulse, pAnchorData->vRelativeAnchor );
        pNode->Position += ( vImpulse * pAnchorData->fInvMassDT );
    }
}



