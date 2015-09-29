/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Console/Interfaces/VideoSettings.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Console Interface : Video Settings
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "VideoSettings.h"

#include "../ConsoleManager.h"

/////////////////////////////////////////////////////////////////////////////////
// VideoSettingsResolutionModel implementation
VideoSettingsResolutionModel::VideoSettingsResolutionModel( UInt iItemCount, const GChar ** arrItemNames, GUISkinImage * arrItemIcons, Void ** arrItemIndices ):
    GUIComboListModel( iItemCount, arrItemNames, arrItemIcons, arrItemIndices )
{
    // nothing to do
}
VideoSettingsResolutionModel::~VideoSettingsResolutionModel()
{
    // nothing to do
}

Void VideoSettingsResolutionModel::OnChoicePick( Void * pUserData )
{
    RenderingFn->SwitchDisplayMode( (UInt)pUserData );
}

/////////////////////////////////////////////////////////////////////////////////
// VideoSettingsFullscreenModel implementation
VideoSettingsFullscreenModel::VideoSettingsFullscreenModel( VideoSettingsWindow * pWindow, UInt iItemCount, const GChar ** arrItemNames, GUISkinImage * arrItemIcons ):
    GUIComboListModel( iItemCount, arrItemNames, arrItemIcons )
{
    m_pWindow = pWindow;
}
VideoSettingsFullscreenModel::~VideoSettingsFullscreenModel()
{
    // nothing to do
}

Void VideoSettingsFullscreenModel::OnChoicePick( Void * /*pUserData*/ )
{
    GUIWidget * pTmp = m_pWindow->m_pWindow->GetClientRoot()->GetChild( TEXT("VideoSettings_Section") );
    pTmp = pTmp->GetChild( TEXT("VideoSettings_Row0") )->GetChild( TEXT("VideoSettings_Row0_ComboList") );
    GUIComboList * pResolutionComboList = (GUIComboList*)pTmp;

    switch( m_iSelectedItem ) {
        case 0:
            RenderingFn->SetFullScreen( false );
            pResolutionComboList->SetVisible( true );
            break;
        case 1:
            RenderingFn->SetFullScreen( true );
            pResolutionComboList->SetVisible( true );
            break;
        case 2:
            RenderingFn->SetFullScreenWindowed( true );
            pResolutionComboList->SetVisible( false );
            break;
        default: Assert( false ); break;
    }
}

/////////////////////////////////////////////////////////////////////////////////
// VideoSettingsWindow implementation
VideoSettingsWindow::VideoSettingsWindow()
{
    m_pSectionModel = NULL;

    m_pResolutionLabelModel = NULL;
    m_pResolutionModel = NULL;

    m_pWindow = NULL;

    _CreateVideoSettings();
}
VideoSettingsWindow::~VideoSettingsWindow()
{
    _DestroyVideoSettings();
}

/////////////////////////////////////////////////////////////////////////////////

