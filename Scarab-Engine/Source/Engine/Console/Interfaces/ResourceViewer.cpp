/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Console/Interfaces/ResourceViewer.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Console Interface : Resource Viewer
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
#include "ResourceViewer.h"

#include "../ConsoleManager.h"

/////////////////////////////////////////////////////////////////////////////////
// ResourceViewerContextMenuModel implementation
const GChar * ResourceViewerContextMenuModel::sm_arrItemNames[sm_iItemCount] = {
    TEXT("Expand All"),
    TEXT("Collapse All"),
    TEXT("Create Package ..."),
    TEXT("Create Directory ..."),
    TEXT("Destroy"),
    TEXT("Import ..."),
    TEXT("Export ...")
};
GUISkinImage ResourceViewerContextMenuModel::sm_arrItemIcons[sm_iItemCount] = {
    GUISKIN_IMAGE_ICON_UNDEFINED,
    GUISKIN_IMAGE_ICON_UNDEFINED,
    GUISKIN_IMAGE_ICON_UNDEFINED,
    GUISKIN_IMAGE_ICON_UNDEFINED,
    GUISKIN_IMAGE_ICON_UNDEFINED,
    GUISKIN_IMAGE_ICON_UNDEFINED,
    GUISKIN_IMAGE_ICON_UNDEFINED
};

ResourceViewerContextMenuModel::ResourceViewerContextMenuModel( ResourceViewerModel * pModel ):
    GUIMenuModel( 16, sm_iItemCount, sm_arrItemNames, sm_arrItemIcons, NULL )
{
    m_pModel = pModel;
}
ResourceViewerContextMenuModel::~ResourceViewerContextMenuModel()
{
    // nothing to do
}

Void ResourceViewerContextMenuModel::OnValidate( UInt iItem )
{
    GUITreeNode * pNode = m_pModel->GetLastSelectedNode();
    if ( pNode == NULL )
        return;

    // Dialog form
    GUIMessageBoxDialogDeclaration arrDialogForm[4];
    MemZero( arrDialogForm, sizeof(GUIMessageBoxDialogDeclaration) );

    // Menu Items
    switch( iItem ) {
        case 0: // Expand All
            if ( !(pNode->IsLeaf()) )
                _rec_Expand( pNode );
            break;
        case 1: // Collapse All
            if ( !(pNode->IsLeaf()) )
                _rec_Collapse( pNode );
            break;
        case 2: // Create Package
            // Launch a dialog asking for Package name
            arrDialogForm[0].iItemType = GUIMSGBOX_DIALOGFORM_TEXT;
            StringFn->Copy( arrDialogForm[0].strLabel, TEXT("Package Name :") );

            GUIFn->DialogBox( arrDialogForm, 1, _Callback_CreatePackage, this );
            break;
        case 3: // Create Directory
            // Launch a dialog asking for Directory name
            arrDialogForm[0].iItemType = GUIMSGBOX_DIALOGFORM_TEXT;
            StringFn->Copy( arrDialogForm[0].strLabel, TEXT("Directory Name :") );

            GUIFn->DialogBox( arrDialogForm, 1, _Callback_CreateDirectory, this );
            break;
        case 4: { // Destroy
                UInt iPathLength = 0;
                GChar strPath[RDB_PATH_LENGTH + 1];
                pNode->GetPath( strPath, &iPathLength );

                Bool bIsLeaf = pNode->IsLeaf();
                m_pModel->DestroyNode( pNode );
                pNode = NULL;

                ResourceDB * pRDB = ResourceFn->GetRDB( RDB_MASTER );

                Bool bOk;
                if ( bIsLeaf )
                    bOk = pRDB->DestroyResource( strPath );
                else
                    bOk = pRDB->DestroyDirectory( strPath );
                Assert( bOk );
            } break;
        case 5: // Import
            // Launch a dialog asking for source file name
            arrDialogForm[0].iItemType = GUIMSGBOX_DIALOGFORM_TEXT;
            StringFn->Copy( arrDialogForm[0].strLabel, TEXT("Import from file :") );

            arrDialogForm[1].iItemType = GUIMSGBOX_DIALOGFORM_TEXT;
            StringFn->Copy( arrDialogForm[1].strLabel, TEXT("Import Name :") );

            arrDialogForm[2].iItemType = GUIMSGBOX_DIALOGFORM_CHOICE;
            StringFn->Copy( arrDialogForm[2].strLabel, TEXT("File Type :") );
            arrDialogForm[2].iOptionCount = 2;
            StringFn->Copy( arrDialogForm[2].strOption[0], TEXT("Bitmap") );
            StringFn->Copy( arrDialogForm[2].strOption[1], TEXT("Texture") );

            arrDialogForm[3].iItemType = GUIMSGBOX_DIALOGFORM_CHOICE;
            StringFn->Copy( arrDialogForm[3].strLabel, TEXT("File Format :") );
            arrDialogForm[3].iOptionCount = 2;
            StringFn->Copy( arrDialogForm[3].strOption[0], TEXT("Native") );
            StringFn->Copy( arrDialogForm[3].strOption[1], TEXT("BMP") );

            GUIFn->DialogBox( arrDialogForm, 4, _Callback_Import, this );
            break;
        case 6: // Export
            // Launch a dialog asking for target file name
            arrDialogForm[0].iItemType = GUIMSGBOX_DIALOGFORM_TEXT;
            StringFn->Copy( arrDialogForm[0].strLabel, TEXT("Export to file :") );

            arrDialogForm[1].iItemType = GUIMSGBOX_DIALOGFORM_CHOICE;
            StringFn->Copy( arrDialogForm[1].strLabel, TEXT("File Format :") );
            arrDialogForm[1].iOptionCount = 2;
            StringFn->Copy( arrDialogForm[1].strOption[0], TEXT("Native") );
            StringFn->Copy( arrDialogForm[1].strOption[1], TEXT("BMP") );

            GUIFn->DialogBox( arrDialogForm, 1, _Callback_Export, this );
            break;
        default: Assert(false); break;
    }
}

