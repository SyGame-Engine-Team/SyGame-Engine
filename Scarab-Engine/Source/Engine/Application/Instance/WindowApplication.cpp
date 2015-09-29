/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Application/Instance/WindowApplication.cpp
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
// Includes
#include "WindowApplication.h"

#include "../ApplicationManager.h"

/////////////////////////////////////////////////////////////////////////////////
// WindowAppCallbacks implementation
WindowAppCallbacks::WindowAppCallbacks( Void * pUserData ):
    DeviceWindowCallbacks( pUserData )
{
    // nothing to do
}
WindowAppCallbacks::~WindowAppCallbacks()
{
    // nothing to do
}

Void WindowAppCallbacks::OnPause()
{
    WindowApplication * pWinApp = (WindowApplication*)m_pUserData;
    pWinApp->Pause( true, true );
}
Void WindowAppCallbacks::OnResume()
{
    WindowApplication * pWinApp = (WindowApplication*)m_pUserData;
    pWinApp->Pause( false, false );
}

Void WindowAppCallbacks::OnIdleTime()
{
    WindowApplication * pWinApp = (WindowApplication*)m_pUserData;
    pWinApp->_OnIdleTime();
}

Void WindowAppCallbacks::OnWindowMonitorChange( Void * pMonitor )
{
    WindowApplication * pWinApp = (WindowApplication*)m_pUserData;
    pWinApp->OnWinMonitorChange( pMonitor );
}
Void WindowAppCallbacks::OnWindowMove( UInt iX, UInt iY )
{
    WindowApplication * pWinApp = (WindowApplication*)m_pUserData;
    pWinApp->OnWinMove( iX, iY );
}
Void WindowAppCallbacks::OnWindowResize( UInt iWidth, UInt iHeight )
{
    WindowApplication * pWinApp = (WindowApplication*)m_pUserData;
    pWinApp->OnWinResize( iWidth, iHeight );
}

Void WindowAppCallbacks::OnWindowFocus()
{
    WindowApplication * pWinApp = (WindowApplication*)m_pUserData;
    pWinApp->OnWinFocus();
}

Void WindowAppCallbacks::OnWindowDraw()
{
    WindowApplication * pWinApp = (WindowApplication*)m_pUserData;
    pWinApp->_OnWindowDraw();
}

Bool WindowAppCallbacks::OnKeyPress( KeyCode iKey )
{
    WindowApplication * pWinApp = (WindowApplication*)m_pUserData;

    Bool bProcessed = pWinApp->OnKeyPress( iKey );

    InputFn->_AsyncUpdateAfterEvent_Keyb( KEYBOARD_PRESS, iKey );
    return bProcessed;
}
Bool WindowAppCallbacks::OnKeyRelease( KeyCode iKey )
{
    WindowApplication * pWinApp = (WindowApplication*)m_pUserData;

    Bool bProcessed = pWinApp->OnKeyRelease( iKey );

    InputFn->_AsyncUpdateAfterEvent_Keyb( KEYBOARD_RELEASE, iKey );
    return bProcessed;
}

Void WindowAppCallbacks::OnMouseMove( UInt iX, UInt iY )
{
    WindowApplication * pWinApp = (WindowApplication*)m_pUserData;

    pWinApp->OnMouseMove( iX, iY );

    InputFn->_AsyncUpdateAfterEvent_Mous( MOUSE_MOVE, iX, iY, KEYCODE_NULL, 0 );
}
Bool WindowAppCallbacks::OnMousePress( UInt iX, UInt iY, KeyCode iKey )
{
    WindowApplication * pWinApp = (WindowApplication*)m_pUserData;

    Bool bProcessed = pWinApp->OnMousePress( iX, iY, iKey );

    InputFn->_AsyncUpdateAfterEvent_Mous( MOUSE_PRESS, iX, iY, iKey, 0 );
    return bProcessed;
}
Bool WindowAppCallbacks::OnMouseRelease( UInt iX, UInt iY, KeyCode iKey )
{
    WindowApplication * pWinApp = (WindowApplication*)m_pUserData;

    Bool bProcessed = pWinApp->OnMouseRelease( iX, iY, iKey );

    InputFn->_AsyncUpdateAfterEvent_Mous( MOUSE_RELEASE, iX, iY, iKey, 0 );
    return bProcessed;
}
Bool WindowAppCallbacks::OnMouseDblClick( UInt iX, UInt iY, KeyCode iKey )
{
    WindowApplication * pWinApp = (WindowApplication*)m_pUserData;

    Bool bProcessed = pWinApp->OnMouseDblClick( iX, iY, iKey );

    InputFn->_AsyncUpdateAfterEvent_Mous( MOUSE_DBLCLK, iX, iY, iKey, 0 );
    return bProcessed;
}
Bool WindowAppCallbacks::OnMouseWheel( UInt iX, UInt iY, Int iWheelDelta )
{
    WindowApplication * pWinApp = (WindowApplication*)m_pUserData;

    Bool bProcessed = pWinApp->OnMouseWheel( iX, iY, iWheelDelta );

    InputFn->_AsyncUpdateAfterEvent_Mous( MOUSE_WHEEL, iX, iY, KEYCODE_NULL, iWheelDelta );
    return bProcessed;
}

