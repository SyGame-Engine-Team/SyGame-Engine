/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SkyforgeAtlas/Main.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Main, Entry Point
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
#ifndef SCARAB_ENTRYPOINT_SKYFORGEATLAS_MAIN_H
#define SCARAB_ENTRYPOINT_SKYFORGEATLAS_MAIN_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Lib/Datastruct/Table/TreeMap.h"

#include "../Framework/Graphics/GraphicsManager.h"
#include "../MainSelector.h"

#include "SkyGraph.h"
#include "Entities.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Declared Actions
#define ACTION_GUI_EXIT            0
#define ACTION_GUI_CONSOLE         1
#define ACTION_GUI_VIDEOSETTINGS   2
#define ACTION_GUI_RESOURCES       3

#define ACTION_GUI_STATISTICS      10
#define ACTION_GUI_SETTINGS        11

#define ACTION_MOVE_FORWARD        20
#define ACTION_MOVE_BACKWARD       21
#define ACTION_STRAFE_LEFT         22
#define ACTION_STRAFE_RIGHT        23

// Declared Lights
enum _LightID {
    LIGHTID_SUN = 0,

    LIGHTID_COUNT,
};

// Declared Materials
enum _MaterialID {
    MATERIALID_CHARACTER = 0,

    MATERIALID_AREA,

    MATERIALID_NODE_RED,
    MATERIALID_NODE_GREEN,
    MATERIALID_NODE_BLUE,
    MATERIALID_NODE_LOCK,
    MATERIALID_NODE_CLASS,
    MATERIALID_NODE_DIVINEFORM,

    MATERIALID_NODELINK,
    MATERIALID_AREALINK,

    MATERIALID_COUNT,
};

// Declared Textures
enum _TextureID {
    TEXTUREID_UNDEFINED = 0,

    TEXTUREID_COUNT,
};

// Declared Effects
enum _EffectID {
    EFFECTID_CHARACTER = 0,

    EFFECTID_AREA,
    EFFECTID_NODE,
    EFFECTID_LINK,

    EFFECTID_COUNT
};

// Declared Geometry
#define SKYGRAPH_AREA_RADIUS 10.0f
#define SKYGRAPH_NODE_RADIUS 1.0f

enum _GeometryID {    
    GEOMETRYID_CHARACTER = 0,

    GEOMETRYID_AREA,
    GEOMETRYID_NODE,
    GEOMETRYID_LINK,

    GEOMETRYID_COUNT
};

// Declared Bitmaps
enum _BitmapID {
    BITMAPID_UNDEFINED = 0,

    BITMAPID_COUNT,
};

    // Prototypes
class SABackboardModel;
class SAApplication;

/////////////////////////////////////////////////////////////////////////////////
// The SABackboardModel class
class SABackboardModel : public GUIBackboardModel
{
public:
    SABackboardModel( SAApplication * pApplication );
    virtual ~SABackboardModel();

    virtual Void OnKeyPress( KeyCode iKey, GUIEventFlag iFlags );
	virtual Void OnKeyRelease( KeyCode iKey, GUIEventFlag iFlags );

	virtual Void OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags );

    virtual Void OnStartDrag( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags );
	virtual Void OnDragLeft( const Point2 & ptLocalPos, GUIEventFlag iFlags );
	virtual Void OnDragRight( const Point2 & ptLocalPos, GUIEventFlag iFlags );

    virtual Void OnScroll( const Point2 & ptLocalPos, Int iWheelDelta, GUIEventFlag iFlags );

private:
    SAApplication * m_pApplication;

    Point2 m_ptLastMouseDragPos;
};

/////////////////////////////////////////////////////////////////////////////////
// The SAApplication class
class SAApplication : public WindowApplication
{
    // Application class
    DeclareApplication;

public:
    SAApplication();
    virtual ~SAApplication();

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

    // Entities access
    inline EntityNodeLink * GetNodeLink( const EntityNodeLinkKey & hKey ) const;

private:
    friend class SABackboardModel;

    // SkyGraph Atlas
    SkyGraph * m_pSkyGraph;

    // Input
    Void _CreateActions();
    Void _DestroyActions();

    UInt m_iActionMap;
    SABackboardModel * m_pBackboardModel;

    // Graphics
    Void _CreateGraphics();
    Void _DestroyGraphics();

    // Entities
    Void _CreateEntities();
    Void _DestroyEntities();

    EntityCharacter * m_pCharacter;
    Array<EntityArea*> m_arrAreas;
    Array<EntityNode*> m_arrNodes;

    typedef TreeMap<EntityNodeLinkKey,EntityNodeLink*> _LinkMap;
    _LinkMap m_mapLinks;

    Void _rec_GenerateBSPTree( WorldBSPNode * pNode, Bool bSplitX, EntityArea ** arrAreas, UInt iAreaCount );
    Void _GenerateBSPTree();

    WorldBSPNode * m_pSkyGraphRoot;

    // Camera
    Camera * m_pRenderCamera;
    WorldCamera3rdPerson * m_pWorldCamera;

    // Debug
	GChar m_strFPSText[64];
    GChar m_strCoordsText[64];
	GChar m_strCullerText[64];

    GPU2DSolidColorBrush * m_pDebugTextBrush;
    GPU2DTextFormat * m_pDebugTextFormat;
};
RegisterApplication( SAApplication );

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Main.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_SKYFORGEATLAS_MAIN_H