/////////////////////////////////////////////////////////////////////////////////

Void ResourceViewerContextMenuModel::_Callback_CreatePackage( GUIMessageBoxResult iResult, UInt iItemCount, const GUIMessageBoxDialogResult * arrItemValues, Void * pUserData )
{
    Assert( iItemCount == 1 );
    if ( iResult != GUIMSGBOX_RESULT_TRUE )
        return; // Aborted

    ResourceDB * pRDB = ResourceFn->GetRDB( RDB_MASTER );

    Bool bOk = pRDB->CreateDirectory( RDB_ROOT_PATH, arrItemValues[0].strValue );
    Assert( bOk );

    ResourceViewerContextMenuModel * pThis = (ResourceViewerContextMenuModel*)pUserData;
    GUITreeNode * pNode  = pThis->m_pModel->GetNode( TEXT("/") );

    // Update population
    pThis->m_pModel->CreateNode( pNode, false, arrItemValues[0].strValue, GUISKIN_IMAGE_ICON_UNDEFINED );
}
Void ResourceViewerContextMenuModel::_Callback_CreateDirectory( GUIMessageBoxResult iResult, UInt iItemCount, const GUIMessageBoxDialogResult * arrItemValues, Void * pUserData )
{
    Assert( iItemCount == 1 );
    if ( iResult != GUIMSGBOX_RESULT_TRUE )
        return; // Aborted

    ResourceViewerContextMenuModel * pThis = (ResourceViewerContextMenuModel*)pUserData;
    GUITreeNode * pNode = pThis->m_pModel->GetLastSelectedNode();
    Assert( pNode != NULL );
    if ( pNode->IsLeaf() )
        pNode = pNode->GetParent();

    UInt iPathLength = 0;
    GChar strPath[RDB_PATH_LENGTH + 1];
    pNode->GetPath( strPath, &iPathLength );

    ResourceDB * pRDB = ResourceFn->GetRDB( RDB_MASTER );

    Bool bOk = pRDB->CreateDirectory( strPath, arrItemValues[0].strValue );
    Assert( bOk );

    // Update population
    pThis->m_pModel->CreateNode( pNode, false, arrItemValues[0].strValue, GUISKIN_IMAGE_ICON_UNDEFINED );
}
Void ResourceViewerContextMenuModel::_Callback_Import( GUIMessageBoxResult iResult, UInt iItemCount, const GUIMessageBoxDialogResult * arrItemValues, Void * pUserData )
{
    Assert( iItemCount == 4 );
    if ( iResult != GUIMSGBOX_RESULT_TRUE )
        return; // Aborted

    ResourceViewerContextMenuModel * pThis = (ResourceViewerContextMenuModel*)pUserData;
    GUITreeNode * pNode = pThis->m_pModel->GetLastSelectedNode();
    Assert( pNode != NULL );
    if ( pNode->IsLeaf() )
        pNode = pNode->GetParent();

    UInt iPathLength = 0;
    GChar strPath[RDB_PATH_LENGTH + 1];
    pNode->GetPath( strPath, &iPathLength );

    GChar strResPath[RDB_PATH_LENGTH + 1];
    Assert( iPathLength + StringFn->Length(arrItemValues[1].strValue) + 1 <= RDB_PATH_LENGTH );
    StringFn->Format( strResPath, TEXT("%s/%s"), strPath, arrItemValues[1].strValue );

    Bool bOk;

    switch( arrItemValues[2].iChoice ) {
        case 0: // Bitmap
            switch( arrItemValues[3].iChoice ) {
                case 0: { // Native
                        BitmapBMAP hFile;
                        bOk = hFile.Load( arrItemValues[0].strValue );
                        Assert( bOk );
                        bOk = hFile.Import( RDB_MASTER, strResPath );
                        Assert( bOk );
                    } break;
                case 1: { // BMP
                        BitmapBMP hFile;
                        bOk = hFile.Load( arrItemValues[0].strValue );
                        Assert( bOk );
                        bOk = hFile.Import( RDB_MASTER, strResPath );
                        Assert( bOk );
                    } break;
                default: Assert(false); break;
            }
            break;
        case 1: // Texture
            switch( arrItemValues[3].iChoice ) {
                case 0: { // Native
                        TextureTEX hFile;
                        bOk = hFile.Load( arrItemValues[0].strValue );
                        Assert( bOk );
                        bOk = hFile.Import( RDB_MASTER, strResPath );
                        Assert( bOk );
                    } break;
                case 1: { // BMP
                        TextureBMP hFile;
                        bOk = hFile.Load( arrItemValues[0].strValue );
                        Assert( bOk );
                        bOk = hFile.Import( RDB_MASTER, strResPath );
                        Assert( bOk );
                    } break;
                default: Assert(false); break;
            }
            break;
        default: Assert(false); break;
    }

    // Update population
    pThis->m_pModel->CreateNode( pNode, true, arrItemValues[1].strValue, GUISKIN_IMAGE_ICON_UNDEFINED );
}
Void ResourceViewerContextMenuModel::_Callback_Export( GUIMessageBoxResult iResult, UInt iItemCount, const GUIMessageBoxDialogResult * arrItemValues, Void * pUserData )
{
    Assert( iItemCount == 2 );
    if ( iResult != GUIMSGBOX_RESULT_TRUE )
        return; // Aborted

    ResourceViewerContextMenuModel * pThis = (ResourceViewerContextMenuModel*)pUserData;
    GUITreeNode * pNode = pThis->m_pModel->GetLastSelectedNode();
    Assert( pNode != NULL && pNode->IsLeaf() );

    UInt iPathLength = 0;
    GChar strPath[RDB_PATH_LENGTH + 1];
    pNode->GetPath( strPath, &iPathLength );

    ResourceDB * pRDB = ResourceFn->GetRDB( RDB_MASTER );

    RDB_ResourceInfos hInfos;
    Bool bOk = pRDB->GetResourceInfo( &hInfos, strPath );
    Assert( bOk );

    switch( hInfos.iType ) {
        case RESOURCE_TYPE_BITMAP: {
                switch( arrItemValues[1].iChoice ) {
                    case 0: { // Native
                            BitmapBMAP hFile;
                            bOk = hFile.Export( RDB_MASTER, strPath );
                            Assert( bOk );
                            bOk = hFile.Save( arrItemValues[0].strValue );
                            Assert( bOk );
                        } break;
                    case 1: { // BMP
                            BitmapBMP hFile;
                            bOk = hFile.Export( RDB_MASTER, strPath );
                            Assert( bOk );
                            bOk = hFile.Save( arrItemValues[0].strValue );
                            Assert( bOk );
                        } break;
                    default: Assert(false); break;
                }
            } break;
        case RESOURCE_TYPE_TEXTURE: {
                switch( arrItemValues[1].iChoice ) {
                    case 0: { // Native
                            TextureTEX hFile;
                            bOk = hFile.Export( RDB_MASTER, strPath );
                            Assert( bOk );
                            bOk = hFile.Save( arrItemValues[0].strValue );
                            Assert( bOk );
                        } break;
                    case 1: { // BMP
                            TextureBMP hFile;
                            bOk = hFile.Export( RDB_MASTER, strPath );
                            Assert( bOk );
                            bOk = hFile.Save( arrItemValues[0].strValue );
                            Assert( bOk );
                        } break;
                    default: Assert(false); break;
                }
            } break;
        default: Assert(false); break;
    }
}

