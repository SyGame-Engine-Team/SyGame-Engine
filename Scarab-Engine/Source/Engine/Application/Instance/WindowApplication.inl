/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Application/Instance/WindowApplication.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Base class for windowed applications.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// WindowApplication implementation
inline const GChar * WindowApplication::GetWindowTitle() const {
    return m_strTitle;
}

inline Void WindowApplication::GetWindowRect( GPURectangle * outRectangle ) const {
    m_pWindow->GetWindowRect( (DeviceRectangle*)outRectangle );
}

inline Bool WindowApplication::IsActive() const {
    return m_pWindow->IsActive();
}
inline Bool WindowApplication::IsFullScreen() const {
    return m_pWindow->IsFullScreen();
}
inline Bool WindowApplication::IsMinimized() const {
    return m_pWindow->IsMinimized();
}
inline Bool WindowApplication::IsMaximized() const {
    return m_pWindow->IsMaximized();
}

inline UInt64 WindowApplication::GetFrameCount() const {
    return m_iFrameCount;
}
inline TimeMeasure WindowApplication::GetLastFrameTime() const {
    return m_fFrameElapsed;
}
inline TimeMeasure WindowApplication::GetAverageFrameTime() const {
    return m_fFrameAverageTime;
}
inline Scalar WindowApplication::GetFPS() const {
    return m_fFPS;
}
inline Scalar WindowApplication::GetAverageFPS() const {
    return m_fAverageFPS;
}

