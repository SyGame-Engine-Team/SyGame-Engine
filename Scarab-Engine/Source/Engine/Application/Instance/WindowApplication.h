/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Application/Instance/WindowApplication.h
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
// Header prelude
#ifndef SCARAB_ENGINE_APPLICATION_INSTANCE_WINDOWAPPLICATION_H
#define SCARAB_ENGINE_APPLICATION_INSTANCE_WINDOWAPPLICATION_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "BaseApplication.h"

    // Managers
#include "../../Resource/ResourceManager.h"
#include "../../Patching/PatchManager.h"

#include "../../Rendering/RenderingManager.h"
#include "../../Modeling/ModelingManager.h"
#include "../../Physics/PhysicsManager.h"
#include "../../World/WorldManager.h"

#include "../../Input/InputManager.h"
#include "../../GUI/GUIManager.h"

#include "../../Console/ConsoleManager.h"

/////////////////////////////////////////////////////////////////////////////////
// The WindowAppCallbacks class
class WindowAppCallbacks : public DeviceWindowCallbacks
{
public:
    WindowAppCallbacks( Void * pUserData = NULL );
    virtual ~WindowAppCallbacks();

    // Main loop
    virtual Void OnPause();
    virtual Void OnResume();

    virtual Void OnIdleTime();
    
    // Window
    virtual Void OnWindowMonitorChange( Void * pMonitor );
    virtual Void OnWindowMove( UInt iX, UInt iY );
    virtual Void OnWindowResize( UInt iWidth, UInt iHeight );

    virtual Void OnWindowFocus();

    virtual Void OnWindowDraw();

    // Keyboard
    virtual Bool OnKeyPress( KeyCode iKey );
    virtual Bool OnKeyRelease( KeyCode iKey );

    // Mouse
    virtual Void OnMouseMove( UInt iX, UInt iY );
    virtual Bool OnMousePress( UInt iX, UInt iY, KeyCode iKey );
    virtual Bool OnMouseRelease( UInt iX, UInt iY, KeyCode iKey );
    virtual Bool OnMouseDblClick( UInt iX, UInt iY, KeyCode iKey );
    virtual Bool OnMouseWheel( UInt iX, UInt iY, Int iWheelDelta );
};

/////////////////////////////////////////////////////////////////////////////////
// The WindowApplication class
class WindowApplication : public BaseApplication
{
protected:
    // Application-Restricted creation/destruction
    WindowApplication( const GChar * strTitle, PixelFormat iPixelFormat, UInt iWidth, UInt iHeight, DWord dwClearColor );
    virtual ~WindowApplication();

    // Entry point access, never call manually !
protected: static Int Run( const CommandLine & cmdLine );
private:   virtual Int Main( const CommandLine & cmdLine );

public:
    // Getters
    inline const GChar * GetWindowTitle() const;

    inline Void GetWindowRect( GPURectangle * outRectangle ) const;

    inline Bool IsActive() const;
    inline Bool IsFullScreen() const;
    inline Bool IsMinimized() const;
    inline Bool IsMaximized() const;

    inline UInt64 GetFrameCount() const;
    inline TimeMeasure GetLastFrameTime() const;
    inline TimeMeasure GetAverageFrameTime() const;
    inline Scalar GetFPS() const;
    inline Scalar GetAverageFPS() const;

    // Application Methods
    Void Exit();

    Void Pause( Bool bPauseTime, Bool bPauseRender );

    // Callbacks
    virtual Bool OnPreCreate();
    virtual Bool OnInitialize();
    virtual Bool OnFrameMove();
    virtual Void OnFrameRender();
    virtual Void OnExit();
    virtual Void OnCleanup();

    virtual Void OnWinMonitorChange( Void * pMonitor );
    virtual Void OnWinMove( UInt iX, UInt iY );
    virtual Void OnWinResize( UInt iWidth, UInt iHeight );

    virtual Void OnWinFocus();

    virtual Bool OnKeyPress( KeyCode iKey );
    virtual Bool OnKeyRelease( KeyCode iKey );

    virtual Void OnMouseMove( UInt iX, UInt iY );
    virtual Bool OnMouseWheel( UInt iX, UInt iY, Int iWheelDelta );
    virtual Bool OnMousePress( UInt iX, UInt iY, KeyCode iButton );
    virtual Bool OnMouseRelease( UInt iX, UInt iY, KeyCode iButton );
    virtual Bool OnMouseDblClick( UInt iX, UInt iY, KeyCode iButton );

private:
    // Callbacks
    friend class WindowAppCallbacks;
    Void _OnIdleTime();
    Void _OnWindowDraw();

    // Device-dependant delegate 
    DeviceWindow * m_pWindow;
    WindowAppCallbacks m_devCallbacks;

    // Window state
    const GChar * m_strTitle;
    PixelFormat m_iCreationFormat;
    UInt m_iCreationWidth;
    UInt m_iCreationHeight;
    DWord m_dwCreationClearColor;

    // Pause/Resume stacks
    Bool m_bTimePaused;
    UInt m_iTimePauseStack;
    Bool m_bRenderPaused;
    UInt m_iRenderPauseStack;

    // Timing support
    UInt64 m_iFrameCount;
    TimeMeasure m_fFrameLast, m_fFrameElapsed;
    TimeMeasure m_fFrameTotalTime, m_fFrameAverageTime;
    Scalar m_fFPS, m_fAverageFPS;
};

//InitCommonControls();

//class MyGame : public WindowApplication {
//    DeclareApplication;
//public:
//    MyGame():WindowApplication( TEXT("TestApp") ) {}
//    ~MyGame(){}
//    virtual Int Main( const CommandLine & cmdLine ) { return 0; }
//};
//RegisterApplication(MyGame);
//DefineWindowApplication(MyGame);

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "WindowApplication.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_APPLICATION_INSTANCE_WINDOWAPPLICATION_H

