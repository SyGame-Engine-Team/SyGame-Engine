/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SkyforgeAtlas/Main.cpp
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
// Includes
#include "Main.h"

/////////////////////////////////////////////////////////////////////////////////
// SABackboardModel implementation
SABackboardModel::SABackboardModel( SAApplication * pApplication ):
    GUIBackboardModel(), m_ptLastMouseDragPos()
{
	m_pApplication = pApplication;
}
SABackboardModel::~SABackboardModel()
{
	// nothing to do
}

Void SABackboardModel::OnKeyPress( KeyCode iKey, GUIEventFlag iFlags )
{
	// Action flags
    ActionFlag iActionFlags = ACTIONFLAG_NONE;
    if ( iFlags & GUIEVENT_FLAG_CTRL )
        iActionFlags |= ACTIONFLAG_MOD_CTRL;
    if ( iFlags & GUIEVENT_FLAG_ALT )
        iActionFlags |= ACTIONFLAG_MOD_ALT;
    if ( iFlags & GUIEVENT_FLAG_SHIFT )
        iActionFlags |= ACTIONFLAG_MOD_SHIFT;

    // Action mapping
    Action iAction = InputFn->GetAction( iKey, iActionFlags );
    switch( iAction ) {
        case ACTION_GUI_EXIT:
            m_pApplication->Exit();
            break;

        case ACTION_GUI_CONSOLE:
            ConsoleFn->Toggle();
            break;
        case ACTION_GUI_VIDEOSETTINGS:
            ConsoleFn->ToggleVideoSettings();
            break;
        case ACTION_GUI_RESOURCES:
            ConsoleFn->ToggleResourceView();
            break;
        
        case ACTION_GUI_STATISTICS:
            /////////////////////////////////
            break;
        case ACTION_GUI_SETTINGS:
            /////////////////////////////////
            break;

        case ACTION_MOVE_FORWARD:
            m_pApplication->m_pCharacter->GetController()->SetMovingForward( true );
            break;
        case ACTION_MOVE_BACKWARD:
            m_pApplication->m_pCharacter->GetController()->SetMovingBackward( true );
            break;
        case ACTION_STRAFE_LEFT:
            m_pApplication->m_pCharacter->GetController()->SetMovingLeft( true );
            break;
        case ACTION_STRAFE_RIGHT:
            m_pApplication->m_pCharacter->GetController()->SetMovingRight( true );
            break;

        default:
            // do nothing
            break;
    }
}
Void SABackboardModel::OnKeyRelease( KeyCode iKey, GUIEventFlag iFlags )
{
    // Action flags
    ActionFlag iActionFlags = ACTIONFLAG_NONE;
    if ( iFlags & GUIEVENT_FLAG_CTRL )
        iActionFlags |= ACTIONFLAG_MOD_CTRL;
    if ( iFlags & GUIEVENT_FLAG_ALT )
        iActionFlags |= ACTIONFLAG_MOD_ALT;
    if ( iFlags & GUIEVENT_FLAG_SHIFT )
        iActionFlags |= ACTIONFLAG_MOD_SHIFT;

    // Action mapping
    Action iAction = InputFn->GetAction( iKey, iActionFlags );
    switch( iAction ) {
        case ACTION_MOVE_FORWARD:
            m_pApplication->m_pCharacter->GetController()->SetMovingForward( false );
            break;
        case ACTION_MOVE_BACKWARD:
            m_pApplication->m_pCharacter->GetController()->SetMovingBackward( false );
            break;
        case ACTION_STRAFE_LEFT:
            m_pApplication->m_pCharacter->GetController()->SetMovingLeft( false );
            break;
        case ACTION_STRAFE_RIGHT:
            m_pApplication->m_pCharacter->GetController()->SetMovingRight( false );
            break;

        default:
            // do nothing
            break;
    }
}

Void SABackboardModel::OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags )
{
	// Action flags
    ActionFlag iActionFlags = ACTIONFLAG_NONE;
    if ( iFlags & GUIEVENT_FLAG_CTRL )
        iActionFlags |= ACTIONFLAG_MOD_CTRL;
    if ( iFlags & GUIEVENT_FLAG_ALT )
        iActionFlags |= ACTIONFLAG_MOD_ALT;
    if ( iFlags & GUIEVENT_FLAG_SHIFT )
        iActionFlags |= ACTIONFLAG_MOD_SHIFT;

    // Ray picking
    Line3 vPickRay;
    Bool bPicked = RenderingFn->GetPickRay( &(vPickRay.Position), &(vPickRay.Direction), ptLocalPos.X, ptLocalPos.Y );
    Assert( bPicked );

    // Ray casting
    WorldFn->RayCast( vPickRay, 0.0f, SCALAR_INFINITE );
    WorldLeaf * pLeaf = WorldFn->GetClosestRayCastResult()->pHit;

    // Action mapping
    if ( iKey == KEYCODE_MOUSELEFT ) {
        if ( iActionFlags == ACTIONFLAG_NONE ) {
            // Left click
            ///////////////////////////////////////////////////
        }
    } else if ( iKey == KEYCODE_MOUSERIGHT ) {
        if ( iActionFlags == ACTIONFLAG_NONE ) {
            // Right click
            ///////////////////////////////////////////////////
        }
    }
}

