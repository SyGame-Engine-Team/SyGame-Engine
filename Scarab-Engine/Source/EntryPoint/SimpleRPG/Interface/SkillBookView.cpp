/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Interface/SkillBookView.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Interface : SkillBookView
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
#include "SkillBookView.h"

#include "InterfaceManager.h"

/////////////////////////////////////////////////////////////////////////////////
// SkillBookViewModel implementation
SkillBookViewModel::SkillBookViewModel( BaseCharacter * pCharacter ):
    GUIWidgetModel()
{
    m_pCharacter = pCharacter;

    m_iPageSize = 8;
    m_iPageCount = 1;
    m_iCurrentPage = 0;

    for( UInt i = 0; i < HUD_SKILLBOOKVIEW_MAX_PAGES; ++i ) {
        m_arrSkillPages[i].iSkillCount = 0;
        for( UInt j = 0; j < HUD_SKILLBOOKVIEW_MAX_PAGESIZE; ++j )
            m_arrSkillPages[i].arrSkills[j] = NULL;
    }

    UpdateSkillPages();

    // Context Menu
    /////////////////////////////////////////////
}
SkillBookViewModel::~SkillBookViewModel()
{
    // nothing to do
}

Void SkillBookViewModel::UpdateSkillPages()
{
    m_iPageCount = 1;
    m_iCurrentPage = 0;
    m_arrSkillPages[m_iCurrentPage].iSkillCount = 0;

    m_pCharacter->EnumSkills();
    SkillInstance * pSkill = m_pCharacter->EnumNextSkill();
    while( pSkill != NULL ) {
        UInt iIndex = m_arrSkillPages[m_iCurrentPage].iSkillCount;
        if ( iIndex >= m_iPageSize ) {
            Assert( m_iPageCount < HUD_SKILLBOOKVIEW_MAX_PAGES );
            ++m_iPageCount;
            ++m_iCurrentPage;
            m_arrSkillPages[m_iCurrentPage].iSkillCount = 0;
            iIndex = 0;
        }
        m_arrSkillPages[m_iCurrentPage].arrSkills[iIndex] = pSkill;
        ++(m_arrSkillPages[m_iCurrentPage].iSkillCount);

        pSkill = m_pCharacter->EnumNextSkill();
    }

    m_iCurrentPage = 0;
}

Void SkillBookViewModel::OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags )
{
    // Next/Prev Page case
    if ( iKey == KEYCODE_MOUSELEFT ) {
        if ( m_iCurrentPage < (m_iPageCount - 1) )
            ++m_iCurrentPage;
    } else if ( iKey == KEYCODE_MOUSERIGHT ) {
        if ( m_iCurrentPage > 0 )
            --m_iCurrentPage;
    }

    // Default handler
    GUIWidgetModel::OnClick( ptLocalPos, iKey, iFlags );
}

Void SkillBookViewModel::OnStartDrag( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags )
{
    if ( iKey == KEYCODE_MOUSELEFT ) {
        UInt iSlot = _PickSlot( ptLocalPos );

        if ( iSlot != INVALID_OFFSET ) {
            SkillID idSkill = m_arrSkillPages[m_iCurrentPage].arrSkills[iSlot]->GetID();

            HUDDragNDropData * pDragNDropData = InterfaceFn->GetDragNDropData();
            pDragNDropData->iType = HUD_DRAGTYPE_SKILL;
            pDragNDropData->iIdentifier = idSkill;

            GUIFn->SetDragData( GUIDRAG_LEFT, pDragNDropData );
        }

        return;
    }

    // Default handler
    GUIWidgetModel::OnStartDrag( ptLocalPos, iKey, iFlags );
}
Void SkillBookViewModel::OnStopDrag( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags )
{
    if ( iKey == KEYCODE_MOUSELEFT ) {
        // nothing to do

        GUIFn->SetDragData( GUIDRAG_LEFT, NULL );

        return;
    }

    // Default handler
    GUIWidgetModel::OnStopDrag( ptLocalPos, iKey, iFlags );
}

/////////////////////////////////////////////////////////////////////////////////

