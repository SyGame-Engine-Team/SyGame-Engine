/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Widgets/Extended/GUIList.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Widget : Extended controls, List view
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
#include "GUIList.h"

#include "../../GUIManager.h"

/////////////////////////////////////////////////////////////////////////////////
// GUIListNode implementation
GUIListNode::GUIListNode()
{
    m_bSelected = false;

    m_iNameLength = 0;
    m_strName[0] = NULLBYTE;
    m_idIcon = GUISKIN_IMAGE_ICON_UNDEFINED;
}
GUIListNode::GUIListNode( const GChar * strName, GUISkinImage idIcon )
{
    m_bSelected = false;

    GChar * pEnd = StringFn->NCopy( m_strName, strName, GUI_NAME_LENGTH - 1 );
    m_iNameLength = ( pEnd - m_strName );
    m_idIcon = idIcon;
}
GUIListNode::~GUIListNode()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// GUIListModel implementation
GUIListModel::GUIListModel( UInt iNodeHeight, UInt iNodeCount, const GChar * arrTexts[], GUISkinImage arrIcons[] ):
    GUIWidgetModel(), m_arrNodes()
{
    m_iDisplayTop = 0;
    m_iDisplayCount = 0;

    m_iNodeHeight = iNodeHeight;
    if ( m_iNodeHeight < GUISKIN_VSPACING_ICON )
        m_iNodeHeight = GUISKIN_VSPACING_ICON;

    m_iLastSelected = INVALID_OFFSET;

    // Item list
    m_arrNodes.UseMemoryContext( GUIFn->GetMemoryContext(), TEXT("LayoutData") );
    m_arrNodes.Create();
    for( UInt i = 0; i < iNodeCount; ++i )
        Push( arrTexts[i], arrIcons[i] );
}
GUIListModel::~GUIListModel()
{
    // Item list
    m_arrNodes.Destroy();
}

Void GUIListModel::Push( const GChar * strName, GUISkinImage idIcon )
{
    // Push
    GUIListNode newNode( strName, idIcon );
    m_arrNodes.Push( newNode );

    // Raise callback
    OnInsert( m_arrNodes.Count() - 1 );
}
Void GUIListModel::Unshift( const GChar * strName, GUISkinImage idIcon )
{
    // Unshift
    GUIListNode newNode( strName, idIcon );
    m_arrNodes.Unshift( newNode );

    // Raise callback
    OnInsert( 0 );
}

Void GUIListModel::Pop()
{
    // Raise callback
    OnRemove( m_arrNodes.Count() - 1 );

    // Pop
    m_arrNodes.Pop( NULL, 1 );
}
Void GUIListModel::Shift()
{
    // Raise callback
    OnRemove( 0 );

    // Shift
    m_arrNodes.Shift( NULL, 1 );
}

Void GUIListModel::Insert( UInt iNode, const GChar * strName, GUISkinImage idIcon )
{
    // Insert
    GUIListNode newNode( strName, idIcon );
    m_arrNodes.Insert( iNode, newNode );

    // Raise callback
    OnInsert( iNode );
}
Void GUIListModel::Replace( UInt iNode, const GChar * strName, GUISkinImage idIcon )
{
    // Raise callback
    OnRemove( iNode );

    // Replace
    StringFn->NCopy( m_arrNodes[iNode].m_strName, strName, GUI_NAME_LENGTH - 1 );
    m_arrNodes[iNode].m_idIcon = idIcon;

    // Raise callback
    OnInsert( iNode );
}
Void GUIListModel::Remove( UInt iNode )
{
    // Raise callback
    OnRemove( iNode );

    // Remove
    m_arrNodes.Remove( iNode, NULL, 1 );
}

