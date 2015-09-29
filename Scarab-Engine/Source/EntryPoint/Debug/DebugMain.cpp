/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/Debug/DebugMain.cpp
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
// Includes
#include "DebugMain.h"

/////////////////////////////////////////////////////////////////////////////////
#if defined(DEBUGMAIN_SCRATCH)

/////////////////////////////////////////////////////////////////////////////////
// ScratchTests implementation
DefineConsoleApplication( ScratchTests );

ScratchTests::ScratchTests():
    ConsoleApplication( TEXT("Scratch Tests") )
{
    // nothing to do
}
ScratchTests::~ScratchTests()
{
    // nothing to do
}

Int ScratchTests::Main( const CommandLine & cmdLine )
{



    // Differential data math ... check that later, needed for patch engine ...

    //UInt iSizeS   = 1024;  // string to search in
    //UInt iSizeT   = 32;    // string to search for
    //Scalar fError = 0.25f; // Error threshold
    //Scalar fP     = 0.05f; // Probability of S being dependant (else unrelated to T and considered uniform random)
    //Scalar fT     = 6;     // Output index count required

    //Scalar fN = (Scalar)iSizeS;
    //Scalar fM = (Scalar)iSizeT;
    //Scalar fSqrtN = MathFn->Sqrt( fN );
    //Scalar fLnN = MathFn->Ln( fN );
    //Scalar fSqrP = ( fP * fP );
    //Scalar fLn_Err_NT =  MathFn->Ln( fError / (fN * fT) );

    //Scalar fA = MathFn->Exp( -0.125f * fM * fSqrP );
    //Scalar fB = MathFn->Exp( -0.125f * fM * fSqrP / (1.0f + fSqrtN) );
    //Scalar fC = 2.0f / fLn_Err_NT;
    //Scalar fSqrtDeltaA = MathFn->Sqrt( fA * fA + 2.0f * MathFn->Exp(-2.0f) );
    //Scalar fSqrtDeltaB = MathFn->Sqrt( fB * fB + 2.0f * MathFn->Exp(fC - 2.0f) );

    //// Constraints on initial values
    //Scalar fUpperBoundP = MathFn->Sqrt( fN / (4.0f * fT * fM * fLnN) );
    //Scalar fUpperBoundT = ( fN / (4.0f * fM * fSqrP * fLnN) );
    //Assert( fP < fUpperBoundP ); // => delta will be in ]0;1/4[
    //Assert( fT < fUpperBoundT ); // => delta will be in ]0;1/4[

    ////Scalar fLowerBoundT = MathFn->Sqr(fA + ) * fN / (4.0f * fM * fSqrP * fLnN);

    //// Compute & Clamp Delta
    //Scalar fDelta = ( fT * fM * fSqrP * fLnN ) / fN;
    //Assert( 0.0f < fDelta && fDelta < 0.25f ); // => ln(root2k(err/nt) - sqrt(delta)) is defined

    //Scalar fSqrtDelta = MathFn->Sqrt( fDelta );

    //Scalar fLowerBoundX = 0.5f * ( -fA - fSqrtDeltaA );
    //Scalar fUpperBoundX = 0.5f * ( -fA + fSqrtDeltaA );
    //Assert( fLowerBoundX < fSqrtDelta && fSqrtDelta < fUpperBoundX ); // => K & L defined

    //fLowerBoundX = 0.5f * ( -fB - fSqrtDeltaB );
    //fUpperBoundX = 0.5f * ( -fB + fSqrtDeltaB );
    //Assert( fSqrtDelta < fLowerBoundX || fUpperBoundX < fSqrtDelta ); // => K & L defined

    //// Compute & Clamp K
    //Scalar fK = fLn_Err_NT / MathFn->Ln( 4.0f * fDelta );
    //Int iK = MathFn->Ceil( fK );

    //Scalar fLowerBoundK = 0.5f * fLn_Err_NT * MathFn->Ln( MathFn->Exp(-fM*fSqrP/8.0f) + fSqrtDelta );
    //Scalar fUpperBoundK = 0.5f * fLn_Err_NT * MathFn->Ln( MathFn->Exp(-fM*fSqrP/(8.0f*(1.0f+fSqrtN))) + fSqrtDelta );
    //Assert( fLowerBoundK < fK && fK < fUpperBoundK );

    //// Compute & Clamp V
    //Scalar fRoot2K = MathFn->Exp( fLn_Err_NT / (Scalar)(iK << 1) ); // 2*k
    //Assert( fRoot2K > fSqrtDelta );

    //Scalar fV =  8.0f * MathFn->Ln( fRoot2K - fSqrtDelta );

    //Scalar fLowerBoundV = -0.5f * fM * fSqrP;
    //Scalar fUpperBoundV = -fM * fSqrP / (1.0f + fSqrtN);
    //Assert( fLowerBoundV < fV && fV < fUpperBoundV );

    //// Compute & Clamp L
    //Scalar fL = -( fN * fV ) / ( fM * fSqrP + fV );
    //Assert( fSqrtN < fL && fL < fN );
    //
    //// Prime interval to generate
    //Scalar fLowerBound = fL;                                      // inclusive
    //Scalar fUpperBound = fL * ( 1.0f + (2.0f / MathFn->Ln(fL)) ); // exclusive

    return 0;
}

