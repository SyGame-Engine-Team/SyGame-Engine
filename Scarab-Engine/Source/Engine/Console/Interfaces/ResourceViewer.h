/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Console/Interfaces/ResourceViewer.h
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
// Header prelude
#ifndef SCARAB_ENGINE_CONSOLE_INTERFACES_RESOURCEVIEWER_H
#define SCARAB_ENGINE_CONSOLE_INTERFACES_RESOURCEVIEWER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../GUI/GUIManager.h"

#include "../../Resource/ResourceManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Prototypes
class ResourceViewerContextMenuModel;
class ResourceViewerModel;
class ResourceViewerWindow;

/////////////////////////////////////////////////////////////////////////////////
// The ResourceViewerContextMenuModel class
class ResourceViewerContextMenuModel : public GUIMenuModel
{
public:
    ResourceViewerContextMenuModel( ResourceViewerModel * pModel );
    virtual ~ResourceViewerContextMenuModel();

    // Controller
    virtual Void OnValidate( UInt iItem );

protected:
    // Menu layout
    static const UInt sm_iItemCount = 7;
    static const GChar * sm_arrItemNames[sm_iItemCount];
    static GUISkinImage sm_arrItemIcons[sm_iItemCount];

    // Dialog callbacks
    static Void _Callback_CreatePackage( GUIMessageBoxResult iResult, UInt iItemCount, const GUIMessageBoxDialogResult * arrItemValues, Void * pUserData );
    static Void _Callback_CreateDirectory( GUIMessageBoxResult iResult, UInt iItemCount, const GUIMessageBoxDialogResult * arrItemValues, Void * pUserData );
    static Void _Callback_Import( GUIMessageBoxResult iResult, UInt iItemCount, const GUIMessageBoxDialogResult * arrItemValues, Void * pUserData );
    static Void _Callback_Export( GUIMessageBoxResult iResult, UInt iItemCount, const GUIMessageBoxDialogResult * arrItemValues, Void * pUserData );

    // Target model
    Void _rec_Expand( GUITreeNode * pNode );
    Void _rec_Collapse( GUITreeNode * pNode );

    ResourceViewerModel * m_pModel;
};

/////////////////////////////////////////////////////////////////////////////////
// The ResourceViewerModel class
class ResourceViewerModel : public GUITreeModel
{
public:
    ResourceViewerModel();
    virtual ~ResourceViewerModel();

    // Controller
    virtual Void OnPopulateNode( GUITreeNode * pNode );

protected:
    Array<RDB_DirectoryInfos> m_arrSubDirs;
    Array<RDB_ResourceInfos> m_arrSubRes;

    ResourceViewerContextMenuModel * m_pResourceViewerContextMenuModel;
};

/////////////////////////////////////////////////////////////////////////////////
// The ResourceViewerWindow class
class ResourceViewerWindow
{
public:
    ResourceViewerWindow();
    ~ResourceViewerWindow();

    inline Void Show();
    inline Void Hide();
    inline Void Toggle();

private:
    // GUI data
    Void _CreateResourceView();
    Void _DestroyResourceView();

    ResourceViewerModel * m_pResourceViewerModel;
    GUITree * m_pResourceViewer;

    GUIWindow * m_pWindow;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "ResourceViewer.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_CONSOLE_INTERFACES_RESOURCEVIEWER_H