Void ResourceViewerContextMenuModel::_rec_Expand( GUITreeNode * pNode )
{
    if ( pNode->IsLeaf() )
        return;

    m_pModel->Expand( pNode );

    for( UInt i = 0; i < pNode->GetChildCount(); ++i )
        _rec_Expand( pNode->GetChild( i ) );
}
Void ResourceViewerContextMenuModel::_rec_Collapse( GUITreeNode * pNode )
{
    if ( pNode->IsLeaf() )
        return;

    for( UInt i = 0; i < pNode->GetChildCount(); ++i )
        _rec_Collapse( pNode->GetChild( i ) );

    m_pModel->Collapse( pNode );
}

/////////////////////////////////////////////////////////////////////////////////
// ResourceViewerModel implementation
ResourceViewerModel::ResourceViewerModel():
    GUITreeModel( 10 )
{
    m_arrSubDirs.UseMemoryContext( ConsoleFn->GetMemoryContext(), TEXT("Interfaces") );
    m_arrSubDirs.Create();
    m_arrSubRes.UseMemoryContext( ConsoleFn->GetMemoryContext(), TEXT("Interfaces") );
    m_arrSubRes.Create();

    // Populate root
    ResourceDB * pRDB = ResourceFn->GetRDB( RDB_MASTER );
    Bool bOk = pRDB->ListDirectory( RDB_ROOT_PATH, &m_arrSubDirs, &m_arrSubRes );
    Assert( bOk );

    for( UInt i = 0; i < m_arrSubDirs.Count(); ++i )
        CreateNode( NULL, false, m_arrSubDirs[i].strName, GUISKIN_IMAGE_ICON_UNDEFINED );
    for( UInt i = 0; i < m_arrSubRes.Count(); ++i )
        CreateNode( NULL, true, m_arrSubRes[i].strName, GUISKIN_IMAGE_ICON_UNDEFINED );

    // Context menu
    ConsoleFn->SelectMemory( TEXT("Interfaces") );
    m_pResourceViewerContextMenuModel = New ResourceViewerContextMenuModel( this );
    ConsoleFn->UnSelectMemory();

    SetContextMenu( m_pResourceViewerContextMenuModel );
}
ResourceViewerModel::~ResourceViewerModel()
{
    SetContextMenu( NULL );

    ConsoleFn->SelectMemory( TEXT("Interfaces") );
    Delete( m_pResourceViewerContextMenuModel );
    m_pResourceViewerContextMenuModel = NULL;
    ConsoleFn->UnSelectMemory();

    m_arrSubDirs.Destroy();
    m_arrSubRes.Destroy();
}