Void SABackboardModel::OnStartDrag( const Point2 & ptLocalPos, KeyCode /*iKey*/, GUIEventFlag /*iFlags*/ )
{
    m_ptLastMouseDragPos = ptLocalPos;
}
Void SABackboardModel::OnDragLeft( const Point2 & ptLocalPos, GUIEventFlag /*iFlags*/ )
{
    // Application data
    WorldCamera3rdPerson * pWorldCamera = (WorldCamera3rdPerson*)( WorldFn->GetWorldCamera() );

    // Arcball rotation
    Int iDX = ( ptLocalPos.X - m_ptLastMouseDragPos.X );
    Int iDY = ( ptLocalPos.Y - m_ptLastMouseDragPos.Y );

    static const Scalar fArcballScale = 1.0e-3f;
    static const Scalar fArcballSpanH = SCALAR_PI;
    static const Scalar fArcballSpanV = SCALAR_PI;

    Scalar fAngleX = ( ((Scalar)iDX) * fArcballScale * fArcballSpanH );
    Scalar fAngleY = ( ((Scalar)iDY) * fArcballScale * fArcballSpanV );

    pWorldCamera->RotateH( -fAngleX );
    pWorldCamera->RotateV( fAngleY );

    m_ptLastMouseDragPos = ptLocalPos;
}
Void SABackboardModel::OnDragRight( const Point2 & ptLocalPos, GUIEventFlag /*iFlags*/ )
{
    // Application data
    WorldCamera3rdPerson * pWorldCamera = (WorldCamera3rdPerson*)( WorldFn->GetWorldCamera() );

    // Arcball rotation
    Int iDX = ( ptLocalPos.X - m_ptLastMouseDragPos.X );
    Int iDY = ( ptLocalPos.Y - m_ptLastMouseDragPos.Y );

    static const Scalar fArcballScale = 1.0e-3f;
    static const Scalar fArcballSpanH = SCALAR_PI;
    static const Scalar fArcballSpanV = SCALAR_PI;

    Scalar fAngleX = ( ((Scalar)iDX) * fArcballScale * fArcballSpanH );
    Scalar fAngleY = ( ((Scalar)iDY) * fArcballScale * fArcballSpanV );

    pWorldCamera->RotateH( -fAngleX );
    pWorldCamera->RotateV( fAngleY );

    m_ptLastMouseDragPos = ptLocalPos;
}

Void SABackboardModel::OnScroll( const Point2 & /*ptLocalPos*/, Int iWheelDelta, GUIEventFlag iFlags )
{
    // Action flags
    ActionFlag iActionFlags = ACTIONFLAG_NONE;
    if ( iFlags & GUIEVENT_FLAG_CTRL )
        iActionFlags |= ACTIONFLAG_MOD_CTRL;
    if ( iFlags & GUIEVENT_FLAG_ALT )
        iActionFlags |= ACTIONFLAG_MOD_ALT;
    if ( iFlags & GUIEVENT_FLAG_SHIFT )
        iActionFlags |= ACTIONFLAG_MOD_SHIFT;

    // Application data
    WorldCamera3rdPerson * pWorldCamera = (WorldCamera3rdPerson*)( WorldFn->GetWorldCamera() );
    
    // Camera Zoom
    pWorldCamera->Zoom( -(Scalar)iWheelDelta );
}

/////////////////////////////////////////////////////////////////////////////////
// SAApplication implementation
DefineWindowApplication( SAApplication );

