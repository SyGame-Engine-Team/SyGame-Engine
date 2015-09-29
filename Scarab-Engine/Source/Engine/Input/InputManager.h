/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Input/InputManager.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Core-Class for any kind of HID I/O ...
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
#ifndef SCARAB_ENGINE_INPUT_INPUTMANAGER_H
#define SCARAB_ENGINE_INPUT_INPUTMANAGER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Lib/Object/Patterns/Manager.h"

#include "../../Lib/Datastruct/Table/TreeMap.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define InputFn InputManager::GetInstancePtr()

    // Generic asynchronous states
#define KEYBOARD_KEYS 256
#define MOUSE_BUTTONS 32

enum KeyboardEventType
{
    KEYBOARD_PRESS = 0,
    KEYBOARD_RELEASE
};
typedef struct _keyboard_state
{
    Bool NumLock, CapsLock, ScrollLock;
    KeyCode LastKey;
    Bool Keys[KEYBOARD_KEYS];
} KeyboardState;

enum MouseEventType
{
    MOUSE_PRESS = 0,
    MOUSE_RELEASE,
    MOUSE_DBLCLK,
    MOUSE_MOVE,
    MOUSE_WHEEL
};
typedef struct _mouse_state
{
    UInt X, Y;
    Int DX, DY, DWheel;
    KeyCode LastButton;
    Bool Buttons[MOUSE_BUTTONS];
} MouseState;

    // Action mapping
#define INPUT_MAX_MAPS 32

typedef UInt Action;
#define ACTION_INVALID INVALID_OFFSET

typedef UInt ActionFlag;
#define ACTIONFLAG_NONE            0x00000000
#define ACTIONFLAG_MOD_SHIFT       0x00000001
#define ACTIONFLAG_MOD_CTRL        0x00000002
#define ACTIONFLAG_MOD_ALT         0x00000004
#define ACTIONFLAG_MOUSE_DBLCLICK  0x00000008
#define ACTIONFLAG_MOUSE_DELTAX    0x00000010
#define ACTIONFLAG_MOUSE_DELTAY    0x00000020
#define ACTIONFLAG_MOUSE_WHEEL     0x00000040

    // Prototypes
class WindowApplication;

/////////////////////////////////////////////////////////////////////////////////
// The InputManager class
class InputManager : public Manager<InputManager>
{
public:
    inline static const GChar * GetName();
    inline static const GChar * GetMemoryBudget();

protected:
    friend class Manager<InputManager>;
    InputManager();
    virtual ~InputManager();

public:
    Void Initialize();
    Void Cleanup();

    // Asynchronous keyboard access
    inline const KeyboardState & GetKeyboardState() const;
    inline Bool IsKeyUp( KeyCode iKey ) const;
    inline Bool IsKeyDown( KeyCode iKey ) const;
    inline KeyCode GetLastKey() const;
    inline Bool IsNumLock() const;
    inline Bool IsCapsLock() const;
    inline Bool IsScrollLock() const;
    inline Bool IsShiftDown() const;
    inline Bool IsCtrlDown() const;
    inline Bool IsAltDown() const;
    inline GChar ConvertKeyCode( KeyCode iKey, Bool bQWERTY = false ) const;

    // Asynchonous mouse access
    inline const MouseState & GetMouseState() const;
    inline Bool IsButtonUp( KeyCode iKey ) const;
    inline Bool IsButtonDown( KeyCode iKey ) const;
    inline KeyCode GetLastButton() const;
    inline Void GetMouseXY( UInt & outX, UInt & outY ) const;
    inline Void GetMouseDeltaXY( Int & outDX, Int & outDY ) const;
    inline Void GetMouseWheel( Int & outDWheel ) const;

    // Action mapping
    UInt CreateMap();
	Void DestroyMap( UInt iMapID );
	Void SelectMap (UInt iMapID );

    Void BindAction( Action iAction, KeyCode iKey, ActionFlag iFlags = ACTIONFLAG_NONE, UInt iActionMap = INVALID_OFFSET );
    Action GetAction( KeyCode iKey, ActionFlag iFlags = ACTIONFLAG_NONE ) const;

private:
    // This is an hidden path to Input low level operations
    // needed at application level ...
    friend class WindowAppCallbacks;
    Void _AsyncUpdateAfterEvent_Keyb( KeyboardEventType iEvent, KeyCode iKey );
    Void _AsyncUpdateAfterEvent_Mous( MouseEventType iEvent, UInt iX, UInt iY, KeyCode iButton, Int iDeltaWheel );

    // Asynchronous states
    KeyboardState m_AsyncKeyboard;
    MouseState m_AsyncMouse;

    // Action mapping
    typedef struct _action_key {
	    KeyCode iKey;
	    ActionFlag iFlags;
    } _ActionKey;
    static Int _Compare_ActionKeys( const _ActionKey & rLeft, const _ActionKey & rRight, Void * pUserData );

    typedef TreeMap<_ActionKey,Action> _ActionMap;

    _ActionMap * m_arrActionMaps[INPUT_MAX_MAPS];
    UInt m_iMapCount, m_iActiveMap;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "InputManager.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_INPUT_INPUTMANAGER_H