/////////////////////////////////////////////////////////////////////////////////
#elif defined(DEBUGMAIN_ENGINE)

/////////////////////////////////////////////////////////////////////////////////
// TestBackboardModel implementation
TestBackboardModel::TestBackboardModel( EngineTests * pApplication ):
    GUIBackboardModel(), m_ptLastMouseDragPos()
{
	m_pApplication = pApplication;
}
TestBackboardModel::~TestBackboardModel()
{
	// nothing to do
}

Void TestBackboardModel::OnKeyPress( KeyCode iKey, GUIEventFlag iFlags )
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
        case ACTION_CONSOLE_INVOKE:
            ConsoleFn->Toggle();
            break;
        case ACTION_CONTROL_EXIT:
            m_pApplication->Exit();
            break;
        //case ACTION_CONTROL_ANIM_PAUSE:
        //    WorldFn->ToggleAnimation();
        //    break;
        //case ACTION_CONTROL_ANIM_STEP:
        //    WorldFn->StepAnimation();
        //    break;
        case ACTION_CONTROL_3D_WIREFRAME: {
                m_pApplication->m_bWireFrame = !( m_pApplication->m_bWireFrame );

                GPURasterizerState * pRasterState = m_pApplication->m_pDefaultEffect->GetRasterizerState(0,0);

                GPURasterizerStateDesc hDesc;
                pRasterState->GetDesc( &hDesc );
                pRasterState->UnBind();

                hDesc.iFillMode = (m_pApplication->m_bWireFrame) ? GPURASTERIZER_FILL_WIREFRAME : GPURASTERIZER_FILL_SOLID;
                pRasterState->Bind( &hDesc );
            } break;

        case ACTION_MOVE_RUN_FORWARD:
            m_pApplication->m_pCharacterController->SetMovingForward( true );
            break;
        case ACTION_MOVE_RUN_BACKWARD:
            m_pApplication->m_pCharacterController->SetMovingBackward( true );
            break;
        case ACTION_MOVE_RUN_LEFT:
            m_pApplication->m_pCharacterController->SetMovingLeft( true );
            break;
        case ACTION_MOVE_RUN_RIGHT:
            m_pApplication->m_pCharacterController->SetMovingRight( true );
            break;
        case ACTION_MOVE_RUN_UP:
            m_pApplication->m_pCharacterController->SetMovingUp( true );
            break;
        case ACTION_MOVE_RUN_DOWN:
            m_pApplication->m_pCharacterController->SetMovingDown( true );
            break;

        case ACTION_CUSTOM_A0: {
                JointHinge * pJoint = (JointHinge*)( m_pApplication->m_pJointAF->GetJoint() );
                if ( pJoint->HasMotor() )
                    pJoint->DisableMotor();
                else
                    pJoint->EnableServoMotor( SCALAR_PI_4, +SCALAR_PI_4, 10.0f );
            } break;
        case ACTION_CUSTOM_A1: {
                JointHinge * pJoint = (JointHinge*)( m_pApplication->m_pJointAF->GetJoint() );
                if ( pJoint->HasMotor() )
                    pJoint->DisableMotor();
                else
                    pJoint->EnableServoMotor( SCALAR_PI_4, -SCALAR_PI_4, 10.0f );
            } break;
        case ACTION_CUSTOM_B0: {
                JointSlider * pJoint = (JointSlider*)( m_pApplication->m_pJointBF->GetJoint() );
                if ( pJoint->HasMotor() )
                    pJoint->DisableMotor();
                else
                    pJoint->EnableServoMotor( 2.0f, +1.0f, 10.0f );
            } break;
        case ACTION_CUSTOM_B1: {
                JointSlider * pJoint = (JointSlider*)( m_pApplication->m_pJointBF->GetJoint() );
                if ( pJoint->HasMotor() )
                    pJoint->DisableMotor();
                else
                    pJoint->EnableServoMotor( 2.0f, -1.0f, 10.0f );
            } break;

        default:
            // do nothing
            break;
    }
}
Void TestBackboardModel::OnKeyRelease( KeyCode iKey, GUIEventFlag iFlags )
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
        case ACTION_MOVE_RUN_FORWARD:
            m_pApplication->m_pCharacterController->SetMovingForward( false );
            break;
        case ACTION_MOVE_RUN_BACKWARD:
            m_pApplication->m_pCharacterController->SetMovingBackward( false );
            break;
        case ACTION_MOVE_RUN_LEFT:
            m_pApplication->m_pCharacterController->SetMovingLeft( false );
            break;
        case ACTION_MOVE_RUN_RIGHT:
            m_pApplication->m_pCharacterController->SetMovingRight( false );
            break;
        case ACTION_MOVE_RUN_UP:
            m_pApplication->m_pCharacterController->SetMovingUp( false );
            break;
        case ACTION_MOVE_RUN_DOWN:
            m_pApplication->m_pCharacterController->SetMovingDown( false );
            break;

        default:
            // do nothing
            break;
    }
}

