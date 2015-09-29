/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/Interface/SelfBar.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : WorldCraft, Interface : SelfBar
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
#include "SelfBar.h"

#include "InterfaceManager.h"

/////////////////////////////////////////////////////////////////////////////////
// SelfBarModel implementation
SelfBarModel::SelfBarModel( BaseCharacter * pCharacter ):
    GUIWidgetModel()
{
    m_pCharacter = pCharacter;

    m_bShowName = true;
    m_bShowClass = false;
    m_bShowValues = true;
    m_bShowPercent = false;

    m_fHPBarSize = 0.5f;
    m_fMPBarSize = 0.5f;

    // Context Menu
    /////////////////////////////////////////////
}
SelfBarModel::~SelfBarModel()
{
    // nothing to do
}

Void SelfBarModel::OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags )
{
    // Auto-Selection case
    if ( iKey == KEYCODE_MOUSELEFT ) {
        m_pCharacter->SelectTarget( m_pCharacter );
        return;
    }

    // Default handler
    GUIWidgetModel::OnClick( ptLocalPos, iKey, iFlags );
}

/////////////////////////////////////////////////////////////////////////////////
// SelfBar implementation
SelfBar::SelfBar( SelfBarModel * pModel, GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName ):
    GUIWidget( pModel, pParent, guiLayout, strName )
{
    // nothing to do
}
SelfBar::~SelfBar()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

Void SelfBar::_Draw( const Rectangle2 & rectClient )
{
    GPU2DContext * pHUDContext = InterfaceFn->GetHUDContext();

    // Model
    SelfBarModel * pModel = (SelfBarModel*)_GetWidgetModel();

    Scalar fHPHeight = ( pModel->GetHPBarSize() * (Scalar)(rectClient.Height()) );
    Scalar fMPHeight = ( ((Scalar)(rectClient.Height())) - fHPHeight );

    GPU2DRectangle hMaxHPRect = rectClient;
    hMaxHPRect.fBottom = hMaxHPRect.fTop + fHPHeight;
    GPU2DRectangle hMaxMPRect = rectClient;
    hMaxMPRect.fTop = hMaxMPRect.fBottom - fMPHeight;

    UInt iHP = pModel->GetCurrentHP();
    UInt iMaxHP = pModel->GetMaxHP();
    UInt iMP = pModel->GetCurrentMP();
    UInt iMaxMP = pModel->GetMaxMP();

    Scalar fRatioHP = ( ((Scalar)iHP) / (Scalar)(iMaxHP) );
    Scalar fRatioMP = ( ((Scalar)iMP) / (Scalar)(iMaxMP) );

    GPU2DRectangle hCurHPRect = hMaxHPRect;
    hCurHPRect.fRight = hCurHPRect.fLeft + ( fRatioHP * (hMaxHPRect.fRight - hMaxHPRect.fLeft) );
    GPU2DRectangle hCurMPRect = hMaxMPRect;
    hCurMPRect.fRight = hCurMPRect.fLeft + ( fRatioMP * (hMaxMPRect.fRight - hMaxMPRect.fLeft) );

    // Skin
    GPU2DBitmap * pHUDBitmap = InterfaceFn->GetHUDSkinBitmap();
    const Rectangle2 & locBackgroundHP = InterfaceFn->GetHUDLocation( HUD_IMAGE_BACKGROUND_HPBAR );
    const Rectangle2 & locForegroundHP = InterfaceFn->GetHUDLocation( HUD_IMAGE_FOREGROUND_HPBAR );
    const Rectangle2 & locBackgroundMP = InterfaceFn->GetHUDLocation( HUD_IMAGE_BACKGROUND_MPBAR );
    const Rectangle2 & locForegroundMP = InterfaceFn->GetHUDLocation( HUD_IMAGE_FOREGROUND_MPBAR );

    GPU2DColor hTextColor = InterfaceFn->GetHUDColor( HUD_COLOR_SELFBAR_TEXT );

    GPU2DSolidColorBrush * pTextBrush = InterfaceFn->GetHUDTextBrush();
    GPU2DTextFormat * pTextFormat = InterfaceFn->GetHUDTextFormat( HUD_FONT_SELFBAR_TEXT );

    // HP/MP Bars
    GPU2DRectangle hSrcRect = locBackgroundHP;
    pHUDContext->DrawBitmap( pHUDBitmap, &hMaxHPRect, &hSrcRect );

    hSrcRect = locBackgroundMP;
    pHUDContext->DrawBitmap( pHUDBitmap, &hMaxMPRect, &hSrcRect );

    hSrcRect = locForegroundHP;
    pHUDContext->DrawBitmap( pHUDBitmap, &hCurHPRect, &hSrcRect );

    hSrcRect = locForegroundMP;
    pHUDContext->DrawBitmap( pHUDBitmap, &hCurMPRect, &hSrcRect );

    // HP/MP Text
    GChar strHP[64];
    GChar strMP[32];

    GChar * pStrHP = strHP;
    GChar * pStrMP = strMP;
    if ( pModel->ShowName() )
        pStrHP = StringFn->Format( pStrHP, TEXT("%s "), pModel->GetName() );
    if ( pModel->ShowClass() )
        pStrHP = StringFn->Format( pStrHP, TEXT("(%s) "), CharStatPackage::GetClassName(pModel->GetClass()) );
    if ( pModel->ShowValues() ) {
        if ( pModel->ShowPercentValues() ) {
            pStrHP = StringFn->Format( pStrHP, TEXT("%1.3f %% "), MathFn->RoundDecimal(fRatioHP * 100.0f, 1) );
            pStrMP = StringFn->Format( pStrMP, TEXT("%1.3f %% "), MathFn->RoundDecimal(fRatioMP * 100.0f, 1) );
        } else {
            pStrHP = StringFn->Format( pStrHP, TEXT("%d / %d "), iHP, iMaxHP );
            pStrMP = StringFn->Format( pStrMP, TEXT("%d / %d "), iMP, iMaxMP );
        }
    }
    if ( pStrHP != strHP ) {
        pTextFormat->SetTextAlignment( GPU2DTEXT_ALIGNMENT_LEADING );
        pTextFormat->SetParagraphAlignment( GPU2DTEXT_PARAGRAPHALIGNMENT_CENTER );
        pTextBrush->SetColor( &hTextColor );

        GPU2DRectangle hDstRect = hMaxHPRect;
        hDstRect.fLeft += (Scalar)GUISKIN_MARGINS;
        hDstRect.fRight -= (Scalar)GUISKIN_MARGINS;

        pHUDContext->DrawText( strHP, (pStrHP - strHP), pTextFormat, &hDstRect, pTextBrush, GPU2DTEXT_DRAWOPTION_CLIP );

        if ( pStrMP != strMP ) {
            hDstRect = hMaxMPRect;
            hDstRect.fLeft += (Scalar)GUISKIN_MARGINS;
            hDstRect.fRight -= (Scalar)GUISKIN_MARGINS;

            pHUDContext->DrawText( strMP, (pStrMP - strMP), pTextFormat, &hDstRect, pTextBrush, GPU2DTEXT_DRAWOPTION_CLIP );
        }
    }
}




