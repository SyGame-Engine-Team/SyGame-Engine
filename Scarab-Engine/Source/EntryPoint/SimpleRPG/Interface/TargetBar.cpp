/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Interface/TargetBar.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Interface : TargetBar
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
#include "TargetBar.h"

#include "InterfaceManager.h"

/////////////////////////////////////////////////////////////////////////////////
// TargetBarModel implementation
TargetBarModel::TargetBarModel( BaseCharacter * pCharacter, Bool bUseFocus ):
    GUIWidgetModel()
{
    m_pCharacter = pCharacter;
    m_bUseFocus = bUseFocus;

    m_bShowName = true;
    m_bShowClass = false;
    m_bShowValues = true;
    m_bShowPercent = false;
    m_bShowStatusEffectBar = false;
    m_bShowCastBar = true;
    m_bShowTargetOfTargetBar = true;

    m_fHPBarSize = 0.75f;
    m_fMPBarSize = 0.25f;
    m_fStatusEffectBarSize = 0.5f;
    m_fCastBarSize = 0.5f;
    m_fTargetOfTargetBarSize = 0.25f;

    // Context Menu
    /////////////////////////////////////////////
}
TargetBarModel::~TargetBarModel()
{
    // nothing to do
}

Void TargetBarModel::OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags )
{
    // Focus-Swap case
    if ( iKey == KEYCODE_MOUSELEFT ) {
        if ( m_bUseFocus ) {
            m_pCharacter->SwapFocusTarget();
            return;
        }
    }

    // Default handler
    GUIWidgetModel::OnClick( ptLocalPos, iKey, iFlags );
}

/////////////////////////////////////////////////////////////////////////////////
// TargetBar implementation
TargetBar::TargetBar( TargetBarModel * pModel, GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName ):
    GUIWidget( pModel, pParent, guiLayout, strName )
{
    // nothing to do
}
TargetBar::~TargetBar()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

