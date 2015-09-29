/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Entities/Entity.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Entities : Entity
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
#include "Entity.h"

#include "EntityManager.h"
#include "../Gameplay/GameplayManager.h"

/////////////////////////////////////////////////////////////////////////////////
// SimpleEntity implementation
SimpleEntity::SimpleEntity( const GChar * strName ):
    WorldLeaf( strName ),
    m_arrEntitiesInRange()
{
    m_arrEntitiesInRange.UseMemoryContext( EntityFn->GetMemoryContext(), TEXT("Scratch") );
    m_arrEntitiesInRange.Create();
}
SimpleEntity::~SimpleEntity()
{
    m_arrEntitiesInRange.Destroy();

    if ( m_pBody != NULL )
        RemoveAnimation();

    if ( m_pMesh != NULL )
        RemoveGraphics();
}

Void SimpleEntity::SetupGraphics( const GChar * strGeometry, EffectID idEffect, Void ** arrParameters )
{
    Assert( m_pMesh == NULL && m_pEffectInstance == NULL );

    GChar strTmp[64];
    StringFn->Format( strTmp, TEXT("Geometry_%s"), strGeometry );

    const GeometryInstance * pGeometryInstance = GraphicsFn->GetGeometryInstance( strTmp );
    SetMesh( pGeometryInstance->pMesh );

    EffectInstance * pEffectInstance = GraphicsFn->CreateEffectInstance( idEffect, arrParameters );
    SetEffectInstance( pEffectInstance );
}
Void SimpleEntity::RemoveGraphics()
{
    Assert( m_pMesh != NULL && m_pEffectInstance != NULL );

    GraphicsFn->DestroyEffectInstance( m_pEffectInstance );
    SetEffectInstance( NULL );

    SetMesh( NULL );
}

Void SimpleEntity::SetupDynamicAnimation( const GChar * strGeometry, const Vertex3 & vPosition, Bool bStatic )
{
    Assert( m_pBody == NULL );

    GChar strTmp[64];
    StringFn->Format( strTmp, TEXT("Geometry_%s"), strGeometry );

    const GeometryInstance * pGeometryInstance = GraphicsFn->GetGeometryInstance( strTmp );

    RigidBody * pBody = PhysicsFn->CreateRigidBody( bStatic, pGeometryInstance->pShape, 1.0f, vPosition, Quaternion::Identity, Vector3::Null, Vector3::Null );
    pBody->SetRestitution( 0.0f );
    pBody->SetFriction( 0.0f );
    pBody->SetRollingFriction( 0.0f );

    SetBody( pBody );
}
Void SimpleEntity::SetupKinematicAnimation( const GChar * strGeometry, const Vertex3 & vPosition, Bool bStatic )
{
    Assert( m_pBody == NULL );

    GChar strTmp[64];
    StringFn->Format( strTmp, TEXT("Geometry_%s"), strGeometry );

    const GeometryInstance * pGeometryInstance = GraphicsFn->GetGeometryInstance( strTmp );

    KinematicBody * pBody = PhysicsFn->CreateKinematicBody( bStatic, pGeometryInstance->pShape, 1.0f, vPosition, Quaternion::Identity, Vector3::Null, Vector3::Null );
    pBody->SetRestitution( 0.0f );
    pBody->SetFriction( 0.0f );
    pBody->SetRollingFriction( 0.0f );

    SetBody( pBody );
}
Void SimpleEntity::RemoveAnimation()
{
    Assert( m_pBody != NULL );

    PhysicsFn->DestroyBody( m_pBody );
    SetBody( NULL );
}

Bool SimpleEntity::HasLineOfSight( SimpleEntity * pEntity ) const
{
    Vertex3 vPosition = GetPosition();
    Vertex3 vTargetPosition = pEntity->GetPosition();
    Vector3 vW = ( vTargetPosition - vPosition );
    Scalar fDistance = vW.Normalize();

    Line3 vRay( vPosition, vW );
    WorldFn->RayCast( vRay, 0.0f, fDistance );

    UInt iCastResultCount = WorldFn->GetRayCastResultCount();
    for( UInt i = 0; i < iCastResultCount; ++i ) {
        WorldLeaf * pLeaf = WorldFn->GetRayCastResult(i)->pHit;
        if ( pLeaf->GetParent()->GetType() == WORLDNODE_OUTDOOR_REGION )
            continue;
        SimpleEntity * pEntity = (SimpleEntity*)pLeaf;
        EntityType iType = pEntity->GetEntityType();
        if ( iType == ENTITYTYPE_LANDSCAPE || iType == ENTITYTYPE_WALL || iType == ENTITYTYPE_SOLID )
            return false;
    }
    return true;
}
const Array<SimpleEntity*> & SimpleEntity::GetEntitiesInRange( EntityType iType, Scalar fRange, Bool bSort ) const
{
    Vertex3 vPosition = GetPosition();

    Sphere vSphere( vPosition, fRange );
    WorldFn->SphereCast( vSphere );
    if ( bSort )
        WorldFn->SortSphereCastResults();

    m_arrEntitiesInRange.Clear();

    UInt iCastResultCount = WorldFn->GetSphereCastResultCount();
    for( UInt i = 0; i < iCastResultCount; ++i ) {
        WorldLeaf * pLeaf = WorldFn->GetSphereCastResult(i)->pHit;
        if ( pLeaf->GetParent()->GetType() == WORLDNODE_OUTDOOR_REGION )
            continue;

        SimpleEntity * pEntity = (SimpleEntity*)pLeaf;
        if ( pEntity != this ) {
            if ( pEntity->GetEntityType() == iType )
                m_arrEntitiesInRange.Push( pEntity );
        }
    }

    return m_arrEntitiesInRange;
}

/////////////////////////////////////////////////////////////////////////////////
// ComplexEntity implementation
ComplexEntity::ComplexEntity( const GChar * strName ):
    WorldNode( strName )
{
}
ComplexEntity::~ComplexEntity()
{
}


