/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Widgets/Common/GUICheckBox.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Widget : Common controls, Check-Box (3-states)
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_GUI_WIDGETS_COMMON_GUICHECKBOX_H
#define SCARAB_ENGINE_GUI_WIDGETS_COMMON_GUICHECKBOX_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../GUIWidget.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
enum GUICheckBoxState
{
    GUICHECKBOX_CLEARED = 0,
    GUICHECKBOX_TICKED,
    GUICHECKBOX_NEUTRAL
};

/////////////////////////////////////////////////////////////////////////////////
// The GUICheckBoxModel class
class GUICheckBoxModel : public GUIWidgetModel
{
public:
    GUICheckBoxModel( Bool bUseThirdState = false );
    virtual ~GUICheckBoxModel();

    // Model
    inline Bool IsCleared() const;
    inline Bool IsTicked() const;
    inline Bool IsNeutral() const;

	Void SetCleared();
	Void SetTicked();
    Void SetNeutral();

    Void Toggle();

    // Controller
    virtual Void OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags );

    virtual Void OnStateChange();

protected:
    Bool m_bUseThirdState;
    GUICheckBoxState m_iState;
};

/////////////////////////////////////////////////////////////////////////////////
// The GUICheckBox class
class GUICheckBox : public GUIWidget
{
public:
    GUICheckBox( GUICheckBoxModel * pModel, GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName );
    virtual ~GUICheckBox();

    // Type
	inline virtual GUIWidgetType GetType() const;

protected:
    // Drawing interface
    virtual Void _Draw( const Rectangle2 & rectClient );
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GUICheckBox.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_GUI_WIDGETS_COMMON_GUICHECKBOX_H

