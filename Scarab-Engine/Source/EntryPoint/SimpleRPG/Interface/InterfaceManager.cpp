/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Interface/InterfaceManager.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Interface : InterfaceManager
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
#include "InterfaceManager.h"

/////////////////////////////////////////////////////////////////////////////////
// HUDBackboardModel implementation
HUDBackboardModel::HUDBackboardModel():
    GUIBackboardModel(),
    m_vMoveX(), m_vMoveY(), m_vMoveZ(), m_ptLastMouseDragPos()
{
	m_pMenuModel = NULL;

    m_bMovingX = false;
    m_bMovingY = false;
    m_bMovingZ = false;
    m_vMoveX = Vector3::Null;
    m_vMoveY = Vector3::Null;
    m_vMoveZ = Vector3::Null;
}
HUDBackboardModel::~HUDBackboardModel()
{
	if ( m_pMenuModel != NULL ) {
		SetContextMenu( NULL );
		Delete( m_pMenuModel );
	}
}

Void HUDBackboardModel::OnKeyPress( KeyCode iKey, GUIEventFlag iFlags )
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
    PCharacter * pPlayer = (PCharacter*)( GameplayFn->GetCharacter(TEXT("Shiki")) );

    const Camera * pRenderCamera = RenderingFn->GetCamera();

    // Compute camera frame
    Vector3 vAim = pRenderCamera->GetViewDir();
    vAim.Z = 0.0f;
    vAim.Normalize();
    Vector3 vSide = pRenderCamera->GetViewSide();
    Vector3 vUp = ( vAim ^ vSide );
    vUp.Normalize();

	// Action mapping
    Action iAction = InputFn->GetAction( iKey, iActionFlags );
    switch( iAction ) {
        case ACTION_CONSOLE_INVOKE:
            ConsoleFn->ToggleVideoSettings();
            break;
        case ACTION_CONTROL_EXIT:
            GUIFn->MessageBox( GUIMSGBOX_TYPE_WARNING, TEXT("Are you sure you want to exit ?\nThat's what you want ?!\nDo as you wish ... TT"),
                               GUIMSGBOX_BUTTONS_YES_NO, _MsgBox_ConfirmExit, this );
            break;
        //case ACTION_CONTROL_ANIM_PAUSE:
        //    WorldFn->ToggleAnimation();
        //    break;
        //case ACTION_CONTROL_ANIM_STEP:
        //    WorldFn->StepAnimation();
        //    break;
        case ACTION_CONTROL_3D_WIREFRAME:
			GraphicsFn->ToggleWireframe();
			break;

        case ACTION_MOVE_RUN_FORWARD:
            pPlayer->SetMovingForward( true );
            break;
        case ACTION_MOVE_RUN_BACKWARD:
            pPlayer->SetMovingBackward( true );
            break;
        case ACTION_MOVE_RUN_LEFT:
            pPlayer->SetMovingLeft( true );
            break;
        case ACTION_MOVE_RUN_RIGHT:
            pPlayer->SetMovingRight( true );
            break;
        case ACTION_MOVE_RUN_UP:
            pPlayer->SetMovingUp( true );
            break;
        case ACTION_MOVE_RUN_DOWN:
            pPlayer->SetMovingDown( true );
            break;

        case ACTION_CUSTOM_A0:
            pPlayer->Cast( SKILLID_MAGE_FIREBOLT, pPlayer->GetSelectedTarget() );
            break;
        case ACTION_CUSTOM_A1:
            pPlayer->Cast( SKILLID_MAGE_ICENOVA, NULL );
            break;

        case ACTION_CUSTOM_A7:
            if ( pPlayer->GetSelectedTarget() != NULL )
                pPlayer->StartPathTracking( pPlayer->GetSelectedTarget()->GetPosition() );
            break;

        case ACTION_CUSTOM_B0:
            pPlayer->SelectNextNearestTarget();
            break;
        case ACTION_CUSTOM_B1:
            pPlayer->FocusTarget( pPlayer->GetSelectedTarget() );
            break;
        case ACTION_CUSTOM_B2:
            pPlayer->SwapFocusTarget();
            break;

        case ACTION_CUSTOM_B4:
            InterfaceFn->ToggleCharacterView();
            break;
        case ACTION_CUSTOM_B5:
            InterfaceFn->ToggleSkillBookView();
            break;
        //case ACTION_CUSTOM_B6:
        //    break;
        case ACTION_CUSTOM_B7:
            InterfaceFn->ToggleInventoryView();
            break;

        default:
            // do nothing
            break;
    }
}
Void HUDBackboardModel::OnKeyRelease( KeyCode iKey, GUIEventFlag iFlags )
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
    PCharacter * pPlayer = (PCharacter*)( GameplayFn->GetCharacter(TEXT("Shiki")) );

	// Action mapping
    Action iAction = InputFn->GetAction( iKey, iActionFlags );
    switch( iAction ) {
        case ACTION_MOVE_RUN_FORWARD:
            pPlayer->SetMovingForward( false );
            break;
        case ACTION_MOVE_RUN_BACKWARD:
            pPlayer->SetMovingBackward( false );
            break;
        case ACTION_MOVE_RUN_LEFT:
            pPlayer->SetMovingLeft( false );
            break;
        case ACTION_MOVE_RUN_RIGHT:
            pPlayer->SetMovingRight( false );
            break;
        case ACTION_MOVE_RUN_UP:
            pPlayer->SetMovingUp( false );
            break;
        case ACTION_MOVE_RUN_DOWN:
            pPlayer->SetMovingDown( false );
            break;
        default:
            // do nothing
            break;
    }
}