UInt SkillBookViewModel::_PickSlot( const Point2 & ptLocalPos ) const
{
    if ( ptLocalPos.X < 4 || ptLocalPos.Y < 4 )
        return INVALID_OFFSET;

    SkillBookView * pSkillBookView = (SkillBookView*)( _GetWidget() );
    UInt iWidth = pSkillBookView->GetClientRect().Width();
    UInt iHeight = pSkillBookView->GetClientRect().Height();
    UInt iPageFooterHeight = 20;
    //UInt iPageHeight = ( iHeight - iPageFooterHeight - 8 );

    if ( ptLocalPos.X >= (Int)(iWidth - 4) || ptLocalPos.Y >= (Int)(iHeight - iPageFooterHeight - 4) )
        return INVALID_OFFSET;

    UInt iPageSkillsLeftColumnWidth = ( (iWidth - 12) >> 1 );
    UInt iPageSkillsRightColumnWidth = ( (iWidth - 12) - iPageSkillsLeftColumnWidth );

    UInt iSkillRowCount = ( m_iPageSize >> 1 );

    // Left page case
    if ( (ptLocalPos.X - 4) < (Int)iPageSkillsLeftColumnWidth ) {
        UInt iSlot = 0;
        UInt iRow = ( (ptLocalPos.Y - 4) / INTERFACE_HUD_ICON_SIZE );
        iSlot += iRow;

        return iSlot;
    }

    if ( (ptLocalPos.X - 4 - iPageSkillsLeftColumnWidth) < 4 )
        return INVALID_OFFSET;
    
    // Right page case
    Assert( (ptLocalPos.X - 8 - iPageSkillsLeftColumnWidth) < iPageSkillsRightColumnWidth );
    UInt iSlot = iSkillRowCount;
    UInt iRow = ( (ptLocalPos.Y - 4) / INTERFACE_HUD_ICON_SIZE );
    iSlot += iRow;

    return iSlot;
}

/////////////////////////////////////////////////////////////////////////////////
// SkillBookView implementation
SkillBookView::SkillBookView( SkillBookViewModel * pModel, GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName ):
    GUIWidget( pModel, pParent, guiLayout, strName )
{
    // nothing to do
}
SkillBookView::~SkillBookView()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

