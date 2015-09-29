/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Bodies/Systems/ParticleSystem.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : System : Particles
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
#include "ParticleSystem.h"

#include "../../PhysicsManager.h"

/////////////////////////////////////////////////////////////////////////////////
// ParticleSystem implementation
//ParticleSystem::ParticleSystem( Scalar fParticleSize ):
//    SystemBody(),
//    m_vParticleSphere( Vertex3::Null, fParticleSize ),
//    m_sParticleShape( m_vParticleSphere, 1, 4 ),
//    m_arrParticles(),
//    m_vLinearSpeed(), m_vAngularSpeed()
//{
//    m_vLinearSpeed = Vector3::Null;
//    m_vAngularSpeed = Vector3::Null;
//
//    m_iActiveParticleCount = 0;
//
//    m_arrParticles.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("Bodies") );
//    m_arrParticles.Create();
//}
//ParticleSystem::~ParticleSystem()
//{
//    PopAllParticles();
//
//    m_arrParticles.Destroy();
//}
//
//Void ParticleSystem::SetAllParticlesLinearSpeed( const Vector3 & vLinearSpeed )
//{
//    UInt iParticleCount = m_arrParticles.Count();
//    for( UInt i = 0; i < iParticleCount; ++i )
//        m_arrParticles[i].vLinearSpeed = vLinearSpeed;
//}
//Void ParticleSystem::SetAllParticlesAngularSpeed( const Vector3 & vAngularSpeed )
//{
//    UInt iParticleCount = m_arrParticles.Count();
//    for( UInt i = 0; i < iParticleCount; ++i )
//        m_arrParticles[i].vAngularSpeed = vAngularSpeed;
//}
//
//Void ParticleSystem::PushParticle( const Vertex3 & vPosition, const Quaternion & qOrientation )
//{
//    GChar strName[CONTROLLER_NAME_LENGTH];
//    StrFormat( strName, TEXT("ParticleController_%x_%d"), (UInt)this, m_arrParticles.Count() );
//
//    _Particle newParticle;
//    newParticle.vLinearSpeed = Vector3::Null;
//    newParticle.vAngularSpeed = Vector3::Null;
//
//    newParticle.pBody = PhysicsFn->CreateKinematicBody( true, &m_sParticleShape, 1.0f, vPosition, qOrientation );
//    newParticle.pController = PhysicsFn->CreateManualController( strName, vPosition, qOrientation,
//                                                                          Vector3::Null, Vector3::Null );
//
//    newParticle.pBody->AttachController( newParticle.pController );
//
//    m_arrParticles.Push( newParticle );
//}
//Void ParticleSystem::PopParticle()
//{
//    UInt iParticleCount = m_arrParticles.Count();
//    Assert( iParticleCount > 0 );
//
//    _Particle * pParticle = &( m_arrParticles[iParticleCount - 1] );
//
//    pParticle->pBody->DetachAllControllers();
//
//    PhysicsFn->DestroyController( pParticle->pController );
//    pParticle->pController = NULL;
//
//    PhysicsFn->DestroyBody( pParticle->pBody );
//    pParticle->pBody = NULL;
//
//    m_arrParticles.Pop( NULL, 1 );
//}
//Void ParticleSystem::PopAllParticles()
//{
//    while( m_arrParticles.Count() > 0 )
//        PopParticle();
//}
//
//Void ParticleSystem::GeneratePlane( const Plane & vPlane, const Vector3 & vAxisU, const Vector3 & vAxisV,
//                                    Scalar fExtentU, Scalar fExtentV, UInt iParticleCount )
//{
//    // Generate particles
//    const Vertex3 & vCenter = vPlane.Position;
//
//    Scalar fU, fV;
//    Vertex3 vPosition;
//
//    for( UInt i = 0; i < iParticleCount; ++i ) {
//        fU = _RandScalar_M1_1() * fExtentU;
//        fV = _RandScalar_M1_1() * fExtentV;
//
//        vPosition = ( vCenter + ( vAxisU * fU ) + ( vAxisV * fV ) );
//        PushParticle( vPosition );
//    }
//}
//Void ParticleSystem::GenerateCircle( const Circle3 & vCircle, UInt iParticleCount )
//{
//    // Generate particles
//    const Vertex3 & vCenter = vCircle.Center;
//    Scalar fRadius = vCircle.Radius;
//    Scalar fSqrRadius = ( fRadius * fRadius );
//
//    Vector3 vAxisU, vAxisV;
//    Vector3::MakeComplementBasis( vAxisU, vAxisV, vCircle.Axis );
//
//    Scalar fU, fV;
//    Vertex3 vPosition;
//    vPosition.Z = vCenter.Z;
//
//    for( UInt i = 0; i < iParticleCount; ++i ) {
//        fU = _RandScalar_M1_1() * fRadius;
//        fV = _RandScalar_M1_1() * fRadius;
//
//        // Fail & Retry if we're not in the circle
//        if ( (fU*fU + fV*fV) > fSqrRadius ) {
//            ++iParticleCount;
//            continue;
//        }
//
//        vPosition = ( vCenter + ( vAxisU * fU ) + ( vAxisV * fV ) );
//        PushParticle( vPosition );
//    }
//}
//
//Void ParticleSystem::GenerateBox( const Box & vBox, UInt iParticleCount )
//{
//    // Generate particles
//    const Vertex3 & vCenter = vBox.Center;
//    const Vector3 & vExtents = vBox.Extents;
//
//    Vector3 vRand;
//    Vertex3 vPosition;
//
//    for( UInt i = 0; i < iParticleCount; ++i ) {
//        vRand.X = _RandScalar_M1_1() * vExtents.X;
//        vRand.Y = _RandScalar_M1_1() * vExtents.Y;
//        vRand.Z = _RandScalar_M1_1() * vExtents.Z;
//
//        vPosition = ( vCenter + vRand );
//        PushParticle( vPosition );
//    }
//}
//Void ParticleSystem::GenerateSphere( const Sphere & vSphere, UInt iParticleCount )
//{
//    // Generate particles
//    const Vertex3 & vCenter = vSphere.Center;
//    Scalar fRadius = vSphere.Radius;
//    Scalar fSqrRadius = ( fRadius * fRadius );
//
//    Vector3 vRand;
//    Vertex3 vPosition;
//
//    for( UInt i = 0; i < iParticleCount; ++i ) {
//        vRand.X = _RandScalar_M1_1() * fRadius;
//        vRand.Y = _RandScalar_M1_1() * fRadius;
//        vRand.Z = _RandScalar_M1_1() * fRadius;
//
//        // Fail & Retry if we're not in the circle
//        if ( vRand.NormSqr() > fSqrRadius ) {
//            ++iParticleCount;
//            continue;
//        }
//
//        vPosition = ( vCenter + vRand );
//        PushParticle( vPosition );
//    }
//}
//
//Void ParticleSystem::GenerateLinearField( const Vector3 & vLinearAxis, Scalar fMinSpeed, Scalar fMaxSpeed, Scalar fMaxDeviation )
//{
//    // Generate linear speeds
//    UInt iParticleCount = m_arrParticles.Count();
//    _Particle * pParticle;
//
//    Vector3 vAxisU, vAxisV, vDeviation;
//    Bool bUseDeviation = ( fMaxDeviation > 0.0f );
//    if ( bUseDeviation )
//        Vector3::MakeComplementBasis( vAxisU, vAxisV, vLinearAxis );
//
//    Vector3 vEffectiveLinearAxis = vLinearAxis;
//
//    for( UInt i = 0; i < iParticleCount; ++i ) {
//        pParticle = &( m_arrParticles[i] );
//
//        if ( bUseDeviation ) {
//            vDeviation = ( vAxisU * _RandScalar_0_1() );
//            vDeviation += ( vAxisV * _RandScalar_0_1() );
//            vDeviation.Normalize();
//            vDeviation *= ( _RandScalar_0_1() * fMaxDeviation );
//
//            vEffectiveLinearAxis = ( vLinearAxis + vDeviation );
//            vEffectiveLinearAxis.Normalize();
//        }
//
//        if ( fMinSpeed == fMaxSpeed )
//            pParticle->vLinearSpeed = ( vEffectiveLinearAxis * fMinSpeed );
//        else
//            pParticle->vLinearSpeed = ( vEffectiveLinearAxis * _RandScalar_Range(fMinSpeed, fMaxSpeed) );
//    }
//}
//Void ParticleSystem::GenerateAngularField( const Vector3 & vAngularAxis, Scalar fMinSpeed, Scalar fMaxSpeed, Scalar fMaxDeviation )
//{
//    // Generate linear speeds
//    UInt iParticleCount = m_arrParticles.Count();
//    _Particle * pParticle;
//
//    Vector3 vAxisU, vAxisV, vDeviation;
//    Bool bUseDeviation = ( fMaxDeviation > 0.0f );
//    if ( bUseDeviation )
//        Vector3::MakeComplementBasis( vAxisU, vAxisV, vAngularAxis );
//
//    Vector3 vEffectiveAngularAxis = vAngularAxis;
//
//    for( UInt i = 0; i < iParticleCount; ++i ) {
//        pParticle = &( m_arrParticles[i] );
//
//        if ( bUseDeviation ) {
//            vDeviation = ( vAxisU * _RandScalar_0_1() );
//            vDeviation += ( vAxisV * _RandScalar_0_1() );
//            vDeviation.Normalize();
//            vDeviation *= ( _RandScalar_0_1() * fMaxDeviation );
//
//            vEffectiveAngularAxis = ( vAngularAxis + vDeviation );
//            vEffectiveAngularAxis.Normalize();
//        }
//
//        if ( fMinSpeed == fMaxSpeed )
//            pParticle->vAngularSpeed = ( vEffectiveAngularAxis * fMinSpeed );
//        else
//            pParticle->vAngularSpeed = ( vEffectiveAngularAxis * _RandScalar_Range(fMinSpeed, fMaxSpeed) );
//    }
//}
//
//Void ParticleSystem::Update( Scalar fTimeStep )
//{
//    ManualController * pController;
//
//    for( UInt i = 0; i < m_iActiveParticleCount; ++i ) {
//        pController = m_arrParticles[i].pController;
//
//        pController->SetManualSpeed (
//            m_vLinearSpeed + m_arrParticles[i].vLinearSpeed,
//            m_vAngularSpeed + m_arrParticles[i].vAngularSpeed
//        );
//
//        pController->Translate( fTimeStep );
//        pController->Rotate( fTimeStep );
//    }
//}
//
///////////////////////////////////////////////////////////////////////////////////
//
//ParticleSystem::_Particle::_Particle():
//    vLinearSpeed(), vAngularSpeed()
//{
//    pBody = NULL;
//    pController = NULL;
//    vLinearSpeed = Vector3::Null;
//    vAngularSpeed = Vector3::Null;
//}
//ParticleSystem::_Particle::_Particle( const _Particle & rhs ):
//    vLinearSpeed( rhs.vLinearSpeed ), vAngularSpeed( rhs.vAngularSpeed )
//{
//    pBody = rhs.pBody;
//    pController = rhs.pController;
//}
//ParticleSystem::_Particle::~_Particle()
//{
//    // nothing to do
//}

