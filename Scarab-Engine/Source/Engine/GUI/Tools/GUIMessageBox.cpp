/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Tools/GUIMessageBox.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Message-Box, pre-cooked window
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
#include "GUIMessageBox.h"

#include "../GUIManager.h"

/////////////////////////////////////////////////////////////////////////////////
// GUIMsgBoxButtonModel implementation
GUIMsgBoxButtonModel::GUIMsgBoxButtonModel( GUIMessageBox * pMessageBox, const GChar * strText ):
    GUIButtonModel( strText )
{
    m_pMessageBox = pMessageBox;

    m_iResult = GUIMSGBOX_RESULT_TRUE;
    m_pfCallback = NULL;
    m_pfDialogCallback = NULL;
    m_pUserData = NULL;
}
GUIMsgBoxButtonModel::~GUIMsgBoxButtonModel()
{
    // nothing to do
}

Void GUIMsgBoxButtonModel::OnPress()
{
    if ( m_pfCallback != NULL )
        m_pfCallback( m_iResult, m_pUserData );
    else if ( m_pfDialogCallback != NULL ) {
        UInt iItemCount = 0;
        GUIMessageBoxDialogResult arrItemValues[GUIMSGBOX_MAX_DIALOG_ITEMS];
        m_pMessageBox->ExtractDialogResults( &iItemCount, arrItemValues );
        m_pfDialogCallback( m_iResult, iItemCount, arrItemValues, m_pUserData );
    }

    m_pMessageBox->Close();
}

