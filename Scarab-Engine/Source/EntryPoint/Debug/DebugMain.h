/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/Debug/DebugMain.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : DebugMain, Entry Point
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
#ifndef SCARAB_ENTRYPOINT_DEBUG_DEBUGMAIN_H
#define SCARAB_ENTRYPOINT_DEBUG_DEBUGMAIN_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../MainSelector.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
#if defined(DEBUGMAIN_SCRATCH)

    // Prototypes

/////////////////////////////////////////////////////////////////////////////////
// The ScratchTests class
class ScratchTests : public ConsoleApplication
{
    // Application class
    DeclareApplication;

public:
    ScratchTests();
    virtual ~ScratchTests();

    virtual Int Main( const CommandLine & cmdLine );
};
RegisterApplication( ScratchTests );

/////////////////////////////////////////////////////////////////////////////////
#elif defined(DEBUGMAIN_ENGINE)

    // Prototypes
class TestBackboardModel;
class EngineTests;

/////////////////////////////////////////////////////////////////////////////////
// The TestBackboardModel class
class TestBackboardModel : public GUIBackboardModel
{
public:
    TestBackboardModel( EngineTests * pApplication );
    virtual ~TestBackboardModel();

    virtual Void OnKeyPress( KeyCode iKey, GUIEventFlag iFlags );
	virtual Void OnKeyRelease( KeyCode iKey, GUIEventFlag iFlags );

	virtual Void OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags );

    virtual Void OnStartDrag( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags );
	virtual Void OnDragRight( const Point2 & ptLocalPos, GUIEventFlag iFlags );

    virtual Void OnScroll( const Point2 & ptLocalPos, Int iWheelDelta, GUIEventFlag iFlags );

private:
    EngineTests * m_pApplication;

    Point2 m_ptLastMouseDragPos;
};

/////////////////////////////////////////////////////////////////////////////////
// The EngineTests class
class EngineTests : public WindowApplication
{
    // Application class
    DeclareApplication;

public:
    EngineTests();
    virtual ~EngineTests();

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
    friend class TestBackboardModel;

    // Input
    Void _CreateActions();
    Void _DestroyActions();

    UInt m_iActionMap;
    TestBackboardModel * m_pBackboardModel;

    // Effects
    Void _CreateEffects();
    Void _DestroyEffects();

    Light m_hSunLight;
    Material m_hMaterialFloor;
    Material m_hMaterialCharacter;
    Material m_hMaterialA;
    Material m_hMaterialB;
    Material m_hMaterialC;
    Material m_hMaterialD;

    EffectLightMaterial * m_pDefaultEffect;
    EffectLightMaterialInstance * m_pEffectInstanceFloor;
    EffectLightMaterialInstance * m_pEffectInstanceCharacter;
    EffectLightMaterialInstance * m_pEffectInstanceA;
    EffectLightMaterialInstance * m_pEffectInstanceB;
    EffectLightMaterialInstance * m_pEffectInstanceC;
    EffectLightMaterialInstance * m_pEffectInstanceD;

    // Instanced Geometry
    Void _CreateInstancedGeometry();
    Void _DestroyInstancedGeometry();

    MeshBox * m_pFloorGeometry;
    ShapeBox * m_pFloorShape;

    MeshSphere * m_pCharacterGeometry;
    ShapeSphere * m_pCharacterShape;

    MeshSphere * m_pSphereGeometry;
    ShapeSphere * m_pSphereShape;

    MeshBox * m_pBoxGeometry;
    ShapeBox * m_pBoxShape;

    // World
    Void _CreateWorld();
    Void _DestroyWorld();

    Camera * m_pRenderCamera;
    WorldCamera3rdPerson * m_pWorldCamera;

    WorldLeaf * m_pFloor;
    WorldLeaf * m_pCharacter;
    CharacterController * m_pCharacterController;

        // Stack tests
#define ENGINE_TEST_STACK_SIZE 2
    WorldLeaf * m_arrBallStack[ENGINE_TEST_STACK_SIZE];
    WorldLeaf * m_arrBoxStack[ENGINE_TEST_STACK_SIZE];

        // Joint tests
    WorldLeaf * m_pFixedObject;
    WorldLeaf * m_pObjectA;
    WorldLeaf * m_pObjectB;

    JointEdge * m_pJointAF;
    JointEdge * m_pJointBF;

    // Debug
    Bool m_bWireFrame;

	GChar m_strFPSText[64];
	GChar m_strCoordsText[64];
	GChar m_strCullerText[64];

    GPU2DSolidColorBrush * m_pDebugTextBrush;
    GPU2DTextFormat * m_pDebugTextFormat;
};
RegisterApplication( EngineTests );

/////////////////////////////////////////////////////////////////////////////////
#endif // DEBUGMAIN_SCRATCH, DEBUGMAIN_ENGINE

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "DebugMain.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_DEBUG_DEBUGMAIN_H