Void TestBackboardModel::OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags )
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

Void TestBackboardModel::OnStartDrag( const Point2 & ptLocalPos, KeyCode /*iKey*/, GUIEventFlag /*iFlags*/ )
{
    m_ptLastMouseDragPos = ptLocalPos;
}
Void TestBackboardModel::OnDragRight( const Point2 & ptLocalPos, GUIEventFlag /*iFlags*/ )
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

Void TestBackboardModel::OnScroll( const Point2 & /*ptLocalPos*/, Int iWheelDelta, GUIEventFlag iFlags )
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
// EngineTests implementation
DefineWindowApplication( EngineTests );

EngineTests::EngineTests():
    WindowApplication( TEXT("EngineTests"), PIXEL_FMT_RGBA8UI, 1024, 768, 0x80808080 ),
    m_hSunLight(), m_hMaterialFloor(), m_hMaterialCharacter(), m_hMaterialA(), m_hMaterialB(), m_hMaterialC(), m_hMaterialD()
{
    // Input
    m_iActionMap = INVALID_OFFSET;
    m_pBackboardModel = NULL;

    // Effects
    m_pDefaultEffect = NULL;
    m_pEffectInstanceFloor = NULL;
    m_pEffectInstanceCharacter = NULL;
    m_pEffectInstanceA = NULL;
    m_pEffectInstanceB = NULL;
    m_pEffectInstanceC = NULL;
    m_pEffectInstanceD = NULL;

    // Instanced Geometry
    m_pFloorGeometry = NULL;
    m_pFloorShape = NULL;

    m_pCharacterGeometry = NULL;
    m_pCharacterShape = NULL;

    m_pSphereGeometry = NULL;
    m_pSphereShape = NULL;

    m_pBoxGeometry = NULL;
    m_pBoxShape = NULL;

	// World
    m_pRenderCamera = NULL;
    m_pWorldCamera = NULL;

    m_pFloor = NULL;
    m_pCharacter = NULL;
    m_pCharacterController = NULL;

        // Stack tests
    for( UInt i = 0; i < ENGINE_TEST_STACK_SIZE; ++i ) {
        m_arrBallStack[i] = NULL;
        m_arrBoxStack[i] = NULL;
    }

        // Joint tests
    m_pFixedObject = NULL;
    m_pObjectA = NULL;
    m_pObjectB = NULL;

    m_pJointAF = NULL;
    m_pJointBF = NULL;

	// Debug
    m_bWireFrame = false;

	m_strFPSText[0] = NULLBYTE;
    m_strCoordsText[0] = NULLBYTE;
	m_strCullerText[0] = NULLBYTE;

    m_pDebugTextBrush = NULL;
    m_pDebugTextFormat = NULL;
}
EngineTests::~EngineTests()
{
    // nothing to do
}