/////////////////////////////////////////////////////////////////////////////////
// GUIMessageBox implementation
GUIMessageBox::GUIMessageBox()
{
    UInt i;

    // Models
    m_pSectionModel = New GUISectionModel();

    m_pTitleModel = New GUILabelModel( TEXT("<notext>") );
    for( i = 0; i < GUIMSGBOX_MAX_BUTTONS; ++i )
        m_arrButtonModels[i] = New GUIMsgBoxButtonModel( this, TEXT("<notext>") );

    m_pMessageModel = NULL;

    m_iDialogItemCount = 0;
    for( i = 0; i < GUIMSGBOX_MAX_DIALOG_ITEMS; ++i ) {
        m_arrDialogItemTypes[i] = GUIMSGBOX_DIALOGFORM_TEXT;
        m_arrDialogOptionCounts[i] = 0;
        m_arrDialogLabelModels[i] = NULL;
        m_arrDialogTextModels[i] = NULL;
        m_arrDialogChoiceGroups[i] = NULL;
        for( UInt j = 0; j < GUIMSGBOX_MAX_OPTIONS; ++j ) {
            m_arrDialogOptionLabelModels[i][j] = NULL;
            m_arrDialogOptionModels[i][j] = NULL;
            m_arrDialogChoiceModels[i][j] = NULL;
        }
    }

    // Window
    UInt iScreenWidth, iScreenHeight;
    GUIFn->GetScreenSize( &iScreenWidth, &iScreenHeight );
    Assert( iScreenWidth > GUIMSGBOX_WIDTH );
    Assert( iScreenHeight > GUIMSGBOX_HEIGHT );

    Rectangle2 rectWindow;
    rectWindow.TopLeft.X = ( (iScreenWidth - GUIMSGBOX_WIDTH) >> 1 );
    rectWindow.TopLeft.Y = ( (iScreenHeight - GUIMSGBOX_HEIGHT) >> 1 );
    rectWindow.DownRight.X = rectWindow.TopLeft.X + ( GUIMSGBOX_WIDTH - 1 );
    rectWindow.DownRight.Y = rectWindow.TopLeft.Y + ( GUIMSGBOX_HEIGHT - 1 );

    m_pWindow = GUIFn->CreateWindow( TEXT("MessageBox"), rectWindow, GUIWINDOW_STYLE_NAKED );

    GUIWidget * pClientRoot = m_pWindow->GetClientRoot();

    GUIGridLayout msgboxLayout;
    Scalar fTitleHeight = ( 20.0f / (Scalar)(pClientRoot->GetClientRect().Height()) );
    Scalar fButtonHeight = ( 2.0f * fTitleHeight );
    Scalar fDialogHeight = ( 1.0f - fTitleHeight - fButtonHeight );

        // Section background
    msgboxLayout.Anchor = GUILAYOUT_ANCHOR_NORTHWEST;
    msgboxLayout.FillX = GUILAYOUT_FILL_RELATIVE;
    msgboxLayout.FillY = GUILAYOUT_FILL_RELATIVE;
    msgboxLayout.RelativeSize.X = 1.0f;
    msgboxLayout.RelativeSize.Y = 1.0f;
    GUISection * pClientSection = (GUISection*)( GUIFn->CreateWidget( GUIWIDGET_SECTION, m_pSectionModel, pClientRoot, msgboxLayout, TEXT("MessageBox_MainSection") ) );

        // Title
    msgboxLayout.Anchor = GUILAYOUT_ANCHOR_RELATIVE;
    msgboxLayout.RelativePos.X = 0.0f;
    msgboxLayout.RelativePos.Y = 0.0f;
    msgboxLayout.FillX = GUILAYOUT_FILL_RELATIVE;
    msgboxLayout.FillY = GUILAYOUT_FILL_RELATIVE;
    msgboxLayout.RelativeSize.X = 1.0f;
    msgboxLayout.RelativeSize.Y = fTitleHeight;
    GUIFn->CreateWidget( GUIWIDGET_LABEL, m_pTitleModel, pClientSection, msgboxLayout, TEXT("MessageBox_Title") );
    
        // Dialog form
    msgboxLayout.Anchor = GUILAYOUT_ANCHOR_RELATIVE;
    msgboxLayout.RelativePos.X = 0.0f;
    msgboxLayout.RelativePos.Y = fTitleHeight;
    msgboxLayout.FillX = GUILAYOUT_FILL_RELATIVE;
    msgboxLayout.FillY = GUILAYOUT_FILL_RELATIVE;
    msgboxLayout.RelativeSize.X = 1.0f;
    msgboxLayout.RelativeSize.Y = fDialogHeight;
    m_pDialogForm = (GUIContainer*)( GUIFn->CreateWidget( GUIWIDGET_CONTAINER, NULL, pClientSection, msgboxLayout, TEXT("MessageBox_DialogForm") ) );

        // Buttons
    msgboxLayout.Anchor = GUILAYOUT_ANCHOR_RELATIVE;
    msgboxLayout.RelativePos.X = 0.0f;
    msgboxLayout.RelativePos.Y = fTitleHeight + fDialogHeight;
    msgboxLayout.FillX = GUILAYOUT_FILL_RELATIVE;
    msgboxLayout.FillY = GUILAYOUT_FILL_RELATIVE;
    msgboxLayout.RelativeSize.X = 1.0f;
    msgboxLayout.RelativeSize.Y = fButtonHeight;

    m_pButtons = (GUIContainer*)( GUIFn->CreateWidget( GUIWIDGET_CONTAINER, NULL, pClientSection, msgboxLayout, TEXT("MessageBox_Buttons") ) );

    msgboxLayout.GridRows = 1;
    msgboxLayout.GridColumns = 3;
    msgboxLayout.Anchor = GUILAYOUT_ANCHOR_CENTER;
    msgboxLayout.FillX = GUILAYOUT_FILL_FIXED;
    msgboxLayout.FillY = GUILAYOUT_FILL_FIXED;
    msgboxLayout.FixedSize.X = 60;
    msgboxLayout.FixedSize.Y = 20;

    msgboxLayout.GridArea.TopLeft.X = 2;
    msgboxLayout.GridArea.TopLeft.Y = 0;
    msgboxLayout.GridArea.DownRight.X = 2;
    msgboxLayout.GridArea.DownRight.Y = 0;
    GUIFn->CreateWidget( GUIWIDGET_BUTTON, m_arrButtonModels[0], m_pButtons, msgboxLayout, TEXT("MessageBox_Button_0") );

    msgboxLayout.GridArea.TopLeft.X = 1;
    msgboxLayout.GridArea.DownRight.X = 1;
    GUIFn->CreateWidget( GUIWIDGET_BUTTON, m_arrButtonModels[1], m_pButtons, msgboxLayout, TEXT("MessageBox_Button_1") );
    
    msgboxLayout.GridArea.TopLeft.X = 0;
    msgboxLayout.GridArea.DownRight.X = 0;
    GUIFn->CreateWidget( GUIWIDGET_BUTTON, m_arrButtonModels[2], m_pButtons, msgboxLayout, TEXT("MessageBox_Button_2") );

    // Done
    GUIFn->RegisterWindow( m_pWindow );

    m_pWindow->SetVisible( false );
    m_bOpened = false;
    m_iType = GUIMSGBOX_TYPE_UNDEFINED;
}
GUIMessageBox::~GUIMessageBox()
{
    if ( m_bOpened )
        Close();

    // Window
    GUIFn->UnregisterWindow( m_pWindow );
    GUIFn->DestroyWindow( m_pWindow );

    // Models
    for( UInt i = 0; i < GUIMSGBOX_MAX_BUTTONS; ++i )
        Delete( m_arrButtonModels[i] );
    Delete( m_pTitleModel );
    Delete( m_pSectionModel );
}

