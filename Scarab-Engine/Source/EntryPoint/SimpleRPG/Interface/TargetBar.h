/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Interface/TargetBar.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Interface : TargetBar
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
#ifndef SCARAB_ENTRYPOINT_SIMPLERPG_INTERFACE_TARGETBAR_H
#define SCARAB_ENTRYPOINT_SIMPLERPG_INTERFACE_TARGETBAR_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Gameplay/Character.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TargetBarModel class
class TargetBarModel : public GUIWidgetModel
{
public:
    TargetBarModel( BaseCharacter * pCharacter, Bool bUseFocus );
    virtual ~TargetBarModel();

    // Model
    inline Bool ShowName() const;
    inline Bool ShowClass() const;
    inline Bool ShowValues() const;
    inline Bool ShowPercentValues() const;
    inline Bool ShowStatusEffectBar() const;
    inline Bool ShowCastBar() const;
    inline Bool ShowTargetOfTargetBar() const;

    inline Void SetShowName( Bool bShowName );
    inline Void SetShowClass( Bool bShowClass );
    inline Void SetShowValues( Bool bShowValues, Bool bShowPercent );
    inline Void SetShowStatusEffectBar( Bool bShowStatusEffectBar );
    inline Void SetShowCastBar( Bool bShowCastBar );
    inline Void SetShowTargetOfTargetBar( Bool bShowTargetOfTargetBar );

    inline Scalar GetHPBarSize() const;
    inline Scalar GetMPBarSize() const;
    inline Scalar GetStatusEffectBarSize() const;
    inline Scalar GetCastBarSize() const;
    inline Scalar GetTargetOfTargetBarSize() const;

    inline Void SetHPBarSize( Scalar fHPBarSize );
    inline Void SetMPBarSize( Scalar fMPBarSize );
    inline Void SetStatusEffectBarSize( Scalar fStatusEffectBarSize );
    inline Void SetCastBarSize( Scalar fCastBarSize );
    inline Void SetTargetOfTargetBarSize( Scalar fTargetOfTargetBarSize );

    inline Bool HasTarget() const;
    inline const GChar * GetName() const;
    inline CharClass GetClass() const;
    inline UInt GetMaxHP() const;
    inline UInt GetCurrentHP() const;
    inline UInt GetMaxMP() const;
    inline UInt GetCurrentMP() const;

    inline UInt GetStatusEffectCount() const;
    //inline StatusEffectInstance * GetStatusEffect( UInt iStatusEffect ) const;

    inline SkillID GetCastingState( Scalar * outCastingTime ) const;

    inline Bool HasTargetOfTarget() const;
    inline const GChar * GetTargetOfTargetName() const;
    inline CharClass GetTargetOfTargetClass() const;
    inline UInt GetTargetOfTargetMaxHP() const;
    inline UInt GetTargetOfTargetCurrentHP() const;

    // Controller
    virtual Void OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags );

protected:
    BaseCharacter * m_pCharacter;
    Bool m_bUseFocus;

    Bool m_bShowName;
    Bool m_bShowClass;
    Bool m_bShowValues;
    Bool m_bShowPercent;
    Bool m_bShowStatusEffectBar;
    Bool m_bShowCastBar;
    Bool m_bShowTargetOfTargetBar;

    Scalar m_fHPBarSize;
    Scalar m_fMPBarSize;
    Scalar m_fStatusEffectBarSize;
    Scalar m_fCastBarSize;
    Scalar m_fTargetOfTargetBarSize;
};

/////////////////////////////////////////////////////////////////////////////////
// The TargetBar class
class TargetBar : public GUIWidget
{
public:
    TargetBar( TargetBarModel * pModel, GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName );
    virtual ~TargetBar();

protected:
    // Drawing interface
    virtual Void _Draw( const Rectangle2 & rectClient );
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "TargetBar.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_SIMPLERPG_INTERFACE_TARGETBAR_H