Bool EngineTests::OnInitialize()
{
    // Initialize everything
    _CreateActions();
    _CreateEffects();
    _CreateInstancedGeometry();
    _CreateWorld();

    m_pBackboardModel = New TestBackboardModel( this );
    GUIFn->SetBackboardModel( m_pBackboardModel );

    // Debug
    WorldFn->ToggleDebugDraw( true );

    GPU2DColor hDebugTextColor = Color4::White;
    m_pDebugTextBrush = New GPU2DSolidColorBrush( RenderingFn->GetScreen2DContext() );
    m_pDebugTextBrush->SetColor( &hDebugTextColor );
    m_pDebugTextBrush->Bind();

    m_pDebugTextFormat = New GPU2DTextFormat();
    m_pDebugTextFormat->Bind( LOCALENAME_EN_US, GPU2DFONTFAMILY_ARIAL, GPU2DFONT_WEIGHT_NORMAL, GPU2DFONT_STYLE_NORMAL, GPU2DFONT_STRETCH_NORMAL, 10.0f );
    m_pDebugTextFormat->SetTextAlignment( GPU2DTEXT_ALIGNMENT_LEADING );
    m_pDebugTextFormat->SetParagraphAlignment( GPU2DTEXT_PARAGRAPHALIGNMENT_CENTER );

    m_pFloor->SetDebugFlags( WORLD_DEBUGDRAW_GIZMOS | WORLD_DEBUGDRAW_PROXIES | WORLD_DEBUGDRAW_JOINTS | WORLD_DEBUGDRAW_CONTACTS | WORLD_DEBUGDRAW_CONTACTPOINTS );
    m_pCharacter->SetDebugFlags( WORLD_DEBUGDRAW_GIZMOS | WORLD_DEBUGDRAW_PROXIES | WORLD_DEBUGDRAW_JOINTS | WORLD_DEBUGDRAW_CONTACTS | WORLD_DEBUGDRAW_CONTACTPOINTS );

    for( UInt i = 0; i < ENGINE_TEST_STACK_SIZE; ++i ) {
        if ( m_arrBallStack[i] != NULL )
            m_arrBallStack[i]->SetDebugFlags( WORLD_DEBUGDRAW_PROXIES | WORLD_DEBUGDRAW_JOINTS | WORLD_DEBUGDRAW_CONTACTS | WORLD_DEBUGDRAW_CONTACTPOINTS );
        if ( m_arrBoxStack[i] != NULL )
            m_arrBoxStack[i]->SetDebugFlags( WORLD_DEBUGDRAW_PROXIES | WORLD_DEBUGDRAW_JOINTS | WORLD_DEBUGDRAW_CONTACTS | WORLD_DEBUGDRAW_CONTACTPOINTS );
    }

    if ( m_pFixedObject != NULL )
        m_pFixedObject->SetDebugFlags( WORLD_DEBUGDRAW_GIZMOS | WORLD_DEBUGDRAW_PROXIES | WORLD_DEBUGDRAW_JOINTS | WORLD_DEBUGDRAW_CONTACTS | WORLD_DEBUGDRAW_CONTACTPOINTS );
    if ( m_pObjectA != NULL )
        m_pObjectA->SetDebugFlags( WORLD_DEBUGDRAW_GIZMOS | WORLD_DEBUGDRAW_PROXIES | WORLD_DEBUGDRAW_JOINTS | WORLD_DEBUGDRAW_CONTACTS | WORLD_DEBUGDRAW_CONTACTPOINTS );
    if ( m_pObjectB != NULL )
        m_pObjectB->SetDebugFlags( WORLD_DEBUGDRAW_GIZMOS | WORLD_DEBUGDRAW_PROXIES | WORLD_DEBUGDRAW_JOINTS | WORLD_DEBUGDRAW_CONTACTS | WORLD_DEBUGDRAW_CONTACTPOINTS );

    // Done
    return true;
}
Void EngineTests::OnCleanup()
{
    // Debug
    m_pDebugTextFormat->UnBind();
    Delete( m_pDebugTextFormat );
    m_pDebugTextFormat = NULL;

    m_pDebugTextBrush->UnBind();
    Delete( m_pDebugTextBrush );
    m_pDebugTextBrush = NULL;

    WorldFn->ToggleDebugDraw( false );

    // Cleanup everything
    GUIFn->SetBackboardModel( NULL );
    Delete( m_pBackboardModel );
    m_pBackboardModel = NULL;

    _DestroyWorld();
    _DestroyInstancedGeometry();
    _DestroyEffects();
    _DestroyActions();
}

Bool EngineTests::OnFrameMove()
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

    // Done
	return true;
}
Void EngineTests::OnFrameRender()
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

Void EngineTests::OnExit()
{
    GUIFn->OnExit();
}

Bool EngineTests::OnKeyPress( KeyCode iKey )
{
    GUIFn->OnKeyPress( iKey );

    return true;
}
Bool EngineTests::OnKeyRelease( KeyCode iKey )
{
    GUIFn->OnKeyRelease( iKey );

    return true;
}

Void EngineTests::OnMouseMove( UInt iX, UInt iY )
{
    GUIFn->OnMouseMove( Point2(iX, iY) );
}
Bool EngineTests::OnMouseWheel( UInt iX, UInt iY, Int iWheelDelta )
{
    GUIFn->OnMouseWheel( Point2(iX, iY), iWheelDelta );

    return true;
}
Bool EngineTests::OnMousePress( UInt iX, UInt iY, KeyCode iButton )
{
    GUIFn->OnMousePress( Point2(iX, iY), iButton );

    return true;
}
Bool EngineTests::OnMouseRelease( UInt iX, UInt iY, KeyCode iButton )
{
    GUIFn->OnMouseRelease( Point2(iX, iY), iButton );

    return true;
}
Bool EngineTests::OnMouseDblClick( UInt iX, UInt iY, KeyCode iButton )
{
    GUIFn->OnMouseDblClick( Point2(iX, iY), iButton );

    return true;
}

