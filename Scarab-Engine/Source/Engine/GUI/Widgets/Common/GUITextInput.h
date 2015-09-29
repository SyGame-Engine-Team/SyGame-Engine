/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Widgets/Common/GUITextInput.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Widget : Common controls, Text Input (monoline)
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
#ifndef SCARAB_ENGINE_GUI_WIDGETS_COMMON_GUITEXTINPUT_H
#define SCARAB_ENGINE_GUI_WIDGETS_COMMON_GUITEXTINPUT_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../GUIWidget.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define GUITEXTINPUT_MAX_LENGTH 1024

/////////////////////////////////////////////////////////////////////////////////
// The GUITextInputModel class
class GUITextInputModel : public GUIWidgetModel
{
public:
    GUITextInputModel( const GChar * strText = NULL );
    virtual ~GUITextInputModel();

    // Model
    inline UInt GetDisplayStart() const;
    inline UInt GetDisplayEnd() const;
    inline UInt GetDisplayText( GChar * outDisplayText ) const;

    inline UInt GetCursor() const;
    inline Void SetCursor( UInt iCursorPos );
	Void CursorIncrease( UInt iAmount = 1 );
	Void CursorDecrease( UInt iAmount = 1 );

    inline Bool HasSelection() const;
	inline UInt GetSelectionStart() const;
	inline UInt GetSelectionEnd() const;
	inline UInt GetSelectionLength() const;
    Void SelectAll();
	Void UnselectAll();
	Void Select( UInt iStart, UInt iEnd );

	inline Bool IsInsertMode() const;
    Void SetInsertMode( Bool bInsertMode );
    Void ToggleInsertMode();

    inline UInt GetTextLength() const;
    inline const GChar * GetText() const;
    Void SetText( const GChar * strText );
    Void ClearText();
    Void PutChar( GChar ch );
	Void PutText( const GChar * strText, UInt iLength );
	Void DeleteText( UInt iLength, Bool bGoLeft );

    // Controller
    virtual Void OnKeyPress( KeyCode iKey, GUIEventFlag iFlags );

    virtual Void OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags );

    virtual Void OnResize();

    virtual Void OnTextChange();
    virtual Void OnCursorChange();
    virtual Void OnSelectionChange();

protected:
    Void _UpdateDisplayFromStart();

    UInt m_iDisplayStart;
    UInt m_iDisplayEnd; // included

    UInt m_iCursor;
    UInt m_iSelectionStart;
	UInt m_iSelectionEnd; // included

    Bool m_bInsertMode;

    UInt m_iLength;
    GChar m_strText[GUITEXTINPUT_MAX_LENGTH + 1];
};

/////////////////////////////////////////////////////////////////////////////////
// The GUITextInput class
class GUITextInput : public GUIWidget
{
public:
    GUITextInput( GUITextInputModel * pModel, GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName );
    virtual ~GUITextInput();

    // Type
	inline virtual GUIWidgetType GetType() const;

protected:
    // Drawing interface
    virtual Void _Draw( const Rectangle2 & rectClient );
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GUITextInput.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_GUI_WIDGETS_COMMON_GUITEXTINPUT_H

