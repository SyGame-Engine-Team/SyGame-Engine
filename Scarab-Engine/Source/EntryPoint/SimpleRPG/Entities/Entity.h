/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Entities/Entity.h
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
// Header prelude
#ifndef SCARAB_ENTRYPOINT_SIMPLERPG_ENTITIES_ENTITY_H
#define SCARAB_ENTRYPOINT_SIMPLERPG_ENTITIES_ENTITY_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Graphics/GraphicsManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

enum EntityType {
    ENTITYTYPE_UNDEFINED = 0,

    ENTITYTYPE_LANDSCAPE,
    ENTITYTYPE_WALL,
    ENTITYTYPE_SOLID,
    ENTITYTYPE_CHARACTER,
    ENTITYTYPE_BULLET,
    ENTITYTYPE_EFFECT,

    ENTITYTYPE_COUNT
};

/////////////////////////////////////////////////////////////////////////////////
// The SimpleEntity class
class SimpleEntity : public WorldLeaf
{
public:
    SimpleEntity( const GChar * strName );
    virtual ~SimpleEntity();

    // Getters
    virtual EntityType GetEntityType() const = 0;

    inline Vertex3 GetPosition() const;

    // Graphics
    Void SetupGraphics( const GChar * strGeometry, EffectID idEffect, Void ** arrParameters );
    Void RemoveGraphics();

    // Animation
    Void SetupDynamicAnimation( const GChar * strGeometry, const Vertex3 & vPosition, Bool bStatic );
    Void SetupKinematicAnimation( const GChar * strGeometry, const Vertex3 & vPosition, Bool bStatic );
    Void RemoveAnimation();

    // Methods
    inline Void SetVisible( Bool bVisible );

    inline Scalar GetDistance( SimpleEntity * pEntity ) const;
    Bool HasLineOfSight( SimpleEntity * pEntity ) const;
    const Array<SimpleEntity*> & GetEntitiesInRange( EntityType iType, Scalar fRange, Bool bSort ) const;

protected:
    // GetEntitiesInRange buffer
    mutable Array<SimpleEntity*> m_arrEntitiesInRange;
};

/////////////////////////////////////////////////////////////////////////////////
// The ComplexEntity class
class ComplexEntity : public WorldNode
{
public:
    ComplexEntity( const GChar * strName );
    virtual ~ComplexEntity();

protected:
    // Animation data
    // Skeleton, ParticleSystem, RagDoll, ...
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Entity.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_SIMPLERPG_ENTITIES_ENTITY_H