SAApplication::SAApplication():
    WindowApplication( TEXT("SkyforgeAtlas"), PIXEL_FMT_RGBA8UI, 1024, 768, 0x80808080 ),
    m_arrAreas(), m_arrNodes(), m_mapLinks()
{
    // SkyAtlas data
    m_pSkyGraph = NULL;

    // Input
    m_iActionMap = INVALID_OFFSET;
    m_pBackboardModel = NULL;

    // Environment & Entities
    m_pCharacter = NULL;

    m_pSkyGraphRoot = NULL;

    // Camera
    m_pRenderCamera = NULL;
    m_pWorldCamera = NULL;

	// Debug
	m_strFPSText[0] = NULLBYTE;
	m_strCoordsText[0] = NULLBYTE;
    m_strCullerText[0] = NULLBYTE;

    m_pDebugTextBrush = NULL;
    m_pDebugTextFormat = NULL;
}
SAApplication::~SAApplication()
{
    // nothing to do
}

Bool SAApplication::OnInitialize()
{
    // Load SkyAtlas
    m_pSkyGraph = New SkyGraph();
    m_pSkyGraph->LoadFromXML( SKYGRAPH_XML_FILE );

    // Initialize everything
    _CreateActions();
    _CreateGraphics();
    _CreateEntities();

    m_pBackboardModel = New SABackboardModel( this );
    GUIFn->SetBackboardModel( m_pBackboardModel );

    // Debug
    GPU2DColor hDebugTextColor = Color4::White;
    m_pDebugTextBrush = New GPU2DSolidColorBrush( RenderingFn->GetScreen2DContext() );
    m_pDebugTextBrush->SetColor( &hDebugTextColor );
    m_pDebugTextBrush->Bind();

    m_pDebugTextFormat = New GPU2DTextFormat();
    m_pDebugTextFormat->Bind( LOCALENAME_EN_US, GPU2DFONTFAMILY_ARIAL, GPU2DFONT_WEIGHT_NORMAL, GPU2DFONT_STYLE_NORMAL, GPU2DFONT_STRETCH_NORMAL, 10.0f );
    m_pDebugTextFormat->SetTextAlignment( GPU2DTEXT_ALIGNMENT_LEADING );
    m_pDebugTextFormat->SetParagraphAlignment( GPU2DTEXT_PARAGRAPHALIGNMENT_NEAR );

    for( UInt i = 0; i < m_arrAreas.Count(); ++i ) {
        EntityArea * pEntityArea = m_arrAreas[i];
        pEntityArea->SetDebugFlags( WORLD_DEBUGDRAW_GIZMOS | WORLD_DEBUGDRAW_WORLDBVS );
    }
    //WorldFn->ToggleDebugDraw( true );

    return true;
}
Void SAApplication::OnCleanup()
{
    // Debug
    //WorldFn->ToggleDebugDraw( false );

    m_pDebugTextFormat->UnBind();
    Delete( m_pDebugTextFormat );
    m_pDebugTextFormat = NULL;

    m_pDebugTextBrush->UnBind();
    Delete( m_pDebugTextBrush );
    m_pDebugTextBrush = NULL;

    // Cleanup everything
    GUIFn->SetBackboardModel( NULL );
    Delete( m_pBackboardModel );
    m_pBackboardModel = NULL;

    _DestroyEntities();
    _DestroyGraphics();
    _DestroyActions();

    // Destroy SkyAtlas
    Delete( m_pSkyGraph );
    m_pSkyGraph = NULL;
}