Bool GUIMessageBox::Open( GUIMessageBoxType iType, const GChar * strMessage, GUIMessageBoxButtons iButtons, GUIMsgBoxCallback pfCallback, Void * pUserData )
{
    if ( m_bOpened )
        return false;

    m_pWindow->SetVisible( true );

    // Type
    switch( iType ) {
        case GUIMSGBOX_TYPE_INFORMATION: m_pTitleModel->SetText( TEXT("Information") ); break;
        case GUIMSGBOX_TYPE_WARNING:     m_pTitleModel->SetText( TEXT("Warning") ); break;
        case GUIMSGBOX_TYPE_ALERT:       m_pTitleModel->SetText( TEXT("Alert") ); break;
        case GUIMSGBOX_TYPE_ERROR:       m_pTitleModel->SetText( TEXT("Error") ); break;
        case GUIMSGBOX_TYPE_FATALERROR:  m_pTitleModel->SetText( TEXT("Fatal Error") ); break;
        default: Assert(false); return false;
    }

    // Message
    GUIFn->SelectMemory( TEXT("Scratch") );
    m_pMessageModel = New GUILabelModel( strMessage );
    GUIFn->UnSelectMemory();

    GUIGridLayout msgboxLayout;

    msgboxLayout.Anchor = GUILAYOUT_ANCHOR_NORTHWEST;
    msgboxLayout.FillX = GUILAYOUT_FILL_RELATIVE;
    msgboxLayout.FillY = GUILAYOUT_FILL_RELATIVE;
    msgboxLayout.RelativeSize.X = 1.0f;
    msgboxLayout.RelativeSize.Y = 1.0f;
    GUIFn->CreateWidget( GUIWIDGET_LABEL, m_pMessageModel, m_pDialogForm, msgboxLayout, TEXT("MessageBox_Message") );

    m_pDialogForm->SetVisible( true );

    // Buttons
    switch( iButtons ) {
        case GUIMSGBOX_BUTTONS_OK:
            m_arrButtonModels[0]->SetText( TEXT("Ok") );
            m_arrButtonModels[0]->SetResult( GUIMSGBOX_RESULT_TRUE );
            m_arrButtonModels[0]->SetCallback( pfCallback, pUserData );
            m_pButtons->GetChild( TEXT("MessageBox_Button_1") )->SetVisible( false );
            m_pButtons->GetChild( TEXT("MessageBox_Button_2") )->SetVisible( false );
            break;
        case GUIMSGBOX_BUTTONS_OK_CANCEL:
            m_arrButtonModels[0]->SetText( TEXT("Cancel") );
            m_arrButtonModels[0]->SetResult( GUIMSGBOX_RESULT_FALSE );
            m_arrButtonModels[0]->SetCallback( pfCallback, pUserData );
            m_arrButtonModels[1]->SetText( TEXT("Ok") );
            m_arrButtonModels[1]->SetResult( GUIMSGBOX_RESULT_TRUE );
            m_arrButtonModels[1]->SetCallback( pfCallback, pUserData );
            m_pButtons->GetChild( TEXT("MessageBox_Button_2") )->SetVisible( false );
            break;
        case GUIMSGBOX_BUTTONS_YES_NO:
            m_arrButtonModels[0]->SetText( TEXT("No") );
            m_arrButtonModels[0]->SetResult( GUIMSGBOX_RESULT_FALSE );
            m_arrButtonModels[0]->SetCallback( pfCallback, pUserData );
            m_arrButtonModels[1]->SetText( TEXT("Yes") );
            m_arrButtonModels[1]->SetResult( GUIMSGBOX_RESULT_TRUE );
            m_arrButtonModels[1]->SetCallback( pfCallback, pUserData );
            m_pButtons->GetChild( TEXT("MessageBox_Button_2") )->SetVisible( false );
            break;
        case GUIMSGBOX_BUTTONS_CONTINUE_STOP:
            m_arrButtonModels[0]->SetText( TEXT("Stop") );
            m_arrButtonModels[0]->SetResult( GUIMSGBOX_RESULT_FALSE );
            m_arrButtonModels[0]->SetCallback( pfCallback, pUserData );
            m_arrButtonModels[1]->SetText( TEXT("Continue") );
            m_arrButtonModels[1]->SetResult( GUIMSGBOX_RESULT_TRUE );
            m_arrButtonModels[1]->SetCallback( pfCallback, pUserData );
            m_pButtons->GetChild( TEXT("MessageBox_Button_2") )->SetVisible( false );
            break;
        case GUIMSGBOX_BUTTONS_RETRY_IGNORE_ABORT:
            m_arrButtonModels[0]->SetText( TEXT("Abort") );
            m_arrButtonModels[0]->SetResult( GUIMSGBOX_RESULT_FALSE );
            m_arrButtonModels[0]->SetCallback( pfCallback, pUserData );
            m_arrButtonModels[1]->SetText( TEXT("Ignore") );
            m_arrButtonModels[1]->SetResult( GUIMSGBOX_RESULT_SPECIAL );
            m_arrButtonModels[1]->SetCallback( pfCallback, pUserData );
            m_arrButtonModels[2]->SetText( TEXT("Retry") );
            m_arrButtonModels[2]->SetResult( GUIMSGBOX_RESULT_TRUE );
            m_arrButtonModels[2]->SetCallback( pfCallback, pUserData );
            break;
        default: Assert(false); return false;
    }

    // Done
    m_bOpened = true;
    m_iType = iType;
    GUIFn->SetFocusWindow( m_pWindow );

    return true;
}
Bool GUIMessageBox::OpenDialog( const GUIMessageBoxDialogDeclaration * arrDialogFormItems, UInt iDialogFormItemCount, GUIMsgBoxDialogCallback pfCallback, Void * pUserData )
{
    Assert( iDialogFormItemCount < GUIMSGBOX_MAX_DIALOG_ITEMS );

    if ( m_bOpened )
        return false;

    m_pWindow->SetVisible( true );

    // Type
    m_pTitleModel->SetText( TEXT("Dialog Form") );

    // Dialog form
    GUIFn->SelectMemory( TEXT("Scratch") );

    m_iDialogItemCount = iDialogFormItemCount;
    for( UInt i = 0; i < iDialogFormItemCount; ++i ) {
        m_arrDialogItemTypes[i] = arrDialogFormItems[i].iItemType;
        m_arrDialogLabelModels[i] = New GUILabelModel( arrDialogFormItems[i].strLabel );
        switch( arrDialogFormItems[i].iItemType ) {
            case GUIMSGBOX_DIALOGFORM_TEXT:
                m_arrDialogOptionCounts[i] = 0;
                m_arrDialogTextModels[i] = New GUITextInputModel();
                break;
            case GUIMSGBOX_DIALOGFORM_OPTIONS:
                m_arrDialogOptionCounts[i] = arrDialogFormItems[i].iOptionCount;
                for( UInt j = 0; j < arrDialogFormItems[i].iOptionCount; ++j ) {
                    m_arrDialogOptionLabelModels[i][j] = New GUILabelModel( arrDialogFormItems[i].strOption[j] );
                    m_arrDialogOptionModels[i][j] = New GUICheckBoxModel();
                }
                break;
            case GUIMSGBOX_DIALOGFORM_CHOICE:
                m_arrDialogOptionCounts[i] = arrDialogFormItems[i].iOptionCount;
                m_arrDialogChoiceGroups[i] = New GUIRadioButtonGroup();
                for( UInt j = 0; j < arrDialogFormItems[i].iOptionCount; ++j ) {
                    m_arrDialogOptionLabelModels[i][j] = New GUILabelModel( arrDialogFormItems[i].strOption[j] );
                    m_arrDialogChoiceModels[i][j] = New GUIRadioButtonModel( m_arrDialogChoiceGroups[i] );
                }
                break;
            default: Assert(false); break;
        }
    }

    GUIFn->UnSelectMemory();

    Scalar fRowHeight = MathFn->Invert( (Scalar)iDialogFormItemCount );
    Scalar fRowTop = 0.0f;
    GChar strName[64];

    for( UInt i = 0; i < iDialogFormItemCount; ++i ) {
        GUIGridLayout msgboxLayout;

        // Row container
        msgboxLayout.Anchor = GUILAYOUT_ANCHOR_RELATIVE;
        msgboxLayout.RelativePos.X = 0.0f;
        msgboxLayout.RelativePos.Y = fRowTop;
        msgboxLayout.FillX = GUILAYOUT_FILL_RELATIVE;
        msgboxLayout.FillY = GUILAYOUT_FILL_RELATIVE;
        msgboxLayout.RelativeSize.X = 1.0f;
        msgboxLayout.RelativeSize.Y = fRowHeight;

        StringFn->Format( strName, TEXT("DialogBox_Row%d"), i );
        GUIContainer * pRow = (GUIContainer*)( GUIFn->CreateWidget( GUIWIDGET_CONTAINER, NULL, m_pDialogForm, msgboxLayout, strName ) );

        // Row grid
        msgboxLayout.GridRows = 1;
        msgboxLayout.GridColumns = 2;

        // Row label
        msgboxLayout.GridArea.TopLeft.X = 0;
        msgboxLayout.GridArea.TopLeft.Y = 0;
        msgboxLayout.GridArea.DownRight.X = 0;
        msgboxLayout.GridArea.DownRight.Y = 0;
        msgboxLayout.Anchor = GUILAYOUT_ANCHOR_EAST;
        msgboxLayout.FillX = GUILAYOUT_FILL_RELATIVE;
        msgboxLayout.FillY = GUILAYOUT_FILL_RELATIVE;
        msgboxLayout.RelativeSize.X = 1.0f;
        msgboxLayout.RelativeSize.Y = 1.0f;

        StringFn->Format( strName, TEXT("DialogBox_Row%d_Label"), i );
        GUIFn->CreateWidget( GUIWIDGET_LABEL, m_arrDialogLabelModels[i], pRow, msgboxLayout, strName );

        // Row Content
        msgboxLayout.GridArea.TopLeft.X = 1;
        msgboxLayout.GridArea.DownRight.X = 1;
        msgboxLayout.Anchor = GUILAYOUT_ANCHOR_WEST;
        msgboxLayout.FillX = GUILAYOUT_FILL_RELATIVE;
        msgboxLayout.FillY = GUILAYOUT_FILL_RELATIVE;
        msgboxLayout.RelativeSize.X = 1.0f;
        msgboxLayout.RelativeSize.Y = 1.0f;

        StringFn->Format( strName, TEXT("DialogBox_Row%d_Content"), i );
        GUIContainer * pRowContent = (GUIContainer*)( GUIFn->CreateWidget( GUIWIDGET_CONTAINER, NULL, pRow, msgboxLayout, strName ) );

        // Row layout
        UInt iGridLeft = 0;
        UInt iGridRight = 0;

        switch( arrDialogFormItems[i].iItemType ) {
            case GUIMSGBOX_DIALOGFORM_TEXT: {
                    msgboxLayout.GridRows = 1;
                    msgboxLayout.GridColumns = 1;

                    msgboxLayout.GridArea.TopLeft.X = 0;
                    msgboxLayout.GridArea.TopLeft.Y = 0;
                    msgboxLayout.GridArea.DownRight.X = 0;
                    msgboxLayout.GridArea.DownRight.Y = 0;
                    msgboxLayout.Anchor = GUILAYOUT_ANCHOR_WEST;
                    msgboxLayout.FillX = GUILAYOUT_FILL_RELATIVE;
                    msgboxLayout.FillY = GUILAYOUT_FILL_FIXED;
                    msgboxLayout.RelativeSize.X = 0.9f;
                    msgboxLayout.FixedSize.Y = 16;

                    StringFn->Format( strName, TEXT("DialogBox_Row%d_Content_Text"), i );
                    GUIFn->CreateWidget( GUIWIDGET_TEXTINPUT, m_arrDialogTextModels[i], pRowContent, msgboxLayout, strName );
                } break;
            case GUIMSGBOX_DIALOGFORM_OPTIONS: {
                    for( UInt j = 0; j < arrDialogFormItems[i].iOptionCount; ++j ) {
                        // Option container
                        msgboxLayout.GridRows = 1;
                        msgboxLayout.GridColumns = arrDialogFormItems[i].iOptionCount;
                        msgboxLayout.GridArea.TopLeft.X = iGridLeft;
                        msgboxLayout.GridArea.TopLeft.Y = 0;
                        msgboxLayout.GridArea.DownRight.X = iGridRight;
                        msgboxLayout.GridArea.DownRight.Y = 0;
                        msgboxLayout.Anchor = GUILAYOUT_ANCHOR_WEST;
                        msgboxLayout.FillX = GUILAYOUT_FILL_RELATIVE;
                        msgboxLayout.FillY = GUILAYOUT_FILL_RELATIVE;
                        msgboxLayout.RelativeSize.X = 1.0f;
                        msgboxLayout.RelativeSize.Y = 1.0f;

                        StringFn->Format( strName, TEXT("DialogBox_Row%d_Content_Option%d"), i, j );
                        GUIContainer * pOptionContent = (GUIContainer*)( GUIFn->CreateWidget( GUIWIDGET_CONTAINER, NULL, pRowContent, msgboxLayout, strName ) );

                        // Option label
                        msgboxLayout.GridRows = 1;
                        msgboxLayout.GridColumns = 2;
                        msgboxLayout.GridArea.TopLeft.X = 0;
                        msgboxLayout.GridArea.TopLeft.Y = 0;
                        msgboxLayout.GridArea.DownRight.X = 0;
                        msgboxLayout.GridArea.DownRight.Y = 0;
                        msgboxLayout.Anchor = GUILAYOUT_ANCHOR_EAST;
                        msgboxLayout.FillX = GUILAYOUT_FILL_RELATIVE;
                        msgboxLayout.FillY = GUILAYOUT_FILL_RELATIVE;
                        msgboxLayout.RelativeSize.X = 1.0f;
                        msgboxLayout.RelativeSize.Y = 1.0f;

                        StringFn->Format( strName, TEXT("DialogBox_Row%d_Content_Option%d_Label"), i, j );
                        GUIFn->CreateWidget( GUIWIDGET_LABEL, m_arrDialogOptionLabelModels[i][j], pOptionContent, msgboxLayout, strName );

                        // Option checkbox
                        msgboxLayout.GridArea.TopLeft.X = 1;
                        msgboxLayout.GridArea.DownRight.X = 1;
                        msgboxLayout.Anchor = GUILAYOUT_ANCHOR_WEST;
                        msgboxLayout.FillX = GUILAYOUT_FILL_FIXED;
                        msgboxLayout.FillY = GUILAYOUT_FILL_FIXED;
                        msgboxLayout.FixedSize.X = 16;
                        msgboxLayout.FixedSize.Y = 16;

                        StringFn->Format( strName, TEXT("DialogBox_Row%d_Content_Option%d_Box"), i, j );
                        GUIFn->CreateWidget( GUIWIDGET_CHECKBOX, m_arrDialogOptionModels[i][j], pOptionContent, msgboxLayout, strName );

                        // Next option
                        ++iGridLeft;
                        ++iGridRight;
                    }
                } break;
            case GUIMSGBOX_DIALOGFORM_CHOICE: {
                    for( UInt j = 0; j < arrDialogFormItems[i].iOptionCount; ++j ) {
                        // Choice container
                        msgboxLayout.GridRows = 1;
                        msgboxLayout.GridColumns = arrDialogFormItems[i].iOptionCount;
                        msgboxLayout.GridArea.TopLeft.X = iGridLeft;
                        msgboxLayout.GridArea.TopLeft.Y = 0;
                        msgboxLayout.GridArea.DownRight.X = iGridRight;
                        msgboxLayout.GridArea.DownRight.Y = 0;
                        msgboxLayout.Anchor = GUILAYOUT_ANCHOR_WEST;
                        msgboxLayout.FillX = GUILAYOUT_FILL_RELATIVE;
                        msgboxLayout.FillY = GUILAYOUT_FILL_RELATIVE;
                        msgboxLayout.RelativeSize.X = 1.0f;
                        msgboxLayout.RelativeSize.Y = 1.0f;

                        StringFn->Format( strName, TEXT("DialogBox_Row%d_Content_Choice%d"), i, j );
                        GUIContainer * pChoiceContent = (GUIContainer*)( GUIFn->CreateWidget( GUIWIDGET_CONTAINER, NULL, pRowContent, msgboxLayout, strName ) );

                        // Choice label
                        msgboxLayout.GridRows = 1;
                        msgboxLayout.GridColumns = 2;
                        msgboxLayout.GridArea.TopLeft.X = 0;
                        msgboxLayout.GridArea.TopLeft.Y = 0;
                        msgboxLayout.GridArea.DownRight.X = 0;
                        msgboxLayout.GridArea.DownRight.Y = 0;
                        msgboxLayout.Anchor = GUILAYOUT_ANCHOR_EAST;
                        msgboxLayout.FillX = GUILAYOUT_FILL_RELATIVE;
                        msgboxLayout.FillY = GUILAYOUT_FILL_RELATIVE;
                        msgboxLayout.RelativeSize.X = 1.0f;
                        msgboxLayout.RelativeSize.Y = 1.0f;

                        StringFn->Format( strName, TEXT("DialogBox_Row%d_Content_Choice%d_Label"), i, j );
                        GUIFn->CreateWidget( GUIWIDGET_LABEL, m_arrDialogOptionLabelModels[i][j], pChoiceContent, msgboxLayout, strName );

                        // Choice radiobutton
                        msgboxLayout.GridArea.TopLeft.X = 1;
                        msgboxLayout.GridArea.DownRight.X = 1;
                        msgboxLayout.Anchor = GUILAYOUT_ANCHOR_WEST;
                        msgboxLayout.FillX = GUILAYOUT_FILL_FIXED;
                        msgboxLayout.FillY = GUILAYOUT_FILL_FIXED;
                        msgboxLayout.FixedSize.X = 16;
                        msgboxLayout.FixedSize.Y = 16;

                        StringFn->Format( strName, TEXT("DialogBox_Row%d_Content_Choice%d_Box"), i, j );
                        GUIFn->CreateWidget( GUIWIDGET_RADIOBUTTON, m_arrDialogChoiceModels[i][j], pChoiceContent, msgboxLayout, strName );

                        // Next choice
                        ++iGridLeft;
                        ++iGridRight;
                    }
                } break;
            default: Assert(false); break;
        }

        // Next row
        fRowTop += fRowHeight;
    }
    
    m_pDialogForm->SetVisible( true );

    // Buttons
    m_arrButtonModels[0]->SetText( TEXT("Cancel") );
    m_arrButtonModels[0]->SetResult( GUIMSGBOX_RESULT_FALSE );
    m_arrButtonModels[0]->SetCallback( pfCallback, pUserData );
    m_arrButtonModels[1]->SetText( TEXT("Ok") );
    m_arrButtonModels[1]->SetResult( GUIMSGBOX_RESULT_TRUE );
    m_arrButtonModels[1]->SetCallback( pfCallback, pUserData );
    m_pButtons->GetChild( TEXT("MessageBox_Button_2") )->SetVisible( false );

    // Done
    m_bOpened = true;
    m_iType = GUIMSGBOX_TYPE_DIALOG;
    GUIFn->SetFocusWindow( m_pWindow );

    return true;
}
Void GUIMessageBox::Close()
{
    Assert( m_bOpened );

    m_pWindow->SetVisible( false );

    // Form
    m_pDialogForm->DestroyChildren();

    // Models
    GUIFn->SelectMemory( TEXT("Scratch") );

    if ( m_iType != GUIMSGBOX_TYPE_DIALOG ) {
        Delete( m_pMessageModel );
        m_pMessageModel = NULL;
    } else {
        for( UInt i = 0; i < m_iDialogItemCount; ++i ) {
            Delete( m_arrDialogLabelModels[i] );
            m_arrDialogLabelModels[i] = NULL;
            switch( m_arrDialogItemTypes[i] ) {
                case GUIMSGBOX_DIALOGFORM_TEXT:
                    Delete( m_arrDialogTextModels[i] );
                    m_arrDialogTextModels[i] = NULL;
                    break;
                case GUIMSGBOX_DIALOGFORM_OPTIONS:
                    for( UInt j = 0; j < m_arrDialogOptionCounts[i]; ++j ) {
                        Delete( m_arrDialogOptionLabelModels[i][j] );
                        m_arrDialogOptionLabelModels[i][j] = NULL;
                        Delete( m_arrDialogOptionModels[i][j] );
                        m_arrDialogOptionModels[i][j] = NULL;
                    }
                    break;
                case GUIMSGBOX_DIALOGFORM_CHOICE:
                    Delete( m_arrDialogChoiceGroups[i] );
                    for( UInt j = 0; j < m_arrDialogOptionCounts[i]; ++j ) {
                        Delete( m_arrDialogOptionLabelModels[i][j] );
                        m_arrDialogOptionLabelModels[i][j] = NULL;
                        Delete( m_arrDialogChoiceModels[i][j] );
                        m_arrDialogChoiceModels[i][j] = NULL;
                    }
                    break;
                default: Assert(false); break;
            }
        }
    }
        
    GUIFn->UnSelectMemory();

    // Done
    m_bOpened = false;
    m_iType = GUIMSGBOX_TYPE_UNDEFINED;
}

Void GUIMessageBox::ExtractDialogResults( UInt * outItemCount, GUIMessageBoxDialogResult * outItemValues ) const
{
    Assert( m_bOpened && (m_iType == GUIMSGBOX_TYPE_DIALOG) );

    *outItemCount = m_iDialogItemCount;

    for( UInt i = 0; i < m_iDialogItemCount; ++i ) {
        switch( m_arrDialogItemTypes[i] ) {
            case GUIMSGBOX_DIALOGFORM_TEXT:
                outItemValues[i].strValue = m_arrDialogTextModels[i]->GetText();
                break;
            case GUIMSGBOX_DIALOGFORM_OPTIONS:
                outItemValues[i].iOptions = 0;
                for( UInt j = 0; j < m_arrDialogOptionCounts[i]; ++j ) {
                    if ( m_arrDialogOptionModels[i][j]->IsTicked() )
                        outItemValues[i].iOptions |= ( 1 << j );
                }
                break;
            case GUIMSGBOX_DIALOGFORM_CHOICE:
                outItemValues[i].iChoice = m_arrDialogChoiceGroups[i]->GetTickedIndex();
                break;
            default: Assert(false); break;
        }
    }
}

