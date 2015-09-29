/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Main.cpp
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
// Includes
#include "Main.h"

/////////////////////////////////////////////////////////////////////////////////
#ifndef MAIN_DEBUG

/////////////////////////////////////////////////////////////////////////////////
// RPGGame implementation
DefineWindowApplication( RPGGame );

RPGGame::RPGGame():
    WindowApplication( TEXT("SimpleRPG"), PIXEL_FMT_RGBA8UI, 1024, 768, 0x80808080 )
{
    // Input
    m_iActionMap = INVALID_OFFSET;

	// World
    m_pLandscape = NULL;

    m_pRenderCamera = NULL;
    m_pWorldCamera = NULL;

	// Debug
	m_strFPSText[0] = NULLBYTE;
    m_strCoordsText[0] = NULLBYTE;
	m_strCullerText[0] = NULLBYTE;

    m_pDebugTextBrush = NULL;
    m_pDebugTextFormat = NULL;
}
RPGGame::~RPGGame()
{
    // nothing to do
}

Bool RPGGame::OnInitialize()
{
    // Initialize everything
    _CreateActions();

    GraphicsManager::Create();
    GraphicsFn->Initialize();
    _CreateLightsMaterials();
    _CreateBitmaps();
    _CreateTextures();
    _CreateInstancedGeometry();

    EntityManager::Create();
    EntityFn->Initialize();

    AnimationManager::Create();
    AnimationFn->Initialize();

    GameplayManager::Create();
    GameplayFn->Initialize();
    _CreateWorld();

    InterfaceManager::Create();
    InterfaceFn->Initialize();

    // Load player character
    PCharacter * pPlayer = (PCharacter*)( GameplayFn->GetCharacter(TEXT("Shiki")) );
    pPlayer->AddSkill( SKILLID_MAGE_FIREBOLT );
    pPlayer->AddSkill( SKILLID_MAGE_ICENOVA );

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

    // Done
    return true;
}
Void RPGGame::OnCleanup()
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
    InterfaceFn->Cleanup();
    InterfaceManager::Destroy();

    _DestroyWorld();
    GameplayFn->Cleanup();
    GameplayManager::Destroy();

    AnimationFn->Cleanup();
    AnimationManager::Destroy();

    EntityFn->Cleanup();
    EntityManager::Destroy();
    
    _DestroyInstancedGeometry();
    _DestroyTextures();
    _DestroyBitmaps();
    _DestroyLightsMaterials();
    GraphicsFn->Cleanup();
    GraphicsManager::Destroy();
    
    _DestroyActions();
}

