/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Input/InputManager.inl
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
// InputManager implementation
inline const GChar * InputManager::GetName() {
    return TEXT("InputManager");
}
inline const GChar * InputManager::GetMemoryBudget() {
    return TEXT("Heap:Scratch(262144);");
}

inline const KeyboardState & InputManager::GetKeyboardState() const {
    return m_AsyncKeyboard;
}
inline Bool InputManager::IsKeyUp( KeyCode iKey ) const {
    if ( KEYCODE_FAMILY(iKey) == KEYCODE_FAMILY_MOUSE )
        return false;
    return ( m_AsyncKeyboard.Keys[KEYCODE_INDEX(iKey)] == 0 );
}
inline Bool InputManager::IsKeyDown( KeyCode iKey ) const {
    if ( KEYCODE_FAMILY(iKey) == KEYCODE_FAMILY_MOUSE )
        return false;
    return ( m_AsyncKeyboard.Keys[KEYCODE_INDEX(iKey)] == 1 );
}
inline KeyCode InputManager::GetLastKey() const {
    return m_AsyncKeyboard.LastKey;
}
inline Bool InputManager::IsNumLock() const {
    return m_AsyncKeyboard.NumLock;
}
inline Bool InputManager::IsCapsLock() const {
    return m_AsyncKeyboard.CapsLock;
}
inline Bool InputManager::IsScrollLock() const {
    return m_AsyncKeyboard.ScrollLock;
}
inline Bool InputManager::IsShiftDown() const {
    return ( m_AsyncKeyboard.Keys[KEYCODE_INDEX(KEYCODE_SHIFT)] == 1 );
}
inline Bool InputManager::IsCtrlDown() const {
    return ( m_AsyncKeyboard.Keys[KEYCODE_INDEX(KEYCODE_CTRL)] == 1 );
}
inline Bool InputManager::IsAltDown() const {
    return ( m_AsyncKeyboard.Keys[KEYCODE_INDEX(KEYCODE_ALT)] == 1 );
}
inline GChar InputManager::ConvertKeyCode( KeyCode iKey, Bool bQWERTY ) const {
    return StringFn->ConvertKeyCode( iKey, ( m_AsyncKeyboard.Keys[KEYCODE_INDEX(KEYCODE_SHIFT)] == 1 ),
                                           ( m_AsyncKeyboard.Keys[KEYCODE_INDEX(KEYCODE_CTRL)] == 1 ),
                                           ( m_AsyncKeyboard.Keys[KEYCODE_INDEX(KEYCODE_ALT)] == 1 ),
                                           m_AsyncKeyboard.NumLock, m_AsyncKeyboard.CapsLock, bQWERTY );
}

inline const MouseState & InputManager::GetMouseState() const {
    return m_AsyncMouse;
}
inline Bool InputManager::IsButtonUp( KeyCode iKey ) const {
    if ( KEYCODE_FAMILY(iKey) != KEYCODE_FAMILY_MOUSE )
        return false;
    return ( m_AsyncMouse.Buttons[KEYCODE_INDEX(iKey)] == 0 );
}
inline Bool InputManager::IsButtonDown( KeyCode iKey ) const {
    if ( KEYCODE_FAMILY(iKey) != KEYCODE_FAMILY_MOUSE )
        return false;
    return ( m_AsyncMouse.Buttons[KEYCODE_INDEX(iKey)] == 1 );
}
inline KeyCode InputManager::GetLastButton() const {
    return m_AsyncMouse.LastButton;
}
inline Void InputManager::GetMouseXY( UInt & outX, UInt & outY ) const {
    outX = m_AsyncMouse.X;
    outY = m_AsyncMouse.Y;
}
inline Void InputManager::GetMouseDeltaXY( Int & outDX, Int & outDY ) const {
    outDX = m_AsyncMouse.DX;
    outDY = m_AsyncMouse.DY;
}
inline Void InputManager::GetMouseWheel( Int & outDWheel ) const {
    outDWheel = m_AsyncMouse.DWheel;
}

