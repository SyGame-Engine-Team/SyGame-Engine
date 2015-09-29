/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Entities/SkillEntities.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Entities : SkillEntities
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
#include "SkillEntities.h"

#include "EntityManager.h"

/////////////////////////////////////////////////////////////////////////////////
// CastAura implementation
CastAura::CastAura( const GChar * strName, Scalar fDuration, const Vertex3 & vPosition ):
    SimpleEntity( strName )
{
    // Graphic data
    Void * arrParameters[2] = {
        GraphicsFn->GetLight( TEXT("Light_Sun") ),
        GraphicsFn->GetMaterial( TEXT("Material_CastAura") )
        //GraphicsFn->GetTexture( TEXT("Texture_CastAura") )
    };
    SetupGraphics( TEXT("CastAura"), EFFECTID_LIGHT_MATERIAL, arrParameters );

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
    ((KinematicBody*)m_pBody)->DetachMotionController();
    PhysicsFn->DestroyController( m_pController );
    m_pController = NULL;

    RemoveAnimation();

    RemoveGraphics();
}

/////////////////////////////////////////////////////////////////////////////////
// FireBoltEntity implementation
FireBoltEntity::FireBoltEntity( const GChar * strName, const Vertex3 & vPosition, const Vertex3 & vTarget ):
    SimpleEntity( strName )
{
    // Graphic data
    Void * arrParameters[2] = {
        GraphicsFn->GetLight( TEXT("Light_Sun") ),
        GraphicsFn->GetMaterial( TEXT("Material_FireBolt") )
        //GraphicsFn->GetTexture( TEXT("Texture_FireBolt") )
    };
    SetupGraphics( TEXT("FireBolt"), EFFECTID_LIGHT_MATERIAL, arrParameters );

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
FireBoltEntity::~FireBoltEntity()
{
    ((KinematicBody*)m_pBody)->DetachMotionController();
    PhysicsFn->DestroyController( m_pController );
    m_pController = NULL;

    RemoveAnimation();

    RemoveGraphics();
}

Void FireBoltEntity::UpdateTarget( const Vertex3 & vTarget )
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
// IceNovaEntity implementation
IceNovaEntity::IceNovaEntity( const GChar * strName, const Vertex3 & vPosition ):
    SimpleEntity( strName )
{
    // Graphic data
    Void * arrParameters[2] = {
        GraphicsFn->GetLight( TEXT("Light_Sun") ),
        GraphicsFn->GetMaterial( TEXT("Material_IceNova") )
        //GraphicsFn->GetTexture( TEXT("Texture_IceNova") )
    };
    SetupGraphics( TEXT("IceNova"), EFFECTID_LIGHT_MATERIAL, arrParameters );

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
IceNovaEntity::~IceNovaEntity()
{
    ((KinematicBody*)m_pBody)->DetachMotionController();
    PhysicsFn->DestroyController( m_pController );
    m_pController = NULL;

    RemoveAnimation();

    RemoveGraphics();
}