Bool RPGGame::OnFrameMove()
{
    static const Scalar fSimulationTimeStep = MathFn->Invert( 500.0f );
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
	InterfaceFn->Update();

    // Update game logic
    GameplayFn->Update( fElapsedTime );

    // Update world & entities
    WorldFn->UpdateWorld( fElapsedTime );
    
    // Timing
    fLastTime = fTime;

	return true;
}
Void RPGGame::OnFrameRender()
{
    // Clear scene
    RenderingFn->ClearRenderTarget( RenderingFn->GetScreenRenderTarget(), (Float*)Color4::DarkGray );
    RenderingFn->ClearDepthStencil( RenderingFn->GetScreenDepthStencil(), true, true, 1.0f );

    // Render World
    WorldFn->RenderWorld();

    // Render GUI
    InterfaceFn->Render();

    // Render Debug text
    Vertex3 vPosition = GameplayFn->GetCharacter( TEXT("Shiki") )->GetEntity()->GetBody()->GetPosition();

    StringFn->Format( m_strFPSText, TEXT("FPS = %2.5f"), GetAverageFPS() );
    StringFn->Format( m_strCoordsText, TEXT("Coords = (%3f, %3f, %3f)"), vPosition.X, vPosition.Y, vPosition.Z );
    StringFn->Format( m_strCullerText, TEXT("Visible leaves = %d"), WorldFn->GetVisibleGroup().GetCount() );

    GPU2DContext * pContext2D = RenderingFn->GetScreen2DContext();

    GPU2DRectangle hDstRect;
    hDstRect.fLeft = 16.0f;
    hDstRect.fTop = 64.0f;
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

Void RPGGame::OnExit()
{
    GUIFn->OnExit();
}

Bool RPGGame::OnKeyPress( KeyCode iKey )
{
    GUIFn->OnKeyPress( iKey );

    return true;
}
Bool RPGGame::OnKeyRelease( KeyCode iKey )
{
    GUIFn->OnKeyRelease( iKey );

    return true;
}

Void RPGGame::OnMouseMove( UInt iX, UInt iY )
{
    GUIFn->OnMouseMove( Point2(iX, iY) );
}
Bool RPGGame::OnMouseWheel( UInt iX, UInt iY, Int iWheelDelta )
{
    GUIFn->OnMouseWheel( Point2(iX, iY), iWheelDelta );

    return true;
}
Bool RPGGame::OnMousePress( UInt iX, UInt iY, KeyCode iButton )
{
    GUIFn->OnMousePress( Point2(iX, iY), iButton );

    return true;
}
Bool RPGGame::OnMouseRelease( UInt iX, UInt iY, KeyCode iButton )
{
    GUIFn->OnMouseRelease( Point2(iX, iY), iButton );

    return true;
}
Bool RPGGame::OnMouseDblClick( UInt iX, UInt iY, KeyCode iButton )
{
    GUIFn->OnMouseDblClick( Point2(iX, iY), iButton );

    return true;
}

/////////////////////////////////////////////////////////////////////////////////

Void RPGGame::_CreateActions()
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
    InputFn->BindAction( ACTION_MOVE_RUN_UP,          KEYCODE_T );
    InputFn->BindAction( ACTION_MOVE_RUN_DOWN,        KEYCODE_G );
    InputFn->BindAction( ACTION_CAMERA_ZOOM_OUT,      KEYCODE_MOUSEWHEELDOWN );
    InputFn->BindAction( ACTION_CAMERA_ZOOM_IN,       KEYCODE_MOUSEWHEELUP );

    InputFn->BindAction( ACTION_CUSTOM_A0, KEYCODE_1 );
    InputFn->BindAction( ACTION_CUSTOM_A1, KEYCODE_2 );

    InputFn->BindAction( ACTION_CUSTOM_A7, KEYCODE_B );

    InputFn->BindAction( ACTION_CUSTOM_B0, KEYCODE_E );
    InputFn->BindAction( ACTION_CUSTOM_B1, KEYCODE_F );
    InputFn->BindAction( ACTION_CUSTOM_B2, KEYCODE_R );

    InputFn->BindAction( ACTION_CUSTOM_B4, KEYCODE_INSERT );
    InputFn->BindAction( ACTION_CUSTOM_B5, KEYCODE_HOME );
    InputFn->BindAction( ACTION_CUSTOM_B6, KEYCODE_PAGEUP );
    InputFn->BindAction( ACTION_CUSTOM_B7, KEYCODE_PAGEDOWN );
}
Void RPGGame::_DestroyActions()
{
    // Action map
    InputFn->DestroyMap( m_iActionMap );
    m_iActionMap = INVALID_OFFSET;
}

Void RPGGame::_CreateLightsMaterials()
{
    // Lights
    Light * pSun = GraphicsFn->CreateLight( TEXT("Light_Sun"), LIGHT_OMNI );
    pSun->Diffuse = Color4::White;
    pSun->Position = Vertex3( 256.00f, 256.0f, 100.0f );

    // Materials
    Material * pMat = GraphicsFn->CreateMaterial( TEXT("Material_Player") );
    pMat->Diffuse = Color4::Blue;
    pMat = GraphicsFn->CreateMaterial( TEXT("Material_SelectionMarker") );
    pMat->Diffuse = Color4::Yellow;
    pMat = GraphicsFn->CreateMaterial( TEXT("Material_CastAura") );
    pMat->Diffuse = Color4::Green;
    pMat = GraphicsFn->CreateMaterial( TEXT("Material_FireBolt") );
    pMat->Diffuse = Color4::Red;
    pMat = GraphicsFn->CreateMaterial( TEXT("Material_IceNova") );
    pMat->Diffuse = Color4::Cyan;
}
Void RPGGame::_DestroyLightsMaterials()
{
    // Lights
    GraphicsFn->DestroyLight( TEXT("Light_Sun") );

    // Materials
    GraphicsFn->DestroyMaterial( TEXT("Material_IceNova") );
    GraphicsFn->DestroyMaterial( TEXT("Material_FireBolt") );
    GraphicsFn->DestroyMaterial( TEXT("Material_CastAura") );
    GraphicsFn->DestroyMaterial( TEXT("Material_SelectionMarker") );
    GraphicsFn->DestroyMaterial( TEXT("Material_Player") );
}

