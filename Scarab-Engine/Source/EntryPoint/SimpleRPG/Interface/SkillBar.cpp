/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Interface/SkillBar.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Interface : SkillBar
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
#include "SkillBar.h"

#include "InterfaceManager.h"

/////////////////////////////////////////////////////////////////////////////////
// SkillBarModel implementation
SkillBarModel::SkillBarModel( BaseCharacter * pCharacter, UInt iRowCount, UInt iSlotsPerRow, UInt iSlotCount ):
    GUIWidgetModel()
{
    m_pCharacter = pCharacter;

    m_iRowCount = iRowCount;
    m_iSlotsPerRow = iSlotsPerRow;
    m_iSlotCount = iSlotCount;
    for( UInt i = 0; i < HUD_SKILLBAR_SIZE; ++i )
        m_arrSlots[i] = SKILLID_UNDEFINED;

    m_bShowKeyBindings = false;

    // Context Menu
    /////////////////////////////////////////////
}
SkillBarModel::~SkillBarModel()
{
    // nothing to do
}

Void SkillBarModel::OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags )
{
    // PS : Shame on you Mouse-Clickers !

    // Cast case
    if ( iKey == KEYCODE_MOUSELEFT ) {
        UInt iSlot = _PickSlot( ptLocalPos );
        SkillID idSkill = m_arrSlots[iSlot];

        if ( idSkill != SKILLID_UNDEFINED )
            m_pCharacter->Cast( idSkill, m_pCharacter->GetSelectedTarget() );

        return;
    }

    // Self-cast case
    if ( iKey == KEYCODE_MOUSERIGHT ) {
        UInt iSlot = _PickSlot( ptLocalPos );
        SkillID idSkill = m_arrSlots[iSlot];

        if ( idSkill != SKILLID_UNDEFINED )
            m_pCharacter->Cast( idSkill, m_pCharacter );

        return;
    }

    // Default handler
    GUIWidgetModel::OnClick( ptLocalPos, iKey, iFlags );
}

Void SkillBarModel::OnStartDrag( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags )
{
    if ( iKey == KEYCODE_MOUSELEFT ) {
        UInt iSlot = _PickSlot( ptLocalPos );
        SkillID idSkill = m_arrSlots[iSlot];
        m_arrSlots[iSlot] = SKILLID_UNDEFINED;

        HUDDragNDropData * pDragNDropData = InterfaceFn->GetDragNDropData();
        pDragNDropData->iType = HUD_DRAGTYPE_SKILL;
        pDragNDropData->iIdentifier = idSkill;

        GUIFn->SetDragData( GUIDRAG_LEFT, pDragNDropData );

        return;
    }

    // Default handler
    GUIWidgetModel::OnStartDrag( ptLocalPos, iKey, iFlags );
}
Void SkillBarModel::OnStopDrag( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags )
{
    if ( iKey == KEYCODE_MOUSELEFT ) {
        HUDDragNDropData * pDragNDropData = InterfaceFn->GetDragNDropData();
        if ( pDragNDropData->iType == HUD_DRAGTYPE_SKILL ) {
            Skill * pSkill =  GameplayFn->GetSkill( (SkillID)(pDragNDropData->iIdentifier) );
            if ( pSkill != NULL && pSkill->IsActive() ) {
                UInt iSlot = _PickSlot( ptLocalPos );
                m_arrSlots[iSlot] = pSkill->GetID();
            }
        }

        GUIFn->SetDragData( GUIDRAG_LEFT, NULL );

        return;
    }

    // Default handler
    GUIWidgetModel::OnStopDrag( ptLocalPos, iKey, iFlags );
}

/////////////////////////////////////////////////////////////////////////////////

UInt SkillBarModel::_PickSlot( const Point2 & ptLocalPos ) const
{
    //SkillBar * pSkillBar = (SkillBar*)( _GetWidget() );
    //UInt iWidth = pSkillBar->GetClientRect().Width();
    //UInt iHeight = pSkillBar->GetClientRect().Height();
    
    UInt iRow = ( ptLocalPos.Y / INTERFACE_HUD_ICON_SIZE );
    if ( iRow > m_iRowCount - 1 )
        iRow = m_iRowCount - 1;

    UInt iSlot = ( iRow * m_iSlotsPerRow );
    iSlot += ( ptLocalPos.X / INTERFACE_HUD_ICON_SIZE );
    if ( iSlot > m_iSlotCount - 1 )
        iSlot = m_iSlotCount - 1;

    return iSlot;
}

