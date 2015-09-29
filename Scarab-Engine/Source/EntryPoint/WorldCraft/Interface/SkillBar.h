/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Interface/SkillBar.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Interface : SkillBar
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
#ifndef SCARAB_ENTRYPOINT_SIMPLERPG_INTERFACE_SKILLBAR_H
#define SCARAB_ENTRYPOINT_SIMPLERPG_INTERFACE_SKILLBAR_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Gameplay/Character.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define HUD_SKILLBAR_SIZE 16

/////////////////////////////////////////////////////////////////////////////////
// The SkillBarModel class
class SkillBarModel : public GUIWidgetModel
{
public:
    SkillBarModel( BaseCharacter * pCharacter, UInt iRowCount = 1, UInt iSlotsPerRow = HUD_SKILLBAR_SIZE, UInt iSlotCount = HUD_SKILLBAR_SIZE );
    virtual ~SkillBarModel();

    // Model
    inline UInt GetRowCount() const;
    inline UInt GetSlotsPerRow() const;
    inline UInt GetSlotCount() const;

    inline SkillID GetSlot( UInt iSlot ) const;
    inline Void SetSlot( UInt iSlot, SkillID idSkill );

    inline Bool IsOnGlobalCooldown() const;
    inline Scalar GetGlobalCooldown() const;
    inline Bool IsOnCooldown( SkillID idSkill ) const;
    inline Scalar GetCooldown( SkillID idSkill ) const;

    inline Bool ShowKeyBindings() const;
    inline Void SetShowKeyBindings( Bool bShowKeyBindings );

    // Controller
    virtual Void OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags );

    virtual Void OnStartDrag( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags );
	virtual Void OnStopDrag( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags );

protected:
    // Helper
    UInt _PickSlot( const Point2 & ptLocalPos ) const;

    BaseCharacter * m_pCharacter;

    UInt m_iRowCount;
    UInt m_iSlotsPerRow;
    UInt m_iSlotCount;
    SkillID m_arrSlots[HUD_SKILLBAR_SIZE];

    Bool m_bShowKeyBindings;
};

/////////////////////////////////////////////////////////////////////////////////
// The SkillBar class
class SkillBar : public GUIWidget
{
public:
    SkillBar( SkillBarModel * pModel, GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName );
    virtual ~SkillBar();

protected:
    // Drawing interface
    virtual Void _Draw( const Rectangle2 & rectClient );
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "SkillBar.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_SIMPLERPG_INTERFACE_SKILLBAR_H