Void RPGGame::_CreateBitmaps()
{
    // HUD
    GPU2DBitmap * pSkinBitmap = GraphicsFn->CreateBitmap( TEXT("Bitmap_HUD_Skin"), PIXEL_FMT_RGBA8UN, 256, 256 );
    GPU2DBitmap * pStatusEffectsBitmap = GraphicsFn->CreateBitmap( TEXT("Bitmap_HUD_StatusEffects"), PIXEL_FMT_RGBA8UN, 256, 256 );
    GPU2DBitmap * pSkillsBitmap = GraphicsFn->CreateBitmap( TEXT("Bitmap_HUD_Skills"), PIXEL_FMT_RGBA8UN, 256, 256 );
    GPU2DBitmap * pItemsBitmap = GraphicsFn->CreateBitmap( TEXT("Bitmap_HUD_Items"), PIXEL_FMT_RGBA8UN, 256, 256 );

    UInt iStride = pSkinBitmap->GetStride();
    UInt iPitch = pSkinBitmap->GetPitch();

    Byte * arrBytes = pSkinBitmap->GetBitmap();
    for( UInt y = 0; y < 32; ++y ) {
        for( UInt x = 0; x < 32; ++x ) {
            Byte * pPixel = ( arrBytes + (y * iPitch) + (x * iStride) );
            Color4 colTmp(0.0f,0.1f,0.0f,1.0f);
            PixelFormatConvertTo( pPixel, (const Float *)colTmp, PIXEL_FMT_RGBA8UN, 1 );

            pPixel = ( arrBytes + (y * iPitch) + ((x+32) * iStride) );
            colTmp = Color4(0.0f,0.8f,0.0f,1.0f);
            PixelFormatConvertTo( pPixel, (const Float *)colTmp, PIXEL_FMT_RGBA8UN, 1 );

            pPixel = ( arrBytes + (y * iPitch) + ((x+64) * iStride) );
            colTmp = Color4(0.0f,0.0f,0.1f,1.0f);
            PixelFormatConvertTo( pPixel, (const Float *)colTmp, PIXEL_FMT_RGBA8UN, 1 );

            pPixel = ( arrBytes + (y * iPitch) + ((x+96) * iStride) );
            colTmp = Color4(0.0f,0.0f,0.8f,1.0f);
            PixelFormatConvertTo( pPixel, (const Float *)colTmp, PIXEL_FMT_RGBA8UN, 1 );

            pPixel = ( arrBytes + (y * iPitch) + ((x+128) * iStride) );
            colTmp = Color4(0.1f,0.0f,0.1f,1.0f);
            PixelFormatConvertTo( pPixel, (const Float *)colTmp, PIXEL_FMT_RGBA8UN, 1 );

            pPixel = ( arrBytes + (y * iPitch) + ((x+160) * iStride) );
            colTmp = Color4(0.8f,0.0f,0.8f,1.0f);
            PixelFormatConvertTo( pPixel, (const Float *)colTmp, PIXEL_FMT_RGBA8UN, 1 );

            pPixel = ( arrBytes + (y * iPitch) + ((x+192) * iStride) );
            colTmp = Color4(0.2f,0.2f,0.2f,1.0f);
            PixelFormatConvertTo( pPixel, (const Float *)colTmp, PIXEL_FMT_RGBA8UN, 1 );

            pPixel = ( arrBytes + (y * iPitch) + ((x+224) * iStride) );
            colTmp = Color4(0.8f,0.8f,0.8f,1.0f);
            PixelFormatConvertTo( pPixel, (const Float *)colTmp, PIXEL_FMT_RGBA8UN, 1 );

            pPixel = ( arrBytes + ((y+32) * iPitch) + (x * iStride) );
            colTmp = Color4(0.2f,0.2f,0.2f,1.0f);
            PixelFormatConvertTo( pPixel, (const Float *)colTmp, PIXEL_FMT_RGBA8UN, 1 );

            pPixel = ( arrBytes + ((y+32) * iPitch) + ((x+32) * iStride) );
            colTmp = Color4(0.0f,0.0f,0.8f,1.0f);
            PixelFormatConvertTo( pPixel, (const Float *)colTmp, PIXEL_FMT_RGBA8UN, 1 );

            pPixel = ( arrBytes + ((y+32) * iPitch) + ((x+64) * iStride) );
            colTmp = Color4(0.8f,0.0f,0.0f,1.0f);
            PixelFormatConvertTo( pPixel, (const Float *)colTmp, PIXEL_FMT_RGBA8UN, 1 );

            pPixel = ( arrBytes + ((y+32) * iPitch) + ((x+96) * iStride) );
            colTmp = Color4(0.0f,0.0f,0.0f,1.0f);
            PixelFormatConvertTo( pPixel, (const Float *)colTmp, PIXEL_FMT_RGBA8UN, 1 );

            pPixel = ( arrBytes + ((y+32) * iPitch) + ((x+128) * iStride) );
            colTmp = Color4(0.0f,0.0f,0.0f,1.0f);
            PixelFormatConvertTo( pPixel, (const Float *)colTmp, PIXEL_FMT_RGBA8UN, 1 );

            pPixel = ( arrBytes + ((y+32) * iPitch) + ((x+160) * iStride) );
            colTmp = Color4(0.0f,0.0f,0.0f,1.0f);
            PixelFormatConvertTo( pPixel, (const Float *)colTmp, PIXEL_FMT_RGBA8UN, 1 );

            pPixel = ( arrBytes + ((y+32) * iPitch) + ((x+192) * iStride) );
            colTmp = Color4(0.0f,0.3f,0.3f,1.0f);
            PixelFormatConvertTo( pPixel, (const Float *)colTmp, PIXEL_FMT_RGBA8UN, 1 );

            pPixel = ( arrBytes + ((y+32) * iPitch) + ((x+224) * iStride) );
            colTmp = Color4(0.3f,0.3f,0.3f,1.0f);
            PixelFormatConvertTo( pPixel, (const Float *)colTmp, PIXEL_FMT_RGBA8UN, 1 );

            pPixel = ( arrBytes + ((y+64) * iPitch) + (x * iStride) );
            colTmp = Color4(0.8f,0.8f,0.8f,1.0f);
            PixelFormatConvertTo( pPixel, (const Float *)colTmp, PIXEL_FMT_RGBA8UN, 1 );

            pPixel = ( arrBytes + ((y+64) * iPitch) + ((x+32) * iStride) );
            colTmp = Color4(0.6f,0.0f,0.0f,1.0f);
            PixelFormatConvertTo( pPixel, (const Float *)colTmp, PIXEL_FMT_RGBA8UN, 1 );

            pPixel = ( arrBytes + ((y+64) * iPitch) + ((x+64) * iStride) );
            colTmp = Color4(0.0f,0.6f,0.0f,1.0f);
            PixelFormatConvertTo( pPixel, (const Float *)colTmp, PIXEL_FMT_RGBA8UN, 1 );

            pPixel = ( arrBytes + ((y+64) * iPitch) + ((x+96) * iStride) );
            colTmp = Color4(0.0f,0.0f,0.6f,1.0f);
            PixelFormatConvertTo( pPixel, (const Float *)colTmp, PIXEL_FMT_RGBA8UN, 1 );

            pPixel = ( arrBytes + ((y+64) * iPitch) + ((x+128) * iStride) );
            colTmp = Color4(8.0f,0.8f,0.0f,1.0f);
            PixelFormatConvertTo( pPixel, (const Float *)colTmp, PIXEL_FMT_RGBA8UN, 1 );
        }
    }
    pSkinBitmap->Bind();

    iStride = pStatusEffectsBitmap->GetStride();
    iPitch = pStatusEffectsBitmap->GetPitch();

    arrBytes = pStatusEffectsBitmap->GetBitmap();
    for( UInt y = 0; y < 32; ++y ) {
        for( UInt x = 0; x < 32; ++x ) {
            Byte * pPixel = ( arrBytes + (y * iPitch) + (x * iStride) );
            Color4 colTmp(0.0f,0.6f,0.6f,1.0f);
            PixelFormatConvertTo( pPixel, (const Float *)colTmp, PIXEL_FMT_RGBA8UN, 1 );
        }
    }
    pStatusEffectsBitmap->Bind();

    iStride = pSkillsBitmap->GetStride();
    iPitch = pSkillsBitmap->GetPitch();

    arrBytes = pSkillsBitmap->GetBitmap();
    for( UInt y = 0; y < 32; ++y ) {
        for( UInt x = 0; x < 32; ++x ) {
            Byte * pPixel = ( arrBytes + (y * iPitch) + (x * iStride) );
            Color4 colTmp(0.8f,0.5f,0.5f,1.0f);
            PixelFormatConvertTo( pPixel, (const Float *)colTmp, PIXEL_FMT_RGBA8UN, 1 );

            pPixel = ( arrBytes + (y * iPitch) + ((x+32) * iStride) );
            colTmp = Color4(0.5f,0.5f,0.8f,1.0f);
            PixelFormatConvertTo( pPixel, (const Float *)colTmp, PIXEL_FMT_RGBA8UN, 1 );
        }
    }
    pSkillsBitmap->Bind();

    iStride = pItemsBitmap->GetStride();
    iPitch = pItemsBitmap->GetPitch();

    arrBytes = pItemsBitmap->GetBitmap();
    for( UInt y = 0; y < 32; ++y ) {
        for( UInt x = 0; x < 32; ++x ) {
            Byte * pPixel = ( arrBytes + (y * iPitch) + (x * iStride) );
            Color4 colTmp(0.8f,0.8f,0.8f,1.0f);
            PixelFormatConvertTo( pPixel, (const Float *)colTmp, PIXEL_FMT_RGBA8UN, 1 );
        }
    }
    pItemsBitmap->Bind();
}
Void RPGGame::_DestroyBitmaps()
{
    // HUD
    GPU2DBitmap * pBitmap = GraphicsFn->GetBitmap( TEXT("Bitmap_HUD_Items") );
    pBitmap->UnBind();
    GraphicsFn->DestroyBitmap( TEXT("Bitmap_HUD_Items") );

    pBitmap = GraphicsFn->GetBitmap( TEXT("Bitmap_HUD_Skills") );
    pBitmap->UnBind();
    GraphicsFn->DestroyBitmap( TEXT("Bitmap_HUD_Skills") );

    pBitmap = GraphicsFn->GetBitmap( TEXT("Bitmap_HUD_StatusEffects") );
    pBitmap->UnBind();
    GraphicsFn->DestroyBitmap( TEXT("Bitmap_HUD_StatusEffects") );

    pBitmap = GraphicsFn->GetBitmap( TEXT("Bitmap_HUD_Skin") );
    pBitmap->UnBind();
    GraphicsFn->DestroyBitmap( TEXT("Bitmap_HUD_Skin") );
}