Bool SAApplication::OnFrameMove()
{
    static TimeMeasure fLastTime = 0.0;

    // Initialization
    if ( fLastTime == 0.0 ) {
        // Timing
        fLastTime = SystemFn->TimeAbsolute( TIMEUNIT_SECONDS );

        ////////////////////////////
        return true;
    }

    // Timing
    TimeMeasure fTime = 0.0;
    Scalar fElapsedTime = (Scalar)( SystemFn->TimeRelative( fLastTime, TIMEUNIT_SECONDS, &fTime ) );

    // Update interface / event processing
	GUIFn->Update();

    // Update world & entities
    WorldFn->UpdateWorld( fElapsedTime );
    
    // Timing
    fLastTime = fTime;

    return true;
}
Void SAApplication::OnFrameRender()
{
    // Clear scene
    RenderingFn->ClearRenderTarget( RenderingFn->GetScreenRenderTarget(), (const Float*)Color4::DarkGray );
    RenderingFn->ClearDepthStencil( RenderingFn->GetScreenDepthStencil(), true, true, 1.0f );

    // Render World
    WorldFn->RenderWorld();

    // Render GUI
    GUIFn->Render();

    // Render Debug text
    Vector3 vPosition = m_pWorldCamera->GetTarget()->WorldTransform.GetTranslate();

    StringFn->Format( m_strFPSText, TEXT("FPS = %2.5f"), GetAverageFPS() );
    StringFn->Format( m_strCoordsText, TEXT("Coords = (%3f, %3f, %3f)"), vPosition.X, vPosition.Y, vPosition.Z );
    StringFn->Format( m_strCullerText, TEXT("Visible leaves = %d"), WorldFn->GetVisibleGroup().GetCount() );

    GPU2DContext * pContext2D = RenderingFn->GetScreen2DContext();

    GPU2DRectangle hDstRect;
    hDstRect.fLeft = 16.0f;
    hDstRect.fTop = 16.0f;
    hDstRect.fRight = hDstRect.fLeft + 256.0f;
    hDstRect.fBottom = hDstRect.fTop + 16.0f;

    pContext2D->Begin();

    pContext2D->DrawText( m_strFPSText, StringFn->Length(m_strFPSText), m_pDebugTextFormat, &hDstRect, m_pDebugTextBrush, GPU2DTEXT_DRAWOPTION_CLIP );
    hDstRect.fTop += 16.0f;
    hDstRect.fBottom += 16.0f;
    pContext2D->DrawText( m_strCoordsText, StringFn->Length(m_strCoordsText), m_pDebugTextFormat, &hDstRect, m_pDebugTextBrush, GPU2DTEXT_DRAWOPTION_CLIP );
    hDstRect.fTop += 16.0f;
    hDstRect.fBottom += 16.0f;
    pContext2D->DrawText( m_strCullerText, StringFn->Length(m_strCullerText), m_pDebugTextFormat, &hDstRect, m_pDebugTextBrush, GPU2DTEXT_DRAWOPTION_CLIP );

    pContext2D->End();

    // Present frame
    RenderingFn->Present();
}

Void SAApplication::OnExit()
{
    GUIFn->OnExit();
}

Bool SAApplication::OnKeyPress( KeyCode iKey )
{
    GUIFn->OnKeyPress( iKey );

    return true;
}
Bool SAApplication::OnKeyRelease( KeyCode iKey )
{
    GUIFn->OnKeyRelease( iKey );

    return true;
}

Void SAApplication::OnMouseMove( UInt iX, UInt iY )
{
    GUIFn->OnMouseMove( Point2(iX, iY) );
}
Bool SAApplication::OnMouseWheel( UInt iX, UInt iY, Int iWheelDelta )
{
    GUIFn->OnMouseWheel( Point2(iX, iY), iWheelDelta );

    return true;
}
Bool SAApplication::OnMousePress( UInt iX, UInt iY, KeyCode iButton )
{
    GUIFn->OnMousePress( Point2(iX, iY), iButton );

    return true;
}
Bool SAApplication::OnMouseRelease( UInt iX, UInt iY, KeyCode iButton )
{
    GUIFn->OnMouseRelease( Point2(iX, iY), iButton );

    return true;
}
Bool SAApplication::OnMouseDblClick( UInt iX, UInt iY, KeyCode iButton )
{
    GUIFn->OnMouseDblClick( Point2(iX, iY), iButton );

    return true;
}

/////////////////////////////////////////////////////////////////////////////////

Void SAApplication::_CreateActions()
{
    // Action map
    m_iActionMap = InputFn->CreateMap();
    InputFn->SelectMap( m_iActionMap );

    InputFn->BindAction( ACTION_GUI_EXIT,          KEYCODE_ESCAPE );
    InputFn->BindAction( ACTION_GUI_CONSOLE,       KEYCODE_OEM_7 );
    InputFn->BindAction( ACTION_GUI_VIDEOSETTINGS, KEYCODE_INSERT );
    InputFn->BindAction( ACTION_GUI_RESOURCES,     KEYCODE_PAGEUP );

    InputFn->BindAction( ACTION_GUI_STATISTICS, KEYCODE_DELETE );
    InputFn->BindAction( ACTION_GUI_SETTINGS,   KEYCODE_END );

    InputFn->BindAction( ACTION_MOVE_FORWARD,  KEYCODE_Z );
    InputFn->BindAction( ACTION_MOVE_BACKWARD, KEYCODE_S );
    InputFn->BindAction( ACTION_STRAFE_LEFT,   KEYCODE_Q );
    InputFn->BindAction( ACTION_STRAFE_RIGHT,  KEYCODE_D );
}
Void SAApplication::_DestroyActions()
{
    // Action map
    InputFn->DestroyMap( m_iActionMap );
    m_iActionMap = INVALID_OFFSET;
}