Void EngineTests::_CreateActions()
{
    // Action map
    m_iActionMap = InputFn->CreateMap();
    InputFn->SelectMap( m_iActionMap );

    InputFn->BindAction( ACTION_CONSOLE_INVOKE,       KEYCODE_OEM_7 );

    InputFn->BindAction( ACTION_CONTROL_EXIT,         KEYCODE_ESCAPE );
    //InputFn->BindAction( ACTION_CONTROL_ANIM_PAUSE,   KEYCODE_P );
    //InputFn->BindAction( ACTION_CONTROL_ANIM_STEP,    KEYCODE_O );
    InputFn->BindAction( ACTION_CONTROL_3D_WIREFRAME, KEYCODE_I );

    InputFn->BindAction( ACTION_MOVE_RUN_FORWARD,     KEYCODE_Z );
    InputFn->BindAction( ACTION_MOVE_RUN_BACKWARD,    KEYCODE_S );
    InputFn->BindAction( ACTION_MOVE_RUN_LEFT,        KEYCODE_Q );
    InputFn->BindAction( ACTION_MOVE_RUN_RIGHT,       KEYCODE_D );
    InputFn->BindAction( ACTION_MOVE_RUN_UP,          KEYCODE_E );
    InputFn->BindAction( ACTION_MOVE_RUN_DOWN,        KEYCODE_A );
    InputFn->BindAction( ACTION_CAMERA_ZOOM_OUT,      KEYCODE_MOUSEWHEELDOWN );
    InputFn->BindAction( ACTION_CAMERA_ZOOM_IN,       KEYCODE_MOUSEWHEELUP );

    InputFn->BindAction( ACTION_CUSTOM_A0,            KEYCODE_UP );
    InputFn->BindAction( ACTION_CUSTOM_A1,            KEYCODE_DOWN );
    InputFn->BindAction( ACTION_CUSTOM_B0,            KEYCODE_UP, ACTIONFLAG_MOD_SHIFT );
    InputFn->BindAction( ACTION_CUSTOM_B1,            KEYCODE_DOWN, ACTIONFLAG_MOD_SHIFT );
}
Void EngineTests::_DestroyActions()
{
    // Action map
    InputFn->DestroyMap( m_iActionMap );
    m_iActionMap = INVALID_OFFSET;
}

Void EngineTests::_CreateEffects()
{
    m_hSunLight.SetType( LIGHT_OMNI );
    m_hSunLight.Diffuse = Color4::White;
    m_hSunLight.Position = Vertex3( 0.0f, 0.0f, 50.0f );

    m_hMaterialFloor.Diffuse = Color4::Cyan;
    m_hMaterialCharacter.Diffuse = Color4::Cyan;
    m_hMaterialA.Diffuse = Color4::Blue;
    m_hMaterialB.Diffuse = Color4::Green;
    m_hMaterialC.Diffuse = Color4::Yellow;
    m_hMaterialD.Diffuse = Color4::Red;

    m_pDefaultEffect = New EffectLightMaterial();

    m_pEffectInstanceFloor = m_pDefaultEffect->CreateInstance( EFFECTLIGHT_TECHNIQUE_POINT_PHONG, &m_hSunLight, &m_hMaterialFloor );
    m_pEffectInstanceCharacter = m_pDefaultEffect->CreateInstance( EFFECTLIGHT_TECHNIQUE_POINT_PHONG, &m_hSunLight, &m_hMaterialCharacter );
    m_pEffectInstanceA = m_pDefaultEffect->CreateInstance( EFFECTLIGHT_TECHNIQUE_POINT_PHONG, &m_hSunLight, &m_hMaterialA );
    m_pEffectInstanceB = m_pDefaultEffect->CreateInstance( EFFECTLIGHT_TECHNIQUE_POINT_PHONG, &m_hSunLight, &m_hMaterialB );
    m_pEffectInstanceC = m_pDefaultEffect->CreateInstance( EFFECTLIGHT_TECHNIQUE_POINT_PHONG, &m_hSunLight, &m_hMaterialC );
    m_pEffectInstanceD = m_pDefaultEffect->CreateInstance( EFFECTLIGHT_TECHNIQUE_POINT_PHONG, &m_hSunLight, &m_hMaterialD );
}
Void EngineTests::_DestroyEffects()
{
    m_pDefaultEffect->DestroyInstance( m_pEffectInstanceFloor );
    m_pEffectInstanceFloor = NULL;
    m_pDefaultEffect->DestroyInstance( m_pEffectInstanceCharacter );
    m_pEffectInstanceCharacter = NULL;
    m_pDefaultEffect->DestroyInstance( m_pEffectInstanceA );
    m_pEffectInstanceA = NULL;
    m_pDefaultEffect->DestroyInstance( m_pEffectInstanceB );
    m_pEffectInstanceB = NULL;
    m_pDefaultEffect->DestroyInstance( m_pEffectInstanceC );
    m_pEffectInstanceC = NULL;
    m_pDefaultEffect->DestroyInstance( m_pEffectInstanceD );
    m_pEffectInstanceD = NULL;

    Delete( m_pDefaultEffect );
    m_pDefaultEffect = NULL;
}

