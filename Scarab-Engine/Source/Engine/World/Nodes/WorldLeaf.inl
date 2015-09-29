/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/Nodes/WorldLeaf.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Node : Leaves
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// WorldLeaf implementation
inline WorldNodeType WorldLeaf::GetType() const {
    return WORLDNODE_LEAF;
}

inline Mesh * WorldLeaf::GetMesh() const {
    return m_pMesh;
}
inline Void WorldLeaf::SetMesh( Mesh * pMesh ) {
    m_pMesh = pMesh;
}

inline EffectInstance * WorldLeaf::GetEffectInstance() const {
    return m_pEffectInstance;
}
inline Void WorldLeaf::SetEffectInstance( EffectInstance * pEffectInstance ) {
    m_pEffectInstance = pEffectInstance;
}

inline BodyNode * WorldLeaf::GetBody() const {
    return m_pBody;
}
inline Void WorldLeaf::SetBody( BodyNode * pBody ) {
    m_pBody = pBody;
    if ( m_pBody != NULL ) {
        m_pBody->SetUserData( this );
        WorldTransform = m_pBody->GetTransform();
    }
}

