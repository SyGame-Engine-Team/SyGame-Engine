/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Interface/CastBar.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Interface : CastBar
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
#include "CastBar.h"

#include "InterfaceManager.h"

/////////////////////////////////////////////////////////////////////////////////
// CastBarModel implementation
CastBarModel::CastBarModel( BaseCharacter * pCharacter ):
    GUIWidgetModel()
{
    m_pCharacter = pCharacter;

    m_bShowName = true;
    m_bShowValues = true;

    // Context Menu
    /////////////////////////////////////////////
}
CastBarModel::~CastBarModel()
{
    // nothing to do
}

Void CastBarModel::OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags )
{
    // nothing to do

    // Default handler
    GUIWidgetModel::OnClick( ptLocalPos, iKey, iFlags );
}

/////////////////////////////////////////////////////////////////////////////////
// CastBar implementation
CastBar::CastBar( CastBarModel * pModel, GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName ):
    GUIWidget( pModel, pParent, guiLayout, strName )
{
    // nothing to do
}
CastBar::~CastBar()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

Void CastBar::_Draw( const Rectangle2 & rectClient )
{
    GPU2DContext * pHUDContext = InterfaceFn->GetHUDContext();

    // Model
    CastBarModel * pModel = (CastBarModel*)_GetWidgetModel();
    if ( !(pModel->IsCasting()) )
        return;

    // Skin
    GPU2DBitmap * pHUDBitmap = InterfaceFn->GetHUDSkinBitmap();
    const Rectangle2 & locBackgroundCastBar = InterfaceFn->GetHUDLocation( HUD_IMAGE_BACKGROUND_CASTBAR );
    const Rectangle2 & locForegroundCastBar = InterfaceFn->GetHUDLocation( HUD_IMAGE_FOREGROUND_CASTBAR );

    GPU2DColor hTextColor = InterfaceFn->GetHUDColor( HUD_COLOR_CASTBAR_TEXT );

    GPU2DSolidColorBrush * pTextBrush = InterfaceFn->GetHUDTextBrush();
    GPU2DTextFormat * pTextFormat = InterfaceFn->GetHUDTextFormat( HUD_FONT_CASTBAR_TEXT );

    // Background & Foreground
    Scalar fCastTime = 0.0f;
    SkillID idSkill = pModel->GetCastingState( &fCastTime );
    Skill * pSkill = GameplayFn->GetSkill( idSkill );

    Scalar fRatioCastTime = ( fCastTime / pSkill->GetCastingTime() );

    GPU2DRectangle hDstBackRect = rectClient;
    GPU2DRectangle hDstFrontRect = rectClient;
    hDstFrontRect.fRight = hDstFrontRect.fLeft + MathFn->Floor( (1.0f - fRatioCastTime) * (Scalar)(rectClient.Width()) );

    GPU2DRectangle hSrcRect = locBackgroundCastBar;
    pHUDContext->DrawBitmap( pHUDBitmap, &hDstBackRect, &hSrcRect );

    hSrcRect = locForegroundCastBar;
    pHUDContext->DrawBitmap( pHUDBitmap, &hDstFrontRect, &hSrcRect );

    // Text
    GChar strCast[64];

    GChar * pStrCast = strCast;
    if ( pModel->ShowName() )
        pStrCast = StringFn->Format( pStrCast, TEXT("%s "), pSkill->GetName() );
    if ( pModel->ShowValues() )
        pStrCast = StringFn->Format( pStrCast, TEXT("%1.3f / %1.3f "), MathFn->RoundDecimal(fCastTime, 1), pSkill->GetCastingTime() );
    if ( pStrCast != strCast ) {
        pTextFormat->SetTextAlignment( GPU2DTEXT_ALIGNMENT_CENTER );
        pTextFormat->SetParagraphAlignment( GPU2DTEXT_PARAGRAPHALIGNMENT_CENTER );
        pTextBrush->SetColor( &hTextColor );

        GPU2DRectangle hDstRect = rectClient;

        pHUDContext->DrawText( strCast, (pStrCast - strCast), pTextFormat, &hDstRect, pTextBrush, GPU2DTEXT_DRAWOPTION_CLIP );
    }
}




