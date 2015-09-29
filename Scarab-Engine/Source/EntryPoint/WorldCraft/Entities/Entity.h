/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/Entities/Entity.h
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
// Header prelude
#ifndef SCARAB_ENTRYPOINT_WORLDCRAFT_ENTITIES_ENTITY_H
#define SCARAB_ENTRYPOINT_WORLDCRAFT_ENTITIES_ENTITY_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Graphics/GraphicsManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

enum EntityType {
    ENTITYTYPE_UNDEFINED = 0,

    ENTITYTYPE_BLOCK,

    ENTITYTYPE_CHARACTER, // PCs, NPCs, Mobs, ...
    ENTITYTYPE_ITEM,      // Dropped items
    ENTITYTYPE_BULLET,    // Projectiles of all kinds
    ENTITYTYPE_EFFECT,    // Visual effects (particles, ...)

    ENTITYTYPE_COUNT
};

/////////////////////////////////////////////////////////////////////////////////
// The Entity class
class Entity : public WorldLeaf
{
public:
    Entity( const GChar * strName );
    virtual ~Entity();

    // Getters
    virtual EntityType GetEntityType() const = 0;

    inline const Vertex3 & GetPosition() const; // Center of BV

    // Graphics
    Void SetupGraphics( const GChar * strGeometryInstance, EffectID idEffect, Void ** arrEffectParameters );
    Void RemoveGraphics();

    // Animation
    Void SetupDynamicAnimation( const GChar * strGeometryInstance, const Vertex3 & vPosition, Bool bStatic );
    Void SetupKinematicAnimation( const GChar * strGeometryInstance, const Vertex3 & vPosition, Bool bStatic );
    Void RemoveAnimation();

    // Methods
    inline Void SetVisible( Bool bVisible );

    inline Scalar GetDistance( Entity * pEntity ) const;
    inline Scalar GetDistanceSqr( Entity * pEntity ) const;

    Bool HasLineOfSight( Entity * pEntity ) const;
    const Array<Entity*> & GetEntitiesInRange( EntityType iType, Scalar fRange, Bool bSort ) const;

protected:
    // GetEntitiesInRange buffer
    mutable Array<Entity*> m_arrEntitiesInRange;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Entity.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDCRAFT_ENTITIES_ENTITY_H


