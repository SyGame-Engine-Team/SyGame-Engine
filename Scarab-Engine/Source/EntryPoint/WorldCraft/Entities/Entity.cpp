/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/Entities/Entity.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : WorldCraft, Entities : Entity
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

/////////////////////////////////////////////////////////////////////////////////
// Entity implementation
Entity::Entity( const GChar * strName ):
    WorldLeaf( strName ), m_arrEntitiesInRange()
{
    m_arrEntitiesInRange.UseMemoryContext( EntityFn->GetMemoryContext(), TEXT("Scratch") );
    m_arrEntitiesInRange.Create();
}
Entity::~Entity()
{
    m_arrEntitiesInRange.Destroy();

    if ( m_pBody != NULL )
        RemoveAnimation();

    if ( m_pMesh != NULL )
        RemoveGraphics();
}

Void Entity::SetupGraphics( const GChar * strGeometry, EffectID idEffect, Void ** arrEffectParameters )
{
    Assert( m_pMesh == NULL && m_pEffectInstance == NULL );

    GChar strTmp[64];
    StringFn->Format( strTmp, TEXT("Geometry_%s"), strGeometry );

    const GeometryInstance * pGeometryInstance = GraphicsFn->GetGeometryInstance( strTmp );
    SetMesh( pGeometryInstance->pMesh );

    EffectInstance * pEffectInstance = GraphicsFn->CreateEffectInstance( idEffect, arrEffectParameters );
    SetEffectInstance( pEffectInstance );
}
Void Entity::RemoveGraphics()
{
    Assert( m_pMesh != NULL && m_pEffectInstance != NULL );

    GraphicsFn->DestroyEffectInstance( m_pEffectInstance );
    SetEffectInstance( NULL );

    SetMesh( NULL );
}

Void Entity::SetupDynamicAnimation( const GChar * strGeometry, const Vertex3 & vPosition, Bool bStatic )
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
Void Entity::SetupKinematicAnimation( const GChar * strGeometry, const Vertex3 & vPosition, Bool bStatic )
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
Void Entity::RemoveAnimation()
{
    Assert( m_pBody != NULL );

    PhysicsFn->DestroyBody( m_pBody );
    SetBody( NULL );
}

Bool Entity::HasLineOfSight( Entity * pEntity ) const
{
    Vertex3 vPosition = GetPosition();
    Vertex3 vTargetPosition = pEntity->GetPosition();
    Vector3 vW = ( vTargetPosition - vPosition );
    Scalar fDistance = vW.Normalize();

    Line3 vRay( vPosition, vW );
    WorldFn->RayCast( vRay, 0.0f, fDistance );

    UInt iCastResultCount = WorldFn->GetRayCastResultCount();
    for( UInt i = 0; i < iCastResultCount; ++i ) {
        Entity * pEntity = (Entity*)( WorldFn->GetRayCastResult(i)->pHit );
        if ( pEntity->GetEntityType() == ENTITYTYPE_BLOCK )
            return false;
    }
    return true;
}
const Array<Entity*> & Entity::GetEntitiesInRange( EntityType iType, Scalar fRange, Bool bSort ) const
{
    Vertex3 vPosition = GetPosition();

    Sphere vSphere( vPosition, fRange );
    WorldFn->SphereCast( vSphere );
    if ( bSort )
        WorldFn->SortSphereCastResults();

    m_arrEntitiesInRange.Clear();

    UInt iCastResultCount = WorldFn->GetSphereCastResultCount();
    for( UInt i = 0; i < iCastResultCount; ++i ) {
        Entity * pEntity = (Entity*)( WorldFn->GetSphereCastResult(i)->pHit );
        if ( pEntity != this ) {
            if ( pEntity->GetEntityType() == iType )
                m_arrEntitiesInRange.Push( pEntity );
        }
    }

    return m_arrEntitiesInRange;
}