Void SAApplication::_CreateGraphics()
{
    // Initialize
    GraphicsManager::Create();
    GraphicsFn->Initialize();

    // Lights
    Light * pLightSun = GraphicsFn->CreateLight( LIGHTID_SUN, LIGHT_OMNI );
    pLightSun->Diffuse = Color4::White;
    pLightSun->Position = Vertex3( 0.0f, 0.0f, 50.0f );

    // Materials
    Material * pMatCharacter = GraphicsFn->CreateMaterial( MATERIALID_CHARACTER );
    pMatCharacter->Diffuse = Color4::White;

    Material * pMatArea = GraphicsFn->CreateMaterial( MATERIALID_AREA );
    pMatArea->Diffuse = Color4::Yellow;

    Material * pMatNodeRed = GraphicsFn->CreateMaterial( MATERIALID_NODE_RED );
    pMatNodeRed->Diffuse = Color4::Red;
    Material * pMatNodeGreen = GraphicsFn->CreateMaterial( MATERIALID_NODE_GREEN );
    pMatNodeGreen->Diffuse = Color4::Green;
    Material * pMatNodeBlue = GraphicsFn->CreateMaterial( MATERIALID_NODE_BLUE );
    pMatNodeBlue->Diffuse = Color4::Blue;
    Material * pMatNodeLock = GraphicsFn->CreateMaterial( MATERIALID_NODE_LOCK );
    pMatNodeLock->Diffuse = Color4::DarkGray;
    Material * pMatNodeClass = GraphicsFn->CreateMaterial( MATERIALID_NODE_CLASS );
    pMatNodeClass->Diffuse = Color4::Cyan;
    Material * pMatNodeDivineForm = GraphicsFn->CreateMaterial( MATERIALID_NODE_DIVINEFORM );
    pMatNodeDivineForm->Diffuse = Color4::Magenta;

    Material * pMatNodeLink = GraphicsFn->CreateMaterial( MATERIALID_NODELINK );
    pMatNodeLink->Emissive = Color4::White;
    Material * pMatAreaLink = GraphicsFn->CreateMaterial( MATERIALID_AREALINK );
    pMatAreaLink->Emissive = Color4::Yellow;

    // Textures
    //////////////////////////////////////

    // Effects
    EffectLightMaterial * pCharacterEffect = GraphicsFn->CreateEffect<EffectLightMaterial>( EFFECTID_CHARACTER );

    EffectLightMaterial * pAreaEffect = GraphicsFn->CreateEffect<EffectLightMaterial>( EFFECTID_AREA );
    GPURasterizerState * pRasterState = pAreaEffect->GetRasterizerState(0,0);
    GPURasterizerStateDesc hDesc;
    pRasterState->GetDesc( &hDesc );
    pRasterState->UnBind();
    hDesc.iFillMode = GPURASTERIZER_FILL_WIREFRAME;
    pRasterState->Bind( &hDesc );

    EffectLightMaterial * pNodeEffect = GraphicsFn->CreateEffect<EffectLightMaterial>( EFFECTID_NODE );

    EffectMaterial * pLinkEffect = GraphicsFn->CreateEffect<EffectMaterial>( EFFECTID_LINK );

    // Instanced geometry
    GraphicsFn->SelectMemory( TEXT("Scratch") );

    GPUInputLayout * pIL = pCharacterEffect->GetInputLayout( EFFECTLIGHT_TECHNIQUE_POINT_PHONG, 0 );
    Sphere vCharacter( Vertex3::Null, 1.0f );
    MeshSphere * pMeshCharacter = New MeshSphere( vCharacter, 16, 16, pIL );
    GraphicsFn->CreateGeometryInstance( GEOMETRYID_CHARACTER, pMeshCharacter, NULL, PhysicsFn->CreateShapeSphere(pMeshCharacter) );

    pIL = pAreaEffect->GetInputLayout( EFFECTLIGHT_TECHNIQUE_POINT_PHONG, 0 );
    Sphere vArea( Vertex3::Null, SKYGRAPH_AREA_RADIUS );
    GraphicsFn->CreateGeometryInstance( GEOMETRYID_AREA, New MeshSphere(vArea, 16, 16, pIL) );

    pIL = pNodeEffect->GetInputLayout( EFFECTLIGHT_TECHNIQUE_POINT_PHONG, 0 );
    Sphere vNode( Vertex3::Null, SKYGRAPH_NODE_RADIUS );
    GraphicsFn->CreateGeometryInstance( GEOMETRYID_NODE, New MeshSphere(vNode, 8, 8, pIL) );

    pIL = pLinkEffect->GetInputLayout( 0, 0 );
    Segment3 vNodeLink( Vertex3::Null, Vertex3(1.0f,0.0f,0.0f) );
    GraphicsFn->CreateGeometryInstance( GEOMETRYID_LINK, New MeshSegment(vNodeLink, pIL) );

    GraphicsFn->UnSelectMemory();

    // Bitmaps
    //////////////////////////////////////
}
Void SAApplication::_DestroyGraphics()
{
    // Bitmaps
    //////////////////////////////////////

    // Instanced geometry
    GraphicsFn->DestroyGeometryInstance( GEOMETRYID_LINK );
    GraphicsFn->DestroyGeometryInstance( GEOMETRYID_NODE );
    GraphicsFn->DestroyGeometryInstance( GEOMETRYID_AREA );
    GraphicsFn->DestroyGeometryInstance( GEOMETRYID_CHARACTER );

    // Effects
    GraphicsFn->DestroyEffect( EFFECTID_LINK );
    GraphicsFn->DestroyEffect( EFFECTID_NODE );
    GraphicsFn->DestroyEffect( EFFECTID_AREA );
    GraphicsFn->DestroyEffect( EFFECTID_CHARACTER );

    // Textures
    //////////////////////////////////////

    // Materials
    GraphicsFn->DestroyMaterial( MATERIALID_AREALINK );
    GraphicsFn->DestroyMaterial( MATERIALID_NODELINK );

    GraphicsFn->DestroyMaterial( MATERIALID_NODE_DIVINEFORM );
    GraphicsFn->DestroyMaterial( MATERIALID_NODE_CLASS );
    GraphicsFn->DestroyMaterial( MATERIALID_NODE_LOCK );
    GraphicsFn->DestroyMaterial( MATERIALID_NODE_BLUE );
    GraphicsFn->DestroyMaterial( MATERIALID_NODE_GREEN );
    GraphicsFn->DestroyMaterial( MATERIALID_NODE_RED );

    GraphicsFn->DestroyMaterial( MATERIALID_AREA );

    GraphicsFn->DestroyMaterial( MATERIALID_CHARACTER );

    // Lights
    GraphicsFn->DestroyLight( LIGHTID_SUN );

    // Cleanup
    GraphicsFn->Cleanup();
    GraphicsManager::Destroy();
}