Void HUDBackboardModel::OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags )
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
    PCharacter * pPlayer = (PCharacter*)( GameplayFn->GetCharacter(TEXT("Shiki")) );

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
            if ( pLeaf != NULL && (pLeaf->GetParent()->GetType() != WORLDNODE_OUTDOOR_REGION) )
                pPlayer->SelectTarget( ((CharEntity*)pLeaf)->GetCharacter() );
            else
                pPlayer->SelectTarget( NULL );
        }
    } else if ( iKey == KEYCODE_MOUSERIGHT ) {
        if ( iActionFlags == ACTIONFLAG_NONE ) {
            // Right click
            if ( pLeaf != NULL && (pLeaf->GetParent()->GetType() != WORLDNODE_OUTDOOR_REGION) ) {
                pPlayer->SelectTarget( ((CharEntity*)pLeaf)->GetCharacter() );
                pPlayer->Attack( pPlayer->GetSelectedTarget(), true, false );
            }
        }
    }
}

Void HUDBackboardModel::OnStartDrag( const Point2 & ptLocalPos, KeyCode /*iKey*/, GUIEventFlag /*iFlags*/ )
{
    m_ptLastMouseDragPos = ptLocalPos;
}
Void HUDBackboardModel::OnDragRight( const Point2 & ptLocalPos, GUIEventFlag /*iFlags*/ )
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
Void HUDBackboardModel::OnStopDrag( const Point2 & /*ptLocalPos*/, KeyCode iKey, GUIEventFlag /*iFlags*/ )
{
    if ( iKey == KEYCODE_MOUSELEFT ) {
        // nothing to do

        GUIFn->SetDragData( GUIDRAG_LEFT, NULL );

        return;
    }
}

Void HUDBackboardModel::OnScroll( const Point2 & /*ptLocalPos*/, Int iWheelDelta, GUIEventFlag iFlags )
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

Void HUDBackboardModel::_MsgBox_ConfirmExit( GUIMessageBoxResult iResult, Void * /*pUserData*/ )
{
    if ( iResult == GUIMSGBOX_RESULT_TRUE )
        ((WindowApplication*)ApplicationFn->GetInstance())->Exit();
}

/////////////////////////////////////////////////////////////////////////////////
// InterfaceManager implementation
InterfaceManager::InterfaceManager():
    Manager<InterfaceManager>(),
    m_hColorBrush( GUIFn->GetScreenContext() ), m_hTextBrush( GUIFn->GetScreenContext() ), m_hTextFormat(), m_hDragNDrop()
{
    // nothing to do
}
InterfaceManager::~InterfaceManager()
{
    // nothing to do
}

