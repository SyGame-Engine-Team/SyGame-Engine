/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Interface/InventoryView.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Interface : InventoryView
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
#include "InventoryView.h"

#include "InterfaceManager.h"

/////////////////////////////////////////////////////////////////////////////////
// InventoryViewModel implementation
InventoryViewModel::InventoryViewModel( BaseCharacter * pCharacter ):
    GUIWidgetModel()
{
    m_pCharacter = pCharacter;

    m_idCurrentBag = INVENTORYBAGID_BUFFER;

    // Context Menu
    /////////////////////////////////////////////
}
InventoryViewModel::~InventoryViewModel()
{
    // nothing to do
}

Void InventoryViewModel::OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags )
{
    // Equipped Item case
    // Switch Bag case
    // Bag Item case
    /////////////////////////////////////////////

    // Default handler
    GUIWidgetModel::OnClick( ptLocalPos, iKey, iFlags );
}

/////////////////////////////////////////////////////////////////////////////////
// InventoryView implementation
InventoryView::InventoryView( InventoryViewModel * pModel, GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName ):
    GUIWidget( pModel, pParent, guiLayout, strName )
{
    // nothing to do
}
InventoryView::~InventoryView()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

Void InventoryView::_Draw( const Rectangle2 & rectClient )
{
    GPU2DContext * pHUDContext = InterfaceFn->GetHUDContext();

    // Model
    InventoryViewModel * pModel = (InventoryViewModel*)_GetWidgetModel();

    InventoryBagID idBag = pModel->GetCurrentBag();

    UInt iLeftColumnWidth = INTERFACE_HUD_ICON_SIZE * 3;
    UInt iMiddleColumnWidth = INTERFACE_HUD_ICON_SIZE * 8;
    UInt iRightColumnWidth = INTERFACE_HUD_ICON_SIZE;

    UInt iButtonHeight = INTERFACE_HUD_ICON_SIZE;

    // Skin
    GPU2DBitmap * pHUDBitmap = InterfaceFn->GetHUDSkinBitmap();
    const Rectangle2 & locBackground = InterfaceFn->GetHUDLocation( HUD_IMAGE_BACKGROUND_INVENTORYVIEW );
    const Rectangle2 & locForeground = InterfaceFn->GetHUDLocation( HUD_IMAGE_FOREGROUND_INVENTORYVIEW );

    const Rectangle2 locBagButton[INVENTORYBAGID_COUNT] = {
        InterfaceFn->GetHUDLocation( HUD_IMAGE_FOREGROUND_INVENTORYVIEW_BAGBUTTON_TRASH ),
        InterfaceFn->GetHUDLocation( HUD_IMAGE_FOREGROUND_INVENTORYVIEW_BAGBUTTON_CONSUMMABLE ),
        InterfaceFn->GetHUDLocation( HUD_IMAGE_FOREGROUND_INVENTORYVIEW_BAGBUTTON_EQUIPMENT ),
        InterfaceFn->GetHUDLocation( HUD_IMAGE_FOREGROUND_INVENTORYVIEW_BAGBUTTON_QUEST ),
        InterfaceFn->GetHUDLocation( HUD_IMAGE_FOREGROUND_INVENTORYVIEW_BAGBUTTON_BUFFER )
    };
    const Rectangle2 & locActiveBagButton = InterfaceFn->GetHUDLocation( HUD_IMAGE_FOREGROUND_INVENTORYVIEW_ACTIVEBAGBUTTON );

    GPU2DBitmap * pHUDItemsBitmap = InterfaceFn->GetHUDItemsBitmap();

    // Background
    GPU2DRectangle hDstRect = rectClient;
    GPU2DRectangle hSrcRect = locBackground;

    pHUDContext->DrawBitmap( pHUDBitmap, &hDstRect, &hSrcRect );

    // Equipment Slots
    GPU2DRectangle hRectEquipmentColumn = rectClient;
    hRectEquipmentColumn.fLeft += 4.0f;
    hRectEquipmentColumn.fTop += 4.0f;
    hRectEquipmentColumn.fRight = hRectEquipmentColumn.fLeft + (Scalar)iLeftColumnWidth;
    hRectEquipmentColumn.fBottom -= 4.0f;

    GPU2DRectangle hRectEquipSlot = hRectEquipmentColumn;
    hRectEquipSlot.fRight = hRectEquipSlot.fLeft + (Scalar)INTERFACE_HUD_ICON_SIZE;
    hRectEquipSlot.fBottom = hRectEquipSlot.fTop + (Scalar)INTERFACE_HUD_ICON_SIZE;

        // Head
    hRectEquipSlot.fLeft += (Scalar)INTERFACE_HUD_ICON_SIZE;
    hRectEquipSlot.fRight += (Scalar)INTERFACE_HUD_ICON_SIZE;

    ItemInstance * pItemInstance = pModel->GetItem( EQUIPSLOTID_HEAD );
    if ( pItemInstance->GetItem() != NULL ) {
        ItemID idItem = pItemInstance->GetID();
        const Rectangle2 & rectIconLocation = InterfaceFn->GetHUDLocation( idItem );

        hSrcRect = rectIconLocation;
        pHUDContext->DrawBitmap( pHUDItemsBitmap, &hRectEquipSlot, &hSrcRect );
    } else {
        // Empty slot
        hSrcRect = locForeground;
        pHUDContext->DrawBitmap( pHUDBitmap, &hRectEquipSlot, &hSrcRect );
    }

        // Neck
    hRectEquipSlot.fLeft += (Scalar)INTERFACE_HUD_ICON_SIZE;
    hRectEquipSlot.fRight += (Scalar)INTERFACE_HUD_ICON_SIZE;

    pItemInstance = pModel->GetItem( EQUIPSLOTID_NECK );
    if ( pItemInstance->GetItem() != NULL ) {
        ItemID idItem = pItemInstance->GetID();
        const Rectangle2 & rectIconLocation = InterfaceFn->GetHUDLocation( idItem );

        hSrcRect = rectIconLocation;
        pHUDContext->DrawBitmap( pHUDItemsBitmap, &hRectEquipSlot, &hSrcRect );
    } else {
        // Empty slot
        hSrcRect = locForeground;
        pHUDContext->DrawBitmap( pHUDBitmap, &hRectEquipSlot, &hSrcRect );
    }

        // Shoulders
    hRectEquipSlot.fLeft -= (Scalar)(INTERFACE_HUD_ICON_SIZE << 1);
    hRectEquipSlot.fRight -= (Scalar)(INTERFACE_HUD_ICON_SIZE << 1);
    hRectEquipSlot.fTop += (Scalar)INTERFACE_HUD_ICON_SIZE;
    hRectEquipSlot.fBottom += (Scalar)INTERFACE_HUD_ICON_SIZE;

    pItemInstance = pModel->GetItem( EQUIPSLOTID_SHOULDERS );
    if ( pItemInstance->GetItem() != NULL ) {
        ItemID idItem = pItemInstance->GetID();
        const Rectangle2 & rectIconLocation = InterfaceFn->GetHUDLocation( idItem );

        hSrcRect = rectIconLocation;
        pHUDContext->DrawBitmap( pHUDItemsBitmap, &hRectEquipSlot, &hSrcRect );
    } else {
        // Empty slot
        hSrcRect = locForeground;
        pHUDContext->DrawBitmap( pHUDBitmap, &hRectEquipSlot, &hSrcRect );
    }

        // Chest
    hRectEquipSlot.fLeft += (Scalar)INTERFACE_HUD_ICON_SIZE;
    hRectEquipSlot.fRight += (Scalar)INTERFACE_HUD_ICON_SIZE;

    pItemInstance = pModel->GetItem( EQUIPSLOTID_CHEST );
    if ( pItemInstance->GetItem() != NULL ) {
        ItemID idItem = pItemInstance->GetID();
        const Rectangle2 & rectIconLocation = InterfaceFn->GetHUDLocation( idItem );
        
        hSrcRect = rectIconLocation;
        pHUDContext->DrawBitmap( pHUDItemsBitmap, &hRectEquipSlot, &hSrcRect );
    } else {
        // Empty slot
        hSrcRect = locForeground;
        pHUDContext->DrawBitmap( pHUDBitmap, &hRectEquipSlot, &hSrcRect );
    }

        // Arms
    hRectEquipSlot.fLeft += (Scalar)INTERFACE_HUD_ICON_SIZE;
    hRectEquipSlot.fRight += (Scalar)INTERFACE_HUD_ICON_SIZE;

    pItemInstance = pModel->GetItem( EQUIPSLOTID_ARMS );
    if ( pItemInstance->GetItem() != NULL ) {
        ItemID idItem = pItemInstance->GetID();
        const Rectangle2 & rectIconLocation = InterfaceFn->GetHUDLocation( idItem );

        hSrcRect = rectIconLocation;
        pHUDContext->DrawBitmap( pHUDItemsBitmap, &hRectEquipSlot, &hSrcRect );
    } else {
        // Empty slot
        hSrcRect = locForeground;
        pHUDContext->DrawBitmap( pHUDBitmap, &hRectEquipSlot, &hSrcRect );
    }

        // Wrists
    hRectEquipSlot.fLeft -= (Scalar)(INTERFACE_HUD_ICON_SIZE << 1);
    hRectEquipSlot.fRight -= (Scalar)(INTERFACE_HUD_ICON_SIZE << 1);
    hRectEquipSlot.fTop += (Scalar)INTERFACE_HUD_ICON_SIZE;
    hRectEquipSlot.fBottom += (Scalar)INTERFACE_HUD_ICON_SIZE;

    pItemInstance = pModel->GetItem( EQUIPSLOTID_WRISTS );
    if ( pItemInstance->GetItem() != NULL ) {
        ItemID idItem = pItemInstance->GetID();
        const Rectangle2 & rectIconLocation = InterfaceFn->GetHUDLocation( idItem );

        hSrcRect = rectIconLocation;
        pHUDContext->DrawBitmap( pHUDItemsBitmap, &hRectEquipSlot, &hSrcRect );
    } else {
        // Empty slot
        hSrcRect = locForeground;
        pHUDContext->DrawBitmap( pHUDBitmap, &hRectEquipSlot, &hSrcRect );
    }

        // Belt
    hRectEquipSlot.fLeft += (Scalar)INTERFACE_HUD_ICON_SIZE;
    hRectEquipSlot.fRight += (Scalar)INTERFACE_HUD_ICON_SIZE;

    pItemInstance = pModel->GetItem( EQUIPSLOTID_BELT );
    if ( pItemInstance->GetItem() != NULL ) {
        ItemID idItem = pItemInstance->GetID();
        const Rectangle2 & rectIconLocation = InterfaceFn->GetHUDLocation( idItem );

        hSrcRect = rectIconLocation;
        pHUDContext->DrawBitmap( pHUDItemsBitmap, &hRectEquipSlot, &hSrcRect );
    } else {
        // Empty slot
        hSrcRect = locForeground;
        pHUDContext->DrawBitmap( pHUDBitmap, &hRectEquipSlot, &hSrcRect );
    }

        // Hands
    hRectEquipSlot.fLeft += (Scalar)INTERFACE_HUD_ICON_SIZE;
    hRectEquipSlot.fRight += (Scalar)INTERFACE_HUD_ICON_SIZE;

    pItemInstance = pModel->GetItem( EQUIPSLOTID_HANDS );
    if ( pItemInstance->GetItem() != NULL ) {
        ItemID idItem = pItemInstance->GetID();
        const Rectangle2 & rectIconLocation = InterfaceFn->GetHUDLocation( idItem );

        hSrcRect = rectIconLocation;
        pHUDContext->DrawBitmap( pHUDItemsBitmap, &hRectEquipSlot, &hSrcRect );
    } else {
        // Empty slot
        hSrcRect = locForeground;
        pHUDContext->DrawBitmap( pHUDBitmap, &hRectEquipSlot, &hSrcRect );
    }

        // Ring 1
    hRectEquipSlot.fLeft -= (Scalar)(INTERFACE_HUD_ICON_SIZE << 1);
    hRectEquipSlot.fRight -= (Scalar)(INTERFACE_HUD_ICON_SIZE << 1);
    hRectEquipSlot.fTop += (Scalar)INTERFACE_HUD_ICON_SIZE;
    hRectEquipSlot.fBottom += (Scalar)INTERFACE_HUD_ICON_SIZE;

    pItemInstance = pModel->GetItem( EQUIPSLOTID_RING_1 );
    if ( pItemInstance->GetItem() != NULL ) {
        ItemID idItem = pItemInstance->GetID();
        const Rectangle2 & rectIconLocation = InterfaceFn->GetHUDLocation( idItem );

        hSrcRect = rectIconLocation;
        pHUDContext->DrawBitmap( pHUDItemsBitmap, &hRectEquipSlot, &hSrcRect );
    } else {
        // Empty slot
        hSrcRect = locForeground;
        pHUDContext->DrawBitmap( pHUDBitmap, &hRectEquipSlot, &hSrcRect );
    }

        // Legs
    hRectEquipSlot.fLeft += (Scalar)INTERFACE_HUD_ICON_SIZE;
    hRectEquipSlot.fRight += (Scalar)INTERFACE_HUD_ICON_SIZE;

    pItemInstance = pModel->GetItem( EQUIPSLOTID_LEGS );
    if ( pItemInstance->GetItem() != NULL ) {
        ItemID idItem = pItemInstance->GetID();
        const Rectangle2 & rectIconLocation = InterfaceFn->GetHUDLocation( idItem );

        hSrcRect = rectIconLocation;
        pHUDContext->DrawBitmap( pHUDItemsBitmap, &hRectEquipSlot, &hSrcRect );
    } else {
        // Empty slot
        hSrcRect = locForeground;
        pHUDContext->DrawBitmap( pHUDBitmap, &hRectEquipSlot, &hSrcRect );
    }

        // Trinket 1
    hRectEquipSlot.fLeft += (Scalar)INTERFACE_HUD_ICON_SIZE;
    hRectEquipSlot.fRight += (Scalar)INTERFACE_HUD_ICON_SIZE;

    pItemInstance = pModel->GetItem( EQUIPSLOTID_TRINKET_1 );
    if ( pItemInstance->GetItem() != NULL ) {
        ItemID idItem = pItemInstance->GetID();
        const Rectangle2 & rectIconLocation = InterfaceFn->GetHUDLocation( idItem );

        hSrcRect = rectIconLocation;
        pHUDContext->DrawBitmap( pHUDItemsBitmap, &hRectEquipSlot, &hSrcRect );
    } else {
        // Empty slot
        hSrcRect = locForeground;
        pHUDContext->DrawBitmap( pHUDBitmap, &hRectEquipSlot, &hSrcRect );
    }

        // Ring 2
    hRectEquipSlot.fLeft -= (Scalar)(INTERFACE_HUD_ICON_SIZE << 1);
    hRectEquipSlot.fRight -= (Scalar)(INTERFACE_HUD_ICON_SIZE << 1);
    hRectEquipSlot.fTop += (Scalar)INTERFACE_HUD_ICON_SIZE;
    hRectEquipSlot.fBottom += (Scalar)INTERFACE_HUD_ICON_SIZE;

    pItemInstance = pModel->GetItem( EQUIPSLOTID_RING_2 );
    if ( pItemInstance->GetItem() != NULL ) {
        ItemID idItem = pItemInstance->GetID();
        const Rectangle2 & rectIconLocation = InterfaceFn->GetHUDLocation( idItem );

        hSrcRect = rectIconLocation;
        pHUDContext->DrawBitmap( pHUDItemsBitmap, &hRectEquipSlot, &hSrcRect );
    } else {
        // Empty slot
        hSrcRect = locForeground;
        pHUDContext->DrawBitmap( pHUDBitmap, &hRectEquipSlot, &hSrcRect );
    }

        // Feet
    hRectEquipSlot.fLeft += (Scalar)INTERFACE_HUD_ICON_SIZE;
    hRectEquipSlot.fRight += (Scalar)INTERFACE_HUD_ICON_SIZE;

    pItemInstance = pModel->GetItem( EQUIPSLOTID_FEET );
    if ( pItemInstance->GetItem() != NULL ) {
        ItemID idItem = pItemInstance->GetID();
        const Rectangle2 & rectIconLocation = InterfaceFn->GetHUDLocation( idItem );

        hSrcRect = rectIconLocation;
        pHUDContext->DrawBitmap( pHUDItemsBitmap, &hRectEquipSlot, &hSrcRect );
    } else {
        // Empty slot
        hSrcRect = locForeground;
        pHUDContext->DrawBitmap( pHUDBitmap, &hRectEquipSlot, &hSrcRect );
    }

        // Trinket 2
    hRectEquipSlot.fLeft += (Scalar)INTERFACE_HUD_ICON_SIZE;
    hRectEquipSlot.fRight += (Scalar)INTERFACE_HUD_ICON_SIZE;

    pItemInstance = pModel->GetItem( EQUIPSLOTID_TRINKET_2 );
    if ( pItemInstance->GetItem() != NULL ) {
        ItemID idItem = pItemInstance->GetID();
        const Rectangle2 & rectIconLocation = InterfaceFn->GetHUDLocation( idItem );

        hSrcRect = rectIconLocation;
        pHUDContext->DrawBitmap( pHUDItemsBitmap, &hRectEquipSlot, &hSrcRect );
    } else {
        // Empty slot
        hSrcRect = locForeground;
        pHUDContext->DrawBitmap( pHUDBitmap, &hRectEquipSlot, &hSrcRect );
    }

        // RH Weapon
    hRectEquipSlot.fLeft -= (Scalar)(INTERFACE_HUD_ICON_SIZE << 1);
    hRectEquipSlot.fRight -= (Scalar)(INTERFACE_HUD_ICON_SIZE << 1);
    hRectEquipSlot.fTop += (Scalar)INTERFACE_HUD_ICON_SIZE;
    hRectEquipSlot.fBottom += (Scalar)INTERFACE_HUD_ICON_SIZE;

    pItemInstance = pModel->GetItem( EQUIPSLOTID_WEAPON_RIGHT );
    if ( pItemInstance->GetItem() != NULL ) {
        ItemID idItem = pItemInstance->GetID();
        const Rectangle2 & rectIconLocation = InterfaceFn->GetHUDLocation( idItem );

        hSrcRect = rectIconLocation;
        pHUDContext->DrawBitmap( pHUDItemsBitmap, &hRectEquipSlot, &hSrcRect );
    } else {
        // Empty slot
        hSrcRect = locForeground;
        pHUDContext->DrawBitmap( pHUDBitmap, &hRectEquipSlot, &hSrcRect );
    }

        // LH Weapon
    hRectEquipSlot.fLeft += (Scalar)INTERFACE_HUD_ICON_SIZE;
    hRectEquipSlot.fRight += (Scalar)INTERFACE_HUD_ICON_SIZE;

    pItemInstance = pModel->GetItem( EQUIPSLOTID_WEAPON_LEFT );
    if ( pItemInstance->GetItem() != NULL ) {
        ItemID idItem = pItemInstance->GetID();
        const Rectangle2 & rectIconLocation = InterfaceFn->GetHUDLocation( idItem );

        hSrcRect = rectIconLocation;
        pHUDContext->DrawBitmap( pHUDItemsBitmap, &hRectEquipSlot, &hSrcRect );
    } else {
        // Empty slot
        hSrcRect = locForeground;
        pHUDContext->DrawBitmap( pHUDBitmap, &hRectEquipSlot, &hSrcRect );
    }

        // Throw Weapon
    hRectEquipSlot.fLeft += (Scalar)INTERFACE_HUD_ICON_SIZE;
    hRectEquipSlot.fRight += (Scalar)INTERFACE_HUD_ICON_SIZE;

    pItemInstance = pModel->GetItem( EQUIPSLOTID_WEAPON_THROW );
    if ( pItemInstance->GetItem() != NULL ) {
        ItemID idItem = pItemInstance->GetID();
        const Rectangle2 & rectIconLocation = InterfaceFn->GetHUDLocation( idItem );

        hSrcRect = rectIconLocation;
        pHUDContext->DrawBitmap( pHUDItemsBitmap, &hRectEquipSlot, &hSrcRect );
    } else {
        // Empty slot
        hSrcRect = locForeground;
        pHUDContext->DrawBitmap( pHUDBitmap, &hRectEquipSlot, &hSrcRect );
    }

    // Bag Slots
    GPU2DRectangle hRectBagContentColumn;
    hRectBagContentColumn.fLeft = hRectEquipmentColumn.fRight + 4;
    hRectBagContentColumn.fTop = hRectEquipmentColumn.fTop;
    hRectBagContentColumn.fRight = hRectBagContentColumn.fLeft + (Scalar)iMiddleColumnWidth;
    hRectBagContentColumn.fBottom = hRectEquipmentColumn.fBottom;

    GPU2DRectangle hRectFirstSlot = hRectBagContentColumn;
    hRectFirstSlot.fRight = hRectFirstSlot.fLeft + (Scalar)INTERFACE_HUD_ICON_SIZE;
    hRectFirstSlot.fBottom = hRectFirstSlot.fTop + (Scalar)INTERFACE_HUD_ICON_SIZE;

    GPU2DRectangle hRectCurrentSlot = hRectFirstSlot;

    for( UInt i = 0; i < INVENTORY_BAG_SIZE; ++i ) {
        ItemInstance * pItemInstance = pModel->GetItem( i );

        if ( pItemInstance->GetItem() != NULL ) {
            ItemID idItem = pItemInstance->GetID();
            const Rectangle2 & rectIconLocation = InterfaceFn->GetHUDLocation( idItem );

            hSrcRect = rectIconLocation;
            pHUDContext->DrawBitmap( pHUDItemsBitmap, &hRectCurrentSlot, &hSrcRect );
        } else {
            // Empty slot
            hSrcRect = locForeground;
            pHUDContext->DrawBitmap( pHUDBitmap, &hRectCurrentSlot, &hSrcRect );
        }

        if ( (i & 7) == 7 ) { // (i%8) == (8-1)
            hRectFirstSlot.fTop += (Scalar)INTERFACE_HUD_ICON_SIZE;
            hRectFirstSlot.fBottom += (Scalar)INTERFACE_HUD_ICON_SIZE;
            hRectCurrentSlot = hRectFirstSlot;
        } else {
            hRectCurrentSlot.fLeft += (Scalar)INTERFACE_HUD_ICON_SIZE;
            hRectCurrentSlot.fRight += (Scalar)INTERFACE_HUD_ICON_SIZE;
        }
    }

    // Bag Selectors
    GPU2DRectangle hRectBagSelectorColumn;
    hRectBagSelectorColumn.fLeft = hRectBagContentColumn.fRight;
    hRectBagSelectorColumn.fTop = hRectBagContentColumn.fTop;
    hRectBagSelectorColumn.fRight = hRectBagSelectorColumn.fLeft + (Scalar)iRightColumnWidth;
    hRectBagSelectorColumn.fBottom = hRectBagContentColumn.fBottom;

    GPU2DRectangle hRectButton = hRectBagSelectorColumn;
    hRectButton.fBottom = hRectButton.fTop + (Scalar)iButtonHeight;

    for( UInt i = 0; i < INVENTORYBAGID_COUNT; ++i ) {
        hSrcRect = ( i == (UInt)idBag ) ? locActiveBagButton : locBagButton[i];
        pHUDContext->DrawBitmap( pHUDBitmap, &hRectButton, &hSrcRect );

        hRectButton.fTop += (Scalar)iButtonHeight;
        hRectButton.fBottom += (Scalar)iButtonHeight;
    }
}