/////////////////////////////////////////////////////////////////////////////////
// WindowApplication implementation
WindowApplication::WindowApplication( const GChar * strTitle, PixelFormat iPixelFormat, UInt iWidth, UInt iHeight, DWord dwClearColor ):
    BaseApplication(), m_devCallbacks()
{
    m_pWindow = NULL;
    m_devCallbacks.SetUserData( (Void*)this );

    m_strTitle = strTitle;
    m_iCreationFormat = iPixelFormat;
    m_iCreationWidth = iWidth;
    m_iCreationHeight = iHeight;
    m_dwCreationClearColor = dwClearColor;

    m_bTimePaused = false;
    m_iTimePauseStack = 0;
    m_bRenderPaused = false;
    m_iRenderPauseStack = 0;
    
    m_iFrameCount = 0;
    m_fFrameLast = 0.0;
    m_fFrameElapsed = 0.0;
    m_fFrameTotalTime = 0.0;
    m_fFrameAverageTime = 0.0;
    m_fFPS = 0.0f;
    m_fAverageFPS = 0.0f;
}
WindowApplication::~WindowApplication()
{
    // nothing to do
}

Int WindowApplication::Run( const CommandLine & cmdLine )
{
    WindowApplication * pAppInst = (WindowApplication*)( ApplicationFn->GetInstance() );
    return pAppInst->Main( cmdLine );
}
Int WindowApplication::Main( const CommandLine & /*cmdLine*/ )
{
    Bool bOk = false;

    // Before window creation
    bOk = OnPreCreate();
    if (!bOk)
        return -1;

    // Create Window
    m_pWindow = New DeviceWindow( m_strTitle, 0, 0, m_iCreationWidth, m_iCreationHeight, &m_devCallbacks );

    // Create & Initialize Managers
    ResourceManager::Create();
    ResourceFn->Initialize();
    PatchManager::Create();
    PatchFn->Initialize();

    RenderingManager::Create();
    RenderingFn->Initialize( m_pWindow );
    ModelingManager::Create();
    ModelingFn->Initialize();
    PhysicsManager::Create();
    PhysicsFn->Initialize();
    WorldManager::Create();
    WorldFn->Initialize();

    InputManager::Create();
    InputFn->Initialize();
    GUIManager::Create();
    GUIFn->Initialize();

    ConsoleManager::Create();
    ConsoleFn->Initialize();

    // Execute application
    Int iReturnCode = -1;
    if ( OnInitialize() ) {
        // Display first frame
        m_pWindow->SetVisible( true );
        OnFrameRender();

        // Main Loop
        m_pWindow->MessageLoop();

        // Cleanup App
        OnCleanup();

        // Application executed properly
        iReturnCode = 0;
    }

    // Cleanup & Destroy Managers
    ConsoleFn->Cleanup();
    ConsoleManager::Destroy();

    GUIFn->Cleanup();
    GUIManager::Destroy();
    InputFn->Cleanup();
    InputManager::Destroy();

    WorldFn->Cleanup();
    WorldManager::Destroy();
    PhysicsFn->Cleanup();
    PhysicsManager::Destroy();
    ModelingFn->Cleanup();
    ModelingManager::Destroy();
    RenderingFn->Cleanup();
    RenderingManager::Destroy();

    PatchFn->Cleanup();
    PatchManager::Destroy();
    ResourceFn->Cleanup();
    ResourceManager::Destroy();

    // Destroy Window
    Delete( m_pWindow );
    m_pWindow = NULL;

    return iReturnCode;
}

