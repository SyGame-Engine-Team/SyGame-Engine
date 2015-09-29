/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Widgets/Common/GUIComboList.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Widget : Common controls, ComboList
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
#include "GUIComboList.h"

#include "../../GUIManager.h"

/////////////////////////////////////////////////////////////////////////////////
// GUIComboListMenuModel implementation
GUIComboListMenuModel::GUIComboListMenuModel( GUIComboListModel * pModel, UInt iItemCount, const GChar ** arrItemNames, GUISkinImage * arrItemIcons, Void ** arrItemUserDatas ):
    GUIMenuModel( 16, iItemCount, arrItemNames, arrItemIcons, arrItemUserDatas, NULL )
{
    m_pModel = pModel;
}
GUIComboListMenuModel::~GUIComboListMenuModel()
{
    // nothing to do
}

Void GUIComboListMenuModel::OnValidate( UInt iItem )
{
    UInt iOldItem = m_pModel->m_iSelectedItem;
    m_pModel->m_iSelectedItem = iItem;
    m_pModel->m_bOpened = false;

    // Raise callback
    if ( iItem != iOldItem )
        m_pModel->OnChoicePick( GetItem(iItem)->GetUserData() );
}
Void GUIComboListMenuModel::OnAbort()
{
    m_pModel->m_bOpened = false;
}

/////////////////////////////////////////////////////////////////////////////////
// GUIComboListModel implementation
GUIComboListModel::GUIComboListModel( UInt iItemCount, const GChar ** arrItemNames, GUISkinImage * arrItemIcons, Void ** arrItemUserDatas ):
    GUIWidgetModel()
{
    m_iSelectedItem = 0;
    m_bOpened = false;

    GUIFn->SelectMemory( TEXT("LayoutData") );
    m_pComboListMenuModel = New GUIComboListMenuModel( this, iItemCount, arrItemNames, arrItemIcons, arrItemUserDatas );
    GUIFn->UnSelectMemory();
}
GUIComboListModel::~GUIComboListModel()
{
    GUIFn->SelectMemory( TEXT("LayoutData") );
    Delete( m_pComboListMenuModel );
    m_pComboListMenuModel = NULL;
    GUIFn->UnSelectMemory();
}

Void GUIComboListModel::OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags )
{
    // Handle context menu
    if ( iKey == KEYCODE_MOUSERIGHT ) {
        GUIWidgetModel::OnClick( ptLocalPos, iKey, iFlags );
        return;
    }

    if ( iKey != KEYCODE_MOUSELEFT )
        return;

    // Widget
    GUIWidget * pWidget = _GetWidget();

    // Enter ComboList menu
    Point2 ptScreenPos;
    ptScreenPos.X = pWidget->GetClientRect().TopLeft.X;
    ptScreenPos.Y = pWidget->GetClientRect().DownRight.Y;

    GUIFn->EnterMenuLoop( m_pComboListMenuModel, TEXT("<ComboListMenu>"), ptScreenPos );

    // Done
    m_bOpened = true;
}

Void GUIComboListModel::OnChoicePick( Void * /*pUserData*/ )
{
    // stub method, do nothing
}

/////////////////////////////////////////////////////////////////////////////////
// GUIComboList implementation
GUIComboList::GUIComboList( GUIComboListModel * pModel, GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName ):
    GUIWidget( pModel, pParent, guiLayout, strName )
{
    // nothing to do
}
GUIComboList::~GUIComboList()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

Void GUIComboList::_Draw( const Rectangle2 & rectClient )
{
    GPU2DContext * pGUIContext = GUIFn->GetScreenContext();

    // Model
    GUIComboListModel * pModel = (GUIComboListModel*)_GetWidgetModel();

    GUISkinImage iBackground = pModel->IsOpened() ? GUISKIN_IMAGE_BUTTON_DOWN : GUISKIN_IMAGE_BUTTON_UP;

    GUISkinColor iTextColor = pModel->IsOpened() ? GUISKIN_COLOR_BUTTON_TEXTDOWN : GUISKIN_COLOR_BUTTON_TEXTUP;

    UInt iTextLength = 0;
    const GChar * strText = pModel->GetSelectedItemName( &iTextLength );

    // Skin
    const GUISkin * pSkin = GUIFn->GetSkin();

    GPU2DBitmap * pSkinBitmap = pSkin->GetSkinBitmap();
    const Rectangle2 & locBackground = pSkin->GetLocation( iBackground );

    GPU2DColor hTextColor = pSkin->GetColor( iTextColor );

    GPU2DSolidColorBrush * pTextBrush = pSkin->GetSkinTextBrush();
    GPU2DTextFormat * pTextFormat = pSkin->GetSkinTextFormat( GUISKIN_FONT_COMBOLIST );

    // Background
    GPU2DRectangle hDstRect = rectClient;
    GPU2DRectangle hSrcRect = locBackground;

    pGUIContext->DrawBitmap( pSkinBitmap, &hDstRect, &hSrcRect );

    // Text
    pTextFormat->SetTextAlignment( GPU2DTEXT_ALIGNMENT_LEADING );
    pTextFormat->SetParagraphAlignment( GPU2DTEXT_PARAGRAPHALIGNMENT_CENTER );

    pTextBrush->SetColor( &hTextColor );
    pGUIContext->DrawText( strText, iTextLength, pTextFormat, &hDstRect, pTextBrush, GPU2DTEXT_DRAWOPTION_CLIP );
}