Void InterfaceManager::Initialize()
{
    // HUD bitmaps
    m_pSkinBitmap = GraphicsFn->GetBitmap( TEXT("Bitmap_HUD_Skin") );
    m_pStatusEffectsBitmap = GraphicsFn->GetBitmap( TEXT("Bitmap_HUD_StatusEffects") );
    m_pSkillsBitmap = GraphicsFn->GetBitmap( TEXT("Bitmap_HUD_Skills") );
    m_pItemsBitmap = GraphicsFn->GetBitmap( TEXT("Bitmap_HUD_Items") );

    // Initialize HUD locations
    m_arrHUDLocations[HUD_IMAGE_BACKGROUND_HPBAR] = HUD_LOCATION_BACKGROUND_HPBAR;
    m_arrHUDLocations[HUD_IMAGE_FOREGROUND_HPBAR] = HUD_LOCATION_FOREGROUND_HPBAR;
    m_arrHUDLocations[HUD_IMAGE_BACKGROUND_MPBAR] = HUD_LOCATION_BACKGROUND_MPBAR;
    m_arrHUDLocations[HUD_IMAGE_FOREGROUND_MPBAR] = HUD_LOCATION_FOREGROUND_MPBAR;

    m_arrHUDLocations[HUD_IMAGE_BACKGROUND_CASTBAR] = HUD_LOCATION_BACKGROUND_CASTBAR;
    m_arrHUDLocations[HUD_IMAGE_FOREGROUND_CASTBAR] = HUD_LOCATION_FOREGROUND_CASTBAR;

    m_arrHUDLocations[HUD_IMAGE_BACKGROUND_STATUSEFFECTBAR] = HUD_LOCATION_BACKGROUND_STATUSEFFECTBAR;
    m_arrHUDLocations[HUD_IMAGE_FOREGROUND_STATUSEFFECTBAR] = HUD_LOCATION_FOREGROUND_STATUSEFFECTBAR;

    m_arrHUDLocations[HUD_IMAGE_BACKGROUND_SKILLBAR] = HUD_LOCATION_BACKGROUND_SKILLBAR;
    m_arrHUDLocations[HUD_IMAGE_FOREGROUND_SKILLBAR_GCD] = HUD_LOCATION_FOREGROUND_SKILLBAR_GCD;
    m_arrHUDLocations[HUD_IMAGE_FOREGROUND_SKILLBAR_CD] = HUD_LOCATION_FOREGROUND_SKILLBAR_CD;

    m_arrHUDLocations[HUD_IMAGE_BACKGROUND_CHARACTERVIEW] = HUD_LOCATION_BACKGROUND_CHARACTERVIEW;

    m_arrHUDLocations[HUD_IMAGE_BACKGROUND_SKILLBOOKVIEW] = HUD_LOCATION_BACKGROUND_SKILLBOOKVIEW;
    
    m_arrHUDLocations[HUD_IMAGE_BACKGROUND_INVENTORYVIEW] = HUD_LOCATION_BACKGROUND_INVENTORYVIEW;
    m_arrHUDLocations[HUD_IMAGE_FOREGROUND_INVENTORYVIEW] = HUD_LOCATION_FOREGROUND_INVENTORYVIEW;
    m_arrHUDLocations[HUD_IMAGE_FOREGROUND_INVENTORYVIEW_BAGBUTTON_TRASH] = HUD_LOCATION_FOREGROUND_INVENTORYVIEW_BAGBUTTON_TRASH;
    m_arrHUDLocations[HUD_IMAGE_FOREGROUND_INVENTORYVIEW_BAGBUTTON_CONSUMMABLE] = HUD_LOCATION_FOREGROUND_INVENTORYVIEW_BAGBUTTON_CONSUMMABLE;
    m_arrHUDLocations[HUD_IMAGE_FOREGROUND_INVENTORYVIEW_BAGBUTTON_EQUIPMENT] = HUD_LOCATION_FOREGROUND_INVENTORYVIEW_BAGBUTTON_EQUIPMENT;
    m_arrHUDLocations[HUD_IMAGE_FOREGROUND_INVENTORYVIEW_BAGBUTTON_QUEST] = HUD_LOCATION_FOREGROUND_INVENTORYVIEW_BAGBUTTON_QUEST;
    m_arrHUDLocations[HUD_IMAGE_FOREGROUND_INVENTORYVIEW_BAGBUTTON_BUFFER] = HUD_LOCATION_FOREGROUND_INVENTORYVIEW_BAGBUTTON_BUFFER;
    m_arrHUDLocations[HUD_IMAGE_FOREGROUND_INVENTORYVIEW_ACTIVEBAGBUTTON] = HUD_LOCATION_FOREGROUND_INVENTORYVIEW_ACTIVEBAGBUTTON;

    // Initialize status effect icons locations
    m_arrStatusEffectIcons[STATUSEFFECTID_CHILL] = HUDICON_LOCATION_STATUSEFFECT_CHILL;

    // Initialize skill icons locations
    m_arrSkillIcons[SKILLID_MAGE_FIREBOLT] = HUDICON_LOCATION_SKILL_MAGE_FIREBOLT;
    m_arrSkillIcons[SKILLID_MAGE_ICENOVA] = HUDICON_LOCATION_SKILL_MAGE_ICENOVA;

    // Initialize item icons locations
    m_arrItemIcons[ITEMID_WEAPON_FISTS] = HUDICON_LOCATION_ITEM_WEAPON_FISTS;

    // Initialize skin colors
    m_hColorBrush.Bind();

    m_arrHUDColors[HUD_COLOR_SELFBAR_TEXT] = Color4::White;
    
    m_arrHUDColors[HUD_COLOR_TARGETBAR_TEXT] = Color4::White;
    
    m_arrHUDColors[HUD_COLOR_CASTBAR_TEXT] = Color4::White;

    m_arrHUDColors[HUD_COLOR_CHARACTERVIEW_TITLE] = Color4::Yellow;
    m_arrHUDColors[HUD_COLOR_CHARACTERVIEW_VALUE] = Color4::Green;

    m_arrHUDColors[HUD_COLOR_SKILLBOOK_TEXT] = Color4::White;

    // Initialize skin text formats
    m_hTextBrush.Bind();
    m_hTextFormat.Bind( LOCALENAME_EN_US, GPU2DFONTFAMILY_ARIAL, GPU2DFONT_WEIGHT_NORMAL, GPU2DFONT_STYLE_NORMAL, GPU2DFONT_STRETCH_NORMAL, 10.0f, NULL );

    m_arrTextFormats[HUD_FONT_SELFBAR_TEXT] = &m_hTextFormat;
    m_arrTextFormats[HUD_FONT_TARGET_TEXT] = &m_hTextFormat;
    m_arrTextFormats[HUD_FONT_CASTBAR_TEXT] = &m_hTextFormat;
    m_arrTextFormats[HUD_FONT_CHARACTERVIEW_TITLE] = &m_hTextFormat;
    m_arrTextFormats[HUD_FONT_CHARACTERVIEW_VALUE] = &m_hTextFormat;
    m_arrTextFormats[HUD_FONT_SKILLBOOK_TEXT] = &m_hTextFormat;

    // Create HUD
    m_pBackboardModel = NULL;

    m_pSelfBarModel = NULL;
    m_pTargetBarModel = NULL;
    m_pFocusBarModel = NULL;

    m_pStatusEffectBarModel = NULL;
    m_pSkillBarModel = NULL;

    m_pCastBarModel = NULL;

    m_pCharacterViewModel = NULL;
    m_pCharacterView = NULL;
    m_pSkillBookViewModel = NULL;
    m_pSkillBookView = NULL;
    m_pInventoryViewModel = NULL;
    m_pInventoryView = NULL;

    m_pHUDWindow = NULL;

    _CreateHeadUpDisplay();

    // Initialize DragNDrop
    m_hDragNDrop.iType = HUD_DRAGTYPE_UNDEFINED;
    m_hDragNDrop.iIdentifier = 0;
}
Void InterfaceManager::Cleanup()
{
    // Destroy HUD
    _DestroyHeadUpDisplay();

    // Destroy HUD skin
    m_hTextFormat.UnBind();
    m_hTextBrush.UnBind();

    m_hColorBrush.UnBind();
}

