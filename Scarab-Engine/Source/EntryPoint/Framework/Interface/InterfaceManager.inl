/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/Framework/Interface/InterfaceManager.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Framework, Interface : InterfaceManager
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// HUDBackboardModel implementation

/////////////////////////////////////////////////////////////////////////////////
// InterfaceManager implementation
inline const GChar * InterfaceManager::GetName() {
    return TEXT("InterfaceManager");
}
inline const GChar * InterfaceManager::GetMemoryBudget() {
    return TEXT (
"Heap:Scratch(4194304);"
    );
}

inline GPU2DContext * InterfaceManager::GetHUDContext() const {
    return GUIFn->GetScreenContext();
}

inline GPU2DBitmap * InterfaceManager::GetHUDSkinBitmap() const {
    return m_pSkinBitmap;
}
inline GPU2DBitmap * InterfaceManager::GetHUDStatusEffectBitmap() const {
    return m_pStatusEffectsBitmap;
}
inline GPU2DBitmap * InterfaceManager::GetHUDSkillsBitmap() const {
    return m_pSkillsBitmap;
}
inline GPU2DBitmap * InterfaceManager::GetHUDItemsBitmap() const {
    return m_pItemsBitmap;
}

inline const Rectangle2 & InterfaceManager::GetHUDLocation( HUDImage iHUDImage ) const {
    Assert( iHUDImage < HUD_IMAGE_COUNT );
    return m_arrHUDLocations[iHUDImage];
}
//inline const Rectangle2 & InterfaceManager::GetHUDLocation( StatusEffectID idStatusEffect ) const {
//    Assert( idStatusEffect < STATUSEFFECTID_COUNT );
//    return m_arrStatusEffectIcons[idStatusEffect];
//}
//inline const Rectangle2 & InterfaceManager::GetHUDLocation( SkillID idSkill ) const {
//    Assert( idSkill < SKILLID_COUNT );
//    return m_arrSkillIcons[idSkill];
//}
//inline const Rectangle2 & InterfaceManager::GetHUDLocation( ItemID idItem ) const {
//    Assert( idItem < ITEMID_COUNT );
//    return m_arrItemIcons[idItem];
//}

inline GPU2DSolidColorBrush * InterfaceManager::GetHUDColorBrush() const {
    return &m_hColorBrush;
}
inline const Color4 & InterfaceManager::GetHUDColor( HUDColor iHUDColor ) const {
    Assert( iHUDColor < HUD_COLOR_COUNT );
    return m_arrHUDColors[iHUDColor];
}

inline GPU2DSolidColorBrush * InterfaceManager::GetHUDTextBrush() const {
    return &m_hTextBrush;
}
inline GPU2DTextFormat * InterfaceManager::GetHUDTextFormat( HUDFont iHUDFont ) const {
    Assert( iHUDFont < HUD_FONT_COUNT );
    return m_arrTextFormats[iHUDFont];
}

//inline Void InterfaceManager::ToggleCharacterView() {
//    m_pCharacterView->SetVisible( !(m_pCharacterView->IsVisible()) );
//}
//inline Void InterfaceManager::ToggleSkillBookView() {
//    m_pSkillBookView->SetVisible( !(m_pSkillBookView->IsVisible()) );
//}
//inline Void InterfaceManager::UpdateSkillBookView() {
//    if ( m_pSkillBookViewModel != NULL )
//        m_pSkillBookViewModel->UpdateSkillPages();
//}
//inline Void InterfaceManager::ToggleInventoryView() {
//    m_pInventoryView->SetVisible( !(m_pInventoryView->IsVisible()) );
//}

inline HUDDragNDropData * InterfaceManager::GetDragNDropData() {
    return &m_hDragNDrop;
}