Void ResourceViewerModel::OnPopulateNode( GUITreeNode * pNode )
{
    // Retrieve path for this node
    UInt iPathLength = 0;
    GChar strPath[RDB_PATH_LENGTH + 1];

    pNode->GetPath( strPath, &iPathLength );

    // List sub-directories & resources
    ResourceDB * pRDB = ResourceFn->GetRDB( RDB_MASTER );

    m_arrSubDirs.Clear();
    m_arrSubRes.Clear();
    Bool bOk = pRDB->ListDirectory( strPath, &m_arrSubDirs, &m_arrSubRes );
    Assert( bOk );

    // Populate node
    for( UInt i = 0; i < m_arrSubDirs.Count(); ++i )
        CreateNode( pNode, false, m_arrSubDirs[i].strName, GUISKIN_IMAGE_ICON_UNDEFINED );
    for( UInt i = 0; i < m_arrSubRes.Count(); ++i )
        CreateNode( pNode, true, m_arrSubRes[i].strName, GUISKIN_IMAGE_ICON_UNDEFINED );
}

/////////////////////////////////////////////////////////////////////////////////
// ResourceViewerWindow implementation
ResourceViewerWindow::ResourceViewerWindow()
{
    m_pResourceViewerModel = NULL;
    m_pResourceViewer = NULL;

    m_pWindow = NULL;

    _CreateResourceView();
}
ResourceViewerWindow::~ResourceViewerWindow()
{
    _DestroyResourceView();
}

