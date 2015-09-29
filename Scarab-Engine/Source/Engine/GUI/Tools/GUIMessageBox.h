/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Tools/GUIMessageBox.h
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
// Header prelude
#ifndef SCARAB_ENGINE_GUI_TOOLS_GUIMESSAGEBOX_H
#define SCARAB_ENGINE_GUI_TOOLS_GUIMESSAGEBOX_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../GUIWindow.h"

#include "../Widgets/Common/GUIContainer.h"
#include "../Widgets/Common/GUISection.h"
#include "../Widgets/Common/GUILabel.h"
#include "../Widgets/Common/GUIButton.h"
#include "../Widgets/Common/GUITextInput.h"
#include "../Widgets/Common/GUICheckBox.h"
#include "../Widgets/Common/GUIRadioButton.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

    // Message-Box layout
#define GUIMSGBOX_WIDTH  400
#define GUIMSGBOX_HEIGHT 200

#define GUIMSGBOX_TITLE_LENGTH GUILABEL_MAX_LENGTH
#define GUIMSGBOX_TEXT_LENGTH  GUILABEL_MAX_LENGTH
#define GUIMSGBOX_MAX_BUTTONS  3

    // Message-Box types
enum GUIMessageBoxType
{
    GUIMSGBOX_TYPE_UNDEFINED = 0,
    GUIMSGBOX_TYPE_DIALOG,
    GUIMSGBOX_TYPE_INFORMATION,
    GUIMSGBOX_TYPE_WARNING,
    GUIMSGBOX_TYPE_ALERT,
    GUIMSGBOX_TYPE_ERROR,
    GUIMSGBOX_TYPE_FATALERROR
};

    // Message-Box dialog form items
enum GUIMessageBoxDialogFormItem
{
    GUIMSGBOX_DIALOGFORM_TEXT = 0, // Plain text value
    GUIMSGBOX_DIALOGFORM_OPTIONS,  // Check boxes
    GUIMSGBOX_DIALOGFORM_CHOICE    // Radio buttons
};

    // Message-Box dialog declaration
#define GUIMSGBOX_MAX_DIALOG_ITEMS 32
#define GUIMSGBOX_MAX_OPTIONS      8

typedef struct _gui_messagebox_dialog_decl {
    GUIMessageBoxDialogFormItem iItemType;
    GChar strLabel[GUIMSGBOX_TEXT_LENGTH];
    UInt iOptionCount;
    GChar strOption[GUIMSGBOX_MAX_OPTIONS][GUIMSGBOX_TEXT_LENGTH];
} GUIMessageBoxDialogDeclaration;

typedef union _gui_messagebox_dialog_result {
    const GChar * strValue;
    UInt iOptions;
    UInt iChoice;
} GUIMessageBoxDialogResult;

    // Message-Box buttons
enum GUIMessageBoxButtons
{
    GUIMSGBOX_BUTTONS_OK = 0,
    GUIMSGBOX_BUTTONS_OK_CANCEL,
    GUIMSGBOX_BUTTONS_YES_NO,
    GUIMSGBOX_BUTTONS_CONTINUE_STOP,
    GUIMSGBOX_BUTTONS_RETRY_IGNORE_ABORT
};

    // Message-Box results
enum GUIMessageBoxResult
{
    GUIMSGBOX_RESULT_FALSE = 0, // CANCEL / NO / STOP / ABORT
    GUIMSGBOX_RESULT_TRUE,      // OK / YES / CONTINUE / RETRY
    GUIMSGBOX_RESULT_SPECIAL    // IGNORE
};

    // Message-Box callback
typedef Void (*GUIMsgBoxCallback)( GUIMessageBoxResult, Void* );
typedef Void (*GUIMsgBoxDialogCallback)( GUIMessageBoxResult, UInt, const GUIMessageBoxDialogResult *, Void* );

    // Prototypes
class GUIMsgBoxButtonModel;

class GUIMessageBox;

/////////////////////////////////////////////////////////////////////////////////
// The GUIMsgBoxButtonModel class
class GUIMsgBoxButtonModel : public GUIButtonModel
{
public:
    GUIMsgBoxButtonModel( GUIMessageBox * pMessageBox, const GChar * strText );
    virtual ~GUIMsgBoxButtonModel();

    inline Void SetResult( GUIMessageBoxResult iResult );
    inline Void SetCallback( GUIMsgBoxCallback pfCallback, Void * pUserData );
    inline Void SetCallback( GUIMsgBoxDialogCallback pfDialogCallback, Void * pUserData );

    virtual Void OnPress();

private:
    GUIMessageBox * m_pMessageBox;

    GUIMessageBoxResult m_iResult;
    GUIMsgBoxCallback m_pfCallback;
    GUIMsgBoxDialogCallback m_pfDialogCallback;
    Void * m_pUserData;
};

/////////////////////////////////////////////////////////////////////////////////
// The GUIMessageBox class
class GUIMessageBox
{
public:
    GUIMessageBox();
    ~GUIMessageBox();

    inline Bool IsOpened() const;

    Bool Open( GUIMessageBoxType iType, const GChar * strMessage, GUIMessageBoxButtons iButtons,
               GUIMsgBoxCallback pfCallback = NULL, Void * pUserData = NULL );
    Bool OpenDialog( const GUIMessageBoxDialogDeclaration * arrDialogFormItems, UInt iDialogFormItemCount,
                     GUIMsgBoxDialogCallback pfCallback = NULL, Void * pUserData = NULL );
    Void Close();

    Void ExtractDialogResults( UInt * outItemCount, GUIMessageBoxDialogResult * outItemValues ) const;

private:
    // Models
    GUISectionModel * m_pSectionModel;

    GUILabelModel * m_pTitleModel;
    GUIMsgBoxButtonModel * m_arrButtonModels[GUIMSGBOX_MAX_BUTTONS];

    GUILabelModel * m_pMessageModel;

    UInt m_iDialogItemCount;
    GUIMessageBoxDialogFormItem m_arrDialogItemTypes[GUIMSGBOX_MAX_DIALOG_ITEMS];
    UInt m_arrDialogOptionCounts[GUIMSGBOX_MAX_DIALOG_ITEMS];

    GUILabelModel * m_arrDialogLabelModels[GUIMSGBOX_MAX_DIALOG_ITEMS];
    GUILabelModel * m_arrDialogOptionLabelModels[GUIMSGBOX_MAX_DIALOG_ITEMS][GUIMSGBOX_MAX_OPTIONS];

    GUITextInputModel * m_arrDialogTextModels[GUIMSGBOX_MAX_DIALOG_ITEMS];

    GUICheckBoxModel * m_arrDialogOptionModels[GUIMSGBOX_MAX_DIALOG_ITEMS][GUIMSGBOX_MAX_OPTIONS];

    GUIRadioButtonModel * m_arrDialogChoiceModels[GUIMSGBOX_MAX_DIALOG_ITEMS][GUIMSGBOX_MAX_OPTIONS];
    GUIRadioButtonGroup * m_arrDialogChoiceGroups[GUIMSGBOX_MAX_DIALOG_ITEMS];

    // Window
    GUIContainer * m_pDialogForm;
    GUIContainer * m_pButtons;
    GUIWindow * m_pWindow;

    // State
    Bool m_bOpened;
    GUIMessageBoxType m_iType;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GUIMessageBox.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_GUI_TOOLS_GUIMESSAGEBOX_H