Void RPGGame::_CreateTextures()
{
    // Characters
    UInt arrPlayerDimensions[2] = { 128, 128 };
    GPUTexture2D * pTexture2D = (GPUTexture2D*)( GraphicsFn->CreateTexture( TEXT("Texture_Player"), GPURESOURCE_TEXTURE_2D, GPURESOURCE_USAGE_DEFAULT, PIXEL_FMT_RGBA8UN, 1, arrPlayerDimensions ) );
    pTexture2D->Bind( GPURESOURCE_BIND_SHADER_INPUT );

    // FireBolts
    UInt arrFireBoltDimensions[2] = { 128, 128 };
    pTexture2D = (GPUTexture2D*)( GraphicsFn->CreateTexture( TEXT("Texture_FireBolt"), GPURESOURCE_TEXTURE_2D, GPURESOURCE_USAGE_DEFAULT, PIXEL_FMT_RGBA8UN, 1, arrFireBoltDimensions ) );
    pTexture2D->Bind( GPURESOURCE_BIND_SHADER_INPUT );

    // IceNovas
    UInt arrIceNovaDimensions[2] = { 128, 128 };
    pTexture2D = (GPUTexture2D*)( GraphicsFn->CreateTexture( TEXT("Texture_IceNova"), GPURESOURCE_TEXTURE_2D, GPURESOURCE_USAGE_DEFAULT, PIXEL_FMT_RGBA8UN, 1, arrIceNovaDimensions ) );
    pTexture2D->Bind( GPURESOURCE_BIND_SHADER_INPUT );
}
Void RPGGame::_DestroyTextures()
{
    // IceNovas
    GPUTexture2D * pTexture2D = (GPUTexture2D*)( GraphicsFn->GetTexture( TEXT("Texture_IceNova") ) );
    pTexture2D->UnBind();
    GraphicsFn->DestroyTexture( TEXT("Texture_IceNova") );

    // FireBolts
    pTexture2D = (GPUTexture2D*)( GraphicsFn->GetTexture( TEXT("Texture_FireBolt") ) );
    pTexture2D->UnBind();
    GraphicsFn->DestroyTexture( TEXT("Texture_FireBolt") );

    // Characters
    pTexture2D = (GPUTexture2D*)( GraphicsFn->GetTexture( TEXT("Texture_Player") ) );
    pTexture2D->UnBind();
    GraphicsFn->DestroyTexture( TEXT("Texture_Player") );
}