Void EngineTests::_CreateInstancedGeometry()
{
    GPUInputLayout * pIL = m_pDefaultEffect->GetInputLayout( EFFECTLIGHT_TECHNIQUE_POINT_PHONG, 0 );

    // Floor
    Box vFloor( Vertex3::Null, Vector3(100.0f,100.0f,5.0f) );
    m_pFloorGeometry = New MeshBox( vFloor, pIL );
    m_pFloorShape = PhysicsFn->CreateShapeBox( m_pFloorGeometry );

    // Character
    Sphere vCharacter( Vertex3::Null, 1.0f );
    m_pCharacterGeometry = New MeshSphere( vCharacter, 16, 16, pIL );
    m_pCharacterShape = PhysicsFn->CreateShapeSphere( m_pCharacterGeometry );

    // Sphere
    Sphere vSphere( Vertex3::Null, 1.0f );
    m_pSphereGeometry = New MeshSphere( vSphere, 16, 16, pIL );
    m_pSphereShape = PhysicsFn->CreateShapeSphere( m_pSphereGeometry );

    // Box
    Box vBox( Vertex3::Null, Vector3(1.0f,1.0f,1.0f) );
    m_pBoxGeometry = New MeshBox( vBox, pIL );
    m_pBoxShape = PhysicsFn->CreateShapeBox( m_pBoxGeometry );
}
Void EngineTests::_DestroyInstancedGeometry()
{
    // Box
    PhysicsFn->DestroyShape( m_pBoxShape );
    m_pBoxShape = NULL;

    Delete( m_pBoxGeometry );
    m_pBoxGeometry = NULL;

    // Sphere
    PhysicsFn->DestroyShape( m_pSphereShape );
    m_pSphereShape = NULL;

    Delete( m_pSphereGeometry );
    m_pSphereGeometry = NULL;

    // Character
    PhysicsFn->DestroyShape( m_pCharacterShape );
    m_pCharacterShape = NULL;

    Delete( m_pCharacterGeometry );
    m_pCharacterGeometry = NULL;

    // Floor
    PhysicsFn->DestroyShape( m_pFloorShape );
    m_pFloorShape = NULL;

    Delete( m_pFloorGeometry );
    m_pFloorGeometry = NULL;
}