Void GUIListModel::SetScroll( UInt iScrollPos )
{
    // Scroll to
    UInt iMaxTop = ( m_arrNodes.Count() - m_iDisplayCount );
    if ( iScrollPos > iMaxTop )
        m_iDisplayTop = iMaxTop;
    else
        m_iDisplayTop = iScrollPos;
}
Void GUIListModel::ScrollUp( UInt iAmount )
{
    // Scroll up
    if ( m_iDisplayTop <= iAmount )
        m_iDisplayTop = 0;
    else
        m_iDisplayTop -= iAmount;
}
Void GUIListModel::ScrollDown( UInt iAmount )
{
    // Scroll down
    UInt iMaxTop = ( m_arrNodes.Count() - m_iDisplayCount );
    if ( m_iDisplayTop + iAmount > iMaxTop )
        m_iDisplayTop = iMaxTop;
    else
        m_iDisplayTop += iAmount;
}

Void GUIListModel::SelectAll()
{
    // Select all
    for( UInt i = 0; i < m_arrNodes.Count(); ++i )
		m_arrNodes[i].m_bSelected = true;
    m_iLastSelected = m_arrNodes.Count() - 1;

    // Raise callback
    OnSelectionChange();
}
Void GUIListModel::UnselectAll()
{
    // Unselect all
    for( UInt i = 0; i < m_arrNodes.Count(); ++i )
		m_arrNodes[i].m_bSelected = false;
    m_iLastSelected = INVALID_OFFSET;

    // Raise callback
    OnSelectionChange();
}
Void GUIListModel::Select( UInt iItem, Bool bSelect )
{
    Assert( iItem < m_arrNodes.Count() );
    if ( m_arrNodes[iItem].m_bSelected == bSelect )
        return;

    // Select
	m_arrNodes[iItem].m_bSelected = bSelect;
    if ( m_arrNodes[iItem].m_bSelected )
        m_iLastSelected = iItem;
    else
        m_iLastSelected = INVALID_OFFSET;

    // Raise callback
    OnSelectionChange();
}
Void GUIListModel::ToggleSelect( UInt iItem )
{
    Assert( iItem < m_arrNodes.Count() );

    // Toggle selection
	m_arrNodes[iItem].m_bSelected = !( m_arrNodes[iItem].m_bSelected );
    if ( m_arrNodes[iItem].m_bSelected )
        m_iLastSelected = iItem;
    else
        m_iLastSelected = INVALID_OFFSET;

    // Raise callback
    OnSelectionChange();
}

Void GUIListModel::OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags )
{
    // Handle context menu
    if ( iKey == KEYCODE_MOUSERIGHT ) {
        GUIWidgetModel::OnClick( ptLocalPos, iKey, iFlags );
        return;
    }

    if ( iKey != KEYCODE_MOUSELEFT )
        return;

    // Flags
    Bool bCapsLock = ( (iFlags & GUIEVENT_FLAG_CAPSLOCK) != 0 );
    Bool bShift    = ( (iFlags & GUIEVENT_FLAG_SHIFT) != 0 );
    Bool bCtrl     = ( (iFlags & GUIEVENT_FLAG_CTRL) != 0 );

    // Pick Node
    UInt iNode = m_iDisplayTop + ( ptLocalPos.Y / m_iNodeHeight );

    // Deal event
    if ( iNode >= m_arrNodes.Count() )
        UnselectAll();
    else if ( bShift != bCapsLock ) {
        if ( m_iLastSelected == INVALID_OFFSET ) {
            ToggleSelect( iNode );
        } else {
            Int iInc = ( m_iLastSelected <= iNode ) ? +1 : -1;
            for( Int i = (Int)m_iLastSelected; i != (Int)iNode; i += iInc )
                Select( (UInt)i, true );
            Select( iNode, true );
        }
    } else if ( bCtrl ) {
        ToggleSelect( iNode );
    } else {
        UnselectAll();
        Select( iNode, true );
    }
}