Void VideoSettingsWindow::_CreateVideoSettings()
{
    // Models
    ConsoleFn->SelectMemory( TEXT("Interfaces") );

        // Window section
    m_pSectionModel = New GUISectionModel();

        // Row 0 : Resolution
    m_pResolutionLabelModel = New GUILabelModel( TEXT("Resolution :") );

    UInt iDisplayModeCount = 0;
    UInt iTotalDisplayModeCount = RenderingFn->GetDisplayModeCount();
    GChar arrDisplayModeDescs[DEVICEWINDOW_MAX_DISPLAYMODES][GUI_NAME_LENGTH + 1];
    GChar * arrDisplayModeDescPtrs[DEVICEWINDOW_MAX_DISPLAYMODES];
    GUISkinImage arrDisplayModeIcons[DEVICEWINDOW_MAX_DISPLAYMODES];
    Void * arrDisplayModeIndices[DEVICEWINDOW_MAX_DISPLAYMODES];
    UInt iSelectedItem = 0;
    for( UInt i = 0; i < iTotalDisplayModeCount; ++i ) {
        GPUDisplayModeDesc hDesc;
        RenderingFn->GetDisplayModeDesc( &hDesc, i );
        if ( hDesc.iFormat != PIXEL_FMT_RGBA8UN )
            continue;
        Scalar fRefreshRate = ( ((Scalar)(hDesc.iRefreshRateNumerator)) / (Scalar)(hDesc.iRefreshRateDenominator) );
        StringFn->Format( arrDisplayModeDescs[iDisplayModeCount], TEXT("%dx%d (%dHz)"), hDesc.iWidth, hDesc.iHeight, MathFn->Round(fRefreshRate) );
        arrDisplayModeDescPtrs[iDisplayModeCount] = arrDisplayModeDescs[iDisplayModeCount];
        arrDisplayModeIcons[iDisplayModeCount] = GUISKIN_IMAGE_ICON_UNDEFINED;
        arrDisplayModeIndices[iDisplayModeCount] = (Void*)( hDesc.iIndex );
        if ( i == RenderingFn->GetCurrentDisplayMode() )
            iSelectedItem = iDisplayModeCount;
        ++iDisplayModeCount;
    }

    m_pResolutionModel = New VideoSettingsResolutionModel( iDisplayModeCount, (const GChar **)arrDisplayModeDescPtrs, arrDisplayModeIcons, arrDisplayModeIndices );
    m_pResolutionModel->SelectItem( iSelectedItem );

        // Row 1 : Fullscreen Mode
    m_pFullscreenLabelModel = New GUILabelModel( TEXT("Fullscreen Mode :") );

    GChar * arrFullscreenModeNames[3] = { TEXT("Windowed"), TEXT("Fullscreen"), TEXT("Fullscreen windowed") };
    GUISkinImage arrFullscreenModeIcons[3] = { GUISKIN_IMAGE_ICON_UNDEFINED, GUISKIN_IMAGE_ICON_UNDEFINED, GUISKIN_IMAGE_ICON_UNDEFINED };

    m_pFullscreenModel = New VideoSettingsFullscreenModel( this, 3, (const GChar **)arrFullscreenModeNames, arrFullscreenModeIcons );
    m_pFullscreenModel->SelectItem( 0 );

    ConsoleFn->UnSelectMemory();

    // Window
    m_pWindow = GUIFn->CreateWindow( TEXT("Video Settings"), Rectangle2( Point2(16,16), 200, 400 ), GUIWINDOW_STYLE_DEFAULTEX );
    GUIWidget * pClientRoot = m_pWindow->GetClientRoot();

    // Layout
    GUIGridLayout hLayout;

    hLayout.Anchor = GUILAYOUT_ANCHOR_NORTHWEST;
    hLayout.FillX = GUILAYOUT_FILL_RELATIVE;
    hLayout.FillY = GUILAYOUT_FILL_RELATIVE;
    hLayout.RelativeSize.X = 1.0f;
    hLayout.RelativeSize.Y = 1.0f;
    GUISection * pClientSection = (GUISection*)( GUIFn->CreateWidget( GUIWIDGET_SECTION, m_pSectionModel, pClientRoot, hLayout, TEXT("VideoSettings_Section") ) );

        // Row 0 : Resolution
    hLayout.Anchor = GUILAYOUT_ANCHOR_RELATIVE;
    hLayout.RelativePos.X = 0.0f;
    hLayout.RelativePos.Y = 0.0f;
    hLayout.FillX = GUILAYOUT_FILL_RELATIVE;
    hLayout.FillY = GUILAYOUT_FILL_RELATIVE;
    hLayout.RelativeSize.X = 1.0f;
    hLayout.RelativeSize.Y = 0.1f;
    GUIContainer * pRow = (GUIContainer*)( GUIFn->CreateWidget( GUIWIDGET_CONTAINER, NULL, pClientSection, hLayout, TEXT("VideoSettings_Row0") ) );

    hLayout.GridRows = 1;
    hLayout.GridColumns = 2;

    hLayout.GridArea.TopLeft.X = 0;
    hLayout.GridArea.TopLeft.Y = 0;
    hLayout.GridArea.DownRight.X = 0;
    hLayout.GridArea.DownRight.Y = 0;
    hLayout.Anchor = GUILAYOUT_ANCHOR_EAST;
    hLayout.FillX = GUILAYOUT_FILL_RELATIVE;
    hLayout.FillY = GUILAYOUT_FILL_RELATIVE;
    hLayout.RelativeSize.X = 1.0f;
    hLayout.RelativeSize.Y = 1.0f;
    GUIFn->CreateWidget( GUIWIDGET_LABEL, m_pResolutionLabelModel, pRow, hLayout, TEXT("VideoSettings_Row0_Label") );

    hLayout.GridArea.TopLeft.X = 1;
    hLayout.GridArea.TopLeft.Y = 0;
    hLayout.GridArea.DownRight.X = 1;
    hLayout.GridArea.DownRight.Y = 0;
    hLayout.Anchor = GUILAYOUT_ANCHOR_WEST;
    hLayout.FillX = GUILAYOUT_FILL_RELATIVE;
    hLayout.FillY = GUILAYOUT_FILL_RELATIVE;
    hLayout.RelativeSize.X = 0.8f;
    hLayout.RelativeSize.Y = 0.8f;
    GUIFn->CreateWidget( GUIWIDGET_COMBOLIST, m_pResolutionModel, pRow, hLayout, TEXT("VideoSettings_Row0_ComboList") );

    hLayout.GridRows = 1;
    hLayout.GridColumns = 1;

    hLayout.GridArea.TopLeft.X = 0;
    hLayout.GridArea.TopLeft.Y = 0;
    hLayout.GridArea.DownRight.X = 0;
    hLayout.GridArea.DownRight.Y = 0;

        // Row 1 : Fullscreen Mode
    hLayout.Anchor = GUILAYOUT_ANCHOR_RELATIVE;
    hLayout.RelativePos.X = 0.0f;
    hLayout.RelativePos.Y = 0.1f;
    hLayout.FillX = GUILAYOUT_FILL_RELATIVE;
    hLayout.FillY = GUILAYOUT_FILL_RELATIVE;
    hLayout.RelativeSize.X = 1.0f;
    hLayout.RelativeSize.Y = 0.1f;
    pRow = (GUIContainer*)( GUIFn->CreateWidget( GUIWIDGET_CONTAINER, NULL, pClientSection, hLayout, TEXT("VideoSettings_Row1") ) );

    hLayout.GridRows = 1;
    hLayout.GridColumns = 2;

    hLayout.GridArea.TopLeft.X = 0;
    hLayout.GridArea.TopLeft.Y = 0;
    hLayout.GridArea.DownRight.X = 0;
    hLayout.GridArea.DownRight.Y = 0;
    hLayout.Anchor = GUILAYOUT_ANCHOR_EAST;
    hLayout.FillX = GUILAYOUT_FILL_RELATIVE;
    hLayout.FillY = GUILAYOUT_FILL_RELATIVE;
    hLayout.RelativeSize.X = 1.0f;
    hLayout.RelativeSize.Y = 1.0f;
    GUIFn->CreateWidget( GUIWIDGET_LABEL, m_pFullscreenLabelModel, pRow, hLayout, TEXT("VideoSettings_Row1_Label") );

    hLayout.GridArea.TopLeft.X = 1;
    hLayout.GridArea.TopLeft.Y = 0;
    hLayout.GridArea.DownRight.X = 1;
    hLayout.GridArea.DownRight.Y = 0;
    hLayout.Anchor = GUILAYOUT_ANCHOR_WEST;
    hLayout.FillX = GUILAYOUT_FILL_RELATIVE;
    hLayout.FillY = GUILAYOUT_FILL_RELATIVE;
    hLayout.RelativeSize.X = 0.8f;
    hLayout.RelativeSize.Y = 0.8f;
    GUIFn->CreateWidget( GUIWIDGET_COMBOLIST, m_pFullscreenModel, pRow, hLayout, TEXT("VideoSettings_Row1_ComboList") );

    hLayout.GridRows = 1;
    hLayout.GridColumns = 1;

    hLayout.GridArea.TopLeft.X = 0;
    hLayout.GridArea.TopLeft.Y = 0;
    hLayout.GridArea.DownRight.X = 0;
    hLayout.GridArea.DownRight.Y = 0;

        // Row 2 : Vertical Sync
        // Row 3 : Fullscreen Anti-Aliasing (SwapChain multisampling)
        // Row 4 : ????

    // Setup
    GUIFn->RegisterWindow( m_pWindow );
    m_pWindow->SetVisible( false );
}
Void VideoSettingsWindow::_DestroyVideoSettings()
{
    // Setup
    GUIFn->UnregisterWindow( m_pWindow );

    // Window
    GUIFn->DestroyWindow( m_pWindow );
    m_pWindow = NULL;

    // Models
    ConsoleFn->SelectMemory( TEXT("Interfaces") );

    Delete( m_pFullscreenModel );
    m_pFullscreenModel = NULL;
    Delete( m_pFullscreenLabelModel );
    m_pFullscreenLabelModel = NULL;

    Delete( m_pResolutionModel );
    m_pResolutionModel = NULL;
    Delete( m_pResolutionLabelModel );
    m_pResolutionLabelModel = NULL;

    Delete( m_pSectionModel );
    m_pSectionModel = NULL;

    ConsoleFn->UnSelectMemory();
}