Void TargetBar::_Draw( const Rectangle2 & rectClient )
{
    GPU2DContext * pHUDContext = InterfaceFn->GetHUDContext();

    // Model
    TargetBarModel * pModel = (TargetBarModel*)_GetWidgetModel();
    if ( !( pModel->HasTarget() ) )
        return;

    GPU2DRectangle hMaxHPRect, hMaxMPRect, hStatusEffectsBarRect, hCastBarRect, hTargetOfTargetMaxHPRect;

    hMaxHPRect.fLeft = (Scalar)( rectClient.TopLeft.X );
    hMaxMPRect.fLeft = (Scalar)( rectClient.TopLeft.X );
    hStatusEffectsBarRect.fLeft = (Scalar)( rectClient.TopLeft.X );
    hStatusEffectsBarRect.fRight = (Scalar)( rectClient.DownRight.X );
    hCastBarRect.fLeft = (Scalar)( rectClient.TopLeft.X );
    hCastBarRect.fRight = (Scalar)( rectClient.DownRight.X );
    if ( pModel->ShowTargetOfTargetBar() ) {
        Scalar fTargetOfTargetWidth = ( pModel->GetTargetOfTargetBarSize() * (Scalar)(rectClient.Width()) );
        hTargetOfTargetMaxHPRect.fLeft = ((Scalar)(rectClient.DownRight.X)) - fTargetOfTargetWidth;
        hTargetOfTargetMaxHPRect.fRight = (Scalar)( rectClient.DownRight.X );

        hMaxHPRect.fRight = hTargetOfTargetMaxHPRect.fLeft;
        hMaxMPRect.fRight = hTargetOfTargetMaxHPRect.fLeft;
    } else {
        hMaxHPRect.fRight = (Scalar)( rectClient.DownRight.X );
        hMaxMPRect.fRight = (Scalar)( rectClient.DownRight.X );
    }
    if ( pModel->ShowStatusEffectBar() ) {
        if ( pModel->ShowCastBar() ) {
            Scalar fTopPartHeight = ( 0.5f * (Scalar)(rectClient.Height()) );
            Scalar fBottomPartHeight = ( ((Scalar)(rectClient.Height())) - fTopPartHeight );

            Scalar fHPHeight = ( pModel->GetHPBarSize() * fTopPartHeight );
            Scalar fMPHeight = ( fTopPartHeight - fHPHeight );

            hMaxHPRect.fTop = (Scalar)( rectClient.TopLeft.Y );
            hMaxHPRect.fBottom = hMaxHPRect.fTop + fHPHeight;
            hMaxMPRect.fTop = hMaxHPRect.fBottom;
            hMaxMPRect.fBottom = hMaxMPRect.fTop + fMPHeight;

            Scalar fStatusEffectBarHeight = ( pModel->GetStatusEffectBarSize() * fBottomPartHeight );

            hStatusEffectsBarRect.fTop = hMaxMPRect.fBottom;
            hStatusEffectsBarRect.fBottom = hStatusEffectsBarRect.fTop + fStatusEffectBarHeight;
            hCastBarRect.fTop = hStatusEffectsBarRect.fBottom;
            hCastBarRect.fBottom = (Scalar)( rectClient.DownRight.Y );
        } else {
            Scalar fTopPartHeight = ( 0.75f * (Scalar)(rectClient.Height()) );

            Scalar fHPHeight = ( pModel->GetHPBarSize() * fTopPartHeight );
            Scalar fMPHeight = ( fTopPartHeight - fHPHeight );

            hMaxHPRect.fTop = (Scalar)( rectClient.TopLeft.Y );
            hMaxHPRect.fBottom = hMaxHPRect.fTop + fHPHeight;
            hMaxMPRect.fTop = hMaxHPRect.fBottom;
            hMaxMPRect.fBottom = hMaxMPRect.fTop + fMPHeight;

            hStatusEffectsBarRect.fTop = hMaxMPRect.fBottom;
            hStatusEffectsBarRect.fBottom = (Scalar)( rectClient.DownRight.Y );
        }
    } else {
        if ( pModel->ShowCastBar() ) {
            Scalar fTopPartHeight = ( 0.75f * (Scalar)(rectClient.Height()) );

            Scalar fHPHeight = ( pModel->GetHPBarSize() * fTopPartHeight );
            Scalar fMPHeight = ( fTopPartHeight - fHPHeight );

            hMaxHPRect.fTop = (Scalar)( rectClient.TopLeft.Y );
            hMaxHPRect.fBottom = hMaxHPRect.fTop + fHPHeight;
            hMaxMPRect.fTop = hMaxHPRect.fBottom;
            hMaxMPRect.fBottom = hMaxMPRect.fTop + fMPHeight;

            hCastBarRect.fTop = hMaxMPRect.fBottom;
            hCastBarRect.fBottom = (Scalar)( rectClient.DownRight.Y );
        } else {
            Scalar fHPHeight = ( pModel->GetHPBarSize() * (Scalar)(rectClient.Height()) );

            hMaxHPRect.fTop = (Scalar)( rectClient.TopLeft.Y );
            hMaxHPRect.fBottom = hMaxHPRect.fTop + fHPHeight;
            hMaxMPRect.fTop = hMaxHPRect.fBottom;
            hMaxMPRect.fBottom = (Scalar)( rectClient.DownRight.Y );
        }
    }
    if ( pModel->ShowTargetOfTargetBar() ) {
        hTargetOfTargetMaxHPRect.fTop = hMaxHPRect.fTop;
        hTargetOfTargetMaxHPRect.fBottom = hMaxMPRect.fBottom;
    }

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
    const Rectangle2 & locBackgroundStatusEffectsBar = InterfaceFn->GetHUDLocation( HUD_IMAGE_BACKGROUND_STATUSEFFECTBAR );
    const Rectangle2 & locForegroundStatusEffectsBar = InterfaceFn->GetHUDLocation( HUD_IMAGE_FOREGROUND_STATUSEFFECTBAR );
    const Rectangle2 & locBackgroundCastBar = InterfaceFn->GetHUDLocation( HUD_IMAGE_BACKGROUND_CASTBAR );
    const Rectangle2 & locForegroundCastBar = InterfaceFn->GetHUDLocation( HUD_IMAGE_FOREGROUND_CASTBAR );

    GPU2DColor hTextColor = InterfaceFn->GetHUDColor( HUD_COLOR_TARGETBAR_TEXT );

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

    // StatusEffects Bar
    if ( pModel->ShowStatusEffectBar() ) {
        hSrcRect = locBackgroundStatusEffectsBar;
        pHUDContext->DrawBitmap( pHUDBitmap, &hStatusEffectsBarRect, &hSrcRect );

        //UInt iStatusEffectsCount = pModel->GetStatusEffectCount();
        //for( UInt i = 0; i < iStatusEffectsCount; ++i ) {
        //    StatusEffectInstance * pStatusEffectInstance = pModel->GetStatusEffect( i );

        //    ///////////////////////////////////////////////
        //}
    }

    // Casting Bar
    if ( pModel->ShowCastBar() ) {
        Scalar fCastTime = 0.0f;
        SkillID idSkill = pModel->GetCastingState( &fCastTime );
        if ( idSkill != SKILLID_UNDEFINED ) {
            Skill * pSkill = GameplayFn->GetSkill( idSkill );
            Scalar fRatioCastTime = ( fCastTime / pSkill->GetCastingTime() );

            GPU2DRectangle hCastTimeRect = hCastBarRect;
            hCastTimeRect.fRight = hCastTimeRect.fLeft + ( (1.0f - fRatioCastTime) * (hCastBarRect.fRight - hCastBarRect.fLeft) );

            hSrcRect = locBackgroundCastBar;
            pHUDContext->DrawBitmap( pHUDBitmap, &hCastBarRect, &hSrcRect );

            hSrcRect = locForegroundCastBar;
            pHUDContext->DrawBitmap( pHUDBitmap, &hCastTimeRect, &hSrcRect );

            pStrHP = strHP;
            if ( pModel->ShowName() )
                pStrHP = StringFn->Format( pStrHP, TEXT("%s "), pSkill->GetName() );
            if ( pModel->ShowValues() )
                pStrHP = StringFn->Format( pStrHP, TEXT("%1.3f / %1.3f "), MathFn->RoundDecimal(fCastTime, 1), pSkill->GetCastingTime() );
            if ( pStrHP != strHP ) {
                pTextFormat->SetTextAlignment( GPU2DTEXT_ALIGNMENT_CENTER );
                pTextFormat->SetParagraphAlignment( GPU2DTEXT_PARAGRAPHALIGNMENT_CENTER );
                pTextBrush->SetColor( &hTextColor );

                GPU2DRectangle hDstRect = hCastBarRect;
                hDstRect.fLeft += (Scalar)GUISKIN_MARGINS;
                hDstRect.fRight -= (Scalar)GUISKIN_MARGINS;

                pHUDContext->DrawText( strHP, (pStrHP - strHP), pTextFormat, &hDstRect, pTextBrush, GPU2DTEXT_DRAWOPTION_CLIP );
            }
        }
    }

    // TargetOfTarget Bar
    if ( pModel->ShowTargetOfTargetBar() && pModel->HasTargetOfTarget() ) {
        UInt iTargetHP = pModel->GetTargetOfTargetCurrentHP();
        UInt iTargetMaxHP = pModel->GetTargetOfTargetMaxHP();
        Scalar fRatioTargetHP = ( ((Scalar)iTargetHP) / (Scalar)(iTargetMaxHP) );

        GPU2DRectangle hTargetOfTargetHPRect = hTargetOfTargetMaxHPRect;
        hTargetOfTargetHPRect.fTop = hTargetOfTargetHPRect.fBottom - ( fRatioTargetHP * (hTargetOfTargetMaxHPRect.fBottom - hTargetOfTargetMaxHPRect.fTop) );

        hSrcRect = locBackgroundHP;
        pHUDContext->DrawBitmap( pHUDBitmap, &hTargetOfTargetMaxHPRect, &hSrcRect );

        hSrcRect = locForegroundHP;
        pHUDContext->DrawBitmap( pHUDBitmap, &hTargetOfTargetHPRect, &hSrcRect );

        pStrHP = strHP;
        if ( pModel->ShowName() )
            pStrHP = StringFn->Format( pStrHP, TEXT("%s"), pModel->GetTargetOfTargetName() );
        if ( pModel->ShowClass() )
            pStrHP = StringFn->Format( pStrHP, TEXT("\n(%s)"), CharStatPackage::GetClassName(pModel->GetTargetOfTargetClass()) );
        if ( pModel->ShowValues() ) {
            if ( pModel->ShowPercentValues() )
                pStrHP = StringFn->Format( pStrHP, TEXT("\n%1.3f %%"), MathFn->RoundDecimal(fRatioTargetHP * 100.0f, 1) );
            else
                pStrHP = StringFn->Format( pStrHP, TEXT("\n%d / %d"), iTargetHP, iTargetMaxHP );
        }
        if ( pStrHP != strHP ) {
            pTextFormat->SetTextAlignment( GPU2DTEXT_ALIGNMENT_LEADING );
            pTextFormat->SetParagraphAlignment( GPU2DTEXT_PARAGRAPHALIGNMENT_CENTER );
            pTextBrush->SetColor( &hTextColor );

            GPU2DRectangle hDstRect = hTargetOfTargetMaxHPRect;
            hDstRect.fLeft += (Scalar)GUISKIN_MARGINS;
            hDstRect.fRight -= (Scalar)GUISKIN_MARGINS;

            pHUDContext->DrawText( strHP, (pStrHP - strHP), pTextFormat, &hDstRect, pTextBrush, GPU2DTEXT_DRAWOPTION_CLIP );
        }
    }    
}