/////////////////////////////////////////////////////////////////////////////////

Void ResourceViewerWindow::_CreateResourceView()
{
    // Models
    ConsoleFn->SelectMemory( TEXT("Interfaces") );
    m_pResourceViewerModel = New ResourceViewerModel();
    ConsoleFn->UnSelectMemory();

    // Window
    m_pWindow = GUIFn->CreateWindow( TEXT("Resource Viewer"), Rectangle2( Point2(16,16), 200, 400 ), GUIWINDOW_STYLE_DEFAULTEX );
    GUIWidget * pClientRoot = m_pWindow->GetClientRoot();

    // Layout
    GUIGridLayout hLayout;

    hLayout.Anchor = GUILAYOUT_ANCHOR_NORTHWEST;
    hLayout.FillX = GUILAYOUT_FILL_RELATIVE;
    hLayout.FillY = GUILAYOUT_FILL_RELATIVE;
    hLayout.RelativeSize.X = 1.0f;
    hLayout.RelativeSize.Y = 1.0f;

    GUIFn->CreateWidget( GUIWIDGET_TREE, m_pResourceViewerModel, pClientRoot, hLayout, TEXT("ResourceViewer_Tree") );

    // Setup
    GUIFn->RegisterWindow( m_pWindow );
    m_pWindow->SetVisible( false );
}
Void ResourceViewerWindow::_DestroyResourceView()
{
    // Setup
    GUIFn->UnregisterWindow( m_pWindow );

    // Window
    GUIFn->DestroyWindow( m_pWindow );
    m_pWindow = NULL;

    // Models
    ConsoleFn->SelectMemory( TEXT("Interfaces") );
    Delete( m_pResourceViewerModel );
    m_pResourceViewerModel = NULL;
    ConsoleFn->UnSelectMemory();
}