Void SkillBookView::_Draw( const Rectangle2 & rectClient )
{
    GPU2DContext * pHUDContext = InterfaceFn->GetHUDContext();

    // Model
    SkillBookViewModel * pModel = (SkillBookViewModel*)_GetWidgetModel();

    UInt iPageCount = pModel->GetPageCount();
    UInt iPage = pModel->GetCurrentPage();

    UInt iPageFooterHeight = 20;
    //UInt iPageHeight = ( rectClient.Height() - iPageFooterHeight - 8 );

    UInt iPageSkillsLeftColumnWidth = ( (rectClient.Width() - 12) >> 1 );
    UInt iPageSkillsRightColumnWidth = ( (rectClient.Width() - 12) - iPageSkillsLeftColumnWidth );

    UInt iSkillRowCount = ( pModel->GetPageSize() >> 1 );

    // Skin
    GPU2DBitmap * pHUDBitmap = InterfaceFn->GetHUDSkinBitmap();
    const Rectangle2 & locBackground = InterfaceFn->GetHUDLocation( HUD_IMAGE_BACKGROUND_SKILLBOOKVIEW );

    GPU2DColor hSkillBookColor = InterfaceFn->GetHUDColor( HUD_COLOR_SKILLBOOK_TEXT );

    GPU2DSolidColorBrush * pTextBrush = InterfaceFn->GetHUDTextBrush();
    GPU2DTextFormat * pTextFormat = InterfaceFn->GetHUDTextFormat( HUD_FONT_SELFBAR_TEXT );

    // Background
    GPU2DRectangle hDstRect = rectClient;
    GPU2DRectangle hSrcRect = locBackground;

    pHUDContext->DrawBitmap( pHUDBitmap, &hDstRect, &hSrcRect );

    // Page skills
    GPU2DRectangle hRectLeftColumn;
    hRectLeftColumn.fLeft = (Scalar)( rectClient.TopLeft.X + 4 );
    hRectLeftColumn.fTop = (Scalar)( rectClient.TopLeft.Y + 4 );
    hRectLeftColumn.fRight = hRectLeftColumn.fLeft + (Scalar)iPageSkillsLeftColumnWidth;
    hRectLeftColumn.fBottom = hRectLeftColumn.fTop + (Scalar)INTERFACE_HUD_ICON_SIZE;

    GPU2DRectangle hRectRightColumn;
    hRectRightColumn.fLeft = (Scalar)( rectClient.TopLeft.X + iPageSkillsLeftColumnWidth + 8 );
    hRectRightColumn.fTop = (Scalar)( rectClient.TopLeft.Y + 4 );
    hRectRightColumn.fRight = hRectLeftColumn.fLeft + (Scalar)iPageSkillsRightColumnWidth;
    hRectRightColumn.fBottom = hRectLeftColumn.fTop + (Scalar)INTERFACE_HUD_ICON_SIZE;

    GPU2DRectangle hRectLeftIcon = hRectLeftColumn;
    hRectLeftIcon.fRight = hRectLeftIcon.fLeft + (Scalar)INTERFACE_HUD_ICON_SIZE;

    GPU2DRectangle hRectLeftText = hRectLeftColumn;
    hRectLeftText.fLeft = hRectLeftIcon.fRight;

    GPU2DRectangle hRectRightIcon = hRectRightColumn;
    hRectRightIcon.fRight = hRectRightIcon.fLeft + (Scalar)INTERFACE_HUD_ICON_SIZE;

    GPU2DRectangle hRectRightText = hRectRightColumn;
    hRectRightText.fLeft = hRectRightIcon.fRight;

    pTextFormat->SetTextAlignment( GPU2DTEXT_ALIGNMENT_LEADING );
    pTextFormat->SetParagraphAlignment( GPU2DTEXT_PARAGRAPHALIGNMENT_CENTER );
    pTextBrush->SetColor( &hSkillBookColor );

    GChar strLine[64];
    GChar * pStrLine;

    for( UInt i = 0; i < iSkillRowCount; ++i ) {
        // Left Column
        SkillInstance * pSkill = pModel->GetSkill( iPage, i );
        if ( pSkill == NULL )
            continue;
        
            // Icon
        const Rectangle2 & locSkillIconL = InterfaceFn->GetHUDLocation( pSkill->GetID() );

        hSrcRect = locSkillIconL;
        pHUDContext->DrawBitmap( pHUDBitmap, &hRectLeftIcon, &hSrcRect );

            // Text
        pStrLine = StringFn->Format( strLine, TEXT("%s (Rank %d/%d)"), pSkill->GetName(), pSkill->GetRank(), pSkill->GetMaxRank() );

        pHUDContext->DrawText( strLine, (pStrLine - strLine), pTextFormat, &hRectLeftText, pTextBrush, GPU2DTEXT_DRAWOPTION_CLIP );

            // Next row
        hRectLeftIcon.fTop += (Scalar)INTERFACE_HUD_ICON_SIZE;
        hRectLeftIcon.fBottom += (Scalar)INTERFACE_HUD_ICON_SIZE;
        hRectLeftText.fTop += (Scalar)INTERFACE_HUD_ICON_SIZE;
        hRectLeftText.fBottom += (Scalar)INTERFACE_HUD_ICON_SIZE;
        
        // Right Column
        pSkill = pModel->GetSkill( iPage, iSkillRowCount + i );
        if ( pSkill == NULL )
            continue;

            // Icon
        const Rectangle2 & locSkillIconR = InterfaceFn->GetHUDLocation( pSkill->GetID() );

        hSrcRect = locSkillIconR;
        pHUDContext->DrawBitmap( pHUDBitmap, &hRectRightIcon, &hSrcRect );

            // Text
        pStrLine = StringFn->Format( strLine, TEXT("%s (Rank %d)"), pSkill->GetName(), pSkill->GetRank() );

        pHUDContext->DrawText( strLine, (pStrLine - strLine), pTextFormat, &hRectRightText, pTextBrush, GPU2DTEXT_DRAWOPTION_CLIP );

            // Next row
        hRectRightIcon.fTop += (Scalar)INTERFACE_HUD_ICON_SIZE;
        hRectRightIcon.fBottom += (Scalar)INTERFACE_HUD_ICON_SIZE;
        hRectRightText.fTop += (Scalar)INTERFACE_HUD_ICON_SIZE;
        hRectRightText.fBottom += (Scalar)INTERFACE_HUD_ICON_SIZE;
    }

    // Page index / count
    pStrLine = StringFn->Format( strLine, TEXT("Page %d / %d"), (iPage+1), iPageCount );

    hDstRect = rectClient;
    hDstRect.fTop = hDstRect.fBottom - (Scalar)iPageFooterHeight;

    pTextFormat->SetTextAlignment( GPU2DTEXT_ALIGNMENT_CENTER );
    pTextFormat->SetParagraphAlignment( GPU2DTEXT_PARAGRAPHALIGNMENT_CENTER );

    pTextBrush->SetColor( &hSkillBookColor );
    pHUDContext->DrawText( strLine, (pStrLine - strLine), pTextFormat, &hDstRect, pTextBrush, GPU2DTEXT_DRAWOPTION_CLIP );
}