Void RPGGame::_CreateInstancedGeometry()
{
    // Characters
    Sphere vPlayerSphere( Vertex3::Null, 1.0f );
    UInt iSamplesX = 16;
    UInt iSamplesY = 16;
    Bool bInterior = false;

    Void * arrPlayerParameters[4] = { &vPlayerSphere, &iSamplesX, &iSamplesY, &bInterior };
    GraphicsFn->CreateGeometryInstance( TEXT("Geometry_Player"), GEOMETRYID_SPHERE, true, EFFECTID_LIGHT_MATERIAL, arrPlayerParameters );

    // Selection Markers
    Box vSelectionMarkerBox( Vertex3::Null, Vector3(1.0f,1.0f,0.4f) );
    bInterior = false;

    Void * arrSelectionMarkerParameters[2] = { &vSelectionMarkerBox, &bInterior };
    GraphicsFn->CreateGeometryInstance( TEXT("Geometry_SelectionMarker"), GEOMETRYID_BOX, true, EFFECTID_LIGHT_MATERIAL, arrSelectionMarkerParameters );

    // CastAuras
    Cylinder vCastAuraCylinder( Vertex3::Null, Vector3::eK, 1.0f, 1.5f );
    UInt iSliceSamples = 2;
    UInt iRadialSamples = 16;
    bInterior = false;

    Void * arrCastAuraParameters[4] = { &vCastAuraCylinder, &iSliceSamples, &iRadialSamples, &bInterior };
    GraphicsFn->CreateGeometryInstance( TEXT("Geometry_CastAura"), GEOMETRYID_CYLINDER, true, EFFECTID_LIGHT_MATERIAL, arrCastAuraParameters );

    // FireBolts
    Box vFireBoltBox( Vertex3::Null, Vector3(1.0f,0.1f,0.1f) );
    bInterior = false;

    Void * arrBoltParameters[2] = { &vFireBoltBox, &bInterior };
    GraphicsFn->CreateGeometryInstance( TEXT("Geometry_FireBolt"), GEOMETRYID_BOX, true, EFFECTID_LIGHT_MATERIAL, arrBoltParameters );

    // IceNovas
    Cylinder vIceNovaCylinder( Vertex3::Null, Vector3::eK, 0.5f, 15.0f );
    iSliceSamples = 2;
    iRadialSamples = 16;
    bInterior = false;

    Void * arrIceNovaParameters[4] = { &vIceNovaCylinder, &iSliceSamples, &iRadialSamples, &bInterior };
    GraphicsFn->CreateGeometryInstance( TEXT("Geometry_IceNova"), GEOMETRYID_CYLINDER, true, EFFECTID_LIGHT_MATERIAL, arrIceNovaParameters );
}
Void RPGGame::_DestroyInstancedGeometry()
{
    // IceNovas
    GraphicsFn->DestroyGeometryInstance( TEXT("Geometry_IceNova") );

    // FireBolts
    GraphicsFn->DestroyGeometryInstance( TEXT("Geometry_FireBolt") );

    // CastAuras
    GraphicsFn->DestroyGeometryInstance( TEXT("Geometry_CastAura") );
    
    // Selection Markers
    GraphicsFn->DestroyGeometryInstance( TEXT("Geometry_SelectionMarker") );

    // Characters
    GraphicsFn->DestroyGeometryInstance( TEXT("Geometry_Player") );
}

Void RPGGame::_CreateWorld()
{
    BaseCharacter * pPlayer = GameplayFn->GetCharacter( TEXT("Shiki") );

    // Landscape
    m_pLandscape = New Landscape();
    m_pLandscape->SetEyeEntity( pPlayer->GetEntity() );
    WorldFn->AddChild( m_pLandscape );

    // World camera
    m_pRenderCamera = New Camera( true );
    m_pWorldCamera = New WorldCamera3rdPerson( m_pRenderCamera, pPlayer->GetEntity(), NULL, 3.0f );    
    WorldFn->SetWorldCamera( m_pWorldCamera );
}
Void RPGGame::_DestroyWorld()
{
    // World camera
    WorldFn->SetWorldCamera( NULL );
    Delete( m_pWorldCamera );
    m_pWorldCamera = NULL;
    Delete( m_pRenderCamera );
    m_pRenderCamera = NULL;

    // Landscape
    WorldFn->RemoveChild( m_pLandscape->GetName() );

    Delete( m_pLandscape );
    m_pLandscape = NULL;
}

/////////////////////////////////////////////////////////////////////////////////
#endif // !MAIN_DEBUG

