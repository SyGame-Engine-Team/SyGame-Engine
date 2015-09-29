/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Tools/GUIEvent.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Messaging support for the GUI system.
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
#ifndef SCARAB_ENGINE_GUI_TOOLS_GUIEVENT_H
#define SCARAB_ENGINE_GUI_TOOLS_GUIEVENT_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Error/ErrorManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define GUIEVENT_MAX_PARAMETERS      16
#define GUIEVENT_MAX_PARAMETERS_SIZE 64

typedef UInt GUIEventFlag;
#define GUIEVENT_FLAG_CTRL       0x01
#define GUIEVENT_FLAG_ALT        0x02
#define GUIEVENT_FLAG_SHIFT      0x04
#define GUIEVENT_FLAG_NUMLOCK    0x08
#define GUIEVENT_FLAG_CAPSLOCK   0x10
#define GUIEVENT_FLAG_SCROLLLOCK 0x20

enum GUIEventType
{
	GUIEVENT_NOP = 0,

	GUIEVENT_KEYPRESS,
	GUIEVENT_KEYRELEASE,

    GUIEVENT_MOUSEPRESS,
	GUIEVENT_MOUSERELEASE,

	GUIEVENT_CLICK,
	GUIEVENT_DBLCLICK,

    GUIEVENT_SCROLLING,

	GUIEVENT_ROLLIN,
	GUIEVENT_ROLLOUT,
    GUIEVENT_ROLLOVER,

	GUIEVENT_STARTDRAG,
	GUIEVENT_DRAGGING_LEFT,
	GUIEVENT_DRAGGING_RIGHT,
	GUIEVENT_DRAGGING_BOTH,
	GUIEVENT_STOPDRAG,

    GUIEVENT_FOCUSGAIN,
	GUIEVENT_FOCUSLOSS,

	GUIEVENT_RESIZE,
	GUIEVENT_EXIT
};

/////////////////////////////////////////////////////////////////////////////////
// The GUIEvent class
class GUIEvent
{
public:
	GUIEvent();
	GUIEvent( GUIEventType iType, GUIEventFlag iFlags );
    GUIEvent( const GUIEvent & rhs );
	~GUIEvent();

    GUIEvent & operator=( const GUIEvent & rhs );

	inline GUIEventType Type() const;
	inline GUIEventFlag Flags() const;
	inline const Byte * Parameter( UInt iParameter ) const;
	inline UInt ParamCount() const;

	Void PushParameter( const Byte * pParameter, UInt iSize );

private:
	GUIEventType m_iType;
	GUIEventFlag m_iFlags;

    UInt m_iParamTotalSize;
    UInt m_iParamCount;
    Byte m_arrParameters[GUIEVENT_MAX_PARAMETERS_SIZE];
	UInt m_arrParamOffsets[GUIEVENT_MAX_PARAMETERS];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GUIEvent.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_GUI_TOOLS_GUIEVENT_H
