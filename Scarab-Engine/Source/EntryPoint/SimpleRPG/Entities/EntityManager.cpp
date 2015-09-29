/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Entities/EntityManager.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Entities : EntityManager
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
#include "EntityManager.h"

#include "../Gameplay/GameplayManager.h"

/////////////////////////////////////////////////////////////////////////////////
// EntityContactCallback implementation
EntityContactCallback::EntityContactCallback():
    ContactCallback()
{
    // nothing to do
}
EntityContactCallback::~EntityContactCallback()
{
    // nothing to do
}

Void EntityContactCallback::ProximityEnter( BodyNode * /*pBodyA*/, BodyNode * /*pBodyB*/ )
{
    // nothing to do
}
Void EntityContactCallback::ProximityLeave( BodyNode * /*pBodyA*/, BodyNode * /*pBodyB*/ )
{
    // nothing to do
}
Void EntityContactCallback::ProximityPersist( BodyNode * /*pBodyA*/, BodyNode * /*pBodyB*/ )
{
    // nothing to do
}

Void EntityContactCallback::SensorTrigger( BodyNode * /*pBodyA*/, BodyNode * /*pBodyB*/ )
{
    // nothing to do
}
Void EntityContactCallback::SensorClear( BodyNode * /*pBodyA*/, BodyNode * /*pBodyB*/ )
{
    // nothing to do
}

Void EntityContactCallback::ContactSeparated( BodyNode * /*pBodyA*/, BodyNode * /*pBodyB*/ )
{
    // nothing to do
}
Void EntityContactCallback::ContactSpeculate( BodyNode * /*pBodyA*/, BodyNode * /*pBodyB*/ )
{
    // nothing to do
}
Void EntityContactCallback::ContactEnter( BodyNode * pBodyA, BodyNode * pBodyB )
{
    SimpleEntity * pEntityA = (SimpleEntity*)(pBodyA->GetUserData());
    SimpleEntity * pEntityB = (SimpleEntity*)(pBodyB->GetUserData());

    switch(pEntityA->GetEntityType()) {
        case ENTITYTYPE_CHARACTER:
            if ( pEntityB->GetEntityType() == ENTITYTYPE_WALL ) {
                BaseCharacter * pCharacter = ((CharEntity*)pEntityA)->GetCharacter();
                pCharacter->SetStanding();
            }
            break;
        case ENTITYTYPE_WALL:
            if ( pEntityB->GetEntityType() == ENTITYTYPE_CHARACTER ) {
                BaseCharacter * pCharacter = ((CharEntity*)pEntityB)->GetCharacter();
                pCharacter->SetStanding();
            }
            break;
        default: Assert(false); break;
    }
}
Void EntityContactCallback::ContactLeave( BodyNode * pBodyA, BodyNode * pBodyB )
{
    ///////////////////////////////
}
Void EntityContactCallback::ContactPersist( BodyNode * pBodyA, BodyNode * pBodyB )
{
    SimpleEntity * pEntityA = (SimpleEntity*)(pBodyA->GetUserData());
    SimpleEntity * pEntityB = (SimpleEntity*)(pBodyB->GetUserData());

    switch(pEntityA->GetEntityType()) {
        case ENTITYTYPE_CHARACTER:
            if ( pEntityB->GetEntityType() == ENTITYTYPE_WALL ) {
                BaseCharacter * pCharacter = ((CharEntity*)pEntityA)->GetCharacter();
                pCharacter->SetStanding();
            }
            break;
        case ENTITYTYPE_WALL:
            if ( pEntityB->GetEntityType() == ENTITYTYPE_CHARACTER ) {
                BaseCharacter * pCharacter = ((CharEntity*)pEntityB)->GetCharacter();
                pCharacter->SetStanding();
            }
            break;
        default: Assert(false); break;
    }
}

/////////////////////////////////////////////////////////////////////////////////
// EntityManager implementation
EntityManager::EntityManager():
    Manager<EntityManager>()
{
    // nothing to do
}
EntityManager::~EntityManager()
{
    // nothing to do
}

Void EntityManager::Initialize()
{
    // Entity map
    m_hEntityMap.SetComparator( _Compare_Strings, NULL );
    m_hEntityMap.UseMemoryContext( GetMemoryContext(), TEXT("Scratch") );
    m_hEntityMap.Create();
}
Void EntityManager::Cleanup()
{
    SelectMemory( TEXT("Scratch") );

    // Entity map
    for( _EntityMap::Iterator itEntity = m_hEntityMap.Begin(); !(itEntity.IsNull()); ++itEntity )
        Delete( itEntity.GetItem() );
    m_hEntityMap.Destroy();

    UnSelectMemory();
}

CharEntity * EntityManager::CreateCharEntity( BaseCharacter * pCharacter, const Vertex3 & vPosition )
{
    SelectMemory( TEXT("Scratch") );
    CharEntity * pEntity = New CharEntity( pCharacter, vPosition );
    UnSelectMemory();

    WorldFn->AddChild( pEntity );

    Bool bInserted = m_hEntityMap.Insert( pEntity->GetName(), pEntity );
    Assert( bInserted );

    return pEntity;
}
SelectionMarkerEntity * EntityManager::CreateSelectionMarker( BaseCharacter * pCharacter )
{
    SelectMemory( TEXT("Scratch") );
    SelectionMarkerEntity * pEntity = New SelectionMarkerEntity( pCharacter );
    UnSelectMemory();

    WorldFn->AddChild( pEntity );

    Bool bInserted = m_hEntityMap.Insert( pEntity->GetName(), pEntity );
    Assert( bInserted );

    return pEntity;
}

CastAura * EntityManager::CreateCastAura( const GChar * strName, Scalar fDuration, const Vertex3 & vPosition )
{
    SelectMemory( TEXT("Scratch") );
    CastAura * pEntity = New CastAura( strName, fDuration, vPosition );
    UnSelectMemory();

    WorldFn->AddChild( pEntity );

    Bool bInserted = m_hEntityMap.Insert( pEntity->GetName(), pEntity );
    Assert( bInserted );

    return pEntity;
}
FireBoltEntity * EntityManager::CreateFireBolt( const GChar * strName, const Vertex3 & vPosition, const Vertex3 & vTarget )
{
    SelectMemory( TEXT("Scratch") );
    FireBoltEntity * pEntity = New FireBoltEntity( strName, vPosition, vTarget );
    UnSelectMemory();

    WorldFn->AddChild( pEntity );

    Bool bInserted = m_hEntityMap.Insert( pEntity->GetName(), pEntity );
    Assert( bInserted );

    return pEntity;
}
IceNovaEntity * EntityManager::CreateIceNova( const GChar * strName, const Vertex3 & vPosition )
{
    SelectMemory( TEXT("Scratch") );
    IceNovaEntity * pEntity = New IceNovaEntity( strName, vPosition );
    UnSelectMemory();

    WorldFn->AddChild( pEntity );

    Bool bInserted = m_hEntityMap.Insert( pEntity->GetName(), pEntity );
    Assert( bInserted );

    return pEntity;
}

Void EntityManager::DestroyEntity( const GChar * strName )
{
    SimpleEntity * pEntity = NULL;
    Bool bRemoved = m_hEntityMap.Remove( strName, &pEntity );
    Assert( bRemoved && pEntity != NULL );

    WorldFn->RemoveChild( pEntity->GetName() );

    SelectMemory( TEXT("Scratch") );
    Delete( pEntity );
    UnSelectMemory();
}

