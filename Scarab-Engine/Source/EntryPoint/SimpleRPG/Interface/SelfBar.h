/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Interface/SelfBar.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Interface : SelfBar
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
#ifndef SCARAB_ENTRYPOINT_SIMPLERPG_INTERFACE_SELFBAR_H
#define SCARAB_ENTRYPOINT_SIMPLERPG_INTERFACE_SELFBAR_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Gameplay/Character.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The SelfBarModel class
class SelfBarModel : public GUIWidgetModel
{
public:
    SelfBarModel( BaseCharacter * pCharacter );
    virtual ~SelfBarModel();

    // Model
    inline Bool ShowName() const;
    inline Bool ShowClass() const;
    inline Bool ShowValues() const;
    inline Bool ShowPercentValues() const;

    inline Void SetShowName( Bool bShowName );
    inline Void SetShowClass( Bool bShowClass );
    inline Void SetShowValues( Bool bShowValues, Bool bShowPercent );

    inline Scalar GetHPBarSize() const;
    inline Scalar GetMPBarSize() const;

    inline Void SetHPBarSize( Scalar fHPBarSize );
    inline Void SetMPBarSize( Scalar fMPBarSize );

    inline const GChar * GetName() const;
    inline CharClass GetClass() const;
    inline UInt GetMaxHP() const;
    inline UInt GetCurrentHP() const;
    inline UInt GetMaxMP() const;
    inline UInt GetCurrentMP() const;

    // Controller
    virtual Void OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags );

protected:
    BaseCharacter * m_pCharacter;

    Bool m_bShowName;
    Bool m_bShowClass;
    Bool m_bShowValues;
    Bool m_bShowPercent;

    Scalar m_fHPBarSize;
    Scalar m_fMPBarSize;
};

/////////////////////////////////////////////////////////////////////////////////
// The SelfBar class
class SelfBar : public GUIWidget
{
public:
    SelfBar( SelfBarModel * pModel, GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName );
    virtual ~SelfBar();

    // Getters
    inline virtual GUIWidgetType GetType() const;

protected:
    // Drawing interface
    virtual Void _Draw( const Rectangle2 & rectClient );
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "SelfBar.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_SIMPLERPG_INTERFACE_SELFBAR_H

