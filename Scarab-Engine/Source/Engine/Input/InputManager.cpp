/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Input/InputManager.cpp
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
// Includes
#include "InputManager.h"

/////////////////////////////////////////////////////////////////////////////////
// InputManager implementation
InputManager::InputManager():
    Manager<InputManager>()
{
    // Initialize MUST be called explicitly
    // nothing to do
}
InputManager::~InputManager()
{
    // Cleanup MUST be called explicitly
    // nothing to do
}

Void InputManager::Initialize()
{
    // default state
    m_AsyncKeyboard.NumLock = true;
    m_AsyncKeyboard.CapsLock = false;
    m_AsyncKeyboard.ScrollLock = false;
    m_AsyncKeyboard.LastKey = KEYCODE_NULL;
    MemZero( m_AsyncKeyboard.Keys, KEYBOARD_KEYS );
    m_AsyncMouse.X = 0;
    m_AsyncMouse.Y = 0;
    m_AsyncMouse.DX = 0;
    m_AsyncMouse.DY = 0;
    m_AsyncMouse.DWheel = 0;
    m_AsyncMouse.LastButton = KEYCODE_NULL;
    MemZero( m_AsyncMouse.Buttons, MOUSE_BUTTONS );

    // Action Mapping
    for( UInt i = 0; i < INPUT_MAX_MAPS; ++i )
        m_arrActionMaps[i] = NULL;
    m_iMapCount = 0;
    m_iActiveMap = INVALID_OFFSET;

    // Build a default map ?
}
Void InputManager::Cleanup()
{
    SelectMemory( TEXT("Scratch") );

    // Action Mapping
    for( UInt i = 0; i < INPUT_MAX_MAPS; ++i ) {
        if ( m_arrActionMaps[i] != NULL ) {
            m_arrActionMaps[i]->Destroy();
            Delete( m_arrActionMaps[i] );
            m_arrActionMaps[i] = NULL;
        }
    }
    m_iMapCount = 0;
    m_iActiveMap = INVALID_OFFSET;

    UnSelectMemory();
}

UInt InputManager::CreateMap()
{
    Assert(m_iMapCount < INPUT_MAX_MAPS);

    UInt iMapID = m_iMapCount++;

    SelectMemory( TEXT("Scratch") );
    m_arrActionMaps[iMapID] = New _ActionMap();
    UnSelectMemory();

    m_arrActionMaps[iMapID]->SetComparator( _Compare_ActionKeys, NULL );
    m_arrActionMaps[iMapID]->UseMemoryContext( GetMemoryContext(), TEXT("Scratch") );
    m_arrActionMaps[iMapID]->Create();

	return iMapID;
}
Void InputManager::DestroyMap( UInt iMapID )
{
    Assert(iMapID < m_iMapCount);

    if ( m_arrActionMaps[iMapID] != NULL ) {
        m_arrActionMaps[iMapID]->Destroy();

        SelectMemory( TEXT("Scratch") );
        Delete( m_arrActionMaps[iMapID] );
        m_arrActionMaps[iMapID] = NULL;
        UnSelectMemory();
    }
    if ( iMapID < m_iMapCount - 1 ) {
        m_arrActionMaps[iMapID] = m_arrActionMaps[m_iMapCount - 1];
        m_arrActionMaps[m_iMapCount - 1] = NULL;
    } else
        m_arrActionMaps[iMapID] = NULL;
    --m_iMapCount;
}
Void InputManager::SelectMap( UInt iMapID )
{
    Assert(iMapID < m_iMapCount);
    Assert(m_arrActionMaps[iMapID] != NULL);

    m_iActiveMap = iMapID;
}

Void InputManager::BindAction( Action iAction, KeyCode iKey, ActionFlag iFlags, UInt iActionMap )
{
    if (iActionMap == INVALID_OFFSET)
		iActionMap = m_iActiveMap;
    Assert(iActionMap < INPUT_MAX_MAPS);
    Assert(m_arrActionMaps[iActionMap] != NULL);

    _ActionKey actKey;
	actKey.iKey = iKey;
	actKey.iFlags = iFlags;
    m_arrActionMaps[iActionMap]->Insert( actKey, iAction );
}
Action InputManager::GetAction( KeyCode iKey, ActionFlag iFlags ) const
{
    _ActionKey actKey;
    actKey.iKey = iKey; 
    actKey.iFlags = iFlags;
    _ActionMap::Iterator itMap = m_arrActionMaps[m_iActiveMap]->Get( actKey );
    if ( itMap.IsNull() )
        return ACTION_INVALID;
    return itMap.GetItem();
}

/////////////////////////////////////////////////////////////////////////////////

Void InputManager::_AsyncUpdateAfterEvent_Keyb( KeyboardEventType iEvent, KeyCode iKey )
{
    if ( iEvent == KEYBOARD_PRESS ) {
        if ( KEYCODE_SUBFAMILY(iKey) == KEYCODE_SUBFAMILY_LOCKS ) {
            // Treat locks
            if ( iKey == KEYCODE_NUMLOCK )
	            m_AsyncKeyboard.NumLock = !(m_AsyncKeyboard.NumLock);
            else if ( iKey == KEYCODE_CAPSLOCK )
	            m_AsyncKeyboard.CapsLock = !(m_AsyncKeyboard.CapsLock);
            else if ( iKey == KEYCODE_SCROLLLOCK )
	            m_AsyncKeyboard.ScrollLock = !(m_AsyncKeyboard.ScrollLock);
        }
        m_AsyncKeyboard.Keys[KEYCODE_INDEX(iKey)] = 1;
    } else // KEYBOARD_RELEASE
        m_AsyncKeyboard.Keys[KEYCODE_INDEX(iKey)] = 0;
    m_AsyncKeyboard.LastKey = iKey;
}
Void InputManager::_AsyncUpdateAfterEvent_Mous( MouseEventType iEvent, UInt iX, UInt iY, KeyCode iButton, Int iDeltaWheel )
{
    m_AsyncMouse.DX = ( (Int)(iX) - (Int)(m_AsyncMouse.X) );
    m_AsyncMouse.DY = ( (Int)(iY) - (Int)(m_AsyncMouse.Y) );
    m_AsyncMouse.X = iX;
    m_AsyncMouse.Y = iY;
    m_AsyncMouse.DWheel = 0; // allways reset wheel
    if ( iEvent == MOUSE_MOVE )
        return;
    if ( iEvent == MOUSE_WHEEL )
        m_AsyncMouse.DWheel = iDeltaWheel;
    else {
        if ( iEvent == MOUSE_PRESS )
            m_AsyncMouse.Buttons[KEYCODE_INDEX(iButton)] = 1;
        else // MOUSE_RELEASE or MOUSE_DBLCLIK
            m_AsyncMouse.Buttons[KEYCODE_INDEX(iButton)] = 0;
        m_AsyncMouse.LastButton = iButton;
    }
}

Int InputManager::_Compare_ActionKeys( const _ActionKey & rLeft, const _ActionKey & rRight, Void * /*pUserData*/ )
{
    if ( rLeft.iKey < rRight.iKey )
        return +1;
    if ( rLeft.iKey > rRight.iKey )
        return -1;
    if ( rLeft.iFlags < rRight.iFlags )
        return +1;
    if ( rLeft.iFlags > rRight.iFlags )
        return -1;
    return 0;
}
