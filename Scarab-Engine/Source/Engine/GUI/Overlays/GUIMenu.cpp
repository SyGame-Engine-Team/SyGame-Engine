/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Overlays/GUIMenu.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Overlay : Generic menu interface
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
#include "GUIMenu.h"

#include "../GUIManager.h"

/////////////////////////////////////////////////////////////////////////////////
// GUIMenuItem implementation
GUIMenuItem::GUIMenuItem()
{
    m_bActive = true;

    m_pSubMenu = NULL;

    m_idIcon = GUISKIN_IMAGE_ICON_UNDEFINED;
    m_iNameLength = 0;
    m_strName[0] = NULLBYTE;
    m_pUserData = NULL;
}
GUIMenuItem::GUIMenuItem( const GChar * strName, GUISkinImage idIcon, Void * pUserData, GUIMenuModel * pSubMenu )
{
    m_bActive = true;

    m_pSubMenu = pSubMenu;

    
    m_idIcon = idIcon;
    GChar * pEnd = StringFn->NCopy( m_strName, strName, GUI_NAME_LENGTH );
    m_iNameLength = ( pEnd - m_strName );
    m_pUserData = pUserData;
}
GUIMenuItem::~GUIMenuItem()
{
    if ( m_pSubMenu != NULL )
        Delete( m_pSubMenu );
}

/////////////////////////////////////////////////////////////////////////////////
// GUIMenuModel implementation
GUIMenuModel::GUIMenuModel( UInt iItemHeight, UInt iItemCount, const GChar * arrNames[], GUISkinImage arrIcons[], Void * arrUserDatas[], GUIMenuModel * arrSubMenus[] ):
    GUIOverlayModel()
{
    Assert( iItemCount > 0 );
    Assert( arrNames != NULL );

    m_iItemHeight = iItemHeight;

    m_iSelectedItem = INVALID_OFFSET;

    m_iItemCount = iItemCount;
    m_arrItems = New GUIMenuItem[m_iItemCount];

    for( UInt iItem = 0; iItem < m_iItemCount; ++iItem ) {
        GChar * pEnd = StringFn->NCopy( m_arrItems[iItem].m_strName, arrNames[iItem], GUI_NAME_LENGTH );
        m_arrItems[iItem].m_iNameLength = ( pEnd - m_arrItems[iItem].m_strName );

        m_arrItems[iItem].m_idIcon = arrIcons[iItem];

        if ( arrUserDatas != NULL )
            m_arrItems[iItem].m_pUserData = arrUserDatas[iItem];

        if ( arrSubMenus != NULL )
            m_arrItems[iItem].m_pSubMenu = arrSubMenus[iItem];
    }
}
GUIMenuModel::~GUIMenuModel()
{
    // Sub menus
    DeleteA( m_arrItems );
}

Void GUIMenuModel::OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag /*iFlags*/ )
{
    if ( iKey != KEYCODE_MOUSELEFT )
        return;

    // Pick item
    UInt iItem = ( ptLocalPos.Y / m_iItemHeight );

    // Validate item
    if ( m_arrItems[iItem].m_bActive ) {
        Assert( iItem == m_iSelectedItem );
        if ( m_arrItems[iItem].m_pSubMenu == NULL ) {
            OnValidate( iItem );
            GUIFn->LeaveMenuLoop( false );
        }
    }
}

Void GUIMenuModel::OnRollOver( const Point2 & ptLocalPos, GUIEventFlag /*iFlags*/ )
{
    // Overlay
    GUIMenu * pMenu = (GUIMenu*)_GetOverlay();

    // Pick previous item
    UInt iPrevItem = m_iSelectedItem;

    // Pick new item
    UInt iItem = ( ptLocalPos.Y / m_iItemHeight );
    if ( iItem == iPrevItem )
        return;

    // Collapse previous submenu
    pMenu->Collapse();

    // Switch item selection
    SelectItem( iItem );

    // Expand new submenu
    pMenu->Expand();
}