Void GUIListModel::OnScroll( const Point2 & /*ptLocalPos*/, Int iWheelDelta, GUIEventFlag iFlags )
{
    if ( m_iDisplayCount >= m_arrNodes.Count() ) {
        if ( m_iDisplayTop != 0 )
            SetScroll( 0 );
        return;
    }

    // Flags
    Bool bCtrl = ( (iFlags & GUIEVENT_FLAG_CTRL) != 0 );

    // Deal event
    if ( iWheelDelta > 0 )
        ScrollUp( bCtrl ? 3 : 1 );
    else if ( iWheelDelta < 0 )
        ScrollDown( bCtrl ? 3 : 1 );
}

Void GUIListModel::OnResize()
{
    // Widget
    GUIList * pWidget = (GUIList*)_GetWidget();
    UInt iClientHeight = pWidget->GetClientRect().Height();

    // Deal event
    m_iDisplayCount = ( iClientHeight / m_iNodeHeight );
}

Void GUIListModel::OnInsert( UInt /*iItem*/ )
{
    // stub method, do nothing
}
Void GUIListModel::OnRemove( UInt /*iItem*/ )
{
    // stub method, do nothing
}
Void GUIListModel::OnSelectionChange()
{
    // stub method, do nothing
}

/////////////////////////////////////////////////////////////////////////////////
// GUIList implementation
GUIList::GUIList( GUIListModel * pModel, GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName ):
    GUIWidget( pModel, pParent, guiLayout, strName )
{
    pModel->OnResize();
}
GUIList::~GUIList()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

