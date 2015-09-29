/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Interface/StatusEffectBar.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Interface : StatusEffectBar
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
#include "StatusEffectBar.h"

#include "InterfaceManager.h"

/////////////////////////////////////////////////////////////////////////////////
// StatusEffectBarModel implementation
StatusEffectBarModel::StatusEffectBarModel( BaseCharacter * pCharacter ):
    GUIWidgetModel()
{
    m_pCharacter = pCharacter;

    // Context Menu
    /////////////////////////////////////////////
}
StatusEffectBarModel::~StatusEffectBarModel()
{
    // nothing to do
}

Void StatusEffectBarModel::OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags )
{
    // Remove self-buff case
    /////////////////////////////////////////////

    // Default handler
    GUIWidgetModel::OnClick( ptLocalPos, iKey, iFlags );
}

/////////////////////////////////////////////////////////////////////////////////
// StatusEffectBar implementation
StatusEffectBar::StatusEffectBar( StatusEffectBarModel * pModel, GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName ):
    GUIWidget( pModel, pParent, guiLayout, strName )
{
}
StatusEffectBar::~StatusEffectBar()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

Void StatusEffectBar::_Draw( const Rectangle2 & rectClient )
{
    GPU2DContext * pHUDContext = InterfaceFn->GetHUDContext();

    // Model
    StatusEffectBarModel * pModel = (StatusEffectBarModel*)_GetWidgetModel();

    // Skin
    GPU2DBitmap * pHUDBitmap = InterfaceFn->GetHUDSkinBitmap();
    const Rectangle2 & locBackground = InterfaceFn->GetHUDLocation( HUD_IMAGE_BACKGROUND_STATUSEFFECTBAR );

    GPU2DBitmap * pHUDStatusEffectBitmap = InterfaceFn->GetHUDStatusEffectBitmap();

    // Background
    GPU2DRectangle hDstRect = rectClient;
    GPU2DRectangle hSrcRect = locBackground;

    pHUDContext->DrawBitmap( pHUDBitmap, &hDstRect, &hSrcRect );

    // Status Effects
    GPU2DRectangle hSlotRect = rectClient;
    hSlotRect.fLeft = hSlotRect.fRight - (Scalar)INTERFACE_HUD_ICON_SIZE;

    pModel->EnumStatusEffects();
    StatusEffectID idStatusEffect = pModel->EnumNextStatusEffect();
    while( idStatusEffect != STATUSEFFECTID_UNDEFINED ) {
        const Rectangle2 & rectIconLocation = InterfaceFn->GetHUDLocation( idStatusEffect );

        hSrcRect = rectIconLocation;

        pHUDContext->DrawBitmap( pHUDStatusEffectBitmap, &hSlotRect, &hSrcRect );

        hSlotRect.fLeft -= (Scalar)INTERFACE_HUD_ICON_SIZE;
        hSlotRect.fRight -= (Scalar)INTERFACE_HUD_ICON_SIZE;
        idStatusEffect = pModel->EnumNextStatusEffect();
    }
}


