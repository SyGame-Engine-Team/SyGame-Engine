/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Interface/CharacterView.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Interface : CharacterView
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
#include "CharacterView.h"

#include "InterfaceManager.h"

/////////////////////////////////////////////////////////////////////////////////
// CharacterViewModel implementation
CharacterViewModel::CharacterViewModel( BaseCharacter * pCharacter ):
    GUIWidgetModel()
{
    m_pCharacter = pCharacter;

    // Context Menu
    /////////////////////////////////////////////
}
CharacterViewModel::~CharacterViewModel()
{
    // nothing to do
}

Void CharacterViewModel::OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags )
{
    // nothing to do

    // Default handler
    GUIWidgetModel::OnClick( ptLocalPos, iKey, iFlags );
}

/////////////////////////////////////////////////////////////////////////////////
// CharacterView implementation
CharacterView::CharacterView( CharacterViewModel * pModel, GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName ):
    GUIWidget( pModel, pParent, guiLayout, strName )
{
    // nothing to do
}
CharacterView::~CharacterView()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

Void CharacterView::_Draw( const Rectangle2 & rectClient )
{
    GPU2DContext * pHUDContext = InterfaceFn->GetHUDContext();

    // Model
    CharacterViewModel * pModel = (CharacterViewModel*)_GetWidgetModel();

    // Skin
    GPU2DBitmap * pHUDBitmap = InterfaceFn->GetHUDSkinBitmap();
    const Rectangle2 & locBackground = InterfaceFn->GetHUDLocation( HUD_IMAGE_BACKGROUND_CHARACTERVIEW );

    GPU2DColor hTitleColor = InterfaceFn->GetHUDColor( HUD_COLOR_CHARACTERVIEW_TITLE );
    GPU2DColor hValueColor = InterfaceFn->GetHUDColor( HUD_COLOR_CHARACTERVIEW_VALUE );

    GPU2DSolidColorBrush * pTextBrush = InterfaceFn->GetHUDTextBrush();
    GPU2DTextFormat * pTextFormat = InterfaceFn->GetHUDTextFormat( HUD_FONT_SELFBAR_TEXT );

    Scalar fLineHeight = pTextFormat->GetLineHeight( false );
    fLineHeight = MathFn->Ceilf(fLineHeight) + 4.0f;

    // Background
    GPU2DRectangle hDstRect = rectClient;
    GPU2DRectangle hSrcRect = locBackground;

    pHUDContext->DrawBitmap( pHUDBitmap, &hDstRect, &hSrcRect );

    // Header line = Name, Class, Level
    hDstRect = rectClient;
    hDstRect.fBottom = hDstRect.fTop + fLineHeight;

    GChar strLine[64];

    GChar * pStrLine = strLine;
    pStrLine = StringFn->Format( pStrLine, TEXT("%s (%s %d)"), pModel->GetName(), CharStatPackage::GetClassName(pModel->GetClass()), pModel->GetLevel() );

    pTextFormat->SetTextAlignment( GPU2DTEXT_ALIGNMENT_CENTER );
    pTextFormat->SetParagraphAlignment( GPU2DTEXT_PARAGRAPHALIGNMENT_CENTER );

    pTextBrush->SetColor( &hTitleColor );
    pHUDContext->DrawText( strLine, (pStrLine - strLine), pTextFormat, &hDstRect, pTextBrush, GPU2DTEXT_DRAWOPTION_CLIP );

    // First line setup
    GPU2DRectangle hRectTitle = rectClient;
    hRectTitle.fBottom = hRectTitle.fTop + fLineHeight;
    hRectTitle.fRight -= (Scalar)( rectClient.Width() >> 1 );
    hRectTitle.fLeft += 4.0f;

    GPU2DRectangle hRectValue = rectClient;
    hRectValue.fBottom = hRectValue.fTop + fLineHeight;
    hRectValue.fLeft = hRectTitle.fRight;
    hRectValue.fRight -= 4.0f;

    hRectTitle.fTop += fLineHeight;
    hRectTitle.fBottom += fLineHeight;
    hRectValue.fTop += fLineHeight;
    hRectValue.fBottom += fLineHeight;

    // Experience
    pStrLine = strLine;
    pStrLine = StringFn->Format( pStrLine, TEXT("Experience :") );

    pTextFormat->SetTextAlignment( GPU2DTEXT_ALIGNMENT_TRAILING );
    pTextFormat->SetParagraphAlignment( GPU2DTEXT_PARAGRAPHALIGNMENT_CENTER );

    pTextBrush->SetColor( &hTitleColor );
    pHUDContext->DrawText( strLine, (pStrLine - strLine), pTextFormat, &hRectTitle, pTextBrush, GPU2DTEXT_DRAWOPTION_CLIP );

    pStrLine = strLine;
    pStrLine = StringFn->Format( pStrLine, TEXT("%d / %d"), pModel->GetCurrentXP(), pModel->GetMaxXP() );

    pTextFormat->SetTextAlignment( GPU2DTEXT_ALIGNMENT_LEADING );
    pTextFormat->SetParagraphAlignment( GPU2DTEXT_PARAGRAPHALIGNMENT_CENTER );

    pTextBrush->SetColor( &hValueColor );
    pHUDContext->DrawText( strLine, (pStrLine - strLine), pTextFormat, &hRectValue, pTextBrush, GPU2DTEXT_DRAWOPTION_CLIP );

    hRectTitle.fTop += fLineHeight;
    hRectTitle.fBottom += fLineHeight;
    hRectValue.fTop += fLineHeight;
    hRectValue.fBottom += fLineHeight;

    // HP
    pStrLine = strLine;
    pStrLine = StringFn->Format( pStrLine, TEXT("Hit Points :") );

    pTextFormat->SetTextAlignment( GPU2DTEXT_ALIGNMENT_TRAILING );
    pTextFormat->SetParagraphAlignment( GPU2DTEXT_PARAGRAPHALIGNMENT_CENTER );

    pTextBrush->SetColor( &hTitleColor );
    pHUDContext->DrawText( strLine, (pStrLine - strLine), pTextFormat, &hRectTitle, pTextBrush, GPU2DTEXT_DRAWOPTION_CLIP );

    pStrLine = strLine;
    pStrLine = StringFn->Format( pStrLine, TEXT("%d / %d"), pModel->GetCurrentHP(), pModel->GetMaxHP() );

    pTextFormat->SetTextAlignment( GPU2DTEXT_ALIGNMENT_LEADING );
    pTextFormat->SetParagraphAlignment( GPU2DTEXT_PARAGRAPHALIGNMENT_CENTER );

    pTextBrush->SetColor( &hValueColor );
    pHUDContext->DrawText( strLine, (pStrLine - strLine), pTextFormat, &hRectValue, pTextBrush, GPU2DTEXT_DRAWOPTION_CLIP );

    hRectTitle.fTop += fLineHeight;
    hRectTitle.fBottom += fLineHeight;
    hRectValue.fTop += fLineHeight;
    hRectValue.fBottom += fLineHeight;

    // MP
    pStrLine = strLine;
    pStrLine = StringFn->Format( pStrLine, TEXT("Mana Points :") );

    pTextFormat->SetTextAlignment( GPU2DTEXT_ALIGNMENT_TRAILING );
    pTextFormat->SetParagraphAlignment( GPU2DTEXT_PARAGRAPHALIGNMENT_CENTER );

    pTextBrush->SetColor( &hTitleColor );
    pHUDContext->DrawText( strLine, (pStrLine - strLine), pTextFormat, &hRectTitle, pTextBrush, GPU2DTEXT_DRAWOPTION_CLIP );

    pStrLine = strLine;
    pStrLine = StringFn->Format( pStrLine, TEXT("%d / %d"), pModel->GetCurrentMP(), pModel->GetMaxMP() );

    pTextFormat->SetTextAlignment( GPU2DTEXT_ALIGNMENT_LEADING );
    pTextFormat->SetParagraphAlignment( GPU2DTEXT_PARAGRAPHALIGNMENT_CENTER );

    pTextBrush->SetColor( &hValueColor );
    pHUDContext->DrawText( strLine, (pStrLine - strLine), pTextFormat, &hRectValue, pTextBrush, GPU2DTEXT_DRAWOPTION_CLIP );

    hRectTitle.fTop += fLineHeight;
    hRectTitle.fBottom += fLineHeight;
    hRectValue.fTop += fLineHeight;
    hRectValue.fBottom += fLineHeight;

    // Movement Speed
    pStrLine = strLine;
    pStrLine = StringFn->Format( pStrLine, TEXT("Movement Speed :") );

    pTextFormat->SetTextAlignment( GPU2DTEXT_ALIGNMENT_TRAILING );
    pTextFormat->SetParagraphAlignment( GPU2DTEXT_PARAGRAPHALIGNMENT_CENTER );

    pTextBrush->SetColor( &hTitleColor );
    pHUDContext->DrawText( strLine, (pStrLine - strLine), pTextFormat, &hRectTitle, pTextBrush, GPU2DTEXT_DRAWOPTION_CLIP );

    pStrLine = strLine;
    pStrLine = StringFn->Format( pStrLine, TEXT("%1.3f"), pModel->GetMovementSpeed() );

    pTextFormat->SetTextAlignment( GPU2DTEXT_ALIGNMENT_LEADING );
    pTextFormat->SetParagraphAlignment( GPU2DTEXT_PARAGRAPHALIGNMENT_CENTER );

    pTextBrush->SetColor( &hValueColor );
    pHUDContext->DrawText( strLine, (pStrLine - strLine), pTextFormat, &hRectValue, pTextBrush, GPU2DTEXT_DRAWOPTION_CLIP );

    hRectTitle.fTop += fLineHeight;
    hRectTitle.fBottom += fLineHeight;
    hRectValue.fTop += fLineHeight;
    hRectValue.fBottom += fLineHeight;

    // Statistics
    for( UInt i = 0; i < CHARSTAT_COUNT; ++i ) {
        pStrLine = strLine;
        pStrLine = StringFn->Format( pStrLine, TEXT("%s :"), CharStatPackage::GetStatName((CharStatistic)i) );

        pTextFormat->SetTextAlignment( GPU2DTEXT_ALIGNMENT_TRAILING );
        pTextFormat->SetParagraphAlignment( GPU2DTEXT_PARAGRAPHALIGNMENT_CENTER );

        pTextBrush->SetColor( &hTitleColor );
        pHUDContext->DrawText( strLine, (pStrLine - strLine), pTextFormat, &hRectTitle, pTextBrush, GPU2DTEXT_DRAWOPTION_CLIP );

        pStrLine = strLine;
        pStrLine = StringFn->Format( pStrLine, TEXT("%1.3f (+%1.3f)"), pModel->GetStat((CharStatistic)i), pModel->GetStatBonus((CharStatistic)i) );

        pTextFormat->SetTextAlignment( GPU2DTEXT_ALIGNMENT_LEADING );
        pTextFormat->SetParagraphAlignment( GPU2DTEXT_PARAGRAPHALIGNMENT_CENTER );

        pTextBrush->SetColor( &hValueColor );
        pHUDContext->DrawText( strLine, (pStrLine - strLine), pTextFormat, &hRectValue, pTextBrush, GPU2DTEXT_DRAWOPTION_CLIP );

        hRectTitle.fTop += fLineHeight;
        hRectTitle.fBottom += fLineHeight;
        hRectValue.fTop += fLineHeight;
        hRectValue.fBottom += fLineHeight;
    }
}
