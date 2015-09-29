/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Interface/CharacterView.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Interface : CharacterView
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
#ifndef SCARAB_ENTRYPOINT_SIMPLERPG_INTERFACE_CHARACTERVIEW_H
#define SCARAB_ENTRYPOINT_SIMPLERPG_INTERFACE_CHARACTERVIEW_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Gameplay/Character.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The CharacterViewModel class
class CharacterViewModel : public GUIWidgetModel
{
public:
    CharacterViewModel( BaseCharacter * pCharacter );
    virtual ~CharacterViewModel();

    // Model
    inline const GChar * GetName() const;
    inline CharClass GetClass() const;

    inline UInt GetLevel() const;
    inline UInt GetCurrentXP() const;
    inline UInt GetMaxXP() const;

    inline UInt GetCurrentHP() const;
    inline UInt GetMaxHP() const;
    inline UInt GetCurrentMP() const;
    inline UInt GetMaxMP() const;
    inline Scalar GetMovementSpeed() const;

    inline Scalar GetStat( CharStatistic iStat ) const;
    inline Scalar GetStatBonus( CharStatistic iStat ) const;

    // Controller
    Void OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags );

protected:
    BaseCharacter * m_pCharacter;
};

/////////////////////////////////////////////////////////////////////////////////
// The CharacterView class
class CharacterView : public GUIWidget
{
public:
    CharacterView( CharacterViewModel * pModel, GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName );
    virtual ~CharacterView();

protected:
    // Drawing interface
    virtual Void _Draw( const Rectangle2 & rectClient );
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "CharacterView.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_SIMPLERPG_INTERFACE_CHARACTERVIEW_H

