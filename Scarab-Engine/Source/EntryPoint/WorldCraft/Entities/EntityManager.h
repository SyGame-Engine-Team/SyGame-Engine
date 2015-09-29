/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/Entities/EntityManager.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : WorldCraft, Entities : EntityManager
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENTRYPOINT_WORLDCRAFT_ENTITIES_ENTITYMANAGER_H
#define SCARAB_ENTRYPOINT_WORLDCRAFT_ENTITIES_ENTITYMANAGER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Graphics/GraphicsManager.h"

#include "Entity.h"
#include "CharEntity.h"
#include "BulletEntities.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define EntityFn EntityManager::GetInstancePtr()

// Prototypes
class BaseCharacter;

/////////////////////////////////////////////////////////////////////////////////
// The EntityContactCallback class
class EntityContactCallback : public ContactCallback
{
public:
    EntityContactCallback();
    virtual ~EntityContactCallback();

    virtual Void ProximityEnter( BodyNode * pBodyA, BodyNode * pBodyB );
    virtual Void ProximityLeave( BodyNode * pBodyA, BodyNode * pBodyB );
    virtual Void ProximityPersist( BodyNode * pBodyA, BodyNode * pBodyB );

    virtual Void SensorTrigger( BodyNode * pBodyA, BodyNode * pBodyB );
    virtual Void SensorClear( BodyNode * pBodyA, BodyNode * pBodyB );

    virtual Void ContactSeparated( BodyNode * pBodyA, BodyNode * pBodyB );
    virtual Void ContactSpeculate( BodyNode * pBodyA, BodyNode * pBodyB );
    virtual Void ContactEnter( BodyNode * pBodyA, BodyNode * pBodyB );
    virtual Void ContactLeave( BodyNode * pBodyA, BodyNode * pBodyB );
    virtual Void ContactPersist( BodyNode * pBodyA, BodyNode * pBodyB );
};

/////////////////////////////////////////////////////////////////////////////////
// The EntityManager class
class EntityManager : public Manager<EntityManager>
{
public:
    inline static const GChar * GetName();
    inline static const GChar * GetMemoryBudget();

protected:
    friend class Manager<EntityManager>;
    EntityManager();
    virtual ~EntityManager();

public:
    Void Initialize();
    Void Cleanup();

    // Entities access
    inline Entity * GetEntity( const GChar * strName ) const;

    CharEntity * CreateCharEntity( BaseCharacter * pCharacter, const Vertex3 & vPosition );
    SelectionMarkerEntity * CreateSelectionMarker( BaseCharacter * pCharacter );

    CastAura * CreateCastAura( const GChar * strName, const Vertex3 & vPosition, Scalar fDuration );
    FireBolt * CreateFireBolt( const GChar * strName, const Vertex3 & vPosition, const Vertex3 & vTarget );
    IceNova * CreateIceNova( const GChar * strName, const Vertex3 & vPosition );

    Void DestroyEntity( const GChar * strName );

private:
    // Helpers
    inline static Int _Compare_Strings( const GChar * const & rLeft, const GChar * const & rRight, Void * pUserData );

    // Entities
    typedef TreeMap<const GChar*,Entity*> _EntityMap;
    _EntityMap m_hEntityMap;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "EntityManager.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDCRAFT_ENTITIES_ENTITYMANAGER_H