/////////////////////////////////////////////////////////////////////////////////
// SkillBar implementation
SkillBar::SkillBar( SkillBarModel * pModel, GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName ):
    GUIWidget( pModel, pParent, guiLayout, strName )
{
}
SkillBar::~SkillBar()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

Void SkillBar::_Draw( const Rectangle2 & rectClient )
{
    GPU2DContext * pHUDContext = InterfaceFn->GetHUDContext();

    // Model
    SkillBarModel * pModel = (SkillBarModel*)_GetWidgetModel();

    UInt iSlotWidth = INTERFACE_HUD_ICON_SIZE;
    UInt iSlotHeight = INTERFACE_HUD_ICON_SIZE;

    UInt iRowCount = pModel->GetRowCount();
    UInt iSlotsPerRow = pModel->GetSlotsPerRow();
    UInt iSlotCount = pModel->GetSlotCount();

    // Skin
    GPU2DBitmap * pHUDBitmap = InterfaceFn->GetHUDSkinBitmap();
    const Rectangle2 & locBackground = InterfaceFn->GetHUDLocation( HUD_IMAGE_BACKGROUND_SKILLBAR );
    const Rectangle2 & locForegroundGCD = InterfaceFn->GetHUDLocation( HUD_IMAGE_FOREGROUND_SKILLBAR_GCD );
    const Rectangle2 & locForegroundCD = InterfaceFn->GetHUDLocation( HUD_IMAGE_FOREGROUND_SKILLBAR_CD );

    GPU2DBitmap * pHUDSkillsBitmap = InterfaceFn->GetHUDSkillsBitmap();

    // Background
    GPU2DRectangle hDstRect = rectClient;
    GPU2DRectangle hSrcRect = locBackground;

    pHUDContext->DrawBitmap( pHUDBitmap, &hDstRect, &hSrcRect );

    // Skills
    GPU2DRectangle hSlotRect = rectClient;
    hSlotRect.fRight = hSlotRect.fLeft + (Scalar)iSlotWidth;
    hSlotRect.fBottom = hSlotRect.fTop + (Scalar)iSlotHeight;

    UInt iSlot = 0;

    for( UInt iRow = 0; iRow < iRowCount; ++iRow ) {
        if ( iSlot >= iSlotCount )
            break;

        for( UInt i = 0; i < iSlotsPerRow; ++i ) {
            if ( iSlot >= iSlotCount )
                break;

            SkillID idSkill = pModel->GetSlot( iSlot++ );

            if ( idSkill != SKILLID_UNDEFINED ) {
                // Skill icon
                const Rectangle2 & rectIconLocation = InterfaceFn->GetHUDLocation( idSkill );

                hSrcRect = rectIconLocation;

                pHUDContext->DrawBitmap( pHUDSkillsBitmap, &hSlotRect, &hSrcRect );

                // Cooldown indicator
                if ( pModel->IsOnCooldown(idSkill) ) {
                    Scalar fSkillCooldown = pModel->GetCooldown( idSkill );
                    Scalar fRatio = ( fSkillCooldown / GameplayFn->GetSkill(idSkill)->GetCooldownTime() );

                    GPU2DRectangle hRectCD = hSlotRect;
                    hRectCD.fTop = hRectCD.fBottom - ( fRatio * (hSlotRect.fBottom - hSlotRect.fTop) );

                    hSrcRect = locForegroundCD;

                    pHUDContext->DrawBitmap( pHUDBitmap, &hRectCD, &hSrcRect );
                }
                if ( pModel->IsOnGlobalCooldown() ) {
                    Scalar fGlobalCooldown = pModel->GetGlobalCooldown();
                    Scalar fRatio = ( fGlobalCooldown / GAMEPLAY_GLOBAL_COOLDOWN );

                    GPU2DRectangle hRectGCD = hSlotRect;
                    hRectGCD.fTop = hRectGCD.fBottom - ( fRatio * (hSlotRect.fBottom - hSlotRect.fTop) );

                    hSrcRect = locForegroundGCD;

                    pHUDContext->DrawBitmap( pHUDBitmap, &hRectGCD, &hSrcRect );
                }

                // Key Binding
                //if ( pModel->ShowKeyBindings() ) {
                //    KeyCode iKey = pModel->GetKeyBinding( i );

                //    /////////////////////////////////
                //}
            }

            hSlotRect.fLeft += (Scalar)iSlotWidth;
            hSlotRect.fRight += (Scalar)iSlotWidth;
        }

        hSlotRect.fTop += (Scalar)iSlotHeight;
        hSlotRect.fBottom += (Scalar)iSlotHeight;
    }
}