Void EngineTests::_CreateWorld()
{
    // Floor
    RigidBody * pFloorBody = PhysicsFn->CreateRigidBody( true, m_pFloorShape, 1.0f, Vertex3(0.0f,0.0f,-10.0f) );
    pFloorBody->SetRestitution( 0.0f );
    pFloorBody->SetFriction( 0.0f );
    pFloorBody->SetRollingFriction( 0.0f );
    pFloorBody->SetCollisionGroup( 0x01 );
    pFloorBody->SetCollisionMask( 0x7f );

    m_pFloor = WorldFn->CreateLeaf( TEXT("Floor") );
    m_pFloor->SetMesh( m_pFloorGeometry );
    m_pFloor->SetEffectInstance( m_pEffectInstanceFloor );
    m_pFloor->SetBody( pFloorBody );
    WorldFn->AddChild( m_pFloor );

    // Character
    Vertex3 vInitialPosition( 0.0f, 0.0f, 2.0f );

    KinematicBody * pCharacterBody = PhysicsFn->CreateKinematicBody( false, m_pCharacterShape, 1.0f );
    pCharacterBody->SetRestitution( 0.0f );
    pCharacterBody->SetFriction( 0.0f );
    pCharacterBody->SetRollingFriction( 0.0f );
    pCharacterBody->SetCollisionGroup( 0x01 );
    pCharacterBody->SetCollisionMask( 0x07 );

    m_pCharacterController = PhysicsFn->CreateCharacterController( TEXT("CharacterController"), vInitialPosition, Quaternion::Identity, Vector3::Null, Vector3::Null );
    m_pCharacterController->Enabled = true;
    m_pCharacterController->EnableForces( false );
    m_pCharacterController->SetMovementSpeed( 5.0f );
    pCharacterBody->AttachController( m_pCharacterController );

    m_pCharacter = WorldFn->CreateLeaf( TEXT("Character") );
    m_pCharacter->SetMesh( m_pCharacterGeometry );
    m_pCharacter->SetEffectInstance( m_pEffectInstanceCharacter );
    m_pCharacter->SetBody( pCharacterBody );
    WorldFn->AddChild( m_pCharacter );

    // Sphere/Box stack
    Vertex3 vBallStackPosition( -10.0f, 10.0f, 0.0f );
    Vertex3 vBoxStackPosition( -10.0f, -10.0f, 0.0f );
    GChar strName[64];

    for( UInt i = 0; i < ENGINE_TEST_STACK_SIZE; ++i ) {
        vBallStackPosition.Z = 5.0f * (Scalar)(i+1);
        vBoxStackPosition.Z = 5.0f * (Scalar)(i+1);

        //StringFn->Format( strName, TEXT("Ball_%d"), i );

        //RigidBody * pBallBody = PhysicsFn->CreateRigidBody( false, m_pSphereShape, 1.0f, vBallStackPosition );
        //pBallBody->SetRestitution( 0.0f );
        //pBallBody->SetFriction( 0.0f );
        //pBallBody->SetRollingFriction( 0.0f );

        //m_arrBallStack[i] = WorldFn->CreateLeaf( strName );
        //m_arrBallStack[i]->SetMesh( m_pSphereGeometry );
        //m_arrBallStack[i]->SetEffectInstance( m_pEffectInstanceA );
        //m_arrBallStack[i]->SetBody( pBallBody );
        //WorldFn->AddChild( m_arrBallStack[i] );

        StringFn->Format( strName, TEXT("Box_%d"), i );

        RigidBody * pBoxBody = PhysicsFn->CreateRigidBody( false, m_pBoxShape, 1.0f, vBoxStackPosition );
        pBoxBody->SetRestitution( 0.0f );
        pBoxBody->SetFriction( 0.0f );
        pBoxBody->SetRollingFriction( 0.0f );

        m_arrBoxStack[i] = WorldFn->CreateLeaf( strName );
        m_arrBoxStack[i]->SetMesh( m_pBoxGeometry );
        m_arrBoxStack[i]->SetEffectInstance( m_pEffectInstanceA );
        m_arrBoxStack[i]->SetBody( pBoxBody );
        WorldFn->AddChild( m_arrBoxStack[i] );
    }

    // Joint systems
    //Matrix3 matRotation;
    //matRotation.MakeRotate( 0.0f, SCALAR_PI_4, SCALAR_PI_4, EULER_ANGLES_XYZ );

    //Vertex3 vFixedObjectPosition( 10.0f, 10.0f, 10.0f );
    //Vertex3 vObjectAPosition( 12.0f, 10.0f, 10.0f );
    //Quaternion qObjectAOrientation( matRotation );
    //Vertex3 vObjectBPosition( 7.0f, 10.0f, 10.0f );

    //RigidBody * pFixedBody = PhysicsFn->CreateRigidBody( true, m_pSphereShape, 1.0f, vFixedObjectPosition );
    //pFixedBody->SetRestitution( 0.0f );
    //pFixedBody->SetFriction( 0.0f );
    //pFixedBody->SetRollingFriction( 0.0f );
    //pFixedBody->SetCollisionGroup( 0x80 );
    //pFixedBody->SetCollisionMask( 0 );

    //m_pFixedObject = WorldFn->CreateLeaf( TEXT("FixedObject") );
    //m_pFixedObject->SetMesh( m_pSphereGeometry );
    //m_pFixedObject->SetEffectInstance( m_pEffectInstanceC );
    //m_pFixedObject->SetBody( pFixedBody );
    //WorldFn->AddChild( m_pFixedObject );

    //RigidBody * pObjectABody = PhysicsFn->CreateRigidBody( false, m_pBoxShape, 1.0f, vObjectAPosition ); //, qObjectAOrientation );
    //pObjectABody->SetRestitution( 0.0f );
    //pObjectABody->SetFriction( 0.0f );
    //pObjectABody->SetRollingFriction( 0.0f );
    //pObjectABody->SetCollisionGroup( 0x02 );
    //pObjectABody->SetCollisionMask( 0x03 );

    //m_pObjectA = WorldFn->CreateLeaf( TEXT("ObjectA") );
    //m_pObjectA->SetMesh( m_pBoxGeometry );
    //m_pObjectA->SetEffectInstance( m_pEffectInstanceA );
    //m_pObjectA->SetBody( pObjectABody );
    //WorldFn->AddChild( m_pObjectA );

    //RigidBody * pObjectBBody = PhysicsFn->CreateRigidBody( false, m_pBoxShape, 1.0f, vObjectBPosition );
    //pObjectBBody->SetRestitution( 0.0f );
    //pObjectBBody->SetFriction( 0.0f );
    //pObjectBBody->SetRollingFriction( 0.0f );
    //pObjectBBody->SetCollisionGroup( 0x04 );
    //pObjectBBody->SetCollisionMask( 0x05 );

    //m_pObjectB = WorldFn->CreateLeaf( TEXT("ObjectB") );
    //m_pObjectB->SetMesh( m_pBoxGeometry );
    //m_pObjectB->SetEffectInstance( m_pEffectInstanceB );
    //m_pObjectB->SetBody( pObjectBBody );
    //WorldFn->AddChild( m_pObjectB );

    //Transform3 vJointFrame;

    //Matrix3 matJointFrame;
    //matJointFrame.SetColumn( 0, Vector3::eJ );
    //matJointFrame.SetColumn( 1, Vector3::eK );
    //matJointFrame.SetColumn( 2, Vector3::eI );
    //vJointFrame.SetRotate( matJointFrame );
    //vJointFrame.SetTranslate( Vector3(11.0f, 10.0f, 10.0f) );

    //m_pJointAF = PhysicsFn->CreateJoint( JOINT_CONETWIST, pObjectABody, pFixedBody, vJointFrame );
    //( (JointConeTwist*)(m_pJointAF->GetJoint()) )->EnableTwistLimits( -SCALAR_PI_4, SCALAR_PI_4 );
    //( (JointHinge*)(m_pJointAF->GetJoint()) )->EnableSpring( 0.5f, SCALAR_PI_4 );

    //vJointFrame.SetTranslate( Vector3(9.0f, 10.0f, 10.0f) );

    //m_pJointBF = PhysicsFn->CreateJoint( JOINT_SLIDER, pObjectBBody, pFixedBody, vJointFrame );
    //( (JointSlider*)(m_pJointBF->GetJoint()) )->EnableLimits( -2.0f, +2.0f );
    //( (JointSlider*)(m_pJointBF->GetJoint()) )->EnableSpring( 0.5f, -1.0f );

    // World camera
    m_pRenderCamera = New Camera( true );
    m_pWorldCamera = New WorldCamera3rdPerson( m_pRenderCamera, m_pCharacter, NULL, 3.0f );
    WorldFn->SetWorldCamera( m_pWorldCamera );
}
Void EngineTests::_DestroyWorld()
{
    // World camera
    WorldFn->SetWorldCamera( NULL );
    Delete( m_pWorldCamera );
    m_pWorldCamera = NULL;
    Delete( m_pRenderCamera );
    m_pRenderCamera = NULL;

    // Joint systems
    //PhysicsFn->DestroyJoint( m_pJointBF );
    //m_pJointBF = NULL;
    //PhysicsFn->DestroyJoint( m_pJointAF );
    //m_pJointAF = NULL;

    //PhysicsFn->DestroyBody( m_pObjectB->GetBody() );
    //m_pObjectB->SetBody( NULL );
    //m_pObjectB->SetEffectInstance( NULL );
    //m_pObjectB->SetMesh( NULL );

    //WorldFn->RemoveChild( TEXT("ObjectB") );
    //WorldFn->DestroyNode( m_pObjectB );
    //m_pObjectB = NULL;

    //PhysicsFn->DestroyBody( m_pObjectA->GetBody() );
    //m_pObjectA->SetBody( NULL );
    //m_pObjectA->SetEffectInstance( NULL );
    //m_pObjectA->SetMesh( NULL );

    //WorldFn->RemoveChild( TEXT("ObjectA") );
    //WorldFn->DestroyNode( m_pObjectA );
    //m_pObjectA = NULL;

    //PhysicsFn->DestroyBody( m_pFixedObject->GetBody() );
    //m_pFixedObject->SetBody( NULL );
    //m_pFixedObject->SetEffectInstance( NULL );
    //m_pFixedObject->SetMesh( NULL );

    //WorldFn->RemoveChild( TEXT("FixedObject") );
    //WorldFn->DestroyNode( m_pFixedObject );
    //m_pFixedObject = NULL;

    // Sphere/Box stack
    GChar strName[64];

    for( UInt i = 0; i < ENGINE_TEST_STACK_SIZE; ++i ) {
        StringFn->Format( strName, TEXT("Box_%d"), i );

        PhysicsFn->DestroyBody( m_arrBoxStack[i]->GetBody() );
        m_arrBoxStack[i]->SetBody( NULL );
        m_arrBoxStack[i]->SetEffectInstance( NULL );
        m_arrBoxStack[i]->SetMesh( NULL );

        WorldFn->RemoveChild( strName );
        WorldFn->DestroyNode( m_arrBoxStack[i] );
        m_arrBoxStack[i] = NULL;

        //StringFn->Format( strName, TEXT("Ball_%d"), i );

        //PhysicsFn->DestroyBody( m_arrBallStack[i]->GetBody() );
        //m_arrBallStack[i]->SetBody( NULL );
        //m_arrBallStack[i]->SetEffectInstance( NULL );
        //m_arrBallStack[i]->SetMesh( NULL );

        //WorldFn->RemoveChild( strName );
        //WorldFn->DestroyNode( m_arrBallStack[i] );
        //m_arrBallStack[i] = NULL;
    }

    // Character
    ((KinematicBody*)m_pCharacter->GetBody())->DetachMotionController();
    PhysicsFn->DestroyController( m_pCharacterController );
    m_pCharacterController = NULL;

    PhysicsFn->DestroyBody( m_pCharacter->GetBody() );
    m_pCharacter->SetBody( NULL );
    m_pCharacter->SetEffectInstance( NULL );
    m_pCharacter->SetMesh( NULL );

    WorldFn->RemoveChild( TEXT("Character") );
    WorldFn->DestroyNode( m_pCharacter );
    m_pCharacter = NULL;

    // Floor
    PhysicsFn->DestroyBody( m_pFloor->GetBody() );
    m_pFloor->SetBody( NULL );
    m_pFloor->SetEffectInstance( NULL );
    m_pFloor->SetMesh( NULL );

    WorldFn->RemoveChild( TEXT("Floor") );
    WorldFn->DestroyNode( m_pFloor );
    m_pFloor = NULL;

}

/////////////////////////////////////////////////////////////////////////////////
#endif // DEBUGMAIN_SCRATCH, DEBUGMAIN_ENGINE