Void WindowApplication::Exit()
{
    OnExit();

    m_pWindow->Exit();
}

Void WindowApplication::Pause( Bool bPauseTime, Bool bPauseRender )
{
    if ( bPauseTime )
        ++m_iTimePauseStack;
    else if ( m_iTimePauseStack > 0 )
        --m_iTimePauseStack;
    m_bTimePaused = ( m_iTimePauseStack > 0 );

    if ( bPauseRender )
        ++m_iRenderPauseStack;
    else if ( m_iRenderPauseStack > 0 )
        --m_iRenderPauseStack;
    m_bRenderPaused = ( m_iRenderPauseStack > 0 );
}

Bool WindowApplication::OnPreCreate()
{
    // Stub-method, do nothing
    return true;
}
Bool WindowApplication::OnInitialize()
{
    // Stub-method, do nothing
    return true;
}
Bool WindowApplication::OnFrameMove()
{
    // Stub-method, do nothing
    return true;
}
Void WindowApplication::OnFrameRender()
{
    // Stub-method, do nothing
}
Void WindowApplication::OnExit()
{
    // Stub-method, do nothing
}
Void WindowApplication::OnCleanup()
{
    // Stub-method, do nothing
}

Void WindowApplication::OnWinMonitorChange( Void * /*pMonitor*/ )
{
    // Stub-method, do nothing
}
Void WindowApplication::OnWinMove( UInt /*iX*/, UInt /*iY*/ )
{
    // Stub-method, do nothing
}
Void WindowApplication::OnWinResize( UInt /*iWidth*/, UInt /*iHeight*/ )
{
    // Stub-method, do nothing
}

Void WindowApplication::OnWinFocus()
{
    // Stub-method, do nothing
}

Bool WindowApplication::OnKeyPress( KeyCode /*iKey*/ )
{
    // Stub-method, do nothing
    return false;
}
Bool WindowApplication::OnKeyRelease( KeyCode /*iKey*/ )
{
    // Stub-method, do nothing
    return false;
}

Void WindowApplication::OnMouseMove( UInt /*iX*/, UInt /*iY*/ )
{
    // Stub-method, do nothing
}
Bool WindowApplication::OnMouseWheel( UInt /*iX*/, UInt /*iY*/, Int /*iWheelDelta*/ )
{
    // Stub-method, do nothing
    return false;
}
Bool WindowApplication::OnMousePress( UInt /*iX*/, UInt /*iY*/, KeyCode /*iButton*/ )
{
    // Stub-method, do nothing
    return false;
}
Bool WindowApplication::OnMouseRelease( UInt /*iX*/, UInt /*iY*/, KeyCode /*iButton*/ )
{
    // Stub-method, do nothing
    return false;
}
Bool WindowApplication::OnMouseDblClick( UInt /*iX*/, UInt /*iY*/, KeyCode /*iButton*/ )
{
    // Stub-method, do nothing
    return false;
}

/////////////////////////////////////////////////////////////////////////////////

Void WindowApplication::_OnIdleTime()
{
    // Yield some cpu time (minimized or paused)
    if ( m_bRenderPaused || !(m_pWindow->IsActive()) )
        SystemFn->Sleep( 50 );

    // Timer introduction
    TimeMeasure fTime = SystemFn->TimeAbsolute( TIMEUNIT_NANOSECONDS );

    // Animate World
    Bool bUpdateWorld = false;
    if ( !m_bTimePaused )
        bUpdateWorld = OnFrameMove();

    // Render World
    if ( bUpdateWorld && !m_bRenderPaused )
        OnFrameRender();

    // Timer conclusion
    m_fFrameElapsed = SystemFn->TimeRelative( fTime, TIMEUNIT_NANOSECONDS );

    ++m_iFrameCount;
    m_fFrameTotalTime += m_fFrameElapsed;
    m_fFrameAverageTime = ( m_fFrameTotalTime / (TimeMeasure)m_iFrameCount );
    m_fFPS = ( 1.0e9f / (Scalar)m_fFrameElapsed );
    m_fAverageFPS = ( 1.0e9f / (Scalar)m_fFrameAverageTime );
}
Void WindowApplication::_OnWindowDraw()
{
    if ( m_bRenderPaused )
        OnFrameRender();
}

