/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SkyforgeAtlas/Entities.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Entities, main scene objects
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
#ifndef SCARAB_ENTRYPOINT_SKYFORGEATLAS_ENTITIES_H
#define SCARAB_ENTRYPOINT_SKYFORGEATLAS_ENTITIES_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Framework/Graphics/GraphicsManager.h"

#include "SkyGraph.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

    // prototypes
class EntityNode;
class EntityArea;

/////////////////////////////////////////////////////////////////////////////////
// The EntityCharacter class
class EntityCharacter : public WorldLeaf
{
public:
    EntityCharacter();
    virtual ~EntityCharacter();

    // Controller access
    inline CharacterController * GetController() const;

protected:
    // Entity data
    EffectLightMaterialInstance * m_pEffectCharacter;
    CharacterController * m_pController;
};

/////////////////////////////////////////////////////////////////////////////////
// The EntitySkyBox class
//class EntitySkyBox : public WorldLeaf
//{
//public:
//    EntitySkyBox();
//    virtual ~EntitySkyBox();
//
//    // Entity callbacks
//    Void OnCreateEffects();
//    Void OnDestroyEffects();
//
//    Void OnCreateWorld();
//    Void OnDestroyWorld();
//
//protected:
//    // Entity data
//    Material m_hMaterial;
//    EffectLightMaterialInstance * m_pEffectSkyBox;
//};

/////////////////////////////////////////////////////////////////////////////////
// The EntityArea class
class EntityArea : public WorldLeaf
{
public:
    EntityArea( SkyArea * pArea );
    virtual ~EntityArea();

    Void GenerateStructure( WorldNode * outAreaContainer );

    // Area access
    inline SkyArea * GetArea() const;

protected:
    // Helper methods
    static const GChar * _MakeName( SkyArea * pArea );

    // Entity data
    SkyArea * m_pArea; // Holders allows entity connectivity
    EffectLightMaterialInstance * m_pEffectArea;
};

/////////////////////////////////////////////////////////////////////////////////
// The EntityNode class
class EntityNode : public WorldLeaf
{
public:
    EntityNode( SkyNode * pNode );
    virtual ~EntityNode();

    // Node access
    inline SkyNode * GetNode() const;

protected:
    // Helper methods
    static const GChar * _MakeName( SkyNode * pNode );

    // Entity data
    SkyNode * m_pNode; // Holders allows entity connectivity
    EffectLightMaterialInstance * m_pEffectNode;
};

/////////////////////////////////////////////////////////////////////////////////
// The EntityNodeLinkKey class
class EntityNodeLinkKey
{
public:
    EntityNodeLinkKey();
    EntityNodeLinkKey( EntityNode * pA, EntityNode * pB );
    EntityNodeLinkKey( const EntityNodeLinkKey & rhs );
    ~EntityNodeLinkKey();

    inline static Int Compare( const EntityNodeLinkKey & rLeft, const EntityNodeLinkKey & rRight, Void * pUserData );

    inline EntityNodeLinkKey & operator=( const EntityNodeLinkKey & rhs );

    inline Bool operator==( const EntityNodeLinkKey & rhs ) const;
    inline Bool operator!=( const EntityNodeLinkKey & rhs ) const;

    EntityNode * pNodeA; // pNodeA < pNodeB
    EntityNode * pNodeB; // to ensure unicity
};

/////////////////////////////////////////////////////////////////////////////////
// The EntityNodeLink class
class EntityNodeLink : public WorldLeaf
{
public:
    EntityNodeLink( EntityNode * pNodeA, EntityNode * pNodeB );
    virtual ~EntityNodeLink();

    // Node access
    inline EntityNode * GetNodeA() const;
    inline EntityNode * GetNodeB() const;

protected:
    // Helper methods
    static const GChar * _MakeName( EntityNode * pNodeA, EntityNode * pNodeB );

    // Entity data
    EntityNode * m_pNodeA;
    EntityNode * m_pNodeB;
    EffectMaterialInstance * m_pEffectNodeLink;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Entities.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_SKYFORGEATLAS_ENTITIES_H