Void InterfaceManager::Update()
{
    GUIFn->Update();
}

Void InterfaceManager::Render()
{
    GUIFn->Render();
}

Void InterfaceManager::_CreateHeadUpDisplay()
{
    PCharacter * pPlayer = (PCharacter*)( GameplayFn->GetCharacter(TEXT("Shiki")) );

    // HUD models
    SelectMemory( TEXT("Scratch") );

    m_pBackboardModel = New HUDBackboardModel();

    m_pSelfBarModel = New SelfBarModel( pPlayer );
    m_pTargetBarModel = New TargetBarModel( pPlayer, false );
    m_pFocusBarModel = New TargetBarModel( pPlayer, true );

    m_pStatusEffectBarModel = New StatusEffectBarModel( pPlayer );
    m_pSkillBarModel = New SkillBarModel( pPlayer, 1, 16 );
    m_pSkillBarModel->SetSlot( 0, SKILLID_MAGE_FIREBOLT );
    m_pSkillBarModel->SetSlot( 1, SKILLID_MAGE_ICENOVA );

    m_pCastBarModel = New CastBarModel( pPlayer );

    m_pCharacterViewModel = New CharacterViewModel( pPlayer );
    m_pSkillBookViewModel = New SkillBookViewModel( pPlayer );
    m_pInventoryViewModel = New InventoryViewModel( pPlayer );

    UnSelectMemory();

    // HUD window
    UInt iWidth, iHeight;
    GUIFn->GetScreenSize( &iWidth, &iHeight );
    m_pHUDWindow = GUIFn->CreateWindow( TEXT("HUD"), Rectangle2( Point2(4,4), iWidth - 4, iHeight - 4 ), GUIWINDOW_STYLE_NAKED );
    GUIWidget * pHUDRoot = m_pHUDWindow->GetClientRoot();

    // HUD layout
    GUIFn->SelectMemory( TEXT("LayoutData") );

    GUIGridLayout hudLayout;
    hudLayout.FillX = GUILAYOUT_FILL_FIXED;
    hudLayout.FillY = GUILAYOUT_FILL_FIXED;
    hudLayout.Anchor = GUILAYOUT_ANCHOR_FIXED;

    hudLayout.FixedPos.X = 0;
    hudLayout.FixedPos.Y = 0;
    hudLayout.FixedSize.X = 150;
    hudLayout.FixedSize.Y = 60;
    SelfBar * pSelfBar = New SelfBar( m_pSelfBarModel, pHUDRoot, hudLayout, TEXT("SelfBar") );
    hudLayout.FixedPos.X = 150 + 4;
    hudLayout.FixedPos.Y = 0;
    hudLayout.FixedSize.X = 200;
    hudLayout.FixedSize.Y = 80;
    TargetBar * pTargetBar = New TargetBar( m_pTargetBarModel, pHUDRoot, hudLayout, TEXT("TargetBar") );
    hudLayout.FixedPos.X = 150 + 4;
    hudLayout.FixedPos.Y = 60 + 4;
    hudLayout.FixedSize.X = 200;
    hudLayout.FixedSize.Y = 80;
    TargetBar * pFocusBar = New TargetBar( m_pFocusBarModel, pHUDRoot, hudLayout, TEXT("FocusBar") );

    hudLayout.FixedPos.X = iWidth - 256 - 8;
    hudLayout.FixedPos.Y = 4;
    hudLayout.FixedSize.X = 256;
    hudLayout.FixedSize.Y = 32;
    StatusEffectBar * pStatusEffectBar = New StatusEffectBar( m_pStatusEffectBarModel, pHUDRoot, hudLayout, TEXT("StatusEffectBar") );
    hudLayout.FixedPos.X = ( (iWidth - 512) >> 1 );
    hudLayout.FixedPos.Y = 704;
    hudLayout.FixedSize.X = 512;
    hudLayout.FixedSize.Y = 32;
    SkillBar * pSkillBar = New SkillBar( m_pSkillBarModel, pHUDRoot, hudLayout, TEXT("SkillBar") );

    hudLayout.FixedPos.X = ( (iWidth - 192) >> 1 );
    hudLayout.FixedPos.Y = 672;
    hudLayout.FixedSize.X = 192;
    hudLayout.FixedSize.Y = 24;
    CastBar * pCastBar = New CastBar( m_pCastBarModel, pHUDRoot, hudLayout, TEXT("CastBar") );

    hudLayout.FixedPos.X = ( (iWidth - 384) >> 1 );
    hudLayout.FixedPos.Y = ( (iHeight - 640) >> 1 );
    hudLayout.FixedSize.X = 384;
    hudLayout.FixedSize.Y = 640;
    m_pCharacterView = New CharacterView( m_pCharacterViewModel, pHUDRoot, hudLayout, TEXT("CharacterView") );

    hudLayout.FixedPos.X = ( (iWidth - 332) >> 1 );
    hudLayout.FixedPos.Y = ( (iHeight - 156) >> 1 );
    hudLayout.FixedSize.X = 332;
    hudLayout.FixedSize.Y = 156;
    m_pSkillBookView = New SkillBookView( m_pSkillBookViewModel, pHUDRoot, hudLayout, TEXT("SkillBookView") );

    hudLayout.FixedPos.X = ( (iWidth - 400) >> 1 );
    hudLayout.FixedPos.Y = ( (iHeight - 264) >> 1 );
    hudLayout.FixedSize.X = 400;
    hudLayout.FixedSize.Y = 264;
    m_pInventoryView = New InventoryView( m_pInventoryViewModel, pHUDRoot, hudLayout, TEXT("InventoryView") );

    GUIFn->UnSelectMemory();

    // HUD setup
    GUIFn->SetBackboardModel( m_pBackboardModel );

    GUIFn->RegisterWindow( m_pHUDWindow );
    m_pHUDWindow->SetEventBlocking( false );
    m_pHUDWindow->SetVisible( true );

    m_pCharacterView->SetVisible( false );
    m_pSkillBookView->SetVisible( false );
    m_pInventoryView->SetVisible( false );
}
Void InterfaceManager::_DestroyHeadUpDisplay()
{
    // HUD setup
    GUIFn->UnregisterWindow( m_pHUDWindow );
   
    GUIFn->SetBackboardModel( NULL );

    // HUD window
    GUIFn->DestroyWindow( m_pHUDWindow );
    m_pHUDWindow = NULL;

    m_pCharacterView = NULL;
    m_pSkillBookView = NULL;
    m_pInventoryView = NULL;

    // HUD models
    SelectMemory( TEXT("Scratch") );

    Delete( m_pInventoryViewModel );
    m_pInventoryViewModel = NULL;
    Delete( m_pSkillBookViewModel );
    m_pSkillBookViewModel = NULL;
    Delete( m_pCharacterViewModel );
    m_pCharacterViewModel = NULL;

    Delete( m_pCastBarModel );
    m_pCastBarModel = NULL;

    Delete( m_pSkillBarModel );
    m_pSkillBarModel = NULL;
    Delete( m_pStatusEffectBarModel );
    m_pStatusEffectBarModel = NULL;

    Delete( m_pFocusBarModel );
    m_pFocusBarModel = NULL;
    Delete( m_pTargetBarModel );
    m_pTargetBarModel = NULL;
    Delete( m_pSelfBarModel );
    m_pSelfBarModel = NULL;

    Delete( m_pBackboardModel );
    m_pBackboardModel = NULL;

    UnSelectMemory();
}

