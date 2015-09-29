/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/Entities/BulletEntities.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : WorldCraft, Entities : BulletEntities
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "BulletEntities.h"

#include "EntityManager.h"

/////////////////////////////////////////////////////////////////////////////////
// BulletEntity implementation
BulletEntity::BulletEntity( const GChar * strName ):
    Entity( strName )
{
    m_pController = NULL;
}
BulletEntity::~BulletEntity()
{
    ((KinematicBody*)m_pBody)->DetachMotionController();
    PhysicsFn->DestroyController( m_pController );
    m_pController = NULL;
}

/////////////////////////////////////////////////////////////////////////////////
// CastAura implementation
CastAura::CastAura( const GChar * strName, const Vertex3 & vPosition, Scalar fDuration ):
    BulletEntity( strName )
{
    // Graphic data
    Void * arrParameters[2] = {
        GraphicsFn->GetLight( TEXT("Light_Sun") ),
        GraphicsFn->GetMaterial( TEXT("Material_CastAura") )
        //GraphicsFn->GetTexture( TEXT("Texture_CastAura") )
    };
    SetupGraphics( TEXT("CastAura"), EFFECTID_DEFAULT, arrParameters );

    // Animation data
    SetupKinematicAnimation( TEXT("CastAura"), vPosition, true );

    m_pController = PhysicsFn->CreateKeyFrameController( TEXT("KeyFrameController"), 3, 0 );
    m_pController->Enabled = true;
    ((KinematicBody*)m_pBody)->AttachController( m_pController );

    Vertex3 vTargetPosition = vPosition;
    vTargetPosition.Z += 1.0f;

    m_pController->MaxTime = fDuration;

    m_pController->SetPositionTime( 0, 0.0f );
    m_pController->SetPosition( 0, vPosition );

    m_pController->SetPositionTime( 1, fDuration * 0.5f );
    m_pController->SetPosition( 1, vTargetPosition );

    m_pController->SetPositionTime( 2, fDuration );
    m_pController->SetPosition( 2, vPosition );
}
CastAura::~CastAura()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// FireBolt implementation
FireBolt::FireBolt( const GChar * strName, const Vertex3 & vPosition, const Vertex3 & vTarget ):
    BulletEntity( strName )
{
    // Graphic data
    Void * arrParameters[2] = {
        GraphicsFn->GetLight( TEXT("Light_Sun") ),
        GraphicsFn->GetMaterial( TEXT("Material_FireBolt") )
        //GraphicsFn->GetTexture( TEXT("Texture_FireBolt") )
    };
    SetupGraphics( TEXT("FireBolt"), EFFECTID_DEFAULT, arrParameters );

    // Animation data
    SetupKinematicAnimation( TEXT("FireBolt"), vPosition, true );

    m_pController = PhysicsFn->CreateKeyFrameController( TEXT("KeyFrameController"), 2, 2 );
    m_pController->Enabled = true;
    ((KinematicBody*)m_pBody)->AttachController( m_pController );

    Vector3 vAim = ( vTarget - vPosition );
    vAim.Normalize();
    Quaternion qOrientation;
    qOrientation.MakeRotate( Vector3::eI, vAim );

    m_pController->MaxTime = 0.5f;

    m_pController->SetPositionTime( 0, 0.0f );
    m_pController->SetPosition( 0, vPosition );
    m_pController->SetOrientationTime( 0, 0.0f );
    m_pController->SetOrientation( 0, qOrientation );

    m_pController->SetPositionTime( 1, 0.5f );
    m_pController->SetPosition( 1, vTarget );
    m_pController->SetOrientationTime( 1, 0.5f );
    m_pController->SetOrientation( 1, qOrientation );
}
FireBolt::~FireBolt()
{
    // nothing to do
}

Void FireBolt::UpdateTarget( const Vertex3 & vTarget )
{
    Vertex3 vPosition = GetPosition();
    Vector3 Aim = ( vTarget - vPosition );
    Aim.Normalize();
    Quaternion qOrientation;
    qOrientation.MakeRotate( Vector3::eI, Aim );

    m_pController->SetPositionTime( 1, 0.5f );
    m_pController->SetPosition( 1, vTarget );
    m_pController->SetOrientationTime( 1, 0.5f );
    m_pController->SetOrientation( 1, qOrientation );
}

/////////////////////////////////////////////////////////////////////////////////
// IceNova implementation
IceNova::IceNova( const GChar * strName, const Vertex3 & vPosition ):
    BulletEntity( strName )
{
    // Graphic data
    Void * arrParameters[2] = {
        GraphicsFn->GetLight( TEXT("Light_Sun") ),
        GraphicsFn->GetMaterial( TEXT("Material_IceNova") )
        //GraphicsFn->GetTexture( TEXT("Texture_IceNova") )
    };
    SetupGraphics( TEXT("IceNova"), EFFECTID_DEFAULT, arrParameters );

    // Animation data
    SetupKinematicAnimation( TEXT("IceNova"), vPosition, true );

    m_pController = PhysicsFn->CreateKeyFrameController( TEXT("KeyFrameController"), 2, 0 );
    m_pController->Enabled = true;
    ((KinematicBody*)m_pBody)->AttachController( m_pController );

    Vertex3 vUpPosition = vPosition;
    vUpPosition.Z += 1.0f;

    m_pController->MaxTime = 0.5f;

    m_pController->SetPositionTime( 0, 0.0f );
    m_pController->SetPosition( 0, vPosition );
    
    m_pController->SetPositionTime( 1, 0.5f );
    m_pController->SetPosition( 1, vUpPosition );
}
IceNova::~IceNova()
{
    // nothing to do
}