Void GUIMenuModel::OnValidate( UInt /*iItem*/ )
{
    // stub method, do nothing
}
Void GUIMenuModel::OnAbort()
{
    // stub method, do nothing
}

/////////////////////////////////////////////////////////////////////////////////
// GUIMenu implementation
GUIMenu::GUIMenu( GUIMenuModel * pModel, const GChar * strName, const Point2 & ptScreenPos ):
    GUIOverlay( pModel, strName, _ComputePlacement(pModel, ptScreenPos) )
{
    // Get screen dimensions
    UInt iScreenWidth, iScreenHeight;
    GUIFn->GetScreenSize( &iScreenWidth, &iScreenHeight );

    GUIFn->SelectMemory( TEXT("LayoutData") );

    // Build sub-menus
    m_iItemCount = pModel->GetItemCount();
    m_arrSubMenus = New GUIMenu*[m_iItemCount];

    Rectangle2 rectParentMenu = GetOverlayRect();
    UInt iItemHeight = pModel->GetItemHeight();

    const GUIMenuItem * pMenuItem;
    GChar strSubMenuName[GUI_NAME_LENGTH+1];
    UInt iSubMenuWidth, iSubMenuHeight;
    Point2 ptSubMenuPos;

    for( UInt iItem = 0; iItem < m_iItemCount; ++iItem ) {
        pMenuItem = pModel->GetItem( iItem );
        if ( !(pMenuItem->HasSubMenu()) ) {
            m_arrSubMenus[iItem] = NULL;
            continue;
        }

        // SubMenu name & dimensions
        StringFn->Format( strSubMenuName, TEXT("%s_%ud"), strName, iItem );
        iSubMenuWidth = GUISKIN_SIZE_ICON_SMALL + GUIMENU_TEXTWIDTH + GUISKIN_SIZE_ICON_TINY + ( GUISKIN_MARGINS << 2 );
        iSubMenuHeight = pMenuItem->GetSubMenu()->GetItemCount() * pMenuItem->GetSubMenu()->GetItemHeight();

        // Right/Left placement
        ptSubMenuPos.X = ( rectParentMenu.DownRight.X + 1 );
        if ( ptSubMenuPos.X + iSubMenuWidth > iScreenWidth ) {
            ptSubMenuPos.X = ( rectParentMenu.TopLeft.X - iSubMenuWidth );
            Assert( ptSubMenuPos.X >= 0 );
        }

        // Down/Up placement
        ptSubMenuPos.Y = ( rectParentMenu.TopLeft.Y + (iItem * iItemHeight) );
        if ( ptSubMenuPos.Y + iSubMenuHeight > iScreenHeight ) {
            ptSubMenuPos.Y += ( iItemHeight - iSubMenuHeight );
            Assert( ptSubMenuPos.Y >= 0 );
        }

        m_arrSubMenus[iItem] = New GUIMenu( pMenuItem->GetSubMenu(), strSubMenuName, ptSubMenuPos );
    }

    GUIFn->UnSelectMemory();
}
GUIMenu::~GUIMenu()
{
    GUIFn->SelectMemory( TEXT("LayoutData") );

    // Destroy sub-menus
    for( UInt iItem = 0; iItem < m_iItemCount; ++iItem ) {
        if ( m_arrSubMenus[iItem] != NULL )
            Delete( m_arrSubMenus[iItem] );
    }
    DeleteA( m_arrSubMenus );

    GUIFn->UnSelectMemory();
}

Void GUIMenu::Expand()
{
    // Model
    GUIMenuModel * pModel = (GUIMenuModel*)_GetOverlayModel();

    // Get selected item
    UInt iSelected = pModel->GetSelectedItem();

    // No-selection case
    if ( iSelected == INVALID_OFFSET )
        return;

    // Get submenu
    Assert( iSelected < m_iItemCount );
    GUIMenu * pSubMenu = m_arrSubMenus[iSelected];

    // No submenu case
    if ( pSubMenu == NULL )
        return;

    // Expand
    pSubMenu->SetVisible( true );
}
Void GUIMenu::Collapse()
{
    // Model
    GUIMenuModel * pModel = (GUIMenuModel*)_GetOverlayModel();

    // Get selected item
    UInt iSelected = pModel->GetSelectedItem();

    // No-selection case
    if ( iSelected == INVALID_OFFSET )
        return;

    // Get submenu
    Assert( iSelected < m_iItemCount );
    GUIMenu * pSubMenu = m_arrSubMenus[iSelected];

    // No submenu case
    if ( pSubMenu == NULL )
        return;

    // Recurse
    pSubMenu->Collapse();

    // Collapse
    pSubMenu->SetVisible( false );
}

