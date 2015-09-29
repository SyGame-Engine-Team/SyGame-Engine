/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Main.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Entry Point
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
#ifndef SCARAB_ENTRYPOINT_SIMPLERPG_MAIN_H
#define SCARAB_ENTRYPOINT_SIMPLERPG_MAIN_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Engine/Application/ApplicationManager.h"

#include "World/Landscape.h"
#include "Graphics/GraphicsManager.h"
#include "Entities/EntityManager.h"
#include "Animation/AnimationManager.h"
#include "Gameplay/GameplayManager.h"
#include "Interface/InterfaceManager.h"

#include "../MainSelector.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
#ifndef MAIN_DEBUG

/////////////////////////////////////////////////////////////////////////////////
// The RPGGame class
class RPGGame : public WindowApplication
{
    // Application class
    DeclareApplication;

public:
    RPGGame();
    virtual ~RPGGame();

    virtual Bool OnInitialize();
    virtual Void OnCleanup();

    virtual Bool OnFrameMove();
    virtual Void OnFrameRender();

    virtual Void OnExit();

    virtual Bool OnKeyPress( KeyCode iKey );
    virtual Bool OnKeyRelease( KeyCode iKey );

    virtual Void OnMouseMove( UInt iX, UInt iY );
    virtual Bool OnMouseWheel( UInt iX, UInt iY, Int iWheelDelta );
    virtual Bool OnMousePress( UInt iX, UInt iY, KeyCode iButton );
    virtual Bool OnMouseRelease( UInt iX, UInt iY, KeyCode iButton );
    virtual Bool OnMouseDblClick( UInt iX, UInt iY, KeyCode iButton );

private:
    friend class RPGBackboardModel;

    // Input
    Void _CreateActions();
    Void _DestroyActions();

    UInt m_iActionMap;

    // Lights & Materials
    Void _CreateLightsMaterials();
    Void _DestroyLightsMaterials();

    // Bitmaps
    Void _CreateBitmaps();
    Void _DestroyBitmaps();

    // Textures
    Void _CreateTextures();
    Void _DestroyTextures();

    // Instanced geometry
    Void _CreateInstancedGeometry();
    Void _DestroyInstancedGeometry();

    // World
    Void _CreateWorld();
    Void _DestroyWorld();

    Landscape * m_pLandscape;

    Camera * m_pRenderCamera;
    WorldCamera3rdPerson * m_pWorldCamera;

	// Debug
	GChar m_strFPSText[64];
	GChar m_strCoordsText[64];
	GChar m_strCullerText[64];

    GPU2DSolidColorBrush * m_pDebugTextBrush;
    GPU2DTextFormat * m_pDebugTextFormat;
};

RegisterApplication( RPGGame );

/////////////////////////////////////////////////////////////////////////////////
#endif // !MAIN_DEBUG

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Main.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_SIMPLERPG_MAIN_H