Void SAApplication::_CreateEntities()
{
    WorldFn->SelectMemory( TEXT("Nodes") );

    // Character
    m_pCharacter = New EntityCharacter();

    // SkyGraph entities
    m_arrAreas.UseMemoryContext( WorldFn->GetMemoryContext(), TEXT("Scratch") );
    m_arrAreas.Create();

    m_arrNodes.UseMemoryContext( WorldFn->GetMemoryContext(), TEXT("Scratch") );
    m_arrNodes.Create();

    m_mapLinks.UseMemoryContext( WorldFn->GetMemoryContext(), TEXT("Scratch") );
    m_mapLinks.SetComparator( EntityNodeLinkKey::Compare );
    m_mapLinks.Create();

    UInt iAreaCount = m_pSkyGraph->GetAreaCount();
    for( UInt i = 0; i < iAreaCount; ++i ) {
        SkyArea * pArea = m_pSkyGraph->GetArea(i);
        m_arrAreas.Push( New EntityArea(pArea) );

        UInt iNodeCount = pArea->GetNodeCount();
        for( UInt j = 0; j < iNodeCount; ++j ) {
            SkyNode * pNode = pArea->GetNode(j);
            m_arrNodes.Push( New EntityNode(pNode) );
        }

        for( UInt j = 0; j < iNodeCount; ++j ) {
            SkyNode * pNode = pArea->GetNode(j);
            EntityNode * pEntNode = (EntityNode*)( pNode->GetHolder() );

            for( UInt k = 0; k < SKYGRAPH_PORT_COUNT; ++k ) {
                SkyGraphPort iPort = (SkyGraphPort)k;
                SkyNode * pTarget = pNode->GetLink(iPort);
                if ( pTarget == NULL )
                    continue;
                if ( pTarget->GetParentArea() != pArea ) // skip area links at this point
                    continue;
                EntityNode * pEntTarget = (EntityNode*)( pTarget->GetHolder() );

                EntityNodeLinkKey hKey(pEntNode,pEntTarget);
                if ( !(m_mapLinks.Contains(hKey)) ) {
                    Bool bInserted = m_mapLinks.Insert( hKey, New EntityNodeLink(pEntNode, pEntTarget) );
                    Assert( bInserted );
                }
            }
        }
    }

    for( UInt i = 0; i < iAreaCount; ++i ) {
        SkyArea * pArea = m_pSkyGraph->GetArea(i);

        for( UInt k = 0; k < SKYGRAPH_PORT_COUNT; ++k ) {
            SkyGraphPort iPort = (SkyGraphPort)k;
            SkyGraphPort iTargetPort;
            SkyArea * pTarget = pArea->GetLink( iPort, &iTargetPort );
            if ( pTarget == NULL )
                continue;
            SkyNode * pDoor = pArea->GetDoorNode(iPort);
            SkyNode * pTargetDoor = pTarget->GetDoorNode(iTargetPort);
            EntityNode * pEntDoor = (EntityNode*)( pDoor->GetHolder() );
            EntityNode * pEntTargetDoor = (EntityNode*)( pTargetDoor->GetHolder() );

            EntityNodeLinkKey hKey(pEntDoor,pEntTargetDoor);
            if ( !(m_mapLinks.Contains(hKey)) ) {
                Bool bInserted = m_mapLinks.Insert( hKey, New EntityNodeLink(pEntDoor, pEntTargetDoor) );
                Assert( bInserted );
            }
        }
    }

    WorldFn->UnSelectMemory();

    // SkyGraph structure
    _GenerateBSPTree();

    // Camera
    m_pRenderCamera = New Camera( true );
    m_pWorldCamera = New WorldCamera3rdPerson( m_pRenderCamera, m_pCharacter, NULL, 3.0f );
    WorldFn->SetWorldCamera( m_pWorldCamera );
}
Void SAApplication::_DestroyEntities()
{
    // Camera
    WorldFn->SetWorldCamera( NULL );
    Delete( m_pWorldCamera );
    m_pWorldCamera = NULL;
    Delete( m_pRenderCamera );
    m_pRenderCamera = NULL;

    // SkyGraph structure & entities
    WorldFn->RemoveChild( TEXT("BSPTree_Root") );
    WorldFn->DestroyNode( m_pSkyGraphRoot, true );
    m_pSkyGraphRoot = NULL;

    m_mapLinks.Destroy();
    m_arrNodes.Destroy();
    m_arrAreas.Destroy();

    // Character
    WorldFn->DestroyNode( m_pCharacter, true );
    m_pCharacter = NULL;
}

