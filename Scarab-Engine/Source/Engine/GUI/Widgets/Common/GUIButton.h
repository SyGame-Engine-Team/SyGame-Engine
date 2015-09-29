/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Widgets/Common/GUIButton.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Widget : Common controls, Button
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
#ifndef SCARAB_ENGINE_GUI_WIDGETS_COMMON_GUIBUTTON_H
#define SCARAB_ENGINE_GUI_WIDGETS_COMMON_GUIBUTTON_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../GUIWidget.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define GUIBUTTON_MAX_LENGTH 256

/////////////////////////////////////////////////////////////////////////////////
// The GUIButtonModel class
class GUIButtonModel : public GUIWidgetModel
{
public:
    GUIButtonModel( const GChar * strText );
    virtual ~GUIButtonModel();

    // Model
    inline Bool IsPressed() const;

    inline const GChar * GetText( UInt * outLength = NULL ) const;
    Void SetText( const GChar * strText );

    // Controller
    virtual Void OnMousePress( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags );
    virtual Void OnMouseRelease( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags );

    virtual Void OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags );

    virtual Void OnPress();
    virtual Void OnTextChange();

protected:
    Bool m_bPressed;

    UInt m_iTextLength;
    GChar m_strText[GUIBUTTON_MAX_LENGTH];
};

/////////////////////////////////////////////////////////////////////////////////
// The GUIButton class
class GUIButton : public GUIWidget
{
public:
    GUIButton( GUIButtonModel * pModel, GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName );
    virtual ~GUIButton();

    // Type
	inline virtual GUIWidgetType GetType() const;

protected:
    // Drawing interface
    virtual Void _Draw( const Rectangle2 & rectClient );
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GUIButton.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_GUI_WIDGETS_COMMON_GUIBUTTON_H