Void GUIList::_Draw( const Rectangle2 & rectClient )
{
    GPU2DContext * pGUIContext = GUIFn->GetScreenContext();

    // Model
    GUIListModel * pModel = (GUIListModel*)_GetWidgetModel();

    UInt iNodeHeight = pModel->GetNodeHeight();
    UInt iNodeCount = pModel->GetNodeCount();

    UInt iDisplayTop = pModel->GetDisplayTop();
    UInt iDisplayCount = pModel->GetDisplayCount();

    // Skin
    const GUISkin * pSkin = GUIFn->GetSkin();

    GPU2DBitmap * pSkinBitmap = pSkin->GetSkinBitmap();
    const Rectangle2 & locBackground = pSkin->GetLocation( GUISKIN_IMAGE_LIST_BACKGROUND );
    const Rectangle2 & locArrowTop = pSkin->GetLocation( GUISKIN_IMAGE_LIST_ARROWTOP );
    const Rectangle2 & locArrowBottom = pSkin->GetLocation( GUISKIN_IMAGE_LIST_ARROWBOTTOM );

    GPU2DSolidColorBrush * pColorBrush = pSkin->GetSkinColorBrush();
    GPU2DColor hTextColor = pSkin->GetColor( GUISKIN_COLOR_LIST_TEXT );
    GPU2DColor hSelectedTextColor = pSkin->GetColor( GUISKIN_COLOR_LIST_TEXTSELECTED );
    GPU2DColor hSelectionColor = pSkin->GetColor( GUISKIN_COLOR_LIST_SELECTION );

    GPU2DSolidColorBrush * pTextBrush = pSkin->GetSkinTextBrush();
    GPU2DTextFormat * pTextFormat = pSkin->GetSkinTextFormat( GUISKIN_FONT_BUTTON );

    // Background
    GPU2DRectangle hDstRect = rectClient;
    GPU2DRectangle hSrcRect = locBackground;

    pGUIContext->DrawBitmap( pSkinBitmap, &hDstRect, &hSrcRect );

    // List rows
    // Margin | (Icon) | Margin | Text | Margin
    if ( pModel->GetNodeCount() == 0 )
        return;

    GUIListNode * pCurNode;
    hDstRect.fTop = (Float)( rectClient.TopLeft.Y );

    pTextFormat->SetTextAlignment( GPU2DTEXT_ALIGNMENT_LEADING );
    pTextFormat->SetParagraphAlignment( GPU2DTEXT_PARAGRAPHALIGNMENT_CENTER );

    for( UInt iNode = 0; iNode < iDisplayCount; ++iNode ) {
        pCurNode = pModel->GetNode( iDisplayTop + iNode );

        hDstRect.fLeft = (Float)( rectClient.TopLeft.X + GUISKIN_MARGINS );

        // Selection
        if ( pCurNode->IsSelected() ) {
            hDstRect.fRight = hDstRect.fLeft + ( rectClient.Width() - (GUISKIN_MARGINS << 1) );
            hDstRect.fBottom = hDstRect.fTop + iNodeHeight;

            pColorBrush->SetColor( &hSelectionColor );
            pGUIContext->FillRectangle( &hDstRect, pColorBrush );
        }

        // Icon
        hDstRect.fRight = hDstRect.fLeft + GUISKIN_HSPACING_ICON;
        hDstRect.fBottom = hDstRect.fTop + GUISKIN_VSPACING_ICON;

        hSrcRect = pSkin->GetLocation( pCurNode->GetIcon() );
        pGUIContext->DrawBitmap( pSkinBitmap, &hDstRect, &hSrcRect );

        hDstRect.fLeft += ( GUISKIN_HSPACING_ICON + GUISKIN_MARGINS );

        // Text
        UInt iLength = 0;
        const GChar * strName = pCurNode->GetName( &iLength );

        hDstRect.fRight = (Float)( rectClient.DownRight.X );
        hDstRect.fBottom = hDstRect.fTop + (Float)iNodeHeight;

        pTextBrush->SetColor( (pCurNode->IsSelected()) ? &hSelectedTextColor : &hTextColor );
        pGUIContext->DrawText( strName, iLength, pTextFormat, &hDstRect, pTextBrush, GPU2DTEXT_DRAWOPTION_CLIP );

        // Next row
        hDstRect.fTop += iNodeHeight;
    }

    UInt iRemainingHeight = ( rectClient.Height() - (iNodeHeight * iDisplayCount) );
    if ( (iRemainingHeight > 0) && ((iDisplayTop + iDisplayCount) < iNodeCount) ) {
        pCurNode = pModel->GetNode( iDisplayTop + iDisplayCount );

        hDstRect.fLeft = (Float)( rectClient.TopLeft.X + GUISKIN_MARGINS );

        // Selection
        if ( pCurNode->IsSelected() ) {
            hDstRect.fRight = hDstRect.fLeft + ( rectClient.Width() - (GUISKIN_MARGINS << 1) );
            hDstRect.fBottom = hDstRect.fTop + iRemainingHeight;

            pColorBrush->SetColor( &hSelectionColor );
            pGUIContext->FillRectangle( &hDstRect, pColorBrush );
        }
    }

    // Scroll arrows
    hDstRect.fLeft = (Float)( rectClient.DownRight.X - GUISKIN_MARGINS - GUISKIN_SIZE_ICON_TINY );

	if ( iDisplayTop > 0 ) {
        hDstRect.fTop = (Float)( rectClient.TopLeft.Y + ((iNodeHeight - GUISKIN_SIZE_ICON_TINY) >> 1) );
        hDstRect.fRight = hDstRect.fLeft + GUISKIN_SIZE_ICON_TINY;
        hDstRect.fBottom = hDstRect.fTop + GUISKIN_SIZE_ICON_TINY;

        hSrcRect = locArrowTop;
        pGUIContext->DrawBitmap( pSkinBitmap, &hDstRect, &hSrcRect );
	}
	if ( (iDisplayTop + iDisplayCount) < iNodeCount ) {
		hDstRect.fTop = (Float)( rectClient.DownRight.Y + 1 - ((iNodeHeight + GUISKIN_SIZE_ICON_TINY) >> 1) );
        hDstRect.fRight = hDstRect.fLeft + GUISKIN_SIZE_ICON_TINY;
        hDstRect.fBottom = hDstRect.fTop + GUISKIN_SIZE_ICON_TINY;

        hSrcRect = locArrowBottom;
        pGUIContext->DrawBitmap( pSkinBitmap, &hDstRect, &hSrcRect );
	}
}