Void SAApplication::_rec_GenerateBSPTree( WorldBSPNode * pNode, Bool bSplitX, EntityArea ** arrAreas, UInt iAreaCount )
{
    // Dispatch all elements, compute min/max
    UInt iNegCount = 0;
    UInt iPosCount = 0;
    EntityArea * arrNegAreas[SKYGRAPH_MAX_AREAS];
    EntityArea * arrPosAreas[SKYGRAPH_MAX_AREAS];

    Vertex3 vNegMin( SCALAR_INFINITE, SCALAR_INFINITE, SCALAR_INFINITE );
    Vertex3 vNegMax( -SCALAR_INFINITE, -SCALAR_INFINITE, -SCALAR_INFINITE );
    Vertex3 vPosMin( SCALAR_INFINITE, SCALAR_INFINITE, SCALAR_INFINITE );
    Vertex3 vPosMax( -SCALAR_INFINITE, -SCALAR_INFINITE, -SCALAR_INFINITE );
    for( UInt i = 0; i < iAreaCount; ++i ) {
        EntityArea * pArea = arrAreas[i];
        Vertex3 vPos = pArea->GetArea()->GetPosition();

        if ( pNode->WorldPlane.ClassifyPointNC(vPos) < 0 ) {
            arrNegAreas[iNegCount++] = pArea;
            if ( vPos.X < vNegMin.X )
                vNegMin.X = vPos.X;
            if ( vPos.Y < vNegMin.Y )
                vNegMin.Y = vPos.Y;
            if ( vPos.Z < vNegMin.Z )
                vNegMin.Z = vPos.Z;
            if ( vPos.X > vNegMax.X )
                vNegMax.X = vPos.X;
            if ( vPos.Y > vNegMax.Y )
                vNegMax.Y = vPos.Y;
            if ( vPos.Z > vNegMax.Z )
                vNegMax.Z = vPos.Z;
        } else {
            arrPosAreas[iPosCount++] = pArea;
            if ( vPos.X < vPosMin.X )
                vPosMin.X = vPos.X;
            if ( vPos.Y < vPosMin.Y )
                vPosMin.Y = vPos.Y;
            if ( vPos.Z < vPosMin.Z )
                vPosMin.Z = vPos.Z;
            if ( vPos.X > vPosMax.X )
                vPosMax.X = vPos.X;
            if ( vPos.Y > vPosMax.Y )
                vPosMax.Y = vPos.Y;
            if ( vPos.Z > vPosMax.Z )
                vPosMax.Z = vPos.Z;
        }
    }
    Vertex3 vNegMid = ( vNegMin + vNegMax.ToVector() ) * 0.5f;
    Vertex3 vPosMid = ( vPosMin + vPosMax.ToVector() ) * 0.5f;

        // Negative branch
    if ( iNegCount > 0 ) {
        if ( iNegCount == 1 ) {
            EntityArea * pEntityArea = arrNegAreas[0];
            
            WorldNode * pAreaContainer = WorldFn->CreateNode( TEXT("AreaContainer_Neg") );
            pEntityArea->GenerateStructure( pAreaContainer );

            pNode->SetNegativeChild( pAreaContainer );
        } else {
            // Recurse
            WorldBSPNode * pNegativeChild = WorldFn->CreateBSPNode( TEXT("Negative") );
            pNegativeChild->SetCoplanarChild( NULL );
            pNegativeChild->SetDebugFlags( WORLD_DEBUGDRAW_BSPPLANES );

            if ( bSplitX ) // Toggle between X/Y
                pNegativeChild->WorldPlane = Plane( Vector3::eJ, vNegMid );
            else
                pNegativeChild->WorldPlane = Plane( Vector3::eI, vNegMid );
            
            _rec_GenerateBSPTree( pNegativeChild, !bSplitX, arrNegAreas, iNegCount );

            pNode->SetNegativeChild( pNegativeChild );
        }
    }

        // Positive branch
    if ( iPosCount > 0 ) {
        if ( iPosCount == 1 ) {
            EntityArea * pEntityArea = arrPosAreas[0];
            
            WorldNode * pAreaContainer = WorldFn->CreateNode( TEXT("AreaContainer_Pos") );
            pEntityArea->GenerateStructure( pAreaContainer );

            pNode->SetPositiveChild( pAreaContainer );
        } else {
            // Recurse
            WorldBSPNode * pPositiveChild = WorldFn->CreateBSPNode( TEXT("Positive") );
            pPositiveChild->SetCoplanarChild( NULL );
            pPositiveChild->SetDebugFlags( WORLD_DEBUGDRAW_BSPPLANES );

            if ( bSplitX ) // Toggle between X/Y
                pPositiveChild->WorldPlane = Plane( Vector3::eJ, vPosMid );
            else
                pPositiveChild->WorldPlane = Plane( Vector3::eI, vPosMid );
            
            _rec_GenerateBSPTree( pPositiveChild, !bSplitX, arrPosAreas, iPosCount );

            pNode->SetPositiveChild( pPositiveChild );
        }
    }
}
Void SAApplication::_GenerateBSPTree()
{
    // Extract all areas, compute min/max
    UInt iAreaCount = m_arrAreas.Count();
    EntityArea * arrAreas[SKYGRAPH_MAX_AREAS];

    Vertex3 vMin( SCALAR_INFINITE, SCALAR_INFINITE, SCALAR_INFINITE );
    Vertex3 vMax( -SCALAR_INFINITE, -SCALAR_INFINITE, -SCALAR_INFINITE );
    for( UInt i = 0; i < iAreaCount; ++i ) {
        arrAreas[i] = m_arrAreas[i];
        Vertex3 vPos = m_arrAreas[i]->GetArea()->GetPosition();
        if ( vPos.X < vMin.X )
            vMin.X = vPos.X;
        if ( vPos.Y < vMin.Y )
            vMin.Y = vPos.Y;
        if ( vPos.Z < vMin.Z )
            vMin.Z = vPos.Z;
        if ( vPos.X > vMax.X )
            vMax.X = vPos.X;
        if ( vPos.Y > vMax.Y )
            vMax.Y = vPos.Y;
        if ( vPos.Z > vMax.Z )
            vMax.Z = vPos.Z;
    }
    Vertex3 vMid = ( vMin + vMax.ToVector() ) * 0.5f;

    // Create root node
    m_pSkyGraphRoot = WorldFn->CreateBSPNode( TEXT("BSPTree_Root") );
    m_pSkyGraphRoot->SetCoplanarChild( NULL );
    m_pSkyGraphRoot->SetDebugFlags( WORLD_DEBUGDRAW_BSPPLANES );

    // Start splitting on X
    m_pSkyGraphRoot->WorldPlane = Plane( Vector3::eI, vMid );

    // Generate BSP Tree
    _rec_GenerateBSPTree( m_pSkyGraphRoot, true, arrAreas, iAreaCount );

    // Done
    WorldFn->AddChild( m_pSkyGraphRoot );
}
