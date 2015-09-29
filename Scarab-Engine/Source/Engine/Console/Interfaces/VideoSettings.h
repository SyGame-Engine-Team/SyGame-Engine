/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Console/Interfaces/VideoSettings.h
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
// Header prelude
#ifndef SCARAB_ENGINE_CONSOLE_INTERFACES_VIDEOSETTINGS_H
#define SCARAB_ENGINE_CONSOLE_INTERFACES_VIDEOSETTINGS_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../GUI/GUIManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Prototypes
class VideoSettingsResolutionModel;
class VideoSettingsFullscreenModel;
class VideoSettingsWindow;

/////////////////////////////////////////////////////////////////////////////////
// The VideoSettingsResolutionModel class
class VideoSettingsResolutionModel : public GUIComboListModel
{
public:
    VideoSettingsResolutionModel( UInt iItemCount, const GChar ** arrItemNames, GUISkinImage * arrItemIcons, Void ** arrItemIndices );
    virtual ~VideoSettingsResolutionModel();

    // Controller
    virtual Void OnChoicePick( Void * pUserData );

protected:
};

/////////////////////////////////////////////////////////////////////////////////
// The VideoSettingsFullscreenModel class
class VideoSettingsFullscreenModel : public GUIComboListModel
{
public:
    VideoSettingsFullscreenModel( VideoSettingsWindow * pWindow, UInt iItemCount, const GChar ** arrItemNames, GUISkinImage * arrItemIcons );
    virtual ~VideoSettingsFullscreenModel();

    // Controller
    virtual Void OnChoicePick( Void * pUserData );

protected:
    VideoSettingsWindow * m_pWindow;
};

/////////////////////////////////////////////////////////////////////////////////
// The VideoSettingsWindow class
class VideoSettingsWindow
{
public:
    VideoSettingsWindow();
    ~VideoSettingsWindow();

    inline Void Show();
    inline Void Hide();
    inline Void Toggle();

private:
    friend class VideoSettingsFullscreenModel;

    // GUI data
    Void _CreateVideoSettings();
    Void _DestroyVideoSettings();

    GUISectionModel * m_pSectionModel;

    GUILabelModel * m_pResolutionLabelModel;
    VideoSettingsResolutionModel * m_pResolutionModel;

    GUILabelModel * m_pFullscreenLabelModel;
    VideoSettingsFullscreenModel * m_pFullscreenModel;

    GUIWindow * m_pWindow;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "VideoSettings.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_CONSOLE_INTERFACES_VIDEOSETTINGS_H