GUIMenu * GUIMenu::GetSelectedMenu()
{
    // Check visibility (root case)
    if ( !IsVisible() )
        return NULL;

    // Model
    GUIMenuModel * pModel = (GUIMenuModel*)_GetOverlayModel();

    // Get selected item
    UInt iSelected = pModel->GetSelectedItem();

    // No-selection case
    if ( iSelected == INVALID_OFFSET )
        return this;

    // Get submenu
    Assert( iSelected < m_iItemCount );
    GUIMenu * pSubMenu = m_arrSubMenus[iSelected];

    // No submenu case
    if ( pSubMenu == NULL )
        return this;

    // Not yet visible case
    if ( !(pSubMenu->IsVisible()) )
        return this;

    // Recurse
    return pSubMenu->GetSelectedMenu();
}

/////////////////////////////////////////////////////////////////////////////////

Void GUIMenu::_Draw( const Rectangle2 & rectClient )
{
    GPU2DContext * pGUIContext = GUIFn->GetScreenContext();

    // Model
    GUIMenuModel * pModel = (GUIMenuModel*)_GetOverlayModel();

    UInt iItemHeight = pModel->GetItemHeight();

    // Skin
    const GUISkin * pSkin = GUIFn->GetSkin();

    GPU2DBitmap * pSkinBitmap = pSkin->GetSkinBitmap();
    const Rectangle2 & locBackground = pSkin->GetLocation( GUISKIN_IMAGE_MENU_BACKGROUND );
    const Rectangle2 & locArrow = pSkin->GetLocation( GUISKIN_IMAGE_MENU_ARROW );

    GPU2DSolidColorBrush * pColorBrush = pSkin->GetSkinColorBrush();
    GPU2DColor hSelectionColor = pSkin->GetColor( GUISKIN_COLOR_MENU_SELECTION );
    GPU2DColor hActiveText = pSkin->GetColor( GUISKIN_COLOR_MENU_TEXTACTIVE );
    GPU2DColor hInactiveText = pSkin->GetColor( GUISKIN_COLOR_MENU_TEXTINACTIVE );

    GPU2DSolidColorBrush * pTextBrush = pSkin->GetSkinTextBrush();
    GPU2DTextFormat * pTextFormat = pSkin->GetSkinTextFormat( GUISKIN_FONT_BUTTON );

    // Background
    GPU2DRectangle hDstRect = rectClient;
    GPU2DRectangle hSrcRect = locBackground;

    pGUIContext->DrawBitmap( pSkinBitmap, &hDstRect, &hSrcRect );

    // Menu rows
    // Margin | (Icon) | Margin | Text | Margin | (Arrow) | Margin
    // Margin |                 Selection                 | Margin
    hDstRect.fTop = (Float)( rectClient.TopLeft.Y );
    
    pTextFormat->SetTextAlignment( GPU2DTEXT_ALIGNMENT_LEADING );
    pTextFormat->SetParagraphAlignment( GPU2DTEXT_PARAGRAPHALIGNMENT_CENTER );

    for( UInt iItem = 0; iItem < m_iItemCount; ++iItem ) {
        const GUIMenuItem * pMenuItem = pModel->GetItem( iItem );

        hDstRect.fLeft = (Float)( rectClient.TopLeft.X + GUISKIN_MARGINS );

        // Selection effect
        if ( iItem == pModel->GetSelectedItem() ) {
            hDstRect.fRight = hDstRect.fLeft + (Float)( rectClient.Width() - (GUISKIN_MARGINS << 1) );
            hDstRect.fBottom = hDstRect.fTop + (Float)iItemHeight;

            pColorBrush->SetColor( &hSelectionColor );
            pGUIContext->FillRectangle( &hDstRect, pColorBrush );
        }

        // Icon
        Float fSavedTop = hDstRect.fTop;

        hDstRect.fTop += ( (iItemHeight - GUISKIN_VSPACING_ICON) >> 1 );
        hDstRect.fRight = hDstRect.fLeft + (Float)GUISKIN_HSPACING_ICON;
        hDstRect.fBottom = hDstRect.fTop + (Float)GUISKIN_VSPACING_ICON;
        
        hSrcRect = pSkin->GetLocation( pMenuItem->GetIcon() );
        pGUIContext->DrawBitmap( pSkinBitmap, &hDstRect, &hSrcRect );

        hDstRect.fTop = fSavedTop;

        // Text
        hDstRect.fLeft = (Float)( rectClient.TopLeft.X + (GUISKIN_MARGINS << 1) + GUISKIN_HSPACING_ICON );
        hDstRect.fRight = (Float)( rectClient.DownRight.X - GUISKIN_MARGINS );
        hDstRect.fBottom = hDstRect.fTop + (Float)iItemHeight;

        UInt iNameLength = 0;
        const GChar * strName = pMenuItem->GetName( &iNameLength );

        pTextBrush->SetColor( (pMenuItem->IsActive()) ? &hActiveText : &hInactiveText );
        pGUIContext->DrawText( strName, iNameLength, pTextFormat, &hDstRect, pTextBrush, GPU2DTEXT_DRAWOPTION_CLIP );

        // SubMenu Arrow
        if ( pMenuItem->HasSubMenu() ) {
            hDstRect.fLeft = (Float)( rectClient.DownRight.X - GUISKIN_MARGINS - GUISKIN_SIZE_ICON_TINY );
            hDstRect.fTop += ( (iItemHeight - GUISKIN_SIZE_ICON_TINY) >> 1 );
            hDstRect.fRight = hDstRect.fLeft + (Float)GUISKIN_SIZE_ICON_TINY;
            hDstRect.fBottom = hDstRect.fTop + (Float)GUISKIN_SIZE_ICON_TINY;

            hSrcRect = locArrow;
            pGUIContext->DrawBitmap( pSkinBitmap, &hDstRect, &hSrcRect );

            hDstRect.fTop = fSavedTop;
        }

        // Next row
        hDstRect.fTop += iItemHeight;
    }
}

