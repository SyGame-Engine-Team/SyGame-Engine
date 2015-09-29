/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Entities/CharEntity.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Entities : CharEntity
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
#ifndef SCARAB_ENTRYPOINT_SIMPLERPG_ENTITIES_CHARENTITY_H
#define SCARAB_ENTRYPOINT_SIMPLERPG_ENTITIES_CHARENTITY_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Entity.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Prototypes
class BaseCharacter;

/////////////////////////////////////////////////////////////////////////////////
// The CharEntity class
class CharEntity : public SimpleEntity
{
public:
    CharEntity( BaseCharacter * pCharacter, const Vertex3 & vPosition );
    virtual ~CharEntity();

    // Getters
    inline virtual EntityType GetEntityType() const;

    inline BaseCharacter * GetCharacter() const;

    // Character movement
    inline CharacterController * GetController() const;

    inline Bool IsMovingForward() const;
    inline Bool IsMovingBackward() const;
    inline Bool IsMovingLeft() const;
    inline Bool IsMovingRight() const;
    inline Bool IsMovingUp() const;
    inline Bool IsMovingDown() const;
    inline Bool IsMoving() const;

    inline Void SetMovingForward( Bool bMoving );
    inline Void SetMovingBackward( Bool bMoving );
    inline Void SetMovingLeft( Bool bMoving );
    inline Void SetMovingRight( Bool bMoving );
    inline Void SetMovingUp( Bool bMoving );
    inline Void SetMovingDown( Bool bMoving );
    inline Void SetStanding();

    inline const Vector3 & GetMovementDirection() const;

    inline Void Teleport( const Vertex3 & vPosition, Bool bKeepInertia = false );

    // Path tracking movement
    inline PathController * GetPathController() const;

    inline BSplinePatch3 * GetLinearPath() const;
    inline BSplinePatch2 * GetLinearTravel() const;
    inline BSplinePatch3 * GetAngularPath() const;
    inline BSplinePatch2 * GetAngularTravel() const;

    inline Bool IsPathTracking() const;

    Void StartPathTracking( const Vertex3 & vTarget );
    Void EndPathTracking();
    Void UpdatePathTracking();

protected:
    BaseCharacter * m_pCharacter;
    CharacterController * m_pController;

    // Path tracking mode
    Void _PathTracking_Start( const Array<WorldPathWaypoint*> & arrPath );
    Void _PathTracking_End();
    Void _PathTracking_Update( const Array<WorldPathWaypoint*> & arrPath );

    Bool m_bPathTrackingMode;

    SearchInstanceID m_idPathTracking;
    UInt m_iLastPassedWaypoint;
    Vertex3 m_vPathTarget;

    BSplinePatch3 * m_pLinearPath;
    BSplinePatch2 * m_pLinearTravel;
    BSplinePatch3 * m_pAngularPath;
    BSplinePatch2 * m_pAngularTravel;

    PathController * m_pPathController;
};

/////////////////////////////////////////////////////////////////////////////////
// The SelectionMarkerEntity class
class SelectionMarkerEntity : public SimpleEntity
{
public:
    SelectionMarkerEntity( BaseCharacter * pCharacter );
    virtual ~SelectionMarkerEntity();

    // Getters
    inline virtual EntityType GetEntityType() const;

    inline BaseCharacter * GetCharacter() const;
    inline ManualController * GetController() const;

    // Methods
    Void TrackTarget( BaseCharacter * pTarget );

protected:
    BaseCharacter * m_pCharacter;
    ManualController * m_pController;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "CharEntity.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_SIMPLERPG_ENTITIES_CHARENTITY_H


