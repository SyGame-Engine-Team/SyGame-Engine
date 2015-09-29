/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/WorldManager.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Core-Class for manipulating the WorldGraph system
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// WorldManager implementation
inline const GChar * WorldManager::GetName() {
    return TEXT("WorldManager");
}
inline const GChar * WorldManager::GetMemoryBudget() {
    return TEXT (
"Heap:Scratch(4194304);Heap:Nodes(4194304);Heap:VisibleGroups(4194304);\
Heap:Indoor(4194304);Heap:Outdoor(4194304);Heap:PathData(67108864);"
    );
}

inline WorldNode * WorldManager::GetWorldRoot() const {
    return m_pWorldRoot;
}

inline Void WorldManager::AddChild( WorldBaseNode * pNode ) {
    Bool bAdded = m_pWorldRoot->AddChild( pNode );
    Assert( bAdded );
}
inline WorldBaseNode * WorldManager::RemoveChild( const GChar * strName ) {
    WorldBaseNode * pChild = m_pWorldRoot->RemoveChild( strName );
    Assert( pChild != NULL );
    return pChild;
}

inline Void WorldManager::ResetWorld() {
    m_pWorldRoot->RemoveAllChildren();
}

inline const VisibleGroup &  WorldManager::GetVisibleGroup() const {
    return m_pWorldCuller->GetVisibleGroup();
}

inline WorldCamera * WorldManager::GetWorldCamera() const {
    return m_pWorldCamera;
}
inline Void WorldManager::SetWorldCamera( WorldCamera * pCamera ) {
    m_pWorldCamera = pCamera;
    if ( m_pWorldCamera != NULL ) {
        m_pWorldCuller->SetCamera( m_pWorldCamera->GetCamera(), true );
        RenderingFn->EnableCamera( m_pWorldCamera->GetCamera() );
    } else
        RenderingFn->EnableDefaultCamera();
}

inline Void WorldManager::ToggleAnimation() {
    m_bRunAnimation = !m_bRunAnimation;
}
inline Void WorldManager::PauseAnimation() {
    m_bRunAnimation = false;
}
inline Void WorldManager::ResumeAnimation() {
    m_bRunAnimation = true;
}
inline Void WorldManager::StepAnimation() {
    m_bRunAnimation = false;
    m_bStepAnimation = true;
}

inline Void WorldManager::RayCast( const Line3 & vRay, Scalar fMinDistance, Scalar fMaxDistance ) {
    m_pWorldCaster->RayCast( m_pWorldRoot, vRay, fMinDistance, fMaxDistance );
}

inline UInt WorldManager::GetRayCastResultCount() const {
    return m_pWorldCaster->GetRayCastResultCount();
}
inline const RayCastResult * WorldManager::GetRayCastResult( UInt iIndex ) const {
    return m_pWorldCaster->GetRayCastResult( iIndex );
}

inline const RayCastResult * WorldManager::GetClosestRayCastResult() {
    return m_pWorldCaster->GetClosestRayCastResult();
}
inline const RayCastResult * WorldManager::GetClosestPositiveRayCastResult() {
    return m_pWorldCaster->GetClosestPositiveRayCastResult();
}
inline const RayCastResult * WorldManager::GetClosestNegativeRayCastResult() {
    return m_pWorldCaster->GetClosestNegativeRayCastResult();
}
inline Void WorldManager::SortRayCastResults() {
    m_pWorldCaster->SortRayCastResults();
}

inline Void WorldManager::SphereCast( const Sphere & vSphere ) {
    m_pWorldCaster->SphereCast( m_pWorldRoot, vSphere );
}

inline UInt WorldManager::GetSphereCastResultCount() const {
    return m_pWorldCaster->GetSphereCastResultCount();
}
inline const SphereCastResult * WorldManager::GetSphereCastResult( UInt iIndex ) const {
    return m_pWorldCaster->GetSphereCastResult( iIndex );
}

inline const SphereCastResult * WorldManager::GetClosestSphereCastResult() {
    return m_pWorldCaster->GetClosestSphereCastResult();
}
inline Void WorldManager::SortSphereCastResults() {
    m_pWorldCaster->SortSphereCastResults();
}

inline WorldPathFinder * WorldManager::GetPathFinder() const {
    return m_pWorldPathFinder;
}

inline GlobalEffect * WorldManager::GetGlobalEffect() const {
    return m_pGlobalEffect;
}
inline Void WorldManager::SetGlobalEffect( GlobalEffect * pEffect ) {
    m_pGlobalEffect = pEffect;
}

inline Void WorldManager::ToggleDebugDraw( Bool bEnable ) {
    m_bEnableDebugDraw = bEnable;
}