/////////////////////////////////////////////////////////////////////////////////

const Rectangle2 & GUIMenu::_ComputePlacement( const GUIMenuModel * pModel, const Point2 & ptScreenPos )
{
    static Rectangle2 s_rectResult;

    // Get screen dimensions
    UInt iScreenWidth, iScreenHeight;
    GUIFn->GetScreenSize( &iScreenWidth, &iScreenHeight );

    // Get model dimensions
    UInt iWidth = GUISKIN_SIZE_ICON_SMALL + GUIMENU_TEXTWIDTH + GUISKIN_SIZE_ICON_TINY + ( GUISKIN_MARGINS << 2 );
    UInt iHeight = pModel->GetItemCount() * pModel->GetItemHeight();

    s_rectResult.TopLeft = ptScreenPos;

    // Right/Left placement
    if ( s_rectResult.TopLeft.X + iWidth > iScreenWidth ) {
        s_rectResult.TopLeft.X -= iWidth;
        Assert( s_rectResult.TopLeft.X >= 0 );
    }

    // Down/Up placement
    if ( s_rectResult.TopLeft.Y + iHeight > iScreenHeight ) {
        s_rectResult.TopLeft.Y -= iHeight;
        Assert( s_rectResult.TopLeft.Y >= 0 );
    }

    s_rectResult.DownRight = s_rectResult.TopLeft;
    s_rectResult.DownRight.X += ( iWidth - 1 );
    s_rectResult.DownRight.Y += ( iHeight - 1 );

    return s_rectResult;
}

