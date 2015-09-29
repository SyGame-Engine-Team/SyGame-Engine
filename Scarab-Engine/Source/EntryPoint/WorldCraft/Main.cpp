/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/Main.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : WorldCraft, Entry Point
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
// WorldCraft implementation
DefineWindowApplication( WorldCraft );

WorldCraft::WorldCraft():
    WindowApplication( TEXT("WorldCraft"), PIXEL_FMT_RGBA8UI, 1024, 768, 0x80808080 )
{
	// Debug
	m_strFPSText[0] = NULLBYTE;
    m_strCoordsText[0] = NULLBYTE;
	m_strCullerText[0] = NULLBYTE;

    m_pDebugTextBrush = NULL;
    m_pDebugTextFormat = NULL;
}
WorldCraft::~WorldCraft()
{
    // nothing to do
}

Bool WorldCraft::OnInitialize()
{
    // Initialize everything
    GraphicsManager::Create();
    GraphicsFn->Initialize();

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
Void WorldCraft::OnCleanup()
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
    GraphicsFn->Cleanup();
    GraphicsManager::Destroy();
}

Bool WorldCraft::OnFrameMove()
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

    // Update interface
	GUIFn->Update();

    // Update game logic
    ////////////////////////////

    // Update world
    WorldFn->UpdateWorld( fElapsedTime );
    
    // Timing
    fLastTime = fTime;

	return true;
}
Void WorldCraft::OnFrameRender()
{
    // Clear scene
    RenderingFn->ClearRenderTarget( RenderingFn->GetScreenRenderTarget(), (Float*)Color4::DarkGray );
    RenderingFn->ClearDepthStencil( RenderingFn->GetScreenDepthStencil(), true, true, 1.0f );

    // Render World
    WorldFn->RenderWorld();

    // Render interface
    GUIFn->Render();

    // Render Debug text
    StringFn->Format( m_strFPSText, TEXT("FPS = %2.5f"), GetAverageFPS() );
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

Void WorldCraft::OnExit()
{
    GUIFn->OnExit();
}

Bool WorldCraft::OnKeyPress( KeyCode iKey )
{
    GUIFn->OnKeyPress( iKey );

    return true;
}
Bool WorldCraft::OnKeyRelease( KeyCode iKey )
{
    GUIFn->OnKeyRelease( iKey );

    return true;
}

Void WorldCraft::OnMouseMove( UInt iX, UInt iY )
{
    GUIFn->OnMouseMove( Point2(iX, iY) );
}
Bool WorldCraft::OnMouseWheel( UInt iX, UInt iY, Int iWheelDelta )
{
    GUIFn->OnMouseWheel( Point2(iX, iY), iWheelDelta );

    return true;
}
Bool WorldCraft::OnMousePress( UInt iX, UInt iY, KeyCode iButton )
{
    GUIFn->OnMousePress( Point2(iX, iY), iButton );

    return true;
}
Bool WorldCraft::OnMouseRelease( UInt iX, UInt iY, KeyCode iButton )
{
    GUIFn->OnMouseRelease( Point2(iX, iY), iButton );

    return true;
}
Bool WorldCraft::OnMouseDblClick( UInt iX, UInt iY, KeyCode iButton )
{
    GUIFn->OnMouseDblClick( Point2(iX, iY), iButton );

    return true;
}

/////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////
#endif // !MAIN_DEBUG
